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
var wizMode = <%getVar("wizmode","","");%>;

total_mainItem = 4;
selectItem = 11;        
befor_selectItem = 11; 
select_subItem = 11;

arrow_right = "graphics/dot-1.png";
arrow_down = "graphics/dot-2.png";
mainItem_TextColor = '#2e2e2e';        
mainItem_BackgrColor = '#f2f2f2';     
change_mainItem_TextColor = '#ffffff';
change_mainItem_BackgrColor = '#ff7f50';
change_subItem_TextColor = '#FF4000';
change_subItem_BackgrColor = '#D8D8D8';


function mouseclick(n) {
	
		var Len;
		Len = total_mainItem;

		for ( i=1; i<=Len; i++){
			if(i!=3)
			{
				imagename="Image"+i;
				listname="list"+i;
				listnameColor="listTitle"+i;

				document.getElementById(imagename).src = arrow_right;

				document.getElementById(listname).style.backgroundColor=mainItem_BackgrColor;
				document.getElementById(listnameColor).style.color=mainItem_TextColor;
			}
		}

		if (n != selectItem)  
		{
			//document.getElementById("lan_1").style.display = "none";
			//document.getElementById("wps_1").style.display = "none";
			document.getElementById("Administration_1").style.display = "none";
		}

		imageName="Image"+n;
		listname="list"+n;
		listnameColor="listTitle"+n;

		
		//if ((n != 1) && (n != 2) && (n != 3) && (n != 4))
		if( (n == 4) || (n == 3))
		{
			document.getElementById(imageName).src = arrow_down;
		}

		document.getElementById(listname).style.backgroundColor=change_mainItem_BackgrColor;
		document.getElementById(listnameColor).style.color=change_mainItem_TextColor;

		if (n == 4)
			$("#Administration_1").slideDown();
		//if( n == 3 )
		//	$("#wps_1").slideDown();
		//if( n == 3 )
		//	$("#lan_1").slideDown();
			

		selectItem = n;  
}
function sub_list_click(cur_subitem, cur_item, total_sub_item) {

		for ( i=1; i<=total_sub_item; i++){;
			sublistname="list"+cur_item+"_"+i;
			sublistnameColor="subl-ist-Title"+cur_item+"_"+i;
			document.getElementById(sublistname).style.backgroundColor=mainItem_BackgrColor;
			document.getElementById(sublistnameColor).style.color=mainItem_TextColor;
		}

		if((select_subItem != 11) && (befor_selectItem != 11)){   
			sublistname="list"+befor_selectItem+"_"+select_subItem;
			sublistnameColor="subl-ist-Title"+befor_selectItem+"_"+select_subItem;
			document.getElementById(sublistname).style.backgroundColor=mainItem_BackgrColor;
			document.getElementById(sublistnameColor).style.color=mainItem_TextColor;
		}
	
		sublistname="list"+cur_item+"_"+cur_subitem;
		sublistnameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
		document.getElementById(sublistname).style.backgroundColor=change_subItem_BackgrColor;
		document.getElementById(sublistnameColor).style.color=change_subItem_TextColor;
		
    select_subItem=cur_subitem;
		befor_selectItem=cur_item;    
}

function ini_list() {
	mouseclick(1);
	setListWidth();
}

function color_change_mouseout(m)
{
	listName = "list"+m;
	listNameColor="listTitle"+m
	if (selectItem != m){
			document.getElementById(listName).style.backgroundColor=mainItem_BackgrColor;
			document.getElementById(listNameColor).style.color=mainItem_TextColor;
	}
}
function color_change_mouseover(m)
{
	listName = "list"+m;
	listNameColor="listTitle"+m
	if (selectItem != m){
		document.getElementById(listName).style.backgroundColor=change_mainItem_BackgrColor;
		document.getElementById(listNameColor).style.color=change_mainItem_TextColor;
	}
}

function sub_color_change_mouseout(cur_subitem, cur_item)
{
	sublistName = "list"+cur_item+"_"+cur_subitem;
	sublistNameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
	if ((select_subItem != cur_subitem) || (cur_item != befor_selectItem)){
			document.getElementById(sublistName).style.backgroundColor=mainItem_BackgrColor;
			document.getElementById(sublistNameColor).style.color=mainItem_TextColor;
	}
}
function sub_color_change_mouseover(cur_subitem, cur_item)
{
	
	sublistName = "list"+cur_item+"_"+cur_subitem;
	sublistNameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
	if ((select_subItem != cur_subitem) || (cur_item != befor_selectItem)){
		document.getElementById(sublistName).style.backgroundColor=change_subItem_BackgrColor;
		document.getElementById(sublistNameColor).style.color=change_subItem_TextColor;
	}
}
function setListWidth()
{	
	List_width=lframe_list_width-1;
	document.getElementById('list_table0').style.width = List_width+"px";
	document.getElementById('list_table1').style.width = List_width+"px";
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
	<table id="list_table0" width="189" frame="rhs" BORDERCOLOR="#d8d8d8" style="border-width: 0px 1px 0px 0px;" cellspacing="0" cellpadding="0" height="698">
		<tr>
			<td valign="top">
				<table id="list_table1" width="189" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td height="100" valign="top">						
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list1" onmouseout="color_change_mouseout(1);" onmouseover="color_change_mouseover(1);">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image1" id="Image1">&nbsp;
										<a href="status_noInternet.asp" target='main'>
										<span class="mainlist" id="listTitle1" onClick="mouseclick(1);" ><script>dw(Status)</script></span>
										</a>
									</td>
								</tr>
							</table>

							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list2" onmouseout="color_change_mouseout(2);" onmouseover="color_change_mouseover(2);">
								<tr>
									<td nowrap="nowrap">
										<img src="" class="arrowPointSize" border="0" name="Image2" id="Image2">&nbsp;
										<a href="setup_wizard.asp" target='main'>
										<span class="mainlist" id="listTitle2" onClick="mouseclick(2);"  ><script>dw(SetupWizard)</script></span>
										</a>
									</td>
								</tr>
							</table>

							
							<!--==<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list3" onmouseout="color_change_mouseout(3);" onmouseover="color_change_mouseover(3);">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image3" id="Image3">&nbsp;
										<span class="mainlist" id="listTitle3" onClick="mouseclick(3);" ><script>dw(LAN)</script></span>
										</a>
									</td>
								</tr>
							</table>
								<div id="lan_1">
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list3_1" onmouseout="sub_color_change_mouseout(1,3);" onmouseover="sub_color_change_mouseover(1,3);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='lan_ap.asp' target='main'><span class="sublist" onClick="sub_list_click(1,3,2);" id="subl-ist-Title3_1">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
											</tr>
										</table>
										<table width="100%" class="last_subitemBorder" border="0" cellspacing="0" cellpadding="3" id="list3_2" onmouseout="sub_color_change_mouseout(2,3);" onmouseover="sub_color_change_mouseover(2,3);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='lan_ap5g.asp' target='main'><span class="sublist" onClick="sub_list_click(2,3,2);" id="subl-ist-Title3_2">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
											</tr>
										</table>
								</div>==-->
							
							<!--==<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list3" onmouseout="color_change_mouseout(3);" onmouseover="color_change_mouseover(3);">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image3" id="Image3">&nbsp;
										<span class="mainlist" id="listTitle3" onClick="mouseclick(3);" ><script>dw(WPS)</script></span>
										</a>
									</td>
								</tr>
							</table>
								<div id="wps_1">
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list3_1" onmouseout="sub_color_change_mouseout(1,3);" onmouseover="sub_color_change_mouseover(1,3);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='wireless_wps.asp' target='main'><span class="sublist" onClick="sub_list_click(1,3,2);" id="subl-ist-Title3_1">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
											</tr>
										</table>
										<table width="100%"  class="last_subitemBorder" border="0" cellspacing="0" cellpadding="3" id="list3_2" onmouseout="sub_color_change_mouseout(2,3);" onmouseover="sub_color_change_mouseover(2,3);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='wireless5g_wps.asp' target='main'><span class="sublist" onClick="sub_list_click(2,3,2);" id="subl-ist-Title3_2">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></td>
											</tr>
										</table>
								</div>
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list5" onmouseout="color_change_mouseout(5);" onmouseover="color_change_mouseover(5);">
								<tr>
									<td nowrap="nowrap" >
										<img src="" class="arrowPointSize" border="0" name="Image5" id="Image5">&nbsp;
										<a href="wireless5g_wps.asp" target='main'>
										<span class="mainlist" id="listTitle5" onClick="mouseclick(5);" >5G&nbsp;Hz&nbsp;<script>dw(WPS)</script></span>
										</a>
									</td>
								</tr>
							</table>==-->
						
							<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list4" onmouseout="color_change_mouseout(4);" onmouseover="color_change_mouseover(4);">
								<tr>
									<td nowrap="nowrap" >

										<img src="" class="arrowPointSize" border="0" name="Image4" id="Image4">&nbsp;
										<span class="mainlist" onClick="mouseclick(4);" id="listTitle4"><script>dw(Administration)</script></span>
									</td>
								</tr>
							</table>
									<div id="Administration_1">
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_1" onmouseout="sub_color_change_mouseout(1,4);" onmouseover="sub_color_change_mouseover(1,4);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='admin_timezone.asp' target='main'><span class="sublist" onClick="sub_list_click(1,4,5);" id="subl-ist-Title4_1"><script>dw(TimeZone)</script></span></a></td>
											</tr>
										</table>
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_2" onmouseout="sub_color_change_mouseout(2,4);" onmouseover="sub_color_change_mouseover(2,4);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='admin_password.asp' target='main'><span class="sublist" onClick="sub_list_click(2,4,5);" id="subl-ist-Title4_2"><script>dw(Password)</script></span></a></td>
											</tr>
										</table>
									
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_3" onmouseout="sub_color_change_mouseout(3,4);" onmouseover="sub_color_change_mouseover(3,4);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='admin_backrestore.asp' target='main'><span class="sublist" onClick="sub_list_click(3,4,5);" id="subl-ist-Title4_3"><script>dw(BackupRestore)</script></span></a></td>
											</tr>
										</table>
										<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_4" onmouseout="sub_color_change_mouseout(4,4);" onmouseover="sub_color_change_mouseover(4,4);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap" id="IEBrowser"><a href='admin_upgradeIE.asp' target='main'><span class="sublist" onClick="sub_list_click(4,4,5);" id="subl-ist-Title4_4"><script>dw(Upgrade)</script></span></a></td>
												<td nowrap="nowrap" id="OtherBrowser"><a href='admin_upgrade.asp' target='main'><span class="sublist" onClick="sub_list_click(4,4,5);" id="subl-ist-Title4_4"><script>dw(Upgrade)</script></span></a></td>
											</tr>
										</table>
										<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list4_5" onmouseout="sub_color_change_mouseout(5,4);" onmouseover="sub_color_change_mouseover(5,4);">
											<tr>
												<td nowrap="nowrap" width="15"></td>
												<td nowrap="nowrap"><a href='admin_restart.asp' target='main'><span class="sublist" onClick="sub_list_click(5,4,5);" id="subl-ist-Title4_5"><script>dw(Restart)</script></span></a></td>
											</tr>
										</table>
										
									</div>
					

						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table> 
<script>
	$(function () {
		var Sys = {};
		var ua = navigator.userAgent.toLowerCase();
		var s;
		(s = ua.match(/rv:([\d.]+)\) like gecko/)) ? Sys.ie = s[1] :
		(s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :
		(s = ua.match(/firefox\/([\d.]+)/)) ? Sys.firefox = s[1] :
		(s = ua.match(/chrome\/([\d.]+)/)) ? Sys.chrome = s[1] :
		(s = ua.match(/opera.([\d.]+)/)) ? Sys.opera = s[1] :
		(s = ua.match(/version\/([\d.]+).*safari/)) ? Sys.safari = s[1] : 0;

		if (Sys.ie == "8.0") {
			//Browser = "1"; //Browser is IE 8	
			document.getElementById("IEBrowser").style.display="block";
			document.getElementById("OtherBrowser").style.display="none";
		}else{
			//Browser = "0"; //Browser is not IE 8	
			document.getElementById("IEBrowser").style.display="none";
			document.getElementById("OtherBrowser").style.display="block";							
		}
	});	
</script>	
</body>
</html>
