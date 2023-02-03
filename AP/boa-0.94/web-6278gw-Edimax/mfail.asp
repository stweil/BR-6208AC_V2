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
var wizMode = <%getVar("wizmode","","");%>;

function goBack()
{
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
.tablewidth13 {width:58%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/chkLink" method="POST" name="check">
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

	<table border="0" style="height:80px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	
	<table border="0" style="height:35px;width:780px" align="center">
		<tr>
			<td align="center" id="tablewidth" class="tablewidth0" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;font-weight:bold; line-height:35px;">
				<script>dw(noresponse)</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:109px;" id="table1width" class="tablewidth0" align="center">
		<tr>
			<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px;">
				<script>
					dw(wisp_detectfail_1);
				</script>
			</td>
		</tr>
	</table>
		
	<table border="0" style="height:153px;width:780px" align="center">
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
<input type="hidden" value="/adhcp_fail.asp" name="submit-url">
</form>
</body>
</html>
