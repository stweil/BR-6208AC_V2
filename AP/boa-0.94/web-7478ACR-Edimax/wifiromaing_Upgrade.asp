<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">

<% language=javascript %>

<style type="text/css">
	#out{width:300px;height:20px;background:#CCCCCC;}
	#in{width:1px; height:20px;background:#ff7f50;color:#000000;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
	.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }
</style>

<SCRIPT>
var Rload = "<% getInfo("Rload"); %>"
var SlaveNum = "<% getInfo("SlaveNum"); %>"
var OneTime = "<% getInfo("OneTime"); %>"
var wizmode = <%getVar("wizmode","","");%>;
var SingleSignonFree = <%getVar("SINGLESIGNON_FREEZE","","");%>;
var SingleSignon = <%getVar("SINGLESIGNON","","");%>;
var timeMAX = "0";
var setloading;
function loading(){
	document.getElementById('progress').innerHTML=parseInt(($('#in').width()/300) * 100, 10)+'%';
}

function init()
{
	$("#help-box").hide();

	if(OneTime == "1")
	{
		document.SlaveTable.checkVersion.disabled=true;
		document.SlaveTable.StartUpgrade.disabled=true;
		document.SlaveTable.checkVersion.style.color=disableButtonColor;
		document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
		if(wizmode==1) //AP mode
		{
			parent.lFrame.location.assign("left_list_ap_sham.asp");
		}
		else if (wizmode==2) //Repeater mode
		{
			parent.lFrame.location.assign("left_list_rep_sham.asp");
		}
	}
	else if(OneTime == "2")
	{
		document.getElementById('SelectFilePage').style.display='none';
		document.getElementById('counting').style.display='block';
		document.getElementById('countingChild1').style.display='block';
		setloading=setInterval('loading()', 1000);
		$(document).ready(function() {
			$("#in").animate({width: '99%'}, 600000,function(){ //900000
				window.clearInterval(setloading);
				document.getElementById("progress").innerHTML="100%";
				$("#in").stop().animate({width: '100%'},function(){
					document.getElementById('counting').style.display='none';
					document.getElementById('countingChild1').style.display='none';
					document.getElementById('countingChild2').style.display='none';
					document.getElementById('ok').style.display='block';
				});
			});
		});
	}
	else
	{
		
		if (SlaveNum !== "0")
		{
			if (SingleSignonFree == "1" && SingleSignon == "1")
			{
				document.SlaveTable.checkVersion.disabled=false;
				document.SlaveTable.checkVersion.style.color=enableButtonColor;
				if(x>="1")
				{
					document.SlaveTable.StartUpgrade.disabled=false;
					document.SlaveTable.StartUpgrade.style.color=enableButtonColor;
				}
				else
				{
					document.SlaveTable.StartUpgrade.disabled=true;
					document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
				}
				if(GoToUpgrade == "0")
				{
					document.SlaveTable.StartUpgrade.disabled=true;
					document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
				}
			}
			if (SingleSignonFree == "1" && SingleSignon == "2")
			{
				document.SlaveTable.checkVersion.disabled=true;
				document.SlaveTable.StartUpgrade.disabled=true;
				document.SlaveTable.checkVersion.style.color=disableButtonColor;
				document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
			}
		}
		else
		{
			document.SlaveTable.checkVersion.disabled=true;
			document.SlaveTable.StartUpgrade.disabled=true;
			document.SlaveTable.checkVersion.style.color=disableButtonColor;
			document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
		}

		if(Rload == "1")
		{
			if(wizmode==1) //AP mode
			{
				parent.lFrame.location.assign("left_list_ap.asp");
			}
			else if (wizmode==2) //Repeater mode
			{
				parent.lFrame.location.assign("left_list_rep.asp");
			}
		}
	}
}

function upgrade_check()
{
	document.SlaveTable.checkVersion.disabled=true;
	document.SlaveTable.StartUpgrade.disabled=true;
	document.SlaveTable.checkVersion.style.color=disableButtonColor;
	document.SlaveTable.StartUpgrade.style.color=disableButtonColor;
	with(document.AutoFWupgrade) {
		action.value = "check_server_version";
		submit();
	}
}

function upgrade_download()
{	
	var DoIt = confirm(showText(WaitMSG));
	if (DoIt == true)
	{
		alert(showText(WaitMSG2));
		with(document.AutoFWupgrade) {
			action.value = "download_server_version";
			submit();
		}
	}
}

</SCRIPT>

</head>
<body onLoad="init()">

<div class="help-information" id="help-box" style="display:none">
	<table class="help-table1">
		<tr><td><div class="help-text" id="help-info1"><script>dw(GroupUpgradeHelp)</script></td></tr>
	</table>
</div>



<script>
	if(OneTime != "2")
	{
		HelpButton();
	}
</script>

<form action="/goform/formWiFiRomaingFWupgrade" method="POST" name="AutoFWupgrade">
	<input type="hidden" value="" name="action">
	<input type="hidden" value="/wifiromaing_Upgrade.asp" name="submit-url">
</form>	

<blockquote>

	<span id='SelectFilePage' style='display:block'>
		<fieldset name="manual_upgrade_fieldset">
			<legend><script>dw(Upgrade)</script></legend>
			<br>

			<table width="700"align="center" border="0">
			<tr>
				<td id="waitTime" style="text-align:center; COLOR: #b40404;" class="itemText" width="100%" height="25px">
					<script>
						if(OneTime == "1")
						{
							document.write("&ensp;(");
							dw(ProcessingPleaseWait);
							document.write(")");
						}
					</script>
				</td>
			</tr>
			</table>
			<form action="" method="POST" name="SlaveTable">
				<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">

					<tr>
						<td width="20%" align="center" class="dnsTableHe"><script>dw(Model)</script></td>
						<td width="20%" align="center" class="dnsTableHe"><script>dw(IPAddr)</script></td>
						<td width="15%" align="center" class="dnsTableHe"><script>dw(CurrentFV)</script></td>
						<td width="15%" align="center" class="dnsTableHe"><script>dw(NewFV)</script></td>
						<td width="30%" align="center" class="dnsTableHe"><script>dw(Status)</script></td>
					</tr>
					<table width="700" cellpadding="2" cellspacing="2" align="center" border="0" class="footBorder">
						<% SlaveList(); %>
					</table>
				</table>
				<br>	
				<span id='UpgradeFail' style='display:none'>
					<td border="0" class="itemText" width="700" height="">
						<script>
							dw(FwUpgradeFailMsg);
						</script>
					</table>
				</span>
				<br>
				<table align="center" border="0" Cellspacing="0" width="700" >
					<tr>
						<td id="checkVer" width="55%" style="text-align:right; padding-top:10px;">
							<script>
							document.write('<input type="button" value="'+showText(CheckFW)+'" name="checkVersion" onclick="upgrade_check()" class="ui-button-nolimit">');	
							</script>
						</td>
						<td id="StartUpgrade" width="45%" style="text-align:left; padding-top:10px;">
							<script>
							document.write('<input type="button" value="'+showText(Upgrade)+'" name="StartUpgrade" onclick="upgrade_download()" class="ui-button-nolimit">');	
							</script>
						</td>
					</tr>
				</table>

			</form>	
		</fieldset>
	</span>

	<form id="checkFWversion">
		<input type="hidden" name="cmd" value="checkSlaveFW">
	</form>

	<script>
		if(OneTime == "1")
		{
			//$(document).ready(function() {
			      CheckSlaveFile = setInterval("Check_Slave_File()", 3000);
			//});

			function Check_Slave_File(){
				timeMAX++;
				if(timeMAX < "7")
				{       
					$.post("/goform/formSlaveResult", $("#checkFWversion").serialize(),function(data){
						if(data=="SlaveResultGet")
						{
							window.clearInterval(CheckSlaveFile);
							window.location.reload();
						}
					},"text");
				}
				else
				{
					window.clearInterval(CheckSlaveFile);
					alert(showText(SlavecheckMSG));
					document.getElementById("waitTime").innerHTML="";
					window.location.reload();
				}
			}
		}
		else if(OneTime == "2")
		{
			//$(document).ready(function() {
			      CheckSlaveFW = setInterval("Check_Slave_FW()", 5000);
			//});

			function Check_Slave_FW(){
				timeMAX++;
				if(timeMAX <= "60") //90
				{       
					$.post("/goform/formSlaveFW", $("#checkFWversion").serialize(),function(data){
						if(timeMAX == "60") //90
						{
							if(data != "SlaveResultGet")	//Download FW fail 
							{
								window.clearInterval(setloading);
								window.clearInterval(CheckSlaveFW);
								document.getElementById('SelectFilePage').style.display='block';
								document.getElementById('counting').style.display='none';
								document.getElementById('countingChild1').style.display='none';
								document.getElementById('UpgradeFail').style.display='block';
								document.getElementById('ok').style.display='none';
							}
							else	//Download FW pass
							{
								document.getElementById('countingChild1').style.display='none';
								document.getElementById('countingChild2').style.display='block';
								window.clearInterval(CheckSlaveFW);
							}
						}
						else	//Download FW pass
						{
							if(data=="SlaveResultGet")
							{
								document.getElementById('countingChild1').style.display='none';
								document.getElementById('countingChild2').style.display='block';
								window.clearInterval(CheckSlaveFW);
							}
						}
					},"text");
				}
				else if(timeMAX == "120") //180
				{
					window.clearInterval(setloading);
				}
				else
				{
					window.clearInterval(CheckSlaveFW);
				}
			}
		}
	</script>

</blockquote>

<span id='counting' style='display:none'>
	<span id='countingChild1' style='display:none'>
		<table width='700' border="0" align='left'>
			<tr>
				<td colspan='2'><span class='restartInfo'><script>dw(FwDownloadMsg);</script></span>
				</td>
			</tr>
		</table>
	</span>
	
	<span id='countingChild2' style='display:none'>
		<table width='700' border="0" align='left'>
			<tr>
				<td colspan='2'><span class='restartInfo'><script>dw(FwUpgradeMsg);</script></span>
				</td>
			</tr>
		</table>
	</span>

	<br>
	<table width='700' border="0" align='left'>
		<tr>
			<td style='width:305px;'>
				<div id='out'>
					<div id='in' style='width:1%;'></div>
				</div>
			</td>
			<td id='progress' style='width:395px;'>1%</td>
		</tr>
	</table>
</span>

<span id='ok' style='display:none'>

	<table width='700' border="0" align='left'>
		<tr>
			<td>
				<span class='restartInfo'><script>dw(upgrade_accomplished)</script></span>
			</td>
		</tr>
	</table>
	<br><br>
	<table width='700' border="0" align='left'>
		<tr>
			<td>
				<span class='restartitemText' style='line-height:22px;'><script>dw(upgrade_accomplished_content)</script></span>
			</td>
		</tr>
	</table>
</span>

<script>
	$("#help-box").hide();
	var SlaveRealNumber = "<% getInfo("SlaveRealNumber"); %>"
	var GoToUpgrade = "<% getInfo("GoToUpgrade"); %>"
	var i,x=0;
	for(i=1; i<=SlaveRealNumber; i++)
	{
		
		if(document.forms['SlaveTable'].elements['Slave'+i].value == "1")
		{
			x++;
		}
		if(i==SlaveRealNumber) //for Master
		{
			if(document.forms['SlaveTable'].elements['Master'].value == "1")
			{
				x++;
			}		
		}
	}
</script>	
</body>
</html>
