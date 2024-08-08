/*************************************************************************\
* The information from the device support what display to OPI
\*************************************************************************/



#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "alarm.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#include "menuYesNo.h"
#define GEN_SIZE_OFFSET
#include "InforminRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record(InforminRecord *, int);
static long process(InforminRecord *);
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
#define get_units NULL
#define get_precision NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double NULL

rset InforminRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};
epicsExportAddress(rset,InforminRSET);

struct Informindset { /* Informindset input dset */
	long		number;
	DEVSUPFUN	dev_report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_Informin; /*returns: (-1,0)=>(failure,success)*/
};
static void monitor(InforminRecord *);
static long readValue(InforminRecord *);


static long init_record(InforminRecord *prec, int pass)
{
    STATIC_ASSERT(sizeof(prec->oval)==sizeof(prec->val));
    struct Informindset *pdset;
    long status;

    if (pass==0) return(0);

    if (prec->siml.type == CONSTANT) {
	recGblInitConstantLink(&prec->siml,DBF_USHORT,&prec->simm);
    }

    /* Informindset.siol must be a CONSTANT or a PV_LINK or a DB_LINK */
    if (prec->siol.type == CONSTANT) {
        recGblInitConstantLink(&prec->siol,DBF_STRING,prec->sval);
    } 

    if(!(pdset = (struct Informindset *)(prec->dset))) {
	recGblRecordError(S_dev_noDSET,(void *)prec,"Informindset: init_record");
	return(S_dev_noDSET);
    }
    /* must have read_Informin function defined */
    if( (pdset->number < 5) || (pdset->read_Informin == NULL) ) {
	recGblRecordError(S_dev_missingSup,(void *)prec,"Informindset: init_record");
	return(S_dev_missingSup);
    }
    if( pdset->init_record ) {
	if((status=(*pdset->init_record)(prec))) return(status);
    }
    strcpy(prec->oval,prec->val);
    return(0);
}

/*
 */
static long process(InforminRecord *prec)
{
	struct Informindset	*pdset = (struct Informindset *)(prec->dset);
	long		 status;
	unsigned char    pact=prec->pact;

	if( (pdset==NULL) || (pdset->read_Informin==NULL) ) {
		prec->pact=TRUE;
		recGblRecordError(S_dev_missingSup,(void *)prec,"read_Informin");
		return(S_dev_missingSup);
	}

	status=readValue(prec); /* read the new value */
	/* check if device support set pact */
	if ( !pact && prec->pact ) return(0);
	prec->pact = TRUE;

	recGblGetTimeStamp(prec);

	/* check event list */
	monitor(prec);
	/* process the forward scan link record */
	recGblFwdLink(prec);

	prec->pact=FALSE;
	return(status);
}

static void monitor(InforminRecord *prec)
{
    unsigned short  monitor_mask;

    monitor_mask = recGblResetAlarms(prec);
    if(strcmp(prec->oval,prec->val)) {
	monitor_mask |= DBE_VALUE|DBE_LOG;
	strcpy(prec->oval,prec->val);
    }
    if (prec->mpst == InforminPOST_Always)
	monitor_mask |= DBE_VALUE;
    if (prec->apst == InforminPOST_Always)
	monitor_mask |= DBE_LOG;
    if(monitor_mask)
	db_post_events(prec,&(prec->val[0]),monitor_mask);
	db_post_events(prec, &prec->ready,monitor_mask);
    return;
}

static long readValue(InforminRecord *prec)
{
	long		status;
        struct Informindset 	*pdset = (struct Informindset *) (prec->dset);

	if (prec->pact == TRUE){
		status=(*pdset->read_Informin)(prec);
		return(status);
	}

	status=dbGetLink(&(prec->siml),DBR_USHORT, &(prec->simm),0,0);
	if (status)
		return(status);

	if (prec->simm == menuYesNoNO){
		status=(*pdset->read_Informin)(prec);
		return(status);
	}
	if (prec->simm == menuYesNoYES){
		status=dbGetLink(&(prec->siol),DBR_STRING,
			prec->sval,0,0);
		if (status==0) {
			strcpy(prec->val,prec->sval);
			prec->udf=FALSE;
		}
	} else {
		status=-1;
		recGblSetSevr(prec,SOFT_ALARM,INVALID_ALARM);
		return(status);
	}
        recGblSetSevr(prec,SIMM_ALARM,prec->sims);

	return(status);
}
