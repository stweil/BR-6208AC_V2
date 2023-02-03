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
var connect_distinguish = "<%getInfo("get_connect_distinguish");%>";
var connectkeyiswep = <%getVar("connect_key_is_wep","","");%>;
var connectIF = <%getVar("connectIF","","");%>;
var apclii0IP="<% getInfo("apclii0IP"); %>";
var apcli0IP="<% getInfo("apcli0IP"); %>";
var connectOK="1";
var getIP=""

if(connectIF == "2")
	getIP=apcli0IP;
else
	getIP=apclii0IP;

function url()
{
	console.log(connectkeyiswep);
	console.log(getIP);

	if((parseInt(connectkeyiswep, 10) == 1) && getIP == ""){ /* WEP and Get IP Fail */
		console.log("go ip page");
		if(connectIF == "2") window.location.assign("/wiz_ip.asp");
		else window.location.assign("/wiz_ip5g.asp");
	}
	else{
		console.log("go connect Status page");
		window.location.assign("/connectStatus.asp");
	}

	//document.applyform.submit();
}
</script>
<body onLoad="url()">	
<form action="/goform/saveAndReboot" method="POST" name="applyform">
<input type="hidden" value="" name="RedirectURL">
</form>
</body>
</html>
