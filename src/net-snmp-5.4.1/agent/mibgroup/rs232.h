/*
 * rs232.h: top level .h file to merely include the sub-module.
 */

config_require(rs232/rs232_access)
config_require(rs232/rs232_checkfns_local)
config_require(rs232/rs232_checkfns)
config_require(rs232/rs232)
config_add_mib(RS-232-MIB)
