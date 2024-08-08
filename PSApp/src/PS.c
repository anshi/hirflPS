/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  PS.c
  *Author:  yunbin Zhou
  *Version:  1.0
  *Date:  2019.8.31
  *Description:  Ramping Power Supply IOC
  *Others:  
  *Function List:  
 
  *History: 
     1.Date:            2021-03-20
       Author:          An Shi    
       Modification:    1. Add current_wave (direct write the float data to wave file, do not
                            convert to 16 Bit DAC data.)
                        2. Fix some minor errors.
     
     1.Date:            2021-07-22
       Author:          An Shi    
       Modification:    1. Add support for triangle PS
                        
                        
**********************************************************************************/

#include "PS.h"
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>


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

int open_ps_server_socket(void *p)
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

int GetState( psRecord *ps, int state_num)
{
    //printf("get state\n");
    // struct InitConfig * config=ps->dpvt;
    // unsigned char cmd[14]={0xff,0xff,0x00,0x00,0x0e,0x00,0x04,0x01,0x2a,0xaa,0x00,0x00,0xff,0xff};
    // if(strcmp(ps->version,"3")==0) cmd[6]=0x03;
    // config->sendcmd=cmd;
    // config->length=0x12;
    // SendPS(config);
    // //strcpy(ps->state,RTN(config->receive[9]));
    // if(config->receive[9]!=0) 
    // {
    // printf("get state error\n");
    // return;
    // }
    // memcpy(&(ps->c_val), &(config->receive[15]), 4);
    // memcpy(&(ps->v_val), &(config->receive[19]), 4);
    //printf("\n");
    (state_num & 0x01)?strcpy(ps->psstate,"OFF"):strcpy(ps->psstate,"ON");//是否运行
    (state_num & 0x02)?strcat(ps->psstate,"|FAULT"):strcat(ps->psstate,"|NORMAL");//电源总故障
    // (state_num & 0x08)?strcat(ps->psstate,"|FUSE"):1;//熔断故障
    // (state_num & 0x10)?strcat(ps->psstate,"|LOAD"):1;//负载故障
    // (state_num & 0x20)?strcat(ps->psstate,"|POWER"):1;//功率器件故障
    // (state_num & 0x40)?strcat(ps->psstate,"|WATER"):1;//缺水故障
    // (state_num & 0x80)?strcat(ps->psstate,"|TEMP"):1;//过热故障
    // ((config->receive[12])&0x01)?strcat(ps->psstate,"|OC"):1;//过流故障
    // ((config->receive[12])&0x02)?strcat(ps->psstate,"|OV"):1;//过压故障
    // ((config->receive[12])&0x04)?strcat(ps->psstate,"|INOV"):1;//交流过压故障
    // ((config->receive[12])&0x08)?strcat(ps->psstate,"|INLV"):1;//交流欠压故障
    // ((config->receive[12])&0x10)?strcat(ps->psstate,"|SW"):1;//开关故障
    // ((config->receive[12])&0x40)?strcat(ps->psstate,"|EMEGRGY"):1;//急停状态
    (state_num & 0x40)?strcat(ps->psstate,"|RAMPING"):strcat(ps->psstate,"|DC");//脉冲，直流
    // ((config->receive[13])&0x40)?strcat(ps->psstate,"|D_LOCK"):1;//门开
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
    char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    unsigned int * delay2=((void *)(&(priv->cmd[S+11]))) + 1;
    //printf("start write\n");
    priv->cmd[S+0]=0xffff;
    priv->cmd[S+1]=0x0000;

    priv->cmd[S+2]=0x2ee5;//data length
    if(strcmp(p->version,"3")==0)
        priv->cmd[S+3]=0x0103;//version
    else
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
    // align[S+31]=count*4;
    // align[S+23]=count*4+16;
    // priv->cmd[S+2]=count*4+16+55;

    char * p_buf = (void *)priv->cmd;
    short total_len = 0;
    total_len = (count * 4 + DIGITAL_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));

    int single_wave_len = count * 4;
    memcpy(p_buf + DIGITAL_PS_SINGLE_WAVE_LEN_OFFSET, &single_wave_len, sizeof(int));
    
    int total_wave_len = count * 4 + 16;
    memcpy(p_buf + DIGITAL_PS_TOTAL_WAVE_LEN_OFFSET, &total_wave_len, sizeof(int));
    //printf("wave3\n");
    for(i=33,j=0;j<count;j++)
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
    // i-=2;
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

    // // write the send buffer to file for debug.
    // FILE *fp;
    // fp=fopen("test_bump_nor.dat", "wb");
    // if(fp == NULL){
    //     printf("open write file error.");
    //     // return;
    // }
    // else
    // {
    //     fwrite(priv->cmd, sizeof(char), len, fp);
    //     fclose(fp);
    // }

    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        // else k=write(priv->socket, start, i+10);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        //sprintf(log_info, "%s send wave data error", p->name);
        //syslog(LOG_INFO, log_info);
        start+=k;
        i=i-k;
        send_len += k;
    }
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
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

#if 0
int WriteExtBumpWave(psRecord *p)
{
    char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    
    char * p_buf = (void *)priv->cmd;
    FILE * template_file;
    
    template_file = fopen("ext_bump_temple","rb");
    if(template_file == NULL)
    {
        printf("open template file failed");
        sprintf(log_info, "%s open template file error", p->name);
        syslog(LOG_ERR, log_info);
        return 0;
    }
    fread(priv->cmd, sizeof(unsigned char), DIGITAL_PS_HEAD_LEN, template_file);
    fclose(template_file);

    int i,j;
    unsigned short k;
    float f_val;
    int count= p->w_length;
    int va_num = p->w_no;
    short total_len = 0;
    
    memcpy(p_buf + DIGITAL_PS_DELAY_OFFSET, &delay, sizeof(int));
    memcpy(p_buf + DIGITAL_PS_DELAY2_OFFSET, &delay, sizeof(int));
    memcpy(p_buf + DIGITAL_PS_VA_NUM_OFFSET, &va_num, sizeof(int));
    
    int single_wave_len = (count + 10) * 4;
    memcpy(p_buf + DIGITAL_PS_SINGLE_WAVE_LEN_OFFSET, &single_wave_len, sizeof(int));
    
    int total_wave_len = (count + 10) * 4 + 16;
    memcpy(p_buf + DIGITAL_PS_TOTAL_WAVE_LEN_OFFSET, &total_wave_len, sizeof(int));
    
    // data end is 0x0000FFFF, 4 byte.
    // Total data size (byte) is result of true size number mod 65535.
    total_len = ((count + 10) * 4 + DIGITAL_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));
    
    int val = 0;
    for(i = DIGITAL_PS_HEAD_LEN, j=0; j<count; j++)
    {
        f_val = ((float *)p->bptr)[j];
        /* just for old ext bump PS 
        k = convert_to_dac(f_val, 600, 0, 1);
        val = k;
        memcpy(p_buf + i, &val, sizeof(int));       // Every point will ocupied 4 bytes.
        */
        memcpy(p_buf + i, &f_val, sizeof(float));   // Every point will ocupied 4 bytes.
        i += 4;
    }

    // Add 10 additional parameters to ramping data
    int i_val;
    //j++;
    //i -= 2;
    printf("debug:final j = %d\n", j);
    for (k = 0; k < 10; k++)
    {
        f_val = ((float *)p->bptr)[j];
        if ((k == 3) || (k == 5) || (k == 6))
            memcpy(p_buf + i, &f_val, 4);
        else
        {
            i_val = (int)(f_val);
            memcpy(p_buf + i, &i_val, 4);
        }
        j++;
        i += 4;
    }
    
    //i-=2;
    p_buf[i] = 0x00;
    p_buf[i + 1] = 0x00;
    i += 2;
    p_buf[i] = 0xff;
    p_buf[i + 1] = 0xff;
    i += 2;

    // int len=i*2+1;
    // i=len;
    int len = i;
    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_bump.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(priv->cmd, sizeof(char), len, fp);
        fclose(fp);
    }
    
    
    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
        send_len += k;
    }
    
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
    // unsigned char temp[20000];
    p->w_length=0;
}
#endif

int WriteExtBumpWave(psRecord *p)
{
    char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    
    char * p_buf = (void *)priv->cmd;
    FILE * template_file;
    
    // template_file = fopen("ext_bump_temple","rb");
    // if(template_file == NULL)
    // {
    //     printf("open template file failed");
    //     sprintf(log_info, "%s open template file error", p->name);
    //     syslog(LOG_ERR, log_info);
    //     return 0;
    // }
    // fread(priv->cmd, sizeof(unsigned char), DIGITAL_PS_HEAD_LEN, template_file);
    // fclose(template_file);
    
    template_file = fopen("ext_bump_templet_2evt","rb");        // two events 
    if(template_file == NULL)
    {
        printf("open template file failed");
        sprintf(log_info, "%s open template file failed", p->name);
        syslog(LOG_ERR, log_info);
        return 0;
    }
    fread(priv->cmd, sizeof(unsigned char), DIGITAL_TRIG_PS_HEAD_LEN, template_file);
    fclose(template_file);
    
	unsigned short * align=((void *)(p_buf))+1;
    int i,j;
    unsigned short k;
    float f_val = 0;
	// int w_length = 0;
	char tmp_buf[32];
    int va_num = p->w_no;

    memcpy(p_buf + 0x1F, &delay, sizeof(int));          // event delay
    memcpy(p_buf + 0x33, &va_num, sizeof(int));         // wave number
	
    
    int count= p->w_length;
    short total_len = 0;
    
    // memcpy(p_buf + DIGITAL_PS_DELAY_OFFSET, &delay, sizeof(int));
    // memcpy(p_buf + DIGITAL_PS_DELAY2_OFFSET, &delay, sizeof(int));
    // memcpy(p_buf + DIGITAL_PS_VA_NUM_OFFSET, &va_num, sizeof(int));
    
    // align[S+19] = (count + 10) * 4+16;
	// align[S+27] = (count + 10) * 4;
    int single_wave_len = (count + 10) * 4;
    int total_wave_len = (count + 10) * 4 + 16;
    memcpy(p_buf + 0x27, &total_wave_len, 4);
	memcpy(p_buf + 0x37, &single_wave_len, 4);
    // int single_wave_len = (count + 10) * 4;
    // memcpy(p_buf + DIGITAL_PS_SINGLE_WAVE_LEN_OFFSET, &single_wave_len, sizeof(int));
    
    // int total_wave_len = (count + 10) * 4 + 16;
    // memcpy(p_buf + DIGITAL_PS_TOTAL_WAVE_LEN_OFFSET, &total_wave_len, sizeof(int));
    
    // data end is 0x0000FFFF, 4 byte.
    // Total data size (byte) is result of true size number mod 65535.
    // total_len = ((count + 10) * 4 + DIGITAL_PS_HEAD_LEN + 4) % 65535;
    total_len = ((count + 10) * 4 + DIGITAL_TRIG_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));
    
    int val = 0;
    for(i = 29, j=0; j<count; j++)
    {
        f_val = ((float *)p->bptr)[j];
        // short short_val = (short)((f_val / 600) * 65535);
        
        memcpy(align + i, &f_val, 4);                   // write float value
        // memcpy(align + i, &short_val, 2);            // write short value
        i += 2;
    }

    // Add 10 additional parameters to ramping data
    int i_val;
    printf("debug:final j = %d\n", j);
    for (k = 0; k < 10; k++)
    {
        f_val = ((float *)p->bptr)[j];
        if ((k == 3) || (k == 5) || (k == 6))
            memcpy(align + i, &f_val, 4);
        else
        {
            if (k == 1)
            {
                i_val = (int)((f_val/600) * 65535);
            }
            else
                i_val = (int)(f_val);
            memcpy(align + i, &i_val, 4);
        }
        j++;
        i += 2;
    }
    
    //i-=2;
    // p_buf[i] = 0x00;
    // p_buf[i + 1] = 0x00;
    // i += 2;
    // p_buf[i] = 0xff;
    // p_buf[i + 1] = 0xff;
    // i += 2;
    align[i]=0x0000;
    i++;
    align[i]=0xffff;
    i++;
    int len = i * 2 + 1;
    i = len;

    // int len=i*2+1;
    // i=len;
    // int len = i;
    // write the send buffer to file for debug.
    // char seps[]   = ":";
    // char *pos = NULL;
    // char temp_string[32] = {0,};

    // strncpy(temp_string, p->name, 13);

    // //get the PS name
    // pos = strtok( temp_string, seps ); 
    // pos = strtok( NULL, seps);
    // if (pos != NULL)
    char f_name[64] = {0,};
    if (strncmp(p->name, "RING:PS4BPE01", 13) == 0)
    {
        strcpy(f_name, "dump_wave_01.bin");
    }

    if (strncmp(p->name, "RING:PS4BPE02", 13) == 0)
    {
        strcpy(f_name, "dump_wave_02.bin");
    }

    if (strncmp(p->name, "RING:PS4BPE03", 13) == 0)
    {
        strcpy(f_name, "dump_wave_03.bin");
    }

    
    FILE *fp;
    
    fp=fopen(f_name, "wb");
    // fp=fopen("dump_bump3.bin", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(priv->cmd, sizeof(char), len, fp);
        fclose(fp);
    }
    
    
    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
        send_len += k;
    }
    
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
    // unsigned char temp[20000];
    p->w_length=0;
}

int WriteCurrentWave(psRecord *p)
{
    char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    
    char * p_buf = (void *)priv->cmd;
    FILE * template_file;
    
    if (strncmp(p->name, "RING:PS4BPI", 11) == 0)
    {
        // template_file = fopen("inj_bump_templet","rb");
        WriteCurrentInjBumpWave(p);
        return;
    }
    else
    {
        template_file = fopen("general_templet","rb");
    }
    
    if(template_file == NULL)
    {
        printf("open template file failed");
        sprintf(log_info, "%s open template file error", p->name);
        syslog(LOG_ERR, log_info);
        return 0;
    }
    fread(priv->cmd, sizeof(unsigned char), DIGITAL_PS_HEAD_LEN, template_file);
    fclose(template_file);

    int i,j;
    unsigned short k;
    float f_val;
    int count= p->w_length;
    int va_num = p->w_no;
    short total_len = 0;
    
    memcpy(p_buf + DIGITAL_PS_DELAY_OFFSET, &delay, sizeof(unsigned int));
    memcpy(p_buf + DIGITAL_PS_VA_NUM_OFFSET, &va_num, sizeof(int));
    
    int single_wave_len = count * 4;
    memcpy(p_buf + DIGITAL_PS_SINGLE_WAVE_LEN_OFFSET, &single_wave_len, sizeof(int));
    
    int total_wave_len = count * 4 + 16;
    memcpy(p_buf + DIGITAL_PS_TOTAL_WAVE_LEN_OFFSET, &total_wave_len, sizeof(int));
    
    // data end is 0x0000FFFF, 4 byte.
    // Total data size (byte) is result of true size number mod 65535.
    total_len = (count * 4 + DIGITAL_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));
    
    for(i = DIGITAL_PS_HEAD_LEN, j=0; j<count; j++)
    {
        f_val = ((float *)p->bptr)[j];
        memcpy(p_buf + i, &f_val, sizeof(float));   // Every point will ocupied 4 bytes.
        i += 4;
    }

    p_buf[i] = 0x00;
    p_buf[i + 1] = 0x00;
    i += 2;
    p_buf[i] = 0xff;
    p_buf[i + 1] = 0xff;
    i += 2;

    // int len=i*2+1;
    // i=len;
    int len = i;
    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_current.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(priv->cmd, sizeof(char), len, fp);
        fclose(fp);
    }
    
    
    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
        send_len += k;
    }
    
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
    // unsigned char temp[20000];
    p->w_length=0;
}


#if 0
int WriteCurrentWave(psRecord *p)
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
    float f_val = 0;
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
        f_val = ((float *)p->bptr)[j];
        memcpy(align + i, &f_val, 4);
        //printf("%f\n",test);
        // k=((ushort *)p->bptr)[j];
        // *(align+i)=k;
        // *(align+i+1)=k;
        //printf("%02X %hu|",k,((ushort *)p->bptr)[j]);
        i+=2;
        //*(priv->cmd+i)=k;
        //i+=2;
    }
    //printf("wave2\n");
    i-=2;
        
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

    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_current.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(priv->cmd, sizeof(char), len, fp);
        fclose(fp);
    }

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
#endif

int WriteAnalogWave(struct psRecord *p)
{
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    char log_info[256] = {0,};
    unsigned short * align=((void *)(priv->cmd))+1;
    char * p_address = (void *)(priv->cmd);
    char event_table[64] = {0,};
    FILE * myfile;
    int ps_type = 0;
    unsigned int delay = p->delay;

    if (strncmp(p->name + 7, "BUMP", 4) == 0)
    {
        ps_type = 1;
        if ((strncmp(p->name + 7, "BUMP3", 5) == 0) || (strncmp(p->name + 7, "BUMP2", 5) == 0))      // Add c0080001 event code to bump3
            ps_type = 2;
    }
    else
        ps_type = 0;

    // if (strncmp(p->name + 7, "BUMP", 4) == 0)
    if (ps_type)
        myfile=fopen("/opt/epics/CSR_PS/iocBoot/iocPS/template_bump","rb");
    else
        myfile=fopen("/opt/epics/CSR_PS/iocBoot/iocPS/template","rb");
    
    if(myfile==NULL)
    {
        printf("open failed\n");
        return 0;
    }
    // fread(priv->cmd, sizeof(unsigned char), 0x56,myfile);
    // Get the head of protocol for analog PS controllor 
    fread(priv->cmd, sizeof(unsigned char), ANALOG_PS_HEAD_LEN, myfile);
    fclose(myfile);
    // Generate the event table for analog PS controllor
    generate_event_table(event_table, delay, ps_type);
    int event_len = strlen(event_table);
    // event table length
    priv->cmd[16] = event_len;
    //waveform data length
    priv->cmd[18] = p->w_length*4+8;
    // write event table to buffer.
    p_address += ANALOG_PS_EVENT_OFFSET;
    memcpy(p_address, event_table, event_len);
    // write the number of waveform points.
    int wave_point = 0;
    //printf("debug: wave_len is %d \n", p->w_length);
    wave_point = p->w_length;
    //printf("debug: short len is %d \n", wave_len);
    // Notice: VA No. at address ANALOG_PS_EVENT_OFFSET + event_len.
    p_address = p_address + event_len + 4;
    // memcpy(p_address + 82, &wave_len, 4);
    memcpy(p_address, &wave_point, 4);
    p_address += 4;
    int i;
    unsigned short k;
    for(i = 0; i < wave_point; i++)
    {
        k=((ushort *)p->bptr)[i];
        memcpy(p_address, &k, 2);
        memcpy(p_address + 2, &k, 2);
        p_address += 4;
        //printf("%02X %hu|",k,((ushort *)p->bptr)[j]);
        //i+=2;
    }

    // the number of event table length occupied 4 byte
    // the number of waveform data length occupied 4 byte
    // the number of VA No. occupied 4 byte
    // the number of waveform points occupied 4 byte
    int buffer_len = ANALOG_PS_HEAD_LEN + event_len + wave_point * 4 + 16;
#if 0
    int i,j;
    unsigned short k;
    //for(i=0;i<99;i++)
    //printf("%f\n",((double *)(p->bptr))[i]);
    // unsigned short count=((double *)(p->bptr))[0]*100;
    //printf("wave7\n");
    int count= p->w_length;
    //printf("wave8\n");
    double test;
    //printf("%d,%d,%d\n",delay2,delay,&priv->cmd);
    //*delay2=delay;
    //printf("wave6\n");
    //printf("wave3\n");
    for(i=43,j=0;j<count;j++)
    {
        k=((ushort *)p->bptr)[j];
        *(align+i)=k;
        *(align+i+1)=k;
        //printf("%02X %hu|",k,((ushort *)p->bptr)[j]);
        i+=2;
    }
#endif
    //printf("wave2\n");
    /*FILE *fp;
    fp=fopen("test.dat", "wb");
    if(fp == NULL){
        printf("写出文件出错");
        return;
    }
    */
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
    
    //int len=i*2+1;
    
#if 0
    // write the send buffer to file for debug.
    FILE *fp;
    if (strcmp(p->name, "CSRM:PS20D:VOL") == 0 )
        fp=fopen("test_v.dat", "wb");
    else if (strcmp(p->name, "CSRM:PS20D:CUR") == 0 )
        fp=fopen("tes_c.dat", "wb");
    else
        fp=fopen("test_bump.dat", "wb");

    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(priv->cmd, sizeof(char), buffer_len, fp);
        fclose(fp);
    }
#endif
    i = buffer_len;
    void * start=(void *)priv->cmd;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i+10);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
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


int WriteCurrentTrigWave(struct psRecord *p)
{
	char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    
    char * p_buf = (void *)priv->cmd;

	FILE * template_file;
    template_file = fopen("20A100Hz","rb");
    if(template_file == NULL)
    {
        printf("open template file failed");
        return 0;
    }
    fread(priv->cmd, sizeof(unsigned char), DIGITAL_TRIG_PS_HEAD_LEN, template_file);
    fclose(template_file);
    // fread(buf, sizeof(unsigned char), 0x3b, template_file);
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
	// fgets(tmp_buf, sizeof(tmp_buf), myfile);
	// w_length = atoi(tmp_buf);
    int count = p->w_length;
	
    short total_len = 0;
    total_len = (count * 4 + DIGITAL_TRIG_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));
	// p_buf[S+4] = count*4+ DIGITAL_TRIG_PS_HEAD_LEN + 4;
	// align[S+19] = count*4+16;
	// align[S+27] = count*4;
    int single_wave_len = count * 4;
    int total_wave_len = count * 4 + 16;
    memcpy(p_buf + 0x27, &total_wave_len, 4);
	memcpy(p_buf + 0x37, &single_wave_len, 4);
	
    for(i=29,j=0;j<count;j++)
    {
        f_val = ((float *)p->bptr)[j];
        memcpy(align + i, &f_val, 4);
        i+=2;
    }
    
    align[i]=0x0000;
    i++;
    align[i]=0xffff;
    i++;
    int len=i*2+1;
    i=len;

    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_current_trig.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(p_buf, sizeof(char), len, fp);
        fclose(fp);
    }

    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
        send_len += k;
    }
    
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
    p->w_length=0;
}


int WriteCurrentInjBumpWave(struct psRecord *p)
{
	char log_info[256] = {0,};
    struct InitConfig * priv=p->dpvt;
    bzero(priv->cmd,WAVE_LENGTH);
    unsigned int delay=p->delay*25;
    
    char * p_buf = (void *)priv->cmd;

	FILE * template_file;
    template_file = fopen("inj_bump_templet_2evt","rb");
    if(template_file == NULL)
    {
        printf("open template file failed");
        sprintf(log_info, "%s open template file failed", p->name);
        syslog(LOG_ERR, log_info);
        return 0;
    }
    fread(priv->cmd, sizeof(unsigned char), DIGITAL_TRIG_PS_HEAD_LEN, template_file);
    fclose(template_file);

	unsigned short * align=((void *)(p_buf))+1;
    int i,j;
    unsigned short k;
    float f_val = 0;
	// int w_length = 0;
	char tmp_buf[32];
    int va_num = p->w_no;

    memcpy(p_buf + 0x1F, &delay, sizeof(int));          // event delay
    memcpy(p_buf + 0x33, &va_num, sizeof(int));         // wave number
	
    int count = p->w_length;
	
    short total_len = 0;
    total_len = (count * 4 + DIGITAL_TRIG_PS_HEAD_LEN + 4) % 65535;
    memcpy(p_buf + DIGITAL_PS_TOTAL_LEN_OFFSET, &total_len, sizeof(short));
	// p_buf[S+4] = count*4+ DIGITAL_TRIG_PS_HEAD_LEN + 4;
	// align[S+19] = count*4+16;
	// align[S+27] = count*4;
    int single_wave_len = count * 4;
    int total_wave_len = count * 4 + 16;
    memcpy(p_buf + 0x27, &total_wave_len, 4);
	memcpy(p_buf + 0x37, &single_wave_len, 4);

    for(i=29,j=0;j<count;j++)
    {
        f_val = ((float *)p->bptr)[j];
        memcpy(align + i, &f_val, 4);
        i+=2;
    }
    
    align[i]=0x0000;
    i++;
    align[i]=0xffff;
    i++;
    int len=i*2+1;
    i=len;

    // write the send buffer to file for debug.
    FILE *fp;
    fp=fopen("test_current_inj_bump.dat", "wb");
    if(fp == NULL){
        printf("open write file error.");
        // return;
    }
    else
    {
        fwrite(p_buf, sizeof(char), len, fp);
        fclose(fp);
    }

    void * start=(void *)priv->cmd;
    int send_len = 0;
    for(k=0;i>0;)
    {
        if(10000<i) k=write(priv->socket, start, 10000);
        else k=write(priv->socket, start, i);
        if(k<0)
        {//发送消息到服务器
            printf("write message error!");
            sprintf(log_info, "%s send wave data error", p->name);
            syslog(LOG_ERR, log_info);
            break;
        }
        printf("have send:%d\n",k);
        start+=k;
        i=i-k;
        send_len += k;
    }
    
    sprintf(log_info, "%s total send %d byte wave data", p->name, send_len);
    syslog(LOG_INFO, log_info);
    p->w_length=0;
}


// Generate a event table for analog PS controllor. type = 0 is general, otherwise is BUMP PS
int generate_event_table(char * event_table, unsigned int delay, int type)
{
    if (event_table == NULL)
        return 0;
    
    char temp_str[16] = {0,}; 
    int delay_number = delay * 100;  //convert 1us delay to 10ns
    sprintf(temp_str, "%x\r\n", delay_number);

    if (type == 0)
        strcpy(event_table, ps_q_d_event);
    if (type == 1)
        strcpy(event_table, ps_bump_event);
    if (type == 2)
        strcpy(event_table, ps_bump3_event);
    
    strcat(event_table, temp_str);
    strcat(event_table, pulse_width);
    return 1;
}


unsigned short convert_to_dac(float real_val, float max_val, float abb_a, float abb_b)
{
    unsigned short dac_num = 0;

    dac_num = (unsigned short)(((real_val * abb_b + abb_a)/max_val) * 65535);
    return dac_num;
}
