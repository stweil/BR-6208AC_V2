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



var virtSVEnable = <%getVar("virtSVEnabled","","");%>;
var vserNum = <%getVar("vserNum","","");%>;
function addClick()
{
	if (document.formPortFwAdd.ip.value=="" && document.formPortFwAdd.fromPort.value=="" &&
		document.formPortFwAdd.toPort.value=="" && document.formPortFwAdd.comment.value=="" )
		return true;
	if ( !ipRule( document.formPortFwAdd.ip, showText(IPAddr), "ip", 0))
		return false;
	if (!portRule(document.formPortFwAdd.fromPort, showText(PrivatePort), 0, "", 1, 65535, 0))
		return false;
	if (!portRule(document.formPortFwAdd.toPort, showText(PublicPort), 0, "", 1, 65535, 0))
		return false;
	//Fixed remote management port the same with Virtual Server. Kyle 20081027
	var remoteManagementPort = <% getInfo("reManPort");%>
	var remoteManagementEnable = <%getVar("reMangEnable","","");%>;
	if(remoteManagementEnable == 1 && document.formPortFwAdd.toPort.value == remoteManagementPort ){
		alert("Port "+remoteManagementPort +" is the same with remote management port");
		return false;
	}
	//end Fixed
   return true;
}
function disableDelButton()
{
	document.formPortFwDel.deleteSelPortFw.disabled = true;
	document.formPortFwDel.deleteAllPortFw.disabled = true;
}
function goToWeb() {
	document.formPortFwAdd.submit();
}
function fresh(select) {
	if (select == "refresh")
		document.name_fresh.submit();
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
function init()
{	
	$("#help-box").hide();
	//if( (virtSVEnable == 1) && (vserNum != 0) && ((nameList.length-1) == 0) )
	//	document.name_fresh.submit();
}
function saveChanges(rebootFlag)
{
	with(document.formVirApply) {

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
				<tr><td><div class="help-text" id="help-info1"><script>dw(VirtualServerhelp)</script>
				</div></td></tr>
			</table>
		</div>
		<script>
		HelpButton();
		</script>



<blockquote>

	<fieldset name="StaticRout_fieldset" id="StaticRout_fieldset">
		<legend><script>dw(VirtualServer)</script></legend>

			<form action="/goform/formVirtualSv" method="POST" name="formPortFwAdd">
				<table width="700" border="0" cellpadding="0" cellspacing="0" align="center">
					<tr>
						<td colspan="6" height="20" valign="middle" align="left" class="">
							<input type="checkbox" name="enabled" value="ON" <%getVar("virtSVEnabled","1","checked");%> onClick="goToWeb();">  
							<span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(VirtualServer)</script></span>
						</td>
					</tr>
				</table>
				<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
					<tr align="center" class="itemText">
						<td ><script>dw(PrivateIP)</script></td>
						<td ><script>dw(Computername)</script></td>
						<td ><script>dw(PrivatePort)</script></td>
						<td ><script>dw(Type)</script></td>
						<td ><script>dw(PublicPort)</script></td>
						<td ><script>dw(Comment)</script></td>
					</tr>
					<tr align="center">
						<td bgcolor="#FFFFFF" class=""><input type="text" name="ip" size="13" maxlength="15"></td>
						<td bgcolor="#FFFFFF" class="">
							<input type="button" value="<<" onclick="addComputerName()" class="ui-smal-button">
							<select size="1" name= "comList" onchange="fresh(this.value)" style="width: 130">
								<option value="0.0.0.0">----<script>dw(select)</script>----</option>
								<option value="refresh">----<script>dw(refresh)</script>----</option>
							</select>
						</td>
						<td bgcolor="#FFFFFF" class=""><input type="text" name="fromPort" size="3" maxlength="5"></td>
						<td bgcolor="#FFFFFF" class="">
							<select name="protocol">
								<option selected value="0"><script>dw(Both)</script></option>
								<option value="1"><script>dw(TCP)</script></option>
								<option value="2"><script>dw(UDP)</script></option>
							</select>
						</td>
						<td bgcolor="#FFFFFF" class=""><input type="text" name="toPort" size="3" maxlength="5"></td>
						<td bgcolor="#FFFFFF" class=""><input type="text" name="comment" size="10" maxlength="10" onchange="check(this)"></td>
					</tr>
				<tr align="right">
					<td colspan="6">
						<input type="submit" value="" name="addPortFw" onClick="return addClick()" class="ui-button-nolimit">
						<!--<input type="reset" value="" name="reset" onClick="document.formPortFwAdd.reset;" class="ui-button-nolimit">-->
						<script language ="javascript">
							document.formPortFwAdd.addPortFw.value =showText(add);
							//document.formPortFwAdd.reset.value =showText(clear);
						</script>
					</td>
				</tr>
				</table>
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="/adv_virtserver.asp" name="submit-url">				
			</form>

			<br>

			<form action="/goform/formVirtualSv" method="POST" name="formPortFwDel">
				<input type="hidden" name="enabled" value="">
				<script> 
					virtSVEnable = <%getVar("virtSVEnabled","","");%>;
					if(virtSVEnable)
						document.formPortFwDel.enabled.value = "ON";
				</script>

				<table width="700" border="0" cellpadding="0" cellspacing="1" align="center">
					<tr><td colspan="8" align="left" class=""><span class="itemText"><script>dw(CurVirtSerTable)</script>&nbsp;:</span></td></tr>
				</table>

				<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
      		<tr align="center" class="dnsTableHe">
						<td width="5%"><script>dw(NO)</script></td>
						<td width="20%"><script>dw(Computername)</script></td>
						<td width="18%"><script>dw(PrivateIP)</script></td>
						<td width="10%"><script>dw(PrivatePort)</script></td>
						<td width="10%"><script>dw(Type)</script></td>
						<td width="10%"><script>dw(PublicPort)</script></td>
						<td width="17%"><script>dw(Comment)</script></td>
						<td width="10%"><script>dw(select)</script></td>
					</tr>
				</table>

				<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
					<% virtualSvList(); %>
				</table>

				<p align="right">
					<script>
						document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="deleteSelPortFw" onClick="" class="ui-button-nolimit">&nbsp;');
						document.write('<input type="submit" value="'+showText(deleteAll)+'" name="deleteAllPortFw" onClick="return deleteAllClick()" class="ui-button-nolimit">');
					</script>
				</p>
				<script>	
					entryNum = <%getVar("vserNum","","");%>;
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
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="/adv_virtserver.asp" name="submit-url">
			</form>

			<form action="/goform/formrefresh" method="POST" name="name_fresh">
				<input type="hidden" value="/adv_virtserver.asp" name="submit-url">
				<input type="hidden" value="" name="isApply">
			</form>

			<br>

			<form action="/goform/formVirtualSv" method="POST" name="formVirApply">
				<table align="center" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td style="text-align:center; padding-top:30px;">
							<script>
								if(virtSVEnable) document.write('<input type="hidden" name="enabled" value="ON">');
								else  document.write('<input type="hidden" name="enabled" value="">');
								//document.write('<input type="submit" value="'+showText(Apply)+'" name="new_apply" class="ui-button">')
								document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
							</script>
							<input type="hidden" value="" name="isApply">
							<input type="hidden" value="/adv_virtserver.asp" name="submit-url">
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
