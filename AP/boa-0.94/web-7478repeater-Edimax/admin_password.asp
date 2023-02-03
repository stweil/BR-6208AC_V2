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

var hostPasswordTbl=<%getInfo("hostpassword");%>;
var hostPassword=hostPasswordTbl[0];

function checkpassword(input) {

	var x = input
	if (x.value != "") {
		if (x.value.search("’") != -1 || x.value.search('”') != -1 || x.value.search("'") != -1 || x.value.search('"') != -1
				|| x.value.search(">") != -1 || x.value.search('<') != -1 || x.value.indexOf("\\") != -1 || x.value.indexOf("\/") != -1
				|| x.value.indexOf("{") != -1 || x.value.indexOf('}') != -1 || x.value.indexOf("[") != -1 || x.value.indexOf("]") != -1
				|| x.value.indexOf("|") != -1 || x.value.indexOf(';') != -1 || x.value.indexOf(":") != -1 || x.value.indexOf(",") != -1
				|| x.value.indexOf(".") != -1 || x.value.indexOf('?') != -1 || x.value.indexOf('`') != -1 
			) {
			alert(showText(PasswordNoSupport));
			//x.value = x.defaultValue;
			x.focus()
			return false
		}
		else
			return true
	}
	else
		return true
}
function init()
{
	$("#help-box").hide();
}
function saveChanges(rebootFlag)
{
	with(document.password) {

		if ( oldpass.value != hostPassword)
		{
			alert(showText(CurrentPasswordnotmatched)+" ("+showText(CurrentPassword)+")");
			oldpass.focus();
			return false;
		}

		if (!checkpassword(newpass)) return false;
		
		if ( newpass.value != confpass.value)
		{
			alert(showText(confirmedPasswordnotmatched)+" ("+showText(ConfirmedPassword)+")");
			confpass.focus();
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
					<tr><td><div class="help-text" id="help-info1"><script>dw(PasswordSetupHelp)</script></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>
<blockquote>

		
		<fieldset name="PasswordSetup_fieldset" id="PasswordSetup_fieldset">
			<legend><script>dw(Password)</script></legend>
					<br>
		
						<form action="/goform/formPasswordSetup" method="POST" name="password">
									<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4" >
											<tr>
												<td><div class="medleft-itemText-len"><script>dw(CurrentPassword)</script></div></td>
												<td><div class="medleft-itemText2-len">
													<input type="text" name="oldpass" size="20" maxlength="30" value="">
												</div></td>
											</tr>
											<tr>
												<td><div class="medleft-itemText-len"><script>dw(NewPassword)</script></div></td>
												<td><div class="medleft-itemText2-len">
													<input type="text" name="newpass" size="20" maxlength="30">
												</div></td>
											</tr>
											<tr>
												<td><div class="medleft-itemText-len"><script>dw(ConfirmedPassword)</script></div></td>
												<td><div class="medleft-itemText2-len">
													<input type="text" name="confpass" size="20" maxlength="30">
												</div></td>
											</tr>
							  </table>

								<br>

								<table align="center" border="0" Cellspacing="0" width="700">
									<tr>
										<td style="text-align:center; padding-top:30px;">
											<script>
												document.write('<input type="button" value="'+showText(Apply)+'" name="save" onclick="saveChanges(0)" class="ui-button">')
											</script>
										</td>
									</tr>
								</table>
								<input type="hidden" value="1" name="iqsetupclose">
								<input type="hidden" value="" name="isApply">
								<input type="hidden" value="/admin_password.asp" name="submit-url">
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
