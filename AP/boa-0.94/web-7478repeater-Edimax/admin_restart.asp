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
function doConfirm()
{
	if(confirm(showText(wanttorestart)))
	{
		alert(showText(takesometime));
		return true;
	}
	document.RebootForm.reset_flag.value=1;
	return false;
}
function logoutF()
{
	document.location.RebootForm.submit();
	return true;
}
function init()
{
	$("#help-box").hide();
}
</SCRIPT>

</head>
<body onLoad="init()">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(restartHelp)</script></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="PasswordSetup_fieldset" id="PasswordSetup_fieldset">
			<legend><script>dw(Restart)</script></legend>

					<table width="600" border="0" cellspacing="1" cellpadding="0" class="" align="center">
							<tr>
								<td valign="top" align="center">

									<p align="left"><span class="itemText" style="line-height:22px;"><script>dw(restartHelp)</script></span></p>
									<form action=/goform/formReboot method=POST name="RebootForm" onsubmit="return doConfirm();">
												<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">
													<tr>
														<td style="text-align:center; padding-top:30px;">
															<script>buffer='<input type=submit value="'+showText(Apply)+'" class="ui-button">';document.write(buffer);</script>
														</td>
													</tr>
												</table>							
												<input type="hidden" name="reset_flag" value="0">
												<input type=hidden value="/admin_restart.asp" name="submit-url">
									</form>
									
								</td>
							</tr>
						</table>
					
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
