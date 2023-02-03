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
var wiredIPTVTbl =	<%getInfo("wiredIPTVAll");%>;

function init()
{
	$("#help-box").hide();
	with(document.IPTVSetup)
	{
		for(i=1;i<=4;i++)
		{
			if(wiredIPTVTbl[i] == 1)
				document.getElementById('Wired_IPTV_Port' + i ).checked=true;
			else
				document.getElementById('Wired_IPTV_Port' + i ).checked=false;
		}

		if(wiredIPTVTbl[0] == 1)
			Wired_IPTV_Enabled.checked=true;
		else
		{
			Wired_IPTV_Enabled.checked=false;
			for(i=1;i<=4;i++)
			{
				document.getElementById('Wired_IPTV_Port' + i ).disabled = true;
			}
		}
	}
}

function saveChanges(rebootFlag)
{
	with(document.IPTVSetup) {
		if(Wired_IPTV_Enabled.checked==true)
			Wired_Port_Check();

		if(rebootFlag)
			isApply.value = "ok;"
		submit();
	}
}

function Wired_IPTV_Switch()
{
	with(document.WiredIPTVEnabled) 
  {
		if (document.IPTVSetup.Wired_IPTV_Enabled.checked==true)
		 Wired_IPTV_Enabled.value="yes";
		else
		 Wired_IPTV_Enabled.value="no";
		submit();
	}
}
function Wired_Port_Check()
{
	document.IPTVSetup.set_Wired_IPTV_Port.value="1";
	for(i=1;i<=4;i++)
	{
		if (document.getElementById('Wired_IPTV_Port' + i ).checked==true)
			document.getElementById('Wired_IPTV_Port' + i ).value="1";
		else
			document.getElementById('Wired_IPTV_Port' + i ).value="0";
		//alert(document.getElementById('Wired_IPTV_Port' + i ).value);
	}
}
</SCRIPT>
</head>


<body class="" onLoad="init()">

<!--====================================================================================================================================-->

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(iptvHelp)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>
	<form action="/goform/formIPTV" method=POST name="WiredIPTVEnabled">
		<input type="hidden" value="1" name="iqsetupclose">
		<input type="hidden" value="" name="Wired_IPTV_Enabled">	
		<input type="hidden" value="" name="isApply">
		<input type="hidden" value="/inter_iptv.asp" name="submit-url">
	</form>	

	<fieldset name="iptv_fieldset" id="iptv_fieldset">
		<legend><script>dw(IPTV)</script></legend>
		<form action="/goform/formIPTV" method=POST name="IPTVSetup">
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td colspan="2"><input type="checkbox" id="Wired_IPTV_Enabled" name="Wired_IPTV_Enabled" value="" onclick="Wired_IPTV_Switch()">&nbsp;<span class="itemText"><script>dw(enable)</script>&nbsp;<script>dw(WIRED)</script>&nbsp;<script>dw(IPTV)</script></span>
				</td>
			</tr>

			</table>
			<table align="center" border="0" cellspacing="5" cellpadding="0" >
				<tr height="55">
					<td align="center" width="55" class="without_pic_iptv" ><a>4</a></td>
					<td align="center" width="55" class="without_pic_iptv" ><a>3</a></td>
					<td align="center" width="55" class="without_pic_iptv" ><a>2</a></td>
					<td align="center" width="55" class="without_pic_iptv" ><a>1</a></td>
					<td align="center" width="55" class="without_pic_iptv" ><a>&nbsp;</a></td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="1" cellpadding="4">

			<tr>
				<td align="center" width="55">
					<input type="checkbox" id="Wired_IPTV_Port4" name="Wired_IPTV_Port4" value="0">
				</td>
				<td align="center" width="55">
					<input type="checkbox" id="Wired_IPTV_Port3" name="Wired_IPTV_Port3" value="0">
				</td>
				<td align="center" width="55">
					<input type="checkbox" id="Wired_IPTV_Port2" name="Wired_IPTV_Port2" value="0">
				</td>
				<td align="center" width="55">
					<input type="checkbox" id="Wired_IPTV_Port1" name="Wired_IPTV_Port1" value="0">
				</td>
				<td align="center" width="55" class="itemText"><script>dw(Internet)</script>
				</td>
			</tr>
			</table>

			<br>
			<table align="center" border="0" Cellspacing="0" width="700">
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
			<input type="hidden" value="" name="set_Wired_IPTV_Port">
			<input type="hidden" value="1" name="iqsetupclose">
			<input type="hidden" value="" name="isApply">
			<input type="hidden" value="/inter_iptv.asp" name="submit-url">
		</form>	
	</fieldset>	
<script>
	$("#help-box").hide();
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>


</html>
