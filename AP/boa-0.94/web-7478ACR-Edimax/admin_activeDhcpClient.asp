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

function init()
{
 $("#help-box").hide();
}
</SCRIPT>
</head>
<body onLoad="init();">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr>
						<td><div class="help-text" id="help-info1"><script>dw(ActDHCPClientHelpInfo)</script>
						</div></td>
					</tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>




			<fieldset name="ActDHCPClient_fieldset" id="ActDHCPClient_fieldset" >
				<legend><script>dw(ActiveDHCPClient)</script></legend>

				<br>
				<form action=/goform/formReflashClientTbl method=POST name="formClientTbl">

					<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
		        	<tr align="center" class="dnsTableHe">
								<td width="30%"><script>dw(IPAddr)</script></td>
								<td width="30%"><script>dw(macAddr)</script></td>
								<td width="40%"><script>dw(timeExpired)</script>&nbsp;(<script>dw(sec)</script>)</td>
							</tr>
					</table>
					<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
						<% dhcpClientList(); %>
					</table>
					<br>

					<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">
						<tr>
							<td style="text-align:center; padding-top:30px;">
								<script>document.write('<input type="submit" value="'+showText(refresh)+'" name="refresh" onclick="" class="ui-button-nolimit">')</script>
							</td>
						</tr>
					</table>

					<input type="hidden" value="/admin_activeDhcpClient.asp" name="submit-url">
				</form>
				
			</fieldset>




<script>

		$("#help-box").hide();


</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		

</html>
