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
var ssidTbl=<%getInfo("ssidAll");%>;
var repeaterssid =	"<%getInfo("repeaterSSID");%>";

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

function gonext() {

	var i,j;
	var WlencryptFrame;

	WlencryptFrame = window.frames["wlencrypt"];

	if( !WlencryptFrame.saveChanges())
		return false;

	if ( GetElementType("manualssid",0).value == "" ) 
		{
			alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(SSID)));
			return false;
		}
		else if (!checkSSID(document.rep24g.manualssid))
		{
			return false;
		}			
	else 
		{
			security=WlencryptFrame.document.wlEncrypt.method.value;	

			GetIDType("method").value = security;

			if(security == 2 || security == 3){	

				for(i=0; i<3; i++)
					if( WlencryptFrame.document.getElementsByName("wpaCipher")[i].checked ) {Cipher=i+1;break;}

				wpaPskFormat = WlencryptFrame.document.wlEncrypt.wpaPskFormat.value;

				GetIDType("wpaCipher").value = Cipher;
				GetIDType("wpaPskValue").value = WlencryptFrame.document.wlEncrypt.wpaPskValue.value;
				GetIDType("wpaPskFormat").value = wpaPskFormat;
			}
			if (security == 1){	
				GetIDType("wepEnabled").value = "ON";
				GetIDType("length").value = WlencryptFrame.document.wlEncrypt.length.value;
				GetIDType("format").value = WlencryptFrame.document.wlEncrypt.format.value;
				GetIDType("key1").value = WlencryptFrame.document.wlEncrypt.key1.value;
				//alert(GetIDType("length").value+" "+GetIDType("format").value+" "+GetIDType("key1").value);
			}
		}
	//}
	GetIDType("wlan1Enable").value = "1";
	document.formWizSurvey.doConnectTest.value="1";
	GetIDType("chan").value = GetElementType("channelno",0).value;

	GetIDType("repeaterSSID").value = document.rep24g.manualssid.value;
	document.formWizSurvey.submit();
}

function goBack()
{
	window.location.assign("wiz_APcl24gauto.asp");
}

function loadonstart() {
	document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(APclmanualSSIDinfo)+'</span>';
}
function setTableWidth()
{
	languIndex=stype;
	if(stype>13) languIndex=13;

	document.all.tablewidth.className="tablewidth"+languIndex;
	document.all.survyinfo.className="tablewidth"+languIndex;
}
</script>
<style type="text/css">
.tablewidth0 {width:76%;}
.tablewidth1 {width:80%;}
.tablewidth2 {width:85%;}
.tablewidth3 {width:85%;}
.tablewidth4 {width:80%;}
.tablewidth5 {width:80%;}
.tablewidth6 {width:75%;}
.tablewidth7 {width:75%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:85%;}
.tablewidth10 {width:78%;}
.tablewidth11 {width:78%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:73%;}
</style>
</head>
<body onLoad="setTableWidth();loadonstart()">
<form action="/goform/formWizSurvey" method="POST" name="rep24g">
<input type="hidden" value="" name="totallist" id="totalList">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center" cellspacing="0">
		<tr>
			<td align="right" class="Arial_16">
				<script>
					 dw(APcl);
				</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:15px;width:780px" align="center" cellspacing="0">
		<tr>
			<td align="center"><span class="Arial_16" style="font-weight:bold;">2.4GHz&nbsp;
				<script>dw(WirelessSiteSurvey)</script></span>
			</td>
		</tr>
	</table>
	<table border="0" style="height:90px;" id="tablewidth" class="tablewidth0" align="center" cellspacing="0">
		<tr>
			<td align="left" id="survyinfo" class="tablewidth0">
			</td>
		</tr>
	</table>

	<table border="0" style="height:50px;width:780px" align="center" cellspacing="0">
		<tr>
			<td style="width:294px" align="right" class="Arial_16"><script>dw(wiz_repmenu1)</script>&nbsp;&nbsp;</td>
			<td style="width:390px"><input type="text" name="manualssid" id="manualssid" onkeyup="changeAll()" size="23" maxlength="32" value="" class="Arial_16"></td>
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
							var regDomain = <%getVar("regDomain","","")%>;
	            var defaultChan = <%getVar("channel","","")%>;
	            var domainMin = new Array("1","1","10","10","14","1","3","5","1");
	            var domainMax = new Array("11","13","11","13","14","14","9","13","14");
					
							/*if( defaultChan == 0)
								document.write('<option selected value="0">'+showText(AutoChen)+'</option>');
							else
								document.write('<option value="0">'+showText(AutoChen)+'</option>');*/

							for (i=0; i<=8; i++) {
								if ( i == regDomain ) {
									for (j=parseInt(domainMin[i],10); j<=parseInt(domainMax[i],10); j++) {
										/*if ( j==defaultChan)
											document.write('<option selected value="'+j+'">'+j+'</option>');
										else*/
											document.write('<option value="'+j+'">'+j+'</option>');
									}
								}
							}
						</script>
					</select>
				</td>
			</tr>
	</table>
	<table border="0" style="width:780px;" align="center" cellspacing="0">
		<tr>
			<td align="center">
				<iframe id="wlencrypt" name="wlencrypt" src="/wiz_rep24gsecurity.asp" style="height:155px;width:600px;" scrolling="no" frameBorder="0"></iframe>
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
<form action=/goform/formWlbasic method=POST name="formWizSurvey">
	<table width="780" border="0" cellspacing="0" align=center>
	<input type="hidden" value="/connect.asp" id="submit-url" name="submit-url">
	<input type="hidden" value="6" name="apMode" id="apMode">
	<input type="hidden" value="0" name="wisp">
	<input type="hidden" value="no" name="hiddenSSID" id="hiddenSSID">
	<input type="hidden" value="" name="repeaterSSID" id="repeaterSSID">
	<input type="hidden" value="" name="ssid" id="ssid">
	<input type="hidden" value="1" name="chan" id="chan">
	<input type="hidden" value="0" name="secchan" id="secchan">
	<input type="hidden" value="" name="method" id="method">
	<input type="hidden" value="" name="wpaCipher" id="wpaCipher">
	<input type="hidden" value="" name="wpa2Cipher" id="wpa2Cipher">
	<input type="hidden" value="" name="wepEnabled" id="wepEnabled">
	<input type="hidden" value="" name="length" id="length">
	<input type="hidden" value="" name="format" id="format">
	<input type="hidden" value="1" name="defaultTxKeyId">
	<input type="hidden" value="" name="key1" id="key1">
	<input type="hidden" value="" name="wpaPskFormat" id="wpaPskFormat">
	<input type="hidden" value="" name="wpaPskValue" id="wpaPskValue">
	<input type="hidden" value="" name="wpa2PskFormat" id="wpa2PskFormat">
	<input type="hidden" value="" name="wpa2PskValue" id="wpa2PskValue">
	<input type="hidden" value="0" name="wlan1Enable" id="wlan1Enable">
	<input type="hidden" value="" name="done" id="Done">
	<input type="hidden" value="" name="isApply">
	<input type="hidden" value="" name="doConnectTest" id="doConnectTest">
	<input type="hidden" value="0" name="supportAllSecurity">
</form>
<script>
init_changeAll();
</script>
</body>
</html>
