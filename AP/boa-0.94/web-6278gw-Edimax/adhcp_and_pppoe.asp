<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var wiz_gwManu = "<% getInfo("wiz_gwManu"); %>";
var pppFail = "<% getInfo("pppFail"); %>";
var wiz_wispManu = "<% getInfo("wiz_wispManu"); %>";
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>";
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>";



function submit_now()
{
	$.ajax({
	  url: 'goform/setWAN',
	  type: 'POST',
	  data: $('#tcpip').serialize(),
	  async: true,
	  dataType:'text',

	  success: function () {
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  },
	  error: function(){
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  }
	});
}



function check(input){

	var x = input

	if (x.value != ""){
		if (x.value.search("\'") != -1 || x.value.search("\"") != -1){
			alert("Invalid value")
			x.value = x.defaultValue
			x.focus()
			return false
		}
		else
			return true
	}
	else
		return true
}

function goBack(){

	if (wizMode==4){
		if (WISP_enable5g==1)
			window.location.assign("wiz_WISP5gauto.asp");
		else		
			window.location.assign("wiz_WISP24gauto.asp");
	}
	else{
		window.location.assign("hwsetup.asp");
	}
}

function goNext(){
	if(document.tcpip.ConnectionMode[0].checked==true){
		document.applyform.submit();
	}
	else{
		if (!strRule(document.tcpip.pppUserName,showText(username))) return false;
		if (!strRule(document.tcpip.pppPassword,showText(password))) return false;

		document.tcpip.wiz_gwManu.value = wiz_gwManu;
		document.tcpip.wiz_wispManu.value = wiz_wispManu;

		submit_now();
	}
}

function ConnectionSwitch(index)
{			
	if(!index)
	{
		document.tcpip.ConnectionMode[0].checked=true;
		document.tcpip.ConnectionMode[1].checked=false;
		document.getElementById('ppp').style.display = "none";
		document.getElementById('nullppp').style.display = "block";
	}
	else
	{
		document.tcpip.ConnectionMode[0].checked=false;
		document.tcpip.ConnectionMode[1].checked=true;
		document.getElementById('ppp').style.display = "block";
		document.getElementById('nullppp').style.display = "none";
		if(pppFail == "1")
			document.getElementById('checkppp').innerHTML = showText(checkNamePassword);
	}
}
</script>
</head>
<body>
<form action="/goform/saveAndReboot" method="POST" name="applyform">
	<input type="hidden" value="/wifi.asp" name="RedirectURL">
	<input type="hidden" value="0" name="wanMode">
</form>
<form action="/goform/setWAN" method="POST" name="tcpip" id="tcpip">
	<input type="hidden" value="" name="wiz_gwManu">
	<input type="hidden" value="" name="wiz_wispManu">
	<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
	<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
	<tr>
	<td align="center">

		<table border="0" style="height:90px;width:700px" align="center">
			<tr>
				<td align="right" class="Arial_16">
					<script>
						if(wizMode==0)
							dw(routermode);
						else if(wizMode==1)
							dw(apmode);
						else if(wizMode==2)
							dw(repeatermode);
						else if(wizMode==3)
							dw(APcl);
						else if(wizMode==4)
							dw(wisp);
						else
							dw(repeatermode);
					</script>
				</td>
			</tr>
		</table>
	
		<table border="0" style="height:150px;width:80%" align="center">
			<tr>
				<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px; line-height:21px;">
					<script>dw(chooseConnectType)</script>
				</td>
			</tr>
		</table>
	

		<table border="0" style="height:70px;width:780px" align="center">
			<tr>
				<td align="right" style="width:30%;">
					<input type="radio" name="ConnectionMode" onClick="ConnectionSwitch(0)" checked>
				</td>
				<td align="left" style="width:70%;">
					<script>dw(chooseDynamicIP)</script>
				</td>
			</tr>
	
			<tr>
				<td align="right">
					<input type="radio" name="ConnectionMode" onClick="ConnectionSwitch(1)" >
				</td>
				<td align="left">
					<script>dw(choosePPPoE)</script>
				</td>
			</tr>
		</table>

		<span id="ppp" style="display:none">
		<table border="0" style="height:80px;width:780px" align="center">
			<tr>
				<td align="right" style="width:40%;font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
					<script>dw(username)</script>&nbsp;:
				</td>
			
				<td align="left" style="width:60%;">
					<input type="text" class="Arial_16" name="pppUserName" size="20" maxlength="64" value="" onchange="check(this)">
				</td>
			</tr>
			<tr>
				<td align="right">
					<script>dw(password)</script>&nbsp;:
				</td>

				<td align="left">
					<input type="text" class="Arial_16" name="pppPassword" size="20" maxlength="64" value="" onchange="check(this)">
				</td>
			</tr>
		</table>
		<table border="0" style="height:30px;width:780px" align="center">
			<tr>
				<td id="checkppp" align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;color:#FF0000">
				</td>
			</tr>
		</table>
		<table border="0" style="height:60px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
		</span>


		<span id="nullppp" style="display:none">
		<table border="0" style="height:170px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
		</span>


		<table border="0" style="height:28px;width:780px" align="center">
			<tr>
				<td align="right" style="width:390px">
					<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
				</td>
			
				<td align="left" style="width:390px">
					<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
				</td>
			</tr>
		</table>

		<table border="0" style="height:60px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</td>
	</tr>
	</table>
	</div>
	<input type="hidden" value="/adhcp_and_pppoe.asp" name="submit-url">
	<input type="hidden" value="" name="DNSMode">
	<input type="hidden" value="2" name="wanMode">
	<input type="hidden" value="1" name="conType">
	<input type="hidden" value="1" name="DHCPtoPPPOE">
</form>

<script>
if(pppFail == "1")
	ConnectionSwitch(1);
else
	ConnectionSwitch(0);
</script>
</body>
</html>
