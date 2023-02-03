<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript">
var wizmode = <%getVar("wizmode","","");%>;
var connectEnabled=0, back2survy=0 ,reload_survey=1;
var sel_value2, gonextEnable=0;
var hiddenssid = <%getiNICVar("hiddenSSID","","");%>;
var rep_enable5g = "<%getInfo("rep_enable5g");%>";
var rep_enable2g = "<%getInfo("rep_enable2g");%>";

function GetElementType(name,idx) { return document.getElementsByName(name)[idx]; }
function GetIDType(name) { return document.getElementById(name); }

function ButtonDoneEnable() {
	
	if(gonextEnable)
	{
		GetElementType("done0",0).disabled = false;
		GetElementType("done1",0).disabled = false;
		GetIDType("Done").value="1";
	}
	else
	{
		GetElementType("done0",0).disabled = true;
		GetElementType("done1",0).disabled = true;
		GetIDType("Done").value="";
	}
}

function setDisplay() {

	//document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(wiz_survy_info)+'</span>';
	
	GetIDType("WizSurvey").style.display = "block";
	GetIDType("nulltable").style.display = "none";
	GetIDType("nulltable1").style.display = "none";
	GetIDType("nulltable2").style.display = "block";
	GetIDType("button1").style.display = "none";
	GetIDType("button0").style.display = "block";

	setTableWidth(0);
	gonextEnable=0;
	ButtonDoneEnable();
}

function setupmanual() {
	window.location.assign("wiz_rep5gmanual.asp");
}

function checkWEPkey(str, len)
{
	if ((len==5) || (len==13)) //ASCll
		return 1;

	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*' )
			continue;
		alert(showText(InvalidSomething).replace(/#####/,showText(KeyFormat))+' '+showText(ItShouldBeHEX));
		return 0;
	}
	return 1;
}

function checkpassword(input) {

	var x = input
	if (x.value != "")
	{
		if (x.value.indexOf("$") != -1 || x.value.indexOf(",") != -1 || x.value.indexOf(';') != -1 || x.value.indexOf("\\") != -1)
		{
			alert(showText(PasswordNoSupport2));
			x.focus();
			return false
		}
		else
			return true
	}
	else
		return true
}

function checkSSID(input) {

	var x = input
	if (x.value != "")
	{
		if (x.value.indexOf("$") != -1 || x.value.indexOf(",") != -1 || x.value.indexOf(';') != -1 || x.value.indexOf("\\") != -1)
		{
			alert(showText(SSIDNoSupport));
			x.focus();
			return false
		}
		else
			return true
	}
	else
		return true
}

function saveall() {

	var wirelesskey, key_Len;
	
	if ( GetIDType("DUTssid").value == "" ) {
		alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(SSID)));
		return false;
	}
	
	if (!checkSSID(document.formAutoConnect.DUTssid)) return false;

	// SSID
	GetIDType("ssid").value = GetElementType("DUTssid",0).value;

	if(GetIDType("method").value != "0")
	{
		wirelesskey=GetIDType("passwordValue").value;
		
		if (!checkpassword(document.formAutoConnect.passwordValue)) return false;
		
		key_Len=wirelesskey.length;

		//WPA-PSK
		if(GetIDType("method").value == "2" || GetIDType("method").value == "3"){	 
			if (((key_Len >= 8) && (key_Len <= 63)) || (key_Len == 64)){
				if(key_Len == 64){  //HEX
					for (var i=0; i<key_Len; i++) {
							if ( (wirelesskey.charAt(i) >= '0' && wirelesskey.charAt(i) <= '9') ||
							(wirelesskey.charAt(i) >= 'a' && wirelesskey.charAt(i) <= 'f') ||
							(wirelesskey.charAt(i) >= 'A' && wirelesskey.charAt(i) <= 'F') || wirelesskey.charAt(i) == '*')
							continue;
						alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
						return false;
					}
					GetIDType("wpaPskFormat").value = "1";
				}
				else{  //Passphrase
					GetIDType("wpaPskFormat").value = "0";
				}
				GetIDType("wpaPskValue").value = wirelesskey; //Key
			}
			else{
				alert(showText(InvalidSomething).replace(/#####/,showText(keyLen))+' '+showText(shouldbeSomething).replace(/####/,"8-63 or 64 "+showText(characters)));
				return false;
			}
		}
		
		//WEP
		if (GetIDType("method").value == "1"){ 
			if ((key_Len == 5) || (key_Len == 10) || (key_Len == 13) || (key_Len == 26)){
				if (checkWEPkey(wirelesskey, key_Len)==0)
					return false;

				GetIDType("wepEnabled").value = "ON";
				
				if ((key_Len == 5) || (key_Len == 13)){  //ascii
					GetIDType("format").value = "1";
					if(key_Len==5)
						GetIDType("length").value = "1";
					else
						GetIDType("length").value = "2";
				}
				else{
					GetIDType("format").value = "2";     //HEX
					if(key_Len==10)
						GetIDType("length").value = "1";
					else
						GetIDType("length").value = "2";
				}
				GetIDType("key1").value = wirelesskey; //Key
			}		
			else{
				alert(showText(InvalidSomething).replace(/#####/, showText(keyLeninfo))+' '+showText(shouldbeSomething).replace(/####/,"5, 10, 13 or 26 "+showText(characters)));
				return false;
			}
		}

	}

	GetIDType("wlan0Enable").value = "1";
	GetIDType("doConnectTest").value="1";

	document.connect.submit();
}

function goBack()
{
	if(back2survy)
	{
		//document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(wiz_survy_info)+'</span>';
		window.location.assign("wiz_rep5gauto.asp");
	}
	else{
		if (rep_enable2g == "1") //24g enable
			window.location.assign("wiz_ip.asp");
		else
			window.location.assign("wiz_crossband.asp");
	}
}
function RESitesurvey() {
	reload_survey=1;
	window.frames[0].location="wiz_wlsurvey5g.asp";
}
function loadonstart() {
	GetElementType("manualclick",0).checked = false;
	//GetElementType("HideSSIDclick",0).checked = false;
	document.formAutoConnect.done0.disabled = true;
	document.formAutoConnect.done1.disabled = true;
	setDisplay();
}
function setHideSSID()
{
	if (document.formAutoConnect.HideSSIDclick.checked==true){
		GetIDType("hiddenSSID").value = "yes";
	}else{
		GetIDType("hiddenSSID").value = "no";
	}
}
function setAutoKey()
{
	document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(manualSSIDinfo)+'</span>';

	GetIDType("autokey").style.display = "block";
	GetIDType("SetupManual").style.display = "none";
	GetIDType("WizSurvey").style.display = "none";

	GetIDType("button0").style.display = "none";
	GetIDType("button1").style.display = "block";

	GetIDType("nulltable").style.display = "block";
	GetIDType("nulltable1").style.display = "block";
	GetIDType("nulltable2").style.display = "none";

	back2survy=1;

	if (GetIDType("method").value == "0")
	{
		GetIDType("passwordValue").disabled = true;
	}
}
function setTableWidth(index)
{
	languIndex=stype;
	if(stype>13) languIndex=13;
	document.all.tablewidth.className="tablewidth"+languIndex;
}
</script>
<style type="text/css">
.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:98%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:98%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:90%;}
</style>
</head>
<body onLoad="setTableWidth();loadonstart();">
<form action="/goform/formWizSurvey" method="POST" name="formAutoConnect">
<input type="hidden" value="" name="totallist" id="totalList">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center" cellspacing="0">
		<tr>
		<td align="right" class="Arial_16">
			<script>
				dw(repeatermode);
			</script>
		</td>
		</tr>
	</table>

	<table border="0" style="height:16px;width:780px" align="center" cellspacing="0">
		<tr>
			<td align="center"><span class="Arial_16" style="font-weight:bold;">
				5GHz&nbsp;<script>dw(WirelessSiteSurvey)</script></span>
			</td>
		</tr>
	</table>
	<table border="0" style="height:110px;" id="tablewidth" class="tablewidth0" align="center" cellspacing="0">
		<tr>
			<!--<td align="left" id="survyinfo"></td>-->
			<td align="left" id="survyinfo"><span class="Arial_16" style="line-height:23px;">
				<script>dw(wiz_survy_info)</script></span>
			</td>
		</tr>
	</table>

<span id="SetupManual" style="display:block;">
	<table border="0" style="height:45px;width:600px" align="center" cellspacing="0">
		<tr>
			<td align="left"><span class="Arial_16" >
				<input type="checkbox" onclick="setupmanual();" name="manualclick">&nbsp;&nbsp;<script>dw(Setupextendermanually)</script>
			</span></td>
		</tr>
	</table>
	<!--<table border="0" style="height:20px;width:600px" align="center" cellspacing="0">
		<tr>
			<td align="left"><span class="Arial_16" >
				<script>
				if(hiddenssid==1)
					document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" checked>');	
				else
					document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick">');
				</script>
				&nbsp;<script>dw(HideSSID)</script>
				</span></td>
		</tr>
	</table>-->
</span>

<span id="autokey" style="display:none;">
	<table border="0" style="height:50px;width:580px" align="center">
		<tr>
			<td style="width:250px" align="right" class="Arial_16">&nbsp;</td>
		</tr>
	</table>
	<table border="0" style="height:80px;width:580px" align="center">
		<tr>
			<td style="width:250px" align="right" class="Arial_16"><script>dw(DeviceSSID)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" id="DUTssid" name="DUTssid" size="23" maxlength="32" value="" class="Arial_16"></td>
		</tr>
		<tr>
			<td style="width:250px" align="right" class="Arial_16"><script>dw(HideSSID)</script>&nbsp;&nbsp;</td>
			<td style="width:390px">
				<script>
					if(hiddenssid==1)
						document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16" checked>');   
					else
						document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16">');		
				</script>			
				<script>dw(enable)</script>
			</td>
		</tr>
		<tr>
			<td style="width:190px" align="right" class="Arial_16"><script>dw(SecurityKey)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" id="passwordValue" name="passwordValue" size="33" maxlength="64" value="" class="Arial_16"></td>
		</tr>
	</table>
	<table border="0" style="height:85px;width:580px" align="center">
		<tr>
			<td style="width:250px" align="right" class="Arial_16">&nbsp;</td>
		</tr>
	</table>
</span>

<span id="WizSurvey" style="display:block;">

	<table border="0" style="height:20px; width:630px;" align="center" cellpadding="2" cellspacing="2">
		<tr style="FONT-FAMILY: Arial,Calibri,Comic Sans MS; font-weight: bold; FONT-SIZE: 16px; color: #FFFFFF;" >
			<td align="center" width="10%" style="background-color:#33666A;" class="Arial_16" id="wlan1_1"><script>dw(select)</script></td>
			<td align="center" width="66%" style="background-color:#33666A;" class="Arial_16" id="wlan1_2"><script>dw(SSID)</script></td>
			<td align="center" width="24%" style="background-color:#33666A;" class="Arial_16" id="wlan1_3"><script>dw(Signal)</script></td>
		</tr>	
	</table>

		<table border="0" style="height:0px;width:660px;" align="center">
			<tr>
				<td>
				  <iframe name="sitesurvey" src="/wiz_wlsurvey5g.asp" style="height:170px;width:660px; overflow-x:hidden; overflow-y:hidden;" frameBorder="0"></iframe>
				</td>
			</tr>
		</table>
	</span>

<span id="nulltable" style="display:none;">
	<table border="0" style="height:20px;width:700px" cellspacing="0">
	<tr><td></td></tr>
	</table>
</span>
<span id="nulltable1" style="display:none;">
	<table border="0" style="height:15px;width:700px" align="center" cellspacing="0">
		<tr><td></td></tr>
	</table>
</span>
<span id="nulltable2" style="display:block;">
	<table border="0" style="height:7px;width:700px" align="center" cellspacing="0">
		<tr><td></td></tr>
	</table>
</span>

<span id="button0" style="display:block;">

	<table border="0" style="height:28px;width:780px" align="center" cellspacing="0">
	<tr>
	<td align="right" width="40%">
	<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">');</script>
	</td>
	<td align="center">
	<script>document.write('<input type="button" value="'+showText(refresh)+'" name="Refresh" onClick="RESitesurvey()" class="button_IQ2" style="cursor:pointer">');</script>
	</td>
	<td align="left" width="40%">
	<script>document.write('<input type="button" value="'+showText(Next)+'" name="done0" onClick="return saveall()" class="button_IQ2gray" style="cursor:pointer">');</script>
	</td>
	</tr>
	</table>
</span>

<span id="button1" style="display:none;">
<table border="0" style="height:28px;width:780px" align="center" cellspacing="0">
	<tr>
	<td align="right" style="width:390px">
	<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
	</td>
	<td align="left" style="width:390px">
	<script>document.write('<input type="button" value="'+showText(Next)+'" name="done1" onClick="return saveall()" class="button_IQ2" style="cursor:pointer">');</script>
	</td>
	</tr>
	</table>
</span>

	<table border="0" style="height:60px;width:780px" align="center" cellspacing="0">
		<tr><td></td></tr>
	</table>

</td>
</tr>
</table>
</div>
</form>
<form action="/goform/formiNICbasic" method="POST" name="connect">
	<table width="780" border="0" cellspacing="0" align=center>
	<input type="hidden" value="/connect.asp" id="submit-url" name="submit-url">
	<input type="hidden" value="6" name="apMode" id="apMode">
	<input type="hidden" value="0" name="wisp">
	<input type="hidden" value="no" name="hiddenSSID" id="hiddenSSID">
	<input type="hidden" value="" name="repeaterSSID" id="repeaterSSID">
	<input type="hidden" value="" name="ssid" id="ssid">
	<input type="hidden" value="19" name="chan" id="chan">
	<input type="hidden" value="" name="secchan" id="secchan">
	<input type="hidden" value="" name="method" id="method">
	<input type="hidden" value="" name="wpaCipher" id="wpaCipher">
	<input type="hidden" value="" name="wepEnabled" id="wepEnabled">
	<input type="hidden" value="" name="length" id="length">
	<input type="hidden" value="" name="format" id="format">
	<input type="hidden" value="1" name="defaultTxKeyId">
	<input type="hidden" value="" name="key1" id="key1">
	<input type="hidden" value="" name="wpaPskFormat" id="wpaPskFormat">
	<input type="hidden" value="" name="wpa2PskFormat" id="wpa2PskFormat">
	<input type="hidden" value="" name="wpaPskValue" id="wpaPskValue">
	<input type="hidden" value="" name="wpa2PskValue" id="wpa2PskValue">
	<input type="hidden" value="0" name="wlan0Enable" id="wlan0Enable">
	<input type="hidden" value="" name="done" id="Done">
	<input type="hidden" value="" name="isApply">
	<input type="hidden" value="" name="doConnectTest" id="doConnectTest">
	<input type="hidden" value="" name="rootAPmac" id="rootAPmac">
	<input type="hidden" value="0" name="supportAllSecurity">
	</table>
</form>
<script>
	setHideSSID();
</script>
</body>
</html>
