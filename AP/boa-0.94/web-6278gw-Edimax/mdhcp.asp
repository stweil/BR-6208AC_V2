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
var wizMode = <%getVar("wizmode","","");%>;
var wiz_gwManu = "<% getInfo("wiz_gwManu"); %>"
var wiz_wispManu = "<% getInfo("wiz_wispManu"); %>"

function copyto()
{
	document.tcpip.macAddr.value = document.tcpip.macAddrValue.value
}

function checkmac(input) 
{
	var x = input
	if (!x.value.match(/^[A-Fa-f0-9]{12}$/) || x.value == "ffffffffffff" || x.value == "FFFFFFFFFFFF")
		{
			x.value = "000000000000"
			alert(showText(InvalidSomething).replace(/#####/, showText(MACAddress)));
			return false
		}
}

function check(input)
{
	var x = input
	if (x.value != "")
		{
			if (x.value.search("\'") != -1 || x.value.search("\"") != -1 || x.value.search("\<") != -1 || x.value.search("\>") != -1)
				{
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

/*function checkdns(input) 
{
	var x = input
	if (x.value != "" && !x.value.match(/\b(?:\d{1,3}\.){3}\d{1,3}\b/) || x.value == "0.0.0.0" || x.value == "255.255.255.255") 
		{
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

function dns_autoDNSClicked()
{
	if(document.getElementsByName("DNSMode")[1].checked)
		{
			document.getElementsByName("dns1")[0].disabled=false
			document.getElementsByName("dns2")[0].disabled=false
		}
	else
		{
			document.getElementsByName("dns1")[0].disabled=true
			document.getElementsByName("dns2")[0].disabled=true
		}
}

function goBack()
{
	window.location.assign("/mstart.asp");
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

function goNext()
{
	var str1 = document.tcpip.dynIPHostName.value
	if (str1.search('"') != -1) {
		alert(showText(wandipAlert3))
		document.tcpip.dynIPHostName.focus()
		return false
	}
	if (document.tcpip.macAddr.value == "")
		document.tcpip.macAddr.value="000000000000"

	var dns1 = document.tcpip.dns1.value;
	var dns2 = document.tcpip.dns2.value;

	if(document.getElementsByName("DNSMode")[1].checked == true)
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
	document.tcpip.wiz_gwManu.value = wiz_gwManu ;
	document.tcpip.wiz_wispManu.value = wiz_wispManu;
	submit_now();
}

function setTableWidth()
{
	document.getElementById('DhcpTable').className="DhcpTable"+stype;
}
</script>
<style type="text/css">
.DhcpTable0 {width:70%;}
.DhcpTable1 {width:70%;}
.DhcpTable2 {width:88%;}
.DhcpTable3 {width:78%;}
.DhcpTable4 {width:89%;}
.DhcpTable5 {width:70%;}
.DhcpTable6 {width:85%;}
.DhcpTable7 {width:85%;}
.DhcpTable8 {width:80%;}
.DhcpTable9 {width:70%;}
.DhcpTable10 {width:84%;}
.DhcpTable11 {width:85%;}
.DhcpTable12 {width:85%;}
.DhcpTable13 {width:70%;}

</style>
</head>
<body onLoad="setTableWidth();" >
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
				<script>
					if(wizMode==0)
						dw(routermode);
					else if(wizMode==1)
						dw(apmode);
					else if(wizMode==2)
						dw(repeatermode);
					else if(wizMode==3)
						dw(APcl);
					else if(wizMode==4)
						dw(wisp);
					else
						dw(repeatermode);
				</script>
			</td>
		</tr>
	</table>

	<table border="0" align="center" style="height:350px;width:650px;">
		<tr>
			<td align="center">

				<table border="0" align="center" id="DhcpTable" class="DhcpTable0">
					<tr>
						<td colspan="3" align="center" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;font-weight:bold;height:60px;"><script>dw(DHCP)</script></td>
					</tr>
			
					<tr>
						<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(HostName)</script>&nbsp;&nbsp;</td>
						<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
							<input type="text" name="dynIPHostName" value="<% getInfo("dynIPHostName"); %>" size="15" maxlength="30" class="Arial_16" onchange="check(this)">
						</td>
						<td></td>
					</tr>

					<script>
					if( wizMode != 4)
					{
						document.write('<tr>');
						document.write('<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">'+showText(MACAddress)+'&nbsp;&nbsp;</td>');
						document.write('<input type="hidden" value="<% getInfo("cloneMac"); %>" name="macAddrValue">');
						document.write('<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">');
						document.write('<input type="text" name="macAddr" value="<% getInfo("wanMac"); %>" size="15" maxlength="12" class="Arial_16" onchange="checkmac(this)"></td>');
						document.write('<td><input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto();" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"></td>');
						document.write('</tr>');
					}
					else
					{
						document.write('<input type=hidden value="" name="macAddr">');
					}
					</script>

					<tr>
						<td align="right" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(dnsAddr)</script>&nbsp;&nbsp;</td>
						<td colspan="2" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
							<input type="radio" value="0" name="DNSMode" <%getVar("wanDNS","0","checked");%> onClick="dns_autoDNSClicked();"><script>dw(ObtainAutomatically)</script><br>
							<input type="radio" value="1" name="DNSMode" <%getVar("wanDNS","1","checked");%> onClick="dns_autoDNSClicked();"><script>dw(manualConfig)</script>
						</td>
						<td></td>
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
		
	<table border="0" style="height:33px;width:780px" align="center">
		<tr><td></td></tr>
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
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>
<script>
dns_autoDNSClicked()
</script>
<input type="hidden" value="" name="DNSMode">
<input type="hidden" value="0" name="wanMode" >
</form>
</body>
</html>
