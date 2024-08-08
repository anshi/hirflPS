/*************************************************************************\
*   get the error code from the digital PS device support and convert to 
*           information message to display at OPI
\*************************************************************************/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "epicsTime.h"
#include "recGbl.h"
#include "devSup.h"
#include "link.h"
#include "InforminRecord.h"
#include "epicsExport.h"
#include <DigitalPSOperationCode.h>

/* Create the dset for devDigitalPSRampInform */
static long init_record(InforminRecord *prec);
static long read_Informin(InforminRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_Informin;
} devDigitalPSRampInform = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_Informin
};
epicsExportAddress(dset, devDigitalPSRampInform);

static long init_record(InforminRecord *prec)
{
    /* INP must be CONSTANT, PV_LINK, DB_LINK or CA_LINK*/
    prec->informcode = -1; //init to no message
    return 0;
}

static long read_Informin(InforminRecord *prec)
{
    long status;

    switch(prec->informcode)
    {
        case OP_SUCCESS:
            strcpy(prec->val, "Operations successful be done, Ready!");
            prec->ready = 1; //ready to run
            break;
        case RECV_DATA:
            strcpy(prec->val, "Receive DB data error check the IOC");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case CREATE_SEND_SOCKET:
            strcpy(prec->val, "Create socket error check the IOC");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case SET_SEND_SOCKET:
            strcpy(prec->val, "Set socket error check the IOC");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case CONNECT_PS:
            strcpy(prec->val, "Connect to PS error check the PS");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case SEND_TO_PS:
            strcpy(prec->val, "Send data error check the PS");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case RECV_RESP_PS:
            strcpy(prec->val, "Receive reponse error check the PS");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case RESP_PS:
            strcpy(prec->val, "Response data error check the PS");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case DATA_PS:
            strcpy(prec->val, "Ramping data error check the data");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case ORACLE_DATA:
            strcpy(prec->val, "Oracle:data error check the data and DB");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case OTHERS:
            strcpy(prec->val, "Others errors");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        case PS_CONTROLLER:
            strcpy(prec->val, "PS Controller disconnected");
            prec->ready = 0; //warning operator to check and fix the error
            break;
        default:
            strcpy(prec->val, "Unkonwn errors");
            prec->ready = 0; //warning operator to check and fix the error
            break;

    }
    
    return status;
}
