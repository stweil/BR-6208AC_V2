<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
<head>
<title>EDIMAX Technology</title>
<script type="text/javascript" src="/file/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="/file/javascript.js"></script>
<script type="text/javascript" src="file/autowan.var"></script>
<script type="text/javascript" src="/file/multilanguage.var"></script>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="cache-control" content="no-store">
<meta http-equiv="Expires" content="-1">
<style type="text/css">
*{font-family:Calibri,Arial,Microsoft JhengHei,sans-serif;font-size:13px;}
.pageSize1 {height:70px; width:1024px;}
.pageSize2 {height:698px; width:190px;}
.pageSize3 {height:698px; width:834px;}
.pageBorder {border-color:#d8d8d8; border-bottom-style:solid; border-left-style:solid; border-right-style:solid; border-width:1px; border-collapse:collapse }
.mode  { font-family:Calibri,Arial,Microsoft JhengHei; FONT-SIZE: 13pt; color:#FFFFFF; }
</style>
<script>
var ipaddr="<%getInfo("ip-rom");%>";
var x = /\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b/

var editapply = 0;
var wispmode = 0;
var wizMode = <%getVar("wizmode","","");%>;
var iqsetup_finish = <%getVar("IQsetupDisable","","");%>;
//var wizMode = 2;
//var iqsetup_finish = 1;


</script>

</head>
<body bgcolor="#FFFFFF" text="#000000">
	<table border="0" align="center" class="pageBorder">
			<tr>
				<td background="/graphics/banner.png" colspan="2" align="center">

					<table border="0" align="center" style="height:70px;width:1024px">
					<tr>
					<td style="width:170px">
					</td>
					<td align="left" style="width:704px" class="mode">
					<script>
							if(wizMode==1)
								dw(apmode);
							else if(wizMode==2)
								dw(repeatermode);
							else
								dw(APcl_1);
							</script>
						</td>
						<td align="center" style="width:150px">
						<form name=form>
							<select name ="site" onchange ="javascript: setlanguage()" style="width:120">
								<script>

﻿//English	Deutsch	Español	Français	Italiano	Nederlands	Português	Română	čeština	Polski	Русский	Slovenčina	Türkce	Vietnamese	繁中	简中
	
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

				</td>
			</tr>

			<tr>
				<td>
					<script>
						if(wizMode==1)
							document.write('<iframe src="left_list_ap.asp" style="height:698px; width:190px;" marginheight="0" marginwidth="0" name="lFrame" frameborder="0"></iframe>');
						else if(wizMode==3)
							document.write('<iframe src="left_list_APcl.asp" style="height:698px; width:190px;" marginheight="0" marginwidth="0" name="lFrame" frameborder="0"></iframe>');
						else
							document.write('<iframe src="left_list_rep.asp" style="height:698px; width:190px;" marginheight="0" marginwidth="0" name="lFrame" frameborder="0"></iframe>');
					</script>
				</td>
				<td>
					<script>
						if(wizMode==2)
							document.write('<iframe src="status_rep.asp" style="height:698px; width:834px;" marginheight="0" marginwidth="0" name="main" id="main" scrolling="" frameborder="0"></iframe>');
						else
							document.write('<iframe src="status_noInternet.asp" style="height:698px; width:834px;" marginheight="0" marginwidth="0" name="main" id="main" scrolling="" frameborder="0"></iframe>');
					</script>
				</td>
			</tr>

	</table>
</body>

</html>
