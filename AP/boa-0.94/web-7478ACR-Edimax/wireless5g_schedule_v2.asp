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
var Max_wireSchCount = 30;
var wlanSchEnabled_5G = "<%getiNICInfo("wlanSchEnabled_5G");%>";

var addWeekArray=["addSunday","addMonday","addTuesday","addWednesday","addThursday","addFriday","addSaturday"];
var selWeekArray=["selSunday","selMonday","selTuesday","selWednesday","selThursday","selFriday","selSaturday"];
var totalWeekArray=["totalSelSunday","totalSelMonday","totalSelTuesday","totalSelWednesday","totalSelThursday","totalSelFriday","totalSelSaturday"];

var charDisconnectTable=[63,31,15,7,3,1];//["z","U","E","7","3","1"];
var DisconnectString="zzzzzzzzzzzzzzzzzzzzzzzz";
var charConnectionTable=[0,32,48,56,60,62];//["0","V","k","s","w","y"];
var ConnectionString="000000000000000000000000";
var charTable_x64=[
"0","1","2","3","4","5","6","7","8","9","@","A","B","C","D","E",
"F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U",
"V","W","X","Y","Z","`","a","b","c","d","e","f","g","h","i","j",
"k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"
];

/*
======================== Char To x64 Table ======================== 
char,ascll,int	char,ascll,int	char,ascll,int	char,ascll,int
	 0,		48,	 0		 F,		70,	16		 V,		86,	32		 k,	 107, 48
	 1,		49,	 1		 G,		71,	17		 W,		87,	33		 l,	 108, 49
	 2,		50,	 2		 H,		72,	18		 X,		88,	34		 m,	 109, 50
	 3,		51,	 3		 I,		73,	19		 Y,		89,	35		 n,	 110, 51
	 4,		52,	 4		 J,		74,	20		 Z,		90,	36		 o,	 111, 52
	 5,		53,	 5		 K,		75,	21		 `,		96,	37		 p,	 112, 53
	 6,		54,	 6		 L,		76,	22		 a,		97,	38		 q,	 113, 54
	 7,		55,	 7		 M,		77,	23		 b,		98,	39		 r,	 114, 55
	 8,		56,	 8		 N,		78,	24		 c,		99,	40		 s,	 115, 56
	 9,		57,	 9		 O,		79,	25		 d,	 100,	41		 t,	 116, 57
	 @,		64,	10		 P,		80,	26		 e,	 101,	42		 u,	 117, 58
	 A,		65,	11		 Q,		81,	27		 f,	 102,	43		 v,	 118, 59
	 B,		66,	12		 R,		82,	28		 g,	 103,	44		 w,	 119, 60
	 C,		67,	13		 S,		83,	29		 h,	 104,	45		 x,	 120, 61
	 D,		68,	14		 T,		84,	30		 i,	 105,	46		 y,	 121, 62
	 E,		69,	15		 U,		85,	31		 j,	 106,	47		 z,	 122, 63

===================================================================
*/
function charConversionComputing(charA,charB)
{
	if( charA == charB )
	{
		return charA ;
	}
	else if( charA == charTable_x64[63] || charB == charTable_x64[63] )
	{
		return charTable_x64[63] ;
	}
	else if( charA == charTable_x64[0] || charB == charTable_x64[0] )
	{
		if(charA != charTable_x64[0])
			return charA ;
		else
			return charB ;
	}
	else
	{
		var intA=0,intB=0,intTmp=0;
		for(intTmp=0;intTmp<64;intTmp++)
		{
			if( charTable_x64[intTmp] == charA )
				intA=intTmp;
			if( charTable_x64[intTmp] == charB )
				intB=intTmp;
		}
		return charTable_x64[intA|intB];
	}
		
}

function TimeConversionString_x64 (SchdayS,SchHourS,SchMinS,SchdayE,SchHourE,SchMinE,type,selSch)
{	//type 1:Add schedule use; 2:Del select schedule use;
	SchdayS  = Number(SchdayS);
	SchHourS = Number(SchHourS);
	SchMinS  = Number(SchMinS);
	SchdayE  = Number(SchdayE);
	SchHourE = Number(SchHourE);
	SchMinE  = Number(SchMinE);
// ====================================== dayS < dayE ====================================== 
	if(SchdayS < SchdayE)
	{
		//alert("case 1");
		for(i=0;i<7;i++)
		{	
			if( i<SchdayS || i>SchdayE)
			{
				if(type==1)
					document.getElementById(addWeekArray[i]).value=ConnectionString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=ConnectionString;
				else alert("TimeConversionString_x64 type error!!");
			}
			else if( i>SchdayS && i<SchdayE)
			{
				if(type==1)
					document.getElementById(addWeekArray[i]).value=DisconnectString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=DisconnectString;
				else alert("TimeConversionString_x64 type error!!");
			}
			else
			{
				tmpString="";
				if( i==SchdayS )
				{
					for(j=0;j<24;j++)
					{
						if(j<SchHourS)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j>SchHourS)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else if(j==SchHourS)
							tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
					}
				}
				else if( i==SchdayE )
				{
					for(j=0;j<24;j++)
					{
						if(j<SchHourE)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else if(j>SchHourE)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j==SchHourE)
							tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
					}
				}
				if(type==1)
					document.getElementById(addWeekArray[i]).value=tmpString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=tmpString;
				else alert("TimeConversionString_x64 type error!!");
			}
		}
	}
// ====================================== dayS > dayE ====================================== 
	else if(SchdayS > SchdayE)
	{
		//alert("case 2");
		for(i=0;i<7;i++)
		{	
			if( i<SchdayS && i>SchdayE)
			{
				if(type==1)
					document.getElementById(addWeekArray[i]).value=ConnectionString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=ConnectionString;
				else alert("TimeConversionString_x64 type error!!");
			}
			else if( i>SchdayS || i<SchdayE)
			{
				if(type==1)
					document.getElementById(addWeekArray[i]).value=DisconnectString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=DisconnectString;
				else alert("TimeConversionString_x64 type error!!");
			}
			else
			{
				tmpString="";
				if( i==SchdayS )
				{
					for(j=0;j<24;j++)
					{
						if(j<SchHourS)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j>SchHourS)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else if(j==SchHourS)
							tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
					}
				}
				else if( i==SchdayE )
				{
					for(j=0;j<24;j++)
					{
						if(j<SchHourE)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else if(j>SchHourE)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j==SchHourE)
							tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
					}
				}
				if(type==1)
					document.getElementById(addWeekArray[i]).value=tmpString;
				else if(type==2)
					document.getElementById(selWeekArray[i] + selSch).value=tmpString;
				else alert("TimeConversionString_x64 type error!!");
			}
		}
	}
// ====================================== dayS = dayE ====================================== 
	else
	{
		if(SchHourS < SchHourE) // HourS < HourE
		{
			//alert("case 3");
			for(i=0;i<7;i++)
			{	
				if( i!=SchdayS )
				{
					if(type==1)
						document.getElementById(addWeekArray[i]).value=ConnectionString;
					else if(type==2)
						document.getElementById(selWeekArray[i] + selSch).value=ConnectionString;
					else alert("TimeConversionString_x64 type error!!");
				}
				else
				{
					tmpString="";
					for(j=0;j<24;j++)
					{
						if(j<SchHourS || j>SchHourE)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j>SchHourS && j<SchHourE)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else
						{
							if( j==SchHourS )
								tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
							else 
								tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
						}
					}
					if(type==1)
						document.getElementById(addWeekArray[i]).value=tmpString;
					else if(type==2)
						document.getElementById(selWeekArray[i] + selSch).value=tmpString;
					else alert("TimeConversionString_x64 type error!!");
				}
			}
		}
		else if(SchHourS > SchHourE) // HourS > HourE
		{
			//alert("case 4");
			for(i=0;i<7;i++)
			{	
				if( i!=SchdayS )
				{
					if(type==1)
						document.getElementById(addWeekArray[i]).value=DisconnectString;
					else if(type==2)
						document.getElementById(selWeekArray[i] + selSch).value=DisconnectString;
					else alert("TimeConversionString_x64 type error!!");
				}
				else
				{
					tmpString="";
					for(j=0;j<24;j++)
					{
						if(j<SchHourS && j>SchHourE)
							tmpString=tmpString+ charTable_x64[ 0 ];
						else if(j>SchHourS || j<SchHourE)
							tmpString=tmpString+ charTable_x64[ 63 ];
						else
						{
							if( j==SchHourS )
								tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
							else 
								tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
						}
					}
					if(type==1)
						document.getElementById(addWeekArray[i]).value=tmpString;
					else if(type==2)
						document.getElementById(selWeekArray[i] + selSch).value=tmpString;
					else alert("TimeConversionString_x64 type error!!");
				}
			}
		}
		else // HourS = HourE
		{
			if(SchMinS < SchMinE)     // MinS < MinE
			{
				//alert("case 5");
				for(i=0;i<7;i++)
				{	
					if( i!=SchdayS )
					{
						if(type==1)
							document.getElementById(addWeekArray[i]).value=ConnectionString;
						else if(type==2)
							document.getElementById(selWeekArray[i] + selSch).value=ConnectionString;
						else alert("TimeConversionString_x64 type error!!");
					}
					else
					{
						tmpString="";
						for(j=0;j<24;j++)
						{
							if( j!=SchHourS )
								tmpString=tmpString+ charTable_x64[ 0 ];
							else
							{
								var BinaryTable=[32,16,8,4,2,1];
								var tmpVal=0;
								for(k=0;k<6;k++)
								{
									if( k >= SchMinS && k < SchMinE)
										tmpVal=tmpVal+BinaryTable[ k ]
								}
								tmpString=tmpString+ charTable_x64[ tmpVal ];
							}
						}
						if(type==1)
							document.getElementById(addWeekArray[i]).value=tmpString;
						else if(type==2)
							document.getElementById(selWeekArray[i] + selSch).value=tmpString;
						else alert("TimeConversionString_x64 type error!!");
					}
				}						
			}
			else if(SchMinS > SchMinE)// MinS > MinE
			{
				//alert("case 6");
				for(i=0;i<7;i++)
				{	
					if( i!=SchdayS )
					{
						if(type==1)
							document.getElementById(addWeekArray[i]).value=DisconnectString;
						else if(type==2)
							document.getElementById(selWeekArray[i] + selSch).value=DisconnectString;
						else alert("TimeConversionString_x64 type error!!");
					}
					else
					{
						tmpString="";
						for(j=0;j<24;j++)
						{
							if( j!=SchHourS )
								tmpString=tmpString+ charTable_x64[ 63 ];
							else
							{
								var BinaryTable=[32,16,8,4,2,1];
								var tmpVal=63;
								for(k=0;k<6;k++)
								{
									if( k < SchMinS && k >= SchMinE)
										tmpVal=tmpVal-BinaryTable[ k ]
								}
								tmpString=tmpString+ charTable_x64[ tmpVal ];
							}
						}
						if(type==1)
							document.getElementById(addWeekArray[i]).value=tmpString;
						else if(type==2)
							document.getElementById(selWeekArray[i] + selSch).value=tmpString;
						else alert("TimeConversionString_x64 type error!!");
					}
				}						
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

function TimeConversionString_for_EveryDay_x64 (SchHourS,SchMinS,SchHourE,SchMinE)
{
	SchHourS = Number(SchHourS);
	SchMinS  = Number(SchMinS);
	SchHourE = Number(SchHourE);
	SchMinE  = Number(SchMinE);
	tmpString="";

	if(SchHourS < SchHourE) // HourS < HourE
	{
		for(j=0;j<24;j++)
		{
			if(j<SchHourS || j>SchHourE)
				tmpString=tmpString+ charTable_x64[ 0 ];
			else if(j>SchHourS && j<SchHourE)
				tmpString=tmpString+ charTable_x64[ 63 ];
			else
			{
				if( j==SchHourS )
					tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
				else 
					tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
			}
		}
	}
	else if(SchHourS > SchHourE) // HourS > HourE
	{
		for(j=0;j<24;j++)
		{
			if(j<SchHourS && j>SchHourE)
				tmpString=tmpString+ charTable_x64[ 0 ];
			else if(j>SchHourS || j<SchHourE)
				tmpString=tmpString+ charTable_x64[ 63 ];
			else
			{
				if( j==SchHourS )
					tmpString=tmpString+ charTable_x64[ charDisconnectTable[SchMinS] ];
				else 
					tmpString=tmpString+ charTable_x64[ charConnectionTable[SchMinE] ];
			}
		}
	}
	else // HourS = HourE
	{
		if(SchMinS < SchMinE)     // MinS < MinE
		{
			for(j=0;j<24;j++)
			{
				if( j!=SchHourS )
					tmpString=tmpString+ charTable_x64[ 0 ];
				else
				{
					var BinaryTable=[32,16,8,4,2,1];
					var tmpVal=0;
					for(k=0;k<6;k++)
					{
						if( k >= SchMinS && k < SchMinE)
							tmpVal=tmpVal+BinaryTable[ k ]
					}
					tmpString=tmpString+ charTable_x64[ tmpVal ];
				}
			}			
		}
		else if(SchMinS > SchMinE)// MinS > MinE
		{
			for(j=0;j<24;j++)
			{
				if( j!=SchHourS )
					tmpString=tmpString+ charTable_x64[ 63 ];
				else
				{
					var BinaryTable=[32,16,8,4,2,1];
					var tmpVal=63;
					for(k=0;k<6;k++)
					{
						if( k < SchMinS && k >= SchMinE)
							tmpVal=tmpVal-BinaryTable[ k ]
					}
					tmpString=tmpString+ charTable_x64[ tmpVal ];
				}
			}
		}
		else
		{
			return false;
		}
	}
	for(i=0;i<7;i++)
	{	
			document.getElementById(addWeekArray[i]).value=tmpString;	
	}
	return true;
}

function WlSchSwitch()
{
	with(document.formWIRESchSave)
	{
		if (document.formWIRESchAdd.wlanSchEnable.checked==true)
			wlanSchEnabled5G.value="1";
		else
			wlanSchEnabled5G.value="0";
	}
}

function EveryDaySwitch()
{
	with(document.formWIRESchAdd)
	{
		if (selEveryDay.checked==true)
		{
			addSchdayS.disabled = true;
			addSchdayE.disabled = true;
		}
		else
		{
			addSchdayS.disabled = false;
			addSchdayE.disabled = false;
		}
	}
}

function addsaveChanges(type)
{
	with(document.formWIRESchAdd)
	{
		if ( selEveryDay.checked == true )
		{
			if( (addSchHourS.value == "") || (addSchMinS.value == "") || (addSchHourE.value == "") || (addSchMinE.value == "") )
			{
				alert(showText(PleasecheckSch));
				return false;
			}
			else if( (addSchHourS.value == addSchHourE.value) && (addSchMinS.value == addSchMinE.value) )
			{
				alert(showText(PleasecheckSch));
				return false;
			}

			SchHourS = addSchHourS.value;
			SchMinS  = addSchMinS.value
			SchHourE = addSchHourE.value;
			SchMinE  = addSchMinE.value
			TimeConversionString_for_EveryDay_x64 (SchHourS,SchMinS,SchHourE,SchMinE);
		}
		else
		{
			if( (addSchdayS.value == "") || (addSchHourS.value == "") || (addSchMinS.value == "") || (addSchdayE.value == "") || (addSchHourE.value == "") || (addSchMinE.value == "") )
			{
				alert(showText(PleasecheckSch));
				return false;
			}
			else if( (addSchdayS.value == addSchdayE.value) && (addSchHourS.value == addSchHourE.value) && (addSchMinS.value == addSchMinE.value) )
			{
				alert(showText(PleasecheckSch));
				return false;
			}

			SchdayS  = addSchdayS.value;
			SchHourS = addSchHourS.value;
			SchMinS  = addSchMinS.value
			SchdayE  = addSchdayE.value;
			SchHourE = addSchHourE.value;
			SchMinE  = addSchMinE.value
			TimeConversionString_x64 (SchdayS,SchHourS,SchMinS,SchdayE,SchHourE,SchMinE,1,"");
		}
		/*
		alert( document.getElementById('addSunday').value);
		alert( document.getElementById('addMonday').value);
		alert( document.getElementById('addTuesday').value);	
		alert( document.getElementById('addWednesday').value);
		alert( document.getElementById('addThursday').value);
		alert( document.getElementById('addFriday').value);
		alert( document.getElementById('addSaturday').value);
		*/
	}

	if (document.formWIRESchAdd.wlanSchEnable.checked==true)
		document.formWIRESchAdd.wlanSchEnabled5G.value="1";
	else
		document.formWIRESchAdd.wlanSchEnabled5G.value="0";

	return true;
}

function deleteClick(type)
{

	if(type==1) //deleteSelSch
	{
		selInterface=document.submit_WIRESchDel.selInterface.value;
		selInterfaceSchCount=document.getElementById('wireSchCount'+ selInterface).value;

		for(selDeviceSch=0;selDeviceSch<selInterfaceSchCount;selDeviceSch++)
		{
			if(document.getElementById('select' + selInterface + '_' + selDeviceSch).checked == true )
			{
					SchdayS  = document.getElementById('weekS' + selInterface + '_' + selDeviceSch).value;
					SchHourS = document.getElementById('timeHS' + selInterface + '_' + selDeviceSch).value;
					SchMinS  = document.getElementById('timeMS' + selInterface + '_' + selDeviceSch).value;
					SchdayE  = document.getElementById('weekE' + selInterface + '_' + selDeviceSch).value;
					SchHourE = document.getElementById('timeHE' + selInterface + '_' + selDeviceSch).value;
					SchMinE  = document.getElementById('timeME' + selInterface + '_' + selDeviceSch).value;

					TimeConversionString_x64 (SchdayS,SchHourS,SchMinS,SchdayE,SchHourE,SchMinE,2,selInterface + '_' + selDeviceSch);
					
					for(totalSclDay=0;totalSclDay<7;totalSclDay++)
					{
						StringA=document.getElementById(selWeekArray[totalSclDay] + selInterface + '_' + selDeviceSch).value;
						StringB=document.getElementById(totalWeekArray[totalSclDay]).value;
						tmpString="";
						for(charHour=0;charHour<24;charHour++)
						{
							tmpString = tmpString + charConversionComputing(StringA[charHour],StringB[charHour]);
						}
						document.getElementById(totalWeekArray[totalSclDay]).value=tmpString;
					}
			}
		}
		/*
		alert( document.getElementById('totalSelSunday').value);
		alert( document.getElementById('totalSelMonday').value);
		alert( document.getElementById('totalSelTuesday').value);	
		alert( document.getElementById('totalSelWednesday').value);
		alert( document.getElementById('totalSelThursday').value);
		alert( document.getElementById('totalSelFriday').value);
		alert( document.getElementById('totalSelSaturday').value);
		*/
	}

	if (document.formWIRESchAdd.wlanSchEnable.checked==true)
		document.submit_WIRESchDel.wlanSchEnabled5G.value="1";
	else
		document.submit_WIRESchDel.wlanSchEnabled5G.value="0";

	with(document.submit_WIRESchDel)
	{
		if(type==1) //deleteSelSch
		{
			deleteSelSch.value="yes";
		}
		else if(type==2) //deleteAllSch
		{
			deleteAllSch.value="yes";
		}
		submit();
	}
}

function saveChanges(rebootFlag)
{

	with(document.formWIRESchSave) {

		if (document.formWIRESchAdd.wlanSchEnable.checked==true){
			if(document.getElementById('wireSchCount2').value == 0){
				alert(showText(SomethingCannotBeEmpty).replace(/#####/,showText(Schedule)));
				return false;
			}
		}

		if(rebootFlag)
		 isApply.value = "ok";
	
		submit();	
	}
}

function displayObj(input)
{
	with(document.formWIRESchAdd) 
	{
		if( input > (Max_wireSchCount-7) )
		{
			document.getElementById("wireSchRuleString").style.display = "block";
			selEveryDay.disabled = true;
		}
		if( input >= Max_wireSchCount )
		{
			addSchdayS.disabled = true;
			addSchHourS.disabled = true;
			addSchMinS.disabled = true;
			addSchdayE.disabled = true;
			addSchHourE.disabled = true;
			addSchMinE.disabled = true;
			addSchdule.disabled = true;
			addSchdule.style.color=disableButtonColor;
		}
	}
}

function init()
{
	$("#help-box").hide();

	var wlsw = <%getiNICVar("wlanDisabled","","");%>;
	if(wlsw)
	{
		with(document.formWIRESchAdd)
		{
				wlanSchEnable.disabled = true;
				selEveryDay.disabled = true;
				addSchdayS.disabled = true;
				addSchHourS.disabled = true;
				addSchMinS.disabled = true;
				addSchdayE.disabled = true;
				addSchHourE.disabled = true;
				addSchMinE.disabled = true;
				addSchdule.disabled = true;
				addSchdule.style.color=disableButtonColor;
		}
		with(document.formWIRESchDel)
		{
				deleteSelSch.disabled = true;
				deleteAllSch.disabled = true;
				deleteSelSch.style.color=disableButtonColor;
				deleteAllSch.style.color=disableButtonColor;
		}
		with(document.formWIRESchSave)
		{
				save.disabled = true;
				save.style.color=disableButtonColor;
		}
	}
}
</SCRIPT>

</head>


<body onLoad="init();">
<!--===========================================================================================-->
	<div class="help-information" id="help-box" style="display:none">
		<table class="help-table1">
			<tr><td>
			<div class="help-text" id="help-info1"><script>dw(wlScheduleHelpInfo)</script></div>
			</td></tr>
		</table>
	</div>
	<script>
	HelpButton();
	</script>
<!--===========================================================================================-->
<blockquote>
	<fieldset name="dynamicip_fieldset" id="dynamicip_fieldset">
		<legend><script>dw(Wireless)</script>&nbsp;<script>dw(Schedule)</script></legend>

			<!--<form action="/goform/formWIRESch" method="POST" name="formWlSchEnabled">
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="wlanSchEnabled5G">
				<input type="hidden" value="/wireless5g_schedule_v2.asp" name="submit-url">
			</form>-->

			<form action="/goform/formWIRESch" name="formWIRESchAdd" method="post">
			<table width="700" border="0" cellspacing=1 cellpadding=0 align="center">
			<tr>
				<td>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
						<tr>
							<td colspan="2">
								<script>
								if (wlanSchEnabled_5G=="1")
									document.write('<input type="checkbox" name="wlanSchEnable" value="" onclick="WlSchSwitch()" checked>&nbsp;');
								else
									document.write('<input type="checkbox" name="wlanSchEnable" value="" onclick="WlSchSwitch()" >&nbsp;');
								</script>
								<span class="itemText">
									<script>dw(enable)</script>&nbsp;<script>dw(wirelessoff)</script>&nbsp;<script>dw(Schedule)</script>
								</span>
							</td>
						</tr>
					</table>

					<br>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
						<tr>
							<td align="right" width="30%">
								<input type="checkbox" name="selEveryDay" onclick="EveryDaySwitch()"><span class="itemText">&nbsp;Every&nbsp;Day</span>
							</td>
							<td align="left">
								&nbsp;
							</td>
						</tr>
					</table>

					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="600">
						<tr>
							<td align="right" width="30%"><span class="itemText"><script>dw(TimeStart);</script></span></td>
							<td align="left">
								<select id="addSchdaySID" name="addSchdayS" class="">
									<script>
									document.write('<option name="weekdayS" value="">---'+showText(weekday)+'---</option>')
									document.write('<option name="weekdayS0" value="0">'+showText(Sunday)+'</option>');
									document.write('<option name="weekdayS1" value="1">'+showText(Monday)+'</option>');
									document.write('<option name="weekdayS2" value="2">'+showText(Tuesday)+'</option>');
									document.write('<option name="weekdayS3" value="3">'+showText(Wednesday)+'</option>');
									document.write('<option name="weekdayS4" value="4">'+showText(Thursday)+'</option>');
									document.write('<option name="weekdayS5" value="5">'+showText(Friday)+'</option>');
									document.write('<option name="weekdayS6" value="6">'+showText(Saturday)+'</option>');
									</script>
								</select>
								<select id="addSchHourSID" name="addSchHourS" class="">
									<script>
									document.write('<option value="">---'+showText(hour)+'---</option>');
									for(i=0;i<24;i++)
									document.write("<option value=\""+i+"\">"+i+"</option>");
									</script>
								</select>
								<select id="addSchMinSID" name="addSchMinS" class="">
									<script>
										document.write('<option value="">---'+showText(minute)+'---</option>');
										for(i=0;i<60;i=i+10)
										document.write("<option value=\""+i/10+"\">"+i+"</option>")
									</script>
								</select>
							</td>
						</tr>
						<tr>
							<td align="right"><span class="itemText"><script>dw(TimeStop);</script></span></td>
							<td align="left">
							<select id="addSchdayEID" name="addSchdayE" class="">
							<script>
							document.write('<option name="weekdayE" value="">---'+showText(weekday)+'---</option>')
							document.write('<option name="weekdayE0" value="0">'+showText(Sunday)+'</option>');
							document.write('<option name="weekdayE1" value="1">'+showText(Monday)+'</option>');
							document.write('<option name="weekdayE2" value="2">'+showText(Tuesday)+'</option>');
							document.write('<option name="weekdayE3" value="3">'+showText(Wednesday)+'</option>');
							document.write('<option name="weekdayE4" value="4">'+showText(Thursday)+'</option>');
							document.write('<option name="weekdayE5" value="5">'+showText(Friday)+'</option>');
							document.write('<option name="weekdayE6" value="6">'+showText(Saturday)+'</option>');
							</script>
							</select>
							<select id="addSchHourEID" name="addSchHourE" class="">
								<script>
								document.write('<option value="">---'+showText(hour)+'---</option>');
								for(i=0;i<24;i++)
								document.write("<option value=\""+i+"\">"+i+"</option>");
								</script>
							</select>
							<select id="addSchMinEID" name="addSchMinE" class="">
								<script>
									document.write('<option value="">---'+showText(minute)+'---</option>');
									for(i=0;i<60;i=i+10)
									document.write("<option value=\""+i/10+"\">"+i+"</option>")
								</script>
							</select>
							</td>
						</tr>
						<tr>
							<td colspan="2" align="right">
								<script>
									document.write('<input type="submit" value="'+showText(add)+'" name="addSchdule" onclick="return addsaveChanges()" class="ui-button-nolimit">');
								</script>
							</td>
						</tr>
					</table>

					<input type="hidden" value="2" id="selInterface" name="selInterface"> <!-- 1(2.4G),2(5G) -->
					<input type="hidden" value="" id="addSunday" name="addSunday">
					<input type="hidden" value="" id="addMonday" name="addMonday">
					<input type="hidden" value="" id="addTuesday" name="addTuesday">
					<input type="hidden" value="" id="addWednesday" name="addWednesday">
					<input type="hidden" value="" id="addThursday" name="addThursday">
					<input type="hidden" value="" id="addFriday" name="addFriday">
					<input type="hidden" value="" id="addSaturday" name="addSaturday">
					<input type="hidden" value="1" name="iqsetupclose">
					<input type="hidden" value="" name="wlanSchEnabled5G">
					<input type="hidden" value="/wireless5g_schedule_v2.asp" name="submit-url">

				</td>
				</tr>
			</table>
			</form >
			<!-- ================================================================== -->

			<br>

			<!-- ================================================================== -->	
			<form name="formWIRESchDel" method="post" >

				<div id="wireSchRuleString" style="display:none">
					<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
						<tr>
							<td align="center" class="itemText"><script>dw(wireSchRuleOnly30);</script></td>
						</tr>
					</table>
					<br>
				</div>

				<table align="center" border="0" Cellspacing="1" cellpadding="4" width="700">
				<tr>
				<td>
					<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
						<tr>
							<td width="45%" align="center" class="dnsTableHe"><script>dw(TimeStart);</script></td>
							<td width="45%" align="center" class="dnsTableHe"><script>dw(TimeStop);</script></td>
							<td width="10%" align="center" class="dnsTableHe"><script>dw(select);</script></td>
						</tr>
					</table>
					<table width="600" cellpadding="2" cellspacing="1" align="center" border="0" class="footBorder">
						<% WIRESchList("2"); %> <!-- 1(2.4G),2(5G) -->
					</table>

					<table width="600" cellpadding="2" cellspacing="1" align="center" border="0">
						<tr>
							<td>
								&nbsp;
							</td>
						</tr>
						<tr>
							<td align="right">
								<script>document.write('<input type="button" value="'+showText(deleteSelected)+'" name="deleteSelSch" onClick="deleteClick(1)" class="ui-button-nolimit">');</script>&nbsp;
								<script>document.write('<input type="button" value="'+showText(deleteAll)+'" name="deleteAllSch" onClick="deleteClick(2)" class="ui-button-nolimit">');</script>
							</td>
						</tr>
					</table>
				</td>
				</tr>
				</table>
			</form>

			<form action="/goform/formWIRESch" name="submit_WIRESchDel" method="post" >
				<input type="hidden" value="2" id="selInterface" name="selInterface"> <!-- 1(2.4G),2(5G) -->
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="wlanSchEnabled5G">
				<input type="hidden" value="/wireless5g_schedule_v2.asp" name="submit-url">
				<input type="hidden" name="deleteSelSch" value="">
				<input type="hidden" name="deleteAllSch" value="">
				<input type="hidden" id="totalSelSunday"    name="totalSelSunday"    value="000000000000000000000000">
				<input type="hidden" id="totalSelMonday"    name="totalSelMonday"    value="000000000000000000000000">
				<input type="hidden" id="totalSelTuesday"   name="totalSelTuesday"   value="000000000000000000000000">
				<input type="hidden" id="totalSelWednesday" name="totalSelWednesday" value="000000000000000000000000">
				<input type="hidden" id="totalSelThursday"  name="totalSelThursday"  value="000000000000000000000000">
				<input type="hidden" id="totalSelFriday"    name="totalSelFriday"    value="000000000000000000000000">
				<input type="hidden" id="totalSelSaturday"  name="totalSelSaturday"  value="000000000000000000000000">
			</form>
			<!-- ================================================================== -->

			<br>

			<!-- ================================================================== -->	
			<form action="/goform/formWIRESch" name="formWIRESchSave" method="post" >
				<table align="center" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td style="text-align:center; padding-top:30px;">
							<script>
								document.write('<input type="button" value="'+showText(save)+' '+showText(Settings)+'" name="save" onclick="return saveChanges(0)" class="ui-button">');	
								var show_reboot = "<% getInfo("show_reboot"); %>"
								if(show_reboot == "1")
									document.write('<br><br><span class="save-itemText">'+showText(saveinfo)+'<a href="#" onClick="return saveChanges(1)" > '+showText(clickheretorestart)+' </a> '+showText(settingsintoeffect)+'</span>');
							</script>
						</td>
					</tr>
				</table>
				<input type="hidden" value="" name="isApply">
				<input type="hidden" value="1" name="iqsetupclose">
				<input type="hidden" value="" name="wlanSchEnabled5G">
				<input type="hidden" value="/wireless5g_schedule_v2.asp" name="submit-url">
			</form>
	</fieldset>			
<!--===========================================================================================-->
<script>
	$("#help-box").hide();
</script>

</blockquote>
<script>
	displayObj(document.getElementById('wireSchCount2').value); // 1(2.4G),2(5G) 
</script>
</body>
<head>
<meta http-equiv="Pragma" content="no-cache">
</head>
</html>

