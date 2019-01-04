#!/bin/sh -e

# added interfaces from https://www.raspberrypi.org/forums/viewtopic.php?t=26795


# purpose of this script is to setup alexa jones hotspot if wifi is not present 
# dhcp hostapd iw are installed by this
# back up files are created by default 

# to add anther network simply add anther wifi SSID and pass copy network1 into a new 
# variable called network2 with the new wifi SSID and pass 
# and appened network2 to the wpa_supplicant_lines as network1 has been added 

# wifi network ssid
wifi_SSID=""
wifi_PASS=""

wpa_supplicant="/etc/wpa_supplicant/wpa_supplicant.conf"
wpa_supplicant_lines="ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=GB
"
network1="network={
	ssid="$wifi_SSID"
	psk="$wifi_PASS"
	key_mgmt=WPA-PSK
}"

wpa_supplicant_lines=$wpa_supplicant"$network1"


##################################
#### HOSTAPD CONFIGURATIONS ######
##################################

piAccessPoint="AlexaJones"
piAccessPassw="floridefrogs"
country="US"
LIST_OF_APPS="dnsmasq hostapd iw"

hostapd_CONF="/etc/hostapd/hostapd.conf"
hostapd_CONF_lines="
#2.4GHz setup wifi 80211 b,g,n
interface=wlan0
driver=nl80211
ssid=$piAccessPoint
hw_mode=g
channel=8
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=$piAccessPassw
wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP TKIP
rsn_pairwise=CCMP

#80211n - Change GB to your WiFi country code
country_code=$country
ieee80211n=1
ieee80211d=1
 "

default_hostapd_CONF="/etc/default/hostapd"


##################################
#### DNSMASQ CONFIGURATIONS ######
##################################

dnsmasq_CONF="/etc/dnsmasq.conf"
dnsmasq_CONF_lines="#AutoHotspot config
interface=wlan0
bind-dynamic 
server=8.8.8.8
domain-needed
bogus-priv
dhcp-range=192.168.50.150,192.168.50.200,255.255.255.0,12h"

##################################
#### Interfaces CONFIGURATIONS ###
##################################

interfaces="/etc/network/interfaces"
sysctl="/etc/sysctl.conf"

##################################
#### DHCPCD CONFIGURATIONS ######
##################################

dhcpcd_CONF="/etc/dhcpcd.conf"
dhcpcd_CONF_lines="nohook wpa_supplicant"


##################################
#### AutoHopSpot CONFIGURATIONS ##
##################################

autohotspot="/etc/systemd/system/autohotspot.service"
autohotspot_lines="[Unit]
Description=Automatically generates an internet Hotspot when a valid ssid is not in range
After=multi-user.target
[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/usr/bin/autohotspotN
[Install]
WantedBy=multi-user.target"


##################################
#### autohotspotN Script Setup ###
##################################

userBin="/usr/bin/"
autohotspotScript="autohotspotN"

all_LOCATIONS="$hostapd_CONF $default_hostapd_CONF $dnsmasq_CONF $interfaces $dhcpcd_CONF"


funBackUp(){
	# pass the file being edited
	echo "backing up file: $1"
	if [ -e "$1" ]; 
	then
		echo "file exists backing up: $1"
		cp $1 $1.orig
	else
		echo "file does not exist: $1"
	fi
}

funGenConfig(){
	# pass the file being edited, and the payload
	echo "#generated $(date) from $(pwd) $BASH_SOURCE">> $1
	echo -e "${2}" >> $1
}


sudo -n true

echo "update"
apt-get -y update
echo "upgrade"
apt-get -y upgrade
echo "installing list of applications"
apt-get -y install $LIST_OF_APPS

echo "stopping the list of newly installed applications"
for a in $LIST_OF_APPS
do 
	echo "....systemctl stop:" $a
	if systemctl disable $a; 
	then 
		echo "disabling $a succeeded"
	else
		echo "disabling $a failed"
	fi
done

echo "moving on to editing files now"


#### editing hostapd_CONF 
echo "editing hostapd_CONF $hostapd_CONF"
# editing the hostapd.conf
funBackUp $hostapd_CONF
funGenConfig $hostapd_CONF "$hostapd_CONF_lines"

#### editing default_hostapd_CONF 
echo "editing default_hostapd_CONF $default_hostapd_CONF"
funBackUp $default_hostapd_CONF
sed -i -e 's;#DAEMON_CONF="";DAEMON_CONF="/etc/hostapd/hostapd.conf";g' $default_hostapd_CONF


#### editing dnsmasq_CONF
echo "editing dnsmasq_CONF $dnsmasq_CONF"
funBackUp $dnsmasq_CONF
funGenConfig $dnsmasq_CONF "$dnsmasq_CONF_lines"


#### editing interfaces 
# backing up file
funBackUp $interfaces

# editing ip forwarding sysctl.conf
sed -i -e 's;#net.ipv4.ip_forward=1;net.ipv4.ip_forward=1;g' $sysctl


# Configuring the DHCP server (dnsmasq)
echo "editing dhcpcd_CONF $dhcpcd_CONF"
funBackUp $dhcpcd_CONF
funGenConfig $dhcpcd_CONF "$dhcpcd_CONF_lines"

echo "editing autohotspot $autohotspot"
funBackUp $autohotspot
funGenConfig $autohotspot "$autohotspot_lines"


if systemctl enable autohotspot.service; 
then
	echo "enable autohotspot.service worked"
else
	echo "enable autohotspot.service failed"
fi


# autohotspotN Script
if [ -f $userBin$autohotspotScript ]; 
then
    echo "File: $userBin$autohotspotScript found!"
else
    echo "File: $userBin$autohotspotScript does not exist!"

	if [ -e "./$autohotspotScript" ]; 
	then
		echo "File $autohotspotScript found, copying it to the right place"
		cp $autohotspotScript $userBin$autohotspotScript
	else
		echo "File: $autohotspotScript was not copied with this script, now exiting"
		exit 
	fi
fi

sudo chmod +x $userBin$autohotspotScript


# finishing up 

echo "check these locations:"
for a in ${all_LOCATIONS}; 
do
	echo $a
done


echo "add /usr/bin/autohotspotN to rc.local"
echo "before exit 0"
echo "add */5 * * * * sudo /usr/bin/autohotspot"
echo "to crontab with crontab -e"

echo "restart if satified"
