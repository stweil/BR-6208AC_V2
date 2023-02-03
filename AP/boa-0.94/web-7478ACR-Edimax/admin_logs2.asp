<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
function switch_mode( mode )
{

	/*$("#help-box").hide();

	var item_array = new Array
	(
		document.getElementById("sysLog_fieldset"),
		document.getElementById("SecurityLog_fieldset")
	)


	var help_info_array = new Array
	(
		showText(sysLogHelpInfo),
		showText(SecurityLogHelpInfo)
	);
	
	for(i=0; i<item_array.length; i++)
	{
		if(item_array[i])
		{

			if(i==mode)
			{
				item_array[i].style.display="block";
				document.getElementById("help-info1").innerHTML=help_info_array[i];
			}
			else
				item_array[i].style.display="none";
		}
	}*/
	document.location.replace("admin_logs.asp");
}

function init()
{
	$("#help-box").hide();
	//document.Log_form.Logtype.selectedIndex = 0;
 	//switch_mode(0);
}
function refreshLog()
{
	document.location.replace("admin_logs2.asp");
}
</SCRIPT>

</head>
<body class="" onLoad="init()">
<!--======================================================================================================================================================-->

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr>
						<td><div class="help-text" id="help-info1"><script>dw(SecurityLogHelpInfo)</script>
						</div></td>
					</tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>
<!--====================================================================================================================================================================================-->


<blockquote>

		<form action="" method="" name="Log_form">
				<table border="0" align="center">
					<tr>
						<td><div class="status-itemText-len"></div></td>
						<td><div style="text-align:right; width:370px;">
							<select name="Logtype" id="Logtype" onChange="switch_mode(this.value);" class="">
								<option value="0"><script>dw(System_Log)</script></option>
								<option value="1" selected><script>dw(Security_Log)</script></option>

							</select>
						</div></td>
					</tr>
				</table>
		</form>


<!--=====================================================================================================================================================================-->
			<fieldset name="SecurityLog_fieldset" id="SecurityLog_fieldset">
				<legend><script>dw(Security_Log)</script></legend>

					<table width="600" border="0" cellspacing="1" cellpadding="0" align="center">
						<tr>
							<td colspan="3" valign="top" align="center"><br>
								<form method="post" action="goform/formSecLog" name="secLog">
									<textarea cols="75" name="logtext" readOnly rows="9" wrap="off" edit="off" style="color:Navy resize: none;"><% getInfo("seclog"); %></textarea>
										<p align="center">
											<script>buffer='<input type ="submit" name ="send" value ="'+showText(save)+'" class="ui-button">';document.write(buffer);</script>
											<script>buffer='<input type ="submit" name ="reset" value ="'+showText(clear)+'" class="ui-button">';document.write(buffer);</script>
											<script>buffer='<input type ="button" name ="refresh" onclick ="refreshLog()" value ="'+showText(refresh)+'" class="ui-button">';document.write(buffer);</script>
											<input type="hidden" value="/admin_logs2.asp" name="submit-url">
										</p>
								</form>
							</td>
						</tr>
					</table>
			</fieldset>


<!--=====================================================================================================================================================================-->
<script>
			
$("#help-box").hide();

</script>
<!--======================================================================================================================================================-->
</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		

</html>
