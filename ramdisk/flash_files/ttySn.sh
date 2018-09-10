#! /bin/sh

while true; do
	/sbin/getty -n -L -l /bin/sh ttyS1 115200 vt220
	sleep 1
done

