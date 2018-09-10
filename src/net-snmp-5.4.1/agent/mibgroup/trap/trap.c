/*
 * start be including the appropriate header files 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * contains prototypes 
 */
#include "notification.h"

/*
 * our initialization routine
 * (to get called, the function name must match init_FILENAME() 
 */
void
init_notification(void)
{
    DEBUGMSGTL(("example_notification",
                "initializing (setting callback alarm)\n"));
    snmp_alarm_register(30,     /* seconds */
                        SA_REPEAT,      /* repeat (every 30 seconds). */
                        send_example_notification,      /* our callback */
                        NULL    /* no callback data needed */
        );
}

void
send_example_notification(unsigned int clientreg, void *clientarg)
{
    /*
     * define the OID for the notification we're going to send
     * NET-SNMP-EXAMPLES-MIB::netSnmpExampleHeartbeatNotification 
     */
    oid             notification_oid[] =
        { 1, 3, 6, 1, 4, 1, 8072, 2, 3, 0, 1 };
    size_t          notification_oid_len = OID_LENGTH(notification_oid);
    static u_long count = 0;

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition. 
     */
    oid             objid_snmptrap[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
    size_t          objid_snmptrap_len = OID_LENGTH(objid_snmptrap);

    /*
     * define the OIDs for the varbinds we're going to include
     *  with the notification -
     * NET-SNMP-EXAMPLES-MIB::netSnmpExampleHeartbeatRate  and
     * NET-SNMP-EXAMPLES-MIB::netSnmpExampleHeartbeatName 
     */
    oid      hbeat_rate_oid[]   = { 1, 3, 6, 1, 4, 1, 8072, 2, 3, 2, 1, 0 };
    size_t   hbeat_rate_oid_len = OID_LENGTH(hbeat_rate_oid);
    oid      hbeat_name_oid[]   = { 1, 3, 6, 1, 4, 1, 8072, 2, 3, 2, 2, 0 };
    size_t   hbeat_name_oid_len = OID_LENGTH(hbeat_name_oid);

    /*
     * here is where we store the variables to be sent in the trap 
     */
    netsnmp_variable_list *notification_vars = NULL;
    const char *heartbeat_name = "A girl named Maria";
#ifdef  RANDOM_HEARTBEAT
    int  heartbeat_rate = rand() % 60;
#else
    int  heartbeat_rate = 30;
#endif

    DEBUGMSGTL(("example_notification", "defining the trap\n"));

    /*
     * add in the trap definition object 
     */
    snmp_varlist_add_variable(&notification_vars,
                              /*
                               * the snmpTrapOID.0 variable 
                               */
                              objid_snmptrap, objid_snmptrap_len,
                              /*
                               * value type is an OID 
                               */
                              ASN_OBJECT_ID,
                              /*
                               * value contents is our notification OID 
                               */
                              (u_char *) notification_oid,
                              /*
                               * size in bytes = oid length * sizeof(oid) 
                               */
                              notification_oid_len * sizeof(oid));

    /*
     * add in the additional objects defined as part of the trap
     */

    snmp_varlist_add_variable(&notification_vars,
                               hbeat_rate_oid, hbeat_rate_oid_len,
                               ASN_INTEGER,
                              (u_char *)&heartbeat_rate,
                                  sizeof(heartbeat_rate));

    /*
     * if we want to insert additional objects, we do it here 
     */
    if (heartbeat_rate < 30 ) {
        snmp_varlist_add_variable(&notification_vars,
                               hbeat_name_oid, hbeat_name_oid_len,
                               ASN_OCTET_STR,
                               heartbeat_name, strlen(heartbeat_name));
    }

    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page. 
     */
    ++count;
    DEBUGMSGTL(("example_notification", "sending trap %ld\n",count));
    send_v2trap(notification_vars);

    /*
     * free the created notification variable list 
     */
    DEBUGMSGTL(("example_notification", "cleaning up\n"));
    snmp_free_varbind(notification_vars);
}