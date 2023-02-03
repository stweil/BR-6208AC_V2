<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/javascript.js"></script>
<!--<link rel="stylesheet" href="/set.css">-->
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">

<title>EDIMAX Technology</title>
</head>

<script>
var ipaddr="<%getInfo("ip-rom");%>";
var x = /\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b/
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;


if(iqsetup_finish == 0){
	/*if( (x.test(location.hostname)) && (location.hostname == ipaddr) ) //if user use DUT IP login DUT, pass iqsetup
		window.location.replace("/index.asp");
	else
		window.location.replace("/aIndex.asp");
	*/
	window.location.replace("http://edimaxext.setup/aIndex.asp");
}
else{
	window.location.replace("/index.asp");
}
</script>
<body bgcolor="#FFFFFF" text="#000000" >
</body>
</html>
