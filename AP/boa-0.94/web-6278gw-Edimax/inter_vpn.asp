<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="/set.css">
<link rel="stylesheet" href="/background.css">
<link rel="stylesheet" href="/iosButton.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/file/autowan.var"></script>
<% language=javascript %>
<SCRIPT>
var openvpnENABLED           = <%getInfo("openvpnENABLED");%>;
var openvpnProtocol      =	<%getInfo("openvpnProtocol");%>;
var openvpnPORT          =	<%getInfo("openvpnPORT");%>;
var openvpnSubnetNetmask =	<%getInfo("openvpnSubnetNetmask");%>;
var openvpnTotalAccountNum   = <%getInfo("openvpnTotalAccountNum");%>;
var openvpnLoginAccountTable = <%getInfo("openvpnLoginAccountTable");%>;
var enable_Edimax_DDNS       = <%getInfo("enable_Edimax_DDNS");%>;
var openvpnTotalUser         =  8;
var check_vpn_setup_submit;  //submit use
var check_vpn_setup_dialogBox; //check use

function displayObj()
{
	with(document.VPNSetup) 
	{	
		VPN_Server_Switch(1);
	}

	with(document.VPNUser) 
	{
		if( openvpnTotalAccountNum[0] >= openvpnTotalUser )
		{
			addAccount.disabled=true;
			addAccount.style.color=disableButtonColor;
		}
		else
		{
			addAccount.disabled=false;
			addAccount.style.color=enableButtonColor;
		}
	}
}

function init()
{
	$("#help-box").hide();
	var tempEntryNum=openvpnTotalAccountNum[0];
	if(openvpnTotalAccountNum==0) {
		tempEntryNum+=1;
	}
	updateHeight=tempEntryNum*25;
	updateHeight=updateHeight+"px";
}

function saveChanges(rebootFlag)
{
	if (rebootFlag == 0)
	{	
		with(document.VPNSetup) 
		{
			if (VPN_Server_Enabled.checked==true)
			{
				//----submit setup----
				document.VPN_Server_Setup.VPN_Enabled.value          = "yes";
				document.VPN_Server_Setup.VPN_InterfaceType.value    = InterfaceType.value;
				document.VPN_Server_Setup.VPN_Subnet.value           = Subnet.value;
				document.VPN_Server_Setup.VPN_Netmask.value          = Netmask.value;				
				document.VPN_Server_Setup.VPN_Protocol.value         = Protocol.value;
				document.VPN_Server_Setup.VPN_Port.value             = ServerPort.value;
				document.VPN_Server_Setup.VPN_AuthOnlyUserName.value = AuthOnlyUserName.value;
				document.VPN_Server_Setup.VPN_AuthMode.value         = AuthMode.value;
				document.VPN_Server_Setup.VPN_RedirectTraffic.value  = RedirectTraffic.value;
				document.VPN_Server_Setup.VPN_PushLAN.value          = PushLAN.value;
				document.VPN_Server_Setup.VPN_ResDNS.value           = ResDNS.value;
				document.VPN_Server_Setup.VPN_AdvSetup.value         = "0";
			}
			else
			{
				//----submit setup----
				document.VPN_Server_Setup.VPN_Enabled.value="no";
			}

			document.VPN_Server_Setup.VPN_Save_Setup.value         = saveVPNsetup.value;
			$.post("/goform/formVPNsetup", $("#submit_VPNdefSetup").serialize());
			check_vpn_setup_submit =setInterval("submitVPNsetup()", 500);
		}

		//----check----
		check_vpn_setup_dialogBox=setInterval("checkVPNsetup()", 2000);
		//-------------
	}
	else if (rebootFlag == 1)
	{	
		document.submit_VPNExportClientConf.VPN_Client_conf.value="1";
		document.submit_VPNExportClientConf.submit();
	}
	else if (rebootFlag == 2)
	{	
		document.submit_VPNExportClientConf.VPN_Client_conf.value="2";
		document.submit_VPNExportClientConf.submit();
	}
}

function submitVPNsetup()
{
	with(document.VPNSetup) 
	{
	$.get("getVPN_Setup_State_Submit.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
		if(data==128) {
			window.clearInterval(check_vpn_setup_submit); //submit ok
			window.location.replace("/inter_vpn.asp");
		}
	});
	}
}

function checkVPNsetup()
{
	$.get("getVPN_Setup_State_DialogBox.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
		//----check----
		if(data==0) {
			closeDialogBox(showText(ProcessingPleaseWait));
			window.clearInterval(check_vpn_setup_dialogBox);
		}
		//-------------
	});
}

function checkSpecialChar(input)
{
	var textName = document.getElementById(input).value;

	for (var charLoc=0; charLoc<textName.length; charLoc++) 
	{
		if ( (textName.charAt(charLoc) >= '0' && textName.charAt(charLoc) <= '9') || 
				 (textName.charAt(charLoc) >= 'a' && textName.charAt(charLoc) <= 'z') || 
				 (textName.charAt(charLoc) >= 'A' && textName.charAt(charLoc) <= 'Z') )
			continue;
		return false;
	}
		return true;
}

function VPN_Server_Switch(input)
{
	with(document.VPNSetup) 
	{
		if(input == 1)
		{
			if (VPN_Server_Enabled.checked==true)
			{
				check_vpn_ClientConf_statStop=setInterval("checkClientConfState()", 2000);
				check_vpn_ServerConf_statStop=setInterval("checkServerConfState()", 2000);
			}
			document.VPNClientFiles.Client1_conf.disabled=true;
			document.VPNClientFiles.Client1_conf.style.color=disableButtonColor;
			document.VPNClientFiles.Client2_conf.disabled=true;
			document.VPNClientFiles.Client2_conf.style.color=disableButtonColor;
		}
		else if(input == 2)
		{
			//----check----
			DialogBox(showText(ProcessingPleaseWait));
			//-------------

			if (VPN_Server_Enabled.checked==true)
			{
				document.VPN_Server_WAN_Check.VPN_Enabled.value           = "yes";
				if(!enable_Edimax_DDNS)
				{
					if (!confirm(showText(vpnErrorMeg4)))
					{
						//----check----
						closeDialogBox(showText(ProcessingPleaseWait));
						//-------------

						document.VPN_Server_WAN_Check.VPN_Enabled.value       = "no";
						VPN_Server_Enabled.checked=false;
						return false;
					}
				}
			}
			else
			{
				document.VPN_Server_WAN_Check.VPN_Enabled.value           = "no";
			}

			$.post("/goform/formVPNenabled", $("#submit_VPNWANCheck").serialize(),function(data){ /* submit and get data */
				/* data value: 0=(enabled check ok or disabled) , 1=(wan port not connected) , 2=(non-public ip) */
				if(data==0)
				{
					saveChanges(0); //Set to take effect
				}
				else 
				{
					//----check----
					closeDialogBox(showText(ProcessingPleaseWait));
					//-------------

					if(data==1)
						alert(showText(vpnErrorMeg1));	
					else if (data==2)
						alert(showText(vpnErrorMeg2));	
					else if (data==3)
						alert(showText(vpnErrorMeg3));
					window.location.replace("/inter_vpn.asp");
				}
			});
		}
	}
}
function checkClientConfState()
{
	with(document.VPNClientFiles) 
	{
	$.get("getVPN_ClientConf_State.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
			var andGateData;

			andGateData=data&1;
			if(andGateData==1)
			{
				Client1_conf.disabled=false;
				Client1_conf.style.color=enableButtonColor;
			}
			else
			{
				Client1_conf.disabled=true;
				Client1_conf.style.color=disableButtonColor;
			}

			andGateData=data&2;
			if(andGateData==2)
			{
				Client2_conf.disabled=false;
				Client2_conf.style.color=enableButtonColor;
			}
			else
			{
				Client2_conf.disabled=true;
				Client2_conf.style.color=disableButtonColor;
			}
	});
	}
}

function checkServerConfState()
{
	with(document.VPNClientFiles) 
	{
	$.get("getVPN_ServerConf_State.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
			if(data==4)
			{
				document.getElementById('ServerDDNS_1').style.display          = "none";
				document.getElementById('ServerSubnetNetmask_1').style.display = "none";
				document.getElementById('ServerProtocol_1').style.display      = "none";
				document.getElementById('ServerPORT_1').style.display          = "none";
				document.getElementById('ServerDDNS_2').style.display          = "block";
				document.getElementById('ServerSubnetNetmask_2').style.display = "block";
				document.getElementById('ServerProtocol_2').style.display      = "block";
				document.getElementById('ServerPORT_2').style.display          = "block";
			}
			else 
			{
				document.getElementById('ServerDDNS_1').style.display          = "block";
				document.getElementById('ServerSubnetNetmask_1').style.display = "block";
				document.getElementById('ServerProtocol_1').style.display      = "block";
				document.getElementById('ServerPORT_1').style.display          = "block";
				document.getElementById('ServerDDNS_2').style.display          = "none";
				document.getElementById('ServerSubnetNetmask_2').style.display = "none";
				document.getElementById('ServerProtocol_2').style.display      = "none";
				document.getElementById('ServerPORT_2').style.display          = "none";
			}
	});
	}
}

function checkAccountClick(input1,input2)
{
	if( input1==0 || input1==1 )
	{
		if(input1==0)
		{
			inputUsername='username';
			inputPassword='password';
			if (document.getElementById(inputUsername).value=="")
			{
				alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(UserName)));
				return false;
			}
			if (document.getElementById(inputPassword).value=="")
			{
				alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(Password)));
				return false;
			}

			var accountNum = openvpnTotalAccountNum[0];
			for(i=1;i<=accountNum;i++)
			{
				if (document.getElementById(inputUsername).value == document.getElementById('UserName' + i ).value)
				{
					alert(showText(canNotSame).replace(/#####/,showText(UserName)));
					return false;
				}
			}
			document.VPNUser.addAccount.value  = input2;
		}
		else if(input1==1)
		{
			document.VPNUser.deleteSelAccount.value  = input2;
		}

		//----check----
		DialogBox(showText(ProcessingPleaseWait));
		//-------------

		$.post("/goform/formVPNuser", $("#submit_VPNUser").serialize(),function(data){ /* submit and get data */
			if(data=="submit_ok")
			{
				//----check----
				closeDialogBox(showText(ProcessingPleaseWait));
				//-------------
				window.location.replace("/inter_vpn.asp");
			}
		});	
	}
	else if(input1==2)
	{
		if (!confirm(showText(vpnErrorMeg5)))
		{
					return false;
		}
		window.location.replace("/admin_password.asp");
	}
}
function getWeb_Help(input)
{
	var LanguageArray=
	[['English', 'EN'],
	['Deutsch', 'DE'],
	['Spain', 'ES'],
	['France', 'FR'],
	['Italy', 'IT'],
	['Netherlands', 'NL'],
	['Portugal', 'PT'],
	['Czech', 'CZ'],
	['Poland', 'PL'],
	['Romania', 'RO'],
	['Russian', 'RU'],
	['Slovak', 'SK'],
	['Ukraine', 'UA'],
	['Taiwan', 'TW'],
	['China', 'CN]']];
	
	var Edimax_PDF_Files="http://www.edimax.com/edimax/mw/cufiles/files/Web/"
	if(input == 1)      window.open(Edimax_PDF_Files+"Windows_VPN_"+LanguageArray[stype][1]+".pdf"); //Windows OS
	else if(input == 2) window.open(Edimax_PDF_Files+"MAC_OS_VPN_"+LanguageArray[stype][1]+".pdf");  //MAC OS
	else if(input == 3) window.open(Edimax_PDF_Files+"iOS_VPN_"+LanguageArray[stype][1]+".pdf");     //iOS (iPhone, iPad)
	else if(input == 4) window.open(Edimax_PDF_Files+"Android_VPN_"+LanguageArray[stype][1]+".pdf"); //Android OS
	else if(input == 5) window.open(Edimax_PDF_Files+"redirect.pdf");    //redirect
	else if(input == 6) window.open(Edimax_PDF_Files+"lan.pdf");         //lan
}
</SCRIPT>
<style type="text/css">
.footBorder {border-bottom-style:solid; border-color:#d8d8d8;border-width:1px;}
</style>

</head>
<body class="" onLoad="init()">
<!--====================================================================================================================================-->
<div class="help-information" id="help-box" style="display:none">
	<table class="help-table1">
		<tr>
			<td>
				<div class="help-text" id="help-info1">
					<script>dw(vpnHelpInfo1)</script>
						<br>
					<script>dw(vpnHelpInfo2)</script>
				</div>
			</td>
		</tr>
	</table>
</div>

<script>
HelpButton();
</script>
<blockquote>
	<fieldset name="vpn_fieldset_1" id="vpn_fieldset_1">
		<legend><script>dw(OpenVPN)</script>&nbsp;<script>dw(Server)</script></legend>
		<form action="/goform/formVPNsetup" method=POST name="VPNSetup">
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
					<tr>
						<td><div class="iosbotton-itemText1-len"><b><script>dw(Enabled)</script>&nbsp;<script>dw(VPN)</script>&nbsp;<script>dw(Server)</script>&nbsp;:</b></div></td>
						<td><div class="iosbotton-itemText2-len">
							<label class="checkbox-switch" style="font-size: 25px;">
								<script>
									if (openvpnENABLED[0]==0)
										document.write('<input  id="VPN_Server_Enabled" name="VPN_Server_Enabled" type="checkbox" onclick="VPN_Server_Switch(2)">');
									else
										document.write('<input  id="VPN_Server_Enabled" name="VPN_Server_Enabled" type="checkbox" onclick="VPN_Server_Switch(2)" checked>');
								</script>
						    <span></span>
							</label>
						</div></td>
					</tr>
					<input type="hidden" value="1" name="InterfaceType">        <!--TUN-->
					<input type="hidden" value="10.8.0.0" name="Subnet">        <!--10.8.0.0-->
					<input type="hidden" value="255.255.255.0" name="Netmask">  <!--255.255.255.0-->
					<input type="hidden" value="1" name="Protocol">             <!--UDP-->
					<input type="hidden" value="443" name="ServerPort">         <!--443-->	
					<input type="hidden" value="no" name="AuthOnlyUserName">    <!--no-->
					<input type="hidden" value="0" name="AuthMode">             <!--TLS-->
					<input type="hidden" value="1" name="PushLAN">              <!--yes-->
					<input type="hidden" value="1" name="RedirectTraffic">      <!--1:yes 0:no-->
					<input type="hidden" value="1" name="ResDNS">               <!--1:yes 0:no-->	
					<input type="hidden" value="1" name="saveVPNsetup">
				</table>
			</table>
		</form>	
		<!-- ================================================================== -->
		<form name="VPN_Server_WAN_Check" method=POST action="" id="submit_VPNWANCheck">
			<input type="hidden" value="" name="VPN_Enabled">
		</form>	
		<form name="VPN_Server_Setup" method="POST" action="" id="submit_VPNdefSetup" >
			<input type="hidden" value="" name="VPN_Enabled">	
			<input type="hidden" value="" name="VPN_InterfaceType">
			<input type="hidden" value="" name="VPN_Subnet">
			<input type="hidden" value="" name="VPN_Netmask">
			<input type="hidden" value="" name="VPN_Protocol">
			<input type="hidden" value="" name="VPN_Port">
			<input type="hidden" value="" name="VPN_AuthMode">
			<input type="hidden" value="" name="VPN_RedirectTraffic">
			<input type="hidden" value="" name="VPN_PushLAN">
			<input type="hidden" value="" name="VPN_ResDNS">
			<input type="hidden" value="" name="VPN_AdvSetup">
			<input type="hidden" value="" name="VPN_AuthOnlyUserName">
			<input type="hidden" value="" name="VPN_Save_Setup">
			<input type="hidden" value="1" name="iqsetupclose">
			<input type="hidden" value="/inter_vpn.asp" name="submit-url">
		</form>
		<form action="/goform/formVPNExportClientConf" method=POST name="submit_VPNExportClientConf">
			<input type="hidden" value="" name="VPN_Client_conf">
		</form>	
		<!-- ================================================================== -->

		<br>

		<!-- ================================================================== -->	
		<form name="VPNClientFiles" method="POST" action="" >
			<table align="center" border="0" Cellspacing="0" width="700">
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
					<tr>
						<td><span class="itemText"><b><script>dw(serverInfo)</script>&nbsp;:</b></span>
						</td>
					</tr>
				</table>

				<table align="center" border="0" Cellspacing="0" width="600" cellpadding="5" style="border-top:1px #848484 solid;border-bottom:1px #848484 solid;">
					<tr style="background-color: #F0F0F0;">
						<td width="50%" >
							<div align="center" class="choose-itemText">
								EDIMAX&nbsp;<script>dw(DDNS)</script>
							</div>
						</td>
						<td width="50%" >
							<div align="center" class="choose-itemText" id="ServerDDNS_1" style="display:block">
								<script>document.write("--");</script>
							</div>
							<div align="center" class="choose-itemText" id="ServerDDNS_2" style="display:none">
								<script>
									var edimaxUID="<% getInfo("ddnsEdimaxUid"); %>";
									var edimaxURL=".router.myedimax.com";
									var edimaxddnsName=edimaxUID+edimaxURL;
									document.write(edimaxddnsName);
								</script>
							</div>
						</td>
					</tr>

					<tr style="background-color: #F0F0F0;">
						<td>
							<div align="center" class="choose-itemText">
								<script>dw(VPN)</script>&nbsp;<script>dw(subnet)</script>/<script>dw(netmask)</script>
							</div>
						</td>
						<td>
							<div align="center" class="choose-itemText" id="ServerSubnetNetmask_1" style="display:block">
								<script>document.write("--");</script>
							</div>
							<div align="center" class="choose-itemText" id="ServerSubnetNetmask_2" style="display:none">
								<script>document.write(openvpnSubnetNetmask[0]);</script>/<script>document.write(openvpnSubnetNetmask[1]);</script>
							</div>
						</td>
					</tr>

					<tr style="background-color: #F0F0F0;">
						<td>
							<div align="center" class="choose-itemText">
								<script>dw(vpnProtocol)</script>
							</div>
						</td>
						<td>
							<div align="center" class="choose-itemText" id="ServerProtocol_1" style="display:block">
								<script>document.write("--");</script>
							</div>
							<div align="center" class="choose-itemText" id="ServerProtocol_2" style="display:none">
								<script>
									if (openvpnProtocol[0]==0)
									{
										document.write(showText(TCP));
									}
									else
									{
										document.write(showText(UDP));
									}
								</script>
							</div>
						</td>
					</tr>

					<tr style="background-color: #F0F0F0;">
						<td>
							<div align="center" class="choose-itemText">
								<script>dw(SerPort)</script>
							</div>
						</td>
						<td>
							<div align="center" class="choose-itemText" id="ServerPORT_1" style="display:block">
								<script>document.write("--");</script>
							</div>
							<div align="center" class="choose-itemText" id="ServerPORT_2" style="display:none">
								<script>document.write(openvpnPORT[0]);</script>
							</div>
						</td>
					</tr>
				</table>
			</table>
			<!-- ================================================================== -->

			<br>

			<!-- ================================================================== -->	
			<table align="center" border="0" Cellspacing="0" width="700">
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
					<tr>
						<td><span class="itemText"><b><script>dw(clientConfFile)</script>&nbsp;:</b></span>
						</td>
					</tr>
				</table>
				<table align="center" border="0" Cellspacing="0" width="600" cellpadding="5" style="border-top:1px #848484 solid;border-bottom:1px #848484 solid;">
					<tr style="background-color: #F0F0F0;">
						<td width="50%">
							<div align="center" class="choose-itemText"><script>dw(overTheWall)</script></div>
						</td>
						<td width="30%"><div align="center" class="choose-itemText">
							<span>
								<script>
									document.write('<input type="button" value="'+showText(Export)+'" name="Client1_conf" onclick="return saveChanges(1)"  class="ui-button">');
								</script>
							</span>
						</div></td>
						<td width="20%">
							<div align="center">
								<script>
									document.write('&nbsp;&nbsp;<span class="itemText"><a href="#" onClick="return getWeb_Help(5)">'+showText(more)+'...</a></span>');
								</script>
							</div>
						</td>
					</tr>
					<tr style="background-color: #F0F0F0;">
						<td>
							<div align="center" class="choose-itemText"><script>dw(onlyAccessLAN)</script></div>
						</td>
						<td><div align="center" class="choose-itemText">
							<span>
								<script>
									document.write('<input type="button" value="'+showText(Export)+'" name="Client2_conf" onclick="return saveChanges(2)"  class="ui-button">');
								</script>
							</span>
						</div></td>
						<td>
							<div align="center">
								<script>
									document.write('&nbsp;&nbsp;<span class="itemText"><a href="#" onClick="return getWeb_Help(6)">'+showText(more)+'...</a></span>');
								</script>
							</div>
						</td>
					</tr>
				</table>
			</table>
		</form>
		<!-- ================================================================== -->	
	</fieldset>	
	<!-- ==================================================================================================================================== -->

	<br>

	<!-- ==================================================================================================================================== -->	
	<fieldset name="vpn_fieldset_2" id="vpn_fieldset_2">
		<legend><script>dw(OpenVPN)</script>&nbsp;<script>dw(clientSetting)</script></legend>
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<table align="center" border="0" Cellspacing="0" width="600" cellpadding="5">
					<tr>
						<td><div align="left" class="choose-itemText">
							<span>
								<script>
									document.write('&nbsp;&nbsp;<span><a href="#" onClick="return getWeb_Help(1)">'+showText(os1)+'</a></span>');
								</script>
							</span>
						</div></td>
					</tr>
					<tr>
						<td><div align="left" class="choose-itemText">
							<span>
								<script>
									document.write('&nbsp;&nbsp;<span><a href="#" onClick="return getWeb_Help(2)">'+showText(os2)+'</a></span>');
								</script>
							</span>
						</div></td>
					</tr>
					<tr>
						<td><div align="left" class="choose-itemText">
							<span>
								<script>
									document.write('&nbsp;&nbsp;<span><a href="#" onClick="return getWeb_Help(3)">'+showText(os3)+'</a></span>');
								</script>
							</span>
						</div></td>
					</tr>
					<tr>
						<td><div align="left" class="choose-itemText">
							<span>
								<script>
									document.write('&nbsp;&nbsp;<span><a href="#" onClick="return getWeb_Help(4)">'+showText(os4)+'</a></span>');
								</script>
							</span>
						</div></td>
					</tr>
				</table>
			</table>
	</fieldset>	
	<!-- ==================================================================================================================================== -->

	<br>

	<!-- ==================================================================================================================================== -->	
	<fieldset name="vpn_fieldset_3" id="vpn_fieldset_3">
		<legend><script>dw(OpenVPN)</script>&nbsp;<script>dw(clientAccCont)</script></legend>
		<form name="VPNUser" method=POST action="" id="submit_VPNUser">
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
					<tr>
						<td width="15%" align="center" class="titletableText"><script>dw(Status)</script></td>
						<td width="35%" align="center" class="titletableText"><script>dw(ClientName)</script></td>
						<td width="35%" align="center" class="titletableText"><script>dw(Password)</script></td>
						<td width="15%" align="center" class="titletableText"></td>
					</tr>
				</table>
				<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
					<tr>
						<td width="15%" align="center"></td>
						<td width="35%" align="center"><input type="text" id="username" name="username" size="16" maxlength="16"></td>
						<td width="35%" align="center"><input type="text" id="password" name="password" size="16" maxlength="16"></td>
						<td width="15%" align="center">
							<script>
							document.write('<input type="button" value="'+showText(add)+'" onClick="return checkAccountClick(0,0)" class="ui-button-nolimit">&nbsp;')
							</script>
						</td>
					</tr>
				</table>

		
				<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
					<% OpenVpnAccountList(); %>
				</table>

				<tr class="titletableText" height="1px">
					<td align="center" ></td>
					<td align="center" ></td>
					<td align="center" ></td>
					<td align="center" ></td>
				</tr>
			</table>

			<input type="hidden" value="" name="addAccount">
			<input type="hidden" value="" name="deleteSelAccount">
			<input type="hidden" value="1" name="iqsetupclose">
		</form>	

	</fieldset>	
<script>
	$("#help-box").hide();
	displayObj();
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
</html>
