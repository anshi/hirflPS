/** @file InforminRecord.h
 * @brief Declarations for the @ref InforminRecord "Informin" record type.
 *
 * This header was generated from InforminRecord.dbd
 */

#ifndef INC_InforminRecord_H
#define INC_InforminRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "devSup.h"
#include "epicsTime.h"

#ifndef InforminPOST_NUM_CHOICES
/** @brief Enumerated type from menu InforminPOST */
typedef enum {
    InforminPOST_OnChange           /**< @brief State string "On Change" */,
    InforminPOST_Always             /**< @brief State string "Always" */
} InforminPOST;
/** @brief Number of states defined for menu InforminPOST */
#define InforminPOST_NUM_CHOICES 2
#endif

/** @brief Declaration of Informin record type. */
typedef struct InforminRecord {
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
    char                val[100];   /**< @brief Current Value */
    epicsInt16          informcode; /**< @brief The information code from device */
    epicsInt16          ready;      /**< @brief Ready state of the PS */
    char                oval[100];  /**< @brief Previous Value */
    DBLINK              inp;        /**< @brief Input Specification */
    epicsEnum16         mpst;       /**< @brief Post Value Monitors */
    epicsEnum16         apst;       /**< @brief Post Archive Monitors */
    DBLINK              siol;       /**< @brief Sim Input Specifctn */
    char                sval[40];   /**< @brief Simulation Value */
    DBLINK              siml;       /**< @brief Sim Mode Location */
    epicsEnum16         simm;       /**< @brief Simulation Mode */
    epicsEnum16         sims;       /**< @brief Sim mode Alarm Svrty */
} InforminRecord;

typedef enum {
	InforminRecordNAME = 0,
	InforminRecordDESC = 1,
	InforminRecordASG = 2,
	InforminRecordSCAN = 3,
	InforminRecordPINI = 4,
	InforminRecordPHAS = 5,
	InforminRecordEVNT = 6,
	InforminRecordTSE = 7,
	InforminRecordTSEL = 8,
	InforminRecordDTYP = 9,
	InforminRecordDISV = 10,
	InforminRecordDISA = 11,
	InforminRecordSDIS = 12,
	InforminRecordMLOK = 13,
	InforminRecordMLIS = 14,
	InforminRecordBKLNK = 15,
	InforminRecordDISP = 16,
	InforminRecordPROC = 17,
	InforminRecordSTAT = 18,
	InforminRecordSEVR = 19,
	InforminRecordAMSG = 20,
	InforminRecordNSTA = 21,
	InforminRecordNSEV = 22,
	InforminRecordNAMSG = 23,
	InforminRecordACKS = 24,
	InforminRecordACKT = 25,
	InforminRecordDISS = 26,
	InforminRecordLCNT = 27,
	InforminRecordPACT = 28,
	InforminRecordPUTF = 29,
	InforminRecordRPRO = 30,
	InforminRecordASP = 31,
	InforminRecordPPN = 32,
	InforminRecordPPNR = 33,
	InforminRecordSPVT = 34,
	InforminRecordRSET = 35,
	InforminRecordDSET = 36,
	InforminRecordDPVT = 37,
	InforminRecordRDES = 38,
	InforminRecordLSET = 39,
	InforminRecordPRIO = 40,
	InforminRecordTPRO = 41,
	InforminRecordBKPT = 42,
	InforminRecordUDF = 43,
	InforminRecordUDFS = 44,
	InforminRecordTIME = 45,
	InforminRecordUTAG = 46,
	InforminRecordFLNK = 47,
	InforminRecordVAL = 48,
	InforminRecordINFORMCODE = 49,
	InforminRecordREADY = 50,
	InforminRecordOVAL = 51,
	InforminRecordINP = 52,
	InforminRecordMPST = 53,
	InforminRecordAPST = 54,
	InforminRecordSIOL = 55,
	InforminRecordSVAL = 56,
	InforminRecordSIML = 57,
	InforminRecordSIMM = 58,
	InforminRecordSIMS = 59
} InforminFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsExport.h>
#include <cantProceed.h>
#ifdef __cplusplus
extern "C" {
#endif
static int InforminRecordSizeOffset(dbRecordType *prt)
{
    InforminRecord *prec = 0;

    if (prt->no_fields != 60) {
        cantProceed("IOC build or installation error:\n"
            "    The InforminRecord defined in the DBD file has %d fields,\n"
            "    but the record support code was built with 60.\n",
            prt->no_fields);
    }
    prt->papFldDes[InforminRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[InforminRecordNAME]->offset = (unsigned short)offsetof(InforminRecord, name);
    prt->papFldDes[InforminRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[InforminRecordDESC]->offset = (unsigned short)offsetof(InforminRecord, desc);
    prt->papFldDes[InforminRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[InforminRecordASG]->offset = (unsigned short)offsetof(InforminRecord, asg);
    prt->papFldDes[InforminRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[InforminRecordSCAN]->offset = (unsigned short)offsetof(InforminRecord, scan);
    prt->papFldDes[InforminRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[InforminRecordPINI]->offset = (unsigned short)offsetof(InforminRecord, pini);
    prt->papFldDes[InforminRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[InforminRecordPHAS]->offset = (unsigned short)offsetof(InforminRecord, phas);
    prt->papFldDes[InforminRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[InforminRecordEVNT]->offset = (unsigned short)offsetof(InforminRecord, evnt);
    prt->papFldDes[InforminRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[InforminRecordTSE]->offset = (unsigned short)offsetof(InforminRecord, tse);
    prt->papFldDes[InforminRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[InforminRecordTSEL]->offset = (unsigned short)offsetof(InforminRecord, tsel);
    prt->papFldDes[InforminRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[InforminRecordDTYP]->offset = (unsigned short)offsetof(InforminRecord, dtyp);
    prt->papFldDes[InforminRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[InforminRecordDISV]->offset = (unsigned short)offsetof(InforminRecord, disv);
    prt->papFldDes[InforminRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[InforminRecordDISA]->offset = (unsigned short)offsetof(InforminRecord, disa);
    prt->papFldDes[InforminRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[InforminRecordSDIS]->offset = (unsigned short)offsetof(InforminRecord, sdis);
    prt->papFldDes[InforminRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[InforminRecordMLOK]->offset = (unsigned short)offsetof(InforminRecord, mlok);
    prt->papFldDes[InforminRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[InforminRecordMLIS]->offset = (unsigned short)offsetof(InforminRecord, mlis);
    prt->papFldDes[InforminRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[InforminRecordBKLNK]->offset = (unsigned short)offsetof(InforminRecord, bklnk);
    prt->papFldDes[InforminRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[InforminRecordDISP]->offset = (unsigned short)offsetof(InforminRecord, disp);
    prt->papFldDes[InforminRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[InforminRecordPROC]->offset = (unsigned short)offsetof(InforminRecord, proc);
    prt->papFldDes[InforminRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[InforminRecordSTAT]->offset = (unsigned short)offsetof(InforminRecord, stat);
    prt->papFldDes[InforminRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[InforminRecordSEVR]->offset = (unsigned short)offsetof(InforminRecord, sevr);
    prt->papFldDes[InforminRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[InforminRecordAMSG]->offset = (unsigned short)offsetof(InforminRecord, amsg);
    prt->papFldDes[InforminRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[InforminRecordNSTA]->offset = (unsigned short)offsetof(InforminRecord, nsta);
    prt->papFldDes[InforminRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[InforminRecordNSEV]->offset = (unsigned short)offsetof(InforminRecord, nsev);
    prt->papFldDes[InforminRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[InforminRecordNAMSG]->offset = (unsigned short)offsetof(InforminRecord, namsg);
    prt->papFldDes[InforminRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[InforminRecordACKS]->offset = (unsigned short)offsetof(InforminRecord, acks);
    prt->papFldDes[InforminRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[InforminRecordACKT]->offset = (unsigned short)offsetof(InforminRecord, ackt);
    prt->papFldDes[InforminRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[InforminRecordDISS]->offset = (unsigned short)offsetof(InforminRecord, diss);
    prt->papFldDes[InforminRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[InforminRecordLCNT]->offset = (unsigned short)offsetof(InforminRecord, lcnt);
    prt->papFldDes[InforminRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[InforminRecordPACT]->offset = (unsigned short)offsetof(InforminRecord, pact);
    prt->papFldDes[InforminRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[InforminRecordPUTF]->offset = (unsigned short)offsetof(InforminRecord, putf);
    prt->papFldDes[InforminRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[InforminRecordRPRO]->offset = (unsigned short)offsetof(InforminRecord, rpro);
    prt->papFldDes[InforminRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[InforminRecordASP]->offset = (unsigned short)offsetof(InforminRecord, asp);
    prt->papFldDes[InforminRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[InforminRecordPPN]->offset = (unsigned short)offsetof(InforminRecord, ppn);
    prt->papFldDes[InforminRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[InforminRecordPPNR]->offset = (unsigned short)offsetof(InforminRecord, ppnr);
    prt->papFldDes[InforminRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[InforminRecordSPVT]->offset = (unsigned short)offsetof(InforminRecord, spvt);
    prt->papFldDes[InforminRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[InforminRecordRSET]->offset = (unsigned short)offsetof(InforminRecord, rset);
    prt->papFldDes[InforminRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[InforminRecordDSET]->offset = (unsigned short)offsetof(InforminRecord, dset);
    prt->papFldDes[InforminRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[InforminRecordDPVT]->offset = (unsigned short)offsetof(InforminRecord, dpvt);
    prt->papFldDes[InforminRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[InforminRecordRDES]->offset = (unsigned short)offsetof(InforminRecord, rdes);
    prt->papFldDes[InforminRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[InforminRecordLSET]->offset = (unsigned short)offsetof(InforminRecord, lset);
    prt->papFldDes[InforminRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[InforminRecordPRIO]->offset = (unsigned short)offsetof(InforminRecord, prio);
    prt->papFldDes[InforminRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[InforminRecordTPRO]->offset = (unsigned short)offsetof(InforminRecord, tpro);
    prt->papFldDes[InforminRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[InforminRecordBKPT]->offset = (unsigned short)offsetof(InforminRecord, bkpt);
    prt->papFldDes[InforminRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[InforminRecordUDF]->offset = (unsigned short)offsetof(InforminRecord, udf);
    prt->papFldDes[InforminRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[InforminRecordUDFS]->offset = (unsigned short)offsetof(InforminRecord, udfs);
    prt->papFldDes[InforminRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[InforminRecordTIME]->offset = (unsigned short)offsetof(InforminRecord, time);
    prt->papFldDes[InforminRecordUTAG]->size = sizeof(prec->utag);
    prt->papFldDes[InforminRecordUTAG]->offset = (unsigned short)offsetof(InforminRecord, utag);
    prt->papFldDes[InforminRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[InforminRecordFLNK]->offset = (unsigned short)offsetof(InforminRecord, flnk);
    prt->papFldDes[InforminRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[InforminRecordVAL]->offset = (unsigned short)offsetof(InforminRecord, val);
    prt->papFldDes[InforminRecordINFORMCODE]->size = sizeof(prec->informcode);
    prt->papFldDes[InforminRecordINFORMCODE]->offset = (unsigned short)offsetof(InforminRecord, informcode);
    prt->papFldDes[InforminRecordREADY]->size = sizeof(prec->ready);
    prt->papFldDes[InforminRecordREADY]->offset = (unsigned short)offsetof(InforminRecord, ready);
    prt->papFldDes[InforminRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[InforminRecordOVAL]->offset = (unsigned short)offsetof(InforminRecord, oval);
    prt->papFldDes[InforminRecordINP]->size = sizeof(prec->inp);
    prt->papFldDes[InforminRecordINP]->offset = (unsigned short)offsetof(InforminRecord, inp);
    prt->papFldDes[InforminRecordMPST]->size = sizeof(prec->mpst);
    prt->papFldDes[InforminRecordMPST]->offset = (unsigned short)offsetof(InforminRecord, mpst);
    prt->papFldDes[InforminRecordAPST]->size = sizeof(prec->apst);
    prt->papFldDes[InforminRecordAPST]->offset = (unsigned short)offsetof(InforminRecord, apst);
    prt->papFldDes[InforminRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[InforminRecordSIOL]->offset = (unsigned short)offsetof(InforminRecord, siol);
    prt->papFldDes[InforminRecordSVAL]->size = sizeof(prec->sval);
    prt->papFldDes[InforminRecordSVAL]->offset = (unsigned short)offsetof(InforminRecord, sval);
    prt->papFldDes[InforminRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[InforminRecordSIML]->offset = (unsigned short)offsetof(InforminRecord, siml);
    prt->papFldDes[InforminRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[InforminRecordSIMM]->offset = (unsigned short)offsetof(InforminRecord, simm);
    prt->papFldDes[InforminRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[InforminRecordSIMS]->offset = (unsigned short)offsetof(InforminRecord, sims);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(InforminRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_InforminRecord_H */
