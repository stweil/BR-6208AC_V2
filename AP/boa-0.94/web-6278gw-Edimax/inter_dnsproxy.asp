<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<% language=javascript %>
<SCRIPT>

var entryNum = <%getVar("DNSProxyrulesNum","","");%>;
var DNSProxyTempRulesNum = <%getVar("DNSProxyTempRulesNum","","");%>;
var DownLoadURL = <% getInfo("DownLoadURL"); %>;
var enable_http_proxy = <%getVar("enable_dns_proxy","","");%>;
var enable_user_define_rules = <%getVar("enable_user_define_rules","","");%>;
var rulelimit=50;
function CheckExistRule(AddDomainName,AddServerIP)
{
	var rulesNum = document.ProxyrulesDel.rulesnum.value;
	for(j=1;j<=rulesNum;j++)
	{
		DomainName = document.getElementById('domainName' + j ).value
		ServerIP = document.getElementById('ProxyServerIP' + j ).value
		if( ( ServerIP.toLowerCase() == AddServerIP.toLowerCase() ) && ( DomainName.toLowerCase() == AddDomainName.toLowerCase() ) )
		{
			return 'Exist';
		}
	}
	return 'Not_Exist';
}

function DNSProxyClicked()
{
	with(document.dnsproxy) {
		if(ProxyruleEnabled[0].checked == true)
		{
			url1.disabled = true;
			UserDefineRulesEnabled[0].disabled = true;
			UserDefineRulesEnabled[1].disabled = true;
		}
		else
		{
			url1.disabled = false;
			UserDefineRulesEnabled[0].disabled = false;
			UserDefineRulesEnabled[1].disabled = false;
		}
	}
	SelectRulesClicked();
}

function saveChanges(rebootFlag)
{
	with(document.dnsproxy) {

	
		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}

function addIndex()
{
	var RulesNum = document.TemporaryProxyrules.TempRulesNum.value;
	var AllSelIndex='',AddIndexCount=0;
	for(i=1;i<=RulesNum;i++)
	{
		if( document.getElementById('TempSelect' + i ).checked == true)
		{
				var results = CheckExistRule(document.getElementById('TempDomainName' + i ).value,document.getElementById('TempProxyServerIP' + i ).value);
				if( results == 'Not_Exist' )
				{
					if( AllSelIndex != '' )
						AllSelIndex = AllSelIndex + document.getElementById('TempIndex' + i ).value +',';
					else
						AllSelIndex = document.getElementById('TempIndex' + i ).value + ',' ;

					AddIndexCount = AddIndexCount+1;
				}
		}
	}
	if( rulelimit < ( Number(AddIndexCount) + Number(document.ProxyrulesDel.rulesnum.value) ) )
	{
		alert( showText(CustomDNSProxyLimit).replace(/#####/,rulelimit) );
		return false;
	}
	else
	{
		document.TemporaryProxyrulesAdd.addSelIndex.value=AllSelIndex;
		if(document.dnsproxy.ProxyruleEnabled[0].checked == true)
		{
			document.TemporaryProxyrulesAdd.ProxyruleEnabled.value="OFF";
		}
		else
		{
			document.TemporaryProxyrulesAdd.ProxyruleEnabled.value="ON";
		}
		if(document.dnsproxy.UserDefineRulesEnabled[0].checked == true)
		{
			document.TemporaryProxyrulesAdd.UserDefineRulesEnabled.value="OFF";
		}
		else
		{
			document.TemporaryProxyrulesAdd.UserDefineRulesEnabled.value="ON";
		}
		document.TemporaryProxyrulesAdd.submit();
	}
}

function delIndex()
{
		if(document.dnsproxy.ProxyruleEnabled[0].checked == true)
		{
			document.ProxyrulesDel.ProxyruleEnabled.value="OFF";
		}
		else
		{
			document.ProxyrulesDel.ProxyruleEnabled.value="ON";
		}

		if(document.dnsproxy.UserDefineRulesEnabled[0].checked == true)
		{
			document.ProxyrulesDel.UserDefineRulesEnabled.value="OFF";
		}
		else
		{
			document.ProxyrulesDel.UserDefineRulesEnabled.value="ON";
		}
}

function SelectRulesClicked()
{
	if( document.dnsproxy.ProxyruleEnabled[0].checked == true )
	{
		with(document.ProxyrulesDel) {
			var rulesNum = rulesnum.value;
			for(i=1;i<=rulesNum;i++)
			{
				document.getElementById('select' + i ).disabled = true;
			}
			selectAllProxyrules1.disabled = true;
			deleteSelProxyrules.disabled = true;
			deleteAllProxyrules.disabled = true;
			deleteSelProxyrules.style.color=disableButtonColor;
			deleteAllProxyrules.style.color=disableButtonColor;
		}
		with(document.TemporaryProxyrules) {
			var rulesNum = TempRulesNum.value;
			for(i=1;i<=rulesNum;i++)
			{
				document.getElementById('TempSelect' + i ).disabled = true;
			}
			selectAllProxyrules2.disabled = true;
			addSelProxyrules.disabled = true;
			addSelProxyrules.style.color=disableButtonColor;
		}
	}
	else
	{
		if(document.dnsproxy.UserDefineRulesEnabled[0].checked == true)
		{
			with(document.ProxyrulesDel) {
				var rulesNum = rulesnum.value;
				for(i=1;i<=rulesNum;i++)
				{
					//document.getElementById('select' + i ).checked = false;
					document.getElementById('select' + i ).disabled = true;
				}
				selectAllProxyrules1.disabled = true;
				deleteSelProxyrules.disabled = true;
				deleteAllProxyrules.disabled = true;
				deleteSelProxyrules.style.color=disableButtonColor;
				deleteAllProxyrules.style.color=disableButtonColor;
			}
			with(document.TemporaryProxyrules) {
				var rulesNum = TempRulesNum.value;
				for(i=1;i<=rulesNum;i++)
				{
					//document.getElementById('TempSelect' + i ).checked = true;
					document.getElementById('TempSelect' + i ).disabled = true;
				}
				selectAllProxyrules2.disabled = true;
				addSelProxyrules.disabled = true;
				addSelProxyrules.style.color=disableButtonColor;
			}
		}
		else
		{
			with(document.ProxyrulesDel) {
				var rulesNum = rulesnum.value;
				for(i=1;i<=rulesNum;i++)
				{
					//document.getElementById('select' + i ).checked = false;
					document.getElementById('select' + i ).disabled = false;
				}
				selectAllProxyrules1.disabled = false;
				deleteSelProxyrules.disabled = false;
				deleteAllProxyrules.disabled = false;
				deleteSelProxyrules.style.color=enableButtonColor;
				deleteAllProxyrules.style.color=enableButtonColor;
			}
			with(document.TemporaryProxyrules) {
				var rulesNum = TempRulesNum.value;
				for(i=1;i<=rulesNum;i++)
				{
					//document.getElementById('TempSelect' + i ).checked = false;
					document.getElementById('TempSelect' + i ).disabled = false;
				}
				selectAllProxyrules2.disabled = false;
				addSelProxyrules.disabled = false;
				addSelProxyrules.style.color=enableButtonColor;
			}
		}
	}
	if(entryNum==0) {
		document.ProxyrulesDel.selectAllProxyrules1.disabled = true;
		document.ProxyrulesDel.deleteSelProxyrules.disabled = true;
		document.ProxyrulesDel.deleteAllProxyrules.disabled = true;
		document.ProxyrulesDel.deleteSelProxyrules.style.color=disableButtonColor;
		document.ProxyrulesDel.deleteAllProxyrules.style.color=disableButtonColor;
	}
}

function selectAllIndex(value)
{
	if( value == 1 )
	{
		with(document.ProxyrulesDel) {
			var rulesNum = rulesnum.value;
			if(selectAllProxyrules1.checked==true)
			{
				for(i=1;i<=rulesNum;i++)
				{
					document.getElementById('select' + i ).checked = true;
				}
			}
			else
			{
				for(i=1;i<=rulesNum;i++)
				{
					document.getElementById('select' + i ).checked = false;
				}
			}
		}
	}
	else if ( value == 2 )
	{
		with(document.TemporaryProxyrules) {
			var rulesNum = TempRulesNum.value;
			if(selectAllProxyrules2.checked==true)
			{
				for(i=1;i<=rulesNum;i++)
				{
					document.getElementById('TempSelect' + i ).checked = true;
				}
			}
			else
			{
				for(i=1;i<=rulesNum;i++)
				{
					document.getElementById('TempSelect' + i ).checked = false;
				}
			}
		}
	}
}

function init()
{
	$("#help-box").hide();
	var tempEntryNum=entryNum;
	if(entryNum==0) {
		tempEntryNum+=1;
		document.ProxyrulesDel.selectAllProxyrules1.disabled = true;
		document.ProxyrulesDel.deleteSelProxyrules.disabled = true;
		document.ProxyrulesDel.deleteAllProxyrules.disabled = true;
		document.ProxyrulesDel.deleteSelProxyrules.style.color=disableButtonColor;
		document.ProxyrulesDel.deleteAllProxyrules.style.color=disableButtonColor;
	}

	updateHeight=tempEntryNum*25;
	if(updateHeight>=505)
		updateHeight="505px";
	else
		updateHeight=updateHeight+"px";

	//alert("FlashTbl="+updateHeight);
	document.getElementById('FlashTbl').style.height=updateHeight;	


	if(DNSProxyTempRulesNum==0)
		DNSProxyTempRulesNum+=1;

	updateHeight=DNSProxyTempRulesNum*25;
	if(updateHeight>=505)
		updateHeight="505px";
	else
		updateHeight=updateHeight+"px";

	//alert("MemoryTbl="+updateHeight);
	document.getElementById('MemoryTbl').style.height=updateHeight;	
}

</SCRIPT>

<style type="text/css">
.footBorder {border-bottom-style:solid; border-color:#d8d8d8;border-width:1px;}
</style>

</head>
<body onLoad="init();">

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton(0);
			</script>

<blockquote>

	<fieldset name="dnsproxy_fieldset" id="dnsproxy_fieldset">
		<legend><script>dw(DNSPROXY)</script></legend>

			<form action=/goform/formDNSProxyrules method=POST name="dnsproxy">
				
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">

						<!-- DNS proxy -->
						<tr>
							<td><div class="itemText-len"><script>dw(DNSPROXY)</script></div></td>
							<td><div class="itemText2-len"><span class="choose-itemText">
								<script>
								if(enable_http_proxy)
								{
									document.write('<input type="radio" value="OFF" name="ProxyruleEnabled" onClick="DNSProxyClicked();" >'+showText(disable))
									document.write('<input type="radio" value="ON" name="ProxyruleEnabled" onClick="DNSProxyClicked();" checked >'+showText(enable))
								}
								else
								{
									document.write('<input type="radio" value="OFF" name="ProxyruleEnabled" onClick="DNSProxyClicked();" checked >'+showText(disable))
									document.write('<input type="radio" value="ON" name="ProxyruleEnabled" onClick="DNSProxyClicked();" >'+showText(enable))
								}
								document.write('&nbsp;')
								</script>
							</span></div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(DNSProxyrule)</script></div></td>
							<td><div class="itemText2-len">
								<script>
									document.write('<input type="text" name="url1" size="40" maxlength="98" value="'+DownLoadURL+'">');
								</script>
							</div></td>
						</tr>

						<tr>
							<td><div class="itemText-len"><script>dw(SelectYourRules)</script></div></td>
							<td><div class="itemText2-len">
								<span class="choose-itemText">
									<script>

										if(enable_user_define_rules)
										{
											document.write('<input type="radio" value="OFF" name="UserDefineRulesEnabled" onClick="SelectRulesClicked()" >'+showText(OriginalRules))
											document.write('<input type="radio" value="ON" name="UserDefineRulesEnabled" onClick="SelectRulesClicked()" checked >'+showText(UserDefineRules))
										}
										else
										{
											document.write('<input type="radio" value="OFF" name="UserDefineRulesEnabled" onClick="SelectRulesClicked()" checked >'+showText(OriginalRules))
											document.write('<input type="radio" value="ON" name="UserDefineRulesEnabled" onClick="SelectRulesClicked()" >'+showText(UserDefineRules))
										}
										document.write('&nbsp;')
									</script>
								</span>
							</div></td>
						</tr>	

					</table>
					<input type="hidden" value="/inter_dnsproxy.asp" name="submit-url" id="submitUrl">
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="" name="isApply">
				</form>

			<br>
			<br>
			<form action="/goform/formDNSProxyrules" method="POST" name="ProxyrulesDel">
					<table width="600" cellpadding="2" cellspacing="1" align="center">
							<tr>
								<td width="40%" align="left" >
									<b><div class="itemText"><script>dw(UserDefineRules)</script>&ensp;:</div></b>
								</td>
								<td width="60%" align="right">
									<script>
										document.write('<span class="itemText">('+showText(CustomDNSProxyLimit).replace(/#####/,rulelimit)+')</span>')
									</script>
								</td>
							</tr>
					</table>
					<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
							<tr>
									<td width="8%" align="center" class="dnsTableHe"><script>dw(NO)</script>.</td>
									<td width="51%" align="center" class="dnsTableHe"><script>dw(domainName)</script></td>
									<td width="26%" align="center" class="dnsTableHe"><script>dw(ProxyServerIP)</script></td>
									<td width="15%" align="center" class="dnsTableHe">
										<input type="checkbox" name="selectAllProxyrules1" onclick="selectAllIndex(1)">	
										<img src="graphics/dot-3.png" width="10px" height="10px">			
									</td>
							</tr>
					</table>
					<div style="text-align:center;padding-left:1.3cm;">
					<div id="FlashTbl" align="center" style="height:500px; width:600px; overflow-x:hidden; overflow-y:scroll;" border="0" class="footBorder">
						<table width="590" cellpadding="2" cellspacing="2" align="center">
							<% DNSPROXYURLList(); %>
						</table>
					</div>
					</div>
					<table border="0" width="600" align="center">
							<tr>
								<td>&ensp;</td>
							</tr>
							<tr>
								<td colspan="5" align="right">
									<script>buffer='<input type="submit" value="'+showText(deleteSelected)+'" id="deleteSelProxyrules" name="deleteSelProxyrules" onClick="delIndex()" class="ui-button-nolimit">';document.write(buffer);</script>&nbsp;
									<script>buffer='<input type="submit" value="'+showText(deleteAll)+'" id="deleteAllProxyrules" name="deleteAllProxyrules" onClick="delIndex()" class="ui-button-nolimit">';document.write(buffer);</script>
								 
							 </td>
							</tr>
					</table>
				<input type="hidden" value="" name="ProxyruleEnabled">
				<input type="hidden" value="" name="UserDefineRulesEnabled">
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="/inter_dnsproxy.asp" name="submit-url">
		 </form>
		 <br>
		 <form action="/goform/formDNSProxyrules" method="POST" name="TemporaryProxyrules">
				<table width="600" cellpadding="2" cellspacing="1" align="center">
					<tr>
						<td width="40%" align="left" >
							<b><div class="itemText"><script>dw(OriginalRules)</script>&ensp;:</div></b>
						</td>
						<td width="60%" align="right">
						</td>
					</tr>
				</table>
				<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
						<tr>
							<td width="8%" align="center" class="dnsTableHe"><script>dw(NO)</script>.</td>
							<td width="51%" align="center" class="dnsTableHe"><script>dw(domainName)</script></td>
							<td width="26%" align="center" class="dnsTableHe"><script>dw(ProxyServerIP)</script></td>
							<td width="15%" align="center" class="dnsTableHe">
								<input type="checkbox" name="selectAllProxyrules2" onclick="selectAllIndex(2)"> 
								<img src="graphics/dot-3.png" width="10px" height="10px">
							</td>
						</tr>
				</table>
				<div style="text-align:center;padding-left:1.3cm;">
				<div id="MemoryTbl" style="width:600px; overflow-x:hidden; overflow-y:scroll; " border="0" class="footBorder">
					<table width="590" cellpadding="2" cellspacing="2" align="center">
						<% TemporaryDNSPROXYURLList(); %>
					</table>
				</div>
				</div>
				<table border="0" width="600" align="center">
						<tr>
							<td>&ensp;</td>
						</tr>
						<tr>
							<td colspan="5" align="right">
								<script>buffer='<input type="button" value="'+showText(Add)+'" id="addSelProxyrules" name="addSelProxyrules" onClick="return addIndex()" class="ui-button-nolimit">';document.write(buffer);</script>
							 
						 </td>
						</tr>
				</table>
		 </form>
		<form action="/goform/formDNSProxyrules" method="POST" name="TemporaryProxyrulesAdd">
			<input type="hidden" value="" name="ProxyruleEnabled">
			<input type="hidden" value="" name="UserDefineRulesEnabled">
			<input type="hidden" value="1" name="addSelIndex" id="addSelIndex">
			<input type="hidden" value="/inter_dnsproxy.asp" name="submit-url" id="submitUrl">
			<input type="hidden" value="1" name="iqsetupclose">
			<input type="hidden" value="" name="isApply">
		</form>
		 <br>
		<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td style="text-align:center; padding-top:30px;">
					<script>
						document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
						var show_reboot = "<% getInfo("show_reboot"); %>"
						if(show_reboot == "1")
							document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
					</script>
				</td>
			</tr>
		</table>				
	</fieldset>			
<script>
$("#help-box").hide();
DNSProxyClicked();
</script>

</blockquote>
	
</body>

<head>
<meta http-equiv="Pragma" content="no-cache">
</head>	

</html>
