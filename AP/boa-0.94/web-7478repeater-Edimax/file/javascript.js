function setlanguage()
{
  var vervaldatum = new Date()
  vervaldatum.setDate(vervaldatum.getDate()+365);
  
  var newValue = document.form.site.options[document.form.site.selectedIndex].value;
  //document.cookie="language="+newValue+"; expires="+vervaldatum+"; path=/";
  //parent.window.location.href = "/index.asp";
  document.cookie="language="+newValue+"; expires="+vervaldatum.toGMTString()+"; path=/";
  top.window.location.href = "/index.asp";
}

function iQsetup_setlanguage()
{
  var vervaldatum = new Date()
  vervaldatum.setDate(vervaldatum.getDate()+365);
  
  var newValue = document.iqv2.site.options[document.iqv2.site.selectedIndex].value;
  document.cookie="language="+newValue+"; expires="+vervaldatum.toGMTString()+"; path=/";
  top.window.location.href = "/aIndex.asp";
} 

function getCookie(c_name)
{
  if (document.cookie.length>0 && document.cookie.indexOf(c_name + "=")!=-1)
  {
    c_start=document.cookie.indexOf(c_name + "=") + c_name.length+1;
    c_end=document.cookie.indexOf(";",c_start);
    if (c_end==-1) c_end=document.cookie.length;
    return unescape(document.cookie.substring(c_start,c_end));
  }
   else
  {
		if (navigator.appName == 'Netscape')
      var language = navigator.language;
    else
      var language = navigator.browserLanguage;

		if (language.toLowerCase().indexOf('us') > -1) stype=0;
		else if (language.toLowerCase().indexOf('de') > -1) stype=1;   //Germany
		else if (language.toLowerCase().indexOf('es') > -1) stype=2;   //Spain
		else if (language.toLowerCase().indexOf('fr') > -1) stype=3;   //France
		else if (language.toLowerCase().indexOf('it') > -1) stype=4;   //Italy
		else if (language.toLowerCase().indexOf('nl') > -1) stype=5;   //Netherlands
		else if (language.toLowerCase().indexOf('pt') > -1) stype=6;   //Portugal
		else if (language.toLowerCase().indexOf('ro') > -1) stype=7;   //Romania
		else if (language.toLowerCase().indexOf('cz') > -1) stype=8;   //Czech Republic
		else if (language.toLowerCase().indexOf('pl') > -1) stype=9;   //Poland
		else if (language.toLowerCase().indexOf('ru') > -1) stype=10;   //Russian
		else if (language.toLowerCase().indexOf('sk') > -1) stype=11;   //Slovak
		//else if (language.toLowerCase().indexOf('ua') > -1) stype=12; //Ukraine
		else if (language.toLowerCase().indexOf('tr') > -1) stype=12;   //Turkey
		else if (language.toLowerCase().indexOf('vn') > -1) stype=13;   //Vietnam
		else if (language.toLowerCase().indexOf('tw') > -1) stype=14;   //Taiwan
		else if (language.toLowerCase().indexOf('cn') > -1) stype=15;   //China
		else stype=0;

		return stype;
  }
}

var stype = getCookie('language');
function showText(text)
{
	if(text[stype]=="" || typeof(text[stype])=="undefined")
		return text[0];
	else
		return text[stype];
}
function dw(message,defaultMs)
{		
	document.write(showText(message));	
}

function setFocus(field) 
{
	field.value = field.defaultValue;
	field.focus();
	return;
}

function checkFormat(str,msg)
{
	i=1;
	while ( str.length!=0)
	{
		if ( str.charAt(0) == '.' ) i++;
			str = str.substring(1);
	}
	if ( ( i > 4 ) || (i < 4) )
	{
		alert(msg);
		return false;
	}
	else 
		return true;
}

function validateKey(str)
{
	for (var i=0; i<str.length; i++)
	{
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' )) continue;
		return 0;
	}
	return 1;
}

function valiIntdate(str)
{
	for (var i=0; i<str.length; i++)
	{
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9')) continue;
		return 0;
	}
	return 1;
}

function getDigit(str, num)
{
	i=1;
	if ( num != 1 )
	{
		while (i!=num && str.length!=0)
		{
			if ( str.charAt(0) == '.' ) i++;
			str = str.substring(1);
		}
		if ( i!=num ) 
			return -1;
	}
	for (i=0; i<str.length; i++)
	{
		if ( str.charAt(i) == '.' )
		{
			str = str.substring(0, i);
			break;
		}
	}

	if ( str.length == 0) return -1;

	d = parseInt(str, 10);
	return d;
}

function checkDigitRange(str, num, min, max)
{
	d = getDigit(str,num);
	if ( d > max || d < min ) 
		return false;
	return true;
}

function checkIpAddr(field, msg)
{
	if ( validateKey(field.value) == 0) {
		alert(''+msg+' '+showText(shouldbeSomething).replace(/####/,showText(decimalnumber)));
		return false;
	}
	if ( !checkDigitRange(field.value,1,0,255) ) {
		alert(''+msg+' '+showText(shouldbeSomethingRange).replace(/####/,"0-255").replace(/####@/,"1")+'.');
		return false;
	}
	if ( !checkDigitRange(field.value,2,0,255) ) {
		alert(''+msg+' '+showText(shouldbeSomethingRange).replace(/####/,"0-255").replace(/####@/,"2")+'.');
		return false;
	}
	if ( !checkDigitRange(field.value,3,0,255) ) {
		alert(''+msg+' '+showText(shouldbeSomethingRange).replace(/####/,"0-255").replace(/####@/,"3")+'.');
		return false;
	}
	if ( !checkDigitRange(field.value,4,0,254) ) {
		alert(''+msg+' '+showText(shouldbeSomethingRange).replace(/####/,"1-254").replace(/####@/,"4")+'.');
		return false;
	}
	return true;
}
function strRule(name, str) 
{
	if(name.value=="") 
	{
		alert(showText(SomethingCannotBeEmpty).replace(/#####/,str));
		setFocus(name);
		return false;
	}

	return true;
}
function ipRule(ip, str, type, defVal)
{
	if (type == "ip")
	{
		if (ip.value=="")
		{
			alert(showText(SomethingCannotBeEmpty).replace(/#####/, str) + ' ' + showText(ItShouldBe4DigitInIpFormat));
			if (defVal == 1)
				ip.value = ip.defaultValue;
      
			ip.focus();
			return false;
		}
	}
	else
	{
		if (ip.value=="" || ip.value=="0.0.0.0")
		{
			ip.value = "0.0.0.0";
			return true;
		}
	}

	if ( checkFormat(ip.value,showText(InvalidSomething).replace(/#####/, str) + ' ' + showText(ItShouldBe4DigitInIpFormat)) == false)
		return false;

	if ( checkIpAddr(ip, showText(InvalidSomething).replace(/#####/, str) ) == false )
	{
		if (defVal == 1)
			ip.value = ip.defaultValue;
		
		ip.focus();
		return false;
	}
	return true;
}
function ipRuleAND(ip, mask, gateway, str, type, defVal)
{
	var check="OK";
	if (type == "gw")
	{
		ipOne = getDigit(ip.value,1);
		ipTwo = getDigit(ip.value,2);
		ipThree = getDigit(ip.value,3);
		ipFour = getDigit(ip.value,4);

		maskOne = getDigit(mask.value,1);
		maskTwo = getDigit(mask.value,2);
		maskThree = getDigit(mask.value,3);
		maskFour = getDigit(mask.value,4);

		gatewayOne = getDigit(gateway.value,1);
		gatewayTwo = getDigit(gateway.value,2);
		gatewayThree = getDigit(gateway.value,3);
		gatewayFour = getDigit(gateway.value,4);

		if ( (ipOne & maskOne) != (gatewayOne & maskOne) ) { check="Fail"; }
		if ( (ipTwo & maskTwo) != (gatewayTwo & maskTwo) ) { check="Fail"; }
		if ( (ipThree & maskThree) != (gatewayThree & maskThree) ) { check="Fail"; }
		if ( (ipFour & maskFour) != (gatewayFour & maskFour) ) { check="Fail"; }
		if ( check=="OK" ) { return true; }

		if (confirm(str))
		{
			return true;
		}
		else
		{
			if (defVal == 1)
				gateway.value = gateway.defaultValue;
			return false;
		}
	}
}
function portRule( fromPort, fromStr, toPort, toStr, min, max, defVal) {
//********** from port **********
	if (fromPort.value=="") {
		alert(showText(InvalidSomething).replace(/#####/, fromStr)+" "+showText(shouldsetvaluebetween)+" "+min+"-"+max+".");
		if ( defVal == 1 )
			fromPort.value = fromPort.defaultValue;
		fromPort.focus();
		return false;
	}
	d1 = getDigit(fromPort.value, 1);
	if (d1 > max || d1 < min) {
		alert(showText(InvalidSomething).replace(/#####/, fromStr)+" "+showText(shouldsetvaluebetween)+" "+min+"-"+max+".");
		if ( defVal == 1 )
			fromPort.value = fromPort.defaultValue;
		fromPort.focus();
		return false;
	}
	if ( validateKey( fromPort.value ) == 0 ) {
		alert(showText(InvalidSomething).replace(/#####/, fromStr)+" "+showText(shouldbeSomething).replace(/####/, showText(decimalnumber)));
		if ( defVal == 1 )
			fromPort.value = fromPort.defaultValue;
		fromPort.focus();
		return false;
	}
	
//********** to port **********
	if (toStr != "") {
		if (toPort.value!="") {
  			if ( validateKey( toPort.value ) == 0 ) {
				alert("["+toStr+"] It should be the decimal number (0-9).");
				if ( defVal == 1 )
					toPort.value = toPort.defaultValue;
				toPort.focus();
				return false;
  			}
			d2 = getDigit(toPort.value, 1);
 			if (d2 > max || d2 < min) {
				alert("["+toStr+"] You should set a value between "+min+"-"+max+".");
				if ( defVal == 1 )
					toPort.value = toPort.defaultValue;
				toPort.focus();
				return false;
  			}
			if (d1 > d2 ) {
				alert("Invalid port range! 1st port value should be less than 2nd value.");
				if ( defVal == 1 )
					toPort.value = toPort.defaultValue;
				fromPort.focus();
				return false;
			}
		}
	}
	return true;
}

function portRule1( fromPort, fromStr, defVal) {
//********** from port **********
	if (fromPort.value=="") {
		alert(showText(InvalidSomething).replace(/#####/, fromStr));
		if ( defVal == 1 )
			fromPort.value = fromPort.defaultValue;
		fromPort.focus();
		return false;
	}
	if ( valiIntdate( fromPort.value ) == 0 ) {
		alert(showText(InvalidSomething).replace(/#####/, fromStr)+" "+showText(shouldbeSomething).replace(/####/, showText(decimalnumber)));
		if ( defVal == 1 )
			fromPort.value = fromPort.defaultValue;
		fromPort.focus();
		return false;
	}
	
	return true;
}

function checkMask(str, num, msg)
{
  d = getDigit(str,num);
  
  switch(num)
  {
    case 1:
      if( !(d==0 || d==128 || d==192 || d==224 || d==240 || d==248 || d==252 || d==254 || d==255 ))
      {
		alert(msg + ' \nIt should be the number of 0, 128, 192, 224, 240, 248, 252, 254 or 255.');
        return false;
      }
      break;
    case 2:
      if( !(d==0 || d==128 || d==192 || d==224 || d==240 || d==248 || d==252 || d==254 || d==255 ))
      {
        alert(msg + ' \nIt should be the number of 0, 128, 192, 224, 240, 248, 252, 254 or 255.');
        return false;
      }
      break;
    case 3:
      if( !(d==0 || d==128 || d==192 || d==224 || d==240 || d==248 || d==252 || d==254 || d==255 ))
      {
        alert(msg + ' \nIt should be the number of 0, 128, 192, 224, 240, 248, 252, 254 or 255.');
        return false;
      }
      break;
    case 4:
      if( !(d==0 || d==128 || d==192 || d==224 || d==240 || d==248 || d==252 || d==254 || d==255 ))
      {
        alert(msg + ' \nIt should be the number of 0, 128, 192, 224, 240, 248, 252 or 254 or 255.');
        return false;
      }
      break;
    default:
      break;
  }
  return true;
}

function maskRule (mask, maskstr, defVal)
{
	if (mask.value == "")
	{
		alert(showText(SomethingCannotBeEmpty).replace(/#####/, maskstr) + ' ' + showText(ItShouldBe4DigitInIpFormat));
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	if (mask.value == "0.0.0.0")
	{
		alert( showText(InvalidSomething).replace(/#####/, maskstr));
		mask.focus();
		return false;
	}  
	if ( validateKey( mask.value ) == 0 )
	{
		alert( showText(InvalidSomething).replace(/#####/, maskstr) + ' ' + showText(ItShouldBe4DigitInIpFormat));
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	if ( !checkMask(mask.value,1, showText(InvalidSomething).replace(/#####/, maskstr)) )
	{
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	if ( !checkMask(mask.value,2, showText(InvalidSomething).replace(/#####/, maskstr)) )
	{
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	if ( !checkMask(mask.value,3, showText(InvalidSomething).replace(/#####/, maskstr)) )
	{
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	if ( !checkMask(mask.value,4, showText(InvalidSomething).replace(/#####/, maskstr)) )
	{
		if (defVal == 1) mask.value = mask.defaultValue;
			mask.focus();
		return false;
	}
	
	if( !checkmaskRule(mask.value))
    {
        alert("Subnet mask ip can't use "+mask.value+", try others !");
                
        if (defVal == 1)
			mask.value = mask.defaultValue;
		mask.focus();
		return false;
    }
	return true;
}

function checkmaskRule(mask)
{
	var tmpArray = new Array();
	patt = new RegExp("^[0-9]\{1,3\}[.][0-9]\{1,3\}[.][0-9]\{1,3\}[.][0-9]\{1,3\}$");
	if( ! patt.test(mask) || mask=="0.0.0.0") return false;
	tmpStr = mask;
	j=0;
	while(tmpStr != "")
	{
		stop = (tmpStr.indexOf(".")==-1)?(tmpStr.length):(tmpStr.indexOf("."));
		number = parseInt( tmpStr.substr(0, stop) );

		if(number>=0 && number<=255)
		{
			for(i=0;i<8;i++)
			{
				tmpArray[j*8 + (7-i)] = number % 2;
				number=parseInt(number/2);
			}
		}
		else return false;
		tmpStr = tmpStr.substr(stop+1, tmpStr.length);
		j++;
	}
	
	j=1;
	for(i=0;i<32;i++)
	{
		if(j==1 && tmpArray[i]==0) j=0;
		else if(j==0 && tmpArray[i]==1) return false;
	}
	return true;
}

function checkSubnet(ip, mask, client)
{
	ip_d = getDigit(ip, 1);
	mask_d = getDigit(mask, 1);
	client_d = getDigit(client, 1);
	if ( (ip_d & mask_d) != (client_d & mask_d ) ) return false;

	ip_d = getDigit(ip, 2);
	mask_d = getDigit(mask, 2);
	client_d = getDigit(client, 2);
	if ( (ip_d & mask_d) != (client_d & mask_d ) ) return false;

	ip_d = getDigit(ip, 3);
	mask_d = getDigit(mask, 3);
	client_d = getDigit(client, 3);
	if ( (ip_d & mask_d) != (client_d & mask_d ) ) return false;

	ip_d = getDigit(ip, 4);
	mask_d = getDigit(mask, 4);
	client_d = getDigit(client, 4);
	if ( (ip_d & mask_d) != (client_d & mask_d ) ) return false;

	return true;
}

function macRule( mac,defVal) {

	var str = mac.value;
	if ( str.length < 12) {
		alert(showText(InvalidSomething).replace(/#####/,showText(macAddr))+' '+showText(shouldbeSomething).replace(/####/,showText(digits12)));
		if ( defVal == 1 )
			mac.value = mac.defaultValue;
		mac.focus();
		return false;
	}

	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;

		alert(showText(InvalidSomething).replace(/#####/,showText(macAddr))+' '+showText(shouldbeSomething).replace(/####/,showText(hexnumber)));
		if ( defVal == 1 )
			mac.value = mac.defaultValue;
		mac.focus();
		return false;
	}
		return true;
}

function multimacRule( mac, macindex, defVal) {

	var str = mac.value;
	if ( str.length < 12) {
		alert(showText(InvalidSomething).replace(/#####/,showText(macAddr)).replace(/Address./,"Address "+macindex+".")+' '+showText(shouldbeSomething).replace(/####/,showText(digits12)));
		if ( defVal == 1 )
			mac.value = mac.defaultValue;
		mac.focus();
		return false;
	}

	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;

		alert(showText(InvalidSomething).replace(/#####/,showText(macAddr)).replace(/Address./,"Address "+macindex+".")+' '+showText(shouldbeSomething).replace(/####/,showText(hexnumber)));
		if ( defVal == 1 )
			mac.value = mac.defaultValue;
		mac.focus();
		return false;
	}
		return true;
}


function subnetRule(ip, mask, client, str, str1) 
{
	if (client.value!="" && client.value!="0.0.0.0") 
	{
		if ( !checkSubnet(ip.value, mask.value, client.value)) 
		{
			alert(showText(InvalidSomething).replace(/#####/,str)+' '+showText(SomethingAndSomethingShouldInSameSubnet).replace(/#####/,str).replace(/####@/,str1));
            client.value = client.defaultValue;
			client.focus();
            return false;
        }
    }
	return true;
}


function MM_openBrWindow(theURL,winName,features)
{
  window.open(theURL,winName,features);
}

function ShowHelpLink(keyword)
{
	if(keyword!="") document.write('<a href="#"onclick="MM_openBrWindow(\'help.asp#'+keyword+'\',\'help\',\'scrollbars=yes,width=600,height=500\')"><img src="file/info_s.gif" width="20" height="20" border="0" align="middle"></a>');
}
function openWindow(url, windowName)
{
        if(url =="/wlsurvey2.asp")
                var wide=800;
        else if(url.search("wlwdsenp")>=0)
                var wide=400;
        else
        	var wide=700;
        var high=520;
        if (document.all)
                var xMax = screen.width, yMax = screen.height;
        else if (document.layers)
                var xMax = window.outerWidth, yMax = window.outerHeight;
        else
           var xMax = 640, yMax=500;
        var xOffset = (xMax - wide)/2;
        var yOffset = (yMax - high)/3;

        var settings = 'width='+wide+',height='+high+',screenX='+xOffset+',screenY='+yOffset+',top='+yOffset+',left='+xOffset+', resizable=yes, toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes';

        window.open( url, windowName, settings );
}
function deleteClick()
{
	if ( !confirm(showText(DoYouReallyWantToDeleteSelected)) ) return false;
	else return true;
}

function deleteAllClick()
{
	if ( !confirm(showText(DoYouReallyWantToDeleteAll)) ) return false;
	else return true;
}

function makeRequest(url)
{
var http_request = false;
if (window.XMLHttpRequest) // Mozilla, Safari,...
{
http_request = new XMLHttpRequest();
if (http_request.overrideMimeType)
{http_request.overrideMimeType('text/html');}}
else if (window.ActiveXObject) // IE
{
try
{http_request = new ActiveXObject("Msxml2.XMLHTTP");}
catch (e)
{
try
{http_request = new ActiveXObject("Microsoft.XMLHTTP");}
catch (e)
{}}}
if (!http_request)
{
alert('Cannot create XMLHTTP instance');
return false;
}
http_request.onreadystatechange = function()
{
if (http_request.readyState == 4)
{top.window.location.replace(url);}}
http_request.open('GET', url, true);
http_request.send(null);
}
function HelpButton()
{
	var showHelp=0;
	if(showHelp){
		document.write('<input type="button" id="help-down" value="Help" class="help-d1-button">');
		$(document).ready(function(){
			$("#help-down").click(function(){
				$("#help-box").slideToggle();
			}); 
		});
	}
}

disableButtonColor='#6E6E6E';
enableButtonColor='#FFFFFF';

