#!/bin/bash -x
#    CC=${COMPILER_DIR}/bin/${CROSS_COMPILE}gcc 
#
#./configure  \
#  --exec-prefix=${ROOT_FS_DIR} \
#  --prefix=${ROOT_FS_DIR} 
#
    #--host=arm-unkown-linux \
    #--build=i686-redhat-linux \


export CC=${COMPILER_DIR}/bin/${CROSS_COMPILE}gcc 
export CROSS_COMPILE=arm-linux-

CC=arm-linux-gcc
./configure --exec-prefix=${ROOT_FS_DIR}/usr/local --with-c-compiler=${CC}

