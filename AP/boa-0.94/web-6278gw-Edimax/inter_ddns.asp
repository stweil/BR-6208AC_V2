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
var strddns = "<% getInfo("ddnsName"); %>"

function displayObj() {
	if (document.ddns.ddnspvidSel.value != "dhis") {
		document.getElementById('genId').style.display = "block";
		document.getElementById('dhisId').style.display = "none";
		document.getElementById('passId').style.display = "none";
		document.getElementById('authId').style.display = "none";
		if(document.ddns.ddnspvidSel.value == "ns2go")
		{
			document.ddns.ddnsName.value="www.ns2go.com";
			document.ddns.ddnsName.disabled=true;
		}
		else
		{
			//document.ddns.ddnsName.value=strddns;
			if (document.ddns.ddnsEnable[0].checked==false) document.ddns.ddnsName.disabled=true;
			else document.ddns.ddnsName.disabled=false;
		}
	}
	else {
		document.getElementById('genId').style.display = "none";
		document.getElementById('dhisId').style.display = "block";
		if (document.ddns.dhisSelect.value == 0) {
			document.getElementById('passId').style.display = "block";
			document.getElementById('authId').style.display = "none";
		}
		else {
			document.getElementById('passId').style.display = "none";
			document.getElementById('authId').style.display = "block";
		}
	}
}
function DDNSEnable() {
	with(document.ddns) {
		if (ddnsEnable[0].checked==false) {
			ddnspvidSel.disabled = true;
			ddnsName.disabled = true;
			ddnsAccount.disabled = true;
			ddnsPass.disabled = true;
			dhisHostID.disabled = true;
			dhisISAddr.disabled = true;
			dhisSelect.disabled = true;
			dhispass.disabled = true;
			dhisAuthP1.disabled = true;
			dhisAuthP2.disabled = true;
			dhisAuthQ1.disabled = true;
			dhisAuthQ2.disabled = true;
		}
		else{
			ddnspvidSel.disabled = false;
			if(ddnspvidSel.value == "ns2go") ddnsName.disabled = true;
			else ddnsName.disabled = false;
			ddnsAccount.disabled = false;
			ddnsPass.disabled = false;
			dhisHostID.disabled = false;
			dhisISAddr.disabled = false;
			dhisSelect.disabled = false;
			dhispass.disabled = false;
			dhisAuthP1.disabled = false;
			dhisAuthP2.disabled = false;
			dhisAuthQ1.disabled = false;
			dhisAuthQ2.disabled = false;
		}
	}
}

function saveChanges(rebootFlag)
{
	with(document.ddns) {

		if (ddnsEnable[1].checked==false)
		{

			if (ddnspvidSel.value!="dhis") 
			{
				if (!strRule(ddnsName, showText(domainName)))
					return false;
				if (!strRule(ddnsAccount, showText(Account)))
					return false;
				if (!strRule(ddnsPass,showText(Password)))
					return false;
			} 
			else 
			{
				if (!strRule(dhisHostID,showText(HostID)))
					return false;

				if ( validateKey( dhisHostID.value ) == 0 ) {
					alert(''+showText(InvalidSomething).replace(/#####/,showText(HostID))+' '+showText(shouldbeSomething).replace(/####/,showText(decimalnumber)));
					setFocus(dhisHostID);
					return false;
				}

				if (dhisISAddr.value=="") {
						alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(IPAddr)));
					setFocus(dhisISAddr);
					return false;
				}

				if (dhisSelect.value == 0 ) {
					if (!strRule(dhispass,showText(HostPass)))
						return false;
				}
			}

		}

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}

function resetform()
{
	document.ddns.reset();
	displayObj();
	DDNSEnable();
}
</SCRIPT>

</head>


<body>


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(DDNSHelpInfo)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="DDNS_fieldset" id="DDNS_fieldset">
			<legend><script>dw(DDNS)</script></legend>
				<form action="/goform/formDDNSSetup" method="POST" name="ddns">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(enable)</script>&nbsp;/&nbsp;<script>dw(disable)</script></div></td>
							<td><div class="itemText2-len"><span class="choose-itemText">
								<script>
									var ddnsenable = <%getVar("ddnsEnable","","");%>;
									if(ddnsenable == 1)
									{
										document.write('<input type="radio" name="ddnsEnable" onClick="DDNSEnable()" value="1" checked>'+showText(enable))
										document.write('<input type="radio" name="ddnsEnable" onClick="DDNSEnable()" value="0" >'+showText(disable))
									}
									else
									{
										document.write('<input type="radio" name="ddnsEnable" onClick="DDNSEnable()" value="1" >'+showText(enable))
										document.write('<input type="radio" name="ddnsEnable" onClick="DDNSEnable()" value="0" checked>'+showText(disable))
									}
								</script>
							</span></div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Provider)</script></div></td>
							<td><div class="itemText2-len">
									<select name="ddnspvidSel" onChange="displayObj();">
										<script>
											var ddnspvidSelGet = "<% getInfo("ddnspvidSel"); %>"
											var PvidTbl = new Array("qdns","dhs","dyndns","ods","tzo","gnudip","dyns","zoneedit","dhis","cybergate","ns2go","noip");
											var PvidName = new Array("3322(qdns)","DHS","DynDNS","ODS","TZO","GnuDIP","DyNS","ZoneEdit","DHIS","CyberGate","NS2GO","NO-IP");
											for (i=0;i<12;i++) {
												if( i != 8 ) //close dhis
												{
													if(PvidTbl[i] == ddnspvidSelGet)
														document.write("<option value=\""+PvidTbl[i]+"\" selected>"+PvidName[i]+"</option>");
													else
														document.write("<option value=\""+PvidTbl[i]+"\">"+PvidName[i]+"</option>");
												}
											}
										</script>
									</select>
							</div></td>
						</tr>
					</table>

					<table border="0" id="genId" align="center" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(domainName)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="ddnsName" size="25" maxlength="30" value="<% getInfo("ddnsName"); %>">
							</div></td>
							<script>//document.ddns.ddnsName.value=strddns</script>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Account)</script>&nbsp;/&nbsp;<script>dw(Email)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="ddnsAccount" size="25" maxlength="30" value="<% getInfo("ddnsAccount"); %>">
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Password)</script>&nbsp;/&nbsp;<script>dw(Key)</script></div></td>
							<td><div class="itemText2-len">
								<input type="password" name="ddnsPass" size="25" maxlength="30" value="<% getInfo("ddnsPass"); %>">
							</div></td>
						</tr>
					</table>

					<table border="0" id="dhisId" align="center" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(HostID)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisHostID" size="25" maxlength="10" value="<% getInfo("dhisHostID"); %>">
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(ISAddr)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisISAddr" size="25" maxlength="30" value="<% getInfo("dhisISAddr"); %>">
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(AuthenticationType)</script></div></td>
							<td><div class="itemText2-len">
								<select name="dhisSelect" onClick="displayObj();">
									<script>
										var dhisSelectGet = <%getVar("dhisSelect","","");%>;
										var authTypeTbl = new Array(showText(Password),"QRC");
										for (i=0;i<2;i++) {
											if(dhisSelectGet == i)
												document.write("<option value=\""+i+"\" selected>"+authTypeTbl[i]+"</option>");
											else
												document.write("<option value=\""+i+"\">"+authTypeTbl[i]+"</option>");
										}
									</script>
								</select>
							</div></td>
						</tr>
					</table>

					<table border="0" id="passId" align="center" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(HostPass)</script></div></td>
							<td><div class="itemText2-len">
								<input type="password" name="dhispass" size="25" maxlength="30" value="<% getInfo("dhispass"); %>">
							</div></td>
						</tr>
					</table>

					<table border="0" id="authId" align="center" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(AuthP)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisAuthP1" size="25" maxlength="50" value="<% getInfo("dhisAuthP1"); %>">
							</div></td>
						</tr>
						<tr>
							<td ><div class="itemText-len"><script>dw(AuthP)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisAuthP2" size="25" maxlength="50" value="<% getInfo("dhisAuthP2"); %>">
							</div></td>
						</tr>
						<tr>		
							<td><div class="itemText-len"><script>dw(AuthQ)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisAuthQ1" size="25" maxlength="50" value="<% getInfo("dhisAuthQ1"); %>">
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(AuthQ)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="dhisAuthQ2" size="25" maxlength="50" value="<% getInfo("dhisAuthQ2"); %>">
							</div></td>
						</tr>
					</table>

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
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="/inter_ddns.asp" name="submit-url">
					<input type="hidden" value="" name="isApply">

		</form>
	</fieldset>
			

<script>
	displayObj();
	DDNSEnable();
	$("#help-box").hide();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
