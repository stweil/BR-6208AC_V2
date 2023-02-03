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


var MFenabled = <%getVar("macFilterEnabled","","");%>;
var MFNum = <%getVar("macFilterNum","","");%>;


function init()
{	
	$("#help-box").hide();
}

function addFilterClick() {
	with(document.formFilterAdd) {
		str = mac;
		if (str.value == "" || str.value == "000000000000" || str.value == "ffffffffffff" || str.value == "FFFFFFFFFFFF")
		{
				alert(showText(InvalidSomething).replace(/#####/,showText(macAddr)));
				str.value = str.defaultValue
				str.focus()
				return false
		}

		if ( !macRule(str, 1)) 
		{
			return false;
		}

		for (var i=1; i<=MFNum; i++) {
			if( document.getElementById("Client_PC_macAddr"+i).value == str.value.toLowerCase() )
			{
				alert(showText(maccannotsame));
				return false;
			}
		}

	}
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



function goToWeb() {
	
	if (document.formFilterAdd.enabled.checked==true) {
		if(confirm(showText(WirelessAccessControlMeg1)))
		{
			document.formFilterEnabled.enabled.value="ON";
			//if (macEntryNum == 0)
			//	alert(showText(fwControlAlertMac));
		}
	}
	document.formFilterEnabled.submit();
	
}

function goToIpWeb() { 
	
		if (document.formACPCDel.enabled.checked==true) {
			if(confirm(showText(WirelessAccessControlMeg1)))
			{
				document.formIpFilterEnabled.enabled.value="ON";
				//if (ipEntryNum == 0)
				//	alert(showText(fwControlAlertIp));
			}
		}
		document.formIpFilterEnabled.submit();

}

function openaddpc() {
	//openWindow("/addPC.asp");
	window.open( "/addPC.asp", 'addPC', 'width=800, height=600 , screenX=100, screenY=5 ,resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes');
}

var nameList = new Array(<% getInfo("getnetName");%> new Array("0","0","0"));
function addComputerName(){
		document.forms['formFilterAdd'].elements['mac'].value = document.forms['formFilterAdd'].elements['comList'].value;
}
function loadNetList(){
	for(i=0;i<nameList.length-1;i++){
		document.forms['formFilterAdd'].elements['comList'].options[i+1]=new Option (nameList[i][1], nameList[i][2]);
	}
	document.forms['formFilterAdd'].elements['comList'].options[i+1]=new Option ("----"+showText(refresh)+"----", "refresh");
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
function saveChanges(rebootFlag)
{
	with(document.formFilterApply) {

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}
</SCRIPT>

</head>


<body onLoad="init();">

<!--=====================================================================================================================-->

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(AccControlHelpInfo)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>

<blockquote>

	<fieldset name="AccessControl_fieldset" id="AccessControl_fieldset">
		<legend><script>dw(AccessControl)</script></legend>

					<form action="/goform/formFilter" method="POST" name="formFilterEnabled">
						<input type="hidden" value="1" name="iqsetupclose">
						<input type="hidden" value="Add" name="addFilterMac">
						<input type="hidden" value="" name="enabled">
						<input type="hidden" name="submit-url" value="/fw_access.asp">
						<input type="hidden" value="1" name="filtermode">
						<input type=hidden value="" name="isApply">
					</form>


					<form action="/goform/formFilter" method="POST" name="formIpFilterEnabled">
						<input type="hidden" value="1" name="iqsetupclose">
						<input type="hidden" value="Add" name="addACPC">
						<input type="hidden" value="" name="enabled">
						<input type="hidden" name="submit-url" value="/fw_access.asp">
						<input type="hidden" value="1" name="filtermode">
						<input type="hidden" value="" name="isApply">
					</form>


					<form action="/goform/formFilter" method="POST" name="formFilterAdd">
								<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
										<tr>
											<td colspan="3" height="20" valign="middle" align="left" class=""><span class="itemText">
												<script>
													var MDenyenable = <%getVar("macDenyEnabled","","");%>;
													var MFenable = <%getVar("macFilterEnabled","","");%>;
													
													if (MFenable) document.write('<input type="checkbox" name="enabled" value="ON" onClick="goToWeb();" checked>&nbsp;&nbsp;'+showText(enable)+'&ensp;'+showText(MACFiltering)+'&nbsp;:')
													else  document.write('<input type="checkbox" name="enabled" value="ON" onClick="goToWeb();">&nbsp;&nbsp;'+showText(enable)+'&ensp;'+showText(MACFiltering)+'&nbsp;:')
													if(MDenyenable)
													{
														document.write('<input type="radio" name="macDenyEnabled" value="yes" onClick="document.formFilterAdd.submit();"checked><span class="choose-itemText ">'+showText(Deny)+'&nbsp;&nbsp;')
														document.write('<input type="radio" name="macDenyEnabled" value="no" onClick="document.formFilterAdd.submit();"><span class="choose-itemText ">'+showText(Allow))
													}
													else
													{
														document.write('<input type="radio" name="macDenyEnabled" value="yes" onClick="document.formFilterAdd.submit()"><span class="choose-itemText ">'+showText(Deny)+'</span>&nbsp;&nbsp;')
														document.write('<input type="radio" name="macDenyEnabled" value="no" onClick="document.formFilterAdd.submit()" checked><span class="choose-itemText ">'+showText(Allow)+'</span>')
													}
												</script>
												</span>
											</td>
										</tr>
										<tr>
											<td align="center" class="itemText"><script>dw(ClientPC);</script>&ensp;<script>dw(macAddr);</script></td>
											<td align="center" class="itemText"><script>dw(Computername);</script></td>
											<td align="center" class="itemText"><script>dw(Comment);</script></td>
										</tr>
										<tr>
											<td width="200" height="20" valign="middle" align="center" class=""><input type="text" name="mac" size="15" maxlength="12" onchange=""></td>
											<td width="200" height="20" valign="middle" align="center" class="">
												<input type="button" value="<<" onclick="addComputerName()" class="ui-button-nolimit">
												<select size="1" name= "comList" style="width: 130" onChange="fresh(this.value);">
													<option value="0.0.0.0">---- <script>dw(select);</script>----</option>
													<option value="refresh">----<script>dw(refresh);</script>----</option>
												</select>
											</td>
											<td width="200" height="20" valign="middle" align="center" class="style1"><input type="text" name="comment" size="16" maxlength="16" onchange="check(this)"></td>
										</tr>
										<input type="hidden" name="submit-url" value="/fw_access.asp">
							</table>

							<table width="700" border="0" cellpadding="1" cellspacing="2" align="center">
										<tr>
											<td colspan="4">&nbsp;</td>
										</tr>
										<tr>
											<td colspan="4" align="right">
											<script>
												document.write('<input type="submit" class="ui-button-nolimit" value="'+showText(add)+'" name="addFilterMac" onClick="return addFilterClick();">&nbsp;');
											</script>
											</td>
										</tr>
										<input type="hidden" name="submit-url" value="/fw_access.asp">
										<input type="hidden" value="1" name="filtermode">
										<input type="hidden" value="" name="isApply">
							</table>
					<input type="hidden" value="1" name="iqsetupclose">
				</form>
			
				<br>

				<form action="/goform/formFilter" method="POST" name="formFilterDel">

								<script>
									if(MFenable) document.write('<input type="hidden" name="enabled" value="ON"')
									else  document.write('<input type="hidden" name="enabled" value=""')
								</script>

								<table width="700" border="0" cellpadding="0" cellspacing="1" class="">
										<tr>
											<td colspan="5" height="20" valign="middle" align="left"><span class="itemText"><script>dw(MACFiltering)</script>&ensp;<script>dw(Table)</script>&nbsp;:</span></td>
										</tr>
								</table>

								<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
										<tr align="center" class="dnsTableHe">
											<td width="5%"><script>dw(NO)</script></td>
											<td width="30%"><script>dw(Computername)</script></td>
											<td width="30%"><script>dw(ClientPC);</script>&ensp;<script>dw(macAddr);</script></td>
											<td width="25%"><script>dw(Comment)</script></td>
											<td width="10%"><script>dw(select)</script></td>
										</tr>	
								</table>
								<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
										<% macFilterList(); %>
								</table>

								<table width="700" border="0" cellpadding="0" cellspacing="1">
										<tr>
											<td>
												&nbsp;
											</td>
										</tr>
										<tr>
											<td colspan="5" align="right">
												<script>
													document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="deleteSelFilterMac" onClick="" class="ui-button-nolimit">&nbsp;');
													document.write('<input type="submit" value="'+showText(deleteAll)+'" name="deleteAllFilterMac" onClick="return deleteAllClick()" class="ui-button-nolimit">');
												</script>
											</td>
										</tr>
										<script>
											var macEntryNum = <%getVar("macFilterNum","","");%>;
											if ( macEntryNum == 0 ) 
											{
												document.formFilterDel.deleteSelFilterMac.disabled = true;
												document.formFilterDel.deleteAllFilterMac.disabled = true;
									
												document.formFilterDel.deleteSelFilterMac.style.color=disableButtonColor;
												document.formFilterDel.deleteAllFilterMac.style.color=disableButtonColor;
											}
											if ( macEntryNum >= 20 ) 
											{
												document.formFilterAdd.addFilterMac.disabled = true;
												document.formFilterAdd.addFilterMac.style.color=disableButtonColor;
											}
										</script>
								</table>
								<input type="hidden" value="1" name="iqsetupclose">
								<input type="hidden" value="1" name="filtermode">
								<input type="hidden" name="submit-url" value="/fw_access.asp">
								<input type="hidden" value="" name="isApply">
				</form>


				<form action="/goform/formFilter" method="POST" name="formACPCDel">
								<table width="700" border="0" cellpadding="2" cellspacing="2" align="center">
										<tr>
											<td colspan="">&nbsp;</td>
										</tr>
										<tr>
											<td colspan="">&nbsp;</td>
										</tr>
										<tr>
											<td height="20" valign="middle" align="left" class=""><span class="itemText">  
												<input type="checkbox" name="enabled" value="ON" <%getVar("ACPCEnabled","1","checked");%> onClick="goToIpWeb()">&nbsp;&nbsp;<script>dw(enable);</script>&ensp;<script>dw(IPFilteringTable);</script>&nbsp;:
												<script>
													var IPDenyenable = <%getVar("ipDenyEnabled","","");%>;
													if(IPDenyenable)
													{
														document.write('<input type="radio" name="ipDenyEnabled" value="yes" onClick="document.formACPCDel.submit();" checked><span class="choose-itemText">'+showText(Deny)+'</span>&nbsp;&nbsp;')
														document.write('<input type="radio" name="ipDenyEnabled" value="no" onClick="document.formACPCDel.submit();"><span class="choose-itemText">'+showText(Allow)+'</span>')
													}
													else
													{
														document.write('<input type="radio" name="ipDenyEnabled" value="yes" onClick="document.formACPCDel.submit();"><span class="choose-itemText">'+showText(Deny)+'</span>&nbsp;&nbsp;')
														document.write('<input type="radio" name="ipDenyEnabled" value="no" onClick="document.formACPCDel.submit();" checked><span class="choose-itemText ">'+showText(Allow)+'</span>')
													}
												</script>
											</span></td>
										</tr>
								</table>


								<table width="700" border="0" cellpadding="0" cellspacing="1" class="">
										<tr><td colspan="7" height="20" valign="middle" align="left"><span class="itemText"><script>dw(IPFilteringTable);</script>&nbsp;:</span></td></tr>
								</table>


								<table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
										<tr align="center" class="dnsTableHe">
											<td width="5%"><script>dw(NO)</script></td>
											<td width="22%"><script>dw(ClientPC);</script>&ensp;<script>dw(Description);</script></td>
											<td width="22%"><script>dw(ClientPC);</script>&ensp;<script>dw(IPAddr);</script></td>
											<td width="15%"><script>dw(ClientService);</script></td>
											<td width="13%"><script>dw(Protocol);</script></td>
											<td width="13%"><script>dw(PortRange);</script></td>
											<td width="10%"><script>dw(select);</script></td>
										</tr>
								</table>
								<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
										<% ACPCList(); %>
								</table>

								<table width="700" border="0" cellpadding="1" cellspacing="1" class="style14">
										<tr>
											<td>
												&nbsp;
											</td>
										</tr>
										<tr>
											<td align="right">
												<script>
													document.write('<input type="button" value="'+showText(AddPC)+'" name ="addPc" onClick="openaddpc();" class="ui-button-nolimit">&nbsp;');
													document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="deleteSelACPC" onClick="" class="ui-button-nolimit">&nbsp;');
													document.write('<input type="submit" value="'+showText(deleteAll)+'" name="deleteAllACPC" onClick="return deleteAllClick()" class="ui-button-nolimit">')
												</script>
											</td>
										</tr>
										<script>
											var ipEntryNum = <%getVar("ACPCNum","","");%>;
											if ( ipEntryNum == 0 ) 
											{
												document.formACPCDel.deleteSelACPC.disabled = true;
												document.formACPCDel.deleteAllACPC.disabled = true;
									
												document.formACPCDel.deleteSelACPC.style.color=disableButtonColor;
												document.formACPCDel.deleteAllACPC.style.color=disableButtonColor;
											}
											if ( ipEntryNum >= 20 ) 
											{
												document.formACPCDel.addPc.disabled = true;
												document.formACPCDel.addPc.style.color=disableButtonColor;
											}
										</script>
								</table>
								<input type="hidden" value="1" name="iqsetupclose">
								<input type="hidden" name="submit-url" value="/fw_access.asp">
								<input type="hidden" value="1" name="filtermode">
								<input type="hidden" value="" name="isApply">
				</form>

				<form action="/goform/formrefresh" method="POST" name="name_fresh">
					<input type="hidden" name="submit-url" value="/fw_access.asp">
					<input type="hidden" value="1" name="filtermode">
					<input type="hidden" value="" name="isApply">
				</form>

				<br>

				<form action="/goform/formFilter" method=POST name="formFilterApply">
					<table align="center" border="0" cellspacing="0" cellpadding="0">
						<tr>
							<td style="text-align:center; padding-top:30px;">
								<script>
									//document.write('<input type="submit" value="'+showText(Apply)+'" name="new_apply" class="ui-button">')
									document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
								</script>
								<input type="hidden" value="" name="isApply">
								<input type="hidden" value="/fw_access.asp" name="submit-url">
								<input type="hidden" value="1" name="filtermode">
							</td>
						</tr>
					</table>
					<input type="hidden" value="1" name="iqsetupclose">
				</form>

	</fieldset>


<!--=====================================================================================================================-->
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
