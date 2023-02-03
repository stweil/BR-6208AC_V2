<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<link rel="stylesheet" href="/set.css">
<SCRIPT>
var methodVal = <%getiNICVar("secMode","","");%>;
var wepLen = <% getiNICVar("wep","","");%>;
var keyTypeTbl =	<%getiNICInfo("keyTypeAll");%>;
var wpa_wpa2_Cipher=	<%getiNICInfo("wpa_wpa2_Cipher");%>;
var wpaCipher =	wpa_wpa2_Cipher[0];
var wpa2Cipher =	wpa_wpa2_Cipher[1];
var pskFormatTbl =	<%getiNICInfo("pskFormatAll");%>;
var pskValueTbl =	<%getiNICInfo("pskValueAll");%>;
var wizMode = <%getVar("wizmode","","");%>;

function wlValidateKey(idx, str, len) {
 
  if (str.length ==0)
  	return 1;

  if ( str.length != len) {
  	idx++;
	alert(showText(InvalidlengthofKey).replace(/\' \+ idx \+ \'/, idx).replace(/\' \+ len \+ \'/, len)    );
	return 0;
  }

  if ( str == "*****" ||
       str == "**********" ||
       str == "*************" ||
       str == "****************" ||
       str == "**************************" ||
       str == "********************************" )
       return 1;

  if (document.wlEncrypt.format.selectedIndex==0)
       return 1;

  for (var i=0; i<str.length; i++) {
    if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*' )
			continue;

	alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
	return 0;
  }

  return 1;
}

function updateFormat() {
  if (document.wlEncrypt.length.selectedIndex == 0) {
    document.wlEncrypt.format.options[0].text = showText(ascii_5cha);
    document.wlEncrypt.format.options[1].text = showText(Hex_10cha);
  }
  else if (document.wlEncrypt.length.selectedIndex == 1){
    document.wlEncrypt.format.options[0].text = showText(ascii_13cha);
    document.wlEncrypt.format.options[1].text = showText(hex_26cha);
  }
	document.wlEncrypt.format.options[0].selected=true;

  setWepKeyLength();	
	setPskKeyLength();
}

function setPskKeyLength()
{
	if (document.wlEncrypt.wpaPskFormat.selectedIndex == 0)
		document.wlEncrypt.wpaPskValue.maxLength = 63;
	else
		document.wlEncrypt.wpaPskValue.maxLength = 64;
}

function setWepKeyLength() {


	with(document.wlEncrypt) {
		if (length.selectedIndex == 0) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 5;
			}
			else {
				key1.maxLength = 10;
			}
		}
		else if (length.selectedIndex == 1) {
			if (format.selectedIndex == 0) {
				key1.maxLength = 13;
			}
			else {
				key1.maxLength = 26;
			}
		}
	}
}

function checkpassword(input) {

	var x = input
	if (x.value != "")
	{
		if (x.value.indexOf("$") != -1 || x.value.indexOf(",") != -1 || x.value.indexOf(';') != -1 || x.value.indexOf("\\") != -1)
		{
			alert(showText(PasswordNoSupport2));
			x.focus();
			return false
		}
		else
			return true
	}
	else
		return true
}

function saveChanges() {
	var keyLen;
	var strMethod = document.wlEncrypt.method.value;
	if ( strMethod==1) {
	
		if (document.wlEncrypt.length.selectedIndex == 0) {
			if ( document.wlEncrypt.format.selectedIndex == 0)
				keyLen = 5;
			else
				keyLen = 10;
		}
		else if (document.wlEncrypt.length.selectedIndex == 1) {
			if ( document.wlEncrypt.format.selectedIndex == 0)
				keyLen = 13;
			else
				keyLen = 26;
		}
		else {
			if ( document.wlEncrypt.format.selectedIndex == 0)
				keyLen = 16;
			else
				keyLen = 32;
		}

		if (wlValidateKey(0,document.wlEncrypt.key1.value, keyLen)==0) {
			document.wlEncrypt.key1.focus();
			return false;
		}
		
	}

	if (strMethod==2 || strMethod==3)//WPA
	{ 
		var str = document.wlEncrypt.wpaPskValue.value;
		if (document.wlEncrypt.wpaPskFormat.selectedIndex==1) 
		{
			if (str.length != 64) 
			{
				alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
				document.wlEncrypt.wpaPskValue.focus();
				return false;
			}
			takedef = 0;
			defPskFormat=document.wlEncrypt.wpaPskFormat.selectedIndex;
			defPskLen=document.wlEncrypt.wpaPskValue.length;
			if (defPskFormat == 1 && defPskLen == 64) 
			{
				for (var i=0; i<64; i++) 
				{
						if ( str.charAt(i) != '*')
						break;
				}
				if (i == 64 )
					takedef = 1;
			}
			if (takedef == 0) 
			{
				for (var i=0; i<str.length; i++) 
				{
						if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
						(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
						(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || str.charAt(i) == '*')
						continue;
						alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, showText(ItShouldBeHEX)));
						document.wlEncrypt.wpaPskValue.focus();
						return false;
				}
			}
		}
		else 
		{
			if (!checkpassword(document.wlEncrypt.wpaPskValue)) return false;

			if (str.length < 8) 
			{
				alert(showText(pskshouldbe8));
				document.wlEncrypt.wpaPskValue.focus();
				return false;
			}
			if (str.length > 63) 
			{
				alert(showText(pskshouldbe64));
				document.wlEncrypt.wpaPskValue.focus();
				return false;
			}
		}
	}
	return true;
}

function lengthClick() {
  updateFormat();
}

function displayObj() {

	var wepTbl = new Array("none","block","none","none");
	var wpaTbl = new Array("none","none","block","block");

		
	for (i=0; i<4; i++) {
		if (document.wlEncrypt.method.value == i) {
			document.getElementById('wepId').style.display = wepTbl[i];
			document.getElementById('wpaId').style.display = wpaTbl[i];
		}
	}
	updateFormat();
}

</SCRIPT>
</head>
<body style="margin-top:0px; margin-left:0px;">
<form action=/goform/formwlencrypt24g method=POST name="wlEncrypt">
<input type="hidden" name="wepEnabled" value="ON">
<table border="0" style="width:570px;" align="center" cellspacing="0">
	<tr>
		<td style="width:200px;" align="right" class="Arial_16"><script>dw(Encryption);</script>&nbsp;&nbsp;&nbsp;</td>
		<td style="width:285px;">
			<select size="1" name="method" onChange="displayObj();" class="Arial_16">
				<script>
					var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA),showText(WPA2),showText(WPA_RADIUS),showText(WPA2_RADIUS));
					for ( i=0; i<4; i++) {
						if ( i == 0)
							document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
						else
							document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
					}
				</script>
			</select>
		</td>
	</tr>
</table>

<table border="0" style="width:570px;" align="center" cellspacing="0" id="wepId">
	<tr>
		<td style="width:236px;" align="right" class="Arial_16"><script>dw(keyLen);</script>&nbsp;&nbsp;&nbsp;</td>
		<td style="width:334px;">
			<select size="1" name="length" ONCHANGE="lengthClick();" class="Arial_16">
				<script>
					document.write('<option value=1 selected>'+showText(bit64)+'</option>')
					document.write('<option value=2>'+showText(bit128)+'</option>')
				</script>
			</select>
		</td>
	</tr>
		
	<tr><td align="right" class="Arial_16"><script>dw(KeyFormat);</script>&nbsp;&nbsp;&nbsp;</td>
			<td>
				<select size="1" name="format" ONCHANGE="setWepKeyLength();" class="Arial_16">
					<option value=1 selected>ASCII</option>
					<option value=2>Hex</option>  
				</select>
			</td>
	</tr>
	
	<tr>
		<td style="width:236px;" align="right" class="Arial_16">
			<script>
				if((wizMode==2) || (wizMode==3) || (wizMode==4))
					dw(wiz_repmenu2);
				else
					dw(Key);
			</script>&nbsp;&nbsp;&nbsp;
		</td>
		<td><input type="text" name="key1" size="30" maxlength="32" class="Arial_16"></td>
	</tr>
</table>

<span  id="wpaId" style="display:none">
	<table border="0" style="width:570px;" align="center" cellspacing="0">
		<tr>
			<td style="width:236px;line-height:30px;" align="right" class="Arial_16"><script>dw(SecurityType)</script>&nbsp;&nbsp;&nbsp;</td>
			<td style="width:334px;" class="Arial_16">
				<script>
					document.write('<input type="radio" id="wpaCipher1" name="wpaCipher" value="1"');
					document.write('>TKIP&nbsp;&nbsp;');
					document.write('<input type="radio" id="wpaCipher2" name="wpaCipher" value="2"');
					document.write(' checked');
					document.write('>AES&nbsp;&nbsp;');
				</script>
			</td>
		</tr>
		
		<tr>
			<td align="right" class="Arial_16"><script>dw(KeyFormat);</script>&nbsp;&nbsp;&nbsp;</td>
			<td align="left">
				<select size="1" name="wpaPskFormat" ONCHANGE="setPskKeyLength()" class="Arial_16">
					<script>
						document.write('<option value=0 selected>'+showText(Passphrase)+'</option>')
						document.write('<option value=1>'+showText(hex_64cha)+'</option>')
					</script>
				</select>
			</td>
		</tr>
		
		<tr>
			<td style="width:236px;" align="right" class="Arial_16">
				<script>
					if((wizMode==2) || (wizMode==3) || (wizMode==4))
						dw(wiz_repmenu2);
					else
						dw(Key);
				</script>&nbsp;&nbsp;&nbsp;
			</td>
			<td align="left"><input type="text" name="wpaPskValue" id="wpaPskValue" size="30" maxlength="64" value="" class="Arial_16"></td>
		</tr>
	</table>
</span>

<script>
displayObj();
</script>
</form>
</body>
</html>
