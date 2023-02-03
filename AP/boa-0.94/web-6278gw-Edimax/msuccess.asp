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
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var wanMode = <%getVar("wanMode","","");%>;

function goBack()
{
	if((wizMode==0) && (wanMode>=2))
		window.location.assign("aIndex2.asp");
	else
		window.location.assign("mstart.asp");
}
function goNext()
{
	window.location.assign("wifi.asp");
}
function setTableWidth()
{
	if(stype>13) stype=13;

	document.all.tablewidth.className="tablewidth"+stype;
	document.all.table1width.className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:80%;}
.tablewidth1 {width:80%;}
.tablewidth2 {width:80%;}
.tablewidth3 {width:80%;}
.tablewidth4 {width:75%;}
.tablewidth5 {width:80%;}
.tablewidth6 {width:80%;}
.tablewidth7 {width:80%;}
.tablewidth8 {width:80%;}
.tablewidth9 {width:80%;}
.tablewidth10 {width:80%;}
.tablewidth11 {width:80%;}
.tablewidth12 {width:80%;}
.tablewidth13 {width:75%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/chkLink" method="POST" name="check">
<input type="hidden" value="/msuccess.asp" name="submit-url">
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

	<table border="0" style="height:68px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	
	<table border="0" style="height:30px;width:780px" align="center">
		<tr>
			<td align="center" id="tablewidth" class="tablewidth0" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px; font-weight:bold;">
				<script>dw(internetConnected)</script></td>
			</tr>
	</table>

	<table border="0" style="height:34px;" id="table1width" class="tablewidth0" align="center">
		<tr>
			<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px;">
				<script>dw(nextWireless)</script>
			</td>
		</tr>
	</table>
	
	<table border="0" style="height:233px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

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
</form>
</body>
</html>
