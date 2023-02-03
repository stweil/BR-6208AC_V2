<html>
<head>
<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
<meta http-equiv='Pragma' content='no-cache'>
<meta http-equiv='Expires' content='-1'>
<script type='text/javascript' src='/file/multilanguage.var'></script>
<script type='text/javascript' src='/file/javascript.js'></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<link rel='stylesheet' href='/set.css'>
<title>Download firmware</title>
<style type="text/css">
	#out{width:300px;height:20px;background:#CCCCCC;}
	#in{width:0px; height:20px;background:#ff7f50;color:#000000;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
	.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }
</style>
<script>
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var check_count = 1; //5000(5s) * 40(check_count)=200000(200s)
var check_file_status;
var retry = 0;
var progressNum = 0;
function loading()
{
	progressNum=parseInt(($("#in").width()/300) * 100, 10);
	if(progressNum == 0) progressNum=1;
	document.getElementById("progress").innerHTML=progressNum+"%";
}
var setloading=setInterval("loading()", 100);

function init()
{
	document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(downloading)+"</span>";
}
function goback2upgrade()
{
	if(parent.back2status == 1){
		parent.back2status=0;

		if((wizMode==0) || (wizMode==4) || (iqsetup_finish==0))
			document.fail.failurl.value="/status.asp";//window.location.assign("status.asp");
		else
			document.fail.failurl.value="/status_noInternet.asp";//window.location.assign("status_noInternet.asp");
	}
	else
		document.fail.failurl.value="/admin_upgrade.asp"; //window.location.assign("admin_upgrade.asp");
	
	document.fail.submit();
}


function check_upgrade_stat(){
	if(check_count>=40)
	{
		window.clearInterval(check_upgrade_statStop);
		$("#in").stop().animate({width: '99%'},function(){
			$("#in").animate({width: '100%'}, 2000,function(){
				window.clearInterval(setloading);
				document.getElementById("progress").innerHTML="100%";
				$("#in").stop().animate({width: '100%'},function(){
					document.getElementById('counting').style.display='none';
					document.getElementById('ok').style.display='block';
					parent.lFrame.location.reload();
				});
			});
		});
	}
	else
	{
		check_count=check_count+1;
		$.get("getFWStatus.asp", function (data) {
			if(data == 0){
				//alert("in Invalidfw");
				window.clearInterval(check_upgrade_statStop);
				window.clearInterval(setloading);
				document.getElementById('counting').style.display='none';
				document.getElementById('error').style.display='block';
			}

			if(data == 1){
				//alert("in rebootComplete");
				window.clearInterval(check_upgrade_statStop);
				$("#in").stop().animate({width: '99%'},function(){
					$("#in").animate({width: '100%'}, 2000,function(){
						window.clearInterval(setloading);
						document.getElementById("progress").innerHTML="100%";
						$("#in").stop().animate({width: '100%'},function(){
							document.getElementById('counting').style.display='none';
							document.getElementById('ok').style.display='block';
							parent.lFrame.location.reload();
						});
					});
				});
			}

		})

	}
}
function periodCheck()
{
	document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(Firmwareupgrading)+"</span>";
	document.getElementById('counting').style.display='block';

	setloading=setInterval('loading()', 1000);
	$(document).ready(function() {
		$("#in").animate({width: '99%'}, 200000,function(){
				window.clearInterval(check_upgrade_statStop);
				window.clearInterval(setloading);
				document.getElementById("progress").innerHTML="100%";
				$("#in").stop().animate({width: '100%'},function(){
					document.getElementById('counting').style.display='none';
					document.getElementById('ok').style.display='block';
					parent.lFrame.location.reload();
				});
		});
	});
	check_upgrade_statStop = setInterval("check_upgrade_stat()", 5000);
}
function goback2upgrade()
{
	if(parent.back2status == 1){
		parent.back2status=0;

		/*if((wizMode==0) || (wizMode==4) || (iqsetup_finish==0))
			document.fail.failurl.value="/status.asp";//window.location.assign("status.asp");
		else*/
			document.fail.failurl.value="/status_noInternet.asp";//window.location.assign("status_noInternet.asp");
	}
	else
		document.fail.failurl.value="/admin_upgrade.asp"; //window.location.assign("admin_upgrade.asp");
	
	document.fail.submit();
}

function installFW_now()
{

	progressNum = 0;
	document.getElementById("progress").innerHTML="1%";
	$('#in').width('1%');

	$.ajax({
	  url: 'goform/formAutoFWupgrade',
	  type: 'POST',
	  data: $('#installFW').serialize(),
	  async: true,
	  dataType:'text',

	  success: function () {
				//alert("success");
	      periodCheck();
	  },
	  error: function(){
				//alert("error");
				periodCheck();
	  }
	});

}
</script>
</head>
<body onLoad="init()">

<form id="checkFWform2">
	<input type="hidden" name="action" value="checkFWfile">
</form>
<form id="checkFWstop">
	<input type="hidden" value="killps" name="action">
</form>
<form action="/goform/formAutoFWupgrade" method="POST" name="installFW" id="installFW">
	<input type="hidden" name="action" value="install_FW">
	<input type="hidden" value="/index.asp" name="submit-url">
</form>
<form action="/goform/formAutoFWupgrade" method="POST" name="fail">
	<input type="hidden" value="fail" name="action">
	<input type="hidden" value="" name="failurl">
</form>

<blockquote>

	<span id='counting'>

			<br>
				<table width="600" border=0 align="left">
					<tr>
						<td colspan="2" id="downloadinfo">
						</td>
					</tr>	
				</table>
				<br>
				<table width="600" border=0 align="left">
					<tr>
						<td style="width:305px;">
							<div id='out'>
								<div id="in" style="width:0%;"></div>
							</div>
						</td>
						<td id="progress" style="width:295px;">1%</td>
					</tr>
				</table>
	</span>

	<span id='error' style="display:none">
		
			<br>
				<table width="600" border=0 align="left">
					<tr>
						<td id="errorinfo">
							<span class="restartInfo"><script>dw(downloadingfail)</script></span>
						</td>
					</tr>	
				</table>
				<br>
				<br>
				<table width="600" border=0 align="left">
					<tr>
						<td >
							<script>
								document.write('<input type="button" value="'+showText(goback)+'" name="goback" id="goback" onclick="goback2upgrade()" class="ui-button-nolimit">');	

							</script>
						</td>
					</tr>
				</table>
		
	</span>

	<span id='ok' style='display:none'>
			<br>
			<table width='700' border=0 align='left'><tr><td>
						<span class='restartInfo'><script>dw(upgrade_accomplished)</script></span>
					</td></tr></table>
			<br><br>
			<table width='700' border=0 align='left'><tr><td>
						<span class='restartitemText' style='line-height:22px;'><script>dw(upgrade_accomplished_content)</script></span>
					</td></tr></table>

	</span>
	</blockquote>

	<script type="text/javascript">
		$(document).ready(function() { /* if page is ready */
			$("#in").animate({width: '90%'}, 90000,function(){	/* use 60 sec set width form 0% to 90% */	  			
				$.post("/goform/formAutoFWupgrade", $("#checkFWform2").serialize(),function(data){
					if(data=="download_ok"){	
							window.clearInterval(check_file_status);
							document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(checkdownloading)+"</span>";
							window.clearInterval(setloading);
							document.getElementById("progress").innerHTML="100%";
							$("#in").stop().animate({width: '100%'},function(){
								document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(downloadingok)+"</span>";
								installFW_now();
								//document.installFW.submit();
							});
					}
				},"text");
			});
			check_file_status = setInterval("check_file_stat()", 15000); /* check download every 20 sec */
		});

	function check_file_stat(){
		$.post("/goform/formAutoFWupgrade", $("#checkFWform2").serialize(),function(data){ /* submit and get data */
			if(data=="download_ok"){
				window.clearInterval(check_file_status);
				document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(checkdownloading)+"</span>";
				$("#in").stop().animate({width: '90%'},function(){ // set width to 90% and stop. change meg 
					$("#in").animate({width: '100%'}, 5000,function(){  // use 5 sec set width form 90% to 100% 
						document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(downloadingok)+"</span>";
						window.clearInterval(setloading);
						document.getElementById("progress").innerHTML="100%";
						//document.installFW.submit();
						installFW_now();
					});
				});
			}
			if((data=="download_fail_md5") || (data=="download_fail_no_file") ){  // show fail info 
				window.clearInterval(check_file_status);
				window.clearInterval(setloading);
				document.getElementById("counting").style.display="none";
				document.getElementById("error").style.display="block";
				$.post("/goform/formAutoFWupgrade", $("#checkFWstop").serialize(),function(data){},"text");  //kill download ps 
			}
			if((data=="waitCurl") && (retry==6)){ // After 90 sec ( 15sec *6 ), if no fw, show fail info 
				window.clearInterval(check_file_status);
				window.clearInterval(setloading);
				document.getElementById("counting").style.display="none";
				document.getElementById("error").style.display="block";
				$.post("/goform/formAutoFWupgrade", $("#checkFWstop").serialize(),function(data){},"text");  //kill download ps 
			}
			else
				retry++;
	
			if(retry==6) /* Change meg */
				document.getElementById("downloadinfo").innerHTML="<span class=\"restartInfo\">"+showText(checkdownloading)+"</span>";
		},"text");
	}

	</script>
</body>
</html>

