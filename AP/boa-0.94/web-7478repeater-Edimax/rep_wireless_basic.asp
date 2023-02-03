<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
var useCrossBand = "<%getInfo("useCrossBand");%>";
var wizMode = <%getVar("wizmode","","");%>;
var apMode = <%getVar("apMode","","");%>;
var methodVal5g = <%getiNICVar("encrypt","","");%>;
var methodVal2g = <%getVar("encrypt","","");%>;
var bandVal = <%getVar("band","","");%>;
var opMode = <%getVar("opMode","","");%>;
var wpsConfigType = <%getVar("wpsConfigType","","");%>;
var wlsw = <%getVar("wlanDisabled","","");%>;
var wlsw5g = <%getiNICVar("wlanDisabled","","");%>;

var settingChannel=0
if ((useCrossBand=="1") && (wlsw ^ wlsw5g)) settingChannel=1;

var wlswoff = 0;
var pskValueTbl5g =	<%getiNICInfo("pskValueAll");%>;
var pskValueTbl2g = <%getInfo("pskValueAll");%>;
var ssidTbl2g = <%getInfo("Gssidtbl");%>;
var ssidTbl5g = <%getiNICInfo("Assidtbl");%>;
var apIso= <%getVar("noForward","","");%>; 
var hidSSID = <%getVar("hiddenSSID","","");%>;
var EncryptionTbl = new Array(showText(OpenSecurity),showText(WEP),showText(WPA),showText(WPA2));
var wpaCipherSuite = new Array("TKIP","AES");
var wepkeyLen2g = <%getVar("wep","","");%>;
var type2g = <%getVar("keyType","","");%>;
var wepkeyLen5g = <%getiNICVar("wep","","");%>;
var type5g = <%getiNICVar("keyType","","");%>;

var wepValueAll2g = <% getInfo("wepValueAll");%>;
var wepValueAll5g = <% getiNICInfo("wepValueAll");%>;

var wpaEncrypt5g =	<%getiNICInfo("wpaEncrypt");%>; 
var wpaEncrypt2g =	<%getInfo("wpaEncrypt");%>; 


var cur_Chan="<% getInfo("autochannel"); %>";

if(wizMode==2 && useCrossBand == "1"){
	wpaCipher =	wpaEncrypt5g[0];
	wpa2Cipher = wpaEncrypt5g[1];
}
else{
	wpaCipher =	wpaEncrypt2g[0];
	wpa2Cipher = wpaEncrypt2g[1];
}
wpaMode = parseInt(wpaCipher,10) + parseInt(wpa2Cipher,10);

function setKeyValue()
{
	if(useCrossBand == "1"){
		methodVal = methodVal5g;
		pskval = pskValueTbl5g[0];
		WEP64Key1 = wepValueAll5g[0];
		WEP128Key1 = wepValueAll5g[4];
		wepkeyLen = wepkeyLen5g;
		type = type5g;
	}
	else{
		methodVal = methodVal2g;
		pskval = pskValueTbl2g[0];
		WEP64Key1 = wepValueAll2g[0];
		WEP128Key1 = wepValueAll2g[4];
		wepkeyLen = wepkeyLen2g;
		type = type2g;
	}

	if(methodVal==0){
		document.getElementById("Encryption").innerHTML='<span class="choose-itemText ">'+EncryptionTbl[0]+'</span>';
		document.getElementById('key').style.display = "none";
	}
	else{
		if(methodVal==2 || methodVal==3)
			document.getElementById("Encryption").innerHTML = '<span class="choose-itemText ">'+EncryptionTbl[methodVal]+'('+wpaCipherSuite[wpaMode-1]+')</span>';
		else
			document.getElementById("Encryption").innerHTML = '<span class="choose-itemText ">'+EncryptionTbl[methodVal]+'</span>';

		document.getElementById('key').style.display = "block";

		if(methodVal == 2 || methodVal == 3){
			if (pskval.search('"') != -1) pskval = pskval.replace(/\"/g, "&quot;");
			keystr=pskval;
		}
		else{
			
			with(document.wlanSetup) {
				if(wepkeyLen != 2) {
					if (type == 0)  keystr = WEP64Key1.substring(0,5);
					else keystr = WEP64Key1;
				}
				else{
					if (type == 0) keystr = WEP128Key1.substring(0,13);
					else  keystr = WEP128Key1;
				}
			}
		}

		if(keystr.length > 40)
			keystr=keystr.substring(0,40)+'<br>'+keystr.substring(40,keystr.length);

		document.getElementById("EncryptionKey").innerHTML = '<span class="choose-itemText ">'+keystr+'</span>';
	}

}

function init()
{
	$("#help-box").hide();

	with(document.wlanSetup) {

		//WLDisabled.disabled = true;

		if(apIso == 1) IsolationEnabled.checked=true;
		else IsolationEnabled.checked=false;

		if(hidSSID == 1) hiddenSsid.checked=true;
		else hiddenSsid.checked=false;

		if(useCrossBand == "1") 
			ssid.value = ssidTbl5g[0];
		else 
			ssid.value = ssidTbl2g[0];


		if(wlsw){
			ssid.disabled = true;
			document.getElementById('IsolationEnable').disabled = true;
			document.getElementById('hiddenSsid').disabled = true;
			document.getElementById('save').disabled = true;
			document.getElementById('wlClient').disabled = true;
			if(settingChannel)
				document.getElementById('chann').disabled = true;
		}
	}
}

function open_Window(url)
{
	window.open( url, 'Wireless', 'width=800, height=600 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
}



function saveChanges(rebootFlag)
{
	
	with(document.wlanSetup) {

		//if (document.wlanSetup.WLDisabled.checked != true){
		if (wlsw != 1){
			wlanDisabled.value="no";
			if (ssid.value=="") {
				alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(essid)));
				ssid.value = ssid.defaultValue;
				ssid.focus();
				return false;
			}
	
			IsolationSwitch();		
			hiddenSsidSwitch();
		}
		else{
			wlanDisabled.value="yes";
		}
		
		if(useCrossBand == "1")
			repUseCrossBand.value = "1";

		if(rebootFlag)  isApply.value = "ok";

		submit();
	}
}


function IsolationSwitch()
{
	with(document.wlanSetup) 
	{
		if (IsolationEnabled.checked==true)
		 	wireless_block_relay.value="on";
		else
		 	wireless_block_relay.value="off";
	}
}
function hiddenSsidSwitch()
{
	with(document.wlanSetup) 
	{
		if (document.wlanSetup.hiddenSsid.checked==true)
		 hiddenSSID.value="yes";
		else
		 hiddenSSID.value="no";
	}
}
</script>
</head>


<body onLoad="init();">

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td>
						<div class="help-text" id="help-info1"> 
							<script>dw(WlBasicHelpInfo)</script>&ensp;<script>dw(WlEncryptionHelpInfo)</script> 				
				 		</div>
					</td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>
<blockquote>


<form action="/goform/formWlbasic" method="POST" name="wlanSetup">
	<fieldset name="wlanSetup_fieldset" id="wlanSetup_fieldset">
		<legend><script>dw(basicsetting)</script></legend>
					<input type="hidden" name="repUseCrossBand" value="">
					<input type="hidden" name="wlanDisabled" value="">
					<input type="hidden" value="<%getVar("opMode","","");%>" name="wisp">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">

						<!--<tr>
							<td colspan="2"><input type="checkbox" name="WLDisabled" value="no" <%getVar("wlanDisabled","1","checked");%> >&nbsp;<span class="itemText"><script>dw(disable)</script>&ensp;<script>dw(Wireless)</script></span></td>
						</tr>-->

						<tr>
							<td><div class="itemText-len"><script>dw(Band)</script></div></td>
							<input type="hidden" name="band" size="25" maxlength="32" value="4">
							<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
													document.write(showText(band4));
											</script>	
										</span></div>
							</td>
						</tr>

						<tr>
							<td><div class="itemText-len"><script>dw(WirelessNetworkName)</script>&ensp;(<script>dw(essid)</script>)</div></td>
							<td><div class="itemText2-len">
										<input type="text" name="ssid" size="25" maxlength="32" value="">
							</div></td>
						</tr>
					</table>

					<table  align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="hiddenSsid" id="hiddenSsid" onclick="hiddenSsidSwitch()">&nbsp;
								<span class="itemText"><script>dw(Hide)</script>&nbsp;<script>dw(essid)</script></span>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" id="IsolationEnable" name="IsolationEnabled" value="no" onclick="IsolationSwitch()">&nbsp;
								<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(wlClients)</script>&nbsp;<script>dw(Isolation)</script></span>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len"><script>dw(ChannelNumber)</script></div></td>
							<!--<td><div class="itemText2-len"><span class="choose-itemText ">
								<script>			
									var defaultChan = <%getVar("channel","","")%>;
									document.write(cur_Chan);
								</script>
							</span></div></td>-->
				
							<script>
							var regDomain = <%getVar("regDomain","","")%>;
		          var defaultChan = <%getVar("channel","","")%>;
		          var domainMin = new Array("1","1","10","10","14","1","3","5","1");
		          var domainMax = new Array("11","13","11","13","14","14","9","13","14");

							if(settingChannel){
								document.write('<td><div class="itemText2-len">');
								document.write('	<select size="1" name="chan" id="chann">');
									
								if( defaultChan == 0)
									document.write('<option selected value="0">'+showText(Auto)+'</option>');
								else
									document.write('<option value="0">'+showText(Auto)+'</option>');

								for (i=0; i<=8; i++) {
									if ( i == regDomain ) {
										for (j=parseInt(domainMin[i],10); j<=parseInt(domainMax[i],10); j++) {
											if ( j==defaultChan)
												document.write('<option selected value="'+j+'">'+j+'</option>');
											else
												document.write('<option value="'+j+'">'+j+'</option>');
										}
									}
								}
									
								document.write('	</select>');
								document.write('</div></td>');
							}
							else{
								document.write('<td><div class="itemText2-len"><span class="choose-itemText ">');
								document.write(cur_Chan);
								document.write('</span></div></td>');
							}
							</script>

						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(SignalStrength)</script></div></td>
								<td><div class="itemText2-len"><span class="choose-itemText ">
							<script>
									var connectStatus2G = "<% getInfo("connectStatus2G"); %>";
									document.write('<div id="width2" style="float:left; width:0%"></div>');
									if(connectStatus2G=="NotAssociated" || connectStatus2G=="disable"){
										document.write('<div style="float:left;">&nbsp;0%</div>');	
									}
									else{
										document.write('<div style="float:left;">&nbsp;'+connectStatus2G+'%</div>');	
										document.getElementById("width2").style.width = parseInt(connectStatus2G,10)*0.73+'%';
									}
							</script>
							</span></div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(wlClients)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(showClients)+'" name="wlClient" id="wlClient" onClick="open_Window(\'/wlClient.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>

					<input type="hidden" value="" name="wpsStatus">
	</fieldset>

	<br>

	<fieldset name="wlEncrypt_fieldset" id="wlEncrypt_fieldset">
		<legend><script>dw(Wireless)</script>&ensp;<script>dw(Security)</script></legend>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(Encryption)</script></div></td>
							<td id="all" style="display:block"><div class="itemText2-len" id="Encryption">
							</div></td>
						</tr>
					</table>
					<span id="key" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(EncryptionKey)</script></div></td>
							<td><div class="itemText2-len" id="EncryptionKey">
							</div></td>
						</tr>
					</table>
					</span>
					
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="/rep_wireless_basic.asp" name="submit-url">
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="" name="wireless_block_relay">
					<input type="hidden" value="" name="hiddenSSID">
					<input type="hidden" value="yes" name="isRepeaterBasicUI">
	</fieldset>
				
	<br>

	<table align="center" border="0" Cellspacing="0" width="700">
		<tr>
			<td style="text-align:center; padding-top:30px;">
				<script>

					document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" id="save" onclick="return saveChanges(0)" class="ui-button">');	
					var show_reboot = "<% getInfo("show_reboot"); %>"
					if(show_reboot == "1")
						document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
				</script>
			</td>
		</tr>
	</table>

</form>

<script>
setKeyValue();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
