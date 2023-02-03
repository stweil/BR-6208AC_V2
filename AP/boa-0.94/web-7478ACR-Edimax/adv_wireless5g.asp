<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>

<SCRIPT>
var wizMode = <%getVar("wizmode","","");%>;

function savechange(rebootFlag)
{
	with(document.advanceSetup) {
	
		if (!portRule(fragThreshold, showText(FragmentThreshold), 0,"", 256, 2346, 1))
			return false;
		if (!portRule(rtsThreshold, showText(RTSThreshold), 0,"", 0, 2347, 1))
			return false;
		if (!portRule(beaconInterval, showText(BeaconInterval), 0,"", 20, 1024, 1))
			return false;
		if (!portRule(dtimPeriod, showText(DTIMPeriod), 0,"", 1, 10, 1))
			return false;


	
		if(rebootFlag)
			isApply.value = "ok"

		submit(); 	
	}
}
function Disable_Date_Rate()
{
	var txRate=document.advanceSetup.txRate;
	var NtxRate=document.advanceSetup.NtxRate;
	if ((band != 8) && (band != 12) && (band != 76) && (band != 72) && (band != 64))
		NtxRate.disabled=true;
	else 
		NtxRate.disabled=false;
	if (band == 8 || band == 64 || band == 72)
		txRate.disabled=true;
}
function init()
{	
	$("#help-box").hide();
}
</SCRIPT>

</head>


<body onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(advanceHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<fieldset name="StaticRout_fieldset" id="StaticRout_fieldset">
		<legend>5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></legend>
				<br>
				<form action="/goform/formiNICAdvanceSetup"  method="POST" name="advanceSetup">

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(WirelessModule)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												var wlsw = <%getiNICVar("wlanDisabled","","");%>;
												if(wlsw)
													document.write(showText(disable));
												else
													document.write(showText(enable));
											</script>	
										</span></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(FragmentThreshold)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="fragThreshold" size="10" maxlength="4" value="<%getiNICInfo("fragThreshold");%>">&nbsp;<span class="choose-itemText ">(256-2346)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(RTSThreshold)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="rtsThreshold" size="10" maxlength="4" value="<%getiNICInfo("rtsThreshold"); %>">&nbsp;<span class="choose-itemText ">(0-2347)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(BeaconInterval)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="beaconInterval" size="10" maxlength="4" value="<% getiNICInfo("beaconInterval"); %>">&nbsp;<span class="choose-itemText ">(20-1024 ms)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(DTIMPeriod)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="dtimPeriod" size="10" maxlength="2" value="<% getiNICInfo("dtimPeriod"); %>">&nbsp;<span class="choose-itemText ">(1-10)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(DataRate)</script></span></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="txRate" onChange="Disable_Date_Rate();">
												<script>
													band = <%getiNICVar("band","","");%>;
													auto = <%getiNICVar("rateAdaptiveEnabled","","");%>;
													txrate = <%getiNICVar("fixTxRate","","");%>;
													wlDev = <%getiNICVar("wlDev","","");%>;

													rate_mask = [7,1,1,1,1,2,2,2,2,2,2,2,2];
													rate_name =["Auto","1M","2M","5.5M","11M","6M","9M","12M","18M","24M","36M","48M","54M"];
													band2mask = [0, 1, 4, 7, 2, 7];
																
													mask=0;
													mask = band2mask[4];
													defidx=0;
													for (idx=0, i=0; i<=28; i++)
													{
														if (rate_mask[i] & mask)
														{
															if (i == 0) rate = 0;
															else
															{
																if(wlDev!=4) rate = (1 << (i-1));
																else rate = i;
															}
															if (txrate == rate) defidx = idx;
															document.write('<option value="' + i + '">' + rate_name[i] + '\n');
															idx++;
														}
													}
													document.advanceSetup.txRate.selectedIndex=defidx;
												</script>
											</select>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(NDataRate)</script></span></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="NtxRate">
												<script>
													band = <%getiNICVar("band","","");%>;
													auto = <%getiNICVar("rateAdaptiveEnabled","","");%>;
													txrate = <%getiNICVar("fixTxRate","","");%>;
													wlDev = <%getiNICVar("wlDev","","");%>;
													txStream = <%getiNICVar("txStream","","");%>;

																	 
													rate_mask = [7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4];
													rate_name =["Auto", "MCS 0", "MCS 1", "MCS 2", "MCS 3", "MCS 4", "MCS 5", "MCS 6", "MCS 7", "MCS 8", "MCS 9", "MCS 10", "MCS 11", "MCS 12", "MCS 13", "MCS 14", "MCS 15"];
					//				    band2mask = [0, 1, 4, 3, 1, 7];
													if (typeof(_SUPPORT_11AC_) != "undefined" ) {
														band2mask = [0, 1, 4, 4, 4, 4];
													} else {
														band2mask = [0, 1, 4, 4];
													}			
													mask=0;
													mask = band2mask[band];
													defidx=0;
													if(txStream == 1) maxMCS=8;
													else             maxMCS=16;

													for (idx=0, i=0; i<=maxMCS; i++)
													{
														if (rate_mask[i] & mask)
														{
															if (i == 0) rate = 0;
															else
															{
																if(wlDev!=4) rate = (1 << (i-1));
																else rate = i;
															}
															if (txrate == rate) defidx = idx;
															document.write('<option value="' + i + '">' + rate_name[i] + '\n');
															idx++;
														}
													}
													document.advanceSetup.NtxRate.selectedIndex=defidx;
												</script>
											</select>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(ChannelWidth)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												
								        document.write('<input type="radio" name="wlanNChanWidth" value="2" <%getiNICVar("NChanWidth","2","checked");%> >20/40/80 '+showText(MHZ));
								        document.write('<input type="radio" name="wlanNChanWidth" value="0" <%getiNICVar("NChanWidth","0","checked");%> >20/40 '+showText(MHZ));   
												
												</script>
												&nbsp;
												<input type="radio" name="wlanNChanWidth" value="1" <%getiNICVar("NChanWidth","1","checked");%> ><script>dw(MHZ20)</script>
											</span></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(PreambleType)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<input type="radio" value="long" name="preamble" <%getiNICVar("preamble","0","checked");%> ><script>dw(ShortPreamble)</script>&nbsp;&nbsp;
											<input type="radio" name="preamble" value="short" <%getiNICVar("preamble","1","checked");%> ><script>dw(LongPreamble)</script>
										</span></div></td>
									</tr>
									<!--<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(BroadcastEssid)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<input type="radio" name="hiddenSSID" value="no" <%getVar("5GhiddenSSID","0","checked");%> ><script>dw(enable)</script>&nbsp;&nbsp;
											<input type="radio" name="hiddenSSID" value="yes" <%getVar("5GhiddenSSID","1","checked");%> ><script>dw(disable)</script>
										</span></div></td>
									</tr>-->
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(CTSProtect)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												var cts = <%getiNICVar("wlanCts","","");%>;
												if(cts == 0)
												{
													document.write('<input type="radio" name="wlanCts" value="auto" checked>'+showText(Auto)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="always">'+showText(Always)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="none">'+showText(None)+'&nbsp;&nbsp;');
												}
												else if(cts==1)
												{
													document.write('<input type="radio" name="wlanCts" value="auto">'+showText(Auto)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="always" checked>'+showText(Always)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="none">'+showText(None)+'&nbsp;&nbsp;');
												}
												else
												{
													document.write('<input type="radio" name="wlanCts" value="auto">'+showText(Auto)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="always">'+showText(Always)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanCts" value="none" checked>'+showText(None)+'&nbsp;&nbsp;');
												}
											</script>
										</span></div></td>
									</tr>
									<input type="hidden" name="getTxPower" value="<% getiNICInfo("wlanTxPower"); %>">
									<tr>
										<td><div class="itemText-len"><script>dw(TxPower)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<select size="1" name="wlanTxPower" class="select">
												<script>
													var rateTbl=new Array("100 %"," 70 %"," 50 %"," 35 %"," 15 %");
													var rValTbl=new Array("100","70","50","35","15");
													for (i=0; i<5; i++)
													{
														if (document.advanceSetup.getTxPower.value == rValTbl[i]) document.write('<option selected value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
														else document.write('<option value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
													}
												</script>
											</select>
										</span></div></td>
									</tr>

									<script>
									if(wizMode != 1){
										document.write('<tr>');
										document.write('	<td><div class="itemText-len"><span class="itemText">'+showText(WMM)+'</span></div></td>');
										document.write('	<td><div class="itemText2-len"><span class="choose-itemText ">');
									
										var wmm = <%getiNICVar("wlanWmm","","");%>;
										if(wmm){
											document.write('<input type="radio" name="wlanWmm" value="yes" checked>'+showText(enable)+'&nbsp;&nbsp;');
											document.write('<input type="radio" name="wlanWmm" value="no">'+showText(disable)+'&nbsp;&nbsp;');
										}
										else{
											document.write('<input type="radio" name="wlanWmm" value="yes">'+showText(enable)+'&nbsp;&nbsp;');
											document.write('<input type="radio" name="wlanWmm" value="no" checked>'+showText(disable)+'&nbsp;&nbsp;');
										}

										document.write('	</span></div></td>');
										document.write('</tr>');
									}
									</script>
							</table>

							<br>

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
								<tr>
									<td style="text-align:center; padding-top:30px;">
										<script>
											
											document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return savechange(0)" class="ui-button">');	
											var show_reboot = "<% getInfo("show_reboot"); %>"
											if(show_reboot == "1")
												document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return savechange(1)" > '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
										</script>
										<input type="hidden" value="" name="isApply">
										<input type="hidden" value="/adv_wireless5g.asp" name="submit-url">
									</td>
								</tr>
							</table>
				<input type="hidden" value="1" name="iqsetupclose">
			</form>
	</fieldset>


<script>
	//Disable_Date_Rate();
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
