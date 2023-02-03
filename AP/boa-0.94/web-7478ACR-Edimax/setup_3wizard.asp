<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">

<% language=javascript %>
<SCRIPT>
var wizMode = <%getVar("wizmode","","");%>;
//var wizmode=2;

function goNext()
{
	window.location.assign("setup_3wizard_1.asp");
}

function goBack()
{
	window.location.assign("setup_wizard.asp");
}


function init()
{
	$("#help-box").hide();
}
</SCRIPT>

</head>
<body class="" onLoad="init()">
	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td><div class="help-text" id="help-info1"><script>dw(iQSetupHelp)</script></div></td></tr>
		</table>
	</div>

	<script>
	HelpButton();
	</script>

<blockquote>
	<fieldset name="Wizard_fieldset" id="Wizard_fieldset" >
		<legend><script>dw(Switchwiz)</script></legend>
			<br>
				<form action="" name="iQSetupWizard">
					<script>
							document.write('<table width="600" border="0" cellspacing="1" cellpadding="3" class="" align="center">');
							document.write('<tr>');
							document.write('<td valign="" align="left" class="itemText" style="line-height:22px;">');
							document.write('<span class="itemText" style="line-height:22px;">'+showText(threemodewiz_info)+'</span>');
							document.write('</td>');
							document.write('</tr>');

							document.write('<tr>');
							document.write('<td valign="" align="left" class="itemText">');
							document.write('</td>');
							document.write('</tr>');

							document.write('<tr>');
							document.write('<td valign="" align="left" class="itemText">');
							document.write('<span class="itemText" style="">'+showText(Step)+' 1. '+showText(Step1_info)+'</span>');
							document.write('</td>');
							document.write('</tr>');

							document.write('<tr>');
							document.write('<td valign="" align="left" class="itemText">');
							document.write('<span class="itemText" style="">'+showText(Step)+' 2. '+showText(Step2_info)+'</span>');
							document.write('</td>');
							document.write('</tr>');
	
							document.write('<tr>');
							document.write('<td valign="" align="left" class="itemText">');
							document.write('<span class="itemText" style="">'+showText(Step)+' 3. '+showText(Step3_info)+'</span>');
							document.write('</td>');
							document.write('</tr>');

							document.write('</table>');

							document.write('<br><br>');

							document.write('<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">');
							document.write('<tr>');
							document.write('<td align="right">');
							document.write('<input type="button" value="'+showText(Back)+'" name="back" onClick="goBack();" class="ui-button-nolimit">&nbsp;&nbsp;')
							document.write('</td>');
							document.write('<td align="left">');
							document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="next" onClick="goNext();" class="ui-button-nolimit">')
							document.write('</td></tr>');
							document.write('</table>');
					
					</script>
					<br>
        </form>
			</fieldset>
<script>
	$("#help-box").hide();
</script>
</blockquote>
</body>
</html>
