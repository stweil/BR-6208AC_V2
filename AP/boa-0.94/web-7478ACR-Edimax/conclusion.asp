<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="file/multilanguage.var"></script>
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var wanMode = <%getVar("wanMode","","");%>;

var pppUsername="<%getInfo("pppUserName");%>";
var pppPassWord="<%getInfo("pppPassword");%>";

var pptpUsername="<%getInfo("pptpUserName");%>";
var pptpPassWord="<%getInfo("pptpPassword");%>";

var l2tpUsername="<%getInfo("L2TPUserName");%>";
var l2tpPassWord="<%getInfo("L2TPPassword");%>";

var telBUsername="<%getInfo("telBPUserName");%>";
var telBPassWord="<%getInfo("telBPPassword");%>";

var wlanDisable = <%getiNICVar("wlanDisabled","","");%>;
var wlan24Disable = <%getVar("wlanDisabled","","");%>;

var pskValueTbl =	<%getInfo("pskValueAll");%>;
var psk5GValueTbl =	<%getiNICInfo("pskValueAll");%>;

var securityModeAll =	<%getInfo("secModeAll");%>;
var securityModeAll5g =	<%getiNICInfo("secModeAll");%>;

var ConnectKey = <%getInfo("ConnectKey");%>; 
var ConnectKey5g =	<%getInfo("ConnectKey5g");%>;

var Apenable5g = "<%getInfo("Ap_enable5g");%>";
var Apenable2g = "<%getInfo("Ap_enable2g");%>";

var repenable5g = "<%getInfo("rep_enable5g");%>";
var repenable2g = "<%getInfo("rep_enable2g");%>";
var rep_EnableCrossBand = "<%getInfo("rep_enableCrossBand");%>";
var force_enable2g5g_ap = "<%getInfo("force_enable2g5g_ap");%>";

if(rep_EnableCrossBand == "1"){
	if(repenable5g == "0" || repenable2g == "0"){
		if(repenable5g == "0") repenable5g = "1";
		else repenable5g = "0";

		if(repenable2g == "0") repenable2g = "1";
		else repenable2g = "0";
	}
}

var APclenable5g = "<% getInfo("APcl_enable5g"); %>";
var APclenable2g = "<% getInfo("APcl_enable2g"); %>";

var WISPenable5g = "<% getInfo("WISP_enable5g"); %>";
var WISPenable2g = "<% getInfo("WISP_enable2g"); %>";


var apcli0IP="<% getInfo("apcli0IP"); %>";
var apclii0IP="<% getInfo("apclii0IP"); %>";


//=============================_HIGHSPEED_WIFI_REPEATER_===============

var apcli0SUBNET="<% getInfo("apcli0_SUBNET"); %>";
var apclii0SUBNET="<% getInfo("apclii0_SUBNET"); %>";

var apcli0ROUTER="<% getInfo("apcli0_ROUTER"); %>";
var apclii0ROUTER="<% getInfo("apclii0_ROUTER"); %>";

var apcli0DNS="<% getInfo("apcli0_DNS"); %>";
var apclii0DNS="<% getInfo("apclii0_DNS"); %>";


var apcli0DHCPSTART="<% getInfo("apcli0_DHCPstart"); %>";
var apclii0DHCPSTART="<% getInfo("apclii0_DHCPstart"); %>";

var apcli0DHCPEND="<% getInfo("apcli0_DHCPend"); %>";
var apclii0DHCPEND="<% getInfo("apclii0_DHCPend"); %>";

//=============================_HIGHSPEED_WIFI_REPEATER_===============
var useGetIP = <%getVar("dhcpswitch","","");%>;
var useGetIP5g = <%getVar("dhcpswitch-5g","","");%>;

var dhcpswitch = <%getVar("dhcpswitch","","");%>;
var dhcpswitch5g = <%getVar("dhcpswitch-5g","","");%>;

if(rep_EnableCrossBand == "1"){
	dhcpswitch = useGetIP5g;
	dhcpswitch5g = useGetIP;
}
else{
	dhcpswitch = useGetIP;
	dhcpswitch5g = useGetIP5g;
}

var Ip="<% getInfo("ip-rom"); %>";
var Ip5G="<% getiNICInfo("ip-rom-5g"); %>";

var wep_ascii_string =	"<%getInfo("wepasciistring");%>";
var wep_ascii_string5g =	"<%getInfo("wepasciistring5g");%>";


var Gband_ssidTbl = <%getInfo("ssidAll");%>;
var Aband_ssidTbl = <%getiNICInfo("ssidAll");%>;
var repAssidtbl = <%getiNICInfo("repAssidtbl");%>;
var repGssidtbl = <%getInfo("repeaterSSIDAll");%>;
var A_ssid =  Aband_ssidTbl[0];
var A_rep_ssid = repAssidtbl[0];
var G_ssid =  Gband_ssidTbl[0];
var G_rep_ssid = repGssidtbl[0];


if (G_ssid.search('"') != -1)   G_ssid = G_ssid.replace(/\"/g, "&quot;");
if (G_rep_ssid.search('"') != -1)   G_rep_ssid = G_rep_ssid.replace(/\"/g, "&quot;");
if (A_ssid.search('"') != -1)   A_ssid = A_ssid.replace(/\"/g, "&quot;");
if (A_rep_ssid.search('"') != -1)  A_rep_ssid = A_rep_ssid.replace(/\"/g, "&quot;");

G_ssid = G_ssid.replace(/\s/g, "&nbsp;");
G_rep_ssid = G_rep_ssid.replace(/\s/g, "&nbsp;");
A_ssid = A_ssid.replace(/\s/g, "&nbsp;");
A_rep_ssid = A_rep_ssid.replace(/\s/g, "&nbsp;");

function goSave()
{
	if(wizMode=="2"){
	  document.saveform.force_enable2g5g_ap.value=1;
	}
	document.saveform.isSave.value="1";
	document.saveform.submit();
}
function goBack()
{
	repenable5g = "<%getInfo("rep_enable5g");%>";
	if(wizMode=="2"){
//	 	if(repenable5g=="1") {
//			//window.location.assign("wiz_ip5g.asp");
//			window.location.assign("wiz_rep5gauto.asp");
//		}
//		else {
//			//window.location.assign("wiz_ip.asp");
//			window.location.assign("wiz_rep24gauto.asp");
//		}
	 	if(repenable5g=="1")
			window.location.assign("wiz_ip5g.asp");
		else
			window.location.assign("wiz_ip.asp");
	}
	else if(wizMode=="3"){ 
		if(APclenable5g=="1")
			window.location.assign("wiz_ip5g.asp");
		else
			window.location.assign("wiz_ip.asp");
	}
	else
		window.location.assign("wifi.asp");
}
function goNext()
{
//===========================_HIGHSPEED_WIFI_REPEATER_=================
	document.applyform.apcli0IP.value = 		apcli0IP;
	document.applyform.apclii0IP.value = 		apclii0IP;
	document.applyform.apcli0_SUBNET.value = 	apcli0SUBNET;
	document.applyform.apclii0_SUBNET.value = 	apclii0SUBNET;
	document.applyform.apcli0_ROUTER.value = 	apcli0ROUTER;
	document.applyform.apclii0_ROUTER.value = 	apclii0ROUTER;
	document.applyform.apcli0_DNS.value = 		apcli0DNS;
	document.applyform.apclii0_DNS.value = 		apclii0DNS;
	

	document.applyform.apcli0DHCP_START.value = 	apcli0DHCPSTART;
	document.applyform.apcli0DHCP_END.value = 		apcli0DHCPEND;

	document.applyform.apclii0DHCP_START.value = 	apclii0DHCPSTART;
	document.applyform.apclii0DHCP_END.value = 		apclii0DHCPEND;

//===========================_HIGHSPEED_WIFI_REPEATER_=================
	document.applyform.repEnableCrossBand.value = rep_EnableCrossBand;
	document.applyform.Ap_enable5g.value = Apenable5g;
	document.applyform.Ap_enable2g.value = Apenable2g;

	repenable5g = "<%getInfo("rep_enable5g");%>";
	repenable2g = "<%getInfo("rep_enable2g");%>";
	document.applyform.rep_enable5g.value = repenable5g;
	document.applyform.rep_enable2g.value = repenable2g;
	if(wizMode=="2"){
	  document.applyform.force_enable2g5g_ap.value=1;
	}
	document.applyform.APcl_enable5g.value = APclenable5g;
	document.applyform.APcl_enable2g.value = APclenable2g;
	document.applyform.WISP_enable5g.value = WISPenable5g;
	document.applyform.WISP_enable2g.value = WISPenable2g;
	document.applyform.isApply.value="1";
	document.applyform.submit();
}

function init()
{
	setTableWidth(wanMode,wizMode);

	if ((wizMode == 0) && (wanMode >= 2))
	{
		document.getElementById("showUserInfo").style.display = "block"; 
		document.getElementById("internetType_low").style.display = "none"; 
		document.getElementById("internetType_up").style.display = "block"; 
	}
	else
	{
		document.getElementById("showUserInfo").style.display = "none"; 
		document.getElementById("internetType_low").style.display = "block"; 
		document.getElementById("internetType_up").style.display = "none"; 
	}
	
	/* For show password */
	var pskval = showText(OpenSecurity);
	var pskval5g = showText(OpenSecurity);

	var display_flag1 = 0;
	var display_flag2 = 0;
	var display_flag3 = 0;
	var display_flag4 = 0;
	var display_flag5 = 0;
	var display_flag6 = 0;


	if(securityModeAll[0]!=0)
	{
		if((wizMode!=2) || (securityModeAll[0]==2))
		{
			if(securityModeAll[0]==1){
				if(wizMode==3) pskval = ConnectKey[0];//ConnectTestKey;
				else pskval = pskValueTbl[0];
			}
			else{
				pskval = pskValueTbl[0];
			}
		}
		else{
			//pskval = ConnectKey[0];//ConnectTestKey;
			
			if(repenable2g =="1")
			    pskval = ConnectKey[0];
			else
			    pskval = pskValueTbl[0];
		}
	}

	if(securityModeAll5g[0]!=0)
	{
		if((wizMode!=2) || (securityModeAll5g[0]==2))
		{
			if(securityModeAll5g[0]==1){
				if(wizMode==3) pskval5g = ConnectKey5g[0];//ConnectTestKey5g;
				else pskval5g = psk5GValueTbl[0];
			}
			else{
				pskval5g = psk5GValueTbl[0];
			}
		}
		else{
			//pskval5g = ConnectKey5g[0];//ConnectTestKey5g;
			
			if(repenable5g =="1")
			    pskval5g = ConnectKey5g[0];
			else
			    pskval5g = psk5GValueTbl[0];
		}
	}

	if( (wizMode==0) || (wizMode==4) || (wizMode==1 && Apenable5g=="1" && Apenable2g=="1") || (wizMode==2 && repenable5g=="1" && repenable2g=="1") ){
		if(pskval.length < 32 && pskval5g.length < 32)
			display_flag1 = 1;
		else if (pskval.length < 32 && pskval5g.length > 32)
			display_flag2 = 1;				
		else if (pskval.length > 32 && pskval5g.length < 32)
			display_flag3 = 1;
		else if (pskval.length > 32 && pskval5g.length > 32)
			display_flag4 = 1;
	}

	if( (wizMode==1 && Apenable5g=="1" && Apenable2g=="0") || (wizMode==2 && repenable5g=="1" && repenable2g=="0") || (wizMode==3 && APclenable5g=="1" && APclenable2g=="0") ){
		if(pskval5g.length < 32)
			display_flag5 = 1;
		else if(pskval5g.length > 32)
			display_flag6 = 1;
	}

	if( (wizMode==1 && Apenable5g=="0" && Apenable2g=="1") || (wizMode==2 && repenable5g=="0" && repenable2g=="1") || (wizMode==3 && APclenable5g=="0" && APclenable2g=="1") ){
		if(pskval.length < 32)
			display_flag5 = 1;
		else if(pskval.length > 32)
			display_flag6 = 1;
	}

		if(pskval.length > 32)
			pskval=pskval.substring(0,32)+'<br>'+pskval.substring(32,pskval.length);

		if(pskval5g.length > 32)
			pskval5g=pskval5g.substring(0,32)+'<br>'+pskval5g.substring(32,pskval5g.length);


	if(rep_EnableCrossBand == "1" && wizMode == "2"){
		document.getElementById('wirelesspassword').innerHTML = pskval5g;
		document.getElementById('wirelesspassword5g').innerHTML = pskval;
	}
	else{
		document.getElementById('wirelesspassword').innerHTML = pskval;
		document.getElementById('wirelesspassword5g').innerHTML = pskval5g;
	}
	

	if(display_flag1 == 1){
		if(wanMode >= 2){
			if(wizMode==0 || wizMode==4)
				document.getElementById("blcok_46px_GW").style.display = "block";
		}
		else{
			if(wizMode==0 || wizMode==4){
				document.getElementById("Gwtable").style.display = "block";
				document.getElementById("blcok_85px_GW").style.display = "block";
			}
			else{
				if(wizMode==1)
					document.getElementById("blcok_61px_AP").style.display = "block";
				else if (wizMode==2)
					document.getElementById("blcok_51px_rep").style.display = "block";
			}
		}
	}
	else if(display_flag2 == 1){
		if(wanMode >= 2){
			if(wizMode==0 || wizMode==4)
				document.getElementById("blcok_28px_GW").style.display = "block";
		}
		else{
			if(wizMode==0 || wizMode==4){
				document.getElementById("Gwtable").style.display = "block";
				document.getElementById("blcok_68px_GW").style.display = "block";
			}
			else{
				if(wizMode==1)
					document.getElementById("blcok_43px_AP").style.display = "block";
				else if (wizMode==2)
					document.getElementById("blcok_32px_rep").style.display = "block";
			}
		}
	}
	else if(display_flag3 == 1){
		if(wanMode >= 2){
			if(wizMode==0 || wizMode==4)
				document.getElementById("blcok_28px_GW").style.display = "block";
		}
		else{
			if(wizMode==0 || wizMode==4){
				document.getElementById("Gwtable").style.display = "block";
				document.getElementById("blcok_68px_GW").style.display = "block";
			}
			else{
				if(wizMode==1)
					document.getElementById("blcok_43px_AP").style.display = "block";
				else if (wizMode==2)
					document.getElementById("blcok_32px_rep").style.display = "block";
			}
		}
	}
	else if(display_flag4 == 1){
		if(wanMode >= 2){
			if(wizMode==0 || wizMode==4)
				document.getElementById("blcok_9px_GW").style.display = "block";
		}
		else{
			if(wizMode==0 || wizMode==4){
				document.getElementById("Gwtable").style.display = "block";
				document.getElementById("blcok_48px_GW").style.display = "block";
			}
			else{
				if(wizMode==1)
					document.getElementById("blcok_24px_AP").style.display = "block";
				else if (wizMode==2)
					document.getElementById("blcok_14px_rep").style.display = "block";
			}
		}
	}
	else if(display_flag5 == 1){
		if( (wizMode==1) || (wizMode==2) || (wizMode==3) )
			document.getElementById("blcok_72px_AP").style.display = "block";
	}
	else if(display_flag6 == 1){
		if( (wizMode==1) || (wizMode==2) || (wizMode==3) )
			document.getElementById("blcok_54px_AP").style.display = "block";
	}
	else{
		document.getElementById("blcok_58px").style.display = "block";
	}


	if( (wizMode==0) || (wizMode==4)){
		document.getElementById("nband_info").style.display = "block";
		document.getElementById("gband_info").style.display = "block";
	}
	else if(wizMode==1){
		if( (Apenable5g=="1") && (Apenable2g=="0") ){
			document.getElementById("nband_info").style.display = "block";
			document.getElementById("gband_info").style.display = "none";
			document.getElementById("5g_table").style.display = "block";
		}
		if( (Apenable5g=="0") && (Apenable2g=="1") ){
			document.getElementById("nband_info").style.display = "none";
			document.getElementById("gband_info").style.display = "block";
			document.getElementById("24g_table").style.display = "block";
		}
		if( (Apenable5g=="1") && (Apenable2g=="1") ){
			document.getElementById("nband_info").style.display = "block";
			document.getElementById("gband_info").style.display = "block";
			document.getElementById("blcok_24px").style.display = "block";
			document.getElementById("Gwtable").style.display = "block";
		}
	}
	else if(wizMode==2){
			
		if((repenable5g=="1") && (repenable2g=="1")){
			document.getElementById("nulltable").style.display = "block";
			document.getElementById("repdisplayIP5G").style.display = "block";
		}
		else{
			document.getElementById("nulltable").style.display = "none";
			document.getElementById("repdisplayIP5G").style.display = "block";
		}
		/*
		else if(repenable5g=="1"){
			document.getElementById("gband_info").style.display = "none";
			document.getElementById("5g_table").style.display = "block";
			document.getElementById("repdisplayIP5G").style.display = "block";
		}
		else{
			document.getElementById("nband_info").style.display = "none";
			document.getElementById("24g_table").style.display = "block";
		}*/
	}
	else if(wizMode==3){
		if(APclenable5g=="0"){
			document.getElementById("nband_info").style.display = "none";
			document.getElementById("24g_table").style.display = "block";
		}
		if(APclenable2g=="0"){
			document.getElementById("gband_info").style.display = "none";
			document.getElementById("5g_table").style.display = "block";
		}
		if((APclenable5g=="0") && (APclenable2g=="0")){
			document.getElementById("band_disable").style.display = "block";
		}
	}
	else{
		if(wlanDisable){
			document.getElementById("nband_info").style.display = "none";
		}
		if(wlan24Disable){
			document.getElementById("gband_info").style.display = "none";
		}
		if((wlan24Disable || wlanDisable)){
			document.getElementById("band_disable").style.display = "block";
		}
	}
}

function setTableWidth()
{
	languIndex=stype;
	if(stype>13) languIndex=13;

	document.getElementById('infotable').className="tablewidth"+languIndex;


	document.getElementById('tableLwidth').className="TypeLtable"+languIndex;
	document.getElementById('tableMwidth').className="TypeMtable"+languIndex;

	if((wizMode != 2) || ((wizMode == 2) && (repenable2g=="1")))
		document.getElementById('tableRwidth').className="TypeRtable"+languIndex;

	document.getElementById('tableL1width').className="TypeLtable"+languIndex;
	document.getElementById('tableM1width').className="TypeMtable"+languIndex;
	document.getElementById('tableR1width').className="TypeRtable"+languIndex;
	
	document.getElementById('tableL2width').className="TypeLtable"+languIndex;
	document.getElementById('tableM2width').className="TypeMtable"+languIndex;
	document.getElementById('tableR2width').className="TypeRtable"+languIndex;

	document.getElementById('tableL3width').className="TypeLtable"+languIndex;
	document.getElementById('tableM3width').className="TypeMtable"+languIndex;
	document.getElementById('tableR3width').className="TypeRtable"+languIndex;

	document.getElementById('tableL4width').className="TypeLtable"+languIndex;
	document.getElementById('tableM4width').className="TypeMtable"+languIndex;
	document.getElementById('tableR4width').className="TypeRtable"+languIndex;

	document.getElementById('tableL5width').className="TypeLtable"+languIndex;
	document.getElementById('tableM5width').className="TypeMtable"+languIndex;
	document.getElementById('tableR5width').className="TypeRtable"+languIndex;

	document.getElementById('tableL6width').className="TypeLtable"+languIndex;
	document.getElementById('tableM6width').className="TypeMtable"+languIndex;
	document.getElementById('tableR6width').className="TypeRtable"+languIndex;
}
</script>
<style type="text/css">

.TypeLtable0 {width:30%;}
.TypeLtable1 {width:30%;}
.TypeLtable2 {width:30%;}
.TypeLtable3 {width:30%;}
.TypeLtable4 {width:30%;}
.TypeLtable5 {width:30%;}
.TypeLtable6 {width:30%;}
.TypeLtable7 {width:25%;}
.TypeLtable8 {width:25%;}
.TypeLtable9 {width:30%;}
.TypeLtable10 {width:25%;}
.TypeLtable11 {width:25%;}
.TypeLtable12 {width:25%;}
.TypeLtable13 {width:26%;}

.TypeMtable0 {width:26%;}
.TypeMtable1 {width:26%;}
.TypeMtable2 {width:26%;}
.TypeMtable3 {width:26%;}
.TypeMtable4 {width:26%;}
.TypeMtable5 {width:26%;}
.TypeMtable6 {width:26%;}
.TypeMtable7 {width:36%;}
.TypeMtable8 {width:36%;}
.TypeMtable9 {width:26%;}
.TypeMtable10 {width:36%;}
.TypeMtable11 {width:36%;}
.TypeMtable12 {width:36%;}
.TypeMtable13 {width:26%;}

.TypeRtable0 {width:44%;}
.TypeRtable1 {width:44%;}
.TypeRtable2 {width:44%;}
.TypeRtable3 {width:44%;}
.TypeRtable4 {width:44%;}
.TypeRtable5 {width:44%;}
.TypeRtable6 {width:44%;}
.TypeRtable7 {width:39%;}
.TypeRtable8 {width:39%;}
.TypeRtable9 {width:44%;}
.TypeRtable10 {width:39%;}
.TypeRtable11 {width:39%;}
.TypeRtable12 {width:44%;}
.TypeRtable13 {width:48%;}

.tablewidth0 {width:90%;}
.tablewidth1 {width:78%;}
.tablewidth2 {width:64%;}
.tablewidth3 {width:74%;}
.tablewidth4 {width:62%;}
.tablewidth5 {width:68%;}
.tablewidth6 {width:62%;}
.tablewidth7 {width:72%;}
.tablewidth8 {width:62%;}
.tablewidth9 {width:78%;}
.tablewidth10 {width:88%;}
.tablewidth11 {width:58%;}
.tablewidth12 {width:85%;}
.tablewidth13 {width:90%;}

</style>
</head>
<body onLoad="init()">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right">
				<script>
					if(wizMode==0)
						dw(routermode);
					else if(wizMode==1)
						dw(apmode);
					else if(wizMode==2)
						dw(repeatermode);
					else if(wizMode==3)
						dw(APcl);
					else if(wizMode==4)
						dw(wisp);
					else
						dw(repeatermode);
				</script>
			</td>
		</tr>
	</table>
	
	<table border="0" style="height:94px;" id="infotable" class="tablewidth0" align="center">
		<tr><td align="left" class="Arial_16" style="line-height:20px;"><script>dw(setupDone)</script></td></tr>
	</table>

<span id="internetType_up" style="display:none">
	<table border="0" style="height:30px;width:700px" align="center">
		<tr>
			<td align="right" id="tableL6width" class="TypeLtable0"></td>
			<td align="left" id="tableM6width" class="TypeMtable0">
				<script>
						dw(internetType);
						document.write('&nbsp;:&nbsp;</td>');
						document.write('<td align="left" id="tableR6width" class="TypeRtable0">');
						switch(wanMode)
						{
						case 0:	
							document.write(''+showText(DHCP)+'');
							document.write('</td>');
							break;
						case 1:	
							document.write(''+showText(staticIP)+'');
							document.write('</td>');
							break;
						case 2:	
							document.write(''+showText(PPPoE)+'');
							document.write('</td>');
							break;
						case 3:	
							document.write(''+showText(PPTP)+'');
							document.write('</td>');
							break;
						case 4:	
							document.write(''+showText(TelBP)+'');
							document.write('</td>');
							break;
						default:	
							document.write(''+showText(L2TP)+'');
							document.write('</td>');
							break;
						}
				</script>
		</tr>
	</table>
</span>

<span id="showUserInfo" style="display:none">
	<table border="0" style="height:30px;width:700px" align="center">
		<tr>
			<td align="right" id="tableL4width" class="TypeLtable0"></td>
			<td align="left" id="tableM4width" class="TypeMtable0"><script>dw(username)</script>&nbsp;:&nbsp;</td>
				
			<td align="left" id="tableR4width" class="TypeRtable0">
				<script>
					if(wanMode==2)
						document.write(''+pppUsername+'');
					else if (wanMode==3)
						document.write(''+pptpUsername+'');
					else if (wanMode==4)
						document.write(''+telBUsername+'');
					else if (wanMode==6)
						document.write(''+l2tpUsername+'');
					document.write('</td>');
				</script>
		</tr>

		<tr>
			<td align="right" id="tableL5width" class="TypeLtable0"></td>
			<td align="left" id="tableM5width" class="TypeMtable0"><script>dw(password)</script>&nbsp;:&nbsp;</td>
				
			<td align="left" id="tableR5width" class="TypeRtable0">
				<script>
					if(wanMode==2)
						document.write(''+pppPassWord+'');
					else if (wanMode==3)
						document.write(''+pptpPassWord+'');
					else if (wanMode==4)
						document.write(''+telBPassWord+'');
					else if (wanMode==6)
						document.write(''+l2tpPassWord+'');
					document.write('</td>');
				</script>
		</tr>
	</table>
</span>

<span id="internetType_low" style="display:none">
	<table border="0" style="height:30px;width:700px" align="center">
		<tr>
			<td align="right" id="tableLwidth" class="TypeLtable0"></td>
			<td align="left" id="tableMwidth" class="TypeMtable0">
				<script>
					if(wizMode >= 0)
					{
						if(wizMode == 2){
							document.write('&nbsp;</td><td align="left" id="tableRwidth" class="TypeRtable0">&nbsp;</td>');
							/*if(repenable2g=="1"){
								dw(IPAddress);
								document.write('&nbsp;:&nbsp;</td>');
								document.write('<td align="left" id="tableRwidth" class="TypeRtable0">');
								if(dhcpswitch==0){
									if(rep_EnableCrossBand == "1") document.write('<span>'+Ip5G+'</span>');
									else document.write('<span>'+Ip+'</span>');
								}
								else{
									if(rep_EnableCrossBand == "1"){
										if(apclii0IP != "")  document.write('<span>'+apclii0IP+'</span>');
										else  document.write('<span>'+showText(getIPfail)+'</span>');
									}
									else{
										if(apcli0IP != "")  document.write('<span>'+apcli0IP+'</span>');
										else  document.write('<span>'+showText(getIPfail)+'</span>');
									}
								}	
								document.write('</td>');
							}*/
						}
						else if(wizMode == 3){
							document.write('&nbsp;</td><td align="left" id="tableRwidth" class="TypeRtable0">&nbsp;</td>');
							/*dw(IPAddress);
							document.write('&nbsp;:&nbsp;</td>');
							document.write('<td align="left" id="tableRwidth" class="TypeRtable0">');

							if((APclenable5g=="1") && (APclenable2g=="0")){
								if(dhcpswitch5g==0){
									document.write('<span>'+Ip5G+'</span>');
								}
								else{
									if(apclii0IP != "") document.write('<span>'+apclii0IP+'</span>');
									else document.write('<span>'+showText(getIPfail)+'</span>');
								}
							}
							else if((APclenable5g=="0") && (APclenable2g=="1")){
								if(dhcpswitch==0){
									document.write('<span>'+Ip+'</span>');
								}
								else{
									if(apcli0IP != "") document.write('<span>'+apcli0IP+'</span>');
									else document.write('<span>'+showText(getIPfail)+'</span>');
								}
							}
							document.write('</td>');*/
						}
						else
						{
							if(wizMode !=1)
								{
									dw(internetType);
									document.write('&nbsp;:&nbsp;</td>');
									document.write('<td align="left" id="tableRwidth" class="TypeRtable0">');
									switch(wanMode)
									{
										case 0:	
											document.write(''+showText(DHCP)+'');
											break;
										case 1:	
											document.write(''+showText(staticIP)+'');
											break;
										case 2:	
											document.write(''+showText(PPPoE)+'');
											break;
										case 3:	
											document.write(''+showText(PPTP)+'');
											break;
										case 4:	
											document.write(''+showText(TelBP)+'');
											break;
										default:	
											document.write(''+showText(L2TP)+'');
											break;
									}
									document.write('</td>');
								}
							else{
								document.write('<td align="left" id="tableRwidth" class="TypeRtable0">');
								document.write('</td>');
							}
						 }
					}
					else
					{
						dw(mode)
						document.write('&nbsp;:&nbsp;</td>');
						document.write('<td align="left">');
						switch(wizMode)
						{
							case 1:	
								document.write(''+showText(apmode)+'');
								break;
							case 2:	
								document.write(''+showText(repeatermode)+'');
								break;
							default:	
								document.write('');
								break;
						}
						document.write('</td>');
					}
				</script>
		</tr>
	</table>
</span>

<span id="gband_info" style="display:block">
	<table border="0" style="height:50px;width:700px" align="center">
		<tr>
			<td align="right" id="tableL1width" class="TypeLtable0">(<script>dw(Gband)</script>)&nbsp;</td>
			<td align="left" id="tableM1width" class="TypeMtable0"><script>dw(wirelessname2)</script>&nbsp;:&nbsp;</td>
			<td align="left" id="tableR1width" class="TypeRtable0">
				<script>
					if(wizMode != 3){
						if(wizMode == 2 && rep_EnableCrossBand == "1") document.write(A_ssid);
						else document.write(G_ssid);
					}
					else document.write(G_rep_ssid);
				</script>
			</td>	
		</tr>
	
		<tr>
			<td></td>
			<td align="left"><script>dw(wirelesspassword)</script>&nbsp;:&nbsp;</td>
				<script>
					document.write('<td id="wirelesspassword" align="left">');
					document.write('</td>');
				</script>
		</tr>
	</table>
</span>

<span id="nulltable" style="display:none">
	<table border="0" style="height:15px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<span id="repdisplayIP5G" style="display:none">
	<table border="0" style="height:30px;width:700px" align="center">
		<tr>
			<td align="left" id="tableL2width" class="TypeLtable0"></td>
			<td align="left" id="tableM2width" class="TypeMtable0">
				<script>
					if(wizMode == 2)
					{
						document.write('&nbsp;</td><td align="left" id="tableR2width" class="TypeRtable0">&nbsp;</td>');
						/*dw(IPAddress);
						document.write('&nbsp;:&nbsp;</td>');
						document.write('<td align="left" id="tableR2width" class="TypeRtable0">');

						if(dhcpswitch5g==0){
							if(rep_EnableCrossBand == "1") document.write('<span>'+Ip+'</span>');
							else document.write('<span>'+Ip5G+'</span>');
						}
						else{
							if(rep_EnableCrossBand == "1"){
								if(apcli0IP != "")  document.write('<span>'+apcli0IP+'</span>');
								else  document.write('<span>'+showText(getIPfail)+'</span>');
							}
							else{
								if(apclii0IP != "") document.write('<span>'+apclii0IP+'</span>');
								else document.write('<span>'+showText(getIPfail)+'</span>');
							}
						}
						document.write('</td>');*/
					}
					else{
						document.write('<td align="left" id="tableR2width" class="TypeRtable0">');
					}
			</script>
		</tr>
	</table>
</span>

<span id="Gwtable" style="display:none">
	<table border="0" style="height:10px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<span id="nband_info" style="display:block">
	<table border="0" style="height:50px;width:700px;" align="center">
		<tr>
			<td align="right" id="tableL3width" class="TypeLtable0">(<script>dw(Nband)</script>)&nbsp;</td>
			<td align="left" id="tableM3width" class="TypeMtable0"><script>dw(wirelessname5)</script>&nbsp;:&nbsp;</td>
			<td align="left" id="tableR3width" class="TypeRtable0">
				<script>
					if(wizMode != 3){
						if(wizMode == 2 && rep_EnableCrossBand == "1") document.write(G_ssid);
						else document.write(A_ssid);
					}
					else document.write(A_rep_ssid);
				</script>
			</td>	
		</tr>
		
		<tr>
			<td></td>
			<td align="left"><script>dw(wirelesspassword)</script>&nbsp;:&nbsp;</td>
				<script>
			 	 document.write('<td id="wirelesspassword5g" align="left">');
				 document.write('</td>');
				</script>
		</tr>
	</table>
</span>

<!---gw; wisp; wanmode<2; pak<32; psk5g<32------>
<span id="blcok_85px_GW" style="display:none">
	<table border="0" style="height:85px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---gw; wisp; wanmode<2; pak<32 and psk5g>32; pak>32 and psk5g<32;------>
<span id="blcok_68px_GW" style="display:none">
	<table border="0" style="height:68px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---gw; wisp; wanmode<2; pak>32 and psk5g>32;------>
<span id="blcok_48px_GW" style="display:none">
	<table border="0" style="height:48px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---gw; wisp; wanmode>=2; pak<32 and psk5g<32;------>
<span id="blcok_46px_GW" style="display:none">
	<table border="0" style="height:46px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---gw; wisp; wanmode>=2; pak<32 and psk5g>32; pak>32 and psk5g<32;------>
<span id="blcok_28px_GW" style="display:none">
	<table border="0" style="height:28px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---gw; wisp; wanmode>=2; pak>32 and psk5g>32;------>
<span id="blcok_9px_GW" style="display:none">
	<table border="0" style="height:9px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---ap; pak<32 and psk5g<32;------>
<span id="blcok_61px_AP" style="display:none">
	<table border="0" style="height:61px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---ap; pak<32 and psk5g>32; pak>32 and psk5g<32;------>
<span id="blcok_43px_AP" style="display:none">
	<table border="0" style="height:43px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---ap; pak>32 and psk5g>32;------>
<span id="blcok_24px_AP" style="display:none">
	<table border="0" style="height:24px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---ap; psk<32; psk5g<32;------>
<span id="blcok_72px_AP" style="display:none">
	<table border="0" style="height:72px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---ap; psk>32; psk5g>32;------>
<span id="blcok_54px_AP" style="display:none">
	<table border="0" style="height:54px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---Ap mode use------>
<span id="blcok_24px" style="display:none">
	<table border="0" style="height:24px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---rep; pak<32 and psk5g<32;------>
<span id="blcok_51px_rep" style="display:none">
	<table border="0" style="height:51px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---rep; pak<32 and psk5g>32; pak>32 and psk5g<32;------>
<span id="blcok_32px_rep" style="display:none">
	<table border="0" style="height:32px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---rep; pak>32 and psk5g>32;------>
<span id="blcok_14px_rep" style="display:none">
	<table border="0" style="height:14px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---only 5g enable------>
<span id="5g_table" style="display:none">
	<table border="0" style="height:50px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---only 24g enable------>
<span id="24g_table" style="display:none">
	<table border="0" style="height:74px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---5g and 24g disable------>
<span id="band_disable" style="display:none">
	<table border="0" style="height:50px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

<!---else------>
<span id="blcok_58px" style="display:none">
	<table border="0" style="height:58px;width:780px;" align="center"><tr><td></td></tr></table>
</span>

	<table border="0" style="height:64px;width:400px;" align="center">
		<form action="/goform/saveAndReboot" method="POST" name="saveform">
			<tr>
				<td align="center" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
					<script>document.write('<input type="button" value="'+showText(backup)+'" name="save" onClick="goSave();">');</script>
				</td>
					<input type="hidden" value="" name="isSave">
					<input type="hidden" value="" name="force_enable2g5g_ap">
			</tr>
		</form>
	</table>

	<table border="0" style="height:28px;width:780px;" align="center">
		<form action="/goform/saveAndReboot" method="POST" name="applyform">
			<tr>
				<td align="right" style="width:390px;">
					<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
				</td>
	
				<td align="left" style="width:390px;">
					<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
				</td>
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="" name="Ap_enable5g">
					<input type="hidden" value="" name="Ap_enable2g">
					<input type="hidden" value="" name="force_enable2g5g_ap">
					<input type="hidden" value="" name="rep_enable5g">
					<input type="hidden" value="" name="rep_enable2g">
					<input type="hidden" value="" name="repEnableCrossBand">
					<input type="hidden" value="" name="APcl_enable5g">
					<input type="hidden" value="" name="APcl_enable2g">
					<input type="hidden" value="" name="WISP_enable5g">
					<input type="hidden" value="" name="WISP_enable2g">

<!--===========================================_HIGHSPEED_WIFI_REPEATER_====================================-->
					<input type="hidden" value="" name="apcli0IP">
					<input type="hidden" value="" name="apclii0IP">
					<input type="hidden" value="" name="apcli0_SUBNET">
					<input type="hidden" value="" name="apclii0_SUBNET">
					<input type="hidden" value="" name="apcli0_ROUTER">
					<input type="hidden" value="" name="apclii0_ROUTER">
					<input type="hidden" value="" name="apcli0_DNS">
					<input type="hidden" value="" name="apclii0_DNS">
					<input type="hidden" value="" name="apcli0DHCP_START">
					<input type="hidden" value="" name="apcli0DHCP_END">
					<input type="hidden" value="" name="apclii0DHCP_START">
					<input type="hidden" value="" name="apclii0DHCP_END">
<!--===========================================_HIGHSPEED_WIFI_REPEATER_====================================-->
			</tr>
		</form>
	</table>

	<table border="0" style="height:60px;width:780px;" align="center">
		<tr><td></td></tr>
	</table>
	
</td>
</tr>
</table>
</div>
<input type="hidden" value="/conclusion.asp" name="submit-url">
<form action="/goform/saveAndReboot" method="POST" name="backto5g">
	<input type="hidden" value="/wiz_rep5g.asp" name="goSitesuvy">
</form>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
