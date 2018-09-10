/*****************************************************************************
Title		: WiFi Operation code

Description	: After Wifi configuration files that set in the WEB, Telnet is read, 
			  Wifi module can be operated.
Argments  1	: None
 ******************************************************************************/
#include "include/sb_include.h"
#include "include/sb_define.h"
#include "include/sb_shared.h"
#include "include/sb_config.h"
#include "include/sb_extern.h"
#include "include/sb_ioctl.h"
#include "include/sb_misc.h"

//=============================================================================
int main(int argc, char *argv[])
{
struct SB_SYSTEM_CONFIG			CFG_SYS;
struct SB_WIFI_CONFIG			CFG_WIFI;
char cmd[256];
int fd;

	SB_msleep (4500);
	SB_ReadConfig  (CFGFILE_ETC_SYSTEM,  (char *)&CFG_SYS, 		sizeof (struct SB_SYSTEM_CONFIG));
	SB_ReadConfig  (CFGFILE_ETC_WIFI,    (char *)&CFG_WIFI,		sizeof(struct SB_WIFI_CONFIG));

	system ("insmod  /etc/drivers/rt73.ko");
	SB_msleep (1000);
	sprintf (cmd, "ifconfig %s up", SB_WLAN_RA_NAME);
	system (cmd);
	SB_msleep (500);

	memset (cmd, 0x00, sizeof(cmd));
	system("/sbin/ifconfig | grep rausb0 > /var/tmp/ifconfig");
	fd = open("/var/tmp/ifconfig", O_RDONLY);
	read(fd, cmd, sizeof(cmd));
	close(fd);
	if (strstr(cmd, SB_WLAN_RA_NAME) == NULL) 
		{
		printf ("WiFi module not found!\n");	
		return 0;
		}

	if(CFG_WIFI.wifi_mode == SB_DISABLE)
		{
		sprintf (cmd, "iwpriv %s set NetworkType=Infra", SB_WLAN_RA_NAME);
		system (cmd);

		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set WirelessMode=%d", SB_WLAN_RA_NAME, CFG_WIFI.b_g_mode);
		system (cmd);
		}
	else	//Adhoc
		{
		int adhoc_mode = 0;
		sprintf (cmd, "iwpriv %s set NetworkType=Adhoc", SB_WLAN_RA_NAME);
		system (cmd);

		SB_msleep (10);
		switch(CFG_WIFI.b_g_mode)
			{
			case 0:					// 802.11b/g
				adhoc_mode = 1;
				break;
			case 1:					// b only
				adhoc_mode = 0;
				break;
			case 2:					// A only
				adhoc_mode = 3;
				break;
			case 3:					// a,b,g
				adhoc_mode = 1;
				break;
			case 4:					// g only
				adhoc_mode = 2;
				break;
			}
		sprintf (cmd, "iwpriv %s set AdhocOfdm=%d", SB_WLAN_RA_NAME, adhoc_mode);
		system (cmd);
		}

	sprintf (cmd, "iwpriv %s set SSID=%s", SB_WLAN_RA_NAME, CFG_WIFI.ssid);
	system (cmd);
	
	if(CFG_WIFI.channel > 0)	// 0 = Auto
		{
		SB_msleep (10);
		sprintf (cmd, "iwconfig %s channel %d", SB_WLAN_RA_NAME, CFG_WIFI.channel);
		system (cmd);
		}

	if(CFG_WIFI.bit_rate > 0)	// 0 = Auto
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set TxRate=%d", SB_WLAN_RA_NAME, CFG_WIFI.bit_rate);
		system (cmd);
		}

	if(CFG_WIFI.rts_thr > 0 && CFG_WIFI.rts_thr < 2348)
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set RTSThreshold=%d", SB_WLAN_RA_NAME, CFG_WIFI.rts_thr);
		system (cmd);
		}

	if(CFG_WIFI.frag_thr > 255 && CFG_WIFI.frag_thr < 2347)
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set FragThreshold=%d", SB_WLAN_RA_NAME, CFG_WIFI.frag_thr);
		system (cmd);
		}

	if(CFG_WIFI.ieee80211h > 0)
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set IEEE80211H=%d", SB_WLAN_RA_NAME, CFG_WIFI.ieee80211h);
		system (cmd);
		}

	if(CFG_WIFI.roaming_mode > 0)
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set FastRoaming=1", SB_WLAN_RA_NAME);
		system (cmd);

		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set RoamThreshold=%d", SB_WLAN_RA_NAME, CFG_WIFI.roaming_threshold);
		system (cmd);
		}

	SB_msleep (10);
	sprintf (cmd, "iwpriv %s auth %d", SB_WLAN_RA_NAME, CFG_WIFI.auth_mode+1);
	system (cmd);

	SB_msleep (10);
	sprintf (cmd, "iwpriv %s enc %d", SB_WLAN_RA_NAME, CFG_WIFI.encryp_type+1);
	system (cmd);

	if(CFG_WIFI.encryp_type == 1)	//WEP
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s set Key%d=%s", SB_WLAN_RA_NAME, CFG_WIFI.key_index+1, CFG_WIFI.key);
		system (cmd);
		}
	else if(CFG_WIFI.encryp_type == 2 || CFG_WIFI.encryp_type == 3)	//TKIP, AES
		{
		SB_msleep (10);
		sprintf (cmd, "iwpriv %s wpapsk %s", SB_WLAN_RA_NAME, CFG_WIFI.passphrase);
		system (cmd);
		}
 
	sprintf (cmd, "/sbin/route del default gw %d.%d.%d.%d dev %s",
		CFG_SYS.gateway[0], CFG_SYS.gateway[1],CFG_SYS.gateway[2],CFG_SYS.gateway[3],SB_WAN_ETH_NAME);
	system (cmd);

	SB_msleep (20);
	if(CFG_WIFI.line == 0)
		{
		sprintf (cmd, "/sbin/udhcpc -i %s &", SB_WLAN_RA_NAME);
		system (cmd);
		}
	else
		{
		FILE *fp;
		sprintf (cmd, "ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d", 
			SB_WLAN_RA_NAME,
			CFG_WIFI.ip[0], 
			CFG_WIFI.ip[1], 
			CFG_WIFI.ip[2], 
			CFG_WIFI.ip[3], 
			CFG_WIFI.mask[0], 
			CFG_WIFI.mask[1], 
			CFG_WIFI.mask[2], 
			CFG_WIFI.mask[3]);
		system (cmd);
		
		if ((fp = fopen("/etc/resolv.conf", "a+")) != NULL)
			{
			fprintf(fp, "nameserver\t%d.%d.%d.%d\n",
			CFG_WIFI.dns[0],CFG_WIFI.dns[1],CFG_WIFI.dns[2],CFG_WIFI.dns[3]);
			fclose(fp);
			}
	
		sprintf (cmd, "/sbin/route add default gw %d.%d.%d.%d dev %s",
			CFG_WIFI.gateway[0],CFG_WIFI.gateway[1],CFG_WIFI.gateway[2],CFG_WIFI.gateway[3],SB_WLAN_RA_NAME);
		system (cmd);
		}
		
	system ("echo  1 > /proc/sys/net/ipv4/ip_forward");		
	
	sprintf (cmd, "/usr/local/sbin/iptables -t nat -A POSTROUTING -s %d.%d.%d.0/%d.%d.%d.%d -j MASQUERADE", 
		CFG_SYS.ip[0], CFG_SYS.ip[1], CFG_SYS.ip[2],
		CFG_SYS.mask[0], CFG_SYS.mask[1], CFG_SYS.mask[2], CFG_SYS.mask[3]);
	system (cmd);			
	SB_msleep (2000);
	return 0;
}
