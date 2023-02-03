<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<SCRIPT>

//var fwVersion = "<% getInfo("fwVersion"); %>";
//var model = "<% getInfo("model"); %>";

//var wizMode = <%getVar("wizmode","","");%>;                 /* 0-Router, 1-AP, 2-Repeater, 3-WiFi Bridge, 4-WISP */
//var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;          /* 0-Run IQsetup, 1-Close IQsetup */

//var ntps_config = <%getVar("ntpsconfig","","");%>;           /* 0-NTPS unconfig, 1-NTPS config */
//var DayLightEnable =  <%getVar("DayLightEnable","","");%>;   /* 0-Daylight Savings OFF, 1-Daylight Savings ON */


//var Gband_apMode = <%getVar("apMode","","");%>;             
//var Aband_apMode = <%getiNICVar("apMode","","");%>;

var Gband_secModeTbl = <%getInfo("secModeAll");%>;
var Aband_secModeTbl = <%getiNICInfo("secModeAll");%>;
//var Gband_M_methodVal = Gband_secModeTbl[0];                /* Gband Main SSID security: 0-(Disabl), 1-(WEP), 2-(WPA), 3-(WPA2) */
//var Gband_G_methodVal = Gband_secModeTbl[1];                /* Gband Guest SSID security: 0-(Disabl), 2-(WPA2)                  */
//var Aband_M_methodVal = Aband_secModeTbl[0];                /* Aband Main SSID security: 0-(Disabl), 1-(WEP), 2-(WPA), 3-(WPA2) */
//var Aband_G_methodVal = Aband_secModeTbl[1];                /* Aband Guest SSID security: 0-(Disabl), 2-(WPA2)                  */

//var Gband_bandVal = <%getVar("band","","");%>;              /* Gband channel: 1-(B), 2-(N),3-(B+G), 4-(G), 5-(B+G+N)     */
//var Aband_bandVal = <%getiNICVar("band","","");%>;              /* Aband channel: 1-(A), 2-(N),3-(A+N), 4-(N+AC), 5-(A+N+AC) */

//var Gband_wlDisable = <%getVar("wlanDisabled","","");%>;     /* 0-Gband SSID enable, 1-Gband SSID disable */
//var Aband_wlDisable = <%getiNICVar("wlanDisabled","","");%>;     /* 0-Aband SSID enable, 1-Aband SSID disable */


var Gband_wpaCipherTbl =<%getInfo("wpaCipherAll");%>;
var Gband_pskFormatTbl =<%getInfo("pskFormatAll");%>;
var Gband_pskValueTbl =	<%getInfo("pskValueAll");%>;
var Aband_wpaCipherTbl =<%getiNICInfo("wpaCipherAll");%>;
var Aband_pskFormatTbl =<%getiNICInfo("pskFormatAll");%>;
var Aband_pskValueTbl =	<%getiNICInfo("pskValueAll");%>;
//var Gband_M_wpaCipher = Gband_wpaCipherTbl[0];               /* Gband Main SSID WPA/WPA2 type: 1-(TKIP), 2-(AES)  */
//var Gband_G_wpaCipher = Gband_wpaCipherTbl[1];               /* Gband Guest SSID WPA/WPA2 type: 1-(TKIP), 2-(AES) */
//var Gband_M_pskFormat = Gband_pskFormatTbl[0];
//var Gband_G_pskFormat = Gband_pskFormatTbl[1];
//var Gband_M_pskValue = Gband_pskValueTbl[0];                 /* Gband Main SSID WPA/WPA2 key  */
//var Gband_G_pskValue = Gband_pskValueTbl[1];                 /* Gband Guest SSID WPA/WPA2 key */
//var Aband_M_wpaCipher = Aband_wpaCipherTbl[0];               /* Aband Main SSID WPA/WPA2 type: 1-(TKIP), 2-(AES)  */
//var Aband_G_wpaCipher = Aband_wpaCipherTbl[1];               /* Aband Main SSID WPA/WPA2 type: 1-(TKIP), 2-(AES)  */
//var Aband_M_pskFormat = Aband_pskFormatTbl[0];
//var Aband_G_pskFormat = Aband_pskFormatTbl[1];
//var Aband_M_pskValue = Aband_pskValueTbl[0];                 /* Aband Main SSID WPA/WPA2 key  */
//var Aband_G_pskValue = Aband_pskValueTbl[1];                 /* Aband Guest SSID WPA/WPA2 key */

var Gband_wepValueAll = <% getInfo("wepValueAll");%>;
//var Gband_M_WEP64Key1 = Gband_wepValueAll[0];                /* Gband Main SSID WEP(64) key   */
//var Gband_M_WEP128Key1 = Gband_wepValueAll[4];               /* Gband Main SSID WEP(128) key  */
//var Gband_G_WEP64Key1 = Gband_wepValueAll[8];                /* Gband Guest SSID WEP(64) key  */
//var Gband_G_WEP128Key1 = Gband_wepValueAll[12];               /* Gband Guest SSID WEP(128) key*/

var Gband_weplengthAll = <%getInfo("wepAll");%>;    
var Gband_wepkeyTypeTbl =<%getInfo("keyTypeAll");%>;                
//var Gband_M_weplengt = Gband_weplengthAll[0];                 /* Gband Main SSID WEP length: 1-64 bit, 2-128 bit  */
//var Gband_G_weplengt = Gband_weplengthAll[1];                 /* Gband Guest SSID WEP length: 1-64 bit, 2-128 bit */
//var Gband_M_wepkeyType = Gband_wepkeyTypeTbl[0];              /* Gband Main SSID WEP type: 0-ASCII, 1-HEX  */
//var Gband_G_wepkeyType = Gband_wepkeyTypeTbl[1];              /* Gband Guest SSID WEP type: 0-ASCII, 1-HEX */

/* WEP rule
 if(weplengt is 1){
		if(wepkeyType is 0) 
			wepkey=WEP64Key1.substring(0,5);   // 5 characters
		else
			wepkey=WEP64Key1;                  // 10 characters
	}
	else{
		if(wepkeyType is 0) 
			wepkey=WEP128Key1.substring(0,13); // 13 characters
		else
			wepkey=WEP128Key1;                 // 26 characters
	}
*/

var Aband_wepValueAll = <% getiNICInfo("wepValueAll");%>;
//var Aband_M_WEP64Key1 = Aband_wepValueAll[0];                /* Aband Main SSID WEP(64) key   */
//var Aband_M_WEP128Key1 = Aband_wepValueAll[4];               /* Aband Main SSID WEP(128) key  */
//var Aband_G_WEP64Key1 = Aband_wepValueAll[8];                /* Aband Guest SSID WEP(64) key  */
//var Aband_G_WEP128Key1 = Aband_wepValueAll[12];              /* Aband Guest SSID WEP(128) key */

var Aband_weplengthAll = <%getiNICInfo("wepAll");%>;        
var Aband_wepkeyTypeTbl = <%getiNICInfo("keyTypeAll");%>;         
//var Aband_M_weplengt = Aband_weplengthAll[0];                 /* Aband Main SSID WEP length: 1-64 bit, 2-128 bit  */
//var Aband_G_weplengt = Aband_weplengthAll[1];                 /* Aband Guest SSID WEP length: 1-64 bit, 2-128 bit */
//var Aband_M_wepkeyType = Aband_wepkeyTypeTbl[0];              /* Aband Main SSID WEP type: 0-ASCII, 1-HEX  */
//var Aband_G_wepkeyType = Aband_wepkeyTypeTbl[1];              /* Aband Guest SSID WEP type: 0-ASCII, 1-HEX */

var Gband_ssidTbl = <%getInfo("ssidAll");%>;
var Aband_ssidTbl=<%getiNICInfo("ssidAll");%>;
//var Gband_M_ssid = Gband_ssidTbl[0];                         /* Gband Main SSID Name */
//var Gband_G_ssid = Gband_ssidTbl[1];                         /* Gband Guest SSID Name */
//var Aband_M_ssid = Aband_ssidTbl[0];                         /* Aband Main SSID Name */
//var Aband_G_ssid = Aband_ssidTbl[1];                         /* Aband Guest SSID Name */

var Gband_GssidenableTbl = <%getInfo("ssidMirrorAll");%>; 
var Aband_GssidenableTbl = <%getiNICInfo("ssidMirrorAll");%>;
//var Gband_Gssidenable = Gband_GssidenableTbl[0];               /* (!=0) -Gband Guest SSID enable, (0)-Gband Guest SSID disable */
//var Aband_Gssidenable = Aband_GssidenableTbl[0];               /* (!=0) -Aband Guest SSID enable, (0)-Aband Guest SSID disable */

//var Gband_Chan = <%getVar("channel","","")%>;                /* Gband Main/Guest SSID channel */
//var Aband_Chan = <%getiNICInfo("channel","","")%>;                /* Aband Main/Guest SSID channel */


//var Gband_rssi = "<% getInfo("get_apcli0_rssi"); %>";
//var Aband_rssi = "<% getInfo("get_apclii0_rssi"); %>";
//var Gband_connectStatus = "<% getInfo("connectStatus2G"); %>";   /* (NotAssociated)-disconnect, (!=NotAssociated) -connect */
//var Aband_connectStatus = "<% getInfo("connectStatus5G"); %>"; /* (NotAssociated)-disconnect, (!=NotAssociated) -connect */


function showWEPkey(weplengt, wepkeyType, wep64key,  wep128key, idname){

	with(document.app) {
		if (weplengt == 1) { /* length: 1-64 bit, 2-128 bit  */
			if (wepkeyType == 0) /* type: 0-ASCII, 1-HEX  */
				document.getElementById(idname).value = wep64key.substring(0,5);
			else 
				document.getElementById(idname).value = wep64key;
		}
		else{
			if (wepkeyType == 0) 
				document.getElementById(idname).value = wep128key.substring(0,13);
			else 
				document.getElementById(idname).value = wep128key;
		}
	}
}

</script>
</head>
<body>	
<form name="app">

	<input type="hidden" name="fwVersion" value="<% getInfo("fwVersion"); %>">
	<input type="hidden" name="model" value="<% getInfo("model"); %>">
	<input type="hidden" name="wizmode" value="<%getVar("wizmode","","");%>">
	<input type="hidden" name="iqsetupfinish" value="<%getVar("IQsetupDisable","","");%>">
	<input type="hidden" name="Gband_wlDisable" value="<%getVar("wlanDisabled","","");%>">
	<input type="hidden" name="Aband_wlDisable" value="<%getiNICVar("wlanDisabled","","");%>">
	<input type="hidden" name="Gband_Chan" value="<%getVar("channel","","");%>">
	<input type="hidden" name="Aband_Chan" value="<%getiNICVar("channel","","");%>">
	<input type="hidden" name="Gband_bandVal" value="<%getVar("band","","");%>">
	<input type="hidden" name="Aband_bandVal" value="<%getiNICVar("band","","");%>">

	<input type="hidden" name="Gband_channelIndex" value="<%getVar("regDomain","","");%>">
	<input type="hidden" name="Aband_channelIndex" value="<%getiNICVar("regDomain","","");%>">

	<input type="hidden" name="ntpsconfig" value="<%getVar("ntpsconfig","","");%>">
	<input type="hidden" name="DayLightEnable" value="<%getVar("DayLightEnable","","");%>">
	<input type="hidden" name="TimeZoneSelget" value="<% getInfo("TimeZoneSel"); %>">
	<input type="hidden" name="ServerSelget" value="<% getInfo("ServerSel"); %>">
	<input type="hidden" name="startMonthget" value="<% getInfo("startMonth"); %>">
	<input type="hidden" name="startDayget" value="<% getInfo("startDay"); %>">
	<input type="hidden" name="endMonthget" value="<% getInfo("endMonth"); %>">
	<input type="hidden" name="endDayget" value="<% getInfo("endDay"); %>">
	
	<input type="hidden" name="Gband_rssi" value="<% getInfo("get_apcli0_rssi"); %>">
	<input type="hidden" name="Aband_rssi" value="<% getInfo("get_apclii0_rssi"); %>">
	<input type="hidden" name="Gband_connectStatus" value="<% getInfo("connectStatus2G"); %>">
	<input type="hidden" name="Aband_connectStatus" value="<% getInfo("connectStatus5G"); %>">

	<input type="hidden" name="useCrossBand" value="<%getInfo("useCrossBand");%>">

	<input type="hidden" name="Gband_M_ssid" value="<% getInfo("ssidArr"); %>">
	<input type="hidden" name="Gband_G_ssid" value="<% getInfo("ssid1Arr"); %>">
	<input type="hidden" name="Aband_M_ssid" value="<% getiNICInfo("ssidArr"); %>">
	<input type="hidden" name="Aband_G_ssid" value="<% getiNICInfo("ssid1Arr"); %>"> 

	<input type="hidden" name="Gband_M_methodVal" value="<%getVar("encrypt","","");%>">
	<input type="hidden" name="Gband_G_methodVal" value="<%getVar("encrypt1","","");%>">
	<input type="hidden" name="Aband_M_methodVal" value="<%getiNICVar("encrypt","","");%>">
	<input type="hidden" name="Aband_G_methodVal" value="<%getiNICVar("encrypt1","","");%>">

	<input type="hidden" name="Gband_G_wpakey" value="<% getInfo("pskValue1Arr"); %>">
	<input type="hidden" name="Aband_G_wpakey" value="<% getiNICInfo("pskValue1Arr"); %>">

	<input type="hidden" name="Gband_Gssidenable" value="<% getInfo("gusetssidenable"); %>">
	<input type="hidden" name="Aband_Gssidenable" value="<% getiNICInfo("gusetssidenable"); %>">

</form>
</body>
</html>
