#include <errno.h>
//#include <sys/stropts.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>		/* no delay getchar() */
#include <unistd.h>
#include <netdb.h>		/* gethostbyname, getnetbyname */
#include <unistd.h>
#include <sys/socket.h>		/* for "struct sockaddr" et al  */
#include <sys/time.h>		/* struct timeval */

#include <linux/version.h>
//#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
//#include <net/if.h>
//#else
unsigned int if_nametoindex(const char *ifname);
//#endif

#include <net/ethernet.h>	/* struct ether_addr */
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <pthread.h>

#include "rtl871x_mp_ioctl.h"

//#define _PTM_SET_AUTO_
//#define _PTM_SUPPORTED_

extern char interface[IFNAMSIZ];

//#define RTL8192SU 1

/*
 * getchar() without waiting enter key be pressed.
 * change local modes temporarily
 *
 * Local Modes
 *	The c_lflag field of the argument structure
 *	is used to control various terminal functions:
 *	ECHO
 *		Enable echo
 * 	ICANON
 *		Canonical input (erase and kill processing).
 * Attribute Selection
 *	The following symbolic constants for use with tcsetattr() are defined:
 *	TCSANOW
 *		Change attributes immediately.
 */
static int getchar_nodelay(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	memcpy(&newt, &oldt, sizeof(struct termios));
	newt.c_lflag &= ~(ICANON | ECHO);	// modify local modes
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}
//----------------------------------------------------------------------------------------
int rfreg_page_changed(struct wpsctrl *pwpsc,unsigned int page);
//----------------------------------------------------------------------------------------
/*
 * return value
 *	0: down
 *	1: up
 */
int is_dev_up(struct wpsctrl *pwpsc)
{
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, interface, IFNAMSIZ);

	if (ioctl(pwpsc->skfd, SIOCGIFFLAGS, &ifr) == 0) {
		if (ifr.ifr_flags & IFF_UP)
			return 1;
	} else
		printf("ioctl[SIOCGIFFLAGS] fail");

	return 0;
}
//----------------------------------------------------------------------------------------
int r8711priv_ioctl(int skfd, unsigned int direction, unsigned int iocode, void *pInBuffer, unsigned int InBufferSize,
					void *pOutBuffer, unsigned int OutBufferSize)
{
	int cmd, ret;
	struct iwreq iwr;

	ret = 0;

	memset(&iwr, 0, sizeof(struct iwreq));
	//printf("interface=%s\n", pwpsc->ifrn_name);
//	printf("interface=%s\n", interface);
	strncpy(iwr.ifr_ifrn.ifrn_name, interface, sizeof(iwr.ifr_ifrn.ifrn_name));
	//strncpy(iwr.ifr_ifrn.ifrn_name, pwpsc->ifrn_name, sizeof(iwr.ifr_ifrn.ifrn_name));

#ifdef RTL8192SU
	cmd =  SIOCIWFIRSTPRIV + 15;
#else
	cmd =  iocode;//SIOCIWFIRSTPRIV + 0x3
#endif

	iwr.u.data.pointer = pInBuffer;
	iwr.u.data.length = (unsigned short)InBufferSize;
	iwr.u.data.flags = direction;//set/query info

	ret = ioctl(skfd, cmd, &iwr);

	if(direction == 0x00)//query
	{
		if(pOutBuffer)
		{
			memcpy(pOutBuffer, iwr.u.data.pointer, OutBufferSize);
		}
	}

	return ret;
}
//----------------------------------------------------------------------------------------
//MP_START_MODE
int r871x_mp_init(struct wpsctrl *pwpsc)
{
	int skfd, skpkt;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) return FALSE;

	skpkt = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (skpkt < 0) return FALSE;

//	pwpsc = malloc(sizeof(struct wpsctrl));
	pwpsc->enter_mp_test_mode = 0;
	pwpsc->priv_ptr = NULL;
	pwpsc->skfd = skfd;
	pwpsc->skpkt = skpkt;
//	pwpsc->iocontrol_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&pwpsc->iocontrol_mutex, NULL);

	//register ops. function
	pwpsc->pwpsc_ops->init = NULL;
	pwpsc->pwpsc_ops->iocontrol = r8711priv_ioctl;

//	printf("$-r871x_mp_init- pwpsc:%x\n",(unsigned int)pwpsc);

	// basic
	// antenna 1T2R
	SET_TX_ANTENNA(pwpsc->antenna, ANTENNA_A);
	SET_RX_ANTENNA(pwpsc->antenna, ANTENNA_AB);

	pwpsc->bGENTSSI = FALSE;
	pwpsc->bstarttx = FALSE;
	pwpsc->bstartrx = FALSE;

	pwpsc->tx_pkt_cnts = 10;
	pwpsc->rx_pkt_ok_cnts = 0;
	pwpsc->rx_pkt_err_cnts = 0;

	pwpsc->payload_len = 10;
	pwpsc->payload_type = 0;
	pwpsc->dig_mode = 1;

	memset(pwpsc->da, 0xFF, ETH_ALEN);
//	memset(pwpsc->TxDesc, 0, sizeof(tx_desc_8712);

//	memset(pwpsc->tssi_table.cck, 0, sizeof(unsigned char)*4);
//	memset(pwpsc->tssi_table.ofdm, 0, sizeof(unsigned char)*4);

//	memset(pwpsc->eeprom_info.TSSI.CCK, 0, sizeof(unsigned char)*4);
//	memset(pwpsc->eeprom_info.TSSI.OFDM, 0, sizeof(unsigned char)*4);

	memset(pwpsc->efuse_map, 0xFF, EFUSE_MAP_MAX_SIZE);

	return TRUE;
}
//----------------------------------------------------------------------------------------
void MPModeParameterAdjustment(struct wpsctrl *pwpsc, int bStartMP)
{
#if 0
	if (bStartMP) {
		// The parameters of CCK are different between MP and normal mode
		// MP mode: 0x1d200240 = 01004081
		//WriteRegData(AdapterObject, 0x1d200240, 4, 0x01004081);
		r871x_write_reg(pwpsc, 0x1d200240, 4, 0x01004081);
		r871x_set_ptm(pwpsc,_POWERTRACK_OFF_);
	}
	else {	// Normal mode: 0x1d200240 = 01004781
		//WriteRegData(AdapterObject, 0x1d200240, 4, 0x01004781);
		r871x_write_reg(pwpsc, 0x1d200240, 4, 0x01004781);
		r871x_set_ptm(pwpsc,_POWERTRACK_PKTTX_);
		if(pwpsc->dig_mode != 1){
			pwpsc->dig_mode = 1;
			r871x_set_dig(pwpsc,pwpsc->dig_mode);
		}
	}
#endif
}
//----------------------------------------------------------------------------------------
int r871x_set_mp_start(struct wpsctrl *pwpsc)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;
	struct mp_status_param		*pmpstatus = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(struct mp_status_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) return -1;

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(MP_START);
	pmpioctlparam->len = sizeof(struct mp_status_param);

	pmpstatus = (struct mp_status_param *)pmpioctlparam->data;
	pmpstatus->mode = _2MAC_MODE_;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);

	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
					(unsigned char *)(pmpioctlparam), msz,
					(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
	MPModeParameterAdjustment(pwpsc, TRUE);

	return ret;
}
//----------------------------------------------------------------------------------------

int r871x_set_mp_stop(struct wpsctrl *pwpsc)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;
	struct mp_status_param		*pmpstatus = NULL;


	MPModeParameterAdjustment(pwpsc, FALSE);

	msz = sizeof(struct mp_ioctl_param) + sizeof(struct mp_status_param);
	pmpioctlparam = (struct mp_ioctl_param*)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(MP_STOP);
	pmpioctlparam->len =  sizeof(struct mp_status_param);

	pmpstatus = (struct mp_status_param *)pmpioctlparam->data;
	pmpstatus->mode = MP_STOP_MODE ;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
					(unsigned char *)(pmpioctlparam), msz,
					(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}

//----------------------------------------------------------------------------------------
int r871x_set_channel(struct wpsctrl *pwpsc, unsigned char ch)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_CHANNEL);
	pmpioctlparam->len = sizeof(unsigned int);
	*((unsigned int*)pmpioctlparam->data) = (unsigned int)ch;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
					(unsigned char *)(pmpioctlparam), msz,
					(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_set_txpower(struct wpsctrl *pwpsc, unsigned int power_index)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct txpower_param *ptxpwr = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(struct txpower_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_TXPOWER);
	pmpioctlparam->len = sizeof(struct txpower_param);

	ptxpwr = (struct txpower_param *)pmpioctlparam->data;
	ptxpwr->pwr_index = power_index;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
					(unsigned char *)(pmpioctlparam), msz,
					(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------

/*
rate_index <-> data_rate mapping

0 <-> CCK 1 Mbps
1 <-> CCK 2 Mbps
2 <-> CCK 5.5 Mbps
3 <-> CCK 11 Mbps
4 <-> OFDM 6 Mbps
5 <-> OFDM 9 Mbps
6 <-> OFDM 12 Mbps
7 <-> OFDM 18 Mbps
8 <-> OFDM 24 Mbps
9 <-> OFDM 36 Mbps
10 <-> OFDM 48 Mbps
11 <-> OFDM 54 Mbps
12~27 <-> MCS0~MCS15
*/
int r871x_set_datarate(struct wpsctrl *pwpsc, unsigned int rate_index)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct datarate_param *pdatarate = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(struct datarate_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_DATARATE);
	pmpioctlparam->len = sizeof(struct datarate_param);

	pdatarate = (struct datarate_param *)pmpioctlparam->data;

	pdatarate->rate_index = rate_index;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_set_bandwidth(struct wpsctrl *pwpsc, unsigned int bandwidth)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_BANDWIDTH);
	pmpioctlparam->len = sizeof(unsigned int);
	*(unsigned int*)pmpioctlparam->data = bandwidth;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_set_antenna(struct wpsctrl *pwpsc, unsigned int antenna)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_ANTENNA);
	pmpioctlparam->len = sizeof(unsigned int);
	*(unsigned int*)pmpioctlparam->data = antenna;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int set_rf_intfs(struct wpsctrl *pwpsc, unsigned int mode)
{
	int ret = 0;
#if 0
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct rf_intfs_param *prfintfs = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct rf_intfs_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);


	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_RF_INTFS);
	pmpioctlparam->len = sizeof(struct rf_intfs_param);

	prfintfs = (struct rf_intfs_param *)pmpioctlparam->data;

	prfintfs->mode = mode;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
#endif
	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_set_ptm(struct wpsctrl *pwpsc, unsigned char mode)
{
	int ret = TRUE;
#ifdef _PTM_SUPPORTED_
        size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	unsigned char *pptm = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (unsigned char);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);


	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_PTM);
	pmpioctlparam->len = sizeof(unsigned char);

	pptm  = (unsigned char *)pmpioctlparam->data;

	*pptm = mode;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//printf("$-r871x_set_ptm type:0x%x\n",mode);

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
#endif
	return ret;
}

//----------------------------------------------------------------------------------------
int r871x_set_dig(struct wpsctrl *pwpsc, unsigned char mode)
{
	int ret = TRUE;
#if 0

        size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	unsigned char *pdig = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (unsigned char);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);


	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_DIG);
	pmpioctlparam->len = sizeof(unsigned char);

	pdig  = (unsigned char *)pmpioctlparam->data;

	*pdig = mode;

	pwpsc_ops = pwpsc->pwpsc_ops;

	printf("$-r871x_set_dig type:0x%x\n",mode);


	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);

	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
#endif

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_set_cca_level(struct wpsctrl *pwpsc, unsigned char level)
{
#if 0
	unsigned int bOFDM = 0x01;
	unsigned int offset= 0x17;
	unsigned int value = 0;

	if(pwpsc->dig_mode != 0){
		pwpsc->dig_mode = 0;
		r871x_set_dig(pwpsc,pwpsc->dig_mode);
	}

	if(level==1)		value = 0x26 ;
	else if(level==2)	value = 0x36 ;
	else if(level==3)	value = 0x46 ;
	else if(level==4)	value = 0x56 ;
	else 			value = 0x26 ;

	//printf("$-r871x_set_cca_level bOFDM:0x%02x offset:0x%02x value:0x%02x\n",bOFDM,offset,(unsigned char)value);
	r871x_write_bbreg(pwpsc, offset,bOFDM ,value);
#endif
	return TRUE;
}
/*-------------------------------------------------------------------------
//for CCK EVM ISSUE 2007.11.22
//  1. if Rate=1/2/5.5/11M,
//            RF reg00=08b		Reg3D => 0x000
//            RF reg0d=000
//            RF reg00=089		Reg19 => 0x3e7
//            RF reg09=3e7
//            RF reg00=08b		Reg37 => 0x000
//            RF reg07=000
//            RF reg00=088
//  2. if Rate=6/9/12/18/24/36/48/54M,
//            RF reg00=08b		Reg3D => 0x200
//            RF reg0d=200
//            RF reg00=089		Reg19 => 0x387 //0x3e7
//            RF reg09=3e7
//            RF reg00=08b		Reg37 => 0x010 //0x002
//            RF reg07=002
//            RF reg00=088
//--------------------------------------------------------------------------*/
void RFRegAdjustment(struct wpsctrl *pwpsc,int RateIdx)
{
#if 0
	unsigned int P3_OffsetD_Value,P1_Offset9_Value,P3_Offset7_Value;

	const int page_0 = 0;
	const int page_1 = 1;
	const int page_3 = 3;

	const int offset_d = 0x0d;
	const int offset_9 = 0x09;
	const int offset_7 = 0x07;

	if(RateIdx==0){	//for Normal mode
		P3_OffsetD_Value = 0x0200;
		P1_Offset9_Value = 0x0367;
		P3_Offset7_Value = 0x0010;
	}
	else if(RateIdx <= 3){	// For CCK
		P3_OffsetD_Value = 0x0000;
		P1_Offset9_Value = 0x03e7;
		P3_Offset7_Value = 0x0000;
	}
	else{				// For OFDM
		P3_OffsetD_Value = 0x0200;
		P1_Offset9_Value = 0x0387;	//0x0387;
		P3_Offset7_Value = 0x0010;	//0x0002;
	}

	//----------  Reg3D ------------------------------------------
	//RFRegPageChanged(AdapterObject,page_3);
	//RFWriteRegData(AdapterObject, offset_d , 4 ,P3_OffsetD_Value);
	r871x_write_rfreg(pwpsc,offset_d, page_3,P3_OffsetD_Value);

	//----------  Reg19 ------------------------------------------
	//RFRegPageChanged(AdapterObject,page_1);
	//RFWriteRegData(AdapterObject, offset_9 , 4 ,P1_Offset9_Value);
	r871x_write_rfreg(pwpsc,offset_9, page_1,P1_Offset9_Value);
	//----------  Reg37 ------------------------------------------
	//RFRegPageChanged(AdapterObject,page_3);
	//RFWriteRegData(AdapterObject, offset_7 , 4 ,P3_Offset7_Value);
	r871x_write_rfreg(pwpsc,offset_7, page_3,P3_Offset7_Value);

	//RFRegPageChanged(AdapterObject,page_0);
	rfreg_page_changed(pwpsc,page_0);
#endif
}
//----------------------------------------------------------------------------------------
int r871x_set_powersave_mode(struct wpsctrl *pwpsc, unsigned int ps_mode,unsigned int smart_ps)
{
	int ret = 0;
#if 0
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct psmode_param *ppsmode = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct psmode_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);


	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(PS_STATE);
	pmpioctlparam->len = sizeof(struct psmode_param);

	ppsmode = (struct psmode_param *)pmpioctlparam->data;

	ppsmode->ps_mode = ps_mode;
	ppsmode->smart_ps = smart_ps;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
#endif
	return ret;
}

//----------------------------------------------------------------------------------------
int r871x_read_reg(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, void *pbuf)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct rwreg_param *prwreg = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct rwreg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(READ_REG);
	pmpioctlparam->len = sizeof(struct rwreg_param);

	prwreg = (struct rwreg_param *)pmpioctlparam->data;

//	if ((addr < 0x10250000) && !((addr >= 0x10100000) && (addr <= 0x101000FF))
//		addr |= 0x10250000;
	prwreg->offset = addr < 0x10000 ? addr | 0x10250000 : addr;
	prwreg->width = cnt;
	prwreg->value = 0xcccccccc;
	//memcpy((void*)&prwreg->value, pbuf, cnt);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							NULL, 0);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);
	if(pbuf)
		memcpy(pbuf, (void*)&prwreg->value, cnt);

	free((void*)pmpioctlparam);

	return ret;

}

//----------------------------------------------------------------------------------------
int r871x_write_reg(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt,unsigned int value)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct rwreg_param *prwreg = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct rwreg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(WRITE_REG);
	pmpioctlparam->len = sizeof(struct rwreg_param);

	prwreg = (struct rwreg_param *)pmpioctlparam->data;

//	if ((addr < 0x10250000) && !((addr >= 0x10100000) && (addr <= 0x101000FF))
//		addr |= 0x10250000;
	prwreg->offset = addr < 0x10000 ? addr | 0x10250000 : addr;
	prwreg->width = cnt;
  	prwreg->value = value;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int read16_eeprom(struct wpsctrl *pwpsc, unsigned int offset,unsigned short*value)
{
	int ret = 0;
#if 0
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct eeprom_rw_param *peeprom = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct eeprom_rw_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(READ16_EEPROM);
	pmpioctlparam->len = sizeof(struct eeprom_rw_param);

	peeprom = (struct eeprom_rw_param *)pmpioctlparam->data;

	peeprom->offset = offset;
	peeprom->value = 0xcccc;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							NULL, 0);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);
	*value = peeprom->value ;

//	printf("$-read16_eeprom(%d) offset:0x%02x value:0x:%04x\n",pmpioctlparam->subcode,peeprom->offset,peeprom->value);
	free((void *)pmpioctlparam);
#endif
	return ret;
}
//----------------------------------------------------------------------------------------
int write16_eeprom(struct wpsctrl *pwpsc, unsigned int offset,unsigned short value)
{
	int ret = 0;
#if 0
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct eeprom_rw_param *peeprom = NULL;


	msz = sizeof (struct mp_ioctl_param) + sizeof (struct eeprom_rw_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}


	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(WRITE16_EEPROM);
	pmpioctlparam->len = sizeof(struct eeprom_rw_param);

	peeprom = (struct eeprom_rw_param *)pmpioctlparam->data;

	peeprom->offset = offset;
	peeprom->value = value;
	//printf("$-write16_eeprom(%d) offset:0x%02x value:0x:%04x\n",pmpioctlparam->subcode,peeprom->offset,peeprom->value);
	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);
#endif
	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_read_eeprom_byte(struct wpsctrl *pwpsc, unsigned char offset,unsigned char *value)
{
#if 0
	unsigned int addr;
	unsigned short rd_val16;

	addr = (offset>>1)*2;

	read16_eeprom(pwpsc, addr,&rd_val16);
	//printf("$-r871x_read_eeprom_byte value:0x%04x\n",rd_val16);

    	if((offset%2)==1)
      		*value = (unsigned char)(rd_val16>>8);
	else
      		*value = (unsigned char)rd_val16;
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_write_eeprom_byte(struct wpsctrl *pwpsc, unsigned char offset,unsigned char value)
{
#if 0
	unsigned int addr;
	unsigned short rd_val16, wr_val16, tmp16;

	addr = (offset>>1)*2;

    	read16_eeprom(pwpsc, addr,&rd_val16);
	//r871x_read_eeprom_byte(pwpsc,addr,&rd_val16);

    	if((offset%2)==1)
	{
      		tmp16 = value;
	  	tmp16 = tmp16<<8;
      		wr_val16 = ((rd_val16&0x00FF) | (tmp16&0xFF00));
	}
	else
	{
      		tmp16 = value;
      		wr_val16 = ((rd_val16&0xFF00) | (tmp16&0x00FF));
	}
	//printf("$-r871x_write_eeprom_byte offset:0x%02x value:0x%04x\n",offset,wr_val16);
	write16_eeprom(pwpsc, addr,wr_val16);
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_read_efuse_map(struct wpsctrl *pwpsc, unsigned char *map)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + EFUSE_MAP_MAX_SIZE;
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) return -1;

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(EFUSE_MAP);
	pmpioctlparam->len = EFUSE_MAP_MAX_SIZE;
	memset(pmpioctlparam->data, 0, pmpioctlparam->len);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	if (map == NULL) map = pwpsc->efuse_map;
	memcpy(map, pmpioctlparam->data, pmpioctlparam->len);

	free((void*)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_write_efuse_map(struct wpsctrl *pwpsc, unsigned char *map)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + EFUSE_MAP_MAX_SIZE;
	pmpioctlparam = (struct mp_ioctl_param*)malloc(msz);
	if (pmpioctlparam == NULL) return -1;

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(EFUSE_MAP);
	pmpioctlparam->len = EFUSE_MAP_MAX_SIZE;
	if (map == NULL) map = pwpsc->efuse_map;
	memcpy(pmpioctlparam->data, map, pmpioctlparam->len);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void*)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------

int r871x_read_efuse_raw(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, unsigned char *data)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;
	PEFUSE_ACCESS_STRUCT		pefusedata = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(EFUSE_ACCESS_STRUCT) + sizeof(unsigned char)*cnt;
	pmpioctlparam = (struct mp_ioctl_param*)malloc(msz);
	if (pmpioctlparam == NULL) return -1;

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(EFUSE);
	pmpioctlparam->len = sizeof(EFUSE_ACCESS_STRUCT) + sizeof(unsigned char)*cnt;
	memset(pmpioctlparam->data, 0, pmpioctlparam->len);

	pefusedata = (PEFUSE_ACCESS_STRUCT)pmpioctlparam->data;
	pefusedata->start_addr = (unsigned short)addr;
	pefusedata->cnts = (unsigned short)cnt;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	memcpy(data, pefusedata->data, cnt);

	free((void*)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_write_efuse_raw(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, unsigned char *data)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;
	PEFUSE_ACCESS_STRUCT		pefusedata = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(EFUSE_ACCESS_STRUCT) + sizeof(unsigned char)*cnt;
	pmpioctlparam = (struct mp_ioctl_param*)malloc(msz);
	if (pmpioctlparam == NULL) return -1;

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(EFUSE);
	pmpioctlparam->len = sizeof(EFUSE_ACCESS_STRUCT) + sizeof(unsigned char)*cnt;;

	pefusedata = (PEFUSE_ACCESS_STRUCT)pmpioctlparam->data;
	pefusedata->start_addr = (unsigned short)addr;
	pefusedata->cnts = (unsigned short)cnt;
	memcpy(pefusedata->data, data, cnt);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void*)pmpioctlparam);

	return ret;
}

//----------------------------------------------------------------------------------------
int read_rf_reg(struct wpsctrl *pwpsc, unsigned int path, unsigned int offset, unsigned int *value)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops	*pwpsc_ops;
	struct mp_ioctl_param		*pmpioctlparam = NULL;
	struct r8712_rf_reg_param	*prfreg = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(struct r8712_rf_reg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(READ_RF_REG);
	pmpioctlparam->len = sizeof(struct r8712_rf_reg_param);

	prfreg = (struct r8712_rf_reg_param *)pmpioctlparam->data;
	memset(prfreg, 0, sizeof(struct r8712_rf_reg_param));
	prfreg->path = path;
	prfreg->offset = offset;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*value = prfreg->value;
//	printf("rtl871x_read_rf_reg path:%d offset:0x%02x value:0x%08x\n", prfreg->path, prfreg->offset, (unsigned char)prfreg->value);
	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int write_rf_reg(struct wpsctrl *pwpsc, unsigned int path, unsigned int offset, unsigned int value)
{
	size_t msz;
	int ret = 0;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct r8712_rf_reg_param *prfreg = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct r8712_rf_reg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(WRITE_RF_REG);
	pmpioctlparam->len = sizeof(struct r8712_rf_reg_param);

	prfreg = (struct r8712_rf_reg_param *)pmpioctlparam->data;

	prfreg->path = path;
	prfreg->offset = offset;
	prfreg->value = value;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_read_rfreg(struct wpsctrl *pwpsc, unsigned int path, unsigned int offset, unsigned int *value)
{
	read_rf_reg(pwpsc, path, offset, value);
	return 0;
}

int r871x_write_rfreg(struct wpsctrl *pwpsc, unsigned int path, unsigned int offset, unsigned int value)
{
	write_rf_reg(pwpsc, path, offset, value);
	return 0;
}
//----------------------------------------------------------------------------------------
int read_bb_reg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int *value)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct r8712_bb_reg_param *prwreg = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct r8712_bb_reg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(READ_BB_REG);
	pmpioctlparam->len = sizeof(struct r8712_bb_reg_param);

	prwreg = (struct r8712_bb_reg_param *)pmpioctlparam->data;
	memset(prwreg, 0, sizeof(struct r8712_bb_reg_param));

	prwreg->offset = offset;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*value = prwreg->value;

	//printf("$-rtl871x_read_bb_reg offset:0x%02x value:0x%02x\n",prwreg->offset,(unsigned char)prwreg->value);
	free((void *)pmpioctlparam);

	return ret;
}

//----------------------------------------------------------------------------------------
int write_bb_reg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int value)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	struct r8712_bb_reg_param *prwreg = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (struct r8712_bb_reg_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(WRITE_BB_REG);
	pmpioctlparam->len = sizeof(struct r8712_bb_reg_param);

	prwreg = (struct r8712_bb_reg_param *)pmpioctlparam->data;

	prwreg->offset = offset;
	prwreg->value = value;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);
	//printf("$-write_bb_reg bOFDM:0x%02x offset:0x%02x value:0x%02x\n",prwreg->bOFDM ,prwreg->offset,(unsigned char)prwreg->value);
	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_read_bbreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int *value)
{
	read_bb_reg(pwpsc, offset, value);
	return 0;
}

int r871x_write_bbreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int value)
{
	write_bb_reg(pwpsc, offset, value);
	return 0;
}

//----------------------------------------------------------------------------------------
/*++

Routine Description:

	This routine sets the DUT to stop(Mode:0) or start(Mode:1) the
	single carrier continus tx.

Arguments:

	Mode -	0:  Disable
		1:  Enable

Return Value:

	Result - TRUE: Success

--*/

#if 0
int r871x_set_single_carrier_tx(struct wpsctrl *pwpsc, int Mode)
{
	int Result = TRUE;
/*
         Transmit LO-only signal:
	  0.1. Set SW decided T/R switch to TX path
		MAC offset 0x266=0xb (default=0x0, hardware decided)
         0.2. Turn on PA
            MAC offset 0x265[7:4]=0xf0 (default=0, hardware decided)

         1. switch RFSI from HW to SW
         2. RF register offset00=089 (original=088)
         3. RF register offset07=c64 (original=464)
         4. RF register offset00=088
         5. RF register offset04=414
          return to normal mode:

	 0.1	Turn off PA
            MAC reg265[7:4]=0x00
        0.2	Set SW decided T/R switch to RX path
            MAC reg266=0x00
         1. RF register offset00=089 (original=088)
         2. RF register offset07=464 (original=464)
         3. RF register offset00=088
         4. RF register offset04=974
         5. switch RFSI from SW to HW
*/

	unsigned short val16;
	if(Mode)
	{
		//WriteRegData(AdapterObject, 0x1d200266, 1, 0x0b);
		r871x_write_reg(pwpsc, 0x1d200266, 1, 0x0b);

		//WriteRegData(AdapterObject, 0x1d200265, 1, 0xf0);
		r871x_write_reg(pwpsc, 0x1d200265, 1, 0xf0);

		//SetRFIntfs(AdapterObject, SWSI);
		set_rf_intfs(pwpsc,SWSI);

		//RFWriteRegData(AdapterObject, 0x00, 4, 0x089);//change page
		write_rf_reg(pwpsc, 0x00, 4, 0x089);

		//RFWriteRegData(AdapterObject, 0x07, 4, 0xc64);
		write_rf_reg(pwpsc,0x07, 4, 0xc64);

		//RFWriteRegData(AdapterObject, 0x00, 4, 0x088);
		write_rf_reg(pwpsc, 0x00, 4, 0x088);

		val16 = ((unsigned char)pwpsc->power_index| 0x400);
		//RFWriteRegData(AdapterObject, 0x04, 4, val16);
		write_rf_reg(pwpsc,0x04, 4, val16);

	}
	else
	{
		//WriteRegData(AdapterObject, 0x1d200265, 1, 0x00);
		r871x_write_reg(pwpsc, 0x1d200265, 1, 0x00);

		//WriteRegData(AdapterObject, 0x1d200266, 1, 0x00);
		r871x_write_reg(pwpsc, 0x1d200266, 1, 0x00);

       	//RFWriteRegData(AdapterObject, 0x00, 4, 0x089);
       	write_rf_reg(pwpsc, 0x00, 4, 0x089);

		//RFWriteRegData(AdapterObject, 0x07, 4, 0x464);
		write_rf_reg(pwpsc, 0x07, 4, 0x464);

		//RFWriteRegData(AdapterObject, 0x00, 4, 0x088);
		write_rf_reg(pwpsc, 0x00, 4, 0x088);

		//RFWriteRegData(AdapterObject, 0x04, 4, 0x974);
		write_rf_reg(pwpsc, 0x04, 4, 0x974);

		//SetRFIntfs(AdapterObject, HWPI);
		set_rf_intfs(pwpsc,HWPI);
	}
	return Result;
}
//----------------------------------------------------------------------------------------
void TxSettingProcedure(struct wpsctrl *pwpsc, int bstart, unsigned int RegValue)
{
	/*
	//Disable OFDM continuous Tx mode
	. 10 od 1d200240 01000003

	//Set CCK continuous TX rate
	1.CCK offset 0x1[5:4], 0=>1M, 1=>2M, 2=>5.5M, 3=>11M (default)

	//continouse packet Tx
	2. 10 orfs 04 414
	3. 10 od 1d200264 000bf000
	4. <send one packet>
	5. 10 od 1d200240 01008a80

	//disable continue CCK
	1. 10 od 1d200240 0100c880
	2. 10 od 1d200264 00000000
	3. 10 orf 04 414
	*/

	unsigned int val32;
	unsigned short val16;
	unsigned char val8 = 3;//11M

	unsigned int rate_idx = pwpsc->rate_index ;
	unsigned int power_idx =pwpsc->power_index ;

	if(bstart)
	{
		if		(rate_idx==0)	val8 = 0;
		else if	(rate_idx==1)	val8 = 1;
		else if	(rate_idx==2)	val8 = 2;
		else if	(rate_idx==3)	val8 = 3;
		else 			       val8 = 0;

		val32 = (0x01000081 | (val8<<8) );
		//WriteRegData(AdapterObject, 0x1d200240, 4, v32);
		r871x_write_reg(pwpsc,0x1d200240, 4, val32);

		//SetRFIntfs(AdapterObject, SWSI);
		set_rf_intfs(pwpsc, SWSI);

		val16 = ((unsigned char)power_idx | 0x400);
		//RFWriteRegData(AdapterObject,0x04,4,v16);
		write_rf_reg(pwpsc,0x04,4,val16);

		//WriteRegData(AdapterObject, 0x1d200264, 4, 0x000bf000);
		r871x_write_reg(pwpsc, 0x1d200264, 4, 0x000bf000);

		//CTx_CCK=>0x01008a80   CarrierSuppressionTx=>0x01008280
		//WriteRegData(AdapterObject, 0x1d200240, 4, RegValue);
		r871x_write_reg(pwpsc,0x1d200240, 4, RegValue);

	}
	else
	{

		//WriteRegData(AdapterObject, 0x1d200240, 4, 0x0100c880);
		r871x_write_reg(pwpsc,0x1d200240, 4, 0x0100c880);

		//WriteRegData(AdapterObject, 0x1d200264, 4, 0x00000000);
		r871x_write_reg(pwpsc, 0x1d200264, 4, 0x00000000);

		//SetRFIntfs(AdapterObject, HWPI);
		set_rf_intfs(pwpsc,HWPI);

		//RFWriteRegData(AdapterObject,0x04,4,0x972);
		write_rf_reg(pwpsc,0x04,4,0x972);
	}
}
//---------------------------------------------------------------------------------------------
void CarrierSuppressionTx(struct wpsctrl *pwpsc, int bstart)
{
	TxSettingProcedure(pwpsc,bstart,0x01008280);
}
//---------------------------------------------------------------------------------------------
void r871x_continuous_tx_cck(struct wpsctrl *pwpsc, int bstart)
{
	TxSettingProcedure(pwpsc,bstart,0x01008a80);
}

unsigned char ContTxRate[8]={0x0b, 0x0f, 0x0a, 0x0e, 0x09, 0x0d, 0x08, 0x0c};
void r871x_continuous_tx_ofdm(struct wpsctrl *pwpsc, int bstart)
{
	unsigned short v16;
	if(bstart)
	{
		//Set TX rate, OFDM offset 0x3e[3:0]
		//WriteBBData(AdapterObject,0x3e,1,ContTxRate[AdapterObject->DataRate_inx-4]);
		write_bb_reg(pwpsc,0x3e,1,ContTxRate[pwpsc->rate_index-4]);

		// Enable OFDM continuous TX, OFDM reg03[7] 1:enable, 0:disable.
		//WriteBBData(AdapterObject,  0x03, 1,    0x80);
		write_bb_reg(pwpsc, 0x03, 1,    0x80);

		// Set RF to TX mode & Disable Hardware HSSI, RF reg04=40f
		//SetRFIntfs(AdapterObject, SWSI);
		set_rf_intfs(pwpsc,SWSI);

		v16 = (unsigned short)(pwpsc->power_index | 0x400);
		//RFWriteRegData(AdapterObject,0x04,4,v16);
		write_rf_reg(pwpsc,0x04,4,v16);

		// Set SW decided T/R switch to TX path
		// MAC offset 0x266=0x0f/0x0a (default=0x0, hardware decided)
		//WriteRegData(AdapterObject, 0x1d200264, 4, 0x000bf000);
		r871x_write_reg(pwpsc,  0x1d200264, 4, 0x000bf000);

		// Turn on PA
		// MAC offset 0x265[7:4]=0xf0 (default=0, hardware decided)
		//WriteRegData(AdapterObject, 0x1d200265, 1, 0xf0);
		r871x_write_reg(pwpsc,0x1d200265, 1, 0xf0);

	}
	else
	{
		// Disable OFDM continuous TX, OFDM reg03[7] 1:enable, 0:disable.
		//WriteBBData(AdapterObject,0x03,1,0x00);
		write_bb_reg(pwpsc,0x03,1,0x00);

		// Turn off PA, MAC reg265[7:4]=0x00
		//WriteRegData(AdapterObject, 0x1d200265, 1, 0x00);
		r871x_write_reg(pwpsc, 0x1d200265, 1, 0x00);

		//Set SW decided T/R switch to RX path, MAC reg266=0x00
		//WriteRegData(AdapterObject, 0x1d200266, 1, 0x00);
		r871x_write_reg(pwpsc, 0x1d200266, 1, 0x00);

		// Set RF to RX mode & enable hardware HSSI, RF reg04=0x972
		//SetRFIntfs(AdapterObject, HWPI);
		set_rf_intfs(pwpsc, HWPI);

		//RFWriteRegData(AdapterObject,0x04,4,0x972);
		write_rf_reg(pwpsc,0x04,4,0x972);

	}

}

void r871x_set_continuous_tx(struct wpsctrl *pwpsc, int bstart)
{
#ifdef _PTM_SET_AUTO_
	if(bstart){
		if (pwpsc->rate_index < 4)//CCK
			r871x_write_reg(pwpsc, 0x1d2000f0, 1, 0xc0);//CCK
		else
			r871x_write_reg(pwpsc, 0x1d2000f0, 1, 0x40);//OFDM

	}
#endif
	if (bstart) {
		printf("$-r871x_set_continuous_tx start\n");

		r871x_set_single_carrier_tx(pwpsc, TRUE);
		sleep(1);
		r871x_set_single_carrier_tx(pwpsc, FALSE);

		r871x_set_pkt_xmit(pwpsc,TRUE);
	}
	else {
		r871x_set_pkt_xmit(pwpsc,FALSE);
		sleep(1);
#ifdef _PTM_SET_AUTO_
		r871x_set_ptm(pwpsc,_POWERTRACK_OFF_);
#endif
	}

	sleep(1);
	if(pwpsc->rate_index < 4){
		r871x_continuous_tx_cck(pwpsc,bstart);
	}
	else{
	 	r871x_continuous_tx_ofdm(pwpsc,bstart);
	}

	sleep(2);
	if(bstart)
	{
#ifdef _PTM_SET_AUTO_
		r871x_set_ptm(pwpsc,_POWERTRACK_OFF_);
		sleep(1);
		if(pwpsc->rate_index<4)
			r871x_set_ptm(pwpsc,_POWERTRACK_CUTTX_CCK_);
		else
			r871x_set_ptm(pwpsc,_POWERTRACK_CUTTX_OFDM_);

#endif

	}
}
//---------------------------------------------------------------------------------------------
void r871x_set_carrier_suppression_tx(struct wpsctrl *pwpsc, int bstart)
{
	if(bstart){

		if(pwpsc->rate_index > 4){//just for CCK
			pwpsc->rate_index = 0;
			r871x_set_datarate(pwpsc, pwpsc->rate_index);
		}


		r871x_set_pkt_xmit(pwpsc,TRUE);

		r871x_set_single_carrier_tx(pwpsc , TRUE);
		sleep(1);
		r871x_set_single_carrier_tx(pwpsc, FALSE);

		printf("$-r871x_set_carrier_suppression_tx start \n");
	}
	else{
		r871x_set_pkt_xmit(pwpsc,FALSE);
		printf("$-r871x_set_carrier_suppression_tx stop \n");
	}

	CarrierSuppressionTx(pwpsc, bstart);
}
#endif
//---------------------------------------------------------------------------------------------
int r871x_set_continuous_tx(struct wpsctrl *pwpsc, int bstart)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(CNTU_TX);
	pmpioctlparam->len = sizeof(unsigned int);
	*((unsigned int*)pmpioctlparam->data) = (unsigned int)bstart;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//---------------------------------------------------------------------------------------------
int r871x_set_single_carrier_tx(struct wpsctrl *pwpsc, int bstart)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SC_TX);
	pmpioctlparam->len = sizeof(unsigned int);
	*((unsigned int*)pmpioctlparam->data) = (unsigned int)bstart;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//---------------------------------------------------------------------------------------------
int r871x_set_carrier_suppression_tx(struct wpsctrl *pwpsc, int bstart)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}

	memset(pmpioctlparam, 0, msz);
	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(CS_TX);
	pmpioctlparam->len = sizeof(unsigned int);
	*((unsigned int*)pmpioctlparam->data) = (unsigned int)bstart;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
//---------------------------------------------------------------------------------------------
int packet_send(struct wpsctrl *pwpsc, unsigned char *packet, int pktlen)
{
#if 1
	struct sockaddr *paddr = (struct sockaddr*)&pwpsc->socket_address;
	return sendto(pwpsc->skpkt, packet, pktlen, 0, paddr, sizeof(struct sockaddr_ll));
#else
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	struct mp_xmit_packet *txpkt = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(struct mp_xmit_packet);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(IOCTL_XMIT_PACKET);
	pmpioctlparam->len = sizeof(struct mp_xmit_packet);

	txpkt = (struct mp_xmit_packet *)pmpioctlparam->data;

	txpkt->len = pktlen;

	memcpy(txpkt->mem,packet,pktlen);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);
	free((void *)pmpioctlparam);

	return ret;
#endif
}
//---------------------------------------------------------------------------------------------
const int MAX_PKT_BUF_SIZE = 2048;
const int ETHER_HEADER_LEN = ETH_HLEN;	// 14

const int TxEthHeaderOffset = 0;
const int TxDescOffset = ETH_HLEN;//ETHER_HEADER_LEN = 2*ETH_ALEN + 2
const int TxDescSize = 32;
const int TxPayloadOffset = 46;//TxDescOffset + TxDescSize;

const int PAYLOAD_HEADER_LEN = 17;
const int CRC_LEN = 1;
const int PAYLOAD_FIX_LEN = 18;//PAYLOAD_HEADER_LEN + CRC_LEN;

const unsigned char HeaderSID[6] = {0x00, 0xE0, 0x4C, 0x12, 0x34, 0x56};
static const unsigned char payloaddata[] = {0x00,0x5a,0xa5,0xff};

const unsigned int crc_ta[16] = {	// CRC餘式表
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

static unsigned char cal_crc(const unsigned char *ptr, unsigned int len)
{
	unsigned int icrc = 0;
	unsigned short scrc = 0;
	unsigned char da;

	if (!ptr) return 0;

	while (len-- != 0) {
		da = ((unsigned char)(icrc/256))/16;	// save the high bits of CRC
		icrc <<= 4;			// right shift CRC 4 bits = take low 12 bits of CRC
		icrc ^= crc_ta[da^(*ptr/16)];	// CRC的高4位和本字的前半字元相加後查表算CRC，然後加上上一次CRC的餘數
		da = ((unsigned char)(icrc/256))/16;	// save the high bits of CRC
		icrc <<= 4;			// right shift CRC 4 bits is equal to take low 12 bits of CRC
		icrc ^= crc_ta[da^(*ptr&0x0f)];	// CRC的高4位和本字元的後半字元相加後查表算CRC,然後再加上上一次CRC的餘數
		ptr++;
	}
	scrc = ((icrc&0xFF000000)>>24)+((icrc&0x00FF0000)>>16)+((icrc&0x0000FF00)>>8)+(icrc&0x000000FF);
	return ((unsigned char)(scrc &0xFF));
}

void* r871x_pkt_xmit(void* arg)
{

#ifdef _FORCE_CANCEL_
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

	int bContinuous = TRUE;
	int i;
	unsigned int Xmited = 0, pktlen = 0, datalen = 0;
	unsigned char pxmitbuff[MAX_PKT_BUF_SIZE], *ppayload;
	unsigned char crc = 0;

	struct wpsctrl *pwpsc = (struct wpsctrl *)arg;

	/*prepare sockaddr_ll*/

	/*RAW communication*/
	pwpsc->socket_address.sll_family   = PF_PACKET;
	/*we don't use a protocoll above ethernet layer
	  ->just use anything here*/
	pwpsc->socket_address.sll_protocol = htons(ETH_P_IP);

	/*index of the network device
	see full code later how to retrieve it*/
	pwpsc->socket_address.sll_ifindex  = if_nametoindex(interface);

	/*ARP hardware identifier is ethernet*/
	pwpsc->socket_address.sll_hatype   = ARPHRD_ETHER;

	/*target is another host*/
	pwpsc->socket_address.sll_pkttype  = PACKET_OTHERHOST;

	/*address length*/
	pwpsc->socket_address.sll_halen    = ETH_ALEN;

	/*MAC - begin*/
	for (i = 0; i < ETH_ALEN; i++)
		pwpsc->socket_address.sll_addr[i] = pwpsc->da[i];

	/*MAC - end*/
	pwpsc->socket_address.sll_addr[ETH_ALEN]  = 0x00;/*not used*/
	pwpsc->socket_address.sll_addr[ETH_ALEN+1]  = 0x00;/*not used*/

	/* end of prepare sockaddr_ll */

	/* initial TxDesc */
	// in driver, only DWORD 3,5,6 used
	pwpsc->TxDesc.rty_lmt_en = 0;	// MAC dont retry
	pwpsc->TxDesc.dis_fb = 1;	// no rate fallback during retry

	pwpsc->TxDesc.use_rate = 1;
	if (pwpsc->rate_index > 0xB)
		pwpsc->TxDesc.tx_ht = 1;
	else
		pwpsc->TxDesc.tx_ht = 0;
	pwpsc->TxDesc.tx_rate = pwpsc->rate_index;

	pwpsc->TxDesc.tx_sc = 0;//Dont care
	pwpsc->TxDesc.tx_bw = pwpsc->bandwidth;

	pwpsc->TxDesc.txpktsize = PAYLOAD_FIX_LEN + pwpsc->payload_len; // not used
	/* end of initial TxDesc */

	memset(pxmitbuff, 0, sizeof(unsigned char)*MAX_PKT_BUF_SIZE);

	//ether 2 header - 14 bytes
	for(i=0; i<ETH_ALEN; i++) *(pxmitbuff + i)= pwpsc->da[i]; //DA, 0~5
	for(i=0; i<ETH_ALEN; i++) *(pxmitbuff + ETH_ALEN + i) = HeaderSID[i];//SA, 6~11
	*(pxmitbuff + (ETH_HLEN - 2)) = 0x87;	// 12
	*(pxmitbuff + (ETH_HLEN - 1)) = 0x12;	// 13

	// Tx Desc
	memcpy(pxmitbuff + TxDescOffset, &pwpsc->TxDesc, sizeof(struct _tx_desc_8712));

	// payload
	ppayload = pxmitbuff + TxPayloadOffset;

	// sequence number, 1~4
	*ppayload = 0x0;
	*(ppayload + 1) = 0x0;
	*(ppayload + 2) = 0x0;
	*(ppayload + 3) = 0x0;

	// data type, 5~6
	*(ppayload + 4) = pwpsc->payload_type & 0xFF;
	*(ppayload + 5) = (pwpsc->payload_type >> 8) & 0xFF;

	// Packet Len, 7~8
	datalen = pwpsc->payload_len;
	pktlen = PAYLOAD_FIX_LEN + datalen;
	*(ppayload + 6) = pktlen & 0xFF;
	*(ppayload + 7) = (pktlen >> 8) & 0xFF;

	// undefined, 9~17
	for (i = 8; i < PAYLOAD_HEADER_LEN; i++)
		*(ppayload + i) = (unsigned char)0xFF;

	// stuff
	ppayload += PAYLOAD_HEADER_LEN;
#if 1
	if (pwpsc->payload_type < 4)
		memset(ppayload, payloaddata[pwpsc->payload_type], datalen);
#else
	for (count = 0; count < datalen; count++)
		if (pwpsc->payload_type < 4)
			*(ppayload + count) = payloaddata[pwpsc->payload_type];
#endif
	// insert crc in last byte
	ppayload += datalen;
	crc = cal_crc(pxmitbuff + TxPayloadOffset, PAYLOAD_HEADER_LEN + datalen);
	*ppayload = crc;

	if (pwpsc->mp_test_mode != PACKET_TX) {
//		if (pwpsc->rate_index < 4)//CCK
//			pwpsc->tx_pkt_cnts = 4;
//		else
			pwpsc->tx_pkt_cnts = 1;
	}
	if (pwpsc->tx_pkt_cnts == 0)
		printf("$-TH:pkt xmit continued\n");
	else
		printf("$-TH:pkt_cnts:%d\n", pwpsc->tx_pkt_cnts);

	pktlen += ETHER_HEADER_LEN + TxDescSize;
	while (bContinuous)
	{
//		memcpy(pxmitbuff + TxPayloadOffset, &Xmited, 4);
		for (i = 0; i < 4; i++)
			*(pxmitbuff + TxPayloadOffset + i) = (Xmited >> (8*i)) & 0xFF;
		crc = cal_crc(pxmitbuff + TxPayloadOffset, PAYLOAD_HEADER_LEN + datalen);
		*ppayload = crc;

		if (packet_send(pwpsc, pxmitbuff, pktlen) == -1)
			printf("send packet error\n");

		Xmited++;
#ifdef _DBG_MSG_
		printf("$-TH:pkt %d sended\n",Xmited);
#endif

		if (pwpsc->mp_test_mode != PACKET_TX)
			sleep(2);

#ifdef _FORCE_CANCEL_
		pthread_testcancel();
#endif
		if (pwpsc->tx_pkt_cnts == 0) {
			bContinuous = pwpsc->bstarttx;
		} else {
			pwpsc->tx_pkt_cnts--;
			bContinuous = (pwpsc->bstarttx && pwpsc->tx_pkt_cnts);
		}
	}

#ifdef _DBG_MSG_
	printf("$-TH:pkt thread exit~\n");
#endif

	return NULL;
}

//----------------------------------------------------------------------------------------
#include <signal.h>
pthread_t pkt_xmit_thread;
pthread_attr_t attr;

void r871x_set_pkt_xmit(struct wpsctrl *pwpsc,int bstart)
{
	pwpsc->bstarttx = bstart;
	if (bstart == TRUE) {
		int res = 0;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

#ifdef _PTM_SET_AUTO_
		if((pwpsc->bGENTSSI == FALSE) && (pwpsc->mp_test_mode == PACKET_TX))
		{
			r871x_set_ptm(pwpsc,_POWERTRACK_PKTTX_);
		}
#endif
		res = pthread_create(&pkt_xmit_thread, &attr, r871x_pkt_xmit, (void *)pwpsc);
		if (res) {
			printf("Thread creation failed!!\n");
			//exit(EXIT_FAILURE);
		}
	} else {
#ifdef _PTM_SET_AUTO_
		if((pwpsc->bGENTSSI == FALSE) && (pwpsc->mp_test_mode == PACKET_TX))
		{
			r871x_set_ptm(pwpsc,_POWERTRACK_OFF_);
		}
#endif
#ifdef _FORCE_CANCEL_
		if (!pthread_cancel(pkt_xmit_thread)) {
			printf("$-pkt xmit pthread_cancel OK\n");
		}
#else
		if (!pthread_join(pkt_xmit_thread, NULL)) {
			printf("$-pkt tx stop\n");
			//abort();
		}
#endif
	}
}
//----------------------------------------------------------------------------------------
int get_tssi(struct wpsctrl *pwpsc, unsigned int *tssi)
{
	int ret = 0;
#if 0
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;
	unsigned int *tmptssi = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(READ_TSSI);
	pmpioctlparam->len = sizeof(unsigned int);

	tmptssi = (unsigned int *)pmpioctlparam->data;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*tssi =(*tmptssi)&0xFF;
	free((void *)pmpioctlparam);
#endif
	return ret;
}
//----------------------------------------------------------------------------------------
int r871x_get_target_tssi(struct wpsctrl *pwpsc, unsigned int *tarteg_tssi)
{
#ifdef _PTM_SUPPORTED_
	unsigned int tmpvalue = 0,tmp = 0, times = 0 ;
	int sum = 0 ;
	int sampletimes = 10;

	while(times < sampletimes){
		get_tssi(pwpsc,&tmpvalue);
		tmp = (((unsigned char)(tmpvalue)) & 0x7F);
		//printf("$-tmp target tssi :0x%02x\n",tmp);
		sum += (((unsigned char)(tmpvalue)) & 0x7F);

		times++;
		sleep(1);
	}

	*(tarteg_tssi) = (sum / sampletimes) ;

    	printf("$-r871x_get_target_tssi value:0x%02x\n",*tarteg_tssi);
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int eeprom_tssi_table_access(struct wpsctrl *pwpsc, int bOFDM,int bRead)
{
#if 0
	int i;
	unsigned int start_addr;
	unsigned char *ptssitab;

	if(!bOFDM){ //CCK
		start_addr = 0x19;
		//ptssitab = pwpsc->tssi_table.cck;
		ptssitab = pwpsc->eeprom_info.TSSI.CCK;
	}
	else{	//OFDM
		start_addr = 0x1d;
		//ptssitab = pwpsc->tssi_table.ofdm;
		ptssitab = pwpsc->eeprom_info.TSSI.OFDM;
	}
	if(bRead){
		for(i=0;i<4;i++){
			r871x_read_eeprom_byte(pwpsc, start_addr+i ,ptssitab+i);
		}
	}
	else{
		for(i=0;i<4;i++){
			//printf("$- write_eeprom offset:0x%02x value:0x%02x\n",start_addr+i,*(ptssitab+i));
			r871x_write_eeprom_byte(pwpsc, start_addr+i ,*(ptssitab+i));
		}
	}
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_read_tssi_from_eeprom(struct wpsctrl *pwpsc, int bOFDM)
{
#ifdef _PTM_SUPPORTED_
	//printf("$-r871x_read_tssi_from_eeprom \n");
	eeprom_tssi_table_access(pwpsc, bOFDM,TRUE);
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_write_tssi_into_eeprom(struct wpsctrl *pwpsc, int bOFDM)
{
#ifdef _PTM_SUPPORTED_
	//printf("$-r871x_write_tssi_into_eeprom \n");
	eeprom_tssi_table_access(pwpsc, bOFDM,FALSE);
#endif
	return 0;
}
//----------------------------------------------------------------------------------------
int r871x_read_thermal_meter(struct wpsctrl *pwpsc, unsigned int *thermal_meter)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(GET_THERMAL_METER);
	pmpioctlparam->len = sizeof(unsigned int);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*thermal_meter = *(unsigned int*)pmpioctlparam->data;

	free((void *)pmpioctlparam);

	return ret;
}
//----------------------------------------------------------------------------------------
#define ACCESSTYPE_CHIPID	0x00
#define ACCESSTYPE_EEVERSION	0x01
#define ACCESSTYPE_VID		0x02
#define ACCESSTYPE_PID		0x03
#define ACCESSTYPE_MACADDR	0x04
#define ACCESSTYPE_POWER_CCK	0x05
#define ACCESSTYPE_POWER_OFDM	0x06
#define ACCESSTYPE_TSSI_CCK	0x07
#define ACCESSTYPE_TSSI_OFDM	0x08
#define ACCESSTYPE_CAP_AMODE	0x09
#define ACCESSTYPE_CAP_GMODE	0x0A
#define ACCESSTYPE_CHAN_PLAN	0x0B
#define ACCESSTYPE_COUNTRY_STR	0x0C

typedef struct _ACC_INFO_STRUCT_{
	int		DataCnt;
	unsigned char	Offset;
	unsigned char	NextState;
}ACC_INFO, *PACC_INFO;

ACC_INFO SDIO_DataInfo_8711[] = {
					//cnts, offset, NextState
/* 0 CHIPID*/		{2,		0x00,	ACCESSTYPE_EEVERSION	},
/* 1 EEVERSION*/	{2,		0x0A,	ACCESSTYPE_MACADDR	},
/* 2 VID*/		{0,		0xFF,	0xFF					},
/* 3 PID*/		{0,		0xFF,	0xFF					},
/* 4 MACADDR*/		{6,		0X03,	ACCESSTYPE_POWER_CCK	},
/* 5 POWER_CCK*/	{4,		0x11,	ACCESSTYPE_POWER_OFDM	},
/* 6 POWER_OFDM*/	{4,		0x15,	ACCESSTYPE_TSSI_CCK	},
/* 7 TSSI_CCK*/		{4,		0x19,	ACCESSTYPE_TSSI_OFDM	},
/* 8 TSSI_OFDM*/	{4,		0x1D,	ACCESSTYPE_CAP_AMODE	},
/* 9 GAP_AMODE*/	{1,		0xB8,	ACCESSTYPE_CAP_GMODE	},
/*10 GAP_GMODE*/	{1,		0xB9,	ACCESSTYPE_CHAN_PLAN	},
/*11 CHAN_PLAN*/	{1,		0x0F,	ACCESSTYPE_COUNTRY_STR},
/*12 COUNTRY_STR*/	{3,		0xB0,	0xFF					}
};
//----------------------------------------------------------------------------------------
int EEpromAccessFunction(struct wpsctrl *pwpsc,unsigned char *Data, int bRead,unsigned char AccessType)
{

	int i;
	PACC_INFO	PDataInfo;
	int			DataCnts;
	unsigned char	offset;

	PDataInfo = SDIO_DataInfo_8711 ;

	DataCnts	= PDataInfo[AccessType].DataCnt;
	offset	= PDataInfo[AccessType].Offset ;

	for(i=0 ; i< DataCnts ; i++){
		if(bRead==TRUE){
			//if(!ReadEEpromByte(AdapterObject, offset + i, &Data[i]))
			r871x_read_eeprom_byte(pwpsc,  offset + i,&Data[i]);

		}
		else{
			//if(!WriteEEpromByte(AdapterObject, offset + i, Data[i]))
			r871x_write_eeprom_byte(pwpsc, offset + i ,Data[i]);
		}

	}
	return 0;
}
//----------------------------------------------------------------------------------------
int ReadEEpromPowerCCK(struct wpsctrl *pwpsc, unsigned char*powertable )
{
	int bRead = TRUE;
	EEpromAccessFunction(pwpsc,powertable,bRead,ACCESSTYPE_POWER_CCK);
	return 0;
}
//-------------------------------------------------------------------------------------
int WriteEEpromPowerCCK(struct wpsctrl *pwpsc, unsigned char*powertable )
{
	int bRead = FALSE;
	EEpromAccessFunction(pwpsc,powertable,bRead,ACCESSTYPE_POWER_CCK);
	return 0;
}
//-------------------------------------------------------------------------------------
int ReadEEpromPowerOFDM(struct wpsctrl *pwpsc, unsigned char*powertable )
{
	int bRead = TRUE;
	EEpromAccessFunction(pwpsc,powertable,bRead,ACCESSTYPE_POWER_OFDM);
	return 0;
}
//-------------------------------------------------------------------------------------
int WriteEEpromPowerOFDM(struct wpsctrl *pwpsc, unsigned char*powertable )
{
	int bRead = FALSE;
	EEpromAccessFunction(pwpsc,powertable,bRead,ACCESSTYPE_POWER_OFDM);
	return 0;
}

int r871x_read_txpower_from_eeprom(struct wpsctrl *pwpsc, int bOFDM)
{
	if(bOFDM){
		ReadEEpromPowerOFDM(pwpsc, pwpsc->eeprom_info.TxPower.OFDM );
	}
	else{
		ReadEEpromPowerCCK(pwpsc, pwpsc->eeprom_info.TxPower.CCK );
	}
	return 0;
}

int r871x_write_txpower_into_eeprom(struct wpsctrl *pwpsc, int bOFDM)
{
	if(bOFDM){
		WriteEEpromPowerOFDM(pwpsc, pwpsc->eeprom_info.TxPower.OFDM ) ;
	}
	else{
		WriteEEpromPowerCCK(pwpsc, pwpsc->eeprom_info.TxPower.CCK );
	}
	return 0;
}

//-------------------------------------------------------------------------------------
int ReadEEpromTSSI_CCK(struct wpsctrl *pwpsc,unsigned char *tssitable )
{
	int bRead = TRUE;
	EEpromAccessFunction(pwpsc,tssitable,bRead,ACCESSTYPE_TSSI_CCK);
	return 0;
}
//-------------------------------------------------------------------------------------
int WriteEEpromTSSI_CCK(struct wpsctrl *pwpsc,unsigned char *tssitable )
{
	int bRead = FALSE;
	EEpromAccessFunction(pwpsc,tssitable,bRead,ACCESSTYPE_TSSI_CCK);
	return 0;
}
//-------------------------------------------------------------------------------------
int ReadEEpromTSSI_OFDM(struct wpsctrl *pwpsc,unsigned char *tssitable )
{
	int bRead = TRUE;
	EEpromAccessFunction(pwpsc,tssitable,bRead,ACCESSTYPE_TSSI_OFDM);
	return 0;
}
//-------------------------------------------------------------------------------------
int WriteEEpromTSSI_OFDM(struct wpsctrl *pwpsc,unsigned char *tssitable )
{
	int bRead = FALSE;
	EEpromAccessFunction(pwpsc,tssitable,bRead,ACCESSTYPE_TSSI_OFDM);
	return 0;
}
//-------------------------------------------------------------------------------------
int ReadEEpromMacAddr(struct wpsctrl *pwpsc,unsigned char *MacAddr )
{
	int bRead = TRUE;
	EEpromAccessFunction(pwpsc,MacAddr,bRead,ACCESSTYPE_MACADDR);
	return 0;
}
//-------------------------------------------------------------------------------------
int WriteEEpromMacAddr(struct wpsctrl *pwpsc,unsigned char *MacAddr )
{
	int bRead = FALSE;
	EEpromAccessFunction(pwpsc,MacAddr,bRead,ACCESSTYPE_MACADDR);
	return 0;
}
//-------------------------------------------------------------------------------------
int EEPROMlInfoAccess(struct wpsctrl *pwpsc,unsigned char *EEPROMData, EEPROMINF *EEPROMInf,int bGetData)
{
	int DataCounts=0,StartIdx = 0 ,i;
	unsigned char *BasedPointer;
	unsigned char AccessState = ACCESSTYPE_CHIPID; //init state
	PACC_INFO PDataInfo;
	PDataInfo = SDIO_DataInfo_8711;

	while(AccessState!=0xff){

		if	(AccessState == ACCESSTYPE_CHIPID )		//read CHIPID
			BasedPointer = EEPROMInf->ID;
		else if	(AccessState == ACCESSTYPE_EEVERSION )		//read EEPROM Version
			BasedPointer = EEPROMInf->EEVersion;
		else if	(AccessState == ACCESSTYPE_VID )		//read VID for USB Interface
			BasedPointer = EEPROMInf->VID;
		else if (AccessState == ACCESSTYPE_PID )		//read PID for USB Interface
			BasedPointer = EEPROMInf->PID;
		else if (AccessState == ACCESSTYPE_MACADDR ) 		//read MAC Address
			BasedPointer = EEPROMInf->MACAddr;
		else if (AccessState == ACCESSTYPE_POWER_CCK ) 		//read TxPowerTable - CCK
			BasedPointer = EEPROMInf->TxPower.CCK;
		else if (AccessState == ACCESSTYPE_POWER_OFDM )		// read TxPowerTable - OFDM - G BAND
			BasedPointer = EEPROMInf->TxPower.OFDM;
		else if (AccessState == ACCESSTYPE_TSSI_CCK )		//read TSSI- CCK
			BasedPointer = EEPROMInf->TSSI.CCK;
		else if (AccessState == ACCESSTYPE_TSSI_OFDM )		//read TSSI - OFDM
			BasedPointer = EEPROMInf->TSSI.OFDM;
		else if (AccessState == ACCESSTYPE_CAP_AMODE )		//read CAP_A_MODE
			BasedPointer = &(EEPROMInf->CAP[0]);
		else if (AccessState == ACCESSTYPE_CAP_GMODE )		//read CAP_G_MODE
			BasedPointer = &(EEPROMInf->CAP[1]);
		else if (AccessState == ACCESSTYPE_CHAN_PLAN )		//read CHANNEL_PLAN
			BasedPointer = &(EEPROMInf->ChannPlan);
		else if (AccessState == ACCESSTYPE_COUNTRY_STR )	//read CHANNEL_PLAN
			BasedPointer = EEPROMInf->Conutry;


		DataCounts = PDataInfo[AccessState].DataCnt;
		StartIdx = PDataInfo[AccessState].Offset ;

		for(i=0;i<DataCounts;i++){
			if(bGetData){
				*(BasedPointer+i) = EEPROMData[i+StartIdx];
			}
			else{
				if((AccessState==ACCESSTYPE_CHIPID) || (AccessState==ACCESSTYPE_EEVERSION)){}
				else{
					EEPROMData[i+StartIdx] = *(BasedPointer+i) ;
				}
			}
		}
		//change to next state
		AccessState = PDataInfo[AccessState].NextState;

	}//END OF While
	return 0;
}
//-------------------------------------------------------------------------------------
#define CCK_MODE	0
#define OFDM_MODE	1

int r871x_gen_tssi_procedure(struct wpsctrl *pwpsc)
{
#ifdef _PTM_SUPPORTED_
	int i,mode;
	unsigned char chid;
	unsigned int rate_index;

	unsigned char *ptssitab;
	unsigned char *ptxpowertab;
	pwpsc->bGENTSSI = TRUE;
	pwpsc->mp_test_mode = PACKET_TX ;
	pwpsc->tx_pkt_cnts = 0;

	memset(pwpsc->eeprom_info.TSSI.CCK,0,sizeof(unsigned char)*4);
	memset(pwpsc->eeprom_info.TSSI.OFDM,0,sizeof(unsigned char)*4);

	memset(pwpsc->eeprom_info.TxPower.CCK,0,sizeof(unsigned char)*4);
	memset(pwpsc->eeprom_info.TxPower.OFDM,0,sizeof(unsigned char)*4);

	ReadEEpromPowerCCK(pwpsc, pwpsc->eeprom_info.TxPower.CCK ) ;
	ReadEEpromPowerOFDM(pwpsc, pwpsc->eeprom_info.TxPower.OFDM ) ;

	r871x_set_ptm(pwpsc,_POWERTRACK_OFF_);
	sleep(1);


	for(mode=0;mode<2;mode++){
		if(mode==CCK_MODE){
			rate_index = 0;
			ptxpowertab = pwpsc->eeprom_info.TxPower.CCK;
			ptssitab = pwpsc->eeprom_info.TSSI.CCK;
		}
		else{
			rate_index = 11;
			ptxpowertab = pwpsc->eeprom_info.TxPower.OFDM;
			ptssitab = pwpsc->eeprom_info.TSSI.OFDM;
		}

		//r871x_set_datarate(pwpsc, rate_index);

		for(i=0;i<4;i++){
			if	(i==0)	chid = 1;
			else if(i==1)	chid = 6;
			else if(i==2)	chid = 11;
			else if(i==3)	chid = 14;
			r871x_set_channel(pwpsc, chid);
			sleep(1);
			printf("rate index:%d\n\n",rate_index);
			r871x_set_datarate(pwpsc, rate_index);
			sleep(1);
			printf("power:0x%02x\n\n",ptxpowertab[i]);
			r871x_set_txpower(pwpsc,ptxpowertab[i]);
			sleep(1);
			pwpsc->bstarttx = TRUE;
			r871x_set_pkt_xmit(pwpsc,pwpsc->bstarttx);
			sleep(10);

			r871x_get_target_tssi(pwpsc,(unsigned int *)&ptssitab[i] );
			pwpsc->bstarttx = FALSE;
			r871x_set_pkt_xmit(pwpsc,pwpsc->bstarttx);

			sleep(6);
		}

		r871x_write_tssi_into_eeprom(pwpsc, mode);// 0:CCK 1:OFDM

	}
	pwpsc->bGENTSSI = FALSE;
	printf("$-Aut-gen TSSI procedure finished \n\n");
#endif
	return 0;
}
//----------------------------------------------------------------------------------
#if 1
int r871x_pkt_recv_reset(struct wpsctrl *pwpsc)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(RESET_PHY_RX_PKT_CNT);
	pmpioctlparam->len = 0;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}
#else
void r871x_pkt_recv_reset(struct wpsctrl *pwpsc)
{
	unsigned int i, phyrx_set = 0;

	for (i = OFDM_PPDU_BIT; i <= HT_MPDU_FAIL_BIT; i++) {
		phyrx_set = 0;
		phyrx_set |= (i << 28);	//select
		phyrx_set |= 0x08000000;			// set counter to zero
		r871x_write_reg(pwpsc, 0x230, 4, phyrx_set);
	}
}
#endif
#if 0
//----------------------------------------------------------------------------------
int GetRxPhyPacketCounts(struct wpsctrl *pwpsc, int selbit, unsigned int *count)
{
	//selection
	unsigned int phyrx_set = 0, phyrx_cnt = 0;
	unsigned int SelectBit;

	SelectBit = selbit << 28;
	phyrx_set |= (SelectBit & 0xF0000000);

	r871x_write_reg(pwpsc, 0x230, 4, phyrx_set);

	//Read packet count
	r871x_read_reg(pwpsc, 0x230, 4, &phyrx_cnt);
	phyrx_cnt = phyrx_cnt & RPTMaxCount;
	*count = phyrx_cnt;

	//printf("$-GetRxPacketCount- phy_rx_cnts:0x%x\n",phyrx_cnt);
	return TRUE;
}
#endif
#if 0
//----------------------------------------------------------------------------------
unsigned int Calculatediff(unsigned int PastValue,unsigned int CurValue)
{
	if(CurValue >= PastValue)
		return (CurValue - PastValue);
	else
		return (CurValue + (RPTMaxCount - PastValue) );
}
#endif
//----------------------------------------------------------------------------------
int r871x_get_phy_rx_pkts_ok(struct wpsctrl *pwpsc,unsigned int* count)
{
#if 1
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(GET_PHY_RX_PKT_RECV);
	pmpioctlparam->len = sizeof(unsigned int);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*count = *(unsigned int*)pmpioctlparam->data;

	free((void *)pmpioctlparam);

	return ret;
#else
	unsigned int OFDM_cnt = 0, CCK_cnt = 0, HT_cnt = 0;

	GetRxPhyPacketCounts(pwpsc, OFDM_MPDU_OK_BIT, &OFDM_cnt);
	GetRxPhyPacketCounts(pwpsc, CCK_MPDU_OK_BIT, &CCK_cnt);
	GetRxPhyPacketCounts(pwpsc, HT_MPDU_OK_BIT, &HT_cnt);

	*count = OFDM_cnt + CCK_cnt + HT_cnt;
	return TRUE;
#endif
}
//----------------------------------------------------------------------------------
int r871x_get_phy_rx_pkts_error(struct wpsctrl *pwpsc, unsigned int *count)
{
#if 1
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(GET_PHY_RX_PKT_ERROR);
	pmpioctlparam->len = sizeof(unsigned int);

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_QUERY, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	*count = *(unsigned int*)pmpioctlparam->data;

	free((void *)pmpioctlparam);

	return ret;
#else
	unsigned int OFDM_cnt = 0, CCK_cnt = 0, HT_cnt = 0;

	GetRxPhyPacketCounts(pwpsc, OFDM_MPDU_FAIL_BIT, &OFDM_cnt);
	GetRxPhyPacketCounts(pwpsc, CCK_MPDU_FAIL_BIT, &CCK_cnt);
	GetRxPhyPacketCounts(pwpsc, HT_MPDU_FAIL_BIT, &HT_cnt);

	*count = OFDM_cnt + CCK_cnt + HT_cnt;
	return TRUE;
#endif
}
//----------------------------------------------------------------------------------
void* phy_pkt_recv_thread(void* arg)
{
//	unsigned int current_pkt_ok = 0, current_pkt_err = 0;
	struct wpsctrl *pwpsc = (struct wpsctrl *)arg;

	while (pwpsc->bstartrx == TRUE)
	{
		//printf("$-recv record packet ok cnts:%d\n",pwpsc->record_rx_pkt_ok_cnts);
		//printf("$-recv record packet err cnts:%d\n",pwpsc->record_rx_pkt_err_cnts);

		sleep(2);
#if 1
		r871x_get_phy_rx_pkts_ok(pwpsc, &pwpsc->rx_pkt_ok_cnts);
		r871x_get_phy_rx_pkts_error(pwpsc, &pwpsc->rx_pkt_err_cnts);
#else
		r871x_get_phy_rx_pkts_ok(pwpsc, &current_pkt_ok);
		r871x_get_phy_rx_pkts_error(pwpsc, &current_pkt_err);

		//printf("$-recv current packet err cnts:%d\n",current_pkt_err);

		pwpsc->rx_pkt_ok_cnts = Calculatediff(pwpsc->record_rx_pkt_ok_cnts, current_pkt_ok);
		pwpsc->rx_pkt_err_cnts = Calculatediff(pwpsc->record_rx_pkt_err_cnts, current_pkt_err);
#endif
//		printf("$-recv packet ok cnts:%d\n", pwpsc->rx_pkt_ok_cnts);
//		printf("$-recv packet error cnts:%d\n\n", pwpsc->rx_pkt_err_cnts);
		printf("\r$-recv packet cnts ok:%d error:%d", pwpsc->rx_pkt_ok_cnts, pwpsc->rx_pkt_err_cnts);
		fflush(stdout);
	}
	printf("\n");

	return NULL;
}
//----------------------------------------------------------------------------------
pthread_t pkt_recv_thread;
//----------------------------------------------------------------------------------
void r871x_get_pkt_recv(struct wpsctrl *pwpsc, int bstart)
{
	int res = 0;

	if (bstart)
	{
		pwpsc->bstartrx = TRUE;

		pwpsc->record_rx_pkt_ok_cnts = 0;
		pwpsc->record_rx_pkt_err_cnts = 0;

		pwpsc->rx_pkt_ok_cnts = 0;
		pwpsc->rx_pkt_err_cnts = 0;

		r871x_pkt_recv_reset(pwpsc);

		r871x_get_phy_rx_pkts_ok(pwpsc, &pwpsc->record_rx_pkt_ok_cnts);
		r871x_get_phy_rx_pkts_error(pwpsc, &pwpsc->record_rx_pkt_err_cnts);

		res = pthread_create(&pkt_recv_thread, NULL, phy_pkt_recv_thread, (void *)pwpsc);
		if (res)
			printf("rx thread creation failed!!\n");
		else {
			printf("press any key to quit\n");
			getchar_nodelay();
			bstart = 0;
		}
	}

	if (!bstart) {
		pwpsc->bstartrx = FALSE;
		if (!pthread_join(pkt_recv_thread, NULL)) {
			printf("$-pkt rx stop.\n");
		}
	}
}
//----------------------------------------------------------------------------------
int r871x_set_pkt_recv_type(struct wpsctrl *pwpsc, int type)
{
	size_t msz;
	int ret;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof(struct mp_ioctl_param) + sizeof(unsigned int);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);
	if (pmpioctlparam == NULL) {
		return -1;
	}
	memset(pmpioctlparam, 0, msz);

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_RX_PKT_TYPE);
	pmpioctlparam->len = sizeof(unsigned int);
	*(unsigned int*)pmpioctlparam->data = type;

	pwpsc_ops = pwpsc->pwpsc_ops;

	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	ret = pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							(unsigned char *)(pmpioctlparam), msz);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);
	pthread_mutex_unlock(&pwpsc->iocontrol_mutex);

	free((void *)pmpioctlparam);

	return ret;
}


//----------------------------------------------------------------------------------
void  r871x_power_down(struct wpsctrl *pwpsc, int bstart)
{
	size_t msz;
	struct wpsctrl_adapter_ops *pwpsc_ops;
	struct mp_ioctl_param *pmpioctlparam = NULL;

	msz = sizeof (struct mp_ioctl_param) + sizeof (unsigned char);
	pmpioctlparam = (struct mp_ioctl_param *)malloc(msz);

	if (pmpioctlparam == NULL) {
		//return -1;
	}

	pmpioctlparam->subcode = GEN_MP_IOCTL_SUBCODE(SET_POWER_DOWN);
	pmpioctlparam->len = sizeof(unsigned char);
	pmpioctlparam->data[0]=bstart;
	pwpsc_ops = pwpsc->pwpsc_ops;
	//_mutex_lock(&pwpsc->iocontrol_mutex);
	pthread_mutex_lock(&pwpsc->iocontrol_mutex);
	pwpsc_ops->iocontrol(pwpsc->skfd, IOCONTROL_SET, SIOCIWFIRSTPRIV + 0x3,
							(unsigned char *)(pmpioctlparam), msz,
							NULL, 0);
	//_mutex_unlock(&pwpsc->iocontrol_mutex);

}
