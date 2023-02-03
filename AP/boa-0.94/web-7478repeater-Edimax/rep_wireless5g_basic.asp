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
var ssidTbl5g = <%getiNICInfo("Assidtbl");%>;
var ssidTbl2g = <%getInfo("Gssidtbl");%>;
var apMode = <%getiNICVar("apMode","","");%>;
var methodVal5g = <%getiNICVar("encrypt","","");%>;
var methodVal2g = <%getVar("encrypt","","");%>;
var opMode = <%getVar("opMode","","");%>;
var wpsConfigType = <%getiNICVar("wpsConfigType","","");%>;
var wlsw = <%getiNICVar("wlanDisabled","","");%>;
var wlsw2g = <%getVar("wlanDisabled","","");%>;

var settingChannel=0
if ((useCrossBand=="1") && (wlsw2g ^ wlsw)) settingChannel=1;

var wlswoff = 0;
var wisp = <%getiNICVar("opMode","","");%>;
var pskValueTbl5g =	<%getiNICInfo("pskValueAll");%>;
var pskValueTbl2g = <%getInfo("pskValueAll");%>;
var apIso= <%getiNICVar("noForward","","");%>;
var hidSSID = <%getiNICVar("hiddenSSID","","");%>;
var EncryptionTbl = new Array(showText(OpenSecurity),showText(WEP),showText(WPA),showText(WPA2));
var wpaCipherSuite = new Array("TKIP","AES");
var wepkeyLen2g = <%getVar("wep","","");%>;
var type2g = <%getVar("keyType","","");%>;
var wepkeyLen5g = <%getiNICVar("wep","","");%>;
var type5g = <%getiNICVar("keyType","","");%>;


wepValueAll5g = <% getiNICInfo("wepValueAll");%>;
wepValueAll2g = <% getInfo("wepValueAll");%>;

var wpaEncrypt5g =	<%getiNICInfo("wpaEncrypt");%>; 
var wpaEncrypt2g =	<%getInfo("wpaEncrypt");%>; 

var cur_Chan5g="<%getiNICInfo("autochannel");%>"; 

if(wizMode==2 && useCrossBand == "1"){
	wpaCipher =	wpaEncrypt2g[0];
	wpa2Cipher = wpaEncrypt2g[1];
}
else{
	wpaCipher =	wpaEncrypt5g[0];
	wpa2Cipher = wpaEncrypt5g[1];
}
wpaMode = parseInt(wpaCipher,10) + parseInt(wpa2Cipher,10);

function setKeyValue()
{
	if(useCrossBand == "1"){
		pskval = pskValueTbl2g[0];
		methodVal = methodVal2g;
		WEP64Key1 = wepValueAll2g[0];
		WEP128Key1 = wepValueAll2g[4];
		wepkeyLen = wepkeyLen2g;
		type = type2g;
	}
	else{
		pskval = pskValueTbl5g[0];
		methodVal = methodVal5g;
		WEP64Key1 = wepValueAll5g[0];
		WEP128Key1 = wepValueAll5g[4];
		wepkeyLen = wepkeyLen5g;
		type = type5g;
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

		if(useCrossBand == "1") ssid.value = ssidTbl2g[0];
		else ssid.value = ssidTbl5g[0];

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



<form action="/goform/formiNICbasic" method="POST" name="wlanSetup">	
		<input type="hidden" name="repUseCrossBand" value="">
		<input type="hidden" name="wlanDisabled" value="">
	<fieldset name="wlanSetup_fieldset" id="wlanSetup_fieldset">
		<legend><script>dw(basicsetting)</script></legend>
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<!--<tr>
							<td colspan="2"><input type="checkbox" name="WLDisabled" value="no" <%getiNICVar("wlanDisabled","1","checked");%> onclick="WlSwitch()">&nbsp;<span class="itemText"><script>dw(disable)</script>&ensp;<script>dw(Wireless)</script></span></td>
						</tr>-->

						<tr>
							<td><div class="itemText-len"><script>dw(Band)</script></div></td>
							<input type="hidden" name="band" size="25" maxlength="32" value="4">
							<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
													document.write("5 "+showText(GHz)+" (A+N+AC)");
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
									var defaultChan = <%getiNICVar("channel","","")%>;
									document.write(cur_Chan5g);
								</script>
							</span></div></td>-->
							
							<script>
							var regDomain = <%getiNICVar("regDomain","","");%>;
							var defaultChan = <%getiNICVar("channel","","");%>;
							var Reg_to_Domain = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11");
							var region_A = Reg_to_Domain[regDomain];
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

							if(settingChannel){
								document.write('<td><div class="itemText2-len">');
								document.write('	<select size="1" name="chan" id="chann">');

								if( defaultChan == 0)
									document.write('<option selected value="0">'+showText(Auto)+'</option>');
								else
									document.write('<option value="0">'+showText(Auto)+'</option>');

								for (i=0;i< achnn[region_A].length;i++)
								{
									if ( achnn[region_A][i] == defaultChan)
										document.write('<option selected value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
									else
										document.write('<option value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
								}
									
								document.write('	</select>');
								document.write('</div></td>');
							}	
							else{
								document.write('<td><div class="itemText2-len"><span class="choose-itemText ">');
								document.write(cur_Chan5g);
								document.write('</span></div></td>');
							}
							</script>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(SignalStrength)</script></div></td>
								<td><div class="itemText2-len"><span class="choose-itemText ">
							<script>
									var connectStatus5G = "<% getInfo("connectStatus5G"); %>";
									document.write('<div id="width5" style="float:left; width:0%"></div>');
									if(connectStatus5G=="NotAssociated" || connectStatus5G=="disable"){
										document.write('<div style="float:left;">&nbsp;0%</div>');	
									}
									else{
										document.write('<div style="float:left;">&nbsp;'+connectStatus5G+'%</div>');	
										document.getElementById("width5").style.width = parseInt(connectStatus5G,10)*0.73+'%';
									}
							</script>
							</span></div></td>
						</tr>

						<tr>
							<td><div class="itemText-len"><script>dw(wlClients)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(showClients)+'" name="wlClient" id="wlClient" onClick="open_Window(\'/wlClient5g.asp\')" class="ui-button-nolimit">')</script></div></td>
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
					<input type="hidden" value="/rep_wireless5g_basic.asp" name="submit-url">
					<input type=hidden value="" name="isApply">
					<input type="hidden" value=<%getVar("opMode","","");%> name="wisp">
					<input type="hidden" value="" name="wireless_block_relay">
					<input type="hidden" value="" name="hiddenSSID">
					<input type="hidden" value="yes" name="isRepeaterBasicUI">
	</fieldset>
				
	<br>

	<table align="center" border="0" Cellspacing="0" width="700">
		<tr>
			<td style="text-align:center; padding-top:30px;">
				<script>

					document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" id="save" name="save" onclick="return saveChanges(0)" class="ui-button">');	
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
