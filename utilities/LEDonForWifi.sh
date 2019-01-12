#!/bin/sh

#  this is a start up script to show that the robot is on 
#  it also leaves on an led to show what network it is on

IP="127.0.0.1" #no place like home
PORT="8080"
LEDS="22 24 26 28 30 32 34"
S_CONST=1
wpassid=$(awk '/ssid="/{ print $0 }' /etc/wpa_supplicant/wpa_supplicant.conf | awk -F'ssid=' '{ print $2 }' ORS=',' | sed 's/\"/''/g' | sed 's/,$//')
HOMENETWORK="LittleMountain2.4Hz1"
ADHOCNETWORK="AlexaJones"



LED_Action () {
	#  $1 is the pin number on the arduino
	#  $2 is the action on or off
	if [ $2 = "on" ] || [ $2 = "On" ] || [ $2 = "ON" ]; then
		echo "S d $1 w 1 \n\0"
	elif [ $2 = "off" ] || [ $2 = "Off" ] || [ $2 = "OFF" ];then
		echo "S d $1 w 0\n\0"
	fi
}

# turn on all LEDS
for a in ${LEDS}; 
do
	echo "$a"
	echo "$(LED_Action $a "ON")" | nc $IP $PORT -w 0
	# echo "\n\n\n\0" | nc $IP $PORT -w 0
	sleep $S_CONST
	echo "$(LED_Action $a "ON")" | nc $IP $PORT -w 0
	sleep $S_CONST

done
sleep 2

# turn off all LEDS
for a in ${LEDS}; 
do
	echo "$(LED_Action $a "OFF")" | nc $IP $PORT -w 0
	sleep $S_CONST
	echo "\n\n\n\0" | nc $IP $PORT -w 0
	echo "$(LED_Action $a "OFF")" | nc $IP $PORT -w 0
	sleep $S_CONST

done

# sleep 2

# for a in ${LEDS}; 
# do
# 	echo "$(LED_Action $a "OFF")" | nc $IP $PORT -w 0
# 	sleep $S_CONST
# 	echo "$a"
# 	echo "$(LED_Action $a "OFF")" | nc $IP $PORT -w 0
# done
sleep 2
echo "current network $wpassid"

if [ $wpassid = $HOMENETWORK ]; then
	echo "$(LED_Action "28" "ON")" | nc $IP $PORT -w 0
	sleep $S_CONST
	echo "$(LED_Action "28" "ON")" | nc $IP $PORT -w 0	
elif [ systemctl -q hostapd is-active ] && [ systemctl -q dnsmasq is active ]; then
	echo "$(LED_Action "26" "ON")" | nc $IP $PORT -w 0
	sleep $S_CONST
	echo "$(LED_Action "26" "ON")" | nc $IP $PORT -w 0

fi
