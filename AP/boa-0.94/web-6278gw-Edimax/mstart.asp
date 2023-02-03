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
var WISP_enable5g = "<% getInfo("WISP_enable5g"); %>";
var WISP_enable2g = "<% getInfo("WISP_enable2g"); %>";

function goBack()
{
	if ((wizMode == 4) && (WISP_enable5g =="1"))
		window.location.assign("/wiz_WISP5gauto.asp");
	else if((wizMode == 4) && (WISP_enable2g =="1"))
		window.location.assign("/wiz_WISP24gauto.asp");
	else
		window.location.assign("/wiz_routerselect.asp");
}

function goNext()
{
	if(wizMode == 4){
		x=document.getElementById("switch_b")
		if(x.selectedIndex==0)
		window.location.assign("mdhcp.asp");
		else if(x.selectedIndex==1)
		window.location.replace("mstatic.asp");
		else if(x.selectedIndex==2)
		window.location.replace("mpppoe.asp");
	}
	else
	{
		x=document.getElementById("switch_a")
		if(x.selectedIndex==0)
			window.location.assign("mdhcp.asp");
		else if(x.selectedIndex==1)
			window.location.replace("mstatic.asp");
		else if(x.selectedIndex==2)
			window.location.replace("mpppoe.asp");
		else if(x.selectedIndex==3)
			window.location.replace("mpptp.asp");
		else if(x.selectedIndex==4)
			window.location.replace("ml2tp.asp");
	}
}

function switch_mode1( mode )
{
	var item_array = new Array 
	(
		document.getElementById("dynamicip_fieldset"),
		document.getElementById("staticip_fieldset"),
		document.getElementById("pppoe_fieldset"),
		document.getElementById("pptp_fieldset"),
		document.getElementById("l2tp_fieldset")
	)

	for(i=0; i<item_array.length; i++)
	{
		if(item_array[i])
			{
				if(i==mode)
					item_array[i].style.display="block";
				else
					item_array[i].style.display="none";
			}
	}
}

function switch_mode2( mode )
{
	var item_array = new Array 
	(
		document.getElementById("dynamicip_fieldset"),
		document.getElementById("staticip_fieldset"),
		document.getElementById("pppoe_fieldset")
	)

	for(i=0; i<item_array.length; i++)
	{
		if(item_array[i])
			{
				if(i==mode)
					item_array[i].style.display="block";
				else
					item_array[i].style.display="none";
			}
	}
}

function init()
{
	setTableWidth();

	if ((wizMode == 4)){
		document.getElementById('switch_1').style.display="none";
		document.getElementById('switch_2').style.display="block";
	}
	else{
		document.getElementById('switch_1').style.display="block";
		document.getElementById('switch_2').style.display="none";
	}

}
function setTableWidth()
{
	if(stype>13) stype=13;

	document.getElementById('tablewidth').className="tablewidth"+stype;
}
</script>
<style type="text/css">
.tablewidth0 {width:80%;}
.tablewidth1 {width:80%;}
.tablewidth2 {width:80%;}
.tablewidth3 {width:80%;}
.tablewidth4 {width:75%;}
.tablewidth5 {width:80%;}
.tablewidth6 {width:80%;}
.tablewidth7 {width:80%;}
.tablewidth8 {width:80%;}
.tablewidth9 {width:80%;}
.tablewidth10 {width:80%;}
.tablewidth11 {width:80%;}
.tablewidth12 {width:80%;}
.tablewidth13 {width:75%;}
</style>
</head>
<body onload="init();">
<form action="/goform/chkLink" method="POST" name="check">
<input type="hidden" value="1" name="back25in1">
<input type="hidden" value="/mstart.asp" name="submit-url">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" align="center" style="height:600px;width:800px" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" align="center" style="height:90px;width:700px">
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
	
	<table border="0" align="center" style="height:150px;" id="tablewidth" class="tablewidth0">
		<tr>
			<td style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px; line-height:35px;">
				<script>
					if(wizMode==4)
						dw(selectType_wisp)
					else
						dw(selectType)
				</script>
			</td>
		</tr>
	</table>

	<span id="switch_1" style="display:none">
		<table border="0" style="height:50px;width:350px" align="center">
			<tr>
				<td width="20%" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" valign="top" align="center">
					<select name="wantype" onChange="switch_mode1(this.value);" class="Arial_16" id="switch_a">
						<option value="0" <%getVar("wanMode","0","checked");%>><script>dw(DHCP)</script></option>
						<option value="1" <%getVar("wanMode","1","checked");%>><script>dw(staticIP)</script></option>
						<option value="2" <%getVar("wanMode","2","checked");%>><script>dw(PPPoE)</script></option>
						<option value="3" <%getVar("wanMode","3","checked");%>><script>dw(PPTP)</script></option>
						<option value="4" <%getVar("wanMode","6","checked");%>><script>dw(L2TP)</script></option>
					</select>
				</td>
			</tr>
		</table>
	</span>

	<span id="switch_2" style="display:none">
		<table border="0" align="center" style="height:50px;width:350px">
			<tr>
				<td width="20%" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;" valign="top" align="center">
					<select name="wantype" onChange="switch_mode2(this.value);" class="Arial_16" id="switch_b">
						<option value="0" <%getVar("wanMode","0","checked");%>><script>dw(DHCP)</script></option>
						<option value="1" <%getVar("wanMode","1","checked");%>><script>dw(staticIP)</script></option>
						<option value="2" <%getVar("wanMode","2","checked");%>><script>dw(PPPoE)</script></option>
					</select>
				</td>
			</tr>
		</table>
	</span>
	
	<table border="0" align="center" style="height:183px;width:780px">
		<tr><td></td></tr>
	</table>

	<table border="0" align="center" style="height:28px;width:780px">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
				</script>
			</td>
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');
				</script>
			</td>
		</tr>
	</table>

	<table border="0" align="center" style="height:60px;width:780px">
		<tr><td></td></tr>
	</table>
	
</td>
</tr>
</table>
</div>
</form>
</body>
</html>
