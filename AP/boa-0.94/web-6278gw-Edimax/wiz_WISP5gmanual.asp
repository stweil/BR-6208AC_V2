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
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>";

var wizmode = <%getVar("wizmode","","");%>;
var back2survy=0, gonextEnable=0;

function GetElementType(name,idx) { return document.getElementsByName(name)[idx]; }
function GetIDType(name) { return document.getElementById(name); }

function changeAll() { 
	var wlan1SSID=GetElementType("manualssid",0).value;
	
}

function init_changeAll() { 
	var wlan1SSID=GetElementType("manualssid",0).value;

}


function ButtonDoneEnable() {
	if(gonextEnable)
	{
		GetElementType("next",0).disabled = false;
		document.formWizSurvey.done.value="1";
	}
	else
	{
		GetElementType("next",0).disabled = true;
		document.formWizSurvey.done.value="";
	}
}


function gonext() {

	var i,j;
	var WlencryptFrame;

	WlencryptFrame = window.frames["wlencrypt"];

	if( !WlencryptFrame.saveChanges())
		return false;

	if ( GetElementType("manualssid",0).value == "" ) {
		alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(SSID)));
		return false;
	}
	else {
		security=WlencryptFrame.document.wisp_form.stadrv_encrypttype.value;	

		GetIDType("stadrv_encrypttype").value = security;
		
		if(security == 3){	
			//Cipher=1
			Cipher=2
			wpa2PskFormat = WlencryptFrame.document.wisp_form.stadrv_WPA2pskformat.value;
			GetIDType("stadrv_WPA2cipher").value = Cipher;
			GetIDType("stadrv_WPA2pskkey").value = WlencryptFrame.document.wisp_form.stadrv_WPA2pskkey.value;
			GetIDType("stadrv_WPA2pskformat").value = wpa2PskFormat;
		}
	}

	GetIDType("stadrv_ssid").value = document.rep5g.manualssid.value;

	if ((wiz_wispManu == "1"))
		GetIDType("submit-url").value = "/mstart.asp";
	else
		GetIDType("submit-url").value = "/wisp_connect.asp";

	GetIDType("stadrv_chan").value = GetElementType("channelno",0).value;
	document.formWizSurvey.submit();
}

function goBack()
{
	window.location.assign("wiz_WISP5gauto.asp");
}

function loadonstart() {
	document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(wisp_manualSSIDinfo)+'</span>';
}
function setTableWidth()
{
	if(stype>13) stype=13;
	
	document.all.tablewidth.className="tablewidth"+stype;
	document.all.survyinfo.className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:90%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:90%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:52%;}
</style>
</head>
<body onLoad="setTableWidth();loadonstart()">
<form action="/goform/formWizSurvey" method="POST" name="rep5g">
<input type="hidden" value="" name="totallist" id="totalList">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center" cellspacing="0">
		<tr><td align="right" class="Arial_16"><script>dw(wisp);</script></td></tr>
	</table>

	<table border="0" style="height:15px;width:780px" align="center" cellspacing="0">
		<tr>
			<td align="center"><span class="Arial_16" style="font-weight:bold;">5GHz&nbsp;
				<script>dw(WirelessSiteSurvey)</script></span>
			</td>
		</tr>
	</table>
	<table border="0" style="height:90px;" id="tablewidth" class="tablewidth0" align="center">
		<tr>
			<td align="left" id="survyinfo" class="tablewidth0">
			</td>
		</tr>
	</table>

	<table border="0" style="height:50px;width:780px" align="center" cellspacing="0">
		<tr>
			<td style="width:294px" align="right" class="Arial_16"><script>dw(wiz_repmenu1)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" name="manualssid" onkeyup="changeAll()" size="23" maxlength="32" value="" class="Arial_16"></td>
		</tr>
		<!--<tr>
			<td style="width:250px" align="right" class="Arial_16"><script>dw(DeviceSSID)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" name="manualdevssid" size="23" maxlength="32" value="" class="Arial_16"></td>
		</tr>-->
		<tr>
				<td style="width:294px" align="right" class="Arial_16"><script>dw(ChannelNum)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
					<select size="1" name="channelno" class="Arial_16">
						<script>

							regDomain = <%getiNICVar("regDomain","","");%>;
							defaultChan = <%getiNICVar("channel","","");%>;
							var Reg_to_Domain = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11");
							var region_A = Reg_to_Domain[regDomain];

							/*if( defaultChan == 0)
								document.write('<option selected value="0">'+showText(Auto)+'</option>');
							else
								document.write('<option value="0">'+showText(Auto)+'</option>');*/

							var achnn = new Array(
								new Array("36", "40", "44", "48", "52", "56", "60", "64", "149", "153", "157", "161", "165"),
								new Array("36", "40", "44", "48", "52", "56", "60", "64", "100", "104", "108", "112", "116", "120", "124", "128", "132", "136", "140"),
								new Array("36", "40", "44", "48", "52", "56", "60", "64"),
								new Array("52", "56", "60", "64", "149", "153", "157", "161"),
								new Array("149", "153", "157", "161", "165"),
								new Array("149", "153", "157", "161"),
								new Array("36", "40", "44", "48"),
								new Array("36", "40", "44", "48", "52", "56", "60", "64", "100", "104", "108", "112", "116", "120", "124", "128", "132", "136", "140", "149", "153", "157", "161", "165"),
								new Array("52", "56", "60", "64"),
								new Array("36", "40", "44", "48", "52", "56", "60", "64", "100", "104", "108", "112", "116", "132", "136", "140", "149", "153", "157", "161", "165"),
								new Array("36", "40", "44", "48", "149", "153", "157", "161", "165"),
								new Array("36", "40", "44", "48", "52", "56", "60", "64", "100", "104", "108", "112", "116", "120", "149", "153", "157", "161")
							);   
						
							for (i=0;i< achnn[region_A].length;i++)
							{
								/*if ( achnn[region_A][i] == defaultChan)
									document.write('<option selected value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
								else*/
									document.write('<option value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
							}
						</script>
					</select>
				</td>
			</tr>		
	</table>
	<table border="0" style="width:780px;" align="center" cellspacing="0">
		<tr>
			<td align="center">
				<iframe id="wlencrypt" name="wlencrypt" src="/wiz_WISPsecurity.asp" style="height:155px;width:600px;" scrolling="no" frameBorder="0"></iframe>
			</td>
		</tr>
	</table>

	<table border="0" style="height:65px;width:780px" align="center" cellspacing="0">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:28px;width:780px" align="center" cellspacing="0">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="next" onClick="return gonext()" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:60px;width:780px" align="center" cellspacing="0">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>
</form>
<form action="/goform/formStaDrvSetup" method="POST" name="formWizSurvey">
	<table width="780" border="0" cellspacing="0" align=center>
	<input type="hidden" value="/mstart.asp" id="submit-url" name="submit-url">
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
	<input type="hidden" value="" name="isApply">
	<input type="hidden" value="1" name="stadrv_band" id="stadrv_band">
	<input type="hidden" value="1" name="stadrv_type" id="stadrv_type">
	<input type="hidden" value="" name="stadrv_ssid" id="stadrv_ssid">
	<input type="hidden" value="0" name="stadrv_defaultkey" id="stadrv_defaultkey">
	<input type="hidden" value="<% getInfo("WISP_enable5g"); %>" name="WISP_enable5g">
	<input type="hidden" value="<% getInfo("wiz_wispManu"); %>" name="wiz_wispManu">
	<input type="hidden" value="1" name="WISPMANU">
	<input type="hidden" value="0" name="supportAllSecurity">
	<input type="hidden" value="36" name="stadrv_chan" id="stadrv_chan">
	<input type="hidden" value="0" name="secchan" id="secchan">
</form>
<script>
init_changeAll();
</script>
</body>
</html>
