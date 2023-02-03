<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<% language=javascript %>

<SCRIPT>
var entryNum = <%getVar("wan1QosNum","","");%>;

function disableDelButton(form)
{
	form.delSelQos.disabled = true;
	form.delAllQos.disabled = true;
	form.showQos.disabled = true;
}
function replWeb() {
	if (entryNum < 16) {
		document.formWan1Qos.select_edit.value=0;
		openWindow("/qosadd.asp");
	}
}
function move_check() {
	if ((entryNum == 1) || (entryNum == 0)) {
   document.formWan1Qos.moveUpQos.disabled = true;
	 document.formWan1Qos.moveUpQos.style.color=disableButtonColor;

	 document.formWan1Qos.moveDownQos.disabled = true;
	 document.formWan1Qos.moveDownQos.style.color=disableButtonColor;
	}
	else
	{	
		var checkentry = 0;
		var checkNum = 0;
		for (var i=1; i<=entryNum; i++) {
			entryIndex="select"+i;
			if (document.getElementById(entryIndex).checked == true)
			{
				checkentry=i;
				checkNum++;
			}
		}

		if (checkNum > 1) {
		 document.formWan1Qos.moveUpQos.disabled = true;
		 document.formWan1Qos.moveUpQos.style.color=disableButtonColor;

		 document.formWan1Qos.moveDownQos.disabled = true;
		 document.formWan1Qos.moveDownQos.style.color=disableButtonColor;
		}
		else if ((checkentry == 1) && (checkNum == 1)) {  // first rule
		 document.formWan1Qos.moveUpQos.disabled = true;
		 document.formWan1Qos.moveUpQos.style.color=disableButtonColor;

		 document.formWan1Qos.moveDownQos.disabled = false;
		 document.formWan1Qos.moveDownQos.style.color='#ffffff';
		}
		else if ((checkentry == entryNum) && (checkNum == 1)) {  // last rule
		 document.formWan1Qos.moveUpQos.disabled = false;
		 document.formWan1Qos.moveUpQos.style.color='#ffffff';

		 document.formWan1Qos.moveDownQos.disabled = true;
		 document.formWan1Qos.moveDownQos.style.color=disableButtonColor;
		}
		else                                                     // medium rule
		{
			document.formWan1Qos.moveUpQos.disabled = false;
		 	document.formWan1Qos.moveUpQos.style.color='#ffffff';
		
			document.formWan1Qos.moveDownQos.disabled = false;
		  document.formWan1Qos.moveDownQos.style.color='#ffffff';
		}
	}
}
function checkNum(form) {
	var checkFlag = 0;
	var checkbox_value = 0 ;
	for (var i=1; i<form.elements.length; i++) {
		if (form.elements[i].type == 'checkbox' ) {
			checkbox_value++;
			if (form.elements[i].checked == true)
			{
				document.formWan1Qos.select_edit.value=checkbox_value;
				checkFlag++;
			}
		}
	}
	if (checkFlag >= 2) {
		alert(showText(onlyselectone));
		return false;
	}
	if (checkFlag < 1) {
		alert(showText(Pleaseselectone));
		return false;
	}
	return true;
}
function clickapply(rebootFlag) 
{

	if (document.formWan1Qos.enabled2.checked==true){
		if(parseInt(document.formWan1Qos.maxdown.value,10) <= 0)
		{
			alert(showText(InvalidSomething).replace(/#####/,showText(TotalDownloadBandwidth))+' '+showText(QoSBandwidtherror));
			return false;
		}
		if(parseInt(document.formWan1Qos.maxup.value,10) <= 0)
		{
			alert(showText(InvalidSomething).replace(/#####/,showText(TotalUploadBandwidth))+' '+showText(QoSBandwidtherror));
			return false;
		}
		if (!portRule1(document.formWan1Qos.maxdown, showText(TotalDownloadBandwidth), 1))
			return false;
		if (!portRule1(document.formWan1Qos.maxup, showText(TotalUploadBandwidth) , 1))
			return false;

		document.qosApply.maxdown.value = parseInt(document.formWan1Qos.maxdown.value,10);
		document.qosApply.maxup.value = parseInt(document.formWan1Qos.maxup.value,10);
	}

	if(rebootFlag)
		document.qosApply.isApply.value="ok";

	document.qosApply.submit();
}
function Clickdelete()
{
	return deleteClick();
}
function ClickdeleteAll()
{
	return deleteAllClick();
}
function goToWeb() {
	if (document.formWan1Qos.enabled2.checked==true)
		document.formQoSEnabled.enabled1.value="ON";
	document.formQoSEnabled.submit();
}
function enmvButton(form)
{
	form.moveUpQos.disabled = true;
	form.moveDownQos.disabled = true;
}
function specified_protocol()
{
	switch ( document.formQos.trafType.value )
	{
	case "0":
		document.formQos.sourPort.disabled=false;
		document.formQos.destPort.disabled=false;
		break;
	default:
		document.formQos.sourPort.value="";
		document.formQos.destPort.value="";
		document.formQos.sourPort.disabled=true;
		document.formQos.destPort.disabled=true;
		break;
	}
}
function displayObj()
{
	if (document.formQos.sourType.value == 0) {
		document.getElementById('ipId').style.display = "block";
		document.getElementById('macId').style.display = "none";
	}
	else {
		document.getElementById('ipId').style.display = "none";
		document.getElementById('macId').style.display = "block";
	}
	specified_protocol();
}
function edit_apply()
{
	if (!checkNum(document.formWan1Qos))
		return false;
	else
		openWindow("/qosadd.asp");
	return true;
}

function init()
{
	$("#help-box").hide();
}

function clearform()
{
	document.formWan1Qos.reset();
}
</SCRIPT>
</head>
<body onLoad="init();">
		<div class="help-information" id="help-box" style="display:none">
			<table class="help-table1">
				<tr><td><div class="help-text" id="help-info1"><script>dw(QoSHelp)</script>
				</div></td></tr>
			</table>
		</div>
		<script>
		HelpButton();
		</script>
<blockquote>

	<fieldset name="QoS_fieldset" id="QoS_fieldset">
		<legend><script>dw(QoS)</script></legend>

			
			<form action="/goform/formQoS" method="POST" name="formQoSEnabled">
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="ON" name="isEnabled">
				<input type="hidden" value="" name="enabled1">
				<input type="hidden" value="QoS" name="EZQos_Mode">
				<input type="hidden" value="/qos.asp" name="submitUrl">
				<input type="hidden" value="" name="isApply">
			</form>

			<form action="/goform/formQoS" method="POST" name="formWan1Qos">
				<table width="700" border="0" cellpadding="2" cellspacing="2" align="center">
					<tr>
						<td colspan="2" height="20" valign="middle" align="left" class="style1">
							<input type="checkbox" name="enabled2" value="ON" <%getVar("wan1QosEnabled","1","checked");%> onClick="goToWeb();">  
							&nbsp;<span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(QoS)</script></span>
						</td>
					</tr>
					<tr>
						<td width="350" height="20" valign="middle" align="right" class="style14"><div class="itemText-len"><script>dw(TotalDownloadBandwidth);</script></div></td>
						<td width="350" height="20" valign="middle" align="left" class="style1">
							<input type="text" name="maxdown" size="5" maxlength="5" value="<% getInfo("maxdownbandwidth"); %>">&nbsp;<span class="itemText"><script>dw(kbits);</script></span>
						</td>
					</tr>
					<tr>
						<td width="350" height="20" valign="middle" align="right" class="style14"><div class="itemText-len"><script>dw(TotalUploadBandwidth);</script></div></td>
						<td width="350" height="20" valign="middle" align="left" class="style1">
							<input type="text" name="maxup" size="5" maxlength="5" value="<% getInfo("maxupbandwidth"); %>">&nbsp;<span class="itemText"><script>dw(kbits);</script></span>
						</td>
					</tr>
				</table>

				<br>

				<table border="0" width="700" cellpadding="2" cellspacing="1">
					<tr>
						<td colspan="5"><span class="itemText"><script>dw(CurQoSTable);</script>&nbsp;:</span></td>
					</tr>
				</table>

				<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
					<tr align="center" class="dnsTableHe">
						<td width="10%"><script>dw(Priority)</script></td>
						<td width="30%"><script>dw(RuleName)</script></td>
						<td width="25%"><script>dw(UploadBandwidth)</script></td>
						<td width="25%"><script>dw(DownloadBandwidth)</script></td>
						<td width="10%"><script>dw(select)</script></td>
					</tr>
				</table>
				<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
					<% Wan1QosList(); %>
				</table>

				<br>

				<table width="600" border="0" cellpadding="0" cellspacing="2" align="center">
					<tr>
						<td height="20" valign="middle" align="center">
							<script>
								document.write('<input type="button" name="addQos" value="'+showText(add)+'" onClick="replWeb();"  class="ui-button-nolimit">&nbsp;');
								document.write('<input type="button" name="showQos" value="'+showText(Edit)+'" onClick="edit_apply();"  class="ui-button-nolimit">&nbsp;');
								document.write('<input type="submit" name="delSelQos" value="'+showText(deleteSelected)+'" onClick="" class="ui-button-nolimit">&nbsp;');
								document.write('<input type="submit" name="delAllQos" value="'+showText(deleteAll)+'" onClick="return ClickdeleteAll()" class="ui-button-nolimit">&nbsp;');
								document.write('<input type="submit" name="moveUpQos" value="'+showText(MoveUp)+'" onClick="return checkNum(document.formWan1Qos);" class="ui-button-nolimit">&nbsp;');
								document.write('<input type="submit" name="moveDownQos" value="'+showText(MoveDown)+'" onClick="return checkNum(document.formWan1Qos);" class="ui-button-nolimit">');
							</script>
							<input type="hidden" value="/qos.asp" name="submitUrl">
							<input type="hidden" value="1" name="configWan">
							<input type="hidden" value="0" name="select_edit">
						</td>
					</tr>
				</table>
				
				<script>
					if ( entryNum == 0 ) 
					{
						document.formWan1Qos.showQos.disabled = true;
						document.formWan1Qos.delSelQos.disabled = true;
						document.formWan1Qos.delAllQos.disabled = true;
						document.formWan1Qos.moveUpQos.disabled = true;
						document.formWan1Qos.moveDownQos.disabled = true;

						document.formWan1Qos.showQos.style.color=disableButtonColor;
						document.formWan1Qos.delSelQos.style.color=disableButtonColor;
						document.formWan1Qos.delAllQos.style.color=disableButtonColor;
						document.formWan1Qos.moveUpQos.style.color=disableButtonColor;
						document.formWan1Qos.moveDownQos.style.color=disableButtonColor;
					}
				
					move_check();
		
					if ( entryNum >= 16 ) 
					{
						document.formWan1Qos.addQos.disabled = true;
						document.formWan1Qos.addQos.style.color=disableButtonColor;
					}
			 </script>
				<input type="hidden" value="1" name="iqsetupclose">	
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="/qos.asp" name="submitUrl">	
			</form>
			
			<br>

			<form action="/goform/formQoS" method="POST" name="qosApply">
				<table align="center" border="0" cellspacing="0" cellpadding="0" width="700">
					<tr>
						<td style="text-align:center; padding-top:30px;">
							<script>

								document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return clickapply(0)" class="ui-button">');	
								var show_reboot = "<% getInfo("show_reboot"); %>"
								if(show_reboot == "1")
									document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return clickapply(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
							</script>
						</td>
					</tr>
				</table>
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="/qos.asp" name="submitUrl">
				<input type="hidden" name="maxup">
				<input type="hidden" name="maxdown">
			</form>
	</fieldset>
<script>
	$("#help-box").hide();
	if ( entryNum == 0 ) disableDelButton(document.formWan1Qos)
	if ( entryNum < 2 ) enmvButton(document.formWan1Qos)
</script>

</blockquote>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</body>
</html>
