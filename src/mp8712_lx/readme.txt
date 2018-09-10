Realtek RTL8712S Wireless Adapter Utility
Copyright 2009 Realtek Semiconductor Corp.
	Version: 2.3.0

NAME
	mp8712 - tool for RTL8712S SDIO/USB mass production.

SYNOPSIS
	mp8712 [options] [interface]

DESCRIPTION
	mp8712 is a tool for RTL8712S USB/SDIO EMI test and EFuse program.
	It's also a sample code for TX Power calibration and auto test.

	Before using mp8712, you have to run RTL8712S driver at MP MODE and bring UP
	this wireless lan interface for sure mp8712 tool can work well.

	You can insert 8712 driver with parameter "mp_mode=1" to make
	driver enter mp mode.
	ex. "insmod 8712s.ko mp_mode=1".

	And use "ifconfig" to bring up RTL8712S,
	ex "ifconfig wlan1 up".

PARAMETERS
	interface
		The name of the RTL8712S wireless interface.
		Usually can use "iwconfig" to find this.
		Default value is "wlan0".

OPTIONS
	normal
		Ask mp tool not to do MP initialiation.
		This would cause EMI test abnormal.

	-f N
		Go to specific function menu directly,
		and won't show the first function choosing menu.
		N can be
			1: EMI Test
			2: EFuse Program
			3: Register Read/Write

		If normal mode is enable, and "-f" doesn't be used,
		N would be set to 3.

	-c [command]
		Execute single register IO command,
		and would not enter mp program.
		command please reference "Register Read/Write Menu".
		ex.
		# ./mp8712 -c 2reg 50 4 874ce000
		$-write_reg_hdl -  addr:0x10250050 bytes:4 value:0x874ce000 success
		# ./mp8712 -c rreg 50 4
		$-read_reg_hdl -  addr:0x10250050 bytes:4 value:0x874ce000

DISPLAY
	The first screen looks like below, if parameter "-f" not be set.

[root@localhost mp8712_lx]# ./mp8712 wlan1

	  Realtek RTL8712 Wireless Adapter Utility
	  Copyright 2009 Realtek Semiconductor Corp.
		Version: 2.0.0

	===========================================
	  help
	  emi		EMI Functions
	  efuse		E-Fuse functions
	  reg		Register Read/Write
	  exit
	===========================================

interface=wlan1
rtl8712>

	All the commands' function description are below

	help
		Print the commands list.

	emi
		Go to EMI test function menu page.

	efuse
		Go to E-Fuse program function menu page.

	reg
		Go to Register Read/Write function menu page.

	exit
		Exit mp8712 program.



	If you go to EMI Test, the screen would be like

rtl8712> emi

	  EMI Functions Menu

	===========================================
	  help
	  setchan chanid[1~14]
	  settxpower powerindex[0x00~0x3F]
	  setrate rateindex[0~3:CCK 4~11:OFDM 12~27:MCS0~15]
	  setbw bandwidth[0:20MHz 1:40MHz]
	  continuoustx mode[1/0(start/stop)]
	  singletx mode[1/0(start/stop)]
	  carriertx mode[1/0(start/stop)]
	  pkttx mode[1/0(start/stop)] cnts len type[0~3] da
	  pktrx mode[1/0(start/stop)]
	  exit
	===========================================

rtl8712/emi>

	Function description is as follow
	Remember EMI function work correctly only when interface is up.

# setchan chanid[1~14]
	[P1]chanid  1~14

# settxpower powerindex[0x00~0x3F]
  set tx power index

# setrate rateindex[0~27]
	[P1]rate_index <-> data rate mapping
		 0		: CCK 1M
		 1		: CCK 2M
		 2		: CCK 5.5M
		 3		: CCK 11M
		 4		: OFDM 6M
		 5		: OFDM 9M
		 6		: OFDM 12M
		 7		: OFDM 18M
		 8		: OFDM 24M
		 9		: OFDM 36M
		10		: OFDM 48M
		11		: OFDM 54M
		12~27		: MCS0~MCS15

	ps. RTL8712S most only support to MCS7,
	    MCS8~MCS15 not be supported.

# setbw bandwidth[0:20MHz 1:40MHz]
	[P1]bandwidth
		0	: 20MHz
		1	: 40MHz

	ps. When test legacy data rate (802.11bg mode),
	    remember to change bandwidth to 20MHz.

# continuoustx mode[1/0(start/stop)]
ex: continuoustx 1 => start continuous tx
	continuoustx 0 => stop continuous tx

# singletx mode[1/0(start/stop)]
  check center frequency
ex: singletx 1 => start single carrier tx
	singletx 0 => stop single carrier tx

# carriertx mode[1/0(start/stop)]
ex: continuoustx 1 => start carrier suppression tx
	continuoustx 0 => stop carrier suppression tx

# pkttx mode[1/0(enable/disable)] cnts len type[0~3] da
	[P1]mode: start or stop packet xmit
	[P2]cnts: option, packet counts [default value:10],
	          value 0 means continues tx, until use "pkttx 0" to stop.
	[P3]len : payload length[default value:10]
	[P4]type: payload type[default value:0]
	          type index => 0:0x00 / 1:0x5a / 2:0xa5 / 3:0xff
	[P5]da  : destination address[default address:0xFFFFFFFFFFFF]

ex: pkttx 1 => pkt tx start by default value
    pkttx 1 100 20 2 0xe04c871122
    pkttx 1 100 20 2 =>send 100*broadcast pkt payload len=20  payload type = 0xa5
    pkttx 0 => pkt tx stop

# pktrx mode[1/0(enable/disable)] antenna[1:A 2:B 3:AB]
	[P1]mode: start or stop packet receive
	[P2]antenna:
		1	: antenna A
		2	: antenna B
		3	: antenna AB
	Before you start pktrx, remember to select "channel" and "bandwidth" first!

ex: pktrx 1 1 => packet rx start and receive packets from antenna A
    pktrx 0 => packet rx stop

ps. After version 2.0.1, pktrx would be a block command.
    Before you pressing any key to exit pktrx, you can't do anything.
    Once you exit pktrx, you don't need to use "pktrx 0" to stop pktrx any more.


	When you go to do some E-Fuse program, you will see

rtl8712> efuse

	  E-Fuse Functions Menu

	===========================================
	  help
	  refraw addr[hex] bytes[1-512]
	  wefraw addr[hex] bytes[1-8] value1[hex] value2 ...
	  refmap	:read EFuse Map from driver
	  wefmap	:write EFuse Map to driver
	  smap		:show EFuse Map
	  rmap addr[hex] bytes[1-128] (LOCAL)
	  wmap addr[hex] bytes[1-8] value1[hex] value2 ... (LOCAL)
	  chmac type[1/2(SDIO/USB)] [MAC Address(xx:xx:xx:xx:xx:xx)]
	  rthermal	:read thermal meter from hardware
	  exit
	===========================================

rtl8712/efuse>

	Each commands deatil is as follow
	Attention! If you write E-Fuse when interface not up,
	may damage E-Fuse content permanently.

	refraw addr[hex] bytes[1-512]
		Read E-Fuse physical address data,

		"addr" is physical address,
		and should be hex value, range is 0x000~0x1FF.

		"bytes" is the amount of bytes to read,
		decimal value, range is 1~512.

	wefraw addr[hex] bytes[1-8] value1[hex] value2 ...
		Read E-Fuse physical address data,

		"addr" is physical address,
		and it should be hex value, range is 0x000~0x1FF.

		"bytes" is the amount of bytes to write,
		decimal value, range is 1~512.

		"valueN" are the data want to be written,
		and all are represent as hex value, 8 bits, valid value is 0x00~0xFF.

		Attention!!
		When use this function, you must very understand what you're doing.
		Because once write the wrong data into E-Fuse,
		the logical map data may be totally corrupted, and can't be repaired.
		That would cause one chip can't be used again.
		So, make sure you really know what you're doing.
		If not necessary, use "wefmap" to write the logical data.

	refmap
		Read whole EFuse Map 128 bytes from driver,
		it will organize physical data to logical map data.
		After "refmap", local will have a temporal backup,
		and can use "smap" to show the temporal backup data.

	wefmap
		Write whole EFuse Map to driver.
		This function will write local temporal backup date to E-Fuse.
		When use this function, must be more careful!
		Make sure you really need to do this,
		because this action can't be recovered,
		and no warning meesage to confirm this actin.

	smap
		Show local EFuse Map backup

	rmap addr[hex] bytes[1-128] (LOCAL)
		Read from local EFuse Map backup.

		"addr" is the start address which want to read,
		and should be hex value, its valid space address is 0x00~0x7F.

		"bytes" is the amount of bytes to write,
		decimal value, range is 1~218.

	wmap addr[hex] bytes[1-8] value1[hex] value2 ... (LOCAL)
		Write to local EFuse Map backup.
		This function won't really write data to E-Fuse,
		so don't need to worry when you write the wrong data,
		just use "refmap" to recover the data.

		"addr" is start write address,
		and it is hex value, valid space address is 0x00~0x7F.

		"bytes" is the amount of bytes to write,
		decimal value, range is 1~512.

		"valueN" are the data want to be written.
		Each data is 8 bits value, so its valid value is 0x00~0xFF.
		You should use hex to represent it.

	chmac type[1/2(SDIO/USB)] MACAddress(xx:xx:xx:xx:xx:xx)
		Write the mac address into local Map backup.

		"type" determine the interface is SDIO or USB,
		The logical address of MAC address is different between SDIO and USB.

		The format of "MACAddress" should be like 01:23:45:67:89:ab.

		After "chmac", you may use "smap" or "rmap" to make sure your
		mac address is correct. The logical address of MAC address in
		E-Fuse is 0x4F for SDIO and 0x12 for USB, both are 6 bytes.

		If the local Map data is correct, remember to use "wefmap" write
		into E-Fuse. Next time mount module, after one time power off,
		the new address will be used.

	rthermal
		Read thermal meter from hardware.
		When you do tx calibration, you also need to record thermal meter at
		that moment, and write it to eFuse.
		Thermal meter will be used in firmware dynamic algorithm to
		adjust power to expected value in different temperture.

	exit
		Return to previous menu, usually is main function selection page.



	The Register Read/Write functions menu page is

rtl8712> reg

	  Register Read/Write Menu

	===========================================
	  help
	  rreg addr[hex] bytes[1/2/4]
	  wreg addr[hex] bytes[1/2/4] val[hex]
	  rrfreg path[0~1] offset[0x00~0xFF]
	  wrfreg path[0~1] offset[0x00~0xFF] val[0x0~0xFFFFFFFF]
	  rbbreg offset[0x800~0xFFF]
	  wbbreg offset[0x800~0xFFF] val[0x0~0xFFFFFFFF]
	  power_down_test mode[1/0(up/down)]
	  exit
	===========================================

rtl8712/reg>

	Commands usage is as follow

# rreg addr[hex] bytes[1/2/4]
  mac register read
ex: rreg 0x10250000 4 or rreg 10250000 4 => read register:10250000 4Bytes

# wreg addr[hex] bytes[1/2/4] val[hex]
  mac register write
ex: wreg 0x10250000 4 0x114ce000 or wreg 10250000 4 114ce000

# rrfreg path[0~1] offset[0x00~0xFF]
  RF register read
ex: rrfreg 0 1 => read rf-reg path:B offset:1

# wrfreg path[0~1] offset[0x00~0xFF] val[0x00000000~0xFFFFFFFF]
  RF register write
ex: wrfreg 1 9 0x387 => write rf reg path:A offset:0x9 value:0x387

# rbbreg offset[0x800~0xFFF]
  base band register read
ex: rbbreg 803 => read base band reg offset:0x803

# wbbreg offset[0x800~0xFFF] val[0x00000000~0xFFFFFFFF]
  base band register write
ex: wbbreg 803 12345678 => write base band reg offset:0x803 value:0x12345678
