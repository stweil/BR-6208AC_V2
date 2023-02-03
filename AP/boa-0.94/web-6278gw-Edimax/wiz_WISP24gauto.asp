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
var wiz_wispManu = "<% getInfo("wiz_wispManu"); %>";
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>";
var hiddenssid = <%getVar("hiddenSSID","","");%>;
var wizmode = <%getVar("wizmode","","");%>;
var connectEnabled=0, back2survy=0 ,reload_survey=1;
var sel_value2, gonextEnable=0;

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

	//document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(wiz_WISPsurvy_info)+'</span>';
	
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
	window.location.assign("wiz_WISP24gmanual.asp");
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

function saveall() {

	var wirelesskey, keyLen;
	
	GetIDType("stadrv_ssid").value = GetIDType("rootssid").value;
	if ( GetIDType("stadrv_ssid").value == "" ) {
		alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(SSID)));
		return false;
	}

	if(GetIDType("stadrv_encrypttype").value != "0")
	{
		wirelesskey=GetIDType("passwordValue").value;
		keyLen=wirelesskey.length;
		//WPA2
		if(GetIDType("stadrv_encrypttype").value == "3"){	 
			if (((keyLen >= 8) && (keyLen <= 63)) || (keyLen == 64)){
				if(keyLen == 64){  //HEX
					for (var i=0; i<keyLen; i++) {
							if ( (wirelesskey.charAt(i) >= '0' && wirelesskey.charAt(i) <= '9') ||
							(wirelesskey.charAt(i) >= 'a' && wirelesskey.charAt(i) <= 'f') ||
							(wirelesskey.charAt(i) >= 'A' && wirelesskey.charAt(i) <= 'F') || wirelesskey.charAt(i) == '*')
							continue;
						alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
						return false;
					}
					GetIDType("stadrv_WPA2pskformat").value = "1";
				}
				else{  //Passphrase
					GetIDType("stadrv_WPA2pskformat").value = "0";
				}
				GetIDType("stadrv_WPA2pskkey").value = wirelesskey; //Key
			}
			else{		
				alert(showText(InvalidSomething).replace(/#####/,showText(keyLeninfo))+" "+showText(shouldbeSomething).replace(/####/,"8-63 or 64 "+showText(characters)));
				return false;
			}
		}		
		
	}

	if ((wiz_wispManu == "1"))
		GetIDType("submit-url").value = "/mstart.asp";
	else
		GetIDType("submit-url").value = "/wisp_connect.asp";

	document.connect.submit();
}

function goBack()
{
	if(back2survy)
		window.location.assign("wiz_WISP24gauto.asp");
	else
		window.location.assign("wiz_WISPselect1.asp");
}
function RESitesurvey() {
	reload_survey=1;
	window.frames[0].location="wiz_wlsurvey24g.asp";
}
function loadonstart() {
	GetElementType("manualclick",0).checked = false;
	//GetElementType("HideSSIDclick",0).checked = false;
	document.formAutoConnect.done0.disabled = true;
	document.formAutoConnect.done1.disabled = true;
	setDisplay();
}
/*function setHideSSID()
{
	if(GetIDType("hiddenSSID").value == "no")
		GetIDType("hiddenSSID").value = "yes";
	else
		GetIDType("hiddenSSID").value = "no";
}*/
function setAutoKey()
{
	if(stype>=13)
		document.all.tablewidth.className="tablewidthsmall0";
	else
		document.all.tablewidth.className="tablewidthsmall";

	document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(wisp_autoinfo)+'</span>';

	GetIDType("autokey").style.display = "block";
	GetIDType("SetupManual").style.display = "none";
	GetIDType("WizSurvey").style.display = "none";

	GetIDType("button0").style.display = "none";
	GetIDType("button1").style.display = "block";

	GetIDType("nulltable").style.display = "block";
	GetIDType("nulltable1").style.display = "block";
	GetIDType("nulltable2").style.display = "none";

	back2survy=1;

	if (GetIDType("stadrv_encrypttype").value == "0")
		GetIDType("passwordValue").disabled = true;
}
function setTableWidth(index)
{
	if(stype>13) stype=13;
	document.all.tablewidth.className="tablewidth"+index;
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
.tablewidthsmall {width:65%;}
.tablewidthsmall0 {width:50%;}
</style>
</head>
<body onLoad="setTableWidth();loadonstart();">
<form action="/goform/formWizSurvey" method="POST" name="formAutoConnect">
<input type="hidden" value="" name="totallist" id="totalList">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center">
		<tr><td align="right" class="Arial_16"><script>dw(wisp);</script></td></tr>
	</table>

	<table border="0" style="height:15px;width:780px" align="center" cellspacing="0">
		<tr>
			<td align="center"><span class="Arial_16" style="font-weight:bold;">
				2.4GHz&nbsp;<script>dw(WirelessSiteSurvey)</script></span>
			</td>
		</tr>
	</table>

	<table border="0" style="height:110px;" id="tablewidth" class="tablewidth0" align="center" cellspacing="0">
		<!--<tr><td align="left" id="survyinfo"></td></tr>-->
		<td align="left" id="survyinfo"><span class="Arial_16" style="line-height:23px;">
			<script>dw(wiz_WISPsurvy_info)</script></span>
		</td>
	</table>

<span id="SetupManual" style="display:block;">
	<table border="0" style="height:45px;width:600px" align="center" cellspacing="0">
		<tr>
			<td align="left"><span class="Arial_16" >
				<input type="checkbox" onclick="setupmanual();" name="manualclick">&nbsp;&nbsp;<script>dw(Setupextendermanually_WISP)</script>
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
		<tr><td style="width:250px" align="right" class="Arial_16">&nbsp;</td></tr>
	</table>

	<table border="0" style="height:80px;width:580px" align="center">
		<tr>
			<td style="width:250px" align="right" class="Arial_16"><script>dw(DeviceSSID)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" id="rootssid" name="rootssid" size="23" maxlength="32" value="" class="Arial_16" disabled="true"></td>
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
				  <iframe name="sitesurvey" src="/wiz_wlsurvey24g.asp" style="height:170px;width:660px;overflow-x:hidden;overflow-y:hidden;" frameBorder="0"></iframe>
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
<form action="/goform/formStaDrvSetup" method="POST" name="connect">
	<table width="780" border="0" cellspacing="0" align="center">
	<input type="hidden" value="" id="submit-url" name="submit-url">
	<input type="hidden" value="19" name="stadrv_chan" id="stadrv_chan">
	<input type="hidden" value="" name="stadrv_ssid" id="stadrv_ssid">
	<input type="hidden" value="" name="secchan" id="secchan">
	<input type="hidden" value="" name="stadrv_encrypttype" id="stadrv_encrypttype">
	<input type="hidden" value="" name="stadrv_WPAcipher" id="stadrv_WPAcipher">
	<input type="hidden" value="" name="stadrv_WPA2cipher" id="stadrv_WPA2cipher">
	<input type="hidden" value="" name="stadrv_weplength" id="stadrv_weplength">
	<input type="hidden" value="" name="stadrv_wepformat" id="stadrv_wepformat">
	<input type="hidden" value="" name="key1" id="key1">
	<input type="hidden" value="" name="stadrv_WPApskformat" id="stadrv_WPApskformat">
	<input type="hidden" value="" name="stadrv_WPA2pskformat" id="stadrv_WPA2pskformat">
	<input type="hidden" value="" name="stadrv_WPApskkey" id="stadrv_WPApskkey">
	<input type="hidden" value="" name="stadrv_WPA2pskkey" id="stadrv_WPA2pskkey">
	<input type="hidden" value="" name="done" id="Done">
	<input type="hidden" value="0" name="stadrv_band" id="stadrv_band">
	<input type="hidden" value="1" name="stadrv_type" id="stadrv_type">
	<input type="hidden" value="0" name="stadrv_defaultkey" id="stadrv_defaultkey">
	<input type="hidden" value="" name="isApply">
	<input type="hidden" value="<% getInfo("WISP_enable2g"); %>" name="WISP_enable2g">
	<input type="hidden" value="<% getInfo("wiz_wispManu"); %>" name="wiz_wispManu">
	<input type="hidden" value="0" name="supportAllSecurity">
	<input type="hidden" value="" name="rootAPmac" id="rootAPmac">
</form>
</body>
</html>
