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
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript">
var wizMode = <%getVar("wizmode","","");%>;
var wiz_gwManu = "<% getInfo("wiz_gwManu"); %>";
var wiz_wispManu = "<% getInfo("wiz_wispManu"); %>";
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>";
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>";
var pppFail = "<% getInfo("pppFail"); %>";


function submit_now()
{
	$.ajax({
	  url: 'goform/setWAN',
	  type: 'POST',
	  data: $('#tcpip').serialize(),
	  async: true,
	  dataType:'text',

	  success: function () {
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  },
	  error: function(){
			if (wiz_gwManu==1 || wiz_wispManu==1)
				window.location.assign("/wifi.asp");
			else
				window.location.assign("/probe.asp");
	  }
	});
}


function check(input){

	var x = input

	if (x.value != ""){
		if (x.value.search("\'") != -1 || x.value.search("\"") != -1){
			alert("Invalid value")
			x.value = x.defaultValue
			x.focus()
			return false
		}
		else
			return true
	}
	else
		return true
}

function goBack(){

	if (wizMode==4){
		if (WISP_enable5g==1)
			window.location.assign("wiz_WISP5gauto.asp");
		else		
			window.location.assign("wiz_WISP24gauto.asp");
	}
	else{
		window.location.assign("hwsetup.asp");
	}
}

function goNext(){
	if (!strRule(document.tcpip.pppUserName,showText(username))) return false;
	if (!strRule(document.tcpip.pppPassword,showText(password))) return false;

	document.tcpip.wiz_gwManu.value = wiz_gwManu;
	document.tcpip.wiz_wispManu.value = wiz_wispManu;

	submit_now();
	//document.tcpip.submit();
}

function setTableWidth()
{
	if(pppFail == "1")
		document.getElementById('checkppp').innerHTML = showText(checkNamePassword);

	if(stype>13) stype=13;

	document.all.tablewidth.className="tablewidth"+stype;
	document.all.tablewidth1.className="tablewidth-"+stype;
	document.all.leftwidth.className="leftwidth"+stype;
	document.all.rightwidth.className="rightwidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:60%;}
.tablewidth1 {width:68%;}
.tablewidth2 {width:62%;}
.tablewidth3 {width:66%;}
.tablewidth4 {width:54%;}
.tablewidth5 {width:72%;}
.tablewidth6 {width:84%;}
.tablewidth7 {width:54%;}
.tablewidth8 {width:60%;}
.tablewidth9 {width:68%;}
.tablewidth10 {width:78%;}
.tablewidth11 {width:74%;}
.tablewidth12 {width:72%;}
.tablewidth13 {width:52%;}

.tablewidth-0 {width:39%;}
.tablewidth-1 {width:39%;}
.tablewidth-2 {width:50%;}
.tablewidth-3 {width:46%;}
.tablewidth-4 {width:43%;}
.tablewidth-5 {width:45%;}
.tablewidth-6 {width:50%;}
.tablewidth-7 {width:50%;}
.tablewidth-8 {width:55%;}
.tablewidth-9 {width:53%;}
.tablewidth-10 {width:51%;}
.tablewidth-11 {width:35%;}
.tablewidth-12 {width:35%;}
.tablewidth-13 {width:38%;}

.leftwidth0 {width:30%;}
.leftwidth1 {width:40%;}
.leftwidth2 {width:40%;}
.leftwidth3 {width:40%;}
.leftwidth4 {width:35%;}
.leftwidth5 {width:35%;}
.leftwidth6 {width:38%;}
.leftwidth7 {width:39%;}
.leftwidth8 {width:38%;}
.leftwidth9 {width:40%;}
.leftwidth10 {width:40%;}
.leftwidth11 {width:40%;}
.leftwidth12 {width:40%;}
.leftwidth13 {width:28%;}

.rightwidth0 {width:70%;}
.rightwidth1 {width:60%;}
.rightwidth2 {width:60%;}
.rightwidth3 {width:60%;}
.rightwidth4 {width:65%;}
.rightwidth5 {width:65%;}
.rightwidth6 {width:62%;}
.rightwidth7 {width:61%;}
.rightwidth8 {width:62%;}
.rightwidth9 {width:60%;}
.rightwidth10 {width:60%;}
.rightwidth11 {width:60%;}
.rightwidth12 {width:72%;}
.rightwidth13 {width:72%;}

</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/setWAN" method="POST" name="tcpip" id="tcpip">
<input type="hidden" value="" name="wiz_gwManu">
<input type="hidden" value="" name="wiz_wispManu">
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

	<table border="0" style="height:85px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	
	<table border="0" style="height:75px;" id="tablewidth" class="tablewidth0" align="center">
		<tr><td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px; line-height:35px;"><script>dw(ISPtip)</script></td>
		</tr>
	</table>
	
	<table border="0" style="height:80px;" id="tablewidth1" class="tablewidth10" align="center">
		<tr>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" id="leftwidth" class="leftwidth0">
				<script>dw(username)</script>&nbsp;:
			</td>
			
			<td align="left" id="rightwidth" class="rightwidth0">
				<input type="text" class="Arial_16" name="pppUserName" size="20" maxlength="64" value="" onchange="check(this)">
			</td>
		</tr>
	
		<tr>
			<td align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;">
				<script>dw(password)</script>&nbsp;:
			</td>

			<td align="left">
				<input type="text" class="Arial_16" name="pppPassword" size="20" maxlength="64" value="" onchange="check(this)">
			</td>
		</tr>
	</table>
	
	<table border="0" style="height:30px;width:780px" align="center">
		<tr>
			<td id="checkppp" align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;color:#FF0000">
			</td>
		</tr>
	</table>

	<table border="0" style="height:120px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:28px;width:780px" align="center">
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
<input type="hidden" value="/apppoe.asp" name="submit-url">
<input type="hidden" value="" name="DNSMode"/>
<input type="hidden" value="2" name="wanMode" >
<input type="hidden" value="1" name="conType" >
</form>
</body>
</html>
