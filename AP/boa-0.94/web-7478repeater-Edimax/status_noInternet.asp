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


function init()
{
	$("#help-box").hide();
	
	document.getElementById("help-info1").innerHTML=showText(APmodeHelp1)+"<br><br><b>"+showText(System)+":</b><br>"+showText(APmodeHelp2)+"<br><b>"+showText(Wireless)+":</b><br>"+showText(APmodeHelp3)+"<br><b>"+showText(LAN)+":</b><br>"+showText(APmodeHelp4)+"<br><br>";

	//set 2.4g ssid
	if(ssidTbl[0].length > 20)
   	ssid24g=ssidTbl[0].substring(0,20)+'<br>'+ssidTbl[0].substring(20,ssidTbl[0].length);
	else
		ssid24g=ssidTbl[0];

  //set 5g ssid
	if(ssidTbl5g[0].length > 20)
   ssid5g=ssidTbl5g[0].substring(0,20)+'<br>'+ssidTbl5g[0].substring(20,ssidTbl5g[0].length);
	else
		ssid5g=ssidTbl5g[0];
	

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
				<td align="center"> 

					<table border="0" style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="left">
							<!-- system -->
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" ><script>dw(System)</script></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Model)</script> </div></td>
							<!--<td><div class="mini2len"><span class="choose-itemText"><% getInfo("name"); %></span></div></td>-->
							<td><div class="mini2len"><span class="choose-itemText"><script>dw(repeatermode)</script></span></div></td>
						</tr>
						<!--<tr>
							<td><div class="minilen"><script>dw(Uptime)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><%getInfo("uptime");%></div></td>
						</tr>-->
						<!--=======================================================================================================-->
					
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
								<td><div class="minilen"><script>dw(HardwareVersion)</script></div></td>
								<td><div class="mini2len"><span class="choose-itemText">Rev. A</span></div></td>
							</tr>
							<!--<tr>
								<td><div class="minilen"><script>dw(BootCodeVersion)</script></div></td>
								<td><div class="mini2len"><span class="choose-itemText">1.0</span></div></td>
							</tr>-->
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
					</table>

					<!--============================== LAN Configuration ==============================-->
					<table border="0"  style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="right">
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
			<script>
				var defaultChan5g = <%getiNICVar("channel","","");%>;
				var defaultChan = <%getVar("channel","","");%>;
				var cur_Chan="<% getInfo("autochannel"); %>";
				var cur_Chan5g="<%getiNICInfo("autochannel");%>"; 
			</script>
			<!--============================== 2.4g wireless Configuration ==============================-->
				<table border="0"  style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="left">
					<tr>
						<td colspan="2" align="center" height="" class="titletableText" >
							2.4<script>dw(GHz)</script>&ensp;<script>dw(Wireless)</script>
							<script>
								if(ra0IF=="down") document.write("&ensp;("+showText(disable)+")");
							</script>
						</td>
					</tr>
					<tr>
						<td><div class="minilen"><script>dw(mode)</script></div></td>
						<td><div class="mini2len"><span class="choose-itemText">
							<script>
								var apMode2g=<%getVar("apMode","","");%>;
								if (wizMode==1){
									if(apMode2g!=0)
										document.write(showText(apmode5));
									else
										document.write(showText(apmode));
								}
								else if(wizMode==3){
									document.write(showText(APcl));
								}
								else if(wizMode==4){
									document.write(showText(wisp));
								}
								else
									document.write(showText(repeatermode));
							</script>
							</span></div></td>
						</tr>
							<script>
							if((wizMode==2)||(wizMode==3)){
								document.write('<tr><td><div class="minilen">'+showText(Status)+'</div></td>');
								var connectStatus2G = "<% getInfo("connectStatus2G"); %>";
								if(connectStatus2G=="NotAssociated"){
									document.write('<td><div class="mini2len"><span class="choose-itemText" style="COLOR: #b40404;">');
									document.write(showText(Disconnect));
								}
								else if(connectStatus2G=="disable"){
									document.write('<td><div class="mini2len"><span class="choose-itemText">');
									document.write(showText(disable));
								}
								else{
									document.write('<td><div class="mini2len"><span class="choose-itemText">');
									document.write(showText(Connect));
								}
								document.write('</span></div></td></tr>');	
								document.write('<tr><td><div class="minilen">'+showText(SignalStrength)+'</div></td>');
								document.write('<td><div class="mini2len"><span class="choose-itemText">');
								document.write('<div id="width2" style="float:left; width:0%"></div>');
								if(connectStatus2G=="NotAssociated" || connectStatus2G=="disable"){
									document.write('<div style="float:left;">&nbsp;0%</div>');	
								}
								else{
									document.write('<div style="float:left;">&nbsp;'+connectStatus2G+'%</div>');	
									document.getElementById("width2").style.width = parseInt(connectStatus2G,10)*0.73+'%';
								}
								document.write('</span></div></td></tr>');	
							}
							</script>
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
							</span></div></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Security)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
                  if((wizMode==2)||(wizMode==3)){
										if(useCrossBand == "1"){
											tkipuse = wpaCipher5g;
											encryptMethod = encrypt5g;
										}
										else{
											tkipuse = wpaCipher;
											encryptMethod = encrypt;
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
										var wpaCipherTbl =	<%getInfo("wpaCipherAll");%>;
										var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
										var CipherTbl = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));
										for (i=0 ; i<6 ; i++){
											if ( i == encrypt ) {
												if( i != 2 )
													document.write(modeTbl[i]);
												else
													document.write(CipherTbl[parseInt(wpaCipherTbl[0],10)-1]);
										  }
		                }
									}
								</script>
						</span></div></tr>
						<tr>
							<td><div class="minilen"><script>dw(macAddr)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><% getInfo("wladdr"); %></span></div></td>
						</tr>		

						<script>
						/*if (( wizMode==2 ) && (connectStatus2G=="NotAssociated")){
							document.write('<tr><td colspan="2"><span class="choose-itemText" style="COLOR: #b40404;">');
							document.write(''+showText(repDisconnect1)+'<a href="#" onClick="openwiz();"> '+showText(repDisconnect2)+' </a> '+showText(repDisconnect3)+'</span>');
							document.write('</span></div></td></tr>');	
						}*/
						</script>
				</table>


				
				<!-- ============================== 5g wireless Configuration ==============================-->
				<table border="0"  style="background: #FFFFFF; width:325px;" cellpadding="0" cellspacing="9" align="right">
					
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" >5<script>dw(GHz)</script>&ensp;<script>dw(Wireless)</script>
								<script>
									if(rai0IF=="down")  document.write("&ensp;("+showText(disable)+")");
								</script>
							</td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(mode)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
									var apMode5g=<%getiNICVar("apMode","","");%>;
									if ( wizMode==1 ){
										if(apMode5g!=0)
											document.write(showText(apmode5));
										else							
											document.write(showText(apmode));
									}
									else if( wizMode==3 ){
										document.write(showText(APcl));
									}
									else if( wizMode==4 ){
										document.write(showText(wisp));
									}
									else
										document.write(showText(repeatermode));
								</script>
							</span></div></td>
						</tr>
						<script>
						if ((wizMode==2)||(wizMode==3)){
							document.write('<tr><td><div class="minilen">'+showText(Status)+'</div></td>');
							var connectStatus5G = "<% getInfo("connectStatus5G"); %>";
							if(connectStatus5G=="NotAssociated"){
								document.write('<td><div class="mini2len"><span class="choose-itemText" style="COLOR: #b40404;">');
								document.write(showText(Disconnect));
							}
							else if(connectStatus5G=="disable"){
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
							if(connectStatus5G=="NotAssociated" || connectStatus5G=="disable"){
								document.write('<div style="float:left;">&nbsp;0%</div>');	
							}
							else{
								document.write('<div style="float:left;">&nbsp;'+connectStatus5G+'%</div>');	
								document.getElementById("width5").style.width = parseInt(connectStatus5G,10)*0.73+'%';
							}
							document.write('</span></div></td></tr>');	
						}
						</script>
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
							</span></div></td>
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
									var wpaCipherTbl5g =	<%getiNICInfo("wpaCipherAll");%>;
									var modeTbl5g = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
									var CipherTbl5g = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));
									for (i=0 ; i<6 ; i++){
										if ( i == encrypt5g ){
											if( i != 2 )
												document.write(modeTbl5g[i]);
											else
												document.write(CipherTbl5g[parseInt(wpaCipherTbl5g[0],10)-1]);
										}
									}
								}
								</script>
						</span></div></tr>
						<tr>
							<td><div class="minilen"><script>dw(macAddr)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><% getInfo("inicaddr"); %></span></div></td>
						</tr>
						<script>
						/*if (( wizMode==2 ) && (connectStatus5G=="NotAssociated"))
						{
							document.write('<tr><td colspan="2"><span class="choose-itemText" style="COLOR: #b40404;">');
							document.write(''+showText(repDisconnect1)+'<a href="#" onClick="openwiz();"> '+showText(repDisconnect2)+' </a> '+showText(repDisconnect3)+'</span>');
							document.write('</span></div></td></tr>');	
						}*/
						</script>
						
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
