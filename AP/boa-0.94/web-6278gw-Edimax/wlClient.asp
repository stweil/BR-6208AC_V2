<html>
  <head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
    <link rel="stylesheet" href="/set.css">
    <script type="text/javascript" src="/file/javascript.js"></script>
    <script type="text/javascript" src="/file/multilanguage.var"></script>
    <title></title>
  </head>
<body>
<blockquote>
 <br>
	<table border='0' width="650" align="center" cellpadding="0" cellspacing="0">
	 	<tr><td align="center">
		<span class="tiltleText"><script>dw(ActiveWirelessClientTable)</script></span>
		</td></tr>
	</table>
	<br>
	<br>
	<!--<table border='0' width="650" align="center" cellpadding="0" cellspacing="0">
		<tr><td align="left">
		<span class="itemText"><script>//dw(WirelessClientTableInfo)</script></span>
	 </td></tr>
	</table>
	<br>
	<br>-->

  <form action=/goform/formWirelessTbl method=POST name="formWirelessTbl">

		<table border='0' width="650px" align="center" cellpadding="2" cellspacing="2">
			<tr class="titletableText">
			<td align="center" width="40%"><script>dw(macAddr)</script></td>
			<td align="center" width="20%"><script>dw(DataRate)</script></td>
			<td align="center" width="20%"><script>dw(ChannelWidth)</script></td>
			<td align="center" width="20%"><script>dw(PowerSaving)</script></td>
			</tr>
			<%wirelessClientList(); %>
		</table>

		<input type="hidden" value="/wlClient.asp" name="submit-url">
		<br>
		<br>

		<table width="650" align="center">
			<tr>
				<td align="center">
					<script>
						document.write('<input type="submit" value="' + showText(refresh)+'" name="refresh" class="ui-button-nolimit">&nbsp');
						document.write('<input type="button" value="' + showText(Close) + '" name="close" onClick="javascript: window.close();" class="ui-button-nolimit">');
					</script>
				</td>
			</tr>
		</table>
  
	</form>
</blockquote>
</body>
</html>
