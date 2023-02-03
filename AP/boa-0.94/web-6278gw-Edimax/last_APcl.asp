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
var APcl_enable5g = "<% getInfo("APcl_enable5g"); %>";
var APcl_enable2g = "<% getInfo("APcl_enable2g"); %>";

var apcli0IP="<% getInfo("apcli0IP"); %>";
var apclii0IP="<% getInfo("apclii0IP"); %>";

var pskValueTbl =	<%getInfo("pskValueAll");%>;
var psk5GValueTbl =	<%getiNICInfo("pskValueAll");%>;

var dhcpswitch = <%getVar("dhcpswitch","","");%>;
var dhcpswitch5g = <%getVar("dhcpswitch-5g","","");%>;

var Ip = "<% getInfo("ip-rom"); %>";
var Ip5G = "<% getiNICInfo("ip-rom-5g"); %>";

var securityModeAll = <%getInfo("secModeAll");%>;
var securityModeAll5g =	 <%getiNICInfo("secModeAll");%>;

var ConnectKey =	<%getInfo("ConnectKey");%>; 
var ConnectKey5g =	<%getInfo("ConnectKey5g");%>;

var isRestDefault = <%getVar("isRestDefault","","");%>;

i=0;secs=90;
function start()
{
	if (i==0)
		document.getElementById("in").innerHTML=i+"%";
	ba=setInterval("begin()",secs*10);
}
function begin()
{
	i+=1;
	if(i>=100)
	{
		document.getElementById("in").innerHTML="100%";
		document.getElementById('apply').style.display="none";
		document.getElementById('setupId').style.display="block";
		document.getElementById('2G_5Gtable').style.display="block";
		document.getElementById('APcl_info').style.display="block";
	
		if ((APcl_enable5g==1) && (APcl_enable2g==0)){

			var pskval5g = showText(OpenSecurity);

			if(securityModeAll5g[0]==2)
				pskval5g = psk5GValueTbl[0];
			else
				pskval5g = ConnectKey5g[0];//ConnectTestKey5g;

			if(pskval5g.length > 32){
				pskval5g=pskval5g.substring(0,32)+'<br>'+pskval5g.substring(32,pskval5g.length);
			}

			document.getElementById('wirelesspassword5g').innerHTML = pskval5g;
		}
		else if ((APcl_enable5g==0) && (APcl_enable2g==1)) {
			var pskval = showText(OpenSecurity);

			if(securityModeAll[0]==2)
				pskval = pskValueTbl[0];
			else
				pskval = ConnectKey[0];//ConnectTestKey;
			
			if(pskval.length > 32){
				pskval=pskval.substring(0,32)+'<br>'+pskval.substring(32,pskval.length);
			}

			document.getElementById('wirelesspassword').innerHTML = pskval;
		}
	}
	else
	{
		document.getElementById("in").innerHTML=i+"%";
	}
}

function setTableWidth()
{
	if(isRestDefault == 1){
		window.top.location.assign("/index.asp");
	}
	else{
		if(stype>13) stype=13;

		document.all.tablewidth.className="tablewidth"+stype;
		document.all.table1width.className="tablewidth"+stype;

		document.all.tableLwidth.className="TypeLtable"+stype;
		document.all.tableMwidth.className="TypeMtable"+stype;
		document.all.tableRwidth.className="TypeRtable"+stype;

		document.all.tableL1width.className="TypeLtable"+stype;
		document.all.tableM1width.className="TypeMtable"+stype;
		
		document.all.tableL2width.className="TypeLtable"+stype;
		document.all.tableM2width.className="TypeMtable"+stype;
	}
}
</script>

<style type="text/css">
.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:90%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:90%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:90%;}

.TypeLtable0 {width:30%;}
.TypeLtable1 {width:22%;}
.TypeLtable2 {width:22%;}
.TypeLtable3 {width:22%;}
.TypeLtable4 {width:22%;}
.TypeLtable5 {width:22%;}
.TypeLtable6 {width:22%;}
.TypeLtable7 {width:18%;}
.TypeLtable8 {width:13%;}
.TypeLtable9 {width:22%;}
.TypeLtable10 {width:22%;}
.TypeLtable11 {width:22%;}
.TypeLtable12 {width:30%;}
.TypeLtable13 {width:30%;}

.TypeMtable0 {width:26%;}
.TypeMtable1 {width:15%;}
.TypeMtable2 {width:15%;}
.TypeMtable3 {width:15%;}
.TypeMtable4 {width:15%;}
.TypeMtable5 {width:15%;}
.TypeMtable6 {width:15%;}
.TypeMtable7 {width:21%;}
.TypeMtable8 {width:31%;}
.TypeMtable9 {width:15%;}
.TypeMtable10 {width:15%;}
.TypeMtable11 {width:15%;}
.TypeMtable12 {width:26%;}
.TypeMtable13 {width:26%;}

.TypeRtable0 {width:48%;}
.TypeRtable1 {width:29%;}
.TypeRtable2 {width:29%;}
.TypeRtable3 {width:29%;}
.TypeRtable4 {width:29%;}
.TypeRtable5 {width:29%;}
.TypeRtable6 {width:29%;}
.TypeRtable7 {width:26%;}
.TypeRtable8 {width:21%;}
.TypeRtable9 {width:29%;}
.TypeRtable10 {width:29%;}
.TypeRtable11 {width:29%;}
.TypeRtable12 {width:48%;}
.TypeRtable13 {width:48%;}

</style>
</head>
<body onload="setTableWidth(); start()">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
		<tr><td align="right" class="Arial_16"><script>dw(APcl);</script></td></tr>
	</table>

	
	<span id="setupId" style="display:none">
		<table border="0" style="height:0px;" id="tablewidth" class="table2width0" align="center">
			<tr>
				<td align="center" class="Arial_20" style="font-weight:bold;"><b><script>dw(last_1)</script></b>
			</tr>
		</table>

		<table border="0" style="height:50px;" id="table1width" class="table2width0" align="center">
			<tr>
				<td align="left" style="font-size:16px;font-family:Calibri,Arial,Microsoft JhengHei; line-height:25px;"><script>dw(last_APclinfo)</script>
			</tr>
		</table>
	</span>

	<span id="APcl_info" style="display:none">
		<table border="0" style="height:100px;width:700px" align="center">
			<tr>
				<td align="right" id="tableLwidth" class="tableLwidth0"></td>
				<td align="left" id="tableMwidth" class="tableMwidth0">
					<script>
						dw(IPAddress);
						document.write('&nbsp;:&nbsp;');
					</script>
				</td>	
				<td align="left" id="tableRwidth" class="tableRwidth0">
					<script>
						if((APcl_enable5g==1) && (APcl_enable2g==0)){
							if(dhcpswitch5g==0){
								document.write('<span>'+Ip5G+'</span>');
							}
							else{
								if(apclii0IP != "") 
									document.write('<span>'+apclii0IP+'</span>');
								else 
									document.write('<span>'+showText(getIPfail)+'</span>');
							}
						}
						else if((APcl_enable5g==0) && (APcl_enable2g==1)){
							if(dhcpswitch==0){
								document.write('<span>'+Ip+'</span>');
							}
							else{
								if(apcli0IP != "") 
									document.write('<span>'+apcli0IP+'</span>');
								else 
									document.write('<span>'+showText(getIPfail)+'</span>');
							}
						}
					</script>
				</td>
			</tr>
			
			<tr>
				<td align="right" id="tableL1width" class="TypeLtable0">
					(<script>
						if((APcl_enable5g==1) && (APcl_enable2g==0)){
							document.write(''+showText(Nband)+'');
						}
						else if ((APcl_enable5g==0) && (APcl_enable2g==1)){
							document.write(''+showText(Gband)+'');			
						}
					 </script>)&nbsp;
				</td>
				<td align="left" id="tableM1width" class="TypeMtable0">
					<script>
						dw(wirelessname5)
					</script>&nbsp;:&nbsp;
				</td>
					<script>
						var repeaterSSID5g ="<%getiNICInfo("repeaterSSID");%>";
						var repeaterSSID ="<%getInfo("repeaterSSID");%>";
							
						if (repeaterSSID5g.search('"') != -1)  
						repeaterSSID5g = repeaterSSID5g.replace(/\"/g, "&quot;");

						if (repeaterSSID.search('"') != -1)  
						repeaterSSID = repeaterSSID.replace(/\"/g, "&quot;");
						repeaterSSID5g = repeaterSSID5g.replace(/\s/g, "&nbsp;");
						repeaterSSID = repeaterSSID.replace(/\s/g, "&nbsp;");

						if ((APcl_enable5g==1) && (APcl_enable2g==0)){
							document.write('<td align="left" id="tableR1width" class="TypeMtable0"><span>'+repeaterSSID5g+'</span>');
							document.write('</td>');
						}
						else if ((APcl_enable5g==0) && (APcl_enable2g==1)){
							document.write('<td align="left" id="tableR1width" class="TypeMtable0"><span>'+repeaterSSID+'</span>');
							document.write('</td>');
						}
					</script>
			</tr>
		
			<tr>
				<td align="right" id="tableL2width" class="tableLwidth0"></td>
				<td align="left" id="tableM2width" class="TypeMtable0">
					<script>
						dw(wirelesspassword);
					</script>&nbsp;:&nbsp;
				</td>
					<script>
						if ((APcl_enable5g==1) && (APcl_enable2g==0)){
							document.write('<td id="wirelesspassword5g" align="left">');
							document.write('</td>');
						}
						else if ((APcl_enable5g==0) && (APcl_enable2g==1)){
							document.write('<td id="wirelesspassword" align="left">');
							document.write('</td>');
						}
					</script>
			</tr>
		</table>
	</span>

	<span id="2G_5Gtable" style="display:none">
		<table border="0" style="height:258px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>
	
	<span id="apply" style="display:block">
		<table border="0" style="height:177px;width:780px" align="center">
			<tr>
				<td align="center"><span class="Arial_20" style="font-weight:bold;"><script>dw(Applyingyoursettings)</script></span></td>
			</tr>
			<tr><td align="center" id="in" class="Arial_20" style="font-weight:bold;"></td></tr>
			<tr><td align="center"><img src="graphics/loading.gif" /></td></tr>
		</table>
		<table border="0" style="height:258px;width:780px" align="center">
			<tr><td></td></tr>
		</table>
	</span>

	<table border="0" style="height:30px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

</td>
</tr>
</table>

</div>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
