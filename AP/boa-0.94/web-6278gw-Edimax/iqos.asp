<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<script type="text/javascript" src="/file/javascript.js"></script>
<link rel="stylesheet" href="/set.css">
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<% language=javascript %>

<SCRIPT>
EMPTY_TD_INNERHTML="<a>&nbsp;</a>"
EZQOS_TD_ID_PREFIX="EZQOS_TD_"
TEMP_TD_ID_PREFIX="TEMP_TD_"
EZQOS_PIC_SIZE="height=\"100%\" width=\"100%\""
TEMP_PIC_SIZE="height=\"35\" width=\"35\""
PIC_ALTNAME=".gif"
apply_lock=0;
function check_enable()
{
	var bandWidth_type0=document.getElementsByName("BandWidth_Type")[0];
	var bandWidth_type1=document.getElementsByName("BandWidth_Type")[1];
	if ( document.formEZQoS.enabled2.checked==true )
	{
		bandWidth_type0.disabled=false;
		bandWidth_type1.disabled=false;
	}
	else
	{
	  bandWidth_type0.disabled=true;
	  bandWidth_type1.disabled=true;
	}
}
function Disable_Bandwidth()
{
	var UpBand=document.formEZQoS.maxup;
	var DownBand=document.formEZQoS.maxdown;
	if (( document.getElementsByName("BandWidth_Type")[1].checked==true ) && ( document.formEZQoS.enabled2.checked==true ))
	{
		UpBand.disabled=false;
		DownBand.disabled=false;
	}
	else
	{
	  UpBand.disabled=true
	  DownBand.disabled=true;
	}
}
function mouseclick_item( obj )
{
	if( obj.innerHTML.toLowerCase() != EMPTY_TD_INNERHTML.toLowerCase())
	{
		//temp block pressed
		if( obj.id.substring(0,TEMP_TD_ID_PREFIX.length).toLowerCase() == TEMP_TD_ID_PREFIX.toLowerCase() )
		{
			REMOVING_PREFIX=TEMP_TD_ID_PREFIX
			ADDING_PREFIX=EZQOS_TD_ID_PREFIX
			OLD_SIZE=TEMP_PIC_SIZE
			NEW_SIZE=EZQOS_PIC_SIZE
		}
		//ezqos block pressed
		else if( obj.id.substring(0,EZQOS_TD_ID_PREFIX.length).toLowerCase() == EZQOS_TD_ID_PREFIX.toLowerCase() )
		{
			REMOVING_PREFIX=EZQOS_TD_ID_PREFIX
			ADDING_PREFIX=TEMP_TD_ID_PREFIX
			OLD_SIZE=EZQOS_PIC_SIZE
			NEW_SIZE=TEMP_PIC_SIZE
		}
		for(i=1; i<=5 && document.getElementById(ADDING_PREFIX+""+parseInt(i)).innerHTML.toLowerCase()!=EMPTY_TD_INNERHTML.toLowerCase() ; i++);
		if(i>5) return;
		temp = obj.innerHTML.substring(0,  obj.innerHTML.search(PIC_ALTNAME)+ PIC_ALTNAME.length+1 )
		document.getElementById(ADDING_PREFIX+""+i).innerHTML = temp +  NEW_SIZE +">"
		for(i=parseInt(obj.id.substring(REMOVING_PREFIX.length, obj.id.length)); i<=5; i++)
		{
			if(i==5)
			{
				document.getElementById(REMOVING_PREFIX + parseInt(i) ).innerHTML=EMPTY_TD_INNERHTML
			}
			else
			{
				obj1=document.getElementById(REMOVING_PREFIX + parseInt(i) )
				obj2=document.getElementById(REMOVING_PREFIX + (parseInt(i)+1) )
				obj1.innerHTML = obj2.innerHTML
			}
		}
		if ( document.getElementById("EZQOS_TD_1").innerHTML == "<a>&nbsp;</a>" || document.getElementById("EZQOS_TD_1").innerHTML == "<A>&nbsp;</A>")
		{
			//document.getElementById("apply_button").disabled = false;
			document.getElementById("save_button").disabled = false;
			//document.getElementById("apply_button").style.color="#ffffff";
	    document.getElementById("save_button").style.color="#ffffff";
			apply_lock=0;
		}
		else
		{
			if( document.getElementById("EZQOS_TD_5").innerHTML == "<a>&nbsp;</a>" ||  document.getElementById("EZQOS_TD_5").innerHTML == "<A>&nbsp;</A>" )
			{
				//document.getElementById("apply_button").disabled = true;
	      document.getElementById("save_button").disabled = true;
				//document.getElementById("apply_button").style.color=disableButtonColor;
	      document.getElementById("save_button").style.color=disableButtonColor;
				apply_lock=1;
			}			
			else
			{
				//document.getElementById("apply_button").disabled = false;
        document.getElementById("save_button").disabled = false;
				//document.getElementById("apply_button").style.color="#ffffff";
	      document.getElementById("save_button").style.color="#ffffff";
				apply_lock=0;
			}
		}
	}
}
function goToWeb() {
		if (document.formEZQoS.enabled2.checked==true)
				document.formEZQoSEnabled.enabled1.value="ON";
			document.formEZQoSEnabled.submit();
	}
function clickapply(rebootFlag)
{
	if(!apply_lock)
	{

		if ( document.formEZQoS.enabled2.checked==true ){
			if(parseInt(document.formEZQoS.maxdown.value,10) <= 0)
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(TotalDownloadBandwidth))+' '+showText(QoSBandwidtherror));
				return false;
			}
			if(parseInt(document.formEZQoS.maxup.value,10) <= 0)
			{
				alert(showText(InvalidSomething).replace(/#####/,showText(TotalUploadBandwidth))+' '+showText(QoSBandwidtherror));
				return false;
			}

			if (!portRule1(document.formEZQoS.maxdown, showText(TotalDownloadBandwidth), 1))
				return false;
			if (!portRule1(document.formEZQoS.maxup, showText(TotalUploadBandwidth) , 1))
				return false;

			document.EZqosApply.maxdown.value = parseInt(document.formEZQoS.maxdown.value,10);
			document.EZqosApply.maxup.value = parseInt(document.formEZQoS.maxup.value,10);
		}


		for(i=1; i<=5; i++)
		{
			if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("APP") != -1)
				document.EZqosApply.ezqos_APP.value = i-1;
			if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("MEDIA") != -1)
				document.EZqosApply.ezqos_MEDIA.value = i-1;
			if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("P2P") != -1)
				document.EZqosApply.ezqos_P2P.value = i-1;
			if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("FTP") != -1)
				document.EZqosApply.ezqos_FTP.value = i-1;
			if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("GAME") != -1)
				document.EZqosApply.ezqos_GAME.value = i-1;
		}

		with(document.EZqosApply) {
			if(rebootFlag)
			 isApply.value = "ok";
	
			submit();	
		}
	}
}
function initclickapply()
{
	if (!portRule(document.formEZQoS.maxdown, showText(TotalDownloadBandwidth), 0,"", 0, 99999, 1))
		return false;
	if (!portRule(document.formEZQoS.maxup, showText(TotalUploadBandwidth), 0,"", 0, 99999, 1))
		return false;

	document.EZqosApply.maxdown.value = document.formEZQoS.maxdown.value;
	document.EZqosApply.maxup.value = document.formEZQoS.maxup.value;

	for(i=1; i<=5; i++)
	{
		if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("APP") != -1)
			document.EZqosApply.ezqos_APP.value = i-1;
		if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("MEDIA") != -1)
			document.EZqosApply.ezqos_MEDIA.value = i-1;
		if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("P2P") != -1)
			document.EZqosApply.ezqos_P2P.value = i-1;
		if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("FTP") != -1)
			document.EZqosApply.ezqos_FTP.value = i-1;
		if(document.getElementById(EZQOS_TD_ID_PREFIX+i).innerHTML.search("GAME") != -1)
			document.EZqosApply.ezqos_GAME.value = i-1;
	}
	return true;
}
//========================================= EZQOS function	end


var entryNum = <%getVar("wan1QosNum","","");%>;
/* function for main */
var comm = new Array( new Array("com00","com01"),
						 new Array("com10","com11"),
						 new Array("com20","com21"),
						 new Array("com30","com31"))
function com_sw(cnum,com)
{
	if (com == 1)
	{
		document.getElementById(comm[cnum][0]).style.display = "none"
		document.getElementById(comm[cnum][1]).style.display = "block"
	}
	else
	{
		document.getElementById(comm[cnum][0]).style.display = "block"
		document.getElementById(comm[cnum][1]).style.display = "none"
	}
}
function init()
{
	$("#help-box").hide();
}
</SCRIPT>

</head>


<body class="" onLoad="init();">



			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1">
							<b><script>dw(iQoS)</script>&ensp;:</b><br>
							<script>dw(iQoSHelp)</script><br>
							<b><script>dw(CuriQoSTable)</script>&ensp;:</b><br>
							<script>dw(iQoStableHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="iQoS_fieldset" id="iQoS_fieldset">
			<legend><script>dw(iQoS)</script></legend>
		
				<form action="/goform/formEZQoS" method="POST" name="formEZQoSEnabled">
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="ON" name="isEnabled">
					<input type="hidden" value="" name="enabled1">
					<input type="hidden" value="" name="autoDetect1">
					<input type="hidden" value="" name="manual1">
					<input type="hidden" value="EZQoS" name="EZQos_Mode">
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="/iqos.asp" name="submitUrl">
				</form>

				<form action="/goform/formEZQoS" method="POST" name="formEZQoS">

					<br>

					<table width="700" cellpadding="0" cellspacing="1"  style="background: #FFFFFF;"  align="center">
						<tr>
							<td colspan="2"><span class="itemText">
								<script>dw(iQoSCont);</script>
							</span></td>
						</tr>
						<tr>
							<td colspan="2">
								&ensp;
							</td>
						</tr>
						<tr>
							<td colspan="2">  
							<input type="checkbox" name="enabled2" value="ON" <%getVar("EZQosEnabled","1","checked");%> onClick="goToWeb();">&nbsp;
              <span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(iQoS)</script></span></td>
						</tr>
						<tr>
							<td colspan="2">
								&ensp;
							</td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(TotalDownloadBandwidth);</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="maxdown" size="5" maxlength="5" value="<% getInfo("maxdownbandwidth"); %>">&nbsp;<span class="itemText"><script>dw(kbits);</script></span>
 							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(TotalUploadBandwidth);</script></div></td>
							<td><div class="itemText2-len">
                <input type="text" name="maxup" size="5" maxlength="5" value="<% getInfo("maxupbandwidth"); %>">&nbsp;<span class="itemText"><script>dw(kbits);</script></span>
              </div></td>
						</tr>
					</table>
					
					<br>

					<table width="600" border="0" cellpadding="0" cellspacing="0" align="center">
						<tr>
							<td colspan="0"><span class="itemText"><script>dw(CuriQoSTable);</script>&nbsp;:</span></td>
						</tr>
					</table>

					<table width="600" border="0" cellpadding="0" cellspacing="0" align="center">
						<tr align="center">
							<td>

								<table cellspacing="5" cellpadding="0" border="0" style="width:auto;" align="center">
									<tr>
										<td colspan="5" class="titletableText">
											<div style="float:left"><script>dw(High);</script></div>
											<div style="float:right"><script>dw(Low);</script></div>
										</td>
									</tr>

									<tr height="71">
										<% EZQosList(); %>
									</tr>

									<tr>
										<td colspan="5" align="center">
											<table cellspacing="5" cellpadding="0">
												<tr height="35">
													<td id="TEMP_TD_1" width="35" class="without_pic_temp" onClick="mouseclick_item(this)"><a>&nbsp;</a></td>
													<td id="TEMP_TD_2" width="35" class="without_pic_temp" onClick="mouseclick_item(this)"><a>&nbsp;</a></td>
													<td id="TEMP_TD_3" width="35" class="without_pic_temp" onClick="mouseclick_item(this)"><a>&nbsp;</a></td>
													<td id="TEMP_TD_4" width="35" class="without_pic_temp" onClick="mouseclick_item(this)"><a>&nbsp;</a></td>
													<td id="TEMP_TD_5" width="35" class="without_pic_temp" onClick="mouseclick_item(this)"><a>&nbsp;</a></td>
												</tr>
											</table>
										</td>
									</tr>
								</table>

							</td>
						</tr>
					</table>

					<div id="moving_img" style="height:0px;position:absolute;display:none;" onMouseDown="mousedown_item(this, event)"></div>
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="1" name="configWan">
					<input type="hidden" value="/iqos.asp" name="submitUrl">
				</form>

				<br>

				<form action="/goform/formEZQoS" method="POST" name="EZqosApply">
						<table width="700" border="0" cellpadding="" cellspacing="" align="center">
							<tr>
								<td height="20" valign="middle" align="center" class="">
									<script>

										document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" id="save_button" name="save" onclick="return clickapply(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return clickapply(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
									</script>
									<input type="hidden" value="/iqos.asp" name="submitUrl">
									<input type="hidden" value="" name="isApply">
									<input type="hidden" name="maxup">
									<input type="hidden" name="maxdown">
									<input type="hidden" name="ezqos_GAME">
									<input type="hidden" name="ezqos_P2P">
									<input type="hidden" name="ezqos_FTP">
									<input type="hidden" name="ezqos_MEDIA">
									<input type="hidden" name="ezqos_APP">
								</td>
							</tr>
						</table>
					<input type="hidden" value="1" name="iqsetupclose">
				</form>

	</fieldset>
			
				



<script>
	initclickapply();
	mouseclick_item(document.getElementById("EZQOS_TD_1"));
	mouseclick_item(document.getElementById("EZQOS_TD_1"));
	mouseclick_item(document.getElementById("EZQOS_TD_1"));
	mouseclick_item(document.getElementById("EZQOS_TD_1"));
	mouseclick_item(document.getElementById("EZQOS_TD_1"));
</script>

</blockquote>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>


</html>
