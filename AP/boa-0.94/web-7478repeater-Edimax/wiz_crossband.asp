<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript">
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var wizMode = <%getVar("wizmode","","");%>;
var rep_enable5g = "<%getInfo("rep_enable5g");%>";
var rep_enable2g = "<%getInfo("rep_enable2g");%>";
var rep_EnableCrossBand = "<%getInfo("rep_enableCrossBand");%>";

function goBack()
{
	window.location.assign("wiz_repselect.asp");
}
function goNext()
{
	with(document.crossBand) {

		//if (rep_enable2g == "1" && rep_enable5g == "1"){
			if (EnableCross.checked == true)
				EnableCrossBand.value="1"
			else
				EnableCrossBand.value="0"
		/*}
		else{
			EnableCrossBand.value="1"
		}*/

		submit();
	}
}

function setTableWidth()
{
	//if (rep_enable2g == "1" && rep_enable5g == "1"){
		if(rep_EnableCrossBand == "-1")
			document.crossBand.EnableCross.checked = false;
		else{
			if(rep_EnableCrossBand == "1")
				document.crossBand.EnableCross.checked = true;
			else
				document.crossBand.EnableCross.checked = false;
		}
	//}
	
	languIndex=stype;
	if(stype>13) languIndex=13;
	document.getElementById('tablewidth').className="tablewidth"+languIndex;
}
</script>
<style type="text/css">
.tablewidth0 {width:80%;}
.tablewidth1 {width:80%;}
.tablewidth2 {width:80%;}
.tablewidth3 {width:80%;}
.tablewidth4 {width:80%;}
.tablewidth5 {width:80%;}
.tablewidth6 {width:80%;}
.tablewidth7 {width:80%;}
.tablewidth8 {width:80%;}
.tablewidth9 {width:80%;}
.tablewidth10 {width:80%;}
.tablewidth11 {width:80%;}
.tablewidth12 {width:80%;}
.tablewidth13 {width:76%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="crossBand">
<input type="hidden" value="" name="EnableCrossBand" id="EnableCrossBand">
<input type="hidden" value="/wiz_sitesurvey_redirect.asp" id="submiturl" name="submiturl">

<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center">
		<tr>
			<td align="right" class="Arial_16">
				<script>dw(repeatermode)</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:240px;width:780px" align="center">
	<tr>
	<td align="center">
	<script>
	if (rep_enable2g == "1" && rep_enable5g == "1")
		document.write('<img src="graphics/Cross-Band.gif" STYLE="cursor:pointer" width="567" height="235" border="0">');
	else if (rep_enable2g == "1" && rep_enable5g == "0")
		document.write('<img src="graphics/Cross_Band_2_4G_only.png" STYLE="cursor:pointer" width="601" height="236" border="0">');
	else
		document.write('<img src="graphics/Cross_Band_5G_only.png" STYLE="cursor:pointer" width="600" height="235" border="0">');
	</script>
	</td>
	</tr>
	</table>

	
	<table border="0" style="height:100px;width:700px" id="tablewidth" class="tablewidth0" align="center" >
		<tr>
			<td align="left"><span class="Arial_16" style="line-height:25px;">
				<script>
					if (rep_enable2g == "1" && rep_enable5g == "1")
						dw(dualCross)
					else
						dw(SingleCross)
				</script>
			</span></td>
		</tr>

	</table>
	
	<table border="0" style="height:60px;width:780px" align="center">
		<tr>
			<script>
				//if (rep_enable2g == "1" && rep_enable5g == "1"){
					document.write('<td align="right" width="40%"><input type="checkbox" name="EnableCross"></td>');
					document.write('<td align="left" width="60%" class="Arial_16">'+showText(EnableCrossBand)+'</td>');
				//}
			</script>
		</tr>
	</table>

	<table border="0" style="height:70px;width:780px" align="center">
	<tr>
	<script>
			document.write('<td align="right" style="width:390px">');
			document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
			document.write('</td>');
			document.write('<td align="left" style="width:390px">');
			document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');
			document.write('</td>');

		</script>
	</tr>
	</table>

	<table border="0" style="height:30px;width:780px" align="center">
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
