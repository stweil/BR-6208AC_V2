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


function savechange(rebootFlag)
{
	with(document.Wireless) {

		if(CrossBandoff.checked == true)
			CrossBand_enable.value="no"
		else
			CrossBand_enable.value="yes"
		
		if(rebootFlag)
			isApply.value = "ok"
		submit(); 	
	}
}

function init()
{	
	$("#help-box").hide();
}

function CrossBandSwitch(flag)
{
	with(document.Wireless) 
	{		
		if (flag){
			CrossBandon.checked=true;
			CrossBandoff.checked=false;
		}
		else{
			CrossBandon.checked=false;
			CrossBandoff.checked=true;
		}
	}
}
</SCRIPT>

</head>


<body onLoad="init();">

	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td><div class="help-text" id="help-info1"><script></script>
			</div></td></tr>
		</table>
	</div>
	<script>
	HelpButton();
	</script>



<blockquote>

	<fieldset name="AdminWireless" id="AdminWireless">
		<legend><script>dw(AdvanceSetting)</script></legend>
				<br>
				<form action="/goform/formAdvanceSetup"  method="POST" name="Wireless">

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<input type="hidden" name="getTxPower" value="<% getInfo("wlanTxPower"); %>">
									<tr>
										<td><div class="itemText-len">2.4G&nbsp;<script>dw(TxPower)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
                      <select size="1" name="wlanTxPower" class="select">
												<script>
													var rateTbl=new Array("100 %"," 70 %"," 50 %"," 35 %"," 15 %");
													var rValTbl=new Array("100","70","50","35","15");
													for (i=0; i<5; i++)
													{
														if (document.Wireless.getTxPower.value == rValTbl[i]) document.write('<option selected value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
														else document.write('<option value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
													}
												</script>
											</select>
										</span></div></td>
									</tr>
									<tr>
									<input type="hidden" name="getTxPower5g" value="<% getiNICInfo("wlanTxPower"); %>">
									<tr>
										<td><div class="itemText-len">5G&nbsp;<script>dw(TxPower)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<select size="1" name="wlanTxPower5g" class="select">
												<script>
													var rateTbl=new Array("100 %"," 70 %"," 50 %"," 35 %"," 15 %");
													var rValTbl=new Array("100","70","50","35","15");
													for (i=0; i<5; i++)
													{
														if (document.Wireless.getTxPower5g.value == rValTbl[i]) document.write('<option selected value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
														else document.write('<option value="'+rValTbl[i]+'">'+rateTbl[i]+'</option>');
													}
												</script>
											</select>
										</span></div></td>
									</tr>
									<input type="hidden" name="CrossBand_enable" value="<% getInfo("useCrossBand"); %>">
									<tr>
										<td><div class="itemText-len"><span class="itemText"><script>dw(CrossBand)</script></span></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText ">
											<script>
												if(document.Wireless.CrossBand_enable.value == "0"){
													document.write('<input type="radio" name="CrossBandoff" value="0" checked onclick="CrossBandSwitch(0)">'+showText(disable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="CrossBandon" value="1"  onclick="CrossBandSwitch(1)">'+showText(enable)+'');
												}
												else{
													document.write('<input type="radio" name="CrossBandoff" value="0" onclick="CrossBandSwitch(0)" >'+showText(disable)+'&nbsp;&nbsp;');
													document.write('<input type="radio" name="CrossBandon" value="1" checked onclick="CrossBandSwitch(1)">'+showText(enable)+'');
												}
											</script>
										</span></div></td>
									</tr>

							</table>

							<br>

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
								<tr>
									<td style="text-align:center; padding-top:30px;">
										<script>
											
											document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return savechange(0)" class="ui-button">');	
											var show_reboot = "<% getInfo("show_reboot"); %>"
											if(show_reboot == "1")
												document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return savechange(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
										</script>
										<input type="hidden" value="" name="isApply">
										<input type="hidden" value="/admin_wireless.asp" name="submit-url">
										<input type="hidden" value="1" name="iqsetupclose">
									</td>
								</tr>
							</table>
				
			</form>
	</fieldset>


</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
