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
var uptime="<% getInfo("uptime"); %>";
var ssidTbl=<%getInfo("ssidAll");%>;
var ssidTbl5g= <%getiNICInfo("Assidtbl");%>;
var curTime = new Date(<%getVar("systime","","");%>);
var DisableGband = <%getVar("wlanDisabled","","");%>;
var DisableAband = <%getiNICVar("wlanDisabled","","");%>;
var ra0IF="<%getInfo("ra0IF");%>";
var rai0IF="<%getInfo("rai0IF");%>";

function switch_mode( mode )
{

	$("#help-box").hide();

	var item_array = new Array
	(
		document.getElementById("sysSta_fieldset"),
		document.getElementById("sysLog_fieldset"),
		document.getElementById("SecurityLog_fieldset"),
		document.getElementById("ActDHCPClient_fieldset"),
		document.getElementById("Statistics_fieldset")
		/*document.getElementById("sysSta"),
		document.getElementById("sysLog"),
		document.getElementById("SecurityLog"),
		document.getElementById("ActiveDHCPClient"),
		document.getElementById("Statistics")*/
	)


	var help_info_array = new Array
	(
		showText(sysHelpInfo0)+"<br><br><b>"+showText(System)+":</b><br>"+showText(sysHelpInfo1)+"<br><b>"+showText(Internet)+":</b><br>"+showText(sysHelpInfo2)+"<br><b>"+showText(Wireless)+":</b><br>"+showText(sysHelpInfo3)+"<br><b>"+showText(LAN)+":</b><br>"+showText(sysHelpInfo4)+"<br><br>",
		showText(sysLogHelpInfo),
		showText(SecurityLogHelpInfo),
		showText(ActDHCPClientHelpInfo),
		showText(transAndreceptHelpInfo)
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
	}
}

function init()
{
	document.getElementById("help-info1").innerHTML=showText(sysHelpInfo0)+"<br><br><b>"+showText(System)+":</b><br>"+showText(sysHelpInfo1)+"<br><b>"+showText(Internet)+":</b><br>"+showText(sysHelpInfo2)+"<br><b>"+showText(Wireless)+":</b><br>"+showText(sysHelpInfo3)+"<br><b>"+showText(LAN)+":</b><br>"+showText(sysHelpInfo4)+"<br><br>";

	$("#help-box").hide();

	//set 2.4g ssid
	if(ssidTbl[0].length > 20)
	{
   ssid24g=ssidTbl[0].substring(0,20)+'<br>'+ssidTbl[0].substring(20,ssidTbl[0].length);
	}
	else
	{
		ssid24g=ssidTbl[0];
	}
	
  //set 5g ssid
	if(ssidTbl5g[0].length > 20)
	{
   ssid5g=ssidTbl5g[0].substring(0,20)+'<br>'+ssidTbl5g[0].substring(20,ssidTbl5g[0].length);
	}
	else
	{
		ssid5g=ssidTbl5g[0];
	}

	ssid5g=ssid5g.replace(/\s/g, "&nbsp;");
	ssid24g=ssid24g.replace(/\s/g, "&nbsp;");

	document.getElementById('mainssid').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid24g+'</span></div>';
	document.getElementById('mainssid5g').innerHTML = '<div class="mini2len" ><span class="choose-itemText">'+ssid5g+'</span></div>';
}

function goCheck()
{
	window.location.assign("admin_upgrade.asp");
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

			<table border="0" align="center" width="685">
				<tr><td>

					<table border="0" width="" style="background: #FFFFFF;" cellpadding="0" cellspacing="9" align="left">
							<!-- system -->
							<tr>
								<td colspan="2" align="center" height="" class="titletableText" ><script>dw(System)</script></td>
							</tr>
							<tr>
								<td><div class="minilen"><script>dw(Model)</script> </div></td>
								<td><div class="mini2len"><span class="choose-itemText"><% getInfo("name"); %></span></div></td>
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
											</td>
										</tr>

							</form>


							<tr>
								<td><div class="minilen"><script>dw(HardwareVersion)</script></div></td>
								<td><div class="mini2len"><span class="choose-itemText">Rev. A</span></div></td>
							</tr>
							<tr>
								<td><div class="minilen"><script>dw(RuntimeCodeVersion)</script></div></td>
								<td><div class="mini2len"><span class="choose-itemText"><% getInfo("fwVersion"); %></span></div></td>
							</tr>
							<tr>
								<td colspan="2" align="center">
									<script>
									var String_Check_Version=showText(checkVersion);
									if (stype == 10) 
									{
										String_Check_Version=String_Check_Version.substring(0,26)+"<br>"+String_Check_Version.substring(26,String_Check_Version.length);
									}
									document.write('<button name="checkVersion" onclick="upgrade_action()" class="ui-button-nolimit">'+String_Check_Version+'</button>');
									</script>
								</td>
							</tr>
					</table>

					<!--  LAN Configuration   -->
					<table width="" style="background: #FFFFFF;" cellpadding="0" cellspacing="9" align="right">
							<tr>
								<td colspan="2" align="center" height="" class="titletableText" ><script>dw(LAN)</script></td>
							</tr>
							
							<tr>
								<td><div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;"><script>dw(IPAddr)</script></div></td>
								<td><div style="text-align:left; width:160px;  padding-left:1px;" ><span class="choose-itemText"><% getInfo("ip"); %></span></div></td>
							</tr>
							<tr>
								<td><div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;"><script>dw(SubnetMask)</script></div></td>
							<td><div style="text-align:left; width:160px;  padding-left:1px;" ><span class="choose-itemText"><% getInfo("mask"); %></span></div></td>
							</tr>
							<tr>
								<td><div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;"><script>dw(DHCPServer)</script></div></td>
								<td><div style="text-align:left; width:160px;  padding-left:1px;" ><span class="choose-itemText">
									<script>
										choice=<%getVar("dhcp","","");%>;
										if ( choice == 0 ) document.write(showText(disable));
										if ( choice == 2 ) document.write(showText(enable));
									</script>							
								</span></div></td>
							</tr>
							<tr>
								<td><div style="text-align:right; width:140px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY:Calibri,Arial,Microsoft JhengHei;"><script>dw(macAddr)</script></div></td>
								<td><div style="text-align:left; width:160px;  padding-left:1px;" ><span class="choose-itemText"><% getInfo("hwaddr"); %></span></div></td>
							</tr>
					</table>

	</td></tr>
	<tr><td colspan="2">
		 &nbsp;
	</td></tr>
	<tr><td>

				<!--  internet connection  -->
					<table width="" style="background: #FFFFFF;" cellpadding="0" cellspacing="9" align="left">
							<form name="internetSta">
								<input type="hidden" name="ip" value="<% getInfo("sta-ip"); %>">
								<input type="hidden" name="dns1" value="<% getInfo("sta-dns1"); %>">
								<input type="hidden" name="dns2" value="<% getInfo("sta-dns2"); %>">
								<input type="hidden" name="dns3" value="<% getInfo("sta-dns3"); %>">
								<input type="hidden" name="gw" value="<% getInfo("sta-gateway"); %>">

								<tr>
									<td colspan="2" align="center" height="" class="titletableText" ><script>dw(Internet)</script></td>
								</tr>
								
								<tr>
									<td ><div class="minilen"><script>dw(IPAddrMode)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText">
											<script>
												temp_connect_type="<% getInfo("sta-current"); %>";

												if ( temp_connect_type.search("Dynamic IP") >=0 )
													conn_type=showText(DynamicIP);
												else if ( temp_connect_type.search("Static IP") >=0 )
													conn_type=showText(StaticIP);
												else if ( temp_connect_type.search("PPPoE") >=0 )
													conn_type=showText(PPPoE);
												else if ( temp_connect_type.search("PPTP") >=0 )
													conn_type=showText(PPTP);
												else if ( temp_connect_type.search("L2TP") >=0 )
													conn_type=showText(L2TP);
												else
													conn_type="N/A";

												if ( temp_connect_type.search(/disconn/i) >=0 )
													conn_status=showText(Disconnect);
												else
													conn_status=showText(Connect);

												document.write(conn_type+'&ensp;'+conn_status)
											</script>
									</span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(IPAddr)</script></div></td>
									<td id="ip"><div class="mini2len"><span class="choose-itemText">
										<script>
											temp_ip="<% getInfo("sta-ip"); %>";
											if (temp_ip == "")
											 document.write("");
											else
												document.write(temp_ip);
										</script>
									</span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(SubnetMask)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText"><% getInfo("sta-mask"); %></span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(gateAddr)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText">
										<script>
											temp_gateway="<% getInfo("sta-gateway"); %>";
											if (( temp_ip == "") || (temp_gateway == ""))
												document.write("");
											else
												document.write(temp_gateway);
										</script>
									</span></div></td>
								</tr>
								<tr>
									<td ><div class="minilen"><script>dw(macAddr)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText"><% getInfo("sta-mac"); %></span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(PrimaryDNS)</script> </div></td>
									<td><div class="mini2len"><span class="choose-itemText">
										<script>
											temp_dns1="<% getInfo("sta-dns1"); %>";
											if(( temp_ip == "") || ( temp_dns1 == "" ))
												document.write("");
											else
												document.write(temp_dns1);
										</script>
									</span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(SecondaryDNS)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText">
										<script>
											temp_dns2="<% getInfo("sta-dns2"); %>";
											if(( temp_ip == "") || ( temp_dns2 == "" ))
												document.write("");
											else
												document.write(temp_dns2);
										</script>
									</span></div></td>
								</tr>
								<tr>
									<td><div class="minilen"><script>dw(ThirdDNS)</script></div></td>
									<td><div class="mini2len"><span class="choose-itemText">
										<script>
											temp_dns3="<% getInfo("sta-dns3"); %>";
											if(( temp_ip == "") || ( temp_dns3 == "" ))
												document.write("");
											else
												document.write(temp_dns3);
										</script>
									</span></div></td>
								</tr>
							</form>
				</table>

				<script>
						var FlashChannel5g = <%getiNICVar("channel","","");%>;
						var FlashChannel = <%getVar("channel","","");%>;
						var AutoChannel5g = "<%getiNICInfo("autochannel");%>"; 
						var AutoChannel = "<% getInfo("autochannel"); %>";
				</script>

				<!-- 2.4g wireless Configuration -->
				<table width="" style="background: #FFFFFF;" cellpadding="0" cellspacing="9" align="right">
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
									var apMode=<%getVar("apMode","","");%>;
									var apmodeTbl = new Array(showText(apmode),showText(apmode1),showText(APcl),showText(apmode3),showText(apmode4),showText(apmode5),showText(repeatermode));
									for (i=0 ; i<7 ; i++)
									{
										if ( i==apMode )
											document.write(apmodeTbl[i]);
									}
								</script>
							</span></div></td>
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
										else if (FlashChannel == 0)
											document.write('<span class="choose-itemText">'+AutoChannel+'</span>');				
										else
											document.write('<span class="choose-itemText">'+FlashChannel+'</span>');
								</script>
							</span></div></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Security)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
									encrypt =<%getVar("encrypt","","");%>;
									var wpaCipherTbl =	<%getInfo("wpaCipherAll");%>;
									var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
									var CipherTbl = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));
									for (i=0 ; i<6 ; i++)
									{
										if ( i == encrypt ) 
										{
											if( i != 2 )
												document.write(modeTbl[i]);
											else
												document.write(CipherTbl[parseInt(wpaCipherTbl[0],10)-1]);
										}
									}
								</script>
						</span></div></tr>
						<tr>
							<td><div class="minilen"><script>dw(macAddr)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><% getInfo("wladdr"); %></span></div></td>
						</tr>

						<tr><td colspan="2">
						 &nbsp;
						</td></tr>
						<!-- 5g wireless Configuration  -->
						<tr>
							<td colspan="2" align="center" height="" class="titletableText" >5<script>dw(GHz)</script>&ensp;<script>dw(Wireless)</script>
								<script>
									if(rai0IF=="down") document.write("&ensp;("+showText(disable)+")");
								</script>
							</td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(mode)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
									var apMode5g = <%getiNICVar("apMode","","");%>;
									var apmodeTbl5g = new Array(showText(apmode),showText(apmode1),showText(APcl),showText(apmode3),showText(apmode4),showText(apmode5),showText(repeatermode));
									for (i=0 ; i<7 ; i++)
									{
										if ( i==apMode5g )
											document.write(apmodeTbl5g[i]);
									}
								</script>
							</span></div></td>
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
										else if (FlashChannel5g == 0)
											document.write('<span class="choose-itemText">'+AutoChannel5g+'</span>');		
										else
											document.write('<span class="choose-itemText">'+FlashChannel5g+'</span>');
								</script>						
							</span></div></td>
						</tr>
						<tr>
							<td><div class="minilen"><script>dw(Security)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText">
								<script>
									encrypt5g = <%getiNICVar("encrypt","","");%>;
									var wpaCipherTbl5g =	<%getiNICInfo("wpaCipherAll");%>;
									var modeTbl5g = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
									var CipherTbl5g = new Array(showText(WPA_TKIP),showText(WPA2_AES),showText(WPA2_Mixed));
									for (i=0 ; i<6 ; i++)
									{
										if ( i == encrypt5g ) 
										{
											if( i != 2 )
												document.write(modeTbl5g[i]);
											else
												document.write(CipherTbl5g[parseInt(wpaCipherTbl5g[0],10)-1]);
										}
									}
								</script>
						</span></div></tr>
						<tr>
							<td><div class="minilen"><script>dw(macAddr)</script></div></td>
							<td><div class="mini2len"><span class="choose-itemText"><% getInfo("inicaddr"); %></span></div></td>
						</tr>
				</table>

		</td></tr>
		<tr><td colspan="2">
		 &nbsp;
		</td></tr>
	</table>

  </fieldset>


<script>
		$("#help-box").hide();
		funClock();
</script>
</blockquote>


</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>
