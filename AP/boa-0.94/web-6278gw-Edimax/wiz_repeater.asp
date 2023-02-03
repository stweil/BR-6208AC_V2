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
	//window.location.assign("wiz_repselect.asp");
	document.upBoth.submit();
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
<body onLoad="setTableWidth()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="upBoth">
<input type="hidden" value="1" name="rep_enable5g">
<input type="hidden" value="1" name="rep_enable2g">
<input type="hidden" value="/wiz_repselect.asp" name="submiturl">
</form>
<form action="/goform/chkLink" method="POST" name="check">
<input type="hidden" value="2" name="wizmode_value">
<input type="hidden" value="1" name="back25in1">
<input type="hidden" value="/wiz_5in1.asp" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(repeatermode)</script>
			</td>
		</tr>
	</table>

	<table border="0" align="center" width="567" height="150">
		<tr>
			<td align="center">
				<img src="graphics/repeater_setup.gif" width="567" height="123">
			</td>
		</tr>
	</table>

	<table border="0" style="height:191px;" id="tablewidth" class="tablewidth0" align="center" >
		<tr>
			<td align="left"><span class="Arial_16" style="line-height:25px;"><script>dw(repeatermode_info)</script></span></td>
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
