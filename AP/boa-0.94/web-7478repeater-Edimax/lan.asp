
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
var timeValue = new Array("1800","3600","7200","43200","86400","172800","604800","1209600","315360000");
var timeName = new Array(showText(halfh),showText(oneh),showText(twohs),showText(halfd),showText(oned),showText(twods),showText(onewk),showText(twowks),showText(forever));

function dhcpChange(index)
{
	if ( index == 1 )
	{
		document.tcpip.dhcpRangeStart.disabled = false;
		document.tcpip.dhcpRangeEnd.disabled = false;
		//document.tcpip.DomainName.disabled = false;
		document.tcpip.leaseTime.disabled = false;
//			document.tcpip.dns1.disabled = false;
//			document.tcpip.dns2.disabled = false;
//		document.tcpip.dhcpGW.disabled = false;
	}
	else
	{
		document.tcpip.dhcpRangeStart.disabled = true;
		document.tcpip.dhcpRangeEnd.disabled = true;
		//document.tcpip.DomainName.disabled = true;
		document.tcpip.leaseTime.disabled = true;
//			document.tcpip.dns1.disabled = true;
//			document.tcpip.dns2.disabled = true;
//		document.tcpip.dhcpGW.disabled = true;
	}
}

function addClick() {

	with(document.formSDHCPAdd) {

		if ( !macRule(mac, 0))
			return false;

		for(i=1; i<=document.getElementsByName("smacnum")[0].value; i++)
		{
			if( mac.value.toLowerCase() == document.getElementsByName("smac"+i)[0].value.toLowerCase() )
			{
				alert(showText(maccannotsame));
				return false;
			}
			if( ip.value == document.getElementsByName("sip"+i)[0].value )
			{
				alert(showText(ipcannotsame));
				return false;
			}
		}
		if( mac.value =="000000000000")
		{
			alert(showText(cannotbe).replace(/####/,showText(macAddr)).replace(/@####/,"00:00:00:00:00:00"));
			return false;
		}
		if( mac.value =="ffffffffffff" || mac.value =="FFFFFFFFFFFF")
		{
			alert(showText(cannotbe).replace(/####/,showText(macAddr)).replace(/@####/,"FF:FF:FF:FF:FF:FF"));
			return false;
		}
		if( ip.value =="0.0.0.0")
		{
			alert(showText(cannotbe).replace(/####/,showText(IPAddr)).replace(/@####/,"0.0.0.0"));
			return false;
		}
		if( ip.value == document.tcpip.ip.value)
		{
			alert(showText(DHCPipandlanipsame));
			return false;
		}
		if (!ipRule( ip, showText(IPAddr), "ip", 1)) //( !checkIpAddr(ip, 'IP address'))
			return false;
		return true;
	}
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
	else
	{
		if(!maskRule(document.tcpip.mask, showText(SubnetMask), 1)) return false;
		return true;
	}
}

function goToWeb() {
	if (document.formSDHCPAdd.SDHCPEnabled.checked==true)
		document.formSDHCPEnabled.SDHCPEnabled.value="ON";
	document.formSDHCPEnabled.submit();
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
	with(document.tcpip) {
		if ( !ipRule( ip, showText(IPAddr), "ip", 1)) return false;
		if(!maskRule(mask, showText(SubnetMask), 1)) return false;
		if ( dhcp.selectedIndex == 1)
		{
			//********** dhcp start ip **********
			if ( dhcpRangeStart.value == ip.value)
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientstartIP))+' '+showText(ShouldBesameasLAN)+'.');
				setFocus(dhcpRangeStart);
				return false;
			}
			if ( !ipRule( dhcpRangeStart, showText(DHCPclientstartIP), "ip", 1)) return false;
			if ( !checkSubnet(ip.value, mask.value, dhcpRangeStart.value))
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientstartIP))+' '+showText(ShouldBesamesubmetCurIP)+'.');
				
				setFocus(dhcpRangeStart);
				return false;
			}
			//********** dhcp end ip **********
			if ( !ipRule( dhcpRangeEnd, showText(DHCPclientEndIP), "ip", 1)) return false;
			if ( !checkSubnet(ip.value, mask.value, dhcpRangeEnd.value))
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(DHCPclientEndIP))+' '+showText(ShouldBesamesubmetCurIP)+'.');
				setFocus(dhcpRangeEnd);
				return false;
			}
			if ( !checkClientRange(dhcpRangeStart.value, dhcpRangeEnd.value) )
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(DHCPIPrange))+' '+showText(EndingGreaterStartIP)+'.');
				setFocus(dhcpRangeStart);
				return false;
			}
//			if (dns1.value=="") dns1.value="0.0.0.0";
//			if (dns2.value=="") dns2.value="0.0.0.0";
//			if ( dhcpGW.value=="" ) dhcpGW.value="0.0.0.0";
//			if ( !ipRule( dns1,showText(PrimaryDNS), "gw", 1)) return false;
//			if ( !ipRule( dns2,showText(SecondaryDNS), "gw", 1)) return false;
//			if ( !ipRule( dhcpGW, showText(gateAddr)+" ", "gw", 1)) return false;
		}
		/*if (checkValue(DomainName.value)==0)
		{
			alert(showText(InvalidSomething).replace(/#####/,showText(domainName))+' '+showText(ShouldBeAlphanumeric)+'.');  
			setFocus(DomainName);
			return false;
		}*/

		if (ip.value!=ip.defaultValue)
		{
			alert(showText(connectNewAddress));
			ipChanged.value = 1;
		}
		
		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}

function DynamicDHCPRange() {
	if ( document.tcpip.ip.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) && document.tcpip.mask.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) && document.tcpip.dhcpRangeStart.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) && document.tcpip.dhcpRangeEnd.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) ) {
		var i = 0;
		var ip = document.tcpip.ip.value.split(".");
		var mask = document.tcpip.mask.value.split(".");
		var start = document.tcpip.dhcpRangeStart.value.split(".");
		var end = document.tcpip.dhcpRangeEnd.value.split(".");
		for ( i = 0; i < 3; i++ ) {
			if ( mask[i] != 0 ) {
				ip[i] = ip[i] & mask[i];
				start[i] = ip[i];
				end[i] = ip[i];
			}
		}
		document.tcpip.dhcpRangeStart.value = start[0] + "." + start[1] + "." + start[2] + "." + start[3];
		document.tcpip.dhcpRangeEnd.value = end[0] + "." + end[1] + "." + end[2] + "." + end[3];
	}
}
</SCRIPT>

</head>


<body>


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(LANHelpInfo)</script></div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<form action="/goform/formTcpipSetup" method="POST" name="tcpip">

		<fieldset name="LANIP_fieldset" id="LANIP_fieldset">
			<legend><script>dw(LANIP)</script></legend>

				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td><div class="itemText-len"><script>dw(IPAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="ip" size="15" maxlength="15" value="<% getInfo("ip-rom"); %>" onChange="if (checkip(1)) DynamicDHCPRange()">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="mask" size="15" maxlength="15" value="<% getInfo("mask-rom"); %>" onChange="if (checkip(2)) DynamicDHCPRange()">
						</div></td>
					</tr>			
				</table>

		</fieldset>

		<br>

		<fieldset name="DHCPServer_fieldset" id="DHCPServer_fieldset">
			<legend><script>dw(DHCPServer)</script></legend>

				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td><div class="itemText-len"><script>dw(DHCPServer)</script></div></td>
						<td><div class="itemText2-len">
							<select size="1" name="dhcp" onChange="dhcpChange(document.tcpip.dhcp.selectedIndex)">
								<script>
									var dhcpmode = <%getVar("dhcp","","");%>;
									if(dhcpmode == 0){
										document.write('<option value="0" selected>'+showText(disable)+'</option>');
										document.write('<option value="2" >'+showText(enable)+'</option>');}
									else{
										document.write('<option value="0">'+showText(disable)+'</option>');
										document.write('<option value="2" selected >'+showText(enable)+'</option>');}
								</script>
							</select>
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(LeaseTime)</script></div></td>
						<td><div class="itemText2-len">
							<select name="leaseTime">
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
							<input type="text" name="dhcpRangeStart" size="15" maxlength="15" value="<% getInfo("dhcpRangeStart"); %>">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(endIP)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="dhcpRangeEnd" size="15" maxlength="15" value="<% getInfo("dhcpRangeEnd"); %>">
						</div></td>
					</tr>
					<!--<tr>
						<td><div class="itemText-len"><script>dw(domainName)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="DomainName" size="20" maxlength="30" value="<% getInfo("DomainName"); %>">
						</div></td>
					</tr>-->
					<!--tr>
					<td width="200" height="20" valign="middle" align="right" class="style14"><script>dw(dnsAddr1)</script>:&nbsp;</td>
					<td width="400" height="20" valign="middle" align="left" class="style1">
						<input type="text" name="dns1" size="15" maxlength="15" value="< getInfo("wan-dns1"); >" class="text">
					</td>
					</tr>
					<tr>
					<td width="200" height="20" valign="middle" align="right" class="style14"><script>dw(dnsAddr2)</script>:&nbsp;</td>
					<td width="400" height="20" valign="middle" align="left" class="style1">
						<input type="text" name="dns2" size="15" maxlength="15" value="< getInfo("wan-dns2");>" class="text">
					</td>
					</tr -->

					<!--
					<tr>
						<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="dhcpGW" size="15" maxlength="15" value="<% getInfo("dhcpGW"); %>">
						</div></td>
					</tr>
					-->
					<input type="hidden" value="/lan.asp" name="submit-url">
					<input type="hidden" value="" name="ipChanged">
				</table>

		</fieldset>
		<input type="hidden" value="1" name="iqsetupclose">
		<input type="hidden" value="" name="isApply">
	</form>
		
	<br>

	
		
	<fieldset name="StaticDHCPLeasesTable_fieldset" id="StaticDHCPLeasesTable_fieldset">
		<legend><script>dw(StaticDHCPLeasesTable)</script></legend>
		
			<form action="/goform/formSDHCP" method="POST" name="formSDHCPEnabled">
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="Add" name="addSDHCPMac">
					<input type="hidden" value="" name="SDHCPEnabled">
					<input type="hidden" value="/lan.asp" name="submit-url">
			</form>

			

		<form action="/goform/formSDHCP" method="POST" name="formSDHCPAdd">

					<table width="600" border="0" cellpadding="2" cellspacing="2" align="center">
						<tr>
							<td width="600" height="20" valign="middle" align="left" class="">
								<input type="checkbox" name="SDHCPEnabled" value="ON" <%getVar("SDHCPEnabled","1","checked");%> onclick="goToWeb()">&nbsp;
								<span class="itemText"><script>dw(EnableStaticDHCPLeases)</script></span>
							</td>
						</tr>
					</table>
					<br>
			
					<table width="600" border="0" cellpadding="0" cellspacing="0" align="center">
						<tr>
							<td width="40%" height="20" valign="middle" align="center">
								<span class="itemText"><script>dw(macAddr)</script></span><br><input type="text" name="mac" size="15" maxlength="12">
							</td>
							<td width="40%" height="20" valign="middle" align="center">
								<span class="itemText"><script>dw(IPAddr)</script></span><br><input type="text" name="ip" size="16" maxlength="16">
							</td>
							<td width="20%" height="20" valign="middle" align="center">
								<script>
									document.write('<input type="submit" value="'+showText(add)+'" name="addSDHCPMac" onClick="return addClick()" class="ui-button-nolimit">&nbsp;')
								</script>

								<input type="hidden" value="/lan.asp" name="submit-url">

							</td>
						</tr>
					</table>

		</form>

		<br>
	
		<form action="/goform/formSDHCP" method="POST" name="formSDHCPDel">
				<input type="hidden" value="1" name="iqsetupclose">
						<table width="600" border="0" align="center">
							<tr>
								<td style="text-align:center;">
									<script>
										document.write('<span class="itemText">'+showText(StaticDHCPLeasesonly16)+'</span>')
									</script>
								</td>
							</tr>
						</table>

						<br>
						<table width="600" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
							<tr>
								<td width="7%" align="center" class="dnsTableHe"><script>dw(NO)</script>.</td>
								<td width="45%" align="center" class="dnsTableHe"><script>dw(macAddr)</script></td>
								<td width="35%" align="center" class="dnsTableHe"><script>dw(IPAddr)</script></td>
								<td width="13%" align="center" class="dnsTableHe"><script>dw(select)</script></td>
							</tr>
						</table>
						<table width="600" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
						<% SDHCPList(); %>
						</table>
							
						<br>

						<table width="600" border="0" cellpadding="0" cellspacing="0" align="center">
							<tr>
								<td align="right">
									<script>
										document.write('<input type="submit" value="'+showText(deleteSelected)+'" id="deleteSelDhcpMac" name="deleteSelDhcpMac" onClick="" class="ui-button-nolimit">&nbsp;');
										document.write('<input type="submit" value="'+showText(deleteAll)+'" id="deleteAllDhcpMac" name="deleteAllDhcpMac" onClick="return deleteAllClick()" class="ui-button-nolimit">&nbsp;');
									</script>
								</td>
							</tr>
							<input type="hidden" value="/lan.asp" name="submit-url">
					</table>

		</form>
  </fieldset>

		<script>
		var sDHCPNum = <%getVar("SDHCPNum","","");%>;
		if ( sDHCPNum == 0 ) {
			document.formSDHCPDel.deleteSelDhcpMac.disabled = true;
			document.formSDHCPDel.deleteAllDhcpMac.disabled = true;
			document.getElementById("deleteSelDhcpMac").style.color=disableButtonColor;
			document.getElementById("deleteAllDhcpMac").style.color=disableButtonColor;
		}
		if ( sDHCPNum >= 32 ) {
			document.formSDHCPAdd.addSDHCPMac.disabled = true;
			document.formSDHCPAdd.addSDHCPMac.style.color=disableButtonColor;
		}
	 </script>

		<br>

		<table align="center" border="0" Cellspacing="0" width="700">
			<tr>
				<td style="text-align:center; padding-top:30px;">
					<script>

						document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
						var show_reboot = "<% getInfo("show_reboot"); %>"
						if(show_reboot == "1")
							document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
					</script>
				</td>
			</tr>
		</table>

<script>
	dhcpChange(document.tcpip.dhcp.selectedIndex);
	$("#help-box").hide();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
