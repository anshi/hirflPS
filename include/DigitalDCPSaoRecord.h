/** @file DigitalDCPSaoRecord.h
 * @brief Declarations for the @ref DigitalDCPSaoRecord "DigitalDCPSao" record type.
 *
 * This header was generated from DigitalDCPSaoRecord.dbd
 */

#ifndef INC_DigitalDCPSaoRecord_H
#define INC_DigitalDCPSaoRecord_H

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

/** @brief Declaration of DigitalDCPSao record type. */
typedef struct DigitalDCPSaoRecord {
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
    epicsFloat64        val_v;      /**< @brief return voltage val from digital PS */
    epicsFloat64        oval;       /**< @brief Output Value */
    epicsInt16          opi_conn;   /**< @brief The number of connections from OPI */
    epicsUInt32         state;      /**< @brief current ps state */
    epicsUInt32         lstat;      /**< @brief Last state val */
    char                psstat[40]; /**< @brief the command for operating PS */
    epicsFloat64        oroc;       /**< @brief Output Rate of Chang */
    char                command[10];/**< @brief the command for operating PS */
    char                lcomm[10];  /**< @brief the type of ioc, ai or ao */
    epicsFloat64        current_val;/**< @brief data of current will send to PS */
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
} DigitalDCPSaoRecord;

typedef enum {
	DigitalDCPSaoRecordNAME = 0,
	DigitalDCPSaoRecordDESC = 1,
	DigitalDCPSaoRecordASG = 2,
	DigitalDCPSaoRecordSCAN = 3,
	DigitalDCPSaoRecordPINI = 4,
	DigitalDCPSaoRecordPHAS = 5,
	DigitalDCPSaoRecordEVNT = 6,
	DigitalDCPSaoRecordTSE = 7,
	DigitalDCPSaoRecordTSEL = 8,
	DigitalDCPSaoRecordDTYP = 9,
	DigitalDCPSaoRecordDISV = 10,
	DigitalDCPSaoRecordDISA = 11,
	DigitalDCPSaoRecordSDIS = 12,
	DigitalDCPSaoRecordMLOK = 13,
	DigitalDCPSaoRecordMLIS = 14,
	DigitalDCPSaoRecordBKLNK = 15,
	DigitalDCPSaoRecordDISP = 16,
	DigitalDCPSaoRecordPROC = 17,
	DigitalDCPSaoRecordSTAT = 18,
	DigitalDCPSaoRecordSEVR = 19,
	DigitalDCPSaoRecordAMSG = 20,
	DigitalDCPSaoRecordNSTA = 21,
	DigitalDCPSaoRecordNSEV = 22,
	DigitalDCPSaoRecordNAMSG = 23,
	DigitalDCPSaoRecordACKS = 24,
	DigitalDCPSaoRecordACKT = 25,
	DigitalDCPSaoRecordDISS = 26,
	DigitalDCPSaoRecordLCNT = 27,
	DigitalDCPSaoRecordPACT = 28,
	DigitalDCPSaoRecordPUTF = 29,
	DigitalDCPSaoRecordRPRO = 30,
	DigitalDCPSaoRecordASP = 31,
	DigitalDCPSaoRecordPPN = 32,
	DigitalDCPSaoRecordPPNR = 33,
	DigitalDCPSaoRecordSPVT = 34,
	DigitalDCPSaoRecordRSET = 35,
	DigitalDCPSaoRecordDSET = 36,
	DigitalDCPSaoRecordDPVT = 37,
	DigitalDCPSaoRecordRDES = 38,
	DigitalDCPSaoRecordLSET = 39,
	DigitalDCPSaoRecordPRIO = 40,
	DigitalDCPSaoRecordTPRO = 41,
	DigitalDCPSaoRecordBKPT = 42,
	DigitalDCPSaoRecordUDF = 43,
	DigitalDCPSaoRecordUDFS = 44,
	DigitalDCPSaoRecordTIME = 45,
	DigitalDCPSaoRecordUTAG = 46,
	DigitalDCPSaoRecordFLNK = 47,
	DigitalDCPSaoRecordVAL = 48,
	DigitalDCPSaoRecordVAL_V = 49,
	DigitalDCPSaoRecordOVAL = 50,
	DigitalDCPSaoRecordOPI_CONN = 51,
	DigitalDCPSaoRecordSTATE = 52,
	DigitalDCPSaoRecordLSTAT = 53,
	DigitalDCPSaoRecordPSSTAT = 54,
	DigitalDCPSaoRecordOROC = 55,
	DigitalDCPSaoRecordCOMMAND = 56,
	DigitalDCPSaoRecordLCOMM = 57,
	DigitalDCPSaoRecordCURRENT_VAL = 58,
	DigitalDCPSaoRecordSCAN_END_VAL = 59,
	DigitalDCPSaoRecordSCAN_STEPS = 60,
	DigitalDCPSaoRecordSCAN_START = 61,
	DigitalDCPSaoRecordIP_ADDR = 62,
	DigitalDCPSaoRecordPORT = 63,
	DigitalDCPSaoRecordDEV_CHAN = 64,
	DigitalDCPSaoRecordOUT = 65,
	DigitalDCPSaoRecordOUT1 = 66,
	DigitalDCPSaoRecordOUT2 = 67,
	DigitalDCPSaoRecordDOL = 68,
	DigitalDCPSaoRecordOMSL = 69,
	DigitalDCPSaoRecordOIF = 70,
	DigitalDCPSaoRecordPREC = 71,
	DigitalDCPSaoRecordLINR = 72,
	DigitalDCPSaoRecordEGUF = 73,
	DigitalDCPSaoRecordEGUL = 74,
	DigitalDCPSaoRecordEGU = 75,
	DigitalDCPSaoRecordROFF = 76,
	DigitalDCPSaoRecordEOFF = 77,
	DigitalDCPSaoRecordESLO = 78,
	DigitalDCPSaoRecordDRVH = 79,
	DigitalDCPSaoRecordDRVL = 80,
	DigitalDCPSaoRecordHOPR = 81,
	DigitalDCPSaoRecordLOPR = 82,
	DigitalDCPSaoRecordAOFF = 83,
	DigitalDCPSaoRecordASLO = 84,
	DigitalDCPSaoRecordHIHI = 85,
	DigitalDCPSaoRecordLOLO = 86,
	DigitalDCPSaoRecordHIGH = 87,
	DigitalDCPSaoRecordLOW = 88,
	DigitalDCPSaoRecordHHSV = 89,
	DigitalDCPSaoRecordLLSV = 90,
	DigitalDCPSaoRecordHSV = 91,
	DigitalDCPSaoRecordLSV = 92,
	DigitalDCPSaoRecordHYST = 93,
	DigitalDCPSaoRecordADEL = 94,
	DigitalDCPSaoRecordMDEL = 95,
	DigitalDCPSaoRecordRVAL = 96,
	DigitalDCPSaoRecordORAW = 97,
	DigitalDCPSaoRecordRBV = 98,
	DigitalDCPSaoRecordORBV = 99,
	DigitalDCPSaoRecordPVAL = 100,
	DigitalDCPSaoRecordLALM = 101,
	DigitalDCPSaoRecordALST = 102,
	DigitalDCPSaoRecordMLST = 103,
	DigitalDCPSaoRecordPBRK = 104,
	DigitalDCPSaoRecordINIT = 105,
	DigitalDCPSaoRecordLBRK = 106,
	DigitalDCPSaoRecordSIOL = 107,
	DigitalDCPSaoRecordSIML = 108,
	DigitalDCPSaoRecordSIMM = 109,
	DigitalDCPSaoRecordSIMS = 110,
	DigitalDCPSaoRecordIVOA = 111,
	DigitalDCPSaoRecordIVOV = 112,
	DigitalDCPSaoRecordOMOD = 113
} DigitalDCPSaoFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsExport.h>
#include <cantProceed.h>
#ifdef __cplusplus
extern "C" {
#endif
static int DigitalDCPSaoRecordSizeOffset(dbRecordType *prt)
{
    DigitalDCPSaoRecord *prec = 0;

    if (prt->no_fields != 114) {
        cantProceed("IOC build or installation error:\n"
            "    The DigitalDCPSaoRecord defined in the DBD file has %d fields,\n"
            "    but the record support code was built with 114.\n",
            prt->no_fields);
    }
    prt->papFldDes[DigitalDCPSaoRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[DigitalDCPSaoRecordNAME]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, name);
    prt->papFldDes[DigitalDCPSaoRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[DigitalDCPSaoRecordDESC]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, desc);
    prt->papFldDes[DigitalDCPSaoRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[DigitalDCPSaoRecordASG]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, asg);
    prt->papFldDes[DigitalDCPSaoRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[DigitalDCPSaoRecordSCAN]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, scan);
    prt->papFldDes[DigitalDCPSaoRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[DigitalDCPSaoRecordPINI]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, pini);
    prt->papFldDes[DigitalDCPSaoRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[DigitalDCPSaoRecordPHAS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, phas);
    prt->papFldDes[DigitalDCPSaoRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[DigitalDCPSaoRecordEVNT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, evnt);
    prt->papFldDes[DigitalDCPSaoRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[DigitalDCPSaoRecordTSE]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, tse);
    prt->papFldDes[DigitalDCPSaoRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[DigitalDCPSaoRecordTSEL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, tsel);
    prt->papFldDes[DigitalDCPSaoRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[DigitalDCPSaoRecordDTYP]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, dtyp);
    prt->papFldDes[DigitalDCPSaoRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[DigitalDCPSaoRecordDISV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, disv);
    prt->papFldDes[DigitalDCPSaoRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[DigitalDCPSaoRecordDISA]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, disa);
    prt->papFldDes[DigitalDCPSaoRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[DigitalDCPSaoRecordSDIS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, sdis);
    prt->papFldDes[DigitalDCPSaoRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[DigitalDCPSaoRecordMLOK]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, mlok);
    prt->papFldDes[DigitalDCPSaoRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[DigitalDCPSaoRecordMLIS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, mlis);
    prt->papFldDes[DigitalDCPSaoRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[DigitalDCPSaoRecordBKLNK]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, bklnk);
    prt->papFldDes[DigitalDCPSaoRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[DigitalDCPSaoRecordDISP]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, disp);
    prt->papFldDes[DigitalDCPSaoRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[DigitalDCPSaoRecordPROC]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, proc);
    prt->papFldDes[DigitalDCPSaoRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[DigitalDCPSaoRecordSTAT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, stat);
    prt->papFldDes[DigitalDCPSaoRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[DigitalDCPSaoRecordSEVR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, sevr);
    prt->papFldDes[DigitalDCPSaoRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[DigitalDCPSaoRecordAMSG]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, amsg);
    prt->papFldDes[DigitalDCPSaoRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[DigitalDCPSaoRecordNSTA]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, nsta);
    prt->papFldDes[DigitalDCPSaoRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[DigitalDCPSaoRecordNSEV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, nsev);
    prt->papFldDes[DigitalDCPSaoRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[DigitalDCPSaoRecordNAMSG]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, namsg);
    prt->papFldDes[DigitalDCPSaoRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[DigitalDCPSaoRecordACKS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, acks);
    prt->papFldDes[DigitalDCPSaoRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[DigitalDCPSaoRecordACKT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ackt);
    prt->papFldDes[DigitalDCPSaoRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[DigitalDCPSaoRecordDISS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, diss);
    prt->papFldDes[DigitalDCPSaoRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[DigitalDCPSaoRecordLCNT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lcnt);
    prt->papFldDes[DigitalDCPSaoRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[DigitalDCPSaoRecordPACT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, pact);
    prt->papFldDes[DigitalDCPSaoRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[DigitalDCPSaoRecordPUTF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, putf);
    prt->papFldDes[DigitalDCPSaoRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[DigitalDCPSaoRecordRPRO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, rpro);
    prt->papFldDes[DigitalDCPSaoRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[DigitalDCPSaoRecordASP]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, asp);
    prt->papFldDes[DigitalDCPSaoRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[DigitalDCPSaoRecordPPN]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ppn);
    prt->papFldDes[DigitalDCPSaoRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[DigitalDCPSaoRecordPPNR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ppnr);
    prt->papFldDes[DigitalDCPSaoRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[DigitalDCPSaoRecordSPVT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, spvt);
    prt->papFldDes[DigitalDCPSaoRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[DigitalDCPSaoRecordRSET]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, rset);
    prt->papFldDes[DigitalDCPSaoRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[DigitalDCPSaoRecordDSET]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, dset);
    prt->papFldDes[DigitalDCPSaoRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[DigitalDCPSaoRecordDPVT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, dpvt);
    prt->papFldDes[DigitalDCPSaoRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[DigitalDCPSaoRecordRDES]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, rdes);
    prt->papFldDes[DigitalDCPSaoRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[DigitalDCPSaoRecordLSET]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lset);
    prt->papFldDes[DigitalDCPSaoRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[DigitalDCPSaoRecordPRIO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, prio);
    prt->papFldDes[DigitalDCPSaoRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[DigitalDCPSaoRecordTPRO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, tpro);
    prt->papFldDes[DigitalDCPSaoRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[DigitalDCPSaoRecordBKPT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, bkpt);
    prt->papFldDes[DigitalDCPSaoRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[DigitalDCPSaoRecordUDF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, udf);
    prt->papFldDes[DigitalDCPSaoRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[DigitalDCPSaoRecordUDFS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, udfs);
    prt->papFldDes[DigitalDCPSaoRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[DigitalDCPSaoRecordTIME]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, time);
    prt->papFldDes[DigitalDCPSaoRecordUTAG]->size = sizeof(prec->utag);
    prt->papFldDes[DigitalDCPSaoRecordUTAG]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, utag);
    prt->papFldDes[DigitalDCPSaoRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[DigitalDCPSaoRecordFLNK]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, flnk);
    prt->papFldDes[DigitalDCPSaoRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[DigitalDCPSaoRecordVAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, val);
    prt->papFldDes[DigitalDCPSaoRecordVAL_V]->size = sizeof(prec->val_v);
    prt->papFldDes[DigitalDCPSaoRecordVAL_V]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, val_v);
    prt->papFldDes[DigitalDCPSaoRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[DigitalDCPSaoRecordOVAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, oval);
    prt->papFldDes[DigitalDCPSaoRecordOPI_CONN]->size = sizeof(prec->opi_conn);
    prt->papFldDes[DigitalDCPSaoRecordOPI_CONN]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, opi_conn);
    prt->papFldDes[DigitalDCPSaoRecordSTATE]->size = sizeof(prec->state);
    prt->papFldDes[DigitalDCPSaoRecordSTATE]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, state);
    prt->papFldDes[DigitalDCPSaoRecordLSTAT]->size = sizeof(prec->lstat);
    prt->papFldDes[DigitalDCPSaoRecordLSTAT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lstat);
    prt->papFldDes[DigitalDCPSaoRecordPSSTAT]->size = sizeof(prec->psstat);
    prt->papFldDes[DigitalDCPSaoRecordPSSTAT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, psstat);
    prt->papFldDes[DigitalDCPSaoRecordOROC]->size = sizeof(prec->oroc);
    prt->papFldDes[DigitalDCPSaoRecordOROC]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, oroc);
    prt->papFldDes[DigitalDCPSaoRecordCOMMAND]->size = sizeof(prec->command);
    prt->papFldDes[DigitalDCPSaoRecordCOMMAND]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, command);
    prt->papFldDes[DigitalDCPSaoRecordLCOMM]->size = sizeof(prec->lcomm);
    prt->papFldDes[DigitalDCPSaoRecordLCOMM]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lcomm);
    prt->papFldDes[DigitalDCPSaoRecordCURRENT_VAL]->size = sizeof(prec->current_val);
    prt->papFldDes[DigitalDCPSaoRecordCURRENT_VAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, current_val);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_END_VAL]->size = sizeof(prec->scan_end_val);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_END_VAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, scan_end_val);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_STEPS]->size = sizeof(prec->scan_steps);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_STEPS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, scan_steps);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_START]->size = sizeof(prec->scan_start);
    prt->papFldDes[DigitalDCPSaoRecordSCAN_START]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, scan_start);
    prt->papFldDes[DigitalDCPSaoRecordIP_ADDR]->size = sizeof(prec->ip_addr);
    prt->papFldDes[DigitalDCPSaoRecordIP_ADDR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ip_addr);
    prt->papFldDes[DigitalDCPSaoRecordPORT]->size = sizeof(prec->port);
    prt->papFldDes[DigitalDCPSaoRecordPORT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, port);
    prt->papFldDes[DigitalDCPSaoRecordDEV_CHAN]->size = sizeof(prec->dev_chan);
    prt->papFldDes[DigitalDCPSaoRecordDEV_CHAN]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, dev_chan);
    prt->papFldDes[DigitalDCPSaoRecordOUT]->size = sizeof(prec->out);
    prt->papFldDes[DigitalDCPSaoRecordOUT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, out);
    prt->papFldDes[DigitalDCPSaoRecordOUT1]->size = sizeof(prec->out1);
    prt->papFldDes[DigitalDCPSaoRecordOUT1]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, out1);
    prt->papFldDes[DigitalDCPSaoRecordOUT2]->size = sizeof(prec->out2);
    prt->papFldDes[DigitalDCPSaoRecordOUT2]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, out2);
    prt->papFldDes[DigitalDCPSaoRecordDOL]->size = sizeof(prec->dol);
    prt->papFldDes[DigitalDCPSaoRecordDOL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, dol);
    prt->papFldDes[DigitalDCPSaoRecordOMSL]->size = sizeof(prec->omsl);
    prt->papFldDes[DigitalDCPSaoRecordOMSL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, omsl);
    prt->papFldDes[DigitalDCPSaoRecordOIF]->size = sizeof(prec->oif);
    prt->papFldDes[DigitalDCPSaoRecordOIF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, oif);
    prt->papFldDes[DigitalDCPSaoRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[DigitalDCPSaoRecordPREC]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, prec);
    prt->papFldDes[DigitalDCPSaoRecordLINR]->size = sizeof(prec->linr);
    prt->papFldDes[DigitalDCPSaoRecordLINR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, linr);
    prt->papFldDes[DigitalDCPSaoRecordEGUF]->size = sizeof(prec->eguf);
    prt->papFldDes[DigitalDCPSaoRecordEGUF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, eguf);
    prt->papFldDes[DigitalDCPSaoRecordEGUL]->size = sizeof(prec->egul);
    prt->papFldDes[DigitalDCPSaoRecordEGUL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, egul);
    prt->papFldDes[DigitalDCPSaoRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[DigitalDCPSaoRecordEGU]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, egu);
    prt->papFldDes[DigitalDCPSaoRecordROFF]->size = sizeof(prec->roff);
    prt->papFldDes[DigitalDCPSaoRecordROFF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, roff);
    prt->papFldDes[DigitalDCPSaoRecordEOFF]->size = sizeof(prec->eoff);
    prt->papFldDes[DigitalDCPSaoRecordEOFF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, eoff);
    prt->papFldDes[DigitalDCPSaoRecordESLO]->size = sizeof(prec->eslo);
    prt->papFldDes[DigitalDCPSaoRecordESLO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, eslo);
    prt->papFldDes[DigitalDCPSaoRecordDRVH]->size = sizeof(prec->drvh);
    prt->papFldDes[DigitalDCPSaoRecordDRVH]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, drvh);
    prt->papFldDes[DigitalDCPSaoRecordDRVL]->size = sizeof(prec->drvl);
    prt->papFldDes[DigitalDCPSaoRecordDRVL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, drvl);
    prt->papFldDes[DigitalDCPSaoRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[DigitalDCPSaoRecordHOPR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, hopr);
    prt->papFldDes[DigitalDCPSaoRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[DigitalDCPSaoRecordLOPR]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lopr);
    prt->papFldDes[DigitalDCPSaoRecordAOFF]->size = sizeof(prec->aoff);
    prt->papFldDes[DigitalDCPSaoRecordAOFF]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, aoff);
    prt->papFldDes[DigitalDCPSaoRecordASLO]->size = sizeof(prec->aslo);
    prt->papFldDes[DigitalDCPSaoRecordASLO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, aslo);
    prt->papFldDes[DigitalDCPSaoRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[DigitalDCPSaoRecordHIHI]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, hihi);
    prt->papFldDes[DigitalDCPSaoRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[DigitalDCPSaoRecordLOLO]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lolo);
    prt->papFldDes[DigitalDCPSaoRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[DigitalDCPSaoRecordHIGH]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, high);
    prt->papFldDes[DigitalDCPSaoRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[DigitalDCPSaoRecordLOW]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, low);
    prt->papFldDes[DigitalDCPSaoRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[DigitalDCPSaoRecordHHSV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, hhsv);
    prt->papFldDes[DigitalDCPSaoRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[DigitalDCPSaoRecordLLSV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, llsv);
    prt->papFldDes[DigitalDCPSaoRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[DigitalDCPSaoRecordHSV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, hsv);
    prt->papFldDes[DigitalDCPSaoRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[DigitalDCPSaoRecordLSV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lsv);
    prt->papFldDes[DigitalDCPSaoRecordHYST]->size = sizeof(prec->hyst);
    prt->papFldDes[DigitalDCPSaoRecordHYST]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, hyst);
    prt->papFldDes[DigitalDCPSaoRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[DigitalDCPSaoRecordADEL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, adel);
    prt->papFldDes[DigitalDCPSaoRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[DigitalDCPSaoRecordMDEL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, mdel);
    prt->papFldDes[DigitalDCPSaoRecordRVAL]->size = sizeof(prec->rval);
    prt->papFldDes[DigitalDCPSaoRecordRVAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, rval);
    prt->papFldDes[DigitalDCPSaoRecordORAW]->size = sizeof(prec->oraw);
    prt->papFldDes[DigitalDCPSaoRecordORAW]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, oraw);
    prt->papFldDes[DigitalDCPSaoRecordRBV]->size = sizeof(prec->rbv);
    prt->papFldDes[DigitalDCPSaoRecordRBV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, rbv);
    prt->papFldDes[DigitalDCPSaoRecordORBV]->size = sizeof(prec->orbv);
    prt->papFldDes[DigitalDCPSaoRecordORBV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, orbv);
    prt->papFldDes[DigitalDCPSaoRecordPVAL]->size = sizeof(prec->pval);
    prt->papFldDes[DigitalDCPSaoRecordPVAL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, pval);
    prt->papFldDes[DigitalDCPSaoRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[DigitalDCPSaoRecordLALM]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lalm);
    prt->papFldDes[DigitalDCPSaoRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[DigitalDCPSaoRecordALST]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, alst);
    prt->papFldDes[DigitalDCPSaoRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[DigitalDCPSaoRecordMLST]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, mlst);
    prt->papFldDes[DigitalDCPSaoRecordPBRK]->size = sizeof(prec->pbrk);
    prt->papFldDes[DigitalDCPSaoRecordPBRK]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, pbrk);
    prt->papFldDes[DigitalDCPSaoRecordINIT]->size = sizeof(prec->init);
    prt->papFldDes[DigitalDCPSaoRecordINIT]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, init);
    prt->papFldDes[DigitalDCPSaoRecordLBRK]->size = sizeof(prec->lbrk);
    prt->papFldDes[DigitalDCPSaoRecordLBRK]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, lbrk);
    prt->papFldDes[DigitalDCPSaoRecordSIOL]->size = sizeof(prec->siol);
    prt->papFldDes[DigitalDCPSaoRecordSIOL]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, siol);
    prt->papFldDes[DigitalDCPSaoRecordSIML]->size = sizeof(prec->siml);
    prt->papFldDes[DigitalDCPSaoRecordSIML]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, siml);
    prt->papFldDes[DigitalDCPSaoRecordSIMM]->size = sizeof(prec->simm);
    prt->papFldDes[DigitalDCPSaoRecordSIMM]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, simm);
    prt->papFldDes[DigitalDCPSaoRecordSIMS]->size = sizeof(prec->sims);
    prt->papFldDes[DigitalDCPSaoRecordSIMS]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, sims);
    prt->papFldDes[DigitalDCPSaoRecordIVOA]->size = sizeof(prec->ivoa);
    prt->papFldDes[DigitalDCPSaoRecordIVOA]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ivoa);
    prt->papFldDes[DigitalDCPSaoRecordIVOV]->size = sizeof(prec->ivov);
    prt->papFldDes[DigitalDCPSaoRecordIVOV]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, ivov);
    prt->papFldDes[DigitalDCPSaoRecordOMOD]->size = sizeof(prec->omod);
    prt->papFldDes[DigitalDCPSaoRecordOMOD]->offset = (unsigned short)offsetof(DigitalDCPSaoRecord, omod);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(DigitalDCPSaoRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_DigitalDCPSaoRecord_H */
