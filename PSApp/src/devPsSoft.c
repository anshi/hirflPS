/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  devPsSoft.c
  *Author:  yunbin Zhou
  *Version:  1.0
  *Date:  2019.8.31
  *Description:  电源控制系统头文件
  *Others:  
  *Function List:  
 
  *History: 
    1.Date: 2019.12.8
       Author: anshi
       Modification: Add new mode (direct write current wave to PS) for test.
        
**********************************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>

#include "PS.h"
#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "psRecord.h"
#include "epicsExport.h"
#include "ellLib.h"
#include "dbScan.h"
#include "cantProceed.h"
#include "epicsThread.h"
#include "epicsMutex.h"
#include "initHooks.h"
#include "cadef.h"
/*Create the dset for devXxxSoft */
static long init_record();
static long read_ps();
static void start_workers(initHookState state);
static long init();
static long get_ioint_info();
static void worker();
static int ca_get_ps_state(struct psRecord *pps);
static int ca_get_ps_current(struct psRecord *pps);
static int ca_put_ps_current(struct psRecord *pps);
static int ca_put_ps_command(struct psRecord *pps);
static long readLocked(struct link *pinp, void *vvt);

struct {
        long		number;
        DEVSUPFUN	report;
        DEVSUPFUN	init;
        DEVSUPFUN	init_record;
        DEVSUPFUN	get_ioint_info;
        DEVSUPFUN	read_ps;
}devPsSoft={
        5,
        NULL,
        init,
        init_record,
        get_ioint_info,
        read_ps,
};
epicsExportAddress(dset,devPsSoft);
static ELLLIST allprngs = ELLLIST_INIT;

static long init(int phase)
{
        if(phase==0)
                initHookRegister(&start_workers);
        return 0;
}

struct aivt {
    unsigned long val;
    epicsTimeStamp *ptime;
};


static long init_record(struct psRecord *pps)
{
    if(recGblInitConstantLink(&pps->inp,DBF_DOUBLE,&pps->val))
         pps->udf = FALSE;
    struct InitConfig *config=(struct InitConfig *)callocMustSucceed(1,sizeof(struct InitConfig),"config");
    strcpy(config->domain,pps->domain);
    config->port=pps->port;
    scanIoInit(&(config->scan));
    config->lock=epicsMutexMustCreate();
    config->generator=NULL;
    ellAdd(&allprngs,&config->node);
    pps->dpvt = config;
    config->prec=pps;
    config->cmd=malloc(WAVE_LENGTH*sizeof(int));
    if(config->cmd==NULL) printf("malloc failed\n");
    
    /*config->socket=open_ps_socket(config);
    if(config->socket>0)
    {
        printf("%s connected\n",config->domain);
        return 0;
    }
    else
    {
        printf("%s connect failed\n",config->domain);
        return 2;
    }*/
    return 0;
}

static void start_workers(initHookState state)
{
        ELLNODE *cur;
        if(state!=initHookAfterInterruptAccept)
                return;
        for(cur=ellFirst(&allprngs); cur; cur=ellNext(cur))
        {
                struct InitConfig *priv = CONTAINER(cur, struct InitConfig, node);
                priv->generator = epicsThreadMustCreate("prngworker",epicsThreadPriorityMedium,epicsThreadGetStackSize(epicsThreadStackSmall),&worker, priv);
        }
}

static void worker(void* raw)
{
        struct InitConfig *config=raw;
        struct psRecord * rec=config->prec;
        strcpy(rec->state,"RUNNING|RAMP|OK");
        strcpy(rec->psstate,"RUNNING|RAMP|OK");
        char log_info[200] = {0,};
        sprintf(log_info, "%s start a worker", config->domain);
        syslog(LOG_INFO, log_info);
        // SEVCHK(ca_context_create(ca_disable_preemptive_callback),"ca_context_create");
        while(1)
        {
                //printf("%s is working\n",config->domain);
                /*if(SocketConnected(config->socket)==0)
                {
                        close(config->socket);
                        printf("try to reopen socket after 10 second, ps %s\n",config->domain);
                        epicsThreadSleep(10.0);
                        config->socket=open_ps_socket(config);
                        continue;
                }*/
                //int state=GenerateCMD(rec);
                //SetCurrent(rec);
                // ca_put_ps_current(rec);
                // ca_get_ps_current(rec);
                // ca_get_ps_state(rec);
                int state = ca_get_ps_state(rec);
                GetState(rec, state);
                ca_put_ps_command(rec);
                if(rec->w_length!=0) 
                {
                    sprintf(log_info, "%s get a wave data", rec->name);
                    syslog(LOG_INFO, log_info);
                    config->socket=open_ps_server_socket(config);  
                    if(SocketConnected(config->socket)==0) 
                    {
                            printf("%s,connect error\n",config->domain);
                            sprintf(log_info, "%s connect to % error", rec->name, config->domain);
                            syslog(LOG_ERR, log_info);
                            close(config->socket);
                            rec->w_length = 0;
                            epicsThreadSleep(1.0);
                            continue;
                    }
                    switch (rec->dtype)
                    {
                            case 0:
                                    WriteWave(rec);             //General Digital PS, injection Digital Bump PS, etc. 
                                    break;
                            case 1:
                                    WriteExtBumpWave(rec);
                                    break;
                            case 2:
                                    WriteCurrentWave(rec);
                                    break;
                            case 3:                             // Triangle Digital PS 
                                    WriteCurrentTrigWave(rec);
                                    break;
                            default:                            // in the HIRFL, must modify dtype to 4
                                    WriteAnalogWave(rec);
                    }
                    close(config->socket);
                        
                }
                scanIoRequest(config->scan);
                epicsThreadSleep(1.0);
        }
}


static long get_ioint_info(int dir,dbCommon* prec,IOSCANPVT* io)
{
        //printf("get_ioint\n");
        struct InitConfig * priv=prec->dpvt;
        *io = priv->scan;
        return 0;
}

static long read_ps(struct psRecord *pps)
{
    long status;

    status = dbGetLink(&(pps->inp),DBF_DOUBLE, &(pps->val),0,0);
    /*If return was succesful then set undefined false*/
    if(!status) pps->udf = FALSE;
    return(0);
}

static int ca_get_ps_state(struct psRecord *pps)
{
    long status;
    struct aivt vt;
    unsigned long ret = 0;
    
    vt.ptime = (dbLinkIsConstant(&pps->tsel) &&
        pps->tse == epicsTimeEventDeviceTime) ? &pps->time : NULL;
    status = dbLinkDoLocked(&pps->inp, readLocked, &vt);
    if (status == S_db_noLSET)
        status = readLocked(&pps->inp, &vt);

    if (!status) {
            ret = vt.val;
        }
    
    pps->udf = FALSE;
    return ret;
//     data = dbGetLink(&(pps->inp),DBR_ULONG, &data, 0, 0);
//     ret = data;
//     printf("debug: get the state of the power %d\n", ret);
//     return ret;
    // printf("debug: get state from PS: %f\n", data);
}

static int ca_get_ps_current(struct psRecord *pps)
{
    chid        mychid;
    double      data;
    
    char dev_pv_name[256] = {0,};
    strcpy(dev_pv_name, pps->name);
    strcat(dev_pv_name, ":AI");

    SEVCHK(ca_create_channel(dev_pv_name, NULL,NULL, 10, &mychid),"ca_create_channel failure");
    SEVCHK(ca_pend_io(2.0),"ca_pend_io failure");
    SEVCHK(ca_get(DBR_DOUBLE,mychid,(void *)&data),"ca_get failure");
    SEVCHK(ca_pend_io(2.0),"ca_pend_io failure");
//     ca_clear_channel(mychid);
    pps->c_val = data;
    return 0;
    // printf("debug: get state from PS: %f\n", data);
}

static int ca_put_ps_current(struct psRecord *pps)
{
    chid        mychid;
    float      data;
    
    struct InitConfig * config = pps->dpvt;
    
    if(config->current_val != pps->current_val)
    {
        data = pps->current_val;
        config->current_val = pps->current_val;
        char dev_pv_name[256] = {0,};
        strcpy(dev_pv_name, pps->name);
        strcat(dev_pv_name, ":AI.CURRENT_VAL");

        SEVCHK(ca_create_channel(dev_pv_name, NULL,NULL, 10, &mychid),"ca_create_channel failure");
        SEVCHK(ca_pend_io(1.0),"ca_pend_io failure");
        SEVCHK(ca_put(DBR_FLOAT, mychid, (void *)&data),"ca_put failure");
        SEVCHK(ca_pend_io(1.0),"ca_pend_io failure");
        ca_clear_channel(mychid);
    }
    return 0;
}

static int ca_put_ps_command(struct psRecord *pps)
{
    chid        mychid;
    char        data[4] = {0,};
    
    struct InitConfig * config = pps->dpvt;
    
    // if(strcmp(config->command, pps->command) != 0)
    if (strlen(pps->command) != 0)
    {
        strcpy(config->command,pps->command);
        memset(pps->command, '\0', sizeof(pps->command));
        if(strcmp(config->command,"ON")==0)
        {
            strcpy(data, "A7");
        }
        if(strcmp(config->command,"OFF")==0)
        {
            strcpy(data, "A8");    
        }
        if(strcmp(config->command,"RESET")==0)
        {
            strcpy(data, "A9");
        }
        if(strcmp(config->command,"SWITCH")==0)
        {
            strcpy(data, "B6");
        }
        if( (strcmp(config->command,"START")==0) || (strcmp(config->command,"STOP")==0) ) 
        {
            strcpy(data, config->command);
        }
        

        dbPutLink(&pps->out, DBR_STRING, &data, strlen(data)); //send a command to operator PV
        
        // char dev_pv_name[256] = {0,};
        // strcpy(dev_pv_name, pps->name);
        // strcat(dev_pv_name, ":AI.COMMAND");

        // SEVCHK(ca_create_channel(dev_pv_name, NULL,NULL, 10, &mychid),"ca_create_channel failure");
        // SEVCHK(ca_pend_io(1.0),"ca_pend_io failure");
        // SEVCHK(ca_put(DBR_STRING, mychid, (void *)&data),"ca_put failure");
        // SEVCHK(ca_pend_io(1.0),"ca_pend_io failure");
        // ca_clear_channel(mychid);
    }
    return 0;
}

static long readLocked(struct link *pinp, void *vvt)
{
    struct aivt *pvt = (struct aivt *) vvt;
    unsigned long data;
    
    long status = dbGetLink( pinp, DBR_ULONG, &data, 0, 0);
    if (!status )
    {
        #if 0
        printf("Debug: Get [%d] from device\n", nRequest);
        // For debug
        int i = 0;
        for (i = 0; i < nRequest; i++)
        {
            /* code */
            printf(" %f ", channel_array[i]);
        }
        printf("\n");
        printf("My Channel is %d\n",pvt->ch_id);
        #endif
        // printf("Debug: Get [%d] from device\n", data);
        pvt->val = data;
    }
    // long status = dbGetLink(pinp, DBR_DOUBLE, &pvt->val, 0, 0);

    return status;
}

