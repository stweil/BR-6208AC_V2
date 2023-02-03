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

var vlantagTbl=<%getInfo("vlanidAll");%>;
var ssidTbl=<%getInfo("ssidAll");%>;
var ssidenableTbl=<%getInfo("ssidMirrorAll");%>;
var apIsoselTbl=<%getInfo("noForwardAll");%>;

var secModeTbl = 	<%getInfo("secModeAll");%>;

var wepkeyLenTbl =		<%getInfo("wepAll");%>;
var keyTypeTbl =	<%getInfo("keyTypeAll");%>;

var wpaCipherTbl =	<%getInfo("wpaCipherAll");%>;
var pskFormatTbl =	<%getInfo("pskFormatAll");%>;
var pskValueTbl =	<%getInfo("pskValueAll");%>;

var enable1XTbl = 	<%getInfo("enable1XAll");%>;
var rsIpTbl=<%getInfo("rsIpall");%>;
var rsPortTbl=<%getInfo("rsPortall");%>;
var rsPasswordTbl=<%getInfo("rsPasswordall");%>;

var apMode = <%getVar("apMode","","");%>;
var bandVal = <%getVar("band","","");%>;
var opMode = <%getVar("opMode","","");%>;
var wlsw = <%getVar("wlanDisabled","","");%>;

var wepValueAll = <% getInfo("wepValueAll");%>;

var mssidindex=0;
var selectIndex=0;
var hiddenSSIDtbl = <%getInfo("hiddenSSIDAll");%>;

var defaultChan = <%getVar("channel","","");%>;

var modeTbl1 = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
var CipherTbl = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));
var macTbl = new Array('<% getInfo("wlmac1"); %>','<% getInfo("wlmac2"); %>','<% getInfo("wlmac3"); %>','<% getInfo("wlmac4"); %>');


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


function displayObj(chose, index)
{
	if (chose == 1)  //basic display
	{	
		document.getElementById('chanId').style.display = "block";
		document.getElementById('all').style.display = "block";
		displayObj(2, 1);
	}
	else //Encryption display
	{
		var wepkeyLen = parseInt(wepkeyLenTbl[mssidindex+1],10);
		var type = parseInt(keyTypeTbl[mssidindex+1],10);
		with(document.wlanSetup) {
			if(wepkeyLen==2)
	  			length.options[1].selected=true;
			else
				length.options[0].selected=true;
			
			format.options[type].selected=true;
		}
		
  
		var wepTbl = new Array("none","block","none","none");
		var wpaTbl = new Array("none","none","block","block");
		var pskTbl = new Array("none","none","block","none");
		var enRadiusTbl = new Array("block","block","none","none");
		var radiusTbl = new Array("none","none","none","block");
		var inRdsTbl = new Array("none","none","block","block","none","none");
		for (i=0; i<=4; i++) 
		{
			if (document.wlanSetup.method.value == i) 
			{
				document.getElementById('wepId').style.display = wepTbl[i];
				document.getElementById('wpaId').style.display = wpaTbl[i];
				document.getElementById('pskId').style.display = pskTbl[i];

				document.getElementById('enRadiusId').style.display = enRadiusTbl[i];
				if (document.getElementById('enRadiusId').style.display=="block" && document.wlanSetup.enRadius.checked==true) 
				{
					document.getElementById('radiusId').style.display="block";
				}
				else 
				{
					document.getElementById('radiusId').style.display = radiusTbl[i];
				}
			}
		}

	
		setwpamode(wpaCipherTbl[mssidindex+1]);
		lengthClick();
	}
}

function lengthClick()
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


	}

	setDefaultKeyValue();
}

function setDefaultKeyValue()
{
	selectIndex = (mssidindex + 1) * 8;
	with(document.wlanSetup) {
		if (length.selectedIndex == 0) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 5;
				key1_1.maxLength = 5;  //text
				key1.value = "*****";  //password
				//key1_1.value =WEP64Key1Tbl[mssidindex+1].substring(0,5);
				
				key1_1.value = wepValueAll[selectIndex].substring(0,5);
			}
			else {
				key1.maxLength = 10;
				key1_1.maxLength = 10;
				key1.value = "**********";
				//key1_1.value = WEP64Key1Tbl[mssidindex+1];
				key1_1.value = wepValueAll[selectIndex];
			}
		}
		else if (length.selectedIndex == 1) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 13;
				key1_1.maxLength = 13;
				key1.value = "*************";
				//key1_1.value = WEP128Key1Tbl[mssidindex+1].substring(0,13);
				key1_1.value = wepValueAll[selectIndex+4].substring(0,13);
			}
			else {
				key1.maxLength = 26;
				key1_1.maxLength = 26;
				key1.value = "**************************";
				//key1_1.value = WEP128Key1Tbl[mssidindex+1];
				key1_1.value = wepValueAll[selectIndex+4];
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
	ssidswitch(0);
}

function saveChanges(rebootFlag)
{
	with(document.wlanSetup) {
	
		if(SSIDSwitch.checked==true)
		{
			if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
			{
				if(parseInt(vlanid.value,10)!=0)
				{
					var error=0;
					var str = vlanid.value;
					for (var x=0; x<str.length; x++)
					{
						if ((str.charAt(x) >= '0' && str.charAt(x) <= '9')) continue;
						error = 1;	
						break;
					}
				
					if ( (parseInt(vlanid.value,10) < 1) || (parseInt(vlanid.value,10) > 4094) )
						error = 1;	

					if(error) {
						alert(showText(InvalidVLANID));					
						return false;
					}
				}
			}
		
			enableMSSID.value="1";
			
			if(IsolationEnabled.checked==true) wireless_block_relay.value="on";
			else wireless_block_relay.value="off";
			
			if(broadcastssid.checked==true) hiddenSSID.value="no";
			else hiddenSSID.value="yes";

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
			//********** radius **********
			if (strMethod == 3 || ((strMethod==0 || strMethod==1) && enRadius.checked==true))
			{
				if ( !portRule(radiusPort, showText(RADIUS_SerPort), 0, "", 1, 65535, 1))
					return false;
				if( !ipRule( radiusIP, showText(RADIUS_SerIPaddr), "ip", 1))
					return false;
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
		else
		{
			enableMSSID.value="0";
		}	

		if(rebootFlag)  isApply.value = "ok";
		submit();
	}
}

function setwpamode(mode)
{
	with(document.wlanSetup) 
	{
		if(mode==0)
		{
			wpaCipher1[1].checked=true;  //default is aes
			wpaCipher.value = 2;
		}
		else
		{
			wpaCipher1[0].checked=false;
			wpaCipher1[1].checked=false;
			wpaCipher1[2].checked=false;
			wpaCipher1[mode-1].checked=true;
			wpaCipher.value = mode;
		}
	}
}

function MSSIDSwitch()
{
	with(document.wlanSetup) 
  {
		if (SSIDSwitch.checked==true)
		 disableall(false);
		else
		 disableall(true);
	}
}

function ssidswitch(index)
{
	
	mssidindex=index;
	
	with(document.wlanSetup) {
		mssidSelect.value = index+1;

		// MAC
		document.getElementById("macaddr").innerHTML="&nbsp( "+showText(macAddr)+" : "+macTbl[mssidindex]+" )";

		// SSID
		ssid.value = ssidTbl[mssidindex+1];
		if(ssidenableTbl[mssidindex] != "0")
			SSIDSwitch.checked=true;
		else
			SSIDSwitch.checked=false;

		//VLAN ID
		if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
			vlanid.value=vlantagTbl[mssidindex+1];
			
		// Isolation
		if(apIsoselTbl[mssidindex+1] == "1")
			IsolationEnabled.checked=true;
		else
			IsolationEnabled.checked=false;

		//hidden SSID
		if(hiddenSSIDtbl[mssidindex+1] == "1")
			broadcastssid.checked=false;
		else
			broadcastssid.checked=true;
		

		//chan.disabled = true;
		//band.disabled = true;

		// Encryption
		method.options.selectedIndex = secModeTbl[mssidindex+1];

		var pskkey = pskValueTbl[mssidindex+1];
		if (pskkey.search('"') != -1)  
			pskkey = pskkey.replace(/\"/g, "&quot;");
		pskValue.value=pskkey;
		pskValue_1.value=pskkey;

		// 802.1x
		if( enable1XTbl[mssidindex+1] == "1") 
			enRadius.checked = true;
		else
			enRadius.checked = false;

		radiusIP.value=rsIpTbl[mssidindex+1];
		if(rsPortTbl[mssidindex+1]=="0") radiusPort.value=1812;
		else radiusPort.value=rsPortTbl[mssidindex+1];
		radiusPass.value=rsPasswordTbl[mssidindex+1];

		displayObj(1, 0);	

		hidePskvalue(1);
		hideWepvalue(1);

		// if MSSID disable
		if(ssidenableTbl[mssidindex] == "0")
			disableall(true);
		else
			disableall(false);

		// if main SSID disable
		if(wlsw)
		{
			disableall(true);
			ssidindex.disabled = true;
			SSIDSwitch.disabled = true;
			save.disabled = true;
			save.style.color=disableButtonColor;
		}
	}
}
function disableall(flagvalue)
{
	with(document.wlanSetup) 
  {
		if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
			vlanid.disabled = flagvalue;
		ssid.disabled = flagvalue;
		
		IsolationEnabled.disabled = flagvalue;
		broadcastssid.disabled = flagvalue;
		method.disabled = flagvalue;
		length.disabled = flagvalue;
		format.disabled = flagvalue;
		key1.disabled = flagvalue;
		key1_1.disabled = flagvalue;
		document.getElementById('keyhidewep').disabled = flagvalue;
		document.getElementById('keyhidewep1').disabled = flagvalue;
		document.getElementById('wpaCipher1_tkip').disabled = flagvalue;
		document.getElementById('wpaCipher1_aes').disabled = flagvalue;
		document.getElementById('wpaCipher1_mixed').disabled = flagvalue;
		pskFormat.disabled = flagvalue;
		pskValue.disabled = flagvalue;
		pskValue_1.disabled = flagvalue;
		document.getElementById('keyhide').disabled = flagvalue;
		document.getElementById('keyhide1').disabled = flagvalue;
		enRadius.disabled = flagvalue;
		radiusIP.disabled = flagvalue;
		radiusPort.disabled = flagvalue;
		radiusPass.disabled = flagvalue;
	}
}
</script>
</head>


<body onLoad="init();">
 
	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td><div class="help-text" id="help-info1"> <script>dw(WlguestHelpInfo)</script>&ensp;<script>dw(WlEncryptionHelpInfo)</script> 				
		 </div></td></tr>
		</table>
	</div>
	<script>
	HelpButton();
	</script>

<!--======================================================= wlanSetup =======================================================-->


<blockquote>

<fieldset name="wlanSetup_Status" id="wlanSetup_Status">
		<legend><script>dw(MultipleSSID)</script>&ensp;<script>dw(Status)</script></legend>
			<br>
			<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
				<tr align="center" class="dnsTableHe">
					<td style="width:45px"><script>dw(NO)</script>.</td>
					<td style="width:85px"><script>dw(Status)</script></td>
					<td style="width:240px"><script>dw(essid)</script></td>
					<script>
					if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
						document.write('<td style="width:80px">'+showText(vlanid)+'</td>');
					</script>
					<td style="width:130px"><script>dw(Encryption)</script></td>	
					<td style="width:140px"><script>dw(macAddr)</script></td>
				</tr>
			</table>
			<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
				<script>
				var str;
				for (i=1 ; i<5 ; i++)
				{
					//document.write('<tr align="center">');
					if(i%2)
						document.write('<tr style="background-color: #F0F0F0;" align="center" class="dnsTableText">');
					else
						document.write('<tr style="background-color: #FFFFFF;" align="center" class="dnsTableText">');

					document.write('<td style="width:45px">'+i+'</td>');

					if(ssidenableTbl[i-1] != "0")
						document.write('<td style="width:85px">'+showText(Enabled)+'</td>');
					else
						document.write('<td style="width:85px">'+showText(disable)+'</td>');

					/*if(ssidenableTbl[i-1] == "1")
						document.write('<td class="listtableText"><input type="checkbox" name="MSSID'+i+'" checked disabled></td>');
					else
						document.write('<td class="listtableText"><input type="checkbox" name="MSSID'+i+'" disabled></td>');*/

					document.write('<td style="width:240px">'+ssidTbl[mssidindex+i]+'</td>');
					if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
						document.write('<td style="width:80px">'+vlantagTbl[mssidindex+i]+'</td>');
					document.write('<td style="width:130px">');
					for (j=0 ; j<6 ; j++)
					{
						if ( j == parseInt(secModeTbl[mssidindex+i],10)) 
						{
							if( j != 2 )
								document.write(modeTbl1[j]);
							else
								document.write(CipherTbl[parseInt(wpaCipherTbl[mssidindex+i],10)-1]);
						}
					}
					j=0;
					document.write('<td style="width:140px">');
					document.write(macTbl[i-1]);
					document.write('</td></tr>');
					//str="MSSID"+i;
					//document.Status.str.disabled = true;
				}
				</script>
			</table>
			<br>
	</fieldset>
<br>

<form action="/goform/formWlbasic" method=POST name="wlanSetup">
	<input type="hidden" value="1" name="iqsetupclose">
	<input type="hidden" value="" name="MSSIDEnbled">
	<input type="hidden" value="" name="wireless_block_relay">
	<input type="hidden" value="" name="hiddenSSID">
	<input type="hidden" value="" name="enableMSSID">
	<fieldset name="wlanSetup_fieldset" id="wlanSetup_fieldset">
		<legend><script>dw(MultipleSSID)</script>&ensp;<script>dw(basicsetting)</script></legend>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">

						

						<tr>
							<td><div class="itemText-len"><script>dw(MultipleSSID)</script></div></td>  
							<td><div class="itemText2-len"><select name="ssidindex" onChange="ssidswitch(document.wlanSetup.ssidindex.selectedIndex);">
								<script>	
									for ( i=1; i<5; i++) {
										document.write('<option value="'+i+'">'+i+'</option>');
									}
								</script>
								</select>
								<span class="itemText" id="macaddr"></span>
							</div></td>
						</tr>
	
						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="SSIDSwitch" onclick="MSSIDSwitch()">&nbsp;
								<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(MultipleSSID)</script></span>
							</div></td>
						</tr>
						
						<tr>
							<td><div class="itemText-len"><script>dw(WirelessNetworkName)</script>&ensp;(<script>dw(essid)</script>)</div></td>  
							<td><div class="itemText2-len">
								<script>	
									document.write('<input type="text" name="ssid" size="25" maxlength="32" value="">');
								</script>
							</div></td>
						</tr>

						

						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="IsolationEnabled">&nbsp;
								<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(wlClients)</script>&nbsp;<script>dw(Isolation)</script></span>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="broadcastssid" value="">&nbsp;
								<span class="itemText"><script>dw(broadcastEssid)</script></span>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len"><script>dw(Band)</script></div></td>
							
							<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												//var showbandVal = bandVal-1;
												//if(wlsw)
													document.write(showText(band4));
												//else
												//	document.write(showText(enable));
											</script>	
										</span></div>
							</td>
						</tr>
					</table>


					<span id="chanId" style="display:none">
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
									</script>
									</span>
								&nbsp;<span class="itemText">(<script>dw(sameasmainssid)</script>)</span>
							</div></td>
						</tr>
						<script>	
						if(typeof(_ENVLAN_) != 'undefined' && apMode==0 )
						{
							document.write('<tr>');
							document.write('<td><div class="itemText-len">'+showText(vlanid)+'</div></td>');
							document.write('<td><div class="itemText2-len">');
							document.write('<input type="text" name="vlanid" size="4" maxlength="4" value="">&nbsp;<span class="itemText">(Untagged:0, Tagged:1~4094)</span>');
							document.write('</div></td></tr>');
						}
						</script>
					</table>
					</span>
	</fieldset>

	<br>


<!--=================================  wlEncrypt  =======================================================-->

	<fieldset name="wlEncrypt_fieldset" id="wlEncrypt_fieldset">
		<legend><script>dw(MultipleSSID)</script>&ensp;<script>dw(Security)</script></legend>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(Encryption)</script></div></td>

							<script>
								var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
							</script>				
							<td id="all" style="display:block"><div class="itemText2-len">
								<select size="1" name="method" onChange="displayObj(2, 1);">
									<script>
										for ( i=0; i<4; i++)
										{
											if ( i == parseInt(secModeTbl[mssidindex+1],10))
											{
												document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
											}
											else
											{
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
								<select size="1" name="length" ONCHANGE="lengthClick()">
									<script>
											document.write('<option value=1>'+showText(bit64)+'</option>')
											document.write('<option value=2>'+showText(bit128)+'</option>')
									</script>
								</select>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(KeyFormat)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="format" ONCHANGE="setDefaultKeyValue()">
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
	
					
					<input type="hidden" name="wpaCipher" value="">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="wpaId" style="display:none">
						<tr>
							<td><div class="itemText-len"><script>dw(WPA_Unicast_Cipher_Suite)</script></div></td>

							<td id="normal" style="display:block"><div class="itemText2-len"><span class="choose-itemText">
								<script>

										document.write('<input type="radio" id="wpaCipher1_tkip" name="wpaCipher1" value="1" onclick="setwpamode(1);">'+showText(WPA_TKIP)+'&nbsp;&nbsp;');
										document.write('<input type="radio" id="wpaCipher1_aes" name="wpaCipher1" value="2"  onclick="setwpamode(2);">'+showText(WPA2_AES)+'&nbsp;&nbsp;');

								
									document.write('<input type="radio" id="wpaCipher1_mixed" name="wpaCipher1" value="3"  onclick="setwpamode(3);">'+showText(WPA2_Mixed));
								</script>
							</span></div></td>

						</tr>
					</table>
					
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="pskId">
						<tr>
							<td><div class="itemText-len"><script>dw(Pre_shared_Key_Format)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="pskFormat">
									<script>
										var pskmod = pskFormatTbl[mssidindex+1];
										if(pskmod=="0")
										{
											document.write('<option value=0 selected>'+showText(Passphrase)+'</option>')
											document.write('<option value=1>'+showText(hex_64cha)+'</option>')
										}
										else
										{
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
											document.write('<input type="password" name="pskValue" size="32" maxlength="64" value="" onchange="setpskvalue(0)">')
										</script>
										<input type="checkbox" name="hidePsk" value="ON" onclick="hidePskvalue(0);" checked="true" id="keyhide"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
									<div id="displaypsk" style="display:none">
										<script>document.write('<input type="text" name="pskValue_1" size="32" maxlength="64" value="" onchange="setpskvalue(1)">')</script><input type="checkbox" name="hidePsk" value="OFF" onclick="hidePskvalue(1);" id="keyhide1"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
							</div></td>
						</tr>
					</table>

					<span id="enRadiusId" style="display:none">
						<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700"> 
							<tr>
								<td><div class="itemText-len">
									<input type="checkbox" name="enRadius" value="ON" onClick="displayObj(1, 0);">&nbsp;&nbsp;<script>dw(Enable_8021x)</script>
								</div></td>	
								<td><div class="itemText2-len">
								</div></td>
							</tr>
						</table>
					</span>

					<span id="radiusId" border="0">
						<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
							<tr>
								<td><div class="itemText-len"><script>dw(RADIUS_SerIPaddr)</script></div></td>
								<td><div class="itemText2-len">
									<input type="text" name="radiusIP" size="15" maxlength="15" value="">
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(RADIUS_SerPort)</script></div></td>
								<td><div class="itemText2-len">
									<input type="text" name="radiusPort" size="5" maxlength="5" value="">
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(RADIUS_SerPass)</script></div></td>
								<td><div class="itemText2-len">
									<input type="password" name="radiusPass" size="20" maxlength="64" value="">
								</div></td>
							</tr>
						</table>
					</span>

				
					<input type="hidden" value="/mssid_wireless_basic.asp" name="submit-url">
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="" name="mssidSelect">
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

