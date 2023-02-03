<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/autowan.var"></script>
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var wifi_changed="<% getInfo("wifi_changed"); %>";
var wl_24mac = "<% getInfo("wlmac_last6"); %>";
var wl_5mac = "<% getInfo("5Gwlmac_last6"); %>";
var Gssidtbl =	<%getInfo("Gssidtbl");%>;
var Assidtbl =	<%getiNICInfo("Assidtbl");%>;

var wl_24ssid =	Gssidtbl[0];
var wl_5ssid =	Assidtbl[0];

var pskValueTbl = <%getInfo("pskValueAll");%>;
var psk5GValueTbl = <%getiNICInfo("pskValueAll");%>;
var Ap_enable5g = "<%getInfo("Ap_enable5g");%>";
var Ap_enable2g = "<%getInfo("Ap_enable2g");%>";
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>";
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>";
var wiz_gwManu = "<%getInfo("wiz_gwManu");%>";
var wizmode_value = "<% getInfo("wizmode_value"); %>";

function goBack()
{
	if(wizMode==0){
		if (wiz_gwManu==1)
			window.location.assign("mstart.asp");
		else
			window.location.assign("hwsetup.asp");
	}
	else if (wizMode==1){
		window.location.assign("wiz_apselect.asp");
	}
	else if (wizMode==4){
		if((WISP_enable5g==1) && (WISP_enable2g==0))
			window.location.assign("wiz_WISP5gauto.asp");
		else
			window.location.assign("wiz_WISP24gauto.asp");
	}
}
function goNext()
{
	var str=document.wifi.pskValue.value;
	var ssid=document.wifi.ssid.value;
	var str5g=document.wifi.pskValue5g.value;
	var ssid5g=document.wifi.ssid5g.value;

	if(wizMode == 0 || wizMode == 4){
		if (ssid5g == ""){
				document.wifi.ssid5g.value = wl_5ssid;
				document.wifi.ssid5g.focus();
				return false;
		}
		if (ssid == ""){
				document.wifi.ssid.value = wl_24ssid;
				document.wifi.ssid.focus();
				return false;
		}
	
		if (str5g.length < 8 && str.length < 8){
				alert(showText(pskshouldbe8));
				document.wifi.pskValue5g.value = ""
				document.wifi.pskValue5g.focus();
				document.wifi.pskValue.value = ""
				document.wifi.pskValue.focus();
				return false;
		}
		if (str5g.length < 8 && str.length >= 8){
				alert(showText(pskshouldbe8)+" (5G)");
				document.wifi.pskValue5g.value = ""
				document.wifi.pskValue5g.focus();
				return false;
		}
		if (str5g.length >= 8 && str.length < 8){
				alert(showText(pskshouldbe8)+" (2.4G)");
				document.wifi.pskValue.value = ""
				document.wifi.pskValue.focus();
				return false;
		}
	}
	else if(wizMode == 1){
		if(Ap_enable5g == "1" && Ap_enable2g == "1"){
			if (ssid5g == ""){
				document.wifi.ssid5g.value = wl_5ssid;
				document.wifi.ssid5g.focus();
				return false;
				}
			if (ssid == ""){
					document.wifi.ssid.value = wl_24ssid;
					document.wifi.ssid.focus();
					return false;
			}
			if (str5g.length < 8 && str.length < 8){
				alert(showText(pskshouldbe8));
				document.wifi.pskValue5g.value = ""
				document.wifi.pskValue5g.focus();
				document.wifi.pskValue.value = ""
				document.wifi.pskValue.focus();
				return false;
			}
			if (str5g.length < 8 && str.length >= 8){
					alert(showText(pskshouldbe8)+" (5G)");
					document.wifi.pskValue5g.value = ""
					document.wifi.pskValue5g.focus();
					return false;
			}
			if (str5g.length >= 8 && str.length < 8){
					alert(showText(pskshouldbe8)+" (2.4G)");
					document.wifi.pskValue.value = ""
					document.wifi.pskValue.focus();
					return false;
			}
		}
		else if(Ap_enable2g == "1" && Ap_enable5g=="0"){
			if (ssid == ""){
				document.wifi.ssid.value = wl_24ssid;
				document.wifi.ssid.focus();
				return false;
			}	
			if (str.length < 8){
				alert(showText(pskshouldbe8)+" (2.4G)");
				document.wifi.pskValue.value = ""
				document.wifi.pskValue.focus();
				return false;
			}
		}
		else if(Ap_enable5g == "1" && Ap_enable2g == "0"){
			if (ssid5g == ""){
					document.wifi.ssid5g.value = wl_5ssid;
					document.wifi.ssid5g.focus();
					return false;
				}
			if (str5g.length < 8){
					alert(showText(pskshouldbe8)+" (5G)");
					document.wifi.pskValue5g.value = ""
					document.wifi.pskValue5g.focus();
					return false;
				}
		}
	}

	
	if (!checkstring(document.wifi.ssid)) {
		//document.wifi.ssid.focus();
		return false;
	}
	if (!checkstring(document.wifi.pskValue)) {
		//document.wifi.pskValue.focus();
		return false;
	}
	if (!checkstring(document.wifi.ssid5g)) {
		//document.wifi.ssid5g.focus();
		return false;
	}
	if (!checkstring(document.wifi.pskValue5g)) {
		//document.wifi.pskValue5g.focus();
		return false;
	}

	document.wifi.Ap_enable5g.value = Ap_enable5g;
	document.wifi.Ap_enable2g.value = Ap_enable2g;
	document.wifi.submit();
}

function checkstring(input) {

	var x = input
	if (x.value != "") {
		if (x.value.search("’") != -1 || x.value.search('”') != -1) {
			alert(showText(stringNoSupport));
			x.focus()
			return false
		}
		else
			return true
	}
	else
		return true
}

function init_wifi()
{
	if(wifi_changed != "1")
	{
		document.wifi.ssid.value="edimax_2.4G_"+wl_24mac;
		document.wifi.ssid5g.value="edimax_5G_"+wl_5mac;
		
		document.wifi.pskValue.value="";
		document.wifi.pskValue5g.value="";
	}
}

function setTableWidth()
{
	if(stype>13) stype=13;

	document.getElementById('infotablewidth').className="infotablewidth"+stype;
	document.getElementById('settablewidth').className="settablewidth"+stype;
	document.getElementById('settable1width').className="settablewidth"+stype;
	document.getElementById('settable2width').className="settablewidth"+stype;
	document.getElementById('settable3width').className="settablewidth"+stype;

	document.getElementById('leftwidth').className="leftwidth"+stype;
	document.getElementById('rightwidth').className="rightwidth"+stype;
	document.getElementById('left1width').className="leftwidth"+stype;
	document.getElementById('right1width').className="rightwidth"+stype;

	document.getElementById('at8rightwidth').className="at8rightwidth"+stype;
	document.getElementById('at8leftwidth').className="at8leftwidth"+stype;
	document.getElementById('at8right1width').className="at8rightwidth"+stype;
	document.getElementById('at8left1width').className="at8leftwidth"+stype;

	if(iqsetup_finish != 1)
		init_wifi();
}

function init()
{
	if((wizMode==0) || (wizMode==4)){
		document.getElementById("24g_info").style.display = "block";
		document.getElementById("5g_info").style.display = "block";
		document.getElementById("24G_5Gtable").style.display = "block";
		document.getElementById("atleast8_24G").style.display = "block";
		document.getElementById("atleast8_5G").style.display = "block";
		document.getElementById("GWtable").style.display = "block";
	}
	else if(wizMode==1){
		if ( (Ap_enable5g == "1") && (Ap_enable2g == "1")){
			document.getElementById("24g_info").style.display = "block";
			document.getElementById("5g_info").style.display = "block";
			document.getElementById("24G_5Gtable").style.display = "block";
			document.getElementById("atleast8_24G").style.display = "block";
			document.getElementById("atleast8_5G").style.display = "block";
			document.getElementById("GWtable").style.display = "block";
		}
		else if ( (Ap_enable5g == "1") && (Ap_enable2g == "0")){
			document.getElementById("24g_info").style.display = "none";
			document.getElementById("5g_info").style.display = "block";
			document.getElementById("5G_table").style.display = "block";
			document.getElementById("atleast8_5G").style.display = "block";
		}
		else if ( (Ap_enable5g == "0") && (Ap_enable2g == "1")){
			document.getElementById("24g_info").style.display = "block";
			document.getElementById("5g_info").style.display = "none";
			document.getElementById("24G_table").style.display = "block";
			document.getElementById("atleast8_24G").style.display = "block";
		}
	}
}
</script>
<style type="text/css">
.infotablewidth0 {width:60%;}
.infotablewidth1 {width:62%;}
.infotablewidth2 {width:56%;}
.infotablewidth3 {width:74%;}
.infotablewidth4 {width:58%;}
.infotablewidth5 {width:60%;}
.infotablewidth6 {width:68%;}
.infotablewidth7 {width:58%;}
.infotablewidth8 {width:70%;}
.infotablewidth9 {width:66%;}
.infotablewidth10 {width:62%;}
.infotablewidth11 {width:40%;}
.infotablewidth12 {width:66%;}
.infotablewidth13 {width:42%;}

.settablewidth0 {width:70%;}
.settablewidth1 {width:70%;}
.settablewidth2 {width:72%;}
.settablewidth3 {width:74%;}
.settablewidth4 {width:70%;}
.settablewidth5 {width:72%;}
.settablewidth6 {width:74%;}
.settablewidth7 {width:82%;}
.settablewidth8 {width:84%;}
.settablewidth9 {width:67%;}
.settablewidth10 {width:67%;}
.settablewidth11 {width:69%;}
.settablewidth12 {width:69%;}
.settablewidth13 {width:68%;}

.leftwidth0 {width:41%;}
.leftwidth1 {width:41%;}
.leftwidth2 {width:41%;}
.leftwidth3 {width:41%;}
.leftwidth4 {width:41%;}
.leftwidth5 {width:43%;}
.leftwidth6 {width:44%;}
.leftwidth7 {width:50%;}
.leftwidth8 {width:52%;}
.leftwidth9 {width:38%;}
.leftwidth10 {width:39%;}
.leftwidth11 {width:41%;}
.leftwidth12 {width:41%;}
.leftwidth13 {width:41%;}

.rightwidth0 {width:59%;}
.rightwidth1 {width:59%;}
.rightwidth2 {width:58%;}
.rightwidth3 {width:53%;}
.rightwidth4 {width:59%;}
.rightwidth5 {width:57%;}
.rightwidth6 {width:56%;}
.rightwidth7 {width:50%;}
.rightwidth8 {width:48%;}
.rightwidth9 {width:62%;}
.rightwidth10 {width:61%;}
.rightwidth11 {width:59%;}
.rightwidth12 {width:61%;}
.rightwidth13 {width:61%;}

.at8leftwidth0 {width:41%;}
.at8leftwidth1 {width:41%;}
.at8leftwidth2 {width:41%;}
.at8leftwidth3 {width:44%;}
.at8leftwidth4 {width:41%;}
.at8leftwidth5 {width:43%;}
.at8leftwidth6 {width:44%;}
.at8leftwidth7 {width:50%;}
.at8leftwidth8 {width:52%;}
.at8leftwidth9 {width:38%;}
.at8leftwidth10 {width:39%;}
.at8leftwidth11 {width:41%;}
.at8leftwidth12 {width:41%;}
.at8leftwidth13 {width:41%;}

.at8rightwidth0 {width:59%;}
.at8rightwidth1 {width:59%;}
.at8rightwidth2 {width:59%;}
.at8rightwidth3 {width:56%;}
.at8rightwidth4 {width:59%;}
.at8rightwidth5 {width:57%;}
.at8rightwidth6 {width:57%;}
.at8rightwidth7 {width:50%;}
.at8rightwidth8 {width:52%;}
.at8rightwidth9 {width:62%;}
.at8rightwidth10 {width:61%;}
.at8rightwidth11 {width:59%;}
.at8rightwidth12 {width:59%;}
.at8rightwidth13 {width:59%;}

</style>
</head>
<body onLoad="setTableWidth(); init();">
<form action="/goform/setWifi" method="POST" name="wifi">
<input type="hidden" value="" name="Ap_enable5g">
<input type="hidden" value="" name="Ap_enable2g">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" align="center" style="height:90px;width:700px">
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
	
	<table border="0" align="center" style="height:103px;" id="infotablewidth" class="infotablewidth0">
		<tr>
			<td align="left" class="Arial_16" style="line-height:25px;"><script>dw(wificonfig)</script></td>
		</tr>
	</table>
	
	<span id="24g_info" style="display:none">
		<table border="0" align="center" style="height:70px;" id="settablewidth" class="settablewidth0">
			<tr>
				<td id="leftwidth" class="leftwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(wirelessname2)</script>&nbsp;(2.4GHz):&nbsp;</td>
				<td id="rightwidth" class="rightwidth0" align="left">
					<script>
						var wl_Gssid = wl_24ssid;
						if (wl_Gssid.search('"') != -1)  
						{
								wl_Gssid = wl_Gssid.replace(/\"/g, "&quot;");
						}
						document.write('<input class="Arial_16" type="text" name="ssid" size="32" maxlength="32" value="'+wl_Gssid+'">')
					</script>
				</td>	
			</tr>
	
			<tr>
				<td align="left" class="Arial_16"><script>dw(wirelesspassword)</script>&nbsp;(WPA2-AES):&nbsp;</td>
				<td align="left">
					<script>
						var pskval = pskValueTbl[0];
						if (pskval.search('"') != -1)  
						{
								pskval = pskval.replace(/\"/g, "&quot;");
						}
						document.write('<input class="Arial_16" type="text" name="pskValue" size="32" maxlength="63" value="'+pskval+'">')
					</script>
				</td>
			</tr>
		</table>
	</span>

	<span id="atleast8_24G" style="display:none">
		<table border="0" align="center" style="height:0px;" id="settable1width" class="settablewidth0">
			<tr>
				<td align="center" style="height:0px;" id="at8leftwidth" class="at8leftwidth0">
				<td align="left" style="height:0px;font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;" id="at8rightwidth" class="at8rightwidth0"><script>dw(Morethan8)</script></td>
			</tr>
		</table>
	</span>


	<span id="GWtable" style="display:none">
		<table border="0" style="height:30px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>


	<span id="5g_info" style="display:none">
		<table border="0" align="center" style="height:70px;" id="settable2width" class="settablewidth0">
			<tr>
				<td id="left1width" class="left1width0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(wirelessname5)</script>&nbsp;(5GHz):&nbsp;</td>
				<td id="right1width" class="rightwidth0" align="left">
					<script>
						var wl_Assid = wl_5ssid;
						if (wl_Assid.search('"') != -1)  
						{
								wl_Assid = wl_Assid.replace(/\"/g, "&quot;");
						}
						document.write('<input class="Arial_16" type="text" name="ssid5g" size="32" maxlength="32" value="'+wl_Assid+'">')
					</script>
					
				</td>	
			</tr>
	
			<tr>
				<td align="left" class="Arial_16"><script>dw(wirelesspassword)</script>&nbsp;(WPA2-AES):&nbsp;</td>
				<td align="left">
					<script>
						var psk5gval = psk5GValueTbl[0];
						if (psk5gval.search('"') != -1)  
						{
								psk5gval = psk5gval.replace(/\"/g, "&quot;");
						}
						document.write('<input class="Arial_16" type="text" name="pskValue5g" size="32" maxlength="63" value="'+psk5gval+'">')
					</script>
				</td>
			</tr>
		</table>
	</span>

	<span id="atleast8_5G" style="display:none">
		<table border="0" align="center" style="height:0px;" id="settable3width" class="settablewidth0">
			<tr>
				<td align="center" style="height:0px;" id="at8left1width" class="at8leftwidth0">
				<td align="left" style="height:0px;font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;" id="at8right1width" class="at8rightwidth0"><script>dw(Morethan8)</script></td>
			</tr>
		</table>
	</span>

	<span id="24G_table" style="display:none">
		<table border="0" style="height:187px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>

	<span id="5G_table" style="display:none">
		<table border="0" style="height:187px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>

	<span id="24G_5Gtable" style="display:none">
		<table border="0" style="height:64px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>

	<table border="0" style="height:28px;width:780px" align="center">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>
	
	<table border="0" style="height:60px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	
</td>
</tr>
</table>
</div>
<input type="hidden" value="/wifi.asp" name="submit-url">
</form>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
