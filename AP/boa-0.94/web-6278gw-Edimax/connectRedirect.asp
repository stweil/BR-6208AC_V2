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
<script>
var ConnectingStatus = "<%getInfo("Connecting");%>";

function url()
{
	if(ConnectingStatus == "2")
		window.location.assign("/wiz_ip.asp");
	else if(ConnectingStatus == "5")
		window.location.assign("/wiz_ip5g.asp");
	else
		alert("error");
}
</script>
</head>
<body onLoad="url()">

</body>
</html>
