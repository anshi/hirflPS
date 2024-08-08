#!../../bin/linux-x86_64/PS

#- You may have to change PS to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/PS.dbd"
PS_registerRecordDeviceDriver pdbbase

#dbLoadRecords ("db/dbHarmonicPSsoft.db", "AREA=TEST, PS_ID=PS1, FUNC=ai, OutLinka=PS:SFC-CH1-A:ai.CURRENT_VAL, OutLinkb=PS:SFC-CH1-B:ai.CURRENT_VAL, OutLinkc=PS:SFC-CH1-C:ai.CURRENT_VAL")
## Load record instances
#dbLoadTemplate "db/user.substitutions"
#dbLoadRecords "db/circle.db"

dbLoadTemplate "db/ramping_ps.substitutions"
#dbLoadTemplate "db/dc_ethernet_ps.substitutions"
#dbLoadTemplate "db/dc_serial_ps.substitutions"


#PS_TEST_Ext_BUMP
#dbLoadRecords "db/dbui.db", "AREA=TEST, PS_ID=EXT_BUMP"
#dbLoadRecords "db/DigitalBumpPS.db", "AREA=TEST, PS_ID=EXT_BUMP,IP=127.0.0.1, PORT=40000,DTYPE=1, VER=4"
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=TEST, PS_SN=PS_EXT_BUMP, FUNC=ai, IP=10.10.40.4, PORT=30000, PARM=@4:1:40000, OUT1=TEST:PS_EXT_BUMP:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=PS_EXT_BUMP")

#dbLoadRecords "db/DigitalBumpPS.db", "AREA=RING, PS_ID=4BPE01,IP=127.0.0.1, PORT=40000,DTYPE=1, VER=4"
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=RING, PS_SN=PS4BPE01, FUNC=AI, IP=10.10.40.4, PORT=30000, PARM=@4:1:40000, OUT1=RING:PS4BPE01:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=RING, PS_SN=PS4BPE01")
# dbLoadRecords ("db/dbDPRAMwv.db", "AREA=TEST, DEV_ID=DPRAM")





#PS_TEST_Inj_BUMP
#dbLoadRecords "db/dbui.db", "AREA=TEST, PS_ID=INJ_BUMP"
#dbLoadRecords "db/DigitalBumpPS.db", "AREA=TEST, PS_ID=INJ_BUMP,IP=127.0.0.1, PORT=40001,DTYPE=2, VER=4"
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=TEST, PS_SN=PS_INJ_BUMP, FUNC=ai, IP=192.168.1.10, PORT=60000, PARM=@4:1:40001, OUT1=TEST:PS_INJ_BUMP:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=PS_INJ_BUMP")


#dbLoadRecords "db/DigitalBumpPS.db", "AREA=RING, PS_ID=4BPI01,IP=127.0.0.1, PORT=40001,DTYPE=2, VER=4"
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=RING, PS_SN=PS4BPI01, FUNC=AI, IP=192.168.1.10, PORT=60000, PARM=@4:1:40001, OUT1=RING:PS4BPI01:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=RING, PS_SN=PS4BPI01")





#TEST:DC (Ramping protocol v3 and v4 DC PS)
#dbLoadRecords ("db/digitalPSInt.db", "AREA=TEST, PS_SN=DC, FUNC=ai, IP=10.10.40.44, PORT=30000, PARM=@4:0.5, OUT1=TEST:DC:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=DC")

#TEST:DC1 (DC protocol v3, via the serial port connect.)
#dbLoadRecords ("db/digitalDCPSInt.db", "AREA=TEST, PS_SN=DC1, FUNC=ai, IP=10.10.22.101, PORT=5300, PARM=@0.5, OUT1=TEST:DC1:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=DC1")

#dbLoadRecords ("db/digitalDCPSInt.db", "AREA=INJLINE, PS_SN=PS3D01, FUNC=AI, IP=10.10.22.101, PORT=5300, PARM=@0.5, OUT1=INJLINE:PS3D01:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=INJLINE, PS_SN=PS3D01")





#TEST:RAMP
#dbLoadRecords "db/dbui.db", "AREA=TEST, PS_ID=RAMP"
#dbLoadRecords "db/CurrentPS.db", "AREA=TEST, PS_ID=RAMP_DATA, IP=127.0.0.1, PORT=40002, DTYPE=2, VER=4"
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=TEST, PS_SN=PS40D01, FUNC=ai, IP=10.10.40.44, PORT=30000, PARM=@4:1:40002, OUT1=TEST:PS40D01:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=PS40D01")

# dbLoadRecords "db/dbui.db", "AREA=RING, PS_ID=PS4D01"
# dbLoadRecords "db/CurrentPS.db", "AREA=MAGMEAS, PS_ID=01, IP=127.0.0.1, PORT=40002, DTYPE=2, VER=4"#
#dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=MAGMEAS, PS_SN=PS01, FUNC=AI, IP=192.6.94.231, PORT=30000, PARM=@4:1:40002, OUT1=MAGMEAS:PS01:error.INFORMCODE")
#dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=MAGMEAS, PS_SN=PS01")
#dbLoadRecords "db/CurrentPS.db", "AREA=MAGMEAS, PS_ID=01, IP=127.0.0.1, PORT=40002, DTYPE=2, VER=4, INP=MAGMEAS:PS01:AI.STATE, OUT=MAGMEAS:PS01:AI.COMMAND"


#TEST:TRG
# dbLoadRecords ("db/digitalRampingPSInt.db", "AREA=TEST, PS_SN=RAMP, FUNC=AI, IP=10.10.15.5, PORT=30000, PARM=@4:0.5:40004, OUT1=TEST:RAMP:error.INFORMCODE")
# dbLoadRecords ("db/dbPSRampingInfo.db", "AREA=TEST, PS_SN=RAMP")
# dbLoadRecords "db/CurrentPS.db", "AREA=TEST, PS_ID=RAMP, IP=127.0.0.1, PORT=40004, DTYPE=3, VER=4, INP=TEST:RAMP:AI.STATE, OUT=EXTLINE:RAMP:AI.COMMAND"



#dbLoadRecords "db/PS.db", "AREA=CSRM, PS_ID=20D:VOL,IP=10.10.20.128,PORT=40009,DTYPE=1, VER=Ana"
#dbLoadRecords "db/PS.db", "AREA=CSRM, PS_ID=20D:CUR,IP=10.10.20.128,PORT=40008,DTYPE=1, VER=Ana"


#dbLoadRecords "db/PS.db", "AREA=CSRM, PS_ID=20MS3,IP=10.10.83.85,PORT=40055,DTYPE=0, VER=3"
#dbLoadRecords "db/PS.db", "AREA=CSRM, PS_ID=20MS4,IP=10.10.83.85,PORT=40056,DTYPE=0, VER=3"




#- Set this to see messages from mySub
#var mySubDebug 1

#- Run this to trace the stages of iocInit
#traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=root"
