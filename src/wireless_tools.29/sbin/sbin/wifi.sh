#!/bin/sh
echo "WiFi shell start=>$0"
echo "copy libiw* to lib$0"
cp ./libiw.so* /lib/
echo "copy wifi tool to sbin$0"
cp ./iw* /sbin/
