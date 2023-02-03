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
function changeMode()
{
	document.check.ModeSelect.value = "fistIsAP";
	document.check.submiturl.value = "/wiz_5in1.asp";
	document.check.submit();
}
function goNext()
{
	document.check.ModeSelect.value = "Router";
	document.check.submiturl.value = "/wiz_routerselect.asp";
	document.check.submit();
}
function setTableWidth()
{
	//document.all.tablewidth.className="tablewidth"+stype;
	document.getElementById("tablewidth").className="tablewidth"+stype;
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
.tablewidth13 {width:90%;}
.tablewidth14 {width:90%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="check">
<input type="hidden" value="" name="ModeSelect">
<input type="hidden" value="" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" align="center" background="/graphics/empty-dot.jpg" style="height:600px;width:800px">
<tr>
<td align="center">

	<table border="0" align="center" style="height:90px;width:700px">
		<tr><td align="right" class="Arial_16"><script>dw(routermode)</script></td></tr>
	</table>

	<table border="0" align="center" style="height:128px;" id="tablewidth" class="tablewidth0">
		<tr><td align="center" class="Arial_16" style="text-align:left;"><script>dw(aindex_sta)</script></td></tr>
	</table>

	<table border="0" align="center" width="600" height="250">
		<tr>
			<td align="center">
		 		<img src="graphics/router_mode.png" width="600" height="250">
			</td>
		</tr>
	</table>
	
	<table border="0" style="height:28px;width:780px" align="center">
		<tr>
			<script>
				document.write('<td align="right">');
				document.write('<input type="button" value="'+showText(router_no)+'" name="close" onClick="changeMode();" class="button_IQ3" style="cursor:pointer">&nbsp;&nbsp;');
				document.write('</td>');
				document.write('<td align="left">');
				document.write('&nbsp;&nbsp;<input type="button" value="'+showText(router_yes)+'" name="close" onClick="goNext();" class="button_IQ3" style="cursor:pointer">');
				document.write('</td>');
			</script>
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
