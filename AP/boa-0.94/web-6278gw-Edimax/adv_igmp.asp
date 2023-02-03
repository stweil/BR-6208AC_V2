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
function saveChanges(rebootFlag)
{
	with(document.advanceIGMP) {

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


<body class="" onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(IGMPHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<fieldset name="IGMP_fieldset" id="IGMP_fieldset">
		<legend align=""><script>dw(IGMP)</script></legend>
				<br>
				<form action="/goform/formIgmpEnable" method="POST" name="advanceIGMP">
							
							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="">
									<tr>
										<td><div class="medleft-itemText-len"><span class="itemText"><script>dw(IGMP)</script>&ensp;<script>dw(Snooping)</script></span></div></td>
										<td><div class="medleft-itemText2-len"><span class="choose-itemText ">
											<script>
												var	igmpsnoopEnabled = <%getVar("igmpsnoopenabled","","");%>;
												if(igmpsnoopEnabled == 1)
												{
													document.write('<input type="radio" name="igmpsnoop_enable" value="yes" checked>'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="igmpsnoop_enable" value="no">'+showText(disable)+'&nbsp;&nbsp;');
												}
												else
												{
													document.write('<input type="radio" name="igmpsnoop_enable" value="yes">'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="igmpsnoop_enable" value="no" checked>'+showText(disable)+'&nbsp;&nbsp;');
												}
											</script>
										</span></div></td>
									</tr>
							</table>
		
							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="">
									<tr>
										<td><div class="medleft-itemText-len"><span class="itemText"><script>dw(IGMP)</script>&ensp;<script>dw(Proxy)</script></span></div></td>
										<td><div class="medleft-itemText2-len"><span class="choose-itemText ">
											<script>
												var	igmpEnabled = <%getVar("igmpEnable","","");%>;
												
												if(igmpEnabled == 1)
												{
													document.write('<input type="radio" name="igmpEnable" value="yes" checked>'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="igmpEnable" value="no">'+showText(disable)+'&nbsp;&nbsp;');
												}
												else
												{
													document.write('<input type="radio" name="igmpEnable" value="yes">'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="igmpEnable" value="no" checked>'+showText(disable)+'&nbsp;&nbsp;');
												}
											</script>
										</span></div></td>
									</tr>
							</table>

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
							<input type="hidden" value="1" name="iqsetupclose">
							<input type="hidden" value="" name="isApply">
							<input type="hidden" value="/adv_igmp.asp" name="wlan-url">									
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
