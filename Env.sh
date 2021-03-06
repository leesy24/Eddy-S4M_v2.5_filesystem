##
##	Configure for Eddy
##

export TOPMOST_DIR=`pwd`;

## Toolchain Path
##                                                      <<-- Config It !!!
export COMPILER_DIR=/opt/lemonix/cdt

#export PATH=$PATH:$TOPMOST_DIR/tool:$COMPILER_DIR/bin
export PATH=$TOPMOST_DIR/tool:$COMPILER_DIR/bin:$PATH
export CROSS_COMPILE=/opt/lemonix/cdt/bin/arm-linux-
export COMPILER_NAME=arm
export TARGET_BOARD=eddy

#USER_ID00=`whoami`
#if [ "$USER_ID00" == "root" ]; then
#	echo -e "\033[0m "
#	echo -e "\033[1m \033[32m Please !!"
#	echo -e "\033[1m \033[32m Login to other user..";
#	echo -e "\033[0m "
#	exit;
#fi

## Package Version
##                                                      <<-- Config It !!!
export PKG_VERSION=21a

## FOR ROOT FILESYSTEM
##                                                      <<-- Config It !!!
export ROOT_FS_SKEL=$TOPMOST_DIR/ramdisk/root_skel.tar.gz
export RAMDISK_DIR=$TOPMOST_DIR/ramdisk
export ROOT_FS_DIR=$TOPMOST_DIR/ramdisk/root

# FOR CONFIG FILESYSTEM
##                                                      <<-- Config It !!!
export CONFIG_FS_DIR=$TOPMOST_DIR/ramdisk/flash


## FOR BOOTLOADER
##                                                      <<-- Config It !!!
export BOOT_VERSION=21a
export BOOTST_DIR=$TOPMOST_DIR/bootstrap_20d 
export BOOTLR_DIR=$TOPMOST_DIR/bootloader_21b

## FOR OS
##                                                      <<-- Config It !!!
export OS_VERSION=220
export OS_DIR=$TOPMOST_DIR/lemonix
#export OS_DIR=$TOPMOST_DIR/lemonix-rt
export OS_INCLUDE_DIR=${OS_DIR}/include

## FOR APPLICATION
##                                                      <<-- Config It !!!
export APP_VERSION=21a
export APP_DIR=$TOPMOST_DIR/src

## FOR WEB
##                                                      <<-- Config It !!!
export WEB_VERSION=20a
export WEB_DIR=$TOPMOST_DIR/web


## FOR FIRMWARE
export FIRMWARE_DIR=$TOPMOST_DIR/firmware


## ETC
##																											<<-- 
#export SUDO_CMD=/usr/bin/sudo
export SUDO_CMD=
export MKIMAGE_CMD=$TOPMOST_DIR/tool/mkimage
export EXT2_CMD=$TOPMOST_DIR/tool/genext2fs
export JFFS2_CMD=$TOPMOST_DIR/tool/mkfs.jffs2
export CRAMFS_CMD=$TOPMOST_DIR/tool/mkcramfs


## Print Current Environment
##
echo ""
echo -e "		\033[9m \033[34m[ $TARGET_BOARD's Current Envirionment ]"
echo -ne "\033[1m \033[32m ------------------------------------------------------------"
echo -e "\033[0m "
echo "  CROSS_COMPILE=arm-linux-"
echo "  COMPILER_DIR=${COMPILER_DIR}"
echo "  User ID = `whoami`"
echo "  Date = `date`"
echo ""
echo "  PKG_VERSION=${PKG_VERSION}"
echo "  BOOT_VERSION=${BOOT_VERSION}"
echo "  OS_VERSION=${OS_VERSION}"
echo -e "\033[1m \033[32m ------------------------------------------------------------"
echo ""
echo -e "\033[1m \033[33m>>>>> Good Luck to $USER_ID00 !! "
echo -e "\033[0m "
echo ""
