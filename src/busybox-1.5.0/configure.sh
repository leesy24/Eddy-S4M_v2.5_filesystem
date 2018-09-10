#!/bin/bash

cp config.${TARGET_BOARD} .config
make oldconfig 

