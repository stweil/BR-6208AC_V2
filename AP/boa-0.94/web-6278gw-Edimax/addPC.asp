<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<% language=javascript %>

<script>
var nameTbl = new Array(16);
nameTbl[0] = "WWW";
nameTbl[1] = "E-mail Sending";
nameTbl[2] = "News Forums";
nameTbl[3] = "E-mail Receiving";
nameTbl[4] = "Secure HTTP";
nameTbl[5] = "File Transfer";
nameTbl[6] = "MSN Messenger";
nameTbl[7] = "Telnet Service";
nameTbl[8] = "AIM";
nameTbl[9] = "NetMeeting";
nameTbl[10] = "DNS";
nameTbl[11] = "SNMP";
nameTbl[12] = "VPN-PPTP";
nameTbl[13] = "VPN-L2TP";
nameTbl[14] = "TCP";
nameTbl[15] = "UDP";
var desTbl = new Array(16);
desTbl[0] = "HTTP, TCP Port 80, 3128, 8000, 8080, 8081";
desTbl[1] = "SMTP, TCP Port 25";
desTbl[2] = "NNTP, TCP Port 119";
desTbl[3] = "POP3, TCP Port 110";
desTbl[4] = "HTTPS, TCP Port 443";
desTbl[5] = "FTP, TCP Port 21, 20";
desTbl[6] = "TCP Port 1863";
desTbl[7] = "TCP Port 23";
desTbl[8] = "AOL Instant Messenger, TCP Port 5190";
desTbl[9] = "H.323, TCP Port 389,522,1503,1720,1731";
desTbl[10] = "UDP Port 53";
desTbl[11] = "UDP Port 161, 162";
desTbl[12] = "TCP Port 1723";
desTbl[13] = "UDP Port 1701";
desTbl[14] = "All TCP Port";
desTbl[15] = "All UDP Port";

function checkValue(str)
{
   for (var i=0; i<str.length; i++) {
    if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
    		(str.charAt(i) == ',' ) || (str.charAt(i) == '-' ))
			continue;
	return 0;
  }
  return 1;
}

function addClick()
{
	if (document.formFilterAdd.sipaddr.value==""){
		alert(showText(fwaddpcAlertIpEmpty));
		document.formFilterAdd.sipaddr.value = document.formFilterAdd.sipaddr.defaultValue;
		document.formFilterAdd.sipaddr.focus();
		return false;
	}
	if ( checkIpAddr(document.formFilterAdd.sipaddr, showText(InvalidStartIP)) == false )
    	return false;
	if (document.formFilterAdd.eipaddr.value!=""){
		if ( checkIpAddr(document.formFilterAdd.eipaddr, showText(InvalidEndIP)) == false )
    		return false;
	}

	if (!portRule(document.formFilterAdd.sipaddr, showText(Bandwidth), 0, "", 1, 99999, 0))
		return false;
//=============================================================================================
//  if (!document.formFilterAdd.enabled.checked)
//  	return true;
	var longVal=0;
	with (document.formFilterAdd ){
		if (Index0.checked==true)	longVal |=0x0001;
		if (Index1.checked==true)	longVal |=0x0002;
		if (Index2.checked==true)	longVal |=0x0004;
		if (Index3.checked==true)	longVal |=0x0008;
		if (Index4.checked==true)	longVal |=0x0010;
		if (Index5.checked==true)	longVal |=0x0020;
		if (Index6.checked==true)	longVal |=0x0040;
		if (Index7.checked==true)	longVal |=0x0080;
		if (Index8.checked==true)	longVal |=0x0100;
		if (Index9.checked==true)	longVal |=0x0200;
		if (Index10.checked==true)	longVal |=0x0400;
		if (Index11.checked==true)	longVal |=0x0800;
		if (Index12.checked==true)	longVal |=0x1000;
		if (Index13.checked==true)	longVal |=0x2000;
		if (Index14.checked==true)	longVal |=0x4000;
		if (Index15.checked==true)	longVal |=0x8000;
	}
	document.formFilterAdd.serindex.value=longVal;
	if ( checkValue( document.formFilterAdd.Port.value ) == 0 ){
		alert(''+showText(InvalidSomething).replace(/#####/,showText(PortRange))+' '+showText(shouldbeSomething).replace(/####/,showText(decimalnumber)));
		document.formFilterAdd.Port.focus();
		return false;
	}
 return true;
}
</script>
</head>
<body topmargin="10">
<blockquote>
<p align="center"><b><span class="tiltleText"><script>dw(AccessControlAddPC);</script></span></b></p>

	<table border="0" width="600" cellspacing="0" cellpadding="0" align="center">
		<tr><td><span class="itemText"><script>dw(fwAccAddInfo);</script></span></td></tr>
	</table>
	
	<br>	

	<table width="600" border="0" cellpadding="2" cellspacing="2" align="center">
		<tr><td align="left"><b><span class="itemText"><script>dw(AccessControlAddPC)</script>&nbsp;:</sapn></b></td></tr>
	</table>

	<form action="/goform/formFilter" method="POST" name="formFilterAdd">
		<script>
			var ACPCEnable = <%getVar("ACPCEnabled","","");%>;
			if(ACPCEnable)
				document.write('<input type="hidden" value="ON" name="enabled">');
			else
				document.write('<input type="hidden" value="" name="enabled">');
		</script>
			<input type="hidden" name="enabled" value="<% %>">

	<table width="600" border="0" cellpadding="2" cellspacing="2" align="center">
		<tr>
			<td width="200" height="20" valign="middle" align="right" style="padding-right:10px;"><span class="itemText"><script>dw(clientPcDescription)</script></span></td>
			<td width="400" height="20" valign="middle" align="left">
				<input type="text" name="comment" size="20" maxlength="16">
			</td>
		</tr>
		<tr>
			<td width="200" height="20" valign="middle" align="right" style="padding-right:10px;"><span class="itemText"><script>dw(clientPcIpAddress)</script></span></td>
			<td width="400" height="20" valign="middle" align="left" class="">
				<input type="text" name="sipaddr" size="15" maxlength="15">&nbsp;<b><span class="itemText">-</span></b>&nbsp;
				<input type="text" name="eipaddr" size="15" maxlength="15">
			</td>
		</tr>
	</table>

	<table width="600" border="0" cellpadding="0" cellspacing="2" align="center" class="style14">
		<tr><td colspaN="3" height="20" valign="middle" align="left"><b><span class="itemText"><script>dw(clientPcService)</script>&nbsp;:</span></b></td></tr>
	</table>
	
	<table width="600" border="0" cellpadding="1" cellspacing="2" align="center">
		<tr>
			<td align="center" width="200" height="20" class="titletableText"><script>dw(serviceName)</script></td>
			<td align="center" width="300" height="20" class="titletableText"><script>dw(detailDescription)</script></td>
			<td align="center" width="100" height="20" class="titletableText"><script>dw(select)</script></td>
		</tr>
			<script>
				for(i=0;i<16;i++)
				{
					document.write("<tr align=center class=\"listtableText\">");
					document.write("<td align=\"center\">" + nameTbl[i] + "</font></td>");
					document.write("<td align=\"left\">" + desTbl[i] + "</font></td>");
					document.write("<td><input type=\"checkbox\" name=\"Index"+i+"\"></td></tr>");
				}
			</script>
	</table>

	<table width="600" border="0" cellpadding="2" cellspacing="2" align="center">
		<tr>
			<td colspaN="3" height="20" valign="middle" align="left" class=""><b><span class="itemText"><script>dw(usrDefineService)</script>&nbsp;:</span></b>
			</td>
		</tr>
		<tr>
			<td width="300" height="20" valign="middle" align="right" class="" style="padding-right:0px;"><span class="itemText"><script>dw(Protocol)</script></span></td>
			<td width="300" height="20" valign="middle" align="left" class="">
				<select name="protocol">
					<option selected value="0"><script>dw(Both);</script></option>
					<option value="1">TCP</option>
					<option value="2">UDP</option>
				</select>
			</td>
		</tr>
		<tr>
			<td width="300" height="20" valign="middle" align="right" class="" style="padding-right:0px;"><span class="itemText"><script>dw(PortRange)</script></span></td>
			<td width="300" height="20" valign="middle" align="left" class="">
				<input type="text" name="Port" size="60" maxlength="59">
			</td>
		</tr>
		<tr>
			<td colspan="5" align="center">
				<script>
					document.write('<input type="submit" class="ui-button-nolimit" value="'+showText(add)+'" name="addACPC" onClick="return addClick()">&nbsp;');
					//document.write('<input type="reset" class="ui-button-nolimit" value="'+showText(clear)+'" name="reset">');
				</script>
				<input type="hidden" name="serindex">
				<input type="hidden" value="frame_1" name="framename">
			</td>
		</tr>
	</table>
	<input type="hidden" value="1" name="filtermode">
</form>
</blockquote>
</body>
</html>
