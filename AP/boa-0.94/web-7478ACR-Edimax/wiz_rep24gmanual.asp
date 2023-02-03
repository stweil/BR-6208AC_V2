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
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>

<!-- hidden ssid start 
<style type="text/css">
	#out{width:300px;height:20px;background:#FFFFFF;}
	#in{width:0px; height:20px;background:#FFFFFF;color:#FFFFFF;text-align:center; FONT-SIZE: 11pt; FONT-FAMILY: Arial;}
	.background{	background-color:#EEEEEE;	color:#000000;	font-family:Arial, Helvetica; }
</style>
 hidden ssid end -->

<script type="text/javascript">
var ssidTbl=<%getInfo("ssidAll");%>;
var repeaterSSIDAll=<%getInfo("repeaterSSIDAll");%>;
var repeaterssid =	repeaterSSIDAll[0];
var rep_EnableCrossBand = "<%getInfo("rep_enableCrossBand");%>";
var back2survy=0, gonextEnable=0;
var hiddenssid = <%getVar("hiddenSSID","","");%>;
var hiddenssid5g = <%getiNICVar("hiddenSSID","","");%>;

function GetElementType(name,idx) { return document.getElementsByName(name)[idx]; }
function GetIDType(name) { return document.getElementById(name); }

function changeAll() { 
	var wlan1SSID=GetElementType("manualssid",0).value;
	
	if(wlan1SSID!="")
	{
		/*if(rep_EnableCrossBand == "1")
			GetElementType("manualdevssid",0).value = wlan1SSID.substr(0,28); 
		else
			GetElementType("manualdevssid",0).value = wlan1SSID.substr(0,28); */
		gonextEnable=1;
		ButtonDoneEnable();
	}
	else
	{
		GetElementType("manualdevssid",0).value="";
		gonextEnable=0;
		ButtonDoneEnable();
	}
}

function init_changeAll() { 
	//GetElementType("manualssid",0).value=repeaterssid;
	var rootAP_name=GetElementType("manualssid",0).value;

	GetElementType("manualdevssid",0).value = "";
	GetElementType("manualdevssid5g",0).value = "";
	
	if(rootAP_name != "")
	{
		//GetElementType("manualdevssid",0).value = wlan1SSID.substr(0,28);
		
		gonextEnable=1;
		ButtonDoneEnable();
	}
	else
	{
		//GetElementType("manualdevssid",0).value="";
		//GetElementType("manualdevssid",0).value = "";
		gonextEnable=0;
		ButtonDoneEnable();
	}
}


function ButtonDoneEnable() {
	if(gonextEnable)
	{
		GetElementType("next",0).disabled = false;
		document.formWizSurvey.done.value="1";
	}
	else
	{
		GetElementType("next",0).disabled = true;
		document.formWizSurvey.done.value="";
	}
}

function checkSSID(input) {

	var x = input
	if (x.value != "")
	{
		if (x.value.indexOf("$") != -1 || x.value.indexOf(",") != -1 || x.value.indexOf(';') != -1 || x.value.indexOf("\\") != -1)
		{
			alert(showText(SSIDNoSupport));
			x.focus();
			return false
		}
		else
			return true
	}
	else
		return true
}

function gonext() {

	var i,j;
	var WlencryptFrame;

	//if(GetIDType("wlan_on").checked == true)
	//{
		WlencryptFrame = window.frames["wlencrypt"];

		if( !WlencryptFrame.saveChanges())
			return false;

		if ( GetElementType("manualssid",0).value == "" ) {
			alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(SSID)));
			return false;
		}
		else if (!checkSSID(document.rep24g.manualssid))
		{
			return false;
		}		
		else {
			security=WlencryptFrame.document.wlEncrypt.method.value;	

			GetIDType("method").value = security;
			GetIDType("repeaterSSID").value = GetElementType("manualssid",0).value;
			GetIDType("ssid").value = GetElementType("manualdevssid",0).value;
			GetIDType("ssid5g").value = GetElementType("manualdevssid5g",0).value;

			if(security == 2 || security == 3){	

				for(i=0; i<3; i++)
					if( WlencryptFrame.document.getElementsByName("wpaCipher")[i].checked ) {Cipher=i+1;break;}

				wpaPskFormat = WlencryptFrame.document.wlEncrypt.wpaPskFormat.value;

				GetIDType("wpaCipher").value = Cipher; // 1:TKIP 2:AES
				GetIDType("wpaPskValue").value = WlencryptFrame.document.wlEncrypt.wpaPskValue.value;
				GetIDType("wpaPskFormat").value = wpaPskFormat;
			}
			if (security == 1){	
				GetIDType("wepEnabled").value = "ON";
				GetIDType("length").value = WlencryptFrame.document.wlEncrypt.length.value;
				GetIDType("format").value = WlencryptFrame.document.wlEncrypt.format.value;
				GetIDType("key1").value = WlencryptFrame.document.wlEncrypt.key1.value;
				//alert(GetIDType("length").value+" "+GetIDType("format").value+" "+GetIDType("key1").value);
			}
		}
	//}
	GetIDType("wlan1Enable").value = "1";
	document.formWizSurvey.doConnectTest.value="1";
	GetIDType("chan").value = GetElementType("channelno",0).value;

	document.formWizSurvey.submit();
	/* hidden ssid start 
	$.ajax({
	  url: 'goform/formWlbasic',
	  type: 'POST',
	  data: $('#Wlbasic').serialize(),
	  async: true,
	  dataType:'text',

	  success: function () {
				periodCheck();
	  },
	  error: function(){
				periodCheck();
	  }
	});
	 hidden ssid end */
}

function goBack()
{
	window.location.assign("wiz_rep24gauto.asp");
}
function loadonstart() {
//	GetElementType("HideSSIDclick",0).checked = true;
	document.getElementById("survyinfo").innerHTML='<span class="Arial_16" style="line-height:23px;">'+showText(manualSSIDinfo)+'</span>';
}
function setTableWidth()
{
	languIndex=stype;
	if(stype>13) languIndex=13;

	document.getElementById("tablewidth").className="tablewidth"+languIndex;
	document.getElementById("survyinfo").className="tablewidth"+languIndex;
}
function setHideSSID()
{
	if (document.rep24g.HideSSIDclick.checked==true){	//BroadCast SSID on
		GetIDType("hiddenSSID").value = "yes";
	}else{
		GetIDType("hiddenSSID").value = "no";
	}
	if (document.rep24g.HideSSIDclick5g.checked==true){	//BroadCast SSID on
		GetIDType("hiddenSSID5g").value = "yes";
	}else{
		GetIDType("hiddenSSID5g").value = "no";
	}
}

/* hidden ssid start 
var setprogressNum;
var EearlyStopcheck;
function progressBar()
{
	progressNum=parseInt(($("#in").width()/300) * 100, 10);
	if(progressNum == 0) progressNum=1;
	document.getElementById("progress").innerHTML=progressNum+"%";
}
function EearlyStop_Check(){
	$.post("/goform/formTestResult", $("#checkTest").serialize(),function(data){

		if(data=="GetIP" || data=="SSID_no_exist" || data=="connectTestdone"){
			window.clearInterval(setprogressNum);
			window.clearInterval(EearlyStopcheck);
			$("#in").stop().animate({width: '90%'},function(){
				document.getElementById("progress").innerHTML="99%";
				$("#in").animate({width: '100%'}, 500,function(){ 
					$.post("/goform/formTestResult", $("#checkIF").serialize(),function(data){
						if(data=="2") window.location.assign("/wiz_ip.asp");
						else window.location.assign("/wiz_ip5g.asp");
					},"text");
				});
			});
		}
	},"text");
}
function periodCheck()
{
	document.getElementById("FirsStep").style.display='none';
	document.getElementById('SecStep').style.display='block';

	setprogressNum=setInterval("progressBar()", 1000);
	$("#in").animate({width: '99%'}, 25000,function(){	
		$.post("/goform/formTestResult", $("#checkIF").serialize(),function(data){
			window.clearInterval(EearlyStopcheck);
			window.clearInterval(setprogressNum);
			if(data=="2") window.location.assign("/wiz_ip.asp");
			else window.location.assign("/wiz_ip5g.asp");
		},"text");
	});

	EearlyStopCheck = setInterval("EearlyStop_Check()", 1000);
}
 hidden ssid end */
</script>
<style type="text/css">
.tablewidth0 {width:90%;}
.tablewidth1 {width:90%;}
.tablewidth2 {width:90%;}
.tablewidth3 {width:98%;}
.tablewidth4 {width:90%;}
.tablewidth5 {width:90%;}
.tablewidth6 {width:90%;}
.tablewidth7 {width:90%;}
.tablewidth8 {width:90%;}
.tablewidth9 {width:98%;}
.tablewidth10 {width:90%;}
.tablewidth11 {width:90%;}
.tablewidth12 {width:90%;}
.tablewidth13 {width:90%;}
</style>
</head>
<body onLoad="setTableWidth();loadonstart()">
<form action="/goform/formWizSurvey" method="POST" name="rep24g">
<input type="hidden" value="" name="totallist" id="totalList">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
<tr>
<td align="center">
	
	<table border="0" style="height:90px;width:700px" align="center" cellspacing="0">
		<tr>
		<td align="right" class="Arial_16">
			<script>
				dw(repeatermode);
			</script>
		</td>
		</tr>
	</table>
	<!-- hidden ssid start 
	<span id='FirsStep' style="display:block">
 hidden ssid end -->
		<table border="0" style="height:20px;width:780px" align="center" cellspacing="0">
			<tr>
				<td align="center"><span class="Arial_16" style="font-weight:bold;">2.4GHz&nbsp;
					<script>dw(WirelessSiteSurvey)</script></span>
				</td>
			</tr>
		</table>
		<table border="0" style="height:90px;" id="tablewidth" class="tablewidth0" align="center" cellspacing="0">
			<tr>
				<td align="left" id="survyinfo" class="tablewidth0">
				</td>
			</tr>
		</table>

		<table border="0" style="height:50px;width:780px" align="center" cellspacing="0">
			<tr>
				<td style="width:294px" align="right" class="Arial_16"><script>dw(wiz_repmenu1)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
				    <input type="text" name="manualssid" id="manualssid" onkeyup="changeAll()" size="23" maxlength="32" value="" class="Arial_16">
				</td>
			</tr>
			
			<tr>
				<td style="width:294px" align="right" class="Arial_16">2.4GHz&nbsp;<script>dw(wiz_repmenu3)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
				    <input type="text" name="manualdevssid" size="23" maxlength="32" value="" class="Arial_16">
				     &nbsp;
				    <script>
					    if(hiddenssid==1)
						    document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16" checked>');   
					    else
						    document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16" >');		
				    </script>	
				    <span style="font-size:14px;"><script>dw(HideSSID)</script></span>
				</td>
			</tr>
			<tr>
				<td style="width:294px" align="right" class="Arial_16">5GHz&nbsp;<script>dw(wiz_repmenu3)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
				    <input type="text" name="manualdevssid5g" size="23" maxlength="32" value="" class="Arial_16">
				     &nbsp;
				    <script>
					    if(hiddenssid5g==1)
						    document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick5g" class="Arial_16" checked>');   
					    else
						    document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick5g" class="Arial_16" >');		
				    </script>	
				    <span style="font-size:14px;"><script>dw(HideSSID)</script></span>
				</td>
			</tr>
		</table>
		<table border="0" style="height:20px;width:780px" align="center" cellspacing="0">
			<!--<tr>
				<td style="width:294px" align="right" class="Arial_16"><script>dw(HideSSID)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
					<script>
						if(hiddenssid==1)
							document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16" checked>');   
						else
							document.write('<input type="checkbox" onclick="setHideSSID();" name="HideSSIDclick" class="Arial_16" >');		
					</script>			
					<script>dw(enable)</script>
				</td>
			</tr>-->

			<tr>
				<td style="width:294px" align="right" class="Arial_16"><script>dw(ChannelNum)</script>&nbsp;&nbsp;</td>
				<td style="width:390px">
						<select size="1" name="channelno" class="Arial_16">
							<script>
								var regDomain = <%getVar("regDomain","","")%>;
		            var defaultChan = <%getVar("channel","","")%>;
		            var domainMin = new Array("1","1","10","10","14","1","3","5","1");
		            var domainMax = new Array("11","13","11","13","14","14","9","13","14");
						
								/*if( defaultChan == 0)
									document.write('<option selected value="0">'+showText(AutoChen)+'</option>');
								else
									document.write('<option value="0">'+showText(AutoChen)+'</option>');*/

								for (i=0; i<=8; i++) {
									if ( i == regDomain ) {
										for (j=parseInt(domainMin[i],10); j<=parseInt(domainMax[i],10); j++) {
											/*if ( j==defaultChan)
												document.write('<option selected value="'+j+'">'+j+'</option>');
											else*/
												document.write('<option value="'+j+'">'+j+'</option>');
										}
									}
								}
							</script>
						</select>
					</td>
			</tr>

		</table>

		<table border="0" style="width:780px;" align="center" cellspacing="0">
			<tr>
				<td align="center">
					<iframe id="wlencrypt" name="wlencrypt" src="/wiz_rep24gsecurity.asp" style="height:155px;width:600px;" scrolling="no" frameBorder="0"></iframe>
				</td>
			</tr>
		</table>

		<table border="0" style="height:30px;width:780px" align="center" cellspacing="0">
			<tr><td></td></tr>
		</table>


		<table border="0" style="height:28px;width:780px" align="center" cellspacing="0">
			<tr>
			<td align="right" style="width:390px">
			<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');</script>
			</td>
			<td align="left" style="width:390px">
			<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="next" onClick="return gonext()" class="button_IQ2" style="cursor:pointer">');</script>
			</td>
			</tr>
		</table>

		<table border="0" style="height:40px;width:780px" align="center" cellspacing="0">
			<tr><td></td></tr>
		</table>
	<!-- hidden ssid start 
	</span>

	<span id='SecStep' style="display:none">
		<table border="0" style="height:85px;width:700px" align="center">
			<tr><td></td></tr>
		</table>
		<table border="0" style="width:790px" align="center">
			<tr>
				<td align="center">
					<span class="Arial_20" style="font-weight:bold;"><script>dw(ConnectionTest24);</script></span>
				</td>
			</tr>
			<tr><td align="center" id="progress" class="Arial_20" style="font-weight:bold;">1%</td></tr>
			<tr><td align="center"><img src="graphics/loading.gif"></td></tr>
		</table>
		<table border="0" style="height:190px;width:700px" align="center">
			<tr><td></td></tr>
		</table>
	
		<form id="checkTest">
			<input type="hidden" name="cmd" value="checkWlanTest">
		</form>
		<form id="checkIF">
			<input type="hidden" name="cmd" value="checkWlanIF">
		</form>
	</span>
 hidden ssid end -->
</td>
</tr>
</table>
</div>
</form>
<!-- hidden ssid start 
<table width="600" border=0 align="left">
	<tr>
		<td style="width:300px;">
			<div id='out'>
				<div id="in" style="width:0%;"></div>
			</div>
		</td>
	</tr>
</table>
 hidden ssid end -->
 
<form action="/goform/formWlbasic" method="POST" name="formWizSurvey" id="Wlbasic">
	<table width="780" border="0" cellspacing="0" align="center">
	<!-- assign next page -->
	<input type="hidden" value="/connect.asp" id="submit-url" name="submit-url">
	<!-- operation mode -->
	<input type="hidden" value="0" name="wlan1Enable" id="wlan1Enable">
	<input type="hidden" value="6" name="apMode" id="apMode">
	<input type="hidden" value="0" name="wisp">
	<input type="hidden" value="1" name="force_enable2g5g_ap">
	<!-- This device SSID information -->
	<input type="hidden" value="no" name="hiddenSSID" id="hiddenSSID">
	<input type="hidden" value="no" name="hiddenSSID5g" id="hiddenSSID5g">
	<input type="hidden" value="" name="repeaterSSID" id="repeaterSSID">
	<input type="hidden" value="" name="ssid" id="ssid">
	<input type="hidden" value="" name="ssid5g" id="ssid5g">
	<!-- Channel information -->
	<input type="hidden" value="1" name="chan" id="chan">
	<input type="hidden" value="0" name="secchan" id="secchan">
	<!-- Security Method -->
	<input type="hidden" value="0" name="supportAllSecurity">
	<input type="hidden" value="" name="method" id="method">
	<!-- WPA Security -->
	<input type="hidden" value="" name="wpaCipher" id="wpaCipher">
	<input type="hidden" value="" name="wpaPskFormat" id="wpaPskFormat">
	<input type="hidden" value="" name="wpaPskValue" id="wpaPskValue">
	<input type="hidden" value="" name="wpa2PskFormat" id="wpa2PskFormat">
	<input type="hidden" value="" name="wpa2PskValue" id="wpa2PskValue">
	<!-- WEP Security -->
	<input type="hidden" value="" name="wepEnabled" id="wepEnabled">
	<input type="hidden" value="" name="length" id="length">
	<input type="hidden" value="" name="format" id="format">
	<input type="hidden" value="1" name="defaultTxKeyId">
	<input type="hidden" value="" name="key1" id="key1">
	<!-- Call connect test function -->
	<input type="hidden" value="" name="doConnectTest" id="doConnectTest">
	<!-- setting -->
	<input type="hidden" value="" name="done" id="Done">
	<input type="hidden" value="" name="isApply">
	
</form>
<script>
init_changeAll();
setHideSSID();
</script>
</body>

<head>
<meta http-equiv="Pragma" content="no-cache">
</head>

</html>
