#ifndef _RTL871X_MP_IOCTL_H
#define _RTL871X_MP_IOCTL_H

#define MP_TOOLS

//#define MAX_MP_XMITBUF_SZ 256
#define MAX_MP_XMITBUF_SZ 2048
#define NR_MP_XMITFRAME	8

#define PWR_CAM 	0
#define PWR_MINPS 	1
#define PWR_MAXPS 	2
#define PWR_UAPSD 	3
#define PWR_VOIP 	4

#define SMART_PW_DISABLE 	0
#define SMART_PW_ENABLE 	1

#define TRUE 	1
#define FALSE 	0

#define IOCONTROL_SET		0x01
#define IOCONTROL_QUERY 	0x00

#define RX_PKT_BROADCAST	1
#define RX_PKT_DEST_ADDR	2
#define RX_PKT_PHY_MATCH	3

//#define _FLAG_CANCEL_TH_

//#define MAX_IOCONTROLBUFSZ	(1024*16)
//#define MAX_PADDEDIE		512

#define _POWERTRACK_OFF_	0
#define _POWERTRACK_PKTTX_	1
#define _POWERTRACK_CUTTX_OFDM_	2
#define _POWERTRACK_CUTTX_CCK_	3

#define MAX_DATA_RATE		28	// 0~27

#define OFDM_PHY 	1
#define MIXED_PHY 	2
#define CCK_PHY		3

typedef enum _HT_CHANNEL_WIDTH {
	HT_CHANNEL_WIDTH_20 = 0,
	HT_CHANNEL_WIDTH_20_40 = 1,
}HT_CHANNEL_WIDTH, *PHT_CHANNEL_WIDTH;

enum RFINTFS {
	SWSI,
	HWSI,
	HWPI
};

#define ANTENNA_A	0x1	// Useless
#define ANTENNA_B	0x2
#define ANTENNA_AB	0x3	// ANTENNA_A | ANTENNA_B

#define ANTENNA_C	0x4
#define ANTENNA_D	0x8

enum MP_TEST_MODE{
	TEST_NONE,
	PACKET_TX,
	CONTINUOUS_TX,
	SINGLE_CARRIER_TX,
	CARRIER_SUPPRISSION_TX,
	PACKET_RX
};

#define RPTMaxCount 0x000FFFFF

//----------------------------------------------------------
// parameter 1 : BitMask
// 	bit 0  : OFDM PPDU
//	bit 1  : OFDM False Alarm
//	bit 2  : OFDM MPDU OK
//	bit 3  : OFDM MPDU Fail
//	bit 4  : CCK PPDU
//	bit 5  : CCK False Alarm
//	bit 6  : CCK MPDU ok
//	bit 7  : CCK MPDU fail
//	bit 8  : HT PPDU counter
//	bit 9  : HT false alarm
//	bit 10 : HT MPDU total
//	bit 11 : HT MPDU OK
//	bit 12 : HT MPDU fail
//	bit 15 : RX full drop
typedef enum _RXPHY_BITMASK_
{
	OFDM_PPDU_BIT = 0,
	OFDM_FALSE_BIT,
	OFDM_MPDU_OK_BIT,
	OFDM_MPDU_FAIL_BIT,
	CCK_PPDU_BIT,
	CCK_FALSE_BIT,
	CCK_MPDU_OK_BIT,
	CCK_MPDU_FAIL_BIT,
	HT_PPDU_BIT,
	HT_FALSE_BIT,
	HT_MPDU_BIT,
	HT_MPDU_OK_BIT,
	HT_MPDU_FAIL_BIT,
} RXPHY_BITMASK;

typedef enum _POWER_MODE_ {
	POWER_LOW = 0,
	POWER_NORMAL
}POWER_MODE;

#define GEN_MP_IOCTL_SUBCODE(code) _MP_IOCTL_ ## code ## _CMD_

enum RTL871X_MP_IOCTL_SUBCODE{
	GEN_MP_IOCTL_SUBCODE(MP_START),			/*0*/
	GEN_MP_IOCTL_SUBCODE(MP_STOP),			/*1*/
	GEN_MP_IOCTL_SUBCODE(READ_REG), 		/*2*/
	GEN_MP_IOCTL_SUBCODE(WRITE_REG),
	GEN_MP_IOCTL_SUBCODE(SET_CHANNEL), 		/*4*/
	GEN_MP_IOCTL_SUBCODE(SET_TXPOWER), 		/*5*/
	GEN_MP_IOCTL_SUBCODE(SET_DATARATE), 		/*6*/
	GEN_MP_IOCTL_SUBCODE(READ_BB_REG), 		/*7*/
	GEN_MP_IOCTL_SUBCODE(WRITE_BB_REG),
	GEN_MP_IOCTL_SUBCODE(READ_RF_REG), 		/*9*/
	GEN_MP_IOCTL_SUBCODE(WRITE_RF_REG),
	GEN_MP_IOCTL_SUBCODE(SET_RF_INTFS),
	GEN_MP_IOCTL_SUBCODE(IOCTL_XMIT_PACKET),	/*12*/
	GEN_MP_IOCTL_SUBCODE(PS_STATE),			/*13*/
	GEN_MP_IOCTL_SUBCODE(READ16_EEPROM),		/*14*/
	GEN_MP_IOCTL_SUBCODE(WRITE16_EEPROM),		/*15*/
	GEN_MP_IOCTL_SUBCODE(SET_PTM),			/*16*/
	GEN_MP_IOCTL_SUBCODE(READ_TSSI),		/*17*/
//	GEN_MP_IOCTL_SUBCODE(SET_DIG),			/*18*/
	GEN_MP_IOCTL_SUBCODE(CNTU_TX),			/*18*/
	GEN_MP_IOCTL_SUBCODE(SET_BANDWIDTH),		/*19*/
	GEN_MP_IOCTL_SUBCODE(SET_RX_PKT_TYPE),		/*20*/
	GEN_MP_IOCTL_SUBCODE(RESET_PHY_RX_PKT_CNT),	/*21*/
	GEN_MP_IOCTL_SUBCODE(GET_PHY_RX_PKT_RECV),	/*22*/
	GEN_MP_IOCTL_SUBCODE(GET_PHY_RX_PKT_ERROR),	/*23*/
	GEN_MP_IOCTL_SUBCODE(SET_POWER_DOWN),		/*24*/
	GEN_MP_IOCTL_SUBCODE(GET_THERMAL_METER),	/*25*/
	GEN_MP_IOCTL_SUBCODE(GET_POWER_MODE),		/*26*/
	GEN_MP_IOCTL_SUBCODE(EFUSE),			/*27*/
	GEN_MP_IOCTL_SUBCODE(EFUSE_MAP),		/*28*/
	GEN_MP_IOCTL_SUBCODE(GET_EFUSE_MAX_SIZE),	/*29*/
	GEN_MP_IOCTL_SUBCODE(GET_EFUSE_CURRENT_SIZE),	/*30*/
	GEN_MP_IOCTL_SUBCODE(SC_TX),			/*31*/
	GEN_MP_IOCTL_SUBCODE(CS_TX),			/*32*/
	GEN_MP_IOCTL_SUBCODE(ST_TX),			/*33*/
	GEN_MP_IOCTL_SUBCODE(SET_ANTENNA),		/*34*/
	MAX_MP_IOCTL_SUBCODE,
};

#define _2MAC_MODE_	0
#define _LOOPBOOK_MODE_	1

enum MP_MODE{
	MP_START_MODE,
	MP_STOP_MODE,
	MP_ERR_MODE
};

struct rwreg_param{
	unsigned int offset;
	unsigned int width;
	unsigned int value;
};
#if 0
struct bbreg_param{
	unsigned int offset;
	unsigned int bOFDM;
	unsigned int value;
};
#endif
struct r8712_bb_reg_param{
	unsigned int offset;
	unsigned int value;
};
struct r8712_rf_reg_param{
	unsigned int path;
	unsigned int offset;
	unsigned int value;
};
/*
struct rfchannel_param{
	unsigned int	ch;
	unsigned int	modem;
};
*/

struct txpower_param{
	unsigned int pwr_index;
};

struct datarate_param{
	unsigned int rate_index;
};

struct rfintfs_parm {
	unsigned int rfintfs;
};

struct mp_xmit_packet{
	unsigned int len;
	unsigned int mem[(MAX_MP_XMITBUF_SZ >> 2)];
};

struct psmode_param{
	unsigned int ps_mode;
	unsigned int smart_ps;
};

struct rf_intfs_param{
	unsigned int mode;
};

struct eeprom_rw_param{
	unsigned int offset;
	unsigned short value;
};

struct mp_status_param{
	unsigned int mode;
};

struct mp_ioctl_param{
	unsigned int subcode;
	unsigned int len;
	unsigned char data[0];
};

struct wpsctrl_adapter_ops
{
	int (*init)(void *pdata);
	int (*open)(void *pdata);
	int (*deint)(void *pdata);
	int (*close)(void *pdata);
	int (*iocontrol)(int skfd, unsigned int direction, unsigned int iocode, void *pInBuffer, unsigned int InBufferSize, void *pOutBuffer, unsigned int OutBufferSize);

	//
	int (*set_scan)(void *pdata);
	int (*get_scan_list)(void *pdata);
	int (*get_scan_results)(void *pdata, unsigned int index, void *pbuf);
};
//----------------------------------------------------------------------------------------

typedef struct _TXPOWERTABLE_{
	unsigned char CCK[4];
	unsigned char OFDM[4];
}TXPOWERTABLE;

typedef struct _TSSI_STRUCT_{
	unsigned char CCK[4];
	unsigned char OFDM[4];
}TSSISTRUCT;

typedef struct _EEPROM_INF_{
	unsigned char	ID[2];
	unsigned char	MACAddr[6];
	unsigned char	VID[2];
	unsigned char	PID[2];
	unsigned char	ChannPlan;
	unsigned char	EEVersion[2];
	TXPOWERTABLE	TxPower;
	TSSISTRUCT	TSSI;
	unsigned char	CAP[2];
	unsigned char	Conutry[3];
}EEPROMINF;


/* E-Fuse */

#define EFUSE_MAP_MAX_SIZE	128
#define EFUSE_MAX_SIZE		512

#define SDIO_MAC_ADDRESS_OFFSET	0x4F
#define USB_MAC_ADDRESS_OFFSET	0x12

typedef struct _EFUSE_ACCESS_STRUCT_ {
	unsigned short	start_addr;
	unsigned short	cnts;
	unsigned char	data[0];
}EFUSE_ACCESS_STRUCT, *PEFUSE_ACCESS_STRUCT;

/* end of E-Fuse */


typedef struct _tx_desc_8712 {
	// DWORD 1
	unsigned int	txpktsize:16;
	unsigned int	offset:8;
	unsigned int	frame_type:2;
	unsigned int	ls:1;
	unsigned int	fs:1;
	unsigned int	linip:1;
	unsigned int	amsdu:1;
	unsigned int	gf:1;
	unsigned int	own:1;
	// DWORD 2
	unsigned int	macid:5;
	unsigned int	moredata:1;
	unsigned int	morefrag:1;
	unsigned int	pifs:1;
	unsigned int	qsel:5;
	unsigned int	ack_policy:2;
	unsigned int	noacm:1;
	unsigned int	non_qos:1;
	unsigned int	key_id:2;
	unsigned int	oui:1;
	unsigned int	pkt_type:1;
	unsigned int	en_desc_id:1;
	unsigned int	sec_type:2;
	unsigned int	wds:1;//padding0
	unsigned int	htc:1;//padding1
	unsigned int	pkt_offset:5;//padding_len (hw)
	unsigned int	hwpc:1;
	// DWORD 3
	unsigned int	data_retry_lmt:6;
	unsigned int	rty_lmt_en:1;
	unsigned int	rsvd:1;
	unsigned int	tsfl:4;
	unsigned int	rts_rc:6;
	unsigned int	data_rc:6;
	unsigned int	rsvd_macid:5;
	unsigned int	agg_en:1;
	unsigned int	bk:1;
	unsigned int	own_mac:1;
	// DWORD 4
	unsigned int	next_head_page:8;
	unsigned int	tail_page:8;
	unsigned int	seq:12;
	unsigned int	frag:4;
	// DWORD 5
	unsigned int	rts_rate:6;
	unsigned int	dis_rts_fb:1;
	unsigned int	rts_rate_fb_lmt:4;
	unsigned int	cts_2_self:1;
	unsigned int	rts_en:1;
	unsigned int	ra_brsr_id:3;
	unsigned int	tx_ht:1;
	unsigned int	tx_short:1;//for data
	unsigned int	tx_bw:1;
	unsigned int	tx_sc:2;
	unsigned int	stbc:2;
	unsigned int	rd:1;
	unsigned int	rts_ht:1;
	unsigned int	rts_short:1;
	unsigned int	rts_bw:1;
	unsigned int	rts_sc:2;
	unsigned int	rts_stbc:2;
	unsigned int	use_rate:1;
	// DWORD 6
	unsigned int	packet_id:9;
	unsigned int	tx_rate:6;
	unsigned int	dis_fb:1;
	unsigned int	data_ratefb_lmt:5;
	unsigned int	tx_agc:11;
	// DWORD 7
	unsigned int	ip_chksum:16;
	unsigned int	tcp_chksum:16;
	// DWORD 8
	unsigned int	tx_buff_size:16;//pcie
	unsigned int	ip_hdr_offset:8;
	unsigned int	rsvd3:7;
	unsigned int	tcp_en:1;
/*
	// DWORD 9
	unsigned int	tx_buffer_address:32;	//pcie
	// DWORD 10
	unsigned int	next_tx_desc_address:32;	//pcie
*/
}tx_desc_8712;

struct wpsctrl
{
	int skfd;
	int skpkt;//use for packet xmit
	char ifrn_name[IFNAMSIZ];	/* if name, e.g. "wlan0" */
	unsigned char enter_mp_test_mode;

	unsigned char chan_index;
//	unsigned char modulation;
	unsigned int power_index;
	unsigned int rate_index;
	unsigned int bandwidth;
	unsigned int antenna;

	unsigned char dig_mode;
	unsigned char cca_leve;
	unsigned char mp_test_mode;

	unsigned int bstarttx;
	unsigned int bstartrx;
	unsigned int tx_pkt_cnts;
	unsigned int rx_pkt_ok_cnts;
	unsigned int rx_pkt_err_cnts;

	unsigned int record_rx_pkt_ok_cnts;
	unsigned int record_rx_pkt_err_cnts;

	unsigned int payload_len;
	unsigned int payload_type;
	unsigned char da[ETH_ALEN];
	unsigned int bGENTSSI;
//	TSSISTRUCT tssi_table;

	EEPROMINF eeprom_info;
	unsigned char efuse_map[EFUSE_MAP_MAX_SIZE];

	unsigned int method;
	unsigned int cnt;
	unsigned int status;
	unsigned int cardinfo;
	unsigned int psk;
	unsigned int legacy_wep;

	unsigned int connect_status;   //0: disconnected; 1:connected
//	unsigned int extraie[MAX_PADDEDIE>>2];
	unsigned char macaddr[ETH_ALEN];
//	unsigned int iocontrolbuf[MAX_IOCONTROLBUFSZ >>2]; //we need the buf to be 4 bytes aligned

	unsigned int numofmasters;

	tx_desc_8712 TxDesc;

	pthread_mutex_t iocontrol_mutex;

	struct sockaddr_ll socket_address;

	struct wpsctrl_adapter_ops *pwpsc_ops;
	void *priv_ptr;
	unsigned int priv_sz;
	unsigned char priv_data[1];
};

#define GET_TX_ANTENNA(ant) ((ant&0xFFFF0000)>>16)
#define GET_RX_ANTENNA(ant) (ant&0xFFFF)
#define SET_TX_ANTENNA(ant, tx) ant=(ant&0x0000FFFF)|((tx&0xFFFF)<<16)
#define SET_RX_ANTENNA(ant, rx) ant=(ant&0xFFFF0000)|(rx&0xFFFF)

//----------------------------------------------------------------------------------------
//extern struct mp_ioctl_handler mp_ioctl_hdl[];
//----------------------------------------------------------------------------------------
extern int	is_dev_up(struct wpsctrl *pwpsc);

extern int	r8711priv_ioctl(int skfd, unsigned int direction, unsigned int iocode, void *pInBuffer, unsigned int InBufferSize, void *pOutBuffer, unsigned int OutBufferSize);

extern int	r871x_mp_init(struct wpsctrl *pwpsc);
extern int	r871x_set_mp_start(struct wpsctrl *pwpsc);
extern int	r871x_set_mp_stop(struct wpsctrl *pwpsc);

extern int	r871x_set_channel(struct wpsctrl *pwpsc, unsigned char ch);
extern int	r871x_set_txpower(struct wpsctrl *pwpsc, unsigned int power_index);
extern int	r871x_set_datarate(struct wpsctrl *pwpsc, unsigned int rate_index);
extern int	r871x_set_bandwidth(struct wpsctrl *pwpsc, unsigned int bandwidth);
extern int	r871x_set_powersave_mode(struct wpsctrl *pwpsc, unsigned int ps_mode,unsigned int smart_ps);
extern int	r871x_set_antenna(struct wpsctrl *pwpsc, unsigned int antenna);

extern int	r871x_read_reg(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, void *pbuf);
extern int	r871x_write_reg(struct wpsctrl *pwpsc, unsigned int addr,unsigned int cnt,unsigned int value);

extern int	r871x_read_rfreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int page,unsigned int *value);
extern int	r871x_write_rfreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int page,unsigned int value);

extern int	r871x_read_bbreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int *value);
extern int	r871x_write_bbreg(struct wpsctrl *pwpsc, unsigned int offset, unsigned int value);

extern int	r871x_set_continuous_tx(struct wpsctrl *pwpsc, int bstart);
extern int	r871x_set_single_carrier_tx( struct wpsctrl *pwpsc, int bstart);
extern int	r871x_set_carrier_suppression_tx(struct wpsctrl *pwpsc, int bstart);
extern void	r871x_set_pkt_xmit(struct wpsctrl *pwpsc, int bstart);
extern int	r871x_set_pkt_recv_type(struct wpsctrl *pwpsc, int type);
extern void	r871x_get_pkt_recv(struct wpsctrl *pwpsc,int bstart);

extern int	r871x_read_eeprom_byte(struct wpsctrl *pwpsc, unsigned char offset, unsigned char *value);
extern int	r871x_write_eeprom_byte(struct wpsctrl *pwpsc, unsigned char offset, unsigned char value);

extern int	r871x_read_efuse_map(struct wpsctrl *pwpsc, unsigned char *map);
extern int	r871x_write_efuse_map(struct wpsctrl *pwpsc, unsigned char *map);

extern int	r871x_read_efuse_raw(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, unsigned char *data);
extern int	r871x_write_efuse_raw(struct wpsctrl *pwpsc, unsigned int addr, unsigned int cnt, unsigned char *data);

extern int	r871x_read_txpower_from_eeprom(struct wpsctrl *pwpsc, int bOFDM);
extern int	r871x_write_txpower_into_eeprom(struct wpsctrl *pwpsc, int bOFDM);

extern int	r871x_set_ptm(struct wpsctrl *pwpsc, unsigned char mode);

extern int	r871x_get_target_tssi(struct wpsctrl *pwpsc, unsigned int *tarteg_tssi);
extern int	r871x_read_tssi_from_eeprom(struct wpsctrl *pwpsc, int bOFDM);
extern int	r871x_write_tssi_into_eeprom(struct wpsctrl *pwpsc, int bOFDM);
extern int	r871x_gen_tssi_procedure(struct wpsctrl *pwpsc);

extern int	r871x_read_thermal_meter(struct wpsctrl *pwpsc, unsigned int *thermal_meter);

extern int	r871x_set_dig(struct wpsctrl *pwpsc, unsigned char mode);
extern int	r871x_set_cca_level(struct wpsctrl *pwpsc, unsigned char level);
extern void	r871x_power_down(struct wpsctrl *pwpsc,int bstart);
//----------------------------------------------------------------------------------------

#if 0
unsigned char DFT_SDIO_MAP_V04[128] = {
0x12,0x87,0x00,0x00,0x16,0xA9,0x00,0x00,0xFF,0x02,0x23,0x00,0x00,0xFC,0x20,0x04,
0x4C,0x02,0x12,0x87,0x21,0x02,0x0C,0x00,0x22,0x04,0x00,0x08,0x00,0x32,0xFF,0x21,
0x02,0x0C,0x00,0x22,0x2A,0x01,0x01,0x00,0x12,0x87,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x02,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEB,0x00,0x6E,0x01,0x00,0x00,0x00,0x00,0x00,
0xE0,0x4C,0x87,0x12,0x00,0x28,0x28,0x28,0xFF,0xFF,0xFF,0x30,0x30,0x30,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x12,0xFF,0xFF,0x09,0x0D,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x00,0x00,0xFF,0xFF
};
#endif
#endif

