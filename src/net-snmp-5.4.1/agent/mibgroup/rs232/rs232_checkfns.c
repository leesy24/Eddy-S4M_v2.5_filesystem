/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.check_values.conf 9254 2004-01-12 00:43:46Z rstory $
 */

/********************************************************************
 *                       NOTE   NOTE   NOTE
 *   This file is auto-generated and SHOULD NOT BE EDITED by hand.
 *   Modify the rs232_checkfns_local.[ch] files insead so that you
 *   can regenerate this one as mib2c improvements are made.
 ********************************************************************/

/* standard headers */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "rs232_checkfns.h"
#include "rs232_checkfns_local.h"
#include "rs232_enums.h"

/** Decides if an incoming value for the rs232PortInSpeed mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232PortInSpeed(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232PortInSpeed_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232PortOutSpeed mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232PortOutSpeed(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232PortOutSpeed_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232PortInFlowType mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232PortInFlowType(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232PORTINFLOWTYPE_NONE:
      	  case RS232PORTINFLOWTYPE_CTSRTS:
      	  case RS232PORTINFLOWTYPE_DSRDTR:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232PortInFlowType_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232PortOutFlowType mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232PortOutFlowType(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232PORTOUTFLOWTYPE_NONE:
      	  case RS232PORTOUTFLOWTYPE_CTSRTS:
      	  case RS232PORTOUTFLOWTYPE_DSRDTR:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232PortOutFlowType_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232AsyncPortBits mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232AsyncPortBits(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

	ret = SNMP_ERR_NOERROR;
    /** Check the ranges of the passed value for legality */
	    if (
	       !(*val >= 5 && *val <= 8)
            ) {
            return SNMP_ERR_WRONGVALUE;
            }


    /** looks ok, call the local version of the same function. */
      return check_rs232AsyncPortBits_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232AsyncPortStopBits mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232AsyncPortStopBits(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232ASYNCPORTSTOPBITS_ONE:
      	  case RS232ASYNCPORTSTOPBITS_TWO:
      	  case RS232ASYNCPORTSTOPBITS_ONEANDHALF:
      	  case RS232ASYNCPORTSTOPBITS_DYNAMIC:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232AsyncPortStopBits_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232AsyncPortParity mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232AsyncPortParity(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232ASYNCPORTPARITY_NONE:
      	  case RS232ASYNCPORTPARITY_ODD:
      	  case RS232ASYNCPORTPARITY_EVEN:
      	  case RS232ASYNCPORTPARITY_MARK:
      	  case RS232ASYNCPORTPARITY_SPACE:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232AsyncPortParity_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232AsyncPortAutobaud mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232AsyncPortAutobaud(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232ASYNCPORTAUTOBAUD_ENABLED:
      	  case RS232ASYNCPORTAUTOBAUD_DISABLED:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232AsyncPortAutobaud_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortClockSource mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortClockSource(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTCLOCKSOURCE_INTERNAL:
      	  case RS232SYNCPORTCLOCKSOURCE_EXTERNAL:
      	  case RS232SYNCPORTCLOCKSOURCE_SPLIT:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortClockSource_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortRole mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortRole(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTROLE_DTE:
      	  case RS232SYNCPORTROLE_DCE:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortRole_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortEncoding mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortEncoding(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTENCODING_NRZ:
      	  case RS232SYNCPORTENCODING_NRZI:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortEncoding_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortRTSControl mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortRTSControl(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTRTSCONTROL_CONTROLLED:
      	  case RS232SYNCPORTRTSCONTROL_CONSTANT:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortRTSControl_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortRTSCTSDelay mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortRTSCTSDelay(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortRTSCTSDelay_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortMode mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortMode(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTMODE_FDX:
      	  case RS232SYNCPORTMODE_HDX:
      	  case RS232SYNCPORTMODE_SIMPLEX_RECEIVE:
      	  case RS232SYNCPORTMODE_SIMPLEX_SEND:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortMode_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortIdlePattern mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortIdlePattern(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values will continue, others return error. */
           switch (*val) {
      	  case RS232SYNCPORTIDLEPATTERN_MARK:
      	  case RS232SYNCPORTIDLEPATTERN_SPACE:
      	    break;

    /** not a legal enum value.  return an error */
      	  default:
      	    return SNMP_ERR_INCONSISTENTVALUE;
      	}
	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortIdlePattern_local(type, val, val_len, old_val, old_val_len);
    }
/** Decides if an incoming value for the rs232SyncPortMinFlags mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
    int
    check_rs232SyncPortMinFlags(int type, long *val, size_t val_len,
             long *old_val, size_t old_val_len) {

    int ret;

    /** Check to see that we were called legally */
      if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
      if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

	ret = SNMP_ERR_NOERROR;


    /** looks ok, call the local version of the same function. */
      return check_rs232SyncPortMinFlags_local(type, val, val_len, old_val, old_val_len);
    }
