#! /bin/sh

while true; do
	/sbin/getty -n -L -l /flash/run_sh.sh ttyS1 115200 linux-m1b
	sleep 1
done

