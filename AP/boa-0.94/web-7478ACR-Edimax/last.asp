<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script>
var pskValueTbl = <%getInfo("pskValueAll");%>;
var psk5GValueTbl = <%getiNICInfo("pskValueAll");%>;
var securityModeAll = <%getInfo("secModeAll");%>;
var securityModeAll5g = <%getiNICInfo("secModeAll");%>;
var wlanDisable = <%getiNICVar("wlanDisabled","","");%>;
var wlan24Disable = <%getVar("wlanDisabled","","");%>;
var wizMode = <%getVar("wizmode","","");%>;
var ConnectKey = <%getInfo("ConnectKey");%>; 
var ConnectKey5g = <%getInfo("ConnectKey5g");%>;
var isRestDefault = <%getVar("isRestDefault","","");%>;
var useCrossBand = "<%getInfo("useCrossBand");%>";

var repenable5g = "<%getInfo("rep_enable5g");%>";
var repenable2g = "<%getInfo("rep_enable2g");%>";

var Gband_ssidTbl = <%getInfo("ssidAll");%>;
var Aband_ssidTbl = <%getiNICInfo("ssidAll");%>;
var A_ssid =  Aband_ssidTbl[0];
var G_ssid =  Gband_ssidTbl[0];
if (G_ssid.search('"') != -1)   G_ssid = G_ssid.replace(/\"/g, "&quot;");
if (A_ssid.search('"') != -1)   A_ssid = A_ssid.replace(/\"/g, "&quot;");

G_ssid = G_ssid.replace(/\s/g, "&nbsp;");
A_ssid = A_ssid.replace(/\s/g, "&nbsp;");


i=0;secs=100;
function start()
{
	if (i==0)
		document.getElementById("in").innerHTML=i+"%";
	ba=setInterval("begin()",secs*10);
}
function begin()
{
	i+=1;
	if(i == 100)
	{
		init();
		document.getElementById("in").innerHTML="100%";
		document.getElementById('apply').style.display="none";
		document.getElementById('circle_table').style.display="none";
		document.getElementById('infotable').style.display="block";
		document.getElementById('setupId').style.display="block";

		if (wizMode != 3)
		{
			if ( (wlan24Disable == 0) && (wlanDisable == 0) ){
				document.getElementById('2G_info').style.display="block";
				document.getElementById('5G_info').style.display="block";
				document.getElementById('Gwtable').style.display="block";
			}
			else if(wlan24Disable == 0){	
				document.getElementById('2G_info').style.display="block";
				document.getElementById('5G_info').style.display="none";				
			}
			else{	
				document.getElementById('2G_info').style.display="none";
				document.getElementById('5G_info').style.display="block";
			}
		}
	}
	else if(i < 100)
	{
		document.getElementById("in").innerHTML=i+"%";
	}
}

function init()
{
	/* For show password */
	var pskval = showText(OpenSecurity);
	var pskval5g = showText(OpenSecurity);

	var display_flag1 = 0;
	var display_flag2 = 0;
	var display_flag3 = 0;
	var display_flag4 = 0;
	var display_flag5 = 0;
	var display_flag6 = 0;

	if(securityModeAll[0]!=0)
	{
		if((wizMode!=2) || (securityModeAll[0]==2)){
			pskval = pskValueTbl[0];
		}
		else{
			if(repenable2g =="1")
			  pskval = ConnectKey[0];//ConnectTestKey;
			else
			  pskval = pskValueTbl[0];//ConnectTestKey;
			
		}

		if (pskval.search('"') != -1)  
			pskval = pskval.replace(/\"/g, "&quot;");
	}

	if(securityModeAll5g[0]!=0)
	{
		if((wizMode!=2) || (securityModeAll5g[0]==2)){
			pskval5g = psk5GValueTbl[0];	
		}
		else{
			if(repenable5g =="1")
			  pskval5g = ConnectKey5g[0];//ConnectTestKey5g;
			else
			  pskval5g = psk5GValueTbl[0];
		}

		if (pskval5g.search('"') != -1)  
			pskval5g = pskval5g.replace(/\"/g, "&quot;");
	}

	if( (wlan24Disable == 0) && (wlanDisable == 0) ){
		if(pskval.length < 32 && pskval5g.length < 32)
			display_flag1 = 1;
		else if (pskval.length < 32 && pskval5g.length > 32)
			display_flag2 = 1;				
		else if (pskval.length > 32 && pskval5g.length < 32)
			display_flag3 = 1;
		else if (pskval.length > 32 && pskval5g.length > 32)
			display_flag4 = 1;
	}

	if( (wlan24Disable == 1) && (wlanDisable == 0) ){
		if(pskval5g.length < 32)
			display_flag5 = 1;
		else if(pskval5g.length > 32)
			display_flag6 = 1;
	}

	if( (wlan24Disable == 0) && (wlanDisable == 1) ){
		if(pskval.length < 32)
			display_flag5 = 1;
		else if(pskval.length > 32)
			display_flag6 = 1;
	}

	if(pskval.length > 32)
		pskval=pskval.substring(0,32)+'<br>'+pskval.substring(32,pskval.length);
	if(pskval5g.length > 32)
	pskval5g=pskval5g.substring(0,32)+'<br>'+pskval5g.substring(32,pskval5g.length);


	if(useCrossBand == "1"){
		document.getElementById('wirelesspassword').innerHTML = pskval5g;
		document.getElementById('wirelesspassword5g').innerHTML = pskval;
	}
	else{
		document.getElementById('wirelesspassword').innerHTML = pskval;
		document.getElementById('wirelesspassword5g').innerHTML = pskval5g;
	}

	if(display_flag1 == 1)
		document.getElementById("blcok_222px").style.display = "block";
	else if(display_flag2 == 1)
		document.getElementById("blcok_209px").style.display = "block";
	else if(display_flag3 == 1)
		document.getElementById("blcok_209px").style.display = "block";
	else if(display_flag4 == 1)
		document.getElementById("blcok_193px").style.display = "block";
	else if(display_flag5 == 1)
		document.getElementById("blcok_283px").style.display = "block";
	else if(display_flag6 == 1)
		document.getElementById("blcok_268px").style.display = "block";
	else
		document.getElementById("blcok_58px").style.display = "block";

	if(wizMode == 3){
		document.getElementById('wifibridge').style.display="block";
		document.getElementById('wifibridge1').style.display="block";
	}
}

function setTableWidth()
{
	if(isRestDefault == 1){
		window.top.location.assign("/index.asp");
	}
	else{
		languIndex=stype;
		if(stype>13) languIndex=13;

		document.getElementById('table1width').className="table1width0";
		document.getElementById('table2width').className="table2width"+languIndex;
		document.getElementById('table3width').className="table2width"+languIndex;

		document.getElementById('table24gLwidth').className="table24gLwidth"+languIndex;
		document.getElementById('table24gMwidth').className="table24gMwidth"+languIndex;
		document.getElementById('table24gRwidth').className="table24gRwidth"+languIndex;

		document.getElementById('table5gLwidth').className="table24gLwidth"+languIndex;
		document.getElementById('table5gMwidth').className="table24gMwidth"+languIndex;
		document.getElementById('table5gRwidth').className="table24gRwidth"+languIndex;
		
		document.getElementById('table24gL1width').className="table24gLwidth"+languIndex;
		document.getElementById('table24gM1width').className="table24gLwidth"+languIndex;

		document.getElementById('table5gL1width').className="table24gLwidth"+languIndex;
		document.getElementById('table5gM1width').className="table24gLwidth"+languIndex;
	}
}
</script>
<style type="text/css">
.table1width0 {width:90%;}

.table2width0 {width:90%;}
.table2width1 {width:90%;}
.table2width2 {width:90%;}
.table2width3 {width:90%;}
.table2width4 {width:90%;}
.table2width5 {width:90%;}
.table2width6 {width:90%;}
.table2width7 {width:90%;}
.table2width8 {width:90%;}
.table2width9 {width:90%;}
.table2width10 {width:90%;}
.table2width11 {width:90%;}
.table2width12 {width:90%;}
.table2width13 {width:90%;}

.table24gLwidth0 {width:15%;}
.table24gLwidth1 {width:15%;}
.table24gLwidth2 {width:15%;}
.table24gLwidth3 {width:15%;}
.table24gLwidth4 {width:15%;}
.table24gLwidth5 {width:15%;}
.table24gLwidth6 {width:15%;}
.table24gLwidth7 {width:15%;}
.table24gLwidth8 {width:15%;}
.table24gLwidth9 {width:15%;}
.table24gLwidth10 {width:15%;}
.table24gLwidth11 {width:15%;}
.table24gLwidth12 {width:28%;}
.table24gLwidth13 {width:28%;}

.table24gMwidth0 {width:20%;}
.table24gMwidth1 {width:20%;}
.table24gMwidth2 {width:20%;}
.table24gMwidth3 {width:20%;}
.table24gMwidth4 {width:20%;}
.table24gMwidth5 {width:20%;}
.table24gMwidth6 {width:20%;}
.table24gMwidth7 {width:20%;}
.table24gMwidth8 {width:20%;}
.table24gMwidth9 {width:20%;}
.table24gMwidth10 {width:20%;}
.table24gMwidth11 {width:20%;}
.table24gMwidth12 {width:25%;}
.table24gMwidth13 {width:25%;}

.table24gRwidth0 {width:20%;}
.table24gRwidth1 {width:20%;}
.table24gRwidth2 {width:20%;}
.table24gRwidth3 {width:20%;}
.table24gRwidth4 {width:20%;}
.table24gRwidth5 {width:20%;}
.table24gRwidth6 {width:20%;}
.table24gRwidth7 {width:20%;}
.table24gRwidth8 {width:20%;}
.table24gRwidth9 {width:20%;}
.table24gRwidth10 {width:20%;}
.table24gRwidth11 {width:20%;}
.table24gRwidth12 {width:55%;}
.table24gRwidth13 {width:55%;}
</style>
</head>
<body onload="setTableWidth(); start()">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
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

	<span id="wifibridge" style="display:none">
	<table border="0" style="height:60px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	</span>

	<span id="setupId" style="display:none">
		<table border="0" style="height:0px;" align="center" id="table2width" class="table2width0">
			<tr>
				<td align="center" class="Arial_20" style="font-weight:bold;"><b><script>dw(last_1)</script></b>
			</tr>
		</table>

		<table border="0" style="height:120px;width:750px" align="center" id="table3width" class="table2width0">
			<tr>
				<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:25px;">
					<script>
						if(wizMode == 3)
							dw(APcl_lastinfo)
						else
							dw(router_ap_rep_lastinfo)
					</script>
			</tr>
		</table>
	</span>

	<span id="wifibridge1" style="display:none">
	<table border="0" style="height:30px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
	</span>

		<!--===== infotable =====-->
	<span id="infotable" style="display:none">
		<table border="0" style="height:15px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<span id="apply" style="display:block">
		<table border="0" style="height:182px;" id="table1width" class="table1width0" align="center">
			<tr>
				<td align="center"><span class="Arial_20" style="font-weight:bold;"><script>dw(Applyingyoursettings)</script></span></td>
			</tr>
			<tr><td align="center" id="in" class="Arial_20" style="font-weight:bold;"></td></tr>
			<tr><td align="center"><img src="graphics/loading.gif" /></td></tr>
		</table>
		<table border="0" style="height:100px;width:760px" align="center">
			<tr><td></td></tr>
		</table>
	</span>

	<span id="circle_table" style="display:block">
		<table border="0" style="height:135px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<span id="2G_info" style="display:none">
		<table border="0" align="center" style="height:50px;width:700px;">
			<tr>
				<td align="right" id="table24gLwidth" class="table24gLwidth0">(<script>dw(Gband)</script>)&nbsp;</td>
				<td align="left" id="table24gMwidth" class="table24gMwidth0"><script>dw(wirelessname2)</script>&nbsp;:&nbsp;</td>
				<td align="left" id="table24gRwidth" class="table24gRwidth0">
					<script>		
						if(wizMode == 2 && useCrossBand == "1") document.write(A_ssid);
						else document.write(G_ssid);
					</script>
				</td>
			</tr>

			<tr>
				<td align="left" id="table24gL1width" class="table24gLwidth0"></td>
				<td align="left" id="table24gM1width" class="table24gMwidth0"><script>dw(wirelesspassword)</script>&nbsp;:&nbsp;</td>
				<td id="wirelesspassword" align="left"></td>
			</tr>
		</table>
	</span>

	<span id="Gwtable" style="display:none">
		<table border="0" style="height:10px;width:780px;" align="center"><tr><td></td></tr></table>
	</span>

	<span id="5G_info" style="display:none">
		<table border="0" align="center" style="height:50px;width:700px;">
			<tr>
				<td align="right" id="table5gLwidth" class="table24gLwidth0">(<script>dw(Nband)</script>)&nbsp;</td>
				<td align="left" id="table5gMwidth" class="table24gMwidth0"><script>dw(wirelessname2)</script>&nbsp;:&nbsp;</td>
				<td align="left" id="table5gRwidth" class="table24gRwidth0">
					<script>
						if(wizMode == 2 && useCrossBand == "1") document.write(G_ssid);
						else document.write(A_ssid);
					</script>
				</td>
			</tr>

			<tr>
				<td align="left" id="table5gL1width" class="table24gLwidth0"></td>
				<td align="left" id="table5gM1width" class="table24gMwidth0"><script>dw(wirelesspassword)</script>&nbsp;:&nbsp;</td>
				<td id="wirelesspassword5g" align="left"></td>
			</tr>
		</table>
	</span>

	<!--===== 5g on; 24g on; psk<32 and psk5g<32;=====-->
	<span id="blcok_222px" style="display:none">
		<table border="0" style="height:222px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<!--===== 5g on; 24g on; psk<32 and psk5g>32; psk>32 and psk5g<32;=====-->
	<span id="blcok_209px" style="display:none">
		<table border="0" style="height:209px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<!--===== 5g on; 24g on; psk>32 and psk5g>32;=====-->
	<span id="blcok_193px" style="display:none">
		<table border="0" style="height:193px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

  <!--===== 24g on or 5g on; psk<32 or 5g<32;=====-->
	<span id="blcok_283px" style="display:none">
		<table border="0" style="height:283px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<!--===== 24g on or 5g on; psk>32 or 5g>32;=====-->
	<span id="blcok_268px" style="display:none">
		<table border="0" style="height:268px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

	<!--===== else =====-->
	<span id="blcok_58px" style="display:none">
		<table border="0" style="height:58px;width:780px" align="center"><tr><td></td></tr></table>
	</span>

</td>
</tr>
</table>
</div>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
