<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="FUNCTION_SCRIPT"></script>

<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">


<% language=javascript %>
<SCRIPT>
var saveSystemLog = "<% getInfo("saveSystemLog"); %>"
var wizMode = <%getVar("wizmode","","");%>;
function resetClick()
{
  if ( !confirm(showText(wanttoreset)) ) 
		return false;
  else
	{
		alert(showText(takesometime));
		return true;
	}
}
function init()
{
	$("#help-box").hide();

	if(typeof(_EXPORT_SYS_LOG_) != 'undefined')
		document.getElementById("Log").style.display = "block";		
}
function goSave()
{
	document.saveform.SaveText.value="1";
	document.saveform.submit();
}

function evaltF()
{
	var hid = document.uploadConfig;
	if(hid.binary.value.length == 0) {
		alert(showText(enterfile));
		return false;
	}
	if(hid.binary.value.match(".bin") != ".bin" ){			
			alert(showText(InvalidSomething).replace(/#####/,showText(FileFormat)));
			return false;
	}

	return true;
}
</SCRIPT>

</head>
<body onLoad="init()">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(BackupRestoreHelp)</script></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="PasswordSetup_fieldset" id="PasswordSetup_fieldset">
			<legend><script>dw(BackupRestore)</script></legend>
					<br>
					<form action=/goform/formSaveConfig method=POST name="saveConfig">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
									<tr>
										<td><div class="itemText-len"><script>dw(BackupSettings)</script></div></td>
										<td><div class="itemText2-len">
											<script>document.write('<input type="submit" value="'+showText(save)+'" name="save" class="ui-button-nolimit">')</script></div></td>
									</tr>
							</table>
							<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
				</form>
				
				<form action=/goform/formSaveConfigSec enctype="multipart/form-data" method="post" name="uploadConfig">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
									<tr>
										<td><div class="itemText-len"><script>dw(RestoreSettings)</script></div></td>
										<td><div class="itemText2-len" nowrap><input type="file" name="binary" size=24>
											<script>document.write('<input type="submit" value="'+showText(Upload)+'" name="load" class="ui-button-nolimit" onclick="return evaltF();">')</script></div></td>
									</tr>
						</table>
						<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
				</form>

				<form action=/goform/formResetDefault enctype="multipart/form-data" method="post" name="resetDefault">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
									<tr>
										<td><div class="itemText-len"><script>dw(RestoretoDefault)</script></div></td>
										<td><div class="itemText2-len"><script>document.write('<input type="submit" value="'+showText(Reset)+'" name="reset" onclick="return resetClick()" class="ui-button-nolimit">')</script></div></td>
										<input type="hidden" value="/index.asp" name="submit-url">
									</tr>
						</table>
				</form>

				<span id="Log" style="display:none">
				<form action="/goform/formSystemLog" method="POST" name="saveSystemLog">
					<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
						<tr>
							<td><div class="itemText-len"><script>dw(Debug_Logs)</script></script></div></td>
							<td><div class="itemText2-len">
							<script>document.write('<input type="submit" value="'+showText(save)+'" name="export" class="ui-button-nolimit">')</script></div></td>
							<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
						</tr>
					</table>
				</form>
				</span>
				<br>
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
