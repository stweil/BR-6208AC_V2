
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/file/autowan.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
//var initialDhcp;
var wizMode = <%getVar("wizmode","","");%>;

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

	if (document.tcpip.ipmode5g[1].checked==true) 
	{
		if (!checkip(0)) return false;
		if (!checkip(1)) return false;
		if (!checkip(2)) return false;
		if (!checkip(3)) return false;
	}

	with(document.tcpip) {
		//if ( !ipRule( ip, showText(IPAddr), "ip", 1)) return false;
		//if(!maskRule(mask, showText(SubnetMask), 1)) return false;

		if (ip5g.value!=ip5g.defaultValue)
		{
			alert(showText(connectNewAddress));
			ipChanged.value = 1;
		}

		if(rebootFlag)
		 isApply.value = "ok";

	  submit();	
	}
}
function checkip(index)
{
	if(index == 1)
	{
		if (document.tcpip.ip5g.value == "0.0.0.0")
		{
			alert(showText(cannotbe).replace(/####/,showText(IPAddr)).replace(/@####/,"0.0.0.0"));
			document.tcpip.ip5g.value = "";
			return false;
		}
		if ( !ipRule( document.tcpip.ip5g, showText(IPAddr), "ip", 1)) return false;
		return true;
	}
	else if(index == 2)
	{
		if(!maskRule(document.tcpip.mask5g, showText(SubnetMask), 1)) return false;
		return true;
	}
	else if(index == 3)
	{
		if ( !ipRule( document.tcpip.gateway5g, showText(gateAddr), "gw", 1)) return false;
		else if ( !ipRuleAND( document.tcpip.ip5g, document.tcpip.mask5g, document.tcpip.gateway5g, showText(defaultGatewayWarning), "gw", 1)) return false;
		else return true;
	}
	else
	{
		if ( !ipRule( document.tcpip.dns2, showText(DNS), "ip", 1)) return false;
		else return true;
	}
}

function autoIp_Clicked() {

	if (document.tcpip.ipmode5g[0].checked==true) {
		document.tcpip.dnsMode.value="dnsAuto";
		document.tcpip.ip5g.disabled = true;
		document.tcpip.mask5g.disabled = true;
		document.tcpip.gateway5g.disabled = true;
		document.tcpip.dns2.disabled = true;
		//document.getElementById("ipsetting").style.display = "none"; 
		//document.getElementById("nosetting").style.display = "block"; 
	}
	else {
		//document.getElementById("ipsetting").style.display = "block"; 
		//document.getElementById("nosetting").style.display = "none"; 
		document.tcpip.dnsMode.value="dnsManual";
		document.tcpip.ip5g.disabled = false;
		document.tcpip.mask5g.disabled = false;
		document.tcpip.gateway5g.disabled = false;
		document.tcpip.dns2.disabled = false;
	}
}
</SCRIPT>

</head>


<body>


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1">
								<script>dw(LANHelpInfoX)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<form action=/goform/saveAndReboot method=POST name="tcpip">
		<input type="hidden" value="1" name="iqsetupclose">
		<input type="hidden" value="0" name="dhcp">
		<input type="hidden" name="clIpDisabled" value="0">
		<input type=hidden value="/lan_ap5g.asp" name="submit-url">
		<input type=hidden value="" name="ipChanged">
		<input type="hidden" value="OK" name="goToRep">
		<input type="hidden" value="dnsAuto" name="dnsMode">

		<fieldset name="LANIP_fieldset" id="LANIP_fieldset">
			<legend><script>dw(LANIP)</script></legend>
				<br>
				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td align="right" width="35%">
						<input type="radio" value="1" name="ipmode5g" <%getVar("dhcpswitch-5g","1","checked")%> onClick="autoIp_Clicked()">
						</td>
						<td align="left" class="choose-itemText">
						<script>dw(ObtainIPauto)</script>
						</td>
					</tr>
					<tr>
						<td align="right" width="35%">
						<input type="radio" value="0" name="ipmode5g" <%getVar("dhcpswitch-5g","0","checked")%> onClick="autoIp_Clicked()">
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
							<input type="text" name="ip5g" size="15" maxlength="15" value="<% getInfo("ip-rom-5g"); %>" onChange="checkip(1)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="mask5g" size="15" maxlength="15" value="<% getInfo("mask-rom-5g"); %>" onChange="checkip(2)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="gateway5g" size="15" maxlength="15" value="<% getInfo("gateway-rom-5g"); %>" onChange="checkip(3)">
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
						<td><div class="itemText2-len">
							<input type="text" name="dns2" size="15" maxlength="15" value="<% getInfo("wan-dns2"); %>" onChange="checkip(4)">
						</div></td>
					</tr>
				</table>
				<br>
		</fieldset>


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
