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


function init()
{
 $("#help-box").hide();
}
</SCRIPT>

</head>
<body class="" onLoad="init();">


			<div class="help-information" id="help-box" style="display:none">
				<table class="help-table1">
					<tr>
						<td><div class="help-text" id="help-info1"><script>dw(transAndreceptHelpInfo)</script>
						</div></td>
					</tr>
				</table>
			</div>

			<script>
			HelpButton();
			</script>



<blockquote>




		<fieldset name="Statistics_fieldset" id="Statistics_fieldset" >
			<legend><script>dw(Statistics)</script></legend>
				<br>

				<form action=/goform/formStats method=POST name="formStats">
						<table width="700" cellpadding="2" cellspacing="1" align="center">
								<script>
									wlDev = <%getVar("wlDev","","");%>;
									if (wlDev!=0)
									{
										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="titletableText" rowspan="2">2.4'+showText(GHz)+'&ensp;'+showText(Wireless)+'</td>');
										document.write('<td width="30%" class="listtableText">' + showText(SentPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("wlanTxPacketNum"); %></td>');
										document.write('</tr>');
										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="listtableText">' + showText(ReceivedPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("wlanRxPacketNum"); %></td>');
										document.write('</tr>');
										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="titletableText" rowspan="2">5'+showText(GHz)+'&ensp;'+showText(Wireless)+'</td>');
										document.write('<td width="30%" class="listtableText">' + showText(SentPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("5GwlanTxPacketNum"); %></td>');
																	document.write('</tr>');

										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="listtableText">' + showText(ReceivedPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("5GwlanRxPacketNum"); %></td>');
										document.write('</tr>');
									}
								</script>
								<tr align="center">
									<td width="30%" class="titletableText" rowspan="2"><script>dw(EthernetLAN)</script></td>
									<td width="30%" class="listtableText"><script>dw(SentPackets)</script></td>
									<td width="20%" class="listtableText">&nbsp;<% getInfo("lanTxPacketNum"); %></td>
								</tr>
								<tr align="center">
									<td width="30%" class="listtableText"><script>dw(ReceivedPackets)</script></td>
									<td width="20%" class="listtableText">&nbsp;<% getInfo("lanRxPacketNum"); %></td>
								</tr>
								<script>
									apMode = <%getVar("apRouterSwitch","","");%>;
									if( apMode !=1)
									{
										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="titletableText" rowspan="2">' + showText(EthernetWAN) + '</td>');
										document.write('<td width="30%" class="listtableText">' + showText(SentPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("wanTxPacketNum"); %></td>');
										document.write('</tr>');
										document.write('<tr align="center" class="">');
										document.write('<td width="30%" class="listtableText">' + showText(ReceivedPackets) + '</td>');
										document.write('<td width="20%" class="listtableText">&nbsp;<% getInfo("wanRxPacketNum"); %></td>');
										document.write('</tr>');
									}
								</script>
					</table>

					<br>

					<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">
						<tr>
							<td style="text-align:center; padding-top:30px;">
								<script>document.write('<input type="submit" value="'+showText(refresh)+'" name="refresh" onclick="" class="ui-button-nolimit">')</script>
							</td>
						</tr>
					</table>

					<input type="hidden" value="/admin_statistics.asp" name="submit-url">
			</form>
							<!--<table width="600" border="1" style="" cellpadding="2" cellspacing="4" align="center">
									<script>

									//if (wlDev!=0) {
										document.write('<tr>');
										document.write('<td width="30%" style="FONT-FAMILY: Arial; font-weight: bold; FONT-SIZE: 10pt; background: #a4a4a4; color: #FFFFFF;" align="center" rowspan="2">' + showText(WirelessLAN) + '</td>');
										document.write('<td width="30%" style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i>' + showText(SentPackets) + '</i></td>');
										document.write('<td width="20%" style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td>');
										document.write('</tr>');
										document.write('<tr>');
										document.write('<td bgcolor="#FFFFFF" style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i>' + showText(ReceivedPackets) + '</i></td>');
										document.write('<td bgcolor="#FFFFFF" style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td>');
										document.write('</tr>');
								//	}
									</script>
									<tr>
										<td style="FONT-FAMILY: Arial; font-weight: bold; FONT-SIZE: 10pt; background: #a4a4a4; color: #FFFFFF;" align="center" rowspan="2"><script>dw(EthernetLAN)</script></td>
										<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i><script>dw(SentPackets)</script></i></td>
										<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td>
									</tr>
									<tr>
										<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i><script>dw(ReceivedPackets)</script></i></td>
										<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td>
									</tr>
										<script>
										
										//if ( apMode !=1) {
											document.write('<tr><td style="FONT-FAMILY: Arial; font-weight: bold; FONT-SIZE: 10pt; background: #a4a4a4; color: #FFFFFF;" align="center" rowspan="2">'+showText(EthernetWAN)+'</td>');
											document.write('<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i>'+showText(SentPackets)+'</i></td>');
											document.write('<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td></tr>');
											document.write('<tr><td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center"><i>'+showText(ReceivedPackets)+'</i></td>');
											document.write('<td style="FONT-FAMILY: Arial;  FONT-SIZE: 10pt;  background: #ffffff; color: #000000;" align="center">123465</td></tr>');
										//}
										</script>
							</table>

							<br>

							<table align="center" border="0" cellspacing="0" cellpadding="0" width="600">
								<tr>
									<td style="text-align:center; padding-top:30px;">
										<script>document.write('<input type="submit" value="'+showText(refresh)+'" name="refresh" onclick="" class="ui-button-nolimit">')</script>
									</td>
								</tr>
							</table>-->
								

	</fieldset>



<!--========================================================================================================================-->
<script>

			$("#help-box").hide();

</script>
<!--========================================================================================================================-->
</blockquote>
</body>
<HEAD>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
</HEAD>
		

</html>
