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


var FirewallEnabled = <%getVar("FirewallEnable","","");%>;

function init()
{
	$("#help-box").hide();
}
function saveChanges(rebootFlag)
{
	with(document.fwseting) {

		if (FirewallEnable[0].checked==true)
		{
			parent.lFrame.showFwSet(8,1);
		}
		else
		{
			parent.lFrame.showFwSet(8,0);
		}

		if(rebootFlag)
			isApply.value = "ok;"

		submit();
	}
}


</SCRIPT>
</head>


<bodyonLoad="init()">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(firewallHelp)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>

	<form action="/goform/formFwEnable" method="POST" name="fwseting">
			<fieldset name="firewall_fieldset" id="firewall_fieldset">
				<legend><script>dw(Firewall)</script></legend>
						<br>
						<table width="600" border="0" cellspacing="1" cellpadding="0" class="" align="center">
							<tr>
								<td valign="top" align="left">

									<span class="itemText"><script>dw(firewallCont)</script></span>
								</td>
							</tr>
						</table>
	
						<table width="600" border="0" cellpadding="2" cellspacing="2" align="center" id="">
								<tr>
									<td><div class="medleft-itemText-len"><script>dw(SPIfirewall)</script></div></td>
									<td><div class="medleft-itemText2-len"><span class="choose-itemText ">
										<input type="radio" name="FirewallEnable" value="yes" <%getVar("FirewallEnable","1","checked");%>>
											<script>dw(enable);</script>&nbsp;
										<input type="radio" name="FirewallEnable" value="no" <%getVar("FirewallEnable","0","checked");%>>
											<script>dw(disable);</script>
									</span></div></td>
								</tr>
						</table>
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
						<input type="hidden" value="1" name="iqsetupclose">
						<input type="hidden" value="" name="isApply">
						<input type="hidden" value="/fw_firewal.asp" name="submit-url">
			</form>
	
			
				



<script>
	$("#help-box").hide();

</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>


</html>
