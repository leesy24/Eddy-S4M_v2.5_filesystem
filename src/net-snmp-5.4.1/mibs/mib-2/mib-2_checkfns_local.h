/*
 * Note: this file originally auto-generated by mib2c using
 *        : : mib2c.check_values_local.conf 10232 2004-05-04 23:35:32Z hardaker $
 * 
 */
#ifndef MIB-2_CHECKFNS_H
#define MIB-2_CHECKFNS_H

/* these functions are designed to check incoming values for 
columns in the sysORTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

/* these functions are designed to check incoming values for 
columns in the ifTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ifAdminStatus_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the atTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_atIfIndex_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_atPhysAddress_local(int type, char *val, size_t val_len, char *old_val, size_t old_val_len);
    int check_atNetAddress_local(int type,  *val, size_t val_len,  *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the ipAddrTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

/* these functions are designed to check incoming values for 
columns in the ipRouteTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ipRouteDest_local(int type, u_long *val, size_t val_len, u_long *old_val, size_t old_val_len);
    int check_ipRouteIfIndex_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteMetric1_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteMetric2_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteMetric3_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteMetric4_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteNextHop_local(int type, u_long *val, size_t val_len, u_long *old_val, size_t old_val_len);
    int check_ipRouteType_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteAge_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipRouteMask_local(int type, u_long *val, size_t val_len, u_long *old_val, size_t old_val_len);
    int check_ipRouteMetric5_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the ipNetToMediaTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ipNetToMediaIfIndex_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ipNetToMediaPhysAddress_local(int type, char *val, size_t val_len, char *old_val, size_t old_val_len);
    int check_ipNetToMediaNetAddress_local(int type, u_long *val, size_t val_len, u_long *old_val, size_t old_val_len);
    int check_ipNetToMediaType_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the tcpConnTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_tcpConnState_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the udpTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

/* these functions are designed to check incoming values for 
columns in the egpNeighTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_egpNeighEventTrigger_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the rs232PortTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_rs232PortInSpeed_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232PortOutSpeed_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232PortInFlowType_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232PortOutFlowType_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the rs232AsyncPortTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_rs232AsyncPortBits_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232AsyncPortStopBits_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232AsyncPortParity_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232AsyncPortAutobaud_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the rs232SyncPortTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_rs232SyncPortClockSource_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortRole_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortEncoding_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortRTSControl_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortRTSCTSDelay_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortMode_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortIdlePattern_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_rs232SyncPortMinFlags_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the rs232InSigTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

/* these functions are designed to check incoming values for 
columns in the rs232OutSigTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

/* these functions are designed to check incoming values for 
columns in the ifXTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ifLinkUpDownTrapEnable_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ifPromiscuousMode_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ifAlias_local(int type, char *val, size_t val_len, char *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the ifStackTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ifStackStatus_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the ifTestTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ifTestId_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ifTestStatus_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ifTestType_local(int type, oid *val, size_t val_len, oid *old_val, size_t old_val_len);
    int check_ifTestOwner_local(int type, char *val, size_t val_len, char *old_val, size_t old_val_len);
/* these functions are designed to check incoming values for 
columns in the ifRcvAddressTable table for legality with respect to 
datatype and value according to local conventions.  You should modify
them as appropriate.  They will be called from parent check_value
functions that are auto-generated using mib2c and the parent functions
should NOT be modified.
 */

    int check_ifRcvAddressStatus_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);
    int check_ifRcvAddressType_local(int type, long *val, size_t val_len, long *old_val, size_t old_val_len);

#endif /* MIB-2_CHECKFNS_H */
