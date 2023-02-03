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

var reMangEnabled = <%getVar("reMangEnable","","");%>;
function init()
{
	$("#help-box").hide();
}
function Enable(resetFlag)
{
	if(!resetFlag)
	{
		if(document.tF.reMangEnable.checked==true) {
			document.tF.reManHostAddr.disabled = false;
			document.tF.reManPort.disabled = false;
		}
		else {
			document.tF.reManHostAddr.disabled = true;
			document.tF.reManPort.disabled = true;
		}
	}	
	else
	{
		if(reMangEnabled) {
			document.tF.reManHostAddr.disabled = false;
			document.tF.reManPort.disabled = false;
		}
		else {
			document.tF.reManHostAddr.disabled = true;
			document.tF.reManPort.disabled = true;
		}
	}
}
function resetform()
{
	Enable(1);
}
function saveChanges(rebootFlag)
{
	with(document.tF) {
		if(reMangEnable.checked==true)
		{
			if ( !ipRule( reManHostAddr, showText(IPAddr), "ip", 1))
				return false;
			if (!portRule(reManPort, showText(PortRange), 0, "", 1, 65535, 1))
				return false;
		}

		if(rebootFlag)
			isApply.value = "ok;"

		submit();
	}
}
</SCRIPT>

</head>
<body onLoad="init()">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(RemoManagHelp)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>


		<fieldset name="PasswordSetup_fieldset" id="PasswordSetup_fieldset">
			<legend><script>dw(RemoteAccess)</script></legend>
					<br>
					<form name="tF" method="post" action="/goform/formReManagementSetup">
							<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
									<tr>
										<td><div class="medleft-itemText-len"><script>dw(Hostaddress)</script></div></td>
										<td><div class="medleft-itemText2-len">
											<input type="text" name="reManHostAddr" size="15" maxlength="15" value="<% getInfo("reManHostAddr"); %>">
										</div></td>
									</tr>
									<tr>
										<td><div class="medleft-itemText-len"><script>dw(Port)</script></div></td>
										<td><div class="medleft-itemText2-len">
											<input type="text" name="reManPort" size="5" maxlength="5" value="<% getInfo("reManPort"); %>">
										</div></td>
									</tr>
									<tr>
										<td><div class="medleft-itemText-len"><script>dw(Enabled)</script></div></td> 
										<td><div class="medleft-itemText2-len">
											<input type="checkbox" name="reMangEnable" value="ON" <%getVar("reMangEnable","1","checked");%> onClick="Enable(0)">
										</div></td>
									</tr>
							</table>
							
							<br>

							<table align="center" border="0" Cellspacing="0" width="700">
								<tr>
									<td style="text-align:center; padding-top:30px;" align="center">
										<script>
											document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
											var show_reboot = "<% getInfo("show_reboot"); %>"
											if(show_reboot == "1")
												document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
										</script>
									</td>
								</tr>
							</table>
							<input type="hidden" value="1" name="iqsetupclose">
							<input type="hidden" value="" name="isApply">
							<input type="hidden" value="/admin_remotmang.asp" name="submit-url">	
					</form>
  </fieldset>



<script>
	$("#help-box").hide();
	Enable(0);
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		

</html>
