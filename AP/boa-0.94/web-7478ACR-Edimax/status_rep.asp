<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>
<SCRIPT>
var ssidTbl=<%getInfo("ssidAll");%>;
var ssidTbl5g=<%getiNICInfo("Assidtbl");%>;
//var ssidTbl=new Array("edimaxEXT.setup","edimax.guest","edimax.2","edimax.3","edimax.4","","","");
//var ssidTbl5g=new Array("edimaxEXT.setup5G");
var rootssid2g=<%getInfo("repeaterSSIDAll");%>;
var rootssid5g=<%getiNICInfo("repAssidtbl");%>;
//var rootssid2g=new Array("rootAP_2.4G_aaaaaaaaaaaa");
//var rootssid5g=new Array("rootAP_5G");

var get_apMode2g=<%getVar("apMode","","");%>;
var get_apMode5g=<%getiNICVar("apMode","","");%>;
apMode2g=parseInt(get_apMode2g,10);
apMode5g=parseInt(get_apMode5g,10);

var curTime=new Date(<%getVar("systime","","");%>);
var wlsw=<%getVar("wlanDisabled","","");%>;
var wlsw5g=<%getiNICVar("wlanDisabled","","");%>;
var ra0IF="<%getInfo("ra0IF");%>";
var rai0IF="<%getInfo("rai0IF");%>";
var wizMode=<%getVar("wizmode","","");%>;  //0,1,2
var useCrossBand = "<%getInfo("useCrossBand");%>";

var wpaEncrypt=	<%getInfo("wpaEncrypt");%>; 
var wpaCipher =	wpaEncrypt[0];
var wpa2Cipher = wpaEncrypt[1];
var encrypt = <%getVar("encrypt","","");%>;

var clientmodeTbl = new Array(showText(disable),showText(WEP),showText(WPA),showText(WPA2),showText(WPA_RADIUS),showText(WPA2_RADIUS));
var wpaCipherSuite = new Array("TKIP","AES");

var wpaEncrypt5g =	<%getiNICInfo("wpaEncrypt");%>; 
var wpaCipher5g  = wpaEncrypt5g[0];
var wpa2Cipher5g = wpaEncrypt5g[1];
var encrypt5g = <%getiNICVar("encrypt","","");%>;

var defaultChan5g = <%getiNICVar("channel","","");%>;
var defaultChan = <%getVar("channel","","");%>;
var cur_Chan="<% getInfo("autochannel"); %>";
var cur_Chan5g="<%getiNICInfo("autochannel");%>"; 


var wpaCipherTbl5g =	<%getiNICInfo("wpaCipherAll");%>;
var wpaCipherTbl =	<%getInfo("wpaCipherAll");%>;
var sec_modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
var tkip_aes_Tbl = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));


var shift_apssid_2g=0;
var shift_apssid_5g=0;
var shift_exssid=0;


function status_tbl_padding(fre)
{

  if(fre=="2"){
	if(!shift_apssid_2g && shift_exssid){	   
	    document.getElementById("apinfo_2g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 31px 0px";
	}
	if(shift_apssid_2g && !shift_exssid){	    
	    document.getElementById("apinfo_2g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 60px 0px";
	}  
	if(!shift_apssid_2g && !shift_exssid){	    
	    document.getElementById("apinfo_2g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 45px 0px";
	}  
	if(shift_apssid_2g && shift_exssid){
	    document.getElementById("apinfo_2g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 45px 0px";
	}  
   }
   if(fre=="5"){
	console.log("shift_apssid_5g"+shift_apssid_5g);
	console.log("shift_exssid"+shift_exssid);
	
	if(!shift_apssid_5g && shift_exssid){
	    document.getElementById("apinfo_5g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 31px 0px";
	}
	if(shift_apssid_5g && !shift_exssid){
	    document.getElementById("apinfo_5g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 60px 0px";
	}  
	if(!shift_apssid_5g && !shift_exssid){
	    document.getElementById("apinfo_5g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 45px 0px";
	} 
	if(shift_apssid_5g && shift_exssid){
	    document.getElementById("apinfo_5g").style.padding = "0px 0px 15px 0px";
	    document.getElementById("exinfo").style.padding = "0px 0px 45px 0px";
	} 
    }
}

function init()
{	
	$("#help-box").hide();
	
	document.getElementById("help-info1").innerHTML=showText(APmodeHelp1)+"<br><br><b>"+showText(System)+":</b><br>"+showText(APmodeHelp2)+"<br><b>"+showText(Wireless)+":</b><br>"+showText(APmodeHelp3)+"<br><b>"+showText(LAN)+":</b><br>"+showText(APmodeHelp4)+"<br><br>";

	//set 2.4g ssid
	if(ssidTbl[0].length > 20){
		ssid24g=ssidTbl[0].substring(0,20)+'<br>'+ssidTbl[0].substring(20,ssidTbl[0].length);
		shift_apssid_2g=1;
	}
	else{
		ssid24g=ssidTbl[0];
	}

	//set 5g ssid
	if(ssidTbl5g[0].length > 20){
		ssid5g=ssidTbl5g[0].substring(0,20)+'<br>'+ssidTbl5g[0].substring(20,ssidTbl5g[0].length);
		shift_apssid_5g=1;
	}
	else{
		ssid5g=ssidTbl5g[0];
	}
		
		
	if(apMode2g == 6){
	    if(rootssid2g[0].length > 20){
		  rootAP=rootssid2g[0].substring(0,20)+'<br>'+rootssid2g[0].substring(20,rootssid2g[0].length);
		  shift_exssid=1;
	    }
	    else{
		  rootAP=rootssid2g[0];
	    }	
	}
	else{
	    if(rootssid5g[0].length > 20){
		  rootAP=rootssid5g[0].substring(0,20)+'<br>'+rootssid5g[0].substring(20,rootssid5g[0].length);
		   shift_exssid=1;
	    }
	    else{
		  rootAP=rootssid5g[0];
	    }
	}
	document.getElementById('rootSSID').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+rootAP+'</span></div>';

	ssid5g=ssid5g.replace(/\s/g, "&nbsp;");
	ssid24g=ssid24g.replace(/\s/g, "&nbsp;");

	if(useCrossBand == "1"){
		document.getElementById('mainssid').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid5g+'</span></div>';
		document.getElementById('mainssid5g').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid24g+'</span></div>';
	}
	else{
		document.getElementById('mainssid').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid24g+'</span></div>';
		document.getElementById('mainssid5g').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid5g+'</span></div>';
	}
	status_tbl_padding(2);
}

function funClock()
{
	var runTime = new Date();
	var hours = document.tF0.hour.value;
	var minutes = document.tF0.min.value;
	var seconds = document.tF0.sec.value;
	if (seconds == 0)
	{
		minutes++;
		document.tF0.min.value++;
		if(minutes ==60)
		{
			hours++;
			document.tF0.hour.value++;
			minutes = 0;
			document.tF0.min.value = 0;
		}
	}
	if (hours == 12 && minutes==0 && seconds == 0)
		setTimeout('document.location.href="status.asp";',0);

	var temp_min=String(minutes);

	if (minutes <= 9 && temp_min.length==1)
		minutes = "0" + minutes;
	if (seconds <= 9)
		seconds = "0" + seconds;

	movingtime = "<font class='choose-itemText'>&nbsp;"+ hours + ":" + minutes + ":" + seconds + "</font>";

	if (document.layers)
	{
		document.layers.clock.document.write(movingtime);
		document.layers.clock.document.close();
	}
	else
		clock.innerHTML = movingtime;

	document.tF0.sec.value++;

	if(document.tF0.sec.value>59)
		document.tF0.sec.value = 0;

	setTimeout("funClock()", 1000)
}
function openwiz()
{
	window.open( '/wiz_repeater.asp', 'IQSetup', 'width=850, height=650 , screenX=100, screenY=50,resizable=yes, 	toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes' );
}
function upgrade_action()
{
	parent.back2status=1;
	with(document.AutoFWupgrade) {
		action.value = "check_version";
		submit();
	}
}


function ap_status_switch(freq){
  if(freq == "2"){
      document.getElementById("2g_ap_info").style.display = "block";
      document.getElementById("5g_ap_info").style.display = "none";
      document.getElementsByName("2g_ap_status_switch")[0].checked=true;
      status_tbl_padding(2);
  }
  if(freq == "5"){
      document.getElementById("2g_ap_info").style.display = "none";
      document.getElementById("5g_ap_info").style.display = "block";
      document.getElementsByName("5g_ap_status_switch")[1].checked=true;
      status_tbl_padding(5);
  }
 
}
</SCRIPT>

</head>
<body onLoad="init();">

<form action="/goform/formAutoFWupgrade" method="POST" name="AutoFWupgrade">
	<input type="hidden" value="" name="action">
	<input type="hidden" value="/auto_upgrade.asp" name="submit-url">
</form>	


	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td><div class="help-text" id="help-info1"></div></td></tr>
		</table>
	</div>

	<script>
	HelpButton();
	</script>

<blockquote>

<fieldset name="sysSta_fieldset" id="sysSta_fieldset">
	<legend><script>dw(System_Status)</script></legend>
	<!--<span id="sysSta" style="display:none">-->
	
		<table border="0" align="center" width="700px">
			<tr>
				<td align="center" > 

					<table border="0" style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="left">
							<!-- system -->
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" ><script>dw(System)</script></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Model)</script> </div></td>
							<!--<td><div class="mini2len"><span class="choose-itemText"><% getInfo("name"); %></span></div></td>-->
							<td><div class="mini2len"><span class="choose-itemText">RE11S</span></div></td>
						</tr>

						<!--<tr>
							<td><div class="minilen"><script>dw(Uptime)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><%getInfo("uptime");%></div></td>
						</tr>-->
						
					
						<form name="tF0" method="post" action="">
							<input type="hidden" name="year" value="2000">
							<input type="hidden" name="mon" value="0">
							<input type="hidden" name="day" value="1">
							<input type="hidden" name="hour" value="1">
							<input type="hidden" name="min" value="1">
							<input type="hidden" name="sec" value="1">

							<script>
								document.tF0.year.value=curTime.getFullYear();
								document.tF0.mon.value=curTime.getMonth();
								document.tF0.day.value=curTime.getDate();
								document.tF0.hour.value=curTime.getHours();
								document.tF0.min.value=curTime.getMinutes();
								document.tF0.sec.value=curTime.getSeconds();
							</script>
									
							<tr>
								<td><div class="minilen"><script>dw(CurrentTime)</script></div></td>	
								<td><div class="mini2len"><span class="choose-itemText">
									<script>
										var d=new Date()
										var monthname=new Array("1","2","3","4","5","6","7","8","9","10","11","12");
										var dayname=new Array("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31");
										/*document.write(monthname[document.tF0.mon.value] + "/")
										document.write(dayname[document.tF0.day.value-1] + "/")
										document.write(document.tF0.year.value)*/
										document.write(document.tF0.year.value + "/")
										document.write(monthname[document.tF0.mon.value] + "/")
										document.write(dayname[document.tF0.day.value-1])
									</script>
									<span Id="clock" class="time"></span>
									</span></div>
								</td>
							</tr>
						</form>

							<tr>
								<td><div class="minilen"><script>dw(RuntimeCodeVersion)</script></div></td>
								<td><div class="mini2len"><span class="choose-itemText"><% getInfo("fwVersion"); %></span></div></td>
							</tr>
							<tr>
								<td colspan="2" align="center">
									<script>
									document.write('<input type="button" value="'+showText(checkVersion)+'" name="checkVersion" onclick="upgrade_action()" class="ui-button-nolimit">');	
									</script>
								</td>
							</tr>
							<tr>
								<td><div class="minilen">&ensp;</div></td>
								<td><div class="mini2len">&ensp;</div></td>
							</tr>
					</table>
				</td>
				<td>
					<!--  LAN Configuration -->
					<table border="0"  style="background: #FFFFFF; width:325px; padding:0px 0px 30px 0px;" cellpadding="0" cellspacing="9" align="right" >
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" ><script>dw(LAN)</script></td>
						</tr>
							
						<tr>
							<td>
								<div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;">
									<script>dw(IPAddr)</script>
								</div>
							</td>
							<td>
								<div style="text-align:left; width:160px;  padding-left:1px;">
									<span class="choose-itemText"><% getInfo("ip"); %></span>
								</div>
							</td>
						</tr>

						<tr>
							<td>
								<div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;">
									<script>dw(SubnetMask)</script>
								</div>
							</td>
							<td>
								<div style="text-align:left; width:160px;  padding-left:1px;">
									<span class="choose-itemText"><% getInfo("mask"); %></span>
								</div>
							</td>
						</tr>
						<tr>
							<td>
								<div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;">
									<script>dw(DefaultGateway)</script>
								</div>
							</td>
							<td>
								<div style="text-align:left; width:160px;  padding-left:1px;">
									<span class="choose-itemText"><% getInfo("brx-gateway"); %></span>
								</div>
							</td>
						</tr>
						<tr>
							<td>
								<div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;">
									<script>dw(macAddr)</script>
								</div>
							</td>
							<td>
								<div style="text-align:left; width:160px;  padding-left:1px;">
									<span class="choose-itemText"><% getInfo("hwaddr"); %></span>
								</div>
							</td>
						</tr>
					</table>
	
			</td>
		</tr>
	
		<tr><td colspan="2">&nbsp;</td></tr>
		<tr><td>
			<!-- 2.4g ap info  -->
				<span id="2g_ap_info" style="display:block">
				<table border="0"  style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="left" id="apinfo_2g">
					
					<tr>
						<td colspan="2" align="center" height="" class="titletableText" >
							<script>dw(apmode)</script>&ensp;<script>dw(Status)</script>
						</td>
					</tr>
					<tr>
					      <td>
						  <div class="minilen"><span class="choose-itemText">
						  <input type="radio" value="" name="2g_ap_status_switch" checked>2.4<script>dw(GHz)</script>
						   </span></div>
					      </td>
					      <td>
						  <div class="mini2len"><span class="choose-itemText">
						  <input type="radio" value="" name="2g_ap_status_switch" onClick="ap_status_switch(5)">5<script>dw(GHz)</script>
						  </span></div>
					      </td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(Status)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
								if(ra0IF=="down") document.write(showText(disable));
								else document.write(showText(enable));
							</script>
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(essid)</script></div></td>
						<td id="mainssid"></td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(ChannelNumber)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
							    if(ra0IF=="down")
								    document.write('<span class="choose-itemText">'+showText(Auto)+'</span>');	
							    else if (defaultChan == 0)
								    document.write('<span class="choose-itemText">'+cur_Chan+'</span>');				
							    else
								    document.write('<span class="choose-itemText">'+defaultChan+'</span>');
							</script>
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(Security)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>		
//							    for (i=0 ; i<6 ; i++){
//								if ( i == encrypt ) {
//								    if( i != 2 )
//									    document.write(sec_modeTbl[i]);
//								    else
//									    document.write(tkip_aes_Tbl[parseInt(wpaCipherTbl[0],10)-1]);
//								}
//							    }	
								tkipuse = wpaCipher;
								encryptMethod = encrypt;
								for ( i=0; i<6; i++){  //Range Extender 0.Disable 1.WEP 2.WPA 3.WPA2
									if ( i == encryptMethod){
										document.write(clientmodeTbl[i]);
										if ( i==2 || i==3 ){
											if(tkipuse != 0) document.write('('+wpaCipherSuite[0]+')');
											else document.write('('+wpaCipherSuite[1]+')');
										}
									}
								}
							</script>
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(macAddr)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText"><% getInfo("wladdr"); %></span></div></td>
					</tr>
					
				</table>
				</span>
				<!-- 2.4g ap end  -->
				
				<!-- 5g ap info -->
				<span id="5g_ap_info" style="display:none">
				<table border="0"  style="background: #FFFFFF; width:325px; padding:0px 0px 15px 0px;" cellpadding="0" cellspacing="9" align="left" id="apinfo_5g">
					<tr>
						<td colspan="2" align="center" height="" class="titletableText" >
							<script>dw(apmode)</script>&ensp;<script>dw(Status)</script>
						</td>
					</tr>
					<tr>
					      <td>
						  <div class="minilen"><span class="choose-itemText">
						  <input type="radio" value="" name="5g_ap_status_switch" onClick="ap_status_switch(2)">2.4<script>dw(GHz)</script>
						   </span></div>
					      </td>
					      <td>
						  <div class="mini2len"><span class="choose-itemText">
						  <input type="radio" value="" name="5g_ap_status_switch" checked>5<script>dw(GHz)</script>
						  </span></div>
					      </td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(Status)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
								if(rai0IF=="down") document.write(showText(disable));
								else document.write(showText(enable));
							</script>
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(essid)</script></div></td>
						<td id="mainssid5g"></td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(ChannelNumber)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
							    if(rai0IF=="down")
								    document.write('<span class="choose-itemText">'+showText(Auto)+'</span>');	
							    else if (defaultChan5g == 0)
								    document.write('<span class="choose-itemText">'+cur_Chan5g+'</span>');		
							    else
								    document.write('<span class="choose-itemText">'+defaultChan5g+'</span>');
							</script>						
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(Security)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
							    
//							    for (i=0 ; i<6 ; i++){
//								if ( i == encrypt5g ){
//								    if( i != 2 )
//									    document.write(sec_modeTbl[i]);
//								    else
//									    document.write(tkip_aes_Tbl[parseInt(wpaCipherTbl5g[0],10)-1]);
//								}
//							    }
								tkipuse = wpaCipher5g;
								encryptMethod = encrypt5g;
								for ( i=0; i<6; i++){  //Range Extender 0.Disable 1.WEP 2.WPA 3.WPA2
									if ( i == encryptMethod){
										document.write(clientmodeTbl[i]);
										if ( i==2 || i==3 ){
											if(tkipuse != 0) document.write('('+wpaCipherSuite[0]+')');
											else document.write('('+wpaCipherSuite[1]+')');
										}
									}
								}
							</script>
						</span></div>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(macAddr)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText"><% getInfo("inicaddr"); %></span></div></td>
					</tr>
					
					
				</table>
				</span>
				<!-- 5g ap info end-->
			</td>
			<td>
				<!--  extender status -->
				<table border="0"  style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="right" id="exinfo">
					
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" >
								<script>dw(repeatermode)</script>&ensp;<script>dw(Status)</script>
								<script>
									if(apMode5g==6)  document.write("&ensp;( 5G )");
									else document.write("&ensp;( 2.4G )");
								</script>
							</td>
						</tr>
						
						<script>
						
						document.write('<tr><td><div class="minilen">'+showText(Status)+'</div></td>');
						
						if(apMode5g==6)
						    var connectStatus = "<% getInfo("connectStatus5G"); %>";
						else
						    var connectStatus = "<% getInfo("connectStatus2G"); %>";
						    
						if(connectStatus=="NotAssociated"){
							document.write('<td><div class="mini2len"><span class="choose-itemText" style="COLOR: #b40404;">');
							document.write(showText(Disconnect));
						}
						else if(connectStatus=="disable"){
							document.write('<td><div class="mini2len"><span class="choose-itemText">');
							document.write(showText(disable));
						}
						else{
							document.write('<td><div class="mini2len"><span class="choose-itemText">');
							document.write(showText(Connect));
						}
						document.write('</span></div></tr>');	
		
						document.write('</span></div></td></tr>');	
						document.write('<tr><td><div class="minilen">'+showText(SignalStrength)+'</div></td>');
						document.write('<td><div class="mini2len"><span class="choose-itemText">');
						document.write('<div id="width5" style="float:left; width:0%"></div>');
						if(connectStatus=="NotAssociated" || connectStatus=="disable"){
							document.write('<div style="float:left;">&nbsp;0%</div>');	
						}
						else{
							document.write('<div style="float:left;">&nbsp;'+connectStatus+'%</div>');	
							document.getElementById("width5").style.width = parseInt(connectStatus,10)*0.73+'%';
						}
						document.write('</span></div></td></tr>');	
					
						</script>
						<tr>
							<td><div class="minilen"><script>dw(extended)</script>&nbsp;<script>dw(essid)</script></div></td>
							<td id="rootSSID"></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(ChannelNumber)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
										if(apMode5g==6){
											if(rai0IF=="down")
												document.write('<span class="choose-itemText">'+showText(Auto)+'</span>');	
											else if (defaultChan5g == 0)
												document.write('<span class="choose-itemText">'+cur_Chan5g+'</span>');		
											else
												document.write('<span class="choose-itemText">'+defaultChan5g+'</span>');
										}
										else{
											if(ra0IF=="down")
												document.write('<span class="choose-itemText">'+showText(Auto)+'</span>');	
											else if (defaultChan == 0)
												document.write('<span class="choose-itemText">'+cur_Chan+'</span>');				
											else
												document.write('<span class="choose-itemText">'+defaultChan+'</span>');
										}
								</script>						
							</span></div>
							</td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Security)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
								if((wizMode==2)||(wizMode==3)){
									if(useCrossBand == "1"){
										tkipuse = wpaCipher;
										encryptMethod = encrypt;
									}
									else{
										tkipuse = wpaCipher5g;
										encryptMethod = encrypt5g;
									}
									for ( i=0; i<6; i++){  //Range Extender 0.Disable 1.WEP 2.WPA 3.WPA2
										if ( i == encryptMethod){
											document.write(clientmodeTbl[i]);
											if ( i==2 || i==3 ){
												if(tkipuse != 0) document.write('('+wpaCipherSuite[0]+')');
												else document.write('('+wpaCipherSuite[1]+')');															
											}
										}
									}
								}
								else{
									for (i=0 ; i<6 ; i++){
										if ( i == encrypt5g ){
											if( i != 2 )
												document.write(sec_modeTbl[i]);
											else
												document.write(tkip_aes_Tbl[parseInt(wpaCipherTbl5g[0],10)-1]);
										}
									}
								}
								</script>
							</span></div>
							</td>
						</tr>
						
						
				</table>

		</td></tr>
		<tr><td colspan="2">
		 &nbsp;
		</td></tr>
	</table>
				
			<!--</span>-->
  </fieldset>



<script>

			$("#help-box").hide();
			//document.getElementById("help-box").style.display = "none";
			funClock();
	
			 
</script>

</blockquote>

		
</body>

<head>
<meta http-equiv="Pragma" content="no-cache">
</head>

</html>
