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

var wlanAcEnable = <%getiNICVar("wlanAcEnabled","","");%>;
var wlanAcNum = <%getiNICVar("wlanAcNum","","");%>;

var nameList = new Array(<% getInfo("getnetName");%> new Array("0","0","0"));
function loadNetList(){
	for(i=0;i<nameList.length-1;i++){
		document.forms['formFilterAdd'].elements['mac_pre'].options[i+1]=new Option (nameList[i][1], nameList[i][2]);
	}
	document.forms['formFilterAdd'].elements['mac_pre'].options[i+1]=new Option ("----"+showText(refresh)+"----", "refresh");
}

function wlanAcSwitch() {
	if (document.formFilterAdd.wlanAcEnabled.checked==true)
	{
		if(confirm(showText(WirelessAccessControlMeg1)))
		{
			document.formWlAcEnabled.wlanAcEnabled.value="ON";
		}
	}
	document.formWlAcEnabled.submit();
}

function checkmac()
{
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

		for (var i=1; i<=wlanAcNum; i++) {
			if( document.getElementById("Client_PC_macAddr"+i).value == str.value.toLowerCase() )
			{
				alert(showText(maccannotsame));
				return false;
			}
		}
	}
	if (check()==false) return false;
	
}
function init()
{	
	$("#help-box").hide();
	var wlsw = <%getVar("wlanDisabled","","");%>;

	if(wlsw == 1)
	{
		document.formWlAcApply.save.disabled = true;
		document.formWlAcApply.save.style.color=disableButtonColor;
		document.formFilterAdd.wlanAcEnabled.disabled = true;

		document.formFilterAdd.addFilterMac.disabled = true;
		document.formFilterAdd.addFilterMac.style.color=disableButtonColor;
		
	}
	/*else  // check nameList
	{
		if( (wlanAcEnable == 1) && (wlanAcNum != 0) && ((nameList.length-1) == 0) )
			document.name_fresh.submit();
	}*/

}
function check() {
	var x = document.formFilterAdd.comment;
	if (x.value != "") {
		if (x.value.search("\'") != -1 || x.value.search("\"") != -1 || x.value.search("\<") != -1 || x.value.search("\>") != -1|| x.value.search("\~") != -1|| x.value.search("\`") != -1) {
				alert(showText(InvalidSomething).replace(/#####/,showText(Comment))+" ( ',\",<,>,~,` )"); 
				//x.value = x.defaultValue;
				x.focus();
				return false;
		}
		else if (x.value.indexOf("?") != -1 || x.value.indexOf("\\") != -1 || x.value.indexOf("%") != -1 || x.value.indexOf("&") != -1) {
			alert(showText(InvalidSomething).replace(/#####/,showText(Comment))+" ( %,?,\\,& )"); 
			//x.value = x.defaultValue;
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
function fresh(select) {
	if (select == "refresh")
		document.name_fresh.submit();
}

function saveChanges(rebootFlag)
{
	with(document.formWlAcApply) {

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}
</SCRIPT>

</head>


<body class="" onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(accessHelpInfor)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>


<blockquote>

		<fieldset name="dynamicip_fieldset" id="dynamicip_fieldset">
			<legend><script>dw(AccessControl)</script></legend>
					<form action="/goform/formiNICAc" method="POST" name="formWlAcEnabled">
						<input type="hidden" value="1" name="iqsetupclose">
						<input type="hidden" value="Add" name="addFilterMac">
						<input type="hidden" value="" name="wlanAcEnabled">
						<input type="hidden" value="/wireless5g_access.asp" name="submit-url">
					</form>
			
					<form action="/goform/formrefresh" method="POST" name="name_fresh">
		        <input type="hidden" value="" name="isApply">
						<input type="hidden" value="/wireless5g_access.asp" name="submit-url">
					</form>

					<form action="/goform/formiNICAc" method="POST" name="formFilterAdd">
					 <input type="hidden" value="1" name="iqsetupclose">
						<table width="700" border="0" cellpadding="2" cellspacing="2" align="center">
							<tr>
								<td colspan="5" style="text-align:left; padding-left:20px;"><input type="checkbox" name="wlanAcEnabled" value="ON" <%getiNICVar("wlanAcEnabled","1","checked");%> onclick="wlanAcSwitch()"><span class="itemText"><script>dw(EnableWlAccessControl)</script></span></td>
							</tr>
							<tr>
								<td colspan="5">&nbsp;</td>
							</tr>
							<tr>
								<input type="hidden" name="tiny_idx" value="0">
									
										<td width="24%" valign="middle" align="center" bgcolor="#FFFFFF" class="">
											<span class="itemText"><script>dw(ClientPC)</script></span><br>
											<select name= "mac_pre" size="1" onChange="fresh(this.value)">
												<option value="------------">----<script>dw(select)</script>----</option>
												<option value="refresh">----<script>dw(refresh)</script>----</option>
											</select>
										</td>
										<td width="5%" valign="middle" align="center" bgcolor="#FFFFFF" class="">
										&nbsp;<br>
										<input type="button" value=">>" class="ui-button-nolimit" style="padding:0;" onclick="if(mac_pre.value!='------------' && mac_pre.value!='000000000000') mac.value=mac_pre.value">
										</td>

										<td width="23%" valign="middle" align="center" bgcolor="#FFFFFF" class="">
											<span class="itemText"><script>dw(macAddr)</script></span><br>
										<input type="text" value="" name="mac" size="12" maxlength="12" style="font-family:Calibri,Arial,Microsoft JhengHei">
										</td>

										<td width="20%" valign="middle" align="center" bgcolor="#FFFFFF" class=""><span class="itemText"><script>dw(Comment)</script></span><br>
										<input type="text" name="comment" size="12" maxlength="16" onchange="check()"></td>
										<td width="20%" valign="middle" align="center" bgcolor="#FFFFFF" class="style22">
										<script>buffer='<input type="submit" value="'+showText(Add)+'" name="addFilterMac" onClick="return checkmac();" class="ui-button-nolimit">';document.write(buffer);</script>
										<script>//buffer='<input type="reset" value="'+showText(clear)+'" name="reset" class="ui-button-nolimit">';document.write(buffer);</script>
										<input type="hidden" value="/wireless5g_access.asp" name="submit-url"></td>
										</tr>
										<tr>
											<td colspan="5">&nbsp;</td>
										</tr>
										<tr>
											<td colspan="5">&nbsp;</td>
										</tr>
						</table>
				</form>
				
				<form action="/goform/formiNICAc" method="POST" name="formFilterDel">
							<input type="hidden" value="1" name="iqsetupclose">
							<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
									<tr>
											<td width="27%" align="center" class="dnsTableHe"><script>dw(macAddr)</script></td>
											<td width="27%" align="center" class="dnsTableHe"><script>dw(DeviceName)</script></td>
											<td width="17%" align="center" class="dnsTableHe"><script>dw(IPAddr)</script></td>
											<td width="19%" align="center" class="dnsTableHe"><script>dw(Comment)</script></td>
											<td width="10%" align="center" class="dnsTableHe"><script>dw(select)</script></td>
									</tr>
							</table>
							<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
									<% wliNICAcList(); %>
							</table>
						
							<table border="0" width="700" align="center">
									<tr>
										<td>&ensp;</td>
									</tr>
									<tr>
										<td colspan="5" align="right">
											<script>buffer='<input type="submit" value="'+showText(deleteSelected)+'" id="deleteSelFilterMac" name="deleteSelFilterMac" onClick="" class="ui-button-nolimit">';document.write(buffer);</script>&nbsp;
											<script>buffer='<input type="submit" value="'+showText(deleteAll)+'" id="deleteAllFilterMac" name="deleteAllFilterMac" onClick="return deleteAllClick()" class="ui-button-nolimit">';document.write(buffer);</script>
										 <script>//buffer='<input type="reset" value="'+showText(Reset)+'" name="reset" class="ui-button-nolimit">';document.write(buffer);</script>
									 </td>
									 <input type="hidden" value="/wireless5g_access.asp" name="submit-url">
									</tr>
							</table>
						 <script>
								var entryNum = wlanAcNum;
								if ( entryNum == 0 ) 
								{
									document.formFilterDel.deleteSelFilterMac.disabled = true;
									document.formFilterDel.deleteAllFilterMac.disabled = true;
									
									document.formFilterDel.deleteSelFilterMac.style.color=disableButtonColor;
									document.formFilterDel.deleteAllFilterMac.style.color=disableButtonColor;
								}
								if ( entryNum >= 20 ) 
								{
									document.formFilterAdd.addFilterMac.disabled = true;
									document.formFilterAdd.addFilterMac.style.color=disableButtonColor;
								}
						 </script>
			 </form>

			<br>

			<form action="/goform/formiNICAc" method="POST" name="formWlAcApply">
				<table align="center" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td style="text-align:center; padding-top:30px;">
							<script>
								document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
								var show_reboot = "<% getInfo("show_reboot"); %>"
								if(show_reboot == "1")
									document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)" > '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
							</script>
							<input type=hidden value="" name="isApply">
							<input type="hidden" value="1" name="iqsetupclose">
							<input type="hidden" value="/wireless5g_access.asp" name="submit-url">
						</td>
					</tr>
				</table>
			</form>

	</fieldset>


<!--==========================================================================================================================================-->
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
