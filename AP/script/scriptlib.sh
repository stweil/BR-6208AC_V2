#!/bin/sh
if [ "$__FUNCTION_SCRIPT" = "" ]; then
. /web/FUNCTION_SCRIPT
__FUNCTION_SCRIPT="y"
fi
findCurrentFlashValue()
{
flash all2 | grep $1 | cut -d= -f 2
}
flashDefault()
{
flash default
if [ "$_WIFI_SUPPORT_" = "y" ]; then 
flash set WLAN_CTS 0
flash set WLAN_N_CHAN_WIDTH 1 #20Mhz
flash set WIFI_TEST 1
flash set WPS_PROXY_ENABLE 1
flash set INIC_CTS 0
flash set INIC_N_CHAN_WIDTH 1 #20Mhz
flash set INIC_WPS_PROXY_ENABLE 1
fi
if [ "$_WirelessInit_" = "y" ]; then
rm /tmp/WscConfStatus 2> /dev/null
fi	
echo flash default
}
reloadFlash()
{
if [ "$_SPECIAL_CHAR_FILTER_IN_SCRIPT_" = "y" ]; then
/bin/flash all2 > /var/flash.inc
else
/bin/flash all > /var/flash.inc
fi
if [ "$_RFCODE_" = "4" ];then
if [ "`flash all2 | grep OP_MODE | cut -f 2 -d =`" = "1" ]; then
echo "_WAN_IF_=\"apcli0\"" >> /var/flash.inc
elif [ "`flash all2 | grep OP_MODE | cut -f 2 -d =`" = "2" ]; then
echo "_WAN_IF_=\"ra0\"" >> /var/flash.inc
elif [ "`flash all2 | grep OP_MODE | cut -f 2 -d =`" = "3" ]; then
echo "_WAN_IF_=\"apclii0\"" >> /var/flash.inc
elif [ "`flash all2 | grep OP_MODE | cut -f 2 -d =`" = "4" ]; then
echo "_WAN_IF_=\"rai0\"" >> /var/flash.inc
elif [ "$_DEFAULT_WAN_IF_" != "" ]; then
echo "_WAN_IF_=\""$_DEFAULT_WAN_IF_"\"" >> /var/flash.inc
else
echo "_WAN_IF_=\"eth2.2\"" >> /var/flash.inc
fi
if [ "$_DEFAULT_LAN_IF_" != "" ]; then
echo "_LAN_IF_=\""$_DEFAULT_LAN_IF_"\"" >> /var/flash.inc
else
echo "_LAN_IF_=\"eth2.1\"" >> /var/flash.inc
fi
echo "_LAN_INTERFACE_=\"eth2\"" >> /var/flash.inc
echo "_GBAND_IF_=\"ra0\"" >> /var/flash.inc
echo "_GBAND_PREFIX_=\"ra\"" >> /var/flash.inc
echo "_GBAND_APCLI_IF_=\"apcli0\"" >> /var/flash.inc
echo "_GBAND_WDS_PREFIX_=\"wds\"" >> /var/flash.inc
echo "_GBAND_STA_IF_=\"ra0\"" >> /var/flash.inc
echo "_GBAND_MESH_IF_=\"mesh0\"" >> /var/flash.inc
echo "_ABAND_IF_=\"rai0\"" >> /var/flash.inc
echo "_ABAND_PREFIX_=\"rai\"" >> /var/flash.inc
echo "_ABAND_APCLI_IF_=\"apclii0\"" >> /var/flash.inc
echo "_ABAND_WDS_PREFIX_=\"wdsi\"" >> /var/flash.inc
echo "_ABAND_STA_IF_=\"rai0\"" >> /var/flash.inc
echo "_ABAND_MESH_IF_=\"meshi0\"" >> /var/flash.inc
fi
}
if [ "$__flash" = "" ]; then
if [ -f /var/flash.inc ]; then
:
else
reloadFlash
fi	
. /var/flash.inc
__flash="y"
fi
__TRUE="1"
__FALSE="0"
__LINK_UP="1"
__LINK_DOWN="0"
__UNDEFINED="-1"
RouterMode=0
APMode=1
RepeaterMode=2
APClientMode=3
WISPMode=4
wireless_driver_single()
{
echo "wireless_driver"
MODULE_PATH="/bin/"
WLAN_INTERFACE=$_GBAND_IF_
INIC_INTERFACE=$_ABAND_IF_
CHANGED_MAC=0
if [ "$_RFCOMB_" = "2" ]; then
AP_DRIVER=rt2860v2_ap
ABAND_DRIVER=MT7610_ap
elif [ "$_RFCOMB_" = "3" ]; then
AP_DRIVER=rt2860v2_ap
ABAND_DRIVER=rlt_wifi
fi
STA_DRIVER=no_sta
if [ "$OP_MODE" = "2" ]; then
BAND_MODULE=`lsmod |grep $STA_DRIVER`
else
BAND_MODULE=`lsmod |grep $AP_DRIVER`
fi
if [ "$OP_MODE" = '0' -o "$OP_MODE" = '1' -o "$OP_MODE" = '3' ] || [ "$2" = $WLAN_INTERFACE -a "$OP_MODE" = '4' ] || [ "$2" = $INIC_INTERFACE -a "$OP_MODE" = '2' ]; then
OPERATION=AP
else
OPERATION=STA
fi
if [ "$1" = "insmod" ];then
if [ "$BAND_MODULE" = "" ];then
if [ "$OPERATION" = "AP" ];then
insmod $MODULE_PATH$AP_DRIVER.ko
sleep 2
insmod $MODULE_PATH$ABAND_DRIVER.ko
else
ifconfig $_GBAND_IF_ down
ifconfig $_ABAND_IF_ down
if [ -e /tmp/system_init ];then
rmmod $AP_DRIVER
fi
fi
for arg in $2 $3;do
if [ "$OPERATION" = "AP" ];then
if [ "$arg" = $WLAN_INTERFACE ];then
HW_MAC_STR="HW_WLAN_ADDR"
else
HW_MAC_STR="HW_INIC_ADDR"
fi
FMAC1=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 0-2`
FMAC2=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 3-4`
FMAC3=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 5-6`
FMAC4=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 7-8`
FMAC5=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 9-10`
FMAC6=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 11-12`
INTERFACE=$arg
else
if [ "$OP_MODE" = '2' ];then
INTERFACE=$_GBAND_STA_IF_
HW_MAC_STR="HW_NIC1_ADDR"
elif [ "$OP_MODE" = '4' ];then
INTERFACE=$_ABAND_STA_IF_
HW_MAC_STR="HW_NIC1_ADDR"
fi
if [ "$STADRV_CLONEMAC" != "000000000000" ]; then
FMAC1=`echo $STADRV_CLONEMAC | cut -b 1-2`
FMAC2=`echo $STADRV_CLONEMAC | cut -b 3-4`
FMAC3=`echo $STADRV_CLONEMAC | cut -b 5-6`
FMAC4=`echo $STADRV_CLONEMAC | cut -b 7-8`
FMAC5=`echo $STADRV_CLONEMAC | cut -b 9-10`
FMAC6=`echo $STADRV_CLONEMAC | cut -b 11-12`
else
FMAC1=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 0-2`
FMAC2=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 3-4`
FMAC3=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 5-6`
FMAC4=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 7-8`
FMAC5=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 9-10`
FMAC6=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 11-12`
fi
fi
ifconfig $INTERFACE up
sleep 2
FMAC_ALL=$FMAC1$FMAC2$FMAC3$FMAC4$FMAC5$FMAC6
MAC1=`iwpriv $INTERFACE e2p 4 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC2=`iwpriv $INTERFACE e2p 4 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC3=`iwpriv $INTERFACE e2p 6 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC4=`iwpriv $INTERFACE e2p 6 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC5=`iwpriv $INTERFACE e2p 8 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC6=`iwpriv $INTERFACE e2p 8 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC_ALL=$MAC1$MAC2$MAC3$MAC4$MAC5$MAC6
if [ "$_MODEL_" != "BR6278AC" ]; then #BR6278AC HW setting by EEPROM.
if [ "$FMAC_ALL" != $MAC_ALL ];then
echo "Write $INTERFACE MAC to EEPROM"
CHANGED_MAC=1
iwpriv $INTERFACE e2p 4=$FMAC2$FMAC1
iwpriv $INTERFACE e2p 6=$FMAC4$FMAC3
iwpriv $INTERFACE e2p 8=$FMAC6$FMAC5
fi
fi
if [ "$OPERATION" = "STA" ];then
iwpriv $INTERFACE set Debug=0
fi
ifconfig $INTERFACE down
done
if [ "$CHANGED_MAC" = "1" ];then
if [ "$OPERATION" = "AP" ];then
rmmod $AP_DRIVER
sleep 2
rmmod $ABAND_DRIVER
insmod $MODULE_PATH$AP_DRIVER.ko
sleep 2
insmod $MODULE_PATH$ABAND_DRIVER.ko
else
rmmod $STA_DRIVER
insmod $MODULE_PATH$STA_DRIVER.ko
fi		
fi
fi
else
if [ "$OPERATION" = "AP" ];then
rmmod $AP_DRIVER
sleep 2
rmmod $ABAND_DRIVER
else
rmmod $STA_DRIVER
fi
fi
}
wireless_driver()
{
echo ">>> wireless_driver()"
MODULE_PATH="/bin/"
WLAN_INTERFACE=$_GBAND_IF_
INIC_INTERFACE=$_ABAND_IF_
if [ "$2" = $WLAN_INTERFACE ];then
echo ">>> Loading 2.4G Wireless Driver "
AP_DRIVER=rt2860v2_ap
AP_RM_DRIVER=rt2860v2_ap
BAND_MODULE=`lsmod |grep rt2860v2_ap`
else
echo ">>> Loading 5G Wireless Driver "
if [ "$_RFCOMB_" = "3" ]; then 
AP_DRIVER=rlt_wifi
AP_RM_DRIVER=rlt_wifi
BAND_MODULE=`lsmod |grep rlt_wifi`
else
AP_DRIVER=MT7610_ap
AP_RM_DRIVER=MT7610_ap
BAND_MODULE=`lsmod |grep MT7610_ap`
fi
fi
echo ">>> Loading $AP_DRIVER"
OPERATION=AP
if [ "$1" = "insmod" ];then
if [ "$BAND_MODULE" = "" ];then
for driver in $AP_DRIVER; do
insmod $MODULE_PATH$driver.ko
done
if [ "$2" = $WLAN_INTERFACE ];then
HW_MAC_STR="HW_WLAN_ADDR"
else
HW_MAC_STR="HW_INIC_ADDR"
fi
FMAC1=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 0-2`
FMAC2=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 3-4`
FMAC3=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 5-6`
FMAC4=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 7-8`
FMAC5=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 9-10`
FMAC6=`flash all | grep -i $HW_MAC_STR | cut -f 2 -d '=' | cut -b 11-12`
INTERFACE=$2
ifconfig $INTERFACE up
sleep 2
FMAC_ALL=$FMAC1$FMAC2$FMAC3$FMAC4$FMAC5$FMAC6
MAC1=`iwpriv $INTERFACE e2p 4 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC2=`iwpriv $INTERFACE e2p 4 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC3=`iwpriv $INTERFACE e2p 6 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC4=`iwpriv $INTERFACE e2p 6 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC5=`iwpriv $INTERFACE e2p 8 | grep :0x | cut -f 2 -d ':' | cut -b 5-6 | tr [A-Z] [a-z]`
MAC6=`iwpriv $INTERFACE e2p 8 | grep :0x | cut -f 2 -d ':' | cut -b 3-4 | tr [A-Z] [a-z]`
MAC_ALL=$MAC1$MAC2$MAC3$MAC4$MAC5$MAC6
if [ "$_MODEL_" != "BR6278AC" ]; then #BR6278AC HW setting by EEPROM.
if [ "$FMAC_ALL" != $MAC_ALL ]; then
echo "Write $INTERFACE MAC to EEPROM"
iwpriv $INTERFACE e2p 4=$FMAC2$FMAC1
iwpriv $INTERFACE e2p 6=$FMAC4$FMAC3
iwpriv $INTERFACE e2p 8=$FMAC6$FMAC5
ifconfig $INTERFACE down
for driver in $AP_RM_DRIVER; do
rmmod $driver
done
for driver in $AP_DRIVER; do
insmod $MODULE_PATH$driver.ko
done
fi
fi
ifconfig $INTERFACE down
fi
else
for driver in $AP_RM_DRIVER; do
rmmod $driver
done
fi
}
interfac_op()
{
WLAN_INTERFACE=$_GBAND_IF_
WLAN_APCLI_INTERFACE=$_GBAND_APCLI_IF_
INIC_INTERFACE=$_ABAND_IF_
INIC_APCLI_INTERFACE=$_ABAND_APCLI_IF_
INTERFACE=$1
OPERATION=$2
if [ "$INTERFACE" = $WLAN_INTERFACE ];then
if [ "$_MSSID_" = 'y' ];then
num=$_MSSIDNUM_;
else
num=0
fi
if [ "$AP_MODE" = '0' -o "$AP_MODE" = '6' ];then
while [ $num -ge 0 ];
do
ifconfig $_GBAND_PREFIX_$num $OPERATION 2> /dev/null
num=`expr $num - 1`
done
if [ "$AP_MODE" = '6' ];then
ifconfig $WLAN_APCLI_INTERFACE $OPERATION
fi
elif [ "$AP_MODE" = '1' ];then
echo "AP_MODE = 1"
elif [ "$AP_MODE" = '2' ];then
ifconfig $WLAN_APCLI_INTERFACE $OPERATION
else
num=0
while [ $num -le 3 ];
do
ifconfig wdsi$num $OPERATION 2> /dev/null
num=`expr $num + 1`
done
fi
elif [ "$INTERFACE" = $INIC_INTERFACE ];then
if [ "$_INIC_MSSID_" = 'y' ];then
num=$_INIC_MSSIDNUM_;
else
num=0
fi
if [ "$INIC_AP_MODE" = '0' -o "$INIC_AP_MODE" = '6' ];then
while [ $num -ge 0 ];
do
ifconfig $_ABAND_PREFIX_$num $OPERATION 2> /dev/null
num=`expr $num - 1`
done
if [ "$INIC_AP_MODE" = '6' ];then
ifconfig $INIC_APCLI_INTERFACE $OPERATION
fi
elif [ "$INIC_AP_MODE" = '1' ];then
echo "INIC_AP_MODE = 1"
elif [ "$INIC_AP_MODE" = '2' ];then
ifconfig $INIC_APCLI_INTERFACE $OPERATION
else
num=0
while [ $num -le 3 ];
do
ifconfig wds$num $OPERATION 2> /dev/null
num=`expr $num + 1`
done
fi
else
ifconfig $INTERFACE $OPERATION
fi
}
down_interface()
{
WIRED_INTERFACE=$_LAN_INTERFACE_
WLAN_INTERFACE=$_GBAND_IF_
WLAN_STA_INTERFACE=$_GBAND_STA_IF_
WLAN_APCLIENT_INTERFACE=$_GBAND_APCLI_IF_
WLAN_WDS_PREFIX=$_GBAND_WDS_PREFIX_
INIC_INTERFACE=$_ABAND_IF_
INIC_STA_INTERFACE=$_ABAND_STA_IF_
INIC_APCLIENT_INTERFACE=$_ABAND_APCLI_IF_
INIC_WDS_PREFIX=$_ABAND_WDS_PREFIX_
INTERFACE=$1
BR_INTERFACE=$2
if [ "$_MESH_SUPPORT_" = 'y' ];then
WLAN_MESH_INTERFACE=$_GBAND_MESH_IF_
INIC_MESH_INTERFACE=$_ABAND_MESH_IF_
fi
if [ "$BR_INTERFACE" = "" ];then
BR_INTERFACE=br0
fi
if [ "$INTERFACE" = $WLAN_INTERFACE ];then
if [ "$_MSSID_" = 'y' ];then
num=$_MSSIDNUM_;
else
num=0
fi
while [ $num -ge 0 ];
do
brctl delif $BR_INTERFACE $_GBAND_PREFIX_$num 2> /dev/null
ifconfig $_GBAND_PREFIX_$num down 2> /dev/null
num=`expr $num - 1`
done    	
num=0
while [ $num -le 3 ];
do
brctl delif $BR_INTERFACE $WLAN_WDS_PREFIX$num 2> /dev/null
ifconfig $WLAN_WDS_PREFIX$num down 2> /dev/null
num=`expr $num + 1`
done
brctl delif $BR_INTERFACE $WLAN_APCLIENT_INTERFACE 2> /dev/null
ifconfig $WLAN_APCLIENT_INTERFACE down 2> /dev/null
ifconfig $WLAN_STA_INTERFACE down
if [ "$_MESH_SUPPORT_" = 'y' ];then
brctl delif $BR_INTERFACE $WLAN_MESH_INTERFACE 2> /dev/null
ifconfig $WLAN_MESH_INTERFACE down > /dev/null
fi
elif [ "$INTERFACE" = $INIC_INTERFACE ];then
if [ "$_MSSID_" = 'y' ];then
num=$_MSSIDNUM_;
else
num=0
fi
while [ $num -ge 0 ];
do
brctl delif $BR_INTERFACE $_ABAND_PREFIX_$num 2> /dev/null
ifconfig $_ABAND_PREFIX_$num down 2> /dev/null
num=`expr $num - 1`
done    	
num=0
while [ $num -le 3 ];
do
brctl delif $BR_INTERFACE $INIC_WDS_PREFIX$num 2> /dev/null
ifconfig $INIC_WDS_PREFIX$num down 2> /dev/null
num=`expr $num + 1`
done
brctl delif $BR_INTERFACE $INIC_APCLIENT_INTERFACE 2> /dev/null
ifconfig $INIC_APCLIENT_INTERFACE down 2> /dev/null
ifconfig $INIC_STA_INTERFACE down
if [ "$_MESH_SUPPORT_" = 'y' ];then
brctl delif $BR_INTERFACE $INIC_MESH_INTERFACE 2> /dev/null
ifconfig $INIC_MESH_INTERFACE down > /dev/null
fi
elif [ "$INTERFACE" = $WIRED_INTERFACE ];then
brctl delif $BR_INTERFACE $_LAN_IF_
ifconfig $_LAN_IF_ down
brctl delif $BR_INTERFACE $_WAN_IF_
ifconfig $_WAN_IF_ down
brctl delif $BR_INTERFACE $INTERFACE
ifconfig $INTERFACE down
else
brctl delif $BR_INTERFACE $INTERFACE
ifconfig $INTERFACE down
fi
}
reloadSwitchDriver()
{
if [ "$_SWTYPE_" = "WLLLL" ];then
model=0
else
model=1
fi
led_mode=$1
if [ "$AP_ROUTER_SWITCH" = "0" ];then
vlan=1
else		
if [ $PS_SCH_WIREDLAN = '2' ];then
vlan=1
else
vlan=0
fi	
fi
if [ -e /tmp/system_init ];then
rmmod rgmii
fi
insmod /bin/rgmii.ko model=$model led_mode=$led_mode vlan=$vlan led_all=$2
}
getWdsNum()
{
COUNT=0
ADDR_NUM=0
while [ $COUNT -lt 6 ]
do
eval "WL_LINKMAC="\$WL_LINKMAC$COUNT""
if [ "$WL_LINKMAC" != "000000000000" ]; then
ADDR_NUM=`expr $ADDR_NUM + 1`
fi
COUNT=`expr $COUNT + 1`
done	
echo $ADDR_NUM
}
setMacFilterByHw()
{
echo "LED ON" > /dev/WPS_LED	
WAIT_TIME=$1
NSSID=\$SSID_"$2"	
flash="/bin/flash" #flash tool
WLAN="ra"$2
if [ "$2" -ge "1" ]; then
MACAC="WLAN_MACAC"`expr $2 + 1`
else
MACAC="WLAN_MACAC"
fi
eval "$MACAC"_NUM="`eval echo \$"$MACAC"_NUM`"
echo  start to collect macs
iwpriv $WLAN set HideSSID=0
if [ "$2" -ge "1" ]; then
iwpriv $WLAN set SSID=`eval echo $NSSID`
else
iwpriv $WLAN set SSID=$SSID
fi
iwpriv $WLAN set AccessPolicy=0
ALL_MAC=`iwpriv $WLAN show_mac_table | grep -v $WLAN`
echo now mac = $ALL_MAC
count=1
BREAK=0
while [ "$BREAK" = "0" ]; do
sleep 1
NOW_MAC=`iwpriv $WLAN show_mac_table | grep -v $WLAN`
echo now mac =  $NOW_MAC
for arg in $NOW_MAC 
do
if [ "`echo $ALL_MAC | grep -i $arg`" = "" ]; then
if [ "`$flash all | grep \"$MACAC\"_ADDR | grep -i $arg`" = "" ]; then
if [ "`eval echo \$\"$MACAC\"_NUM`" = "20" ]; then
$flash set "$MACAC"_ADDR del del # delete first mac in mac filter
fi
$flash set "$MACAC"_ADDR add $arg
sleep 1
BREAK=1
fi
fi
done
if [ "$count" = "$WAIT_TIME" ]; then
BREAK=1
fi
count=`expr $count + 1`
echo $count
ALL_MAC=$NOW_MAC
done
echo finish collecting mac
if [ "$2" -ge "1" ]; then
aclList="aclList""`expr $2 + 1`"
MAC_LIST=`flash $aclList | cut -d '=' -f 2`
else
MAC_LIST=`flash aclList | cut -d '=' -f 2`
fi
echo "MAC LIST="$MAC_LIST
if [ "$MAC_LIST" != "" ]; then
iwpriv $WLAN set ACLClearAll=1
iwpriv $WLAN set ACLAddEntry="$MAC_LIST"
else
echo no mac in mac list
fi
iwpriv $WLAN set AccessPolicy=1
iwpriv $WLAN set HideSSID=1
if [ "$2" -ge "1" ]; then
iwpriv $WLAN set SSID=`eval echo $NSSID`
flash set HIDDEN_SSID_$2 1
flash set WLAN_MACAC"`expr $2 + 1`"_ENABLED 1
else
iwpriv $WLAN set SSID=$SSID
flash set HIDDEN_SSID 1
flash set WLAN_MACAC_ENABLED 1
fi
reloadFlash
kill -USR1 `pidof webs`
echo "LED OFF" > /dev/WPS_LED	
}
ClientRenewIP()
{
if [ "$_WIFI_REPEATER_" = "y" ] && [ "`switchLocation`" = "sleepMode" ]; then
return 0;
fi
RJ45_down_up=0
crossBand=0
Gbandoff=`flash get WLAN_DISABLED | cut -d = -f 2`
Abandoff=`flash get INIC_DISABLED | cut -d = -f 2`
tatalmssid=4
if [ "$_CROSSBAND_" = "y" ] && [ "$CROSSBAND_ENABLE" = 1 ]; then
crossBand=1
fi
if [ "$WIZ_MODE" != "1" ]; then
if [ "$Abandoff" = "0" -a "$Gbandoff" = "0" ]; then
if [ "$crossBand" = "1" -a "$1" = "Gband" ]; then
RJ45_down_up=1
fi	
if [ "$crossBand" = "0" -a "$1" = "Aband" ]; then
RJ45_down_up=1
fi	
else
RJ45_down_up=1
fi
if [ "$_WIFI_REPEATER_" = "y" -a "$RJ45_down_up" = "1" ]; then
if [ "$_MODEL_" = "EW7478AC" ]; then
RJ45port_control 3 down
sleep 1
RJ45port_control 3 up
else
RJ45port_control 4 down
sleep 1
RJ45port_control 4 up
fi
fi
if [ "$_MODEL_" = "Customer" -a "$RJ45_down_up" = "1" ]; then
RJ45port_control 1 down
RJ45port_control 2 down
RJ45port_control 3 down
RJ45port_control 4 down
sleep 1
RJ45port_control 1 up
RJ45port_control 2 up
RJ45port_control 3 up
RJ45port_control 4 up
fi
fi
if [ "$WIZ_MODE" != "3" ]; then
if [ "$1" = "Aband" -a "$Abandoff" = "0" ]; then
ifconfig rai0 down 2> /dev/null
mssid_control $tatalmssid Aband down
sleep 7
if [ "$INIC_SCH_ENABLED" = "1" ]; then
flash getWireSchSet 1 5G `TimePointNow`
else
ifconfig rai0 up 2> /dev/null
mssid_control $tatalmssid Aband up
fi
fi
if [ "$1" = "Gband" -a "$Gbandoff" = "0" ]; then
ifconfig ra0 down 2> /dev/null
mssid_control $tatalmssid Gband down
sleep 7
if [ "$WLAN_SCH_ENABLED" = "1" ]; then
flash getWireSchSet 1 24G `TimePointNow`
else
ifconfig ra0 up 2> /dev/null
mssid_control $tatalmssid Gband up
fi
fi
fi
}
setUpWireless()
{
if [ "$4" != "" ]; then
echo ">>> setUpWireless sleep $4"
sleep $4
fi
Gbandoff=`flash get WLAN_DISABLED | cut -d = -f 2`
Abandoff=`flash get INIC_DISABLED | cut -d = -f 2`
crossBand=0
SwitchAtSleepMode=0
tatalmssid=4
if [ "$_WIFI_REPEATER_" = "y" ] && [ "`switchLocation`" = "sleepMode" ]; then
SwitchAtSleepMode=1
fi
if [ "$_CROSSBAND_" = "y" ] && [ "$CROSSBAND_ENABLE" = 1 ]; then
crossBand=1
fi
if [ "$SwitchAtSleepMode" = "0" ] && [ "$1" = "1" -o "$1" = "up" ]; then
if [ "$2" = "Gband" ]; then
if [ "$Gbandoff" = "0" ]; then
if [ "$WLAN_SCH_ENABLED" = "1" -a "$3" = "sleepMode" ]; then
flash getWireSchSet 1 24G `TimePointNow`
else
if [ "$_MODEL_" = "EW7478AC" ]; then
echo "=========24G LED ON==========="
echo "LED ON" > /dev/GBAND_LED
fi
sleep 2	#fixed AP Mode wifi schedule interface can not open.
ifconfig ra0 up 2> /dev/null
sleep 2				
if [ "$crossBand" = 0 ]; then
ifconfig apcli0 up 2> /dev/null
else
ifconfig apclii0 up 2> /dev/null
fi
mssid_control $tatalmssid Gband up
fi
fi
if [ "$3" = "schedule" ]; then
rm -f /tmp/Gschedule 2> /dev/null
fi
else
if [ "$Abandoff" = "0" ]; then
if [ "$INIC_SCH_ENABLED" = "1" -a "$3" = "sleepMode" ]; then
flash getWireSchSet 1 5G `TimePointNow`
else
if [ "$_MODEL_" = "EW7478AC" ]; then
echo "=========5G LED ON==========="
echo "LED ON" > /dev/ABAND_LED
fi				
sleep 2	#fixed AP Mode wifi schedule interface can not open.
ifconfig rai0 up 2> /dev/null
sleep 2				
if [ "$crossBand" = 0 ]; then
ifconfig apclii0 up 2> /dev/null
else
ifconfig apcli0 up 2> /dev/null
fi
mssid_control $tatalmssid Aband up
fi
fi
if [ "$3" = "schedule" ]; then
rm -f /tmp/Aschedule 2> /dev/null
fi
fi
if [ "$3" = "sleepMode" ] && [ -f /tmp/SleepMode ]; then
rm -f /tmp/SleepMode 2> /dev/null
if [ "$_MODEL_" = "EW7478AC" ]; then
RJ45port_control 3 up
else
RJ45port_control 4 up
fi
killall led.sh 2> /dev/null
/bin/led.sh &	
fi
fi
if [ "$1" = "0" -o "$1" = "down" ]; then
if [ "$3" = "sleepMode" ]; then 
sleep 10
fi
if [ "$2" = "Gband" ]; then
if [ "$_MODEL_" = "EW7478AC" ]; then
echo "=========24G LED off==========="
echo "LED OFF" > /dev/GBAND_LED
fi			
sleep 2	#fixed AP Mode wifi schedule interface can not close.
ifconfig ra0 down 2> /dev/null
sleep 2
if [ "$crossBand" = 0 ]; then
ifconfig apcli0 down 2> /dev/null
else
ifconfig apclii0 down 2> /dev/null
fi
mssid_control $tatalmssid Gband down
if [ "$3" = "schedule" ]; then
echo 1 > /tmp/Gschedule
fi
else
if [ "$_MODEL_" = "EW7478AC" ]; then
echo "=========5G LED off==========="
echo "LED OFF" > /dev/ABAND_LED
fi			
sleep 2	#fixed AP Mode wifi schedule interface can not open.
ifconfig rai0 down 2> /dev/null		
sleep 2
if [ "$crossBand" = 0 ]; then
ifconfig apclii0 down 2> /dev/null
else
ifconfig apcli0 down 2> /dev/null
fi
mssid_control $tatalmssid Aband down
if [ "$3" = "schedule" ]; then
echo 1 > /tmp/Aschedule
fi
fi
if [ "$3" = "sleepMode" ] && [ ! -f /tmp/SleepMode ]; then
echo 1 > /tmp/SleepMode
if [ "$_MODEL_" = "EW7478AC" ]; then
RJ45port_control 3 down
else
RJ45port_control 4 down
fi
all_led_off
fi
fi
}
setUpLED()
{
if [ "$1" = "all_led_off" ]; then
all_led_off
else
/bin/led.sh &
fi
}
apcli_channel_auto()
{
if [ "$1" = $_GBAND_APCLI_IF_ ];then
BAND_ALIVE=`dmesg -c |grep $SSID |wc -l`
if [ "$BAND_ALIVE" = "0" ];then
iwpriv $_GBAND_IF_ set SiteSurvey=1
sleep 4
iwpriv $_GBAND_IF_ get_site_survey > /tmp/sitesurvey
BAND_CUR_CH=`cat /tmp/sitesurvey | grep $SSID |cut -f 1 -d " "`
if [ "$BAND_CUR_CH" != $CHANNEL -a "$BAND_CUR_CH" != "" ];then
ifconfig $_GBAND_APCLI_IF_ down
ifconfig $_GBAND_IF_ down
rm /var/flash.inc
flash set CHANNEL $BAND_CUR_CH
/bin/wlan.sh $_GBAND_IF_
ifconfig $_GBAND_IF_ up
ifconfig $_GBAND_APCLI_IF_ up
fi
fi
elif [ "$1" = $_ABAND_APCLI_IF_ ];then
BAND_ALIVE=`dmesg -c |grep $INIC_SSID |wc -l`
if [ "$BAND_ALIVE" = "0" ];then
iwpriv $_ABAND_IF_ set SiteSurvey=1
sleep 4
iwpriv $_ABAND_IF_ get_site_survey > /tmp/sitesurvey
BAND_CUR_CH=`cat /tmp/sitesurvey | grep $INIC_SSID | cut -f 1 -d " "`
if [ "$BAND_CUR_CH" != $INIC_CHANNEL -a "$BAND_CUR_CH" != "" ];then
ifconfig $_ABAND_APCLI_IF_ down
ifconfig $_ABAND_IF_ down
rm /var/flash.inc
flash set INIC_CHANNEL $BAND_CUR_CH
/bin/inic_wlan.sh $_ABAND_IF_
ifconfig $_ABAND_IF_ up
ifconfig $_ABAND_APCLI_IF_ up
fi
fi
fi
}
setPassthroughLanToWan()
{
MAC=`cat /proc/net/arp | grep $1 | cut -b42-59`
iptables -t nat -I PREROUTING -m physdev --physdev-in rai0  -m mac --mac-source $MAC -j RETURN
echo -n $MAC >> /var/MacAddr
}
removePassthroughLanToWan()
{
OLD_MAC=`cat /var/MacAddr`
ALL_MAC=`iwpriv ra0 show_mac_table | grep -v ra0`
rm -f /var/MacAddr
touch /var/MacAddr
for arg in $OLD_MAC 
do
if [ "`echo $ALL_MAC | grep -i $arg`" = "" ]; then
iptables -t nat -D PREROUTING -m physdev --physdev-in rai0  -m mac --mac-source $arg -j RETURN
else
echo -n $arg >> /var/MacAddr
fi
done
}
PREVIOUS_STATE=$__UNDEFINED
checkWANStatus()
{
WAN_MODE=`flash get WAN_MODE | cut -d = -f 2`
if [ "$WAN_MODE" = "0" ]; then
if [ "$_SWTYPE_" = "WLLLL" ];then
LINK_STATE=`/bin/linkdetect.sh 0 | cut -f 2 -d =`
else
LINK_STATE=`/bin/linkdetect.sh 4 | cut -f 2 -d =`
fi
if [ "$PREVIOUS_STATE" != "$__UNDEFINED" ]; then 
if [ "$PREVIOUS_STATE" = "0" ] && [ "$LINK_STATE" = "1" ]; then
echo "WAN DOWN to UP"
/bin/dhcpc.sh $_WAN_IF_ wait
elif  [ "$PREVIOUS_STATE" = "1" ] && [ "$LINK_STATE" = "0" ]; then
echo "WAN UP to DOWN"
ifconfig $_WAN_IF_ 0.0.0.0
fi			
fi
PREVIOUS_STATE=$LINK_STATE
else
PREVIOUS_STATE=$__UNDEFINED
fi
}
pingtool()
{
killall -2 ping
ping -W 5 -c $2 $1 > /tmp/$3
echo "---   END   ---" >> /tmp/$3 		
}
Schedule_PortForwarding()
{
/bin/flash set PORTFW_ENABLED $1
reloadFlash
/bin/firewall.sh
kill -USR1 `pidof webs`
}
Schedule_UrlBlocking()
{
/bin/flash set URLB_ENABLED $1
reloadFlash
/bin/firewall.sh
kill -USR1 `pidof webs`
}
getCurrentWANIP()
{
CURRENT_WAN="$_WAN_IF_"
if [ "$OP_MODE" = '1' ];then
CURRENT_WAN="$_GBAND_APCLI_IF_"
elif [ "$OP_MODE" = '2' ];then
CURRENT_WAN="$_GBAND_STA_IF_"
elif [ "$OP_MODE" = '3' ];then
CURRENT_WAN="$_ABAND_APCLI_IF_"
elif [ "$OP_MODE" = '4' ];then
CURRENT_WAN="$_ABAND_STA_IF_"
fi
if [ "$WAN_MODE" = "2" ] || [ "$WAN_MODE" = "3" ] || [ "$WAN_MODE" = "6" ]; then
CURRENT_WAN="ppp0"
if [ "$MPPPOE" != "" ]; then
CURRENT_WAN1="ppp1"
fi
fi
CURRENT_WAN_IP=`ifconfig $CURRENT_WAN | grep -i "addr:" | cut -f2 -d: | cut -f1 -d " "`
}
setSuperDMZ()
{
killall udhcpd	
getCurrentWANIP
echo "============SET Super DMZ================"
echo "==== "DMZ_MAC=$DMZ_MAC_ADDR   LANMAC=$HW_NIC0_ADDR   LANIP=$IP_ADDR  WANIP=$CURRENT_WAN_IP"  ===="
echo $DMZ_MAC_ADDR $HW_NIC0_ADDR $IP_ADDR $CURRENT_WAN_IP $1 > /proc/sys/net/ipv4/super_dmz_control 
dhcpd.sh br0
}
wait_time_pass()
{
COUNT=1
TIMEOUT=$1
while [ "$COUNT" -le "$TIMEOUT" ]; do 
echo -e "$COUNT / $TIMEOUT\r\c"
COUNT=`expr $COUNT + 1`
done	
}
checkIsConnect()
{	
STATUS_FILE=/tmp/wanStatus
CURRENT_WAN="$_WAN_IF_"
Status=`cat /tmp/wanStatus`    #wan無連接狀態
if [ "$Status" = "4" ]; then
echo "wan port no link!!"
exit 0
fi
echo "start detecting wan state."
COUNT=1
while [ true ]; do
IPADDR=`route | grep -i "default" | tr -s " " | cut -d" " -f2`
COUNT=`expr $COUNT + 1`
sleep 1
if [ "$COUNT" = "10" ]; then
if [ "$IPADDR" = "" ]; then
PACKET_LOSS="100%"
else
PACKET_LOSS=`ping -W 10 -c 7 $IPADDR | grep -i "packet loss" | tr -s " " | cut -d" " -f7`
fi
break
fi
done
if [ "$PACKET_LOSS" != "100%" ]; then
echo "link successfully!!"
echo "1" > $STATUS_FILE
else
case "$WAN_MODE" in
"0")
echo "2" > $STATUS_FILE        #dhcp無連上internet
echo "dhcp client is NG!!"
;;
"1" | "2")
echo "3" > $STATUS_FILE       #撥號連線無連上internet
echo "pppoe client is NG!!"
;;
esac
fi
}
led_control()
{
DEV_WLAN_LED=/dev/WLAN_LED
DEV_POW_LED=/dev/PowerLED
DEV_WLAN_ABAND_LED=/dev/ABAND_LED
DEV_WLAN_GBAND_LED=/dev/GBAND_LED
if [ "$1" = "ON" ]; then
reg s 0xb0110000
reg w a4 5
reg w a8 5
reg w ac 5
reg w b0 5
reg w b4 5
if [ ! -f $DEV_WLAN_LED ]; then
mknod /dev/WLAN_LED c 166 $_HW_LED_WIRELESS_
mknod /dev/WLAN_ABAND_LED c 166 $_HW_LED_WIRELESS_ABAND_
mknod /dev/WLAN_GBAND_LED c 166 $_HW_LED_WIRELESS_GBAND_
echo "LED BLINK RANDOM" > $DEV_WLAN_LED
echo "LED BLINK RANDOM" > $DEV_WLAN_ABAND_LED
echo "LED BLINK RANDOM" > $DEV_WLAN_GBAND_LED
else
echo "LED BLINK RANDOM" > $DEV_WLAN_LED
echo "LED BLINK RANDOM" > $DEV_WLAN_ABAND_LED
echo "LED BLINK RANDOM" > $DEV_WLAN_GBAND_LED
fi
if [ "$_CONVERT_LED_CONTROL_" = "y" ]; then
if [ ! -f /dev/CONVERT_LED ]; then
mknod /dev/CONVERT_LED c 166 $_HW_LED_USB_
fi
if [ `flash get AP_ROUTER_SWITCH | cut -d"=" -f2` = "2" ]; then
echo "LED ON" > /dev/CONVERT_LED
fi
fi
elif [ "$1" = "OFF" ]; then
reg s 0xb0110000
reg w a4 7
reg w a8 7
reg w ac 7
reg w b0 7
reg w b4 7
echo "LED OFF" > $DEV_WLAN_LED
echo "LED OFF" > $DEV_WLAN_ABAND_LED
echo "LED OFF" > $DEV_WLAN_GBAND_LED
rm -f $DEV_WLAN_LED
rm -f $DEV_WLAN_ABAND_LED
rm -f $DEV_WLAN_GBAND_LED
if [ "$_CONVERT_LED_CONTROL_" = "y" ]; then
if [ ! -f /dev/CONVERT_LED ]; then
mknod /dev/CONVERT_LED c 166 $_HW_LED_USB_
fi
if [ `flash get AP_ROUTER_SWITCH | cut -d"=" -f2` = "2" ]; then
echo "LED OFF" > /dev/CONVERT_LED
fi
fi
fi
}
create_pppoe_secretfile()
{
echo "#################################################" > $3
echo "\"$1\"	*	\"$2\" *" >> $3
}
create_pppoe_optionfile()
{
if [ "$1" != "add" ]; then
echo "name \"$1\"" > $5
echo "noipdefault" >> $5  
echo "hide-password" >> $5 
echo "ipcp-accept-remote" >> $5  
echo "ipcp-accept-local" >> $5
echo "nodetach" >> $5
echo "usepeerdns" >> $5
echo "lcp-echo-interval 2" >> $5
echo "lcp-echo-failure 20" >> $5
echo "lock" >> $5
echo "mtu $2" >> $5
echo "mru $3" >> $5
echo "sync" >> $5
echo "$4" >> $5
elif [ "$1" = "add" ]; then
echo "$2" >> "$3"
fi
}
eth_port_control()
{
if [ "$1" = "0" ] || [ "$1" = "1" ] || [ "$1" = "2" ] || [ "$1" = "3" ] || [ "$1" = "4" ]; then
if [ "$2" = "ON" ] || [ "$2" = "1" ]; then 
mii_mgr -s -p $1 -r 0 -v 0x3100 2> /dev/null
elif [ "$2" = "OFF" ] || [ "$2" = "0" ]; then 
mii_mgr -s -p $1 -r 0 -v 0x3900 2> /dev/null	
else
echo "Input Error Var2 != ON or OFF"
fi
else 
echo "Input Error Var1 !=  0-4"
fi 
}
start_usb_server()
{
if [ "$_USB_NAS_" =  'y' ];then
echo -e "1\n2\n3\n4\n5\n6\n7\n9\na\nb\n12\nd\ne\ne0\nef\nfe\nff\n" | cat > /proc/sxuptp/devices.allow
echo -e "8\n" | cat > /proc/sxuptp/devices.deny
fi
if [ -f /tmp/share.tmp ]; then
rm /tmp/share.tmp
fi
}
stop_usb_server()
{
umount /proc/bus/usb
rmmod NetUSB
rmmod GPL_NetUSB
if [ -f /tmp/share.tmp ]; then
rm /tmp/share.tmp
fi
}
get_sntp_status()
{
if [ "$_POWERSAVINGLOGITEC_" != "" ]; then
/bin/sntp.sh
sntpStatus=`cat /var/log/sntp.log | grep -i "ERROR"`
if [ "$sntpStatus" != "" ]; then
echo "0" > /tmp/sntpStatus.txt
else
echo "1" > /tmp/sntpStatus.txt
fi
fi
}
dhcpc_up_down()
{
if [ "$1" = "0" ]; then
echo "_____udhcpc down_____" 
ifconfig $_WAN_IF_ 0.0.0.0
kill -9 `pidof udhcpc` 2> /dev/null
else
echo "_____udhcpc up_____"
/bin/dhcpc.sh $_WAN_IF_ wait
fi
}
ststic_up_down()
{
if [ "$1" = "0" ]; then
echo "_____ststic IP down_____" 
ifconfig $_WAN_IF_ 0.0.0.0
else
echo "_____ststic IP up_____"
/bin/fixedip.sh $_WAN_IF_ $WAN_IP_ADDR $WAN_SUBNET_MASK $WAN_DEFAULT_GATEWAY
/bin/ipup.sh static
fi
}
is_decimal()
{
if [ "$1" ] && [ "`echo \"$1\" | grep \"^[0-9]\\{1,10\\}$\"`" ]; then
echo "y"
fi
}
is_hex()
{
if [ "$1" ] && [ "`echo \"$1\" | grep \"^[0-9a-fA-F]\\{1,10\\}$\"`" ]; then
echo "y"
fi
}
module_is_loaded()
{
if [ "$1" ] && [ "`lsmod | grep \"^$1 \"`" ]; then
echo "y"
fi
}
interface_is_up()
{
if [ "$1" ] && [ "`ifconfig | grep \"^$1\"`" ]; then
echo "y"
fi
}
interface_exist()
{
if [ "$1" ] && [ "`ifconfig -a | grep \"^$1\"`" ]; then
echo "y"
fi
}
iptables_chain_exist()
{
if [ "$1" ] && [ "$2" ] && [ "`iptables -L -t $1 | grep \"^Chain $2 ([0-9]\{1,10\} references)$\"`" ]; then
echo "y"
fi
}
security_log()
{
if [ ! -d /var/log ]; then
mkdir -p /var/log
fi
echo [`date +"%F %T"`]: [$1]: $2 >> /var/log/security
}
ip_of()
{
if [ "$1" ]; then
ip="`ifconfig $1 | grep \"inet addr:\" | cut -f 2 -d : | cut -f 1 -d \" \"`"
if [ "$ip" ]; then
echo $ip
fi
fi
}
lan_of()
{
echo "br0"
}
wan_of()
{
Operation_Mode=$1
WAN_Type=$2
WAN_Number=$3
case "$WAN_Type" in
"0"|"1")
case "$Operation_Mode" in
"0")
case "$WAN_Number" in
"1") echo $_WAN_IF_ ;;
"P") echo $_WAN_IF_ ;;
esac
;;
"1")
case "$WAN_Number" in
"1") echo $_GBAND_APCLI_IF_ ;;
"P") echo $_GBAND_APCLI_IF_;;
esac
;;
esac
;;
"2")
case "$Operation_Mode" in
"0")
case "$WAN_Number" in
"1") echo ppp0 ;;
"2") echo ppp1 ;;
"3") echo ppp2 ;;
"P") echo $_WAN_IF_ ;;
esac
;;
"1")
case "$WAN_Number" in
"1") echo ppp0 ;;
"2") echo ppp1 ;;
"3") echo ppp2 ;;
"P") echo $_GBAND_APCLI_IF_ ;;
esac
;;
esac
;;
"3"|"6")
case "$Operation_Mode" in
"0")
case "$WAN_Number" in
"1") echo ppp0 ;;
"P") echo $_WAN_IF_ ;;
esac
;;
"1")
case "$WAN_Number" in
"1") echo ppp0 ;;
"P") echo $_GBAND_APCLI_IF_ ;;
esac
;;
esac
;;
esac
}
. /bin/oem_script.sh
lan_interface()
{
echo "br0"
}
ip_of_interface()
{
if [ "$1" = "" ]; then
echo "0.0.0.0"
else
result="`ifconfig $1 | grep \"inet addr:\" | cut -f 2 -d : | cut -f 1 -d \" \"`"
if [ "$result" = "" ]; then
echo "0.0.0.0"
else
echo $result
fi
fi
}
security_log()
{
echo [`date +"%F %T"`]: [$SCRIPT_TITLE]: $1 >> /var/log/security
}
dhcp_discover()
{
result="`udhcpc -n -q -t 2 -T 2 -i $_WAN_IF_ | grep \"^Lease\" | cut -f 3 -d \" \"`"
if [ "$result" ]; then
echo "$result"
fi
}
pppoe_discover()
{
if [ "$WIZ_MODE" = "4" ];then
if [ "`pppoe -d -U -A -I $_WAN_IF_`" ]; then #Fix WISP disconnect pppoe issue
echo "y"
fi	
else
if [ "`pppoe -d -I $_WAN_IF_`" ]; then
echo "y"
fi	
fi	
}
iQsetup_ping()
{	
rm -rf /tmp/pingStatus 2> /dev/null
echo "2" > /tmp/pingStatus
if [ "`fping 8.8.8.8 | grep alive | wc -l`" = 1 ]; then  # Google DNS
checkInternet=ok 
elif [ "`fping 208.67.222.222 | grep alive | wc -l`" = 1 ]; then  # OpenDNS DNS
checkInternet=ok
else
checkInternet=fail
fi 
if [ "$checkInternet" = "ok" ]; then
echo "1" > /tmp/pingStatus
else
echo "2" > /tmp/pingStatus
fi
}
delay_ping()
{	
rm -f /tmp/pingStatus 2> /dev/null
echo "2" > /tmp/pingStatus
rm -f /tmp/DetectInternetDone 2> /dev/null
rm -f /tmp/after_delay_ping 2> /dev/null # for adhcp_and_pppoe.asp
rm -f /tmp/pppFail 2> /dev/null # for adhcp_and_pppoe.asp
eval `flash get OP_MODE`
eval `flash get WAN_MODE`
WAN=$_WAN_IF_
if [ "$OP_MODE" = '1' ];then
WAN=$_GBAND_APCLI_IF_
elif [ "$OP_MODE" = '3' ];then
WAN=$_ABAND_APCLI_IF_
fi
if [ "$WAN_MODE" = "2" ]; then
WAN=ppp0
fi
if [ "$WAN_MODE" = "3" ]; then
WAN=ppp0
fi
if [ "$WAN_MODE" = "6" ]; then
WAN=ppp0
fi
waitTime=0
IFgetIP=0
while [ "$waitTime" -le "$1" ]; do
sleep 2
waitTime=`expr $waitTime + 2`
if [ "`ifconfig | grep ${WAN} | wc -l`" != "0" ] && [ "`ifconfig ${WAN} | grep 'inet addr' | grep -v grep | wc -l`" != "0" ]; then
echo "--> $WAN get IP at ${waitTime}sec"
IFgetIP=1
break
fi
done
sleep 2
if [ "`fping 8.8.8.8 | grep alive | wc -l`" = 1 ]; then  # Google DNS
checkInternet=ok 
elif [ "`fping 208.67.222.222 | grep alive | wc -l`" = 1 ]; then  # OpenDNS DNS
checkInternet=ok
else
checkInternet=fail
fi 
echo "--> checkInternet ${checkInternet}"
if [ "$IFgetIP" = "1" ] && [ "$checkInternet" = "ok" ]; then
echo "1" > /tmp/pingStatus #connect
else
echo "2" > /tmp/pingStatus #disconnect
echo "1" > /tmp/pppFail # for adhcp_and_pppoe.asp
fi
echo 1 > /tmp/after_delay_ping # for adhcp_and_pppoe.asp
echo 1 > /tmp/DetectInternetDone
}
iQsetup_detect()
{
echo ">>> iQsetup_detect" 
yes=1
no=0
result=0
dhcpDiscover=$no
dhcpConnected=$no
pppoeDiscover=$no
rm -f /tmp/wanStatus
rm -f /tmp/DetectInternetDone
rm -f /tmp/pppFail # for adhcp_and_pppoe.asp
echo "5" > /tmp/wanStatus #patrick add init status for redirect.asp bug (if null, redirect will fail)
chmod 755 /tmp/wanStatus  #patrick add init status for redirect.asp bug (if null, redirect will fail)
disconnect.sh all
sleep 3
if [ "`pppoe_discover`" ]; then
pppoeDiscover=$yes
fi
if [ "`dhcp_discover`" ]; then
flash set WAN_MODE 0
killall -SIGUSR1 webs
dhcpDiscover=$yes
if [ "$OP_MODE" = "1" ]; then
echo "->dhcpc apcli0"
/bin/dhcpc.sh apcli0 wait &
elif [ "$OP_MODE" = "3" ]; then
echo "->dhcpc apclii0"
dhcpc.sh apclii0 wait &
else
echo "->dhcpc eth2.2"
dhcpc.sh eth2.2 wait &
fi
sleep 5
if [ "`fping 8.8.8.8 | grep alive | wc -l`" = 1 ]; then  # Google DNS
checkInternet=ok 
elif [ "`fping 208.67.222.222 | grep alive | wc -l`" = 1 ]; then  # OpenDNS DNS
checkInternet=ok
else
checkInternet=fail
fi 
if [ "$checkInternet" = "ok" ]; then
dhcpConnected=$yes #dhcp connected
fi
fi
if [ "$pppoeDiscover" = "$yes" ]; then
if [ "$dhcpDiscover" = "$yes" -a "$dhcpConnected" = "$yes" ]; then
echo "6" > /tmp/wanStatus #pppoe disconnected & dhcp connected
else
echo "4" > /tmp/wanStatus #pppoe disconnected
fi
elif [ "$dhcpDiscover" = "$yes" ]; then
if [ "$dhcpConnected" = "$yes" ]; then
echo "1" > /tmp/wanStatus #dhcp connected
else
echo "2" > /tmp/wanStatus #dhcp disconnected
fi
else
echo "5" > /tmp/wanStatus
fi
echo 1 > /tmp/DetectInternetDone
chmod 755 /tmp/wanStatus
}
iQsetup_set()
{
NEW_IP_ADDR=`flash get IP_ADDR | cut -d "'" -f 2`
mkdir -p /tmp/www
axhttpd -w /tmp/www &
echo "<html>" > /tmp/www/index.html
echo "<head>" >> /tmp/www/index.html
echo "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">" >> /tmp/www/index.html
echo "<meta http-equiv=\"Pragma\" content=\"no-cache\">" >> /tmp/www/index.html
echo "<meta http-equiv=\"cache-control\" content=\"no-cache\">" >> /tmp/www/index.html
echo "<meta http-equiv=\"cache-control\" content=\"no-store\">" >> /tmp/www/index.html
echo "<meta http-equiv=\"Expires\" content=\"-1\">" >> /tmp/www/index.html
echo "</head>" >> /tmp/www/index.html
echo "<script>" >> /tmp/www/index.html
echo "function asd()" >> /tmp/www/index.html
echo "{" >> /tmp/www/index.html
if [ "$_WIFI_REPEATER_" = "y" ]; then
echo "window.location.assign('http://edimaxext.setup/aIndex.asp');" >> /tmp/www/index.html
else
echo "window.location.assign('http://edimax.setup/aIndex.asp');" >> /tmp/www/index.html
fi
echo "}" >> /tmp/www/index.html
echo "</script>" >> /tmp/www/index.html
echo "<body onload=\"asd();\">" >> /tmp/www/index.html
echo "</body>" >> /tmp/www/index.html
echo "</html>" >> /tmp/www/index.html 
chmod 755 /tmp/www/index.html
echo "#!/bin/sh" > /tmp/iptables_build.sh
echo "currentIP=`ifconfig br0 | grep 'inet addr' | tr -s " " | cut -d ":" -f 2 | cut -d " " -f 1`" >> /tmp/iptables_build.sh
echo "iptables -t nat -D PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 80 -j DNAT --to \$currentIP" >> /tmp/iptables_build.sh
echo "iptables -t nat -D PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 443 -j DNAT --to \$currentIP" >> /tmp/iptables_build.sh
echo "iptables -t nat -I PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 80 -j DNAT --to \$currentIP" >> /tmp/iptables_build.sh
echo "iptables -t nat -I PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 443 -j DNAT --to \$currentIP" >> /tmp/iptables_build.sh
chmod 755 /tmp/iptables_build.sh
echo "#!/bin/sh" > /tmp/iptables_release.sh     
echo "currentIP=`ifconfig br0 | grep 'inet addr' | tr -s " " | cut -d ":" -f 2 | cut -d " " -f 1`" >> /tmp/iptables_release.sh
echo "iptables -t nat -D PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 80 -j DNAT --to \$currentIP" >> /tmp/iptables_release.sh
echo "iptables -t nat -D PREROUTING -i br0 ! -d \$currentIP --proto tcp --dport 443 -j DNAT --to \$currentIP" >> /tmp/iptables_release.sh
chmod 755 /tmp/iptables_release.sh
sh /tmp/iptables_build.sh
}
ap_wifi_Setting()
{
last_two_mac=`flash all | grep HW_WLAN_ADDR | cut -d "=" -f2 | cut -b 11-`	
newSSID="edimax.setup $last_two_mac"
echo "[ 2.4G SSID = $newSSID ]"
flash set SSID "$newSSID"
last_two_mac=`flash all | grep HW_INIC_ADDR | cut -d "=" -f2 | cut -b 11-`	
newSSID="edimax.setup5G $last_two_mac"
echo "[ 5G SSID = $newSSID ]"
flash set INIC_SSID "$newSSID"
if [ "$_MSSID_SUPPORT_GUEST_" != "y" ]; then
flash set INIC_SSID_1 edimax5G.1
flash set SSID_1 edimax.1
fi
wifi_key=`flash get HW_MODEL_NAME | cut -d "=" -f2 | cut -d "'" -f 2`
if [ "$wifi_key" = "" ]; then
wifi_key="12345678"
fi
echo "[ WiFi key = $wifi_key ]"
flash set WLAN_WPA_PSK "$wifi_key"
flash set INIC_WPA_PSK "$wifi_key"
}
Init_for_IQv2()
{
if [ "$1" = "checkDefSetting" ]; then
if [ "$INIC_DISABLED" = "1" ]; then
echo 0 > /tmp/Ap_enable5g
echo 0 > /tmp/APcl_enable5g
else
echo 1 > /tmp/Ap_enable5g
echo 1 > /tmp/APcl_enable5g
fi
if [ "$WLAN_DISABLED" = "1" ]; then
echo 0 > /tmp/Ap_enable2g
echo 0 > /tmp/APcl_enable2g
else
echo 1 > /tmp/Ap_enable2g
echo 1 > /tmp/APcl_enable2g
fi
echo 1 > /tmp/rep_enable2g
echo 1 > /tmp/rep_enable2g
echo 0 > /tmp/wiz_gwManu	
echo 0 > /tmp/wiz_wispManu	
echo 0 > /tmp/wizmode_value	
echo 0 > /tmp/WISP_enable5g	
echo 0 > /tmp/WISP_enable2g	
echo 0 > /tmp/pause_iQsetup  # 1: pause
if [ "$_APMODE_PLUG_AND_PLAY_" = "y" -a "$IQSET_DISABLE" = "2" ]; then # IQSET_DISABLE=2 is APMODE_PLUG_AND_PLAY default
echo "[ APMODE_PLUG_AND_PLAY First setting ]"
ap_wifi_Setting
flash set IQSET_DISABLE 1
fi
if [ "$IQSET_DISABLE" = "0" ]; then
flashDefault
if [ "$_APMODE_PLUG_AND_PLAY_" = "y" ]; then
ap_wifi_Setting
flash set IS_RESET_DEFAULT 0
flash set IQSET_DISABLE 0
flash set WIZ_MODE 0
flash set AP_ROUTER_SWITCH 0
fi			
if [ "$_WIFI_REPEATER_" = "y" ] && [ "$_WIFI_ROMAING_" != "y" ]; then
last_two_mac=`flash all | grep HW_WLAN_ADDR= | cut -d "=" -f2 | cut -b 11-`	
newSSID="edimaxEXT.setup $last_two_mac"
flash set SSID "$newSSID"
last_two_mac=`flash all | grep HW_INIC_ADDR= | cut -d "=" -f2 | cut -b 11-`	
newSSID="edimaxEXT.setup5G $last_two_mac"
flash set INIC_SSID "$newSSID"
fi
if [ "$_WIFI_ROMAING_" = "y" ]; then
flash set SECURITY_MODE 3
flash set WLAN_WPA_CIPHER_SUITE 0
flash set WPA2_CIPHER_SUITE 2
flash set WLAN_WPA_PSK "$HW_MODEL_NAME"
flash set SSID "$HW_COUNTRY_CODE"
flash set INIC_SECURITY_MODE 3
flash set INIC_WPA_CIPHER_SUITE 0
flash set INIC_WPA2_CIPHER_SUITE 2
flash set INIC_WPA_PSK "$HW_MODEL_NAME"
flash set INIC_SSID "$HW_COUNTRY_CODE"
fi
else
if [ "$IS_RESET_DEFAULT" = 0 ]; then
if [ "$WIZ_MODE" = 1 -a "$_MSSID_SUPPORT_GUEST_" != "y" ]; then
flash set INIC_SSID_1 edimax5G.1
flash set SSID_1 edimax.1
fi
flash set IS_RESET_DEFAULT 1
fi
fi
rm -f /var/flash.inc
fi
if [ "$1" = "checkRunIQsetup" ]; then
if [ "$IQSET_DISABLE" = "0" ]; then
echo "[ Enable iQsetup ]"
kill -SIGUSR2 `pidof dnrd`
iQsetup_set
/bin/run_iqv2.sh &
else
echo "[ Disable iQsetup ]"
fi
fi
}
set_wireless_Schedule()
{
flash getWireSchSet 0 $1
chmod 777 /tmp/wireSchSet.sh
./tmp/wireSchSet.sh
}
update_systemtime()
{
if [ "$1" != "" ]; then
profile="/var/cron/crontabs/root"
echo "*/$1 * * * * /bin/sntp.sh &" >> $profile 
echo ">>> Update system time every $1min"
else
echo "Please input correct update interval(min)"
return 1;
fi
}
GetNewFwVersion()
{
if [ "$1" = "start" ]; then
/bin/autoCheckFW.sh "$1" &
elif [ "$1" = "check" ]; then
charMAC=`flash get HW_NIC0_ADDR |  cut -d "=" -f2 | cut -b 12`
intMAC="`hex_dec_convert hex2dec $charMAC`" #Max is 15
CheckWeek=`expr "${intMAC}" % 7 + 1` #1~7
profile="/var/cron/crontabs/root"
echo "0 0 * * $CheckWeek /bin/autoCheckFW.sh $1 &" >> $profile 
echo ">>> Check new fw version is every week ${CheckWeek} 00:00."
else
echo "Usage: /bin/autoCheckFW.sh [start|check]"
fi
}
telnet()
{
if [ "$1" = "up" ]; then
mount -t devpts devpts /dev/pts
telnetd -l /bin/sh
fi
if [ "$1" = "down" ]; then
killall telnetd
fi
}
all_led_on()
{
killall led.sh 2> /dev/null
sleep 1
echo "LED ON" > /dev/ABAND_LED
echo "LED ON" > /dev/GBAND_LED
echo "LED ON" > /dev/WlanLED
echo "LED ON" > /dev/PowerLED
echo "LED ON" > /dev/WPS_LED
echo "LED ON" > /dev/LAN_LED
}
all_led_off()
{
killall led.sh 2> /dev/null
sleep 1
echo "LED OFF" > /dev/ABAND_LED
echo "LED OFF" > /dev/GBAND_LED
echo "LED OFF" > /dev/WlanLED
echo "LED OFF" > /dev/PowerLED
echo "LED OFF" > /dev/WPS_LED
echo "LED OFF" > /dev/LAN_LED
}
GetUplinkRssistrength()
{
Rssi_strength=0
if [ "`IFstatus $1`" = "down" ]; then
Rssi_strength="disable"
echo "$Rssi_strength" > /tmp/"$1"RssiStrength
echo "$Rssi_strength"
else
rootAPmac="`iwconfig $1 | grep Mode | tr -s " " | cut -f6 -d ' '`"
if [ "$rootAPmac" = "Not-Associated" ]; then
DisconnectCount=`cat /tmp/$1_DisconnectCount`			
if [ "$DisconnectCount" -ge 6 ]; then
Rssi_strength="NotAssociated"
echo "$Rssi_strength" > /tmp/"$1"RssiStrength
echo "$Rssi_strength"
else
echo "`cat /tmp/"$1"RssiStrength`"
fi
else	
uplink_signal="`iwpriv $1 stat | grep RSSI| tr -s " " | cut -f 3 -d ' '`"
if [ "$_SIGNAL_FORMULA_" = "y" ]; then
if [ $uplink_signal -ge -50 ]; then   # Rssi  >= -50dbm
Rssi_strength=100
elif [ $uplink_signal -gt -99 ]; then
Rssi_strength_Buff=`expr $uplink_signal + 114`
echo "$Rssi_strength_Buff" > /tmp/"$1"Rssi_strength_Buff
if [ $Rssi_strength_Buff -ge 50 ]; then
Rssi_strength=100
else
Rssi_strength=`expr $Rssi_strength_Buff \* 2`
echo "$Rssi_strength" > /tmp/"$1"strength
fi
else 
Rssi_strength=0
fi
else
if [ $uplink_signal -ge -50 ]; then   # Rssi  >= -50dbm
Rssi_strength=100
elif [ $uplink_signal -gt -80 ]; then # Rssi between -51 ~ -79dbm
Rssi_strength=$(( ( ( (uplink_signal + 80) * 26 ) / 10 ) + 24 ))
elif [ $uplink_signal -gt -90 ]; then # Rssi between -80 ~ -89dbm
Rssi_strength=$(( ( (uplink_signal + 90) * 26 ) / 10 ))
else 
Rssi_strength=0
fi
fi
echo "$Rssi_strength" > /tmp/"$1"RssiStrength
echo "$Rssi_strength"
fi
fi
}
openvpn_data_structure()
{
mkdir -p /dev/net
mknod /dev/net/tun c 10 200
mkdir -p /tmp/Cloud
mkdir -p /tmp/vpnkeys_def
mkdir -p /tmp/vpnkeys_user
mkdir -p /tmp/openvpn/keys/user
mkdir -p /tmp/openvpn/keys/def
mkdir -p /tmp/openvpn/account/user1
mkdir -p /tmp/openvpn/account/user2
mkdir -p /tmp/openvpn/account/user3
mkdir -p /tmp/openvpn/account/user4
mkdir -p /tmp/openvpn/account/user5
mkdir -p /tmp/openvpn/account/user6
mkdir -p /tmp/openvpn/account/user7
mkdir -p /tmp/openvpn/account/user8
echo "" > /tmp/openvpn/account_connection_status
openvpn_key.sh SetupKey all
}
flash_to_data_structure_for_openvpn_user()
{
eval `flash get OPENVPN_USER_NUM`
eval `flash get USER_NAME`
eval `flash get USER_PASSWORD`
if [ "${OPENVPN_USER_NUM}" = "0" ]; then
flash set OPENVPN_USER_TBL add "${USER_NAME}" "${USER_PASSWORD}" 1
else
OPENVPN_USER_DATA=`flash OpenvpnUserList 1`
OPENVPN_USER_NAME=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 1`
OPENVPN_USER_PASSWORD=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 2`
OPENVPN_USER_ENABLED=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 3`
if [ "${USER_NAME}" != "${OPENVPN_USER_NAME}" -o "${USER_PASSWORD}" != "${OPENVPN_USER_PASSWORD}" ]; then
flash set OPENVPN_USER_TBL edit 1 "${USER_NAME}" "${USER_PASSWORD}" "${OPENVPN_USER_ENABLED}"
fi
fi
eval `flash get OPENVPN_USER_NUM`
for i in 1 2 3 4 5 6 7 8
do
if [ "${i}" -le "${OPENVPN_USER_NUM}" ]; then
OPENVPN_USER_DATA=`flash OpenvpnUserList ${i}`
OPENVPN_USER_NAME=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 1`
OPENVPN_USER_PASSWORD=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 2`
OPENVPN_USER_ENABLED=`echo ${OPENVPN_USER_DATA} |cut -d "," -f 3`
echo "$OPENVPN_USER_ENABLED" > /tmp/openvpn/account/user"${i}"/ENABLED
echo "$OPENVPN_USER_NAME" > /tmp/openvpn/account/user"${i}"/NAME
echo "$OPENVPN_USER_PASSWORD" > /tmp/openvpn/account/user"${i}"/PASSWORD
echo "0" > /tmp/openvpn/account/user"${i}"/LOGIN
echo "" > /tmp/openvpn/account/user"${i}"/IP
else
echo "0" > /tmp/openvpn/account/user"${i}"/ENABLED
echo "" > /tmp/openvpn/account/user"${i}"/NAME
echo "" > /tmp/openvpn/account/user"${i}"/PASSWORD
echo "0" > /tmp/openvpn/account/user"${i}"/LOGIN
echo "" > /tmp/openvpn/account/user"${i}"/IP
fi
done
}
check_openvpn_account()
{
FILE_STATUS_LOG=/tmp/openvpn/account_connection_status
for i in 1 2 3 4 5 6 7 8
do
FILE_USER_ENABLED=/tmp/openvpn/account/user"${i}"/ENABLED
FILE_USER_NAME=/tmp/openvpn/account/user"${i}"/NAME
FILE_USER_LOGIN=/tmp/openvpn/account/user"${i}"/LOGIN
FILE_USER_IP=/tmp/openvpn/account/user"${i}"/IP
USER_ENABLED=`cat "${FILE_USER_ENABLED}"`
USER_NAME=`cat "${FILE_USER_NAME}"`
if [ "${USER_ENABLED}" = "1" ]; then
cat "${FILE_STATUS_LOG}" |grep "\["${USER_NAME}"\]" |cut -d "," -f 2 > "${FILE_USER_IP}"
if [ "`cat ${FILE_USER_IP}`" != "" ]; then
echo "1" > "${FILE_USER_LOGIN}"
else
echo "0" > "${FILE_USER_LOGIN}"
fi
fi
done
}
GetphyStat()
{
echo "`GetLinkStat $1 $2`"
}
currnetIFstatus()
{
echo "`IFstatus $1`"
}
start_localsearch_agent()
{
echo "start_localsearch_agent()"
if [ "$REPEATER_ENABLED" = 1 -a "$DHCP_SWITCH" = "0" ]; then 
eval `flash get DEFAULT_GATEWAY`
GW=$DEFAULT_GATEWAY
if [ "$INIC_REPEATER_ENABLED" = 1 ]; then
echo $GW > /tmp/br1_gw
else
echo $GW > /tmp/br0_gw
fi
fi
if [ "$DHCP_SWITCH_5G" = "0" -a "$INIC_REPEATER_ENABLED" = 1 ]; then
eval `flash get DEFAULT_GATEWAY_5G`
GW=$DEFAULT_GATEWAY_5G
echo $GW > /tmp/br0_gw
fi
flash appagent &
}
setTxpower()
{
if [ "`IFstatus $1`" = "up" ]; then
if [ "$1" = "rai0" ]; then
Current_TxPower=$INIC_TX_POWER
elif [ "$1" = "ra0" ]; then
Current_TxPower=$WLAN_TX_POWER
fi
Update_TxPower=`expr $Current_TxPower / $2`
echo "Set $1 Transmit Power: $Update_TxPower"
iwpriv $1 set TxPower=$Update_TxPower
fi
}
RJ45port_control()
{
if [ "$2" = "down" ]; then
mii_mgr -s -p $1 -r 0 -v 3900
fi
if [ "$2" = "up" ]; then
mii_mgr -s -p $1 -r 0 -v 3300
fi
}
iptables_l7_driver()
{
echo "iptables_l7_driver"
XT_Layer7_KO=`lsmod |grep xt_layer7 |wc -l`
if [ "$XT_Layer7_KO" = "0" ];then
insmod /bin/xt_layer7.ko
fi
}
usb()
{
if [ "$1" = "setup" ]; then
dmesg -c > /dev/null
echo "1" > /tmp/rebootStatus
rm -rf /tmp/kc_smb
rm -rf /tmp/smbConfig
sleep 1
if [ "$FTP_ANON_ENABLE" = "1" ]; then
cp -f /bin/kc_smb /tmp
cp -f /bin/smbConfig /tmp
else
cp -f /bin/ACCOUNT_SAMBA/kc_smb /tmp
cp -f /bin/ACCOUNT_SAMBA/smbConfig /tmp
fi
eval `flash get SAMBA_HOSTNAME`
eval `flash get SAMBA_GROUP`
eval `flash get USB_ACC_NUM`
ACCOUNTNUM=$USB_ACC_NUM
if [ "$FTP_ANON_ENABLE" = "1" ]; then
if [ "$SAMBA_HOSTNAME" != "MyStorage" ] || [ "$SAMBA_GROUP" != "Workgroup" ]; then
echo HostName=\""$SAMBA_HOSTNAME"\" > /tmp/smbConfig
echo GroupName=\""$SAMBA_GROUP"\" >> /tmp/smbConfig
echo AuthLevel=\""0"\" >> /tmp/smbConfig
echo MountRoot=\""/tmp/SMBshare/"\" >> /tmp/smbConfig
fi
else
echo HostName=\""$SAMBA_HOSTNAME"\" > /tmp/smbConfig
echo GroupName=\""$SAMBA_GROUP"\" >> /tmp/smbConfig
echo MountRoot=\""/tmp/SMBshare/"\" >> /tmp/smbConfig
echo UserName=\""NoUse"\" >> /tmp/smbConfig
echo UserPassword=\""1234"\" >> /tmp/smbConfig
while [ "$ACCOUNTNUM" -ge 1 ]; do
SAMBA_ADMIN_ACCOUNT=`flash all|grep ACC_TBL$ACCOUNTNUM|cut -d = -f 2|cut -d \" -f 2|cut -d , -f 1`
SAMBA_ADMIN_PASSWORD=`flash all|grep ACC_TBL$ACCOUNTNUM|cut -d = -f 2|cut -d \" -f 2|cut -d , -f 2`
SAMBA_ADMIN_RW=`flash all|grep ACC_TBL$ACCOUNTNUM|cut -d = -f 2|cut -d \" -f 2|cut -d , -f 3`
echo AccountUser=\""$SAMBA_ADMIN_ACCOUNT"\" >> /tmp/smbConfig
echo AccountPass=\""$SAMBA_ADMIN_PASSWORD"\" >> /tmp/smbConfig
if [ "$SAMBA_ADMIN_RW" = "0" ]; then	#Read & Write
echo AccountRight=\""7"\" >> /tmp/smbConfig
elif [ "$SAMBA_ADMIN_RW" = "1" ]; then	#Read Only
echo AccountRight=\""1"\" >> /tmp/smbConfig
fi
ACCOUNTNUM=`expr $ACCOUNTNUM - 1`
done		
fi
/bin/USBModeBox.sh &
if [ "$PRINT_STORAGE_RADIO" = "0" -a "$USB_ENABLED" = "1" ]; then
/bin/UnofficialUSBdisable.sh &
fi
fi
if [ "$1" = "mount" ]; then # for mp test
if [ "$FTP_ANON_ENABLE" = "1" ]; then
cp -f /bin/kc_smb /tmp
cp -f /bin/smbConfig /tmp
else
cp -f /bin/ACCOUNT_SAMBA/kc_smb /tmp
cp -f /bin/ACCOUNT_SAMBA/smbConfig /tmp
fi
/bin/USBModeBox.sh &
fi
if [ "$1" = "umount" ]; then # for mp test
umount /dev/sda* 2>/dev/null
fi
}
