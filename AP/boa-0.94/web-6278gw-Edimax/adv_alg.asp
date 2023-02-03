<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
var ALGHexArray=new Array(0x002,0x004,0x020,0x040,0x080,0x200);

function saveChanges(rebootFlag)
{
	with(document.ALG) {
		/*
		0x002 ftp   2  
		0x004 h323  4 
		0x020 pptp  32
		0x040 ipsec 64 
		0x080 l2tp  128
		0x200 sip   512
		*/
		var longVal=281;
		if (Index0.checked==true)	longVal |= ALGHexArray[0];
		if (Index1.checked==true)	longVal |= ALGHexArray[1];
		if (Index2.checked==true)	longVal |= ALGHexArray[2];
		if (Index3.checked==true)	longVal |= ALGHexArray[3];
		if (Index4.checked==true)	longVal |= ALGHexArray[4];
		if (Index5.checked==true)	longVal |= ALGHexArray[5];
		appLyGatewayValue.value=longVal;

		if(rebootFlag)
			isApply.value = "ok;"

		submit();
	}
}

function init()
{
	$("#help-box").hide();
}
</SCRIPT>

</head>
<body class="" onLoad="init()">

<!--=============================================================================================-->
<div class="help-information" id="help-box" style="display:none">
	<table class="help-table1">
		<tr><td><div class="help-text" id="help-info1">
		<script>dw(ALGHelp)</script>
		</div></td></tr>
	</table>
</div>
<script>HelpButton();</script>
<!--=============================================================================================-->
<blockquote>
<fieldset name="ALG_fieldset" id="ALG_fieldset">
	<legend><script>dw(ALG)</script></legend>
	<br>
	<form action="/goform/formALGSetup" method="POST" name="ALG">
	<input type="hidden" value="<% getInfo("appLayerGateway"); %>" name="appLyGatewayValue">								
		<table border="0" width="700" cellpadding="2" cellspacing="1" align="center">
			<tr align="center" class="titletableText">
				<td width="10%" nowrap><script>dw(enable)</script>
				<td width="24%" nowrap><script>dw(Name)</script></td>
				<td width="70%" nowrap><script>dw(Comment)</script></td>
			</tr>
			<script>
				var ALGArray = new Array(showText(FTP),showText(H323),showText(PPTP),showText(IPsec),showText(L2TP),showText(SIP));
				var ALGComArray = new Array(showText(SupFTP),showText(SupH323netmeeting),showText(SupPPTP),showText(SupIPsecpassthrough),showText(SupL2TPpassthrough),showText(SupSIP));

				for(i=0;i<6;i++)
				{
					document.write("<tr align=center class='listtableText'>");
					if (document.ALG.appLyGatewayValue.value & ALGHexArray[i])
						document.write('<td ><input type="checkbox" name="Index'+i+'" checked></td>');
					else
						document.write('<td align="center" class=""><input type="checkbox" name="Index'+i+'"></td>');
					document.write('<td align="center" >' + ALGArray[i] + '</span></td>');
					document.write('<td align="center" >' + ALGComArray[i] + '</span></td></tr>');
				}
			</script>
		</table>
		<input type="hidden" value="" name="isApply">
		<input type="hidden" value="/adv_alg.asp" name="wlan-url">
		<br>
		<table align="center" border="0" cellspacing="0" cellpadding="0">
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
	</form>			
</fieldset>
<!--=============================================================================================-->
<script>
	$("#help-box").hide();
</script>
</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>

</html>
