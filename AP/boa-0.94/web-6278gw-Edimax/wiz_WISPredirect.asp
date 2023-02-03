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
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>"
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>"

function url()
{
	if((WISP_enable5g == "1") && (WISP_enable2g == "0"))
		window.location.assign("/wiz_WISP5gauto.asp");//5g enable
	else
		window.location.assign("/wiz_WISP24gauto.asp");//24g enable
}
</script>
<body onLoad="url()">
</body>
</html>
