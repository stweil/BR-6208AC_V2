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
var rep_enable5g = "<% getInfo("rep_enable5g"); %>";
var rep_enable2g = "<% getInfo("rep_enable2g"); %>";
var Ip="<% getInfo("ip-rom"); %>";
var Mask="<% getInfo("mask-rom"); %>";
var Gateway="<% getInfo("gateway-rom"); %>";
var DNS1="<% getInfo("wan-dns1"); %>"
var apcli0IP="<% getInfo("apcli0IP"); %>";
var wizMode = <%getVar("wizmode","","");%>;
var useAutoIP = <%getVar("dhcpswitch","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var showConnectResult = <%getVar("showConnectResult2","","");%>;
var namevalue="";
var indexvalue=0;

function autoIp_Clicked() {
	
	if(Ip != "")
	{
		document.tcpip.ip0.value=Ip.split(".")[0];
		document.tcpip.ip1.value=Ip.split(".")[1];
		document.tcpip.ip2.value=Ip.split(".")[2];
		document.tcpip.ip3.value=Ip.split(".")[3];
	}

	if(Mask!="")
	{
		document.tcpip.mask0.value=Mask.split(".")[0];
		document.tcpip.mask1.value=Mask.split(".")[1];
		document.tcpip.mask2.value=Mask.split(".")[2];
		document.tcpip.mask3.value=Mask.split(".")[3];
	}
	if(Gateway!="")
	{
		document.tcpip.gateway0.value=Gateway.split(".")[0];
		document.tcpip.gateway1.value=Gateway.split(".")[1];
		document.tcpip.gateway2.value=Gateway.split(".")[2];
		document.tcpip.gateway3.value=Gateway.split(".")[3];
	}

	if(DNS1 != "")
	{
		document.tcpip.Dns0.value=DNS1.split(".")[0];
		document.tcpip.Dns1.value=DNS1.split(".")[1];
		document.tcpip.Dns2.value=DNS1.split(".")[2];
		document.tcpip.Dns3.value=DNS1.split(".")[3];
	}

	if (document.tcpip.ipmode[0].checked==true) {
		document.tcpip.dnsMode.value="dnsAuto";
		document.tcpip.ip0.disabled = true;
		document.tcpip.ip1.disabled = true;
		document.tcpip.ip2.disabled = true;
		document.tcpip.ip3.disabled = true;

		document.tcpip.mask0.disabled = true;
		document.tcpip.mask1.disabled = true;
		document.tcpip.mask2.disabled = true;
		document.tcpip.mask3.disabled = true;

		document.tcpip.gateway0.disabled = true;
		document.tcpip.gateway1.disabled = true;
		document.tcpip.gateway2.disabled = true;
		document.tcpip.gateway3.disabled = true;
		
		document.tcpip.Dns0.disabled = true;
		document.tcpip.Dns1.disabled = true;
		document.tcpip.Dns2.disabled = true;
		document.tcpip.Dns3.disabled = true;

		if ((wizMode==2) || (wizMode==3) || (wizMode==4)){
			if((apcli0IP==""))
				document.tcpip.NextButton.disabled = true;
		}
	}
	else {
		document.tcpip.dnsMode.value="dnsManual";
	
		document.tcpip.ip0.disabled = false;
		document.tcpip.ip1.disabled = false;
		document.tcpip.ip2.disabled = false;
		document.tcpip.ip3.disabled = false;

		document.tcpip.mask0.disabled = false;
		document.tcpip.mask1.disabled = false;
		document.tcpip.mask2.disabled = false;
		document.tcpip.mask3.disabled = false;

		document.tcpip.gateway0.disabled = false;
		document.tcpip.gateway1.disabled = false;
		document.tcpip.gateway2.disabled = false;
		document.tcpip.gateway3.disabled = false;

		document.tcpip.Dns0.disabled = false;
		document.tcpip.Dns1.disabled = false;
		document.tcpip.Dns2.disabled = false;
		document.tcpip.Dns3.disabled = false;

		if ((wizMode==2) || (wizMode==3) || (wizMode==4)){
			document.tcpip.NextButton.disabled = false;
		}
	}
}


function goBack()
{
	if(wizMode==1)
		document.ui_redirect.go2gSitesuvy.value="/wiz_apmode.asp";
	else if(wizMode == 3)
		document.ui_redirect.go2gSitesuvy.value="/wiz_APcl24gauto.asp";
	else
		document.ui_redirect.go2gSitesuvy.value="/wiz_rep24gauto.asp";
	
	document.ui_redirect.submit();
}

function goNext()
{
	if (document.tcpip.ipmode[1].checked==true) 
	{
		strIp = document.tcpip.ip0.value+"."+
						document.tcpip.ip1.value+"."+
						document.tcpip.ip2.value+"."+
						document.tcpip.ip3.value;

		strMask = document.tcpip.mask0.value+"."+
							document.tcpip.mask1.value+"."+
							document.tcpip.mask2.value+"."+
							document.tcpip.mask3.value;

		strGateway = document.tcpip.gateway0.value+"."+
								 document.tcpip.gateway1.value+"."+
								 document.tcpip.gateway2.value+"."+
								 document.tcpip.gateway3.value;

		strDNS1 = document.tcpip.Dns0.value+"."+
							document.tcpip.Dns1.value+"."+
							document.tcpip.Dns2.value+"."+
							document.tcpip.Dns3.value;

		if (!checkip(0)) return false;
		if (!checkip(1)) return false;
		if (!checkip(2)) return false;
		if (!checkip(3)) return false;

		document.tcpip.ip.value=strIp;
		document.tcpip.mask.value=strMask;
		document.tcpip.gateway.value=strGateway;
		document.tcpip.dns1.value=strDNS1;
	}

	if(wizMode==1)
		document.tcpip.submiturl.value="/wifi.asp";
	else{
		if(wizMode==2){
			if(rep_enable5g == "1")
				document.tcpip.submiturl.value="/wiz_rep5gauto.asp";
			else
				document.tcpip.submiturl.value="/conclusion.asp";
		}
		else
			document.tcpip.submiturl.value="/conclusion.asp";
	}

	if(wizMode==2)
		document.tcpip.SetDyip.value="YES";

	document.tcpip.submit();
}

function checkip(index)
{
	document.tcpip.ip.value = document.tcpip.ip0.value+"."+
														document.tcpip.ip1.value+"."+
														document.tcpip.ip2.value+"."+
														document.tcpip.ip3.value;

	document.tcpip.mask.value = document.tcpip.mask0.value+"."+
															document.tcpip.mask1.value+"."+
															document.tcpip.mask2.value+"."+
															document.tcpip.mask3.value;

	document.tcpip.gateway.value = document.tcpip.gateway0.value+"."+
																 document.tcpip.gateway1.value+"."+
																 document.tcpip.gateway2.value+"."+
																 document.tcpip.gateway3.value;

	document.tcpip.dns1.value = document.tcpip.Dns0.value+"."+
																 document.tcpip.Dns1.value+"."+
																 document.tcpip.Dns2.value+"."+
																 document.tcpip.Dns3.value;
 

  with(document.tcpip) 
	{
		switch(index)
		{
      case 0:
				if ( !ipRule( document.tcpip.ip, showText(IPAddress), "ip", 1)) return false;
				else return true;
			break;
			case 1:
				if ( !maskRule(document.tcpip.mask, showText(SubnetMask), 1)) return false;
				else return true;
			break;
   		case 2:
				if(document.tcpip.gateway.value == document.tcpip.ip.value){
					alert(showText(InvalidSomething).replace(/#####/, showText(defaultGateway)) + ' ' + showText(NotSameAsLAN));
					return false;
				}
				if ( !ipRule( document.tcpip.gateway, showText(defaultGateway), "gw", 1)) return false;
				else if ( !ipRuleAND( document.tcpip.ip, document.tcpip.mask, document.tcpip.gateway, showText(defaultGatewayWarning), "gw", 1)) return false;
				else return true;
			break;
			case 3:
				if ( !ipRule( document.tcpip.dns1, showText(dnsAddr), "ip", 1)) return false;
				else return true;
			break;
	
			default: 
			alert("error!");
			return false;
			break;
	 }
 }

}

function setTableWidth()
{
	if(stype>13) stype=13;

	//document.all.ipselect.className="modewidth"+stype;
	document.all.iptableLeft.className="leftwidth"+stype;
	document.all.iptable1Left.className="leftwidth"+stype;
	document.all.iptable2Left.className="leftwidth"+stype;
	document.all.iptable3Left.className="leftwidth"+stype;
	document.all.iptabmwidth.className="iptabmwidth"+stype;
	document.all.iptabm1width.className="iptabmwidth"+stype;
	document.all.iptabm2width.className="iptabmwidth"+stype;
	document.all.iptabm3width.className="iptabmwidth"+stype;
	document.all.iptableright.className="rightwidth"+stype;
	document.all.iptable1right.className="rightwidth"+stype;
	document.all.iptable2right.className="rightwidth"+stype;
	document.all.iptable3right.className="rightwidth"+stype;

	document.all.iptable.className="tablewidth0";
}

function tap2next(event)
{
	var evt = event ? event : (window.event ? window.event : null);

	if ( evt.keyCode == 110 )
	{	
		document.getElementById(''+namevalue+''+indexvalue+'').focus();
		document.getElementById(''+namevalue+''+indexvalue+'').select();
		return false;
	}
	document.onkeydown = tap2next;
}

function setvalue(name,index)
{
	namevalue=name;
	indexvalue=index;
}

function init()
{
	if ((wizMode==2) || (wizMode==3) || (wizMode==4)){
		if( (apcli0IP=="") )
			document.tcpip.NextButton.disabled = true;
	}
}

</script>

<style type="text/css">

.tablewidth0 {width:80%;}

.leftwidth0 {width:22%;}
.leftwidth1 {width:22%;}
.leftwidth2 {width:22%;}
.leftwidth3 {width:22%;}
.leftwidth4 {width:22%;}
.leftwidth5 {width:22%;}
.leftwidth6 {width:22%;}
.leftwidth7 {width:22%;}
.leftwidth8 {width:22%;}
.leftwidth9 {width:22%;}
.leftwidth10 {width:22%;}
.leftwidth11 {width:22%;}
.leftwidth12 {width:22%;}
.leftwidth13 {width:22%;}

.iptabmwidth0 {width:20%;}
.iptabmwidth1 {width:25%;}
.iptabmwidth2 {width:25%;}
.iptabmwidth3 {width:25%;}
.iptabmwidth4 {width:25%;}
.iptabmwidth5 {width:25%;}
.iptabmwidth6 {width:25%;}
.iptabmwidth7 {width:20%;}
.iptabmwidth8 {width:20%;}
.iptabmwidth9 {width:25%;}
.iptabmwidth10 {width:20%;}
.iptabmwidth11 {width:20%;}
.iptabmwidth12 {width:20%;}
.iptabmwidth13 {width:20%;}

.rightwidth0 {width:67%;}
.rightwidth1 {width:52%;}
.rightwidth2 {width:52%;}
.rightwidth3 {width:52%;}
.rightwidth4 {width:52%;}
.rightwidth5 {width:58%;}
.rightwidth6 {width:58%;}
.rightwidth7 {width:57%;}
.rightwidth8 {width:59%;}
.rightwidth9 {width:58%;}
.rightwidth10 {width:57%;}
.rightwidth11 {width:59%;}
.rightwidth12 {width:67%;}
.rightwidth13 {width:67%;}
</style>
</head>
<body onLoad="setTableWidth()">
<form action="/goform/wiz_5in1_redirect" method="POST" name="ui_redirect">
<input type="hidden" value="" name="go2gSitesuvy">
</form>
<form action="/goform/formTcpipSetup" method="POST" name="tcpip">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<input type="hidden" name="clIpDisabled" value="0">
<input type="hidden" value="" name="gateway">
<input type="hidden" value="" name="ip">
<input type="hidden" name="ipChanged" value="">
<input type="hidden" value="" name="mask">
<input type="hidden" value="" name="dns1">
<input type="hidden" value="dnsAuto" name="dnsMode">
<input type="hidden" value="OK" name="goToWifi">
<input type="hidden" value="" name="submiturl">
<input type="hidden" value="0" name="dhcp">
<input type="hidden" value="" name="SetDyip">

<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">

	<table border="0" style="height:90px;width:700px" align="center">
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


	<table border="0" style="height:85px;" id="tablewidth" class="tablewidth0" align="center">
		<tr>
				<script>
					if(wizMode==2){
						if(apcli0IP == "")
							document.write('<td align="left" class="Arial_16" style="line-height:23px;">'+showText(get24GIPfail));
						else
							document.write('<td align="center" class="Arial_16" style="line-height:23px;">'+showText(get24GIPok));
					}
					else if(wizMode==3){
						if(apcli0IP == "")
							document.write('<td align="left" class="Arial_16" style="line-height:23px;">'+showText(APclget24GIPfail));
						else
							document.write('<td align="center" class="Arial_16" style="line-height:23px;">'+showText(APclget24GIPok));
					}
					else
						document.write('<td align="center" class="Arial_16" style="line-height:25px;">'+showText(settheIPaddress));
				</script>
		</tr>
	</table>


	<table border="0" style="height:100px;width:400px;" align="center">
		<tr>
			<td align="left" class="Arial_16">
			<input type="radio" value="1" name="ipmode" onClick="autoIp_Clicked()">
			<script>dw(ObtainIpauto)
				if((wizMode==2) || (wizMode==3))
				{
					document.write('<br>');
					if(apcli0IP != "") document.write('<span class="iQsubText">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(IP&nbsp;:&nbsp;'+apcli0IP+')</span>');
					else document.write('<span class="iQsubText">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;('+showText(getIPfail)+')</span>');
				}
			</script>
			</td>
		</tr>
		<tr>
			<td align="left" class="Arial_16">
			<input type="radio" value="0" name="ipmode" onClick="autoIp_Clicked()">
			<script>dw(FollowIp)</script>
			</td>
		</tr>
	</table>
	<span id="ipsetting" style="display:block">
	
	<table border="0" style="height:100px;width:700px;" id="iptable" align="center">
		<tr>
			<td id="iptableLeft" class="leftwidth0" align="left">
			<td id="iptabmwidth" class="iptabmwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(IPAddress)</script>&nbsp;:&nbsp;</td>
			<td id="iptableright" class="rightwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;">&nbsp;
			<input type="text" class="Arial_16" name="ip0" id="ip0" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('ip',1);tap2next(event);">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="ip1" id="ip1" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('ip',2);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="ip2" id="ip2" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('ip',3);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="ip3" id="ip3" size="3" maxlength="3" value="0">
			</td>	
		</tr>
		<tr>
			<td id="iptable1Left" class="leftwidth0" align="left">
			<td id="iptabm1width" class="iptabmwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(SubnetMask)</script>&nbsp;:&nbsp;</td>
			<td id="iptable1right" align="left" class="rightwidth0" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;">&nbsp;
			
			<input type="text" class="Arial_16" name="mask0" id="mask0" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('mask',1);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="mask1" id="mask1" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('mask',2);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="mask2" id="mask2" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('mask',3);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="mask3" id="mask3" size="3" maxlength="3" value="0">
			</td>
		</tr>
		<tr>
			<td id="iptable2Left" class="leftwidth0" align="left">
			<td id="iptabm2width" class="iptabmwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(defaultGateway)</script>&nbsp;:&nbsp;</td>
			<td id="iptable2right" align="left" class="rightwidth0" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;">&nbsp;
			<input type="text" class="Arial_16" name="gateway0" id="gateway0" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('gateway',1);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="gateway1" id="gateway1" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('gateway',2);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="gateway2" id="gateway2" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('gateway',3);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="gateway3" id="gateway3" size="3" maxlength="3" value="0">
			</td>
		</tr>
		<tr>
			<td id="iptable3Left" class="leftwidth0" align="left">
			<td id="iptabm3width" class="iptabmwidth0" align="left" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:16px;"><script>dw(dnsAddr3)</script>&nbsp;:&nbsp;
			</td>
			<td id="iptable3right" align="left" class="rightwidth0" style="font-family:Calibri,Arial,Microsoft JhengHei;font-size:14px;">&nbsp;
			<input type="text" class="Arial_16" name="Dns0" id="Dns0" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('Dns',1);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="Dns1" id="Dns1" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('Dns',2);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="Dns2" id="Dns2" size="3" maxlength="3" value="0" onkeydown="javascript:setvalue('Dns',3);tap2next(event)">&nbsp;.&nbsp;
			<input type="text" class="Arial_16" name="Dns3" id="Dns3" size="3" maxlength="3" value="0" >
			</td>
		</tr>
	</table>
	</span>

	<span id="nosetting" style="display:none">
		<table border="0" style="height:100px;width:500px" align="center">
			<tr><td width="40%" align="right" class="Arial_16"></td></tr>
		</table>
	</span>

	<table border="0" style="height:83px;width:780px" align="center">
		<tr><td></td></tr>
	</table>

	<table border="0" style="height:30px;width:780px" align="center">
		<tr>
		<td align="right" style="width:390px">
		<script>
			document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
		</script>
		</td>
		<td align="left" style="width:390px">
		<script>
			document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="NextButton" onClick="return goNext()" class="button_IQ2" style="cursor:pointer">');
		</script>
		</td>
		</tr>
	</table>

	<table border="0" style="height:60px;width:780px" align="center">
		<tr><td></td></tr>
	</table>
</td>
</tr>
</table>
</div>
<script>
if(showConnectResult){
	document.tcpip.ipmode[0].checked=true;
	document.tcpip.ipmode[1].checked=false;
}
else{
	if(useAutoIP){
		document.tcpip.ipmode[0].checked=true;
		document.tcpip.ipmode[1].checked=false;
	}
	else{
		document.tcpip.ipmode[0].checked=false;
		document.tcpip.ipmode[1].checked=true;
	}
}
autoIp_Clicked();
</script>
</form>
</body>

<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>

</html>
