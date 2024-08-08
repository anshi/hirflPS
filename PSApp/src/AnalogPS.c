/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  PS.c
  *Author:  yunbin Zhou
  *Version:  1.0
  *Date:  2019.8.31
  *Description:  电源控制系统头文件
  *Others:  
  *Function List:  
 
  *History: 
     1.Date:
       Author:
       Modification:
**********************************************************************************/
#include "PS.h"

int SocketConnected(int sock)
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

int open_ps_socket(void *p)
{
    struct InitConfig *config=p;
    char * url=config->domain;
    int port=config->port;

    if(url == NULL) return -EINVAL;
    
    /*
    struct hostent *hptr = gethostbyname(url);
    if(hptr == NULL) return -EFAULT;
    */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0){
        return -EFAULT;
    }
 
    struct sockaddr_in servaddr;
    memset((void*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    //inet_pton(AF_INET, ipBuf, &servaddr.sin_addr.s_addr);
    //servaddr.sin_addr.s_addr = *((unsigned long*)hptr->h_addr_list[0]);  
    servaddr.sin_addr.s_addr = inet_addr(url);     
    int reval = connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    return sockfd;
    //printf("%s, socket connected \n",config->domain);
    //return reval;
}

int GenerateCMD(struct psRecord *ps)
{
	int i=0;
	struct InitConfig * config=ps->dpvt;
	unsigned char cmd[14]={0xff,0xff,0x00,0x00,0x0e,0x00,0x04,0x01,0x2a,0x00,0x00,0x00,0xff,0xff};
	if(strcmp(ps->version,"3")==0) cmd[6]=0x03;
	config->sendcmd=cmd;
	if(strcmp(config->command,ps->command)!=0)
	{
		config->length=0x0e;
		strcpy(config->command,ps->command);
		if(strcmp(config->command,"startup")==0)
		{
			//printf("start up\n");
			cmd[9]=0xa7;
			//config->length=0x0e;
		}
		if(strcmp(config->command,"shutdown")==0)
                {
                        cmd[9]=0xa8;
                        //config->length=0x0e;
                }
		if(strcmp(config->command,"reset")==0)
                {
                        cmd[9]=0xa9;
                        //config->length=0x0e;
                }
		if(strcmp(config->command,"switch")==0)
                {
                        cmd[9]=0xb6;
                        //config->length=0x0e;
                }
		if(cmd[9]!=0)
		{
			for(i=0;i<14;i++)
				printf("%#X ",config->sendcmd[i]);
			SendPS(config);
            		strcpy(ps->state,RTN(config->receive[9]));
			strcpy(ps->command," ");
		}
	}
	
	return 0;
}

int SetCurrent(psRecord *ps)
{
	struct InitConfig * config=ps->dpvt;
        unsigned char cmd[18]={0xff,0xff,0x00,0x00,0x12,0x00,0x04,0x01,0x2a,0xb3,0x00,0x00,0x00,0x00,0x00,0x00,0x0ff,0xff};
	
        if(strcmp(ps->version,"3")==0) cmd[6]=0x03;
        config->sendcmd=cmd;
	if(config->current_val!=ps->current_val)
	{
		config->current_val=ps->current_val;
		memcpy(cmd+10, &(config->current_val), 4);
		config->length=0x12;
        	SendPS(config);
        	strcpy(ps->state,RTN(config->receive[9]));
	}
	return 0;
}

int GetState(psRecord *ps)
{
    //printf("get state\n");
    struct InitConfig * config=ps->dpvt;
    unsigned char cmd[14]={0xff,0xff,0x00,0x00,0x0e,0x00,0x04,0x01,0x2a,0xaa,0x00,0x00,0xff,0xff};
    if(strcmp(ps->version,"3")==0) cmd[6]=0x03;
    config->sendcmd=cmd;
    config->length=0x12;
    SendPS(config);
    //strcpy(ps->state,RTN(config->receive[9]));
    if(config->receive[9]!=0) 
    {
	printf("get state error\n");
	return;
    }
    memcpy(&(ps->c_val), &(config->receive[15]), 4);
    memcpy(&(ps->v_val), &(config->receive[19]), 4);
    //printf("\n");
    ((config->receive[11])&0x01)?strcpy(ps->psstate,"OK\0"):strcpy(ps->psstate,"ERROR\0");//电源总故障
    ((config->receive[11])&0x02)?strcat(ps->psstate,"|STOP"):strcat(ps->psstate,"|RUNNING");//是否运行
    ((config->receive[11])&0x08)?strcat(ps->psstate,"|FUSE"):1;//熔断故障
    ((config->receive[11])&0x10)?strcat(ps->psstate,"|LOAD"):1;//负载故障
    ((config->receive[11])&0x20)?strcat(ps->psstate,"|POWER"):1;//功率器件故障
    ((config->receive[11])&0x40)?strcat(ps->psstate,"|WATER"):1;//缺水故障
    ((config->receive[11])&0x80)?strcat(ps->psstate,"|TEMP"):1;//过热故障
    ((config->receive[12])&0x01)?strcat(ps->psstate,"|OC"):1;//过流故障
    ((config->receive[12])&0x02)?strcat(ps->psstate,"|OV"):1;//过压故障
    ((config->receive[12])&0x04)?strcat(ps->psstate,"|INOV"):1;//交流过压故障
    ((config->receive[12])&0x08)?strcat(ps->psstate,"|INLV"):1;//交流欠压故障
    ((config->receive[12])&0x10)?strcat(ps->psstate,"|SW"):1;//开关故障
    ((config->receive[12])&0x40)?strcat(ps->psstate,"|EMEGRGY"):1;//急停状态
    ((config->receive[12])&0x80)?strcat(ps->psstate,"|RAMP"):strcat(ps->psstate,"|DC");//脉冲，直流
    ((config->receive[13])&0x40)?strcat(ps->psstate,"|D_LOCK"):1;//门开
    //printf("state:%s\n",ps->psstate);
    return 0;
}

int SendPS(struct InitConfig * config)
{
	psRecord *ps=config->prec;
	if (send(config->socket, config->sendcmd, config->length, MSG_DONTWAIT ) == -1)
    	{
            printf("connection error %s\n",config->domain);
            close(config->socket);
            return 1;
    	}
    	config->recv_length = recv(config->socket, config->receive, 64, MSG_DONTWAIT);        
	return 0;
}

char * RTN(int i)
{
	switch(i)
	{
		case 0:return "command sent successfully";
		case 1:return "wrong version number";
		case 3:return "data length incorrect";
		case 4:return "invalid CID2";
		case 5:return "data format incorrect";
		case 6:return "invalid data";
		case 7:return "data value is not invalid,ps can't proccess";
		return "other error,please contact the PS administrator";
	}
}



int WriteWave(struct psRecord *p)
{
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    unsigned int * delay2=(void *)(&(priv->cmd[S+13])+1);
    //printf("start write\n");
    priv->cmd[S+0]=0xffff;
    priv->cmd[S+1]=0x0000;

    priv->cmd[S+2]=0x2ee5;//data length
    priv->cmd[S+3]=0x0104;//version
    priv->cmd[S+4]=0xb42a;//order

    priv->cmd[S+5]=0x2000;//table length

    priv->cmd[S+6]=0x0000;
    priv->cmd[S+7]=0x0300;//event count
    priv->cmd[S+8]=0x0000;

    priv->cmd[S+9]=0x0100;//waveform count
    priv->cmd[S+10]=0x0500;
    priv->cmd[S+11]=0x1cc0;
    priv->cmd[S+12]=0x06fb;
    priv->cmd[S+13]=0x0100;
    
    priv->cmd[S+14]=0x0900;
    priv->cmd[S+15]=0xf7c0;
    priv->cmd[S+16]=0x003f;
    priv->cmd[S+17]=0x0100;

    priv->cmd[S+18]=0x5a20;
    priv->cmd[S+19]=0xf7c0;
    priv->cmd[S+20]=0x003f;
    priv->cmd[S+21]=0x0000;

    priv->cmd[S+22]=0x0040;
    //printf("wave4\n");
    unsigned short * align=((void *)(priv->cmd))+1;
    //printf("wave5\n");
    priv->cmd[S+23]=0x0000;
    priv->cmd[S+24]=0x0000;
    priv->cmd[S+25]=0x0100;
    
    priv->cmd[S+26]=0x0000;
    priv->cmd[S+27]=0x0000;
    priv->cmd[S+28]=0x0000;
    priv->cmd[S+29]=0x0000;

    priv->cmd[S+30]=0x0000; //waveform number
    //priv->cmd[S+30]=p->w_no;
    priv->cmd[S+31]=0x0000;
    priv->cmd[S+32]=0x0000;
    priv->cmd[S+33]=0x0000;


    int i,j;
    unsigned short k;
    //for(i=0;i<99;i++)
//      printf("%f\n",((double *)(p->bptr))[i]);
   // unsigned short count=((double *)(p->bptr))[0]*100;
    //printf("wave7\n");
    int count= p->w_length;
    //printf("wave8\n");
    double test;
    //printf("%d,%d,%d\n",delay2,delay,&priv->cmd);
    memcpy(delay2,&delay,sizeof(unsigned int));
    //*delay2=delay;
    //printf("wave6\n");
    align[S+30]=p->w_no;
    align[S+31]=count*4-2;
    align[S+23]=count*4+16-2;
    priv->cmd[S+2]=count*4+16+53-2;
    //printf("wave3\n");
    for(i=33,j=1;j<=count;j++)
    {
        //test=((double *)p->bptr)[j-1];
        //printf("%f\n",test);
        k=((ushort *)p->bptr)[j];
        *(align+i)=k;
        *(align+i+1)=k;
        //printf("%02X %hu|",k,((ushort *)p->bptr)[j]);
        i+=2;
        //*(priv->cmd+i)=k;
        //i+=2;
    }
    //printf("wave2\n");
    i-=2;
    /*FILE *fp;
    fp=fopen("test.dat", "wb");
    if(fp == NULL){
        printf("写出文件出错");
        return;
    }
    */
    align[i]=0x0000;
    i++;
    align[i]=0xffff;
    i++;
    //unsigned int index=16;
    //unsigned char * cp=(unsigned char *)priv->cmd;
    /*for(k=0;k<i*2+1;k++)
    {
        //if(k>24&&k<i-3) fwrite(&(priv->cmd[k]),sizeof(short),1,fp);
        printf("%02X ",cp[k]);
        if((k+1)%16==0&&k!=0) 
        {
                printf("\n%8X:",index);
                index+=16;
        }
    }
    fclose(fp);
    printf("\ntotal:%d\n",i);*/
    //sendData(priv->cmd,i*2+1); 
    int len=i*2+1;
    i=len;
    void * start=(void *)priv->cmd;
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
    //printf("send:%d,%d\n",k,len);
    //close(sockfd);
    //return;
    unsigned char temp[20000];
    /*if(k=read(priv->socket, temp, 20000) < 0)
    {//接收服务器返回的消息
          printf("read message error");
    } 
    strcpy(p->state,RTN(temp[9]));  
    //printf("%s\n",RTN(temp[9]));
    */
    p->w_length=0;
    //printf("length:%d\n",p->w_length);
    //dbPutLink(&(priv->prec->inp), DBR_SHORT, priv->cmd, length+6);
}
