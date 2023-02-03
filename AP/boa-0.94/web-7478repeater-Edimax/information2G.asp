<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<link rel="stylesheet" href="/set.css">
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<script>document.write('<title>'+showText(Status)+'</title>')</script>
<script>
var useCrossBand = "<%getInfo("useCrossBand");%>";
var wpaEncrypt5g =	<%getiNICInfo("wpaEncrypt");%>; 
var wpaCipher5g  = wpaEncrypt5g[0];				
var encrypt5g = <%getiNICVar("encrypt","","");%>;				
var wpaEncrypt= <%getInfo("wpaEncrypt");%>;
var wpaCipher = wpaEncrypt[0];
var encrypt = <%getVar("encrypt","","");%>;
var clientmodeTbl = new Array(showText(disable),showText(WEP),showText(WPA),showText(WPA2),showText(WPA_RADIUS),showText(WPA2_RADIUS));
var wpaCipherSuite = new Array("TKIP","AES");
var wizMode=<%getVar("wizmode","","");%>;
if(useCrossBand == 1)
{
	var showSSID = "<%getiNICInfo("repeaterSSID");%>";
	var showChannel = "<% getInfo("inic_channel_drv"); %>";
	var showRMac = "<% getInfo("inicaddr"); %>";
}	
else
{
	var showSSID = "<%getInfo("repeaterSSID");%>";
	var showChannel = "<% getInfo("channel_drv"); %>";
	var showRMac = "<% getInfo("wladdr"); %>";
}
</script>
</head>
<body>
<br><br>
<blockquote>
<form action=/goform/formToken method=POST name="token">
<input type="hidden" value="/index.asp" id="submit-url" name="submit-url">
<input type="hidden" value="1" id="Gband" name="Gband">
<a href="#" onclick="document.token.submit()"><p align=center><b>
<font size=4 style="font-weight:bold;text-decoration:underline;color:#0099ff">Go to Web Control Panel</font>
</b></p></a> 
</form>
<br>
<div align="left">
	<form method="post" action="goform/formUpload" enctype="multipart/form-data" name="upload">
		<table border="1" width="550" cellspacing="0" cellpadding="0" align=center>
			<tr>
				<td width=100% colspan="3" class="titletableText"><font size=2><b>&nbsp;<script>dw(System);</script></b></font></td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(time);</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2>&nbsp;<% getInfo("uptime"); %></font></td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(HardwareVersion);</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2>&nbsp;V1.0A</font></td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2>Firmware Version</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2>&nbsp;<% getInfo("fwVersion"); %></font>
				</td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(mode);</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2>
				<script>
					if( wizMode==3 ) document.write('&nbsp;'+showText(APcl)); 
					else document.write('&nbsp;'+showText(repeatermode));
				</script>
				</font></td>
			</tr>
			<tr>
				<td width=100% colspan="3" class="titletableText"><font size=2><b>&nbsp;Wireless Configuration</b></font></td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(ESSID);</script>&nbsp;</td>
				<td width=70% colspan="2">&nbsp;
					<script>
						document.write('<font size=2>'+showSSID+'</font>');
					</script>
				</td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(ChannelNumber);</script>&nbsp;</font></td>
				<td width=70%>&nbsp;
					<script>
						document.write('<font size=2>'+showChannel+'</font>');
					</script>
				</td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(Security);</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2>&nbsp;
				<script>
					if(useCrossBand == "1")
					{
						tkipuse = wpaCipher5g;
						encryptMethod = encrypt5g;
					}
					else
					{
						tkipuse = wpaCipher;
						encryptMethod = encrypt;
					}
					for ( i=0; i<6; i++)
					{
						if ( i == encryptMethod)
						{
							document.write(clientmodeTbl[i]);
							if ( i==2 || i==3 )
							{
								if(tkipuse != 0) document.write('('+wpaCipherSuite[0]+')');
								else document.write('('+wpaCipherSuite[1]+')');															
							}
						}
					}
				</script>
				</font></td>
			</tr>		
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(BSSID);</script>&nbsp;</font></td>
				<td width=70% colspan="2">&nbsp;
					<script>
						document.write('<font size=2>'+showRMac+'</font>');
					</script>
				</td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(Status);</script>&nbsp;</font></td>
				<td width=70% colspan="2">
				<script>
					var connectStatus2G = "<% getInfo("connectStatus2G"); %>";
					if(connectStatus2G=="disable")
					{
						document.write('<font size=2 style=font-weight:bold;color:#ff0000>&nbsp;Wi-Fi disabled');
					}
					else
					{
						document.write('<font size=2 style=font-weight:bold;color:#ff0000>&nbsp;'+showText(Disconnect)+'</font>');
					}
				</script>
				</td>
			</tr>
			<tr class="table2">
				<td width=30% align=right><font size=2><script>dw(SignalStrength);</script>&nbsp;</font></td>
				<td width=70% colspan="2"><font size=2><div id="width2" style="float:left; width:0%"></div><div style="float:left; ">&nbsp;0%</div></font></td>
			<script>
				document.getElementById("width2").style.width = 0*0.75+'%';
			</script>
			</tr>
		</table>
	</form>
	<br>
	<table border="0" cellspacing="0" cellpadding="0" align="center" style="height:0px;width:520px">
		<tr><td><font size="4" style="color:#ff0000"><script>dw(disconn_1);</script></font></td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td><font size="4"><script>dw(disconn_2);</script></font></td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td><font size="4"><script>dw(disconn_3);</script></font></td></tr>
	</table>
</div>
<br>

</blockquote>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
</body>
</html>
