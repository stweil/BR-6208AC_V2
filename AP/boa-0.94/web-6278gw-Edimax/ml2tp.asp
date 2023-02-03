<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript">
var wiz_gwManu = "<% getInfo("wiz_gwManu"); %>";
var wiz_wispManu = "<% getInfo("wiz_wispManu"); %>";
</script>
<style type="text/css">
.tablewidth0 {width:100%;}

.leftwidth0 {width:40%;}
.leftwidth4 {width:35%;}

.iptabmwidth0 {width:22%;}
.iptabmwidth4 {width:22%;}

.rightwidth0 {width:38%;}
.rightwidth4 {width:43%;}
</style>
<script type="text/javascript">
	var L2TPIpMode = <%getVar("L2TPIpMode","","");%>;

function copyto(){
	document.tcpip.macAddr.value = document.tcpip.macAddrValue.value
}

function checkmac(input){
	var x = input
	if (!x.value.match(/^[A-Fa-f0-9]{12}$/) || x.value == "ffffffffffff" || x.value == "FFFFFFFFFFFF"){
		x.value = "000000000000"
		alert(showText(InvalidSomething).replace(/#####/, showText(MACAddress)));
		return false
	}
}

function check(input){
	
	var x = input
	if (x.value != ""){
		if (x.value.search("\'") != -1 || x.value.search("\"") != -1 || x.value.search("\<") != -1 || x.value.search("\>") != -1){
			alert("Invalid value")
			x.value = x.defaultValue
			x.focus()
			return false
		}
		else
			return true
	}
	else
		return true
}

/*function checkdns(input){

	var x = input
	if (x.value != "" && !x.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) || x.value == "0.0.0.0" || x.value == "255.255.255.255"){
		alert(showText(InvalidSomething).replace(/#####/, showText(dnsAddr)));
		x.value = x.defaultValue
		x.focus()
		return false
	}
	else
		return true
}*/

function checkdns(index)
{
  with(document.tcpip) 
	{
		switch(index)
		{
      case 0:
				if ( !ipRule( dns1, showText(dnsAddr1), "ip", 1))
					return false;
				else 
					return true;
			case 1:
				if ( !ipRule( dns2, showText(dnsAddr2), "ip", 1))
					return false;
				else 
					return true;
			default: 
			alert("error!");
			return false;
			break;
	 }
 }
}


function dns_autoDNSClicked(){

	if(document.getElementsByName("DNSMode")[1].checked){
		document.getElementsByName("dns1")[0].disabled=false
		document.getElementsByName("dns2")[0].disabled=false
	}
	else{
		document.getElementsByName("dns1")[0].disabled=true
		document.getElementsByName("dns2")[0].disabled=true
	}
}

function autoIpClicked(){

	if(document.tcpip.L2TPIpMode[0].checked==true){
		document.tcpip.dynIPHostName.disabled=false;
		document.tcpip.L2TPIPAddr.disabled=true;
		document.tcpip.L2TPMaskAddr.disabled=true;
		document.tcpip.L2TPDefGateway.disabled = true;
		document.tcpip.DNSMode[0].disabled = false;
	}
	else{
		document.tcpip.dynIPHostName.disabled=true;
		document.tcpip.L2TPIPAddr.disabled=false;
		document.tcpip.L2TPMaskAddr.disabled=false;
		document.tcpip.L2TPDefGateway.disabled = false;
		document.tcpip.DNSMode[0].checked = false;
		document.tcpip.DNSMode[0].disabled = true;
		document.tcpip.DNSMode[1].checked = true;
		dns_autoDNSClicked();
	}
}

function goBack(){
	window.location.assign("mstart.asp");
}


function submit_now()
{
	$.ajax({
	  url: 'goform/setWAN',
	  type: 'POST',
	  data: $('#tcpip').serialize(),
	  async: true,
	  dataType:'text',

	  success: function () {
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  },
	  error: function(){
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  }
	});
}

function goNext(){

	var str1 = document.tcpip.dynIPHostName.value
	if (str1.search('"') != -1){
		alert(showText(wandipAlert3))
		document.tcpip.dynIPHostName.focus()
		return false
	}

	if (document.tcpip.macAddr.value == "")
		document.tcpip.macAddr.value="000000000000"

	if(document.tcpip.L2TPIpMode[1].checked==true){
		if (!checkip(0)) return false;
		if (!checkip(1)) return false;
		if (!checkip(2)) return false;
		if (!checkip(3)) return false;
	}

	var dns1 = document.tcpip.dns1.value;
	var dns2 = document.tcpip.dns2.value;

	if(document.getElementsByName("DNSMode")[1].checked)
	{
		if (!checkdns(0)) return false;

		if (( dns1 == "" ) || ( dns1 == "0.0.0.0") || ( dns1 == "255.255.255.255")) 
		{
			alert(showText(InvalidSomething).replace(/#####/, showText(dnsAddr1)));
			return false;
		}

		if (dns2!="")
		{
			if ((dns2 == "" ) || ( dns2 == "0.0.0.0") || ( dns2 == "255.255.255.255" )) 
			{
				alert(showText(InvalidSomething).replace(/#####/, showText(dnsAddr2)));
				return false;
			}
			if (!checkdns(1))	return false;
		}
	}
	
	document.tcpip.wiz_gwManu.value = wiz_gwManu;
	document.tcpip.wiz_wispManu.value = wiz_wispManu;
	submit_now();
}

function checkip(index)
{
  with(document.tcpip) {
		switch(index)
		{
      case 0:
				if ( !ipRule( L2TPIPAddr, showText(IPAddress), "ip", 1)) return false;
				else return true;
			break;

			case 1:
				if ( !maskRule(L2TPMaskAddr, showText(SubnetMask), 1)) return false;
				else return true;
			break;

   		case 2:
				if ( !ipRule( L2TPDefGateway, showText(defaultGateway), "gw", 1)) return false;
				else return true;
			break;

	 		case 3:
				if ( !ipRule( L2TPGateway, showText(l2tpdefaultGateway), "gw", 1)) return false;
				else return true;
			break;
	
			default: 
			alert("error!");
			return false;
			break;
	 }
 }
}
function setTableWidth()
{

	if(stype>13) stype=13;
	document.all.tablewidth.className="tablewidth0";

	if(stype==4)
	{
		document.all.leftwidth.className="leftwidth"+stype;
		document.all.iptabmwidth.className="iptabmwidt"+stype;
		document.all.rightwidth.className="rightwidth"+stype;
	}
	else
	{
		document.all.leftwidth.className="leftwidth0";
		document.all.iptabmwidth.className="iptabmwidt0";
		document.all.rightwidth.className="rightwidth0";
	}
}
</script>


</head>
<body onLoad="setTableWidth();">
<form action="/goform/setWAN" method="POST" name="tcpip" id="tcpip">
<input type="hidden" value="" name="wiz_gwManu">
<input type="hidden" value="" name="wiz_wispManu">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(routermode)</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:383px;width:790px" align="center">
		<tr>
			<td align="center">

			<table border="0" id="tablewidth" class="tablewidth0" align="center">
				<tr>
					<td colspan="3" align="center" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;font-weight:bold;height:0px;">
						<script>dw(L2TP)</script>
					</td>
				</tr>
			</table>

			<table border="0" id="tablewidth" class="tablewidth0" align="center">
				<tr>
					<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
						<span style="font-weight:bold;"></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						<script>dw(IPAddress)</script>&nbsp;&nbsp;
					</td>

					<td colspan="2" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" >
						<input type="radio" value="0" name="L2TPIpMode" <%getVar("L2TPIpMode","0","checked");%> onClick="autoIpClicked()"><script>dw(ObtainAutomatically)</script>
						<input type="radio" value="1" name="L2TPIpMode" <%getVar("L2TPIpMode","1","checked");%> onClick="autoIpClicked()"><script>dw(manualConfig)</script>
					</td>
				</tr>

				<tr>
					<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" id="leftwidth" class="leftwidth0"><script>dw(HostName)</script>&nbsp;&nbsp;</td>
					<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" id="iptabmwidth" class="iptabmwidth0">
						<input type="text" name="dynIPHostName" value="<%getInfo("dynIPHostName");%>" size="15" maxlength="30" class="Arial_16" onchange="check(this)">
					</td>
					<td id="rightwidth" class="rightwidth0"></td>
				</tr>

				<tr>
					<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(MACAddress)</script>&nbsp;&nbsp;</td>
						<input type="hidden" name="macAddrValue" value="<%getInfo("cloneMac");%>">
					<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
						<input type="text" name="macAddr" value="<%getInfo("wanMac");%>" size="15" maxlength="12" class="Arial_16" onchange="checkmac(this)">
					</td>
					<td>
						<script>document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto();" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">');</script>
					</td>
				</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(IPAddress)</script>&nbsp;&nbsp;
			</td>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" >
			<input type="text" class="Arial_16" name="L2TPIPAddr" size="15" maxlength="15" value="<% getInfo("L2TPIPAddr"); %>" onchange="checkip(0)">
			</td>
			<td></td>
			</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(SubnetMask)</script>&nbsp;&nbsp;
			</td>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<input type="text" class="Arial_16" name="L2TPMaskAddr" size="15" maxlength="15" value="<% getInfo("L2TPMaskAddr"); %>" onchange="checkip(1)">
			</td>
			<td></td>
			</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(defaultGateway)</script>&nbsp;&nbsp;
			</td>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<input type="text" class="Arial_16" name="L2TPDefGateway" size="15" maxlength="15" value="<% getInfo("L2TPDefGateway"); %>" onchange="checkip(2)">
			</td>
			<td></td>
			</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(username)</script>&nbsp;&nbsp;
			</td>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<input type="text" class="Arial_16" name="L2TPUserName" size="15" maxlength="64" value="<% getInfo("L2TPUserName"); %>" onchange="check(this)">
			</td>
			<td></td>
			</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(password)</script>&nbsp;&nbsp;
			</td>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<input type="text" class="Arial_16" name="L2TPPassword" size="15" maxlength="30" value="<% getInfo("L2TPPassword"); %>" onchange="check(this)">
			</td>
			<td></td>
			</tr>

			<tr>
				<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(L2TPGateway)</script>&nbsp;&nbsp;</td>
				<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
					<input type="text" class="Arial_16" name="L2TPGateway" size="15" maxlength="30" value="<%getInfo("L2TPGateway");%>" onchange="checkip(3)">
				</td>
			<td></td>
			</tr>

			<tr>
			<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
			<script>dw(dnsAddr)</script>&nbsp;&nbsp;
			</td>
			<td colspan="2" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" >
			<input type="radio" value="0" name="DNSMode" <%getVar("wanDNS","0","checked");%> onClick="dns_autoDNSClicked();"><script>dw(ObtainAutomatically)</script>
			<input type="radio" value="1" name="DNSMode" <%getVar("wanDNS","1","checked");%> onClick="dns_autoDNSClicked();"><script>dw(manualConfig)</script>
			</td>
			</tr>

			<tr>
			<td align="right" class="text" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(dnsAddr1)</script>&nbsp;&nbsp;</td>
			<td><input type="text" class="Arial_16" name="dns1" size="15" maxlength="15" value="<% getInfo("wan-dns1"); %>" onchange="checkdns(0)"></td>
			<td></td>
			</tr>
			<tr>
			<td align="right" class="text" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(dnsAddr2)</script>&nbsp;&nbsp;</td>
			<td><input type="text" class="Arial_16" name="dns2" size="15" maxlength="15" value="<% getInfo("wan-dns2"); %>" onchange="checkdns(1)"></td>
			<td></td>
			</tr>
		</table>
	</td>
	</tr>
	</table>

	<table border="0" style="height:28px;width:780px" align="center">
	<tr>
	<td align="right" style="width:390px">
	<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
	</td>
	<td align="left" style="width:390px">
	<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
	</td>
	</tr>
	</table>
	<table border="0" style="height:60px;width:780px" align="center">
		<tr>
		<td>
		</td>
		</tr>
	</table>
</td>
</tr>
</table>
</div>
<script>
autoIpClicked()
dns_autoDNSClicked()
</script>
<input type="hidden" value="" name="DNSMode">
<input type="hidden" value="6" name="wanMode">
</form>
</body>
</html>
