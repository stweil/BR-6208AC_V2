#!/bin/bash
. ../define/FUNCTION_SCRIPT

driver=(
'mt7610_ap'    	 # link to mt7610
'rlt_wifi_ap'    # link to mt7612
'rt2860v2_ap'    # link to mt7620
);
#init driver version 
version=(
'3.0.0.8'		 # mt7610
'3.0.3.2'    # mt7612
'2.7.2.0'    # mt7620
);

T_SDKVERSION=${1}
#driver version array mapping to _WIRELESS_DRIVER_VERSION_
if [ "$T_SDKVERSION" = "4200" ]; then
	echo "set T_SDKVERSION: $T_SDKVERSION" 
	wireless_driver_set=(
		'3.0.0.8 3.0.3.2 2.7.2.0'	         # MT7610 MT7612, MT7620
	);
fi

wireless_driver=${wireless_driver_set[$_WIRELESS_DRIVER_VERSION_]}
#driver init version will rewrite by _WIRELESS_DRIVER_VERSION_
if [ "$wireless_driver" != "" ]; then
	version[0]=`echo $wireless_driver | cut -f 1 -d ' '`
	version[1]=`echo $wireless_driver | cut -f 2 -d ' '`
	version[2]=`echo $wireless_driver | cut -f 3 -d ' '`
fi


item=`echo ${#driver[*]}`

rm -f mt7610_ap
rm -f rlt_wifi
rm -f rlt_wifi_ap
rm -f rt2860v2
rm -f rt2860v2_ap

for ((i = 0; i < $item; i = i + 1)) do

	echo 'wireless driver version -> '${driver[$i]}':'${version[$i]}''

	rm -f ${driver[$i]}
	if [ ${driver[$i]} = "rt2860v2_ap" ];then
		rm -f rt2860v2
		ln -s wireless/mt7620_ap/${version[$i]}/${driver[$i]} ${driver[$i]}
		ln -s wireless/mt7620_ap/${version[$i]}/rt2860v2 rt2860v2
	elif [ ${driver[$i]} = "rlt_wifi_ap" ];then
		rm -f rlt_wifi
		ln -s wireless/mt7612_ap/${version[$i]}/${driver[$i]} ${driver[$i]}
		ln -s wireless/mt7612_ap/${version[$i]}/rlt_wifi rlt_wifi
	elif [ ${driver[$i]} = "mt7610_ap" ];then
		ln -s wireless/${driver[$i]}/${version[$i]} ${driver[$i]}
	else
		echo "driver link error !!"
	fi
done



