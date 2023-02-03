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
function init()
{	
	$("#help-box").hide();
}

function Disable_Date_Rate()
{
	var txRate=document.advanceSetup.txRate;
	var NtxRate=document.advanceSetup.NtxRate;

	if ((band != 2) && (band != 5)) NtxRate.disabled=true;
	else  NtxRate.disabled=false;

	if (band == 2) txRate.disabled=true;
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
		<legend>2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></legend>
				<br>
				<form action="/goform/formAdvanceSetup" method="POST" name="advanceSetup">

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(WirelessModule)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												var wlsw = <%getVar("wlanDisabled","","");%>;
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
											<input type="text" name="fragThreshold" size="10" maxlength="4" value="<% getInfo("fragThreshold"); %>">&nbsp;<span class="choose-itemText ">(256-2346)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(RTSThreshold)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="rtsThreshold" size="10" maxlength="4" value="<% getInfo("rtsThreshold"); %>">&nbsp;<span class="choose-itemText ">(0-2347)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(BeaconInterval)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="beaconInterval" size="10" maxlength="4" value="<% getInfo("beaconInterval"); %>">&nbsp;<span class="choose-itemText ">(20-1024 ms)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(DTIMPeriod)</script></span></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="dtimPeriod" size="10" maxlength="2" value="<% getInfo("dtimPeriod"); %>">&nbsp;<span class="choose-itemText ">(1-10)</span>
										</div></td>
									</tr>
		
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(DataRate)</script></span></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="txRate" onChange="Disable_Date_Rate();">
												<script>
													band = <%getVar("band","","");%>;
													auto = <%getVar("rateAdaptiveEnabled","","");%>;
													txrate = <%getVar("fixTxRate","","");%>;
													wlDev = <%getVar("wlDev","","");%>;
													rate_mask = [7,1,1,1,1,2,2,2,2,2,2,2,2];
													rate_name =["Auto","1M","2M","5.5M","11M","6M","9M","12M","18M","24M","36M","48M","54M"];
													band2mask = [0, 1, 4, 7, 2, 7];
													mask=0;
													mask = band2mask[band];
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
													band = <%getVar("band","","");%>;
													auto = <%getVar("rateAdaptiveEnabled","","");%>;
													txrate = <%getVar("NfixTxRate","","");%>;
													wlDev = <%getVar("wlDev","","");%>;
													txStream = <%getVar("txStream","","");%>;
													rate_mask = [7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4];
													rate_name =["Auto", "MCS0", "MCS1", "MCS2", "MCS3", "MCS4", "MCS5", "MCS6", "MCS7", "MCS8", "MCS9", "MCS10", "MCS11", "MCS12", "MCS13", "MCS14", "MCS15"];
													band2mask = [0, 1, 4, 3, 1, 7];
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
												var NChanW = <%getVar("NChanWidth","","");%>;
												if(NChanW!=0)
												{
													document.write('<input type="radio" name="wlanNChanWidth" value="1" checked>'+showText(Auto2040MHZ)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanNChanWidth" value="0" >'+showText(MHZ20)+'');
												}
												else
												{
													document.write('<input type="radio" name="wlanNChanWidth" value="1" >'+showText(Auto2040MHZ)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="wlanNChanWidth" value="0" checked>'+showText(MHZ20)+'');
												}
											</script>
										</span></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(PreambleType)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												var pream = <%getVar("preamble","","");%>;
												if(!pream)
												{
													document.write('<input type="radio" name="preamble" value="long" checked>'+showText(ShortPreamble)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="preamble" value="short">'+showText(LongPreamble)+'');
												}
												else
												{
													document.write('<input type="radio" name="preamble" value="long">'+showText(ShortPreamble)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="preamble" value="short" checked>'+showText(LongPreamble)+'');
												}

											</script>
										</span></div></td>
									</tr>
									<!--<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(BroadcastEssid)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>	
												var BSSID = <%getVar("hiddenSSID","","");%>;
												if(!BSSID)
												{
													document.write('<input type="radio" name="hiddenSSID" value="no" checked>'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="hiddenSSID" value="yes">'+showText(disable)+'&nbsp;&nbsp;');
												}
												else
												{
													document.write('<input type="radio" name="hiddenSSID" value="no">'+showText(enable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="hiddenSSID" value="yes" checked>'+showText(disable)+'&nbsp;&nbsp;');
												}
											</script>
										</span></div></td>
									</tr>-->
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(CTSProtect)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												var cts = <%getVar("wlanCts","","");%>;
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
									
									<input type="hidden" name="getTxPower" value="<% getInfo("wlanTxPower"); %>">
									<tr>
										<td><div class="itemText-len"><script>dw(TxPower)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
                      <select size="1" name="wlanTxPower" class="select">
												<script>
													wlDev = <%getVar("wlDev","","");%>;
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
									
										var wmm = <%getVar("wlanWmm","","");%>;
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
											/*document.write('<input type="button" value="'+showText(save)+'" name="save" onClick="savechange(0)" class="ui-button">&nbsp;')
											document.write('<input type="button" value="'+showText(Apply)+'" name="Apply" onClick="savechange(1)" class="ui-button">&nbsp;')
											document.write('<input type="reset" value="'+showText(cancel)+'" name="cancel" class="ui-button">')*/
											document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return savechange(0)" class="ui-button">');	
											var show_reboot = "<% getInfo("show_reboot"); %>"
											if(show_reboot == "1")
												document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return savechange(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
										</script>
										<input type="hidden" value="" name="isApply">
										<input type="hidden" value="/adv_wireless.asp" name="submit-url">
									</td>
								</tr>
							</table>
				<input type="hidden" value="1" name="iqsetupclose">
			</form>
	</fieldset>

<script>

	$("#help-box").hide();
	Disable_Date_Rate();
</script>
</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
