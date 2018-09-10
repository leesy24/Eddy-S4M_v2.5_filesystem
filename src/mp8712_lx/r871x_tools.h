#ifndef _R871X_TOOLS_H
#define _R871X_TOOLS_H

#include "rtl871x_mp_ioctl.h"


#define MAX_CMD_LEN		20
#define MAX_PARAMETER_NUM	20

typedef struct _CMD_STRUCTURE_ {
	char command[MAX_CMD_LEN];
	int param_cnts;
	void (*func)(struct wpsctrl*, char**);
	const char *description;
}CMD_STRUCTURE;

#define CMDCNTS(cmds) sizeof(cmds)/sizeof(CMD_STRUCTURE)

typedef struct _FUNCTION_MENU_ FUNCTION_MENU, *PFUNCTION_MENU;

struct _FUNCTION_MENU_ {
	PFUNCTION_MENU prev;
	CMD_STRUCTURE *cmds;
	int cnts;
	char prompt[50];
};

typedef struct _INPUT_CMD_ {
	char cmd[MAX_CMD_LEN];
	int  param_cnts;
	char* param[MAX_PARAMETER_NUM];
}INPUT_CMD;


void cmd_help_hdl(struct wpsctrl *pwpsc, char **param);
void exit_hdl(struct wpsctrl *pwpsc, char **param);

/* EMI */
void emi_hdl(struct wpsctrl *pwpsc, char **param);
void mpstart_hdl(struct wpsctrl *pwpsc, char **param);

void set_channel_hdl(struct wpsctrl *pwpsc, char **param);
void set_output_power_hdl(struct wpsctrl *pwpsc, char **param);
void set_data_rate_hdl(struct wpsctrl *pwpsc, char **param);
void set_bandwidth_hdl(struct wpsctrl *pwpsc, char **param);

void set_continuous_tx_hdl(struct wpsctrl *pwpsc,  char **param);
void set_single_carrier_tx_hdl(struct wpsctrl *pwpsc,  char **param);
void set_carrier_suppression_tx_hdl(struct wpsctrl *pwpsc, char **param);
void set_pkt_tx_hdl(struct wpsctrl *pwpsc, char **param);
void get_pkt_phy_rx_hdl(struct wpsctrl *pwpsc, char **param);

void set_dynamic_initial_gain_state_hdl(struct wpsctrl *pwpsc, char **param);
void set_cca_level_hdl(struct wpsctrl *pwpsc, char **param);
void set_power_save_mode_hdl(struct wpsctrl *pwpsc, char **param);

void set_power_tracking_mode_hdl(struct wpsctrl *pwpsc, char **param);
void get_target_tssi_hdl(struct wpsctrl *pwpsc, char **param);
void auto_gen_tssi_hdl(struct wpsctrl *pwpsc, char **param);
void read_tssi_hdl(struct wpsctrl *pwpsc, char **param);
void write_tssi_hdl(struct wpsctrl *pwpsc, char **param);

void read_txpower_hdl(struct wpsctrl *pwpsc, char **param);
void write_txpower_hdl(struct wpsctrl *pwpsc, char **param);


/* E-Fuse */
void read_eeprom_byte_hdl(struct wpsctrl *pwpsc, char **param);
void write_eeprom_byte_hdl(struct wpsctrl *pwpsc, char **param);

void efuse_hdl(struct wpsctrl *pwpsc, char **param);
void read_efuse_map_hdl(struct wpsctrl *pwpsc, char **param);
void write_efuse_map_hdl(struct wpsctrl *pwpsc, char **param);
void show_efuse_map_hdl(struct wpsctrl *pwpsc, char **param);
void read_efuse_hdl(struct wpsctrl *pwpsc, char **param);
void write_efuse_hdl(struct wpsctrl *pwpsc, char **param);

void read_efuse_raw_hdl(struct wpsctrl *pwpsc, char **param);
void write_efuse_raw_hdl(struct wpsctrl *pwpsc, char **param);

void change_mac_address_hdl(struct wpsctrl *pwpsc, char **param);

void read_thermal_meter_hdl(struct wpsctrl *pwpsc, char **param);

/* Register Read/Write */
void regio_hdl(struct wpsctrl *pwpsc, char **param );
void read_reg_hdl(struct wpsctrl *pwpsc, char **param );
void write_reg_hdl(struct wpsctrl *pwpsc, char **param );
void read_rfreg_hdl(struct wpsctrl *pwpsc, char **param );
void write_rfreg_hdl(struct wpsctrl *pwpsc, char **param );
void read_bbreg_hdl(struct wpsctrl *pwpsc, char **param );
void write_bbreg_hdl(struct wpsctrl *pwpsc, char **param );


/* Others */
void power_down_hdl(struct wpsctrl *pwpsc, char **param);

#ifdef CONFIG_MP871X_DBG
#include "r871x_tools_dbg.h"
#endif

#endif

