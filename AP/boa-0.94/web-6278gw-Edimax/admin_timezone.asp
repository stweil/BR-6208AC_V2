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
var Month = new Array(showText(jan),showText(feb),showText(march),showText(april),showText(may),showText(june),showText(July),showText(august),showText(sep),showText(oct),showText(nov),showText(dec));
var monthNum = new Array(31,28,31,30,31,30,31,31,30,31,30,31);
var ntp_zone_array=new Array
(
	"(GMT-12:00)Eniwetok, Kwajalein",
	"(GMT-11:00)Midway Island, Samoa",
	"(GMT-10:00)Hawaii",
	"(GMT-09:00)Alaska",
	"(GMT-08:00)Pacific Time (US & Canada); Tijuana",
	"(GMT-07:00)Arizona",
	"(GMT-07:00)Mountain Time (US & Canada)",
	"(GMT-06:00)Central Time (US & Canada)",
	"(GMT-06:00)Mexico City, Tegucigalpa",
	"(GMT-06:00)Saskatchewan",
	"(GMT-05:00)Bogota, Lima, Quito",
	"(GMT-05:00)Eastern Time (US & Canada)",
	"(GMT-05:00)Indiana (East)",
	"(GMT-04:00)Atlantic Time (Canada)",
	"(GMT-04:00)Caracas, La Paz",
	"(GMT-04:00)Santiago",
	"(GMT-03:30)Newfoundland",
	"(GMT-03:00)Brasilia",
	"(GMT-03:00)Buenos Aires, Georgetown",
	"(GMT-02:00)Mid-Atlantic",
	"(GMT-01:00)Azores, Cape Verde Is.",
	"(GMT)Casablanca, Monrovia",
	"(GMT)Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London",
	"(GMT+01:00)Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna",
	"(GMT+01:00)Belgrade, Bratislava, Budapest, Ljubljana, Prague",
	"(GMT+01:00)Barcelona, Madrid",
	"(GMT+01:00)Brussels, Copenhagen, Madrid, Paris, Vilnius",
	"(GMT+01:00)Paris",
	"(GMT+01:00)Sarajevo, Skopje, Sofija, Warsaw, Zagreb",
	"(GMT+02:00)Athens, Istanbul, Minsk",
	"(GMT+02:00)Bucharest",
	"(GMT+02:00)Cairo",
	"(GMT+02:00)Harare, Pretoria",
	"(GMT+02:00)Helsinki, Riga, Tallinn",
	"(GMT+02:00)Jerusalem",
	"(GMT+03:00)Baghdad, Kuwait, Riyadh",
	"(GMT+03:00)Moscow, St. Petersburg, Volgograd",
	"(GMT+03:00)Nairobi",
	"(GMT+03:30)Tehran",
	"(GMT+04:00)Abu Dhabi, Muscat",
	"(GMT+04:00)Baku, Tbilisi",
	"(GMT+04:30)Kabul",
	"(GMT+05:00)Ekaterinburg",
	"(GMT+05:00)Islamabad, Karachi, Tashkent",
	"(GMT+05:30)Bombay, Calcutta, Madras, New Delhi, Sri Lanka",
	"(GMT+05:45)Kathmandu, Nepal",
	"(GMT+06:00)Astana, Almaty, Dhaka",
	"(GMT+06:00)Colombo",
	"(GMT+07:00)Bangkok, Hanoi, Jakarta",
	"(GMT+08:00)Beijing, Chongqing, Hong Kong, Urumqi",
	"(GMT+08:00)Perth",
	"(GMT+08:00)Singapore",
	"(GMT+08:00)Taipei",
	"(GMT+09:00)Osaka, Sapporo, Tokyo",
	"(GMT+09:00)Seoul",
	"(GMT+09:00)Yakutsk",
	"(GMT+09:30)Adelaide",
	"(GMT+09:30)Darwin",
	"(GMT+10:00)Brisbane",
	"(GMT+10:00)Canberra, Melbourne, Sydney",
	"(GMT+10:00)Guam, Port Moresby",
	"(GMT+10:00)Hobart",
	"(GMT+10:00)Vladivostok",
	"(GMT+11:00)Magadan, Solomon Is., New Caledonia",
	"(GMT+12:00)Auckland, Wllington",
	"(GMT+12:00)Fiji, Kamchatka, Marshall Is."
);
var server_array=new Array
(
	"pool.ntp.org",
	"asia.pool.ntp.org",
	"europe.pool.ntp.org",
	"north-america.pool.ntp.org",
	"oceania.pool.ntp.org",
	"south-america.pool.ntp.org"
);


var wizardEnabled = <%getVar("wizardEnabled","","");%>;
var DayLightEnabled = <%getVar("DayLightEnable","","");%>;

function saveChanges(rebootFlag)
{
	with(document.timezone) {

		if(document.timezone.DayLightEnable.checked==true){
			if(endMonth.selectedIndex == startMonth.selectedIndex){
				alert(showText(InvalidSomething).replace(/#####/, showText(TimeStop)));
				endMonth.focus();
				return false;
			}
		}

		NTPServerName.value=server_array[ServerSel.value];

		if(rebootFlag)
			isApply.value = "ok;"

		submit();
	}
}

function TimeEnable(resetFlag)
{
	if(!resetFlag)
	{
		if(document.timezone.DayLightEnable.checked==false)
		{
			document.timezone.startMonth.disabled = true;
			document.timezone.startDay.disabled = true;
			document.timezone.endMonth.disabled = true;
			document.timezone.endDay.disabled = true;
		}
		else
		{
			document.timezone.startMonth.disabled = false;
			document.timezone.startDay.disabled = false;
			document.timezone.endMonth.disabled = false;
			document.timezone.endDay.disabled = false;
		}
	}
	else
	{
		if(!DayLightEnabled)
		{
			document.timezone.startMonth.disabled = true;
			document.timezone.startDay.disabled = true;
			document.timezone.endMonth.disabled = true;
			document.timezone.endDay.disabled = true;
		}
		else
		{
			document.timezone.startMonth.disabled = false;
			document.timezone.startDay.disabled = false;
			document.timezone.endMonth.disabled = false;
			document.timezone.endDay.disabled = false;
		}
	}
}
/*
function chDayNum(Month,Day)
{
	if (monthNum[Month.selectedIndex] == 28)
	{ 
		Day.options[29] = null;
	}
	else if(monthNum[Month.selectedIndex] == 30)
	{
		if(Day.selectedIndex == 29)
			Day.options[29] = new Option(30,30, true);
		else
			Day.options[29] = new Option(30,30);
		Day.options[30] = null;
	}
	else if (monthNum[Month.selectedIndex] == 31)
	{
		if(Day.selectedIndex == 29)
			Day.options[29] = new Option(30,30, true);
		else
			Day.options[29] = new Option(30,30);

		if(Day.selectedIndex == 30)
			Day.options[30] = new Option(31,31, true);
		else
			Day.options[30] = new Option(31,31);
	}
}
*/
function chDayNum(Month,Day) {
	if (monthNum[Month.selectedIndex] == 28) { 
		if(Day.length == 31)
			Day.length-=2;
		if(Day.length == 30)
			Day.length-=1;
	}
	else if(monthNum[Month.selectedIndex] == 30) {
		if(Day.length == 31)
			Day.length-=1;
		if(Day.length == 29){
			Day.options[29] = new Option(30,30);
		}
	}
	else if (monthNum[Month.selectedIndex] == 31) {
		if(Day.length == 30){
			Day.options[30] = new Option(31,31);
		}
		if(Day.length == 29){
			Day.options[29] = new Option(30,30);
			Day.options[30] = new Option(31,31);
		}	
	}
	Day.selectedIndex = parseInt(Day.value,10) - 1;
}
/*function ServerEnable(resetFlag)
{
	if(!resetFlag)
		if((document.getElementsByName("ServerAuto")[0].checked))
		{
			document.timezone.ServerSel.disabled = false;
			document.timezone.TimeServerAddr.disabled = true;
		}
		else
		{
			document.timezone.ServerSel.disabled = true;
			document.timezone.TimeServerAddr.disabled = false;
		}
	else
	{
		if(serverAuto)
		{
			document.timezone.ServerSel.disabled = false;
			document.timezone.TimeServerAddr.disabled = true;
		}
		else
		{
			document.timezone.ServerSel.disabled = true;
			document.timezone.TimeServerAddr.disabled = false;
		}
	}
}			*/

function resetform()
{
	chDayNum(document.timezone.startMonth,document.timezone.startDay);
	chDayNum(document.timezone.endMonth,document.timezone.endDay);
	TimeEnable(1);
	//ServerEnable(1);
}
function init()
{
	$("#help-box").hide();
}
</SCRIPT>

</head>
<body onLoad="init()">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr><td><div class="help-text" id="help-info1"><script>dw(TimeZoneHelpInfo)</script></div></td></tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>

		<fieldset name="TimeZone_fieldset" id="TimeZone_fieldset">
			<legend><script>dw(TimeZone)</script></legend>
					<br>
			
					<form action="/goform/formTimeZoneSetup" method="POST" name="timezone">
						<input type="hidden" value="<% getInfo("TimeZoneSel"); %>" name="TimeZoneSelget">
						<input type="hidden" value="<% getInfo("startMonth"); %>" name="startMonthget">
						<input type="hidden" value="<% getInfo("startDay"); %>" name="startDayget">
						<input type="hidden" value="<% getInfo("endMonth"); %>" name="endMonthget">
						<input type="hidden" value="<% getInfo("endDay"); %>" name="endDayget">
						<input type="hidden" value="<% getInfo("ServerSel"); %>" name="ServerSelget">
						<input type="hidden" value="" name="NTPServerName">

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
								<tr>
									<td><div class="wizard-itemText-len"><script>dw(setTimeZone)</script></div></td>
									<td><div class="wizard-itemText2-len">
										<select name="TimeZoneSel">
											<script language="javascript">
												for(i=0;i<ntp_zone_array.length;i++)
												{
													if(i==document.timezone.TimeZoneSelget.value)
														document.write("<option value=\""+i+"\" selected>"+ntp_zone_array[i]+"</option>");
													else
														document.write("<option value=\""+i+"\">"+ntp_zone_array[i]+"</option>");
												}
											</script>
										</select>
									</div></td>
								</tr>
								<tr>
									<td><div class="wizard-itemText-len"><script>dw(timeServerAddr)</script></div></td>
									<td><div class="wizard-itemText2-len">
										
										<select name="ServerSel">
											<script language="javascript">
												for(i=0;i<server_array.length;i++)
												{
													if(i==document.timezone.ServerSelget.value)
														document.write("<option value=\""+i+"\" selected>"+server_array[i]+"</option>");
													else
														document.write("<option value=\""+i+"\">"+server_array[i]+"</option>");
												}
											</script>
										</select>
									</div></td>
								</tr>
					
								<tr>
									<td><div class="wizard-itemText-len"><script>dw(DaylightSavings)</script></div></td>
									<td><div class="wizard-itemText2-len">
										<input type="checkbox" name="DayLightEnable" value="ON" <%getVar("DayLightEnable","1","checked");%> onClick="TimeEnable(0)"><span class="choose-itemText"><script>dw(EnableFunction)</script></span></br>
										<select name="startMonth" onChange="chDayNum(this,document.timezone.startDay);">
											<script language="javascript">
												for(i=1;i<13;i++)
												{
													if(i==document.timezone.startMonthget.value)
														document.write("<option value=\""+i+"\" selected>"+Month[(i-1)]+"</option>");
													else
														document.write("<option value=\""+i+"\">"+Month[(i-1)]+"</option>");
												}
											</script>
										</select>
										<select name="startDay">
											<script>
												for(i=1;i<32;i++)
												{
													if(i==document.timezone.startDayget.value)
														document.write("<option value=\""+i+"\" selected>"+i+"</option>");
													else
														document.write("<option value=\""+i+"\">"+i+"</option>");
												}
											</script>
										</select>
										<span class="choose-itemText"><script>dw(To)</script></span>
										<select name="endMonth" onChange="chDayNum(this,document.timezone.endDay);">
											<script language="javascript">
												for(i=1;i<13;i++)
												{
													if(i==document.timezone.endMonthget.value)
														document.write("<option value=\""+i+"\" selected>"+Month[(i-1)]+"</option>");
													else
														document.write("<option value=\""+i+"\">"+Month[(i-1)]+"</option>");
												}
											</script>
										</select>
										<select name="endDay">
											<script>
												for(i=1;i<32;i++)
												{
													if(i==document.timezone.endDayget.value)
														document.write("<option value=\""+i+"\" selected>"+i+"</option>");
												else
														document.write("<option value=\""+i+"\">"+i+"</option>");
												}
											</script>
										</select>
									</div></td>
								</tr>
							</table>
							<script>
								chDayNum(document.timezone.startMonth,document.timezone.startDay);
								chDayNum(document.timezone.endMonth,document.timezone.endDay);
								TimeEnable(0);
								//ServerEnable(0);
							</script>

							<br>

							<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
								<tr>
									<td style="text-align:center; padding-top:30px;">
										<script>
											document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
											var show_reboot = "<% getInfo("show_reboot"); %>"
											if(show_reboot == "1")
												document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)"> '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
										</script>
									</td>
								</tr>
							</table>
							<input type="hidden" value="1" name="iqsetupclose">
							<input type="hidden" value="" name="isApply">
							<input type="hidden" value="/admin_timezone.asp" name="submit-url">
					</form>
					
							
  </fieldset>



<script>
$("#help-box").hide();
</script>

</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		

</html>
