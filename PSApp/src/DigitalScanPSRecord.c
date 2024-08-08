/*************************************************************************\
* Copyright (c) 2013 IMP, CAS
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*DigitalScanPSRecord.c*/
/* DigitalScanPSRecord.c - Record Support Routines for Digital DC PS */
/*
 *      Original Author: Dave Barker
 *
 *      C  E  B  A  F
 *     
 *      Continuous Electron Beam Accelerator Facility
 *      Newport News, Virginia, USA.
 *
 *      Copyright SURA CEBAF 1993.
 *
 *      Current Author: An Shi
 *      Date:            4-JAN-2013
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "cvtTable.h"
#include "epicsPrint.h"
#include "epicsMath.h"
#include "epicsString.h"
#include "alarm.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "dbScan.h"
#include "devSup.h"
#include "errMdef.h"
#include "special.h"
#include "recSup.h"
#include "recGbl.h"
#include "cantProceed.h"
#include "menuYesNo.h"
#include "menuConvert.h"
#include "menuOmsl.h"
#define GEN_SIZE_OFFSET
#include "DigitalScanPSRecord.h"
#undef  GEN_SIZE_OFFSET
#include "menuIvoa.h"
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record(DigitalScanPSRecord *, int);
static long process(DigitalScanPSRecord *);
static long special(DBADDR *, int);
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units(DBADDR *, char *);
static long get_precision(DBADDR *, long *);
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double(DBADDR *, struct dbr_grDouble *);
static long get_control_double(DBADDR *, struct dbr_ctrlDouble *);
static long get_alarm_double(DBADDR *, struct dbr_alDouble *);

rset DigitalScanPSRSET={
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
epicsExportAddress(rset,DigitalScanPSRSET);

struct DigitalScanPSdset { /* DigitalScanPS dset */
    long      number;
    DEVSUPFUN dev_report;
    DEVSUPFUN init;
    DEVSUPFUN init_record; /*returns: (-1,0)=>(failure,success)*/
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_DigitalScan; /*returns: (-1,0)=>(failure,success)*/
	DEVSUPFUN special_linconv;
};

static void checkAlarms(DigitalScanPSRecord *);
static long fetch_value(DigitalScanPSRecord *, double *);
static void convert(DigitalScanPSRecord *, double);
static void monitor(DigitalScanPSRecord *);
static long writeValue(DigitalScanPSRecord *);

static long init_record(DigitalScanPSRecord *prec, int pass)
{
    struct DigitalScanPSdset *pdset;
    double 	eoff = prec->eoff, eslo = prec->eslo;
    double	value;

    if (pass==0) return(0);

    /* ao.siml must be a CONSTANT or a PV_LINK or a DB_LINK */
    if (prec->siml.type == CONSTANT) {
	recGblInitConstantLink(&prec->siml,DBF_USHORT,&prec->simm);
    }

    if(!(pdset = (struct DigitalScanPSdset *)(prec->dset))) {
	recGblRecordError(S_dev_noDSET,(void *)prec,"DigitalScanPS: init_record");
	return(S_dev_noDSET);
    }
    /* get the initial value if dol is a constant*/
    if (prec->dol.type == CONSTANT) {
	if(recGblInitConstantLink(&prec->dol,DBF_DOUBLE,&prec->val))
	    prec->udf = isnan(prec->val);
    }

    /* must have write_DigitalScan function defined */
    if ((pdset->number < 6) || (pdset->write_DigitalScan ==NULL)) {
	recGblRecordError(S_dev_missingSup,(void *)prec,"DigitalScanPS: init_record");
	return(S_dev_missingSup);
    }
    prec->init = TRUE;
    /*The following is for old device support that doesnt know about eoff*/
    if ((prec->eslo==1.0) && (prec->eoff==0.0)) {
	prec->eoff = prec->egul;
    }

    if (pdset->init_record) {
	long status=(*pdset->init_record)(prec);
	if (prec->linr == menuConvertSLOPE) {
	    prec->eoff = eoff;
	    prec->eslo = eslo;
	}
        switch(status){
        case(0): /* convert */
	    value = (double)prec->rval + (double)prec->roff;
	    if(prec->aslo!=0.0) value *= prec->aslo;
	    value += prec->aoff;
            if (prec->linr == menuConvertNO_CONVERSION){
		; /*do nothing*/
            } else if ((prec->linr == menuConvertLINEAR) ||
		      (prec->linr == menuConvertSLOPE)) {
                value = value*prec->eslo + prec->eoff;
            }else{
                if(cvtRawToEngBpt(&value,prec->linr,prec->init,
			(void *)&prec->pbrk,&prec->lbrk)!=0) break;
            }
	    prec->val = value;
	    prec->udf = isnan(value);
        break;
        case(2): /* no convert */
        break;
        default:
	     recGblRecordError(S_dev_badInitRet,(void *)prec,"ao: init_record");
	     return(S_dev_badInitRet);
        }
    }
    prec->oval = prec->pval = prec->val;
    prec->mlst = prec->val;
    prec->alst = prec->val;
    prec->lalm = prec->val;
    prec->oraw = prec->rval;
    prec->orbv = prec->rbv;
    return(0);
}

static long process(DigitalScanPSRecord *prec)
{
	struct DigitalScanPSdset	*pdset = (struct DigitalScanPSdset *)(prec->dset);
	long		 status=0;
	unsigned char    pact=prec->pact;
	double		value;

	if ((pdset==NULL) || (pdset->write_DigitalScan==NULL)) {
		prec->pact=TRUE;
		recGblRecordError(S_dev_missingSup,(void *)prec,"write_DigitalScan");
		return(S_dev_missingSup);
	}

	/* fetch value and convert*/
	if (prec->pact == FALSE) {
                if ((prec->dol.type != CONSTANT)
                && (prec->omsl == menuOmslclosed_loop)) {
                    status = fetch_value(prec, &value);
                }
                else {
                    value = prec->val;
                }
		if(!status) convert(prec, value);
		prec->udf = isnan(prec->val);
	}

	/* check for alarms */
	checkAlarms(prec);

	if (prec->nsev < INVALID_ALARM )
		status=writeValue(prec); /* write the new value */
	else {
    		switch (prec->ivoa) {
		    case (menuIvoaContinue_normally) :
			status=writeValue(prec); /* write the new value */
		        break;
		    case (menuIvoaDon_t_drive_outputs) :
			break;
		    case (menuIvoaSet_output_to_IVOV) :
	                if(prec->pact == FALSE){
			 	prec->val=prec->ivov;
			 	value=prec->ivov;
				convert(prec,value);
                        }
			status=writeValue(prec); /* write the new value */
		        break;
		    default :
			status=-1;
		        recGblRecordError(S_db_badField,(void *)prec,
		                "ao:process Illegal IVOA field");
		}
	}

	/* check if device support set pact */
	if ( !pact && prec->pact ) return(0);
	prec->pact = TRUE;

	recGblGetTimeStamp(prec);

	/* check event list */
	monitor(prec);

	/* process the forward scan link record */
        recGblFwdLink(prec);

	prec->init=FALSE;
	prec->pact=FALSE;
	return(status);
}

static long special(DBADDR *paddr, int after)
{
    DigitalScanPSRecord     *prec = (DigitalScanPSRecord *)(paddr->precord);
    struct DigitalScanPSdset       *pdset = (struct DigitalScanPSdset *) (prec->dset);
    int                 special_type = paddr->special;

    switch(special_type) {
    case(SPC_LINCONV):
        if(pdset->number<6 ) {
            recGblDbaddrError(S_db_noMod,paddr,"ao: special");
            return(S_db_noMod);
        }
	prec->init=TRUE;
        if ((prec->linr == menuConvertLINEAR) && pdset->special_linconv) {
	    double eoff = prec->eoff;
	    double eslo = prec->eslo;
	    long status;
	    prec->eoff = prec->egul;
	    status = (*pdset->special_linconv)(prec,after);
	    if (eoff != prec->eoff)
		db_post_events(prec, &prec->eoff, DBE_VALUE|DBE_LOG);
            if (eslo != prec->eslo)
		db_post_events(prec, &prec->eslo, DBE_VALUE|DBE_LOG);
	    return (status);
	}
	return (0);
    default:
        recGblDbaddrError(S_db_badChoice,paddr,"ao: special");
        return(S_db_badChoice);
    }
}

static long get_units(DBADDR * paddr,char *units)
{
    DigitalScanPSRecord	*prec=(DigitalScanPSRecord *)paddr->precord;

    strncpy(units,prec->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr,long *precision)
{
    DigitalScanPSRecord	*prec=(DigitalScanPSRecord *)paddr->precord;

    *precision = prec->prec;
    if(paddr->pfield == (void *)&prec->val
    || paddr->pfield == (void *)&prec->oval
    || paddr->pfield == (void *)&prec->pval) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    DigitalScanPSRecord	*prec=(DigitalScanPSRecord *)paddr->precord;

    if(paddr->pfield==(void *)&prec->val
    || paddr->pfield==(void *)&prec->hihi
    || paddr->pfield==(void *)&prec->high
    || paddr->pfield==(void *)&prec->low
    || paddr->pfield==(void *)&prec->lolo
    || paddr->pfield==(void *)&prec->oval
    || paddr->pfield==(void *)&prec->pval){
        pgd->upper_disp_limit = prec->hopr;
        pgd->lower_disp_limit = prec->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr, struct dbr_ctrlDouble *pcd)
{
    DigitalScanPSRecord	*prec=(DigitalScanPSRecord *)paddr->precord;

    if(paddr->pfield==(void *)&prec->val
    || paddr->pfield==(void *)&prec->hihi
    || paddr->pfield==(void *)&prec->high
    || paddr->pfield==(void *)&prec->low
    || paddr->pfield==(void *)&prec->lolo
    || paddr->pfield==(void *)&prec->oval
    || paddr->pfield==(void *)&prec->pval){
        pcd->upper_ctrl_limit = prec->drvh;
        pcd->lower_ctrl_limit = prec->drvl;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}
static long get_alarm_double(DBADDR *paddr, struct dbr_alDouble *pad)
{
    DigitalScanPSRecord	*prec=(DigitalScanPSRecord *)paddr->precord;

    if(paddr->pfield==(void *)&prec->val){
        pad->upper_alarm_limit = prec->hhsv ? prec->hihi : epicsNAN;
        pad->upper_warning_limit = prec->hsv ? prec->high : epicsNAN;
        pad->lower_warning_limit = prec->lsv ? prec->low : epicsNAN;
        pad->lower_alarm_limit = prec->llsv ? prec->lolo : epicsNAN;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(DigitalScanPSRecord *prec)
{
    double val, hyst, lalm;
    double alev;
    epicsEnum16 asev;

    if (prec->udf) {
        recGblSetSevr(prec, UDF_ALARM, INVALID_ALARM);
        return;
    }

    val = prec->val;
    hyst = prec->hyst;
    lalm = prec->lalm;

    /* alarm condition hihi */
    asev = prec->hhsv;
    alev = prec->hihi;
    if (asev && (val >= alev || ((lalm == alev) && (val >= alev - hyst)))) {
        if (recGblSetSevr(prec, HIHI_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition lolo */
    asev = prec->llsv;
    alev = prec->lolo;
    if (asev && (val <= alev || ((lalm == alev) && (val <= alev + hyst)))) {
        if (recGblSetSevr(prec, LOLO_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition high */
    asev = prec->hsv;
    alev = prec->high;
    if (asev && (val >= alev || ((lalm == alev) && (val >= alev - hyst)))) {
        if (recGblSetSevr(prec, HIGH_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition low */
    asev = prec->lsv;
    alev = prec->low;
    if (asev && (val <= alev || ((lalm == alev) && (val <= alev + hyst)))) {
        if (recGblSetSevr(prec, LOW_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* we get here only if val is out of alarm by at least hyst */
    prec->lalm = val;
    return;
}

static long fetch_value(DigitalScanPSRecord *prec,double *pvalue)
{
	short		save_pact;
	long		status;

	save_pact = prec->pact;
	prec->pact = TRUE;

	/* don't allow dbputs to val field */
	prec->val=prec->pval;

        status = dbGetLink(&prec->dol,DBR_DOUBLE,pvalue,0,0);
        prec->pact = save_pact;

	if (status) {
           recGblSetSevr(prec,LINK_ALARM,INVALID_ALARM);
           return(status);
	}

        if (prec->oif == aoOIF_Incremental)
           *pvalue += prec->val;

	return(0);
}

static void convert(DigitalScanPSRecord *prec, double value)
{
    /* check drive limits */
    if (prec->drvh > prec->drvl) {
        if (value > prec->drvh)
            value = prec->drvh;
        else if (value < prec->drvl)
            value = prec->drvl;
    }
    prec->val = value;
    prec->pval = value;

    /* now set value equal to desired output value */
    /* apply the output rate of change */
    if (prec->oroc != 0){/*must be defined and >0*/
        double diff;

        diff = value - prec->oval;
        if (diff < 0) {
            if (prec->oroc < -diff)
                value = prec->oval - prec->oroc;
        } else if (prec->oroc < diff)
            value = prec->oval + prec->oroc;
    }
    prec->omod = (prec->oval!=value);
    prec->oval = value;

    /* convert */
    switch (prec->linr) {
    case menuConvertNO_CONVERSION:
        break; /* do nothing*/
    case menuConvertLINEAR:
    case menuConvertSLOPE:
        if (prec->eslo == 0.0) value = 0;
        else value = (value - prec->eoff) / prec->eslo;
        break;
    default:
        if (cvtEngToRawBpt(&value, prec->linr, prec->init,
            (void *)&prec->pbrk, &prec->lbrk) != 0) {
            recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
            return;
        }
    }
    value -= prec->aoff;
    if (prec->aslo != 0) value /= prec->aslo;
    if (value >= 0.0)
        prec->rval = (epicsInt32)(value + 0.5) - prec->roff;
    else
        prec->rval = (epicsInt32)(value - 0.5) - prec->roff;
}


static void monitor(DigitalScanPSRecord *prec)
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(prec);
        /* check for value change */
        delta = prec->mlst - prec->val;
        if(delta<0.0) delta = -delta;
        if (delta > prec->mdel) {
                /* post events for value change */
                monitor_mask |= DBE_VALUE;
                /* update last value monitored */
                prec->mlst = prec->val;
        }
        /* check for archive change */
        delta = prec->alst - prec->val;
        if(delta<0.0) delta = -delta;
        if (delta > prec->adel) {
                /* post events on value field for archive change */
                monitor_mask |= DBE_LOG;
                /* update last archive value monitored */
                prec->alst = prec->val;
        }

        /*check for state change */
        if (prec->state != prec->lstat)
        {
            /* code */
            monitor_mask |= DBE_VALUE;
            prec->lstat = prec->state; // Update last power supply state
        }


        /* send out monitors connected to the value field */
        if (monitor_mask){
                db_post_events(prec,&prec->val,monitor_mask);
                db_post_events(prec,&prec->state,monitor_mask);
	}
	if(prec->omod) monitor_mask |= (DBE_VALUE|DBE_LOG);
	if(monitor_mask) {
		prec->omod = FALSE;
		db_post_events(prec,&prec->oval,monitor_mask);
		if(prec->oraw != prec->rval) {
                	db_post_events(prec,&prec->rval,
			    monitor_mask|DBE_VALUE|DBE_LOG);
			prec->oraw = prec->rval;
		}
		if(prec->orbv != prec->rbv) {
                	db_post_events(prec,&prec->rbv,
			    monitor_mask|DBE_VALUE|DBE_LOG);
			prec->orbv = prec->rbv;
		}
	}
	return;
}

static long writeValue(DigitalScanPSRecord *prec)
{
	long		status;
        struct DigitalScanPSdset 	*pdset = (struct DigitalScanPSdset *) (prec->dset);

	if (prec->pact == TRUE){
		status=(*pdset->write_DigitalScan)(prec);
		return(status);
	}

        status = dbGetLink(&prec->siml,DBR_USHORT,&(prec->simm),0,0);
	if (status)
		return(status);

	if (prec->simm == menuYesNoNO){
		status=(*pdset->write_DigitalScan)(prec);
		return(status);
	}
	if (prec->simm == menuYesNoYES){
		status = dbPutLink(&(prec->siol),DBR_DOUBLE,&(prec->oval),1);
	} else {
		status=-1;
		recGblSetSevr(prec,SOFT_ALARM,INVALID_ALARM);
		return(status);
	}
        recGblSetSevr(prec,SIMM_ALARM,prec->sims);

	return(status);
}

