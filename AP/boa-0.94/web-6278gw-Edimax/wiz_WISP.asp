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
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var wizMode = <%getVar("wizmode","","");%>;

function goBack()
{
	//window.location.assign("wiz_5in1.asp");
	document.check.submit();
}
function goNext()
{
	window.location.assign("wiz_WISPselect.asp");
}
function setTableWidth()
{
	if(stype>13) stype=13;
	//document.all.tablewidth.className="tablewidth"+stype;
	document.getElementById("tablewidth").className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:95%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:95%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:98%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:90%;}
.tablewidth10 {width:93%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:65%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/chkLink" method="POST" name="check">
<input type="hidden" value="4" name="wizmode_value">
<input type="hidden" value="1" name="back25in1">
<input type="hidden" value="/wiz_5in1.asp" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(wisp)</script>
			</td>
		</tr>
	</table>

	<table border="0" align="center" width="567" height="150">
		<tr>
			<td align="center">
				<img src="graphics/wisp_setup.gif" width="567" height="123">
			</td>
		</tr>
	</table>
		
	<table border="0" style="height:191px;" id="tablewidth" class="tablewidth0" align="center">
		<tr>
			<td align="left" class="Arial_16" style="line-height:25px;"><script>dw(wisp_info)</script></td>
		</tr>
	</table>
	
	<table border="0" style="height:42px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	
	<table border="0" style="height:28px;width:780px" align="center">
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
