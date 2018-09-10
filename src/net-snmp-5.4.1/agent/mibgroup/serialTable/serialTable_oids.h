/*
 * Note: this file originally auto-generated by mib2c using
 *  : generic-table-oids.m2c 12855 2005-09-27 15:56:08Z rstory $
 *
 * $Id:$
 */
#ifndef SERIALTABLE_OIDS_H
#define SERIALTABLE_OIDS_H

#ifdef __cplusplus
extern "C" {
#endif


/* column number definitions for table serialTable */
#define SERIALTABLE_OID              1,3,6,1,4,1,1,3

#define COLUMN_PORTINDEX         1
    
#define COLUMN_PORTNAME         2
#define COLUMN_PORTNAME_FLAG    (0x1 << 1)
    
#define COLUMN_SOCKETNUMBER         3
    
#define COLUMN_PROTOCOL         4
    
#define COLUMN_DEVICETYPE         5
    
#define COLUMN_REMOTEIPADDRESS         6
    
#define COLUMN_REMOTESOCKETNUMBER         7
    
#define COLUMN_BYPASSENABLE         8
    
#define COLUMN_LOGINENABLE         9
    
#define COLUMN_LOGINUSERNAME         10
    
#define COLUMN_LOGINPASSWORD         11
    
#define COLUMN_CONNECTIONCOUNT         12
    
#define COLUMN_CONNECTIONSTATUS         13
    
#define COLUMN_SERIALROWSTATUS         14
#define COLUMN_SERIALROWSTATUS_FLAG    (0x1 << 13)
    

#define SERIALTABLE_MIN_COL   COLUMN_PORTNAME
#define SERIALTABLE_MAX_COL   COLUMN_SERIALROWSTATUS
    

    /*
     * TODO:405:r: Review SERIALTABLE_SETTABLE_COLS macro.
     * OR together all the writable cols.
     */
#define SERIALTABLE_SETTABLE_COLS (COLUMN_PORTNAME_FLAG | COLUMN_SERIALROWSTATUS_FLAG)
    /*
     * TODO:405:r: Review SERIALTABLE_REQUIRED_COLS macro.
     * OR together all the required rows for row creation.
     * default is writable cols w/out defaults.
     */
#define SERIALTABLE_REQUIRED_COLS (COLUMN_PORTNAME_FLAG | COLUMN_SERIALROWSTATUS_FLAG)
    

#ifdef __cplusplus
}
#endif

#endif /* SERIALTABLE_OIDS_H */