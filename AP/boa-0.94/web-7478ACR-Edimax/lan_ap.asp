
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
//var initialDhcp;
var wizMode = <%getVar("wizmode","","");%>;
var dhcpmode = <%getVar("dhcp","","");%>;
var timeValue = new Array("1800","3600","7200","43200","86400","172800","604800","1209600","315360000");
var timeName = new Array(showText(halfh),showText(oneh),showText(twohs),showText(halfd),showText(oned),showText(twods),showText(onewk),showText(twowks),showText(forever));

function autoIp_Clicked() 
{
	with(document.tcpip)
	if (ipmode[0].checked==true) 
	{
		HS24_DHCP.value=0;
		HS24_DHCPswitch.value=0;
		dnsMode.value="dnsAuto";
		ip.disabled = true;
		mask.disabled = true;
		gateway.disabled = true;
		dns1.disabled = true;
		//document.getElementById("ipsetting").style.display = "none"; 
		//document.getElementById("nosetting").style.display = "block";
		HS24_DHCP.disabled = true;
		HS24_dhcpRangeStart.disabled = true;
		HS24_dhcpRangeEnd.disabled = true;
		//DomainName.disabled = true;
		HS24_leaseTime.disabled = true;
//		dns1.disabled = true;
//		dns2.disabled = true;
		HS24_DhcpGatewayIP.disabled = true;			
	}
	else 
	{
		//document.getElementById("ipsetting").style.display = "block"; 
		//document.getElementById("nosetting").style.display = "none"; 
		dnsMode.value="dnsManual";
		ip.disabled = false;
		mask.disabled = false;
		gateway.disabled = false;
		dns1.disabled = false;
		HS24_DHCP.disabled = false;
			if(HS24_DHCP.value ==2)
			{
			HS24_DHCPswitch.value=2;
			HS24_dhcpRangeStart.disabled = false;
			HS24_dhcpRangeEnd.disabled = false;
			//DomainName.disabled = false;
			HS24_leaseTime.disabled = false;
	//		dns1.disabled = false;
	//		dns2.disabled = false;
			HS24_DhcpGatewayIP.disabled = false;
			}
			else
			{
			HS24_DHCPswitch.value=0;
			HS24_dhcpRangeStart.disabled = true;
			HS24_dhcpRangeEnd.disabled = true;
			//DomainName.disabled = true;
			HS24_leaseTime.disabled = true;
	//		dns1.disabled = true;
	//		dns2.disabled = true;
			HS24_DhcpGatewayIP.disabled = true;
			}
	}

}

function resetform(who)
{
	switch(who)
	{
		case 0:
			document.tcpip.reset();
			break;
		case 1:
			document.formSDHCPAdd.mac.value="";
			document.formSDHCPAdd.ip.value="";
			break;
		default: 
			alert("error!");
			return false;
			break;
		}
}

function checkValue(str)
{
	for (var i=0; i<str.length; i++)
	{
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'z') ||
			(str.charAt(i) == '_')|| (str.charAt(i) == '-')||
			(str.charAt(i) == '.'))
			continue;
		return 0;
	}
	return 1;
}
function saveChanges(rebootFlag)
{

	if (document.tcpip.ipmode[1].checked==true) 
	{
		if (!checkip(0)) return false;
		if (!checkip(1)) return false;
		if (!checkip(2)) return false;

		//_HIGHSPEED_WIFI_REPEATER_
		with(document.tcpip) 
		{
			//if ( !ipRule( ip, showText(IPAddr), "ip", 1)) return false;
			//if(!maskRule(mask, showText(SubnetMask), 1)) return false;
			if ( HS24_DHCP.value == 2)
			{
				//********** dhcp start ip **********
				if ( HS24_dhcpRangeStart.value == ip.value)
				{
					alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientstartIP))+' '+showText(ShouldBesameasLAN)+'.');
					setFocus(HS24_dhcpRangeStart);
					return false;
				}
				if ( !ipRule( HS24_dhcpRangeStart, showText(DHCPclientstartIP), "ip", 1)) return false;
				if ( !checkSubnet(ip.value, mask.value, HS24_dhcpRangeStart.value))
				{
					alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientstartIP))+' '+showText(ShouldBesamesubmetCurIP)+'.');
				
					setFocus(HS24_dhcpRangeStart);
					return false;
				}
				//********** dhcp end ip **********
				if ( !ipRule( HS24_dhcpRangeEnd, showText(DHCPclientEndIP), "ip", 1)) return false;
				if ( !checkSubnet(ip.value, mask.value, HS24_dhcpRangeEnd.value))
				{
					alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientEndIP))+' '+showText(ShouldBesamesubmetCurIP)+'.');
					setFocus(HS24_dhcpRangeEnd);
					return false;
				}
				if ( !checkClientRange(HS24_dhcpRangeStart.value, HS24_dhcpRangeEnd.value) )
				{
					alert(showText(InvalidSomething).replace(/#####/,showText(DHCPIPrange))+' '+showText(EndingGreaterStartIP)+'.');
					setFocus(HS24_dhcpRangeStart);
					return false;
				}
				//********** dhcp server gateway **********
				if (HS24_DhcpGatewayIP.value == "0.0.0.0")
				{
					alert(showText(cannotbe).replace(/####/,showText(gateAddr)).replace(/@####/,"0.0.0.0"));
					setFocus(HS24_DhcpGatewayIP);
					return false;
				}

				if (HS24_DhcpGatewayIP.value == "")
				{
					alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(gateAddr)));
					setFocus(HS24_DhcpGatewayIP);
					return false;
				}

				if ( !ipRule( HS24_DhcpGatewayIP, showText(gateAddr)+" ", "gw", 1)) return false;
			}
		

			if (ip.value!=ip.defaultValue)
			{
				alert(showText(connectNewAddress));
				ipChanged.value = 1;
			}
		}
	}
	//_HIGHSPEED_WIFI_REPEATER_
	with(document.tcpip) 
	{
		//if ( !ipRule( ip, showText(IPAddr), "ip", 1)) return false;
		//if(!maskRule(mask, showText(SubnetMask), 1)) return false;		
	if(rebootFlag)
	isApply.value = "ok";
		
	submit();	
	}
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
	if ( start_d < end_d )
		return true;
	return false;
}

function checkip(index)
{
	if(index == 1)
	{
		if (document.tcpip.ip.value == "0.0.0.0")
		{
			alert(showText(cannotbe).replace(/####/,showText(IPAddr)).replace(/@####/,"0.0.0.0"));
			document.tcpip.ip.value = "";
			return false;
		}
		if ( !ipRule( document.tcpip.ip, showText(IPAddr), "ip", 1)) return false;
		return true;
	}
	else if(index == 2)
	{
		if(!maskRule(document.tcpip.mask, showText(SubnetMask), 1)) return false;
		return true;
	}
	else if(index == 3)
	{
		if ( !ipRule( document.tcpip.gateway, showText(gateAddr), "gw", 1)) return false;
		else return true;
	}
	else
	{
		if ( !ipRule( document.tcpip.dns1, showText(DNS), "ip", 1)) return false;
		else return true;
	}
}

</SCRIPT>

</head>


<body>



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1">
								<script>//dw(LANHelpInfo)</script>
							You can allocate IP addresses to your LAN client PCs. The wireless router must have an IP Address in the local area network.
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<form action=/goform/formTcpipSetup method=POST name="tcpip">
		<input type="hidden" value="1" name="iqsetupclose">
		<input type="hidden" value="0" name="dhcp">
		<input type="hidden" value="" name="HS24_DHCPswitch">
		<input type="hidden" name="clIpDisabled" value="0">
		<input type="hidden" value="/lan_ap.asp" name="submit-url">
		<input type="hidden" value="" name="ipChanged">
		<input type="hidden" value="dnsAuto" name="dnsMode">

		<fieldset name="LANIP_fieldset" id="LANIP_fieldset">
			<legend><script>dw(LANIP)</script></legend>
				<br>
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td align="right" width="35%">
						<input type="radio" value="1" name="ipmode" <%getVar("dhcpswitch","1","checked")%> onClick="autoIp_Clicked()">
						</td>
						<td align="left" class="choose-itemText">
						<script>dw(ObtainIPauto)</script>
						</td>
					</tr>
					<tr>
						<td align="right" width="35%">
						<input type="radio" value="0" name="ipmode" <%getVar("dhcpswitch","0","checked")%> onClick="autoIp_Clicked()">
						</td>
						<td align="left" class="choose-itemText">
						<script>dw(UsefollowingIP)</script>
						</td>
					</tr>
				</table>
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td><div class="itemText-len"><script>dw(IPAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="ip" size="15" maxlength="15" value="<% getInfo("ip-rom"); %>" onChange="checkip(1)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="mask" size="15" maxlength="15" value="<% getInfo("mask-rom"); %>" onChange="checkip(2)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="gateway" size="15" maxlength="15" value="<% getInfo("gateway-rom"); %>" onChange="checkip(3)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="dns1" size="15" maxlength="15" value="<% getInfo("wan-dns1"); %>" onChange="checkip(4)">
						</div></td>
					</tr>
				</table>
				<br>
		</fieldset>
		<br>

		<!--_HIGHSPEED_WIFI_REPEATER_-->
		<fieldset name="DHCPServer_fieldset" id="DHCPServer_fieldset">
			<legend><script>dw(DHCPServer)</script></legend>

				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td><div class="itemText-len"><script>dw(DHCPServer)</script></div></td>
						<td><div class="itemText2-len">
							<select size="1" name="HS24_DHCP" onChange="autoIp_Clicked()">
								<script>
									
									if(dhcpmode == 0){
										document.write('<option value="0" selected>'+showText(disable)+'</option>');
										document.write('<option value="2" >'+showText(enable)+'</option>');}
									else{
										document.write('<option value="0">'+showText(disable)+'</option>');
										document.write('<option value="2" selected >'+showText(enable)+'</option>');
										}
								</script>
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(LeaseTime)</script></div></td>
						<td><div class="itemText2-len">
							<select name="HS24_leaseTime">
								<script language="javascript">	
									var leaseTimeGet = "<% getInfo("leaseTime"); %>";
									
									for(i=0;i<9;i++)
									{
										if(timeValue[i] == leaseTimeGet)
											document.write("<option value=\""+timeValue[i]+"\" selected>"+timeName[i]+"</option>");
										else
											document.write("<option value=\""+timeValue[i]+"\">"+timeName[i]+"</option>");
									}
								</script>
							</select>
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(StartIP)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="HS24_dhcpRangeStart" size="15" maxlength="15" value="<% getInfo("dhcpRangeStart"); %>">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(endIP)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="HS24_dhcpRangeEnd" size="15" maxlength="15" value="<% getInfo("dhcpRangeEnd"); %>">
						</div></td>
					</tr>
					<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="HS24_DhcpGatewayIP" size="15" maxlength="15" value="<% getInfo("DhcpGatewayIP"); %>">
						</div></td>
					</tr>
				</table>
		
		</fieldset>
		<!--_HIGHSPEED_WIFI_REPEATER_-->
		<input type="hidden" value="1" name="iqsetupclose">
		<input type="hidden" value="" name="isApply">
	</form>
		
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
		

<script>
	$("#help-box").hide();
	autoIp_Clicked();
</script>
<input type=hidden value="" name="isApply">
		</form>
</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
