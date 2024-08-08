/** @file DigitalScanPSRecord.h
 * @brief Declarations for the @ref DigitalScanPSRecord "DigitalScanPS" record type.
 *
 * This header was generated from DigitalScanPSRecord.dbd
 */

#ifndef INC_DigitalScanPSRecord_H
#define INC_DigitalScanPSRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "devSup.h"
#include "epicsTime.h"

#ifndef aoOIF_NUM_CHOICES
/** @brief Enumerated type from menu aoOIF */
typedef enum {
    aoOIF_Full                      /**< @brief State string "Full" */,
    aoOIF_Incremental               /**< @brief State string "Incremental" */
} aoOIF;
/** @brief Number of states defined for menu aoOIF */
#define aoOIF_NUM_CHOICES 2
#endif

/** @brief Declaration of DigitalScanPS record type. */
typedef struct DigitalScanPSRecord {
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
    epicsFloat64        val;        /**< @brief return current val from digital PS */
    epicsFloat64        oval;       /**< @brief Output Value */
    epicsInt16          opi_conn;   /**< @brief The number of connections from OPI */
    epicsUInt32         state;      /**< @brief The state of PS */
    epicsUInt32         lstat;      /**< @brief Last state val */
    epicsFloat64        oroc;       /**< @brief Output Rate of Chang */
    char                command[10];/**< @brief the command for operating PS */
    char                lcomm[10];  /**< @brief the type of ioc, ai or ao */
    epicsFloat32        freq_val;   /**< @brief data of frequency will send to scan PS */
    epicsFloat32        amp_val;    /**< @brief data of amplitude will send to scan PS */
    epicsFloat32        scan_end_val;/**< @brief use to scan mode PS */
    epicsInt16          scan_steps; /**< @brief total scan steps */
    epicsInt16          scan_start; /**< @brief a start command from scan PV */
    char                ip_addr[16];/**< @brief ip address of PS */
    char                port[10];   /**< @brief ip port of PS */
    char                dev_chan[10];/**< @brief channel of device */
    DBLINK              out;        /**< @brief Output Specification */
    DBLINK              out1;       /**< @brief Output Specification */
    DBLINK              out2;       /**< @brief Output Specification */
    DBLINK              dol;        /**< @brief Desired Output Loc */
    epicsEnum16         omsl;       /**< @brief Output Mode Select */
    epicsEnum16         oif;        /**< @brief Out Full/Incremental */
    epicsInt16          prec;       /**< @brief Display Precision */
    epicsEnum16         linr;       /**< @brief Linearization */
    epicsFloat64        eguf;       /**< @brief Eng Units Full */
    epicsFloat64        egul;       /**< @brief Eng Units Low */
    char                egu[16];    /**< @brief Engineering Units */
    epicsInt32          roff;       /**< @brief Raw Offset, obsolete */
    epicsFloat64        eoff;       /**< @brief EGU to Raw Offset */
    epicsFloat64        eslo;       /**< @brief EGU to Raw Slope */
    epicsFloat64        drvh;       /**< @brief Drive High Limit */
    epicsFloat64        drvl;       /**< @brief Drive Low Limit */
    epicsFloat64        hopr;       /**< @brief High Operating Range */
    epicsFloat64        lopr;       /**< @brief Low Operating Range */
    epicsFloat64        aoff;       /**< @brief Adjustment Offset */
    epicsFloat64        aslo;       /**< @brief Adjustment Slope */
    epicsFloat64        hihi;       /**< @brief Hihi Alarm Limit */
    epicsFloat64        lolo;       /**< @brief Lolo Alarm Limit */
    epicsFloat64        high;       /**< @brief High Alarm Limit */
    epicsFloat64        low;        /**< @brief Low Alarm Limit */
    epicsEnum16         hhsv;       /**< @brief Hihi Severity */
    epicsEnum16         llsv;       /**< @brief Lolo Severity */
    epicsEnum16         hsv;        /**< @brief High Severity */
    epicsEnum16         lsv;        /**< @brief Low Severity */
    epicsFloat64        hyst;       /**< @brief Alarm Deadband */
    epicsFloat64        adel;       /**< @brief Archive Deadband */
    epicsFloat64        mdel;       /**< @brief Monitor Deadband */
    epicsInt32          rval;       /**< @brief Current Raw Value */
    epicsInt32          oraw;       /**< @brief Previous Raw Value */
    epicsInt32          rbv;        /**< @brief Readback Value */
    epicsInt32          orbv;       /**< @brief Prev Readback Value */
    epicsFloat64        pval;       /**< @brief Previous value */
    epicsFloat64        lalm;       /**< @brief Last Value Alarmed */
    epicsFloat64        alst;       /**< @brief Last Value Archived */
    epicsFloat64        mlst;       /**< @brief Last Val Monitored */
    void *	pbrk;                    /**< @brief Ptrto brkTable */
    epicsInt16          init;       /**< @brief Initialized? */
    epicsInt16          lbrk;       /**< @brief LastBreak Point */
    DBLINK              siol;       /**< @brief Sim Output Specifctn */
    DBLINK              siml;       /**< @brief Sim Mode Location */
    epicsEnum16         simm;       /**< @brief Simulation Mode */
    epicsEnum16         sims;       /**< @brief Sim mode Alarm Svrty */
    epicsEnum16         ivoa;       /**< @brief INVALID output action */
    epicsFloat64        ivov;       /**< @brief INVALID output value */
    epicsUInt8          omod;       /**< @brief Was OVAL modified? */
} DigitalScanPSRecord;

typedef enum {
	DigitalScanPSRecordNAME = 0,
	DigitalScanPSRecordDESC = 1,
	DigitalScanPSRecordASG = 2,
	DigitalScanPSRecordSCAN = 3,
	DigitalScanPSRecordPINI = 4,
	DigitalScanPSRecordPHAS = 5,
	DigitalScanPSRecordEVNT = 6,
	DigitalScanPSRecordTSE = 7,
	DigitalScanPSRecordTSEL = 8,
	DigitalScanPSRecordDTYP = 9,
	DigitalScanPSRecordDISV = 10,
	DigitalScanPSRecordDISA = 11,
	DigitalScanPSRecordSDIS = 12,
	DigitalScanPSRecordMLOK = 13,
	DigitalScanPSRecordMLIS = 14,
	DigitalScanPSRecordBKLNK = 15,
	DigitalScanPSRecordDISP = 16,
	DigitalScanPSRecordPROC = 17,
	DigitalScanPSRecordSTAT = 18,
	DigitalScanPSRecordSEVR = 19,
	DigitalScanPSRecordAMSG = 20,
	DigitalScanPSRecordNSTA = 21,
	DigitalScanPSRecordNSEV = 22,
	DigitalScanPSRecordNAMSG = 23,
	DigitalScanPSRecordACKS = 24,
	DigitalScanPSRecordACKT = 25,
	DigitalScanPSRecordDISS = 26,
	DigitalScanPSRecordLCNT = 27,
	DigitalScanPSRecordPACT = 28,
	DigitalScanPSRecordPUTF = 29,
	DigitalScanPSRecordRPRO = 30,
	DigitalScanPSRecordASP = 31,
	DigitalScanPSRecordPPN = 32,
	DigitalScanPSRecordPPNR = 33,
	DigitalScanPSRecordSPVT = 34,
	DigitalScanPSRecordRSET = 35,
	DigitalScanPSRecordDSET = 36,
	DigitalScanPSRecordDPVT = 37,
	DigitalScanPSRecordRDES = 38,
	DigitalScanPSRecordLSET = 39,
	DigitalScanPSRecordPRIO = 40,
	DigitalScanPSRecordTPRO = 41,
	DigitalScanPSRecordBKPT = 42,
	DigitalScanPSRecordUDF = 43,
	DigitalScanPSRecordUDFS = 44,
	DigitalScanPSRecordTIME = 45,
	DigitalScanPSRecordUTAG = 46,
	DigitalScanPSRecordFLNK = 47,
	DigitalScanPSRecordVAL = 48,
	DigitalScanPSRecordOVAL = 49,
	DigitalScanPSRecordOPI_CONN = 50,
	DigitalScanPSRecordSTATE = 51,
	DigitalScanPSRecordLSTAT = 52,
	DigitalScanPSRecordOROC = 53,
	DigitalScanPSRecordCOMMAND = 54,
	DigitalScanPSRecordLCOMM = 55,
	DigitalScanPSRecordFREQ_VAL = 56,
	DigitalScanPSRecordAMP_VAL = 57,
	DigitalScanPSRecordSCAN_END_VAL = 58,
	DigitalScanPSRecordSCAN_STEPS = 59,
	DigitalScanPSRecordSCAN_START = 60,
	DigitalScanPSRecordIP_ADDR = 61,
	DigitalScanPSRecordPORT = 62,
	DigitalScanPSRecordDEV_CHAN = 63,
	DigitalScanPSRecordOUT = 64,
	DigitalScanPSRecordOUT1 = 65,
	DigitalScanPSRecordOUT2 = 66,
	DigitalScanPSRecordDOL = 67,
	DigitalScanPSRecordOMSL = 68,
	DigitalScanPSRecordOIF = 69,
	DigitalScanPSRecordPREC = 70,
	DigitalScanPSRecordLINR = 71,
	DigitalScanPSRecordEGUF = 72,
	DigitalScanPSRecordEGUL = 73,
	DigitalScanPSRecordEGU = 74,
	DigitalScanPSRecordROFF = 75,
	DigitalScanPSRecordEOFF = 76,
	DigitalScanPSRecordESLO = 77,
	DigitalScanPSRecordDRVH = 78,
	DigitalScanPSRecordDRVL = 79,
	DigitalScanPSRecordHOPR = 80,
	DigitalScanPSRecordLOPR = 81,
	DigitalScanPSRecordAOFF = 82,
	DigitalScanPSRecordASLO = 83,
	DigitalScanPSRecordHIHI = 84,
	DigitalScanPSRecordLOLO = 85,
	DigitalScanPSRecordHIGH = 86,
	DigitalScanPSRecordLOW = 87,
	DigitalScanPSRecordHHSV = 88,
	DigitalScanPSRecordLLSV = 89,
	DigitalScanPSRecordHSV = 90,
	DigitalScanPSRecordLSV = 91,
	DigitalScanPSRecordHYST = 92,
	DigitalScanPSRecordADEL = 93,
	DigitalScanPSRecordMDEL = 94,
	DigitalScanPSRecordRVAL = 95,
	DigitalScanPSRecordORAW = 96,
	DigitalScanPSRecordRBV = 97,
	DigitalScanPSRecordORBV = 98,
	DigitalScanPSRecordPVAL = 99,
	DigitalScanPSRecordLALM = 100,
	DigitalScanPSRecordALST = 101,
	DigitalScanPSRecordMLST = 102,
	DigitalScanPSRecordPBRK = 103,
	DigitalScanPSRecordINIT = 104,
	DigitalScanPSRecordLBRK = 105,
	DigitalScanPSRecordSIOL = 106,
	DigitalScanPSRecordSIML = 107,
	DigitalScanPSRecordSIMM = 108,
	DigitalScanPSRecordSIMS = 109,
	DigitalScanPSRecordIVOA = 110,
	DigitalScanPSRecordIVOV = 111,
	DigitalScanPSRecordOMOD = 112
} DigitalScanPSFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsExport.h>
#include <cantProceed.h>
#ifdef __cplusplus
extern "C" {
#endif
static int DigitalScanPSRecordSizeOffset(dbRecordType *prt)
{
    DigitalScanPSRecord *prec = 0;

    if (prt->no_fields != 113) {
        cantProceed("IOC build or installation error:\n"
            "    The DigitalScanPSRecord defined in the DBD file has %d fields,\n"
            "    but the record support code was built with 113.\n",
            prt->no_fields);
    }
    prt->papFldDes[DigitalScanPSRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[DigitalScanPSRecordNAME]->offset = (unsigned short)offsetof(DigitalScanPSRecord, name);
    prt->papFldDes[DigitalScanPSRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[DigitalScanPSRecordDESC]->offset = (unsigned short)offsetof(DigitalScanPSRecord, desc);
    prt->papFldDes[DigitalScanPSRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[DigitalScanPSRecordASG]->offset = (unsigned short)offsetof(DigitalScanPSRecord, asg);
    prt->papFldDes[DigitalScanPSRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[DigitalScanPSRecordSCAN]->offset = (unsigned short)offsetof(DigitalScanPSRecord, scan);
    prt->papFldDes[DigitalScanPSRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[DigitalScanPSRecordPINI]->offset = (unsigned short)offsetof(DigitalScanPSRecord, pini);
    prt->papFldDes[DigitalScanPSRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[DigitalScanPSRecordPHAS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, phas);
    prt->papFldDes[DigitalScanPSRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[DigitalScanPSRecordEVNT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, evnt);
    prt->papFldDes[DigitalScanPSRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[DigitalScanPSRecordTSE]->offset = (unsigned short)offsetof(DigitalScanPSRecord, tse);
    prt->papFldDes[DigitalScanPSRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[DigitalScanPSRecordTSEL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, tsel);
    prt->papFldDes[DigitalScanPSRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[DigitalScanPSRecordDTYP]->offset = (unsigned short)offsetof(DigitalScanPSRecord, dtyp);
    prt->papFldDes[DigitalScanPSRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[DigitalScanPSRecordDISV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, disv);
    prt->papFldDes[DigitalScanPSRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[DigitalScanPSRecordDISA]->offset = (unsigned short)offsetof(DigitalScanPSRecord, disa);
    prt->papFldDes[DigitalScanPSRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[DigitalScanPSRecordSDIS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, sdis);
    prt->papFldDes[DigitalScanPSRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[DigitalScanPSRecordMLOK]->offset = (unsigned short)offsetof(DigitalScanPSRecord, mlok);
    prt->papFldDes[DigitalScanPSRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[DigitalScanPSRecordMLIS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, mlis);
    prt->papFldDes[DigitalScanPSRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[DigitalScanPSRecordBKLNK]->offset = (unsigned short)offsetof(DigitalScanPSRecord, bklnk);
    prt->papFldDes[DigitalScanPSRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[DigitalScanPSRecordDISP]->offset = (unsigned short)offsetof(DigitalScanPSRecord, disp);
    prt->papFldDes[DigitalScanPSRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[DigitalScanPSRecordPROC]->offset = (unsigned short)offsetof(DigitalScanPSRecord, proc);
    prt->papFldDes[DigitalScanPSRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[DigitalScanPSRecordSTAT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, stat);
    prt->papFldDes[DigitalScanPSRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[DigitalScanPSRecordSEVR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, sevr);
    prt->papFldDes[DigitalScanPSRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[DigitalScanPSRecordAMSG]->offset = (unsigned short)offsetof(DigitalScanPSRecord, amsg);
    prt->papFldDes[DigitalScanPSRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[DigitalScanPSRecordNSTA]->offset = (unsigned short)offsetof(DigitalScanPSRecord, nsta);
    prt->papFldDes[DigitalScanPSRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[DigitalScanPSRecordNSEV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, nsev);
    prt->papFldDes[DigitalScanPSRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[DigitalScanPSRecordNAMSG]->offset = (unsigned short)offsetof(DigitalScanPSRecord, namsg);
    prt->papFldDes[DigitalScanPSRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[DigitalScanPSRecordACKS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, acks);
    prt->papFldDes[DigitalScanPSRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[DigitalScanPSRecordACKT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ackt);
    prt->papFldDes[DigitalScanPSRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[DigitalScanPSRecordDISS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, diss);
    prt->papFldDes[DigitalScanPSRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[DigitalScanPSRecordLCNT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lcnt);
    prt->papFldDes[DigitalScanPSRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[DigitalScanPSRecordPACT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, pact);
    prt->papFldDes[DigitalScanPSRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[DigitalScanPSRecordPUTF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, putf);
    prt->papFldDes[DigitalScanPSRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[DigitalScanPSRecordRPRO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, rpro);
    prt->papFldDes[DigitalScanPSRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[DigitalScanPSRecordASP]->offset = (unsigned short)offsetof(DigitalScanPSRecord, asp);
    prt->papFldDes[DigitalScanPSRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[DigitalScanPSRecordPPN]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ppn);
    prt->papFldDes[DigitalScanPSRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[DigitalScanPSRecordPPNR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ppnr);
    prt->papFldDes[DigitalScanPSRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[DigitalScanPSRecordSPVT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, spvt);
    prt->papFldDes[DigitalScanPSRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[DigitalScanPSRecordRSET]->offset = (unsigned short)offsetof(DigitalScanPSRecord, rset);
    prt->papFldDes[DigitalScanPSRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[DigitalScanPSRecordDSET]->offset = (unsigned short)offsetof(DigitalScanPSRecord, dset);
    prt->papFldDes[DigitalScanPSRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[DigitalScanPSRecordDPVT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, dpvt);
    prt->papFldDes[DigitalScanPSRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[DigitalScanPSRecordRDES]->offset = (unsigned short)offsetof(DigitalScanPSRecord, rdes);
    prt->papFldDes[DigitalScanPSRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[DigitalScanPSRecordLSET]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lset);
    prt->papFldDes[DigitalScanPSRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[DigitalScanPSRecordPRIO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, prio);
    prt->papFldDes[DigitalScanPSRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[DigitalScanPSRecordTPRO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, tpro);
    prt->papFldDes[DigitalScanPSRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[DigitalScanPSRecordBKPT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, bkpt);
    prt->papFldDes[DigitalScanPSRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[DigitalScanPSRecordUDF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, udf);
    prt->papFldDes[DigitalScanPSRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[DigitalScanPSRecordUDFS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, udfs);
    prt->papFldDes[DigitalScanPSRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[DigitalScanPSRecordTIME]->offset = (unsigned short)offsetof(DigitalScanPSRecord, time);
    prt->papFldDes[DigitalScanPSRecordUTAG]->size = sizeof(prec->utag);
    prt->papFldDes[DigitalScanPSRecordUTAG]->offset = (unsigned short)offsetof(DigitalScanPSRecord, utag);
    prt->papFldDes[DigitalScanPSRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[DigitalScanPSRecordFLNK]->offset = (unsigned short)offsetof(DigitalScanPSRecord, flnk);
    prt->papFldDes[DigitalScanPSRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[DigitalScanPSRecordVAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, val);
    prt->papFldDes[DigitalScanPSRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[DigitalScanPSRecordOVAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, oval);
    prt->papFldDes[DigitalScanPSRecordOPI_CONN]->size = sizeof(prec->opi_conn);
    prt->papFldDes[DigitalScanPSRecordOPI_CONN]->offset = (unsigned short)offsetof(DigitalScanPSRecord, opi_conn);
    prt->papFldDes[DigitalScanPSRecordSTATE]->size = sizeof(prec->state);
    prt->papFldDes[DigitalScanPSRecordSTATE]->offset = (unsigned short)offsetof(DigitalScanPSRecord, state);
    prt->papFldDes[DigitalScanPSRecordLSTAT]->size = sizeof(prec->lstat);
    prt->papFldDes[DigitalScanPSRecordLSTAT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lstat);
    prt->papFldDes[DigitalScanPSRecordOROC]->size = sizeof(prec->oroc);
    prt->papFldDes[DigitalScanPSRecordOROC]->offset = (unsigned short)offsetof(DigitalScanPSRecord, oroc);
    prt->papFldDes[DigitalScanPSRecordCOMMAND]->size = sizeof(prec->command);
    prt->papFldDes[DigitalScanPSRecordCOMMAND]->offset = (unsigned short)offsetof(DigitalScanPSRecord, command);
    prt->papFldDes[DigitalScanPSRecordLCOMM]->size = sizeof(prec->lcomm);
    prt->papFldDes[DigitalScanPSRecordLCOMM]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lcomm);
    prt->papFldDes[DigitalScanPSRecordFREQ_VAL]->size = sizeof(prec->freq_val);
    prt->papFldDes[DigitalScanPSRecordFREQ_VAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, freq_val);
    prt->papFldDes[DigitalScanPSRecordAMP_VAL]->size = sizeof(prec->amp_val);
    prt->papFldDes[DigitalScanPSRecordAMP_VAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, amp_val);
    prt->papFldDes[DigitalScanPSRecordSCAN_END_VAL]->size = sizeof(prec->scan_end_val);
    prt->papFldDes[DigitalScanPSRecordSCAN_END_VAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, scan_end_val);
    prt->papFldDes[DigitalScanPSRecordSCAN_STEPS]->size = sizeof(prec->scan_steps);
    prt->papFldDes[DigitalScanPSRecordSCAN_STEPS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, scan_steps);
    prt->papFldDes[DigitalScanPSRecordSCAN_START]->size = sizeof(prec->scan_start);
    prt->papFldDes[DigitalScanPSRecordSCAN_START]->offset = (unsigned short)offsetof(DigitalScanPSRecord, scan_start);
    prt->papFldDes[DigitalScanPSRecordIP_ADDR]->size = sizeof(prec->ip_addr);
    prt->papFldDes[DigitalScanPSRecordIP_ADDR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ip_addr);
    prt->papFldDes[DigitalScanPSRecordPORT]->size = sizeof(prec->port);
    prt->papFldDes[DigitalScanPSRecordPORT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, port);
    prt->papFldDes[DigitalScanPSRecordDEV_CHAN]->size = sizeof(prec->dev_chan);
    prt->papFldDes[DigitalScanPSRecordDEV_CHAN]->offset = (unsigned short)offsetof(DigitalScanPSRecord, dev_chan);
    prt->papFldDes[DigitalScanPSRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[DigitalScanPSRecordOUT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, out);
    prt->papFldDes[DigitalScanPSRecordOUT1]->size = sizeof(prec->out1);
    prt->papFldDes[DigitalScanPSRecordOUT1]->offset = (unsigned short)offsetof(DigitalScanPSRecord, out1);
    prt->papFldDes[DigitalScanPSRecordOUT2]->size = sizeof(prec->out2);
    prt->papFldDes[DigitalScanPSRecordOUT2]->offset = (unsigned short)offsetof(DigitalScanPSRecord, out2);
    prt->papFldDes[DigitalScanPSRecordDOL]->size = sizeof(prec->dol);
    prt->papFldDes[DigitalScanPSRecordDOL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, dol);
    prt->papFldDes[DigitalScanPSRecordOMSL]->size = sizeof(prec->omsl);
    prt->papFldDes[DigitalScanPSRecordOMSL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, omsl);
    prt->papFldDes[DigitalScanPSRecordOIF]->size = sizeof(prec->oif);
    prt->papFldDes[DigitalScanPSRecordOIF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, oif);
    prt->papFldDes[DigitalScanPSRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[DigitalScanPSRecordPREC]->offset = (unsigned short)offsetof(DigitalScanPSRecord, prec);
    prt->papFldDes[DigitalScanPSRecordLINR]->size = sizeof(prec->linr);
    prt->papFldDes[DigitalScanPSRecordLINR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, linr);
    prt->papFldDes[DigitalScanPSRecordEGUF]->size = sizeof(prec->eguf);
    prt->papFldDes[DigitalScanPSRecordEGUF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, eguf);
    prt->papFldDes[DigitalScanPSRecordEGUL]->size = sizeof(prec->egul);
    prt->papFldDes[DigitalScanPSRecordEGUL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, egul);
    prt->papFldDes[DigitalScanPSRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[DigitalScanPSRecordEGU]->offset = (unsigned short)offsetof(DigitalScanPSRecord, egu);
    prt->papFldDes[DigitalScanPSRecordROFF]->size = sizeof(prec->roff);
    prt->papFldDes[DigitalScanPSRecordROFF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, roff);
    prt->papFldDes[DigitalScanPSRecordEOFF]->size = sizeof(prec->eoff);
    prt->papFldDes[DigitalScanPSRecordEOFF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, eoff);
    prt->papFldDes[DigitalScanPSRecordESLO]->size = sizeof(prec->eslo);
    prt->papFldDes[DigitalScanPSRecordESLO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, eslo);
    prt->papFldDes[DigitalScanPSRecordDRVH]->size = sizeof(prec->drvh);
    prt->papFldDes[DigitalScanPSRecordDRVH]->offset = (unsigned short)offsetof(DigitalScanPSRecord, drvh);
    prt->papFldDes[DigitalScanPSRecordDRVL]->size = sizeof(prec->drvl);
    prt->papFldDes[DigitalScanPSRecordDRVL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, drvl);
    prt->papFldDes[DigitalScanPSRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[DigitalScanPSRecordHOPR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, hopr);
    prt->papFldDes[DigitalScanPSRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[DigitalScanPSRecordLOPR]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lopr);
    prt->papFldDes[DigitalScanPSRecordAOFF]->size = sizeof(prec->aoff);
    prt->papFldDes[DigitalScanPSRecordAOFF]->offset = (unsigned short)offsetof(DigitalScanPSRecord, aoff);
    prt->papFldDes[DigitalScanPSRecordASLO]->size = sizeof(prec->aslo);
    prt->papFldDes[DigitalScanPSRecordASLO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, aslo);
    prt->papFldDes[DigitalScanPSRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[DigitalScanPSRecordHIHI]->offset = (unsigned short)offsetof(DigitalScanPSRecord, hihi);
    prt->papFldDes[DigitalScanPSRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[DigitalScanPSRecordLOLO]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lolo);
    prt->papFldDes[DigitalScanPSRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[DigitalScanPSRecordHIGH]->offset = (unsigned short)offsetof(DigitalScanPSRecord, high);
    prt->papFldDes[DigitalScanPSRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[DigitalScanPSRecordLOW]->offset = (unsigned short)offsetof(DigitalScanPSRecord, low);
    prt->papFldDes[DigitalScanPSRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[DigitalScanPSRecordHHSV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, hhsv);
    prt->papFldDes[DigitalScanPSRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[DigitalScanPSRecordLLSV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, llsv);
    prt->papFldDes[DigitalScanPSRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[DigitalScanPSRecordHSV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, hsv);
    prt->papFldDes[DigitalScanPSRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[DigitalScanPSRecordLSV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lsv);
    prt->papFldDes[DigitalScanPSRecordHYST]->size = sizeof(prec->hyst);
    prt->papFldDes[DigitalScanPSRecordHYST]->offset = (unsigned short)offsetof(DigitalScanPSRecord, hyst);
    prt->papFldDes[DigitalScanPSRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[DigitalScanPSRecordADEL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, adel);
    prt->papFldDes[DigitalScanPSRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[DigitalScanPSRecordMDEL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, mdel);
    prt->papFldDes[DigitalScanPSRecordRVAL]->size = sizeof(prec->rval);
    prt->papFldDes[DigitalScanPSRecordRVAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, rval);
    prt->papFldDes[DigitalScanPSRecordORAW]->size = sizeof(prec->oraw);
    prt->papFldDes[DigitalScanPSRecordORAW]->offset = (unsigned short)offsetof(DigitalScanPSRecord, oraw);
    prt->papFldDes[DigitalScanPSRecordRBV]->size = sizeof(prec->rbv);
    prt->papFldDes[DigitalScanPSRecordRBV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, rbv);
    prt->papFldDes[DigitalScanPSRecordORBV]->size = sizeof(prec->orbv);
    prt->papFldDes[DigitalScanPSRecordORBV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, orbv);
    prt->papFldDes[DigitalScanPSRecordPVAL]->size = sizeof(prec->pval);
    prt->papFldDes[DigitalScanPSRecordPVAL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, pval);
    prt->papFldDes[DigitalScanPSRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[DigitalScanPSRecordLALM]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lalm);
    prt->papFldDes[DigitalScanPSRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[DigitalScanPSRecordALST]->offset = (unsigned short)offsetof(DigitalScanPSRecord, alst);
    prt->papFldDes[DigitalScanPSRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[DigitalScanPSRecordMLST]->offset = (unsigned short)offsetof(DigitalScanPSRecord, mlst);
    prt->papFldDes[DigitalScanPSRecordPBRK]->size = sizeof(prec->pbrk);
    prt->papFldDes[DigitalScanPSRecordPBRK]->offset = (unsigned short)offsetof(DigitalScanPSRecord, pbrk);
    prt->papFldDes[DigitalScanPSRecordINIT]->size = sizeof(prec->init);
    prt->papFldDes[DigitalScanPSRecordINIT]->offset = (unsigned short)offsetof(DigitalScanPSRecord, init);
    prt->papFldDes[DigitalScanPSRecordLBRK]->size = sizeof(prec->lbrk);
    prt->papFldDes[DigitalScanPSRecordLBRK]->offset = (unsigned short)offsetof(DigitalScanPSRecord, lbrk);
    prt->papFldDes[DigitalScanPSRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[DigitalScanPSRecordSIOL]->offset = (unsigned short)offsetof(DigitalScanPSRecord, siol);
    prt->papFldDes[DigitalScanPSRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[DigitalScanPSRecordSIML]->offset = (unsigned short)offsetof(DigitalScanPSRecord, siml);
    prt->papFldDes[DigitalScanPSRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[DigitalScanPSRecordSIMM]->offset = (unsigned short)offsetof(DigitalScanPSRecord, simm);
    prt->papFldDes[DigitalScanPSRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[DigitalScanPSRecordSIMS]->offset = (unsigned short)offsetof(DigitalScanPSRecord, sims);
    prt->papFldDes[DigitalScanPSRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[DigitalScanPSRecordIVOA]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ivoa);
    prt->papFldDes[DigitalScanPSRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[DigitalScanPSRecordIVOV]->offset = (unsigned short)offsetof(DigitalScanPSRecord, ivov);
    prt->papFldDes[DigitalScanPSRecordOMOD]->size = sizeof(prec->omod);
    prt->papFldDes[DigitalScanPSRecordOMOD]->offset = (unsigned short)offsetof(DigitalScanPSRecord, omod);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(DigitalScanPSRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_DigitalScanPSRecord_H */
