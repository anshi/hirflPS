TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET
# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += psRecord
DBDINC += DigitalDCPSaoRecord
DBDINC += InforminRecord
DBDINC += DigitalScanPSRecord


# Install xxxSupport.dbd into <top>/dbd
DBD += psSupport.dbd
DBD += DigitalDCPSaoRecord.dbd
DBD += DigitalScanPSRecord.dbd
DBD += InforminRecord.dbd
# Build an IOC support library
LIBRARY_IOC += PSSupport

# Compile and add code to the support library
PSSupport_SRCS += psRecord.c
PSSupport_SRCS += devPsSoft.c
PSSupport_SRCS += PS.c
PSSupport_SRCS += DigitalDCPSaoRecord.c
PSSupport_SRCS += DigitalScanPSRecord.c
PSSupport_SRCS += InforminRecord.c
PSSupport_SRCS += devDigitalPSRampInt.c
PSSupport_SRCS += devDigitalPSRampInform.c
PSSupport_SRCS += devDigitalPSInt.c
PSSupport_SRCS += devDigitalDCPSInt.c
PSSupport_SRCS += devDigitalScanPSInt.c
# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds

PSSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = PSVersion.h
# Macro name
GENVERSIONMACRO = PSVERSION

# Build the IOC application
#PROD_IOC = PS

# PS.dbd will be created and installed
# DBD += PS.dbd

# # PS.dbd will include these files:
# PS_DBD += base.dbd
# PS_DBD += psSupport.dbd
# PS_DBD += dbSubExample.dbd
# PS_DBD += devPSVersion.dbd
# PS_DBD += PSHello.dbd
# PS_DBD += initTrace.dbd

# PS_registerRecordDeviceDriver.cpp derives from PS.dbd
# PS_SRCS += PS_registerRecordDeviceDriver.cpp

# # Build the main IOC entry point where needed
# PS_SRCS_DEFAULT += PSMain.cpp
# PS_SRCS_vxWorks += -nil-

# # Link in the code from our support library
# PS_LIBS += PSSupport

# # To build SNL programs, SNCSEQ must be defined
# # in the <top>/configure/RELEASE file
# ifneq ($(SNCSEQ),)
#     # Build sncExample into PSSupport
#     sncExample_SNCFLAGS += +r
#     PS_DBD += sncExample.dbd
#     # A .stt sequence program is *not* pre-processed:
#     PSSupport_SRCS += sncExample.stt
#     PSSupport_LIBS += seq pv
#     PS_LIBS += seq pv

#     # Build sncProgram as a standalone program
#     PROD_HOST += sncProgram
#     sncProgram_SNCFLAGS += +m
#     # A .st sequence program *is* pre-processed:
#     sncProgram_SRCS += sncProgram.st
#     sncProgram_LIBS += seq pv
#     sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
# endif

# # Link QSRV (pvAccess Server) if available
# ifdef EPICS_QSRV_MAJOR_VERSION
#     PS_LIBS += qsrv
#     PS_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
#     PS_DBD += PVAServerRegister.dbd
#     PS_DBD += qsrv.dbd
# endif

# # Finally link IOC to the EPICS Base libraries
# PS_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
devPSVersion$(DEP): $(COMMON_DIR)/$(GENVERSION)
