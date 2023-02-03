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
var WISP_enable5g = "<%getInfo("WISP_enable5g");%>";
var WISP_enable2g = "<%getInfo("WISP_enable2g");%>";
var wispConnect = "<%getInfo("wispConnect");%>";

function goBack()
{
if ( WISP_enable5g == "1" ) 
	window.location.replace("/wiz_WISP5gauto.asp");
else
	window.location.replace("/wiz_WISP24gauto.asp");
}

function goNext()
{
	document.check.submit();
}

function init()
{
	if ( wispConnect == 1)
		{
			document.getElementById('Connect_success').style.display="block";
			document.getElementById('Connect_fail').style.display="none";	
		}
	else
		{
			document.getElementById('Connect_fail').style.display="block";
			document.getElementById('Connect_success').style.display="none";	
		}

	setTableWidth();
}

function setTableWidth()
{
	if(stype>13) stype=13;
	document.getElementById('tablesuccesswidth').className="tablewidth"+stype;
	document.getElementById('tablefailwidth').className="tablewidth"+stype;
}
</script>

<style type="text/css">
.tablewidth0 {width:70%;}
.tablewidth1 {width:55%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:56%;}
.tablewidth4 {width:85%;}
.tablewidth5 {width:70%;}
.tablewidth6 {width:80%;}
.tablewidth7 {width:50%;}
.tablewidth8 {width:70%;}
.tablewidth9 {width:85%;}
.tablewidth10 {width:70%;}
.tablewidth11 {width:60%;}
.tablewidth12 {width:80%;}
.tablewidth13 {width:44%;}
</style>
</head>
<body onLoad="init()">
<form action="/goform/chkLink" method="POST" name="check">
<input type="hidden" value="/redirect.asp" name="submit-url">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(wisp);</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:80px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<span id="Connect_success" style="display:none">
		<table border="0" align="center" style="height:35px;width:780px;">
			<tr>
				<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;font-weight:bold; line-height:35px;">
					<b><script>dw(wisp_detectsuccess);</script></b>
				</td>
			</tr>
		</table>

		<table border="0" align="center" style="height:102px;" id="tablesuccesswidth" class="tablewidth0">
			<tr>
				<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px;">
					<script>dw(wisp_detectsuccess_1);</script>
				</td>
			</tr>
		</table>
	</span>

	<span id="Connect_fail" style="display:none">
		<table border="0" align="center" style="height:35px;width:780px;">
			<tr>
				<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;font-weight:bold; line-height:35px;">
					<b><script>dw(wisp_detectfail);</script></b>
				</td>
			</tr>
		</table>

		<table border="0" align="center" style="height:102px;" id="tablefailwidth" class="tablewidth0">
			<tr>
				<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:35px;">
					<script>dw(wisp_detectfail_1);</script>
				</td>
			</tr>
		</table>
	</span>
		
	<table border="0" style="height:160px;width:780px" align="center">
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
