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

<script>
apMode=3;

var methodVal = <%getVar("wdsEncrypt","","");%>;
var opMode = <%getVar("opMode","","");%>;
var APSecMode = <%getVar("secMode","","");%>;

function wlValidateKey(idx, str, len)
{
	//if (str.length ==0) return 1;
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
	if (document.formWdsEncrypt.format.selectedIndex==0)
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

function updateFormat()
{
  if (document.formWdsEncrypt.length.selectedIndex == 0) {
    document.formWdsEncrypt.format.options[0].text = showText(ascii_5cha);
    document.formWdsEncrypt.format.options[1].text = showText(Hex_10cha);

		document.formWdsEncrypt.wdsformat.options[0].text = showText(ascii_5cha);
    document.formWdsEncrypt.wdsformat.options[1].text = showText(Hex_10cha);
  }
  else if (document.formWdsEncrypt.length.selectedIndex == 1){
    document.formWdsEncrypt.format.options[0].text = showText(ascii_13cha);
    document.formWdsEncrypt.format.options[1].text = showText(hex_26cha);

		document.formWdsEncrypt.wdsformat.options[0].text = showText(ascii_13cha);
    document.formWdsEncrypt.wdsformat.options[1].text = showText(hex_26cha);
  }
	
	var type = <%getVar("keyType","","");%>;
  document.formWdsEncrypt.format.options[type].selected=true;
	document.formWdsEncrypt.wdsformat.options[type].selected=true;
	

  setDefaultKeyValue();
}

function setDefaultKeyValue()
{
	if (document.formWdsEncrypt.length.selectedIndex == 0) {
		if ( document.formWdsEncrypt.format.selectedIndex == 0) {
			document.formWdsEncrypt.key1.maxLength = 5;
			document.formWdsEncrypt.wdskey1.maxLength = 5;
			
			document.formWdsEncrypt.key1.value = "*****";
			document.formWdsEncrypt.wdskey1.value = "*****";
			
		}
		else {
			document.formWdsEncrypt.key1.maxLength = 10;
			document.formWdsEncrypt.wdskey1.maxLength = 10;
			document.formWdsEncrypt.key1.value = "**********";
			document.formWdsEncrypt.wdskey1.value = "**********";
		}
	}
	else if (document.formWdsEncrypt.length.selectedIndex == 1) {
  		if ( document.formWdsEncrypt.format.selectedIndex == 0) {
			document.formWdsEncrypt.key1.maxLength = 13;
			document.formWdsEncrypt.wdskey1.maxLength = 13;
			document.formWdsEncrypt.key1.value = "*************";
			document.formWdsEncrypt.wdskey1.value = "*************";
		}
		else {
			document.formWdsEncrypt.key1.maxLength = 26;
			document.formWdsEncrypt.wdskey1.maxLength = 26;
			document.formWdsEncrypt.key1.value = "**************************";
			document.formWdsEncrypt.wdskey1.value = "**************************";
		}
	}
}

function saveChanges()
{
	var keyLen;
	var strMethod = document.formWdsEncrypt.method.value;
	var str = document.formWdsEncrypt.wdsPskValue.value;

	if ( strMethod==1) {
		if (document.formWdsEncrypt.length.selectedIndex == 0) {
			if ( document.formWdsEncrypt.format.selectedIndex == 0)
				keyLen = 5;
			else
				keyLen = 10;
		}
		else if (document.formWdsEncrypt.length.selectedIndex == 1) {
			if ( document.formWdsEncrypt.format.selectedIndex == 0)
				keyLen = 13;
			else
				keyLen = 26;
		}
		else {
			if ( document.formWdsEncrypt.format.selectedIndex == 0)
				keyLen = 16;
			else
				keyLen = 32;
		}

		if (wlValidateKey(0,document.formWdsEncrypt.key1.value, keyLen)==0) {
			document.formWdsEncrypt.key1.focus();
			return false;
		}
	}
	//********** psk **********
	if (strMethod==2) {
		if (document.formWdsEncrypt.wdsPskFormat.selectedIndex==1) {
			if (str.length != 64) {
				alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
				document.formWdsEncrypt.wdsPskValue.focus();
				return false;
			}
			takedef = 0;
			defPskFormat=document.formWdsEncrypt.wdsPskFormat.selectedIndex;
			defPskLen=document.formWdsEncrypt.wdsPskValue.value.length;
			if (defPskFormat == 1 && defPskLen == 64) {
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
					document.formWdsEncrypt.wdsPskValue.focus();
					return false;
					}
			}
		}
		else {
			if (str.length < 8) {
				alert(showText(pskshouldbe8));
				document.formWdsEncrypt.wdsPskValue.focus();
				return false;
			}
			if (str.length > 63) {
				alert(showText(pskshouldbe64));
				document.formWdsEncrypt.wdsPskValue.focus();
				return false;
			}
		}
	}

  return true;
}

function lengthClick()
{
  updateFormat();
}

function displayObj() {
	var wepTbl = new Array("none","block","none","none");
	var wpaTbl = new Array("none","none","block","block");
	var pskTbl = new Array("none","none","block","none");

	document.getElementById('wepIdShowOnly').style.display = "none";

	for (i=0; i<3; i++) {
		if (document.formWdsEncrypt.method.value == i) {
			if(apMode == 3 && APSecMode == 1)
			{
				document.getElementById('wepId').style.display = "none";

				document.getElementById('wepIdShowOnly').style.display = "block";
				document.formWdsEncrypt.wdslength.disabled = true;
				document.formWdsEncrypt.wdsformat.disabled = true;
				document.formWdsEncrypt.wdskey1.disabled = true;
			}
			else
				document.getElementById('wepId').style.display = wepTbl[i];

			document.getElementById('wpaId').style.display = wpaTbl[i];
  		document.getElementById('pskId').style.display = pskTbl[i];
			if(apMode == 3 && APSecMode == 2)
				document.formWdsEncrypt.showwdsPskFormat.disabled = true;
	   }
	}

	lengthClick();
}

</script>
</head>    

<body>
<blockquote>

<p align="center">
	<br>
  <span class="tiltleText"><script>dw(apmode3)</script>&nbsp;<script>dw(SecuritySetting)</script></span>
  <!--<br>
  <br>
  <span class="itemText"><script>dw(WDSSecurityInfo)</script></span>-->
  <br>
  <br>

	<form action="/goform/formWdsEncrypt" method=POST name="formWdsEncrypt">
	<input type="hidden" name="wepEnabled" value="ON">
			
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(Encryption)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="method" onChange="displayObj();">
							<script>
								var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA_pre_shared_key));
								if( apMode == 3 )
									methodVal = APSecMode;
								for ( i=0; i<3; i++)
								{
									if ( i == methodVal)
									{
										if (opMode==1)
										{
											if (i!=3)
												document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
										}
										else
											document.write('<option selected value="'+i+'">'+modeTbl[i]+'</option>');
									}
									else
									{
										if( apMode == 3 )
										{
											if(APSecMode == i )
												document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
											else if( i==2 && APSecMode == 3 )
												document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
										}
										else
										{
											document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');	
										}
									}
								}
							</script>
						</select>
					</div></td>
				</tr>
			</table>

			<span id="wepId">
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(keyLen)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="length" ONCHANGE=lengthClick()>
							<script>	
								var wepmode = <%getVar("wep","","");%>;				
								if(wepmode != 2)
								{
									document.write('<option value="1" selected>'+showText(bit64)+'</option>')
									document.write('<option value="2">'+showText(bit128)+'</option>')
								}
								else
								{
									document.write('<option value="1">'+showText(bit64)+'</option>')
									document.write('<option value="2" selected>'+showText(bit128)+'</option>')
								}
							</script>				
						</select>
					</div></td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(KeyFormat)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="format" ONCHANGE=setDefaultKeyValue()>
							<option value="1"><script>dw(ascii)</script></option>
							<option value="2"><script>dw(Hex)</script></option>
						</select>
					</div></td>
				</tr>
			</table>

			<input type="hidden" value="1" name="defaultTxKeyId">
			
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(EncryptionKey)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;"><input type="text" name="key1" size="32" maxlength="32"></td>
				</tr>
			</table>
			</span>
<!--==================================================================================================================================================-->
			<span id="wepIdShowOnly" style="display:none">
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(keyLen)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="wdslength" ONCHANGE="">
							<script>	
								var wepkeyLen = <%getVar("wep","","");%>;				
								if(wepkeyLen != 2)
								{
									document.write('<option value="1" selected>'+showText(bit64)+'</option>')
									document.write('<option value="2">'+showText(bit128)+'</option>')
								}
								else
								{
									document.write('<option value="1">'+showText(bit64)+'</option>')
									document.write('<option value="2" selected>'+showText(bit128)+'</option>')
								}
							</script>				
						</select>
					</div></td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(KeyFormat)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="wdsformat" ONCHANGE="">
							<option value="1"><script>dw(ascii)</script></option>
							<option value="2"><script>dw(Hex)</script></option>
						</select>
					</div></td>
				</tr>
			</table>
			
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(EncryptionKey)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;"><input type="text" name="wdskey1" size="32" maxlength="32"></td>
				</tr>
			</table>
			</span>
<!--======================================================================== end =======================================================================-->
			<span id="wpaId" style="display:none">
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(WPA_Unicast_Cipher_Suite)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;"><span class="choose-itemText">
						<script>
							var wpaCipher = <%getVar("wpaCipher","","");%>;
							var wdswpaCipher = <%getVar("wdsWpaCipher","","");%>;
							if(wpaCipher == 1)
								document.write('<input type="radio" name="wdsWpaCipher" value="1" checked>'+showText(WPA_TKIP));
							else
								document.write('<input type="radio" name="wdsWpaCipher" value="2" checked>'+showText(WPA2_AES));
						</script>
					</span></div></td>
				</tr>
			</table>
			</span>

<!--==================================================================================================================================================-->
			<span id="wdspskId" style="display:none">
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(Pre_shared_Key_Format)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="wdsPskFormat">
							<script>	
								var wdsPskFormat = <%getVar("wdsPskFormat","","");%>;				
								var pskFormat = <%getVar("pskFormat","","");%>;
								//if(wdsPskFormat == 0)
								if(pskFormat == 0)
								{
									document.write('<option value="0" selected>'+showText(Passphrase)+'</option>')
									document.write('<option value="1">'+showText(hex_64cha)+'</option>')
								}
								else
								{
									document.write('<option value="0">'+showText(Passphrase)+'</option>')
									document.write('<option value="1" selected>'+showText(hex_64cha)+'</option>')
								}
							</script>				
						</select>
					</div></td>
				</tr>
			</table>
			</span>
<!--====================================================================== end =======================================================================-->


			<span id="pskId">
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(Pre_shared_Key_Format)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<select size="1" name="showwdsPskFormat">
							<script>	
								var wdsPskFormat = <%getVar("wdsPskFormat","","");%>;				
								var pskFormat = <%getVar("pskFormat","","");%>;
								//if(wdsPskFormat == 0)
								if(pskFormat == 0)
								{
									document.write('<option value="0" selected>'+showText(Passphrase)+'</option>')
									//document.write('<option value="1">'+showText(hex_64cha)+'</option>')
								}
								else
								{
									//document.write('<option value="0">'+showText(Passphrase)+'</option>')
									document.write('<option value="1" selected>'+showText(hex_64cha)+'</option>')
								}
							</script>				
						</select>
					</div></td>
				</tr>
			</table>

			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td><div style="text-align:right; width:200px;  padding-right:10px; FONT-SIZE: 10pt; COLOR: #6e6e6e; FONT-FAMILY: Arial;"><script>dw(Pre_shared_Key)</script></div></td>
					<td><div style="text-align:left; width:250px;  padding-left:1px;">
						<script>
							var wdsPskValue =	<%getInfo("wdsPskValue");%>;
							var pskval = wdsPskValue[0];
							if (pskval.search('"') != -1)  
							{
									pskval = pskval.replace(/\"/g, "&quot;");
							}
							document.write('<input type="text" name="wdsPskValue" size="32" maxlength="64" value="'+pskval+'">')
						</script>
					</div></td>
				</tr>
			</table>
			</span>

			<br><br>
			<table align="center" border="0" Cellspacing="0" width="450">
				<tr>
					<td align="center">
						<script>
							document.write('<input type="submit" value="'+showText(save)+'" onClick="return saveChanges()" class="ui-button-nolimit">&nbsp;&nbsp;')
							document.write('<input type="reset" Value="'+showText(Close)+'" onclick="window.close()" class="ui-button-nolimit">')
						</script>
					</td>
				</tr>
			</table>
			<input type=hidden value="" name="isApply">
			<input type="hidden" value="/wlWDS3_key.asp" name="submit-url">
	</form>
			
<script>
	updateFormat();
	displayObj();
</script>
</blockquote>
</body>
</html>
