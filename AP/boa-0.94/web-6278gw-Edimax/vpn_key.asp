<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<title>EDIMAX Technology</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<% language=javascript %>
<SCRIPT>
function checkAuthMode()
{
	if( document.VPNSetupKey.AuthMode.value == 0 ) //TLS
	{
		document.getElementById('VPN_TLS1').style.display="block";
		if (document.VPNSetupKey.AuthOnlyUserName.value == "no")
		{
			document.getElementById('VPN_TLS2').style.display="block";
		}
		else
		{
			document.getElementById('VPN_TLS2').style.display="none";
		}
		document.getElementById('VPN_Static_Key').style.display="none";
	}
	else //Static Key
	{
		document.getElementById('VPN_TLS1').style.display="none";
		document.getElementById('VPN_TLS2').style.display="none";
		document.getElementById('VPN_Static_Key').style.display="block";
	}
}
function checkKEYset()
{
	with(document.VPNSetupKey)
	{
		if( AuthMode.value == 0 )
		{
			window.opener.document.VPN_Ca_Crt_form.VPN_Ca_Crt.value         = edit_ca_crt.value;
			window.opener.document.VPN_DH_Pem_form.VPN_DH_Pem.value         = edit_dh_pem.value;
			window.opener.document.VPN_Server_Crt_form.VPN_Server_Crt.value = edit_server_crt.value;
			window.opener.document.VPN_Server_Key_form.VPN_Server_Key.value = edit_server_key.value;

			if (AuthOnlyUserName.value == "no")
			{
				window.opener.document.VPN_Client_Crt_form.VPN_Client_Crt.value = edit_client_crt.value;
				window.opener.document.VPN_Client_Key_form.VPN_Client_Key.value = edit_client_key.value;
			}
		}
		else
		{
			window.opener.document.VPN_Static_Key_form.VPN_Static_Key.value = edit_static_key.value;
		}
	}
	window.close();	
}
</SCRIPT>
</head>


<body class="">

<!--====================================================================================================================================-->
<blockquote>
	<fieldset name="vpn_set_key" id="vpn_set_key">
		<legend><script>dw(keysAndCertification)</script></legend>
		<form action="/goform/formVPNsetup" method=POST name="VPNSetupKey">
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<tr>
					<td>
						<script>
							showVPNkeyHelp=showText(vpnKeyHelp).replace(/#####/, " <span style=\"color:#0C66FF;\">----- BEGIN *** -----</span> / <span style=\"color:#0C66FF;\">----- END *** -----</span>");
							showVPNkeyHelp=showVPNkeyHelp.replace(/##/, "<br>");
							document.write('<span class="itemText2">'+showVPNkeyHelp+'</span>')
						</script>
					</td>
				</tr>
			</table>
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_TLS1" style="display:none">
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(caCrt)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_ca_crt" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(dhPem)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_dh_pem" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(serverCrt)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_server_crt" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(serverKey)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_server_key" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_TLS2" style="display:none">
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(clientCrt)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_client_crt" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(clientKey)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_client_key" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="VPN_Static_Key" style="display:none">
				<tr>
					<td>
						<div class="itemText-len"><h3><b><script>dw(staticKey)</script>&nbsp;:</b></h3></div>
					</td>
					<td>
						<textarea rows="6" name="edit_static_key" cols="40" maxlength="2999"></textarea>
					</td>
				</tr>
			</table>

			<br>			
	
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<tr>
					<td align="center">
						 <script>document.write('<input type="button" value="'+showText(Close)+'" name="close" onClick="window.close();" class="ui-button-nolimit">&nbsp');</script>
						 <script>document.write('<input type="button" value="'+showText(save)+'" name="save" onClick="checkKEYset()" class="ui-button-nolimit">');</script>
					</td>
				</tr>
			</table>
			<input type="hidden" value="0" name="AuthMode">
			<input type="hidden" value="0" name="AuthOnlyUserName">
		</form>
	</fieldset>
</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
<script>
	with(document.VPNSetupKey)
	{
		AuthMode.value=window.opener.document.VPNSetup.AuthMode.value;
		if (window.opener.document.VPNSetup.AuthOnlyUserName[0].checked == false)
			AuthOnlyUserName.value = "no";
		else
			AuthOnlyUserName.value = "yes";		
						
		checkAuthMode();
		if( AuthMode.value == 0 )
		{
			edit_ca_crt.value     = window.opener.document.VPN_Ca_Crt_form.VPN_Ca_Crt.value;
			edit_dh_pem.value     = window.opener.document.VPN_DH_Pem_form.VPN_DH_Pem.value;
			edit_server_crt.value = window.opener.document.VPN_Server_Crt_form.VPN_Server_Crt.value;
			edit_server_key.value = window.opener.document.VPN_Server_Key_form.VPN_Server_Key.value;
			if (AuthOnlyUserName.value == "no")
			{
				edit_client_crt.value = window.opener.document.VPN_Client_Crt_form.VPN_Client_Crt.value;
				edit_client_key.value = window.opener.document.VPN_Client_Key_form.VPN_Client_Key.value;
			}
		}
		else
		{
			edit_static_key.value=window.opener.document.VPN_Static_Key_form.VPN_Static_Key.value;
		}
	}
</script>
</html>
