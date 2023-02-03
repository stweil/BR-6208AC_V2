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
<script type="text/javascript">
//var WISP_enable5g = "<%getInfo("WISP_enable5g");%>";
//var WISP_enable2g = "<%getInfo("WISP_enable2g");%>";

function init()
{
	document.WISP_mode.enable5g.checked = true;
	document.WISP_mode.enable2g.checked = false;
}

function modeswitch(i)
{
	if ( i == 1){
			document.WISP_mode.enable5g.checked = true;
			document.WISP_mode.enable2g.checked = false;
	}
	else{
			document.WISP_mode.enable5g.checked = false;
		 	document.WISP_mode.enable2g.checked = true;
	}
}

function goBack()
{
	window.location.assign("/wiz_WISPselect.asp");
}
function goNext()
{
	if ( document.WISP_mode.enable5g.checked == true )
		document.WISP_mode.WISP_enable5g.value = "1";
	else
		document.WISP_mode.WISP_enable5g.value = "0";	

	if ( document.WISP_mode.enable2g.checked == true )
		document.WISP_mode.WISP_enable2g.value = "1";
	else
		document.WISP_mode.WISP_enable2g.value = "0";	

	document.WISP_mode.submiturl.value = "/wiz_WISPredirect.asp";
	document.WISP_mode.submit(); 
}

function setTableWidth()
{
	if(stype>13) stype=13;

	document.getElementById('tablewidth').className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:80%;}
.tablewidth1 {width:80%;}
.tablewidth2 {width:80%;}
.tablewidth3 {width:80%;}
.tablewidth4 {width:80%;}
.tablewidth5 {width:80%;}
.tablewidth6 {width:80%;}
.tablewidth7 {width:80%;}
.tablewidth8 {width:80%;}
.tablewidth9 {width:80%;}
.tablewidth10 {width:80%;}
.tablewidth11 {width:80%;}
.tablewidth12 {width:80%;}
.tablewidth13 {width:76%;}
</style>
</head>
<body onload="init()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="WISP_mode">
<input type="hidden" value="" name="WISP_enable5g">
<input type="hidden" value="" name="WISP_enable2g">
<input type="hidden" value="" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">

<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
	<tr>
		<td align="center">

			<table border="0" style="height:90px;width:700px" align="center">
				<tr><td align="right" class="Arial_16"><script>dw(wisp)</script></td></tr>
			</table>

			<table border="0" style="height:101px;" id="tablewidth" class="tablewidth0" align="center">
				<tr>
					<td align="center" class="Arial_16"><script>dw(wisp_select)</script></td>
				</tr>
			</table>
	
			<table border="0" align="center" style="height:0px;width:780px">
				<tr>
					<td align="right" width="40%"><input type="radio" name="enable5g" onClick="modeswitch(1)">
					<td align="left" width="60%" class="Arial_16">1.&nbsp;<script>dw(wizard_repeater_mode1)</script></td>
				</tr>
			</table>

			<table border="0" align="center" style="height:0px;width:780px">
				<tr>
					<td align="right" width="40%"><input type="radio" name="enable2g" onClick="modeswitch(2)">
					<td align="left" width="60%" class="Arial_16">2.&nbsp;<script>dw(wizard_repeater_mode2)</script></td>
				</tr>
			</table>			

			<table border="0" style="height:231px;width:780px" align="center">
				<tr><td></td></tr>
			</table>

			<table border="0" style="height:30px;width:780px" align="center">
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
</form>
</body>
</html>
