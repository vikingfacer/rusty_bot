piAccessPoint="AlexaJones"
piAccessPassw="floridefrogs"
country="US"
LIST_OF_APPS="dnsmasq hostapd"


default_hostapd_CONF="test.txt"
hostapd_CONF="text.txt1"
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


dhcpcd_CONF_lines="interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant"


dhcpcd_CONF="text.txt"


for a in $LIST_OF_APPS
do 
	echo $a 
done

# editing the hostapd.conf
# echo "#generated $(date) from $(pwd) $BASH_SOURCE">> $hostapd_CONF
# echo -e "${hostapd_CONF_lines}" >> $hostapd_CONF
# sed -i -e 's;#DAEMON_CONF="";DAEMON_CONF="/etc/hostapd/hostapd.conf";g' $default_hostapd_CONF


userBin="/usr/bin/"
autohotspotScript="autohotspotN"

echo $userBin$autohotspotScript


echo "check these locations:"

for i in ${all_LOCATIONS[@]}; do echo "$i"; done

funBackUp(){
	# pass the file being edited
	echo "backing up file: $1"
	cp $1 $1.orig
	return 1
}

funGenConfig(){
	# pass the file being edited, and the payload
	echo "#generated $(date) from $(pwd) $BASH_SOURCE">> $1
	echo -e "{$2}" >> $1
	return 1
}



funBackUp $hostapd_CONF

funGenConfig $hostapd_CONF "$hostapd_CONF_lines"
wifi_SSID="netwoke"
wifi_PASS="fart"

wpa_supplicant_lines="ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=GB
"
network1="network={
	ssid="$wifi_SSID"
	psk="$wifi_PASS"
	key_mgmt=WPA-PSK
}"

wpa_supplicant_lines=$wpa_supplicant"${network1}"
echo "$wpa_supplicant_lines"


all_LOCATIONS="$hostapd_CONF $default_hostapd_CONF $dnsmasq_CONF $interfaces $dhcpcd_CONF"
echo $all_LOCATIONS

for a in ${all_LOCATIONS}; 
do
	echo $a
done
