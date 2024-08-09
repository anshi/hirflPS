TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Install databases, templates & substitutions like this
DB += ramping_ps.substitutions
DB += dc_ethernet_ps.substitutions
DB += dc_serial_ps.substitutions
DB += dbRampingPS.db
DB += dbDigitalDCEthernetPS.db
DB += dbDigitalDCSerialPS.db
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

