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

<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var connectIF = <%getVar("connectIF","","");%>;
var connect_distinguish = "<%getInfo("get_connect_distinguish");%>";
//var rep_enable5g = "<% getInfo("rep_enable5g"); %>";

function goBack()
{
	if (wizMode == 3){
		if(parseInt(connectIF, 10) == 2)
			document.ui_redirect.go2gSitesuvy.value="/wiz_APcl24gauto.asp";
		else
			document.ui_redirect.go5gSitesuvy.value="/wiz_APcl5gauto.asp";
	}
	else{
		if(parseInt(connectIF, 10) == 2)
			document.ui_redirect.go2gSitesuvy.value="/wiz_rep24gauto.asp";
		else
			document.ui_redirect.go5gSitesuvy.value="/wiz_rep5gauto.asp";
	}

	document.ui_redirect.submit();
}

function goNext()
{
//	if (wizMode == 2){
//		if(parseInt(connectIF, 10) == 2) {
//			if(rep_enable5g == "1")
//				window.location.assign("/wiz_rep5gauto.asp");
//			else
//				window.location.assign("/conclusion.asp");
//		}
//		else window.location.assign("/conclusion.asp");
//	}
//	else {
//		if(parseInt(connectIF, 10) == 2) window.location.assign("/wiz_ip.asp");
//		else window.location.assign("/wiz_ip5g.asp");
//	}
	if(parseInt(connectIF, 10) == 2) window.location.assign("/wiz_ip.asp");
	else window.location.assign("/wiz_ip5g.asp");
}

function setTableWidth()
{
	if(connect_distinguish == "1"){
		document.getElementById("link_info").innerHTML=showText(connection_succeeded_gonext);
		document.getElementById('show_next').style.display = "block";
		document.getElementById('show_back').style.display = "none";
	}
	else{
		document.getElementById("link_info").innerHTML=showText(connection_fail_goback);
		document.getElementById('show_next').style.display = "none";
		document.getElementById('show_back').style.display = "block";
	}

	languIndex=stype;
	if(stype>13) languIndex=13;

}


</script>

<style type="text/css">
.tablewidth0 {width:80%;}
</style>

</head>
<body onLoad="setTableWidth()">

<form action="/goform/wiz_5in1_redirect" method="POST" name="ui_redirect">
	<input type="hidden" value="" name="go5gSitesuvy">
	<input type="hidden" value="" name="go2gSitesuvy">
</form>

<form action="/goform/saveAndReboot" method="POST" name="tcpip">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">

<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
		<td align="right" class="Arial_16">
			<script>
				if(wizMode==0)
					dw(routermode);
				else if(wizMode==1)
					dw(apmode);
				else if(wizMode==2)
					dw(repeatermode);
				else if(wizMode==3)
					dw(APcl);
				else if(wizMode==4)
					dw(wisp);
				else
					dw(repeatermode);
			</script>
		</td>
		</tr>
	</table>

	<table border="0" style="height:90px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:100px;" id="tablewidth" class="tablewidth0" align="center">
		<tr>
			<td align="center" class="Arial_16" style="line-height:23px;" id="link_info">
			</td>
		</tr>
	</table>


	<table border="0" style="height:200px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<span id="show_next" style="display:none">
	<table border="0" style="height:30px;width:780px" align="center">
		<tr>
		<td align="right" style="width:390px">
		<script>
			document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
		</script>
		</td>
		<td align="left" style="width:390px">
		<script>
			document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close1" onClick="return goNext()" class="button_IQ2" style="cursor:pointer">');
		</script>
		</td>
		</tr>
	</table>
	</span>
	<span id="show_back" style="display:none">
	<table border="0" style="height:30px;width:780px" align="center">
		<tr>
		<td align="center" style="width:390px">
		<script>
			document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
		</script>
		</td>
		</tr>
	</table>
	</span>
	<table border="0" style="height:60px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>

</form>
</body>

<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>

</html>
