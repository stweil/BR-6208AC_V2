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

<script>
var wizMode = <%getVar("wizmode","","");%>;

function progressBar()
{
	progressNum=parseInt(($("#in").width()/300) * 100, 10);
	if(progressNum == 0) progressNum=1;
	document.getElementById("progress").innerHTML=progressNum+"%";
}
var setprogressNum=setInterval("progressBar()", 1000);
var EearlyStopcheck;

</script>

<style type="text/css">
	#out{width:300px;height:20px;background:#FFFFFF;}
	#in{width:0px; height:20px;background:#FFFFFF;color:#FFFFFF;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
</style>

</head>
<body>
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

	<table border="0" style="height:50px;width:700px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="width:790px" align="center">
		<tr><td align="center"><span class="Arial_20" style="font-weight:bold;"><script>dw(ConnectingtoInternet)</script></span></td></tr>
		<tr><td align="center" id="progress" class="Arial_20" style="font-weight:bold;">1%</td></tr>
		<tr><td align="center"><img src="graphics/loading.gif" /></td></tr>
	</table>

	<table border="0" style="height:245px;width:700px" align="center">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>

<table width="600" border=0 align="left">
	<tr>
		<td style="width:300px;">
			<div id='out'>
				<div id="in" style="width:0%;"></div>
			</div>
		</td>
	</tr>
</table>

<form id="checkTest">
	<input type="hidden" name="cmd" value="checkWanTest">
</form>


<script>
	$(document).ready(function() {
		$("#in").animate({width: '99%'}, 33000,function(){	
			window.clearInterval(setprogressNum);
			window.clearInterval(EearlyStopcheck);
			//alert("/redirect.asp");
			window.location.assign("/redirect.asp");
		});
		EearlyStopCheck = setInterval("EearlyStop_Check()", 2000);
	});

	function EearlyStop_Check(){
		$.post("/goform/formTestResult", $("#checkTest").serialize(),function(data){
			if(data=="DetectDone"){
				window.clearInterval(setprogressNum);
				window.clearInterval(EearlyStopcheck);
				$("#in").stop().animate({width: '90%'},function(){
					document.getElementById("progress").innerHTML="99%";
					$("#in").animate({width: '100%'}, 500,function(){ 
						//alert("/redirect.asp");
						window.location.assign("/redirect.asp");
					});
				});
			}
		},"text");
	}
</script>

</body>
</html>
