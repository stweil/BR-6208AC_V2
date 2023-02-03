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
var pingStatus = "<%getInfo("pingStatus");%>";
var conType = "<%getInfo("conType");%>";
var wanStatus = "<%getInfo("wanStatus");%>";


function url()
{
	
	if(pingStatus == "1"){
		document.applyform.RedirectURL.value="/msuccess.asp"; //window.location.assign("/msuccess.asp");
	}
	else{
		if(conType == "1"){
			if(wanStatus == "6")
				document.applyform.RedirectURL.value="/adhcp_and_pppoe.asp"; //window.location.assign("/adhcp_and_pppoe.asp");
			else
				document.applyform.RedirectURL.value="/apppoe.asp"; //window.location.assign("/apppoe.asp");
		}
		else{
			document.applyform.RedirectURL.value="/mfail.asp"; //window.location.assign("/mfail.asp");
		}
	}
	document.applyform.submit();
}
</script>
<body onload="url()">
	<form action="/goform/saveAndReboot" method="POST" name="applyform">
		<input type="hidden" value="" name="RedirectURL">
	</form>
</body>
</html>
