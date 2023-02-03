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

//var FirewallEnabled = <%getVar("FirewallEnable","","");%>;

function init()
{
	$("#help-box").hide();
}
function saveChanges(rebootFlag)
{
	with(document.fwseting) {
	
		if(checkvaule()==false)
			return false;

		if(rebootFlag)
			isApply.value = "ok;"

		submit();
	}

}

/*function showDos() {
	with(document.fwseting) {
		if (FirewallEnable[0].checked==false) {
		 displayObj(0);
		}
		else{
			displayObj(1);
		}
	}
}

function displayObj(Flag) {

	//$("#help-box").hide();

	if (Flag)
		document.getElementById('dosId').style.display = "block";	
	else
		document.getElementById('dosId').style.display = "none";

	var help_info_array = new Array
	(
		showText(firewallHelp),
		"<b>"+showText(Firewall)+":</b><br>"+showText(firewallHelp)+"<br><b>"+showText(DoS)+":</b><br>"+showText(DoSHelp)+"<br><br>"
	);
	
	document.getElementById("help-info1").innerHTML=help_info_array[Flag];
}*/


function validateNum(str)
{
  for (var i=0; i<str.length; i++) {
   	if ( !(str.charAt(i) >='0' && str.charAt(i) <= '9')) {
		//alert(showText(fwadvInvValue));
		alert(''+showText(InvalidSomething).replace(/#####/,showText(Value))+' '+showText(shouldbeSomething).replace(/####/,showText(decimalnumber)));
		return false;
  	}
  }
  return true;
}
function checkvaule()
{
	strpodPack = document.fwseting.podPack;
	strpodBur = document.fwseting.podBur;
	strsynPack = document.fwseting.synPack;
	strsynBur = document.fwseting.synBur;
	if (document.fwseting.podEnable.checked) {
		if ( validateNum(strpodPack.value) == 0 ) {
			setFocus(strpodPack);
			return false;
		}
		num = parseInt(strpodPack.value);
		if (strpodPack.value == "" || num < 0 || num > 255) {
			//alert(showText(fwadvInvPacket));
			alert(showText(InvalidpacketOFDeath));
			setFocus(strpodPack);
			return false;
		}
		if ( validateNum(strpodBur.value) == 0 ) {
			setFocus(strpodBur);
			return false;
		}
		num = parseInt(strpodBur.value);
		if (strpodBur.value == "" || num < 0 || num > 255) {
			//alert(showText(fwadvInvBurst));
			alert(showText(InvalidBurstofDeath));
			setFocus(strpodBur);
			return false;
		}
	}
	if (document.fwseting.synEnable.checked) {
		if ( validateNum(strsynPack.value) == 0 ) {
			setFocus(strsynPack);
			return false;
		}
		num = parseInt(strsynPack.value);
		if (strsynPack.value == "" || num < 0 || num > 255) {
			//alert(showText(fwadvInvPacketSync));
			alert(showText(InvalidpacketofSync));
			setFocus(strsynPack);
			return false;
		}
		if ( validateNum(strsynBur.value) == 0 ) {
			setFocus(strsynBur);
			return false;
		}
		num = parseInt(strsynBur.value);
		if (strsynBur.value == "" || num < 0 || num > 255) {
			//alert(showText(fwadvInvBurstSync));
			alert(showText(InvalidpacketBurstofSync));
			setFocus(strsynBur);
			return false;
		}
	}
	if (document.fwseting.scanEnable.checked) {
		var longVal=0;
		if (document.fwseting.Index0.checked==true)	longVal |= 0x001;
		if (document.fwseting.Index1.checked==true)	longVal |= 0x002;
		if (document.fwseting.Index2.checked==true)	longVal |= 0x004;
		if (document.fwseting.Index3.checked==true)	longVal |= 0x008;
		if (document.fwseting.Index4.checked==true)	longVal |= 0x010;
		if (document.fwseting.Index5.checked==true)	longVal |= 0x020;
		if (document.fwseting.Index6.checked==true)	longVal |= 0x040;
		document.fwseting.scanNumVal.value=longVal;
	}
	return true;
}
function ItemEnable() {
	if (!document.fwseting.podEnable.checked) {
		document.fwseting.podPack.disabled = true;
		document.fwseting.podBur.disabled = true;
		document.fwseting.podTime.disabled = true;
	}
	else {
		document.fwseting.podPack.disabled = false;
		document.fwseting.podBur.disabled = false;
		document.fwseting.podTime.disabled = false;
	}
	if (!document.fwseting.synEnable.checked) {
		document.fwseting.synPack.disabled = true;
		document.fwseting.synBur.disabled = true;
		document.fwseting.synTime.disabled = true;
	}
	else {
		document.fwseting.synPack.disabled = false;
		document.fwseting.synBur.disabled = false;
		document.fwseting.synTime.disabled = false;
	}
	if (!document.fwseting.scanEnable.checked) {
		document.fwseting.Index0.disabled = true;
		document.fwseting.Index1.disabled = true;
		document.fwseting.Index2.disabled = true;
		document.fwseting.Index3.disabled = true;
		document.fwseting.Index4.disabled = true;
		document.fwseting.Index5.disabled = true;
		document.fwseting.Index6.disabled = true;
	}
	else {
		document.fwseting.Index0.disabled = false;
		document.fwseting.Index1.disabled = false;
		document.fwseting.Index2.disabled = false;
		document.fwseting.Index3.disabled = false;
		document.fwseting.Index4.disabled = false;
		document.fwseting.Index5.disabled = false;
		document.fwseting.Index6.disabled = false;
	}
}
</SCRIPT>

</head>


<body onLoad="init()">

<!--====================================================================================================================================-->

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(DoSHelp)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>
<!--==========================================================================================================================================================-->


<blockquote>

	<form action="/goform/formPreventionSetup" method="POST" name="fwseting">
			<!--<fieldset name="firewall_fieldset" id="firewall_fieldset">
				<legend><script>dw(Firewall)</script></legend>
						<br>
						<table width="600" border="0" cellspacing="1" cellpadding="0" class="" align="center">
							<tr>
								<td valign="top" align="left">

									<span class="itemText"><script>dw(firewallCont)</script></span></p>
								</td>
							</tr>
						</table>
	
						<table width="600" border="0" cellpadding="2" cellspacing="2" align="center" id="">
								<tr>
									<td><div class="medleft-itemText-len"><script>dw(SPIfirewall)</script></div></td>
									<td><div class="medleft-itemText2-len"><span class="choose-itemText ">
										<input type="radio" name="FirewallEnable" value="yes" onClick="showDos()" <%getVar("FirewallEnable","1","checked");%> >
											<script>dw(enable);</script>&nbsp;
										<input type="radio" name="FirewallEnable" value="no"  onClick="showDos()" <%getVar("FirewallEnable","0","checked");%> >
											<script>dw(disable);</script>
									</span></div></td>
								</tr>
						</table>
	</fieldset>	
	<br>-->

<!--===========================================================================================================================================-->
	<fieldset name="dos_fieldset" id="dosId">
		<legend><script>dw(DoS)</script></legend>
						<!--<span style="display:none" id="dosId" >-->
						<br>
						
						<table border="0" id="authId" align="center" Cellspacing="1" cellpadding="4" width="700">
              <input type="hidden" value="<% getInfo("scanNum"); %>" name="scanNumVal">
								<tr>
									<td width="30%" align="left" class="" style="padding-right:20px;">
										<input type="checkbox" name="podEnable" value="ON" <% getVar("podEnable","1","checked"); %>  onClick="ItemEnable();"><span class="itemText"><script>dw(PingDeath)</script>
									</span></td>
									<td width="70%" align="left"><span class="itemText">
										<input type="text" name="podPack" size="3" maxlength="3" value="<% getInfo("podPack"); %>">&ensp;<script>dw(PingDeath);</script>
										<script>dw(PacketS);</script>
										<script>dw(Per);</script>&ensp;
										<select name="podTime">
											<script>
												var TimeTbl = new Array(showText(Second),showText(minute),showText(hour));
												var podTimeVal = <%getVar("podTime","","");%>;
												for ( i=0; i<=2; i++) {
													if (i == podTimeVal)
														document.write('<option selected value="'+i+'">'+TimeTbl[i]+'</option>');
													else
														document.write('<option value="'+i+'">'+TimeTbl[i]+'</option>');
												}
											</script>
										</select>
										&ensp;<script>dw(Burst);</script>&ensp;
										<input type="text" name="podBur" size="3" maxlength="3" value="<% getInfo("podBur"); %>">
									</span></td>
								</tr>
								<tr>
									<td colspan="2">
										<span class="itemText">&ensp;</span>
									</td>
								</tr>
								<tr>
									<td width="30%" align="left" class="" style="padding-right:20px;"><span class="itemText">
										<input type="checkbox" name="pingEnable" value="ON" <% getVar("pingEnable","1","checked"); %> ><script>dw(DiscardPingFromWAN)</script>
									</span></td>
									<td width="70%" align="left">
								</tr>
								<tr>
									<td width="30%" align="left" class="" style="padding-right:20px;"><span class="itemText">
										<input type="checkbox" name="scanEnable" value="ON" <% getVar("scanEnable","1","checked"); %> onClick="ItemEnable();"><script>dw(PortScan)</script>
									</span></td>
									<td width="400" height="20" valign="middle" align="left" class="style1">
										<script>
											var scanTbl = new Array(showText(PortScan1),showText(PortScan2),showText(PortScan3),showText(PortScan4),
																							showText(PortScan5),showText(PortScan6),showText(PortScan7));
											var val=0x001;
											for(i=0; i<=6; i++){
												if (document.fwseting.scanNumVal.value & val)
													document.write('<input type="checkbox" name="Index'+i+'" checked>');
												else
													document.write('<input type="checkbox" name="Index'+i+'">');
												document.write('<span class="itemText">' + scanTbl[i] + '</span><br>');
												val *=2;
											}
										</script>
									</td>
								</tr>
								<tr>
									<td colspan="2">
										<span class="itemText">&ensp;</span>
									</td>
								</tr>
								<tr>
									<td width="30%" align="left" class="" style="padding-right:20px;"><span class="itemText">
										<input type="checkbox" name="synEnable" value="ON" <% getVar("synEnable","1","checked"); %>  onClick="ItemEnable();">
										<script>dw(SyncFlood)</script>
									</span></td>
									<td width="70%" align="left"><span class="itemText">
										<input type="text" name="synPack" size="3" maxlength="3" value="<% getInfo("synPack"); %>">&ensp;<script>dw(PacketS);</script>
										<script>dw(Per);</script>&ensp;
											<select name="synTime">
												<script>
												var synTimeVal = <%	getVar("synTime","","");%>;
												for ( i=0; i<=2; i++) {
													if (i == synTimeVal)
														document.write('<option selected value="'+i+'">'+TimeTbl[i]+'</option>');
													else
														document.write('<option value="'+i+'">'+TimeTbl[i]+'</option>');
												}
												</script>
											</select>
										&ensp;<script>dw(Burst);</script>&ensp;<input type="text" name="synBur" size="3" maxlength="3" value="<% getInfo("synBur"); %>">
										<script>ItemEnable();</script>
									</span></td>
								</tr>
							</table>
				

						<!--</span>-->
<!--===========================================================================================================================================-->
						<br>

						<table align="center" border="0" Cellspacing="0" width="700">
							<tr>
								<td style="text-align:center; padding-top:30px;">
									<script>
										//document.write('<input type="button" value="'+showText(save)+'" name="save" onclick="saveChanges(0)" class="ui-button">&nbsp;')
										//document.write('<input type="button" value="'+showText(Apply)+'" name="new_apply" onclick="saveChanges(1)" class="ui-button">&nbsp;')
										//document.write('<input type="reset" value="'+showText(cancel)+'" name="clear" onClick="displayObj('+FirewallEnabled+');" class="ui-button">')
										document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
									</script>
								</td>
							</tr>
						</table>
						<input type="hidden" value="" name="isApply">
						<input type="hidden" value="/fw_dos.asp" name="submit-url">
				</fieldset>
				<input type="hidden" value="1" name="iqsetupclose">
			</form>
	
			
				


<!--===========================================================================================================================================-->
<script>
	$("#help-box").hide();
	//displayObj(FirewallEnabled);
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>


</html>
