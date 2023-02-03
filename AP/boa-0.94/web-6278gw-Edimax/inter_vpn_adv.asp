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
<% language=javascript %>
<SCRIPT>
var openvpnENABLED       =	<%getInfo("openvpnENABLED");%>;
var openvpnInterType     =	<%getInfo("openvpnInterType");%>;
var openvpnProtocol      =	<%getInfo("openvpnProtocol");%>;
var openvpnPORT          =	<%getInfo("openvpnPORT");%>;
var openvpnAuthMode      =	<%getInfo("openvpnAuthMode");%>;
var openvpnOnlyAuthUser  =	<%getInfo("openvpnOnlyAuthUser");%>;
var openvpnSubnetNetmask =	<%getInfo("openvpnSubnetNetmask");%>;
var openvpnStartAndEndIP =	<%getInfo("openvpnStartAndEndIP");%>;
var openvpnTotalUser     =  8;
var dhcpRangeStart="<% getInfo("dhcpRangeStart"); %>";
var dhcpRangeEnd="<% getInfo("dhcpRangeEnd"); %>";
var check_vpn_setup_submit1, check_vpn_setup_submit2;


function displayObj()
{
	with(document.VPNSetup) 
	{	
		if (openvpnENABLED[0]==0)
			VPN_Server_Enabled.checked=false;
		else
			VPN_Server_Enabled.checked=true;

		if ((openvpnSubnetNetmask[0]=="0.0.0.0") && (openvpnSubnetNetmask[1]=="0.0.0.0"))
		{
			Subnet.value  = "10.8.0.0";
			Netmask.value = "255.255.255.0";
		}
		else
		{
			Subnet.value     = openvpnSubnetNetmask[0];
			Netmask.value    = openvpnSubnetNetmask[1];
		}

		StartIP.value    = openvpnStartAndEndIP[0];
		EndIP.value      = openvpnStartAndEndIP[1];
		ServerPort.value = openvpnPORT[0];
	}
	checkInterfaceType()
	checkAuthOnlyUserName()
	VPN_Server_Switch()
}

function init()
{
	$("#help-box").hide();
}

function checkClientRange(start,end)
{
	start_d = getDigit(start,4);
	start_d += getDigit(start,3)*256;
	start_d += getDigit(start,2)*256;
	start_d += getDigit(start,1)*256;
	end_d = getDigit(end,4);
	end_d += getDigit(end,3)*256;
	end_d += getDigit(end,2)*256;
	end_d += getDigit(end,1)*256;
	if (start_d < end_d)
		return true;
	return false;
}

function checkInterfaceType()
{
	if( document.VPNSetup.InterfaceType.value == 0 )
	{
		document.getElementById('VPN_Subnet_Netmask').style.display="none";
		document.getElementById('VPN_StartIP_EndIP').style.display="block";
	}
	else
	{
		document.getElementById('VPN_Subnet_Netmask').style.display="block";
		document.getElementById('VPN_StartIP_EndIP').style.display="none";
	}
}
function checkAuthOnlyUserName()
{
	if( document.VPNSetup.AuthOnlyUserName[0].checked == false )
	{
			document.getElementById('VPN_Keys_Certification').style.display="block";
	}
	else
	{
			document.VPNSetup.AuthMode.value = 0;
			document.getElementById('VPN_Keys_Certification').style.display="none";
	}
}

function checkkeysUpload()
{
		window.open( "vpn_key.asp", 'OpenVPN', 'width=800, height=600 , screenX=100, screenY=50,resizable=yes,   toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=yes' );

}

function checkVPNsetup(saveFlag)
{
	with(document.VPNSetup) 
	{
		if( saveFlag == 0 )
		{	
			if (VPN_Server_Enabled.checked==true)
			{
				//----check setup----
				if(InterfaceType.value == 0)
				{
					if (!ipRule( StartIP, showText(VPNclientstartIP), "ip", 0)) return false;
					if (!ipRule( EndIP, showText(VPNclientEndIP), "ip", 0)) return false;
					if (!checkClientRange(StartIP.value, EndIP.value))
					{
						alert(showText(InvalidSomething).replace(/#####/,showText(VPNIPrange))+' '+showText(EndingGreaterStartIP)+'.');
						return false;
					}
				}
				else
				{
					if (!ipRule( Subnet, showText(subnet), "ip", 0)) return false;
					if (!maskRule( Netmask, showText(netmask), 0)) 
					{
						return false;
					}

					SubnetOne   = getDigit(Subnet.value,1);
					SubnetTwo   = getDigit(Subnet.value,2);
					SubnetThree = getDigit(Subnet.value,3);
					SubnetFour  = getDigit(Subnet.value,4);
					NetmaskOne   = getDigit(Netmask.value,1);
					NetmaskTwo   = getDigit(Netmask.value,2);
					NetmaskThree = getDigit(Netmask.value,3);
					NetmaskFour  = getDigit(Netmask.value,4);

					Subnet.value = (SubnetOne & NetmaskOne)+'.'+(SubnetTwo & NetmaskTwo)+'.'+(SubnetThree & NetmaskThree)+'.'+(SubnetFour & NetmaskFour);
				}

				if (!portRule(ServerPort, showText(PortRange), 0, "", 1, 65535, 1)) return false;

				if (AuthMode.value == 0)
				{
					if (document.VPN_Ca_Crt_form.VPN_Ca_Crt.value == "")
					{
						alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(caCrt)));
						return false;
					}
					else if (document.VPN_DH_Pem_form.VPN_DH_Pem.value == "")
					{
						alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(dhPem)));
						return false;
					}
					else if (document.VPN_Server_Crt_form.VPN_Server_Crt.value == "")
					{
						alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(serverCrt)));
						return false;
					}
					else if (document.VPN_Server_Key_form.VPN_Server_Key.value == "")
					{
						alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(serverKey)));
						return false;
					}

					if (AuthOnlyUserName[0].checked == false)
					{
						if (document.VPN_Client_Crt_form.VPN_Client_Crt.value == "")
						{
							alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(clientCrt)));
							return false;
						}
						else if (document.VPN_Client_Key_form.VPN_Client_Key.value == "")
						{
							alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(clientKey)));
							return false;
						}
					}
				}
				else
				{
					if (document.VPN_Static_Key_form.VPN_Static_Key.value == "")
					{
						alert(showText(keysAndCertification) +" -> "+ showText(SomethingCannotBeEmpty).replace(/#####/,showText(staticKey)));
						return false;
					}
				}

				//----submit setup----
				document.VPN_Server_Setup.VPN_Enabled.value="yes";
				document.VPN_Server_Setup.VPN_InterfaceType.value=InterfaceType.value;
				if(InterfaceType.value == 0)
				{
					document.VPN_Server_Setup.VPN_StartIP.value = StartIP.value;
					document.VPN_Server_Setup.VPN_EndIP.value   = EndIP.value;
				}
				else
				{
					document.VPN_Server_Setup.VPN_Subnet.value  = Subnet.value;
					document.VPN_Server_Setup.VPN_Netmask.value = Netmask.value;				
				}

				if(AuthOnlyUserName[0].checked == false)
				{
					document.VPN_Server_Setup.VPN_AuthOnlyUserName.value = "no";
				}
				else
				{
					document.VPN_Server_Setup.VPN_AuthOnlyUserName.value = "yes";
				}
				document.VPN_Server_Setup.VPN_AuthMode.value         = AuthMode.value;
				document.VPN_Server_Setup.VPN_Protocol.value         = Protocol.value;
				document.VPN_Server_Setup.VPN_Port.value             = ServerPort.value;

				document.VPN_Server_Setup.VPN_AdvSetup.value         = "1";
			}
			else
			{
				//----submit setup----
				document.VPN_Server_Setup.VPN_Enabled.value="no";
			}
			document.VPN_Server_Setup.VPN_Save_Setup.value         = saveVPNsetup.value;


			//----submit cgi----
			if (AuthMode.value == 0)
			{
				$.post("/goform/formVPNCaCrt",$("#submit_VPNCaCrt").serialize());
				$.post("/goform/formVPNDHPem"    , $("#submit_VPNDHPem").serialize());
				$.post("/goform/formVPNServerCrt", $("#submit_VPNServerCrt").serialize());
				$.post("/goform/formVPNServerKey", $("#submit_VPNServerKey").serialize());

				if(AuthOnlyUserName[0].checked == false)
				{
					$.post("/goform/formVPNClientCrt", $("#submit_VPNClientCrt").serialize());
					$.post("/goform/formVPNClientKey", $("#submit_VPNClientKey").serialize());
				}
			}
			else
			{
				$.post("/goform/formVPNStaticKey", $("#submit_VPNStaticKey").serialize());
			}
			check_vpn_setup_submit1=setInterval("submitVPNsetup1()", 500);
		}
		else if (saveFlag == 1)
		{	
			document.submit_VPNExportClientConf.VPN_Client_conf.value="3";
			document.submit_VPNExportClientConf.submit();
		}
	}
}

function submitVPNsetup1()
{
	with(document.VPNSetup) 
	{
	$.get("getVPN_Setup_State_Submit.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
		if (AuthMode.value == 0)
		{
			if(AuthOnlyUserName[0].checked == false)
			{
				if(data==63) {
					window.clearInterval(check_vpn_setup_submit1);
					$.post("/goform/formVPNsetup", $("#submit_VPNadvSetup").serialize());
					check_vpn_setup_submit2=setInterval("submitVPNsetup2()", 500);
				}
			}
			else
			{
				if(data==15) {
					window.clearInterval(check_vpn_setup_submit1);
					$.post("/goform/formVPNsetup", $("#submit_VPNadvSetup").serialize());
					check_vpn_setup_submit2=setInterval("submitVPNsetup2()", 500);
				}
			}
		}
		else
		{
			if(data==64) {
				window.clearInterval(check_vpn_setup_submit1);
				$.post("/goform/formVPNsetup", $("#submit_VPNadvSetup").serialize());
				check_vpn_setup_submit2=setInterval("submitVPNsetup2()", 500);
			}
		}
	});
	}
}

function submitVPNsetup2()
{
		$.get("getVPN_Setup_State_Submit.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
			if(data==128) {
				window.clearInterval(check_vpn_setup_submit2);
				window.location.replace("/inter_vpn_adv.asp");
			}
	});
}

function VPN_Server_Switch()
{
	with(document.VPNSetup) 
	{
		if (VPN_Server_Enabled.checked==true)
		{
			InterfaceType.disabled=false;
			Subnet.disabled=false;
			Netmask.disabled=false;
			StartIP.disabled=false;
			EndIP.disabled=false;
			Protocol.disabled=false;
			ServerPort.disabled=false;
			AuthMode.disabled=false;
			AuthOnlyUserName[0].disabled=false;
			AuthOnlyUserName[1].disabled=false;
			keysUpload.disabled=false;
			check_vpn_ClientConf_statStop=setInterval("checkClientConfState()", 2000);
		}
		else
		{
			InterfaceType.disabled=true;
			Subnet.disabled=true;
			Netmask.disabled=true;
			StartIP.disabled=true;
			EndIP.disabled=true;
			Protocol.disabled=true;
			ServerPort.disabled=true;
			AuthMode.disabled=true;
			AuthOnlyUserName[0].disabled=true;
			AuthOnlyUserName[1].disabled=true;
			keysUpload.disabled=true;
		}
		Client_conf.disabled=true;
	}
}

function checkClientConfState()
{
	with(document.VPNSetup) 
	{
	$.get("getVPN_ClientConf_State.asp", { noCache: Math.floor(Math.random() * 10000) }, function (data) {
			var andGateData;

			andGateData=data&4;
			if(andGateData==4) Client_conf.disabled=false;
			else Client_conf.disabled=true;
	});
	}
}
</SCRIPT>
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
	<fieldset name="vpn_set_fieldset" id="vpn_set_fieldset">
		<legend><script>dw(OpenVPN)</script>&nbsp;<script>dw(Server)</script>&nbsp;-&nbsp;<script>dw(AdvanceSetting)</script></legend>
		<form action="/goform/formVPNsetup" method=POST name="VPNSetup">
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td colspan="2"><input type="checkbox" id="VPN_Server_Enabled" name="VPN_Server_Enabled" value="" onclick="VPN_Server_Switch()">&nbsp;<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(VPN)</script>&nbsp;<script>dw(Server)</script></span>
				</td>
			</tr>

			<tr>
				<td><div class="itemText-len"><script>dw(interfaceType)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<select size="1" name="InterfaceType" onChange="checkInterfaceType()">
							<script>
								if (openvpnInterType[0]==0)
								{
									document.write('<option value="0" selected>'+showText(TAP)+'</option>');
									document.write('<option value="1" >'+showText(TUN)+'</option>');
								}
								else
								{
									document.write('<option value="0" >'+showText(TAP)+'</option>');
									document.write('<option value="1" selected>'+showText(TUN)+'</option>');
								}
							</script>
						</select>
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_Subnet_Netmask" style="display:none">
			<tr>
				<td><div class="itemText-len"><script>dw(VPN)</script>&nbsp;<script>dw(subnet)</script>/<script>dw(netmask)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<input type="text" name="Subnet"  size="15" maxlength="15" value="">/
						<input type="text" name="Netmask" size="15" maxlength="15" value="">
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_StartIP_EndIP" style="display:none">
			<tr>
				<td><div class="itemText-len"><script>dw(VPN)</script>&nbsp;<script>dw(StartIP)</script>/<script>dw(endIP)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<input type="text" name="StartIP"  size="15" maxlength="15" value="">/
						<input type="text" name="EndIP" size="15" maxlength="15" value="">
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td><div class="itemText-len"><script>dw(vpnProtocol)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<select size="1" name="Protocol">
							<script>
								if (openvpnProtocol[0]==0)
								{
									document.write('<option value="0" selected>'+showText(TCP)+'</option>');
									document.write('<option value="1" >'+showText(UDP)+'</option>');
								}
								else
								{
									document.write('<option value="0" >'+showText(TCP)+'</option>');
									document.write('<option value="1" selected>'+showText(UDP)+'</option>');
								}
							</script>
						</select>
					</span>
				</div></td>
			</tr>

			<tr>
				<td><div class="itemText-len"><script>dw(SerPort)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<input type="text" name="ServerPort" size="5" maxlength="5" value="">
						(<script>dw(Default)</script>&nbsp;:&nbsp;443)
					</span>
				</div></td>
			</tr>

			<tr>
				<td><div class="itemText-len"><script>dw(clientNamePassword)</script>&nbsp;<script>dw(onlyAuth)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<script>
							if (openvpnOnlyAuthUser[0]==1)
							{
								document.write('<input type="radio" name="AuthOnlyUserName" onClick="checkAuthOnlyUserName()" checked>YES');
								document.write('<input type="radio" name="AuthOnlyUserName" onClick="checkAuthOnlyUserName()" >NO');
							}
							else
							{
								document.write('<input type="radio" name="AuthOnlyUserName" onClick="checkAuthOnlyUserName()" >YES');
								document.write('<input type="radio" name="AuthOnlyUserName" onClick="checkAuthOnlyUserName()" checked>NO');
							}
						</script>
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_Keys_Certification" style="display:none">
			<tr>
				<td><div class="itemText-len"><script>dw(authMode)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
						<select size="1" name="AuthMode">
							<script>
								if (openvpnAuthMode[0]==0)
								{
									document.write('<option value="0" selected>TLS</option>');
									document.write('<option value="1" >'+showText(staticKey)+'</option>');
								}
								else
								{
									document.write('<option value="0" >TLS</option>');
									document.write('<option value="1" selected>'+showText(staticKey)+'</option>');
								}
							</script>
						</select>
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="700">
			<tr>
				<td><div class="itemText-len"><script>dw(keysAndCertification)</script></div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
					<script>
						document.write('<input type="button" value="'+showText(contentModification)+'" name="keysUpload" onclick="return checkkeysUpload()">');	
					</script>
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="700">
			<tr>
				<td><div class="itemText-len"><script>dw(clientConfFile)</script>
				</div></td>
				<td><div class="itemText2-len">
					<span class="choose-itemText ">
					<script>
						document.write('<input type="button" value="'+showText(Export)+'" name="Client_conf" onclick="return checkVPNsetup(1)">');	
					</script>
					</span>
				</div></td>
			</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="700">
			<tr>
				<td style="text-align:center; padding-top:10px;">
					<script>
						document.write('<input type="button" value="'+showText(Apply)+'&nbsp;'+showText(VPN)+'&nbsp;'+showText(Settings)+'" name="saveVPNsetup" onclick="return checkVPNsetup(0)" class="ui-button1">');
					</script>
				</td>
			</tr>
			</table>
		</form>	
		<!-- ================================================================== -->

		<form name="VPN_Server_Setup" method="POST" action="" id="submit_VPNadvSetup" >
			<input type="hidden" value="" name="VPN_Enabled">	
			<input type="hidden" value="" name="VPN_InterfaceType">
			<input type="hidden" value="" name="VPN_Subnet">
			<input type="hidden" value="" name="VPN_Netmask">
			<input type="hidden" value="" name="VPN_StartIP">
			<input type="hidden" value="" name="VPN_EndIP">
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
			<input type="hidden" value="/inter_vpn_adv.asp" name="submit-url">
		</form>	
		<form name="VPN_Ca_Crt_form" method="POST" action="" id="submit_VPNCaCrt">
			<input type="hidden" value="<% getInfo("openvpnCaCrt"); %>" name="VPN_Ca_Crt">
		</form>
		<form name="VPN_DH_Pem_form" method="POST" action="" id="submit_VPNDHPem">
			<input type="hidden" value="<% getInfo("openvpnDHPem"); %>" name="VPN_DH_Pem">
		</form>	
		<form name="VPN_Server_Crt_form" method="POST" action="" id="submit_VPNServerCrt">
			<input type="hidden" value="<% getInfo("openvpnServerCrt"); %>" name="VPN_Server_Crt">
		</form>	
		<form name="VPN_Server_Key_form" method="POST" action="" id="submit_VPNServerKey">
			<input type="hidden" value="<% getInfo("openvpnServerKey"); %>" name="VPN_Server_Key">
		</form>	
		<form name="VPN_Client_Crt_form" method="POST" action="" id="submit_VPNClientCrt">
			<input type="hidden" value="<% getInfo("openvpnClientCrt"); %>" name="VPN_Client_Crt">
		</form>	
		<form name="VPN_Client_Key_form" method="POST" action="" id="submit_VPNClientKey">
			<input type="hidden" value="<% getInfo("openvpnClientKey"); %>" name="VPN_Client_Key">
		</form>	
		<form name="VPN_Static_Key_form" method="POST" action="" id="submit_VPNStaticKey">
			<input type="hidden" value="<% getInfo("openvpnStaticKey"); %>" name="VPN_Static_Key">
		</form>	
		<form action="/goform/formVPNExportClientConf" method=POST name="submit_VPNExportClientConf">
			<input type="hidden" value="" name="VPN_Client_conf">
		</form>	
		<!-- ================================================================== -->
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
