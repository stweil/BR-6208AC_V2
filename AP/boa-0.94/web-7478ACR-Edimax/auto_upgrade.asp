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
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var dut_fwinfo="<% getInfo("fwVersion"); %>";
		dut_fwinfo=dut_fwinfo.substring(0,4);
var fwinfo=<%getInfo("fw_info");%>;
var newFWVersion = fwinfo[0];
		newFWVersion = newFWVersion.substring(0,4);
var newFWVersionZipurl = fwinfo[1];
var newFWVersionurl = fwinfo[2];

var information_error=0;

function FWVersionError(str)
{
	if(str == "") return 1;
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') || str.charAt(i) == '.' )
			continue;
		return 1;
	}
	return 0;
}


function init()
{
	if(FWVersionError(newFWVersion))
		information_error = 1;

	if(information_error){
		document.getElementById('strcheck').innerHTML = showText(serverError);
		document.getElementById('download').style.display = "none";
	}
	else{
		if(parseFloat(dut_fwinfo) >= parseFloat(newFWVersion)){
			document.getElementById('download').style.display = "none";
			document.getElementById('strcheck').innerHTML = showText(NoNeedupdate);
		}
		else{
			document.getElementById('download').style.display = "block";
			document.getElementById('strcheck').innerHTML = showText(newVersion)+' V'+newFWVersion+'. '+showText(actionByChose);	
		}
	}
}
function startupgrade()
{
	if ( messageCheck() )
		return true;
	return false;
}
function messageCheck()
{
	var hid = document.doupgrade;
	if (confirm(showText(continueupgrade)))
	{
		alert(showText(noteForUpgrade));
		hid.submit();
	}
}
function goback2upgrade()
{
	if(parent.back2status == 1){
		parent.back2status=0;

		if((wizMode==0) || (wizMode==4))
			document.fail.failurl.value="/status.asp";//window.location.assign("status.asp");
		else
			document.fail.failurl.value="/status_noInternet.asp";//window.location.assign("status_noInternet.asp");
	}
	else
		document.fail.failurl.value="/admin_upgrade.asp"; //window.location.assign("admin_upgrade.asp");
	
	document.fail.submit();
}
function downloadfw()
{
	window.location.href=newFWVersionZipurl;
}
</SCRIPT>

</head>
<body onLoad="init()">

<form action="/goform/formAutoFWupgrade" method="POST" name="doupgrade">
	<input type="hidden" value="download_FW" name="action">
	<input type="hidden" value="/autofwdownload.asp" name="submit-url">
</form>	

<form action="/goform/formAutoFWupgrade" method="POST" name="fail">
	<input type="hidden" value="fail" name="action">
	<input type="hidden" value="" name="failurl">
</form>

<form action="/goform/formAutoFWupgrade" method="POST" name="back">
	<input type="hidden" value="download_FW" name="action">
	<input type="hidden" value="/autofwdownload.asp" name="submit-url">
</form>	

<blockquote>
<fieldset name="checkVersion_fieldset">
			<legend><script>dw(checkVersion)</script></legend>
		
				<br>

				<table align="center" border="0" Cellspacing="0" width="700">
					<tr>
						<td style="text-align:center;" class="itemText" id="strcheck">
						</td>
					</tr>
				</table>
		
				<br>
				<br>

				<span id="download" style="display:block">
				<table align="center" border="0" Cellspacing="0" width="700">
					<tr>
						<td style="text-align:center; padding-top:10px;">
							<script>

								document.write('<input type="submit" value="'+showText(autoupgrade)+'" name="autoupgrade" id="autoupgrade" onclick="return startupgrade()" class="ui-button-nolimit">');	
								document.write('&nbsp;&nbsp;&nbsp;<input type="button" value="'+showText(saveAs)+'" name="saveAs" id="saveAs" onclick="downloadfw()" class="ui-button-nolimit">');	

							</script>
						</td>
					</tr>
				</table>
				<br>
				</span>

				<table align="center" border="0" Cellspacing="0" width="700">
					<tr>
						<td style="text-align:center; padding-top:10px;">
							<script>
								document.write('<input type="button" value="'+showText(goback)+'" name="goback" id="goback" onclick="goback2upgrade()" class="ui-button-nolimit">');	

							</script>
						</td>
					</tr>
				</table>

				<br>
 </fieldset>
</blockquote>

<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		
</body>
</html>
