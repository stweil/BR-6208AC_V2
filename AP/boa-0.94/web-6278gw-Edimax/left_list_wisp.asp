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
total_mainItem = 10;
selectItem = 11;        // for init selected item , default is STATUS
befor_selectItem = 11; // for init selected subitem befor click new subitem
select_subItem = 11;

arrow_right = "graphics/dot-1.png";
arrow_down = "graphics/dot-2.png";
mainItem_TextColor = '#2e2e2e';        // subitem color is the same
mainItem_BackgrColor = '#f2f2f2';      // subitem color is the same
change_mainItem_TextColor = '#ffffff';
change_mainItem_BackgrColor = '#ff7f50';
change_subItem_TextColor = '#FF4000';
change_subItem_BackgrColor = '#D8D8D8';
var wizMode = <%getVar("wizmode","","");%>;
var wisp_enable=<% getInfo("stadrv_type");%>;
var FwEnabled = <%getVar("FirewallEnable","","");%>;
var FwEnabled_fromUI=-1; 
function showFwSet(x,display) {
	
		image="Image"+x;

		if (display == 1)
		{
			document.getElementById(image).src = arrow_down;
			$("#firewall_1").slideDown();
		}
		else
		{
			document.getElementById(image).src = arrow_right;
			document.getElementById("firewall_1").style.display = "none";
		}

		FwEnabled_fromUI=display;

		selectItem = x;  // for mouse out fail
}

function mouseclick(n) {
	
		var Len;
		Len = total_mainItem;

		for ( i=1; i<=Len; i++){
			imagename="Image"+i;
			listname="list"+i;
			listnameColor="listTitle"+i;

			if(i==6)
				document.getElementById("list9_6").style.display = "none";
			if( wisp_enable == 3 ) // 1 -> 2.4G , 3 -> 5G
				document.getElementById("list6_4").style.display = "none";
			else
				document.getElementById("list5_4").style.display = "none";

			document.getElementById(imagename).src = arrow_right;
			document.getElementById(listname).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(listnameColor).style.color=mainItem_TextColor;//'#2e2e2e';
		}

		if (n != selectItem)  // if click new main item, hide other. if click again, no action
		{
			document.getElementById("firewall_1").style.display = "none";
			document.getElementById("Internet_1").style.display = "none";
			document.getElementById("Wireless_24").style.display = "none";
			document.getElementById("Wireless_5").style.display = "none";
			document.getElementById("QOS_1").style.display = "none";
			document.getElementById("Advance_1").style.display = "none";
			document.getElementById("Administration_1").style.display = "none";
		}

		imageName="Image"+n;
		listname="list"+n;
		listnameColor="listTitle"+n;

		if ((n != 1) && (n != 2) && (n != 4) && (n != 7))
		{
			document.getElementById(imageName).src = arrow_down;
		}

		if (n == 7)
		{
			if ((FwEnabled_fromUI < 0 ) && (FwEnabled==1))            //if reload
				document.getElementById(imageName).src = arrow_down;
			if ((FwEnabled_fromUI >= 0 ) && (FwEnabled_fromUI == 1))  //if UI set
				document.getElementById(imageName).src = arrow_down;
		}


		document.getElementById(listname).style.backgroundColor=change_mainItem_BackgrColor;//'#ff7f50';
		document.getElementById(listnameColor).style.color=change_mainItem_TextColor;//'#ffffff';

		if (n == 10)
			$("#Administration_1").slideDown();
		if (n == 9)
			$("#Advance_1").slideDown();
		if (n == 8)
			$("#QOS_1").slideDown();			
		if (n == 7)
		{
			if ((FwEnabled_fromUI < 0 ) && (FwEnabled==1))            //if reload
				$("#firewall_1").slideDown();
			if ((FwEnabled_fromUI >= 0 ) && (FwEnabled_fromUI == 1))  //if UI set
				$("#firewall_1").slideDown();
		}
		if (n == 6)
			$("#Wireless_5").slideDown();
		if (n == 5)
			$("#Wireless_24").slideDown();
		if(n == 3)
			$("#Internet_1").slideDown();

		selectItem = n;  // for mouse out fail

}
function sub_list_click(cur_subitem, cur_item, total_sub_item) {

		for ( i=1; i<=total_sub_item; i++){;
			sublistname="list"+cur_item+"_"+i;
			sublistnameColor="subl-ist-Title"+cur_item+"_"+i;
			document.getElementById(sublistname).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(sublistnameColor).style.color=mainItem_TextColor;//'#2e2e2e';
		}

		if((select_subItem != 11) && (befor_selectItem != 11)){   // init selected subitem befor click new subitem, 11 is default value
			sublistname="list"+befor_selectItem+"_"+select_subItem;
			sublistnameColor="subl-ist-Title"+befor_selectItem+"_"+select_subItem;
			document.getElementById(sublistname).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(sublistnameColor).style.color=mainItem_TextColor;//'#2e2e2e';
		}
	
		sublistname="list"+cur_item+"_"+cur_subitem;
		sublistnameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
		document.getElementById(sublistname).style.backgroundColor=change_subItem_BackgrColor;//'#D8D8D8';
		document.getElementById(sublistnameColor).style.color=change_subItem_TextColor;//'#FF4000';
		
    select_subItem=cur_subitem;
		befor_selectItem=cur_item;    //record last click subitem
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
			document.getElementById(listName).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(listNameColor).style.color=mainItem_TextColor;//'#2e2e2e';
	}
}
function color_change_mouseover(m)
{
	listName = "list"+m;
	listNameColor="listTitle"+m
	if (selectItem != m){
		document.getElementById(listName).style.backgroundColor=change_mainItem_BackgrColor;//'#ff7f50';
		document.getElementById(listNameColor).style.color=change_mainItem_TextColor;//'#ffffff';
	}
}

function sub_color_change_mouseout(cur_subitem, cur_item)
{
	sublistName = "list"+cur_item+"_"+cur_subitem;
	sublistNameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
	if ((select_subItem != cur_subitem) || (cur_item != befor_selectItem)){
			document.getElementById(sublistName).style.backgroundColor=mainItem_BackgrColor;//'#f2f2f2';
			document.getElementById(sublistNameColor).style.color=mainItem_TextColor;//'#2e2e2e';
	}
}
function sub_color_change_mouseover(cur_subitem, cur_item)
{
	
	sublistName = "list"+cur_item+"_"+cur_subitem;
	sublistNameColor="subl-ist-Title"+cur_item+"_"+cur_subitem;
	if ((select_subItem != cur_subitem) || (cur_item != befor_selectItem)){
		document.getElementById(sublistName).style.backgroundColor=change_subItem_BackgrColor;//'#D8D8D8';
		document.getElementById(sublistNameColor).style.color=change_subItem_TextColor;//'#FF4000';
	}
}
function setDisplay()
{
	parent.wispmode=1;
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

	<table id="list_table1" width="189" border="0" cellspacing="0" cellpadding="0"><!-- class="autobreak">-->
		<tr>
			<td height="100" valign="top">
			
	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list1" onmouseout="color_change_mouseout(1);" onmouseover="color_change_mouseover(1);">
		<tr>
			<td nowrap="nowrap">
				<img src="" class="arrowPointSize" border="0" name="Image1" id="Image1">&nbsp;
					<a href="status.asp" target='main'>
						<span class="mainlist" id="listTitle1" onClick="mouseclick(1);">
							<script>dw(Status)</script>
						</span>
					</a>
			</td>
		</tr>
	</table>
							
	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list2" onmouseout="color_change_mouseout(2);" onmouseover="color_change_mouseover(2);">
		<tr>
			<td nowrap="nowrap">
				<img src="" class="arrowPointSize" border="0" name="Image2" id="Image2">&nbsp;
					<a href="setup_wizard.asp" target='main'>
						<span class="mainlist" id="listTitle2" onClick="mouseclick(2);">
							<script>dw(SetupWizard)</script>
						</span>
					</a>
			</td>
		</tr>
	</table>

	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list3" onmouseout="color_change_mouseout(3);" onmouseover="color_change_mouseover(3);">
		<tr>
			<td nowrap="nowrap">
				<img src="" class="arrowPointSize" border="0" name="Image3" id="Image3">&nbsp;
					<script>
						document.write('<a href="inter_wisp.asp" target="main" onClick="setDisplay();">')
					</script>
						<span class="mainlist" id="listTitle3" onClick="mouseclick(3);" >
							<script>
								dw(WISP)
							</script>
						</span>
			</td>
		</tr>
	</table>
	<div id="Internet_1" >
		<table width="100%" border="0" cellspacing="0" cellpadding="3"  id="list3_1" onmouseout="sub_color_change_mouseout(1,3);" onmouseover="sub_color_change_mouseover(1,3);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
					<td nowrap="nowrap">
						<a href='inter_wan.asp' target='main'>
							<span class="sublist" id="subl-ist-Title3_1" onClick="sub_list_click(1,3,2);" >
								<script>dw(wansetup)</script>
							</span>
						</a>
					</td>
			</tr>
		</table>
	
		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list3_2" onmouseout="sub_color_change_mouseout(2,3);" onmouseover="sub_color_change_mouseover(2,3);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
					<td nowrap="nowrap">
						<a href="inter_ddns.asp" target="main">
							<span class="sublist" id="subl-ist-Title3_2" onClick="sub_list_click(2,3,2);" >
								<script>dw(DDNS)</script>
							</span>
						</a>
					</td>
			</tr>
		</table>
	</div>

	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list4" onmouseout="color_change_mouseout(4);" onmouseover="color_change_mouseover(4);">
		<tr>
			<td nowrap="nowrap" >
				<img src="" class="arrowPointSize" border="0" name="Image4" id="Image4">&nbsp;
					<a href="lan.asp" target='main'>
						<span class="mainlist" id="listTitle4" onClick="mouseclick(4);" ><script>dw(LAN)</script></span>
					</a>
			</td>
		</tr>
	</table>
							
	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer"  id="list5" onmouseout="color_change_mouseout(5);" onmouseover="color_change_mouseover(5);">
		<tr>
			<td nowrap="nowrap">
				<img src="" class="arrowPointSize" border="0" name="Image5" id="Image5">&nbsp;
					<span class="mainlist" onClick="mouseclick(5);" id="listTitle5">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span>
			</td>
		</tr>
	</table>

	<div id="Wireless_24">
		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_1" onmouseout="sub_color_change_mouseout(1,5);" onmouseover="sub_color_change_mouseover(1,5);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless_basic.asp' target='main'>
						<span class="sublist" id="subl-ist-Title5_1" onClick="sub_list_click(1,5,4);">
							<script>dw(Basic)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_2" onmouseout="sub_color_change_mouseout(2,5);" onmouseover="sub_color_change_mouseover(2,5);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='guest_wireless_basic.asp' target='main'>
						<span class="sublist" id="subl-ist-Title5_2" onClick="sub_list_click(2,5,4);">
							<script>dw(Guest)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_3" onmouseout="sub_color_change_mouseout(3,5);" onmouseover="sub_color_change_mouseover(3,5);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless_wps.asp' target='main'>
						<span class="sublist" id="subl-ist-Title5_3" onClick="sub_list_click(3,5,4);">
							<script>dw(WPS)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list5_4" onmouseout="sub_color_change_mouseout(4,5);" onmouseover="sub_color_change_mouseover(4,5);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless_schedule_v2.asp' target='main'>
						<span class="sublist" id="subl-ist-Title5_4" onClick="sub_list_click(4,5,4);">
							<script>dw(Schedule)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>
	</div>
			
	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list6" onmouseout="color_change_mouseout(6);" onmouseover="color_change_mouseover(6);">
		<tr>
			<td nowrap="nowrap">
				<img src="" class="arrowPointSize" border="0" name="Image6" id="Image6">&nbsp;
					<span class="mainlist" onClick="mouseclick(6);" id="listTitle6">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span>
			</td>
		</tr>
	</table>

	<div id="Wireless_5">
		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_1" onmouseout="sub_color_change_mouseout(1,6);" onmouseover="sub_color_change_mouseover(1,6);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless5g_basic.asp' target='main'>
						<span class="sublist" id="subl-ist-Title6_1" onClick="sub_list_click(1,6,4);">
							<script>dw(Basic)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_2" onmouseout="sub_color_change_mouseout(2,6);" onmouseover="sub_color_change_mouseover(2,6);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='guest_wireless5g_basic.asp' target='main'>
						<span class="sublist" onClick="sub_list_click(2,6,4);" id="subl-ist-Title6_2">
							<script>dw(Guest)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_3" onmouseout="sub_color_change_mouseout(3,6);" onmouseover="sub_color_change_mouseover(3,6);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless5g_wps.asp' target='main'>
						<span class="sublist" onClick="sub_list_click(3,6,4);" id="subl-ist-Title6_3">
							<script>dw(WPS)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list6_4" onmouseout="sub_color_change_mouseout(4,6);" onmouseover="sub_color_change_mouseover(4,6);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='wireless5g_schedule_v2.asp' target='main'>
						<span class="sublist" onClick="sub_list_click(4,6,4);" id="subl-ist-Title6_4">
							<script>dw(Schedule)</script>
						</span>
					</a>
				</td>
			</tr>
		</table>
	</div>

	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list7" onmouseout="color_change_mouseout(7);" onmouseover="color_change_mouseover(7);">
		<tr>
			<td nowrap="nowrap">
				<a href="fw_firewal.asp" target='main'>
					<img src="" class="arrowPointSize" border="0" name="Image7" id="Image7">&nbsp;
						<span class="mainlist" onClick="mouseclick(7);" id="listTitle7"><script>dw(Firewall)</script></span>
				</a>
			</td>
		</tr>
	</table>
						
	<div id="firewall_1">
		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_1" onmouseout="sub_color_change_mouseout(1,7);" onmouseover="sub_color_change_mouseover(1,7);">
			<tr >
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='fw_access.asp' target='main'>
						<span class="sublist" onClick="sub_list_click(1,7,3);" id="subl-ist-Title7_1"><script>dw(AccessControl)</script></span>
					</a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_2" onClick="sub_list_click(2,7,3);" onmouseout="sub_color_change_mouseout(2,7);" onmouseover="sub_color_change_mouseover(2,7);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap">
					<a href='fw_dmz.asp' target='main'><span class="sublist" id="subl-ist-Title7_2"><script>dw(DMZ)</script></span></a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list7_3" onClick="sub_list_click(3,7,3);" onmouseout="sub_color_change_mouseout(3,7);" onmouseover="sub_color_change_mouseover(3,7);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='fw_dos.asp' target='main'><span class="sublist" id="subl-ist-Title7_3"><script>dw(DoS)</script></span></a></td>
			</tr>
		</table>
	</div>

	<table width="100%" border="0" cellpadding="7" cellspacing="0"  class="cursor_pointer" id="list8" onmouseout="color_change_mouseout(8);" onmouseover="color_change_mouseover(8);">
		<tr>
			<td nowrap="nowrap" >
				<img src="" class="arrowPointSize" border="0" name="Image8" id="Image8">&nbsp;
				<span class="mainlist" onClick="mouseclick(8);" id="listTitle8"><script>dw(QoS)</script></span>
			</td>
		</tr>
	</table>

	<div id="QOS_1">
		<table width="100%" border="0" cellspacing="0" cellpadding="3"  id="list8_1" onmouseout="sub_color_change_mouseout(1,8);" onmouseover="sub_color_change_mouseover(1,8);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='qos.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(1,8,2);" id="subl-ist-Title8_1"><script>dw(QoS)</script></span></a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list8_2" onmouseout="sub_color_change_mouseout(2,8);" onmouseover="sub_color_change_mouseover(2,8);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='iqos.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(2,8,2);" id="subl-ist-Title8_2"><script>dw(iQoS)</script></span></a>
				</td>
			</tr>
		</table>

	</div>


	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list9" onmouseout="color_change_mouseout(9);" onmouseover="color_change_mouseover(9);">
		<tr>
			<td nowrap="nowrap" >
				<img src="" class="arrowPointSize" border="0" name="Image9" id="Image9">&nbsp;
					<span class="mainlist" onClick="mouseclick(9);" id="listTitle9"><script>dw(Advance)</script></span>
			</td>
		</tr>
	</table>

	<div id="Advance_1">
		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_1" onmouseout="sub_color_change_mouseout(1,9);" onmouseover="sub_color_change_mouseover(1,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_staticrout.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(1,9,8);" id="subl-ist-Title9_1"><script>dw(StaticRout)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_2" onmouseout="sub_color_change_mouseout(2,9);" onmouseover="sub_color_change_mouseover(2,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_portforward.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(2,9,8);" id="subl-ist-Title9_2"><script>dw(PortForwarding)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_3" onmouseout="sub_color_change_mouseout(3,9);" onmouseover="sub_color_change_mouseover(3,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_virtserver.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(3,9,8);" id="subl-ist-Title9_3"><script>dw(VirtualServer)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_4" onmouseout="sub_color_change_mouseout(4,9);" onmouseover="sub_color_change_mouseover(4,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_wireless.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(4,9,8);" id="subl-ist-Title9_4">2.4<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_5" onmouseout="sub_color_change_mouseout(5,9);" onmouseover="sub_color_change_mouseover(5,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_wireless5g.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(5,9,8);" id="subl-ist-Title9_5">5<script>dw(GHz)</script>&nbsp;<script>dw(Wireless)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_6" onmouseout="sub_color_change_mouseout(6,9);" onmouseover="sub_color_change_mouseover(6,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_alg.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(6,9,8);" id="subl-ist-Title9_6"><script>dw(ALG)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_7" onmouseout="sub_color_change_mouseout(7,9);" onmouseover="sub_color_change_mouseover(7,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_igmp.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(7,9,8);" id="subl-ist-Title9_7"><script>dw(IGMP)</script></span></a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list9_8" onmouseout="sub_color_change_mouseout(8,9);" onmouseover="sub_color_change_mouseover(8,9);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='adv_upnp.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(8,9,8);" id="subl-ist-Title9_8"><script>dw(UPnP)</script></span></a>
				</td>
			</tr>
		</table>
	</div>

	<table width="100%" border="0" cellpadding="7" cellspacing="0" class="cursor_pointer" id="list10" onmouseout="color_change_mouseout(10);" onmouseover="color_change_mouseover(10);">
		<tr>
			<td nowrap="nowrap" >
				<img src="" class="arrowPointSize" border="0" name="Image10" id="Image10">&nbsp;
					<span class="mainlist" onClick="mouseclick(10);" id="listTitle10"><script>dw(Administration)</script></span>
			</td>
		</tr>
	</table>						
	<div id="Administration_1">
		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_1" onmouseout="sub_color_change_mouseout(1,10);" onmouseover="sub_color_change_mouseover(1,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_timezone.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(1,10,9);" id="subl-ist-Title10_1"><script>dw(TimeZone)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_2" onmouseout="sub_color_change_mouseout(2,10);" onmouseover="sub_color_change_mouseover(2,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_password.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(2,10,9);" id="subl-ist-Title10_2"><script>dw(Password)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_3" onmouseout="sub_color_change_mouseout(3,10);" onmouseover="sub_color_change_mouseover(3,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td><a href='admin_remotmang.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(3,10,9);" id="subl-ist-Title10_3"><script>dw(RemoteAccess)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_4" onmouseout="sub_color_change_mouseout(4,10);" onmouseover="sub_color_change_mouseover(4,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_backrestore.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(4,10,9);" id="subl-ist-Title10_4"><script>dw(BackupRestore)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_5" onmouseout="sub_color_change_mouseout(5,10);" onmouseover="sub_color_change_mouseover(5,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap" id="IEBrowser"><a href='admin_upgradeIE.asp' target='main'><span class="sublist" onClick="sub_list_click(5,10,9);" id="subl-ist-Title10_5"><script>dw(Upgrade)</script></span></a>
				<td nowrap="nowrap" id="OtherBrowser"><a href='admin_upgrade.asp' target='main'><span class="sublist" onClick="sub_list_click(5,10,9);" id="subl-ist-Title10_5"><script>dw(Upgrade)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_6" onmouseout="sub_color_change_mouseout(6,10);" onmouseover="sub_color_change_mouseover(6,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_restart.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(6,10,9);" id="subl-ist-Title10_6"><script>dw(Restart)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_7" onmouseout="sub_color_change_mouseout(7,10);" onmouseover="sub_color_change_mouseover(7,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_logs.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(7,10,9);" id="subl-ist-Title10_7"><script>dw(Logs)</script></span></a>
				</td>
			</tr>
		</table>

		<table width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_8" onmouseout="sub_color_change_mouseout(8,10);" onmouseover="sub_color_change_mouseover(8,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_activeDhcpClient.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(8,10,9);" id="subl-ist-Title10_8"><script>dw(ActiveDHCPClient)</script></span></a>
				</td>
			</tr>
		</table>

		<table class="last_subitemBorder" width="100%" border="0" cellspacing="0" cellpadding="3" id="list10_9" onmouseout="sub_color_change_mouseout(9,10);" onmouseover="sub_color_change_mouseover(9,10);">
			<tr>
				<td nowrap="nowrap" width="15"></td>
				<td nowrap="nowrap"><a href='admin_statistics.asp' target='main'>
					<span class="sublist" onClick="sub_list_click(9,10,9);" id="subl-ist-Title10_9"><script>dw(Statistics)</script></span></a>	
				</td>
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
