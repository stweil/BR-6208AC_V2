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

<script>
var qosShow = new Array('','','','','','','','','','','','','','','','','');

Select_Edit=window.opener.document.forms['formWan1Qos'].select_edit.value;
<% QosShow(); %>
if (Select_Edit!=0)
{
	for (var i=0;i<17;i++)
	{
		qosShow[i] = Qos_All_Show[ (Select_Edit-1)*17+i ];
	}
}
/*function macRange(macAddr) {
	str = macAddr.value;
	for (var i=0; i<str.length; i++) {
		if ( (str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) >= 'A' && str.charAt(i) <= 'F') || (str.charAt(i) >= 'a' && str.charAt(i) <= 'f') || (str.charAt(i) == ','))
			continue;
		alert(showText(wanqosaddAlertMac));
		macAddr.focus();
		return 0;
	}
	return 1;
}*/
function portRange(port) {
	str = port.value;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == ',') || (str.charAt(i) == '-'))
			continue;
		//alert(showText(wanqosaddAlertPort));
		alert(''+showText(InvalidSomething).replace(/#####/,showText(PortRange))+' '+showText(shouldbeSomething).replace(/####/,showText(decimalnumber)));
		port.focus();
		return 0;
	}
	return 1;
}
function addClick() {
        strBwVal = document.formQos.bwidthVal;
        strDownVal = document.formQos.bwidthVal;
        strScStIp = document.formQos.sourStIp;
        strScEdIp = document.formQos.sourEdIp;
        strDtStIp = document.formQos.destStIp;
        strDtEdIp = document.formQos.destEdIp;
        strScPort = document.formQos.sourPort;
        strDtPort = document.formQos.destPort;
	if (qosShow[3] != 2) {
		if ((strDownVal.value=="" && strBwVal.value=="") || (strScStIp.value=="" && strScEdIp.value=="" && strDtStIp.value=="" && strDtEdIp.value=="" && strScPort.value=="" && strDtPort.value=="" && document.formQos.trafType.value==0) ) {
			alert(showText(entryempty));
			return false;
        }
    }

		if ( strScStIp.value=="" && strScEdIp.value!="") {
            alert(showText(sourcestartIP)+" "+showText(entryempty));
            return false;
        }
		if ( strDtStIp.value=="" && strDtEdIp.value!="") {
            alert(showText(destinationstartIP)+" "+showText(entryempty));
            return false;
        }


	if (!portRule(strBwVal, showText(Bandwidth), 0, "", 1, 99999, 0))
		return false;


    if (document.formQos.sourType.value == 0 ) {
        if ( !ipRule(strScStIp, showText(sourcestartIP), "gw", 0))
            return false;
        if ( !ipRule(strScEdIp, showText(sourceendIP), "gw", 0))
            return false;
    }
    else{
        if (strScMac.value != "" )
					if ( !macRule(str, 1)) return false;  //if ( !macRange(strScMac))
		}

    if ( !ipRule(strDtStIp, showText(destinationstartIP), "gw", 0))
        return false;
    if ( !ipRule(strDtEdIp, showText(destinationendIP), "gw", 0))
        return false;
    strScPort.disabled = false;
    strDtPort.disabled = false;
    if (strScPort.value != "" )
    {
        //if (!portRule(strScPort, showText(PortRange), 0,"", 2, 65535, 1))//if ( !portRange(strScPort))
				if ( !portRange(strScPort))
            return false;
    }
    else
    {
        strScPort.value = "2-65535"
    }
    if (strDtPort.value != "" )
    {
        //if (!portRule(strDtPort, showText(PortRange), 0,"", 2, 65535, 1))//if ( !portRange(strDtPort))
				if ( !portRange(strDtPort))
            return false;
    }
    else
    {
        strDtPort.value = "2-65535"
    }
    var longVal=0x001;
    document.formQos.configWan.value=longVal;
    document.formQos.configNum.value=qosShow[14];
    if (qosShow[14]) {
        document.formQos.addQos.disabled = true;
        document.formQos.editQos.disabled = false;
    }
    else {
        document.formQos.addQos.disabled = false;
        document.formQos.editQos.disabled = true;
    }

		$.post("/goform/formQoS", $("#formQos").serialize(),function(data){ /* submit and get data */
			if(data=="submit_ok"){
				self.opener.location.reload();
				window.close();
			}
		},"text");
}
function specified_protocol()
{
	switch ( document.formQos.trafType.value )
	{
	case "0":
		document.formQos.sourPort.disabled=false;
		document.formQos.destPort.disabled=false;
		break;
	default:
		document.formQos.sourPort.value="";
		document.formQos.destPort.value="";
		document.formQos.sourPort.disabled=true;
		document.formQos.destPort.disabled=true;
		break;
	}
}
</script>
</head>
<body class="" topmargin="10">
<blockquote>

<p align="center"><b><span class="tiltleText"><script>dw(QoS)</script></span></b></p>
<p align="center"><span class="itemText"><script>dw(qosAddInfo)</script></span><br></p>
<br>
<form id="formQos" name="formQos">
	
	<table width="520" border="0" cellpadding="4" cellspacing="2" align="center">
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(RuleName);</script></span>&nbsp;</td>
			<td width="70%" align="left">&nbsp;<input type="text" name="ruleName" size="15" maxlength="15"></td>
		</tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(Bandwidth);</script></span>&nbsp;</td>
			<td width="70%" align="left" nowrap>&ensp;
				<select name="downUpType">
					<script>
						var downUpTbl = new Array(showText(Upload),showText(Download),showText(Both));
						if (qosShow[1] != 0) {
							document.write("<option value=0 selected>"+ downUpTbl[0] +"</option>");
							document.write("<option value=1>"+ downUpTbl[1] +"</option>");
						}
						else {
							document.write("<option value=0>"+ downUpTbl[0] +"</option>");
							document.write("<option value=1 selected>"+ downUpTbl[1] +"</option>");
						}
					</script>
			</select>

			<input type="text" name="bwidthVal" size="5" maxlength="5"><font size="2">&nbsp;<span class="itemText"><script>dw(kbits);</script></span>
	&nbsp;<select name="bwidthType">
					<script>
						var bwTypeTbl = new Array(showText(guarantee),showText(Max));
						if (qosShow[1] != 0) {
							for(i=0; i<2; i++) {
								if (i == qosShow[2])
									document.write('<option value="'+ i +'" selected>'+ bwTypeTbl[i] +'</option>');
								else
									document.write('<option value="'+ i +'">'+ bwTypeTbl[i] +'</option>');
							}
						}
						else {
							for(i=0; i<2; i++) {
								if (i == qosShow[16])
									document.write('<option value="'+ i +'" selected>'+ bwTypeTbl[i] +'</option>');
								else
									document.write('<option value="'+ i +'">'+ bwTypeTbl[i] +'</option>');
							}
						}
				 </script>
			 </select>
				</font>
			</td>
		</tr>
		<input type="hidden" name="sourType" value=0>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(LocalIPAddress);</script>&nbsp;</span></td>
			<td width="70%" align="left" nowrap>&nbsp;<input type="text" name="sourStIp" size="15" maxlength="15"><font size="2"> - </font><input type="text" name="sourEdIp" size="15" maxlength="15"></td>
		</tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(LocalPortRange);</script>&nbsp;</span></td>
			<td width="70%" align="left" nowrap>&nbsp;<input type="text" name="sourPort" size="35" maxlength="55"></td>
		</tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(RemoteIPAddress);</script>&nbsp;</span></td>
			<td width="70%" align="left" nowrap>&nbsp;<input type="text" name="destStIp" size="15" maxlength="15"><font size="2"> - </font><input type="text" name="destEdIp" size="15" maxlength="15"></td></tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(RemotePortRange);</script>&nbsp;</span></td>
			<td width="70%" align="left" nowrap>&nbsp;<input type="text" name="destPort" size="35" maxlength="55"></td>
		</tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(TrafficType);</script>&nbsp;</span></td>
			<td width="70%" align="left">&nbsp;<select name="trafType" onchange="specified_protocol()">
				<script>
				var trafTbl = new Array(showText(None),"SMTP","HTTP","POP3","FTP");
				for(i=0; i<5; i++) {
					if (i == qosShow[9])
						document.write('<option value="'+ i +'" selected>'+ trafTbl[i] +'</option>');
					else
						document.write('<option value="'+ i +'">'+ trafTbl[i] +'</option>');
				}
				</script>
				</select>
			</td>
		</tr>
		<tr class="">
			<td width="30%" align="right" class=""><span class="itemText"><script>dw(Protocol);</script>&nbsp;</span></td>
			<td width="70%" align="left">&nbsp;
				<select name="protocol">
					<script>
						var ptlTbl = new Array("TCP","UDP");
						for(i=0; i<2; i++) {
							if (i == qosShow[10])
								document.write('<option value="'+ i +'" selected>'+ ptlTbl[i] +'</option>');
							else
								document.write('<option value="'+ i +'">'+ ptlTbl[i] +'</option>');
						}
					</script>
				</select>
			</td>
		</tr>
	</table>

	<script>
	with(document.formQos) {
			ruleName.value = qosShow[0];
			if( qosShow[1] != 0 ) { // Upload
			    bwidthVal.value = qosShow[1];
			    sourStIp.value = qosShow[4];
			    sourEdIp.value = qosShow[5];
			    sourPort.value = qosShow[11];
			    destStIp.value = qosShow[7];
			    destEdIp.value = qosShow[8];
			    destPort.value = qosShow[12];
			}
			else {  // Download
			    bwidthVal.value = qosShow[15];
			    sourStIp.value = qosShow[7];
			    sourEdIp.value = qosShow[8];
			    sourPort.value = qosShow[12];
			    destStIp.value = qosShow[4];
			    destEdIp.value = qosShow[5];
			    destPort.value = qosShow[11];
			}
	}
	</script>

	<br>
	<br>

	<table width="520" border="0" cellspacing="0" cellpadding="0" align="center">
		<tr>
			<td colspan=4 align="center">
				<script>document.write('<input type="button" value="'+showText(save)+'" class="ui-button-nolimit" name="apply" onClick="addClick()">');</script>&nbsp;
				<input type="hidden" value="" name="configWan">
				<input type="hidden" value="" name="configNum">
				<input type="hidden" value="Add" name="addQos">
				<input type="hidden" value="Edit" name="editQos">
				<input type="hidden" value="frame_1" name="framename">
			</td>
		</tr>
	</table>

</form>

</blockquote>
</body>
</html>
