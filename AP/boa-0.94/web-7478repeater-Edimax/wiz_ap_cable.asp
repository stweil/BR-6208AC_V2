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

function goBack()
{
	window.location.assign("wiz_apmode.asp");
}
function goNext()
{
	window.location.assign("cable_redirect.asp");
}
function setTableWidth()
{
	//document.all.tablewidth.className="tablewidth"+stype;
	document.getElementById("tablewidth").className="tablewidth"+stype;
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
.tablewidth12 {width:70%;}
.tablewidth13 {width:70%;}
</style>
</head>
<body onLoad="setTableWidth()">

<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(apmode)</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:156px;width:780px" align="center">
		<tr>
		<td align="center" class="Arial_16">
		<br> 
		<img src="graphics/AP_disconnect.gif" STYLE="cursor:pointer" width="567" height="156" border="0" name="Image3" onClick="">
		</td>
		</tr>
	</table>
	
	<table border="0" style="height:146px;" id="tablewidth" class="tablewidth0" align="center">
		<tr><td align="left" class="Arial_16" style="line-height:25px;"><script>dw(nsurethecable)</script></td></tr>
	</table>
	
	<table border="0" style="height:87px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:30px;width:780px" align="center">
	<tr>
	<script>

			document.write('<td align="right" style="width:390px">');
			document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
			document.write('</td>');
			document.write('<td align="left" style="width:390px">');
			document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');
			document.write('</td>');
		
		</script>
	</tr>
	</table>
		
	<table border="0" style="height:30px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

</td>
</tr>
</table>
</div>
</body>
</html>
