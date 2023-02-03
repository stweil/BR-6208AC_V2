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
function goBack()
{
window.location.assign("hwsetup.asp");
}
function goNext()
{
if(document.getElementsByName("startconfig")[0].checked == true)
document.check.submit();
else if(document.getElementsByName("startconfig")[1].checked == true)
window.location.assign("mstart.asp");
}

function setTableWidth()
{
	if(stype>13) stype=13;
	document.getElementById('ManuallySeleTable').className="tablewidth"+stype;

	document.getElementById('linkunset').className="linkunset"+stype;
	document.getElementById('nextBelow').className="nextBelow"+stype;

	document.getElementById('leftwidth').className="leftwidth"+stype;
	document.getElementById('mwidth').className="mwidth"+stype;
	document.getElementById('rightwidth').className="rightwidth"+stype;
}

function init(){
	document.getElementsByName("startconfig")[0].checked = true;
	setTableWidth();
}
</script>
<style type="text/css">

.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:90%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:90%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:90%;}

.linkunset0 {width:90%;}
.linkunset1 {width:90%;}
.linkunset2 {width:90%;}
.linkunset3 {width:90%;}
.linkunset4 {width:90%;}
.linkunset5 {width:90%;}
.linkunset6 {width:90%;}
.linkunset7 {width:90%;}
.linkunset8 {width:90%;}
.linkunset9 {width:90%;}
.linkunset10 {width:90%;}
.linkunset11 {width:90%;}
.linkunset12 {width:90%;}
.linkunset13 {width:90%;}

.nextBelow0 {width:90%;}
.nextBelow1 {width:90%;}
.nextBelow2 {width:90%;}
.nextBelow3 {width:90%;}
.nextBelow4 {width:90%;}
.nextBelow5 {width:90%;}
.nextBelow6 {width:90%;}
.nextBelow7 {width:90%;}
.nextBelow8 {width:90%;}
.nextBelow9 {width:90%;}
.nextBelow10 {width:90%;}
.nextBelow11 {width:90%;}
.nextBelow12 {width:90%;}
.nextBelow13 {width:90%;}

.leftwidth0 {width:22%;}
.leftwidth1 {width:18%;}
.leftwidth2 {width:18%;}
.leftwidth3 {width:18%;}
.leftwidth4 {width:14%;}
.leftwidth5 {width:21%;}
.leftwidth6 {width:21%;}
.leftwidth7 {width:22%;}
.leftwidth8 {width:22%;}
.leftwidth9 {width:22%;}
.leftwidth10 {width:17%;}
.leftwidth11 {width:26%;}
.leftwidth12 {width:18%;}
.leftwidth13 {width:32%;}

.mwidth0 {width:27%;}
.mwidth1 {width:33%;}
.mwidth2 {width:33%;}
.mwidth3 {width:31%;}
.mwidth4 {width:39%;}
.mwidth5 {width:30%;}
.mwidth6 {width:30%;}
.mwidth7 {width:27%;}
.mwidth8 {width:27%;}
.mwidth9 {width:27%;}
.mwidth10 {width:40%;}
.mwidth11 {width:25%;}
.mwidth12 {width:35%;}
.mwidth13 {width:15%;}

.rightwidth0 {width:76%;}
.rightwidth1 {width:66%;}
.rightwidth2 {width:66%;}
.rightwidth3 {width:76%;}
.rightwidth4 {width:86%;}
.rightwidth5 {width:70%;}
.rightwidth6 {width:54%;}
.rightwidth7 {width:76%;}
.rightwidth8 {width:76%;}
.rightwidth9 {width:76%;}
.rightwidth10 {width:56%;}
.rightwidth11 {width:55%;}
.rightwidth12 {width:55%;}
.rightwidth13 {width:55%;}

</style>
</head>
<body onLoad="init()">
<form action="/goform/chkLink" method="POST" name="check">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" align="center" style="height:600px;width:800px" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" align="center" style="height:90px;width:700px">
		<tr>
			<td align="right">
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

	<table border="0" align="center" width="567" height="150">
		<tr>
			<td align="center">
				<img src="graphics/no_connect.png" width="567" height="123">
			</td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:50px;" id="linkunset" class="linkunset0">
		<tr>
			<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;color:#FF0000"><script>dw(linkunset)</script></td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:50px;" id="nextBelow" class="nextBelow0">
		<tr>
			<td align="center" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei"><script>dw(nextBelow)</script></td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:80px;" id="ManuallySeleTable" class="tablewidth0">
		<tr>
			<td id="leftwidth" class="leftwidth0" align="left"></td>
			<td id="mwidth" class="mwidth0" align="left"><input type="radio" name="startconfig" value="no"><script>dw(redetect)</script>&nbsp;:</td>
			<td id="rightwidth" class="rightwidth0" align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;cursor:pointer" ><script>dw(redetect2)</script></td>
		</tr>
	
		<tr>
			<td></td>
			<td align="left"><input type="radio" name="startconfig" value="yes"><script>dw(manualConfig)</script>&nbsp;:</td>
			<td style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei;cursor:pointer" ><script>dw(manualConfig2)</script></td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:53px;width:780px">
		<tr><td></td></tr>
	</table>

	<table border="0" align="center" style="height:28px;width:780px">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>
	
	<table border="0" align="center" style="height:60px;width:780px">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
<input type="hidden" value="/hwsetup.asp" name="submit-url">
</div>
</form>
</body>
</html>
