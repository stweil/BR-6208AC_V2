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
var rep_enable5g = "<% getInfo("rep_enable5g"); %>";
var rep_enable2g = "<% getInfo("rep_enable2g"); %>";

function url()
{
/*
	if( (rep_enable5g == "1") && (rep_enable2g == "1") ) //5g and 24g enable
		window.location.assign("/wiz_rep5gauto.asp");
	else if( (rep_enable5g == "1") && (rep_enable2g == "0") ) //5g enable
		window.location.assign("/wiz_rep5gauto.asp");
	else
		window.location.assign("/wiz_rep24gauto.asp"); //24g enable
*/
	if( (rep_enable5g == "1") && (rep_enable2g == "1") ) //5g and 24g enable
		window.location.assign("/wiz_rep24gauto.asp");
	else if( (rep_enable5g == "1") && (rep_enable2g == "0") ) //5g enable
		window.location.assign("/wiz_rep5gauto.asp");
	else
		window.location.assign("/wiz_rep24gauto.asp"); //24g enable
}
</script>
<body onLoad="url()">
</body>
</html>
