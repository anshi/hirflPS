/* devDigitalPSInt.c */
/* Digital DC Power Supply interrupt device support module with ramping protocol*/


#include <stdlib.h>
#include <epicsExport.h>
#include <dbAccess.h>
#include <devSup.h>
#include <recGbl.h>
#include <dbScan.h>
#include <dbDefs.h>
#include <ellLib.h>
#include <cantProceed.h>
#include <epicsThread.h>
#include <epicsMutex.h>
#include <initHooks.h>
#include "DigitalDCPSaoRecord.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <DigitalPSOperationCode.h>
#include <math.h>

#define MAX_BUFF_SIZE 64

static long init(int phase);
static long init_record();
static long get_ioint_info(int dir,dbCommon* prec,IOSCANPVT* io);
static long write_DigitalDC();
static int analyse_rec_buff(unsigned char *buf, void* raw);
static int analyse_ps_state(void *p, unsigned long state_num);
static long gen_send_buf(void *p, int is_adjust, unsigned char *buf, float current_val);

struct {
    long num;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_DigitalDC;
    DEVSUPFUN special_linconv;
} devDigitalPSInt = {
    6, /* space for 6 functions */
    NULL,
    init,
    init_record,
    get_ioint_info,
    write_DigitalDC,
    NULL
};
epicsExportAddress(dset,devDigitalPSInt);

static ELLLIST allprngs = ELLLIST_INIT;

struct prngState {
    ELLNODE node;
    double lastnum;     //readback current of PS
    double lastnum_v;   //readback voltage of PS
    char strstat[40]; //readback state of PS
    unsigned long laststat;
    double interval;  //the interval of scan , second
    char ip[20];
    char port[8];
    int ver;	//the protocol version of PS
    bool is_write;  //is or not write setpoint to PS
    char command[8]; // the command will send to PS
    float current_val; //current will send to PS
    epicsMutexId lock;
    IOSCANPVT scan;
    epicsThreadId generator;
    struct DigitalDCPSaoRecord *pao;
};


static void start_workers(initHookState state);
static void worker(void* raw);

static long init(int phase)
{
    if(phase==0)
        initHookRegister(&start_workers);
    return 0;
}


static long init_record(pao)
    struct DigitalDCPSaoRecord	*pao;
{
    struct prngState* priv;
    char temp_string[40] = {0,};
    char seps[]   = ":";
    char *pos = NULL;

    priv=callocMustSucceed(1,sizeof(*priv),"prngintr");
    
    //get the parament
    strcpy(temp_string, pao->out.value.instio.string);
    //printf("debug:inp string is %s\n", temp_string);
    
    if (strlen(temp_string) == 0)
    {
        /* code */
        pao->udf = TRUE;
        return -1;
    }

    pos = strtok( temp_string, seps ); //get the version of PS
    if (pos == NULL)
    {
        priv->ver = 4;
    }
    char *endptr;
    priv->ver = strtod(pos, &endptr);

    pos = strtok( NULL, seps); //get the scan interval time
    if (pos == NULL)
    {
        priv->interval = 0.5; //default 500mS
    }
    
    priv->interval = strtod(pos, &endptr);
    /*
    recGblInitConstantLink(&pao->inp,DBF_USHORT,&int_time);
    priv->interval = int_time;
    */


    scanIoInit(&priv->scan);
    priv->lock = epicsMutexMustCreate();
    priv->generator = NULL;
    
    ellAdd(&allprngs, &priv->node);

    //init the command string
    memset(pao->command, '\0', sizeof(pao->command));
    strcpy(pao->command, "AA");
    pao->linr = 0;
    pao->opi_conn = 1;
    // pao->state = 0;

    strcpy(priv->ip, pao->ip_addr);
    strcpy(priv->port, pao->port);
    strcpy(priv->command, "AA");
    // strcpy(priv->laststat, "OK");
    priv->laststat = 0;
    priv->is_write = FALSE;
    priv->current_val = 0;
    priv->pao = pao;
    pao->dpvt = priv;
    return 0;
}


static void start_workers(initHookState state)
{
    ELLNODE *cur;
    
    if(state!=initHookAfterInterruptAccept)
        return;
    
    for(cur=ellFirst(&allprngs); cur; cur=ellNext(cur)) 
    {
        struct prngState *priv = CONTAINER(cur, struct prngState, node);

        priv->generator = epicsThreadMustCreate("prngworker",
                            epicsThreadPriorityMedium,
                            epicsThreadGetStackSize(epicsThreadStackSmall),
                            &worker, priv);
    }
}

static void worker(void* raw)
{
    struct prngState* priv=raw;
    //char *endptr;
    unsigned char send_buff[MAX_BUFF_SIZE] = {0,};
    unsigned char receive_buff[MAX_BUFF_SIZE] = {0,};
    Operat_Code op_result = OP_SUCCESS; //success

    //memcpy(send_buff, QUERY_COMMAND, 14);
    short send_size = 0;
    int recvd = 0;
    //init socket
    int sockfd;
    struct sockaddr_in device_addr;
    device_addr.sin_family = AF_INET;
    device_addr.sin_port = htons(atoi(priv->port));
    device_addr.sin_addr.s_addr = inet_addr(priv->ip);
    memset(device_addr.sin_zero, '\0', sizeof(device_addr.sin_zero));

    //set the timeout of network function
    struct timeval timeout={2,0};//2s
    struct DigitalDCPSaoRecord* pao = priv->pao;

    //int reconnect_times = 0;
    int sleep_time = 5; //at first sleep 5 seconds
    while(1)
    {
        memset(receive_buff, '\0', MAX_BUFF_SIZE);
        recvd = 0;
        op_result = OP_SUCCESS; //success
        /* ------------ create socket -----------------*/
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket");
            epicsThreadSleep(5);
            continue;
        }

        
        //set the connet time out also
        if ( (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout))) == -1 )
        {
            perror("setsockopt");
            close(sockfd);
            epicsThreadSleep(1);
            continue;
        }
        if ( (setsockopt( sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout) )) == -1)
        {
            perror("setsockopt");
            close(sockfd);
            epicsThreadSleep(1);
            continue;
        }

        

        if (connect(sockfd, (struct sockaddr *)&device_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("connect");
            op_result = CONNECT_PS; //connect to PS error
            //if (priv->is_write)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            /*
            reconnect_times ++;
            if (reconnect_times == 5)
            {
                sleep_time ++;
                reconnect_times = 0;
            }
            */
            //sleep_time = 5;
            close(sockfd);
            printf("connect to %s[%s] failed, will retry in %d seconds\n", pao->name, priv->ip, sleep_time);
            epicsThreadSleep(sleep_time);
            continue;
        }

        op_result = OP_SUCCESS;
        //reconnect_times = 0;
        //sleep_time = 3;

        int is_adjust = 0;

        if (priv->is_write)
        {
            is_adjust = 1;
            priv->is_write = FALSE;
        }

        if ( !gen_send_buf(priv, is_adjust, send_buff, priv->current_val) )
        {
            close(sockfd);
            op_result = DATA_PS; //operator unsuccess
            if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            epicsThreadSleep(1);
            continue;
        }


        send_size = *(send_buff + 4);
        if ( send(sockfd, send_buff, send_size, 0) == -1 )
        {
            perror("send begin");
            close(sockfd);
            op_result = SEND_TO_PS; //operator unsuccess
            if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            epicsThreadSleep(3);
               continue;
        }

        recvd = recv(sockfd, receive_buff, MAX_BUFF_SIZE, 0);
        if(recvd == -1 && errno == EAGAIN)
        {
            printf("recv timeout\n");
            close(sockfd);
            op_result = RECV_RESP_PS; //operator unsuccess
            if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            epicsThreadSleep(3);
            continue;  //can't get the response of device
        }

        if (analyse_rec_buff(receive_buff, priv) != 1)
        {
            close(sockfd);
            printf("received buffer is error\n");
            op_result = RESP_PS; //operator unsuccess
            if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            epicsThreadSleep(priv->interval);
            continue;  //can't get the response of device
        }
            
        close(sockfd);

        if (is_adjust)
        {
            /* code */
            //strcpy(pao->lcomm, "ai");
            dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
        }
        scanIoRequest(priv->scan);
        epicsThreadSleep(priv->interval);
            
    }
    
}

static long get_ioint_info(int dir,dbCommon* prec,IOSCANPVT* io)
{
    struct prngState* priv=prec->dpvt;
    
    *io = priv->scan;
    return 0;
}

static long write_DigitalDC(pao)
    struct DigitalDCPSaoRecord	*pao;
{
    struct prngState* priv=pao->dpvt;

    if (!priv)
    {
        /* code */
        return (2);
    }

    epicsMutexMustLock(priv->lock);
    pao->val = priv->lastnum;
    pao->val_v = priv->lastnum_v;
    pao->state = priv->laststat;
    strcpy(pao->psstat, priv->strstat);
    // if (pao->current_val != priv->current_val)  //will write the new current value to PS
    if ( fabsf(pao->current_val - priv->current_val) > 0.0001)
    {
        priv->is_write = TRUE;
        priv->current_val = pao->current_val;
    }
    else
    {
        priv->is_write = FALSE;
        strcpy(priv->command, "AA");
        if (strcmp(pao->command, "AA") != 0 )
        {
            strcpy(priv->command, pao->command);
            strcpy(pao->command, "AA");
        }
    }
    epicsMutexUnlock(priv->lock);
    
    return (2);
}

static int analyse_rec_buff(unsigned char *buf, void* raw)
{
    if (!buf)
        return 0;

    if (!raw)
    {
        /* code */
        return 0;
    }

    struct prngState* priv=raw;

    unsigned char *p_buf;
    short command_len = 0;
    
    p_buf = buf;
    command_len = *(buf + 4);
    
    if ( (*p_buf != 0xff) || (*(p_buf + command_len - 1) != 0xff) )
    {
        printf("Debug:received buffer is error!\n");
        return 0;
    }
    
    
    char rtn_code = '\0';
    rtn_code = *(p_buf + 9);

    
    if ( (rtn_code == 0x00) && (*(p_buf + 10) == 0xaa)) //command is validation and is state information
    {
        //TODO: get the return value
        //current
        float f_curr = 0;
        //voltage
        float f_volt = 0;
        unsigned long l_state = 0;

        p_buf += 11; // state of PS
        memcpy(&l_state, p_buf, 4);
        analyse_ps_state(priv, l_state);

        p_buf = buf;
        p_buf += 15;    // current value
        memcpy(&f_curr, p_buf, 4);
        p_buf += 4;     // voltage
        memcpy(&f_volt, p_buf, 4);

        epicsMutexMustLock(priv->lock);
        priv->lastnum = f_curr;
        priv->lastnum_v = f_volt;
        epicsMutexUnlock(priv->lock);
    }
    

    return 1;

}

static int analyse_ps_state(void *p, unsigned long state_num)
{
    struct prngState *priv = p;
    char str_state[40] = {0,};
    unsigned long temp_state = 0;

    //printf("Debug:origin state_num is %u\n", state_num);
    if (state_num & 0x00000002) 	// PS ON/OFF?
    {
        temp_state |= 0x00000001;
    }

    if (!(state_num & 0x00000001)) 	// PS Fault/Normal
    {
        temp_state |= 0x00000002;
    }

    if (state_num & 0x00000010) //ps load error
    {
        temp_state |= 0x00000004;
    }

    if (state_num & 0x00000040) //ps water error
    {
        temp_state |= 0x00000008;
    }

    if ( state_num & 0x3fc00000 ) //ps IGBT error
    {
        temp_state |= 0x00000010;
    }

    if ( state_num & 0x40000000 ) //ps door lock error
    {
        temp_state |= 0x00000020;
    }
    
    if (state_num & 0x00008000 ) //DC or ramping mode?
    {
        temp_state |= 0x00000040;
    }
    
    (temp_state & 0x00000001)?strcpy(str_state,"OFF"):strcpy(str_state,"ON");			// PS running or stop?
    (temp_state & 0x00000002)?strcat(str_state,"|FAULT"):strcat(str_state,"|NORMAL");	// PS have waring?
    (temp_state & 0x00000040)?strcat(str_state,"|RAMPING"):strcat(str_state,"|DC");	// PS have waring?
    

    #if 0
    (state_num&0x00000001)?strcpy(temp_state,"OK\0"):strcpy(temp_state,"ERROR\0");//电源总故障
    (state_num&0x00000002)?strcat(temp_state,"|STOP"):strcat(temp_state,"|RUNNING");//是否运行
    (state_num&0x00000008)?strcat(temp_state,"|FUSE"):1;//熔断故障
    (state_num&0x00000010)?strcat(temp_state,"|LOAD"):1;//负载故障
    (state_num&0x00000020)?strcat(temp_state,"|POWER"):1;//功率器件故障
    (state_num&0x00000040)?strcat(temp_state,"|WATER"):1;//缺水故障
    (state_num&0x00000080)?strcat(temp_state,"|TEMP"):1;//过热故障
    // (state_num&0x01)?strcat(ps->psstate,"|OC"):1;//过流故障
    // (state_num&0x02)?strcat(ps->psstate,"|OV"):1;//过压故障
    // (state_num&0x04)?strcat(ps->psstate,"|INOV"):1;//交流过压故障
    // (state_num&0x08)?strcat(ps->psstate,"|INLV"):1;//交流欠压故障
    // (state_num&0x10)?strcat(ps->psstate,"|SW"):1;//开关故障
    // (state_num&0x40)?strcat(ps->psstate,"|EMEGRGY"):1;//急停状态
    (state_num&0x00008000)?strcat(temp_state,"|RAMP"):strcat(temp_state,"|DC");//脉冲，直流
    (state_num&0x40000000)?strcat(temp_state,"|D_LOCK"):1;//门开
    #endif

    epicsMutexMustLock(priv->lock);
    priv->laststat = temp_state;
    strcpy(priv->strstat, str_state);
    epicsMutexUnlock(priv->lock);

    // epicsMutexMustLock(priv->lock);
    // priv->laststat = temp_state;
    // epicsMutexUnlock(priv->lock);
    
    return 1;
}


//generate the send buffer
static long gen_send_buf(void *p, int is_adjust, unsigned char *buf, float current_val)
{
    if (!buf || !p)
        return 0;
    
    struct prngState *priv = p;
    unsigned char *p_buf;
        
    p_buf = buf;
    //header of the command
    *(p_buf++) = 0xff;
    *(p_buf++) = 0xff;
    *(p_buf++) = 0x00;
    *(p_buf++) = 0x00;

    //the length of the command, write the 0x00, after gen , write the real length
    *(p_buf++) = 0x00;
    *(p_buf++) = 0x00;
    //version
    if (priv->ver == 4)
    {
        /* code */
        *(p_buf++) = 0x04;
    }
    else
        *(p_buf++) = 0x03;
    
    //channel
    *(p_buf++) = 0x01;
    //CID1
    *(p_buf++) = 0x2a;
    if (is_adjust)  //adjust current of power
    {
        *(p_buf++) = 0xb3;
        memcpy(p_buf, &current_val, 4);
        *(buf + 4) = 0x12;  //length
        p_buf += 4;
    }
    else	//others command
    {
        sscanf(priv->command, "%02x", p_buf);
        *(buf + 4) = 0x0e; //length
        p_buf ++;
    }

    //the end of the command
    *(p_buf++) = 0x00;
    *(p_buf++) = 0x00;
    *(p_buf++) = 0xff;
    *(p_buf++) = 0xff;
    
    return 1;
}
