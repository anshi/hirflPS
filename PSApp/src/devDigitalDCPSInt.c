/* devDigitalDCPSInt.c */
/* Digital DC Power Supply interrupt device support module with DC protocol(V3)*/


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
static int gen_checksum(unsigned char *buf);

struct {
    long num;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_DigitalDC;
    DEVSUPFUN special_linconv;
} devDigitalDCPSInt = {
    6, /* space for 6 functions */
    NULL,
    init,
    init_record,
    get_ioint_info,
    write_DigitalDC,
    NULL
};
epicsExportAddress(dset,devDigitalDCPSInt);

static ELLLIST allprngs = ELLLIST_INIT;

struct prngState {
    ELLNODE node;
    double lastnum;  //readback current of PS
    double lastnum_v;  //readback voltage of PS
    char strstat[40]; //readback state of PS
    unsigned long laststat;	//readback state of PS
    double interval;  //the interval of scan , second
    char ip[20];
    char port[8];
    //int ver;	//the protocol version of PS
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
    

    priv=callocMustSucceed(1,sizeof(*priv),"prngintr");
    
    //get the parament
    strcpy(temp_string, pao->out.value.instio.string);
    //printf("debug:inp string is %s\n", temp_string);
    
    if (strlen(temp_string) == 0)
    {
        priv->interval = 1; //default 1 second
    }

    char *endptr;
    priv->interval = strtod(temp_string, &endptr);
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
    struct timeval timeout={6,0};//6s
    struct DigitalDCPSaoRecord* pao = priv->pao;

    //int reconnect_times = 0;
    int sleep_time = 3; //at first reconnect delay is  3 seconds
    int connect_stat = 18; //don't have connect
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

            /*
            op_result = 8; //operator unsuccess
            if (priv->is_write)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
            */
            op_result = CONNECT_PS; //connect to PS error
            //if (priv->is_write)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV

            close(sockfd);
            printf("connect to %s[%s] failed, will retry in %d seconds\n", pao->name, priv->ip, sleep_time);
            epicsThreadSleep(sleep_time);
            continue;
        }
#if 0
        if (connect_stat == 18)
        {
            connect_stat = 19; //success established a connect
            dbPutLink(&pao->out1, DBR_USHORT, &connect_stat, 1); //send a connect state to operator PV
        }
#endif
        op_result = OP_SUCCESS;
        //reconnect_times = 0;
        //sleep_time = 3;

        int is_adjust = 0;

        epicsMutexMustLock(priv->lock);
        if (priv->is_write)
        {
            is_adjust = 1;
            priv->is_write = FALSE;
        }
        epicsMutexUnlock(priv->lock);
        
        memset(send_buff, '\0', sizeof(send_buff));
        if ( !gen_send_buf(priv, is_adjust, send_buff, priv->current_val) )
        {
            close(sockfd);
            op_result = DATA_PS; //operator unsuccess
            if (is_adjust)
                dbPutLink(&pao->out1, DBR_USHORT, &op_result, 1); //send a signal to operator PV
            epicsThreadSleep(1);
            continue;
        }


        send_size = strlen(send_buff);
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

        op_result = OP_SUCCESS; //send successful
        if (is_adjust)
            dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV

        //epicsThreadSleep(0.1);
        if (!is_adjust)
        {
            recvd = recv(sockfd, receive_buff, MAX_BUFF_SIZE, 0);
            if(recvd == -1 && errno == EAGAIN)
            {	
                printf("%s recv timeout\n", pao->name);
                close(sockfd);
                op_result = RECV_RESP_PS; //operator unsuccess
                //if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
                epicsThreadSleep(3);
                continue;  //can't get the response of device
            }
            

            if (analyse_rec_buff(receive_buff, priv) != 1)
            {
                close(sockfd);
                // printf("%s received buffer is error\n", pao->name);
                op_result = RESP_PS; //operator unsuccess
                if (is_adjust)
                dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
                epicsThreadSleep(priv->interval);
                continue;  //can't get the response of device
            }
        }
        
        if (is_adjust)
        {
            /* code */
            // Debug
            recvd = recv(sockfd, receive_buff, MAX_BUFF_SIZE, 0);
            if (recvd > 0)
            {
                printf("Debug: %s received %d bytes\n", pao->name, recvd);
                printf("Debug: %s received buffer is: %s\n", pao->name, receive_buff);
                // for (int i = 0; i < recvd; ++i)
                // {
                //     printf("%02x ", receive_buff[i]);
                // }
                printf("\n");
            }
            //strcpy(pao->lcomm, "ai");
            dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
        }
        
        close(sockfd);
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
    strcpy(pao->psstat, priv->strstat);
    pao->state = priv->laststat;
    
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
    struct DigitalDCPSaoRecord* pao = priv->pao;

    unsigned char *p_buf;
    p_buf = buf;
    if ( (*p_buf != '~') || strlen(buf) < 14)
    {
        printf("Debug:%s received buffer is error!buf = %s\n", pao->name, buf);
        #if 0
         FILE *infile;
                infile = fopen("/usr/local/debug/buffer", "w+");
                if (infile == NULL)
                        return 1;
            fwrite(buf, sizeof(unsigned char), strlen(buf), infile);
            fclose(infile);
        #endif
        return 0;
    }

    unsigned char temp_comm[64];

    memset(temp_comm, '\0', sizeof(temp_comm));
    //we don't checksum anymore
    #if 0 
    strncpy(temp_comm, buf, strlen(buf) - 5); //checksum len is 4 , end of command len is 1, so we sub the 5 bytes

    if (!gen_checksum(temp_comm))
        return 0;

    strcat(temp_comm, "\r");

    if (strcmp(temp_comm, buf) != 0)
    {
        //printf("Debug:check sum error!\n");
        //return 0;  //ssc-linac ECR X and Y PS check sum error,but receive data is correct
    }
    #endif
    
    char rtn_code = '\0';
    rtn_code = *(p_buf + 8);

    if (rtn_code != '0')
    {
        return 0; //there are some errors
    }
    /*
    switch (rtn_code)
    {
        case '0':
            strcpy(pao->command, "success");
            break;
        case '1':
            strcpy(pao->command, "ver error");
            break;
        case '2':
            strcpy(pao->command, "check err");
            break;
        case '3':
            strcpy(pao->command, "len error");
            break;
        case '4':
            strcpy(pao->command, "CID2 err");
            break;
        case '5':
            strcpy(pao->command, "comm err");
            break;
        case '6':
            strcpy(pao->command, "invalid");
            break;
        default:
            strcpy(pao->command, "other err");
            break;				
    }
    */

    memset(temp_comm, '\0', sizeof(temp_comm));
    strncpy(temp_comm, p_buf + 11, 2);
    //printf("Debug:temp_comm is %s\n", temp_comm);
    if ( (strcmp(temp_comm, "1A") == 0)) //command is validation and is state information
    {
        if ( strlen(buf) < 39)
        {
            return 0; //not a validation readback
        }

        //get the current readback
        memset(temp_comm, '\0', sizeof(temp_comm));
        strncpy(temp_comm, p_buf + 23, 8);
        char str_temp[10] = {0,};
        strcpy(str_temp, temp_comm);
        int i;

        for (i = 0; i < 8; i++)
        {
            /* code */
            temp_comm[i] = str_temp[6-i];
            temp_comm[i + 1] = str_temp[7-i];
            i++;
        }
        //printf("debug: after reverse is %s\n", temp_comm);
        float f_curr = 0;
        sscanf(temp_comm, "%08x", &f_curr);

        //get the voltage readback
        memset(temp_comm, '\0', sizeof(temp_comm));
        memset(str_temp, '\0', sizeof(str_temp)); 
        
        strncpy(temp_comm, p_buf + 31, 8);
        strcpy(str_temp, temp_comm);
        
        for (i = 0; i < 8; i++)
        {
            /* code */
            temp_comm[i] = str_temp[6-i];
            temp_comm[i + 1] = str_temp[7-i];
            i++;
        }
        //printf("debug: after reverse is %s\n", temp_comm);
        float f_volt = 0;
        sscanf(temp_comm, "%08x", &f_volt);

        epicsMutexMustLock(priv->lock);
        priv->lastnum = f_curr;
        priv->lastnum_v = f_volt;
        epicsMutexUnlock(priv->lock);

        unsigned int int_state = 0;
        sscanf(p_buf + 17, "%04x", &int_state);
        analyse_ps_state(priv, int_state);
    }

    return 1;
}

static int analyse_ps_state(void *p, unsigned long state_num)
{
    struct prngState *priv = p;
    char str_state[40] = {0,};
    unsigned long temp_state;

    //printf("Debug:origin state_num is %u\n", state_num);
    
    if (!(state_num & 0x00004000))	// PS ON/OFF ?
    {
        temp_state |= 0x00000001;
    }

    if (state_num & 0x00008000) // PS Fault/Normal ?
    {
        temp_state |= 0x00000002;
    }

    if (state_num & 0x00000800) //ps load error
    {
        temp_state |= 0x00000004;
    }

    if (state_num & 0x00000200) //ps water error
    {
        temp_state |= 0x00000008;
    }

    if ( state_num & 0x00000400 ) //ps IGBT error
    {
        temp_state |= 0x00000010;
    }

    if ( state_num & 0x00000040 ) //ps door lock error
    {
        temp_state |= 0x00000020;
    }
    (temp_state & 0x00000001)?strcpy(str_state,"OFF"):strcpy(str_state,"ON");			// PS running or stop?
    (temp_state & 0x00000002)?strcat(str_state,"|FAULT"):strcat(str_state,"|NORMAL");	// PS have waring?
    strcat(str_state, "|DC");
    epicsMutexMustLock(priv->lock);
    strcpy(priv->strstat, str_state);
    priv->laststat = temp_state;
    epicsMutexUnlock(priv->lock);
    
    return 1;
}

//generate the send buffer
static long gen_send_buf(void *p, int is_adjust, unsigned char *buf, float current_val)
{
    if (!buf || !p)
        return 0;
    
    struct prngState *priv = p;
    unsigned char temp_com[64];
    unsigned char *p_current_val;
    unsigned char temp_current_val[16];
    unsigned char temp_buff[64];
    
    memset(temp_com, '\0', sizeof temp_com);
    memset(temp_current_val, '\0', sizeof temp_current_val);
    memset(temp_buff, '\0', sizeof temp_buff);
    
    if (is_adjust)  //adjust current of power
    {		
        p_current_val = (unsigned char *)&(current_val);
        
        sprintf(temp_current_val, "%02x%02x%02x%02x", p_current_val[0],p_current_val[1],p_current_val[2],p_current_val[3]);
        //printf("Debug: temp_current_val = %s\n", temp_current_val);
        sprintf(temp_com, "B30008%s", temp_current_val);
    }
    else	//others command
    {
        sprintf(temp_com, "%s0000", priv->command);
    }
    sprintf(temp_buff, "~03%s2A%s", "01", temp_com); //write the command head to buffer
    if (!gen_checksum(temp_buff))
        return 0;

    strcat(temp_buff, "\r");
    memcpy(buf, temp_buff, sizeof(temp_buff));
    return 1;
}

int gen_checksum(unsigned char *buf)
{
    int int_checksum = 0;
    int buf_len = 0;
    unsigned char buf_p[10] = {0,};

    buf_len = strlen(buf);

    if (buf_len == 0)
    {
        return 0;
    }

    int i = 0;
    for(i = 1; i < buf_len; i++)
    {
        int_checksum += *(buf + i);
    }

    int_checksum %= 65536;
    int_checksum = ~int_checksum + 1;
    int_checksum &= 0x0000ffff;
    //printf("Debug: checksum is %04x\n", int_checksum);
    sprintf(buf_p, "%04x", int_checksum);
    strcat(buf, buf_p);
    return 1;
    
}
