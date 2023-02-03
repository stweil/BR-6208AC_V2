<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
var useCrossBand = "<%getInfo("useCrossBand");%>";
var wpsConType = <%getiNICVar("wpsConfigType","","");%>;

var methodVal5g = <%getiNICVar("encrypt","","");%>;
var methodVal2g = <%getVar("encrypt","","");%>;

var wpaMode = <%getiNICVar("wpaCipher","","");%>
var enable1x = <%getiNICVar("enable1X","","");%>
var hiddenSSID = <%getiNICVar("hiddenSSID","","");%>;
var wizMode = <%getVar("wizmode","","");%>;

var wpaEncrypt5g =	<%getiNICInfo("wpaEncrypt");%>; 
var wpaEncrypt2g =	<%getInfo("wpaEncrypt");%>; 

var MFenable = <%getVar("macFilterEnabled","","");%>;
var ACPCenable = <%getVar("ACPCEnabled","","");%>
var wlanAcEnable = <%getiNICVar("wlanAcEnabled","","");%>;
var wlsw = <%getiNICVar("wlanDisabled","","");%>;
var wpsenable = <%getiNICVar("wpsEnable","","");%>;

var ssidTbl2g=<%getInfo("ssidAll");%>;
var ssidTbl5g=<%getiNICInfo("ssidAll");%>;


if(useCrossBand == "1"){
	ssidname = ssidTbl2g[0];
	methodVal = methodVal2g;
}
else{
 	ssidname = ssidTbl5g[0];
	methodVal = methodVal5g;
}

ssidname = ssidname.replace(/\s/g, "&nbsp;");

if(wizMode==2){
	if(useCrossBand == "1") wps_key="<%getInfo("rep_wps_key_val");%>";
	else	wps_key="<%getiNICInfo("rep_wps_key_val");%>";
}
else
	wps_key="<%getiNICInfo("wps_key_val");%>";

wps_key = wps_key.replace(/\s/g, "&nbsp;");



if(wizMode==2 && useCrossBand == "1"){
	wpaCipher  = wpaEncrypt2g[0];
	wpa2Cipher = wpaEncrypt2g[1];
}
else{
	wpaCipher  = wpaEncrypt5g[0];
	wpa2Cipher = wpaEncrypt5g[1];
}
var clientmodeTbl = new Array(showText(disable),showText(WEP),showText(WPA),showText(WPA2),showText(WPA_RADIUS),showText(WPA2_RADIUS));
var wpaCipherSuite = new Array("TKIP","AES");
var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));

function WPSswitch(){
	if(document.Wpsseting.wpsEnable.checked==false) {
		document.WpsDevice.startPbc.disabled = true;
		document.WpsDevice.startPin.disabled = true;
		document.WpsDevice.pinCode.disabled = true;
		document.WpsDevice.startPbc.style.color=disableButtonColor;
		document.WpsDevice.startPin.style.color=disableButtonColor;
		document.Wpsseting.wps5g.value="OFF"
	}
	else{
		document.WpsDevice.startPbc.disabled = false;
		document.WpsDevice.startPin.disabled = false;
		document.WpsDevice.pinCode.disabled = false;
		document.WpsDevice.startPbc.style.color=enableButtonColor;
		document.WpsDevice.startPin.style.color=enableButtonColor;
		document.Wpsseting.wps5g.value="ON"
	}
}
function savechange(rebootFlag)
{
	if(document.Wpsseting.wpsEnable.checked==false)
		document.Wpsseting.wps5g.value="OFF"
	else
		document.Wpsseting.wps5g.value="ON"

	if(rebootFlag)
		document.Wpsseting.isApply.value = "ok;"

	document.Wpsseting.submit();
}

function checkPin()
{
	if(document.WpsDevice.configOption.value == "pin")
	{
		if (( document.WpsDevice.pinCode.value.length != 8) &&  ( document.WpsDevice.pinCode.value.length != 4))
		{
			alert("Pin Code Value should be set 4 characters or 8 characters");
			return 0;
		}
	}
	return 1;
}
function saveChanges(mode)
{
	if(wizMode==2)
		wpaMode = parseInt(wpaCipher,10) + parseInt(wpa2Cipher,10);

	if(hiddenSSID == 1){
		alert(showText(broadcastEssidfirst));
		return false;
	}
	if ( wizMode==2 ){
		if(methodVal == 1 || methodVal == 2 || (methodVal == 3 && wpaMode == 1) || (methodVal == 3 && wpaMode == 3) || (enable1x == 1)){
			alert(showText(WPS20supportaes));
			return false;
		}
	}
	else{
		if(methodVal == 1 || (methodVal == 2 && wpaMode == 1) || (methodVal == 2 && wpaMode == 3) || (enable1x == 1)){
			alert(showText(WPS20supportaes));
			return false;
		}
	}


	with(document.WpsDevice) 
  {
		configMode=confMode.selectedIndex;
		if(configMode!=1)
		{
			if(mode == 1)   //PIN
			{
				if( checkPin() ==0)
				{
					pinCode.focus();
					return false;
				}
				if( checkValue(pinCode.value) == 0)
				{
					pinCode.focus();
					return false
				}
	
				configOption.value="pin";
				PinCode.value=pinCode.value;
			}
			else if(mode == 2)   //PBC
			{
				configOption.value="pbc";
				alert(showText(mustpushbutton));
			}
		}
		else
		{
			alert(showText(configureByexternal));
		}

		startPin.disabled = true;
		startPbc.disabled = true;
		pinCode.disabled = true;
		//document.getElementById('save').disabled = true;
		document.Wpsseting.wpsEnable.disabled = true;
		countDown(mode);
		submit();

		return true;
	}
}

var secs = 120; 
var wait = secs * 1000;

function countDown(mode)
{
	for(i = 1; i <= secs; i++)
	{
		setTimeout("update(" + i + ","+mode+")", i * 1000);
	}
}
function update(num,mode)
{
	with(document.WpsDevice) 
	{
		if(mode == 1)  //PIN
		{
			if (num == (wait/1000))
			{
				startPin.value = showText(StartPIN);
			}
			else
			{
				printnr = (wait / 1000)-num;
				startPin.value =   printnr + " "+showText(sec)+" .";
			}
		}
		else   //PBC
		{
			if (num == (wait/1000))
			{
				startPbc.value = showText(StartPBC);
			}
			else
			{
				printnr = (wait / 1000)-num;
				startPbc.value =  printnr + " "+showText(sec)+" .";
			}
		}
	}
}

function init()
{
	$("#help-box").hide();
	
	if(wlsw != 0 || wlanAcEnable !=0 || MFenable !=0 || ACPCenable !=0){
		document.WpsDevice.startPbc.disabled = true;
		document.WpsDevice.startPin.disabled = true;
		document.Wpsseting.wpsEnable.disabled = true;
		document.WpsDevice.pinCode.disabled = true;
		//document.getElementById('save').disabled = true;
		document.WpsDevice.startPbc.style.color=disableButtonColor;
		document.WpsDevice.startPin.style.color=disableButtonColor;
		//document.getElementById('save').style.color=disableButtonColor;
	}
	else{
		if(wpsenable != 1)
		{
			document.WpsDevice.startPbc.disabled = true;
			document.WpsDevice.startPin.disabled = true;
			document.WpsDevice.pinCode.disabled = true;
			document.WpsDevice.startPbc.style.color=disableButtonColor;
			document.WpsDevice.startPin.style.color=disableButtonColor;
		}
	}

	//set 5g key
	if(wps_key.length > 50)
   wps_key5g=wps_key.substring(0,50)+'<br>'+wps_key.substring(50,wps_key.length);
	else
	 wps_key5g=wps_key;

	document.getElementById('wpskey').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+wps_key5g+'</span></div>';
}
function checkValue(str)
{
	for (var i=0; i<str.length; i++)
	{
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9')) continue;
		alert(showText(Invalidpin));
		return 0;
	}
	return 1;
}
function validateChecksum(pincode)
{
	var accum = 0;
	var temp=""
	accum += 3 * (Math.floor(pincode/ 10000000) % 10);
	accum += 1 * (Math.floor(pincode/ 1000000) % 10);
	accum += 3 * (Math.floor(pincode/ 100000) % 10);
	accum += 1 * (Math.floor(pincode/ 10000) % 10);
	accum += 3 * (Math.floor(pincode/ 1000) % 10);
	accum += 1 * (Math.floor(pincode/ 100) % 10);
	accum += 3 * (Math.floor(pincode/ 10) % 10);
	accum += 1 * (Math.floor(pincode/ 1) % 10);
	return (0 == (accum % 10));
}
</SCRIPT>
</head>
<body onLoad="init();">

<div class="help-information" id="help-box" style="display:none">
	<table class="help-table1">
		<tr>
			<td>
				<div class="help-text" id="help-info1"><script>dw(WPShelpInfor)</script></div>
			</td>
		</tr>
	</table>
</div>
	<script>
	HelpButton();
	</script>

<blockquote>
	<fieldset name="dynamicip_fieldset" id="dynamicip_fieldset">
		<legend><script>if(wizMode==2)document.write('5'+showText(GHz)+'&nbsp;');dw(WPS)</script></legend>
			<br>
				<form action="/goform/formWpsEnable" method="POST" name="Wpsseting">
					<table align="center" border="0" Cellspacing="4" width="700">
						<tr>
							<td colspan="0" style="text-align:left; padding-left:50px;">
								<input type="checkbox" name="wpsEnable"  <%getiNICVar("wpsEnable","1","checked");%> value="ON" onClick="savechange(0)"><span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(WPS)</script></span>
									<script>
										/*if(stype==7) document.write('WPS '+showText(enable))
										else document.write(showText(enable)+' WPS')*/
									 </script>
							</td>
								
						</tr>							
					</table>
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="" name="wps5g">
					<input type="hidden" value="/wireless5g_wps.asp" name="wlan-url">
					<input type="hidden" value="" name="isApply">
				</form>

				<table align="center" border="0" Cellspacing="4" width="700">
					<tr>
						<td><div class="status-itemText-len"><b><script>dw(WPSInfor)</script>&ensp;:</b></div></td>
						<td><div class="status-status-itemText2-len">
						</div></td>
					</tr>
	
					<tr>
						<td><div class="status-itemText-len"><script>dw(wspStatus)</script></div></td>
						<td><div class="status-itemText2-len">
							<span class="choose-itemText ">
								<script>if (!wpsConType) document.write(showText(Unconfigured));else document.write(showText(Configured))</script>
							</span>
						</div></td>
					</tr>
					
					<tr>
						<td><div class="status-itemText-len"><script>dw(SelfPinCode)</script></div></td>
						<td><div class="status-itemText2-len"><span class="choose-itemText "><%getiNICInfo("pinNumber");%></span></div></td>
					</tr>

					<tr>
						<td><div class="status-itemText-len"><script>dw(SSID)</script></div></td>
						<td><div class="status-itemText2-len" id="mainssid"></div></td>	
							<script>document.getElementById('mainssid').innerHTML = '<span class="choose-itemText">'+ssidname+'</span>';</script>
					</tr>

					<tr>
						<td><div class="status-itemText-len"><script>dw(AutMode)</script></div></td>
						<td><div class="status-itemText2-len">
							<span class="choose-itemText ">
								<script>
										if ( wizMode==2 ){
		                  wpaMode = parseInt(wpaCipher,10) + parseInt(wpa2Cipher,10);
										  for ( i=0; i<6; i++){  //Range Extender 0.Disable 1.WEP 2.WPA 3.WPA2
												if ( i == methodVal){
													if ( i==2 || i==3 )
													  document.write(clientmodeTbl[i].concat("(",wpaCipherSuite[wpaMode-1],")"));
		                      else
		                        document.write(clientmodeTbl[i]);
												}
									  	}	
										}
										else{						
											document.write(modeTbl[methodVal]);
										}
								</script>
							</span>
						</div></td>
					</tr>

					<tr>
						<td><div class="status-itemText-len"><script>dw(AuthKey)</script></div></td>
						<td><div class="status-itemText2-len" id="wpskey"></div></td>
					</tr>

					<tr>
						<td colspan="2" >
							&nbsp;
						</td>
					</tr>
				</table>

				<form action="/goform/formWpsStart" method="POST" name="WpsDevice">
					<input type="hidden" value="/wireless5g_wps.asp" name="wlan-url">
					<input type="hidden" value="0" name="confMode">
					<input type="hidden" value="1" name="band_select">
					<input type="hidden" value="" name="PinCode">
						<table  align="center" border="0" Cellspacing="4" width="700">
							<tr>
								<td><div class="status-itemText-len"><b><span class="itemText"><script>dw(DeviceConfigure)</script>&ensp;:</b></div></td>
								<td><div class="status-itemText2-len"></div></td>
							</tr>
				
							<tr>
								<td><div class="status-itemText-len"><script>dw(ConfigMode)</script></div></td>
								<td><div class="status-itemText2-len">
									<span class="choose-itemText ">
										<script>
											if(wizMode==3)
												dw(Enrollee)
											else
												dw(Registrar)
										</script>
									</span>

								</div></td>
							</tr>
							
							<tr>
								<td ><div class="status-itemText-len"><script>dw(ConfigurePushButton)</script></div></td>
								<td><div class="status-itemText2-len">
									<input type="hidden" value="pin" name="configOption">
										<script>document.write('<input type="button" name="startPbc" value="'+showText(StartPBC)+'" onclick="return saveChanges(2)"   class="ui-button-nolimit">');</script>
								</div></td>
							</tr>
							
							<tr>
								<td><div class="status-itemText-len"><script>dw(ConfigureClientPinCode)</script></div></td>
								<td><div class="status-itemText2-len">
									<input type="text" name="pinCode" size="15" maxlength="8">
										<script>document.write('<input type="button" name="startPin" value="'+showText(StartPIN)+'" onclick="return saveChanges(1);"  class="ui-button-nolimit">');</script>
								</div></td>
							</tr>
					 </table>
				</form>

				<table align="center" border="0" cellpadding="4" width="700">
					<tr>
						<td style="text-align:center; padding-top:15px;">
							<script>
								//document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" id="save" onclick="savechange(0)" class="ui-button">');
								
								var show_reboot = "<% getInfo("show_reboot"); %>"
								if(show_reboot == "1"){
									document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="savechange(1)" > '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
								}
								else{
									document.write('<tr><td colspan="2" >&nbsp;</td></tr>');
								}
							</script>
						</td>
					</tr>
				</table>
			</fieldset>

<script>
	$("#help-box").hide();
</script>
</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
