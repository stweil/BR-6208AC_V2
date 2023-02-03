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
var wizMode = <%getVar("wizmode","","");%>;
var apMode = <%getiNICVar("apMode","","");%>;
var methodVal = <%getiNICVar("encrypt","","");%>;
var opMode = <%getVar("opMode","","");%>;
var wpsConfigType = <%getiNICVar("wpsConfigType","","");%>;
var wlsw = <%getiNICVar("wlanDisabled","","");%>;
//var wlswoff = 0;
var wpaCipher5G = <%getiNICVar("wpaCipher","","");%>;
var pskValueTbl =<%getiNICInfo("pskValueAll");%>;
var ssidTbl = <%getiNICInfo("Assidtbl");%>;
var apIso= <%getiNICVar("noForward","","");%>; 
var type = <%getiNICVar("keyType","","");%>;
var hidSSID = <%getiNICVar("hiddenSSID","","");%>;

if (wlsw == 1)
{
	methodVal=0;
}
else
{
	if(methodVal == 3)
	{
		methodVal=2;
	}
}

function displayObj(chose, index)
{
	if (chose == 1)  //basic setting
	{	
		var showSsid = new Array("block","block","block","none","none","block","block");
		var showbroadcastSsid = new Array("block","block","none","none","none","block","block");
		var showChan = new Array("block","block","none","block","block","block","block");
		var showClit = new Array("block","none","none","none","none","block","block");
		var showSurvey = new Array("none","block","block","none","none","none","none");
		var showMac = new Array("none","none","none","block","block","block","none");
		var showvlan = new Array("block","none","none","none","none","none","none");
		var showMMac = new Array("none","none","none","none","block","block","none");
		var showSec3 = new Array("none","none","none","block","none","none","none");
		var showSec4 = new Array("none","none","none","none","block","none","none");
		var showSec5 = new Array("none","none","none","none","none","block","none");
		var showRepeater = new Array("none","none","none","none","none","none","block");
		var showClone = new Array("none","none","none","none","none","none","none");

		for (i=0; i<=6; i++)
		{
			if (parseInt(apMode,10) == i)
			{
				//apMode = i;

				document.getElementById('ssidId').style.display = showSsid[i];
				document.getElementById('broadcastEssidId').style.display = showbroadcastSsid[i];
				document.getElementById('chanId').style.display = showChan[i];
				document.getElementById('clitId').style.display = showClit[i];
				document.getElementById('surveyId').style.display = showSurvey[i];
				document.getElementById('macId').style.display = showMac[i];
				//document.getElementById('multiMacId').style.display = showMMac[i];
				document.getElementById('secId3').style.display = showSec3[i];
				document.getElementById('secId4').style.display = showSec4[i];
				document.getElementById('secId5').style.display = showSec5[i];
				document.getElementById('repeaterId').style.display = showRepeater[i];
			}
		}

//		document.getElementById('noRADIUS').style.display = "none";
		document.getElementById('all').style.display = "none";
//		document.getElementById('PSKonly').style.display = "none";

//		var checkFlag = 0;
//		if (apMode==1 || apMode==2 || opMode==1)
//		{
//			checkFlag=checkFlag+1;
//			document.getElementById('noRADIUS').style.display = "block";
//			document.getElementById('all').style.display = "none";
//			document.getElementById('PSKonly').style.display = "none";
//			displayObj(2, 2);
//		}
//		if (document.wlanSetup.band.value == 1)  
//		{
//			checkFlag=checkFlag+1;
//			document.getElementById('noRADIUS').style.display = "none";
//			document.getElementById('PSKonly').style.display = "block";
//			displayObj(2, 3);
//		}
//		if(checkFlag==0)
//		{
			document.getElementById('all').style.display = "block";
			displayObj(2, 1);
//		}
	}
	else //Encryption setting
	{
//		if(index==1)
			document.wlanSetup.method.value=document.wlanSetup.method1.value;
//		else if(index==2)
//			document.wlanSetup.method.value=document.wlanSetup.method2.value;
//		else if(index==3)
//			document.wlanSetup.method.value=document.wlanSetup.method3.value;

		var wepTbl = new Array("none","block","none","none");
		var wpaTbl = new Array("none","none","block","block");
		var pskTbl = new Array("none","none","block","none");

//		if (apMode==1 || apMode==2 || opMode==1)
//			var enRadiusTbl = new Array("none","none","none","none");
//		else
//			var enRadiusTbl = new Array("block","block","none","none");

		var radiusTbl = new Array("none","none","none","block");
		var inRdsTbl = new Array("none","none","block","block","none","none");
		for (i=0; i<=4; i++) 
		{
			//if(typeof(_WIFI_11N_STANDARD_)!='undefined' && bandVal==1 && i==1)
			//	continue;
			if (document.wlanSetup.method.value == i) 
			{
				document.getElementById('wepId').style.display = wepTbl[i];
				document.getElementById('wpaId').style.display = wpaTbl[i];
				document.getElementById('pskId').style.display = pskTbl[i];
				//document.getElementById('inRdsId').style.display = "none";
				//document.getElementById('enRadiusId').style.display = enRadiusTbl[i];
				//if (document.getElementById('enRadiusId').style.display=="block" && document.wlanSetup.enRadius.checked==true) 
				//{
				//	document.getElementById('radiusId').style.display="block";
				//}
				//else 
				//{
				//	document.getElementById('radiusId').style.display = radiusTbl[i];
				//}
			}
		}

		//if bandVal= 1 = N band, AES only, so no need changed
		if (document.wlanSetup.band.value != 1)  
		{
			document.getElementById('normal').style.display = "block";
			document.getElementById('noMixed').style.display = "none";

			if(document.wlanSetup.apMode.value != "0" && document.wlanSetup.apMode.value != "5")
			{
				document.getElementById('normal').style.display = "none";
				document.getElementById('noMixed').style.display = "block";
				setwpamode(2, wpaCipher5G);
			}
		}

		lengthClick();
	}
}

function lengthClick()
{
  updateFormat();
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
  	format.options[type].selected=true;
	}

	
	
	setDefaultKeyValue();
}

function setDefaultKeyValue()
{
	wepValueAll = <% getiNICInfo("wepValueAll");%>;
	var WEP64Key1 = wepValueAll[0];
	var WEP128Key1 = wepValueAll[4];
	
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
//		if(apIso == 1)
//			IsolationEnabled.checked=true;
//		else
//			IsolationEnabled.checked=false;

		if(hidSSID == 1)
			hiddenSsid.checked=true;
		else
			hiddenSsid.checked=false;

		ssid.value = ssidTbl[0];

		if(wlsw)
		{
			ssid.disabled = true;
			document.getElementById('hiddenSsid').disabled = true;
			chan.disabled = true;
			showClients.disabled = true;
			repeaterSSID.disabled = true;
			method1.disabled = true;
			save.disabled = true;
		}
	}
}

function open_Window(url)
{
	window.open( url, 'Wireless', 'width=800, height=600 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
}

function wlValidateKey(idx, str, len)
{
	//if (str.length ==0) return 1;
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

//		if (document.wlanSetup.WLDisabled.checked != true){
//			wlanDisabled.value="no";

			if (apMode.value == "0" || apMode.value == "1" || apMode.value == "2" || apMode.value == "5" ) {
				if (ssid.value=="") {
					alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(essid)));
					ssid.value = ssid.defaultValue;
					ssid.focus();
					return false;
				}
			}
			if (apMode.value == "3") {
				if ( !multimacRule(wlLinkMac1, 1, 1)) return false;
			}
			if (apMode.value == "4" || apMode.value == "5" ) {
				if ( !multimacRule(wlLinkMac1, 1, 1)) return false;
				if ( !multimacRule(wlLinkMac2, 2, 1)) return false;
				if ( !multimacRule(wlLinkMac3, 3, 1)) return false;					
				if ( !multimacRule(wlLinkMac4, 4, 1)) return false;						
			}

			wpsStatus.value=wpsConfigType;

			if(wpsConfigType == 0){
				if((ssid.value != ssidTbl[0]) || (strMethod != 0)){
					wpsStatus.value="1";
				}
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
//			if (strMethod == 3 || ((strMethod==0 || strMethod==1) && enRadius.checked==true))
//			{
//				if ( !portRule(radiusPort, showText(RADIUS_SerPort), 0, "", 1, 65535, 1))
//					return false;
//				if( !ipRule( radiusIP, showText(RADIUS_SerIPaddr), "ip", 1))
//					return false;
//			}
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

//			IsolationSwitch();	
			hiddenSsidSwitch();
//		}
//		else{
//			wlanDisabled.value="yes";
//		}

		if(rebootFlag)  isApply.value = "ok";

		if(document.wlanSetup.wpaCipher.value == 1 && document.wlanSetup.method.value == 2) //repeater mode wpa-tkip
		{
			document.wlanSetup.method.value = 2;
		}
		
		if(document.wlanSetup.wpaCipher.value == 2 && document.wlanSetup.method.value == 2) //repeater mode wpa2-aes
		{
			document.wlanSetup.method.value = 3;
		}
		submit();
	}
}

function wlEncrypt_displayObj(index)
{
	document.getElementById('AESonly').style.display = "none";
	document.getElementById('normal').style.display = "none";
	document.getElementById('noMixed').style.display = "none";

	if(index == 1)
	{
		// if N band, only AES
		document.getElementById('normal').style.display = "none";
		document.getElementById('noMixed').style.display = "none";
		document.getElementById('AESonly').style.display = "block";
		setwpamode(3, wpaCipher5G);
	}
	else
	{
		document.getElementById('AESonly').style.display = "none";
		document.getElementById('normal').style.display = "block";  // let apmode decision
		document.getElementById('noMixed').style.display = "none";	
	}
	
	displayObj(1, 0);
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
					<tr><td><div class="help-text" id="help-info1"> <script>dw(WlBasicHelpInfo)</script>&ensp;<script>dw(WlEncryptionHelpInfo)</script> 				
				 </div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>

<!-- wlanSetup -->


<blockquote>

<form action="/goform/formiNICbasicREP" method="POST" name="wlanSetup">
	<fieldset name="wlanSetup_fieldset" id="wlanSetup_fieldset">
		<legend><script>dw(basicsetting)</script></legend>
					<input type="hidden" value="<%getiNICVar("opMode","","");%>" name="wisp">
					<input type="hidden" name="apMode" value="">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
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

					<span id="ssidId" style="display:none">
					<table  align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(WirelessNetworkName)</script>&ensp;(<script>dw(essid)</script>)</div></td>
							<td><div class="itemText2-len">
										<input type="text" name="ssid" size="25" maxlength="32" value="">
							</div></td>
						</tr>
					</table>
					</span>

					<span id="broadcastEssidId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len">&nbsp;</div></td>
							<td>
								<div class="itemText2-len"><input type="checkbox" name="hiddenSsid" id="hiddenSsid" onclick="hiddenSsidSwitch()">&nbsp;
								<span class="itemText"><script>dw(Hide)</script>&nbsp;<script>dw(essid)</script></span>
							</div></td>
						</tr>
					</table>
					</span>

					<span id="chanId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(ChannelNumber)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="chan">
									<script>

										regDomain = <%getiNICVar("regDomain","","");%>;
										defaultChan = <%getiNICVar("channel","","");%>;
										var Reg_to_Domain = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11");
										var region_A = Reg_to_Domain[regDomain];

										if( defaultChan == 0)
											document.write('<option selected value="0">'+showText(Auto)+'</option>');
										else
											document.write('<option value="0">'+showText(Auto)+'</option>');

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
											if ( achnn[region_A][i] == defaultChan)
												document.write('<option selected value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
											else
												document.write('<option value="'+achnn[region_A][i]+'">'+achnn[region_A][i]+'</option>');
										}
									</script>
								</select>
							</div></td>
						</tr>
					</table>



					</span>

					<span id="clitId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(wlClients)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type=button value="'+showText(showClients)+'" name="showClients" onClick="open_Window(\'/wlClient5g.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>
					</span>

					<span id="surveyId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(wlSiteSurvey)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(selSiteSurvey)+'" name="selSurvey" onClick="open_Window(\'/wlsurvey5g.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>
					</span>

					<span id="repeaterId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(RootAPSSID)</script></div></td>
							<td><div class="itemText2-len"><input type="text" name="repeaterSSID" size="25" maxlength="32" value="<% getiNICInfo("repeaterSSID"); %>"></div></td>
						</tr>
					</table>

					</span>

					<span id="macId" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(macAddr)</script>&ensp;1</div></td>
							<td><div class="itemText2-len"><input type="text" size="15" maxlength="12" value="<% getiNICInfo("wlLinkMac1"); %>" name="wlLinkMac1"></div></td>
						</tr>
					</table>
					</span>



					<span id="secId3" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(SetSecurity)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(SetSecurity)+'" name="SetSecurity3" onClick="open_Window(\'/wlWDS3_key5g.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>
					</span>
					<span id="secId4" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(SetSecurity)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(SetSecurity)+'" name="SetSecurity4" onClick="open_Window(\'/wlWDS4_key5g.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>
					</span>

					<span id="secId5" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(SetSecurity)</script></div></td>
							<td><div class="itemText2-len"><script>document.write('<input type="button" value="'+showText(SetSecurity)+'" name="SetSecurity5" onClick="open_Window(\'/wlWDS5_key5g.asp\')" class="ui-button-nolimit">')</script></div></td>
						</tr>
					</table>
					</span>

					<input type="hidden" value="" name="wpsStatus">
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
			
							<td id="all" style="display:block"><div class="itemText2-len">
								<select size="1" name="method1" onChange="displayObj(2, 1);">
									<script>

										for ( i=0; i<3; i++)
										{
											if ( i == methodVal)
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
								<select size="1" name="length" ONCHANGE=lengthClick()>
									<script>
										var wepkeyLen = <%getiNICVar("wep","","");%>;
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

					<input type="hidden" name="wpaCipher" value="<%getiNICVar("wpaCipher","","");%>;">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="wpaId" style="display:none">
						<tr>
							<td><div class="itemText-len"><script>dw(WPA_Unicast_Cipher_Suite)</script></div></td>

							<td id="normal" style="display:block"><div class="itemText2-len"><span class="choose-itemText">
								<script>

									document.write('<input type="radio" id="wpaCipher1_tkip" name="wpaCipher1" value="1" onclick="setwpamode(1,1);" <%getiNICVar("wpaCipher","1","checked");%>>'+showText(WPA_TKIP)+'&nbsp;&nbsp;');
									document.write('<input type="radio" id="wpaCipher1_aes" name="wpaCipher1" value="2"  onclick="setwpamode(1,2);" <%getiNICVar("wpaCipher","2","checked");%>>'+showText(WPA2_AES)+'&nbsp;&nbsp;');
									document.write('<input type="radio" id="wpaCipher1_mixed" name="wpaCipher1" value="3"  onclick="setwpamode(1,3);" <%getiNICVar("wpaCipher","3","checked");%>>'+showText(WPA2_Mixed));
								</script>
							</span></div></td>

							<td id="noMixed" style="display:none"><div class="itemText2-len"><span class="choose-itemText">
							<script>
								document.write('<input type="radio" id="wpaCipher2_tkip" name="wpaCipher2" value="1"  onclick="setwpamode(2,1);" <%getiNICVar("wpaCipher","1","checked");%>>'+showText(WPA_TKIP)+'&nbsp;&nbsp;');
								document.write('<input type="radio" id="wpaCipher2_aes" name="wpaCipher2" value="2"  onclick="setwpamode(2,2);" <%getiNICVar("wpaCipher","2","checked");%>>'+showText(WPA2_AES)+'&nbsp;&nbsp;');
							</script>
							</span></div></td>

							<td id="AESonly" style="display:none"><div class="itemText2-len"><span class="choose-itemText">
							<script>
								document.write('<input type="radio" id="wpaCipher3_aes" name="wpaCipher3" value="2"  onclick="setwpamode(3,2);" <%getiNICVar("5GwpaCipher","3","checked");%>>'+showText(WPA2_AES)+'&nbsp;&nbsp;');
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
										var pskmod = <%getiNICVar("pskFormat","","");%>;
										if(!pskmod)
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
											var pskval = pskValueTbl[0];
											if (pskval.search('"') != -1)  
											{
													pskval = pskval.replace(/\"/g, "&quot;");
											}
											document.write('<input type="password" name="pskValue" size="32" maxlength="64" value="'+pskval+'" onchange="setpskvalue(0)">')
										</script>
										<input type="checkbox" name="hidePsk" value="ON" onclick="hidePskvalue(0);" checked="true" id="keyhide"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
									<div id="displaypsk" style="display:none">
										<script>document.write('<input type="text" name="pskValue_1" size="32" maxlength="64" value="'+pskval+'" onchange="setpskvalue(1)">')</script><input type="checkbox"  name="hidePsk" value="OFF" onclick="hidePskvalue(1);" id="keyhide1"><span class="choose-itemText"><script>dw(Hide)</script></span>
									</div>
							</div></td>
						</tr>
					</table>

					<!--<span id="enRadiusId" style="display:none">
						<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700"> 
							<tr>
								<td><div class="itemText-len">
									<input type="checkbox" <%getiNICVar("enable1X","1","checked");%> name="enRadius" value="ON" onClick="displayObj(1, 0);">&nbsp;&nbsp;<script>dw(Enable_8021x)</script>
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
									<input type="text" name="radiusIP" size="15" maxlength="15" value="<% getiNICInfo("rsIp"); %>">
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(RADIUS_SerPort)</script></div></td>
								<td><div class="itemText2-len">
									<input type="text" name="radiusPort" size="5" maxlength="5" value="<% getiNICInfo("rsPort"); %>">
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(RADIUS_SerPass)</script></div></td>
								<td><div class="itemText2-len">
									<input type="password" name="radiusPass" size="20" maxlength="64" value="<% getiNICInfo("rsPassword"); %>">
								</div></td>
							</tr>
						</table>
					</span>-->


					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="/rep_wireless5g_basic.asp" name="submit-url">
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="" name="hiddenSSID">
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
	document.wlanSetup.apMode.value=apMode;
	displayObj(1, 0);
	if (opMode==3) document.wlanSetup.chan.disabled=true;
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
