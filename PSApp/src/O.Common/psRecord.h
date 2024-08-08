/** @file psRecord.h
 * @brief Declarations for the @ref psRecord "ps" record type.
 *
 * This header was generated from psRecord.dbd
 */

#ifndef INC_psRecord_H
#define INC_psRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "devSup.h"
#include "epicsTime.h"
#include "callback.h"

#ifndef psPOST_NUM_CHOICES
/** @brief Enumerated type from menu psPOST */
typedef enum {
    psPOST_Always                   /**< @brief State string "Always" */,
    psPOST_OnChange                 /**< @brief State string "On Change" */
} psPOST;
/** @brief Number of states defined for menu psPOST */
#define psPOST_NUM_CHOICES 2
#endif

/** @brief Declaration of ps record type. */
typedef struct psRecord {
    char                name[61];   /**< @brief Record Name */
    char                desc[41];   /**< @brief Descriptor */
    char                asg[29];    /**< @brief Access Security Group */
    epicsEnum16         scan;       /**< @brief Scan Mechanism */
    epicsEnum16         pini;       /**< @brief Process at iocInit */
    epicsInt16          phas;       /**< @brief Scan Phase */
    char                evnt[40];   /**< @brief Event Name */
    epicsInt16          tse;        /**< @brief Time Stamp Event */
    DBLINK              tsel;       /**< @brief Time Stamp Link */
    epicsEnum16         dtyp;       /**< @brief Device Type */
    epicsInt16          disv;       /**< @brief Disable Value */
    epicsInt16          disa;       /**< @brief Disable */
    DBLINK              sdis;       /**< @brief Scanning Disable */
    epicsMutexId        mlok;       /**< @brief Monitor lock */
    ELLLIST             mlis;       /**< @brief Monitor List */
    ELLLIST             bklnk;      /**< @brief Backwards link tracking */
    epicsUInt8          disp;       /**< @brief Disable putField */
    epicsUInt8          proc;       /**< @brief Force Processing */
    epicsEnum16         stat;       /**< @brief Alarm Status */
    epicsEnum16         sevr;       /**< @brief Alarm Severity */
    char                amsg[40];   /**< @brief Alarm Message */
    epicsEnum16         nsta;       /**< @brief New Alarm Status */
    epicsEnum16         nsev;       /**< @brief New Alarm Severity */
    char                namsg[40];  /**< @brief New Alarm Message */
    epicsEnum16         acks;       /**< @brief Alarm Ack Severity */
    epicsEnum16         ackt;       /**< @brief Alarm Ack Transient */
    epicsEnum16         diss;       /**< @brief Disable Alarm Sevrty */
    epicsUInt8          lcnt;       /**< @brief Lock Count */
    epicsUInt8          pact;       /**< @brief Record active */
    epicsUInt8          putf;       /**< @brief dbPutField process */
    epicsUInt8          rpro;       /**< @brief Reprocess  */
    struct asgMember    *asp;       /**< @brief Access Security Pvt */
    struct processNotify *ppn;      /**< @brief pprocessNotify */
    struct processNotifyRecord *ppnr; /**< @brief pprocessNotifyRecord */
    struct scan_element *spvt;      /**< @brief Scan Private */
    struct typed_rset   *rset;      /**< @brief Address of RSET */
    unambiguous_dset    *dset;      /**< @brief DSET address */
    void                *dpvt;      /**< @brief Device Private */
    struct dbRecordType *rdes;      /**< @brief Address of dbRecordType */
    struct lockRecord   *lset;      /**< @brief Lock Set */
    epicsEnum16         prio;       /**< @brief Scheduling Priority */
    epicsUInt8          tpro;       /**< @brief Trace Processing */
    epicsUInt8          bkpt;       /**< @brief Break Point */
    epicsUInt8          udf;        /**< @brief Undefined */
    epicsEnum16         udfs;       /**< @brief Undefined Alarm Sevrty */
    epicsTimeStamp      time;       /**< @brief Time */
    epicsUInt64         utag;       /**< @brief Time Tag */
    DBLINK              flnk;       /**< @brief Forward Process Link */
    void *		val;                    /**< @brief Value */
    DBLINK              inp;        /**< @brief Input Specification */
    epicsInt16          dtype;      /**< @brief Input Specification */
    char                domain[32]; /**< @brief domain for ps */
    char                command[16];/**< @brief command for ps */
    epicsFloat64        current_val;/**< @brief set ps current val */
    epicsFloat64        c_val;      /**< @brief ps current val */
    epicsFloat64        v_val;      /**< @brief ps volt val */
    epicsInt32          w_length;   /**< @brief WAVE FORM LENGTH */
    epicsInt32          w_no;       /**< @brief WAVE FORM LENGTH */
    char                state[64];  /**< @brief current ps state */
    char                psstate[256];/**< @brief current ps state */
    char                version[16];/**< @brief ps program version */
    epicsUInt32         delay;      /**< @brief delay of ps */
    epicsUInt16         port;       /**< @brief connection port */
    epicsUInt16         max;        /**< @brief MAX CURRENT OF POWER */
    epicsInt16          prec;       /**< @brief Display Precision */
    DBLINK              out;        /**< @brief Output Specification */
    char                egu[16];    /**< @brief Engineering Units */
    epicsFloat64        hopr;       /**< @brief High Operating Range */
    epicsFloat64        lopr;       /**< @brief Low Operating Range */
    epicsUInt32         nelm;       /**< @brief Number of Elements */
    epicsEnum16         ftvl;       /**< @brief Field Type of Value */
    epicsUInt32         nord;       /**< @brief Number elements read */
    void *		bptr;                   /**< @brief Buffer Pointer */
    DBLINK              siml;       /**< @brief Simulation Mode Link */
    epicsEnum16         simm;       /**< @brief Simulation Mode */
    epicsEnum16         sims;       /**< @brief Simulation Mode Severity */
    DBLINK              siol;       /**< @brief Simulation Output Link */
    epicsEnum16         oldsimm;    /**< @brief Prev. Simulation Mode */
    epicsEnum16         sscn;       /**< @brief Sim. Mode Scan */
    epicsFloat64        sdly;       /**< @brief Sim. Mode Async Delay */
    CALLBACK            *simpvt;    /**< @brief Sim. Mode Private */
    epicsEnum16         mpst;       /**< @brief Post Value Monitors */
    epicsEnum16         apst;       /**< @brief Post Archive Monitors */
    epicsUInt32         hash;       /**< @brief Hash of OnChange data. */
} psRecord;

typedef enum {
	psRecordNAME = 0,
	psRecordDESC = 1,
	psRecordASG = 2,
	psRecordSCAN = 3,
	psRecordPINI = 4,
	psRecordPHAS = 5,
	psRecordEVNT = 6,
	psRecordTSE = 7,
	psRecordTSEL = 8,
	psRecordDTYP = 9,
	psRecordDISV = 10,
	psRecordDISA = 11,
	psRecordSDIS = 12,
	psRecordMLOK = 13,
	psRecordMLIS = 14,
	psRecordBKLNK = 15,
	psRecordDISP = 16,
	psRecordPROC = 17,
	psRecordSTAT = 18,
	psRecordSEVR = 19,
	psRecordAMSG = 20,
	psRecordNSTA = 21,
	psRecordNSEV = 22,
	psRecordNAMSG = 23,
	psRecordACKS = 24,
	psRecordACKT = 25,
	psRecordDISS = 26,
	psRecordLCNT = 27,
	psRecordPACT = 28,
	psRecordPUTF = 29,
	psRecordRPRO = 30,
	psRecordASP = 31,
	psRecordPPN = 32,
	psRecordPPNR = 33,
	psRecordSPVT = 34,
	psRecordRSET = 35,
	psRecordDSET = 36,
	psRecordDPVT = 37,
	psRecordRDES = 38,
	psRecordLSET = 39,
	psRecordPRIO = 40,
	psRecordTPRO = 41,
	psRecordBKPT = 42,
	psRecordUDF = 43,
	psRecordUDFS = 44,
	psRecordTIME = 45,
	psRecordUTAG = 46,
	psRecordFLNK = 47,
	psRecordVAL = 48,
	psRecordINP = 49,
	psRecordDTYPE = 50,
	psRecordDOMAIN = 51,
	psRecordCOMMAND = 52,
	psRecordCURRENT_VAL = 53,
	psRecordC_VAL = 54,
	psRecordV_VAL = 55,
	psRecordW_LENGTH = 56,
	psRecordW_NO = 57,
	psRecordSTATE = 58,
	psRecordPSSTATE = 59,
	psRecordVERSION = 60,
	psRecordDELAY = 61,
	psRecordPORT = 62,
	psRecordMAX = 63,
	psRecordPREC = 64,
	psRecordOUT = 65,
	psRecordEGU = 66,
	psRecordHOPR = 67,
	psRecordLOPR = 68,
	psRecordNELM = 69,
	psRecordFTVL = 70,
	psRecordNORD = 71,
	psRecordBPTR = 72,
	psRecordSIML = 73,
	psRecordSIMM = 74,
	psRecordSIMS = 75,
	psRecordSIOL = 76,
	psRecordOLDSIMM = 77,
	psRecordSSCN = 78,
	psRecordSDLY = 79,
	psRecordSIMPVT = 80,
	psRecordMPST = 81,
	psRecordAPST = 82,
	psRecordHASH = 83
} psFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsExport.h>
#include <cantProceed.h>
#ifdef __cplusplus
extern "C" {
#endif
static int psRecordSizeOffset(dbRecordType *prt)
{
    psRecord *prec = 0;

    if (prt->no_fields != 84) {
        cantProceed("IOC build or installation error:\n"
            "    The psRecord defined in the DBD file has %d fields,\n"
            "    but the record support code was built with 84.\n",
            prt->no_fields);
    }
    prt->papFldDes[psRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[psRecordNAME]->offset = (unsigned short)offsetof(psRecord, name);
    prt->papFldDes[psRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[psRecordDESC]->offset = (unsigned short)offsetof(psRecord, desc);
    prt->papFldDes[psRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[psRecordASG]->offset = (unsigned short)offsetof(psRecord, asg);
    prt->papFldDes[psRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[psRecordSCAN]->offset = (unsigned short)offsetof(psRecord, scan);
    prt->papFldDes[psRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[psRecordPINI]->offset = (unsigned short)offsetof(psRecord, pini);
    prt->papFldDes[psRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[psRecordPHAS]->offset = (unsigned short)offsetof(psRecord, phas);
    prt->papFldDes[psRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[psRecordEVNT]->offset = (unsigned short)offsetof(psRecord, evnt);
    prt->papFldDes[psRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[psRecordTSE]->offset = (unsigned short)offsetof(psRecord, tse);
    prt->papFldDes[psRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[psRecordTSEL]->offset = (unsigned short)offsetof(psRecord, tsel);
    prt->papFldDes[psRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[psRecordDTYP]->offset = (unsigned short)offsetof(psRecord, dtyp);
    prt->papFldDes[psRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[psRecordDISV]->offset = (unsigned short)offsetof(psRecord, disv);
    prt->papFldDes[psRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[psRecordDISA]->offset = (unsigned short)offsetof(psRecord, disa);
    prt->papFldDes[psRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[psRecordSDIS]->offset = (unsigned short)offsetof(psRecord, sdis);
    prt->papFldDes[psRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[psRecordMLOK]->offset = (unsigned short)offsetof(psRecord, mlok);
    prt->papFldDes[psRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[psRecordMLIS]->offset = (unsigned short)offsetof(psRecord, mlis);
    prt->papFldDes[psRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[psRecordBKLNK]->offset = (unsigned short)offsetof(psRecord, bklnk);
    prt->papFldDes[psRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[psRecordDISP]->offset = (unsigned short)offsetof(psRecord, disp);
    prt->papFldDes[psRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[psRecordPROC]->offset = (unsigned short)offsetof(psRecord, proc);
    prt->papFldDes[psRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[psRecordSTAT]->offset = (unsigned short)offsetof(psRecord, stat);
    prt->papFldDes[psRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[psRecordSEVR]->offset = (unsigned short)offsetof(psRecord, sevr);
    prt->papFldDes[psRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[psRecordAMSG]->offset = (unsigned short)offsetof(psRecord, amsg);
    prt->papFldDes[psRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[psRecordNSTA]->offset = (unsigned short)offsetof(psRecord, nsta);
    prt->papFldDes[psRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[psRecordNSEV]->offset = (unsigned short)offsetof(psRecord, nsev);
    prt->papFldDes[psRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[psRecordNAMSG]->offset = (unsigned short)offsetof(psRecord, namsg);
    prt->papFldDes[psRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[psRecordACKS]->offset = (unsigned short)offsetof(psRecord, acks);
    prt->papFldDes[psRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[psRecordACKT]->offset = (unsigned short)offsetof(psRecord, ackt);
    prt->papFldDes[psRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[psRecordDISS]->offset = (unsigned short)offsetof(psRecord, diss);
    prt->papFldDes[psRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[psRecordLCNT]->offset = (unsigned short)offsetof(psRecord, lcnt);
    prt->papFldDes[psRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[psRecordPACT]->offset = (unsigned short)offsetof(psRecord, pact);
    prt->papFldDes[psRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[psRecordPUTF]->offset = (unsigned short)offsetof(psRecord, putf);
    prt->papFldDes[psRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[psRecordRPRO]->offset = (unsigned short)offsetof(psRecord, rpro);
    prt->papFldDes[psRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[psRecordASP]->offset = (unsigned short)offsetof(psRecord, asp);
    prt->papFldDes[psRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[psRecordPPN]->offset = (unsigned short)offsetof(psRecord, ppn);
    prt->papFldDes[psRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[psRecordPPNR]->offset = (unsigned short)offsetof(psRecord, ppnr);
    prt->papFldDes[psRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[psRecordSPVT]->offset = (unsigned short)offsetof(psRecord, spvt);
    prt->papFldDes[psRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[psRecordRSET]->offset = (unsigned short)offsetof(psRecord, rset);
    prt->papFldDes[psRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[psRecordDSET]->offset = (unsigned short)offsetof(psRecord, dset);
    prt->papFldDes[psRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[psRecordDPVT]->offset = (unsigned short)offsetof(psRecord, dpvt);
    prt->papFldDes[psRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[psRecordRDES]->offset = (unsigned short)offsetof(psRecord, rdes);
    prt->papFldDes[psRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[psRecordLSET]->offset = (unsigned short)offsetof(psRecord, lset);
    prt->papFldDes[psRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[psRecordPRIO]->offset = (unsigned short)offsetof(psRecord, prio);
    prt->papFldDes[psRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[psRecordTPRO]->offset = (unsigned short)offsetof(psRecord, tpro);
    prt->papFldDes[psRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[psRecordBKPT]->offset = (unsigned short)offsetof(psRecord, bkpt);
    prt->papFldDes[psRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[psRecordUDF]->offset = (unsigned short)offsetof(psRecord, udf);
    prt->papFldDes[psRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[psRecordUDFS]->offset = (unsigned short)offsetof(psRecord, udfs);
    prt->papFldDes[psRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[psRecordTIME]->offset = (unsigned short)offsetof(psRecord, time);
    prt->papFldDes[psRecordUTAG]->size = sizeof(prec->utag);
    prt->papFldDes[psRecordUTAG]->offset = (unsigned short)offsetof(psRecord, utag);
    prt->papFldDes[psRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[psRecordFLNK]->offset = (unsigned short)offsetof(psRecord, flnk);
    prt->papFldDes[psRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[psRecordVAL]->offset = (unsigned short)offsetof(psRecord, val);
    prt->papFldDes[psRecordINP]->size = sizeof(prec->inp);
    prt->papFldDes[psRecordINP]->offset = (unsigned short)offsetof(psRecord, inp);
    prt->papFldDes[psRecordDTYPE]->size = sizeof(prec->dtype);
    prt->papFldDes[psRecordDTYPE]->offset = (unsigned short)offsetof(psRecord, dtype);
    prt->papFldDes[psRecordDOMAIN]->size = sizeof(prec->domain);
    prt->papFldDes[psRecordDOMAIN]->offset = (unsigned short)offsetof(psRecord, domain);
    prt->papFldDes[psRecordCOMMAND]->size = sizeof(prec->command);
    prt->papFldDes[psRecordCOMMAND]->offset = (unsigned short)offsetof(psRecord, command);
    prt->papFldDes[psRecordCURRENT_VAL]->size = sizeof(prec->current_val);
    prt->papFldDes[psRecordCURRENT_VAL]->offset = (unsigned short)offsetof(psRecord, current_val);
    prt->papFldDes[psRecordC_VAL]->size = sizeof(prec->c_val);
    prt->papFldDes[psRecordC_VAL]->offset = (unsigned short)offsetof(psRecord, c_val);
    prt->papFldDes[psRecordV_VAL]->size = sizeof(prec->v_val);
    prt->papFldDes[psRecordV_VAL]->offset = (unsigned short)offsetof(psRecord, v_val);
    prt->papFldDes[psRecordW_LENGTH]->size = sizeof(prec->w_length);
    prt->papFldDes[psRecordW_LENGTH]->offset = (unsigned short)offsetof(psRecord, w_length);
    prt->papFldDes[psRecordW_NO]->size = sizeof(prec->w_no);
    prt->papFldDes[psRecordW_NO]->offset = (unsigned short)offsetof(psRecord, w_no);
    prt->papFldDes[psRecordSTATE]->size = sizeof(prec->state);
    prt->papFldDes[psRecordSTATE]->offset = (unsigned short)offsetof(psRecord, state);
    prt->papFldDes[psRecordPSSTATE]->size = sizeof(prec->psstate);
    prt->papFldDes[psRecordPSSTATE]->offset = (unsigned short)offsetof(psRecord, psstate);
    prt->papFldDes[psRecordVERSION]->size = sizeof(prec->version);
    prt->papFldDes[psRecordVERSION]->offset = (unsigned short)offsetof(psRecord, version);
    prt->papFldDes[psRecordDELAY]->size = sizeof(prec->delay);
    prt->papFldDes[psRecordDELAY]->offset = (unsigned short)offsetof(psRecord, delay);
    prt->papFldDes[psRecordPORT]->size = sizeof(prec->port);
    prt->papFldDes[psRecordPORT]->offset = (unsigned short)offsetof(psRecord, port);
    prt->papFldDes[psRecordMAX]->size = sizeof(prec->max);
    prt->papFldDes[psRecordMAX]->offset = (unsigned short)offsetof(psRecord, max);
    prt->papFldDes[psRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[psRecordPREC]->offset = (unsigned short)offsetof(psRecord, prec);
    prt->papFldDes[psRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[psRecordOUT]->offset = (unsigned short)offsetof(psRecord, out);
    prt->papFldDes[psRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[psRecordEGU]->offset = (unsigned short)offsetof(psRecord, egu);
    prt->papFldDes[psRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[psRecordHOPR]->offset = (unsigned short)offsetof(psRecord, hopr);
    prt->papFldDes[psRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[psRecordLOPR]->offset = (unsigned short)offsetof(psRecord, lopr);
    prt->papFldDes[psRecordNELM]->size = sizeof(prec->nelm);
    prt->papFldDes[psRecordNELM]->offset = (unsigned short)offsetof(psRecord, nelm);
    prt->papFldDes[psRecordFTVL]->size = sizeof(prec->ftvl);
    prt->papFldDes[psRecordFTVL]->offset = (unsigned short)offsetof(psRecord, ftvl);
    prt->papFldDes[psRecordNORD]->size = sizeof(prec->nord);
    prt->papFldDes[psRecordNORD]->offset = (unsigned short)offsetof(psRecord, nord);
    prt->papFldDes[psRecordBPTR]->size = sizeof(prec->bptr);
    prt->papFldDes[psRecordBPTR]->offset = (unsigned short)offsetof(psRecord, bptr);
    prt->papFldDes[psRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[psRecordSIML]->offset = (unsigned short)offsetof(psRecord, siml);
    prt->papFldDes[psRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[psRecordSIMM]->offset = (unsigned short)offsetof(psRecord, simm);
    prt->papFldDes[psRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[psRecordSIMS]->offset = (unsigned short)offsetof(psRecord, sims);
    prt->papFldDes[psRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[psRecordSIOL]->offset = (unsigned short)offsetof(psRecord, siol);
    prt->papFldDes[psRecordOLDSIMM]->size = sizeof(prec->oldsimm);
    prt->papFldDes[psRecordOLDSIMM]->offset = (unsigned short)offsetof(psRecord, oldsimm);
    prt->papFldDes[psRecordSSCN]->size = sizeof(prec->sscn);
    prt->papFldDes[psRecordSSCN]->offset = (unsigned short)offsetof(psRecord, sscn);
    prt->papFldDes[psRecordSDLY]->size = sizeof(prec->sdly);
    prt->papFldDes[psRecordSDLY]->offset = (unsigned short)offsetof(psRecord, sdly);
    prt->papFldDes[psRecordSIMPVT]->size = sizeof(prec->simpvt);
    prt->papFldDes[psRecordSIMPVT]->offset = (unsigned short)offsetof(psRecord, simpvt);
    prt->papFldDes[psRecordMPST]->size = sizeof(prec->mpst);
    prt->papFldDes[psRecordMPST]->offset = (unsigned short)offsetof(psRecord, mpst);
    prt->papFldDes[psRecordAPST]->size = sizeof(prec->apst);
    prt->papFldDes[psRecordAPST]->offset = (unsigned short)offsetof(psRecord, apst);
    prt->papFldDes[psRecordHASH]->size = sizeof(prec->hash);
    prt->papFldDes[psRecordHASH]->offset = (unsigned short)offsetof(psRecord, hash);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(psRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_psRecord_H */
