<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<style type="text/css">
	#out{width:300px;height:20px;background:#CCCCCC;}
	#in{width:1px; height:20px;background:#ff7f50;color:#000000;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
	.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }
</style>
<SCRIPT>
var saveGAdevicelog = "<% getInfo("saveGAdevicelog"); %>"
var saveSystemLog = "<% getInfo("saveSystemLog"); %>"
var wizMode = <%getVar("wizmode","","");%>;
var check_count = 1; //5000(5s) * 40(check_count)=200000(200s)
var setloading;

function loading(){
	document.getElementById('progress').innerHTML=parseInt(($('#in').width()/300) * 100, 10)+'%';
}
function resetClick()
{
  if ( !confirm(showText(wanttoreset)) ) 
		return false;
  else
	{
		alert(showText(takesometime));
		return true;
	}
}
function init()
{
	$("#help-box").hide();

	if(typeof(_EXPORT_SYS_LOG_) != 'undefined')
		document.getElementById("Log").style.display = "block";		
	if(typeof(_WIFI_ROMAING_) != 'undefined')
		document.getElementById("GALog").style.display = "block";	
}
function goSave()
{
	document.saveform.SaveText.value="1";
	document.saveform.submit();
}

/*function evaltF()
{
	var hid = document.uploadConfig;
	if(hid.binary.value.length == 0) {
		alert(showText(enterfile));
		return false;
	}
	if(hid.binary.value.match(".bin") != ".bin" ){			
			alert(showText(InvalidSomething).replace(/#####/,showText(FileFormat)));
			return false;
	}

	return true;
}*/

function evaltF()
{
	if ( messageCheck() )
		return true;
	return false;
}

function messageCheck()
{
	var hid = document.uploadConfig;
	if(hid.binary.value.length == 0) {
		alert(showText(enterfile));
		return false;
	}
	else if(hid.binary.value.match(".bin") != ".bin" ){			
		alert(showText(InvalidSomething).replace(/#####/,showText(FileFormat)));
		return false;
	}
	else{
//		if (confirm(showText(continueupgrade)))
//		{
//			alert(showText(endofupgrade));

			var jForm = new FormData();
			jForm.append("file", $('#binaryFile').get(0).files[0]);

			$.ajax({
				url: 'goform/formSaveConfigSec',
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
//		}
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
		$.get("getConfigStatus.asp", function (data) {
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

function goHome(){
	window.top.location.replace("/index.asp");
}
</SCRIPT>

</head>
<body onLoad="init()">
			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(BackupRestoreHelp)</script></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>
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
							document.write('<input type="button" value="'+showText(OK)+'" name="goback" id="goback" onclick="goHome()" class="ui-button-nolimit">');	

						</script>
					</td>
				</tr>
			</table>

		</span>

		<span id='SelectFilePage' style='display:block'>
			<fieldset name="PasswordSetup_fieldset" id="PasswordSetup_fieldset">
				<legend><script>dw(BackupRestore)</script></legend>
						<br>
						<form action=/goform/formSaveConfig method=POST name="saveConfig">
							<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
										<tr>
											<td><div class="itemText-len"><script>dw(BackupSettings)</script></div></td>
											<td><div class="itemText2-len">
												<script>document.write('<input type="submit" value="'+showText(save)+'" name="save" class="ui-button-nolimit">')</script></div></td>
										</tr>
								</table>
								<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
					</form>
				
					<form action=/goform/formSaveConfigSec enctype="multipart/form-data" method="post" name="uploadConfig">
							<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
										<tr>
											<td><div class="itemText-len"><script>dw(RestoreSettings)</script></div></td>
											<td><div class="itemText2-len" nowrap>
											<input type="file" size=24 name="binary" id="binaryFile" onkeypress="return false">
											<!--<input type="file" name="binary" size=24>-->
											<script>document.write('<input type="submit" value="'+showText(Upload)+'" name="load" class="ui-button-nolimit" onclick="return evaltF();">')</script></div></td>
										</tr>
							</table>
							<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
					</form>

					<form action=/goform/formResetDefault enctype="multipart/form-data" method="post" name="resetDefault">
							<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
										<tr>
											<td><div class="itemText-len"><script>dw(RestoretoDefault)</script></div></td>
											<td><div class="itemText2-len"><script>document.write('<input type="submit" value="'+showText(Reset)+'" name="reset" onclick="return resetClick()" class="ui-button-nolimit">')</script></div></td>
											<input type="hidden" value="/index.asp" name="submit-url">
										</tr>
							</table>
					</form>

					<span id="Log" style="display:none">
					<form action="/goform/formSystemLog" method="POST" name="saveSystemLog">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
							<tr>
								<td><div class="itemText-len"><script>dw(Debug_Logs)</script></div></td>
								<td><div class="itemText2-len">
								<script>document.write('<input type="submit" value="'+showText(save)+'" name="export" class="ui-button-nolimit">')</script></div></td>
								<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
							</tr>
						</table>
					</form>
					</span>

					<span id="GALog" style="display:none">
					<form action="/goform/formGALog" method="POST" name="saveGAdevicelog">
						<table width="600" align="center" border="0" Cellspacing="1" cellpadding="4">
							<tr>
								<td><div class="itemText-len">GA_<script>dw(Debug_Logs)</script></div></td>
								<td><div class="itemText2-len">
								<script>document.write('<input type="submit" value="'+showText(save)+'" name="export2" class="ui-button-nolimit">')</script></div></td>
								<input type="hidden" value="/admin_backrestore.asp" name="submit-url">
							</tr>
						</table>
					</form>
					</span>
					<br>
			</fieldset>
		</span>
		
		<span id='counting' style='display:none'>

					<table width='700' border="0" align='left'>
						<tr>
							<td colspan='2'><span class='restartInfo'><script>document.write(showText(ProcessingPleaseWait));</script></span>
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
								<span class='restartInfo'><script>document.write(showText(Reloadsuccess));</script></span>
							</td></tr>
					</table>
					
					<br><br>
					
					<table width='700' border="0" align='left'><tr><td>
								<span class='restartitemText' style='line-height:22px;'><script>document.write('<input type=button name="okbutton" value="'+showText(OK)+'" class="ui-button" OnClick="return goHome()">');</script></span>
							</td></tr>
					</table>
		</span>
		
<script>
	$("#help-box").hide();
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
</html>
