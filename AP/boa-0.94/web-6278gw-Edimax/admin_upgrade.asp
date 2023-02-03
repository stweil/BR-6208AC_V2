<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">

<% language=javascript %>

<style type="text/css">
	#out{width:300px;height:20px;background:#CCCCCC;}
	#in{width:1px; height:20px;background:#ff7f50;color:#000000;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
	.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }
</style>

<SCRIPT>
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;

var check_count = 1; //5000(5s) * 40(check_count)=200000(200s)
var setloading;
function loading(){
	document.getElementById('progress').innerHTML=parseInt(($('#in').width()/300) * 100, 10)+'%';
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
	document.getElementById('SelectFilePage').style.display='none';
	document.getElementById('counting').style.display='block';

	setloading=setInterval('loading()', 1000);
	$(document).ready(function() {
		$("#in").animate({width: '99%'}, 200000,function(){
			//$.post("/goform/formJQueryData", $("#checkUpgrade").serialize(),function(data){
				window.clearInterval(check_upgrade_statStop);
				window.clearInterval(setloading);
				document.getElementById("progress").innerHTML="100%";
				$("#in").stop().animate({width: '100%'},function(){
					document.getElementById('counting').style.display='none';
					document.getElementById('ok').style.display='block';
					parent.lFrame.location.reload();
				});
			//});
		});
	});
	check_upgrade_statStop = setInterval("check_upgrade_stat()", 5000);
}

function init()
{
	$("#help-box").hide();
}
function evaltF()
{
	if ( messageCheck() )
		return true;
	return false;
}
function messageCheck()
{
	var hid = document.upload;
	if(hid.binary.value.length == 0) {
		alert(showText(enterfile));
		return false;
	}
	else if(hid.binary.value.match(".bin") != ".bin" ){			
			alert(showText(InvalidSomething).replace(/#####/,showText(FileFormat)));
			return false;
	}
	else{
		if (confirm(showText(continueupgrade)))
		{
			alert(showText(endofupgrade));

      var jForm = new FormData();
			jForm.append("file", $('#binaryFile').get(0).files[0]);

      $.ajax({
          url: 'goform/formUpload',
          type: 'POST',
					mimeType:"multipart/form-data",
          data: jForm,
          async: true,
          cache: false,
          contentType: false,
          processData: false,
          success: function () {
              periodCheck();
          },
          error: function(){
							periodCheck();
          }
      });


		}
	}
}

function upgrade_action()
{
	with(document.AutoFWupgrade) {
		action.value = "check_version";
		submit();
	}
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
</SCRIPT>

</head>
<body onLoad="init()">



	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td><div class="help-text" id="help-info1"><script>dw(UpgradeHelp)</script></td></tr>
		</table>
	</div>

	<script>
	HelpButton();
	</script>

<form action="/goform/formAutoFWupgrade" method="POST" name="fail">
	<input type="hidden" value="fail" name="action">
	<input type="hidden" value="" name="failurl">
</form>

<form id="checkUpgrade">
	<input type="hidden" name="action" value="checkFWupgrade">
</form>

<form action="/goform/formAutoFWupgrade" method="POST" name="AutoFWupgrade">
	<input type="hidden" value="" name="action">
	<input type="hidden" value="/auto_upgrade.asp" name="submit-url">
</form>	

<blockquote>

<span id='error' style="display:none">

	<table width="600" border="0" align="left">
		<tr>
			<td id="errorinfo">
				<span class="restartInfo"><script>document.write(showText(InvalidSomething).replace(/#####/,showText(FileFormat)))</script></span>
			</td>
		</tr>	
	</table>
	<br>
	<br>
	<table width="600" border="0" align="left">
		<tr>
			<td >
				<script>
					document.write('<input type="button" value="'+showText(goback)+'" name="goback" id="goback" onclick="goback2upgrade()" class="ui-button-nolimit">');	

				</script>
			</td>
		</tr>
	</table>

</span>


<span id='SelectFilePage' style='display:block'>
	
		<fieldset name="manual_upgrade_fieldset">
			<legend><script>dw(Upgrade)</script></legend>
					<br>
					
					<table align="center" border="0" Cellspacing="0" width="700">
							<tr>
								<td style="text-align:center;" class="itemText" width="60%">
									<script>dw(CurrentFirmwareVersion)</script>&nbsp;:&nbsp;
									<span class="choose-itemText"><% getInfo("fwVersion"); %></span>
								</td>
							</tr>
					</table>
					<table align="center" border="0" Cellspacing="0" width="700" >
						<tr>
							<td style="text-align:center; padding-top:10px;">
								<script>
								document.write('<input type="button" value="'+showText(checkVersion)+'" name="checkVersion" onclick="upgrade_action()" class="ui-button-nolimit">');	
								</script>
							</td>
						</tr>
					</table>
					<br>
					<form method="post" action="goform/formUpload" enctype="multipart/form-data" name="upload" id="fwupload">
							<table border="0" cellspacing="0" cellpadding="0" align="center">
								<tr>
										<td>
									<p align="center"><input type="file" size="31" maxlength="31" name="binary" id="binaryFile" onkeypress="return false"></p>
									</td>
								</tr>
							</table>
							
							<p align="center">
								<script>buffer='<input type="button" id="apply" value="'+showText(Apply)+'" onclick="return evaltF();" class="ui-button">';document.write(buffer);</script>
								<input type="hidden" value="/index.asp" name="submit-url">
							</p>
					</form>
					
  </fieldset>
	
</span>


<span id='counting' style='display:none'>

			<table width='700' border="0" align='left'>
				<tr>
					<td colspan='2'><span class='restartInfo'><script>dw(Firmwareupgrading);</script></span>
					</td>
				</tr>
			</table>
		<br>
			<table width='700' border="0" align='left'>
				<tr>
					<td style='width:305px;'>
						<div id='out'>
							<div id='in' style='width:1%;'></div>
						</div>
					</td>
					<td id='progress' style='width:395px;'>1%</td>
				</tr>
			</table>

</span>
<span id='ok' style='display:none'>

			<table width='700' border="0" align='left'><tr><td>
						<span class='restartInfo'><script>dw(upgrade_accomplished)</script></span>
					</td></tr></table>
			<br><br>
			<table width='700' border="0" align='left'><tr><td>
						<span class='restartitemText' style='line-height:22px;'><script>dw(upgrade_accomplished_content)</script></span>
					</td></tr></table>

</span>

	</blockquote>
<script>
	$("#help-box").hide();
</script>

		
</body>
</html>
