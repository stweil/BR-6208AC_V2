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
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var wizMode = <%getVar("wizmode","","");%>;
function goBack()
{
	if ((wizMode == 0))
		window.location.assign("/wiz_routerselect.asp");
	else
		window.location.assign("/wiz_5in1.asp");
}
function goNext()
{
	document.check.submit();
}
function setTableWidth()
{
	document.getElementById('tablewidth1').className="tablewidth"+stype;
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
.tablewidth13 {width:82%;}
.tablewidth14 {width:82%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/chkLink" method="POST" name="check">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" align="center" style="height:90px;width:700px">
		<tr><td align="right" class="Arial_16"><script>dw(routermode)</script></td></tr>
	</table>

	<table border="0" align="center" width="567" height="150">
		<tr>
			<td align="center">
				<img src="graphics/router_setup.gif" width="567" height="123">
			</td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:122px;" id="tablewidth1" class="tablewidth0">
		<tr><td align="left" class="Arial_16" style="line-height:25px;"><script>dw(hw_info3)</script></td></tr>
	</table>
	
	<table border="0" align="center" style="height:109px;width:780px">
		<tr><td></td></tr>
	</table>

	<table border="0" align="center" style="height:28px;width:780px">
		<tr>
			<script>
			if(iqsetup_finish == 0)
			{
				document.write('<td align="right" style="width:390px">');
				document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
				document.write('</td>');
				document.write('<td align="left" style="width:390px">');
				document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');
				document.write('</td>');
			}
			else
			{
				document.write('<td align="center">');
				document.write('<input type="button" value="'+showText(getStarted)+'" name="close" onClick="goNext();" class="button_IQ" style="cursor:pointer">');
				document.write('</td>');
			}
			</script>
		</tr>
	</table>

	<table border="0" align="center" style="height:60px;width:780px">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
<input type="hidden" value="/redirect.asp" name="submit-url">
</div>
</form>
</body>
</html>
