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

var dmzEnable = <%getVar("dmzEnabled","","");%>;
var dmzNum = <%getVar("dmzNum","","");%>;

function addClick()
{
	if (document.formFilterAdd.dmzType[1].checked == true) {
		if ( !ipRule( document.formFilterAdd.pip, showText(Public)+" "+showText(IPAddr), "ip", 0)) 
			return false;
	}
	if ( !ipRule( document.formFilterAdd.cip, showText(ClientPC)+" "+showText(IPAddr), "ip", 0))
		return false;
  return true;
}
function disableDelButton()
{
	document.formFilterDel.delSelDMZ.disabled = true;
	document.formFilterDel.delAllDMZ.disabled = true;
}
function goToWeb() {

	if (document.formFilterAdd.enabled.checked==true)
		document.formDMZEnabled.enabled.value="ON";
	document.formDMZEnabled.submit();
	
}
function goToApply() {
	document.filtApply.submit();
}
function disType() {
	if (document.formFilterAdd.dmzType[0].checked == true ) {
		document.formFilterAdd.session.disabled = false;
		document.formFilterAdd.pip.disabled = true;
	}
	else{
		document.formFilterAdd.session.disabled = true;
		document.formFilterAdd.pip.disabled = false;
	}
}
//Support NetBiosName add by Kyle 2007/12/12
var nameList = new Array(<% getInfo("getnetName");%> new Array("0","0","0"));
function addComputerName(){
		document.forms['formFilterAdd'].elements['cip'].value = document.forms['formFilterAdd'].elements['comList'].value;
}
function loadNetList(){
	for(i=0;i<nameList.length-1;i++){
		document.forms['formFilterAdd'].elements['comList'].options[i+1]=new Option (nameList[i][1], nameList[i][0]);
	}
	document.forms['formFilterAdd'].elements['comList'].options[i+1]=new Option ("----"+showText(refresh)+"----", "refresh");
}
function searchComName(ipAdr,type){
	//var comName="OFFLINE";
	var comName="–";
	for (i=0;i<nameList.length-1;i++){
		if(ipAdr==nameList[i][type]){
			comName=nameList[i][1];
			break;
		}
	}
	return comName;
}
function fresh(select) {
	if (select == "refresh")
		document.name_fresh.submit();
}
function init()
{
	$("#help-box").hide();

	//if( (dmzEnable == 1) && (dmzNum != 0) && ((nameList.length-1) == 0) )
	//	document.name_fresh.submit();
	
}
function saveChanges(rebootFlag)
{
	with(document.formDMZApply) {

		if(rebootFlag)
		 isApply.value = "ok";
		
	  submit();	
	}
}
</SCRIPT>

</head>

<body onLoad="init()">

			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(DMZHelp)</script>
					</div></td></tr>
				</table>
			</div>
			<script>
			HelpButton();
			</script>



<blockquote>

	<fieldset name="IGMP_fieldset" id="IGMP_fieldset">
		<legend><script>dw(DMZ)</script></legend>

				<form action="/goform/formFilter" method="POST" name="formDMZEnabled">
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="Add" name="addDMZ">
					<input type="hidden" value="" name="enabled">
					<input type="hidden" value="" name="isApply">
					<input type="hidden" value="/fw_dmz.asp" name="submit-url">
				</form>

				<form action="/goform/formFilter" method="POST" name="formFilterAdd">

					<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
						<tr>
							<td width="200" height="20" valign="middle" align="left" class="">
								<input type="checkbox" name="enabled" value="ON" <% getVar("dmzEnabled","1","checked");%> onClick="goToWeb();" >&nbsp;
								<span class="itemText"><script>dw(enable)</script>&ensp;<script>dw(DMZ)</script></span>
							</td>
						</tr>
					</table>
					<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
						<tr class="itemText">
							<td width="250" align="center"><script>dw(Public)</script></td>
							<td width="150" align="center"><script>dw(ClientPC)</script></td>
							<td width="200" align="center"><script>dw(Computername)</script></td>
						</tr>
						<tr>
							<td height="20" valign="middle" align="left" class="">
								<input type="radio" name="dmzType" value="0" checked onClick="disType();"><span class="choose-itemText "><script>dw(DynamicIP);</script></span>
								<select name="session">
									<script>
										for (i=1; i<=1; i++) {
										if (i == 1)
										document.write('<option selected value="'+i+'">'+showText(Session)+'&ensp;'+i+'</option>');
										else
										document.write('<option value="'+i+'">'+showText(Session)+'&ensp;'+i+'</option>');
										}
									</script>
								</select>
							</td>
							<td rowspan="2" height="20" valign="middle" align="center" class="">
								<input type="text" name="cip" size="15" maxlength="15">
							</td>
							<td rowspan="2" height="20" valign="middle" align="center" class="">
								<input type="button" value="<<" name="addCom" onclick="addComputerName()" class="ui-smal-button">
								<select size="1" name= "comList" onchange="fresh(this.value)" style="width: 130">
									<option value="0.0.0.0">----<script>dw(select)</script>----</option>
									<option value="refresh">----<script>dw(refresh)</script>----</option>
								</select>
							</td>
						</tr>
						<tr>
							<td height="20" valign="middle" align="left" class="">
								<input type="radio" name="dmzType" value="1" onClick="disType();"><span class="choose-itemText "><script>dw(StaticIP);</script></span>
								<input type="text" name="pip" size="15" maxlength="15">
							</td>
						</tr>
						<tr>
							<td colspan="3" height="20" valign="middle" align="right" class="">
								<script>
									document.write('<input type="submit" value="'+showText(add)+'" name="addDMZ" onClick="return addClick()" class="ui-button-nolimit">&nbsp;');
								</script>
								<input type="hidden" value="" name="isApply">
								<input type="hidden" value="/fw_dmz.asp" name="submit-url">
							</td>
						</tr>
					</table>
					<input type="hidden" value="1" name="iqsetupclose">
				</form>

				<br>

		    <form action="/goform/formFilter" method="POST" name="formFilterDel">

					<table width="700" border="0" cellpadding="2" cellspacing="1" align="center">
						<tr>
							<td width="200" height="20" valign="middle" align="left" class=""><span class="choose-itemText "><script>dw(CurDMZTable)</script>&nbsp;:</span></td>
						</tr>
					</table>

		     <table width="700" cellpadding="2" cellspacing="2" align="center" border="0"  class="footBorder">
		        <tr align="center" class="dnsTableHe">
		          <td width="10%"><script>dw(NO)</script></td>
		          <td width="30%"><script>dw(Computername)</script></td>
		          <td width="25%"><script>dw(Public)</script>&ensp;<script>dw(IPAddr)</script></td>
		          <td width="25%"><script>dw(ClientPC)</script>&ensp;<script>dw(IPAddr)</script></td>
		          <td width="10%"><script>dw(select)</script></td>
		        </tr>
		        <% DMZList(); %>
	       	</table>
						
						<table border="0" width="700" align="center">
							<tr>
								<td>&ensp;</td>
							</tr>
							<tr>
				      	<td colspan="5" align="right">
				          <script>document.write('<input type="submit" value="'+showText(deleteSelected)+'" name="delSelDMZ" onClick="" class="ui-button-nolimit">');</script>&nbsp;
				          <script>document.write('<input type="submit" value="'+showText(deleteAll)+'" name="delAllDMZ" onClick="return deleteAllClick()" class="ui-button-nolimit">');</script>
				          <script>//document.write('<input type="reset" value="'+showText(Reset)+'" name="reset" class="button">');</script>
				          <input type=hidden value="" name="isApply">
									<input type="hidden" value="/fw_dmz.asp" name="submit-url">
									<script>
										var entryNum = <%getVar("dmzNum","","");%>;
										if ( entryNum == 0 ) 
										{
											document.formFilterDel.delSelDMZ.disabled = true;
											document.formFilterDel.delAllDMZ.disabled = true;
									
											document.formFilterDel.delSelDMZ.style.color=disableButtonColor;
											document.formFilterDel.delAllDMZ.style.color=disableButtonColor;
										}
										if ( entryNum >= 20 ) 
										{
											document.formFilterAdd.addDMZ.disabled = true;
											document.formFilterAdd.addDMZ.style.color=disableButtonColor;
										}
							 		</script>
								</td>
							</tr>
					</table>
					<input type="hidden" value="1" name="iqsetupclose">
		    </form>

				<br>

				<form action="/goform/formFilter" method="POST" name="formDMZApply">
					<table align="center" border="0" cellspacing="0" cellpadding="0">
						<tr>
							<td style="text-align:center; padding-top:30px;">
								<script>
									//document.write('<input type="submit" value="'+showText(Apply)+'" name="new_apply" class="ui-button">')
									document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
								</script>
								<input type="hidden" value="" name="isApply">
								<input type="hidden" value="/fw_dmz.asp" name="submit-url">
							</td>
						</tr>
					</table>
					<input type="hidden" value="1" name="iqsetupclose">
				</form>

				 <form action="/goform/formrefresh" method="POST" name="name_fresh">
          <input type="hidden" value="" name="isApply">
					<input type="hidden" value="/fw_dmz.asp" name="submit-url">
				</form>					
	</fieldset>

<script>
	$("#help-box").hide();
	loadNetList();
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
</html>
