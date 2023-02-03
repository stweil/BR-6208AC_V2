<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="file/javascript.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script>
var connectEnabled=0, autoconf=0;
var sel_value;
function verifyBrowser() {
  var ms = navigator.appVersion.indexOf("MSIE");
  ie4 = (ms>0) && (parseInt(navigator.appVersion.substring(ms+5, ms+6)) >= 4);
  var ns = navigator.appName.indexOf("Netscape");
  ns= (ns>=0) && (parseInt(navigator.appVersion.substring(0,1))>=4);
  if (ie4)
	return "ie4";
  else
	if(ns)
		return "ns";
	else
		return false;
}
function disableButton (button) {
  if (verifyBrowser() == "ns")
  	return;
  if (document.all || document.getElementById)
    button.disabled = true;
  else if (button) {
    button.oldOnClick = button.onclick;
    button.onclick = null;
    button.oldValue = button.value;
    button.value = 'DISABLED';
  }
}
function enableButton (button) {
  if (verifyBrowser() == "ns")
  	return;
  if (document.all || document.getElementById)
    button.disabled = false;
  else if (button) {
    button.onclick = button.oldOnClick;
    button.value = button.oldValue;
  }
}
function enableConnect(val)
{
  document.forms['formWlSiteSurvey'].elements['done'].disabled = false;
	document.forms['formWlSiteSurvey'].elements['done'].style.color="#ffffff";
  connectEnabled=1;
  sel_value=val;
}
function connectClick()
{
  var i;
  var apNum=document.forms['formWlSiteSurvey'].elements['apCount'].value;
  if (connectEnabled==1) {
		for ( i=0; i<apNum; i++) {
			if (sel_value == "sel"+i) {
				encryptiontext = document.forms['formWlSiteSurvey'].elements['encryption'+i].value;
				ciphersutietext = document.forms['formWlSiteSurvey'].elements['ciphersutie'+i].value;
				if (encryptiontext == "WPA2PSK" && ciphersutietext == "AES")
				{
					window.opener.document.forms['wisp_form'].stadrv_encrypttype.selectedIndex = 1;
				}
				else if (encryptiontext == "OPEN" && ciphersutietext == "NONE")
				{
					window.opener.document.forms['wisp_form'].stadrv_encrypttype.selectedIndex = 0;
				}
				else
				{
					alert(showText(WISPSecuritySupport));
					document.getElementsByName("select")[i].checked=false;
					return false;
				}

				window.opener.document.forms['wisp_form'].stadrv_ssid.value = document.forms['formWlSiteSurvey'].elements['ssid'+i].value;
				window.opener.document.forms['wisp_form'].stadrv_chan.value = parseInt(document.forms['formWlSiteSurvey'].elements['chan'+i].value);
				window.opener.displayObj();
			}
		}
		window.close();
  }
	return false;
}
function loadonstart() {
	
	var sitesurveyed = <%getVar("sitesurveyed","","");%>;
	if (sitesurveyed == 0)
	{
		disableButton(document.formWlSiteSurvey.refresh);
		document.searching.submit();
	}
}
</script>
</head>
  <body class="" onload="loadonstart();">
    <blockquote>
		<p align="center">
    <br>
    <span class="tiltleText"><script>dw(WirelessSiteSurvey)</script></span>
    <br>
    <br>

		<table width="700" border="0" cellspacing="0" cellpadding="0">
			<tr><td>
				<span class="itemText"><script>dw(WirelessSiteSurveyInfo);</script></span>
			</td></tr>
		</table>

		<form action="/goform/formWlSiteSurvey" method="POST" name="searching">
			<input type="hidden" value="Refresh" name="refresh">
			<input type="hidden" value="/wisp_wlsurvey.asp" name="submit-url">
		</form>

		<form action="/goform/formWlSiteSurvey" method="POST" name="formWlSiteSurvey">

		 	<table border='0' width="700" align="center" cellpadding="2" cellspacing="1">
          <tr class="titletableText" >
            <td align="center" height="" ><script>dw(select)</script></td>
            <td align="center" height="" ><script>dw(SSID)</script></td>
            <td align="center" height="" ><script>dw(BSSID)</script></td>
            <td align="center" height="" ><script>dw(Band)</script></td>
            <td align="center" height="" ><script>dw(Channel)</script></td>
            <td align="center" height="" ><script>dw(Type)</script></td>
            <td align="center" height="" ><script>dw(Encryption)</script></td>
            <td align="center" height="" ><script>dw(Signal)</script></td>
          </tr>
		  <% wispSiteSurveyTbl(); %>
		  </table>

		  <br><br>

			<table border='0' align="center" width="700">
				<tr>
					<td align="center">
				 		 <script>document.write('<input type="submit" value="'+showText(refresh)+'" name="refresh" class="ui-button-nolimit">&nbsp');</script>
						 <script>document.write('<input type="button" value="'+showText(Done)+'" name="done" onClick="connectClick()" class="ui-button-nolimit">&nbsp');</script>
						 <script>document.write('<input type="button" value="'+showText(Close)+'" name="close" onClick="window.close();" class="ui-button-nolimit">');</script>
					</td>
				</tr>
			</table>
  
	 		 <input type="hidden" value="/wisp_wlsurvey.asp" name="submit-url">
			 <script>
				 document.forms['formWlSiteSurvey'].elements['done'].disabled = true;
				 document.forms['formWlSiteSurvey'].elements['done'].style.color=disableButtonColor;
			 </script>
				
		</form>

    </blockquote>
  </body>
</html>
