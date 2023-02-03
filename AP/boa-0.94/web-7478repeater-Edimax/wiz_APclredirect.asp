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
var APcl_enable5g = "<%getInfo("APcl_enable5g");%>";
var APcl_enable2g = "<%getInfo("APcl_enable2g");%>";
function url()
{
	if(APcl_enable5g == "1")
		window.location.assign("/wiz_APcl5gauto.asp");
	else
		window.location.assign("/wiz_APcl24gauto.asp");
}
</script>
<body onLoad="url()">
</body>
</html>
