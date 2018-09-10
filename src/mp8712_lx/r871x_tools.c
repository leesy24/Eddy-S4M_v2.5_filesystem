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
#include <unistd.h>
#include <netdb.h>              /* gethostbyname, getnetbyname */
#include <unistd.h>
#include <sys/socket.h>         /* for "struct sockaddr" et al  */
#include <sys/time.h>           /* struct timeval */

#include <linux/version.h>
//#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
//#include <net/if.h>
//#endif
#include <net/ethernet.h>       /* struct ether_addr */
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include "r871x_tools.h"
#include <pthread.h>
//----------------------------------------------------------------------------------------
#if 0
#define	_CON_TX_FUNC_TEST_  	0
#define	_PKT_TX_FUNC_TEST_	0
#define	_POER_SAVE_FUNC_TEST_	1
#endif

char interface[IFNAMSIZ];

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

unsigned char DFT_USB_MAP_V04[128];

//----------------------------------------------------------------------------------------
const char MP871x_UTILITY_TITLE[] =
{"\n" \
 "\t  Realtek RTL8712 Wireless Adapter Utility\n" \
 "\t  Copyright 2010 Realtek Semiconductor Corp.\n" \
 "\t\tVersion: 2.3.1\n"};
const char MP871x_EMI_TITLE[] = {"\n\t  EMI Functions Menu\n"};
const char MP871x_EFUSE_TITLE[] = {"\n\t  E-Fuse Functions Menu\n"};
const char MP871x_REGIO_TITLE[] = {"\n\t  Register Read/Write Menu\n"};

//----------------------------------------------------------------------------------------
const char paramerrstr[]	= {"parameters error !!\n"};

/* EMI */
const char emistr[] 		= {"emi\t\tEMI Functions"};
const char setchanstr[] 	= {"setchan chanid[1~14]"};
const char settxpowerstr[]	= {"settxpower powerindex[0x00~0x3F]"};
const char setratestr[]		= {"setrate rateindex[0~3:CCK 4~11:OFDM 12~27:MCS0~15]"};
const char setbwstr[]		= {"setbw bandwidth[0:20MHz 1:40MHz]"};

const char continuoustxstr[] 	= {"continuoustx mode[1/0(start/stop)]"};
const char singletxstr[] 	= {"singletx mode[1/0(start/stop)]"};
const char carriersuppressiontxstr[] 	= {"carriertx mode[1/0(start/stop)]"};
const char pkttxstr[] 		= {"pkttx mode[1/0(start/stop)] cnts len type[0~3] da"};
const char pktrxstr[] 		= {"pktrx mode[1/0(start/stop)] antenna[1:A 2:B 3:AB]"};

/* deprecated */
const char setprmstr[]		= {"setpower mode[0~4] smart_ps[0/1]"};

const char setdigstr[]		= {"setdig mode[0/1(disable/enable)]"};
const char setccastr[]		= {"setcca level[1~4]"};

const char setptmstr[]		= {"setptm mode[0~3]"};
const char gettssistr[]		= {"gettssi"};
const char autogentssistr[]	= {"autogentssi"};
const char readtssistr[]	= {"readtssi bOFDM[0:CCK 1:OFDM]"};
const char writetssistr[]	= {"writetssi bOFDM[0:CCK 1:OFDM] ch1-tssi ch6-tssi ch11-tssi ch14-tssi"};

const char readtxpowerstr[]	= {"readtxpower bOFDM[0:CCK 1:OFDM]"};
const char writetxpowerstr[]	= {"writetxpower bOFDM[0:CCK 1:OFDM] ch1-tp ch6-tp ch11-tp ch14-tp"};

/* E-Fuse */
const char reepromstr[]		= {"reeprom offset[0x00~0xFF]"};
const char weepromstr[]		= {"weeprom offset[0x00~0xFF] value[hex]"};

const char efusestr[]		= {"efuse\t\tE-Fuse Functions"};
const char exitefusestr[]	= {"exit\t\t:Back to main menu"};

const char refrawstr[]		= {"refraw addr[hex] bytes[1-512]"};
const char wefrawstr[]		= {"wefraw addr[hex] bytes[1-8] value1[hex] value2 ..."};
const char refmapstr[]		= {"refmap\t:read EFuse Map from driver"};
const char wefmapstr[]		= {"wefmap\t:write EFuse Map to driver"};
const char smapstr[]		= {"smap\t\tshow EFuse Map"};
const char rmapstr[]		= {"rmap addr[hex] bytes[1-128] (LOCAL)"};
const char wmapstr[]		= {"wmap addr[hex] bytes[1-8] value1[hex] value2 ... (LOCAL)"};
const char chmacstr[]		= {"chmac type[1/2(SDIO/USB)] MAC_Address[xx:xx:xx:xx:xx:xx]"};
const char rthermalstr[]	= {"rthermal\t:read thermal meter from hardware"};

/* Register IO */
const char regiostr[] 		= {"reg\t\tRegister Read/Write"};
const char rregstr[] 		= {"rreg addr[hex] bytes[1/2/4]"};
const char wregstr[] 		= {"wreg addr[hex] bytes[1/2/4] val[hex]"};
const char rrfregstr[] 		= {"rrfreg path[0~1] offset[0x00~0xFF]"};
const char wrfregstr[] 		= {"wrfreg path[0~1] offset[0x00~0xFF] val[0x0~0xFFFFFFFF]"};
const char rbbregstr[]		= {"rbbreg offset[0x800~0xFFF]"};
const char wbbregstr[]		= {"wbbreg offset[0x800~0xFFF] val[0x0~0xFFFFFFFF]"};

/* Other */
#ifdef CONFIG_MP871X_DBG
const char dbg_page_dump[]	= {"dbg_page_dump"};
#endif
const char power_down_str[] 	= {"power_down_test mode[1/0(up/down)]"};

PFUNCTION_MENU menu;

//----------------------------------------------------------------------------------------
CMD_STRUCTURE mp871x_cmds[] =
{
//	command		param	handler				description
	{"help",	0,	cmd_help_hdl,			NULL},
	{"emi",		0,	emi_hdl,			emistr},
	{"efuse",	0,	efuse_hdl,			efusestr},
	{"reg",		0,	regio_hdl,			regiostr},

#ifdef CONFIG_MP871X_DBG
	{"dbg_page_dump", 0,	page_dump_dbg_hdl,		dbg_page_dump},
#endif
	{"exit",	0,	exit_hdl,			NULL}
};

CMD_STRUCTURE emi_cmds[] = {
//	command		param	handler				description
	{"help",	0,	cmd_help_hdl,			NULL},
//	{"mpstart",	0,	mpstart_hdl,			NULL},
	{"setchan",	1,	set_channel_hdl,		setchanstr},
	{"settxpower",	1,	set_output_power_hdl,		settxpowerstr},
	{"setrate",	1,	set_data_rate_hdl,		setratestr},
	{"setbw",	1,	set_bandwidth_hdl,		setbwstr},
//	{"setpower",	2,	set_power_save_mode_hdl,	setprmstr},
//	{"setdig",	1,	set_dynamic_initial_gain_state_hdl,	setdigstr},
//	{"setcca",	1,	set_cca_level_hdl,		setccastr},

//	{"reeprom",	1, 	read_eeprom_byte_hdl,		reepromstr},
//	{"weeprom",	2, 	write_eeprom_byte_hdl,		weepromstr},

//	{"setptm",	1,	set_power_tracking_mode_hdl,	setptmstr},
//	{"gettssi",	0,	get_target_tssi_hdl,		gettssistr},
//	{"autogentssi",	0,  	auto_gen_tssi_hdl,		autogentssistr},
//	{"readtssi",	1,	read_tssi_hdl,			readtssistr},
//	{"writetssi",	5,	write_tssi_hdl,			writetssistr},

//	{"readtxpower",	1,	read_txpower_hdl,		readtxpowerstr},
//	{"writetxpower",5,	write_txpower_hdl,		writetxpowerstr},

	{"continuoustx",1,	set_continuous_tx_hdl,		continuoustxstr},
	{"singletx",	1,	set_single_carrier_tx_hdl,	singletxstr},
	{"carriertx",	1,	set_carrier_suppression_tx_hdl,	carriersuppressiontxstr},
	{"pkttx",	1,	set_pkt_tx_hdl,			pkttxstr},
	{"pktrx",	1,	get_pkt_phy_rx_hdl,		pktrxstr},
	{"exit",	0,	exit_hdl,			NULL}
};

CMD_STRUCTURE efuse_cmds[] = {
//	command		param	handler				description
	{"help",	0,	cmd_help_hdl,			NULL},
	{"refraw",	2,	read_efuse_raw_hdl,		refrawstr},
	{"wefraw",	3,	write_efuse_raw_hdl,		wefrawstr},
	{"refmap",	0,	read_efuse_map_hdl,		refmapstr},
	{"wefmap",	0,	write_efuse_map_hdl,		wefmapstr},
	{"smap",	0,	show_efuse_map_hdl,		smapstr},
	{"rmap",	2,	read_efuse_hdl,			rmapstr},
	{"wmap",	3,	write_efuse_hdl,		wmapstr},
	{"chmac",	2,	change_mac_address_hdl,		chmacstr},
	{"rthermal",	0,	read_thermal_meter_hdl,		rthermalstr},
	{"exit",	0,	exit_hdl,			exitefusestr}
};

CMD_STRUCTURE regio_cmds[] = {
//	command		param	handler				description
	{"help",	0,	cmd_help_hdl,			NULL},
	{"rreg",	2,	read_reg_hdl,			rregstr},
	{"wreg",	3,	write_reg_hdl,			wregstr},
	{"rrfreg",	2,	read_rfreg_hdl,			rrfregstr},
	{"wrfreg",	3,	write_rfreg_hdl,		wrfregstr},
	{"rbbreg",	1,	read_bbreg_hdl,			rbbregstr},
	{"wbbreg",	2,	write_bbreg_hdl,		wbbregstr},
	{"power_down_test",1,	power_down_hdl,			power_down_str},
	{"exit",	0,	exit_hdl,			NULL}
};
//----------------------------------------------------------------------------------------
static int is_hex_str(char *cstr)
{
	int i;
	for (i=0; i<strlen(cstr); i++) {
		if( !((cstr[i] >= '0' && cstr[i] <= '9') || ((cstr[i]  | 0x20) >= 'a' && (cstr[i]  | 0x20) <= 'f')) )
			return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------------
static int is_dec_str(char *cstr)
{
	int i;
	for (i=0; i<strlen(cstr); i++) {
		if( !(cstr[i] >= '0' && cstr[i] <= '9') )
			return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------
unsigned char Asc2Hex(char  ASCCode)
{
	if ((ASCCode <= '9') && (ASCCode >= '0')) {
		return  (ASCCode-'0');
	} else {
		if ((ASCCode <= 'f') && (ASCCode >= 'a')) {
			return  (ASCCode-'a'+10);
		} else {
			if ( (ASCCode <= 'F') && (ASCCode >= 'A') ) {
				return  (ASCCode-'A'+10);
			} else {
				return  0xFF;
			}
		}
	}
}
//----------------------------------------------------------------------------------------
void cmdparser(char *cmdstr, INPUT_CMD *pinput)
{
	int n, idx = 0;
	char delimiters[] = " \t\n";
	char *p;

	if (strlen(cmdstr) == 0) return;

	p = strtok(cmdstr, delimiters);

	if (p != NULL)
	{
		n = strlen(p) + 1;
		if (n > MAX_CMD_LEN) return;
		strcpy(pinput->cmd, p);

		do {
			p = strtok(NULL, delimiters);
			if (p == NULL) break;

			n = strlen(p) + 1;
			pinput->param[idx] = (char*)malloc(n);
			strcpy(pinput->param[idx], p);

			if (++idx >= MAX_PARAMETER_NUM) break;

		} while (1);
	}
	pinput->param_cnts = idx;
}

void cmd_help_hdl(struct wpsctrl *pwpsc, char **param)
{
	int i;
	CMD_STRUCTURE *cmds;

	cmds = menu->cmds;

	printf("\t===========================================\n");
	for (i = 0; i < menu->cnts; i++)
	{
		printf("\t  ");	//indent
		if (cmds[i].description != NULL)
			printf("%s", cmds[i].description);
		else
			printf("%s", cmds[i].command);
		printf("\n");
	}
	printf("\t===========================================\n\n");
}
//----------------------------------------------------------------------------------------
void exit_hdl(struct wpsctrl *pwpsc, char **param)
{
	PFUNCTION_MENU leavemenu;
	CMD_STRUCTURE *cmds;

	cmds = menu->cmds;

	if (menu->prev == NULL) {
		if (pwpsc->enter_mp_test_mode) {
			r871x_set_mp_stop(pwpsc);
		}
		exit(0);
	}

	leavemenu = menu;
	menu = menu->prev;
	free(leavemenu);
}
//----------------------------------------------------------------------------------------
void emi_hdl(struct wpsctrl *pwpsc, char **param)
{
	PFUNCTION_MENU emimenu;

	if (!is_dev_up(pwpsc)) {
		printf("!Attention: device not up yet,\nuse \"ifconfig\" to up interface first!\n");
		return;
	}

	emimenu = (PFUNCTION_MENU)malloc(sizeof(FUNCTION_MENU));
	emimenu->prev = menu;
	emimenu->cmds = emi_cmds;
	emimenu->cnts = CMDCNTS(emi_cmds);
	sprintf(emimenu->prompt, "%s/%s", menu->prompt, "emi");

	menu = emimenu;

	printf("%s", MP871x_EMI_TITLE);
	if (!pwpsc->enter_mp_test_mode)
		printf("\n!!ATTENTION!! MP Tool at NORMAL mode now, EMI test may fail!\n" \
			"If want to do EMI test, please don't start MP tool at normal mode.\n\n");
	cmd_help_hdl(NULL, NULL);
}
//----------------------------------------------------------------------------------------
void set_channel_hdl(struct wpsctrl *pwpsc, char **param)
{
	int chan = 0;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, setchanstr);
		return;
	}

	sscanf(param[0], "%d", &chan);
	if ((chan < 1) ||(chan > 14)) {	// ch = 1~14
		printf("%s  %s\n", paramerrstr, setchanstr);
		return;
	}
	pwpsc->chan_index = (unsigned char)chan;

	printf("$-set_channel_hdl - chan:%d", chan);
	if (r871x_set_channel(pwpsc, pwpsc->chan_index))
		printf(" fail!!");
	printf("\n\n");
}
//----------------------------------------------------------------------------------------
void set_output_power_hdl(struct wpsctrl *pwpsc, char **param)
{
	int power_index = 0;
	char *tmpparam1 = NULL;

	if ((strlen(param[0]) > 2) && (((param[0][1] | 0x20) == 'x')))
		tmpparam1 = param[0] + 2;
	else
		tmpparam1 = param[0];

	if (!is_hex_str(tmpparam1)) {
		printf("%s  %s\n", paramerrstr, settxpowerstr);
		return;
	}

	sscanf(tmpparam1, "%x", &power_index);
	if ((power_index < 0x0) || (power_index > 0x3F)) {
		printf("%s  %s\n",paramerrstr,settxpowerstr);
		return;
	}

	pwpsc->power_index = power_index; ;//tx_power index value
	printf("$-set_output_power_hdl - power index:0x%02x", pwpsc->power_index);
	if (r871x_set_txpower(pwpsc, pwpsc->power_index))
		printf(" fail!!");
	printf("\n\n");
}

//----------------------------------------------------------------------------------------
void set_data_rate_hdl(struct wpsctrl *pwpsc, char **param)
{
	int data_rate = 0;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, setratestr);
		return;
	}

	sscanf(param[0], "%d", &data_rate);
	if ((data_rate < 0) || (data_rate >= MAX_DATA_RATE)) {
		printf("%s  %s\n", paramerrstr, setratestr);
		return;
	}

	pwpsc->rate_index = data_rate;
	r871x_set_datarate(pwpsc, pwpsc->rate_index);//datarate index

	printf("$-set_data_rate_hdl - rate:%d\n\n", data_rate);
}
//----------------------------------------------------------------------------------------
void set_bandwidth_hdl(struct wpsctrl *pwpsc, char **param)
{
	int bandwidth = 0;
	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, setbwstr);
		return;
	}

	sscanf(param[0], "%d", &bandwidth);
	if ((bandwidth < 0) ||(bandwidth > 1)) {
		printf("%s  %s\n", paramerrstr, setbwstr);
		return;
	}

	pwpsc->bandwidth = bandwidth;
	r871x_set_bandwidth(pwpsc, pwpsc->bandwidth);

	printf("$-set_bandwidth_hdl - bandwidth:%d\n\n", bandwidth);
}
//----------------------------------------------------------------------------------------
void set_dynamic_initial_gain_state_hdl(struct wpsctrl *pwpsc, char **param)
{
	int dig_mode = 0;
	if (!is_dec_str(param[0])) {
		printf("%s  %s\n",paramerrstr,setdigstr);
		return;
	}

	sscanf(param[0], "%d", &dig_mode);
	if ((dig_mode<0) || (dig_mode>1)) {
		printf("%s  %s\n",paramerrstr,setdigstr);
		return;
	}

	pwpsc->dig_mode = dig_mode;
	r871x_set_dig(pwpsc, pwpsc->dig_mode );

	printf("$-set_dynamic_initial_gain_state_hdl - type:%d\n\n", dig_mode);
}
//----------------------------------------------------------------------------------------
void set_cca_level_hdl(struct wpsctrl *pwpsc, char **param)
{
	int cca_level = 0;
	if (!is_dec_str(param[0])){
		printf("%s  %s\n",paramerrstr,setccastr);
		return;
	}

	sscanf(param[0], "%d", &cca_level);

	if ((cca_level<1) || (cca_level>4)) {
		printf("%s  %s\n",paramerrstr,setccastr);
		return;
	}

	pwpsc->cca_leve = cca_level;
	r871x_set_cca_level(pwpsc, pwpsc->cca_leve);

	printf("$-set_cca_level_hdl - level:%d\n\n", cca_level);
}
//----------------------------------------------------------------------------------------
void set_power_save_mode_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int ps_mode = 0, smart_ps = 0;

	if ((!is_dec_str(param[0])) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, setprmstr);
		return;
	}
	sscanf(param[0], "%d", &ps_mode);
	sscanf(param[1], "%d", &smart_ps);

	if ((ps_mode<PWR_CAM) || (ps_mode>PWR_VOIP) || (smart_ps<SMART_PW_DISABLE) || (smart_ps>SMART_PW_ENABLE))
	{
		printf("%s  %s\n", paramerrstr, setprmstr);
		return;
	}

	r871x_set_powersave_mode(pwpsc, ps_mode, smart_ps);

	printf("$-set_power_save_mode_hdl -  ps:%d sps:%d\n\n", ps_mode, smart_ps);
}
//----------------------------------------------------------------------------------------
#define MAX_HEXSTR_BUF	20   //EX: "0x1d200050\0"

void regio_hdl(struct wpsctrl *pwpsc, char **param)
{
	PFUNCTION_MENU regiomenu;

	regiomenu = (PFUNCTION_MENU)malloc(sizeof(FUNCTION_MENU));
	regiomenu->prev = menu;
	regiomenu->cmds = regio_cmds;
	regiomenu->cnts = CMDCNTS(regio_cmds);
	sprintf(regiomenu->prompt, "%s/%s", menu->prompt, "reg");

	menu = regiomenu;

	printf("%s", MP871x_REGIO_TITLE);
	cmd_help_hdl(NULL, NULL);
}

void read_reg_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0, value = 0;
	char *tmpparam1 = NULL;

	if ((strlen(param[0])>8) && (((param[0][1]|0x20)=='x')))
		tmpparam1 = param[0] + 2;
	else
		tmpparam1 = param[0];

	if ((!is_hex_str(tmpparam1)) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, rregstr);
		return;
	}

	sscanf(tmpparam1, "%x", &addr);
	if (addr < 0x10000)
		addr |= 0x10250000;

	sscanf(param[1], "%d", &byte_cnts);
	if ((byte_cnts!=1) && (byte_cnts!=2) && (byte_cnts!=4)) {
		printf("%s  %s\n", paramerrstr, rregstr);
		return;
	}

	r871x_read_reg(pwpsc, addr, byte_cnts, &value);

	printf("$-read_reg_hdl -  addr:0x%08x bytes:%d ", addr,byte_cnts);
	if (byte_cnts == 1)
		printf("value:0x%02x", (unsigned char)(value&0xFF));
	else if(byte_cnts==2)
		printf("value:0x%04x", (unsigned short)(value&0xFFFF));
	else
		printf("value:0x%08x", value);
	printf("\n\n");
}
//----------------------------------------------------------------------------------------
void write_reg_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0, value = 0, rvalue = 0;
 	char *param1 = NULL, *param3 = NULL;

	if ((strlen(param[0]) > 8) && ((param[0][1] | 0x20) == 'x'))
		param1 = param[0] + 2;
	else
		param1 = param[0];

	if ((param[2][1] | 0x20) == 'x')
		param3 = param[2] + 2;
	else
		param3 = param[2];

	if ((!is_hex_str(param1)) || (!is_dec_str(param[1])) || (!is_hex_str(param3))) {
		printf("%s  %s\n", paramerrstr, wregstr);
		return;
	}

	sscanf(param1, "%x", &addr);
	if (addr < 0x10000)
		addr |= 0x10250000;
	sscanf(param[1], "%d", &byte_cnts);
	sscanf(param3, "%x", &value);

	if ((byte_cnts != 1) && (byte_cnts != 2) && (byte_cnts != 4)) {
		printf("%s  %s\n", paramerrstr, wregstr);
		return;
	}

	r871x_write_reg(pwpsc, addr, byte_cnts, value);

        r871x_read_reg(pwpsc, addr, byte_cnts, &rvalue);

	if (value == rvalue)
		printf("$-write_reg_hdl - addr:0x%08x bytes:%d value:0x%x success\n\n", addr, byte_cnts, value);
	else
		printf("$-write_reg_hdl - addr:0x%08x bytes:%d wv:0x%x rv:0x%x fail\n\n", addr, byte_cnts, value, rvalue);
}
//----------------------------------------------------------------------------------------
void read_rfreg_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int path = 0;
	unsigned int offset = 0;
	unsigned int value = 0;

	char *param2;

	if ((strlen(param[1])>2) && ((param[1][1] | 0x20) == 'x'))
		param2 = param[1] + 2;
	else
		param2 = param[1];

	if ((!is_dec_str(param[0])) || (!is_hex_str(param2))) {
		printf("%s  %s\n", paramerrstr, rrfregstr);
		return;
	}

	sscanf(param[0], "%d", &path);
	sscanf(param2, "%x", &offset);

	if ((offset<0x00) || (offset>0xFF) || (path<0x00) || (path>0x01))
	{
		printf("%s  %s\n", paramerrstr, rrfregstr);
		return;
	}

	r871x_read_rfreg(pwpsc, path, offset, &value);

	printf("$-read_rfreg_hdl - path:%x offset:0x%02x value:0x%08x\n\n", path, offset, value);
}
//----------------------------------------------------------------------------------------
void write_rfreg_hdl(struct wpsctrl *pwpsc, char **param )
{
	unsigned int path = 0;
	unsigned int offset = 0;
	unsigned int value = 0;
	unsigned int rvalue = 0;

	char *param2, *param3;

	if ((strlen(param[1]) > 2) && ((param[1][1] | 0x20) == 'x'))
		param2 = param[1] + 2;
	else
		param2 = param[1];

	if ((strlen(param[2]) > 2) && ((param[2][1] | 0x20) == 'x'))
		param3 = param[2] + 2;
	else
		param3 = param[2];

	if ((!is_dec_str(param[0])) || (!is_hex_str(param2)) || (!is_hex_str(param3))) {
		printf("%s  %s\n", paramerrstr, wrfregstr);
		return;
	}

	sscanf(param[0], "%d", &path);
	sscanf(param2, "%x", &offset);
	sscanf(param3, "%x", &value);

	if ((offset < 0x00) || (offset > 0xFF) || (path < 0x00) ||(path > 0x01)) {
		printf("%s  %s\n", paramerrstr, wrfregstr);
		return;
	}

	r871x_write_rfreg(pwpsc, path, offset, value);

	r871x_read_rfreg(pwpsc, path, offset, &rvalue);

	if (rvalue == value)
		printf("$-write_rfreg_hdl -  path:%x offset:0x%02x value:0x%08x success\n\n", path, offset, value);
	else
		printf("$-write_rfreg_hdl -  path:%x offset:0x%02x wv:0x%08x rv:0x%08x fail\n\n", path, offset, value, rvalue );
}

//----------------------------------------------------------------------------------------
void read_bbreg_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int offset = 0;
	unsigned int value = 0;
	char *tmpparam = NULL;

	if ((strlen(param[0]) > 2) && ((param[0][1] | 0x20)=='x'))
		tmpparam = param[0] + 2;
	else
		tmpparam = param[0];
	if (!is_hex_str(tmpparam)) {
		printf("%s  %s\n", paramerrstr, rbbregstr);
		return;
	}

	sscanf(tmpparam, "%x", &offset);
	if ((offset < 0x800) || (offset > 0xFFF)) {
		printf("%s  %s\n", paramerrstr, rbbregstr);
		return;
	}

	r871x_read_bbreg(pwpsc, offset, &value);

	printf("$-read_bbreg_hdl - offset:0x%x value:0x%08x\n\n", offset, value);
}

//----------------------------------------------------------------------------------------
void write_bbreg_hdl(struct wpsctrl *pwpsc, char **param )
{
	unsigned int offset=0;
	unsigned int value = 0;
	unsigned int rvalue = 0;

	char *param1 = NULL, *param2 = NULL;

	if((strlen(param[0])>2) && ((param[0][1]|0x20)=='x'))
		param1 = param[0] + 2;
	else
		param1 = param[0];

	if((strlen(param[1])>2) && ((param[1][1]|0x20)=='x'))
		param2 = param[1] + 2;
	else
		param2 = param[1];

	if ((!is_hex_str(param1)) || (!is_hex_str(param2))) {
		printf("%s  %s\n", paramerrstr, wbbregstr);
		return;
	}

	sscanf(param1, "%x", &offset);
	sscanf(param2, "%x", &value);
	if ((offset < 0x800) || (offset > 0xFFF)) {
		printf("%s  %s\n", paramerrstr, rbbregstr);
		return;
	}

	r871x_write_bbreg(pwpsc, offset, value);

	r871x_read_bbreg(pwpsc, offset, &rvalue);

	if (rvalue == value)
		printf("$-write_bbreg_hdl -  offset:0x%x value:0x%08x success\n\n", offset, rvalue);
	else
		printf("$-write_bbreg_hdl -  offset:0x%x wv:0x%08x rv:0x%08x fail\n\n", offset, value, rvalue );
}
//----------------------------------------------------------------------------------------
void set_continuous_tx_hdl(struct wpsctrl *pwpsc, char **param)
{
	int mode = 0;
	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, continuoustxstr);
		return;
	}

	sscanf(param[0], "%d", &mode);
	if ((mode < 0) ||(mode > 1)) {
		printf("%s  %s\n", paramerrstr, continuoustxstr);
		return;
	}

	pwpsc->mp_test_mode = CONTINUOUS_TX;

	printf("$-set_continuous_tx_hdl - mode:%d\n", mode);
	if (r871x_set_continuous_tx(pwpsc, mode))
		printf("FAIL!!\n");
	else
		r871x_set_pkt_xmit(pwpsc, mode);

	printf("\n");
}
//----------------------------------------------------------------------------------------
void set_single_carrier_tx_hdl(struct wpsctrl *pwpsc, char **param)
{
	int mode = 0;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, singletxstr);
		return;
	}

	sscanf(param[0], "%d", &mode);
	if ((mode<0) || (mode>1)) {
		printf("%s  %s\n", paramerrstr, singletxstr);
		return;
	}

	pwpsc->mp_test_mode = SINGLE_CARRIER_TX;

	printf("$-set_single_carrier_tx_hdl - mode:%d\n", mode);
	if (r871x_set_single_carrier_tx(pwpsc, mode))
		printf("FAIL!!\n");
	else
		r871x_set_pkt_xmit(pwpsc, mode);

	printf("\n");
}
//----------------------------------------------------------------------------------------
void set_carrier_suppression_tx_hdl(struct wpsctrl *pwpsc, char **param)
{
	int mode = 0;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, carriersuppressiontxstr);
		return;
	}

	sscanf(param[0], "%d", &mode);
	if ((mode<0) || (mode>1)) {
		printf("%s  %s\n", paramerrstr, carriersuppressiontxstr);
		return;
	}

	pwpsc->mp_test_mode = CARRIER_SUPPRISSION_TX;

	printf("$-set_carrier_suppression_tx_hdl - mode:%d\n", mode);
	if (r871x_set_carrier_suppression_tx(pwpsc, mode))
		printf("FAIL!!\n");
	else
		r871x_set_pkt_xmit(pwpsc, mode);

	printf("\n");
}
//----------------------------------------------------------------------------------------
static const unsigned char payloaddata[]={0x00,0x5a,0xa5,0xff};
void set_pkt_tx_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int bstarttx = 0;
	unsigned int pkt_cnts = 0 ;
	unsigned int payload_len = 0;//max:2018 2048-14-16
	unsigned int payload_type = 0;//0 1 2 3

	char *param5 = NULL;
	int i;

	// paramer 1 : bstarttx
	if (!is_dec_str(param[0])) goto _ERROR_EXIT_FUNC_;
	sscanf(param[0], "%d", &bstarttx);
	if((bstarttx<0) || (bstarttx>1)) goto _ERROR_EXIT_FUNC_;
	pwpsc->bstarttx = bstarttx;
	if ((bstarttx==0) || (param[1]==NULL))
		goto _EXECUTE_FUNC_;

	// paramer 2 : pkt cnts
	if (!is_dec_str(param[1])) goto _ERROR_EXIT_FUNC_;
	sscanf(param[1], "%d", &pkt_cnts);
	pwpsc->tx_pkt_cnts = pkt_cnts;
	if (param[2] == NULL)
		goto _EXECUTE_FUNC_;

	// paramer 3 : payload len
	if (!is_dec_str(param[2])) goto _ERROR_EXIT_FUNC_;
	sscanf(param[2], "%d", &payload_len);
	if ((payload_len<0) || (payload_len>2018)) goto _ERROR_EXIT_FUNC_;
	pwpsc->payload_len = payload_len;
	if (param[3] == NULL)
		goto _EXECUTE_FUNC_;

	// paramer 4 : payload type
	if (!is_dec_str(param[3])) goto _ERROR_EXIT_FUNC_;
	sscanf(param[3], "%d", &payload_type);
	if((payload_type<0) || (payload_type>4)) goto _ERROR_EXIT_FUNC_;
	pwpsc->payload_type = payload_type;
	if (param[4] == NULL)
		goto _EXECUTE_FUNC_;

	// paramer 5 : destination address
	if ((param[4][1]|0x20) == 'x')
		param5 = param[4] + 2;
	else
		param5 = param[4];

	if (strlen(param5) < (2 * ETH_ALEN)) goto _ERROR_EXIT_FUNC_;

	for (i = 0; i < ETH_ALEN; i++) {
		unsigned char CurByte;
		CurByte = Asc2Hex(param5[2 * i]);
		CurByte <<= 4;
		CurByte += Asc2Hex(param5[2 * i + 1]);
		pwpsc->da[i] = CurByte;
	}

_EXECUTE_FUNC_:

	if (pwpsc->bstarttx == 1) {
		printf("$-set_pkt_tx_hdl - bstarttx:%x pkt_cnts:%d len:%d type:0x%02x da:0x%s \n",\
			pwpsc->bstarttx, pwpsc->tx_pkt_cnts, pwpsc->payload_len,\
			payloaddata[pwpsc->payload_type], pwpsc->da);
	}

	pwpsc->mp_test_mode = PACKET_TX;

	r871x_set_pkt_xmit(pwpsc, bstarttx);

	return;

_ERROR_EXIT_FUNC_:
	printf("%s  %s\n", paramerrstr, pkttxstr);
	return;
}
//----------------------------------------------------------------------------------
void get_pkt_phy_rx_hdl(struct wpsctrl *pwpsc, char **param)
{
	int bstart = 0;
	unsigned int rx_ant = 0;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, pktrxstr);
		return;
	}
	sscanf(param[0], "%d", &bstart);
	if ((bstart < 0) || (bstart > 1)) {
		printf("%s  %s\n", paramerrstr, pktrxstr);
		return;
	}

	if ((param[1] != NULL) && is_dec_str(param[1])) {
		sscanf(param[1], "%d", &rx_ant);
		if ((rx_ant >= ANTENNA_A) && (rx_ant <= ANTENNA_AB)) {
			if (GET_RX_ANTENNA(pwpsc->antenna) != rx_ant) {
				unsigned int antenna = pwpsc->antenna;
				SET_RX_ANTENNA(pwpsc->antenna, rx_ant);
				if (r871x_set_antenna(pwpsc, pwpsc->antenna)) {
					pwpsc->antenna = antenna;
					printf("$-get_pkt_phy_rx_hdl - switch RX antenna FAIL!!\n");
					return;
				}
			}
		}
	}

	pwpsc->mp_test_mode = PACKET_RX;

	rx_ant = GET_RX_ANTENNA(pwpsc->antenna); // update to new rx antenna
	printf("$-get_pkt_phy_rx_hdl - mode:%d antenna:%d\n", bstart, rx_ant);
	r871x_get_pkt_recv(pwpsc, bstart);
	printf("\n");
}
//----------------------------------------------------------------------------------------
void read_eeprom_byte_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int offset = 0;
	unsigned char value;
	char *param1;

	if(strlen(param[0]) > 5) {
		printf("%s  %s\n",paramerrstr,reepromstr);
		return;
	}

	if((strlen(param[0])>2) && ((param[0][1] |0x20)=='x'))
		param1 = param[0] + 2;
	else
		param1 = param[0];

	if (!is_hex_str(param1)) {
		printf("%s  %s\n", paramerrstr, reepromstr);
		return;
	}

	sscanf(param1, "%x", &offset);
	if ((offset<0x00) || (offset>0xFF)) {
		printf("%s  %s\n", paramerrstr, reepromstr);
		return;
	}

	r871x_read_eeprom_byte(pwpsc, offset,&value);

	printf("$-read_eeprom_byte_hdl - offset:0x%02x value:0x%02x\n\n", offset, value);
}
//----------------------------------------------------------------------------------------
void write_eeprom_byte_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int offset = 0;
	unsigned int value = 0;

	char *param1, *param2;

	if ((strlen(param[0])>2) && ((param[0][1]|0x20)=='x'))
		param1 = param[0] + 2;
	else
		param1 = param[0];

	if ((strlen(param[1])>2) && ((param[1][1]|0x20)=='x'))
		param2 = param[1] + 2;
	else
		param2 = param[1];

	if ((!is_hex_str(param1)) || (!is_hex_str(param2))) {
		printf("%s  %s\n", paramerrstr, weepromstr);
		return;
	}

	sscanf(param1, "%x", &offset);
	sscanf(param2, "%x", &value);
	if ((offset<0x00) || (offset>0xFF) || (value<0x00) || (value>0xff)) {
		printf("%s  %s\n", paramerrstr, weepromstr);
		return;
	}

	r871x_write_eeprom_byte(pwpsc, (unsigned char)offset, (unsigned char)value);

	printf("$-write_eeprom_byte_hdl - offset:0x%02x value:0x%02x \n\n", offset, value);
}
//----------------------------------------------------------------------------------------
void efuse_hdl(struct wpsctrl *pwpsc, char **param)
{
	PFUNCTION_MENU efusemenu;

	if (!is_dev_up(pwpsc)) {
		printf("!Attention: device not up yet,\nuse \"ifconfig\" to up interface first!\n");
		return;
	}

	efusemenu = (PFUNCTION_MENU)malloc(sizeof(FUNCTION_MENU));
	efusemenu->prev = menu;
	efusemenu->cmds = efuse_cmds;
	efusemenu->cnts = CMDCNTS(efuse_cmds);
	sprintf(efusemenu->prompt, "%s/%s", menu->prompt, "efuse");

	menu = efusemenu;

	printf("%s", MP871x_EFUSE_TITLE);
	cmd_help_hdl(NULL, NULL);
}
//----------------------------------------------------------------------------------------
void read_efuse_raw_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0;
	unsigned char value[EFUSE_MAX_SIZE];
	char *param1;
	int i;

	if ((strlen(param[0]) > 2) && (((param[0][1] | 0x20)=='x')))
		param1 = param[0] + 2;
	else
		param1 = param[0];
	if ((!is_hex_str(param1)) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, refrawstr);
		return;
	}

	sscanf(param1, "%x", &addr);
	sscanf(param[1], "%d", &byte_cnts);
	if ((addr + byte_cnts) > EFUSE_MAX_SIZE) {
		printf("%s  %s\n", paramerrstr, refrawstr);
		return;
	}

	printf("$-read_efuse_raw_hdl - addr:0x%03x cnt:%d", addr, byte_cnts);

	if (r871x_read_efuse_raw(pwpsc, addr, byte_cnts, value)) {
		printf(" read fail!!");
	} else {
		printf("\nvalue:\t");
		for (i=0; i<byte_cnts; i++) {
			printf(" %02x", value[i]);
			if ((i & 0xF) == 0xF) printf("\n\t");
			else if ((i & 0x7) == 0x7) printf("\t");
		}
	}
	printf("\n\n");
}
//----------------------------------------------------------------------------------------
void write_efuse_raw_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0, v;
	unsigned char value[EFUSE_MAX_SIZE];
	char *p;
	int i, n;

	if ((strlen(param[0]) > 2) && (((param[0][1] | 0x20)=='x')))
		p = param[0] + 2;
	else
		p = param[0];
	if ((!is_hex_str(p)) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, wefrawstr);
		return;
	}

	sscanf(p, "%x", &addr);
	sscanf(param[1], "%d", &byte_cnts);
	if ((addr + byte_cnts) > EFUSE_MAX_SIZE) {
		printf("%s  %s\n", paramerrstr, wefrawstr);
		return;
	}

	n = byte_cnts + 2;
	for (i=2; i<n; i++) {
		if ((strlen(param[i]) > 2) && (((param[i][1] | 0x20)=='x')))
			p = param[i] + 2;
		else
			p = param[i];
		if (!is_hex_str(p)) {
			printf("%s  %s\n", paramerrstr, wefrawstr);
			return;
		}
		sscanf(p, "%x", &v);
		value[i-2] = (unsigned char)v;
	}

	printf("$-write_efuse_raw_hdl -  addr:0x%03x cnt:%d", addr, byte_cnts);
	if (!r871x_write_efuse_raw(pwpsc, addr, byte_cnts, value))
		printf(" ok");
	else
		printf(" fail!!");
	printf("\n\n");
}
//----------------------------------------------------------------------------------------
static void dump_efuse_map(unsigned char *map)
{
	int i, j;
	printf("\tOFFSET\tVALUE(hex)\n");
	for (i = 0; i < EFUSE_MAP_MAX_SIZE; i = i+16) {
		printf("\t0x%02x\t", i);
		for (j = 0; j < 8; j++)
			printf("%02x ", map[i+j]);
		printf("   ");
		for (; j < 16; j++)
			printf("%02x ", map[i+j]);
		printf("\n");
	}
	printf("\n");
}
//----------------------------------------------------------------------------------------
void read_efuse_map_hdl(struct wpsctrl *pwpsc, char **param)
{
	if (r871x_read_efuse_map(pwpsc, pwpsc->efuse_map)) {
		printf("$-read_efuse_map_hdl - fail!!\n");
		return;
	}

	printf("$-read_efuse_map_hdl - success\n");
	dump_efuse_map(pwpsc->efuse_map);
}
//----------------------------------------------------------------------------------------
void write_efuse_map_hdl(struct wpsctrl *pwpsc, char **param)
{
	if (r871x_write_efuse_map(pwpsc, pwpsc->efuse_map)){
		printf("$-write_efuse_map_hdl - fail!!\n");
		return;
	}
	printf("$-write_efuse_map_hdl - success\n");
}
//----------------------------------------------------------------------------------------
void show_efuse_map_hdl(struct wpsctrl *pwpsc, char **param)
{
	printf("$-show_efuse_map_hdl\n");
	dump_efuse_map(pwpsc->efuse_map);
}
//----------------------------------------------------------------------------------------
void read_efuse_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0;
	char *param1 = NULL;
	int i;

	if ((strlen(param[0]) > 2) && (((param[0][1] | 0x20)=='x')))
		param1 = param[0] + 2;
	else
		param1 = param[0];
	if ((!is_hex_str(param1)) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, rmapstr);
		return;
	}

	sscanf(param1, "%x", &addr);
	sscanf(param[1], "%d", &byte_cnts);

	if ((byte_cnts < 1) || (byte_cnts > EFUSE_MAP_MAX_SIZE)) {
		printf("%s  %s\n", paramerrstr, rmapstr);
		return;
	}

	printf("$-read_efuse_hdl - addr:0x%02x cnt:%d value(hex):", addr, byte_cnts);
	for (i = 0; i < byte_cnts; i++)
		if (addr+i < 128) printf(" %02x", pwpsc->efuse_map[addr+i]);
	printf("\n\n");
}
//----------------------------------------------------------------------------------------
void write_efuse_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int addr = 0, byte_cnts = 0, value;
	char *param1 = NULL, *p;
	int i;

	if ((strlen(param[0]) > 4) && ((param[0][1] | 0x20) == 'x'))
		param1 = param[0] + 2;
	else
		param1 = param[0];

	if ((!is_hex_str(param1)) || (!is_dec_str(param[1]))) {
		printf("%s  %s\n", paramerrstr, wmapstr);
		return;
	}

	sscanf(param1, "%x", &addr);
	sscanf(param[1], "%d", &byte_cnts);

	printf("$-write_efuse_hdl - addr:0x%02x cnt:%d\n", addr, byte_cnts);
	for (i = 0; i < byte_cnts; i++) {
		if ((addr + i) >= 128) break;
		p = param[i + 2];
		if (p == NULL) break;
		sscanf(p, "%x", &value);
		pwpsc->efuse_map[addr + i] = (unsigned char)value;

//		printf("write addr:0x%02x value:0x%02x ok.\n", addr + i, pwpsc->efuse_map[addr + i]);
	}
#if 0
	if (i != byte_cnts)
		printf("$-write_efuse_hdl: fail!!\n\n");
	else
		printf("$-write_efuse_hdl: success.\n\n");
#endif
}
//----------------------------------------------------------------------------------------
void change_mac_address_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int address[ETH_ALEN], type;
	char delimiters[] = " ,.-:";
	char *p;
	int i, j;

	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, chmacstr);
		return;
	}
	sscanf(param[0], "%d", &type);
	if ((type != 1) && (type != 2)) {
		printf("%s  %s\n", paramerrstr, chmacstr);
		return;
	}
	if (type == 1) type = SDIO_MAC_ADDRESS_OFFSET;
	else type = USB_MAC_ADDRESS_OFFSET;

	i = 0;
	j = 1;
	do {
		if (param[j] == NULL) break;
		p = strtok(param[j++], delimiters);
		for (; i < ETH_ALEN; i++) {
			if (p == NULL) break;
			sscanf(p, "%x", &address[i]);
			p = strtok(NULL, delimiters);
		}
	} while (i < ETH_ALEN);

	if (i != ETH_ALEN) {
		printf("%s  %s\n", paramerrstr, chmacstr);
		return;
	}

	if (pwpsc->efuse_map[0] == 0xFF) {
		printf("E-Fuse Map is not vaild, read from driver.\n");
		if (r871x_read_efuse_map(pwpsc, pwpsc->efuse_map)) {
			printf("!!Read E-Fuse Map from driver fail!!\n");
		}
		if (pwpsc->efuse_map[0] == 0xFF) {
			printf("use default Map data!\n");
			if (type == SDIO_MAC_ADDRESS_OFFSET)
				memcpy(pwpsc->efuse_map, DFT_SDIO_MAP_V04, EFUSE_MAP_MAX_SIZE);
			else {
				printf("USB not support yet!\n Change MAC Address fail!!");
				return;
			}
		}
	}

	for (i = 0; i < ETH_ALEN; i++)
		pwpsc->efuse_map[type+ i] = (unsigned char) address[i];

}
//----------------------------------------------------------------------------------------
void set_power_tracking_mode_hdl(struct wpsctrl *pwpsc, char **param)
{
	int ptm_mode = 0;
	if (!is_hex_str(param[0])){
		printf("%s  %s\n", paramerrstr, setptmstr);
		return;
	}
	sscanf(param[0], "%x", &ptm_mode);

	if ((ptm_mode<_POWERTRACK_OFF_) || (ptm_mode>_POWERTRACK_CUTTX_CCK_)) {
		printf("%s%s\n", paramerrstr, setptmstr);
		return;
	}

	printf("$-set_power_tracking_mode_hdl - mode:0x%02x\n\n",ptm_mode);
	r871x_set_ptm(pwpsc, ptm_mode);

}
//----------------------------------------------------------------------------------------
void get_target_tssi_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int tarteg_tssi;
	r871x_get_target_tssi(pwpsc, &tarteg_tssi);
	printf("$-get_target_tssi_hdl - tarteg_tssi:0x%02x\n\n", tarteg_tssi);
}
//----------------------------------------------------------------------------------------
void auto_gen_tssi_hdl(struct wpsctrl *pwpsc, char **param)
{
	printf("$-auto_gen_tssi_hdl - tarteg_tssi\n\n");
	r871x_gen_tssi_procedure(pwpsc);
}
//----------------------------------------------------------------------------------------
void read_thermal_meter_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int thermal_meter;
	r871x_read_thermal_meter(pwpsc, &thermal_meter);
	printf("$-read_thermal_meter_hdl - thermal meter:0x%02x\n\n", thermal_meter);
}
//----------------------------------------------------------------------------------------
void read_txpower_hdl(struct wpsctrl *pwpsc, char **param)
{
	int bOFDM = 0, i, chidx;
	unsigned char *ptxpowertab;
	char tmpstr[100], shortstr[50];

	if (!is_hex_str(param[0])) {
		printf("%s  %s\n", paramerrstr, readtxpowerstr);
		return;
	}

	sscanf(param[0],"%x", &bOFDM);
	if ((bOFDM<0) || (bOFDM>1)) {
		printf("%s  %s\n", paramerrstr, readtxpowerstr);
		return;
	}

	r871x_read_txpower_from_eeprom(pwpsc, bOFDM);

	memset(tmpstr, 0, 100);
	memset(shortstr, 0, 50);
	sprintf(tmpstr, "$-read_txpower_hdl -");

	if (!bOFDM) {
		ptxpowertab = pwpsc->eeprom_info.TxPower.CCK;
		sprintf(shortstr," [CCK]: ");
	} else {
		ptxpowertab = pwpsc->eeprom_info.TxPower.OFDM;
		sprintf(shortstr," [OFDM]: ");
	}
	strcat(tmpstr,shortstr);

	for (i=0; i<4; i++) {
		if	(i==0) chidx=1;
		else if	(i==1) chidx=6;
		else if	(i==2) chidx=11;
		else if	(i==3) chidx=14;
		sprintf(shortstr, " ch%d-0x%02x ", chidx, ptxpowertab[i]);
		strcat(tmpstr, shortstr);
	}
	printf("%s\n\n", tmpstr);
}
//----------------------------------------------------------------------------------------
void write_txpower_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int bOFDM = 0;
	unsigned int txpowertab[4];
	unsigned char *ptxpowertab = NULL;

	char *flag;
	char *param1 = NULL;
	char *param2 = NULL;
	char *param3 = NULL;
	char *param4 = NULL;

	int i;
	char tmpstr[50];

	for (i=0; i<4; i++) {
		if (strlen(param[i+1]) > 4) {
			printf("#1 %s  %s\n", paramerrstr, writetxpowerstr);
			return;
		}
	}

	flag = param[0];

	if((strlen(param[1])>2) && ((param[1][1]|0x20)=='x'))
		param1 = param[1] + 2;
	else
		param1 = param[1];

	if((strlen(param[2])>2) && ((param[2][1]|0x20)=='x'))
		param2 = param[2] + 2;
	else
		param2 = param[2];

	if((strlen(param[3])>2) && ((param[3][1]|0x20)=='x'))
		param3 = param[3] + 2;
	else
		param3 = param[3];

	if((strlen(param[4])>2) && ((param[4][1]|0x20)=='x'))
		param4 = param[4] + 2;
	else
		param4 = param[4];

	if ((!is_hex_str(flag)) || (!is_hex_str(param1)) ||
	    (!is_hex_str(param2)) || (!is_hex_str(param3)) || (!is_hex_str(param4)))
	{
		printf("#2 %s  %s\n", paramerrstr, writetxpowerstr);
		return;
	}

	sscanf(flag, "%x", &bOFDM);
	if ((bOFDM<0x00) || (bOFDM>0x01)) {
		printf("#3 %s  %s\n", paramerrstr, writetxpowerstr);
		return;
	}

	memset(txpowertab, 0, 4);
	sscanf(param1, "%x", &txpowertab[0]);
	sscanf(param2, "%x", &txpowertab[1]);
	sscanf(param3, "%x", &txpowertab[2]);
	sscanf(param4, "%x", &txpowertab[3]);

	if (!bOFDM)
		ptxpowertab = pwpsc->eeprom_info.TxPower.CCK; //ptssitab = pwpsc->tssi_table.cck;
	else
		ptxpowertab = pwpsc->eeprom_info.TxPower.OFDM; //ptssitab = pwpsc->tssi_table.ofdm;

	//memcpy(ptxpowertab, txpowertab, sizeof(unsigned char)*4);

	for (i=0; i<4; i++)
        	ptxpowertab[i] = (unsigned char)txpowertab[i];

	memset(tmpstr, 0, 50);
	if (bOFDM == 1)
		sprintf(tmpstr, "OFDM");
	else
		sprintf(tmpstr, "CCK");

	printf("$-write_tssi_hdl - [%s]: p1-0x%02x p2-0x%02x p3-0x%02x p4-0x%02x \n\n",
			tmpstr, txpowertab[0], txpowertab[1], txpowertab[2], txpowertab[3]);

	r871x_write_txpower_into_eeprom(pwpsc, bOFDM);
}
//----------------------------------------------------------------------------------------
void read_tssi_hdl(struct wpsctrl *pwpsc, char **param)//READ TSSI from EEPROM
{
	int bOFDM = 0, i, chidx;
	unsigned char *ptssitab;
	char tmpstr[100], shortstr[50];

	if (!is_hex_str(param[0])) {
		printf("%s %s\n", paramerrstr, readtssistr);
		return;
	}

	sscanf(param[0], "%x", &bOFDM);
	if ((bOFDM<0) || (bOFDM>1)) {
		printf("%s  %s\n",paramerrstr,readtssistr);
		return;
	}

	r871x_read_tssi_from_eeprom(pwpsc, bOFDM);

	memset(tmpstr, 0, 100);
	memset(shortstr, 0, 50);
	sprintf(tmpstr, "$-read_tssi_hdl -");

	if (!bOFDM) {
		//ptssitab = pwpsc->tssi_table.cck;
		ptssitab =  pwpsc->eeprom_info.TSSI.CCK;
		sprintf(shortstr," [CCK]: ");
	} else {
		//ptssitab = pwpsc->tssi_table.ofdm;
		ptssitab = pwpsc->eeprom_info.TSSI.OFDM;
		sprintf(shortstr," [OFDM]: ");
	}
	strcat(tmpstr,shortstr);

	for (i=0; i<4; i++) {
		if	(i==0) chidx=1;
		else if	(i==1) chidx=6;
		else if	(i==2) chidx=11;
		else if	(i==3) chidx=14;
		sprintf(shortstr," ch%d-0x%02x ",chidx,ptssitab[i]);
		strcat(tmpstr,shortstr);
	}
	printf("%s\n\n",tmpstr);

}
//----------------------------------------------------------------------------------------
void write_tssi_hdl(struct wpsctrl *pwpsc, char **param)
{
	unsigned int bOFDM = 0;
	unsigned int tssitab[4];
	unsigned char *ptssitab = NULL;

	char *flag;
	char *param1 = NULL;
	char *param2 = NULL;
	char *param3 = NULL;
	char *param4 = NULL;

	int i;
	char tmpstr[50];

	for (i=0; i<4; i++) {
		if (strlen(param[i+1]) > 4) {
			printf("#1 %s  %s\n", paramerrstr, writetssistr);
			return;
		}
	}

	flag = param[0];

	if((strlen(param[1])>2) && ((param[1][1]|0x20)=='x'))
		param1 = param[1] + 2;
	else
		param1 = param[1];

	if((strlen(param[2])>2) && ((param[2][1]|0x20)=='x'))
		param2 = param2+2;
	else
		param2 = param2;

	if((strlen(param[3])>2) && ((param[3][1]|0x20)=='x'))
		param3 = param[3] + 2;
	else
		param3 = param[3];

	if((strlen(param[4])>2) && ((param[4][1]|0x20)=='x'))
		param4 = param[4] + 2;
	else
		param4 = param[4];

	if ((!is_hex_str(flag)) || (!is_hex_str(param1)) ||
	    (!is_hex_str(param2)) || (!is_hex_str(param3)) || (!is_hex_str(param4)))
	{
		printf("#2 %s  %s\n", paramerrstr, writetssistr);
		return;
	}

	sscanf(flag, "%x", &bOFDM);
	if ((bOFDM < 0x00) || (bOFDM > 0x01)) {
		printf("#3 %s  %s\n", paramerrstr, writetssistr);
		return;
	}

	memset(tssitab, 0, 4);
	sscanf(param1, "%x", &tssitab[0]);
	sscanf(param2, "%x", &tssitab[1]);
	sscanf(param3, "%x", &tssitab[2]);
	sscanf(param4, "%x", &tssitab[3]);

	if (!bOFDM)
		ptssitab = pwpsc->eeprom_info.TSSI.CCK ; //ptssitab = pwpsc->tssi_table.cck;
	else
		ptssitab = pwpsc->eeprom_info.TSSI.OFDM ; //ptssitab = pwpsc->tssi_table.ofdm;

	for (i=0; i<4; i++)
		ptssitab[i] = tssitab[i];

	memset(tmpstr, 0, 50);
	if (bOFDM == 1)
		sprintf(tmpstr,"OFDM");
	else
		sprintf(tmpstr,"CCK");

	printf("$-write_tssi_hdl - [%s]: p1-0x%02x p2-0x%02x p3-0x%02x p4-0x%02x \n\n",
			tmpstr, ptssitab[0], ptssitab[1], ptssitab[2], ptssitab[3]);
	r871x_write_tssi_into_eeprom(pwpsc, bOFDM);

}
//----------------------------------------------------------------------------------
void power_down_hdl(struct wpsctrl *pwpsc, char **param)
{
	int bstart = 0;
	if (!is_dec_str(param[0])) {
		printf("%s  %s\n", paramerrstr, power_down_str);
		return;
	}

	sscanf(param[0], "%d", &bstart);
	if ((bstart < 0) || (bstart > 1)) {
		printf("%s  %s\n", paramerrstr, power_down_str);
		return;
	}
	r871x_power_down(pwpsc, bstart);

	printf("$-power_down_hdl - mode:%d [1:power up/0:power down]\n\n", bstart);
}
//----------------------------------------------------------------------------------------
void mpstart_hdl(struct wpsctrl *pwpsc, char **param)
{
	if (!r871x_set_mp_start(pwpsc))
		pwpsc->enter_mp_test_mode = 1;
	else {
		printf("$-mpstart_hdl - fail\n\n");
		exit(0);
	}
}
//----------------------------------------------------------------------------------------
static int parse_regcmd(INPUT_CMD *pinput, int argc, char **argv, int *cmd)
{
	int i, n;
	int ret = 0;

	n = CMDCNTS(regio_cmds);

	strcpy(pinput->cmd, argv[0]);
	for (i = 0; i < n; i++)
	{
		if (strcmp(pinput->cmd, regio_cmds[i].command)) continue;

		*cmd = i;

		if (argc < regio_cmds[i].param_cnts) {
			printf("parameters error!!\n");
			return -1;
		}

		ret = pinput->param_cnts = regio_cmds[i].param_cnts;

		break;
	}

	if (i == n) {
		printf("command not found!!\n");
		return -1;
	}

	for (i = 0; i < ret; i++)
		pinput->param[i] = argv[i+1];

	return ret;
}
//----------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int	mp_mode, ret = 0;
	char	cmdstr[80];
	char	input_char;
	int	cmd_index = 0;
	int 	i, n;
	int	action = 0;
	int	single_run = 0;
	INPUT_CMD input;

	struct wpsctrl *pwpsc = NULL;
	struct wpsctrl_adapter_ops wpsc_ops;


	pwpsc = malloc(sizeof(struct wpsctrl));
	if (pwpsc == NULL) return 0;
	memset(pwpsc, 0, sizeof(struct wpsctrl));
	pwpsc->pwpsc_ops = &wpsc_ops;

//	printf("argc=%d, argv=%s\n", argc, argv[1]);

	//default
	mp_mode = 1;
	strncpy(pwpsc->ifrn_name, "wlan0", IFNAMSIZ);
	strncpy(interface, "wlan0", IFNAMSIZ);

	if (argc == 1) {
		strncpy(pwpsc->ifrn_name, "wlan0", IFNAMSIZ);
		strncpy(interface, "wlan0", IFNAMSIZ);
		//printf("interface=%s\n", interface);
	} else {
		i = 1;
		do {
			if (!strcmp("normal", argv[i])) {
				printf("enter normal mode\n");
				mp_mode = 0;
 				if (!action) action = 3;	// 1:EMI, 2:EFUSE, 3:REG IO
				i++;
				continue;
			}

			if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
					case 'f':
						if (++i >= argc) {
							ret = -1;
							break;
						}
						sscanf(argv[i], "%d", &action);
						break;

					case 'c':
						if (++i >= argc) {
							ret = -1;
							break;
						}

						single_run = 1;

						memset(&input, 0, sizeof(INPUT_CMD));
						n = parse_regcmd(&input, argc-i-1, &argv[i], &cmd_index);
						if (n < 0) {
							ret = n;
							break;
						}
						i += n;
						break;

					default:
						printf("error, try again!\n");
						printf("mp8712 [OPTION]... INTERFACE\n\n");
						printf("OPTIONS\n");
						printf("\tnormal\n\t\tNormal Mode, otherwise MP Mode\n\n");
						printf("\t-f FUNCTION\n\t\tchange to FUNCTION menu directly.\n\t\tFUNCTION value can be 1-3.\n\n");
						ret = -1;
						break;
				}

				if (ret != 0) break;
				i++;
				continue;
			}

			strncpy(pwpsc->ifrn_name, argv[i], IFNAMSIZ);
			strncpy(interface, argv[i], IFNAMSIZ);
			i++;
		} while (i < argc);
	}

	if (ret != 0) {
		free(pwpsc);
		return ret;
	}

	if ((!r871x_mp_init(pwpsc)) || (pwpsc==NULL)) {
		printf("mp_init fail\n");
		free(pwpsc);
		return -1;
	}

	if (single_run == 1) {
		regio_cmds[cmd_index].func(pwpsc, input.param);
		free(pwpsc);
		return 0;
	}

	if (mp_mode)//r871x_set_mp_start(pwpsc);
		mpstart_hdl(pwpsc, NULL);

	printf("%s", MP871x_UTILITY_TITLE);

	menu = (PFUNCTION_MENU)malloc(sizeof(FUNCTION_MENU));
	menu->prev = NULL;
	menu->cmds = mp871x_cmds;
	menu->cnts = CMDCNTS(mp871x_cmds);
	strcpy(menu->prompt, "rtl8712");

	if (action)
		menu->cmds[action].func(pwpsc, input.param);
	if (menu->cmds == mp871x_cmds) // still at main menu page
		cmd_help_hdl(NULL, NULL);

	printf("interface=%s\n", interface);

	if (!is_dev_up(pwpsc))
		printf("\n!Attention: device not up yet.\n");

	while(1)
	{
		printf("%s> ", menu->prompt);

		memset(cmdstr, 0, sizeof(cmdstr));

		cmd_index = 0;

		while ((input_char = getchar()) != '\n')
		{
			// skip start space
			if ((cmd_index == 0) &&
			    ((input_char == ' ') || (input_char == '\t')))
				continue;

			cmdstr[cmd_index++] = input_char;

#ifdef _AVANTIS_PLATFORM_
			if ((input_char == 0x08) && (cmd_index > 0)) {
				cmd_index--;
			}
#endif
		}
//		printf("cmd_index:%d cmd_str:%s\n", cmd_index, cmdstr);

		if (strlen(cmdstr) == 0) continue;

		memset(&input, 0, sizeof(INPUT_CMD));
		cmdparser(cmdstr, &input);	// need to free input.param[] later
//		printf("%s,%d\n",input.cmd,input.param_cnts);

		for (i = 0; i < menu->cnts; i++)
		{
			if (strcmp(input.cmd, menu->cmds[i].command)) continue;

			if (input.param_cnts < menu->cmds[i].param_cnts) {
				printf("\tparameters error!!\n");
				cmd_help_hdl(NULL, NULL);
				break;
			}

			menu->cmds[i].func(pwpsc, input.param);
			break;
		}

		if (i == menu->cnts)
			printf("\tcommand not found!!\n");

		// free parameters
		for (i = 0; i < input.param_cnts; i++) {
			free(input.param[i]);
			input.param[i] = NULL;
		}
	}

	free(menu);
	free((void *)pwpsc);

	return ret;
}

