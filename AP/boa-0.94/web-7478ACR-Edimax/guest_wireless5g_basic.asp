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

var defaultChan = <%getiNICInfo("channel","","");%>;
var ssidTbl=<%getiNICInfo("ssidAll");%>;
var ssidenableTbl=<%getiNICInfo("ssidMirrorAll");%>;
var apIsoselTbl=<%getiNICInfo("noForwardAll");%>;
var secModeTbl = <%getiNICInfo("secModeAll");%>;
var enable1XTbl = <%getiNICInfo("enable1XAll");%>;
var wepTbl = <%getiNICInfo("wepAll");%>;
var wepValueAll = <% getiNICInfo("wepValueAll");%>;
var keyTypeTbl =<%getiNICInfo("keyTypeAll");%>;
var wpaCipherTbl =<%getiNICInfo("wpaCipherAll");%>;
var pskFormatTbl =<%getiNICInfo("pskFormatAll");%>;
var pskValueTbl =	<%getiNICInfo("pskValueAll");%>;
var defaultKeyIdTbl=<%getiNICInfo("defaultKeyIdAll");%>;
var wpaCipherguest = wpaCipherTbl[1];
var wl_ssid = ssidTbl[1];
var apMode = <%getiNICVar("apMode","","");%>;
var methodVal = secModeTbl[1];

var opMode = <%getiNICVar("opMode","","");%>;
var wlsw = <%getiNICVar("wlanDisabled","","");%>;
var hidSSID = <%getiNICVar("hiddenSSID1","","");%>;

function displayObj(chose, index)
{
	if (chose == 1){  //basic setting
		displayObj(2, 1);
	}
	else //Encryption setting
	{
		document.wlanSetup.method.value=document.wlanSetup.method1.value;
		var wepTbl = new Array("none","block","none","none");
		var wpaTbl = new Array("none","none","block","block");
		var pskTbl = new Array("none","none","block","none");

		for (i=0; i<=4; i++) {
			if (document.wlanSetup.method.value == i) {
				document.getElementById('wepId').style.display = wepTbl[i];
				document.getElementById('wpaId').style.display = wpaTbl[i];
				document.getElementById('pskId').style.display = pskTbl[i];
			}
		}
		updateFormat();
	}
}

function updateFormat()
{
	with(document.wlanSetup) {
		if (length.selectedIndex == 0) {
			format.options[0].text = showText(ascii_5cha);
			format.options[1].text = showText(Hex_10cha);
		}
		else if (length.selectedIndex == 1){
			format.options[0].text = showText(ascii_13cha);
			format.options[1].text = showText(hex_26cha);
		}

		var type = keyTypeTbl[1];
  	format.options[type].selected=true;
	}
	
	setDefaultKeyValue();
}

function setDefaultKeyValue()
{
	var WEP64Key1 = wepValueAll[8];
	var WEP128Key1 = wepValueAll[12];

	with(document.wlanSetup) {
		if (length.selectedIndex == 0) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 5;
				key1_1.maxLength = 5;  //text
				key1.value = "*****";  //password
				key1_1.value =WEP64Key1.substring(0,5);
			}
			else {
				key1.maxLength = 10;
				key1_1.maxLength = 10;
				key1.value = "**********";
				key1_1.value = WEP64Key1;
			}
		}
		else if (length.selectedIndex == 1) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 13;
				key1_1.maxLength = 13;
				key1.value = "*************";
				key1_1.value = WEP128Key1.substring(0,13);
			}
			else {
				key1.maxLength = 26;
				key1_1.maxLength = 26;
				key1.value = "**************************";
				key1_1.value = WEP128Key1;
			}
		}
	}
}
function hideWepvalue(x)
{
	if (x == 1)
	{
		document.getElementById('keyhidewep').checked=true;
		document.getElementById('wephide').style.display="block";
		document.getElementById('wepdisplay').style.display="none";
	}	
	else
	{
		document.getElementById('keyhidewep1').checked=false;
		document.getElementById('wephide').style.display="none";
		document.getElementById('wepdisplay').style.display="block";
	}
}
function hidePskvalue(x)
{
	if (x == 1)
	{
		document.getElementById('keyhide').checked=true;
		document.getElementById('hidepsk').style.display="block";
		document.getElementById('displaypsk').style.display="none";
	}	
	else
	{
		document.getElementById('keyhide1').checked=false;
		document.getElementById('hidepsk').style.display="none";
		document.getElementById('displaypsk').style.display="block";
}
}
function setpskvalue(x)
{
	with(document.wlanSetup) {
		if (x == 1)
			pskValue.value=pskValue_1.value;
		else
			pskValue_1.value=pskValue.value;
	}
}
function setWepvalue(x)
{
	with(document.wlanSetup) {
		if (x == 1)
			key1.value=key1_1.value;
		else
			key1_1.value=key1.value;
	}
}
function init()
{
	$("#help-box").hide();

	with(document.wlanSetup) {

		ssid.value = ssidTbl[1];

		if(ssidenableTbl[0] != 0)
			wlanswEnabled.checked=true;
		else
			wlanswEnabled.checked=false;

		if(apIsoselTbl[1] == 1)
			IsolationEnabled.checked=true;
		else
			IsolationEnabled.checked=false;

		if(hidSSID == 1)
			hiddenSsid.checked=true;
		else
			hiddenSsid.checked=false;
	
		if(wlsw!=0)
		{ 
			disableall(true);
			save.disabled = true;
			wlanswEnabled.disabled = true;
			save.style.color=disableButtonColor;
		}
		else
		{
			wlanswEnabled.disabled = false;
			if(ssidenableTbl[0] != 0) disableall(false);
			else disableall(true);
		}
	}
}

function disableall(flagvalue)
{
	with(document.wlanSetup) {
		ssid.disabled = flagvalue;
		IsolationEnabled.disabled = flagvalue;
		hiddenSsid.disabled = flagvalue;
		method1.disabled = flagvalue;
		length.disabled = flagvalue;
		format.disabled = flagvalue;
		key1.disabled = flagvalue;
		key1_1.disabled = flagvalue;

		pskFormat.disabled = flagvalue;
		pskValue.disabled = flagvalue;
		pskValue_1.disabled = flagvalue;
		document.getElementById('keyhide').disabled = flagvalue;
		document.getElementById('keyhide1').disabled = flagvalue;

		
	}
}

function open_Window(url)
{
	openWindow(url, 'showWirelessClient' );
}

function wlValidateKey(idx, str, len)
{
	if (str.length ==0) return 1;
	if ( str.length != len) {
		idx++;
		alert(showText(InvalidSomething).replace(/#####/,showText(keyLen))+' '+showText(shouldbeSomething).replace(/####/,""+len+" "+showText(characters)));
		return 0;
	}
	if ( str == "*****" ||
		str == "**********" ||
		str == "*************" ||
		str == "****************" ||
		str == "**************************" ||
		str == "********************************" )
		return 1;
	if (document.wlanSetup.format.selectedIndex==0)
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

function saveChanges(rebootFlag)
{
	
	with(document.wlanSetup) {
		if (wlanswEnabled.checked == true){
			enableMSSID.value="1";
			if (ssid.value=="") {
				alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(essid)));
				ssid.value = ssid.defaultValue;
				ssid.focus();
				return false;
			}
		
			var keyLen;
			var strMethod = method.value;
			var str = pskValue.value;
			if ( strMethod==1)
			{
				if (length.selectedIndex == 0)
				{
					if ( format.selectedIndex == 0) keyLen = 5;
					else keyLen = 10;
				}
				else if (length.selectedIndex == 1)
				{
					if ( format.selectedIndex == 0) keyLen = 13;
					else keyLen = 26;
				}
				else
				{
					if ( format.selectedIndex == 0) keyLen = 16;
					else keyLen = 32;
				}
				if (wlValidateKey(0,key1.value, keyLen)==0)
				{
					//key1.focus();
					return false;
				}
			}

			//********** psk **********
			if (strMethod==2)
			{
				if (pskFormat.selectedIndex==1) {
					if (str.length != 64) {
						alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
						return false;
					}
					takedef = 0;
					if (pskFormat.value == 1 && pskValue.value.length == 64) {
						for (var i=0; i<64; i++) {
								if ( str.charAt(i) != '*')
								break;
						}
						if (i == 64 )
							takedef = 1;
					}
					if (takedef == 0) {
						for (var i=0; i<str.length; i++) {
								if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
								(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
								(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*')
								continue;
							alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
							//pskValue.focus();
							return false;
						}
					}
				}
				else {
					if (str.length < 8) {
						alert(showText(pskshouldbe8));
						return false;
					}
					if (str.length > 63) {
						alert(showText(pskshouldbe64));
						return false;
					}
				}
			}

		}
		else{
			enableMSSID.value="0";
		}

		if(rebootFlag)  isApply.value = "ok";

		submit();
	}
}


function setwpamode(who,mode)
{
	with(document.wlanSetup) 
 	{
		if(who == 1)
		{
			wpaCipher1[0].checked=false;
			wpaCipher1[1].checked=false;
			wpaCipher1[2].checked=false;
			wpaCipher1[mode-1].checked=true;
		}
		if(who == 2)
		{
			wpaCipher2[0].checked=false;
			wpaCipher2[1].checked=false;
			wpaCipher2[mode-1].checked=true;
		}
		if(who == 3)
		{
			wpaCipher3.checked=true;
		}
		wpaCipher.value = mode;
	}
}

function resetform()
{
	with(document.wlanSetup) 
 {
		reset();
		displayObj(1, 0);
	}
}

function WlSwitch()
{
	with(document.EnableSwitch) 
  {
		if (document.wlanSetup.wlanswEnabled.checked==true)
		 wlDisabled.value="no";
		else
		 wlDisabled.value="yes";
		submit();
	}
}

function IsolationSwitch()
{
	with(document.wlanSetup) 
	{
		if (document.wlanSetup.IsolationEnabled.checked==true)
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
					<tr><td><div class="help-text" id="help-info1"><script>dw(WlguestHelpInfo)</script>&ensp;<script>dw(WlEncryptionHelpInfo)</script>
				 </div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>

<!-- wlanSetup -->


<blockquote>

	<form action="/goform/formiNICEnableSwitch" method="POST" name="EnableSwitch">
		<input type="hidden" value="1" name="mssidSelect">
		<input type="hidden" value="" name="wlDisabled">
		<input type="hidden" value="/guest_wireless5g_basic.asp" name="submit-url">
	</form>

<!--<form action="/goform/form5GguestWlbasic" method="POST" name="wlanSetup">-->
	<form action="/goform/formiNICbasic" method="POST" name="wlanSetup">
	<input type="hidden" value="1" name="mssidSelect">
	<input type="hidden" value="" name="wireless_block_relay">
	<input type="hidden" value="" name="hiddenSSID">
	<input type="hidden" value="" name="enableMSSID">
	<fieldset name="wlanSetup_fieldset" id="wlanSetup_fieldset">
		<legend><script>dw(basicsetting)</script></legend>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td colspan="2"><input type="checkbox" name="wlanswEnabled" value="no" onclick="WlSwitch()">&nbsp;<span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(Guest)</script>&ensp;<script>dw(SSID)</script></span></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Guestname)</script></div></td>  
							<td><div class="itemText2-len">
								<script>	
									document.write('<input type="text" name="ssid" size="25" maxlength="32" value="">');
								</script>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="hiddenSsid" onclick="hiddenSsidSwitch()">&nbsp;
								<span class="itemText"><script>dw(Hide)</script>&nbsp;<script>dw(essid)</script></span>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="IsolationEnabled" value="no" onclick="IsolationSwitch()">&nbsp;
								<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(wlClients)</script>&nbsp;<script>dw(Isolation)</script></span>
							</div></td>
						</tr>

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
					</table>


					<span id="chanId" style="display:block">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(ChannelNumber)</script></div></td>
							<td><div class="itemText2-len">
								<span class="choose-itemText ">
								<script>
									if( defaultChan == 0)
										document.write(showText(Auto));
									else
										document.write(defaultChan);
			
									//document.wlanSetup.chan.value=defaultChan;
								</script>
								</span>
								&nbsp;<span class="itemText">(<script>dw(sameasmainssid)</script>)</span>
							</div></td>
						</tr>
					</table>
					</span>

	</fieldset>

	<br>


<!--  wlEncrypt  -->

	<fieldset name="wlEncrypt_fieldset" id="wlEncrypt_fieldset">
		<legend><script>dw(Wireless)</script>&ensp;<script>dw(Security)</script></legend>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(Encryption)</script></div></td>
							
							<script>
								var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key));
							</script>				
							<input type="hidden" name="method" value="">
			
							<td><div class="itemText2-len">
								<select size="1" name="method1" onChange="displayObj(2, 1);">
									<script>
										for ( i=0; i<3; i++){
											if ( i == methodVal)
												document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
											else{
												if(i != 1)
													document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
											}
										}
									</script>
								</select>
							</div></td>
	
						</tr>
					</table>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="wepId">
						<tr>
							<td><div class="itemText-len"><script>dw(keyLen)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="length" ONCHANGE=updateFormat()>
									<script>
										var wepkeyLen = wepTbl[1];
										if(wepkeyLen != 2)
										{
											document.write('<option value=1 selected>'+showText(bit64)+'</option>')
											document.write('<option value=2>'+showText(bit128)+'</option>')
										}
										else if(wepkeyLen == 2)
										{
											document.write('<option value=1>'+showText(bit64)+'</option>')
											document.write('<option value=2 selected>'+showText(bit128)+'</option>')
										}
									</script>
								</select>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(KeyFormat)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="format" ONCHANGE=setDefaultKeyValue()>
									<option value=1><script>dw(ascii)</script></option>
									<option value=2><script>dw(Hex)</script></option>
								</select>
							</div></td>
						</tr>
						<input type="hidden" value="1" name="defaultTxKeyId">
						<tr>
							<td><div class="itemText-len"><script>dw(EncryptionKey)</script></div></td>
							<td><div class="itemText2-len">
								<div id="wephide" style="display:block">
									<input type="password" name="key1" size="32" maxlength="32" onchange="setWepvalue(0)">
									<input type="checkbox" name="hidewep" value="ON" onclick="hideWepvalue(0);" checked="true" id="keyhidewep"><span class="choose-itemText"><script>dw(Hide)</script></span>
								</div>
								<div id="wepdisplay" style="display:none">
									<input type="text" name="key1_1" size="32" maxlength="32" onchange="setWepvalue(1)">
									<input type="checkbox" name="hidewep" value="OFF" onclick="hideWepvalue(1);" id="keyhidewep1"><span class="choose-itemText"><script>dw(Hide)</script></span>
								</div>
							</div></td>
						</tr>
						<input type="hidden" name="wepEnabled" value="ON">
					</table>
	
					
					<input type="hidden" name="wpaCipher" value="2">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="wpaId" style="display:none">
						<tr>
							<td><div class="itemText-len"><script>dw(WPA_Unicast_Cipher_Suite)</script></div></td>

							<td><div class="itemText2-len"><span class="choose-itemText">
								<script>
									document.write(showText(WPA2_AES));
								</script>
								</span></div>
							</td>

						</tr>
					</table>
					
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="pskId">
						<tr>
							<td><div class="itemText-len"><script>dw(Pre_shared_Key_Format)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="pskFormat">
									<script>
										var pskmod = pskFormatTbl[1];
										if(pskmod=="0"){
											document.write('<option value=0 selected>'+showText(Passphrase)+'</option>')
											document.write('<option value=1>'+showText(hex_64cha)+'</option>')
										}
										else{
											document.write('<option value=0>'+showText(Passphrase)+'</option>')
											document.write('<option value=1 selected>'+showText(hex_64cha)+'</option>')
										}
									</script>
								</select>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Pre_shared_Key)</script></div></td>
							<td><div class="itemText2-len">
									<div id="hidepsk" style="display:block">
										<script>
											var pskval = pskValueTbl[1];
											if (pskval.search('"') != -1)  
													pskval = pskval.replace(/\"/g, "&quot;");
											document.write('<input type="password" name="pskValue" size="32" maxlength="64" value="'+pskval+'" onchange="setpskvalue(0)">')
										</script>
										<input type="checkbox" name="hidePsk" value="ON" onclick="hidePskvalue(0);" checked="true" id="keyhide"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
									<div id="displaypsk" style="display:none">
										<script>document.write('<input type="text" name="pskValue_1" size="32" maxlength="64" value="'+pskval+'" onchange="setpskvalue(1)">')</script><input type="checkbox" name="hidePsk" value="OFF" onclick="hidePskvalue(1);" id="keyhide1"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
							</div></td>
						</tr>
					</table>

					<input type="hidden" value="/guest_wireless5g_basic.asp" name="submit-url">
					<input type=hidden value="" name="isApply">

	</fieldset>
				
	<br>

	<table align="center" border="0" Cellspacing="0" width="700">
		<tr>
			<td style="text-align:center; padding-top:30px;">
				<script>

					document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
					var show_reboot = "<% getInfo("show_reboot"); %>"
					if(show_reboot == "1")
						document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)" > '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
				</script>
			</td>
		</tr>
	</table>
<input type="hidden" value="1" name="iqsetupclose">
</form>

<script>
	$("#help-box").hide();
	displayObj(1, 0);
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>

