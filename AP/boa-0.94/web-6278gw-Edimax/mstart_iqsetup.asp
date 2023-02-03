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
var bandindex = <% getInfo("stadrv_band"); %>;

function displayObj()
{
	var i=document.wisp_form.stadrv_encrypttype.value;
	switch(i)
	{
		case "0":
			document.getElementById("WEP").style.display = "none";
			document.getElementById("WPA").style.display = "none";
			break;
		case "1":
			document.getElementById("WEP").style.display = "block";
			document.getElementById("WPA").style.display = "none";
			break;
		case "2":
			document.getElementById("WEP").style.display = "none";
			document.getElementById("WPA").style.display = "block";
			break;
		default: break;
	}
	lengthClick();
}
function lengthClick()
{
	updateFormat();
}
function updateFormat()
{
  if (document.wisp_form.stadrv_weplength.selectedIndex == 0)
  {
		document.wisp_form.stadrv_wepformat.options[0].text = showText(ascii_5cha);
		document.wisp_form.stadrv_wepformat.options[1].text = showText(Hex_10cha);
  }
  else if (document.wisp_form.stadrv_weplength.selectedIndex == 1)
  {
		document.wisp_form.stadrv_wepformat.options[0].text = showText(ascii_13cha);
		document.wisp_form.stadrv_wepformat.options[1].text = showText(hex_26cha);
  }

  setDefaultKeyValue();
}
function wlValidateKey(idx, str, len)
{
	if (str.length ==0) return 1;
	if ( str.length != len) {
		idx++;
		alert(showText(InvalidSomething).replace(/#####/,showText(keyLen))+' '+showText(shouldbeSomething).replace(/####/,""+len+" "+showText(characters)));
		return 0;
	}
	if ( str == "*****" ||
		str == "**********" ||
		str == "*************" ||
		str == "****************" ||
		str == "**************************" ||
		str == "********************************" )
		return 1;
	if (document.wisp_form.stadrv_wepformat.selectedIndex==0)
		return 1;
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*' )
			continue;
		alert(showText(InvalidSomething).replace(/#####/,showText(KeyFormat))+' '+showText(ItShouldBeHEX));
		return 0;
	}
	return 1;
}

function setDefaultKeyValue()
{
	//var WEP64Key1 = "<% getInfo("WEP64Key1_sta");%>"
	//var WEP128Key1 = "<% getInfo("WEP128Key1_sta");%>"

	with(document.wisp_form) {
		if (stadrv_weplength.selectedIndex == 0) {
			if (stadrv_wepformat.selectedIndex == 0) {
				key1.maxLength = 5;
				//key1_1.maxLength = 5;  //text
				key1.value = "*****";  //password
				//key1_1.value =WEP64Key1.substring(0,5);
			}
			else {
				key1.maxLength = 10;
				//key1_1.maxLength = 10;
				key1.value = "**********";
				//key1_1.value = WEP64Key1;
			}
		}
		else if (stadrv_weplength.selectedIndex == 1) {
			if (stadrv_wepformat.selectedIndex == 0) {
				key1.maxLength = 13;
				//key1_1.maxLength = 13;
				key1.value = "*************";
				//key1_1.value = WEP128Key1.substring(0,13);
			}
			else {
				key1.maxLength = 26;
				//key1_1.maxLength = 26;
				key1.value = "**************************";
				//key1_1.value = WEP128Key1;
			}
		}
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
	//with(document.wisp_form) {
	 	//if (stadrv_type[0].checked==true)
		if (enable == 0)
		{
			document.wisp_form.stadrv_weplength.disabled=true;
			document.wisp_form.stadrv_wepformat.disabled=true;
			document.wisp_form.key1.disabled=true;
			
				
			document.wisp_form.stadrv_wpacipher[0].disabled=true;
			document.wisp_form.stadrv_wpacipher[1].disabled=true;
			document.wisp_form.stadrv_pskformat.disabled=true;
			document.wisp_form.stadrv_pskkey.disabled=true;

			
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
			document.wisp_form.stadrv_weplength.disabled=false;
			document.wisp_form.stadrv_wepformat.disabled=false;
			document.wisp_form.key1.disabled=false;

			document.wisp_form.stadrv_wpacipher[0].disabled=false;
			document.wisp_form.stadrv_wpacipher[1].disabled=false;
			document.wisp_form.stadrv_pskformat.disabled=false;
			document.wisp_form.stadrv_pskkey.disabled=false;

			document.wisp_form.stadrv_ssid.disabled=false;
			//stadrv_chan.disabled=false;
			document.wisp_form.selSurvey.disabled=false;
			document.wisp_form.stadrv_band[0].disabled=false;
			document.wisp_form.stadrv_band[1].disabled=false;
			document.wisp_form.stadrv_chan.disabled=false;
			document.wisp_form.stadrv_encrypttype.disabled=false;
		}
	//}
}
function setPskKeyValue()
{
	if (document.wisp_form.stadrv_pskformat.selectedIndex == 0)
	{
		document.wisp_form.stadrv_pskkey.maxLength = 63;
		document.wisp_form.stadrv_pskkey.value=document.wisp_form.stadrv_pskkey.value.substr(0,63);
	}
		else document.wisp_form.stadrv_pskkey.maxLength = 64;
}


var cloud_sta = "<% getInfo("cloud-sta"); %>"
var tcpipwanmac = "<% getInfo("wanMac"); %>"
var macaddrvalue = "<% getInfo("cloneMac"); %>"
var dyiphostname = "<% getInfo("dynIPHostName"); %>"
var valdns1 = "<% getInfo("wan-dns1"); %>"
var valdns2 = "<% getInfo("wan-dns2"); %>"
var valdns3 = "<% getInfo("wan-dns3"); %>"
var internet_sta = "<% getInfo("sta-current"); %>"
var DUAL_WAN_IGMP="<% getInfo("DUAL_WAN_IGMP"); %>"

var pptp_IpMode=<%getVar("pptpIpMode","","");%>;
var l2tp_IpMode=<%getVar("L2TPIpMode","","");%>;


var wizardEnabled = <%getVar("wizardEnabled","","");%>;
var wanmod = <%getVar("wanMode","","");%>;
var dnsmode = <%getVar("wanDNS","","");%>;
var ttlval = <%getVar("pppEnTtl","","");%>;
var dualaccessSW = <%getVar("isDuallAccessEnable","","");%>;
var duallAccessType = <%getVar("duallAccessMode","","");%>;


function switch_mode(mode)
{
	
	var item_array = new Array
	(
		document.getElementById("dynamicip_fieldset"), document.getElementById("staticip_fieldset"),
		document.getElementById("pppoe_fieldset"), document.getElementById("pptp_fieldset"),
		null, null,
		document.getElementById("l2tp_fieldset"), document.getElementById("wisp_fieldset")
	)

	

	for(i=0; i<item_array.length; i++)
	{
		if(item_array[i])
		{
			if(i==mode)
			{
				item_array[i].style.display="block";
				resetform(i);
			}
			else
			{
				item_array[i].style.display="none";
			}
		}
	}
}

function copyto(dom)
{
	dom.value = macaddrvalue;
}

function autoDNSClicked(dnsmodesw)
{
	if(dnsmodesw.DNSMode[0].checked == true)
	{
		dnsmodesw.dns1.disabled = true;
		dnsmodesw.dns2.disabled = true;
		dnsmodesw.dns3.disabled = true;
	}
	else
	{ 
		dnsmodesw.dns1.disabled = false;
		dnsmodesw.dns2.disabled = false;
		dnsmodesw.dns3.disabled = false;
	}
}


var pppConnectStatus=0;
function setPPPConnected()
{
  pppConnectStatus = 1; // pppoe pptp l2tp  1:now is connect  else:now is disconnect
}

function displayUpdate()
{
	with(document.ppp) {
		if (duallAccessMode[0].checked==true){
			document.getElementById('dynamicBtId').style.display="block";
			document.getElementById('staticBtId').style.display="none";
		}
		else if (duallAccessMode[1].checked==true){
			document.getElementById('dynamicBtId').style.display="none";
			document.getElementById('staticBtId').style.display="block";
		}

		if(enableDuallAccessppp.checked==true)
			document.getElementById('DualAccessEnableBtId').style.display="block";
		else
			document.getElementById('DualAccessEnableBtId').style.display="none";

		if(enableDuallAccessppp.checked==true)
		{
			if (duallAccessMode[0].checked==true)   //DynamicIP
			{
				macAddr.disabled=true;
				Clone.disabled=true;
				Clone.style.color=disableButtonColor;
			}
			else       // StaticIP
			{
				macAddr.disabled=false
				Clone.disabled=false
				Clone.style.color='#ffffff';
			}
		}
		else
		{
			macAddr.disabled=false
			Clone.disabled=false
			Clone.style.color='#ffffff';
		}
	}
}


function autoIpClicked(ipmodesw)
{
	if(ipmodesw.IpMode[0].checked==true)   //Obtain an IP address automatically
	{
		ipmodesw.IPaddr.disabled=true;
		ipmodesw.IPMaskAddr.disabled=true;
		ipmodesw.DfGateway.disabled = true;
		ipmodesw.macAddr1.disabled = true;
		ipmodesw.Clone1.disabled = true;
		ipmodesw.Clone1.style.color=disableButtonColor;

		ipmodesw.HostName.disabled = false;
		ipmodesw.macAddr.disabled = false;
		ipmodesw.Clone.disabled = false;
		ipmodesw.Clone.style.color='#ffffff';
		ipmodesw.DNSMode[0].disabled = false;
	}
	else       //Use the following IP address
	{
		ipmodesw.IPaddr.disabled=false;
		ipmodesw.IPMaskAddr.disabled=false;
		ipmodesw.DfGateway.disabled = false;
		ipmodesw.macAddr1.disabled = false;
		ipmodesw.Clone1.disabled = false;
		ipmodesw.Clone1.style.color='#ffffff';

		ipmodesw.HostName.disabled = true;
		ipmodesw.macAddr.disabled = true;
		ipmodesw.Clone.disabled = true;
		ipmodesw.Clone.style.color=disableButtonColor;
		ipmodesw.DNSMode[0].checked = false;
		ipmodesw.DNSMode[0].disabled = true;
		ipmodesw.DNSMode[1].checked = true;
		autoDNSClicked(ipmodesw);
	}
}


function saveChanges(who, rebootFlag, connect)
{
	switch(who)
	{
		case 0:
			with(document.dip) {
					if ( macAddr.value == "" ) macAddr.value="000000000000";
					var str = macAddr;
					if ( !macRule(str, 1)) return false;

					if(DNSMode[1].checked == true)
					{
						if (( dns1.value == "" ) || ( dns1.value == "0.0.0.0" )) 
						{
							alert(showText(InvalidSomething).replace(/#####/, showText(DNS)));
							return false;
						}
						if ( !ipRule( dns1, ""+showText(DNS)+"1 "+showText(address)+"", "ip", 1)) return false;
						if ( !ipRule( dns2, ""+showText(DNS)+"2 "+showText(address)+"", "ip", 1)) return false;
						if ( !ipRule( dns3, ""+showText(DNS)+"3 "+showText(address)+"", "ip", 1)) return false;
					}

					if (!portRule(dhcpMTU,showText(MTU), 0, "", 576, 1500, 1)) return false;
		
					if(rebootFlag)
						isApply.value = "ok";

					submit();
				}
			break;
		case 1:
				with(document.sip) {
					if ( !ipRule( ip, showText(IPAddr), "ip", 1)) return false;
					if ( !maskRule(mask, showText(SubnetMask), 1)) return false;
					if ( !ipRule( gateway, showText(gateAddr), "gw", 1)) return false;
					if ( !subnetRule(ip, mask, gateway, showText(gateAddr), showText(IPAddr))) return false;
					if ( !macRule(macAddr, 1)) return false;

					if (( dns1.value == "" ) || ( dns1.value == "0.0.0.0" )) 
					{
						alert(showText(InvalidSomething).replace(/#####/, showText(DNS)));
						return false;
					}
					if ( !ipRule( dns1, ""+showText(DNS)+"1 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns2, ""+showText(DNS)+"2 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns3, ""+showText(DNS)+"3 "+showText(address)+"", "ip", 1)) return false;

					if (!portRule(stipMTU,showText(MTU), 0, "", 576, 1500, 1)) return false;


					if(rebootFlag)
						isApply.value = "ok";
					submit();
				}
			break;
		case 2:
			with(document.ppp) {
					
					//if ( !(ConnectType.selectedIndex == 2 && pppConnectStatus==connect) && !(connect==3)) return false;  

					if (!strRule(pppUserName,showText(UserName))) return false;
					if (!strRule(pppPassword,showText(Password))) return false;

					if(enableDuallAccessppp.checked==true) enableDuallAccess.value="ON";							
					else enableDuallAccess.value="OFF"; 
			

					var str;
					if(enableDuallAccessppp.checked==true && duallAccessMode[0].checked==true) //Dynamic IP
					{
						str = macAddr2;
						macAddr.value = " ";
					}
					else  //Static IP
					{
						str = macAddr;
						macAddr2.value = " ";
					}
					if ( str.value == "" ) str.value="000000000000";
					if ( !macRule(str, 1)) return false;

					if(DNSMode[1].checked == true)
					{
						if (( dns1.value == "" ) || ( dns1.value == "0.0.0.0" )) 
						{
							alert(showText(InvalidSomething).replace(/#####/, showText(DNS)));
							return false;
						}
						if ( !ipRule( dns1, ""+showText(DNS)+"1 "+showText(address)+"", "ip", 1)) return false;
						if ( !ipRule( dns2, ""+showText(DNS)+"2 "+showText(address)+"", "ip", 1)) return false;
						if ( !ipRule( dns3, ""+showText(DNS)+"3 "+showText(address)+"", "ip", 1)) return false;
					}
					
					if (!portRule(pppMTU,showText(MTU), 0, "", 576, 1492, 1)) return false;
					if ( ConnectType.selectedIndex != 0 && !portRule(IdleTime,showText(IdleTimeOut), 0, "", 1, 1000, 1)) return false;

					pppConnectType.value = ConnectType.value;
					pppIdleTime.value = IdleTime.value;

					pppConnect.value = "";
					pppDisconnect.value = "";

					if ((ConnectType.value == "2") && (connect == "0"))
						pppConnect.value = "Connect";
					if ((ConnectType.value == "2") && (connect == "1"))
						pppDisconnect.value = "Disconnect";
					if(rebootFlag)
						isApply.value = "ok";
						
					submit();
				}
			break;
		case 3:
			with(document.pptp) {

				//if ( !(ConnectType.selectedIndex == 2 && pppConnectStatus==connect) && !(connect==3)) return false;

				if (IpMode[1].checked==true) //Use the following IP address
				{
					pptpIpMode.value=IpMode[1].value;

					var str = macAddr1;
					macAddr.value = "";
					if ( str.value == "" ) str.value="000000000000";
				
					if ( !ipRule( IPaddr, showText(IPAddr), "ip", 1)) return false;
					if ( !maskRule(IPMaskAddr, showText(SubnetMask), 1)) return false;
					if ( !ipRule( DfGateway, showText(gateAddr), "gw", 1)) return false;
					if ( !subnetRule(IPaddr, IPMaskAddr, DfGateway, showText(gateAddr), showText(IPAddr))) return false;
				}
				else //Obtain an IP address automatically
				{
					pptpIpMode.value=IpMode[0].value;

					str =macAddr;
					macAddr1.value = "";
					if ( str.value == "" ) str.value="000000000000";
					if ( !macRule(str, 1)) return false;
				}
		
				if(DNSMode[1].checked == true)
				{
					if (( dns1.value == "" ) || ( dns1.value == "0.0.0.0" )) 
					{
						alert(showText(InvalidSomething).replace(/#####/, showText(DNS)));
						return false;
					}
					if ( !ipRule( dns1, ""+showText(DNS)+"1 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns2, ""+showText(DNS)+"2 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns3, ""+showText(DNS)+"3 "+showText(address)+"", "ip", 1)) return false;
				}

				if (!strRule(pptpUserName,showText(UserName)) || !strRule(pptpPassword,showText(Password)) ) return false;

				if ( typeof _PPTP_FQDN_ == 'undefined' )
				{
					if ( !ipRule( pptpGateway, showText(PPTPGateway), "gw", 1)) return false;

					if ( IpMode[1].checked==true){
						if ( !checkSubnet( IPaddr.value, IPMaskAddr.value, pptpGateway.value) )
						{
							/*if ( !checkSubnet( IPaddr.value, IPMaskAddr.value, DfGateway.value)) 
							{
								alert(showText(InvalidSomething).replace(/#####/,showText(PPTPGateway))+' '+showText(SomethingAndSomethingShouldInSameSubnet).replace(/#####/,showText(PPTPGateway)).replace(/####@/,showText(IPAddr))      );
								setFocus(DfGateway);
								return false;
							}*/
							if ( !subnetRule(IPaddr, IPMaskAddr, DfGateway, showText(PPTPGateway), showText(IPAddr))) return false;
						}
						else
						{
							if (DfGateway.value != "" && DfGateway.value != "0.0.0.0")
							{
								alert(showText(Defaultshouldbeempty));
								setFocus(DfGateway);
								return false;
							}
						}
					}
				}
				else
				{
					if (!ipRule(pptpGateway,showText(PPTPGateway)))
						return false
				}

				if (ConnectType.value==1 && !portRule(IdleTime,showText(IdleTimeOut), 0, "", 1, 1000, 1)) return false;
				if (!portRule(pptpMTU,showText(MTU), 0, "", 576, 1492, 1)) return false;

		
				pptpIPAddr.value=IPaddr.value;
				pptpIPMaskAddr.value=IPMaskAddr.value;
				pptpDfGateway.value=DfGateway.value;
				pptpConnectType.value=ConnectType.value;
				pptpConnect.value="";
				pptpDisconnect.value="";
				pptpIdleTime.value=IdleTime.value;

				if ((ConnectType.value == "2") && (connect == "0"))
						pptpConnect.value = "Connect";
				if ((ConnectType.value == "2") && (connect == "1"))
					pptpDisconnect.value = "Disconnect";


				if(enableDuallAccesspptp.checked==true) enableDuallAccess.value="ON";							
				else enableDuallAccess.value="OFF"; 

				if(rebootFlag)
				 isApply.value = "ok";
				
			 submit();				
			}
			break;
		case 6:
			with(document.l2tp) {
				//if ( !(ConnectType.selectedIndex == 2 && pppConnectStatus==connect) && !(connect==3)) return false;
				if (IpMode[1].checked==true)
				{
					L2TPIpMode.value=IpMode[1].value;
				
					var str = macAddr1;
					macAddr.value = "";
					if ( str.value == "" ) str.value="000000000000";
				
					if ( !ipRule( IPaddr, showText(IPAddr), "ip", 1)) return false;
					if ( !maskRule(IPMaskAddr, showText(SubnetMask), 1)) return false;
					if ( !ipRule( DfGateway, showText(gateAddr), "gw", 1)) return false;
					if ( !subnetRule(IPaddr, IPMaskAddr, DfGateway, showText(gateAddr), showText(IPAddr))) return false;
				}
				else
				{
					L2TPIpMode.value=IpMode[0].value;

					var str = macAddr;
					macAddr1.value = "";
					if ( str.value == "" ) str.value="000000000000";
					if ( !macRule(str, 1)) return false;
				}

				if(DNSMode[1].checked == true)
				{
					if (( dns1.value == "" ) || ( dns1.value == "0.0.0.0" )) 
					{
						alert(showText(InvalidSomething).replace(/#####/, showText(DNS)));
						return false;
					}
					if ( !ipRule( dns1, ""+showText(DNS)+"1 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns2, ""+showText(DNS)+"2 "+showText(address)+"", "ip", 1)) return false;
					if ( !ipRule( dns3, ""+showText(DNS)+"3 "+showText(address)+"", "ip", 1)) return false;
				}

				if (!strRule(L2TPUserName, showText(UserName)) ||
					!strRule(L2TPPassword, showText(Password)) ||
					!ipRule( L2TPGateway, showText(L2TPGateway), "ip", 1) ||
					!portRule(L2TPMTU, showText(MTU), 0, "", 576, 1492, 1)) return false;

				if ( ConnectType.selectedIndex == 1 && !portRule(IdleTime, showText(IdleTimeOut), 0, "", 1, 1000, 1)) return false;

				L2TPIPAddr.value=IPaddr.value;
				L2TPMaskAddr.value=IPMaskAddr.value;
				L2TPDefGateway.value=DfGateway.value;
				L2TPConnectType.value=ConnectType.value;
				L2TPIdleTime.value=IdleTime.value;
				L2TPConnect.value="";
				L2TPDisconnect.value ="";

				if ((ConnectType.value == "2") && (connect == "0"))
					L2TPConnect.value = "Connect";
				if ((ConnectType.value == "2") && (connect == "1"))
					L2TPDisconnect.value = "Disconnect";

				if(enableDuallAccessl2tp.checked==true) enableDuallAccess.value="ON";							
				else enableDuallAccess.value="OFF"; 
	
				if(rebootFlag)
				 isApply.value = "ok";
				
			  submit();				
			}
			break;

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
					
						var keyLen;
						var strMethod = stadrv_encrypttype.value;
						var str = stadrv_pskkey.value;
						if ( strMethod==1)
						{
							if (stadrv_weplength.selectedIndex == 0)
							{
								if ( stadrv_wepformat.selectedIndex == 0) keyLen = 5;
								else keyLen = 10;
							}
							else if (stadrv_weplength.selectedIndex == 1)
							{
								if ( stadrv_wepformat.selectedIndex == 0) keyLen = 13;
								else keyLen = 26;
							}
							else
							{
								if ( stadrv_wepformat.selectedIndex == 0) keyLen = 16;
								else keyLen = 32;
							}
							if (wlValidateKey(0,key1.value, keyLen)==0)
							{
								//key1.focus();
								return false;
							}
						}
					
						//********** psk **********
						if (strMethod==2)
						{
							if (stadrv_pskformat.selectedIndex==1) {
								if (str.length != 64) {
									alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
									return false;
								}
								takedef = 0;
								if (stadrv_pskformat.value == 1 && stadrv_pskkey.value.length == 64) {
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
		case 0:
			document.dip.reset();
			autoDNSClicked(document.dip);
			break;
		case 1:
				document.sip.reset();
			break;
		case 2:
				document.ppp.reset();
				autoDNSClicked(document.ppp);
				//TypeSelection(document.ppp);

				if (DUAL_WAN_IGMP == 0) document.ppp.DUAL_WAN_IGMP[0].checked = true;
				else document.ppp.DUAL_WAN_IGMP[1].checked = true;
			break;
		case 3:
				document.pptp.reset();
				autoIpClicked(document.pptp);
				autoDNSClicked(document.pptp);
				//TypeSelection(document.pptp);

				if (DUAL_WAN_IGMP == 0) document.pptp.DUAL_WAN_IGMP[0].checked = true;
				else document.pptp.DUAL_WAN_IGMP[1].checked = true;
				
			break;
		case 6:
				document.l2tp.reset();
				autoIpClicked(document.l2tp);
				autoDNSClicked(document.l2tp);
				//TypeSelection(document.l2tp);

				if (DUAL_WAN_IGMP == 0) document.l2tp.DUAL_WAN_IGMP[0].checked = true;
				else document.l2tp.DUAL_WAN_IGMP[1].checked = true;

			break;
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

function setpskvalue(x)
{
	with(document.wisp_form) {
		if (x == 1)
			stadrv_pskkey.value=stadrv_pskkey_1.value;
		else
			stadrv_pskkey_1.value=stadrv_pskkey.value;
	}
}
function hidePskvalue(x)
{
	if (x == 1)
	{
		document.getElementById('keyhide').checked=true;
		document.getElementById('hidepsk').style.display="block";
		document.getElementById('displaypsk').style.display="none";
	}	
	else
	{
		document.getElementById('keyhide1').checked=false;
		document.getElementById('hidepsk').style.display="none";
		document.getElementById('displaypsk').style.display="block";
	}
}
function setWepvalue(x)
{
	with(document.wisp_form) {
		if (x == 1)
			key1.value=key1_1.value;
		else
			key1_1.value=key1.value;
	}
}
function hideWepvalue(x)
{
	if (x == 1)
	{
		document.getElementById('keyhidewep').checked=true;
		document.getElementById('wephide').style.display="block";
		document.getElementById('wepdisplay').style.display="none";
	}	
	else
	{
		document.getElementById('keyhidewep1').checked=false;
		document.getElementById('wephide').style.display="none";
		document.getElementById('wepdisplay').style.display="block";
	}
}
</script>
</SCRIPT>
</head>
<body>
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
	<form action="" method="" name="wantype_form">
		<table border="1" style="height:600px;width:800px" align="center" background="/graphics/empty-dot.jpg">
			<tr>
				<td align="center">

			<table border="1" style="height:0px;width:780px" align="center">
				<tr>
				<td>
				</td>
				</tr>
			</table>	
	
		<table border="1" align="center" style="height:0px;width:780px">
			<tr>
				<td align="right" style="font-family:Arial;font-size:16px;"><script>dw(ConnectionMode)</script></td>
				<td align="left" style="font-family:Arial;font-size:16px;">
					<select name="wantype" id="wantype" onChange="switch_mode(this.value);" class="">
						<script>
							var wanTbl = new Array(showText(DynamicIP), showText(StaticIP), showText(PPPoE),showText(PPTP), null, null, showText(L2TP), showText(WISP));
							for (i=0 ; i<=7 ; i++)
							{
								if ( i == wanmod )
									document.write("<option selected value='" +i+"'>" +wanTbl[i]+ "</option>");
								else{
									if (( i != 4 ) && (i != 5))
										document.write("<option value='" +i+"'>" +wanTbl[i]+ "</option>");
								}										
							}
							</script>
						</select>
					</td>
				</tr>
			</table>
		</form>

	<br>







<!-- ===================================         dynamicip    ===================================          -->
<form action=/goform/formWanTcpipSetup method=POST name="dip">
	<table align="center" border="1" name="dynamicip_fieldset" id="dynamicip_fieldset" style="height:0px;width:780px">
		<tr>
			<td><div class="itemText-len"><script>dw(HostName)</script></div></td>
			<td><div class="itemText2-len">
					<script>
						document.write('<input type="text" name="dynIPHostName" value="'+dyiphostname+'" size="20" maxlength="30" class="text">')
					</script>
				</div>
			</td>
		</tr>
		
		<tr>
			<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
			<td><div class="itemText2-len">
				<script>
					document.write('<input type="text" name="macAddr" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
					document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto(document.dip.macAddr);" class="ui-button-nolimit">')
				</script>
			</div></td>
		</tr>
						
		<tr>
			<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
			<td><div class="itemText2-len">
				<span class="choose-itemText">
					<script>
						if (dnsmode)
						{
							document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.dip);" >'+showText(ObtainIPauto))
							document.write('<br>')
							document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.dip);" checked>'+showText(UsefollowingIP))
						}
						else
						{
							document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.dip);" checked >'+showText(ObtainIPauto))
							document.write('<br>')
							document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.dip);">'+showText(UsefollowingIP))
						}
					</script>
				</span>
			</div></td>
		</tr>
		
		<tr>
			<td><div class="itemText-len"><script>dw(DNS)</script>1&ensp;<script>dw(address)</script></div></td>
			<td><div class="itemText2-len">
				<script>
					if(valdns1)	document.write('<input type="text" name="dns1" size="15" maxlength="15" value="'+valdns1+'" size="15" maxlength="30">')
					else document.write('<input type="text" name="dns1" value="0.0.0.0" size="15" maxlength="30">')
				</script>
			</div></td>
		</tr>

		<tr>
			<td><div class="itemText-len"><script>dw(DNS)</script>2&ensp;<script>dw(address)</script></div></td>
			<td><div class="itemText2-len">
				<script>
					if(valdns2)	document.write('<input type="text" name="dns2" size="15" maxlength="15" value="'+valdns2+'" size="15" maxlength="30">')
					else document.write('<input type="text" name="dns2" value="0.0.0.0" size="15" maxlength="30">')
				</script>
			</div></td>
		</tr>

		<tr>
			<td><div class="itemText-len"><script>dw(DNS)</script>3&ensp;<script>dw(address)</script></div></td>
			<td><div class="itemText2-len">
				<script>
					if(valdns3)	document.write('<input type="text" name="dns3" size="15" maxlength="15" value="'+valdns3+'" size="15" maxlength="30">')
					else document.write('<input type="text" name="dns3" value="0.0.0.0" size="15" maxlength="30">')
				</script>
			</div></td>
		</tr>		

		<tr>
			<td><div class="itemText-len"><script>dw(MTU)</script></div></td>
			<td><div class="itemText2-len">
				<input type="text" name="dhcpMTU" size="10" maxlength="4" value="<% getInfo("dhcpMTU"); %>">&nbsp;<span class="itemText">(576&lt;= <script>dw(MTU)</script>&ensp;<script>dw(Value)</script> &lt;=1500)</span>
			</div></td>
		</tr>

		<tr>
			<td><div class="itemText-len"><script>dw(TTL)</script></div></td>
			<td><div class="itemText2-len">
				<span class="choose-itemText">
					<script>
						if(!ttlval)
						{
							document.write('<input type="radio" value="0" name="pppEnTtl" checked>'+showText(disable))
							document.write('<input type="radio" value="1" name="pppEnTtl">'+showText(enable))
						}
						else
						{
							document.write('<input type="radio" value="0" name="pppEnTtl">'+showText(disable))
							document.write('<input type="radio" value="1" name="pppEnTtl" checked>'+showText(enable))
						}
					</script>
				</span>
			</div></td>
		</tr>
	</table>

	<br>

	<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
		<tr>
			<td style="text-align:center; padding-top:30px;">
				<script>
					document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0,0,4)" class="ui-button">');	
					var show_reboot = "<% getInfo("show_reboot"); %>"
					if(show_reboot == "1")
						document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(0,1,4)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
				</script>
			</td>
		</tr>
	</table>
					
	<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
	<input type=hidden value="0" name="wanMode">
	<!--<input type=hidden value="" name="igmpEnable"> -->
	<input type=hidden value="" name="isApply">
</form>

















		<!-- ===================================         staticip    ===================================          -->
		<fieldset name="staticip_fieldset" id="staticip_fieldset" style="display:none">
				<form action=/goform/formWanTcpipSetup method=POST name="sip">

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td><div class="itemText-len"><script>dw(FixedIP)</script>&ensp;<script>dw(IPAddr)</script></div></td>
							<td><div class="itemText2-len"><input type="text" name="ip" size="15" maxlength="15" value="<% getInfo("wan-ip-rom"); %>"></div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
							<td><div class="itemText2-len"><input type="text" name="mask" size="15" maxlength="15" value="<% getInfo("wan-mask-rom"); %>"><div></td>
						</tr>
						<input type="hidden" name="DNSMode" value="1">
						<tr>
							<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
							<td><div class="itemText2-len"><input type="text" name="gateway" size="15" maxlength="15" value="<% getInfo("wan-gateway-rom"); %>"></div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
							<td><div class="itemText2-len">
								<script>
									document.write('<input type="text" name="macAddr" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
									document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto(document.sip.macAddr);" class="ui-button-nolimit">')
								</script>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(DNS)</script>1&ensp;<script>dw(address)</script></div></td>
							<td><div class="itemText2-len">
								<script>
									if(valdns1)	document.write('<input type="text" name="dns1" size="15" maxlength="15" value="'+valdns1+'" size="15" maxlength="30">')
									else document.write('<input type="text" name="dns1" size="15" maxlength="15" value="0.0.0.0" size="15" maxlength="30">')
								</script>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(DNS)</script>2&ensp;<script>dw(address)</script></div></td>
							<td><div class="itemText2-len">
								<script>
									if(valdns2)	document.write('<input type="text" name="dns2" size="15" maxlength="15" value="'+valdns2+'" size="15" maxlength="30">')
									else document.write('<input type="text" name="dns2" size="15" maxlength="15" value="0.0.0.0" size="15" maxlength="30">')
								</script>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(DNS)</script>3&ensp;<script>dw(address)</script></div></td>
							<td><div class="itemText2-len">
								<script>
									if(valdns3)	document.write('<input type="text" name="dns3" size="15" maxlength="15" value="'+valdns3+'" size="15" maxlength="30">')
									else document.write('<input type="text" name="dns3" value="0.0.0.0" size="15" maxlength="30">')
								</script>
							<div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(MTU)</script></div></td>
							<td><div class="itemText2-len">
								<input type="text" name="stipMTU" size="10" maxlength="4" value="<% getInfo("dhcpMTU"); %>">&nbsp;<span class="itemText">(576&lt;= <script>dw(MTU)</script>&ensp;<script>dw(Value)</script> &lt;=1500)</span>
							</div></td>
						</tr>
						<tr>
							<td><div class="itemText-len"><script>dw(TTL)</script></div></td>
							<td><div class="itemText2-len"><span class="choose-itemText">
								<script>
									if(!ttlval)
									{
										document.write('<input type="radio" value="0" name="pppEnTtl" checked>'+showText(disable))
										document.write('<input type="radio" value="1" name="pppEnTtl">'+showText(enable))
									}
									else
									{
										document.write('<input type="radio" value="0" name="pppEnTtl">'+showText(disable))
										document.write('<input type="radio" value="1" name="pppEnTtl" checked>'+showText(enable))
									}
								</script>
							</span></div></td>
						</tr>
					</table>

					<br>

	<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
		<tr>
			<td style="text-align:center; padding-top:30px;">
				<script>
					document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(1,0,4)" class="ui-button">');	
					var show_reboot = "<% getInfo("show_reboot"); %>"
					if(show_reboot == "1")
					document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1,1,4)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
				</script>
			</td>
		</tr>
	</table>

	<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
	<input type=hidden value="1" name="wanMode" >
	<!--<input type=hidden value="" name="igmpEnable"> EDX patrick-->
	<input type=hidden value="1" name="DNSMode">
	<input type=hidden value="fixedIp" name="ipMode" >
	<input type=hidden value="" name="isApply">
</form>















			
   <!-- ===================================         PPPOE    ===================================          -->
			<fieldset name="pppoe_fieldset" id="pppoe_fieldset" style="display:none">
					<form action=/goform/formWanTcpipSetup method=POST name="ppp">
							
							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
								<tr>
									<td><div class="itemText-len"><script>dw(UserName)</script></div></td>
									<td><div class="itemText2-len"><input type="text" name="pppUserName" size="20" maxlength="64" value="<% getInfo("pppUserName"); %>"></div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(Password)</script></div></td>
									<td><div class="itemText2-len"><input type="text" name="pppPassword" size="20" maxlength="64" value="<% getInfo("pppPassword"); %>"></div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
									<td><div class="itemText2-len">
										<script>
											document.write('<input type="text" name="macAddr" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
											document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto(document.ppp.macAddr);" class="ui-button-nolimit">')
										</script>
									</div></td>
								</tr>

								<tr>
									<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
									<td><div class="itemText2-len"><span class="choose-itemText">
										<script>
												if (dnsmode)
												{
													document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.ppp);">'+showText(ObtainIPauto))
													document.write('<br>')
													document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.ppp);" checked >'+showText(UsefollowingIP))
												}
												else
												{
													document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.ppp);" checked >'+showText(ObtainIPauto))
													document.write('<br>')
													document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.ppp);">'+showText(UsefollowingIP))
												}
											</script>
									</span></div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(DNS)</script>1&ensp;<script>dw(address)</script></div></td>
									<td><div class="itemText2-len">
										<script>
											if(valdns1)	document.write('<input type="text" name="dns1" size="15" maxlength="15" value="'+valdns1+'" size="15" maxlength="30">')
											else document.write('<input type="text" name="dns1" value="0.0.0.0" size="15" maxlength="30">')
										</script>
									</div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(DNS)</script>2&ensp;<script>dw(address)</script></div></td>
									<td><div class="itemText2-len">
										<script>
											if(valdns2)	document.write('<input type="text" name="dns2" size="15" maxlength="15" value="'+valdns2+'" size="15" maxlength="30">')
											else document.write('<input type="text" name="dns2" value="0.0.0.0" size="15" maxlength="30">')
										</script>
									</div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(DNS)</script>3&ensp;<script>dw(address)</script></div></td>
									<td><div class="itemText2-len">
										<script>
											if(valdns3)	document.write('<input type="text" name="dns3" size="15" maxlength="15" value="'+valdns3+'" size="15" maxlength="30">')
											else document.write('<input type="text" name="dns3" value="0.0.0.0" size="15" maxlength="30">')
										</script>
									</div></td>
								</tr>		

								<tr>
									<td><div class="itemText-len"><script>dw(TTL)</script></div></td>
									<td><div class="itemText2-len"><span class="choose-itemText">
										<script>
											if(!ttlval)
											{
												document.write('<input type="radio" value="0" name="pppEnTtl" checked>'+showText(disable))
												document.write('<input type="radio" value="1" name="pppEnTtl">'+showText(enable))
											}
											else
											{
												document.write('<input type="radio" value="0" name="pppEnTtl">'+showText(disable))
												document.write('<input type="radio" value="1" name="pppEnTtl" checked>'+showText(enable))
											}
										</script>
									</span><div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(ServerceName)</script></div></td>
									<td><div class="itemText2-len"><input type="text" name="pppServName" size="20" maxlength="30" value="<% getInfo("pppServName"); %>"></div></td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(MTU)</script></div></td>
									<td><div class="itemText2-len">
										<input type="text" name="pppMTU" size="10" maxlength="4" value="<% getInfo("pppMTU"); %>">&nbsp;<span class="itemText">(576&lt;= <script>dw(MTU)</script>&ensp;<script>dw(Value)</script> &lt;=1492)</span>
									</div></td>
								</tr>
								

								<tr>
									<td colspan="2" style="text-align:left; padding-left:50px;">
										<script>

												document.write('<input type="checkbox" name="enableDuallAccessppp" <%getVar("isDuallAccessEnable","1","checked");%> value="ON" onclick=displayUpdate();>&nbsp;<span class="itemText">'+showText(enable)+'&ensp;'+showText(DualWanAccess)+'&ensp;:</span>')
										</script>
									</td>
									<td>
									</td>
								</tr>
								<tr>
									<td><div class="itemText-len"><script>dw(IGMPSource)</script></div></td>
									<td><div class="itemText2-len">
										<input type="radio" name="DUAL_WAN_IGMP" value="0"><span class="choose-itemText "><script>dw(ETH)</script></span>&nbsp;&nbsp;<input type="radio" name="DUAL_WAN_IGMP" value="1"><span class="choose-itemText "><script>dw(PPP)</script></span>
										<script>
											if (DUAL_WAN_IGMP == 0)
												document.ppp.DUAL_WAN_IGMP[0].checked = true;
											else
												document.ppp.DUAL_WAN_IGMP[1].checked = true;
										</script>
									</div></td>
								</tr>
						</table>
							
						<div id="DualAccessEnableBtId" style="display:none">
							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<tr>
										<td><div class="itemText-len"></div></td>
										<td><div class="itemText2-len">
											<script>
												if(!duallAccessType){
													document.write('<input type="radio" name="duallAccessMode" value="0" onClick="displayUpdate();" checked><span class="choose-itemText ">'+showText(DynamicIP)+'</span>')
													document.write('<input type="radio" name="duallAccessMode" value="1" onClick="displayUpdate();"><span class="choose-itemText ">'+showText(StaticIP)+'</span>')
												}
												else
												{	
													document.write('<input type="radio" name="duallAccessMode" value="0" onClick="displayUpdate();"><span class="choose-itemText ">'+showText(DynamicIP)+'</span>')
													document.write('<input type="radio" name="duallAccessMode" value="1" onClick="displayUpdate();" checked><span class="choose-itemText ">'+showText(StaticIP)+'</span>')
												}	
											</script>
										</div></td>
									</tr>
								</table>
								<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="dynamicBtId" style="display:none" >
									<tr>
										<td><div class="itemText-len"><script>dw(HostName)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="dynIPHostName" value="<% getInfo("dynIPHostName"); %>" size="20" maxlength="30" class="text">
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
										<td><div class="itemText2-len">
												<input type="text" name="macAddr2" value="<% getInfo("wanMac"); %>" size="20" maxlength="12">&nbsp;
											<script>
												document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone2" onClick="copyto(document.ppp.macAddr2);" class="ui-button-nolimit">')
											</script>
										</div></td>
									</tr>
								</table>
								<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700" id="staticBtId" style="display:none" >
									<tr>
										<td><div class="itemText-len"><script>dw(StaticIP)</script> <script>dw(address)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="ip" size="15" maxlength="15" value="<% getInfo("wan-ip-rom"); %>">
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="mask" size="15" maxlength="15" value="<% getInfo("wan-mask-rom"); %>">
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="gateway" size="15" maxlength="15" value="<% getInfo("wan-gateway-rom"); %>">
										</div></td>
									</tr>
								</table>
						</div>
							
							<br>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td style="text-align:center; padding-top:30px;">
								<script>
									document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(2,0,4)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(2,1,4)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
								</script>
							</td>
						</tr>
					</table>

					<input type=hidden value="0" name="ConnectType" >
					<input type=hidden value="<% getInfo("wan-ppp-idle"); %>" name="IdleTime" >

					<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
					<input type=hidden value="2" name="wanMode">
					<input type=hidden value="ppp" name="ipMode" >

					<input type=hidden value="" name="pppConnect" >
					<input type=hidden value="" name="pppDisconnect" >
					<input type=hidden value="" name="pppConnectType" >
					<input type=hidden value="" name="pppIdleTime" >
          <input type=hidden value="" name="enableDuallAccess">
					<input type=hidden value="" name="isApply">
					<!--<input type=hidden value="" name="igmpEnable"> EDX patrick-->
				</form>
			</fieldset>
        <!-- ===================================         pptp    ===================================          -->
				<fieldset name="pptp_fieldset" id="pptp_fieldset" style="display:none">
			
						<form action=/goform/formPPTPSetup method=POST name="pptp">
					
								<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<tr>
										<td colspan="2" style="text-align:left; padding-left:50px;">
											<script>
												if(!pptp_IpMode) document.write('<input type="radio" value="0" name="IpMode" onClick="autoIpClicked(document.pptp)" checked>&nbsp;<b><span class="itemText">'+showText(ObtainIPauto)+'&ensp;:<span></b>')
												else document.write('<input type="radio" value="0" name="IpMode" onClick="autoIpClicked(document.pptp)">&nbsp;<b><span class="itemText">'+showText(ObtainIPauto)+'&ensp;:<span></b>')
											</script>
										</td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(HostName)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="HostName" value="<% getInfo("dynIPHostName"); %>" size="20" maxlength="30" class="text">
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="macAddr" value="<% getInfo("wanMac"); %>" size="20" maxlength="12">&nbsp;
											<script>
													document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto(document.pptp.macAddr);" class="ui-button-nolimit">')
											</script>
										</div></td>
									</tr>
									<tr>
										<td colspan="2" style="text-align:left; padding-left:50px;">
											<script>
												if(pptp_IpMode) document.write('<input type="radio" value="1" name="IpMode" onClick="autoIpClicked(document.pptp)" checked>&nbsp;<b><span class="itemText">'+showText(UsefollowingIP)+'&ensp;:<span></b>')
												else document.write('<input type="radio" value="1" name="IpMode" onClick="autoIpClicked(document.pptp)">&nbsp;<b><span class="itemText">'+showText(UsefollowingIP)+'&ensp;:<span></b>')
											</script>
										</td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(StaticIP)</script>&ensp;<script>dw(address)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="IPaddr" size="15" maxlength="15" value="<% getInfo("pptpIPAddr"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="IPMaskAddr" size="15" maxlength="15" value="<% getInfo("pptpIPMaskAddr"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="DfGateway" size="15" maxlength="15" value="<% getInfo("pptpDfGateway"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
										<td><div class="itemText2-len">
											<script>
													document.write('<input type="text" name="macAddr1" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
													document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone1" onClick="copyto(document.pptp.macAddr1);" class="ui-button-nolimit">')
											</script>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText">
											<script>
												if (dnsmode)
												{
													document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.pptp);"  >'+showText(ObtainIPauto))
													document.write('<br>')
													document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.pptp);" checked>'+showText(UsefollowingIP))
												}
												else
												{
													document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.pptp);" checked >'+showText(ObtainIPauto))
													document.write('<br>')
													document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.pptp);">'+showText(UsefollowingIP))
												}
											</script>
										</span></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(DNS)</script>1&ensp;<script>dw(address)</script></div></td>
										<td><div class="itemText2-len">
											<script>
												if(valdns1)	document.write('<input type="text" name="dns1" size="15" maxlength="15" value="'+valdns1+'" size="15" maxlength="30">')
												else document.write('<input type="text" name="dns1" value="0.0.0.0" size="15" maxlength="30">')
											</script>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(DNS)</script>2&ensp;<script>dw(address)</script></div></td>
										<td><div class="itemText2-len">
											<script>
												if(valdns2)	document.write('<input type="text" name="dns2" size="15" maxlength="15" value="'+valdns2+'" size="15" maxlength="30">')
												else document.write('<input type="text" name="dns2" value="0.0.0.0" size="15" maxlength="30">')
											</script>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(DNS)</script>3&ensp;<script>dw(address)</script></div></td>
										<td><div class="itemText2-len">
											<script>
												if(valdns3)	document.write('<input type="text" name="dns3" size="15" maxlength="15" value="'+valdns3+'" size="15" maxlength="30">')
												else document.write('<input type="text" name="dns3" value="0.0.0.0" size="15" maxlength="30">')
											</script>
										</div></td>
									</tr>		
									<tr>
										<td colspan="2" style="text-align:left; padding-left:50px;">
											<script>
												/*if(dualaccessSW == 1)  document.write('<input type="checkbox" name="enableDuallAccesspptp" value="ON" onclick="" checked>&nbsp;<span class="itemText">'+showText(enable)+'&ensp;'+showText(DualWanAccess)+'&ensp;:</span>')
												else*/  document.write('<input type="checkbox" name="enableDuallAccesspptp" <%getVar("isDuallAccessEnable","1","checked");%> value="ON" onclick="">&nbsp;<span class="itemText">'+showText(enable)+'&ensp;'+showText(DualWanAccess)+'&ensp;:</span>')
											</script>
										</td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(IGMPSource)</script><div></td>
										<td><div class="itemText2-len">
											<input type="radio" name="DUAL_WAN_IGMP" value="0"><span class="choose-itemText "><script>dw(ETH)</script></span>&nbsp;&nbsp;<input type="radio" name="DUAL_WAN_IGMP" value="1"><span class="choose-itemText "><script>dw(PPP)</script></span>
											<script>
													if (DUAL_WAN_IGMP == 0)
														document.pptp.DUAL_WAN_IGMP[0].checked = true
													else
														document.pptp.DUAL_WAN_IGMP[1].checked = true
											</script>
										</div></td>
									</tr>
									<tr>
										<td colspan="2" style="text-align:left; padding-left:50px;">
											<script>
												document.write('<b><span class="itemText">'+showText(PPTP)+'&ensp;'+showText(Settings)+'&ensp;:</span></b>')
											</script>
										</td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(UserID)</script></div><div></td>
										<td><div class="itemText2-len"><input type="text" name="pptpUserName" size="20" maxlength="31" value="<% getInfo("pptpUserName"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(Password)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="pptpPassword" size="20" maxlength="31" value="<% getInfo("pptpPassword"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(PPTPGateway)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="pptpGateway" size="20" maxlength="30" value="<% getInfo("pptpGateway"); %>"></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(ConnectionID)</script></div></td>
										<td><div class="itemText2-len"><input type="text" name="pptpConntID" size="20" maxlength="30" value="<% getInfo("pptpConntID"); %>">&nbsp;<span class="itemText">(Optional)</span></div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(MTU)</script></div></td>
										<td><div class="itemText2-len">
											<input type="text" name="pptpMTU" size="10" maxlength="4" value="<% getInfo("pptpMTU"); %>">&nbsp;<span class="itemText">(576&lt;= <script>dw(MTU)</script>&ensp;<script>dw(Value)</script> &lt;=1492)</span>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(BEZEQISRAEL)</script></div></td>
										<td><div class="itemText2-len">
											<script>
											document.write('<input type="checkbox" name="pptpBEZEQEnable" value="ON" <%getVar("pptpBEZEQEnable","1","checked");%> >&nbsp;<span class="itemText">'+showText(enable)+'&ensp;('+showText(BEZEQISRAELonly)+') </span>')
											</script>
										</div></td>
									</tr>
								</table>

								<br>

								<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
									<tr>
										<td style="text-align:center; padding-top:30px;">
											<script>
												document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(3,0,3)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(3,1,3)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
											</script>
										</td>
									</tr>
								</table>

								<input type=hidden value="0" name="ConnectType" >
								<input type=hidden value="<% getInfo("wan-pptp-idle"); %>" name="IdleTime" >

								<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
								<input type=hidden value="3" name="wanMode" >
	
								<input type=hidden value="" name="pptpIpMode"> 
								<input type=hidden value="" name="pptpIPAddr">
								<input type=hidden value="" name="pptpIPMaskAddr"> 
								<input type=hidden value="" name="pptpDfGateway"> 
								<input type=hidden value="" name="pptpConnectType"> 

								<input type=hidden value="" name="pptpConnect"> 
								<input type=hidden value="" name="pptpDisconnect"> 
								<input type=hidden value="" name="pptpIdleTime"> 
								<input type=hidden value="" name="enableDuallAccess">
								<input type=hidden value="" name="isApply">

				</form>
			</fieldset>
      <!-- ===================================         l2tp    ===================================          -->
			<fieldset name="l2tp_fieldset" id="l2tp_fieldset" style="display:none">
		
					<form action=/goform/formL2TPSetup method=POST name="l2tp">
										
						<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
							<tr>
								<td colspan="2" style="text-align:left; padding-left:50px;"><span class="itemText">
									<script> 
										if(!l2tp_IpMode) document.write('<input type="radio" value="0" name="IpMode" onClick="autoIpClicked(document.l2tp)" checked>&nbsp;<b>'+showText(ObtainIPauto)+'&ensp;:</b>')
										else document.write('<input type="radio" value="0" name="IpMode" onClick="autoIpClicked(document.l2tp)">&nbsp;<b>'+showText(ObtainIPauto)+'&ensp;:</b>')
									</script>
								<span></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(HostName)</script></div></td>
								<td><div class="itemText2-len">
									<script>
											document.write('<input type="text" name="HostName" value="'+dyiphostname+'" size="20" maxlength="30" class="text">')
									</script>
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
								<td><div class="itemText2-len">
									<script>
											document.write('<input type="text" name="macAddr" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
											document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone" onClick="copyto(document.l2tp.macAddr);" class="ui-button-nolimit">')
									</script>
								</div></td>
							</tr>
							<tr>
								<td colspan="2" style="text-align:left; padding-left:50px;"><span class="itemText">
									<script>
									 	if(l2tp_IpMode) document.write('<input type="radio" value="1" name="IpMode" onClick="autoIpClicked(document.l2tp)" checked>&nbsp;<b>'+showText(UsefollowingIP)+'&ensp;:</b>')
										else document.write('<input type="radio" value="1" name="IpMode" onClick="autoIpClicked(document.l2tp)">&nbsp;<b>'+showText(UsefollowingIP)+'&ensp;:</b>')
									</script>
								<span></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(StaticIP)</script>&ensp;<script>dw(address)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="IPaddr" size="15" maxlength="15" value="<% getInfo("L2TPIPAddr"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(SubnetMask)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="IPMaskAddr" size="15" maxlength="15" value="<% getInfo("L2TPMaskAddr"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(gateAddr)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="DfGateway" size="15" maxlength="15" value="<% getInfo("L2TPDefGateway"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(macAddr)</script></div></td>
								<td><div class="itemText2-len">
									<script>
										document.write('<input type="text" name="macAddr1" value="'+tcpipwanmac+'" size="20" maxlength="12">&nbsp;')
										document.write('<input type="button" value="'+showText(CloneMAC)+'" name="Clone1" onClick="copyto(document.l2tp.macAddr1);" class="ui-button-nolimit">')
									</script>
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(DNS)</script>&ensp;<script>dw(address)</script></div></td>
								<td><div class="itemText2-len"><span class="choose-itemText">
									<script>
										if (dnsmode)
										{
											document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.l2tp);"  >'+showText(ObtainIPauto))
											document.write('<br>')
											document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.l2tp);" checked >'+showText(UsefollowingIP))
										}
										else
										{
											document.write('<input type="radio" value="0" name="DNSMode" onClick="autoDNSClicked(document.l2tp);" checked >'+showText(ObtainIPauto))
											document.write('<br>')
											document.write('<input type="radio" value="1" name="DNSMode" onClick="autoDNSClicked(document.l2tp);">'+showText(UsefollowingIP))
										}
									</script>
								</span></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(DNS)</script>1&ensp;<script>dw(address)</script></div></td>
								<td><div class="itemText2-len">
									<script>
										if(valdns1)	document.write('<input type="text" name="dns1" size="15" maxlength="15" value="'+valdns1+'" size="15" maxlength="30">')
										else document.write('<input type="text" name="dns1" value="0.0.0.0" size="15" maxlength="30">')
									</script>
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(DNS)</script>2&ensp;<script>dw(address)</script></div></td>
								<td><div class="itemText2-len">
									<script>
										if(valdns2)	document.write('<input type="text" name="dns2" size="15" maxlength="15" value="'+valdns2+'" size="15" maxlength="30">')
										else document.write('<input type="text" name="dns2" value="0.0.0.0" size="15" maxlength="30">')
									</script>
								</div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(DNS)</script>3&ensp;<script>dw(address)</script></div></td>
								<td><div class="itemText2-len">
									<script>
										if(valdns3)	document.write('<input type="text" name="dns3" size="15" maxlength="15" value="'+valdns3+'" size="15" maxlength="30">')
										else document.write('<input type="text" name="dns3" value="0.0.0.0" size="15" maxlength="30">')
									</script>
								</div></td>
							</tr>		
							<tr>
								<td colspan="2" style="text-align:left; padding-left:50px;">
									<script>
										/*if(dualaccessSW == 1) document.write('<input type="checkbox"name="enableDuallAccessl2tp" value="ON" onclick="" checked>&nbsp;<span class="itemText">'+showText(enable)+'&ensp;'+showText(DualWanAccess)+'&ensp;:</span>')
										else*/ document.write('<input type="checkbox"name="enableDuallAccessl2tp" <%getVar("isDuallAccessEnable","1","checked");%> value="ON" onclick="">&nbsp;<span class="itemText">'+showText(enable)+'&ensp;'+showText(DualWanAccess)+'&ensp;:</span>')
									</script>
								</td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(IGMPSource)</script><div></td>
								<td><div class="itemText2-len">
									<input type="radio" name="DUAL_WAN_IGMP" value="0"><span class="choose-itemText "><script>dw(ETH)</script></span>&nbsp;&nbsp;<input type="radio" name="DUAL_WAN_IGMP" value="1"><span class="choose-itemText "><script>dw(PPP)</script></span>
									<script>
										if (DUAL_WAN_IGMP == 0)
											document.l2tp.DUAL_WAN_IGMP[0].checked = true
										else
											document.l2tp.DUAL_WAN_IGMP[1].checked = true
									</script>
								</div></td>
							</tr>
						
							<tr>
								<td colspan="2" style="text-align:left; padding-left:50px;">
									<script>
										document.write('<b><span class="itemText">'+showText(L2TP)+'&ensp;'+showText(Settings)+'&ensp;:</span></b>')
									</script>
								</td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(UserID)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="L2TPUserName" size="18" maxlength="64" value="<% getInfo("L2TPUserName"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(Password)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="L2TPPassword" size="18" maxlength="64" value="<% getInfo("L2TPPassword"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(L2TPGateway)</script></div></td>
								<td><div class="itemText2-len"><input type="text" name="L2TPGateway" size="18" maxlength="30" value="<% getInfo("L2TPGateway"); %>"></div></td>
							</tr>
							<tr>
								<td><div class="itemText-len"><script>dw(MTU)</script></div></td>
								<td><div class="itemText2-len">
									<input type="text" name="L2TPMTU" size="10" maxlength="4" value="<% getInfo("L2TPMTU"); %>">&nbsp;<span class="itemText">(576&lt;= <script>dw(MTU)</script>&ensp;<script>dw(Value)</script> &lt;=1492)</span>
								</div></td>
							</tr>
						</table>
						
						<br>

						<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
							<tr>
								<td style="text-align:center; padding-top:30px;">
									<script>
										document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(6,0,4)" class="ui-button">');	
									var show_reboot = "<% getInfo("show_reboot"); %>"
									if(show_reboot == "1")
										document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(6,1,4)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
									</script>
								</td>
							</tr>
						</table>

						<input type=hidden value="0" name="ConnectType" >
						<input type=hidden value="<% getInfo("L2TPIdleTime"); %>" name="IdleTime" >

						<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
						<input type=hidden value="6" name="wanMode" >
						<input type=hidden value="" name="isApply">

						<input type=hidden value="" name="L2TPIpMode">
						<input type=hidden value="" name="L2TPIPAddr">
						<input type=hidden value="" name="L2TPMaskAddr">
						<input type=hidden value="" name="L2TPDefGateway">
						<input type=hidden value="" name="L2TPConnect">
						<input type=hidden value="" name="L2TPDisconnect">
						<input type=hidden value="" name="L2TPConnectType">
						<input type=hidden value="" name="L2TPIdleTime">
						<input type=hidden value="" name="enableDuallAccess">

				</form>
			</fieldset>
      <!-- ===================================        wisp   ===================================          -->
			<fieldset name="wisp_fieldset" id="wisp_fieldset" style="display:none">
		
					<form action=/goform/formStaDrvSetup method="POST" name="wisp_form">
						
						<table class="" border="0" align="left">
								<tr>
									<td><div class="itemText-len"><script>dw(enable)</script>&nbsp;/&nbsp;<script>dw(disable)</script></div></td>
									<td><div class="itemText2-len"><span class="choose-itemText">						
										<script>
											var statype=<% getInfo("stadrv_type");%>;
	
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
											document.write('<input type="button" class="ui-button-nolimit" name="selSurvey" value="'+showText(selSiteSurvey)+'" onClick="WISP_SiteSurvey()">')
										</script>
									</span></div></td>
								</tr>

								<tr>
									<td><div class="itemText-len"><script>dw(ChannelNumber)</script></div></td>
									<td><div class="itemText2-len">
											<input type="text" name="stadrv_chan" size="3" maxlength="3" value="<% getInfo("stadrv_chan"); %>">
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
												var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key),showText(WPA_RADIUS));
												for ( i=0; i<3; i++)
												{
													if ( i == <% getInfo("stadrv_encrypttype"); %>)
														document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
													else
														document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
												}
											</script>
										</select>
										
									</div></td>
								</tr>
							</table>					
							
							<div id="WEP" style="display:none">
								<table class="" border="0" align="left">		
									<tr>
										<td><div class="itemText-len"><script>dw(keyLen)</script></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="stadrv_weplength" ONCHANGE=lengthClick()>
												<script>
													for(i=0;i<2;i++)
													{
														if( i == <% getInfo("stadrv_weplength"); %> )
															document.write('<option selected value="'+i+'">'+(i+1)*64+'-bit</option>');
														else
															document.write('<option value="'+i+'">'+(i+1)*64+'-bit</option>');
													}
												</script>
											</select>
										</div></td>
									</tr>
									<tr>
										<td><div class="itemText-len"><script>dw(KeyFormat)</script></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="stadrv_wepformat" ONCHANGE="setDefaultKeyValue()" class="select">
												<script>
													for(i=0;i<2;i++)
													{
														if( i == <% getInfo("stadrv_wepformat"); %> )
															document.write('<option selected value="'+i+'"></option>');
														else
															document.write('<option value="'+i+'"></option>');
													}
												</script>
											</select>
										</div></td>
									</tr>
									<input type="hidden" value="0" name="stadrv_defaultkey">
									<tr>
										<td><div class="itemText-len"><script>dw(EncryptionKey)</script></div></td>
										<td><div class="itemText2-len">
												<input type="text" name="key1" size="32" maxlength="32" onchange="">
										</div></td>
									</tr>
								</table>
							</div>
							
				
							<div id="WPA" style="display:none">
								<table class="" border="0" align="left">		
									<tr>
										<td><div class="itemText-len"><script>dw(WPA_Unicast_Cipher_Suite)</script></div></td>
										<td><div class="itemText2-len"><span class="choose-itemText">
											<script>
												cipherTbl = new Array(showText(WPA_TKIP), showText(WPA2_AES), showText(WPA2_Mixed));
												for(i=0;i<2;i++)
												{
													if(i==<%getInfo("stadrv_wpacipher");%>)
														document.write('<input type="radio" name="stadrv_wpacipher" value="'+i+'" id="wpaCipher'+i+'" checked>'+cipherTbl[i]+'&nbsp;&nbsp;');
													else
														document.write('<input type="radio" name="stadrv_wpacipher" value="'+i+'" id="wpaCipher'+i+'">'+cipherTbl[i]+'&nbsp;&nbsp;');
												}
											</script>
										</span></div></td>
									</tr>
									<tr>
										<td ><div class="itemText-len"><script>dw(Pre_shared_Key_Format)</script></div></td>
										<td><div class="itemText2-len">
											<select size="1" name="stadrv_pskformat" ONCHANGE="setPskKeyValue()">
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
													document.write('<input type="text" name="stadrv_pskkey" size="32" maxlength="64" value="'+pskval+'" class="text" onchange="">')
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
							<input type=hidden value="/inter_wan.asp" name="submit-url" id="submitUrl">
							<input type=hidden value="7" name="wanMode">
							<input type=hidden value="" name="isApply">
			</form>

	<table border="0" style="height:28px;width:780px" align="center">
		<tr>
			<td align="right" style="width:390px">
				<script>document.write('<input type="button" value="'+showText(Back)+'" name="close" onClick="goBack();" class="button_IQ2" style="cursor:pointer">&nbsp;&nbsp;');
				</script>
			</td>
	
			<td align="left" style="width:390px">
				<script>document.write('&nbsp;&nbsp;<input type="button" value="'+showText(Next)+'" name="close" onClick="goNext();" class="button_IQ2" style="cursor:pointer">');
				</script>
			</td>
		</tr>
	</table>

	<table border="0" style="height:60px;width:780px" align="center">
		<tr>
		<td>
		</td>
		</tr>
	</table>
			




</td>
</tr>
</table>
</form>
</div>
</body>

<head>
<meta http-equiv="Pragma" content="no-cache">
</head>	

</html>

<script>
switch_mode(wanmod);  //if save or apply, lock the mode page

switch(wanmod)
{
	case 0:
		autoDNSClicked(document.dip);
		break;
	case 1:
		break;
	case 2:
		autoDNSClicked(document.ppp);
		//TypeSelection(document.ppp);
		break;
	case 3:
		autoDNSClicked(document.pptp);
		//TypeSelection(document.pptp);		
		break;
	case 6:
		autoDNSClicked(document.l2tp);
		//TypeSelection(document.l2tp);
		break;
	case 7:
		displayObj();
		lockUI(wisp_enable);
	  break;
	default: break;
}

displayUpdate();
</script>
