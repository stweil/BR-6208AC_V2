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
var wanStatus = "<%getInfo("wanStatus");%>";

function url()
{
	if((wanStatus == "1") || (wanStatus == "3"))
		document.applyform.RedirectURL.value="/aconnected.asp"; //window.location.assign("/aconnected.asp");
	else if(wanStatus == "2")
		document.applyform.RedirectURL.value="/adhcp_fail.asp"; //window.location.assign("/adhcp_fail.asp");
	else if(wanStatus == "4")
		document.applyform.RedirectURL.value="/apppoe.asp"; //window.location.assign("/apppoe.asp");
	else if(wanStatus == "6")
		document.applyform.RedirectURL.value="/adhcp_and_pppoe.asp"; //window.location.assign("/adhcp_and_pppoe.asp");
	else
		document.applyform.RedirectURL.value="/mstart.asp"; //window.location.assign("/mstart.asp");

	document.applyform.submit();
}
</script>
<body onLoad="url()">
<form action="/goform/saveAndReboot" method="POST" name="applyform">
<input type="hidden" value="" name="RedirectURL">
</form>
</body>
</html>
