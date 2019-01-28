#!/bin/sh

# description:
# this is just a simple script to find the ip address, the serial divice and run the controller application 
# 

SERIALDEVICE=/dev/ttyUSB0
ROBOTMACADDRESS="b8:27:eb:52:17:b3"
ROBOTIPADDRESS=$(arp -n | grep "b8:27:eb:52:17:b3" | awk '{print $1}')
RUSTCONTROLLER=~/rusty_bot/ArController/target/debug/ArController
# the controller line will need to changed currently this needs to be built on the raspberry zero due to xbuntu's linker

if [ ! -e $SERIALDEVICE ]; then
	echo "serial device does not exist: $SERIALDEVICE"
else
	if [ -f $RUSTCONTROLLER ]; then
		echo "running executable"
		$($RUSTCONTROLLER $SERIALDEVICE $ROBOTIPADDRESS)

	elif [ $(cargo run $SERIALDEVICE $ROBOTIPADDRESS) ]; then
		echo "running with cargo"

	else
		echo "unable to run controller"
	fi
fi
