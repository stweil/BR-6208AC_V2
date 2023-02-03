<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<% language=javascript %>
<SCRIPT>

var wisp_enable=<% getInfo("stadrv_type");%>;
//var bandindex = <% getInfo("stadrv_band"); %>;

function displayObj()
{
	var i=document.wisp_form.stadrv_encrypttype.value;
	switch(i)
	{
		case "0":
			document.getElementById("WPA2").style.display = "none";
			break;
		case "3":
			document.getElementById("WPA2").style.display = "block";
			break;
		default: break;
	}
}

function WISP_SiteSurvey()
{ 
	if(document.getElementsByName("stadrv_band")[0].checked)
		window.open( "wisp_wlsurvey.asp", 'Wireless', 'width=800, height=600 , screenX=100, screenY=50,resizable=yes,   toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=yes' );
	else
		window.open( "wisp_wlsurvey5g.asp", 'Wireless', 'width=800, height=600 , screenX=100, screenY=50,resizable=yes,   toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=yes' );
}
function lockUI(enable)
{
		if (enable == 0)
		{
			document.wisp_form.stadrv_WPA2cipher.disabled=true;
			document.wisp_form.stadrv_WPA2pskformat.disabled=true;
			document.wisp_form.stadrv_WPA2pskkey.disabled=true;

			
			document.wisp_form.stadrv_ssid.disabled=true;
			//stadrv_chan.disabled=true;
			document.wisp_form.stadrv_band[0].disabled=true;
			document.wisp_form.stadrv_band[1].disabled=true;
			document.wisp_form.selSurvey.disabled=true;
			document.wisp_form.stadrv_chan.disabled=true;
			document.wisp_form.stadrv_encrypttype.disabled=true;
		}
		else
		{
			document.wisp_form.stadrv_WPA2cipher.disabled=false;
			document.wisp_form.stadrv_WPA2pskformat.disabled=false;
			document.wisp_form.stadrv_WPA2pskkey.disabled=false;

			document.wisp_form.stadrv_ssid.disabled=false;
			//stadrv_chan.disabled=false;
			document.wisp_form.selSurvey.disabled=false;
			document.wisp_form.stadrv_band[0].disabled=false;
			document.wisp_form.stadrv_band[1].disabled=false;
			document.wisp_form.stadrv_chan.disabled=false;
			document.wisp_form.stadrv_encrypttype.disabled=false;
		}
}
function setPskKeyValue()
{
	if (document.wisp_form.stadrv_WPA2pskformat.selectedIndex == 0)
	{
		document.wisp_form.stadrv_WPA2pskkey.maxLength = 63;
		document.wisp_form.stadrv_WPA2pskkey.value=document.wisp_form.stadrv_WPA2pskkey.value.substr(0,63);
	}
	else 
	{
		document.wisp_form.stadrv_WPA2pskkey.maxLength = 64;
	}
}

var wanmod = <%getVar("wanMode","","");%>;

function switch_mode(mode)
{
	document.getElementById("help-box").style.display="none";

	var item_array = new Array
	(
		document.getElementById("dynamicip_fieldset"), document.getElementById("staticip_fieldset"),
		document.getElementById("pppoe_fieldset"), document.getElementById("pptp_fieldset"),
		null, null,
		document.getElementById("l2tp_fieldset"), document.getElementById("wisp_fieldset")
	)

	var help_info_array = new Array
	(
		showText(dynamicipHelp), showText(staticipHelp),
		showText(pppoeHelp), showText(pptpHelp),
		null, null, showText(l2tpHelp), showText(wispHelp)
	);

	for(i=0; i<item_array.length; i++)
	{
		if(item_array[i])
		{
			if(i==mode)
			{
				item_array[i].style.display="block";
				document.getElementById("help-info1").innerHTML=help_info_array[i];
				resetform(i);
			}
			else
			{
				item_array[i].style.display="none";
			}
		}
	}
}

function saveChanges(who, rebootFlag, connect)
{
	switch(who)
	{
		case 7:
				with(document.wisp_form) {

					if ((wisp_enable != 0 ) || (stadrv_type[1].checked==true)) // if enable, check setting value
					{
						if (stadrv_ssid.value=="") { 
							alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(essid)));
							stadrv_ssid.value = stadrv_ssid.defaultValue;
							stadrv_ssid.focus();
							return false;
						}

						if (stadrv_chan.value=="") { 
							alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(ChannelNumber)));
							stadrv_chan.value = stadrv_chan.defaultValue;
							stadrv_chan.focus();
							return false;
						}

									
						var error=0;
						var str = stadrv_chan.value;
						for (var x=0; x<str.length; x++)
						{
							if ((str.charAt(x) >= '0' && str.charAt(x) <= '9')) continue;
							error = 1;	
							break;
						}
						if(error==0){
							if(document.getElementsByName("stadrv_band")[0].checked)
							{
								if ( (parseInt(stadrv_chan.value,10) < 1) || (parseInt(stadrv_chan.value,10) > 14) )
									error = 1;	
							}
							if(document.getElementsByName("stadrv_band")[1].checked)
							{
								if ( (parseInt(stadrv_chan.value,10) < 36) || (parseInt(stadrv_chan.value,10) > 165) )
									error = 1;	
							}
						}
						if(error) {
							alert(showText(InvalidSomething).replace(/#####/,showText(ChannelNumber)));		
							stadrv_chan.focus();			
							return false;
						}
						
						var strMethod = stadrv_encrypttype.value;

						if (strMethod==3) //WPA2
						{
							var str = stadrv_WPA2pskkey.value;
							if (stadrv_WPA2pskformat.selectedIndex==1) {
								if (str.length != 64) {
									alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
									return false;
								}
								takedef = 0;
								if (stadrv_WPA2pskformat.value == 1 && stadrv_WPA2pskkey.value.length == 64) {
									for (var i=0; i<64; i++) {
											if ( str.charAt(i) != '*')
											break;
									}
									if (i == 64 )
										takedef = 1;
								}
								if (takedef == 0) {
									for (var i=0; i<str.length; i++) {
											if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
											(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
											(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*')
											continue;
										alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
										//pskValue.focus();
										return false;
									}
								}
							}
							else {
								if (str.length < 8) {
									alert(showText(pskshouldbe8));
									return false;
								}
								if (str.length > 63) {
									alert(showText(pskshouldbe64));
									return false;
								}
							}
						}
					}

					if(rebootFlag)  
            isApply.value = "ok";

					submit();
				}
			break;
		default: 
			alert("error!");
			return false;
			break;
		}
}

function resetform(who)
{
	switch(who)
	{
		case 7:
				document.wisp_form.reset();
				displayObj();
				lockUI(wisp_enable);
			break;
		default: 
			alert("error!");
			return false;
			break;
		}
}

function init()
{
	$("#help-box").hide();
}
function setwna_type()
{
	if(parent.wispmode==1)
		switch_mode(7);
}
</script>
</head>
<body onLoad="init();">

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>

<blockquote>
	<fieldset name="wisp_fieldset" id="wisp_fieldset" style="display:none">
		<legend><script>dw(WISP)</script></legend>
			<form action="/goform/formStaDrvSetup" method="POST" name="wisp_form">
				<table class="" border="0" align="left">
					<tr>
						<td><div class="itemText-len"><script>dw(enable)</script>&nbsp;/&nbsp;<script>dw(disable)</script></div></td>
						<td><div class="itemText2-len"><span class="choose-itemText">						
							<script>
								var statype=wisp_enable;
								if(statype==0)
								{
									document.write('<input type="radio" value="0" name="stadrv_type" checked onClick="lockUI(0)">'+showText(disable));
									document.write('<input type="radio" value="1" name="stadrv_type" onClick="lockUI(1)">'+showText(enable));
								}
								else if((statype==1) || (statype==3))
								{
									document.write('<input type="radio" value="0" name="stadrv_type" onClick="lockUI(0)">'+showText(disable));
									document.write('<input type="radio" value="1" name="stadrv_type" checked onClick="lockUI(1)">'+showText(enable));
								}
								else if((statype==2)||(statype==4))
								{
									document.write('<input type="radio" value="0" name="stadrv_type" onClick="">'+showText(disable));
									document.write('<input type="radio" value="1" name="stadrv_type" onClick="">'+showText(enable));
								}
							</script>
						</span></div></td>
					</tr>
					<tr>
						<td colspan="2">
							&nbsp;
						</td>
					</tr>
					<tr>	
						<td><div class="itemText-len">
							<script>
								document.write('<b><span class="itemText">'+showText(basicsetting)+'&ensp;:</span></b>')
							</script></div></td>
						<td colspan="" class="itemText2-len"></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(essid)</script></div></td>
						<td><div class="itemText2-len">
							<script>
									document.write('<input type="text" name="stadrv_ssid" size="25" maxlength="32" value="<% getInfo("stadrv_ssid"); %>">')
							</script>
						</div></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(wlSiteSurvey)</script></div></td>
						<td><div class="itemText2-len"><span class="choose-itemText">
							<script>	
								var band = <% getInfo("stadrv_band"); %>;
         
								if (band == 0){
									document.write('<input type="radio" value="0" name="stadrv_band" onClick="" checked >2.4G&nbsp;');   
									document.write('<input type="radio" value="1" name="stadrv_band" onClick="">5G&nbsp;&nbsp;');
								}else{
									document.write('<input type="radio" value="0" name="stadrv_band" onClick="">2.4G&nbsp;');   
									document.write('<input type="radio" value="1" name="stadrv_band" onClick="" checked >5G&nbsp;&nbsp;');
								}
								document.write('<input type="button" class="ui-button-nolimit" name="selSurvey" value="'+showText(selSiteSurvey)+'" onClick="WISP_SiteSurvey()">');
							</script>
						</span></div></td>
					</tr>

					<tr>
						<td><div class="itemText-len"><script>dw(ChannelNumber)</script></div></td>
						<td><div class="itemText2-len">
								<script>
									var flash_chan = "<% getInfo("stadrv_chan");%>"
									var mib_chan24g = "<% getInfo("stadrv_autochan"); %>"
									var mib_chan5g = "<% getiNICInfo("stadrv_iNICautochan"); %>"
									
									if(flash_chan == "")
										{
											if (band)
												document.write('<input type="text" name="stadrv_chan" size="3" maxlength="3" value="'+mib_chan5g+'">');
											else
												document.write('<input type="text" name="stadrv_chan" size="3" maxlength="3" value="'+mib_chan24g+'">');
										}
									else
										document.write('<input type="text" name="stadrv_chan" size="3" maxlength="3" value="<% getInfo("stadrv_chan");%>">');
								</script>
						</div></td>
					</tr>
					<tr>
						<td colspan="2">
							&nbsp;
						</td>
					</tr>
					<tr>	
						<td><div class="itemText-len">
							<script>
								document.write('<b><span class="itemText">'+showText(SecuritySetting)+'&ensp;:</span></b>')
							</script></div></td>
						<td colspan="" class="itemText2-len"></td>
					</tr>
					<tr>
						<td><div class="itemText-len"><script>dw(Encryption)</script></div></td>
						<td><div class="itemText2-len">
							<select size="1" name="stadrv_encrypttype" onChange="displayObj()">
								<script>
									//0.Disable 3.WPA2
									var encryptType=<% getInfo("stadrv_encrypttype"); %>
									if (encryptType==0)
									{
										document.write('<option selected value="0">'+showText(disable)+'</option>');
										document.write('<option value="3">'+showText(WPA2)+'</option>');
									}
									else if (encryptType==3)
									{
										document.write('<option value="0">'+showText(disable)+'</option>');
										document.write('<option selected value="3">'+showText(WPA2)+'</option>');
									}
								</script>
							</select>
							
						</div></td>
					</tr>
				</table>					
							
				<div id="WPA2" style="display:none">
					<input type="hidden" id="wpa2Cipher1" name="stadrv_WPA2cipher" value="2">
					<table class="" border="0" align="left">		
						<tr>
							<td><div class="itemText-len"><script>dw(SecurityType)</script></div></td>
							<td><div class="itemText2-len"><span class="choose-itemText">
								<script>
										document.write('&nbsp;AES');
								</script>
							</span></div></td>
						</tr>
						<tr>
							<td ><div class="itemText-len"><script>dw(Pre_shared_Key_Format)</script></div></td>
							<td><div class="itemText2-len">
								<select size="1" name="stadrv_WPA2pskformat" ONCHANGE="setPskKeyValue()">
									<script>
										pskformatTbl = new Array (showText(Passphrase), showText(hex_64cha));
										for(i=0;i<2;i++)
										{
											if(i==<%getInfo("stadrv_pskformat");%>)
												document.write('<option selected value="'+i+'">'+pskformatTbl[i]+'</option>');
											else
												document.write('<option value="'+i+'">'+pskformatTbl[i]+'</option>');
										}
									</script>
								</select>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(Pre_shared_Key)</script></div></td>
							<td><div class="itemText2len">
									<script>
										var wisppskval = <%getInfo("wisp_pskkey");%>;
										pskval = wisppskval[0];
										if (wisppskval[0].search('"') != -1)  
										{
												pskval = wisppskval[0].replace(/\"/g, "&quot;");
										}
										document.write('<input type="text" name="stadrv_WPA2pskkey" size="32" maxlength="64" value="'+pskval+'" class="text" onchange="">')
									</script>
							</div></td>
						</tr>
					</table>
				</div>

				<br>

				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
					<tr>
						<td style="text-align:center; padding-top:30px;">
							<script>
								document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(7,0,4)" class="ui-button">');	
						var show_reboot = "<% getInfo("show_reboot"); %>"
						if(show_reboot == "1")
							document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(7,1,4)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
							</script>
						</td>
					</tr>
				</table>
				<input type=hidden value="/inter_wisp.asp" name="submit-url" id="submitUrl">
				<input type=hidden value="7" name="wanMode">
				<input type="hidden" name="supportAllSecurity" value="0">
				<input type=hidden value="" name="isApply">
			</form>			
	</fieldset>
				
<script>
			$("#help-box").hide();
</script>

</blockquote>
	
</body>

<head>
<meta http-equiv="Pragma" content="no-cache">
</head>	

</html>

<script>
switch_mode(wanmod);  //if save or apply, lock the mode page

switch(wanmod)
{
	case 7:
		displayObj();
		lockUI(wisp_enable);
	  break;
	default: break;
}
setwna_type();
</script>
