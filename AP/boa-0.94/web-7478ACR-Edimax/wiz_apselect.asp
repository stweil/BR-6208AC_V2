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
var Ap_enable5g = "<%getInfo("Ap_enable5g");%>";
var Ap_enable2g = "<%getInfo("Ap_enable2g");%>";
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;

function init()
{	
	/*if (iqsetup_finish != 0){
		if((Ap_enable5g==1) && (Ap_enable2g==1)){
			document.AP_mode.enable5g.checked = true;
			document.AP_mode.enable2g.checked = true;
		}
		else if((Ap_enable5g==1) && (Ap_enable2g==0)){
			document.AP_mode.enable5g.checked = true;
			document.AP_mode.enable2g.checked = false;
		}
		else if((Ap_enable5g==0) && (Ap_enable2g==1)){
			document.AP_mode.enable5g.checked = false;
			document.AP_mode.enable2g.checked = true;
		}
	}
	else{*/
		document.AP_mode.enable5g.checked = true;
		document.AP_mode.enable2g.checked = true;
	//}
}

function goBack()
{
	window.location.assign("/wiz_apmode.asp");
}
function goNext()
{
	// 5G
	if (document.AP_mode.enable5g.checked == true)
		document.AP_mode.Ap_enable5g.value = "1";
	else
		document.AP_mode.Ap_enable5g.value = "0";

	/***************************************************************/	
	// 2.4G
	if (document.AP_mode.enable2g.checked == true)
		document.AP_mode.Ap_enable2g.value = "1";
	else
		document.AP_mode.Ap_enable2g.value = "0";

	if( (document.AP_mode.enable2g.checked == false) && (document.AP_mode.enable5g.checked == false) ){
			alert(showText(ap_alert));
			return false;
	}
	document.AP_mode.submiturl.value = "/wiz_ip.asp";
	document.AP_mode.submit(); 
}

function setTableWidth()
{
	languIndex=stype;
	if(stype>13) languIndex=13;
	//document.all.tablewidth.className="tablewidth"+languIndex;
	document.getElementById("tablewidth").className="tablewidth"+languIndex;
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
.tablewidth13 {width:70%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="AP_mode">
<input type="hidden" value="" name="Ap_enable5g">
<input type="hidden" value="" name="Ap_enable2g">
<input type="hidden" value="" name="submiturl">

<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">

<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
	<tr>
		<td align="center">

			<table border="0" style="height:90px;width:700px" align="center">
				<tr>
					<td align="right" class="Arial_16">
						<script>dw(apmode)</script>
					</td>
				</tr>
			</table>

			<table border="0" style="height:100px;" id="tablewidth" class="tablewidth0" align="center">
				<tr>
					<td align="left" class="Arial_16">
						<script>dw(ap_select1)</script>
						<script>dw(ap_select2)</script>
					</td>
				</tr>
			</table>

			<table border="0" style="height:10px;width:700px" align="center">
				<tr><td></td></tr>
			</table>


			<table border="0" align="center" style="height:70px;width:780px">
			
				<tr>
					<td align="right" style="width:312px"><input type="checkbox" name="enable2g"></td>
					<td align="left" style="width:468px">&nbsp;<script>dw(wizard_repeater_mode2)</script></td>
				</tr>

				<tr>
					<td align="right" style="width:312px"><input type="checkbox" name="enable5g"></td>
					<td align="left" style="width:468px">&nbsp;<script>dw(wizard_repeater_mode1)</script></td>
				</tr>

			</table>
	

			<table border="0" style="height:210px;width:780px" align="center">
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
<script>
init();
</script>
