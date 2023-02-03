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
</head>
<script>
var cableStatus = "<%getInfo("cableStatus");%>";

function url()
{
	if(cableStatus == "disconnect")
		window.location.assign("/wiz_ap_cable.asp");
	else
		window.location.assign("/wiz_apselect.asp");
}
</script>
<body onLoad="url()">
</body>
</html>
