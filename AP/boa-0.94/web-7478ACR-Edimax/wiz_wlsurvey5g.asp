<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<meta http-equiv="X-UA-Compatible" content="IE=9">
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="file/javascript.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script>
var wizmode = <%getVar("wizmode","","");%>;
var rep_EnableCrossBand = "<%getInfo("rep_enableCrossBand");%>";
var ssidTbl=<%getInfo("ssidAll");%>;
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
function connectClick(select)
{
  var i, root_chwidth, root_secchan;
  var apNum=document.forms['formiNICSiteSurvey'].elements['apCount'].value;
	//var apNum=11;
	
	for ( i=0; i<apNum; i++) {
		if (select == "sel"+i) {

			// SSID
			if(wizmode==2){ //REPEATER

				if(parseInt(document.forms['formiNICSiteSurvey'].elements['signal'+i].value,10) <= 40)	{
					alert(showText(signalmorethan40));
					document.forms['formiNICSiteSurvey'].elements['select'+i].checked = false;
					return false;
				}

				if(rep_EnableCrossBand == "1")
				{
					//parent.document.forms['formAutoConnect'].DUTssid.value = ssidTbl[0];
					parent.document.forms['connect'].Rootssid.value = document.forms['formiNICSiteSurvey'].elements['ssid'+i].value.substr(0,32);
				}
				else
				{
					//parent.document.forms['formAutoConnect'].DUTssid.value = ssidTbl[0];
					parent.document.forms['connect'].Rootssid.value = document.forms['formiNICSiteSurvey'].elements['ssid'+i].value.substr(0,32);
				}

				parent.document.forms['connect'].repeaterSSID.value = document.forms['formiNICSiteSurvey'].elements['ssid'+i].value;	
			}
			else{ //WISP, APCLIENT
				parent.document.forms['formAutoConnect'].rootssid.value = document.forms['formiNICSiteSurvey'].elements['ssid'+i].value;
			}

			// RootAP Channel
			if(wizmode==4) //WISP
				parent.document.forms['connect'].stadrv_chan.value = parseInt(document.forms['formiNICSiteSurvey'].elements['chan'+i].value);
			else
				parent.document.forms['connect'].chan.value = parseInt(document.forms['formiNICSiteSurvey'].elements['chan'+i].value);

			// RootAP 40M,80M use up/low
			root_secchan = parseInt(document.forms['formiNICSiteSurvey'].elements['secchan'+i].value, 10);
			root_chwidth = document.forms['formiNICSiteSurvey'].elements['ch_width'+i].value;
			if(root_chwidth == "80M") root_secchan += 2; //INIC_N_CHAN_WIDTH += 2 when 80M, so we can know 40M or 80M
			parent.document.forms['connect'].secchan.value = root_secchan;

			parent.document.forms['connect'].rootAPmac.value = document.forms['formiNICSiteSurvey'].elements['bssid'+i].value;
		

			encryptiontext = document.forms['formiNICSiteSurvey'].elements['encryption'+i].value;
			ciphersutietext = document.forms['formiNICSiteSurvey'].elements['ciphersutie'+i].value;

			if ((encryptiontext == "WPA2PSK") || (encryptiontext == "WPA-PSK/WPA2-PSK")) {  // AES or Mixed
				if((wizmode==2) || (wizmode==3)){ 
					parent.document.forms['connect'].method.value = "3";     // WPA2-PSK
					if (ciphersutietext == "TKIP")
						parent.document.forms['connect'].wpaCipher.value = "1";  // TKIP 
					else if (ciphersutietext == "AES") 
						parent.document.forms['connect'].wpaCipher.value = "2";  // AES 
					else if (ciphersutietext == "AES/TKIP")
						parent.document.forms['connect'].wpaCipher.value = "2";  // AES 
				}
				else{
					parent.document.forms['connect'].stadrv_encrypttype.value = "3";     // WPA2-PSK
					if (ciphersutietext == "TKIP"){
						alert(showText(WISPSecuritySupport));
						document.forms['formiNICSiteSurvey'].elements['select'+i].checked=false;
						return false;
						//parent.document.forms['connect'].stadrv_WPA2cipher.value = "4";  // TKIP 
					}
					else if (ciphersutietext == "AES") 
						parent.document.forms['connect'].stadrv_WPA2cipher.value = "2";  // AES 
					else if (ciphersutietext == "AES/TKIP")
						parent.document.forms['connect'].stadrv_WPA2cipher.value = "2";  // AES 
				}
			}
			else if (encryptiontext == "WPAPSK") {
				if((wizmode==2) || (wizmode==3)){
					parent.document.forms['connect'].method.value = "2";     // WPA-PSK
					if (ciphersutietext == "TKIP")
						parent.document.forms['connect'].wpaCipher.value = "1";  // TKIP 	
					else if (ciphersutietext == "AES") 
						parent.document.forms['connect'].wpaCipher.value = "2";  // AES
					else if (ciphersutietext == "AES/TKIP")
						parent.document.forms['connect'].wpaCipher.value = "2";  // AES 
				}
				else{
					alert(showText(WISPSecuritySupport));
					document.forms['formiNICSiteSurvey'].elements['select'+i].checked=false;
					return false;

					parent.document.forms['connect'].stadrv_encrypttype.value = "2";     // WPA-PSK
					if (ciphersutietext == "TKIP")
						parent.document.forms['connect'].stadrv_WPAcipher.value = "0";  // TKIP 
					else if (ciphersutietext == "AES") 
						parent.document.forms['connect'].stadrv_WPAcipher.value = "3";  // AES 
					else if (ciphersutietext == "AES/TKIP")
						parent.document.forms['connect'].stadrv_WPAcipher.value = "3";  // AES 
				}
			}
			else if (encryptiontext == "OPEN"){
				if (ciphersutietext == "WEP")
				{
					if((wizmode==2) || (wizmode==3)){ 
						parent.document.forms['connect'].method.value = "1";
					}
					else{
						alert(showText(WISPSecuritySupport));
						document.forms['formiNICSiteSurvey'].elements['select'+i].checked=false;
						return false;			
						//parent.document.forms['connect'].stadrv_encrypttype.value = "1";
					}
				}
				else if (ciphersutietext == "NONE") 
				{
					if((wizmode==2) || (wizmode==3))
						parent.document.forms['connect'].method.value = "0";
					else
						parent.document.forms['connect'].stadrv_encrypttype.value = "0";
				}
			}
			else
			{
				if((wizmode==2) || (wizmode==3))
					parent.document.forms['connect'].method.value = "0";
				else
					parent.document.forms['connect'].stadrv_encrypttype.value = "0";
			}
			// can go next step
			parent.gonextEnable=1;
			parent.ButtonDoneEnable();
			parent.setAutoKey();
		}
	}
}
function loadonstart() {

	if (parent.reload_survey != 0)
	{
		parent.reload_survey=0
		document.searching.submit();
	}
}
</script>
</head>
  <!--<body onload="loadonstart();" topmargin="0">-->
	<body topmargin="0">
		<form action="/goform/formiNICSiteSurvey" method="POST" name="formiNICSiteSurvey">
			<div style="height:150px; width:620px; overflow-x:hidden; overflow-y:auto;">
			 	<table border="0" align="center" style="height:110px; width:578px;" cellpadding="2" cellspacing="2">
			 		 <% wliNICSiteSurveyTbl(); %>
				</table>
			</div>
			<input type="hidden" value="/wiz_wlsurvey5g.asp" name="submit-url">
		</form>
		
		<form action="/goform/formiNICSiteSurvey" method="POST" name="searching">
			<input type="hidden" value="Refresh" name="refresh">
			<input type="hidden" value="/wiz_wlsurvey5g.asp" name="submit-url">
		</form>
	
	</body>
</html>
