/* devDigitalPSRampInt.c */
/* Digital Ramping mode Power Supply interrupt device support module*/
/* ********************
 * 
 *	Date: 2020.7.1
 * 	Author:		An Shi
 *	Modification:
 * 		1. Re-write as long-connection mode.
 *      2. Use message queue to commuication with three threads.
 * 
 *  Date: 2021.05.07
 *  Author:     An Shi
 *  Mofification:
 *      1. Add a function to write DC current to Ramping PS.
 * 
 * 
 *  Date: 2021.07.22
 *  Author:     An Shi
 *  Mofification:
 *      1. Add support for triangle PS (start/stop command).
 *
 *
 *  
 * *******************/


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
#include <math.h>
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
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <DigitalPSOperationCode.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

#define MAX_BUFF_SIZE 256
#define RAMP_DATA_SIZE 4000*1024   //max 4M ramping data
#define BACKLOG 100

static long init(int phase);
static long init_record();
static long get_ioint_info(int dir,dbCommon* prec,IOSCANPVT* io);
static long write_DigitalDC();
static int analyse_rec_buff(unsigned char *buf, void* raw);
static int analyse_ps_state(void *p, unsigned long state_num);
static long gen_send_buf(void *p, unsigned char *buf);
void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen);
static int open_ps_socket(void *p);
static int PSSocketConnected(int sock);
struct {
    long num;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_DigitalDC;
    DEVSUPFUN special_linconv;
} devDigitalPSRampInt = {
    6, /* space for 6 functions */
    NULL,
    init,
    init_record,
    get_ioint_info,
    write_DigitalDC,
    NULL
};
epicsExportAddress(dset,devDigitalPSRampInt);

static ELLLIST allprngs = ELLLIST_INIT;

struct prngState {
    ELLNODE node;
    double lastnum;  //readback current of PS
    unsigned long laststat; //readback state of PS
    double interval;  //the interval of scan , second
    char ip[20];
    char port[8];
    char listen_port[8];
    int ver;	//the protocol version of PS
    bool is_write;  //is or not write the ramping data to PS
    bool is_read;	// is or not read the state of PS
    char command[8]; // the command will send to PS
    float current_val; //current will send to PS
    epicsMutexId lock;
    IOSCANPVT scan;
    epicsThreadId generator;
    struct DigitalDCPSaoRecord *pao;
    unsigned char ramping_data[RAMP_DATA_SIZE];
    int ramping_data_len;       // The length(byte) of the ramping data.
    int sockfd;     // The socket what to connect to PS
};


static void start_workers(initHookState state);
static void worker(void* raw);
static void receive_ramping_data_worker(void* raw);
static void ps_command_worker(void* raw);
// int open_ps_socket(void *p);
// int PSSocketConnected(int sock);
int create_message_queue(void *p);

static long init(int phase)
{
    if(phase==0)
    {
        printf("device init phase 0\n");
        initHookRegister(&start_workers);
    }
        
    return 0;
}


static long init_record(pao)
    struct DigitalDCPSaoRecord	*pao;
{
    struct prngState* priv;
    char temp_string[40] = {0,};
    char seps[]   = ":";
    char *pos = NULL;

    priv = callocMustSucceed(1,sizeof(*priv),"prngintr");
    
    //get the parament
    strcpy(temp_string, pao->out.value.instio.string);
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
        priv->interval = 1.0; //default 1 Second
    }
    priv->interval = strtod(pos, &endptr);

    pos = strtok( NULL, seps); //get the listen port what use to receive data from database server application
    if (pos == NULL)
    {
        strcpy(priv->listen_port, "40000");
    }
    strcpy(priv->listen_port, pos);

    // Register IOC as interrupt mode.
    scanIoInit(&priv->scan);
    priv->lock = epicsMutexMustCreate();
    priv->generator = NULL;
    
    ellAdd(&allprngs, &priv->node);
    //init the command string
    memset(pao->command, '\0', sizeof(pao->command));
    // strcpy(pao->command, "AA");
    pao->linr = 0;
    pao->opi_conn = 1;
    pao->state = 0;

    strcpy(priv->ip, pao->ip_addr);
    strcpy(priv->port, pao->port);
    strcpy(priv->command, "AA");
    priv->is_write = FALSE;
    priv->is_read = FALSE;
    priv->current_val = 0;
    priv->pao = pao;
    pao->dpvt = priv;
    memset(priv->ramping_data, '\0', RAMP_DATA_SIZE);
    printf("%s init finished\n", pao->name);
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
    struct prngState* priv = raw;
    unsigned char send_buff[MAX_BUFF_SIZE] = {0,};
    unsigned char receive_buff[MAX_BUFF_SIZE] = {0,};
    Operat_Code op_result = 0; //success
    // structure for message queue 
    struct mesg_buffer { 
        long mesg_type; 
        char mesg_text[16]; 
    } message;

    printf("%s worker start\n", priv->pao->name);
    short send_size = 0;
    int recvd = 0;
    struct DigitalDCPSaoRecord* pao = priv->pao;
    //int reconnect_times = 0;
    float sleep_time = 3.0; //sleep 3 seconds
    
    //TODO: Create the thread -- receive_ramping_data_worker
    pthread_t thread_receive;
    pthread_t thread_command;

    if ( pthread_create(&thread_receive, NULL, receive_ramping_data_worker, priv)!= 0 )
    {
        /* code */
        printf("create recevie ramping data thread failed,can't get the data from database\n");
    }

    if ( pthread_create(&thread_command, NULL, ps_command_worker, priv)!= 0 )
    {
        /* code */
        printf("create command thread failed,can't send command to PS\n");
    }

    char log_info[254] = {0,};
    // Create a message queue to receive command from data receive thread and PS operation thread.
    printf("%s create message queue\n", priv->pao->name);
    int msgid;
    msgid = create_message_queue(raw);
    printf("message queue id is %d\n", msgid);
    message.mesg_type = 1;
    int connect_flag = 1;       // a flag for connection status.
    int first_time = 1;         // flag for IOC first time start.
    while(1)
    {
        // establish a connection to PS
        priv->sockfd = open_ps_socket(priv);
        if(PSSocketConnected(priv->sockfd)==0)
        {
            if (connect_flag)
            {
                connect_flag = 0;
                sprintf(log_info, "%s connect to %s error", pao->name, priv->ip);
                syslog(LOG_ERR, log_info);
            }
                
            close(priv->sockfd);
            priv->sockfd = 0;
            op_result = CONNECT_PS; //connect to PS error
            dbPutLink(&pao->out1, DBR_USHORT, &op_result, 1); //send a connect state to operator PV
            epicsThreadSleep(30);
            continue;
        }

        while (priv->sockfd)
        {
            if (!connect_flag || first_time)
            {
                connect_flag = 1;
                first_time = 0;
                sprintf(log_info, "%s established a connect to %s", pao->name, priv->ip);
                syslog(LOG_INFO, log_info);
            }
            
            // msgrcv to receive message 
            msgrcv(msgid, &message, sizeof(message), 1, 0);
            //printf("Get a message %s\n", message.mesg_text);
            // Get a ramping data from ramping data thread.
            if (strcmp(message.mesg_text, "RAMPING") == 0)
            {
                int total_send = 0;
                total_send = send(priv->sockfd, priv->ramping_data, priv->ramping_data_len , 0);
                if (total_send == -1)
                {
                    perror("send begin");
                    close(priv->sockfd);
                    priv->sockfd = 0;
                    sprintf(log_info, "[%s] send ramping data to %s failed\n", pao->name, priv->ip);
                    syslog(LOG_ERR, log_info);
                    op_result = SEND_TO_PS;
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                    continue;
                }
        
                sprintf(log_info, "[%s] total send [%d]byte ramping data to %s\n", pao->name, total_send, priv->ip);
                syslog(LOG_INFO, log_info);
                memset(receive_buff, '\0', MAX_BUFF_SIZE);
                int recvd = recv(priv->sockfd, receive_buff, MAX_BUFF_SIZE, 0);
                if(recvd == -1 && errno == EAGAIN)
                {
                    printf("recv timeout\n");
                    close(priv->sockfd);
                    priv->sockfd = 0;
                    sprintf(log_info, "[%s] receive response from %s failed\n", pao->name, priv->ip);
                    syslog(LOG_ERR, log_info);
                    op_result = RECV_RESP_PS; //can't get the response from the PS
                    dbPutLink(&pao->out1, DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                    continue;  //can't get the response of device
                }
        

                // log the response command from the PS
                char str_command[128] = {0,};
                HexToAscii(receive_buff, str_command, recvd);
                sprintf(log_info, "[%s] get response command [%s]:len[%d] from PS devcie\n", pao->name, str_command, recvd);
                syslog(LOG_INFO, log_info);
            
                //analyse the receive response from the PS
                int ret = 0;
                ret = analyse_rec_buff(receive_buff, priv);
                if ( ret != 1)
                {
                    sprintf(log_info, "[%s:%s] Return a error\n", pao->name, priv->ip);
                    syslog(LOG_ERR, log_info);
                    if (ret == 3)
                    {
                        op_result = PS_CONTROLLER;  // Injection BUMP PS return a PS controller connection error
                    }
                    else
                    {
                        op_result = DATA_PS; //the ramping data what send to PS is error
                    }
                        
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                }
                else
                {
                    op_result = OP_SUCCESS; //the ramping data what send to PS successful
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                }
                //continue;
            }

            if (strcmp(message.mesg_text, "COMMAND") == 0)
            {
                memset(receive_buff, '\0', MAX_BUFF_SIZE);
                recvd = 0;
                //send_count++;
                if (op_result == CONNECT_PS)
                {
                    //send_count = 0;
                    op_result = OP_SUCCESS; //success established a connect
                    dbPutLink(&pao->out1, DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                }
                
                epicsMutexMustLock(priv->lock);

                
                if ( !gen_send_buf(priv, send_buff) )
                {
                    op_result = OTHERS; //operator unsuccess
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
                    epicsMutexUnlock(priv->lock);
                    continue;
                }
                epicsMutexUnlock(priv->lock);

                send_size = *(send_buff + 4);
                if ( send(priv->sockfd, send_buff, send_size, 0) == -1 )
                {
                    perror("send begin");
                    close(priv->sockfd);
                    priv->sockfd = 0;
                    // epicsThreadSleep(sleep_time);
                    // printf("after close socket = %d", priv->sockfd);
                    continue;
                }

                recvd = recv(priv->sockfd, receive_buff, MAX_BUFF_SIZE, 0);
                if(recvd == -1 && errno == EAGAIN)
                {
                    printf("recv timeout\n");
                    close(priv->sockfd);
                    priv->sockfd = 0;
                    continue;  //can't get the response of device
                }

                //analyse the receive response from the PS
                int ret = 0;
                ret = analyse_rec_buff(receive_buff, priv);
                if ( ret != 1)
                {
                    if (ret == 3)
                    {
                        op_result = PS_CONTROLLER;  // Injection BUMP PS return a PS controller connection error
                    }
                    else
                    {
                        op_result = OTHERS; //the ramping data what send to PS is error
                    }
                        
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a connect state to operator PV
                    epicsThreadSleep(priv->interval);
                    continue;  //can't get the response of device
                }
                else
                {
                    op_result = OP_SUCCESS;
                    dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a successful state to operator PV
                }
                /*
                if (analyse_rec_buff(receive_buff, priv) != 1)
                {
                    printf("received buffer is error\n");
                    epicsThreadSleep(priv->interval);
                    continue;  //can't get the response of device
                }
                */
                    
                scanIoRequest(priv->scan);
                // epicsThreadSleep(priv->interval);
            }
        }    
    }
}

static void receive_ramping_data_worker(void *raw)
{
    struct prngState* priv=raw;

    if (!priv)
    {
        /* code */
        printf("the priv is NULL, receive data thread can't be create\n");
        return;
    }
    struct DigitalDCPSaoRecord* pao = priv->pao;

    if (!pao)
    {
        /* code */
        printf("the priv is NULL, receive data thread can't be create\n");
        return;
    }

    //TODO: 1.Create a listen socket on the priv->listen_port;
    //      2.wait for data and receive data, after that save the data in priv->ramping_data
    //		3.create a connect to PS then send the ramping data to PS
    //		notice : we need three sockets, one is listen socket, second is new accept socket to receive data, the third is send socket
    // unsigned char receive_buff[MAX_BUFF_SIZE] = {0,};
    int sockfd,new_fd, send_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    socklen_t sin_size;
        
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        return;
    }
    
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    {
        perror("setsockopt");
        return;
    }
    
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons( atoi(priv->listen_port) );     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) 
    {
        perror("bind");
        return;
    }

    if (listen(sockfd, BACKLOG) == -1) 
    {
        perror("listen");
        return;
    }

    char log_info[200] = {0,};
    struct mesg_buffer { 
        long mesg_type; 
        char mesg_text[16]; 
    } message;

    // Create a message queue to receive command from data receive thread and PS operation thread.
    int msgid; 
    msgid = create_message_queue(raw);
    message.mesg_type = 1;
    strcpy(message.mesg_text, "RAMPING");

    while(1) 
    {  // main accept() loop
        sin_size = sizeof their_addr;
        Operat_Code opt_state = OP_SUCCESS; //the state code of operation
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size)) == -1) 
        {
            perror("accept");
            continue;
        }
        printf("[%s]have a Database connection\n", pao->name);
        unsigned long recv_num = 0, accept_len = 0;
        unsigned char *p_buff;
        epicsMutexMustLock(priv->lock);
        memset(priv->ramping_data, '\0', RAMP_DATA_SIZE);
        p_buff = priv->ramping_data;
        do 
        {
            recv_num = recv( new_fd, p_buff + accept_len, RAMP_DATA_SIZE - accept_len, 0 );
            accept_len = accept_len + recv_num;

        }while ((recv_num > 0)); //receive all of the ramping data
        close(new_fd);

        //TODO: Create the send socket , and send the priv->ramping_data to PS , the send size is accept_len
        if (accept_len == 0)
        {
            /* code */
            opt_state = RECV_DATA;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            continue;
        }

        if (*p_buff == '#')
        {
            opt_state = ORACLE_DATA;   //oracle app send a error message
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            continue;	
        }
        priv->ramping_data_len = accept_len;
        epicsMutexUnlock(priv->lock);
        printf("[%s]Receive ramping data [size %ld]\n", pao->name, accept_len);
        sprintf(log_info, "[%s] received ramping data [size %ld]\n", pao->name, accept_len);
        syslog(LOG_INFO, log_info);

        //if(PSSocketConnected(priv->sockfd) == 0)
        // if (strncmp(pao->name, "RING:PS4BPI", 11) == 0)
        // {
        //     // msgsnd to send message 
        //     msgsnd(msgid, &message, sizeof(message), 0);
        //     continue;
        // }

        if (priv->sockfd == 0)
        {
            printf("disconnected, can't send data to PS %s\n", pao->name);
            epicsThreadSleep(3.0);
            continue;
        }
        // msgsnd to send message 
        msgsnd(msgid, &message, sizeof(message), 0); 
        #if 0
        while (priv->is_read) //now is reading the state of  PS, so we just wait
        {
            /* code */
            epicsThreadSleep(0.1);
        }
        priv->is_write = TRUE;
        /* ------------ create send socket -----------------*/
        struct sockaddr_in device_addr;
        device_addr.sin_family = AF_INET;
        device_addr.sin_port = htons(atoi(priv->port));
        device_addr.sin_addr.s_addr = inet_addr(priv->ip);
        memset(device_addr.sin_zero, '\0', sizeof(device_addr.sin_zero));

        if((send_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket");
            priv->is_write = FALSE;
            opt_state = CREATE_SEND_SOCKET;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            //epicsThreadSleep(5);
            continue;
        }

        //set the timeout of network function
        struct timeval timeout={2,0};//2s
        //set the connet time out also
        if ( (setsockopt(send_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout))) == -1 )
        {
            perror("setsockopt");
            close(send_fd);
            priv->is_write = FALSE;
            opt_state = SET_SEND_SOCKET;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            //epicsThreadSleep(1);
            continue;
        }
        if ( (setsockopt( send_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout) )) == -1)
        {
            perror("setsockopt");
            close(send_fd);
            priv->is_write = FALSE;
            opt_state = SET_SEND_SOCKET;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            //epicsThreadSleep(1);
            continue;
        }

        
        if (connect(send_fd, (struct sockaddr *)&device_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("connect");
            priv->is_write = FALSE;
            opt_state = CONNECT_PS;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            close(send_fd);
            printf("[%s] connect to %s failed\n", pao->name, priv->ip);
            sprintf(log_info, "[%s]connect to %s failed\n", pao->name, priv->ip);
            syslog(LOG_ERR, log_info);
            //epicsThreadSleep(sleep_time);
            continue;
        }

        // int i = 0;
        // int send_times = 0;
        // send_times = 2; 			//Test: send ramping data two times
        // for (i = 0; i < send_times; i++){
        int total_send = 0;
        total_send = send(send_fd, priv->ramping_data, accept_len , 0);
        // if ( send(send_fd, priv->ramping_data, accept_len, 0) == -1 )
        if (total_send == -1)
        {
            perror("send begin");
            close(send_fd);
            sprintf(log_info, "[%s] send ramping data to %s failed\n", pao->name, priv->ip);
            syslog(LOG_ERR, log_info);
            priv->is_write = FALSE;
            opt_state = SEND_TO_PS;
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            //epicsThreadSleep(3);
            continue;
        }
        // int k = 0;
        // int total_send = 0;
        // void * start = (void *)priv->ramping_data;
        // for( k = 0; accept_len > 0; )
        // {
        //     if (accept_len > 4000)
        //         k = send(send_fd, start, 4000, 0);
        //     else
        //         k = send(send_fd, start, accept_len, 0);
        //     if (k < 0)
        //     {
        //         sprintf(log_info, "[%s] send ramping data to %s failed\n", pao->name, priv->ip);
        //         syslog(LOG_ERR, log_info);
        //     }
        //     printf("have send:%d\n",k);
        //     total_send += k;
        //     start += k;
        //     accept_len = accept_len - k;
        // }
        sprintf(log_info, "[%s] total send [%d]byte ramping data to %s\n", pao->name, total_send, priv->ip);
        syslog(LOG_INFO, log_info);
        memset(receive_buff, '\0', MAX_BUFF_SIZE);
        int recvd = recv(send_fd, receive_buff, MAX_BUFF_SIZE, 0);
        if(recvd == -1 && errno == EAGAIN)
        {
            printf("recv timeout\n");
            close(send_fd);
            sprintf(log_info, "[%s] receive response from %s failed\n", pao->name, priv->ip);
            syslog(LOG_ERR, log_info);
            priv->is_write = FALSE;
            opt_state = RECV_RESP_PS; //can't get the response from the PS
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
            //epicsThreadSleep(3);
            continue;  //can't get the response of device
        }
        // if (i == 0)
        //     epicsThreadSleep(0.5);
        // }
        // sprintf(log_info, "[%s] receive response command [%s]\n", pao->name, priv->ip);
        // syslog(LOG_ERR, log_info);
        //TODO: analyse the PS response state
        
        //priv->is_write = FALSE;

        // log the response command from the PS
        char str_command[128] = {0,};
        HexToAscii(receive_buff, str_command, recvd);
        sprintf(log_info, "[%s] get response command [%s]:len[%d] from PS devcie\n", pao->name, str_command, recvd);
        syslog(LOG_INFO, log_info);
    
        //analyse the receive response from the PS
        if (analyse_rec_buff(receive_buff, priv) != 1)
        {
            sprintf(log_info, "[%s:%s] analyze the response command failed\n", pao->name, priv->ip);
            syslog(LOG_ERR, log_info);
            opt_state = DATA_PS; //the ramping data what send to PS is error
            dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
        }

        opt_state = OP_SUCCESS; //the ramping data what send to PS successful
        dbPutLink(&pao->out1,DBR_USHORT, &opt_state, 1); //send a connect state to operator PV
        close(send_fd);
        priv->is_write = FALSE;
        #endif
    }
    
}

static void ps_command_worker(void *raw)
{
    struct prngState* priv = raw;
    struct DigitalDCPSaoRecord* pao = priv->pao;
    
    char log_info[254] = {0,};
    struct mesg_buffer { 
        long mesg_type; 
        char mesg_text[16]; 
    } message;

    // Create a message queue to receive command from data receive thread and PS operation thread.
    int msgid; 
    msgid = create_message_queue(raw);
    message.mesg_type = 1;
    strcpy(message.mesg_text, "COMMAND");
    while (1)
    {
        /* code */
        // msgsnd to send message
        //if(PSSocketConnected(priv->sockfd) == 0)
        if (priv->sockfd == 0)
        {
            // printf("disconnected, can't send command to PS %s\n", pao->name);
            epicsThreadSleep(3.0);
            continue;
        }

        msgsnd(msgid, &message, sizeof(message), 0);
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
    pao->state = priv->laststat;

    // strcpy(priv->command, "AA");
    // //if (strcmp(pao->command, "AA") != 0 )
    // if (strlen(pao->command) != 0)
    // {
    //     strcpy(priv->command, pao->command);
    //     //strcpy(pao->command, "AA");
    //     memset(pao->command, '\0', sizeof(pao->command));
    // }
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
    short command_len = 0;
    
    p_buf = buf;
    command_len = *(buf + 4);
    char log_info[200] = {0,};
    // char str_command[128] = {0,};
    // if (*(p_buf + 10) == 0xb4)
    // {
    //     HexToAscii(p_buf, str_command, command_len);
    //     sprintf(log_info, "[%s] get response command [%s] from PS devcie\n", pao->name, str_command);
    //     syslog(LOG_INFO, log_info);
    // }
    
    if ( (*p_buf != 0xff) || (*(p_buf + command_len - 1) != 0xff) )
    {
        printf("Debug:[%s]received buffer is error!\n", pao->name);
        // sprintf(log_info, "[%s] got response command buffer is error\n", pao->name);
        // syslog(LOG_ERR, log_info);
        return 0;
    }
    // Store response command what from the PS to log. 
    
    
    if ( *(p_buf + 9) != 0x00) //PS return a error
    {
        if ( *(p_buf + 9) == 0xEE) //Inject BUMP PS return PS controller disconnectted.
            return 3;
        
        return 2;
    }
    //char rtn_code = '\0';
    //rtn_code = *(p_buf + 9);
    
    //if ( (rtn_code == 0x00) && (*(p_buf + 10) == 0xaa)) //command is validation and is state information
    if ( *(p_buf + 10) == 0xaa) //command is state information
    {
        //TODO: get the return value
        //current
        float f_curr = 0;
        unsigned long l_state = 0;

        p_buf += 11; // state of PS
        memcpy(&l_state, p_buf, 4);
        analyse_ps_state(priv, l_state);

        p_buf = buf;
        p_buf += 15; //current value
        memcpy(&f_curr, p_buf, 4);
        epicsMutexMustLock(priv->lock);
        priv->lastnum = f_curr;
        epicsMutexUnlock(priv->lock);
    }
    

    return 1;

}

static int analyse_ps_state(void *p, unsigned long state_num)
{
    struct prngState *priv = p;
    unsigned long temp_state = 0;

    //printf("Debug:origin state_num is %u\n", state_num);
    
    if (state_num & 0x00000002) // ps run or stop
    {
        temp_state |= 0x00000001;
    }

    if (!(state_num & 0x00000001)) // ps relay error
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
    

    epicsMutexMustLock(priv->lock);
    priv->laststat = temp_state;
    epicsMutexUnlock(priv->lock);
    
    return 1;
}


//generate the send buffer
static long gen_send_buf(void *p, unsigned char *buf)
{
    if (!buf || !p)
        return 0;
    
    struct prngState *priv = p;
    unsigned char *p_buf;
    struct DigitalDCPSaoRecord* pao = priv->pao;
    char log_info[256] = {0,};
        
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

    // if (pao->current_val != priv->current_val)  //will write the new current value to PS
    if ( fabsf(pao->current_val - priv->current_val) > 0.0001)
    {
        *(p_buf++) = 0xb3;
        float current_val = pao->current_val;
        memcpy(p_buf, &current_val, 4);
        *(buf + 4) = 0x12;  //length
        p_buf += 4;
        priv->current_val = pao->current_val;
        sprintf(log_info, "[%s] will adjust current to [%f]\n", pao->name, priv->current_val);
        syslog(LOG_INFO, log_info);
    }
    else
    {
        strcpy(priv->command, "AA");        // Default will send readback command.
        //if (strcmp(pao->command, "AA") != 0 )
        if (strlen(pao->command) != 0)
        {
            strcpy(priv->command, pao->command);
            if ( (strcmp(priv->command, "START") == 0) || (strcmp(priv->command, "STOP") == 0))
            {
                // Here is just for v4/v3 triangle scan PS
                if (priv->ver == 4)
                    *(p_buf++) = 0xac;
                else
                    *(p_buf++) = 0xab;                              // CID2 set scan PS start/stop output is 0xB2
                if (strcmp(priv->command, "START") == 0)        // 0x00 is start, 0x01 is stop
                {    
                    *(p_buf++) = 0x00;
                    printf("debug: SCAN PS [%s] will START\n", pao->name);
                }
                else
                {
                    *(p_buf++) = 0x01;
                    printf("debug: SCAN PS [%s] will STOP\n", pao->name);
                }
                
                *(buf + 4) = 0x0f;  //length is 15 bytes
            }
            else
            {
                if ( (strcmp(priv->command, "B601") == 0) || (strcmp(priv->command, "B600") == 0))
                {
                    *(p_buf++) = 0xb6;
                    if (strcmp(priv->command, "B601") == 0)
                    {
                        *(p_buf++) = 0x01;
                    }
                    else
                    {
                        *(p_buf++) = 0x00;
                    }
                    *(buf + 4) = 0x0f;      //length is 15 bytes
                }
                else
                {
                    sscanf(priv->command, "%02x", p_buf);
                    *(buf + 4) = 0x0e;      //length
                    p_buf ++;
                }
            }
            //strcpy(pao->command, "AA");
            memset(pao->command, '\0', sizeof(pao->command));
            sprintf(log_info, "[%s] will send command [%s]\n", pao->name, priv->command);
            syslog(LOG_INFO, log_info);
        }
        else
        {
            sscanf(priv->command, "%02x", p_buf);
            *(buf + 4) = 0x0e; //length
            p_buf ++;
        }
        
    }
    
    //the end of the command
    *(p_buf++) = 0x00;
    *(p_buf++) = 0x00;
    *(p_buf++) = 0xff;
    *(p_buf++) = 0xff;

    // if priv->command is B601 or B600, print the buffer to log for debug
    if ( (strcmp(priv->command, "B601") == 0) || (strcmp(priv->command, "B600") == 0))
    {
        char str_command[32] = {0,};
        HexToAscii(buf, str_command, *(buf + 4));
        sprintf(log_info, "[%s] send command [%s]:len[%d] to PS devcie\n", pao->name, str_command, *(buf + 4));
        syslog(LOG_INFO, log_info);
    }
    return 1;
}


void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen)
{
    unsigned char Nibble[2];
    int i = 0;
    int j = 0;
    for (i = 0; i < nLen; i++)
    {
        Nibble[0] = (pHex[i] & 0xF0) >> 4;
        Nibble[1] = pHex[i] & 0x0F;
        for (j = 0; j < 2; j++)
        {
            if (Nibble[j] < 10)
                Nibble[j] += 0x30;
            else
            {
                if (Nibble[j] < 16)
                    Nibble[j] = Nibble[j] - 10 + 'A';
            }
            *pAscii++ = Nibble[j];
        }	// for (int j = ...)
    }	// for (int i = ...)
}

static int PSSocketConnected(int sock)
{
    if (sock <= 0)
        return 0;
    struct tcp_info info;
    int len = sizeof(info);
    getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *) & len);
    if ((info.tcpi_state == TCP_ESTABLISHED)) 
    {
         return 1;
    } else {
         return 0;
    }
}

static int open_ps_socket(void *p)
{
    struct prngState *priv=p;
    /* ------------ create socket -----------------*/
    if(priv->ip == NULL) return -EINVAL;
    
    /*
    struct hostent *hptr = gethostbyname(url);
    if(hptr == NULL) return -EFAULT;
    */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0){
        return -EFAULT;
    }
    struct sockaddr_in device_addr;
    device_addr.sin_family = AF_INET;
    device_addr.sin_port = htons(atoi(priv->port));
    device_addr.sin_addr.s_addr = inet_addr(priv->ip);
    memset(device_addr.sin_zero, '\0', sizeof(device_addr.sin_zero));

    //set the timeout of network function
    struct timeval timeout={30,0};//3s
    
    //set the connect timeout also
    if ( (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout))) == -1 )
    {
        perror("setsockopt");
    }
    if ( (setsockopt( sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout) )) == -1)
    {
        perror("setsockopt");
    }
    
    int reval = connect(sockfd,(struct sockaddr *)&device_addr,sizeof(device_addr));
    return sockfd;
    //printf("%s, socket connected \n",config->domain);
    //return reval;
}


int create_message_queue(void *p)
{
    struct prngState* priv = p;
    struct DigitalDCPSaoRecord* pao = priv->pao;
    key_t key; 
    int msgid;
    char path_name[128] = "/var/tmp/";

    strcat(path_name, pao->name);
    printf("%s\n", path_name);
    FILE* fp = fopen(path_name, "w");
    printf("%u\n", (unsigned int)fp);
    if (!fp)
        perror("fopen");
    else
        fclose(fp);

    // 
    // ftok to generate unique key 
    key = ftok(path_name, 65); 

    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT);
    return msgid;
}
