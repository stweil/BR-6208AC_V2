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
var rep_enable5g = "<%getInfo("rep_enable5g");%>";
var rep_enable2g = "<%getInfo("rep_enable2g");%>";
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;

function init()
{

    if(rep_enable2g=="1"){
	    document.repeater_mode.enableBand[1].checked = false;
	    document.repeater_mode.enableBand[0].checked = true;
    }
    else{
	    document.repeater_mode.enableBand[1].checked = true;
	    document.repeater_mode.enableBand[0].checked = false;
    }
}

function goBack()
{
	window.location.assign("/wiz_repeater.asp");
}
function goNext()
{
	// 5G
	if (document.repeater_mode.enableBand[1].checked == true)
		document.repeater_mode.rep_enable5g.value = "1" ;
	else
		document.repeater_mode.rep_enable5g.value = "0" ;
	
	// 2.4G
	if (document.repeater_mode.enableBand[0].checked == true)
		document.repeater_mode.rep_enable2g.value = "1" ;
	else
		document.repeater_mode.rep_enable2g.value = "0" ;

	/*if( (document.repeater_mode.enable5g.checked == false) && (document.repeater_mode.enable2g.checked == false)){
	alert(showText(ap_alert));
	return false;
	}*/

//	document.repeater_mode.submiturl.value = "/wiz_crossband.asp";	
	document.repeater_mode.submiturl.value = "/wiz_sitesurvey_redirect.asp";	
	document.repeater_mode.submit(); 
}

function setTableWidth()
{
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
<body onload="setTableWidth; init()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="repeater_mode">
<input type="hidden" value="" name="rep_enable5g">
<input type="hidden" value="" name="rep_enable2g">
<input type="hidden" value="yes" name="rep_select">
<input type="hidden" value="" name="submiturl">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">

<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
	<tr>
		<td align="center">

			<table border="0" style="height:90px;width:700px;" align="center">
				<tr>
					<td align="right" class="Arial_16">
						<script>dw(repeatermode)</script>
					</td>
				</tr>
			</table>

			<table border="0" style="height:100px;" id="tablewidth" class="tablewidth0" align="center">
				<tr>
					<td align="center" class="Arial_16"><script>dw(wiz_repselect)</script></td>
				</tr>
			</table>

			
			<table border="0" align="center" style="height:70px;width:780px">
				<tr>
					<td align="right" width="40%"><input type="radio" name="enableBand" value=""></td>
					<td align="left" width="60%" class="Arial_16">&nbsp;<script>dw(wizard_repeater_mode2)</script></td>
				</tr>
		
				<tr>
					<td align="right" width="40%"><input type="radio" name="enableBand" value=""></td>
					<td align="left" width="60%" class="Arial_16">&nbsp;<script>dw(wizard_repeater_mode1)</script></td>
				</tr>
			</table>
	

			<table border="0" style="height:240px;width:780px" align="center">
				<tr><td></td></tr>
			</table>

			<table border="0" style="height:30px;width:780px" align="center">
				<tr>
					<td align="right" style="width:390px">
						<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
					</td>
			
					<td align="left" style="width:390px">
						<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
					</td>
				</tr>
			</table>

			<table border="0" style="height:40px;width:780px" align="center">
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
