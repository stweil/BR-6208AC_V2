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

  if (document.wisp_form.stadrv_wepformat.selectedIndex==0)
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

function saveChanges() {
	var keyLen;
	var strMethod = document.wisp_form.stadrv_encrypttype.value;
	
	if (strMethod==3) //WPA2
	{ 
		var str = document.wisp_form.stadrv_WPA2pskkey.value;
		if (document.wisp_form.stadrv_WPA2pskformat.selectedIndex==1) 
		{
			if (str.length != 64) 
			{
				alert(showText(InvalidSomething).replace(/#####/, showText(Pre_shared_Key))+" "+showText(shouldbeSomething).replace(/####/, "64 "+showText(characters)+"."));
				document.wisp_form.stadrv_WPA2pskkey.focus();
				return false;
			}
			takedef = 0;
			defPskFormat=document.wisp_form.stadrv_WPA2pskformat.selectedIndex;
			defPskLen=document.wisp_form.stadrv_WPA2pskkey.length;
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
						document.wisp_form.stadrv_WPA2pskkey.focus();
						return false;
				}
			}
		}
		else 
		{
			if (str.length < 8) 
			{
				alert(showText(pskshouldbe8));
				document.wisp_form.stadrv_WPA2pskkey.focus();
				return false;
			}
			if (str.length > 63) 
			{
				alert(showText(pskshouldbe64));
				document.wisp_form.stadrv_WPA2pskkey.focus();
				return false;
			}
		}

	}
	return true;
}

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

function setPskKeyLength()
{
	if (document.wisp_form.stadrv_WPA2pskformat.selectedIndex == 0)
		document.wisp_form.stadrv_WPA2pskkey.maxLength = 63;
	else 
		document.wisp_form.stadrv_WPA2pskkey.maxLength = 64;
}
</script>
</head>
<body style="margin-top:0px; margin-left:0px;">
	<form action="/goform/formStaDrvSetup" method="POST" name="wisp_form">
		<table border="0" style="width:570px;" align="center" cellspacing="0">
			<tr>
				<td style="width:200px;" align="right" class="Arial_16"><script>dw(Encryption)</script>&nbsp;&nbsp;&nbsp;</td>
				<td style="width:285px;">
					<select size="1" name="stadrv_encrypttype" onChange="displayObj()" class="Arial_16">
							<script>
							var modeTbl = new Array(showText(disable),showText(WEP),showText(WPA),showText(WPA2),showText(WPA_RADIUS),showText(WPA2_RADIUS));
							for ( i=0; i<4; i++) { //Range Extender 0.Disable 1.WEP 2.WPA 3.WPA2
								if ( i == 0 )
									document.write('<option value="'+i+'" selected>'+modeTbl[i]+'</option>');
								else if ( i == 0 ||  i == 3)
									document.write('<option value="'+i+'">'+modeTbl[i]+'</option>');
							}
							</script>
					</select>	
				</td>
			</tr>
		</table>					
							
		
		<div id="WPA2" style="display:none">
			<input type="hidden" id="wpa2Cipher1" name="stadrv_WPA2cipher" value="2">
			<table border="0" style="width:570px;" align="center" cellspacing="0">
				<tr>
					<td style="width:236px;line-height:30px;" align="right" class="Arial_16"><script>dw(SecurityType)</script>&nbsp;&nbsp;&nbsp;</td>
					<td style="width:334px;" class="Arial_16">
						<script>
								document.write('&nbsp;AES');
							</script>
					</td>
				</tr>

				<tr>
					<td align="right" class="Arial_16"><script>dw(KeyFormat);</script>&nbsp;&nbsp;&nbsp;</td>
					<td align="left">
						<select size="1" name="stadrv_WPA2pskformat" ONCHANGE="setPskKeyLength()" class="Arial_16">
							<script>
								pskformatTbl = new Array (showText(Passphrase), showText(hex_64cha));
								for(i=0;i<2;i++)
								{
									if(i==0)
										document.write('<option selected value="'+i+'">'+pskformatTbl[i]+'</option>');
									else
										document.write('<option value="'+i+'">'+pskformatTbl[i]+'</option>');
								}
							</script>
						</select>
					</td>
				</tr>

				<tr>
					<td style="width:236px;" align="right" class="Arial_16">
						<script>
							dw(wiz_repmenu2);
						</script>&nbsp;&nbsp;&nbsp;
					</td>
					<td align="left">
						<input type="text" name="stadrv_WPA2pskkey" size="30" maxlength="64" value="" class="Arial_16">	
				</td>
			</tr>
		</table>
	</div>

<script>
setPskKeyLength();
displayObj();
</script>
	</form>			
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>	
</html>

