<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/autowan.var"></script>
<script type="text/javascript">
var wizmode = <%getVar("wizmode","","");%>;
var ssid2scan_fail = <%getVar("ssid2scan_fail","","");%>;
var ssid2scan_fail5g = <%getVar("ssid2scan_fail5g","","");%>;
</script>
</head>
<body>
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr><td align="right" class="Arial_16"><script>dw(wisp);</script></td></tr>
	</table>
	
	<table border="0" style="height:30px;width:700px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="width:790px" align="center">
		<tr><td align="center"><span class="Arial_20" style="font-weight:bold;"><script>dw(ConnectionTest24);</script></td></tr>
		<tr><td align="center" id="in" class="Arial_20" style="font-weight:bold;">1%</td></tr>
		<tr><td align="center"><img src="graphics/loading.gif"></td></tr>
	</table>

	<table border="0" style="height:270px;width:700px" align="center">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>
<script>
var secs = 30;

var progressInterval = 3;
if(ssid2scan_fail || ssid2scan_fail5g ){
	secs = 10;	
	progressInterval = 10;
}

var wait = secs * 1000;

document.getElementById("in").innerHTML="1%";

for(i = 1; i <= secs; i++) setTimeout("update(" + i + ")", i * 1000);

setTimeout("timer()", wait);

function update(num)
{
	if (num == (wait/1000)) document.getElementById("in").innerHTML="100%";
	else
	{
		document.getElementById("in").innerHTML= num*progressInterval+"%";
	}
}

function timer()
{
	window.location.replace("/wispRedirect.asp");
}
</script>
</body>
</html>
