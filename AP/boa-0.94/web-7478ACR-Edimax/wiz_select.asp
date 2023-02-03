<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var wizmode_value = "<% getInfo("wizmode_value"); %>";

function goBack()
{
	document.check.OtherModeSelect.value="";
	document.check.back2iQmain.value="1";
	document.check.submiturl.value="/aIndex2.asp";
	document.check.submit();
}
function goNext()
{
	document.check.ip5g.value="192.168.9.2";
	document.check.ip.value="192.168.9.2";
	document.check.back2iQmain.value="";
	document.check.submiturl.value = parseInt(document.check.submiturl.value,10)

	if(document.check.OtherModeSelect.value == 1){

		document.check.submiturl.value="/wiz_apmode.asp";
	}
	else if(document.check.OtherModeSelect.value == 0){

		document.check.submiturl.value="/wiz_routerselect.asp";
	}
	else if(document.check.OtherModeSelect.value == 3){

		document.check.submiturl.value="/wiz_APcl.asp";
	}
	else if(document.check.OtherModeSelect.value == 4){

		document.check.submiturl.value="/wiz_WISP.asp";		
	}
	else{
		alert("error!");
	}

	document.check.submit();
}

function init()
{
	setTableWidth();
	document.check.OtherModeSelect.value=wizmode_value;
	
	if(wizmode_value=="1"){
		document.getElementById("apmode").style.backgroundColor="#2894FF";
		document.getElementById("S1").style.display="block";
		document.getElementById("Tab1").style.display="block";
		document.getElementById('Tab2').style.display="none";
		document.getElementById('Tab3').style.display="none";
		//document.getElementById('Tab4').style.display="none";

		document.getElementById('toptable').style.height="85px";
		document.getElementById('betw_ico_but').style.height="21px";
		document.getElementById('bottomtable').style.height="20px";
	}
	/*else if(wizmode_value=="0"){
		document.getElementById("routermode").style.backgroundColor="#2894FF";
		document.getElementById("S2").style.display="block";
		document.getElementById('Tab1').style.display="none";
		document.getElementById('Tab2').style.display="block";
		document.getElementById('Tab3').style.display="none";
		document.getElementById('Tab4').style.display="none";

		
		document.getElementById('toptable').style.height="85px";
		document.getElementById('betw_ico_but').style.height="21px";
		document.getElementById('bottomtable').style.height="20px";
	}
	else */if(wizmode_value=="3"){
		document.getElementById("APcl").style.backgroundColor="#2894FF";
		document.getElementById("S3").style.display="block";
		document.getElementById('Tab1').style.display="none";
		document.getElementById('Tab2').style.display="none";
		document.getElementById('Tab3').style.display="block";
		//document.getElementById('Tab4').style.display="none";
		
		
		document.getElementById('toptable').style.height="85px";
		document.getElementById('betw_ico_but').style.height="21px";
		document.getElementById('bottomtable').style.height="20px";
	}
	/*else if(wizmode_value=="4"){
		document.getElementById("wisp").style.backgroundColor="#2894FF";
		document.getElementById("S4").style.display="block";
		document.getElementById('Tab1').style.display="none";
		document.getElementById('Tab2').style.display="none";
		document.getElementById('Tab3').style.display="none";
		document.getElementById('Tab4').style.display="block";
		
		document.getElementById('toptable').style.height="85px";
		document.getElementById('betw_ico_but').style.height="21px";
		document.getElementById('bottomtable').style.height="20px";
	}*/
}

function loadTab(n){

	if (n == 1){
			document.getElementById("apmode").style.backgroundColor="#2894FF";
			//document.getElementById("routermode").style.backgroundColor="#a1a1a1";
			document.getElementById("APcl").style.backgroundColor="#a1a1a1";
			//document.getElementById("wisp").style.backgroundColor="#a1a1a1";
			document.getElementById("S1").style.display="block";
			document.getElementById("S2").style.display="none";
			document.getElementById("S3").style.display="none";
			document.getElementById("S4").style.display="none";
			document.getElementById('Tab1').style.display="block";
			document.getElementById('Tab2').style.display="none";
			document.getElementById('Tab3').style.display="none";
			//document.getElementById('Tab4').style.display="none";
			

			document.getElementById('toptable').style.height="85px";
			document.getElementById('betw_ico_but').style.height="21px";
			document.getElementById('bottomtable').style.height="20px";
			
		}
	else if(n == 0){
			document.getElementById("apmode").style.backgroundColor="#a1a1a1";
			//document.getElementById("routermode").style.backgroundColor="#2894FF";
			document.getElementById("APcl").style.backgroundColor="#a1a1a1";
			//document.getElementById("wisp").style.backgroundColor="#a1a1a1";
			document.getElementById("S1").style.display="none";
			document.getElementById("S2").style.display="block";
			document.getElementById("S3").style.display="none";
			document.getElementById("S4").style.display="none";
			document.getElementById('Tab1').style.display="none";
			document.getElementById('Tab2').style.display="block";
			document.getElementById('Tab3').style.display="none";
			//document.getElementById('Tab4').style.display="none";
			
			
			document.getElementById('toptable').style.height="85px";
			document.getElementById('betw_ico_but').style.height="21px";
			document.getElementById('bottomtable').style.height="20px";
		}
	else if(n == 3){
			document.getElementById("apmode").style.backgroundColor="#a1a1a1";
			//document.getElementById("routermode").style.backgroundColor="#a1a1a1";
			document.getElementById("APcl").style.backgroundColor="#2894FF";
			//document.getElementById("wisp").style.backgroundColor="#a1a1a1";
			document.getElementById("S1").style.display="none";
			document.getElementById("S2").style.display="none";
			document.getElementById("S3").style.display="block";
			document.getElementById("S4").style.display="none";
			document.getElementById('Tab1').style.display="none";
			document.getElementById('Tab2').style.display="none";
			document.getElementById('Tab3').style.display="block";
			//document.getElementById('Tab4').style.display="none";
			

			document.getElementById('toptable').style.height="85px";
			document.getElementById('betw_ico_but').style.height="21px";
			document.getElementById('bottomtable').style.height="20px";
		}
	else if(n == 4){
			document.getElementById("apmode").style.backgroundColor="#a1a1a1";
			//document.getElementById("routermode").style.backgroundColor="#a1a1a1";
			document.getElementById("APcl").style.backgroundColor="#a1a1a1";
			//document.getElementById("wisp").style.backgroundColor="#2894FF";
			document.getElementById("S1").style.display="none";
			document.getElementById("S2").style.display="none";
			document.getElementById("S3").style.display="none";
			document.getElementById("S4").style.display="block";
			document.getElementById('Tab1').style.display="none";
			document.getElementById('Tab2').style.display="none";
			document.getElementById('Tab3').style.display="none";
			//document.getElementById('Tab4').style.display="block";
			
			document.getElementById('toptable').style.height="85px";
			document.getElementById('betw_ico_but').style.height="21px";
			document.getElementById('bottomtable').style.height="20px";
		}

	document.check.OtherModeSelect.value=n;
}

function setTableWidth()
{
	if(stype <= 12){
		document.getElementById('tablewidth1').className="tablewidth90";
		document.getElementById('tablewidth2').className="tablewidth90";
		document.getElementById('tablewidth3').className="tablewidth90";
		document.getElementById('tablewidth4').className="tablewidth90";
	}
	else{
		document.getElementById('tablewidth1').className="tablewidth80";
		document.getElementById('tablewidth2').className="tablewidth80";
		document.getElementById('tablewidth3').className="tablewidth80";
		document.getElementById('tablewidth4').className="tablewidth80";
	}
	
	if ((stype == 0)){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('routermode').style.width="180px";
		//document.getElementById('wisp').style.width="180px";
		document.getElementById('APcl').style.width="180px";
	}
	if ((stype == 2)){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('wisp').style.width="100px";
	}
	if ((stype == 3) || (stype == 5)){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('routermode').style.width="180px";
		//document.getElementById('wisp').style.width="120px";
	}

	if ((stype == 10)){
		document.getElementById('apmode').style.width="190px";
		//document.getElementById('routermode').style.width="190px";
		document.getElementById('APcl').style.width="220px";
		//document.getElementById('wisp').style.width="100px";
	}
	if(stype == 6){
		document.getElementById('apmode').style.width="200px";
		//document.getElementById('wisp').style.width="100px";
	}
	if(stype == 7){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('routermode').style.width="180px";
	}
	if(stype == 12){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('routermode').style.width="180px";
	}
	if((stype == 13) || (stype == 14)){
		document.getElementById('apmode').style.width="180px";
		//document.getElementById('routermode').style.width="180px";
		//document.getElementById('wisp').style.width="150px";
	}
	if (stype == 8) {
		document.getElementById('apmode').style.width="200px";
		//document.getElementById('routermode').style.width="200px";
		document.getElementById('APcl').style.width="220px";
		//document.getElementById('wisp').style.width="80px";
	}

	if((stype == 4) || (stype == 11)){
		document.getElementById('apmode').style.width="160px";
		//document.getElementById('routermode').style.width="290px";
		//document.getElementById('wisp').style.width="80px";
	}

}
</script>
<style type="text/css">
.tablewidth90 {width:90%;}
.tablewidth80 {width:80%;}



</style>
</head>
<body onload="init()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="check">
<input type="hidden" value="" name="ip">
<input type="hidden" value="" name="ip5g">
<input type="hidden" value="" name="OtherModeSelect">
<input type="hidden" value="" name="submiturl">
<input type="hidden" value="" name="back2iQmain">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">


	<table border="0" id="toptable" align="center" style="height:90px;width:750px">
		<tr><td></td></tr>
	</table>

		<table border="0" align="center" style="height:0px;width:730px">
			<tr>
				<td align="left" class="Arial_16">
					<script>
						document.write('<input type="button" id="apmode" value="'+showText(apmode)+'" name="apmode" onClick=\"loadTab(1);\" class="button_ap">&nbsp;');
						document.write('<input type="button" id="APcl" value="'+showText(APcl)+'" name="APcl" onClick=\"loadTab(3);\" class="button_apcl">&nbsp;');
						//document.write('<input type="button" id="routermode" value="'+showText(routermode)+'" name="routermode" onClick=\"loadTab(0);\" class="button_rep">&nbsp;');
						//document.write('<input type="button" id="wisp" value="'+showText(wisp)+'" name="wisp" onClick=\"loadTab(4);\" class="button_wisp">');
					</script>				
				</td>
			</tr>
		</table>

	<span id="S1" style="display:none;">
		<table border="0" style="height:100px;" id="tablewidth1" class="tablewidth0" align="center">
			<tr>
				<td align="left" class="Arial_16" style="height:20px;">
					<script>
						document.write('<b>'+showText(S1_head)+'</b>');
					</script>				
				</td>
			</tr>
			<tr>
				<td align="left" class="Arial_16" style="height:80px;line-height:21px;">
					<script>
						document.write(''+showText(S1_info)+'');
					</script>				
				</td>
			</tr>			
		</table>
	</span>

	<span id="S2" style="display:none;">
		<table border="0" style="height:100px;" id="tablewidth2" class="tablewidth0" align="center">
			<tr>
				<td align="left" class="Arial_16" style="height:20px;">
					<script>
						document.write('<b>'+showText(YouSelectRouterMode)+'</b>');
					</script>				
				</td>
			</tr>
			<tr>
				<td align="left" class="Arial_16" style="height:80px;line-height:21px;">
					<script>
						document.write(''+showText(ConnectViaCable)+'');
					</script>				
				</td>
			</tr>			
		</table>
	</span>

	<span id="S3" style="display:none;">
		<table border="0" style="height:100px;" id="tablewidth3" class="tablewidth0" align="center">
			<tr>
				<td align="left" class="Arial_16" style="height:20px;">
					<script>
						document.write('<b>'+showText(S3_head)+'</b>');
					</script>				
				</td>
			</tr>
			<tr>
				<td align="left" class="Arial_16" style="height:80px;line-height:21px;">
					<script>
						document.write(''+showText(S3_info)+'');
					</script>				
				</td>
			</tr>			
		</table>
	</span>

	<span id="S4" style="display:none;">
		<table border="0" style="height:100px;" id="tablewidth4" class="tablewidth0" align="center">
			<tr>
				<td align="left" class="Arial_16" style="height:20px;">
					<script>
						document.write('<b>'+showText(S4_head)+'</b>');
					</script>				
				</td>
			</tr>
			<tr>
				<td align="left" class="Arial_16" style="height:80px;line-height:21px;">
					<script>
						document.write(''+showText(S4_info)+'');
					</script>				
				</td>
			</tr>			
		</table>
	</span>

	<span id="Tab1" style="display:none">
		<table border="0" style="height:235px;width:600px" align="center">
			<tr>
				<td align="center" class="Arial_16"><br> 
					<img src="graphics/AP-mode_gif.gif" STYLE="cursor:pointer" width="600" height="235" border="0">
				</td>
			</tr>
		</table>
	</span>

	<span id="Tab2" style="display:none">
		<table border="0" style="height:235px;width:600px" align="center">
			<tr>
				<td align="center" class="Arial_16"><br> 
					<img src="graphics/WiFi_router.gif" STYLE="cursor:pointer" width="600" height="235" border="0">
				</td>
			</tr>
		</table>
	</span>

	<span id="Tab3" style="display:none">
		<table border="0" style="height:235px;width:600px" align="center">
			<tr>
				<td align="center" class="Arial_16"><br> 
					<img src="graphics/Wi-Fi-Bridge.gif" STYLE="cursor:pointer" width="600" height="235" border="0">
				</td>
			</tr>
		</table>
	</span>

	<!--<span id="Tab4" style="display:none">
		<table border="0" style="height:235px;width:600px" align="center">
			<tr>
				<td align="center" class="Arial_16"><br> 
					<img src="graphics/WISP.gif" STYLE="cursor:pointer" width="600" height="235" border="0">
				</td>
			</tr>
		</table>
		<table border="0" style="height:15px;width:794px" align="center">
			<tr><td></td></tr>
		</table>
	</span>-->

	<table border="0" id="betw_ico_but" style="height:21px;width:760px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:28px;width:600px" align="center">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
		
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>

	<table border="0" id="bottomtable" style="height:21px;width:760px" align="center">
		<tr><td></td></tr>
	</table>

</td>
</tr>
</table>
</div>
</form>
</body>
</html>
