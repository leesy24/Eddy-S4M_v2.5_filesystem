/*
 * module to include the modules
 */

config_require(sysbas/sysbas_access)
config_require(sysbas/sysbas_checkfns_local)
config_require(sysbas/sysbas_checkfns)
config_require(sysbas/sysbas)
config_add_mib(SYSBAS-MIB)
