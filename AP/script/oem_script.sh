#!/bin/sh
IFstatus()
{
if [ "`ifconfig | grep $1 | wc -l`" = 0 ]; then
echo "down"
else
echo "up"
fi
}
TimePointNow()
{
WEEK=`date +%w`
YEAR=`date +%Y`
MON=`date +%m`
DAY=`date +%d`
HOUR=`date +%H`
MINUTE=`date +%M`
CurrentWeek=`date +%w`
CurrentWeek=`expr $CurrentWeek \* 24`
CurrentWeek=`expr $CurrentWeek \* 60`
CurrentHour=`date +%H`
CurrentHour=`expr $CurrentHour \* 60`
CurrentMin=`date +%M`
CurrentTimePoint=`expr $CurrentWeek + $CurrentHour`
CurrentTimePoint=`expr $CurrentTimePoint + $CurrentMin` # system current time point(min)
echo "$CurrentTimePoint"
}
mssid_control()
{
num=1
while [ $num -le $1 ]
do
if [ "$2" = "Aband" ]; then
IFup=`flash get INIC_SSID_MIRROR_$num | cut -d "=" -f 2`
else
IFup=`flash get SSID_MIRROR_$num | cut -d "=" -f 2`
fi
if [ "$IFup" != "0" ]; then
if [ "$2" = "Aband" ]; then
ifconfig rai$num $3 2> /dev/null
else
ifconfig ra$num $3 2> /dev/null
fi
fi
num=`expr $num + 1`
done
}
switchLocation()
{
if [ "$_WIFI_REPEATER_" = "y" ]; then
switchLocation=`cat /proc/MODE_SW`
case "$switchLocation" in
"0")
echo "NormalMode"
;;
"2")
echo "sleepMode"
;;
"6")
echo "greenMode"
;;
esac
else
echo "NormalMode"
fi
}
GetUplinkRssi()
{
uplink_signal=-100
if [ "`IFstatus $1`" = "up" ]; then
rootAPmac="`iwconfig $1 | grep Mode | tr -s " " | cut -f6 -d ' '`"
if [ "$rootAPmac" != "Not-Associated" ]; then
uplink_signal="`iwpriv $1 stat | grep RSSI| tr -s " " | cut -f 3 -d ' '`"
fi	
fi
echo "$uplink_signal"
}
GetLinkStat()
{
LinkStat=`mii_mgr -g -p $1 -r $2 | cut -d= -f 2 | tr -d "[ *]"`
if [ "$_MODEL_" = "BR6278AC" -a "$LinkStat" = "7869" ]; then #Retry
LinkStat=`mii_mgr -g -p $1 -r $2 | cut -d= -f 2 | tr -d "[ *]"`
fi
LinkStat=`echo $LinkStat | cut -b 4-`
case "$LinkStat" in
"d")
echo "connect"
;;
"9")
echo "disconnect"
;;
esac
}
switch_detect ()
{
sw1=0
if [ "$_WIRELESS_SWITCH_" = "y" ];then
sw1=`cat /dev/switch_but`
if [ "$_WIRELESS_SWITCH_REVERSE_" = "y" ]; then
sw1=`expr 1 - $sw1`
fi
if [ "$_WEB_WL_SW_" != "y" ]; then
sw2=`flash get WLAN_DISABLED | cut -d= -f2`
if [ $sw1 = '0' -a $sw2 != '0' ];then
flash set WLAN_DISABLED 0
flash set INIC_DISABLED 0
elif [ $sw1 = '1' -a $sw2 != '1' ];then
flash set WLAN_DISABLED 1
flash set INIC_DISABLED 1
fi
fi
fi
if [ "$_SW_WIRELESS_SWITCH_" = "y" ];then
sw1=`flash all | grep AP_ROUTER_SWITCH | cut -f 2 -d =`
fi
case "$1" in
"ap_router")
sw1=0
;;
"wireless")
;;
"switch_but")
;;
esac
return $sw1
}
oem_led()
{
case "$1" in
"init")
if [ "$_MODEL_" = "Customer" ]; then
echo "LED OFF" > /dev/ABAND_LED
echo "LED OFF" > /dev/GBAND_LED
if [ "$_USB_SERVER_" = 'y' ]; then
echo "LED OFF" > /dev/USB_LED0
if [ "$USB_ENABLED" = "1" ]; then
echo "LED OFF" > /dev/USB_LED1
else
echo "LED ON" > /dev/USB_LED1
fi
fi
echo "LED BLINK 500" > /dev/PowerLED
echo "LED OFF" > /dev/WLAN_LED
echo "LED OFF" > /dev/WPS_LED	
elif [ "$_MODEL_" = "BR6278AC" ]; then		
echo "LED OFF" > /dev/ABAND_LED
echo "LED OFF" > /dev/GBAND_LED
echo "LED OFF" > /dev/NewFW_LED
echo "LED OFF" > /dev/OpenVPN_LED
echo "LED BLINK 500" > /dev/Internet_LED
elif [ "$_WIFI_REPEATER_" = "y" ]; then
echo "LED BLINK 1000" > /dev/ABAND_LED
echo "LED BLINK 1000" > /dev/GBAND_LED
echo "LED OFF" > /dev/WlanLED
echo "LED BLINK 1000" > /dev/PowerLED
echo "LED OFF" > /dev/WPS_LED
echo "LED OFF" > /dev/LAN_LED
fi
;;
"init_done")
if [ "$_MODEL_" = "Customer" ]; then
echo "LED ON" > /dev/WPS_LED	#EDX yihong, It's PowerLED
echo "LED BLINK 500" > /dev/PowerLED #EDX yihong, It's InternetLED	
fi
run_led=yes
if [ "$_MODEL_" = "EW7478AC" -a "$WIZ_MODE" = "1" ]; then
run_led=no
fi
if [ "$run_led" = "yes" ]; then
if [ "$_WIFI_ROMAING_" = "y" ]; then
if [ "$WPS_CONFIG_STATUS" = "34" ] || [ "$INIC_WPS_CONFIG_STATUS" = "34" ]; then
echo "LED ON" > /dev/WPS_LED
flash set INIC_WPS_CONFIG_STATUS 0
flash set WPS_CONFIG_STATUS 0
fi
fi
/bin/led.sh &
fi
;;
"wps")
if [ "$_MODEL_" = "Customer" ]; then
WPS_LED="/dev/ABAND_LED"
elif [ "$_MODEL_" = "BR6278AC" ]; then
WIFI_24G_LED=/dev/GBAND_LED
WIFI_5G_LED=/dev/ABAND_LED
else
WPS_LED="/dev/WPS_LED"
AbandLED="/dev/ABAND_LED"
GbandLED="/dev/GBAND_LED"
AbandOff=`cat /tmp/DisableABAND`
GbandOff=`cat /tmp/DisableGBAND`
fi
if [ "$2" = "wps_init" ]; then
killall led.sh 2> /dev/null
if [ "$_MODEL_" = "Customer" -a "$WIZ_MODE" = "$APMode" ]; then
echo "LED OFF" > $AbandLED
echo "LED OFF" > $GbandLED
fi
if [ "$3" = "Aband" ]; then
echo 1 > /tmp/runningWPS5g
fi
if [ "$3" = "Gband" ]; then
echo 1 > /tmp/runningWPS
fi
if [ "$_WIFI_ROMAING_" = "y" ]; then
if [ "$IQSET_DISABLE" = "0" ]; then
if [ "$3" = "Aband" ]; then
echo 1 > /tmp/WPS5g
fi
if [ "$3" = "Gband" ]; then
echo 1 > /tmp/WPS24g
fi
else
/bin/led.sh &
fi
else
/bin/led.sh &
fi
if [ "$_MODEL_" = "Customer" ]; then
if [ "$_USB_SERVER_" != "" ]; then
killall probeUSB 2> /dev/null
fi
echo "LED BLINK 500" > $WPS_LED
elif [ "$_MODEL_" = "BR6278AC" ]; then
if [ "$3" = "Gband" ]; then
echo "LED BLINK 500" > $WIFI_24G_LED
fi
if [ "$3" = "Aband" ]; then
echo "LED BLINK 500" > $WIFI_5G_LED
fi
else
if [ "`switchLocation`" != "NormalMode" ]; then
echo "LED OFF" > $WPS_LED
rm -f /tmp/runningWPS5g 2> /dev/null
rm -f /tmp/runningWPS 2> /dev/null
return 0;
else
echo "LED BLINK 1000" > $WPS_LED
if [ "$_MODEL_" = "Customer" -a "$WIZ_MODE" = "$APMode" ]; then 
if [ "$3" = "Aband" ]; then
echo "LED BLINK 3000" > $AbandLED
fi
if [ "$3" = "Gband" ]; then
echo "LED BLINK 3000" > $GbandLED
fi
fi
fi
fi
elif [ "$2" = "wps_done" ]; then	
killall led.sh 2> /dev/null
if [ "$_MODEL_" = "Customer" -a "$WIZ_MODE" = "$APMode" ]; then
echo "LED OFF" > $AbandLED
echo "LED OFF" > $GbandLED
fi
if [ "$3" = "Aband" ]; then
rm -f /tmp/runningWPS5g 2> /dev/null
fi
if [ "$3" = "Gband" ]; then
rm -f /tmp/runningWPS 2> /dev/null
fi
/bin/led.sh &
killall wpstool 2> /dev/null
if [ "$_MODEL_" = "Customer" ]; then
if [ "$WLAN_DISABLED" = '1' -a "$INIC_DISABLED" = '1' ]; then
echo "LED OFF" > $WPS_LED
else
echo "LED ON" > $WPS_LED
fi
elif [ "$_MODEL_" = "BR6278AC" ]; then
if [ "$WLAN_DISABLED" = "1" ]; then
echo "LED OFF" > $WIFI_24G_LED
else
echo "LED ON" > $WIFI_24G_LED
fi
if [ "$INIC_DISABLED" = "1" ]; then
echo "LED OFF" > $WIFI_24G_LED
else
echo "LED ON" > $WIFI_24G_LED
fi
else
if [ "`cat $WPS_LED`" != "LED ON" ]; then # WPS fail
echo "LED OFF" > $WPS_LED
fi
fi
fi
;;
"wireless")
if [ "$_MODEL_" = "Customer" ]; then
if [ "$WLAN_DISABLED" = '0' ]; then
echo "LED ON" > /dev/GBAND_LED
echo "0" > /tmp/DisableGBAND
else	
echo "LED OFF" > /dev/GBAND_LED
echo "1" > /tmp/DisableGBAND
fi
if [ "$INIC_DISABLED" = '0' ]; then
echo "LED ON" > /dev/ABAND_LED
echo "0" > /tmp/DisableABAND
else	
echo "LED OFF" > /dev/ABAND_LED
echo "1" > /tmp/DisableABAND
fi
elif [ "$_MODEL_" = "BR6278AC" ]; then
if [ "$WLAN_DISABLED" = "0" ]; then
echo "LED ON" > /dev/GBAND_LED
echo "0" > /tmp/DisableGBAND
else	
echo "LED OFF" > /dev/GBAND_LED
echo "1" > /tmp/DisableGBAND
fi
if [ "$INIC_DISABLED" = "0" ]; then
echo "LED ON" > /dev/ABAND_LED
echo "0" > /tmp/DisableABAND
else	
echo "LED OFF" > /dev/ABAND_LED
echo "1" > /tmp/DisableABAND
fi
else
if [ "$INIC_DISABLED" = 1 ]; then
ifconfig rai0 down
echo "1" > /tmp/DisableABAND # for site survey in wiz page 
else
echo "0" > /tmp/DisableABAND
fi
if [ "$WLAN_DISABLED" = 1 ]; then
ifconfig ra0 down
echo "1" > /tmp/DisableGBAND # for site survey in wiz page 
else
echo "0" > /tmp/DisableGBAND
fi			
fi
;;
"usb_server")
if [ "$DEV_PATH" == "" ]; then
echo "LED OFF" > /dev/WPS_LED
else
echo "LED ON" > /dev/WPS_LED
fi
;;
"uplink_led")
if [ "$2" = "apcli0" ]; then
if [ -f  /tmp/runningWPS ]; then
return 1;
fi
UPLINK_LED=/dev/GBAND_LED
elif [ "$2" = "apclii0" ]; then
if [ -f  /tmp/runningWPS5g ]; then
return 1;
fi
UPLINK_LED=/dev/ABAND_LED
else
echo "oem_led uplink_led apcli0|apclii0"
return 1;
fi
if [ "$_MODEL_" = "Customer" ] && [ "`switchLocation`" != "NormalMode" -o "`IFstatus $2`" = "down" ]; then
echo "LED OFF" > $UPLINK_LED
return 0;
fi
if [ "$_MODEL_" = "EW7478AC" ] && [ "`switchLocation`" = "sleepMode" -o "`IFstatus $2`" = "down" ]; then
echo "LED OFF" > $UPLINK_LED
return 0;
fi
Rssi_strength=`GetUplinkRssi $2`
if [ "$_MODEL_" = "EW7478AC" ]; then
if [ $Rssi_strength -ge -50 ]; then   # Rssi  >= -50dbm
transfer_strength=100
elif [ $Rssi_strength -gt -80 ]; then # Rssi between -51 ~ -79dbm
transfer_strength=$(( ( ( (Rssi_strength + 80) * 26 ) / 10 ) + 24 ))
elif [ $Rssi_strength -gt -90 ]; then # Rssi between -80 ~ -89dbm
transfer_strength=$(( ( (Rssi_strength + 90) * 26 ) / 10 ))
else 
transfer_strength=0
fi
if [ $transfer_strength -ge 80 ]; then
if [ "`cat $UPLINK_LED`" != "LED BLINK 2000" ]; then
echo "LED BLINK 2000" > $UPLINK_LED
fi
elif [ $transfer_strength -ge 60 ]; then # -66 ~ -58
echo "LED ON" > $UPLINK_LED
elif [ $transfer_strength -ge 40 ]; then
if [ "`cat $UPLINK_LED`" != "LED BLINK 2000" ]; then
echo "LED BLINK 2000" > $UPLINK_LED
fi
elif [ $transfer_strength -gt 0 ]; then
if [ "`cat $UPLINK_LED`" != "LED BLINK 500" ]; then
echo "LED BLINK 500" > $UPLINK_LED
fi
else
echo "LED OFF" > $UPLINK_LED
fi
else
if [ $Rssi_strength -ge -60 ]; then
echo "LED ON" > $UPLINK_LED
elif [ $Rssi_strength -lt -60 -a $Rssi_strength -gt -85 ]; then
if [ "`cat $UPLINK_LED`" != "LED BLINK 3000" ]; then
echo "LED BLINK 3000" > $UPLINK_LED
fi
else
echo "LED OFF" > $UPLINK_LED
fi
fi
;;
"phylink_led")
LAN_LED=/dev/LAN_LED
if [ "$2" != " " -a "$3" != " " ]; then
phyNumber=$2
registerNumber=$3
else
echo "oem_led phylink_led 'phy number' 'register number'"
return 1;
fi
if [ "`switchLocation`" = "sleepMode" -o "`GetLinkStat $phyNumber $registerNumber`" = "disconnect" ]; then
echo "LED OFF" > $LAN_LED
return 0;
fi
if [ "`GetLinkStat $phyNumber $registerNumber`" = "connect" ]; then
echo "LED ON" > $LAN_LED
fi
;;
"crossBand_led")
CROSSBAND_LED=/dev/WlanLED
if [ "$_MODEL_" = "Customer" ] && [ "`switchLocation`" != "NormalMode" ]; then
echo "LED OFF" > $CROSSBAND_LED
return 0;
fi
if [ "$_MODEL_" = "EW7478AC" ] && [ "`switchLocation`" = "sleepMode" ]; then
echo "LED OFF" > $CROSSBAND_LED
return 0;
fi
if [ "$2" = "ON" ]; then
echo "LED ON" > $CROSSBAND_LED
else
echo "LED OFF" > $CROSSBAND_LED
fi
;;
"wlan_led")
if [ "$_MODEL_" = "Customer" ]; then
WLAN_LED=/dev/PowerLED #EDX yihong, It's InternetLED
elif [ "$_MODEL_" = "BR6278AC" ]; then
WLAN_LED=/dev/Internet_LED
else
WLAN_LED=/dev/WlanLED
fi
if [ "$2" != "apcli0" -o "$3" != "apclii0" ]; then
echo "oem_led wlan_led apcli0 apclii0"
return 1;
fi
if [ "`switchLocation`" != "NormalMode" ] || [ "`IFstatus $2`" = "down" -a "`IFstatus $3`" = "down" ]; then
echo "LED OFF" > $WLAN_LED
return 0;
fi
if [ "`IFstatus $2`" = "up" -a "`IFstatus $3`" = "up" ]; then
GRssi_strength=`GetUplinkRssi $2`
ARssi_strength=`GetUplinkRssi $3`
if [ "$GRssi_strength" -ge -65 -a "$ARssi_strength" -ge -65 ]; then
if [ "$_MODEL_" = "Customer" ]; then
echo "LED ON" > $WLAN_LED
elif [ "$_MODEL_" = "BR6278AC" ]; then
echo "LED ON" > $WLAN_LED
else
echo "LED OFF" > $WLAN_LED
fi
elif [ "$GRssi_strength" -lt -85 -o "$ARssi_strength" -lt -85 ]; then
if [ "$_MODEL_" = "Customer" ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 1000" ]; then
echo "LED BLINK 1000" > $WLAN_LED
fi
elif [ "$_MODEL_" = "BR6278AC" ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 1000" ]; then
echo "LED BLINK 1000" > $WLAN_LED
fi
else
echo "LED ON" > $WLAN_LED
fi
elif [ "$GRssi_strength" -lt -65 -a "$ARssi_strength" -lt -65 ]; then
if [ "$_MODEL_" = "Customer" ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 2000" ]; then
echo "LED BLINK 2000" > $WLAN_LED
fi
elif [ "$_MODEL_" = "BR6278AC" ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 2000" ]; then
echo "LED BLINK 2000" > $WLAN_LED
fi
else
if [ "`cat $WLAN_LED`" != "LED BLINK 1000" ]; then
echo "LED BLINK 1000" > $WLAN_LED
fi
fi
elif [ "$GRssi_strength" -lt -65 -o "$ARssi_strength" -lt -65 ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 3000" ]; then
echo "LED BLINK 3000" > $WLAN_LED
fi
fi
else
Rssi_strength=-100
if [ "`IFstatus $2`" = "up" ]; then
Rssi_strength=`GetUplinkRssi $2`
else
Rssi_strength=`GetUplinkRssi $3`
fi
if [ "$Rssi_strength" -ge -65 ]; then
echo "LED OFF" > $WLAN_LED
elif [ "$Rssi_strength" -lt -85 ]; then
echo "LED ON" > $WLAN_LED
elif [ "$Rssi_strength" -lt -65 ]; then
if [ "`cat $WLAN_LED`" != "LED BLINK 3000" ]; then
echo "LED BLINK 3000" > $WLAN_LED	
fi
fi
fi
;;
"power_led")
POWER_LED=/dev/PowerLED
if [ "$_MODEL_" = "Customer" ] && [ "`switchLocation`" != "NormalMode" ]; then
echo "LED OFF" > $POWER_LED
elif [ "$_MODEL_" = "EW7478AC" ] && [ "`switchLocation`" = "sleepMode" ]; then
echo "LED OFF" > $POWER_LED
else
echo "LED ON" > $POWER_LED
fi
;;
"AGband_led")
if [ "$_MODEL_" = "EW7478AC" -a "$WIZ_MODE" = "1" ]; then
sleep 3	#Fixed EW7478AC AP mode WiFi LED detect fail bug.
fi
ABANDLED=/dev/ABAND_LED
GBANDLED=/dev/GBAND_LED
if [ "$_MODEL_" = "Customer" ] && [ "`switchLocation`" != "NormalMode" ]; then
echo "LED OFF" > $ABANDLED
echo "LED OFF" > $GBANDLED			
elif [ "$_MODEL_" = "EW7478AC" ] && [ "`switchLocation`" = "sleepMode" ]; then
echo "LED OFF" > $ABANDLED
echo "LED OFF" > $GBANDLED
else
if [ "$_IQv2_" = "y" ] && [ "$IQSET_DISABLE" = "0" ]; then				
if [ "$_MODEL_" = "EW7478AC" ] || [ ! -f  /tmp/runningWPS5g ]; then
if [ "`cat $ABANDLED`" !=  "LED BLINK 1000" ]; then
echo "LED BLINK 1000" > $ABANDLED
fi
fi
if [ "$_MODEL_" = "EW7478AC" ] || [ ! -f  /tmp/runningWPS ]; then
if [ "`cat $GBANDLED`" !=  "LED BLINK 1000" ]; then
echo "LED BLINK 1000" > $GBANDLED
fi
fi
else			
if [ "$WIZ_MODE" = "0" -o "$WIZ_MODE" = "1" ]; then
if [ "$_MODEL_" = "EW7478AC" ] || [ ! -f  /tmp/runningWPS ]; then
if [ "`IFstatus ra0`" = "up" -a "`cat $GBANDLED`" != "LED ON" ]; then
echo "LED ON" > $GBANDLED							
fi
if [ "`IFstatus ra0`" = "down" -a "`cat $GBANDLED`" != "LED OFF" ]; then
echo "LED OFF" > $GBANDLED									
fi
fi
if [ "$_MODEL_" = "EW7478AC" ] || [ ! -f  /tmp/runningWPS5g ]; then
if [ "`IFstatus rai0`" = "up" -a "`cat $ABANDLED`" != "LED ON" ]; then
echo "LED ON" > $ABANDLED								
fi
if [ "`IFstatus rai0`" = "down" -a "`cat $ABANDLED`" != "LED OFF" ]; then
echo "LED OFF" > $ABANDLED								
fi
fi
fi
fi
fi
;;
esac
}
oem_sku()
{
if [ "$_GBAND_SKU_ENABLE_" = "y" ]; then
rm -rf /etc/Wireless/RT2860AP
mkdir /etc/Wireless/RT2860AP
fi
if [ "$_MODEL_" = "EW7478AC" ]; then
if [ "$_GBAND_SKU_ENABLE_" = "y" -o "$_ABAND_SKU_ENABLE_" = "y" ]; then
if [ "$HW_REG_DOMAIN" = '1' ] && [ "$HW_COUNTRY_REG_ABAND" = '1' -o "$HW_COUNTRY_REG_ABAND" = '2' -o "$HW_COUNTRY_REG_ABAND" = '6' ]; then #CE
echo "==>SingleSKU_CE.dat"
cp -f /bin/7478-SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU_CE.dat
mv /etc/Wireless/RT2860AP/SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU.dat
elif [ "$HW_REG_DOMAIN" = '1' -a "$HW_COUNTRY_REG_ABAND" = '10' ]; then #Australia
echo "==>SingleSKU_AU.dat"
cp -f /bin/7478-SingleSKU_AU.dat /etc/Wireless/RT2860AP/SingleSKU_AU.dat
mv /etc/Wireless/RT2860AP/SingleSKU_AU.dat /etc/Wireless/RT2860AP/SingleSKU.dat	
else # fcc
echo "==>SingleSKU_FCC.dat"
cp -f /bin/7478-SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU_FCC.dat
mv /etc/Wireless/RT2860AP/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU.dat
fi
fi
fi
if [ "$_MODEL_" = "Customer" ]; then
echo "oem_sku: BR6477AC --> "
if [ "$_ID_LAB_" = "y" ];then
echo "==>SingleSKU_ID.dat"
cp -f /bin/SingleSKU_ID.dat /etc/Wireless/RT2860AP/SingleSKU_ID.dat
mv /etc/Wireless/RT2860AP/SingleSKU_ID.dat /etc/Wireless/RT2860AP/SingleSKU.dat
else
if [ "$_GBAND_SKU_ENABLE_" = "y" -o "$_ABAND_SKU_ENABLE_" = "y" ]; then
if [ "$HW_REG_DOMAIN" = '1' ] && [ "$HW_COUNTRY_REG_ABAND" = '1' -o "$HW_COUNTRY_REG_ABAND" = '2' -o "$HW_COUNTRY_REG_ABAND" = '6' ]; then #CE
echo "==>SingleSKU_CE.dat"
cp -f /bin/SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU_CE.dat
mv /etc/Wireless/RT2860AP/SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU.dat
elif [ "$HW_REG_DOMAIN" = '1' -a "$HW_COUNTRY_REG_ABAND" = '10' ]; then #Australia
echo "==>SingleSKU_AU.dat"
cp -f /bin/SingleSKU_AU.dat /etc/Wireless/RT2860AP/SingleSKU_AU	.dat
mv /etc/Wireless/RT2860AP/SingleSKU_AU.dat /etc/Wireless/RT2860AP/SingleSKU.dat	
else # fcc
echo "==>SingleSKU_FCC.dat"
cp -f /bin/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU_FCC.dat
mv /etc/Wireless/RT2860AP/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU.dat
fi
fi
fi
fi
if [ "$_MODEL_" = "Customer" ]; then
echo "oem_sku: EW7438AC --> "
if [ "$_GBAND_SKU_ENABLE_" = "y" ]; then
if [ "$HW_REG_DOMAIN" = '0' ];then # fcc: 1-11
echo "==>SingleSKU_FCC.dat"
cp -f /bin/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU_FCC.dat
mv /etc/Wireless/RT2860AP/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU.dat		
elif [ "$HW_REG_DOMAIN" = '1' ];then # ce | Australia: 1-13
echo "==>SingleSKU_CE.dat"
cp -f /bin/SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU_CE.dat
mv /etc/Wireless/RT2860AP/SingleSKU_CE.dat /etc/Wireless/RT2860AP/SingleSKU.dat
else # fcc: 1-11
echo "==>SingleSKU_FCC.dat"
cp -f /bin/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU_FCC.dat
mv /etc/Wireless/RT2860AP/SingleSKU_FCC.dat /etc/Wireless/RT2860AP/SingleSKU.dat		
fi
fi
if [ "$_ABAND_SKU_ENABLE_" = "y" ]; then
if [ "$HW_COUNTRY_REG_ABAND" = '10' ];then # fcc | Australia : BAND 1,4
echo "+SingleSKU_5G_FCC.dat"
cp -f /bin/SingleSKU_5G_FCC.dat /etc/Wireless/iNIC/SingleSKU_5G_FCC.dat
mv /etc/Wireless/iNIC/SingleSKU_5G_FCC.dat /etc/Wireless/iNIC/SingleSKU_5G.dat
elif [ "$HW_COUNTRY_REG_ABAND" = '1' -o "$HW_COUNTRY_REG_ABAND" = '2' -o "$HW_COUNTRY_REG_ABAND" = '6' ];then # ce: BAND 1 | 1,2 | 1,2,3
echo "==>SingleSKU_5G_CE.dat"
cp -f /bin/SingleSKU_5G_CE.dat /etc/Wireless/iNIC/SingleSKU_5G_CE.dat
mv /etc/Wireless/iNIC/SingleSKU_5G_CE.dat /etc/Wireless/iNIC/SingleSKU_5G.dat
else # fcc
echo "==>SingleSKU_5G_FCC.dat"
cp -f /bin/SingleSKU_5G_FCC.dat /etc/Wireless/iNIC/SingleSKU_5G_FCC.dat
mv /etc/Wireless/iNIC/SingleSKU_5G_FCC.dat /etc/Wireless/iNIC/SingleSKU_5G.dat
fi
fi
fi
}
oem_system()
{
if [ $1 = "init" ]; then
oem_led init
if [ "$IS_RESET_DEFAULT" = 0 ]; then
if [ "$_SSID_MAC_" = "y" ]; then
/bin/setssidmac.sh
fi
if [ "$_WEP_MAC_" = "y" ]; then
/bin/AutoWPA $HW_NIC0_ADDR wepmac
elif [ "$_WPA_KEY_BY_MAC_" = "y" ]; then
/bin/AutoWPA $HW_NIC0_ADDR wpamac
if [ "$_WPA_KEY_BY_MAC_INIC_" = "y" ]; then
/bin/AutoWPA $HW_INIC_ADDR wpamacinic
fi
fi
if [ "$_IQv2_" != "y" ]; then
flash set IS_RESET_DEFAULT 1
fi
if [ "$_AUTOWPA_" = "y" ]; then
flash set WPS_CONFIG_TYPE 1
flash set INIC_WPS_CONFIG_TYPE 1
fi
if [ "$_USB_NAS_" != "" ]; then
MAC=`echo "$HW_NIC0_ADDR" | cut -d "=" -f2 | cut -b7-`
flash set NAS_FOLDER_NAME Logi"$MAC"
fi
reloadFlash
fi
if [ -e /tmp/system_init ]; then
killall -9 pptp.sh
killall -9 pppoe.sh
rm -f /var/ppp/first*
disconnect.sh all
sleep 2
down_interface $_ABAND_IF_
echo ">>> down_interface ABAND"
down_interface $_GBAND_IF_
echo ">>> down_interface GBAND_IF"
if [ "$_USB_SERVER_" = "y" ]; then
stop_usb_server
fi
kill -9 `pidof udhcpd` > /dev/null
wireless_driver_single rmmod $_GBAND_IF_ $_ABAND_IF_
else
/sbin/watchdog -t 3 -F /dev/watchdog&
if [ "$_RFCODE_" = "2" ]; then
cp /etc_ro/Wireless/RT2860AP/RT5392_PCIe_2T2R_ALC_V1_2.bin /tmp/RT30xxEEPROM.bin
fi
fi
elif [ $1 = "init_done" ]; then
if [ "$_MODEL_" = "Customer" ]; then
HWNAT_KO=`lsmod |grep hw_nat |wc -l`
if [ "$HWNAT_KO" = "1" ]; then
rmmod hw_nat
fi
if [ "$IQSET_DISABLE" = "1" -a "$WIZ_MODE" = "0" ]; then #Robert Add, Only Dynamic IP, Static IP and PPPoE support hw_nat.
if [ "$WAN_MODE" -le "2" -a "$FAST_NAT_ENABLE" = "1" ]; then
insmod /bin/hw_nat.ko
fi
fi
fi
if [ "$AP_ROUTER_SWITCH" = "0" ]; then
if [ "$_LAN_WAN_ACCESS_" = "y" ]; then
echo $LAN_WAN_ACCESS > /proc/br_blocklan
fi
if [ "$_IPV6_BRIDGE_" = "y" ]; then
if [ "$IPV6_BRIDGE" = "1" ]; then
echo 1 > /proc/br_ipv6
else
echo 0 > /proc/br_ipv6
fi
fi
if [ "$_DISABLE_IGMP_" = "" ]; then
if [ "$_IGMP_SNOOPING_ENABLED_" = "y" ]; then
enable=$IGMP_SNOOP_ENABLED
else
enable=1
fi
if [ "$enable" = 1 ]; then
echo "$_WAN_IF_" > /proc/br_wanIfx
if [ "$IGMP_SNOOP_ENABLED" = 0 ]; then
echo 0 > /proc/br_igmpsnoop
else
echo 1 > /proc/br_igmpsnoop
fi
else
if [ "$IGMP_SNOOP_ENABLED" = 0 ]; then
echo 0 > /proc/br_igmpsnoop
else
echo 1 > /proc/br_igmpsnoop
fi
fi
else
echo 0 > /proc/br_igmpsnoop
echo 0 > /proc/br_igmpProxy
fi
else
echo "$_WAN_IF_" > /proc/br_wanIfx
kill -9 `pidof udhcpd` > /dev/null
echo 0 > /proc/br_blocklan
echo 0 > /proc/br_ipv6
echo 0 > /proc/br_igmpsnoop
echo 0 > /proc/br_igmpProxy
fi
if [ "$_HOSTNAME_" ]; then
if [ ! "$DOMAIN_NAME" ]; then
DOMAIN_NAME=$_MODE_
fi
echo "127.0.0.1 $_HOSTNAME_.$DOMAIN_NAME" > /etc/hosts
echo "$IP_ADDR $_HOSTNAME_.$DOMAIN_NAME" > /etc/dnrd/master
dnrd -k
dnrd.sh
fi
if [ "$_USB_SERVER_" = "y" ]; then
start_usb_server
fi
echo 1 > /tmp/system_init
oem_led init_done
else
oem_led init_done
fi
}
wisp_wan_mac()
{
echo ">> wisp_wan_mac"
WANIF=$1
WANMACADDR=$2
ifconfig $WANIF down
ifconfig $WANIF hw ether $WANMACADDR
ifconfig $WANIF up
}
auto_channel()
{
set_status=first_set
if [ "$_AUTO_SELECT_CHANNEL_BY_RSSI_" = "y" ]; then
if [ "$_IQv2_" = "y" ] && [ "$IQSET_DISABLE" = "0" ]; then
set_status=already_set
echo 1 > /tmp/wireless_busy # for wlanapp.sh check use.
wlan_scan ra0 best_channel_by_rssi NULL NULL NULL
ifconfig ra0 down up
sleep 5
wlan_scan rai0 best_channel_by_rssi NULL NULL NULL
ifconfig rai0 down up
rm -r /tmp/wireless_busy 2> /dev/null # for wlanapp.sh check use.
echo "yes" > /tmp/autochannel_used    # for MP test check
else
echo "no" > /tmp/autochannel_used    # for MP test check
fi
fi
if [ "$_GBAND_AUTO_SELECT_CLEAN_CHANNEL_" = "y" -a "$set_status" = "first_set" ]; then
if [ "$CHANNEL" = "0" ] && [ "$HW_REG_DOMAIN" = "0" -o "$HW_REG_DOMAIN" = "1" ]; then
echo 1 > /tmp/wireless_busy # for wlanapp.sh check use.
Check_Channel="no"
if [ "$_WIRELESS_SCHEDULE_V2_" = "y" ] && [ "$WLAN_SCH_ENABLED" = "0" -a "$WLAN_DISABLED" = "0" ]; then 
Check_Channel="yes"
elif [ "$WLAN_DISABLED" = "0" ]; then
Check_Channel="yes"
fi
if [ "$Check_Channel" = "yes" ]; then
if [ "$_WIFI_ROMAING_" = "y" ]; then
if [ "$WIZ_MODE" = "2" ]; then	#Rep mode
wlan_scan ra0 setAutoChannel24G 2 10 2
else	#AP or Client mode
wlan_scan ra0 setAutoChannel24G 2 10
fi
else
wlan_scan ra0 setAutoChannel24G 2 10 
fi
ifconfig ra0 down up
fi
rm -r /tmp/wireless_busy 2> /dev/null # for wlanapp.sh check use.
echo "yes" > /tmp/gband_is_autochannel    # for MP test check
else
echo "no" > /tmp/gband_is_autochannel    # for MP test check
fi
fi
if [ "$_WIFI_ROMAING_" = "y" ] && [ "$IQSET_DISABLE" = "1" ] && [ "$WIZ_MODE" = "2" ]; then
if [ "$INIC_CHANNEL" = "0" -a "$INIC_DISABLED" = "0" ]; then
RAI0_CHANNEL=`iwconfig rai0|grep Channel|tr -s ' '|cut -d ' ' -f 3|cut -d = -f 2`
echo "======$RAI0_CHANNEL====="
flash set INIC_CHANNEL $RAI0_CHANNEL
sed s/^Channel=.*/Channel=$RAI0_CHANNEL/g -i /etc/Wireless/iNIC/iNIC_ap.dat
ifconfig rai0 down up
fi
fi
}
