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

function goSave()
{
	document.saveform.SaveText.value="1";
	document.saveform.submit();
}
</SCRIPT>

</head>
<body>


<blockquote>
	<fieldset name="PasswordSetup_fieldset" id="SaveText_fieldset" style="display:block">
			<legend><script>dw(BackupRestore)</script>&nbsp;(<script>dw(apmode0)</script>)</legend>
				<br>
				<table border="0" style="height:28px;width:400px" align="center">
					<form action=/goform/saveAndReboot method=POST name="saveform">
						<tr>
							<td><div class="itemText-len"><script>dw(BackupSettings)</script></div></td>
							<td><div class="itemText2-len">
								<script>document.write('<input type="button" value="'+showText(save)+'" name="save" onClick="goSave();" class="ui-button-nolimit">');</script></div>
						</td>
						<input type="hidden" value="" name="SaveText">
						</tr>
					</form>
				</table>
				<form action=/goform/formSaveText enctype="multipart/form-data" method="post" name="saveText">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
									<tr>
										<td><div class="itemText-len"><script>dw(RestoreSettings)</script></div></td>
										<td><div class="itemText2-len" nowrap><input type="file" name="binary2" size=24>
											<script>document.write('<input type="submit" value="'+showText(Upload)+'" name="aaa" class="ui-button-nolimit">')</script></div></td>
									</tr>
						</table>
						<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
				</form>
				<br>
  </fieldset>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
	
</html>
