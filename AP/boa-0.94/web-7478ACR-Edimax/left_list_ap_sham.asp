<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>

<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">


<script language="JavaScript">
total_mainItem = 7;
selectItem = 11;        // for init selected item , default is STATUS

arrow_right = "graphics/dot-1.png";
arrow_down = "graphics/dot-2.png";
mainItem_TextColor = '#2e2e2e';        // subitem color is the same
mainItem_BackgrColor = '#f2f2f2';      // subitem color is the same
change_mainItem_TextColor = '#ffffff';
change_mainItem_BackgrColor = '#ff7f50';

function mouseclick(n) {
	
		var Len;
		Len = total_mainItem;

		for ( i=1; i<=Len; i++){
			imagename="Image"+i;
			listname="list"+i;
			listnameColor="listTitle"+i;

			document.getElementById(imagename).src = arrow_right;

			document.getElementById(listname).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(listnameColor).style.color=mainItem_TextColor;//'#2e2e2e';
		}

		if (n != selectItem)  // if click new main item, hide other. if click again, no action
		{
			document.getElementById("Wireless_24").style.display = "none";
			document.getElementById("Wireless_5").style.display = "none";
			document.getElementById("Advance_1").style.display = "none";
			document.getElementById("Administration_1").style.display = "none";
		}

		imageName="Image"+n;
		listname="list"+n;
		listnameColor="listTitle"+n;

		if (n > 3)
		{
			document.getElementById(imageName).src = arrow_down;
		}

		document.getElementById(listname).style.backgroundColor=change_mainItem_BackgrColor;//'#ff7f50';
		document.getElementById(listnameColor).style.color=change_mainItem_TextColor;//'#ffffff';

		if (n == 7)
			$("#Administration_1").slideDown();
		if (n == 6)
			$("#Advance_1").slideDown();
		if (n == 4)
			$("#Wireless_24").slideDown();
		if (n == 5)
			$("#Wireless_5").slideDown();

		selectItem = n;  // for mouse out fail
}

function ini_list() {
	mouseclick(1);
}
</script>

<style type="text/css">
a {FONT-SIZE: 15pt; COLOR: #2e2e2e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei; text-decoration: none;}
.mainlist { cursor: pointer; FONT-SIZE: 13pt; COLOR: #2e2e2e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;}
.sublist {FONT-SIZE: 12pt; COLOR: #2e2e2e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;}
.body_background{	background-color:#f2f2f2; overflow-x:auto; overflow-y:hidden;}
.cursor_pointer {border-bottom-style:solid; border-width:1px; border-color:#d8d8d8;}
.last_subitemBorder {border-bottom-style:solid; border-color:#d8d8d8;border-width:1px;}
.arrowPointSize {height:8px; width:8px;}
</style>

</head>
<body class="body_background" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" onload="ini_list()">
	<table width="189" frame="rhs" BORDERCOLOR="#d8d8d8" style="border-width: 0px 1px 0px 0px;" cellspacing="0" cellpadding="0" height="698">
		<tr>
			<td valign="top">
				<table width="189" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td height="100" valign="top">						
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="" id="list1">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image1" id="Image1">&nbsp;
										<span class="mainlist" id="listTitle1"><script>dw(Status)</script></span>
									</td>
								</tr>
							</table>

							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list2">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image2" id="Image2">&nbsp;
										<span class="mainlist" id="listTitle2"><script>dw(SetupWizard)</script></span>
									</td>
								</tr>
							</table>

							
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list3">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image3" id="Image3">&nbsp;
										<span class="mainlist" id="listTitle3"><script>dw(LAN)</script></span>
									</td>
								</tr>
							</table>
							
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer"  id="list4">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image4" id="Image4">&nbsp;
										<span class="mainlist" id="listTitle4">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span>
									</td>
								</tr>
							</table>
								
								<div id="Wireless_24">
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_1">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title4_1"><script>dw(Basic)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_2">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title4_2"><script>dw(WPS)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3"  id="list4_3">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title4_3"><script>dw(AccessControl)</script></span></td>
										</tr>
									</table>
								</div>


							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list5">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image5" id="Image5">&nbsp;
										<span class="mainlist" id="listTitle5">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span>
									</td>
								</tr>
							</table>
							
								<div id="Wireless_5">
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_1">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title5_1" ><script>dw(Basic)</script></span></td>
										</tr>
									</table>
									
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_2">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title5_2"><script>dw(WPS)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3"  id="list5_3">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title5_3"><script>dw(AccessControl)</script></span></td>
										</tr>
									</table>										
								</div>

							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list6">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image6" id="Image6">&nbsp;
										<span class="mainlist" id="listTitle6"><script>dw(Advance)</script></span>
									</td>
								</tr>
							</table>

								<div id="Advance_1">

									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_1">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title6_1">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
										</tr>
									</table>

									<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_2">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title6_2">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
										</tr>
									</table>	
								</div>
		
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list7">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image7" id="Image7">&nbsp;
										<span class="mainlist" id="listTitle7"><script>dw(Administration)</script></span>
									</td>
								</tr>
							</table>
								<div id="Administration_1">
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_1"">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title7_1"><script>dw(TimeZone)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_2">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title7_2"><script>dw(Password)</script></span></td>
										</tr>
									</table>
								
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_3">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title7_3"><script>dw(BackupRestore)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_4">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap" id="IEBrowser"><span class="sublist" id="subl-ist-Title7_4"><script>dw(Upgrade)</script></span></td>
											<td nowrap="nowrap" id="OtherBrowser"><span class="sublist" id="subl-ist-Title7_4"><script>dw(Upgrade)</script></span></td>
										</tr>
									</table>
									<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_5">
										<tr>
											<td nowrap="nowrap" width="15"></td>
											<td nowrap="nowrap"><span class="sublist" id="subl-ist-Title7_5"><script>dw(Restart)</script></span></td>
										</tr>
									</table>
								</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table> 
</body>
</html>
