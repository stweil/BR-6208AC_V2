<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<style>
legend         {FONT-SIZE: 12pt; COLOR: #b40404; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial;}
.itemText      {FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial;}
.itemText1-len {FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial; }
.itemText2-len {FONT-SIZE: 10pt; COLOR: #000000; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial; }
.itemText3-len {FONT-SIZE: 10pt; COLOR: #000000; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial; }
.ui-button     {FONT-SIZE: 10pt; COLOR: #000000; FONT-FAMILY: Calibri,Microsoft JhengHei,Arial; }
</style>
<% language=javascript %>

<script>
var manualavdcontrol = <%getVar("manualavdcontrol","","");%>;

var secs = 60;
var wait = secs * 1000;

function update(num)
{
	printnr = (wait / 1000)-num;
	document.advanced_management.into_effect.value = "Save and Reboot (" + printnr + ")";
}

function timer()
{
	displayObj('all','display');
	document.advanced_management.into_effect.value = "Save and Reboot";
	window.location.replace("/advanced_management.asp");
}

function displayObj(input1,input2)
{
	with(document.advanced_management)
	{
		if(input1=="all")
		{
			if(input2=="display")
			{
				manualAdaptivityEnabled.disabled = false;
				adaptivity24g_on.disabled        = false;
				adaptivity24g_off.disabled       = false;
				adaptivity5g_on.disabled         = false;
				adaptivity5g_off.disabled        = false;
				verifyCode.disabled              = false;
				into_effect.disabled             = false;

			}
			else
			{
				manualAdaptivityEnabled.disabled = true;
				adaptivity24g_on.disabled        = true;
				adaptivity24g_off.disabled       = true;
				adaptivity5g_on.disabled         = true;
				adaptivity5g_off.disabled        = true;
				verifyCode.disabled              = true;
				into_effect.disabled             = true;
			}
		}
		else if(input1=="Adaptivity")
		{
			if(manualAdaptivityEnabled.checked == true)
			{
				adaptivity24g_on.disabled  = false;
				adaptivity24g_off.disabled = false;
				adaptivity5g_on.disabled   = false;
				adaptivity5g_off.disabled  = false;
			}
			else
			{
				adaptivity24g_on.disabled  = true;
				adaptivity24g_off.disabled = true;
				adaptivity5g_on.disabled   = true;
				adaptivity5g_off.disabled  = true;
			}
		}
	}
}

function saveChanges()
{
	var setValue=0;
	with(document.advanced_management)
	{
		if (manualAdaptivityEnabled.checked == true)	setValue |= 0x001;
		if (adaptivity24g[0].checked == true)	setValue |= 0x002;
		if (adaptivity5g[0].checked == true)	setValue |= 0x004;

		document.Adv_Management.AdvManagement.value=setValue;
		document.Adv_Management.AdvSetup_verifyCode.value = verifyCode.value;

		$.post("/goform/formAdvManagement",$("#submit_adv_management").serialize());
	}

	displayObj('all','non-display');
	document.advanced_management.into_effect.value = "Save and Reboot (" + secs + ")";
	for(i = 1; i <= secs; i++)
	{
		setTimeout("update(" + i + ")", i * 1000);
	}
	setTimeout("timer()", wait);
}
</script>
</head>
<body>
<blockquote>
	<form name="advanced_management">
		<fieldset>
		<legend>Advanced Management</legend>

			<br>
			<table align="center" border="0" Cellspacing="1" cellpadding="4" width="400">
				<tr>
					<td colspan="2" align='left'>
						<div class="itemText">
							<script>
								
								if(manualavdcontrol & 1)
									document.write('<input type="checkbox" name="manualAdaptivityEnabled" onclick="displayObj(\'Adaptivity\',\'null\')" checked>');
								else
									document.write('<input type="checkbox" name="manualAdaptivityEnabled" onclick="displayObj(\'Adaptivity\',\'null\')">');
							</script>
							<span>Manual Control Adaptivity</span>
						</div>
					</td>
				</tr>
			
				<tr>
					<td width="40%" align='right'><div class="itemText1-len"><span>2.4GHz Wireless :</span></div></td>
					<td width="60%" >
						<div class="itemText2-len">
							<script>
								if(manualavdcontrol & 2){
									document.write('<input type="radio" id="adaptivity24g_on" name="adaptivity24g" checked><span>Enable</span>&nbsp;');
									document.write('<input type="radio" id="adaptivity24g_off" name="adaptivity24g"><span>Disable</span>');
								}
								else{
									document.write('<input type="radio" id="adaptivity24g_on" name="adaptivity24g"><span>Enable</span>&nbsp;');
									document.write('<input type="radio" id="adaptivity24g_off" name="adaptivity24g" checked><span>Disable</span>');
								}
								
							</script>
						</div>
					</td>	
				</tr>
				<tr>
					<td align='right'><div class="itemText1-len"><span>5GHz Wireless :</span></div></td>
					<td>
						<div class="itemText2-len">
							<script>
								if(manualavdcontrol & 4){
									document.write('<input type="radio" id="adaptivity5g_on" name="adaptivity5g" checked><span>Enable</span>&nbsp;');
									document.write('<input type="radio" id="adaptivity5g_off" name="adaptivity5g"><span>Disable</span>');
								}
								else{
									document.write('<input type="radio" id="adaptivity5g_on" name="adaptivity5g"><span>Enable</span>&nbsp;');
									document.write('<input type="radio" id="adaptivity5g_off" name="adaptivity5g" checked><span>Disable</span>');
								}
								
							</script>
						</div>
					</td>	
				</tr>
			</table>
			<br>
		</fieldset>

		<br>

		<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td width="45%" align='right'><div class="itemText1-len"><span>Verify Code</span></div></td>
				<td width="55%" align='left'>
					<div class="itemText2-len">
						<input type="test" name="verifyCode" value="" size="10" maxlength="10">
					</div>
				</td>
			</tr>
		</table>

		<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
			<tr>
				<td style="text-align:center;">
					<div>
						<input class="ui-button" type="button" name="into_effect" value="Save and Reboot" onclick="saveChanges()">
					</div>
				</td>
			</tr>
		</table>
	</form>
</blockquote>

<form name="Adv_Management" method="POST" action="/goform/formAdvManagement" id="submit_adv_management">
	<input type="hidden" value="" name="AdvSetup_verifyCode">
	<input type="hidden" value="" name="AdvManagement">
	<input type="hidden" value="yes" name="AdvSetup_into_effect">
</form>	
<script>
	displayObj('Adaptivity','null');
</script>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>


