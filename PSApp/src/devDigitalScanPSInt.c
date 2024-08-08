/* devDigitalScanPSInt.c */
/* Digital Scan Power Supply interrupt device support module with ramping protocol(V3 & V4)*/


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
#include "DigitalScanPSRecord.h"

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

#define MAX_SEND_BUFF_SIZE 1024 * 100
#define MAX_BUFF_SIZE 64

static long init(int phase);
static long init_record();
static long get_ioint_info(int dir,dbCommon* prec,IOSCANPVT* io);
static long write_DigitalScan();
static int analyse_rec_buff(unsigned char *buf, void* raw);
static int analyse_ps_state(void *p, unsigned long state_num);
static long gen_send_buf(void *p, int is_adjust, unsigned char *buf, float freq_val, float amp_val);
static int WriteCurrentTrigWave(unsigned char *buf);
struct {
	long num;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN write_DigitalScan;
	DEVSUPFUN special_linconv;
} devDigitalScanPSInt = {
	6, /* space for 6 functions */
	NULL,
	init,
	init_record,
	get_ioint_info,
	write_DigitalScan,
	NULL
};
epicsExportAddress(dset,devDigitalScanPSInt);

static ELLLIST allprngs = ELLLIST_INIT;

struct prngState {
	ELLNODE node;
	double lastnum;  //readback current of PS
	unsigned long laststat; //readback state of PS
	double interval;  //the interval of scan , second
	char ip[20];
	char port[8];
	int ver;	//the protocol version of PS
	bool is_write;  //is or not write setpoint to PS
	char command[8]; // the command will send to PS
	float freq_val; //frequency will send to PS
    float amp_val; //amplitude will send to PS
	epicsMutexId lock;
	IOSCANPVT scan;
	epicsThreadId generator;
	struct DigitalScanPSRecord *pao;
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
    struct DigitalScanPSRecord	*pao;
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
    	priv->ver = 3;                // default version set as 3.
    }
    char *endptr;
    priv->ver = strtod(pos, &endptr);

    pos = strtok( NULL, seps); //get the AI scan interval time
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
	pao->state = 0;

	strcpy(priv->ip, pao->ip_addr);
    strcpy(priv->port, pao->port);
    strcpy(priv->command, "AA");
    priv->is_write = FALSE;
    priv->freq_val = 0;
    priv->amp_val = 0;
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
	unsigned char send_buff[MAX_SEND_BUFF_SIZE] = {0,};
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
    struct DigitalScanPSRecord* pao = priv->pao;

    //int reconnect_times = 0;
    int sleep_time = 10; //at first sleep 10 seconds
    while(1)
    {
		memset(send_buff, '\0', MAX_SEND_BUFF_SIZE);
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

	    if ( !gen_send_buf(priv, is_adjust, send_buff, priv->freq_val, priv->amp_val) )
	    {
	    	close(sockfd);
	    	op_result = DATA_PS; //operator unsuccess
        	if (is_adjust)
        		dbPutLink(&pao->out1, DBR_USHORT, &op_result, 1); //send a signal to operator PV
	    	epicsThreadSleep(1);
	    	continue;
	    }

		memcpy(&send_size, send_buff + 4, sizeof(short));
	    // send_size = (short)*(send_buff + 4);
		int send_count = send(sockfd, send_buff, send_size, 0);
	    if ( send_count == -1 )
	    {
	        perror("send begin");
	        close(sockfd);
	        op_result = SEND_TO_PS; //operator unsuccess
        	if (is_adjust)
        		dbPutLink(&pao->out1,DBR_USHORT, &op_result, 1); //send a signal to operator PV
	        epicsThreadSleep(3);
	       	continue;
	    }
		if (is_adjust)
		{
			printf("will send %d, have send %d byte to PS\n", send_size, send_count);
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

static long write_DigitalScan(pao)
    struct DigitalScanPSRecord	*pao;
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

	if ( (pao->freq_val != priv->freq_val) || (pao->amp_val != priv->amp_val) )  //will write the new value to PS
	{
		priv->is_write = TRUE;
		priv->freq_val = pao->freq_val;
        priv->amp_val = pao->amp_val;
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

	int i  = 0;
	if (*(p_buf + 10) == 0xb4)
	{
		printf("receive command [command length %d]: ", command_len);
		for (i = 0; i < command_len; i++)
		{
			printf("%02X ", *(p_buf + i));
		}
	}

	if ( (rtn_code == 0x00) && (*(p_buf + 10) == 0xaa)) //command is validation and is state information
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

	if (state_num & 0x00000004) // ps relay error
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

	if (state_num & 0x00008000 ) //DC or ramping mode?
	{
		temp_state |= 0x00000040;
	}
	}

	epicsMutexMustLock(priv->lock);
	priv->laststat = temp_state;
	epicsMutexUnlock(priv->lock);
	
	return 1;
}


//generate the send buffer
static long gen_send_buf(void *p, int is_adjust, unsigned char *buf, float freq_val, float amp_val)
{
	if (!buf || !p)
		return 0;
	
	
	struct prngState *priv = p;
	unsigned char *p_buf;
	struct DigitalScanPSRecord* pao = priv->pao;	
	p_buf = buf;

	if (priv->ver == 4 && is_adjust)
	{
		FILE *fp;
    	char buffer[80];
		char command_buf[64] = {0,};
		sprintf(command_buf, "zerorpc tcp://127.0.0.1:4242 cal_triangle_wave %f %f", freq_val, amp_val);
		// fp=popen("zerorpc tcp://127.0.0.1:4242 cal_triangle_wave 50 12", "r");
		fp = popen(command_buf, "r");
		fgets(buffer,sizeof(buffer),fp);
		printf("%s\n",buffer);

		if (strstr(buffer, "success"))
		{
			return WriteCurrentTrigWave(buf);
		}
		else
		{
			return 0;
		}
		
	}
	else
	{
		/* code */
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
		
		// channel/address
		*(p_buf++) = 0x01;
		// CID1
		*(p_buf++) = 0x2a;
		if (is_adjust)  //adjust current of power
		{
			*(p_buf++) = 0xb2;              // CID2 set scan PS outpu is 0xB2
			freq_val = 1.0 / freq_val;        // convert frequency to cycle time (second)
			memcpy(p_buf, &freq_val, 4);
			p_buf += 4;
			memcpy(p_buf, &amp_val, 4);
			*(buf + 4) = 0x16;  //length is 22 bytes
			p_buf += 4;
			printf("debug: SCAN PS freq = %f, amp = %f \n", freq_val, amp_val);
		}
		else	//others command
		{
			if ( (strcmp(priv->command, "START") == 0) || (strcmp(priv->command, "STOP") == 0))
			{
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
		//	printf("debug: SCAN PS will START/STOP\n");
				//p_buf += 4;
			}
			else
			{
				sscanf(priv->command, "%02x", p_buf);
				*(buf + 4) = 0x0e; //length is 14 bytes
				p_buf ++;
			}
		}

		//the end of the command
		*(p_buf++) = 0x00;
		*(p_buf++) = 0x00;
		*(p_buf++) = 0xff;
		*(p_buf++) = 0xff;
		if (is_adjust)  //adjust current of power
		{
			int i = 0;
			for (i=0; i < 22; i++)
				printf("%02X ", *(buf + i));
		}	
		return 1;
	}
	
	
}

static int WriteCurrentTrigWave(unsigned char *buf)
{
	unsigned short *p_buf;
	int S = 0;
	// unsigned int delay= 400*25;
    // unsigned int * delay2=(void *)(&(p_buf[S+13])+1);

	p_buf = (unsigned short *)buf;

	FILE * myfile;
    myfile=fopen("/var/tmp/trig_wave","r");		//wave data file generate by Python server
    if(myfile==NULL)
    {
        printf("open failed");
        return 0;
    }

	FILE * template_file;
    template_file = fopen("20A100Hz","rb");
    if(template_file == NULL)
    {
        printf("open template file failed");
        return 0;
    }
    fread(buf, sizeof(unsigned char), 0x3b, template_file);
    //printf("start write\n");
	#if 0
    p_buf[S+0]=0xffff;
    p_buf[S+1]=0x0000;

    p_buf[S+2]=0x2ee5;//data length
    p_buf[S+3]=0x0004;//version
    p_buf[S+4]=0xb42a;//order

    p_buf[S+5]=0x2000;//table length

    p_buf[S+6]=0x0000;
    p_buf[S+7]=0x0300;//event count
    p_buf[S+8]=0x0000;

    p_buf[S+9]=0x0100;//waveform count
    p_buf[S+10]=0x0500;
    p_buf[S+11]=0x1cc0;
    p_buf[S+12]=0x06fb;
    p_buf[S+13]=0x0100;
    
    p_buf[S+14]=0x0900;
    p_buf[S+15]=0xf7c0;
    p_buf[S+16]=0x003f;
    p_buf[S+17]=0x0100;

    p_buf[S+18]=0x5a20;
    p_buf[S+19]=0xf7c0;
    p_buf[S+20]=0x003f;
    p_buf[S+21]=0x0000;

    p_buf[S+22]=0x0040;
    //printf("wave4\n");
	
    unsigned short * align=((void *)(p_buf))+1;
    //printf("wave5\n");
    p_buf[S+23]=0x0000;
    p_buf[S+24]=0x0000;
    p_buf[S+25]=0x0100;
    
    p_buf[S+26]=0x0000;
    p_buf[S+27]=0x0000;
    p_buf[S+28]=0x0000;
    p_buf[S+29]=0xff00;	//waveform number

    p_buf[S+30]=0x0000; //waveform number
    //p_buf[S+30]=p->w_no;
    p_buf[S+31]=0x0000;
    p_buf[S+32]=0x0000;
    p_buf[S+33]=0x0000;
	#endif

	unsigned short * align=((void *)(p_buf))+1;
    int i,j;
    unsigned short k;
    float f_val = 0;
    //for(i=0;i<99;i++)
//      printf("%f\n",((double *)(p->bptr))[i]);
   // unsigned short count=((double *)(p->bptr))[0]*100;
    //printf("wave7\n");
	int w_length = 0;
	char tmp_buf[32];
	fgets(tmp_buf, sizeof(tmp_buf), myfile);
	w_length = atoi(tmp_buf);
    int count = w_length;
	p_buf[S+2] = count*4+ 0x3b + 4;
	align[S+19] = count*4+16;
	align[S+27] = count*4;
    //printf("wave8\n");
    // double test;
    //printf("%d,%d,%d\n",delay2,delay,&p_buf);
    // memcpy(delay2, &delay, 4);
    //*delay2=delay;
    //printf("wave6\n");
    // align[S+30] = p->w_no;
	//align[S+30] = 0x0000;
    // align[S+31] = count*4-2;
	
    
	//align[S+23] = count*4+16;
    ;
	//printf("wave3\n");
	
    for(i=29,j=1;j<=count;j++)
    {
        //test=((double *)p->bptr)[j-1];
    	fgets(tmp_buf, sizeof tmp_buf, myfile);
		f_val = atof(tmp_buf);
        // f_val = ((float *)p->bptr)[j];
        memcpy(align + i, &f_val, 4);
        //printf("%f\n",test);
        // k=((ushort *)p->bptr)[j];
        // *(align+i)=k;
        // *(align+i+1)=k;
        //printf("%02X %hu|",k,((ushort *)p->bptr)[j]);
        i+=2;
        //*(p_buf+i)=k;
        //i+=2;
    }
    //printf("wave2\n");
    // i-=2;
        
    align[i]=0x0000;
    i++;
    align[i]=0xffff;
    i++;
    //unsigned int index=16;
    //unsigned char * cp=(unsigned char *)p_buf;
    /*for(k=0;k<i*2+1;k++)
    {
        //if(k>24&&k<i-3) fwrite(&(p_buf[k]),sizeof(short),1,fp);
        printf("%02X ",cp[k]);
        if((k+1)%16==0&&k!=0) 
        {
                printf("\n%8X:",index);
                index+=16;
        }
    }
    fclose(fp);
    printf("\ntotal:%d\n",i);*/
    //sendData(p_buf,i*2+1); 
    int len=i*2+1;
    i=len;

    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_current.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(p_buf, sizeof(char), len, fp);
        fclose(fp);
    }
	return 1;
	/*
    void * start=(void *)p_buf;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i+10);
        if(k<0)
        {//发送消息到服务器
                printf("write message error!");
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
    }
	*/

    //printf("send:%d,%d\n",k,len);
    //close(sockfd);
    //return;
    // unsigned char temp[20000];
    /*if(k=read(priv->socket, temp, 20000) < 0)
    {//接收服务器返回的消息
          printf("read message error");
    } 
    strcpy(p->state,RTN(temp[9]));  
    //printf("%s\n",RTN(temp[9]));
    */
    // p->w_length=0;
    //printf("length:%d\n",p->w_length);
    //dbPutLink(&(priv->prec->inp), DBR_SHORT, p_buf, length+6);
}
