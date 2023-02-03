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

var APcl_enable5g = "<%getInfo("APcl_enable5g","","");%>";
var APcl_enable2g = "<%getInfo("APcl_enable2g","","");%>";

var WISP_enable5g = "<%getInfo("WISP_enable5g","","");%>";
var WISP_enable2g = "<%getInfo("WISP_enable2g","","");%>";

function goBack()
{
	if( (wizMode == "3") && (APcl_enable5g == "1") )
		window.location.assign("wiz_APcl5gauto.asp");
	else if( (wizMode == "3") && (APcl_enable2g == "1") )
		window.location.assign("wiz_APcl24gauto.asp");
	else if( (wizMode == "4") && (WISP_enable5g == "1") )
		window.location.assign("wiz_WISP5gauto.asp");
	else if( (wizMode == "4") && (WISP_enable2g == "1") )
		window.location.assign("wiz_WISP24gauto.asp");
	else	
		window.location.assign("hwsetup.asp");
}

function goNext()
{
	window.location.assign("wifi.asp");
}

function setTableWidth()
{
	if(stype>13) stype=13;
	document.all.tablewidth.className="tablewidth"+stype;
}
</script>
<style type="text/css">

.tablewidth0 {width:51%;}
.tablewidth1 {width:65%;}
.tablewidth2 {width:55%;}
.tablewidth3 {width:59%;}
.tablewidth4 {width:48%;}
.tablewidth5 {width:62%;}
.tablewidth6 {width:55%;}
.tablewidth7 {width:57%;}
.tablewidth8 {width:52%;}
.tablewidth9 {width:73%;}
.tablewidth10 {width:72%;}
.tablewidth11 {width:57%;}
.tablewidth12 {width:68%;}
.tablewidth13 {width:34%;}

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
			<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px; font-weight:bold;">
				<script>dw(internetConnected)</script></td>
			</tr>
	</table>
	
	<table border="0" style="height:109px;" id="tablewidth" class="tablewidth0" align="center">
		<tr>
			<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px;">
				<script>dw(nextWireless)</script></td>
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
<input type="hidden" value="/aconnected.asp" name="submit-url">
</form>
</body>
</html>
