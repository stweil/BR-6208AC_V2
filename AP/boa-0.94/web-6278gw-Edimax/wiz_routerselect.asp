<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript">
var wiz_gwManu = "<%getInfo("wiz_gwManu");%>";
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;

function init()
{
	if (iqsetup_finish != 0){
		if(wiz_gwManu==0){
			document.router_mode.iq_auto.checked = true;
			document.router_mode.iq_hand.checked = false;
		}
		else{
			document.router_mode.iq_auto.checked = false;
			document.router_mode.iq_hand.checked = true;	
		}
	}
	else{
		document.router_mode.iq_auto.checked = true;
		document.router_mode.iq_hand.checked = false;	
	}
}

function modeswitch (i)
{
	if (i == 1){
		//iQsetup by Auto
		document.router_mode.iq_auto.checked = true;
		document.router_mode.iq_hand.checked = false;
	}
	else{
		//iQsetup by hand
		document.router_mode.iq_auto.checked = false;
	 	document.router_mode.iq_hand.checked = true;
	}
}

function goBack()
{
	window.location.assign("/aIndex2.asp");
}

function goNext()
{
	if ( document.router_mode.iq_auto.checked == true){
		//iQsetup by Auto
		document.router_mode.wiz_gwManu.value = "0";
		document.router_mode.submiturl.value = "/hwsetup.asp";	
	}
	else{
		//iQsetup by Hand
		document.router_mode.wiz_gwManu.value = "1";
		document.router_mode.submiturl.value = "/mstart.asp";
	}
	document.router_mode.submit(); 
}

function setTableWidth()
{
	document.getElementById('tablewidth1').className="tablewidth"+stype;
	document.getElementById('tablewidth2').className="tablewidth"+stype;
	document.getElementById('tablewidth3').className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:90%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:90%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:80%;}
.tablewidth14 {width:80%;}
</style>
</head>
<body onload="setTableWidth();init();">
<form action="/goform/wiz_5in1_redirect" method="POST" name="router_mode">
<input type="hidden" value="" name="wiz_gwManu">
<input type="hidden" value="" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr><td align="right" class="Arial_16"><script>dw(routermode)</script></td></tr>
	</table>

	<table border="0" align="center" style="height:87px;" id="tablewidth1" class="tablewidth0">
		<tr><td align="left" class="Arial_16"><script>dw(wiz_gwinfo)</script></td></tr>
	</table>
	
	<table border="0" align="center" style="height:0px;" id="tablewidth2" class="tablewidth0">
		<tr>
			<td align="right" width="40%">
				<input type="radio" value="1" name="iq_auto" onClick="modeswitch(1)">
			</td>
			<td align="left" width="60%" class="Arial_16">1.&nbsp;<script>dw(wizard_router_mode1)</script></td>
		</tr>
	</table>

	<table border="0" align="center" style="height:0px;" id="tablewidth3" class="tablewidth0">
		<tr>
			<td align="right" width="40%">
				<input type="radio" value="0" name="iq_hand" onClick="modeswitch(2)">
			</td>
			<td align="left" width="60%" class="Arial_16">2.&nbsp;<script>dw(wizard_router_mode2)</script></td>
		</tr>
	</table>			

	<table border="0" align="center" style="height:245px;width:780px">
		<tr><td></td></tr>
	</table>

	<table border="0" align="center" style="height:28px;width:780px">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>

	<table border="0" align="center" style="height:60px;width:780px">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>
</form>
</body>
</html>
