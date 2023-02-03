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
function init()
{
	$("#help-box").hide();
}
function evaltF()
{
	if ( messageCheck() )
		return true;
	return false;
}
function messageCheck()
{
	var hid = document.upload;
	if(hid.binary.value.length == 0) {
		alert(showText(enterfile));
		return false;
	}
	else if(hid.binary.value.match(".bin") != ".bin" ){			
			alert(showText(InvalidSomething).replace(/#####/,showText(FileFormat)));
			return false;
	}
	else{
		if (confirm(showText(continueupgrade)))
		{
			hid.submit();
			alert(showText(endofupgrade));
		}
	}
}
function upgrade_action()
{
	with(document.AutoFWupgrade) {
		action.value = "check_version";
		submit();
	}
}

</SCRIPT>

</head>
<body onLoad="init()">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(UpgradeHelp)</script></td></tr>
				</table>
			</div>

			<input type="button" id="help-down" value="Help" class="help-d1-button">
			<script>

			$(document).ready(function(){
				$("#help-down").click(function(){
					$("#help-box").slideToggle();
				}); 
			});

			</script>


<form action="/goform/formAutoFWupgradeIE" method="POST" name="AutoFWupgrade">
	<input type="hidden" value="" name="action">
	<input type="hidden" value="/auto_upgrade.asp" name="submit-url">
</form>	

<blockquote>


		<fieldset name="manual_upgrade_fieldset">
			<legend><script>dw(Upgrade)</script></legend>
					<br>
					
					<table align="center" border="0" Cellspacing="0" width="700">
							<tr>
								<td style="text-align:right;" class="itemText" width="60%"><script>dw(CurrentFirmwareVersion)</script>&nbsp;:&nbsp;</td>
								<td style="text-align:left;" width="40%"><span class="choose-itemText"><% getInfo("fwVersion"); %></span></td>
							</tr>
						</tr>
					</table>
					<table align="center" border="0" Cellspacing="0" width="700" >
						<tr>
							<td style="text-align:center; padding-top:10px;"">
								<script>
								document.write('<input type="button" value="'+showText(checkVersion)+'" name="checkVersion" onclick="upgrade_action()" class="ui-button-nolimit">');	
								</script>
							</td>
						</tr>
					</table>
					<br>
					<form method="post" action="goform/formUploadIE" enctype="multipart/form-data" name="upload">
							<table border="0" cellspacing="0" cellpadding="0" align="center">
								<tr>
										<td>
									<p align="center"><input type="file" size="31" maxlength="31" name="binary" onkeypress="return false"></p>
									</td>
								</tr>
							</table>
							
							<p align="center">
								<script>buffer='<input type=submit value="'+showText(Apply)+'" onclick="return evaltF();" class="ui-button">';document.write(buffer);</script>
								<input type="hidden" value="/index.asp" name="submit-url">
							</p>
					</form>
					
  </fieldset>



<script>

			$("#help-box").hide();


</script>

</blockquote>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		
</body>
</html>

