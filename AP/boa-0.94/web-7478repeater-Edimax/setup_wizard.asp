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
var mode_select=2;

function openWeb()
{
	
		if (mode_select==0) 
		{
			if(wizMode==0)
			  window.open( '/hwsetup.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
			else if(wizMode==1)
				window.open( '/wiz_apmode.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
			else if(wizMode==2)
				window.open( '/wiz_repeater.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
			else if(wizMode==3)
				window.open( '/wiz_APcl.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
			else
				window.open( '/wiz_WISP.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
		}
		else           
		{
			window.location.assign("setup_3wizard.asp");
		}
}

function runwiz(x)
{
	mode_select=x;

	document.iQSetupWizard.runwiz1.checked=false;
	document.iQSetupWizard.runwiz3.checked=false;

	if(!mode_select)
		document.iQSetupWizard.runwiz1.checked=true;
	else
		document.iQSetupWizard.runwiz3.checked=true;
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
					<tr><td><div class="help-text" id="help-info1"><script>dw(iQSetupHelp)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>


			<fieldset name="Wizard_fieldset" id="Wizard_fieldset" >
				<legend><script>dw(SetupWizard)</script></legend>
					<br>
				<form action="" name="iQSetupWizard">

					<script>
	
							/*document.write('<table width="600" border="0" cellspacing="1" cellpadding="0" class="" align="center">');
							document.write('<tr>');
							document.write('<td valign="" align="center" width="5%">');
							document.write('<input type="radio" value="0" name="runwiz1" onClick="runwiz(0)" >');
							document.write('</td>');
							document.write('<td valign="" align="left" class="choose-itemText">'+showText(SetupWizard));		
							document.write('</td>');
							document.write('</tr>');

							document.write('<tr><td></td>');
							document.write('<td class="itemText" style="line-height:22px;">'+showText(onemodewiz));
							document.write('</td>');
							document.write('</tr>');
							document.write('</table>');
			
							document.write('<br>');*/

							document.write('<table width="600" border="0" cellspacing="1" cellpadding="0" class="" align="center">');
							document.write('<tr>');
							document.write('<td valign="" align="center" width="5%">');
							//document.write('<input type="radio" value="0" name="runwiz3" onClick="runwiz(1)">');
							document.write('</td>');
							document.write('<td valign="" align="left" class="choose-itemText">'+showText(Switchwiz));		
							document.write('</td>');
							document.write('</tr>');

							document.write('<tr><td></td>');
							document.write('<td class="itemText" style="line-height:22px;">'+showText(threemodewiz));
							document.write('</td>');
							document.write('</tr>');
							document.write('</table>');

							document.write('<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">');
							document.write('<tr>');
							document.write('<td style="text-align:center; padding-top:30px;">');
							document.write('<input type="button" value="'+showText(runWizard)+'" name="new_apply" onClick="openWeb();" class="ui-button-nolimit">')
							document.write('</td></tr>');
							document.write('</table>');
						
					</script>
        </form>
				<br>
			</fieldset>
				



<script>


			$("#help-box").hide();
			//runwiz(0);
		

</script>

</blockquote>

		
</body>
</html>
