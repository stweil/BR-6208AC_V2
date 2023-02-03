<html>
<head>
<title>EDIMAX Technology</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<link rel="stylesheet" href="set.css">
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/FUNCTION_SCRIPT"></script>
<script type="text/javascript">
var ipaddr="<%getInfo("ip-rom");%>";
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
var x = /\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b/

/*if(iqsetup_finish != 1)
{
	if( ( !x.test(location.hostname) && location.hostname != "edimaxext.setup" ) || ( (x.test(location.hostname)) && location.hostname != ipaddr) )
	{
		 window.top.location.assign("http://edimaxext.setup/aIndex.asp");
	}
}*/
</script>
<script type="text/javascript">
function goNext()
{
	window.location.assign("aIndex2.asp");
}
function language_index()
{
	document.iqv2.site.selectedIndex = stype;
}
</script>
</head>
<body onLoad="language_index()">
<div style="position:absolute;margin:-300px 0px 0px -400px;top:50%;left:50%">
<table border="0" align="center" style="height:600px;width:800px" background="/graphics/bg2.jpg">
<tr>
<td align="center">

	<table border="0" align="center" style="height:120px;width:780px">
		<tr><td></td></tr>
	</table>
	
	<table border="0" align="center" style="height:100px;width:730px">
		<tr>
			<td class="text" style="text-align:right">
				<form name="iqv2">
					<select name ="site" onchange ="iQsetup_setlanguage()" class="text" >
						<script>
//English	Deutsch	Español	Français	Italiano	Nederlands	Português	Română	čeština	Polski	Русский	Slovenčina	Türkce	Vietnamese	繁中	简中

									document.write("<option name =\"SelectUK\" value =\"0\"");
									if(stype == "0") document.write("selected");
									document.write(">English</option>");

									document.write("<option name =\"SelectDE\" value =\"1\""); //Germany
									if(stype == "1") document.write("selected");
									document.write(">Deutsch</option>");

									document.write("<option name =\"SelectES\" value =\"2\""); //Spain
									if(stype == "2" ) document.write("selected");
									document.write(">Español</option>");

									document.write("<option name =\"SelectFR\" value =\"3\""); //France
									if(stype == "3" ) document.write("selected");
									document.write(">Français</option>");

									document.write("<option name =\"SelectIT\" value =\"4\""); 
									if(stype == "4" ) document.write("selected");
									document.write(">Italiano</option>");

									document.write("<option name =\"SelectNL\" value =\"5\""); 
									if(stype == "5" ) document.write("selected");
									document.write(">Nederlands</option>");

									document.write("<option name =\"SelectPT\" value =\"6\"");   //Portugal 葡萄牙
									if(stype == "6" ) document.write("selected");
									document.write(">Português</option>");

									document.write("<option name =\"SelectRO\" value =\"7\"");  //Romania
									if(stype == "7" ) document.write("selected");
									document.write(">Română</option>");

									document.write("<option name =\"SelectCZ\" value =\"8\""); //Czech Republic 捷克語
									if(stype == "8" ) document.write("selected");
									document.write(">čeština</option>");

									document.write("<option name =\"SelectPL\" value =\"9\""); //Poland
									if(stype == "9" ) document.write("selected");
									document.write(">Polski</option>");

									
									document.write("<option name =\"SelectRU\" value =\"10\""); //Russian
									if(stype == "10" ) document.write("selected");
									document.write(">Русский</option>");

									document.write("<option name =\"SelectSK\" value =\"11\""); //Slovakia 斯洛伐克語
									if(stype == "11" ) document.write("selected");
									document.write(">Slovenčina</option>");

									/*document.write("<option name =\"SelectUKR\" value =\"12\""); //Українська 烏克蘭
									if(stype == "12" ) document.write("selected");
									document.write(">﻿Українська</option>");*/

									document.write("<option name =\"SelectTurk\" value =\"12\"");
									if(stype == "12" ) document.write("selected");
									document.write(">Türkce</option>");

									document.write("<option name =\"SelectVie\" value =\"13\"");
									if(stype == "13" ) document.write("selected");
									document.write(">Vietnamese</option>");


									document.write("<option name =\"SelectTW\" value =\"14\""); 
									if(stype == "14" ) document.write("selected");
									document.write(">繁中</option>");

									document.write("<option name =\"SelectCN\" value =\"15\""); 
									if(stype == "15" ) document.write("selected");
									document.write(">简中</option>");
						</script>
					</select>
				</form>
			</td>
		</tr>
	</table>	

	<table border="0" align="center" style="height:300px;width:780px">
		<tr><td></td></tr>
	</table>

	<table border="0" align="center" style="height:70px;width:780px">
		<tr>
			<td align="center">
				<script>document.write('<input type="button" value="'+showText(getStarted)+'" name="close" onClick="goNext();" class="button_IQ4" style="cursor:pointer">');</script>
			</td>
		</tr>
	</table>

</td>
</tr>
</table>
</div>
</body>
</html>
