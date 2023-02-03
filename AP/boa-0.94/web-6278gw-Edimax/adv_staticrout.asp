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
var entryNum = <%getVar("sroutNum","","");%>;
function addClick()
{
	if ( !ipRule( document.formFilterAdd.sroutIp, showText(IPAddr), "ip", 0))
		return false;
	if(!maskRule(document.formFilterAdd.sroutMask, showText(SubnetMask),0))
		return false;
	if ( !ipRule( document.formFilterAdd.sroutGateway, showText(DefaultGateway), "ip", 0))
		return false;
	if (!portRule(document.formFilterAdd.sroutCount, showText(HopCount), 0, "", 0, 255, 0))
		return false;
	return true;
}
function disableDelButton()
{
	document.formFilterDel.delSelSRout.disabled=true;
	document.formFilterDel.delAllSRout.disabled=true;
}
function goToWeb() {
	if (document.formFilterAdd.enabled.checked==true)
		document.formFilterEnabled.enabled.value="ON";
	document.formFilterEnabled.submit();
}
function goToApply() {
	document.formPortFwApply.submit();
}
function init()
{	
	$("#help-box").hide();
}

function saveChanges(rebootFlag)
{
	with(document.formFilterApply) {

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}
</SCRIPT>

</head>


<body class="" onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(StaticRoutHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="StaticRout_fieldset" id="StaticRout_fieldset">
			<legend><script>dw(StaticRout)</script></legend>

					<form action="/goform/formFilter" method="POST" name="formFilterEnabled">
						<input type="hidden" value="1" name="iqsetupclose">
						<input type="hidden" value="Add" name="addSRout">
						<input type="hidden" value="" name="enabled">
						<input type="hidden" value="" name="isApply">
						<input type="hidden" value="/adv_staticrout.asp" name="submit-url">
					</form>

					<form action="/goform/formFilter" method="POST" name="formFilterAdd">
						
						<table width="700" border="0" cellpadding="2" cellspacing="1" align="center"> 
							<tr>
								<td colspan="5" align="left" class="">
									<input type="checkbox" name="enabled" value="ON" <%getVar("sroutEnabled","1","checked");%> ONCLICK="goToWeb();"><span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(StaticRout)</script></span>
								</td>
							</tr>
							<tr align="center" class="itemText">
								<td><script>dw(DestinationLANIP)</script></td>
								<td><script>dw(SubnetMask)</script></td>
								<td><script>dw(DefaultGateway)</script></td>
								<td><script>dw(HopCount)</script></td>
								<td><script>dw(Interface)</script></td>
							</tr>
							<tr align="center" bgcolor="#FFFFFF">
								<td ><input type="text" name="sroutIp" size="15" maxlength="15"></td>
								<td ><input type="text" name="sroutMask" size="15" maxlength="15"></td>
								<td ><input type="text" name="sroutGateway" size="15" maxlength="15"></td>
								<td ><input type="text" name="sroutCount" size="3" maxlength="3"></td>
								<td >
								<select name="sroutFace">
									<option selected value="0"><script>dw(LAN)</script></option>
									<option value="1"><script>dw(WAN)</script></option>
								</select>
								</td>
							</tr>
							<tr>
								<td colspan="5">
									<p align="right">
										<script>document.write('<input type="submit" value="'+showText(add)+'" name="addSRout" onClick="return addClick()" class="ui-button-nolimit">&nbsp;')</script>
										<script>//document.write('<input type="reset" value="'+showText(clear)+'" name="reset" onClick="" class="ui-button-nolimit">')</script>		
										<input type="hidden" value="" name="isApply">
										<input type="hidden" value="/adv_staticrout.asp" name="submit-url">
									</p>
								</td>
							</tr>
						</table>
						<input type="hidden" value="1" name="iqsetupclose">
					</form>

					<form action="/goform/formFilter" method="POST" name="formFilterDel">

						<table width="700" border="0" cellpadding="2" cellspacing="1" class="">
							<tr><td colspan="7" align="left"><span class="itemText"><script>dw(CurStaticRoutTable)</script>&nbsp;:</td></span></tr>
						</table>

						<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
		        	<tr align="center" class="dnsTableHe">
								<td width="5%"><script>dw(NO)</script></td>
								<td width="22%"><script>dw(DestinationLANIP)</script></td>
								<td width="20%"><script>dw(SubnetMask)</script></td>
								<td width="22%"><script>dw(DefaultGateway)</script></td>
								<td width="10%"><script>dw(HopCount)</script></td>
								<td width="13%"><script>dw(Interface)</script></td>
								<td width="8%"><script>dw(select)</script></td>
							</tr>
						</table>
						<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
							<% StcRoutList(); %>
						</table>
						
						<br>
	
						<table width="700" border="0" cellpadding="2" cellspacing="1" class="">
							<tr>
								<td colspan="7" align="right">
									<script>
										document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="delSelSRout" onClick="" class="ui-button-nolimit">&nbsp;');
										document.write('<input type="submit" value="'+showText(deleteAll)+'" name="delAllSRout" onClick="return deleteAllClick()" class="ui-button-nolimit">');
									</script>
									<input type="hidden" value="" name="isApply">
									<input type="hidden" value="/adv_staticrout.asp" name="submit-url">
								</td>
							</tr>
						</table>
							
						<script>
							if ( entryNum == 0 ) 
							{
								document.formFilterDel.delSelSRout.disabled = true;
								document.formFilterDel.delAllSRout.disabled = true;
								
								document.formFilterDel.delSelSRout.style.color=disableButtonColor;
								document.formFilterDel.delAllSRout.style.color=disableButtonColor;
							}
							if ( entryNum >= 20 ) 
							{
								document.formFilterAdd.addSRout.disabled = true;
								document.formFilterAdd.addSRout.style.color=disableButtonColor;
							}
					 </script>		
						<input type="hidden" value="1" name="iqsetupclose">
					</form>


					<br>

					<form action="/goform/formFilter" method="POST" name="formFilterApply">
						<table align="center" border="0" cellspacing="0" cellpadding="0">
							<tr>
								<td style="text-align:center; padding-top:30px;">
									<script>
										//document.write('<input type="submit" value="'+showText(Apply)+'" name="new_apply" class="ui-button">')
										document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
									</script>
									<input type="hidden" value="" name="isApply">
									<input type="hidden" value="/adv_staticrout.asp" name="submit-url">
								</td>
							</tr>
						</table>
						<input type="hidden" value="1" name="iqsetupclose">
					</form>

	</fieldset>
			
				



<script>
	$("#help-box").hide();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
