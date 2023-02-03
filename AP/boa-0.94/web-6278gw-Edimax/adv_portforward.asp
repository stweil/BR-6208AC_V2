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

var portFwEnable = <%getVar("portFwEnabled","","");%>;
var portFwNum = <%getVar("portFwNum","","");%>;

function disableDelButton()
{
	document.formPortFwDel.deleteSelPortFw.disabled = true;
	document.formPortFwDel.deleteAllPortFw.disabled = true;
}
function addClick()
{
strIp = document.formPortFwAdd.ip;
strFromPort = document.formPortFwAdd.fromPort;
strToPort = document.formPortFwAdd.toPort;
strComment = document.formPortFwAdd.comment;
	if (strIp.value=="" && strFromPort.value=="" &&	strToPort.value=="" && strComment.value=="" )
		return true;
	if ( !ipRule( strIp, showText(IPAddr), "ip", 0))
		return false;
	if (!portRule(strFromPort, showText(PortRange), strToPort, showText(PortRange), 1, 65535, 0))
		return false;
	return true;
}
function goToWeb() {
  document.formPortFwAdd.submit();
}
function goToApply() {
	if(document.formPortFwAdd.enabled.checked == true)
		document.formPortFwApply.enabled.value = "ON"
	document.formPortFwApply.submit();
}
//Support NetBiosName add by Kyle 2007/12/12
var nameList = new Array(<% getInfo("getnetName");%> new Array("0","0","0"));
function addComputerName(){
		document.forms['formPortFwAdd'].elements['ip'].value = document.forms['formPortFwAdd'].elements['comList'].value;
}
function loadNetList(){
	for(i=0;i<nameList.length-1;i++){
		document.forms['formPortFwAdd'].elements['comList'].options[i+1]=new Option (nameList[i][1], nameList[i][0]);
	}
	document.forms['formPortFwAdd'].elements['comList'].options[i+1]=new Option ("----"+showText(refresh)+"----", "refresh");
}
function searchComName(ipAdr,type){
	//var comName="OFFLINE";
	var comName="–";
	for (i=0;i<nameList.length-1;i++){
		if(ipAdr==nameList[i][type]){
			comName=nameList[i][1];
			break;
		}
	}
	return comName;
}
function fresh(select) {
	if (select == "refresh")
		document.name_fresh.submit();
}
function init()
{	
	$("#help-box").hide();
	//if( (portFwEnable == 1) && (portFwNum != 0) && ((nameList.length-1) == 0) )
	//	document.name_fresh.submit();
}
function check(input) {
	var x = input
	if (x.value != "") {
		if (x.value.search("\'") != -1 || x.value.search("\"") != -1 || x.value.search("\<") != -1 || x.value.search("\>") != -1|| x.value.search("\~") != -1|| x.value.search("\`") != -1) {
				alert(showText(InvalidSomething).replace(/#####/,showText(Comment))); 
				x.value = x.defaultValue;
				x.focus();
				return false;
		}
		else if (x.value.indexOf("?") != -1 || x.value.indexOf("\\") != -1) {
			alert(showText(InvalidSomething).replace(/#####/,showText(Comment))); 
			x.value = x.defaultValue;
			x.focus();
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}
function saveChanges(rebootFlag)
{
	with(document.formPortFwApply) {

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}
</SCRIPT>

</head>


<body onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(PortForwardingHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<fieldset name="StaticRout_fieldset" id="StaticRout_fieldset">
		<legend><script>dw(PortForwarding)</script></legend>

			<form action="/goform/formPortFw" method="POST" name="formPortFwAdd">

				<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
					<tr>
						<td colspan="2" height="20" valign="middle" align="left" class=""><span class="itemText">
							<input type="checkbox" name="enabled" value="ON" <%getVar("portFwEnabled","1","checked");%> onClick="goToWeb();">&nbsp;<script>dw(enable)</script>&ensp;<script>dw(PortForwarding)</script>
						</span></td>
					</tr>
					<tr align="center" class="itemText">
						<td width="15%"><script>dw(PrivateIP)</script></td>
						<td width="32%"><script>dw(Computername)</script></td>
						<td width="10%"><script>dw(Type)</script></td>
						<td width="28%"><script>dw(PortRange)</script></td>
						<td width="15%"><script>dw(Comment)</script></td>
					</tr>
					<tr>
						<td height="20" valign="middle" align="left" class=""><input type="text" name="ip" size="12" maxlength="15"></td>
						<td height="20" valign="middle" align="center" class="">
							<input type="button" value="<<" name="addCom" onclick="addComputerName()" class="ui-smal-button" style="width: 50">
							<select size="1" name= "comList" onchange="fresh(this.value)" style="width: 130">
								<option value="0.0.0.0">----<script>dw(select)</script>----</option>
								<option value="refresh">----<script>dw(refresh)</script>----</option>
							</select>
						</td>
						<td height="20" valign="middle" align="left" class="">
							<select name="protocol">
								<option selected value="0"><script>dw(Both)</script></option>
								<option value="1"><script>dw(TCP)</script></option>
								<option value="2"><script>dw(UDP)</script></option>
							</select>
						</td>
						<td height="20" valign="middle" align="center" class="">
							<input type="text" name="fromPort" size="5" maxlength="5">-<input type="text" name="toPort" size="5" maxlength="5">
						</td>
						<td height="20" valign="middle" align="left" class="">
							<input type="text" name="comment" size="15" maxlength="15" onchange="check(this)">
						</td>
					</tr>
				</table>
	
				<br>

				<table width="700" border="0" cellpadding="0" cellspacing="0" align="center">
					<tr>
						<td height="20" valign="middle" align="right" class="">
							<script>
								document.write('<input type="submit" value="'+showText(add)+'" name="addPortFw" onClick="return addClick()" class="ui-button-nolimit">&nbsp;');
								//document.write('<input type="reset" value="'+showText(clear)+'" name="reset" onClick="document.formPortFwAdd.reset;" class="ui-button-nolimit">');
							</script>
							<input type="hidden" value="" name="isApply">
							<input type="hidden" value="/adv_portforward.asp" name="submit-url">
						</td>
					</tr>
				</table>
			<input type="hidden" value="1" name="iqsetupclose">
		</form>

		<form action="/goform/formPortFw" method="POST" name="formPortFwDel">
			<script>
				if(portFwEnable) document.write('<input type="hidden" name="enabled" value="ON">')
				else  document.write('<input type="hidden" name="enabled" value="">')
			</script>

			<table width="700" border="0" cellpadding="2" cellspacing="2" align="center">
				<tr>
					<td width="200" height="20" valign="middle" align="left" class=""><span class="itemText"><script>dw(CurPortForwTable)</script>&nbsp;:<span></td>
				</tr>
			</table>

			<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
      	<tr align="center" class="dnsTableHe">
					<td width="5%" ><script>dw(NO)</script></td>
					<td width="25%" ><script>dw(Computername)</script></td>
					<td width="14%" ><script>dw(PrivateIP)</script></td>
					<td width="12%" ><script>dw(Type)</script></td>
					<td width="12%" ><script>dw(PortRange)</script></td>
					<td width="22%" ><script>dw(Comment)</script></td>
					<td width="10%" ><script>dw(select)</script></td>
				</tr>
			</table>
			<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
				<% portFwList(); %>
			</table>

			<br>

			<table width="700" border="0" cellpadding="0" cellspacing="0" align="center">
				<tr>
					<td height="20" valign="middle" align="right" class="">
						<script>
							document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="deleteSelPortFw" onClick="" class="ui-button-nolimit">&nbsp;');
							document.write('<input type="submit" value="'+showText(deleteAll)+'" name="deleteAllPortFw" onClick="return deleteAllClick()" class="ui-button-nolimit">');
						</script>

						<script>
							entryNum = <%getVar("portFwNum","","");%>;
							if ( entryNum == 0 ) 
							{
								document.formPortFwDel.deleteSelPortFw.disabled = true;
								document.formPortFwDel.deleteAllPortFw.disabled = true;
								
								document.formPortFwDel.deleteSelPortFw.style.color=disableButtonColor;
								document.formPortFwDel.deleteAllPortFw.style.color=disableButtonColor;
							}
							if ( entryNum >= 20 ) 
							{
								document.formPortFwAdd.addPortFw.disabled = true;
								document.formPortFwAdd.addPortFw.style.color=disableButtonColor;
							}
					 </script>
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="/adv_portforward.asp" name="submit-url">
					</td>
				</tr>
			</table>
			<input type="hidden" value="1" name="iqsetupclose">
		</form>

		<form action="/goform/formrefresh" method="POST" name="name_fresh">
			<input type="hidden" name="submit-url" value="/adv_portforward.asp">
			<input type="hidden" value="" name="isApply">
		</form>

		<br>

		<form action="/goform/formPortFw" method="POST" name="formPortFwApply">
			<table align="center" border="0" cellspacing="0" cellpadding="0">
				<tr>
					<td style="text-align:center; padding-top:30px;">
						<script>
							if(portFwEnable) document.write('<input type="hidden" name="enabled" value="ON">');
							else  document.write('<input type="hidden" name="enabled" value="">');
							//document.write('<input type="submit" value="'+showText(Apply)+'" name="new_apply" class="ui-button">')
							document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
						</script>
						<input type="hidden" value="" name="isApply">
					<input type="hidden" value="/adv_portforward.asp" name="submit-url">
					</td>
				</tr>
			</table>
			<input type="hidden" value="1" name="iqsetupclose">
		</form>
	</fieldset>
			
				



<script>
	$("#help-box").hide();
	loadNetList();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
