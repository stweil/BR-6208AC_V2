/*
 *      Web server handler routines wlan stuffs
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: fmwlan.c,v 1.3 2005/05/13 07:53:56 tommy Exp $
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include	<sys/ioctl.h>
#include	<arpa/inet.h>

#include	<linux/wireless.h>
#include "../webs.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"
#include <ctype.h>
static SS_STATUS_Tp pStatus=NULL;
int wizardEnabled = 0;
int stadrvenable = 0; //EDX Timmy
int sitesurveyed = 0;
#ifdef _AUTO_CHAN_SELECT_
char ConnectTestKey[200];
int Chan = 0;
int width = 0;
int ssid2scan_fail=0;
extern int ssid2scan_fail5g;
extern int wep_used;
#endif
int selectNumx = -1;
int selectWanx = -1;

int selectNumy = -1;
int selectWany = -1;

int connectIF=0;
int SSIDexist=1;
/////////////////////////////////////////////////////////////////////////////
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
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
char charx64table[64]=
{
'0','1','2','3','4','5','6','7','8','9','@','A','B','C','D','E',
'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U',
'V','W','X','Y','Z','`','a','b','c','d','e','f','g','h','i','j',
'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};
static int	CharToInt_x64(char chVal)
{
	int intVal=0;
	if(chVal>90)
	{
		intVal=chVal-59;
	}
	else	if(chVal>57)
	{
		intVal=chVal-54;
	}
	else
	{
		intVal=chVal-48;
	}
	return intVal;
}

static char CharLogicGatesComputing(char chValA,char chValB,int type)  //type: 1=OR 2=XOR 
{
	char chValC='0';
	int intValA,intValB,intValC;
	if(chValA>90)
	{
		intValA=chValA-59;
	}
	else	if(chValA>57)
	{
		intValA=chValA-54;
	}
	else
	{
		intValA=chValA-48;
	}

	if(chValB>90)
	{
		intValB=chValB-59;
	}
	else	if(chValB>57)
	{
		intValB=chValB-54;
	}
	else
	{
		intValB=chValB-48;
	}

	if(type == 1) //OR
	{
	intValC=intValA|intValB;
	}
	else if(type == 2) //XOR
	{
	intValC=intValA^intValB;
	}
	if(intValC > 36 )
	{
		return chValC+intValC+11;
	}
	else if(intValC > 9 )
	{
		return chValC+intValC+6;
	}
	else
	{
		return chValC+intValC;
	}
}
#endif
/////////////////////////////////////////////////////////////////////////////
static inline int isAllStar(char *data)
{
	int i;
	for (i=0; i<strlen(data); i++) {
		if (data[i] != '*')
			return 0;
	}
	return 1;
}
// Preamble MODE in TxD
#define MODE_CCK	0
#define MODE_OFDM   1
#define MODE_HTMIX	2
#define MODE_HTGREENFIELD	3
static inline char* GetPhyMode(int Mode)
{
	switch(Mode)
	{
		case MODE_CCK:
			return "CCK";

		case MODE_OFDM:
			return "OFDM";

		case MODE_HTMIX:
			return "HTMIX";

		case MODE_HTGREENFIELD:
			return "GREEN";

		default:
			return "N/A";
	}
}
// BW
#define BW_20	0
#define BW_40	1
#define BW_BOTH		2
#define BW_10		3
static inline char* GetBW(int BW)
{
	switch(BW)
	{
		case BW_10:
			return "10M";

		case BW_20:
			return "20M";

		case BW_40:
			return "40M";

		default:
			return "N/A";
	}
}
/////////////////////////////////////////////////////////////////////////////
void formLicence(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAccept, *submitUrl;
	char tmpBuf[100];
	int val=1;
	strAccept = websGetVar(wp, T("accept"), T(""));
	if (strAccept[0]) {
		if ( apmib_set(MIB_LICENCE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormLicence1))</script>"));
			goto setErr_licence;
		}
		apmib_update(CURRENT_SETTING);
	}

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	if (strAccept[0]) {		//accept
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		else
			websDone(wp, 200);
	}
	else					//reject
		REJECT_MSG();

  	return;

setErr_licence:
	ERR_MSG(wp, tmpBuf);
}
/////////////////////////////WiFi Test///////////////////////////

void formWifiEnable(webs_t wp, char_t *path, char_t *query)
{
   	char_t *submitUrl, *strWifiEnable, *strWpsTest,*strReg,*strAReg;
	int val;
		char tmpBuf[100];

	strWifiEnable =  websGetVar(wp, T("Customer"), T(""));

		if (!gstrcmp(strWifiEnable, T("YES")))
			val = 1;
		else
			val = 0;
		if ( apmib_set(MIB_WIFI_TEST, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	#ifdef _DFS_

		strReg  =  websGetVar(wp, T("countryReg"), T(""));
		strAReg =  websGetVar(wp, T("aBandCountryReg"), T(""));
		if ( strReg[0] ) {
			val = (strReg[0] - '0');
			apmib_set(MIB_HW_COUNTRY_REG, (void *)&val);
		}
		if ( strAReg[0] ) {
			val = (strAReg[0] - '0');
			apmib_set(MIB_HW_COUNTRY_REG_ABAND, (void *)&val);
		}
		apmib_update(HW_SETTING);
	#endif


	apmib_update(CURRENT_SETTING);
	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

#ifdef _Customer_
	strWpsTest			= websGetVar(wp, T("wpsTest"), T(""));
	if(strWpsTest[0]){
		if (!gstrcmp(strWpsTest, T("ON")))
			system("/bin/wpsbuttontest.sh &");
	}
#endif
	OK_MSG(wp, submitUrl);
	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
void formWpsEnable(webs_t wp, char_t *path, char_t *query)
{

  char_t *submitUrl, *strWpsEnable, *iqsetupclose;

	int close_iQsetup,wiz_mode;
	int val;
	char tmpBuf[100];
	strWpsEnable =  websGetVar(wp, T("wps2g"), T(""));
	if(strWpsEnable[0]){
		if (!gstrcmp(strWpsEnable, T("ON"))) val = 1;
		else val = 0;

		printf("2.4G WPS Enable=%d\n",val);
		if ( apmib_set(MIB_WPS_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}

		apmib_get(MIB_WIZ_MODE, (void *)&wiz_mode);

		if( val == 1){ 
			if(wiz_mode!=2)
				val = 7;
			else
				val = 6; //EDX Ken, update Repeater mode not support wps Enrollee
		}
		else 
			val = 0;

		if ( apmib_set(MIB_WPS_CONFIG_MODE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	}
	strWpsEnable =  websGetVar(wp, T("wps5g"), T(""));
	if(strWpsEnable[0]){
		if (!gstrcmp(strWpsEnable, T("ON"))) val = 1;
		else val = 0;

		printf("5G WPS Enable=%d\n",val);
		if ( apmib_set(MIB_INIC_WPS_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}

		if( val == 1){ 
			if(wiz_mode!=2)
				val = 7;
			else
				val = 6;//EDX Ken, update Repeater mode not support wps Enrollee
		}
		else 
			val = 0;

		if ( apmib_set(MIB_INIC_WPS_CONFIG_MODE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	}
	#ifdef _IQv2_
	//char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);
	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	
	char_t *strApply;
	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;
		
			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp,tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp,tmpBufapply ,submitUrl);
		}
		system("/bin/reboot.sh 3 &");
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
/*
/////////WPSCONFIG//////proxy,internal Register ////////
void formWpsConfig(webs_t wp, char_t *path, char_t *query)
{
   char_t *submitUrl, *strEnProxy, *strEnInternalReg;
	int proxyVal,regVal,cmodeVal;
	char tmpBuf[100];

	strEnProxy = websGetVar(wp, T("enProxy"), T(""));
	if (strEnProxy[0]) {
		if (!gstrcmp(strEnProxy, T("no")))
			proxyVal = 0;
		else if (!gstrcmp(strEnProxy, T("yes")))
			proxyVal = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable1))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WPS_PROXY_ENABLE, (void *)&proxyVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	}
		strEnInternalReg = websGetVar(wp, T("enInternalReg"), T(""));
	if (strEnInternalReg[0]) {
		if (!gstrcmp(strEnInternalReg, T("no")))
			regVal = 0;
		else if (!gstrcmp(strEnInternalReg, T("yes")))
			regVal = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable1))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WPS_INTERNAL_REG, (void *)&regVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	}
		cmodeVal=1+(2*proxyVal)+(4*regVal);
		if ( apmib_set(MIB_WPS_CONFIG_MODE, (void *)&cmodeVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	//sprintf(tmpBuf, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG); // Tommy
	//system(tmpBuf);
#endif


	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl); // Tommy
	OK_MSG(wp, submitUrl);

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}


///////////////////////////WpS Reset//Out-Of-Box//////////////////
void formWpsRest(webs_t wp, char_t *path, char_t *query)
{
	char_t *resetOOB,*submitUrl;
	int val;
	char tmpBuf[100];
		resetOOB = websGetVar(wp, T("resetOOB"), T(""));
	if (resetOOB[0]) {
		if (!gstrcmp(resetOOB, T("no")))
			val = 0;
		else if (!gstrcmp(resetOOB, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable1))</script>"));
			goto setErr_advance;
		}
	}
		sprintf(tmpBuf, "iwpriv rai0 set WscOOB=%d", val);
		system(tmpBuf);

	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	//sprintf(tmpBuf, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG); // Tommy
	//system(tmpBuf);
#endif


	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl); // Tommy
	OK_MSG(wp, submitUrl);

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);

}
*/
////////////////////WpsStart/////////////////////////////////////
void formWpsStart(webs_t wp, char_t *path, char_t *query)
{
 	char_t *submitUrl, *strModeOpt ,*pingCode, *band_select;
	int modeVal, usePin=1, usePbc=2, useAband=0,i;
	char tmpBuf[100], tmpIF[10], tmpSCRIPT[10];
	char pinNumber[10];//vance

	band_select = websGetVar(wp, T("band_select"), T(""));
	if (band_select[0]){
		useAband = atoi(band_select);
		if(useAband){
			sprintf(tmpIF, "rai0");
			sprintf(tmpSCRIPT, "inic_wps.sh");
		}
		else{
		 	sprintf(tmpIF, "ra0");
			sprintf(tmpSCRIPT, "wps.sh");
		}
	}
	else {
		DEBUGP(tmpBuf, T("band_select error!"));
		goto setErr_advance;
	}
	
	strModeOpt = websGetVar(wp, T("configOption"), T(""));
	if (strModeOpt[0]) {
		if (!gstrcmp(strModeOpt, T("pin")))
			modeVal = usePin;
		else if (!gstrcmp(strModeOpt, T("pbc")))
			modeVal = usePbc;
		else {
			DEBUGP(tmpBuf, T("configOption error!"));
			goto setErr_advance;
		}

		pingCode=websGetVar(wp, T("PinCode"), T(""));
		printf("pingCode=%s\n",pingCode);
		if(modeVal == usePin){
			if(pingCode[0])
			{
				if (strlen(pingCode) == 4 || strlen(pingCode) == 8) //EDX, Robert 2015_09_04 add, Avoid pinCode is "systemcode".
				{
					sprintf(pinNumber, "%s",pingCode);

					for(i=0;i<strlen(pinNumber);i++)
					{
						if(pinNumber[i]<48 || 57<pinNumber[i])
						{
							DEBUGP(tmpBuf, T("<script>document.write('Input PinCode error!')</script>"));
							goto setErr_advance;
						}
					}
					sprintf(tmpBuf, "%s %s %d %s 0", tmpSCRIPT, tmpIF, usePin, pinNumber);
				}
				else
				{
					DEBUGP(tmpBuf, T("<script>document.write('Input PinCode error!')</script>"));
					goto setErr_advance;
				}
			}
			else
				sprintf(tmpBuf, "%s %s %d -1 0", tmpSCRIPT, tmpIF, usePin);
		}
		else if(modeVal == usePbc){
			sprintf(tmpBuf, "%s %s %d 0 0", tmpSCRIPT, tmpIF, usePbc);
		}
		
		printf("%s\n",tmpBuf);
		#ifdef __TARGET_BOARD__	
		system(tmpBuf);
		#endif
	}

	/*sprintf(pinNumber, "%s",pingCode);
	if ( apmib_set(MIB_WPS_PING_CODE, (void *)&pinNumber) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_advance;
	}*/

	#ifdef __TARGET_BOARD__	
	apmib_reinit();
	#endif

	submitUrl = websGetVar(wp, T("wlan-url"), T(""));
	websRedirect(wp, submitUrl);

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#ifdef HOME_GATEWAY
void formWizSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strEnWiz;
	char_t *strStateChanged, *strMcastChanged;
	int intVal;
   submitUrl = websGetVar(wp, T("setPage"), T(""));
   strEnWiz = websGetVar(wp, T("wizEnabled"), T(""));
   strStateChanged = websGetVar(wp, T("chagestate"), T(""));
   wizardEnabled = atoi(strEnWiz);

#ifdef _LEDSWITCH_
    char tempBuf[100];
    if (strStateChanged[0]){
	intVal = atoi(strStateChanged);
	if ( !apmib_set(MIB_LED_CONTROL, (void *)&intVal)) {
	   return;
	}
    	apmib_update(CURRENT_SETTING);

#ifdef __TARGET_BOARD__
	if ( intVal == 1)
		system("/bin/scriptlib_util.sh led_control ON");
	else if ( intVal == 0)
		system("/bin/scriptlib_util.sh led_control OFF");
#endif

      websRedirect(wp,"/template1.asp");
      return;
     }
#endif

#ifdef _MCAST_RATE_SUPPORT_
	strMcastChanged = websGetVar(wp, T("mcast_chagestate"), T(""));

	if (strMcastChanged[0]){
		int bandVal;
		intVal = atoi (strMcastChanged);

		if ( !apmib_set(MIB_MCAST_RATE_ENABLED,  (void *)&intVal) )
                        return;

		if (intVal == 0){
			intVal = 16;

			if ( !apmib_set(MIB_MCAST_RATE,  (void *)&intVal) )
        	                return;

			if ( !apmib_set(MIB_INIC_MCAST_RATE,  (void *)&intVal) )
        	                return;
		}else{
			// 1:B , 2:N , 3:B+G ,  4:G ,  5:B+G+N
			// CCK: MCS 0 ~ 3 1Mbps ~ 11Mbps 	3: 11Mbps
			// OFDM: MCS 0 ~ 7 6Mbps ~ 54Mbps	7: 54Mbps
			// HT: MCS 0 ~ 15 6.5Mbps ~ 130Mbps	5: 52Mbps ==> for logitec ui mapping to 11Mbps
			apmib_get(MIB_WLAN_BAND, (void *)&bandVal);

			apmib_get(MIB_MCAST_RATE,  (void *)&intVal);
			if (intVal != 16){
				if (bandVal == 1){
					intVal = 3;

				}else if ((bandVal == 2) || (bandVal == 5)){
					intVal = 3;

				}else if ((bandVal == 3) || (bandVal == 4)){
					intVal = 7;
				}
			}
			if ( !apmib_set(MIB_MCAST_RATE,  (void *)&intVal) )
				return;

			// 1:A , 2:N , 3:A+N
			// OFDM: MCS 0 ~ 7 6Mbps ~ 54Mbps	7: 54Mbps
			// HT: MCS 0 ~ 15 6.5Mbps ~ 130Mbps	5: 52Mbps ==> for logitec ui mapping to 11Mbps
			apmib_get(MIB_INIC_BAND, (void *)&bandVal);
			apmib_get(MIB_INIC_MCAST_RATE,  (void *)&intVal);

			if (intVal != 16){
				if (bandVal == 1){
					intVal = 7;

				}else{
					intVal = 3;

				}
			}

			if ( !apmib_set(MIB_INIC_MCAST_RATE,  (void *)&intVal) )
				return;

		}

		apmib_update(CURRENT_SETTING);

      		websRedirect(wp,"/template1.asp");
		return;
	}

#endif //_MCAST_RATE_SUPPORT_


   if(wizardEnabled == 1)
    {
        if ( !strcmp(submitUrl,"/setmenu.html")) {
        		system("echo \"replace1 funciton\"");
            REPLACE1_MSG(submitUrl);
            return;
        }
        else {
        	system("echo \"replace funciton\"");
        		REPLACE_MSG(submitUrl);
            return;
        }
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////
void formALGSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *strALG, *submitUrl;
	char tmpBuf[100];
	int intVal;

	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	strALG = websGetVar(wp, T("appLyGatewayValue"), T(""));

	if (strALG[0]) {
		if (string_to_dec(strALG, &intVal))
			apmib_set(MIB_APP_LAYER_GATEWAY, (void *)&intVal);
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);

	char_t *strApply;
	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void formWlEnable(webs_t wp, char_t *path, char_t *query)
{
   	char_t *submitUrl, *strWirelessEnable, *strnextEnable, *strIsolationEnable;
	int val;
	char tmpBuf[100];
	FILE *fp;

	strWirelessEnable = websGetVar(wp, T("wlanDisabled"), T(""));
	if (strWirelessEnable[0]) {
		if (!gstrcmp(strWirelessEnable, T("no")))
			val = 1;
		else if (!gstrcmp(strWirelessEnable, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_advance;
		}

		if ( apmib_set(MIB_WLAN_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_advance;
		}
	}

	#ifdef _TEXTFILE_CONFIG_
	strIsolationEnable = websGetVar(wp, T("mainIsoEnbled"), T(""));
	if (strIsolationEnable[0]) {
		printf("2.4G BLOCKRELAY=%s\n",strIsolationEnable);
		if (!gstrcmp(strIsolationEnable, T("yes")))
			val = 1;
		else if (!gstrcmp(strIsolationEnable, T("no")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}

		if ( apmib_set(MIB_WLAN_NOFORWARD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("set MIB_WLAN_NOFORWARD error!"));
			goto setErr_advance;
		}
	}
	#endif
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	// run script file to update WLAN parameters
	//sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "gw", "all"); // Tommy
	//system(tmpBuf);
#endif

	//submitUrl = websGetVar(wp, T("wlanurl"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl);

//20060410 pippen
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif


	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	strnextEnable = websGetVar(wp, T("nextEnable"), T(""));

	if (strnextEnable[0] || wizardEnabled == 1){
		REPLACE_MSG(submitUrl);
	} else {
		OK_MSG(wp, submitUrl);
	}
	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#if defined(_IGMP_PROXY_)
/////////////////////////////////////////////////////////////////////////////
void formIgmpEnable(webs_t wp, char_t *path, char_t *query)
{
  char_t *submitUrl, *strIgmpEnable, *iqsetupclose;
	int val;
	char tmpBuf[100];

	strIgmpEnable = websGetVar(wp, T("igmpEnable"), T(""));
	if (strIgmpEnable[0]) {
		if (!gstrcmp(strIgmpEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strIgmpEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_IGMP_PROXY_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
			goto setErr_advance;
		}
	}

	#ifdef _IGMP_SNOOPING_ENABLED_   //EDX patrick add
	strIgmpEnable = websGetVar(wp, T("igmpsnoop_enable"), T(""));
	if (strIgmpEnable[0]) {
		if (!gstrcmp(strIgmpEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strIgmpEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_IGMP_SNOOP_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
			goto setErr_advance;
		}
	}
	#endif

	#ifdef _IQv2_
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page

	char tmpBufapply[200];
	char_t *strApply;
	strApply = websGetVar(wp, T("isApply"),T(""));


	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#endif
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void formAPModeSwitch(webs_t wp, char_t *path, char_t *query)
{
   	char_t *submitUrl, *strSwitch;
	int apEnable,chkAPEnable,val,oldSwitchValue;
	char tmpBuf[200];
	strSwitch = websGetVar(wp, T("switch"), T(""));
	if (strSwitch[0]) {
		if (!gstrcmp(strSwitch, T("Router"))){
			apEnable = 0;
			chkAPEnable=0;

		}
		else if (!gstrcmp(strSwitch, T("AP"))){
			apEnable = 1;
			chkAPEnable=1;

		}
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		apmib_get(MIB_AP_ROUTER_SWITCH, (void *)&oldSwitchValue);
		if ( apmib_set(MIB_AP_ROUTER_SWITCH, (void *)&apEnable) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
			goto setErr_advance;
		}
	}

	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	if(apEnable==1){
		val=0;
		apmib_set(MIB_DHCP, (void *)&val);
		apmib_set(MIB_FIREWALL_ENABLE, (void *)&val);

	}else{
		val=2;
		apmib_set(MIB_DHCP, (void *)&val);
		val=1;
		apmib_set(MIB_FIREWALL_ENABLE, (void *)&val);
	}

	apmib_update(CURRENT_SETTING);
	if(oldSwitchValue!=chkAPEnable){

#ifdef __TARGET_BOARD__
		system("killall -SIGTERM pppd");
		sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "gw", "all");
		system(tmpBuf);
#else
		sleep(3);
#endif

	}
	websRedirect(wp, submitUrl);
	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}

/////////////////////////////////////////////////////////////////////////////
void formNatEnable(webs_t wp, char_t *path, char_t *query)
{
   	char_t *submitUrl, *strNatEnable,*strFastNatEnable;
	int val;
	char tmpBuf[100];

	strNatEnable = websGetVar(wp, T("natEnable"), T(""));
	if (strNatEnable[0]) {
		if (!gstrcmp(strNatEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strNatEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_NAT_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
			goto setErr_advance;
		}
	}

	strFastNatEnable = websGetVar(wp, T("fastNatEnable"), T(""));
	if (strFastNatEnable[0]) {
		if (!gstrcmp(strFastNatEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strFastNatEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_FAST_NAT_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormFastNatEnable))</script>"));
			goto setErr_advance;
		}
	}

	apmib_update(CURRENT_SETTING);
#ifndef NO_ACTION
	//sprintf(tmpBuf, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG); // Tpmmy
	//system(tmpBuf);
#endif

	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl); // Tommy

#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif
#if defined(_Customer_)
                char_t *strnextEnable;
                strnextEnable = websGetVar(wp, T("new_apply"), T(""));
                if (strnextEnable[0])
                {
                        system("/bin/reset.sh gw 2");
                }
                REPLACE_MSG(submitUrl);
#else
	OK_MSG(wp, submitUrl);
#endif
	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
/////////////////////////////////////////////////////////////////////////////
void formFwEnable(webs_t wp, char_t *path, char_t *query)
{
 	char_t *submitUrl, *strFwEnable, *strpingEnable, *strApply;
	int val;
	char tmpBuf[100];

	strFwEnable = websGetVar(wp, T("FirewallEnable"), T(""));
	if (strFwEnable[0]) {
		if (!gstrcmp(strFwEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strFwEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable1))</script>"));
			goto setErr_advance;
		}
		printf("FirewallEnable=%d\n", val);
		if ( apmib_set(MIB_FIREWALL_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
	}


	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

  apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	
	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////
void formWlanSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strSSID, *strSSID1, *strChan, *strDisabled, *strBand,*strABand, *strAutoClone, *strVal, *strWisp, *strnextEnable;
	char_t *strMode, *strMac1, *strMac2, *strMac3, *strMac4, *strMac5, *strMac6, *strWlanMac, *strWpsStatus, *strWirelessEnable;
	int chan, disabled, mode, mv, val, wisp,txval;
	NETWORK_TYPE_T net;
	char tmpBuf[100];
	int intVal=0;
#ifdef _AUTO_MANUAL_SELECTED_CHAN_
	int intChantemp = 1;
	char_t *strchtype;
#endif
#ifdef _WEB_WL_SW_
	// set wlan Disabled
	strDisabled = websGetVar(wp, T("wlanDisabled"), T(""));
	if ( !gstrcmp(strDisabled, T("no")))
		disabled = 1;
	else
		disabled = 0;

	if ( apmib_set( MIB_WLAN_DISABLED, (void *)&disabled) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
		goto setErr_wlan;
	}
	if ( disabled )
		goto setwlan_ret;
#endif
	// set AP Mode
	strMode = websGetVar(wp, T("apMode"), T(""));
	if (strMode[0]) {
		errno=0;
		mode = strtol( strMode, (char **)NULL, 10);
		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup2))</script>"));
			goto setErr_wlan;
		}
		if ( apmib_set( MIB_AP_MODE, (void *)&mode) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup3))</script>"));
			goto setErr_wlan;
		}
	}

	strBand = websGetVar(wp, T("band"), T(""));
	if ( strBand[0] ) {
		if (strBand[0] < '0' || strBand[0] > '6') {
  			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup4))</script>"));
			goto setErr_wlan;
		}
		val = (strBand[0] - '0' + 1);
		if ( apmib_set( MIB_WLAN_BAND, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_wlan;
		}
	}

#ifdef _MCAST_RATE_SUPPORT_
	{
	char *strMcastRate;

	strMcastRate = websGetVar (wp, T("McastRate"), T(""));
	if (strMcastRate[0]){
		val = atoi (strMcastRate);
		printf ("strMcastRate = %s val = %d \n", strMcastRate, val);
		apmib_set (MIB_MCAST_RATE, (void *)&val);
	}
	}
#endif


#if defined(_Customer_) && defined(_EW7415PDN_)
	if(mode==0||mode==2||mode==5||mode==6)
	{
		char_t *wireless_block_relay;
		wireless_block_relay = websGetVar(wp, T("wireless_block_relay"), T(""));
		if( !strcmp(wireless_block_relay, "on") ) val=1;
		else val=0;
		apmib_set( MIB_WLAN_NOFORWARD, (void *)&val);
	}
#endif
#ifdef _Customer_
  char_t *strChanWidth;
  strChanWidth = websGetVar(wp, T("wlanNChanWidth"), T(""));      //no use
  if (strChanWidth[0]) {
    if (!gstrcmp(strChanWidth, T("0")))
	    val = 0;
	  else if (!gstrcmp(strChanWidth, T("1")))
      val = 1;
    else {
      DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
      goto setErr_wlan;
    }
    if ( apmib_set(MIB_WLAN_N_CHANNEL_WIDTH, (void *)&val) == 0) {
      DEBUGP(tmpBuf, T("Invalid Channel Width 1"));
      goto setErr_wlan;
    }
  }

  // set hidden SSID
	char_t *strHiddenSSID;
	strHiddenSSID = websGetVar(wp, T("hiddenSSID"), T(""));
	if (strHiddenSSID[0]) {
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup20))</script>"));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_wlan;
		}
	}
                                                                                                                                                                                               #endif
#ifdef _DBAND_
	strABand = websGetVar(wp, T("aband"), T(""));

	if ( strABand[0] ) {
		if (strABand[0] < '0' || strABand[0] > '1') {
  			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup4))</script>"));
			goto setErr_wlan;
		}
		val = (strABand[0] - '0' + 1);
		if ( apmib_set( MIB_ABAND_MODE, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_wlan;
		}
	}
#endif
#ifdef UNIVERSAL_REPEATER
	// Universal Repeater
	strWisp = websGetVar(wp, T("wisp"), T(""));
	wisp = strtol( strWisp, (char **)NULL, 10);

	if (mode == 6 || mode == 2 || mode == 7) {

		val = 1;
		if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup6))</script>"));
			goto setErr_wlan;
		}

		//Kyle add.Set AP Mode , bridge wan and lan , disable nat dhcp and some web ui.
		//if ( apmib_set( MIB_AP_ROUTER_SWITCH, (void *)&val) == 0) {
			//DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup6))</script>"));
			//goto setErr_wlan;
		//}

		if (mode == 2) {
			strSSID = websGetVar(wp, T("ssid"), T(""));
		}
		else {
			strSSID = websGetVar(wp, T("repeaterSSID"), T(""));
		}
		printf("repeaterssid=%s\n",strSSID);
		if ( strSSID[0] ) {
			int ii;
			for(ii=MAX_SSID_LEN-1;ii>0 && isspace(strSSID[ii]) ;ii--)
			{
				strSSID[ii]='\0';
			}
			if ( apmib_set(MIB_REPEATER_SSID, (void *)strSSID) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
					goto setErr_wlan;
			}
		}
	}
	else {
		val = 0;
		if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspformStatsErrRep))</script>"));
		goto setErr_wlan;
		}
	}

	//AP client mode type
	#ifndef _RESERVE_ENCRYPTION_SETTING_
	if (mode == 1 || mode ==2) {
		if (strMode[0]=='1')
			net = ADHOC;
		if (strMode[0]=='2')
			net = INFRASTRUCTURE;
		if ( apmib_set(MIB_WLAN_NETWORK_TYPE, (void *)&net) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
			goto setErr_wlan;
		}

	}
	#endif
#endif
	// set auto MAC Clone
	strAutoClone = websGetVar(wp, T("autoMacClone"), T(""));
	if (strAutoClone[0]) {
		if (!gstrcmp(strAutoClone, T("no")))
			val = 0;
		else if (!gstrcmp(strAutoClone, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup13))</script>"));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_AUTO_MAC_CLONE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup14))</script>"));
			goto setErr_wlan;
		}
	}

	strSSID = websGetVar(wp, T("ssid"), T(""));
	if ( strSSID[0] ) {
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_wlan;
		}
	}


	strChan = websGetVar(wp, T("chan"), T(""));
	if ( strChan[0] ) {
		errno=0;
		chan = strtol( strChan, (char **)NULL, 10);
		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup16))</script>"));
			goto setErr_wlan;
		}
		if ( apmib_set( MIB_WLAN_CHAN_NUM, (void *)&chan) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
			goto setErr_wlan;
		}
	}

	strWisp = websGetVar(wp, T("wisp"), T(""));
	wisp = strtol( strWisp, (char **)NULL, 10);
	if ( mode == 1 || mode ==2 || mode==6 || wisp == 1 || wisp == 3) {
		strWlanMac = websGetVar(wp, T("wlanMac"), T(""));
		if (strWlanMac[0]) {
			if (strlen(strWlanMac)!=12 || !string_to_hex(strWlanMac, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup18))</script>"));
				goto setErr_wlan;
			}
			if ( !apmib_set(MIB_WLAN_MAC_ADDR, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup19))</script>"));
				goto setErr_wlan;
			}

			if (strcmp(strWlanMac,"000000000000")) {
				for (mv=5; mv>=3; mv--) {
					tmpBuf[mv] = (tmpBuf[mv]-1);
					if ( (tmpBuf[mv]+1) != 0 )
						break;
				}
			}
			else {
				if (!string_to_hex("000000000000", tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup20))</script>"));
					goto setErr_wlan;
				}
			}

			if ( !apmib_set(MIB_ELAN_MAC_ADDR, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup21))</script>"));
				goto setErr_wlan;
			}
		}
	}

//**************************************** WDS setting ****************************************
	strMac1 = websGetVar(wp, T("wlLinkMac1"), T(""));
	if (strMac1[0]) {
		if (strlen(strMac1)!=12 || !string_to_hex(strMac1, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup22))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC1, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup23))</script>"));
			goto setErr_wlan;
		}
	}
	strMac2 = websGetVar(wp, T("wlLinkMac2"), T(""));
	if (strMac2[0]) {
		if (strlen(strMac2)!=12 || !string_to_hex(strMac2, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup24))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC2, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup25))</script>"));
			goto setErr_wlan;
		}
	}
	strMac3 = websGetVar(wp, T("wlLinkMac3"), T(""));
	if (strMac3[0]) {
		if (strlen(strMac3)!=12 || !string_to_hex(strMac3, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup26))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC3, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup27))</script>"));
			goto setErr_wlan;
		}
	}
	strMac4 = websGetVar(wp, T("wlLinkMac4"), T(""));
	if (strMac4[0]) {
		if (strlen(strMac4)!=12 || !string_to_hex(strMac4, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup28))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC4, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup29))</script>"));
			goto setErr_wlan;
		}
	}
	strMac5 = websGetVar(wp, T("wlLinkMac5"), T(""));
	if (strMac5[0]) {
		if (strlen(strMac5)!=12 || !string_to_hex(strMac5, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup30))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC5, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup31))</script>"));
			goto setErr_wlan;
		}
	}
	strMac6 = websGetVar(wp, T("wlLinkMac6"), T(""));
	if (strMac6[0]) {
		if (strlen(strMac6)!=12 || !string_to_hex(strMac6, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup32))</script>"));
			goto setErr_wlan;
		}
		if ( !apmib_set(MIB_WL_LINKMAC6, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup33))</script>"));
			goto setErr_wlan;
		}
	}

setwlan_ret:
//EDX shakim start
	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
	if (!gstrcmp(strWpsStatus, T("1"))){
		val = 1;
		if ( apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_wlan;
		}
		val = 0;
		if ( apmib_set(MIB_WPS_DISPLAY_KEY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_wlan;
		}
	}
//EDX shakim end
	apmib_update(CURRENT_SETTING);


	strWirelessEnable = websGetVar(wp, T("wlanDisabled"), T(""));
	if (strWirelessEnable[0]) {
		if (!gstrcmp(strWirelessEnable, T("no")))
			val = 1;
		else if (!gstrcmp(strWirelessEnable, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_wlan;
		}
	}
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif

//20060410 pippen
#if defined(_WLBASIC_SECURITY_MIXED_PAGE_)
	strnextEnable=NULL;
	strnextEnable = websGetVar(wp, T("apmoderedirect"), T(""));
	if(!strnextEnable[0]){
		formWlEncrypt(wp,path,query);
		return;
	}
	else
	{
		submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
		strnextEnable = websGetVar(wp, T("redirect"), T(""));
		if (strnextEnable[0] || wizardEnabled == 1){
			REPLACE_MSG(submitUrl);
		}
		else{
			OK_MSG(wp, submitUrl);
		}
	}
	return;
#else
	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	strnextEnable = websGetVar(wp, T("redirect"), T(""));
	if (strnextEnable[0] || wizardEnabled == 1){
		REPLACE_MSG(submitUrl);
	}
	else{
		OK_MSG(wp, submitUrl);
   }
   return;
#endif

setErr_wlan:
	ERR_MSG(wp, tmpBuf);
}
/////////////////////////////////////////////////////////////////////////////
#ifndef HOME_GATEWAY
void formStaEncrypt(webs_t wp, char_t *path, char_t *query)
{
	int keyLen, ret, i;
	WEP_T wep;
	char tmpBuf[100], key[30];
	int intVal=0, method=0, len, eapType=0, secMode=0, enRadius=0,ahSecMode=0;

   	char_t *wepKey, *strKeyLen, *strFormat, *strKeyId, *submitUrl, *strVal;
	char_t *strMethod, *strWepTp, *strEnRadius, *strEapTp;
	char_t *strId, *strPass, *strTtls, *strPeap, *strEapId, *strEapPass;
	char_t *strEnCltCer, *strEnSerCer, *strCltPass, *strSerPass;

	strMethod = websGetVar(wp, T("method"), T(""));
	method = strMethod[0] - '0';
	if ( !apmib_set( MIB_STA_ENCRYPT, (void *)&method ) ) {
 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt1))</script>"));
		goto setErr_encrypt;
	}

//*********************************** Wep setting ***********************************
	strWepTp = websGetVar(wp, T("wepTp"), T(""));

	if ( method==1 || (method==0 && strWepTp[0]=='1')) {
		strKeyLen = websGetVar(wp, T("length"), T(""));
		if (!strKeyLen[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt2))</script>"));
			goto setErr_encrypt;
		}
		if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt3))</script>"));
			goto setErr_encrypt;
		}
		if (strKeyLen[0] == '1')
			wep = WEP64;
		else if (strKeyLen[0] == '2')
			wep = WEP128;
	}
	else
		wep = WEP_DISABLED;

	if ( apmib_set( MIB_STA_WEP, (void *)&wep) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt4))</script>"));
		goto setErr_encrypt;
	}

//	if (wep == WEP_DISABLED)
//		goto setwep_ret;

	if ( method==1 || (method==0 && strWepTp[0]=='1')) {
		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt5))</script>"));
			goto setErr_encrypt;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt6))</script>"));
			goto setErr_encrypt;
		}

		i = strFormat[0] - '0' - 1;
		if ( apmib_set( MIB_STA_WEP_KEY_TYPE, (void *)&i) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_encrypt;
		}

		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;
			else
				keyLen = WEP128_KEY_LEN*2;
		}

		strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText())</script>Invalid default tx key id!"));
   				goto setErr_encrypt;
			}
			i = strKeyId[0] - '0' - 1;
			if ( !apmib_set( MIB_STA_WEP_DEFKEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt8))</script>"));
   				goto setErr_encrypt;
			}
		}

		wepKey = websGetVar(wp, T("key1"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY1, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
					goto setErr_encrypt;
				}
			}
		}
		wepKey = websGetVar(wp, T("key2"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt13))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt14))</script>"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY2, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt15))</script>"));
					goto setErr_encrypt;
				}
			}
		}

		wepKey = websGetVar(wp, T("key3"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt16))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt17))</script>"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY3, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt18))</script>"));
					goto setErr_encrypt;
				}
			}
		}

		wepKey = websGetVar(wp, T("key4"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt19))</script>"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY4, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY4, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt20))</script>"));
					goto setErr_encrypt;
				}
			}
		}
	}

	if ( method==2 || method==3) {
		strVal = websGetVar(wp, T("wpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if ( apmib_set(MIB_STA_WPA_CIPHER_SUITE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
			goto setErr_encrypt;
		}
	}
//*********************************** pre-shared key setting ***********************************
	if ( method==2 ) {
		int oldFormat, oldPskLen, i;

		strVal = websGetVar(wp, T("pskFormat"), T(""));
		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt22))</script>"));
			goto setErr_encrypt;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt23))</script>"));
			goto setErr_encrypt;
		}

		// remember current psk format and length to compare to default case "****"
		apmib_get(MIB_STA_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_STA_WPA_PSK, (void *)tmpBuf);
		oldPskLen = strlen(tmpBuf);

		strVal = websGetVar(wp, T("psk"), T(""));
		len = strlen(strVal);

		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto setwep_ret;
		}

		if ( apmib_set(MIB_STA_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt24))</script>"));
			goto setErr_encrypt;
		}

		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		if ( !apmib_set(MIB_STA_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt26))</script>"));
			goto setErr_encrypt;
		}
	}
//**************************************** Radius ****************************************
	if (method!=3) {
		strEnRadius = websGetVar(wp, T("enRadius"), T(""));
		if ( !gstrcmp(strEnRadius, T("ON")))
			enRadius = 1;
		else
			enRadius = 0;
	}
	else
		enRadius = 1;
	if ( !apmib_set( MIB_STA_ENRADIUS, (void *)&enRadius ) ) {
 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt27))</script>"));
		goto setErr_encrypt;
	}

	if ( enRadius==1 && method!=2 ) {

		strEapTp = websGetVar(wp, T("eapType"), T(""));
		eapType = strEapTp[0] - '0';
		if ( !apmib_set( MIB_STA_EAP_TYPE, (void *)&eapType ) ) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt28))</script>"));
			goto setErr_encrypt;
		}
		strId = websGetVar(wp, T("identity"), T(""));
		if ( strId[0] ) {
			if ( apmib_set( MIB_STA_ID, (void *)strId) == 0) {
   		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt29))</script>"));
					goto setErr_encrypt;
			}
		}
		if ( eapType==3 ) {
			strPass = websGetVar(wp, T("md5Pass"), T(""));
			if ( strPass[0] ) {
				if ( apmib_set( MIB_STA_PASS, (void *)strPass) == 0) {
   			 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt30))</script>"));
						goto setErr_encrypt;
				}
			}
		}

		if ( eapType==0 ) {
			strPeap = websGetVar(wp, T("peap"), T(""));
			if ( strPeap[0] ) {
				if ( apmib_set( MIB_STA_PROTOCOL, (void *)strPeap) == 0) {
   			 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt31))</script>"));
						goto setErr_encrypt;
				}
			}
		}
		else if ( eapType==2 ) {
			strTtls = websGetVar(wp, T("ttls"), T(""));
			if ( strTtls[0] ) {
				if ( apmib_set( MIB_STA_PROTOCOL, (void *)strTtls) == 0) {
   		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt31))</script>"));
					goto setErr_encrypt;
				}
			}
		}

		if ( eapType==0 || eapType==2) {
			strEapId = websGetVar(wp, T("eapId"), T(""));
			if ( strEapId[0] ) {
				if ( apmib_set( MIB_STA_EAP_ID, (void *)strEapId) == 0) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt32))</script>"));
					goto setErr_encrypt;
				}
			}

			strEapPass = websGetVar(wp, T("eapPass"), T(""));
			if ( strEapPass[0] ) {
				if ( apmib_set( MIB_STA_EAP_PASS, (void *)strEapPass) == 0) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt33))</script>"));
					goto setErr_encrypt;
				}
			}
		}

		if ( eapType!=1 ) {
			strEnCltCer = websGetVar(wp, T("enCltCer"), T(""));
			if ( !gstrcmp(strEnCltCer, T("ON")))
				intVal = 1;
			else
				intVal = 0;
		}
		else {
			intVal = 1;
		}
		if ( !apmib_set( MIB_STA_ENCLT_CER, (void *)&intVal ) ) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt34))</script>"));
			goto setErr_encrypt;
		}

		strCltPass = websGetVar(wp, T("cltPass"), T(""));
		if ( strCltPass[0] ) {
			if ( apmib_set( MIB_STA_CLIENT_PASS, (void *)strCltPass) == 0) {
   		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt35))</script>"));
					goto setErr_encrypt;
			}
		}

		strEnSerCer = websGetVar(wp, T("enSerCer"), T(""));
		if ( !gstrcmp(strEnSerCer, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( !apmib_set( MIB_STA_ENSER_CER, (void *)&intVal ) ) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt36))</script>"));
			goto setErr_encrypt;
		}

		strSerPass = websGetVar(wp, T("serPass"), T(""));
		if ( strSerPass[0] ) {
			if ( apmib_set( MIB_STA_SERVER_PASS, (void *)strSerPass) == 0) {
   		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt37))</script>"));
					goto setErr_encrypt;
			}
		}
	}

setwep_ret:


	apmib_get( MIB_AP_MODE, (void *)&intVal);

//	if ( intVal == 2) {

		if (method==0 && wep==WEP_DISABLED && enRadius==0) {
			ahSecMode = 0;
			secMode = 0;
		}
		else if ((method==0 || method==1) && wep!=WEP_DISABLED && enRadius==0) {
			ahSecMode = 1;
			secMode = 1;
		}
		else if (method==0 &&  wep==WEP_DISABLED && enRadius==1) {
			ahSecMode = 0;
			secMode = 2;
		}
		else if ((method==0 || method==1) && wep!=WEP_DISABLED && enRadius==1) {
			ahSecMode = 1;
			secMode = 3;
		}
		else if (method==2) {
			ahSecMode = 0;
			secMode = 4;
		}
		else {
			ahSecMode = 0;
			secMode = 5;
		}

		if ( !apmib_set( MIB_STA_SEC_MODE, (void *)&secMode ) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt38))</script>"));
			goto setErr_encrypt;
		}

		if ( !apmib_set( MIB_ADHOC_SEC_MODE, (void *)&ahSecMode ) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt39))</script>"));
			goto setErr_encrypt;
		}
/*	}
	else if (intVal ==1) {
		if (method==0 && wep==WEP_DISABLED)
			secMode = 0;
		else
			secMode = 1;

		if ( !apmib_set( MIB_ADHOC_SEC_MODE, (void *)&secMode ) ) {
			DEBUGP(tmpBuf, T("Set MIB_ADHOC_SEC_MODE error!"));
			goto setErr_encrypt;
		}
	}
*/

	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	#ifdef HOME_GATEWAY
		//sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "gw", "all"); // Tommy
		//system(tmpBuf);
	#else
		//sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "ap", "all"); // Tommy
		//system(tmpBuf);
	#endif
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	OK_MSG(wp, submitUrl);

	return;

setErr_encrypt:
	ERR_MSG(wp, tmpBuf);
}
#endif


/////////////////////////////////////////////////////////////////////////////
#ifndef HOME_GATEWAY

#if 0
void formStaSec(webs_t wp, char_t *path, char_t *query)
{
   	char_t *wepKey;
   	char_t *strKeyLen, *strFormat, *strKeyId, *strEnabled;
	char key[30];
	int enabled, keyLen, ret, i;
	WEP_T wep;


   	char_t *submitUrl, *strEncrypt, *strVal;
	char tmpBuf[100];
	ENCRYPT_T encrypt;
	int enableRS=0, intVal, getPSK=0, len;




	//Set security method
   	strEncrypt = websGetVar(wp, T("method"), T(""));
	if (!strEncrypt[0]) {
 		DEBUGP(tmpBuf, T("Error! no encryption method."));
		goto setErr_encrypt;
	}
	encrypt = (ENCRYPT_T) strEncrypt[0] - '0';
	if (encrypt!=ENCRYPT_DISABLED && encrypt!=ENCRYPT_WEP && encrypt!=ENCRYPT_WPA &&
		encrypt!=ENCRYPT_WSK && encrypt!=ENCRYPT_PSK && encrypt!=ENCRYPT_ARS) {
		DEBUGP(tmpBuf, T("Invalid encryption method!"));
		goto setErr_encrypt;
	}
	if (apmib_set( MIB_STA_SEC_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("Set MIB_STA_SEC_MODE mib error!"));
		goto setErr_encrypt;
	}

	//Follow Realtek, Set WLAN_ENCRYPT
	if (encrypt==ENCRYPT_DISABLED || encrypt==ENCRYPT_WPA)
		intVal = 0;
	else if (encrypt==ENCRYPT_WEP || encrypt==ENCRYPT_WSK)
		intVal = 1;
	else
		intVal = 2;
	if (apmib_set( MIB_STA_ENCRYPT, (void *)&intVal) == 0) {
  		DEBUGP(tmpBuf, T("Set MIB_STA_ENCRYPT mib error!"));
		goto setErr_encrypt;
	}


   	strEncrypt = websGetVar(wp, T("method"), T(""));
	if (strEncrypt[0]=='0')
		intVal=0;
	else if (strEncrypt[0]=='1')
		intVal=1;
	else
		intVal=2;
	if (apmib_set( MIB_STA_ENCRYPT, (void *)&intVal) == 0) {
  		DEBUGP(tmpBuf, T("Set  mib error!"));
		goto setErr_encrypt;
	}





	// WPA authentication
	if (encrypt!=ENCRYPT_DISABLED && encrypt!=ENCRYPT_WEP) {
		strVal = websGetVar(wp, T("wpaAuth"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("eap"))) {
				intVal = WPA_AUTH_AUTO;
				enableRS = 1;
			}
			else if ( !gstrcmp(strVal, T("psk"))) {
				intVal = WPA_AUTH_PSK;
				getPSK = 1;
			}
			else {
				DEBUGP(tmpBuf, T("Error! Invalid wpa authentication value."));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_STA_WPA_AUTH, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("Set MIB_STA_AUTH_TYPE failed!"));
				goto setErr_encrypt;
			}
		}
	}

	// cipher suite
	if (encrypt==ENCRYPT_PSK || encrypt==ENCRYPT_ARS) {
		strVal = websGetVar(wp, T("wpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if ( apmib_set(MIB_STA_WPA_CIPHER_SUITE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set MIB_STA_WPA_CIPHER_SUITE failed!"));
			goto setErr_encrypt;
		}
/*		intVal = 0;
		strVal = websGetVar(wp, T("tkip"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal |= WPA_CIPHER_TKIP;
		strVal = websGetVar(wp, T("aes"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal |= WPA_CIPHER_AES;
		if (intVal == 0) {
			DEBUGP(tmpBuf, T("Invalid value of cipher suite!"));
			goto setErr_encrypt;
		}
		if ( apmib_set(MIB_STA_WPA_CIPHER_SUITE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set MIB_STA_WPA_CIPHER_SUITE failed!"));
			goto setErr_encrypt;
		}
*/	}

//*********************************** Wep setting ***********************************
	strEnabled = websGetVar(wp, T("wepEnabled"), T(""));
	if ( !gstrcmp(strEnabled, T("ON")))
		enabled = 1;
	else
		enabled = 0;

	if ( enabled ) {
		strKeyLen = websGetVar(wp, T("length"), T(""));
		if (!strKeyLen[0]) {
 			DEBUGP(tmpBuf, T("Key length must exist!"));
			goto setErr_encrypt;
		}
		if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 			DEBUGP(tmpBuf, T("Invalid key length value!"));
			goto setErr_encrypt;
		}
		if (strKeyLen[0] == '1')
			wep = WEP64;
		else if (strKeyLen[0] == '2')
			wep = WEP128;
	}
	else
		wep = WEP_DISABLED;

	if ( apmib_set( MIB_STA_WEP, (void *)&wep) == 0) {
  		DEBUGP(tmpBuf, T("Set STA WEP error!"));
		goto setErr_encrypt;
	}

	if (wep == WEP_DISABLED)
		goto setwep_ret;

	if (encrypt==ENCRYPT_WEP || encrypt==ENCRYPT_WSK) { //Erwin Modify 01.09.'03
		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
 			DEBUGP(tmpBuf, T("Key type must exist!"));
			goto setErr_encrypt;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("Invalid key type value!"));
			goto setErr_encrypt;
		}

		i = strFormat[0] - '0' - 1;
		if ( apmib_set( MIB_STA_WEP_KEY_TYPE, (void *)&i) == 0) {
  			DEBUGP(tmpBuf, T("Set WEP key type error!"));
			goto setErr_encrypt;
		}

		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;
			else
				keyLen = WEP128_KEY_LEN*2;
		}

		strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("Invalid default tx key id!"));
   				goto setErr_encrypt;
			}
			i = strKeyId[0] - '0' - 1;
			if ( !apmib_set( MIB_STA_WEP_DEFKEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("Set default tx key id error!"));
   				goto setErr_encrypt;
			}
		}

		wepKey = websGetVar(wp, T("key1"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("Invalid key 1 length!"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("Invalid wep-key1 value!"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY1, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("Set wep-key1 error!"));
					goto setErr_encrypt;
				}
			}
		}
		wepKey = websGetVar(wp, T("key2"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("Invalid key 2 length!"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("Invalid wep-key2 value!"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY2, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("Set wep-key2 error!"));
					goto setErr_encrypt;
				}
			}
		}

		wepKey = websGetVar(wp, T("key3"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("Invalid key 3 length!"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("Invalid wep-key3 value!"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY3, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("Set wep-key3 error!"));
					goto setErr_encrypt;
				}
			}
		}

		wepKey = websGetVar(wp, T("key4"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("Invalid key 1 length!"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("Invalid wep-key4 value!"));
   						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_STA_WEP64_KEY4, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_STA_WEP128_KEY4, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("Set wep-key4 error!"));
					goto setErr_encrypt;
				}
			}
		}
	}
//*********************************** pre-shared key setting ***********************************
	if ( getPSK ) {
		int oldFormat, oldPskLen, i;

		strVal = websGetVar(wp, T("pskFormat"), T(""));
		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("Error! no psk format."));
			goto setErr_encrypt;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("Error! invalid psk format."));
			goto setErr_encrypt;
		}

		// remember current psk format and length to compare to default case "****"
		apmib_get(MIB_STA_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_STA_WPA_PSK, (void *)tmpBuf);
		oldPskLen = strlen(tmpBuf);

		strVal = websGetVar(wp, T("pskValue"), T(""));
		len = strlen(strVal);

		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto rekey_time;
		}

		if ( apmib_set(MIB_STA_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set MIB_STA_WPA_PSK_FORMAT failed!"));
			goto setErr_encrypt;
		}

		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("Error! invalid psk value."));
				goto setErr_encrypt;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("Error! invalid psk value."));
				goto setErr_encrypt;
			}
		}
		if ( !apmib_set(MIB_STA_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("Set MIB_STA_WPA_PSK error!"));
			goto setErr_encrypt;
		}
	}
//****************************************  ****************************************
rekey_time:

		// get 802.1x WEP key length
		strVal = websGetVar(wp, T("wepKeyLen"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("wep64")))
				intVal = WEP64;
			else if ( !gstrcmp(strVal, T("wep128")))
				intVal = WEP128;
			else {
				DEBUGP(tmpBuf, T("Error! Invalid wepkeylen value."));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_STA_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("Set MIB_STA_WEP failed!"));
				goto setErr_encrypt;
			}
		}
//-----------------------------------------------------------------------------------------------
setwep_ret:
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	#ifdef HOME_GATEWAY
		sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "gw", "all");
		system(tmpBuf);
	#else
		sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "ap", "all");
		system(tmpBuf);
	#endif
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	OK_MSG(wp, submitUrl);

	return;

setErr_encrypt:
	ERR_MSG(wp, tmpBuf);
}
#endif
#endif

#ifdef WLAN_WPA
/////////////////////////////////////////////////////////////////////////////
void formWlEncrypt(webs_t wp, char_t *path, char_t *query)
{

   	char_t *wepKey;
   	char_t *strKeyLen, *strFormat, *strKeyId, *strEnabled, *strnextEnable, *strisApply;
	char key[30];
	int enabled, keyLen, ret, i;
	WEP_T wep;

#ifdef _WEP_OPEN_SHARED_MODE_
	char_t *strWepMode;
#endif
   	char_t *submitUrl, *strEncrypt, *strVal, *strEnRadius, *strWpaCipher, *str1xMode;
	char tmpBuf[100];
	ENCRYPT_T encrypt;
	int enableRS=0, intVal, getPSK=0, len, intTkip, intAes;
	unsigned long reKeyTime;
	SUPP_NONWAP_T suppNonWPA;
	struct in_addr inIp;
		//200703.29 kyle wps status
			intVal = 1;
			if ( apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_encrypt;
			}
			intVal = 0;
			if ( apmib_set(MIB_WPS_DISPLAY_KEY, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_encrypt;
			}
	//Set security method
   	strEncrypt = websGetVar(wp, T("method"), T(""));

//	printf("strEncrypt=%s\n");

	if (!strEncrypt[0]) {
 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt1))</script>"));
		goto setErr_encrypt;
	}
	encrypt = strEncrypt[0] - '0';

#ifdef _MSSID_

char *temp;
int ssidID;
temp=websGetVar(wp, T("selected_ssid"), T(""));
ssidID=(int)temp[0] - '0';
switch(ssidID)
{
	case 0: apmib_set( MIB_SECURITY_MODE, (void *)&encrypt); break;
	case 1: apmib_set( MIB_SECURITY_MODE_1, (void *)&encrypt); break;
	case 2: apmib_set( MIB_SECURITY_MODE_2, (void *)&encrypt); break;
	case 3: apmib_set( MIB_SECURITY_MODE_3, (void *)&encrypt); break;
	case 4: apmib_set( MIB_SECURITY_MODE_4, (void *)&encrypt); break;
	case 5: apmib_set( MIB_SECURITY_MODE_5, (void *)&encrypt); break;
	case 6: apmib_set( MIB_SECURITY_MODE_6, (void *)&encrypt); break;
	case 7: apmib_set( MIB_SECURITY_MODE_7, (void *)&encrypt); break;
	default: break;
}

#else
	if (apmib_set( MIB_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}
    #if defined (_Customer_) || defined (_Customer_)
	if (apmib_set( MIB_WLAN_WDS_ENCRYPT, (void *)&encrypt) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}
    #endif
#endif

	//Set 802.1x Enable
   	strEnRadius = websGetVar(wp, T("enRadius"), T(""));
	if ( ((encrypt==0 || encrypt==1) && !gstrcmp(strEnRadius, T("ON"))) || encrypt==3) {
		intVal = 1;
		enableRS = 1;
	}
	else
		intVal = 0;

#ifdef _MSSID_
switch(ssidID)
{
	case 0: apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal); break;
	case 1: apmib_set( MIB_WLAN_ENABLE_1X_1, (void *)&intVal); break;
	case 2: apmib_set( MIB_WLAN_ENABLE_1X_2, (void *)&intVal); break;
	case 3: apmib_set( MIB_WLAN_ENABLE_1X_3, (void *)&intVal); break;
	case 4: apmib_set( MIB_WLAN_ENABLE_1X_4, (void *)&intVal); break;
	case 5: apmib_set( MIB_WLAN_ENABLE_1X_5, (void *)&intVal); break;
	case 6: apmib_set( MIB_WLAN_ENABLE_1X_6, (void *)&intVal); break;
	case 7: apmib_set( MIB_WLAN_ENABLE_1X_7, (void *)&intVal); break;
	default: break;
}

#else
	if ( apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
		goto setErr_encrypt;
	}
#endif

#ifdef _Customer_
#ifdef _MSSID_ 
        strVal = websGetVar(wp, T("wlanWmm"), T(""));
        intVal = strVal[0] - '0';
switch(ssidID)
{
	case 0: apmib_set( MIB_WLAN_WMM, (void *)&intVal); break;
	case 1: apmib_set( MIB_WLAN_WMM_1, (void *)&intVal); break;
	case 2: apmib_set( MIB_WLAN_WMM_2, (void *)&intVal); break;
	case 3: apmib_set( MIB_WLAN_WMM_3, (void *)&intVal); break;
	case 4: apmib_set( MIB_WLAN_WMM_4, (void *)&intVal); break;
	case 5: apmib_set( MIB_WLAN_WMM_5, (void *)&intVal); break;
	case 6: apmib_set( MIB_WLAN_WMM_6, (void *)&intVal); break;
	case 7: apmib_set( MIB_WLAN_WMM_7, (void *)&intVal); break;
	default: break;
}
        strVal = websGetVar(wp, T("hiddenSSID"), T(""));
        intVal = strVal[0] - '0';
switch(ssidID)
{
	case 0: apmib_set( MIB_WLAN_HIDDEN_SSID, (void *)&intVal); break;
	case 1: apmib_set( MIB_WLAN_HIDDEN_SSID_1, (void *)&intVal); break;
	case 2: apmib_set( MIB_WLAN_HIDDEN_SSID_2, (void *)&intVal); break;
	case 3: apmib_set( MIB_WLAN_HIDDEN_SSID_3, (void *)&intVal); break;
	case 4: apmib_set( MIB_WLAN_HIDDEN_SSID_4, (void *)&intVal); break;
	case 5: apmib_set( MIB_WLAN_HIDDEN_SSID_5, (void *)&intVal); break;
	case 6: apmib_set( MIB_WLAN_HIDDEN_SSID_6, (void *)&intVal); break;
	case 7: apmib_set( MIB_WLAN_HIDDEN_SSID_7, (void *)&intVal); break;
	default: break;
}

#endif
#endif

#ifndef HOME_GATEWAY
	//set internal or external RADIUS
	str1xMode = websGetVar(wp, T("sel1xMode"), T(""));
	if (!gstrcmp(str1xMode, T("ON")))
		intVal = 1;
	else
		intVal = 0;

	if ( apmib_set(MIB_1X_MODE, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("Set 802.1x Mode failed!"));
		goto setErr_encrypt;
	}
#endif

	// support nonWPA client
	strVal = websGetVar(wp, T("nonWpaSupp"), T(""));
	if ( !gstrcmp(strVal, T("ON")))
		intVal = 1;
	else
		intVal = 0;
	if ( apmib_set( MIB_WLAN_ENABLE_SUPP_NONWPA, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt2))</script>"));
		goto setErr_encrypt;
	}
	if ( intVal ) {
		suppNonWPA = SUPP_NONWPA_NONE;
		strVal = websGetVar(wp, T("nonWpaWep"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			suppNonWPA |= SUPP_NONWPA_WEP;

		strVal = websGetVar(wp, T("nonWpa1x"), T(""));
		if ( !gstrcmp(strVal, T("ON"))) {
			suppNonWPA |= SUPP_NONWPA_1X;
			enableRS = 1;
		}
		if ( apmib_set( MIB_WLAN_SUPP_NONWPA, (void *)&suppNonWPA) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt3))</script>"));
			goto setErr_encrypt;
		}
	}

	if (encrypt==2 || encrypt==3) {
		// WPA authentication
		if (encrypt==2) {
			intVal = WPA_AUTH_PSK;
			getPSK = 1;
		}
		else if (encrypt==3) {
			intVal = WPA_AUTH_AUTO;
			enableRS = 1;
		}
		if ( apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
			goto setErr_encrypt;
		}

		// cipher suite
		strVal = websGetVar(wp, T("wpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if (strVal[0]) {
			if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
			if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;

#ifdef _MSSID_

switch(ssidID)
{
	case 0: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip);   apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);break;
	case 1: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_1, (void *)&intAes);break;
	case 2: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_2, (void *)&intAes);break;
	case 3: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_3, (void *)&intAes);break;
	case 4: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_4, (void *)&intAes);break;
	case 5: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_5, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_5, (void *)&intAes);break;
	case 6: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_6, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_6, (void *)&intAes);break;
	case 7: apmib_set( MIB_WLAN_WPA_CIPHER_SUITE_7, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_7, (void *)&intAes);break;
	default: break;
}

#else
			if ( apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
#if defined (_Customer_) || defined (_Customer_)
			if ( apmib_set(MIB_WLAN_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_WLAN_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
#endif
#endif
		}
	}

	//Follow Realtek, Set WLAN_ENCRYPT
	strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));
	if (strEncrypt[0]=='0')	encrypt=0;
	else if (strEncrypt[0]=='1') encrypt=1;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='1')) encrypt=2;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='2')) encrypt=4;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='3')) encrypt=6;
#ifdef _MSSID_
printf("wowowo444\n");
	switch(ssidID)
	{
		case 0:	apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt); break;
		case 1:	apmib_set( MIB_WLAN_ENCRYPT_1, (void *)&encrypt); break;
		case 2:	apmib_set( MIB_WLAN_ENCRYPT_2, (void *)&encrypt); break;
		case 3:	apmib_set( MIB_WLAN_ENCRYPT_3, (void *)&encrypt); break;
		case 4:	apmib_set( MIB_WLAN_ENCRYPT_4, (void *)&encrypt); break;
		case 5:	apmib_set( MIB_WLAN_ENCRYPT_5, (void *)&encrypt); break;
		case 6:	apmib_set( MIB_WLAN_ENCRYPT_6, (void *)&encrypt); break;
		case 7:	apmib_set( MIB_WLAN_ENCRYPT_7, (void *)&encrypt); break;
		default: break;
	}
#else
	if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
		goto setErr_encrypt;
	}
#endif
//*********************************** Wep setting ***********************************
	if (encrypt==ENCRYPT_WEP) {
		strEnabled = websGetVar(wp, T("wepEnabled"), T(""));
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( enabled ) {
			strKeyLen = websGetVar(wp, T("length"), T(""));
			if (!strKeyLen[0]) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt2))</script>"));
				goto setErr_encrypt;
			}
			if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt3))</script>"));
				goto setErr_encrypt;
			}
			if (strKeyLen[0] == '1')
				wep = WEP64;
			else if (strKeyLen[0] == '2')
				wep = WEP128;
		}
		else
			wep = WEP_DISABLED;

#ifdef _MSSID_

switch(ssidID)
{
	case 0:	apmib_set( MIB_WLAN_WEP, (void *)&wep);  break;
	case 1:	apmib_set( MIB_WLAN_WEP_1, (void *)&wep);break;
	case 2:	apmib_set( MIB_WLAN_WEP_2, (void *)&wep);break;
	case 3:	apmib_set( MIB_WLAN_WEP_3, (void *)&wep);break;
	case 4:	apmib_set( MIB_WLAN_WEP_4, (void *)&wep);break;
	case 5:	apmib_set( MIB_WLAN_WEP_5, (void *)&wep);break;
	case 6:	apmib_set( MIB_WLAN_WEP_6, (void *)&wep);break;
	case 7:	apmib_set( MIB_WLAN_WEP_7, (void *)&wep);break;
	default:break;
}
#else
		if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt5))</script>"));
			goto setErr_encrypt;
		}
#endif
		if (wep == WEP_DISABLED)
			goto setwep_ret;

		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt5))</script>"));
			goto setErr_encrypt;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt6))</script>"));
			goto setErr_encrypt;
		}

		i = strFormat[0] - '0' - 1;

#ifdef _MSSID_

switch(ssidID)
{
	case 0:	apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i);  break;
	case 1:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_1, (void *)&i);break;
	case 2:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_2, (void *)&i);break;
	case 3:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_3, (void *)&i);break;
	case 4:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_4, (void *)&i);break;
	case 5:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_5, (void *)&i);break;
	case 6:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_6, (void *)&i);break;
	case 7:	apmib_set( MIB_WLAN_WEP_KEY_TYPE_7, (void *)&i);break;
	default:break;
}
#else
		if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_encrypt;
		}
#endif
		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;
			else
				keyLen = WEP128_KEY_LEN*2;
		}
		strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt8))</script>"));
				goto setErr_encrypt;
			}
			i = strKeyId[0] - '0' - 1;

#ifdef _MSSID_
			switch(ssidID)
			{
				case 0:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i);  break;
				case 1:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_1, (void *)&i);break;
				case 2:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_2, (void *)&i);break;
				case 3:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_3, (void *)&i);break;
				case 4:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_4, (void *)&i);break;
				case 5:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_5, (void *)&i);break;
				case 6:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_6, (void *)&i);break;
				case 7:	apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_7, (void *)&i);break;
				default:break;
			}
#else
			if ( !apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_encrypt;
			}
#endif
		}




#ifdef _MSSID_

wepKey = websGetVar(wp, T("key1"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isAllStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else				string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP64_KEY1, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP64_KEY1_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP64_KEY1_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP64_KEY1_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP64_KEY1_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP64_KEY1_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP64_KEY1_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP64_KEY1_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP128_KEY1, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP128_KEY1_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP128_KEY1_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP128_KEY1_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP128_KEY1_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP128_KEY1_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP128_KEY1_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP128_KEY1_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key2"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isAllStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP64_KEY2, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP64_KEY2_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP64_KEY2_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP64_KEY2_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP64_KEY2_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP64_KEY2_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP64_KEY2_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP64_KEY2_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP128_KEY2, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP128_KEY2_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP128_KEY2_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP128_KEY2_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP128_KEY2_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP128_KEY2_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP128_KEY2_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP128_KEY2_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key3"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isAllStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP64_KEY3, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP64_KEY3_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP64_KEY3_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP64_KEY3_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP64_KEY3_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP64_KEY3_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP64_KEY3_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP64_KEY3_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP128_KEY3, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP128_KEY3_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP128_KEY3_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP128_KEY3_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP128_KEY3_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP128_KEY3_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP128_KEY3_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP128_KEY3_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key4"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isAllStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP64_KEY4, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP64_KEY4_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP64_KEY4_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP64_KEY4_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP64_KEY4_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP64_KEY4_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP64_KEY4_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP64_KEY4_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WEP128_KEY4, (void *)&key);	break;
			case 1: apmib_set(MIB_WLAN_WEP128_KEY4_1, (void *)&key);	break;
			case 2: apmib_set(MIB_WLAN_WEP128_KEY4_2, (void *)&key);	break;
			case 3: apmib_set(MIB_WLAN_WEP128_KEY4_3, (void *)&key);	break;
			case 4: apmib_set(MIB_WLAN_WEP128_KEY4_4, (void *)&key);	break;
			case 5: apmib_set(MIB_WLAN_WEP128_KEY4_5, (void *)&key);	break;
			case 6: apmib_set(MIB_WLAN_WEP128_KEY4_6, (void *)&key);	break;
			case 7: apmib_set(MIB_WLAN_WEP128_KEY4_7, (void *)&key);	break;
			default:break;
		}
}
#else
		wepKey = websGetVar(wp, T("key1"), T(""));
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		if( !strcmp(wepKey,"") )
		{
			strcpy(key, wepKey);
			if (wep == WEP64)
				ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
			if (!ret) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
				goto setErr_encrypt;
			}
		}
		else
		{
#endif
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
					goto setErr_encrypt;
				}
			}
		}
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		}
#endif

		wepKey = websGetVar(wp, T("key2"), T(""));
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		if( !strcmp(wepKey,"") )
		{
			strcpy(key, wepKey);
			if (wep == WEP64)
				ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_WLAN_WEP128_KEY2, (void *)key);
			if (!ret) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt15))</script>"));
				goto setErr_encrypt;
			}
		}
		else
		{
#endif
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt13))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt14))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY2, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt15))</script>"));
					goto setErr_encrypt;
				}
			}
		}
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		}
#endif

		wepKey = websGetVar(wp, T("key3"), T(""));
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		if( !strcmp(wepKey,"") )
		{
			strcpy(key, wepKey);
			if (wep == WEP64)
				ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_WLAN_WEP128_KEY3, (void *)key);
			if (!ret) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt18))</script>"));
				goto setErr_encrypt;
			}
		}
		else
		{
#endif
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt16))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt17))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY3, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt18))</script>"));
					goto setErr_encrypt;
				}
			}
		}
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		}
#endif


		wepKey = websGetVar(wp, T("key4"), T(""));

#if defined(_Sitecom330_) || defined(_Sitecom331_)
		if( !strcmp(wepKey,"") )
		{
			strcpy(key, wepKey);
			if (wep == WEP64)
				ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_WLAN_WEP128_KEY4, (void *)key);
			if (!ret) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt20))</script>"));
				goto setErr_encrypt;
			}
		}
		else
		{
#endif
		if(wepKey[0])
		{
			if (strlen(wepKey) != keyLen)
			{
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) )
			{
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else // hex
				{
					if ( !string_to_hex(wepKey, key, keyLen))
					{
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt19))</script>"));
						goto setErr_encrypt;
					}
				}

				if (wep == WEP64)        ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
				else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY4, (void *)key);

				if (!ret)
				{
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt20))</script>"));
					goto setErr_encrypt;
				}
			}
		}
#if defined(_Sitecom330_) || defined(_Sitecom331_)
		}
#endif

#endif
	}

//*********************************** pre-shared key setting ***********************************
	if ( getPSK ) {
		int oldFormat, oldPskLen, i;

		strVal = websGetVar(wp, T("pskFormat"), T(""));
		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt22))</script>"));
			goto setErr_encrypt;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt23))</script>"));
			goto setErr_encrypt;
		}

		// remember current psk format and length to compare to default case "****"
#ifdef _MSSID_
		//vance 2009.02.23 fixed pskvalue error
		switch(ssidID)
		{
			case 0:apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf);break;
			case 1:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_1, (void *)tmpBuf);break;
			case 2:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_2, (void *)tmpBuf);break;
			case 3:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_3, (void *)tmpBuf);break;
			case 4:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_4, (void *)tmpBuf);break;
			case 5:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_5, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_5, (void *)tmpBuf);break;
			case 6:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_6, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_6, (void *)tmpBuf);break;
			case 7:apmib_get(MIB_WLAN_WPA_PSK_FORMAT_7, (void *)&oldFormat);apmib_get(MIB_WLAN_WPA_PSK_7, (void *)tmpBuf);break;
			default:break;
		}
		oldPskLen = strlen(tmpBuf);

		strVal = websGetVar(wp, T("pskValue"), T(""));
		len = strlen(strVal);
			if (oldFormat == intVal && len == oldPskLen ) {
				for (i=0; i<len; i++) {
					if ( strVal[i] != '*' )
						break;
				}
				if (i == len)
					goto rekey_time;
			}
		switch(ssidID)
		{
			case 0: apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal);	break;
			case 1: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_1, (void *)strVal);	break;
			case 2: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_2, (void *)strVal);	break;
			case 3: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_3, (void *)strVal);	break;
			case 4: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_4, (void *)strVal);	break;
			case 5: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_5, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_5, (void *)strVal);	break;
			case 6: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_6, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_6, (void *)strVal);	break;
			case 7: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_7, (void *)&intVal);	apmib_set(MIB_WLAN_WPA_PSK_7, (void *)strVal);	break;
			default:break;
		}
#else
		apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf);
		oldPskLen = strlen(tmpBuf);

		strVal = websGetVar(wp, T("pskValue"), T(""));
		len = strlen(strVal);
		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto rekey_time;
		}
		if ( apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}
#if defined (_Customer_) || defined (_Customer_)
		if ( apmib_set(MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}
#endif
		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
        #if defined (_Customer_) || defined (_Customer_)
		if ( !apmib_set(MIB_WLAN_WDS_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
        #endif
#endif



	}
//****************************************  ****************************************
rekey_time:
		// group key rekey time
/*		reKeyTime = 0;
		strVal = websGetVar(wp, T("groupKeyTimeDay"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of rekey day."));
				goto setErr_encrypt;
			}
			reKeyTime += intVal*86400;
		}
		strVal = websGetVar(wp, T("groupKeyTimeHr"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of rekey hr."));
				goto setErr_encrypt;
			}
			reKeyTime += intVal*3600;
		}
		strVal = websGetVar(wp, T("groupKeyTimeMin"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of rekey min."));
				goto setErr_encrypt;
			}
			reKeyTime += intVal*60;
		}

		strVal = websGetVar(wp, T("groupKeyTimeSec"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of rekey sec."));
				goto setErr_encrypt;
			}
			reKeyTime += intVal;
		}
		if (reKeyTime) {
			if ( !apmib_set(MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&reKeyTime)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_WPA_GROUP_REKEY_TIME error!"));
				goto setErr_encrypt;
			}
		}
*/
//*********************************** RADIUS Server setting ***********************************
	if (enableRS == 1) { // if 1x enabled, get RADIUS server info
		#ifndef HOME_GATEWAY
		 if (!str1xMode[0]) {
		 #endif
		// set Radius Port
		strVal = websGetVar(wp, T("radiusPort"), T(""));
		if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt7))</script>"));
				goto setErr_encrypt;
			}
			if (!string_to_dec(strVal, &intVal) || intVal<=0 || intVal>65535) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt8))</script>"));
				goto setErr_encrypt;
			}

			if ( !apmib_set(MIB_WLAN_RS_PORT, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt9))</script>"));
				goto setErr_encrypt;
			}

		//Radius Ip
			strVal = websGetVar(wp, T("radiusIP"), T(""));
			if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !inet_aton(strVal, &inIp) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt11))</script>"));
				goto setErr_encrypt;
			}


			if ( !apmib_set(MIB_WLAN_RS_IP, (void *)&inIp)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt12))</script>"));
				goto setErr_encrypt;
			}

			//Radius password
			strVal = websGetVar(wp, T("radiusPass"), T(""));
			if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt13))</script>"));
				goto setErr_encrypt;
			}


			if ( !apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_encrypt;
			}

		#ifndef HOME_GATEWAY
		}
		#endif



/*
		strVal = websGetVar(wp, T("radiusRetry"), T("")); // differance "rsRetry"
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid RS retry value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_WLAN_RS_RETRY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_RS_RETRY error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("radiusTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid RS time value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_WLAN_RS_INTERVAL_TIME, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_RS_INTERVAL_TIME error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("useAccount"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_WLAN_ACCOUNT_RS_ENABLED, (void *)&intVal) == 0) {
  			DEBUGP(tmpBuf, T("Set MIB_WLAN_ACCOUNT_RS_ENABLED mib error!"));
			goto setErr_encrypt;
		}
		if (intVal == 0)
			goto get_wepkey;

		strVal = websGetVar(wp, T("accountPort"), T(""));
		if (!strVal[0]) {
			DEBUGP(tmpBuf, T("No account RS port number!"));
			goto setErr_encrypt;
		}
		if (!string_to_dec(strVal, &intVal) || intVal<=0 || intVal>65535) {
			DEBUGP(tmpBuf, T("Error! Invalid value of account RS port number."));
			goto setErr_encrypt;
		}
		if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_PORT, (void *)&intVal)) {
			DEBUGP(tmpBuf, T("Set account RS port error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountIP"), T(""));
		if (!strVal[0]) {
			DEBUGP(tmpBuf, T("No account RS IP address!"));
			goto setErr_encrypt;
		}
		if ( !inet_aton(strVal, &inIp) ) {
			DEBUGP(tmpBuf, T("Invalid account RS IP-address value!"));
			goto setErr_encrypt;
		}
		if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_IP, (void *)&inIp)) {
			DEBUGP(tmpBuf, T("Set account RS IP-address error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountPass"), T(""));
		if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
			DEBUGP(tmpBuf, T("Account RS password length too long!"));
			goto setErr_encrypt;
		}
		if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_PASSWORD, (void *)strVal)) {
			DEBUGP(tmpBuf, T("Set account RS password error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountRetry"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid account RS retry value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_RETRY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_ACCOUNT_RS_RETRY error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("accountTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid account RS time value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("accountUpdateEnabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_WLAN_ACCOUNT_UPDATE_ENABLED, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set MIB_WLAN_ACCOUNT_UPDATE_ENABLED mib error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountUpdateTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of update time"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_WLAN_ACCOUNT_UPDATE_DELAY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_WLAN_ACCOUNT_UPDATE_DELAY mib error!"));
				goto setErr_encrypt;
			}
		}
*/
get_wepkey:
		// get 802.1x WEP key length
		strVal = websGetVar(wp, T("wepKeyLen"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("wep64")))
				intVal = WEP64;
			else if ( !gstrcmp(strVal, T("wep128")))
				intVal = WEP128;
			else {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt15))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_WLAN_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_encrypt;
			}
		}
	}
//-----------------------------------------------------------------------------------------------
setwep_ret:
	apmib_update(CURRENT_SETTING);

#ifdef HOME_GATEWAY
	//sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "gw", "all"); // Tommy
	//system(tmpBuf);
#else
	//sprintf(tmpBuf, "%s %s %s", "/bin/init.sh", "ap", "all"); // Tommy
	//system(tmpBuf);
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif
        //strnextEnable = websGetVar(wp, T("redirect"), T(""));
	strisApply = websGetVar(wp,T("isApply"),T(""));
        //if ((strnextEnable[0] || wizardEnabled == 1) && !strisApply[0]){
        if ( wizardEnabled == 1 && !strisApply[0] ){
                REPLACE_MSG(submitUrl);
        }
        else{
#if defined(_Customer_)
                char_t *strnextEnable;
                strnextEnable = websGetVar(wp, T("new_apply"), T(""));
                if (strnextEnable[0])
                {
                        system("/bin/reset.sh gw 2");
                }
                REPLACE_MSG(submitUrl);
#else
                OK_MSG(wp, submitUrl);
#endif
        }
        return;
setErr_encrypt:
	ERR_MSG(wp, tmpBuf);
}
#endif
/////////////////////////////////////////////////////////////////////////////
int wlAcList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	MACFILTER_T entry;
	char tmpBuf[100];
	char macBuf[100];

	#ifdef __TARGET_BOARD__
	FILE *fp;
	char command[100],IPaddress[20],tmp[2];
	int ipCount=0;
	if( (fp=fopen("/tmp/auto_refresh_nbtscan", "r")) != NULL )
	    fgets(tmp,2,fp);
	fclose(fp);

	if( !strcmp(tmp, "1") )
	{
		system("cat /proc/net/arp | grep : | grep br0 | grep -v 00:00:00:00:00:00 | cut -d ' ' -f 1 > /tmp/log/br0_linkIP.log");
		FILE *fp=fopen("/tmp/log/br0_linkIP.log","r");
		while(1)
		{
			fscanf(fp,"%s",&IPaddress);
			if(feof(fp)) break;

			printf("Auto Refresh nbtscan for br0 ,IP:[%s]\n",IPaddress);

			if( ipCount == 0) 
			{
				sprintf(command,"/bin/nbtscan -a -v %s > /var/netName.var",IPaddress); system(command);
			}
			else
			{
				sprintf(command,"/bin/nbtscan -a -v %s >> /var/netName.var",IPaddress); system(command);
			}
			ipCount = ipCount + 1;
		}
		fclose(fp);
	}
	else
	{
		 system("echo 1 > /tmp/auto_refresh_nbtscan"); //EDX, Robert add, "0:Close 1:Run" auto refresh nbtscan.
	}
	#endif

	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
	if ( argv[0][0] == '1' ) {
		if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '2' ) {
		if ( !apmib_get(MIB_WLAN_AC2_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '3' ) {
		if ( !apmib_get(MIB_WLAN_AC3_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '4' ) {
		if ( !apmib_get(MIB_WLAN_AC4_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	#else
	if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum))
	{
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	#endif


	for (i=1; i<=entryNum; i++)
	{
		*((char *)&entry) = (char)i;

	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( argv[0][0] == '1' ) {
			if ( !apmib_get(MIB_WLAN_AC_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '2' ) {
			if ( !apmib_get(MIB_WLAN_AC2_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '3' ) {
			if ( !apmib_get(MIB_WLAN_AC3_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '4' ) {
			if ( !apmib_get(MIB_WLAN_AC4_ADDR, (void *)&entry)) return -1;
		}
	#else
		if ( !apmib_get(MIB_WLAN_AC_ADDR, (void *)&entry)) return -1;
	#endif
		snprintf(tmpBuf, 100, T("%02x:%02x:%02x:%02x:%02x:%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		snprintf(macBuf, 100, T("%02x%02x%02x%02x%02x%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);

		char tmpBuf1[100];
		snprintf(tmpBuf1, 100, T("%02x%02x%02x%02x%02x%02x"),entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		nBytesSent += websWrite(wp, T("\n<!-- ===================================================================== -->\n"));
		if(i%2) nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
		else nBytesSent += websWrite(wp, T("<tr style=\"background-color: #FFFFFF;\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"27%%\">%s</td>        \n"),tmpBuf);
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"27%%\">               \n"));
		nBytesSent += websWrite(wp, T("    <script>                                                            \n"));
		nBytesSent += websWrite(wp, T("      for(i=0;i<nameList.length-1;i++)                                  \n"));
		nBytesSent += websWrite(wp, T("      {                                                                 \n"));
		nBytesSent += websWrite(wp, T("        if(nameList[i][2]==\"%s\" && nameList[i][2]!=\"000000000000\")  \n"),tmpBuf1);
		nBytesSent += websWrite(wp, T("        {                                                               \n"));
		nBytesSent += websWrite(wp, T("          document.write(nameList[i][1]);                               \n"));
		nBytesSent += websWrite(wp, T("          break;                                                        \n"));
		nBytesSent += websWrite(wp, T("        }                                                               \n"));
		nBytesSent += websWrite(wp, T("      }                                                                 \n"));
		//nBytesSent += websWrite(wp, T("      if(i==nameList.length-1) document.write(\"OFFLINE\");             \n"));
		nBytesSent += websWrite(wp, T("      if(i==nameList.length-1) document.write(\"–\");                   \n"));
		nBytesSent += websWrite(wp, T("    </script>                                                           \n"));
		nBytesSent += websWrite(wp, T("  </td>                                                                 \n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"17%%\">               \n"));
		nBytesSent += websWrite(wp, T("    <script>                                                            \n"));
		nBytesSent += websWrite(wp, T("      for(i=0;i<nameList.length-1;i++)                                  \n"));
		nBytesSent += websWrite(wp, T("      {                                                                 \n"));
		nBytesSent += websWrite(wp, T("        if(nameList[i][2]==\"%s\" && nameList[i][2]!=\"000000000000\")  \n"),tmpBuf1);
		nBytesSent += websWrite(wp, T("        {                                                               \n"));
		nBytesSent += websWrite(wp, T("          document.write(nameList[i][0]);                               \n"));
		nBytesSent += websWrite(wp, T("          break;                                                        \n"));
		nBytesSent += websWrite(wp, T("        }                                                               \n"));
		nBytesSent += websWrite(wp, T("      }                                                                 \n"));
		//nBytesSent += websWrite(wp, T("      if(i==nameList.length-1) document.write(\"OFFLINE\");             \n"));
		nBytesSent += websWrite(wp, T("      if(i==nameList.length-1) document.write(\"–\");                   \n"));
		nBytesSent += websWrite(wp, T("    </script>                                                           \n"));
		nBytesSent += websWrite(wp, T("  </td>                                                                 \n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"19%%\">%s</td>        \n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"10%%\">               \n"));
		nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" name=\"select%d\" value=\"ON\">            \n"),i);
		nBytesSent += websWrite(wp, T("  </td>                                                                 \n"));
		nBytesSent += websWrite(wp, T("</tr>                                                                   \n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"Client_PC_macAddr%d\" name=\"Client_PC_macAddr%d\" value=\"%s\">\n"),i,i,tmpBuf1);
		nBytesSent += websWrite(wp, T("<!-- ===================================================================== -->\n"));

	}

	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" style=\"FONT-SIZE: 11pt; \">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"sipnum\" value=\"%d\">\n"),i-1);
	return nBytesSent;
}
#ifdef _WIFI_ROMAING_
int SlaveList(int eid, webs_t wp, int argc, char_t **argv)
{
	char ModelName[10] = {0}, IPValue[20] = {0},  DUTVersion[10] = {0}, ServerVersion[10] = {0}, DownloadStatus[15] = {0};
	char ModelNameMaster[10] = {0}, IPValueMaster[20] = {0}, DUTVersionMaster[10] = {0}, ServerVersionMaster[10] = {0}, StatusMaster[10] = {0}, DownloadStatusMaster[15] = {0};
	char cmdModel[100] = {0}, cmdIP[100] = {0}, cmdDUTVersion[100] = {0}, cmdServerVersion[100] = {0}, cmdDownloadStatus[100] = {0};
	char cmdModelMaster[100] = {0}, cmdIPMaster[100] = {0}, cmdDUTVersionMaster[100] = {0}, cmdServerVersionMaster[100] = {0}, cmdDownloadStatusMaster[100] = {0};
	char command[100], commandTwo[100], tmp[10] = {0}, tmpTwo[10] = {0};
	int SlaveRealNumber_int=0, i=0, nBytesSent=0, NewVer=0, NewVerMaster=0, MasterNum=0;
	float ServerVersion_float, DUTVersion_float, ServerVersionMaster_float, DUTVersionMaster_float;
	FILE *fp, *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7, *fp8, *fp9, *fp10;
	FILE *fpSlave, *fpMaster, *fpFW, *fpStatus, *fpStatusM;

#if 0
	if( (fp=fopen("/tmp/SlaveRealNumber", "r")) != NULL )
	{
		system("cat /tmp/SlaveRealNumber > /tmp/SlaveRealNumberBuffer");
		fgets(tmp,2,fp);
		fclose(fp);	
		SlaveRealNumber_int = atoi(tmp);
		if(SlaveRealNumber_int > 0)
		{
			system("cat /tmp/Slave_* > /tmp/Slave_Data");
			sleep(1);
			if( (fpFW=fopen("/tmp/Master_FW_Data", "r")) != NULL )
			{
				system("rm -rf /tmp/Master_Data");
				sleep(1);
				system("mv /tmp/Master_FW_Data /tmp/Master_Data");
				system(1);
				fclose(fpFW);
			}
			else
			{
				system("/bin/Master_Data.sh");
				sleep(1);
			}
		}
	}
	else
	{
		SlaveRealNumber_int = 0;
	}
#else
	if( (fp9=fopen("/tmp/DownloadFW", "r")) != NULL )
	{
		if( (fp10=fopen("/tmp/SlaveRealNumberBuffer", "r")) != NULL )
		{
			fgets(tmpTwo,2,fp10);
			fclose(fp10);
			SlaveRealNumber_int = atoi(tmpTwo);
//			if(SlaveRealNumber_int > 0)
//			{
//				printf("================================================\n");
//				system("cat /tmp/Slave_Data");
//				system("cat /tmp/Master_Data");
//				printf("================================================\n");
//				MasterNum=SlaveRealNumber_int+1;
//				sprintf(command,"head -n %d /tmp/Slave_Data |tail -n 1 > /tmp/Master_Data",MasterNum);
//				system(command);
//				sprintf(commandTwo,"head -n %d /tmp/Slave_Data |tail -n %d > /tmp/Slave_Data_Buffer",SlaveRealNumber_int, SlaveRealNumber_int);
//				system(commandTwo);
//				system("rm -rf /tmp/Slave_Data");
//				sleep(2);
//				system("cat /tmp/Slave_Data_Buffer > /tmp/Slave_Data");	
//			}
		}
		else
		{
			SlaveRealNumber_int = 0;
		}
	}
	else
	{
		if( (fp=fopen("/tmp/SlaveRealNumber", "r")) != NULL )
		{
			system("cat /tmp/SlaveRealNumber > /tmp/SlaveRealNumberBuffer");
			fgets(tmp,2,fp);
			fclose(fp);	
			SlaveRealNumber_int = atoi(tmp);
			if(SlaveRealNumber_int > 0)
			{
				system("cat /tmp/Slave_* > /tmp/Slave_Data");
				sleep(1);
				if( (fpFW=fopen("/tmp/Master_FW_Data", "r")) != NULL )
				{
					system("rm -rf /tmp/Master_Data");
					sleep(1);
					system("mv /tmp/Master_FW_Data /tmp/Master_Data");
					system(1);
					fclose(fpFW);
				}
				else
				{
					system("/bin/Master_Data.sh");
					sleep(1);
				}
			}
		}
		else
		{
			SlaveRealNumber_int = 0;
		}
	}
#endif

	for (i=1; i<=SlaveRealNumber_int; i++)
	{
                if((fpSlave = fopen("/tmp/Slave_Data","r"))!= NULL){
			sleep(1);
			fclose(fpSlave);
                        snprintf(cmdModel, sizeof(cmdModel), "head -n %d /tmp/Slave_Data |tail -n 1|cut -d ';' -f 1|cut -d = -f 2",i);
                        if((fp1 = popen(cmdModel, "r")) != NULL){
                                fgets(ModelName,sizeof(ModelName), fp1);
				fclose(fp1);
                                if (ModelName[strlen(ModelName)-1]== '\n')
                                        ModelName[strlen(ModelName)-1] = '\0';
                        }
			sleep(1);
                        snprintf(cmdIP, sizeof(cmdIP), "head -n %d /tmp/Slave_Data |tail -n 1|cut -d ';' -f 4|cut -d = -f 2",i);
                        if((fp2 = popen(cmdIP, "r")) != NULL){
                                fgets(IPValue,sizeof(IPValue), fp2);
				fclose(fp2);
                                if (IPValue[strlen(IPValue)-1]== '\n')
                                        IPValue[strlen(IPValue)-1] = '\0';
                        }
			sleep(1);
                        snprintf(cmdDUTVersion, sizeof(cmdDUTVersion), "head -n %d /tmp/Slave_Data |tail -n 1|cut -d ';' -f 5|cut -d = -f 2",i);
                        if((fp3 = popen(cmdDUTVersion, "r")) != NULL){
                                fgets(DUTVersion,sizeof(DUTVersion), fp3);
				fclose(fp3);
                                if (DUTVersion[strlen(DUTVersion)-1]== '\n')
                                        DUTVersion[strlen(DUTVersion)-1] = '\0';
                        }
			sleep(1);
                        snprintf(cmdServerVersion, sizeof(cmdServerVersion), "head -n %d /tmp/Slave_Data |tail -n 1|cut -d ';' -f 6|cut -d = -f 2",i);
                        if((fp4 = popen(cmdServerVersion, "r")) != NULL){
                                fgets(ServerVersion,sizeof(ServerVersion), fp4);
				fclose(fp4);
                                if (ServerVersion[strlen(ServerVersion)-1]== '\n')
                                        ServerVersion[strlen(ServerVersion)-1] = '\0';
                        }
			sleep(1);
                        snprintf(cmdDownloadStatus, sizeof(cmdDownloadStatus), "head -n %d /tmp/Slave_Data |tail -n 1|cut -d ';' -f 7|cut -d = -f 2",i);
                        if((fpStatus = popen(cmdDownloadStatus, "r")) != NULL){
                                fgets(DownloadStatus,sizeof(DownloadStatus), fpStatus);
				fclose(fpStatus);
                                if (DownloadStatus[strlen(DownloadStatus)-1]== '\n')
				{
                                        DownloadStatus[strlen(DownloadStatus)-1] = '\0';
				}
                        }
                }
                else
                {
                        ModelName[10] = 0;
			IPValue[20] = 0;
			DUTVersion[10] = 0;
			ServerVersion[10] = 0;
			DownloadStatus[15] = 0;
                }
		
		ServerVersion_float = atof(ServerVersion);
		DUTVersion_float = atof(DUTVersion);
		if( ServerVersion_float > DUTVersion_float )
		{
			NewVer = 1; //ServerVer New than DUTVersion
		}
		else if( ServerVersion_float < DUTVersion_float )
		{
			NewVer = 2; //DUTVersion New than ServerVer
		}
		else if( ServerVersion_float = DUTVersion_float )
		{
			NewVer = 3; //ServerVer the Same DUTVersion
		}
		else
		{
			NewVer = 4;
		}
		
		nBytesSent += websWrite(wp, 
		T("\n<!-- ===================================================================== -->\n"
		"<tr style=\"background-color: #F0F0F0;\">\n"
		"<td align=center class=\"dnsTableText\" width=\"20%%\">%s</td>\n"
		"<td align=center class=\"dnsTableText\" width=\"20%%\">%s</td>\n"
		"<td align=center class=\"dnsTableText\" width=\"15%%\">%s</td>\n"
		"<td align=center class=\"dnsTableText\" width=\"15%%\">%s</td>\n"),
		ModelName,IPValue,DUTVersion,ServerVersion);

		if (NewVer == 1)
		{
			nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script>dw(VersionStatus1)</script></td>\n"));
		}
		else if (NewVer == 2 || NewVer == 3 )
		{
			nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script>dw(VersionStatus2)</script></td>\n"));
		}
		else
		{
			nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script></script></td>\n"));
		}

		nBytesSent += websWrite(wp, 
		T("</tr>\n"
		"<input type=\"hidden\" name=\"Slave%d\" value=\"%d\">\n"
		"<input type=\"hidden\" name=\"Slave_FWdownload%d\" value=\"%s\">\n"
		"<!-- ===================================================================== -->\n"),
		i,NewVer,i,DownloadStatus
		);

		if(i==SlaveRealNumber_int) //for Master
		{
		        if((fpMaster = fopen("/tmp/Master_Data","r"))!= NULL){
				sleep(1);
				fclose(fpMaster);
		                snprintf(cmdModelMaster, sizeof(cmdModelMaster), "head -n 1 /tmp/Master_Data |tail -n 1|cut -d ';' -f 1|cut -d = -f 2");
		                if((fp5 = popen(cmdModelMaster, "r")) != NULL){
		                        fgets(ModelNameMaster,sizeof(ModelNameMaster), fp5);
					fclose(fp5);
		                        if (ModelNameMaster[strlen(ModelNameMaster)-1]== '\n')
		                                ModelNameMaster[strlen(ModelNameMaster)-1] = '\0';
		                }
				sleep(1);
		                snprintf(cmdIPMaster, sizeof(cmdIPMaster), "head -n 1 /tmp/Master_Data |tail -n 1|cut -d ';' -f 4|cut -d = -f 2");
		                if((fp6 = popen(cmdIPMaster, "r")) != NULL){
		                        fgets(IPValueMaster,sizeof(IPValueMaster), fp6);
					fclose(fp6);
		                        if (IPValueMaster[strlen(IPValueMaster)-1]== '\n')
		                                IPValueMaster[strlen(IPValueMaster)-1] = '\0';
		                }
				sleep(1);
		                snprintf(cmdDUTVersionMaster, sizeof(cmdDUTVersionMaster), "head -n 1 /tmp/Master_Data |tail -n 1|cut -d ';' -f 5|cut -d = -f 2");
		                if((fp7 = popen(cmdDUTVersionMaster, "r")) != NULL){
		                        fgets(DUTVersionMaster,sizeof(DUTVersionMaster), fp7);
					fclose(fp7);
		                        if (DUTVersionMaster[strlen(DUTVersionMaster)-1]== '\n')
		                                DUTVersionMaster[strlen(DUTVersionMaster)-1] = '\0';
		                }
				sleep(1);
		                snprintf(cmdServerVersionMaster, sizeof(cmdServerVersionMaster), "head -n 1 /tmp/Master_Data |tail -n 1|cut -d ';' -f 6|cut -d = -f 2");
		                if((fp8 = popen(cmdServerVersionMaster, "r")) != NULL){
		                        fgets(ServerVersionMaster,sizeof(ServerVersionMaster), fp8);
					fclose(fp8);
		                        if (ServerVersionMaster[strlen(ServerVersionMaster)-1]== '\n')
		                                ServerVersionMaster[strlen(ServerVersionMaster)-1] = '\0';
		                }
				sleep(1);
		                snprintf(cmdDownloadStatusMaster, sizeof(cmdDownloadStatusMaster), "head -n 1 /tmp/Master_Data |tail -n 1|cut -d ';' -f 7|cut -d = -f 2");
		                if((fpStatusM = popen(cmdDownloadStatusMaster, "r")) != NULL){
		                        fgets(DownloadStatusMaster,sizeof(DownloadStatusMaster), fpStatusM);
					fclose(fpStatusM);
		                        if (DownloadStatusMaster[strlen(DownloadStatusMaster)-1]== '\n')
					{
		                                DownloadStatusMaster[strlen(DownloadStatusMaster)-1] = '\0';
					}
		                }
		        }
		        else
		        {
		                ModelNameMaster[10] = 0;
				IPValueMaster[20] = 0;
				DUTVersionMaster[10] = 0;
				ServerVersionMaster[10] = 0;
				DownloadStatusMaster[15] = 0;
		        }

			ServerVersionMaster_float = atof(ServerVersionMaster);
			DUTVersionMaster_float = atof(DUTVersionMaster);
			if( ServerVersionMaster_float > DUTVersionMaster_float )
			{
				NewVerMaster = 1; //ServerVer New than DUTVersion
			}
			else if( ServerVersionMaster_float < DUTVersionMaster_float )
			{
				NewVerMaster = 2; //DUTVersion New than ServerVer
			}
			else if( ServerVersionMaster_float = DUTVersionMaster_float )
			{
				NewVerMaster = 3; //ServerVer the Same DUTVersion
			}
			else
			{
				NewVerMaster = 4;
			}

			nBytesSent += websWrite(wp, 
			T("\n<!-- ===================================================================== -->\n"
			"<tr style=\"background-color: #F0F0F0;\">\n"
			"<td align=center class=\"dnsTableText\" width=\"20%%\">%s</td>\n"
			"<td align=center class=\"dnsTableText\" width=\"20%%\">%s</td>\n"
			"<td align=center class=\"dnsTableText\" width=\"15%%\">%s</td>\n"
			"<td align=center class=\"dnsTableText\" width=\"15%%\">%s</td>\n"),
			ModelNameMaster,IPValueMaster,DUTVersionMaster,ServerVersionMaster);

			if (NewVerMaster == 1)
			{
				nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script>dw(VersionStatus1)</script></td>\n"));
			}
			else if (NewVerMaster == 2 || NewVerMaster == 3 )
			{
				nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script>dw(VersionStatus2)</script></td>\n"));
			}
			else
			{
				nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"30%%\"><script></script></td>\n"));
			}

			nBytesSent += websWrite(wp, 
			T("</tr>\n"
			"<input type=\"hidden\" name=\"Master\" value=\"%d\">\n"
			"<input type=\"hidden\" name=\"Master_FWdownload\" value=\"%s\">\n"
			"<!-- ===================================================================== -->\n"),
			NewVerMaster,DownloadStatusMaster
			);
		}
	}

	if (NewVerMaster != 1 || NewVer != 1)
	{
		system("echo 0 > /tmp/GoToUpgrade");
	}
	else
	{
		system("echo 1 > /tmp/GoToUpgrade");
	}

	if(SlaveRealNumber_int==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" style=\"FONT-SIZE: 11pt; \">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	system("rm -rf `ls /tmp/Slave_* |grep -v Slave_Data`");
	return nBytesSent;
}
#endif
/////////////////////////////////////////////////////////////////////////////
void formWlAc(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddMac, *strDelMac, *strDelAllMac, *strVal, *submitUrl, *strEnabled, *strApply;
	char tmpBuf[100];
	int entryNum, i, enabled;
	MACFILTER_T macEntry;

	strAddMac = websGetVar(wp, T("addFilterMac"), T(""));
	strDelMac = websGetVar(wp, T("deleteSelFilterMac"), T(""));
	strDelAllMac = websGetVar(wp, T("deleteAllFilterMac"), T(""));
	strEnabled = websGetVar(wp, T("wlanAcEnabled"), T(""));

	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		char_t *strIndex;
		strIndex = websGetVar(wp, T("wlanSSIDIndex"), T(""));
	#endif
	#if defined(_WLAN_AC_SWITCH_)
		int wlanacval;
		char_t *wlanacstr,*wlanacstr1,*wlanacstr2,*wlanacstr3;

		wlanacstr =websGetVar(wp, T("accessctrlval"), T(""));
		if(wlanacstr[0])
		{
			wlanacval = atoi(wlanacstr);
			apmib_set( MIB_WLAN_AC_SWITCH, (void *)&wlanacval);
		}
		wlanacstr1 =websGetVar(wp, T("accessctrlval1"), T(""));
		if(wlanacstr1[0])
		{
			wlanacval = atoi(wlanacstr1);
			apmib_set( MIB_WLAN_AC1_SWITCH, (void *)&wlanacval);
		}
		wlanacstr2 =websGetVar(wp, T("accessctrlval2"), T(""));
		if(wlanacstr2[0])
		{
			wlanacval = atoi(wlanacstr2);
			apmib_set( MIB_WLAN_AC2_SWITCH, (void *)&wlanacval);
		}
		wlanacstr3 =websGetVar(wp, T("accessctrlval3"), T(""));
		if(wlanacstr3[0])
		{
			wlanacval = atoi(wlanacstr3);
			apmib_set( MIB_WLAN_AC3_SWITCH, (void *)&wlanacval);
		}

	#endif
	//printf("strIndex=%c\n",strIndex[0]);
	if (strAddMac[0]) {
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;


	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( apmib_set( MIB_WLAN_AC_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( apmib_set( MIB_WLAN_AC2_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( apmib_set( MIB_WLAN_AC3_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( apmib_set( MIB_WLAN_AC4_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( apmib_set( MIB_WLAN_AC_ENABLED, (void *)&enabled) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
			goto setErr_ac;
		}
	#endif


		strVal = websGetVar(wp, T("mac"), T(""));
		if ( !strVal[0] ) {
//			DEBUGP(tmpBuf, T("Error! No mac address to set."));
			goto setac_ret;
		}
		if (strlen(strVal)!=12 || !string_to_hex(strVal, macEntry.macAddr, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc2))</script>"));
			goto setErr_ac;
		}

		strVal = websGetVar(wp, T("comment"), T(""));
		if ( strVal[0] ) {
			if (strlen(strVal) > COMMENT_LEN-1) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc3))</script>"));
				goto setErr_ac;
			}
			strcpy(macEntry.comment, CharFilter3(strVal));
		}
		else
			macEntry.comment[0] = '\0';


	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_get(MIB_WLAN_AC2_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_get(MIB_WLAN_AC3_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_get(MIB_WLAN_AC4_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}

	#else
		if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_ac;
		}
	#endif

		if ( (entryNum + 1) > MAX_WLAN_AC_NUM) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc5))</script>"));
			goto setErr_ac;
		}

		// set to MIB. try to delete it first to avoid duplicate case
	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			apmib_set(MIB_WLAN_AC2_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_WLAN_AC2_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			apmib_set(MIB_WLAN_AC3_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_WLAN_AC3_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			apmib_set(MIB_WLAN_AC4_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_WLAN_AC4_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
	#else
		apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry);
		if ( apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
			goto setErr_ac;
		}
	#endif


	}

	/* Delete entry */
	if (strDelMac[0]) {


	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_get(MIB_WLAN_AC2_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_get(MIB_WLAN_AC3_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_get(MIB_WLAN_AC4_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( !apmib_get(MIB_WLAN_AC_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_ac;
		}
	#endif


		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)&macEntry) = (char)i;


	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
				if ( strIndex[0] == '1' ) {
					if ( !apmib_get(MIB_WLAN_AC_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '2' ) {
					if ( !apmib_get(MIB_WLAN_AC2_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_WLAN_AC2_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '3' ) {
					if ( !apmib_get(MIB_WLAN_AC3_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_WLAN_AC3_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '4' ) {
					if ( !apmib_get(MIB_WLAN_AC4_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_WLAN_AC4_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
	#else
				if ( !apmib_get(MIB_WLAN_AC_ADDR, (void *)&macEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
					goto setErr_ac;
				}
				if ( !apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&macEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
					goto setErr_ac;
				}
	#endif


			}
		}
	}

	/* Delete all entry */
	if ( strDelAllMac[0]) {


	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_set(MIB_WLAN_AC2_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_set(MIB_WLAN_AC3_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_set(MIB_WLAN_AC4_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( !apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
			goto setErr_ac;
		}
	#endif


	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

setac_ret:
	apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;
			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}


  return;

setErr_ac:
	ERR_MSG(wp, tmpBuf);
}
///////////////////////////////////////////////////////////////////////////////////////////
#if defined(HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
void formTimeZoneSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strTimeZoneSel, *strTimeServerAddr, *strDisabled, *strtimezoneEnable;
	char_t *strstartMonth, *strstartDay, *strendMonth, *strendDay, *iqsetupclose;

	char_t *strServerSel;
	char_t *strServerAuto;
	int select, select1; 
	int val, disabled;

	struct in_addr inTimeServerAddr;
	char tmpBuf[100];


	strTimeZoneSel = websGetVar(wp, T("TimeZoneSel"), T(""));
	if (strTimeZoneSel[0]) {
		if ( !string_to_dec(strTimeZoneSel, &val) || val<0 || val>66) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup1))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_TIME_ZONE_SEL, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup2))</script>"));
			goto setErr_advance;
		}
	}

#if defined(_TIMESERVER_)   //EDX patrick add
	strTimeServerAddr = websGetVar(wp, T("NTPServerName"), T("")); //select domain name form list
	if ( strTimeServerAddr[0] ) {
		printf("-->NTPServer Domain Name:%s\n",strTimeServerAddr);
		apmib_set( MIB_NTPS_ADDR, (void *)strTimeServerAddr);        //sntp.sh will used ping to get domain name ip
	}
	strServerSel = websGetVar(wp, T("ServerSel"), T(""));
	if ( strTimeServerAddr[0] ) {
		string_to_dec(strServerSel, &val);
		apmib_set(MIB_SERVER_SEL, (void *)&val);
	}
#endif

//***********************************************************************************

	strDisabled = websGetVar(wp, T("DayLightEnable"), T(""));
	if ( !gstrcmp(strDisabled, T("ON")))
		disabled = 1;
	else
		disabled = 0;
	if ( apmib_set( MIB_DAYLIGHT_ENABLE, (void *)&disabled) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
		goto setErr_advance;
	}
	strstartMonth = websGetVar(wp, T("startMonth"), T(""));
	if (strstartMonth[0]) {
		if ( !string_to_dec(strstartMonth, &val)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup5))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_START_MONTH, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup6))</script>"));
			goto setErr_advance;
		}
	}
	strstartDay = websGetVar(wp, T("startDay"), T(""));
	if (strstartDay[0]) {
		if ( !string_to_dec(strstartDay, &val)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup7))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_START_DAY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup8))</script>"));
			goto setErr_advance;
		}
	}
	strendMonth = websGetVar(wp, T("endMonth"), T(""));
	if (strendMonth[0]) {
		if ( !string_to_dec(strendMonth, &val)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup9))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_END_MONTH, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup10))</script>"));
			goto setErr_advance;
		}
	}
	strendDay = websGetVar(wp, T("endDay"), T(""));
	if (strendDay[0]) {
		if ( !string_to_dec(strendDay, &val)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup11))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_END_DAY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTimeZoneSetup12))</script>"));
			goto setErr_advance;
		}
	}

	#ifdef _IQv2_
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif
	
	val=1;
	if ( apmib_set(MIB_NTPS_CONFIG, (void *)&val) == 0) {
		DEBUGP(tmpBuf, T("set MIB_NTPS_CONFIG error!"));
		goto setErr_advance;
	}

	apmib_update(CURRENT_SETTING);

	#ifndef NO_ACTION
	system("/bin/sntp.sh");
	#endif

	char_t *strApply;
	char tmpBufapply[200];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strtimezoneEnable = websGetVar(wp, T("settimezoneEnable"), T(""));

	if (strtimezoneEnable[0]){
		system("echo 1 > /tmp/show_reboot_info");
		REPLACE_MSG(submitUrl);
	} 
	else {
		strApply = websGetVar(wp, T("isApply"),T(""));
		if(strApply[0]){
			if (lanipChanged == 1) {
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBuf, newurl);
			}
			else
			{			
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			system("echo 1 > /tmp/show_reboot_info");
			REDIRECT_PAGE(wp, submitUrl);
		}
	}

	return;


setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef HOME_GATEWAY

/////////////////////////////////////////////////////////////////////////////
void formReManagementSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strreManHostAddr, *strreMangEnable, *strPort;
	int disabled, val;
	struct in_addr inreManHostAddr;
	char tmpBuf[100];

		strreManHostAddr = websGetVar(wp, T("reManHostAddr"), T(""));
		if ( strreManHostAddr[0] ) {
			if ( !inet_aton(strreManHostAddr, &inreManHostAddr) ) {
				DEBUGP(tmpBuf, T("Invalid IP-address value!"));
				goto setErr_advance;
			}
			if ( !apmib_set( MIB_REMANHOST_ADDR, (void *)&inreManHostAddr)) {
				DEBUGP(tmpBuf, T("set IP-address fail!"));
				goto setErr_advance;
			}
		}
	strPort = websGetVar(wp, T("reManPort"), T(""));
	if (strPort[0]) {
		if ( !string_to_dec(strPort, &val)) {
			DEBUGP(tmpBuf, T("Invalid Port value!"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_REMAN_PORT, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("set Port fail!"));
			goto setErr_advance;
		}
	}
	strreMangEnable = websGetVar(wp, T("reMangEnable"), T(""));
	if ( !gstrcmp(strreMangEnable, T("ON")))
		disabled = 1;
	else
		disabled = 0;
	if ( apmib_set( MIB_REMANG_ENABLE, (void *)&disabled) == 0) {
  		DEBUGP(tmpBuf, T("set Enable fail!"));
		goto setErr_advance;
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	char_t *strApply;
	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}

  return;



setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
#endif
/////////////////////////////////////////////////////////////////////////////
void formAdvanceSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strAuth, *strFragTh, *strRtsTh, *strBeacon, *strPreamble, *strBlock , *strCrossBand;
	char_t *strRate, *strHiddenSSID, *strDtim, *strIapp, *strTx, *strProtection;
	char_t *strTxPower, *strWmm, *strTurbo, *strNFixRate, *strChanWidth, *strNoForward;
	AUTH_TYPE_T authType;
	int val, ARepeaterON, GRepeaterON, intOne=1, intZero=0;

	char tmpBuf[100];
	PREAMBLE_T preamble;

#ifdef _IGMP_SNOOPING_ENABLED_
	char_t *strIGMPsnoop;
#endif

#ifdef _MCAST_RATE_SUPPORT_
	char_t *mcastindex;
#endif


#ifdef _LAN_WAN_ACCESS_
	int blValue=0;
	char_t *strBlockLan;
	strBlockLan = websGetVar(wp, T("blockLan"), T(""));

	if (strBlockLan[0]) apmib_get( MIB_LAN_WAN_ACCESS, (void *)&blValue);

	if ( !gstrcmp(strBlockLan, T("yes"))){
		blValue|=1;
	}
	else{
		blValue=blValue& (~1);
	}
	apmib_set(MIB_LAN_WAN_ACCESS, (void *)&blValue);

#endif
	char_t *strCts;

	strAuth = websGetVar(wp, T("authType"), T(""));
	if (strAuth[0]) {
		if ( !gstrcmp(strAuth, T("open")))
			authType = AUTH_OPEN;
		else if ( !gstrcmp(strAuth, T("shared")))
			authType = AUTH_SHARED;
		else if ( !gstrcmp(strAuth, T("both")))
			authType = AUTH_BOTH;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup1))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup12))</script>"));
			goto setErr_advance;
		}
	}
	strFragTh = websGetVar(wp, T("fragThreshold"), T(""));
	if (strFragTh[0]) {
		if ( !string_to_dec(strFragTh, &val) || val<256 || val>2346) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup2))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_FRAG_THRESHOLD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup3))</script>"));
			goto setErr_advance;
		}
	}
	strRtsTh = websGetVar(wp, T("rtsThreshold"), T(""));
	if (strRtsTh[0]) {
		if ( !string_to_dec(strRtsTh, &val) || val<0 || val>2347) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup4))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_RTS_THRESHOLD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup5))</script>"));
			goto setErr_advance;
		}
	}

	strBeacon = websGetVar(wp, T("beaconInterval"), T(""));
	if (strBeacon[0]) {
		if ( !string_to_dec(strBeacon, &val) || val<20 || val>1024) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup6))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_BEACON_INTERVAL, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup7))</script>"));
			goto setErr_advance;
		}
	}


	strTxPower = websGetVar(wp, T("wlanTxPower"), T(""));	
	printf("wlanTxPower=%s\n",strTxPower);
	if (strTxPower[0]) {
		if ( !string_to_dec(strTxPower, &val) || val >100 || val<0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup8))</script>"));
			goto setErr_advance;
		}
		//val = val % 100; // 0:100%
		if ( apmib_set(MIB_WLAN_TX_POWER, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup9))</script>"));
			goto setErr_advance;
		}
	}
	
	strTxPower = websGetVar(wp, T("wlanTxPower5g"), T(""));	
	printf("wlanTxPower5g=%s\n",strTxPower);
	if (strTxPower[0]) {
		if ( !string_to_dec(strTxPower, &val) || val >100 || val<0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup8))</script>"));
			goto setErr_advance;
		}
		//val = val % 100; // 0:100%
		if ( apmib_set(MIB_INIC_TX_POWER, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup9))</script>"));
			goto setErr_advance;
		}
	}

	strCrossBand = websGetVar(wp, T("CrossBand_enable"), T(""));
	if(strCrossBand[0]){
		printf("strCrossBand=%s\n",strCrossBand);
		if (!gstrcmp(strCrossBand, T("yes"))) val = 1;
		else val = 0;

		apmib_get( MIB_REPEATER_ENABLED, (void *)&GRepeaterON);
		apmib_get( MIB_INIC_REPEATER_ENABLED, (void *)&ARepeaterON);
		if(!val){ // cross band off = 2.4G(client) <-> 2.4G(ap),  5G(client) <-> 5G(ap) 
			printf("cross band off >> 2.4G(client)-2.4G(ap),  5G(client)-5G(ap)\n");
			if(GRepeaterON && ARepeaterON){
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
			}
			else if(GRepeaterON){
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
				apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
			}
			else{
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);
			}
		}
		else{ // cross band ON = 5G(client) <-> 2.4G(ap),  2.4G(client) <-> 5G(ap) 
			printf("cross band on >> 5G(client)-2.4G(ap),  2.4G(client)-5G(ap) \n");
			if(GRepeaterON && ARepeaterON){
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
			}
			else if(GRepeaterON){
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);
			}
			else{
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
				apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
			}
		}
	
		#if defined(_CROSSBAND_)
		if ( apmib_set(MIB_CROSSBAND_ENABLE, (void *)&val) == 0)
			goto setErr_advance;
		#endif
	}

	//kyle
		strNoForward = websGetVar(wp, T("noForwarding"), T(""));	//no use
	if (strNoForward[0]) {
		if (!gstrcmp(strNoForward, T("no")))
			val = 0;
		else if (!gstrcmp(strNoForward, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup10))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_NOFORWARD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_advance;
		}
	}

	strTurbo = websGetVar(wp, T("wlanTurbo"), T(""));	//no use
	if (strTurbo[0]) {
		if (!gstrcmp(strTurbo, T("no")))
			val = 0;
		else if (!gstrcmp(strTurbo, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup10))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_TURBO, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_advance;
		}
	}

	strWmm = websGetVar(wp, T("wlanWmm"), T(""));	//no use
	if (strWmm[0]) {
		if (!gstrcmp(strWmm, T("yes")))
			val = 1;
		else if (!gstrcmp(strWmm, T("no")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup12))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_WMM, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
#ifdef _NetIndex_
		if ( apmib_set(MIB_WLAN_WMM_1, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
#endif
	}
#ifdef _IGMP_SNOOPING_ENABLED_
	strIGMPsnoop = websGetVar(wp, T("igmpsnoop"), T(""));	//no use
	if (strIGMPsnoop[0]) {
		if (!gstrcmp(strIGMPsnoop, T("no")))
			val = 1;
		else if (!gstrcmp(strIGMPsnoop, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup12))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_IGMP_SNOOP_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
	}
#endif
	strChanWidth = websGetVar(wp, T("wlanNChanWidth"), T(""));	//no use
	if (strChanWidth[0]) {
		if (!gstrcmp(strChanWidth, T("0")))
			val = 0;
		else if (!gstrcmp(strChanWidth, T("1")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_N_CHANNEL_WIDTH, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("Invalid Channel Width 1"));
			goto setErr_advance;
		}
	}


	strNFixRate = websGetVar(wp, T("NtxRate"), T(""));
    if ( strNFixRate[0] ) {
		val = atoi(strNFixRate);
        if ( apmib_set(MIB_WLAN_N_FIX_RATE, (void *)&val) == 0) {
        	strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup16))</script>"));
            goto setErr_advance;
		}
	}

	// set tx rate
	strRate = websGetVar(wp, T("txRate"), T(""));
	if ( strRate[0] ) {

		if ( strRate[0] == '0' ) { // auto
			val = 1;
			if ( apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup14))</script>"));
				goto setErr_advance;
			}
			val = atoi(strRate);


			if ( apmib_set(MIB_WLAN_FIX_RATE, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup15))</script>"));
				goto setErr_advance;
			}
		}
		else
		{
			val = 0;
			if ( apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup14))</script>"));
				goto setErr_advance;
			}

			val = atoi(strRate);

	//		val = 1 << (val-1);

			if ( apmib_set(MIB_WLAN_FIX_RATE, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup15))</script>"));
				goto setErr_advance;
			}



			strRate = websGetVar(wp, T("basicrates"), T(""));
			if ( strRate[0] ) {
				val = atoi(strRate);
				if ( apmib_set(MIB_WLAN_BASIC_RATE, (void *)&val) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup16))</script>"));
					goto setErr_advance;
				}
			}

			strRate = websGetVar(wp, T("operrates"), T(""));
			if ( strRate[0] ) {
				val = atoi(strRate);
				if ( apmib_set(MIB_WLAN_SUPPORTED_RATE, (void *)&val) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup17))</script>"));
					goto setErr_advance;
				}
			}
		}
	}

	val = 0;
	strRate = websGetVar(wp, T("operRate1M"), T(""));
	if (strRate==NULL || strRate[0]==0)
		goto skip_rate_setting;
	if ( !gstrcmp(strRate, T("1M")))
		val |= TX_RATE_1M;
	strRate = websGetVar(wp, T("operRate2M"), T(""));
	if ( !gstrcmp(strRate, T("2M")))
		val |= TX_RATE_2M;
	strRate = websGetVar(wp, T("operRate5M"), T(""));
	if ( !gstrcmp(strRate, T("5M")))
		val |= TX_RATE_5M;
	strRate = websGetVar(wp, T("operRate11M"), T(""));
	if ( !gstrcmp(strRate, T("11M")))
		val |= TX_RATE_11M;
	strRate = websGetVar(wp, T("operRate6M"), T(""));
	if ( !gstrcmp(strRate, T("6M")))
		val |= TX_RATE_6M;
	strRate = websGetVar(wp, T("operRate9M"), T(""));
	if ( !gstrcmp(strRate, T("9M")))
		val |= TX_RATE_9M;
	strRate = websGetVar(wp, T("operRate12M"), T(""));
	if ( !gstrcmp(strRate, T("12M")))
		val |= TX_RATE_12M;
	strRate = websGetVar(wp, T("operRate18M"), T(""));
	if ( !gstrcmp(strRate, T("18M")))
		val |= TX_RATE_18M;
	strRate = websGetVar(wp, T("operRate24M"), T(""));
	if ( !gstrcmp(strRate, T("24M")))
		val |= TX_RATE_24M;
	strRate = websGetVar(wp, T("operRate36M"), T(""));
	if ( !gstrcmp(strRate, T("36M")))
		val |= TX_RATE_36M;
	strRate = websGetVar(wp, T("operRate48M"), T(""));
	if ( !gstrcmp(strRate, T("48M")))
		val |= TX_RATE_48M;
	strRate = websGetVar(wp, T("operRate54M"), T(""));
	if ( !gstrcmp(strRate, T("54M")))
		val |= TX_RATE_54M;
	if ( apmib_set(MIB_WLAN_SUPPORTED_RATE, (void *)&val) == 0) {
		strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup17))</script>"));
		goto setErr_advance;
	}

	// set basic tx rate
	val = 0;
	strRate = websGetVar(wp, T("basicRate1M"), T(""));
	if (strRate==NULL || strRate[0]==0)
		goto skip_rate_setting;
	if ( !gstrcmp(strRate, T("1M")))
		val |= TX_RATE_1M;
	strRate = websGetVar(wp, T("basicRate2M"), T(""));
	if ( !gstrcmp(strRate, T("2M")))
		val |= TX_RATE_2M;
	strRate = websGetVar(wp, T("basicRate5M"), T(""));
	if ( !gstrcmp(strRate, T("5M")))
		val |= TX_RATE_5M;
	strRate = websGetVar(wp, T("basicRate11M"), T(""));
	if ( !gstrcmp(strRate, T("11M")))
		val |= TX_RATE_11M;
	strRate = websGetVar(wp, T("basicRate6M"), T(""));
	if ( !gstrcmp(strRate, T("6M")))
		val |= TX_RATE_6M;
	strRate = websGetVar(wp, T("basicRate9M"), T(""));
	if ( !gstrcmp(strRate, T("9M")))
		val |= TX_RATE_9M;
	strRate = websGetVar(wp, T("basicRate12M"), T(""));
	if ( !gstrcmp(strRate, T("12M")))
		val |= TX_RATE_12M;
	strRate = websGetVar(wp, T("basicRate18M"), T(""));
	if ( !gstrcmp(strRate, T("18M")))
		val |= TX_RATE_18M;
	strRate = websGetVar(wp, T("basicRate24M"), T(""));
	if ( !gstrcmp(strRate, T("24M")))
		val |= TX_RATE_24M;
	strRate = websGetVar(wp, T("basicRate36M"), T(""));
	if ( !gstrcmp(strRate, T("36M")))
		val |= TX_RATE_36M;
	strRate = websGetVar(wp, T("basicRate48M"), T(""));
	if ( !gstrcmp(strRate, T("48M")))
		val |= TX_RATE_48M;
	strRate = websGetVar(wp, T("basicRate54M"), T(""));
	if ( !gstrcmp(strRate, T("54M")))
		val |= TX_RATE_54M;
	if ( apmib_set(MIB_WLAN_BASIC_RATE, (void *)&val) == 0) {
		strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup16))</script>"));
		goto setErr_advance;
	}
skip_rate_setting:




/*
	// set tx rate
	strRate = websGetVar(wp, T("txRate"), T(""));
	if ( strRate[0] ) {
		if ( strRate[0] == '0' ) { // auto
			val = 1;
			if ( apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set rate adaptive failed!"));
				goto setErr_advance;
			}
		}
		else  {
			val = 0;
			if ( apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set rate adaptive failed!"));
				goto setErr_advance;
			}

			if (strRate[0] == '1' )  // 11M
				val = TX_RATE_11M;
			else if (strRate[0] == '2' ) // 5.5M
				val = TX_RATE_5M;
			else if (strRate[0] == '3' ) // 2M
				val = TX_RATE_2M;
			else if (strRate[0] == '4' ) // 1M
				val = TX_RATE_1M;
			else {
				DEBUGP(tmpBuf, T("invalud value of tx rate!"));
				goto setErr_advance;
	else if ( !strcmp(name, T("hiddenSSID"))) {
		if ( !apmib_get( MIB_WLAN_HIDDEN_SSID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
			}
			if ( apmib_set(MIB_WLAN_SUPPORTED_RATE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set Tx operation rate failed!"));
				goto setErr_advance;
			}
			if ( apmib_set(MIB_WLAN_BASIC_RATE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set Tx basic rate failed!"));
				goto setErr_advance;
			}
		}
	}
	else { // set rate in operate, basic sperately
		val = 0;
		// disable rate adaptive
		if ( apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("Set rate adaptive failed!"));
			goto setErr_advance;
			}

		strRate = websGetVar(wp, T("operRate1M"), T(""));
		if ( !gstrcmp(strRate, T("1M")))
			val |= TX_RATE_1M;
		strRate = websGetVar(wp, T("operRate2M"), T(""));
		if ( !gstrcmp(strRate, T("2M")))
			val |= TX_RATE_2M;
		strRate = websGetVar(wp, T("operRate5M"), T(""));
		if ( !gstrcmp(strRate, T("5M")))
			val |= TX_RATE_5M;
		strRate = websGetVar(wp, T("operRate11M"), T(""));
		if ( !gstrcmp(strRate, T("11M")))
			val |= TX_RATE_11M;
		if ( apmib_set(MIB_WLAN_SUPPORTED_RATE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("Set Tx operation rate failed!"));
			goto setErr_advance;
		}

		// set basic tx rate
		val = 0;
		strRate = websGetVar(wp, T("basicRate1M"), T(""));
		if ( !gstrcmp(strRate, T("1M")))
			val |= TX_RATE_1M;
		strRate = websGetVar(wp, T("basicRate2M"), T(""));
		if ( !gstrcmp(strRate, T("2M")))
			val |= TX_RATE_2M;
		strRate = websGetVar(wp, T("basicRate5M"), T(""));
		if ( !gstrcmp(strRate, T("5M")))
			val |= TX_RATE_5M;
		strRate = websGetVar(wp, T("basicRate11M"), T(""));
		if ( !gstrcmp(strRate, T("11M")))
			val |= TX_RATE_11M;
		if ( apmib_set(MIB_WLAN_BASIC_RATE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("Set Tx basic rate failed!"));
			goto setErr_advance;
		}
	}
*/
	// set preamble
	strPreamble = websGetVar(wp, T("preamble"), T(""));
	if (strPreamble[0]) {
		if (!gstrcmp(strPreamble, T("long")))
			preamble = LONG_PREAMBLE;
		else if (!gstrcmp(strPreamble, T("short")))
			preamble = SHORT_PREAMBLE;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup18))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_PREAMBLE_TYPE, (void *)&preamble) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup19))</script>"));
			goto setErr_advance;
		}
	}

#ifdef _MCAST_RATE_SUPPORT_
	mcastindex = websGetVar(wp, T("multicastrate"), T(""));	//no use
	printf("mcastindex = %s \n" , mcastindex);
	if (mcastindex[0]) {
		/*if ( !string_to_dec(mcastindex, &val) || val >21 || val<0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup8))</script>"));
			goto setErr_advance;
		}*/
		if ( apmib_set(MIB_MCAST_RATE, (void *)mcastindex) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup9))</script>"));
			goto setErr_advance;
		}
	}
#endif
	// set hidden SSID
	strHiddenSSID = websGetVar(wp, T("hiddenSSID"), T(""));
	if (strHiddenSSID[0]) {
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup20))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_advance;
		}
	}

	strDtim = websGetVar(wp, T("dtimPeriod"), T(""));
	if (strDtim[0]) {
		if ( !string_to_dec(strDtim, &val) || val<1 || val>255) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup21))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_DTIM_PERIOD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup22))</script>"));
			goto setErr_advance;
		}
	}
	strIapp = websGetVar(wp, T("iapp"), T(""));
	if (strIapp[0]) {
		if (!gstrcmp(strIapp, T("no")))
			val = 1;
		else if (!gstrcmp(strIapp, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup23))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_IAPP_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup24))</script>"));
			goto setErr_advance;
		}
	}

	strBlock = websGetVar(wp, T("block"), T(""));
	if (strBlock[0]) {
		if (!gstrcmp(strBlock, T("no")))
			val = 0;
		else if (!gstrcmp(strBlock, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup25))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_BLOCK_RELAY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup26))</script>"));
			goto setErr_advance;
		}
	}

	// set CTS Protection
	strCts = websGetVar(wp, T("wlanCts"), T(""));
	if (strCts[0]) {
		if (!gstrcmp(strCts, T("auto")))
			val = 0;
		else if (!gstrcmp(strCts, T("always")))
			val = 1;
		else if (!gstrcmp(strCts, T("none")))
			val = 2;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup34))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_CTS, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup35))</script>"));
			goto setErr_advance;
		}
	}

	strProtection= websGetVar(wp, T("disProtection"), T(""));
	if (strProtection[0]) {
		if (!gstrcmp(strProtection, T("no")))
			val = 1;
		else if (!gstrcmp(strProtection, T("yes")))
			val = 0;
		else {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup36))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_WLAN_PROTECTION_DISABLED, (void *)&val) == 0) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup37))</script>"));
			goto setErr_advance;
		}
	}

#ifdef _WATCH_DOG_
	char_t *watchdog_enable;
	struct in_addr watchdog_host;


	watchdog_enable= websGetVar(wp, T("watchdog_enable"), T(""));
	if ( !gstrcmp(watchdog_enable, T("ON")))
		val = 1;
	else
		val = 0;
	apmib_set(MIB_WATCH_DOG_ENABLE, (void *)&val);

	if(val==1)
	{
		val= atoi((char_t *)websGetVar(wp, T("watchdog_interval"), T("1")));;
		apmib_set(MIB_WATCH_DOG_INTERVAL, (void *)&val);

		inet_aton(websGetVar(wp, T("watchdog_host"), T("0.0.0.0")), &watchdog_host);
		apmib_set(MIB_WATCH_DOG_HOST, (void *)&watchdog_host);
	}
#endif

#ifdef _AP_WLAN_NOFORWARD_
	char_t *strwlanforward;
	strwlanforward = websGetVar(wp, T("wlanForward"), T(""));
	if ( !gstrcmp(strwlanforward, T("yes")))
		val=1;
	else
		val=0;

	apmib_set(MIB_WLAN_NOFORWARD, (void *)&val);
#endif
#ifdef _MCAST_RATE_SUPPORT_
{
	char *strMcastRate;
	strMcastRate = websGetVar (wp, T("McastRate"), T(""));
	if (strMcastRate[0]) {
		val = atoi (strMcastRate);
		apmib_set (MIB_MCAST_RATE, (void *)&val);
	}
}
#endif // _MCAST_RATE_SUPPORT_

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);



	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	char tmpBufapply[200];
	char_t *strApply;
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			REBOOT_MSG(wp, tmpBufapply ,submitUrl);
		}
		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 3 &");
		#endif
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}
	return;

setErr_advance:
	ERR_MSG(wp, tmpBuf);
}

/////////////////////////////////////////////////////////////////////////////
int wirelessClientList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, found=0;
	WLAN_STA_INFO_T *pInfo;
	char buff[WEBS_BUFSIZE], interface[8];

	#ifdef __TARGET_BOARD__
	if (getWlStaInfo(WLAN_IF,  (WLAN_STA_INFO_Tp)buff ) < 0 ) {
		printf("<script>document.write(showText(aspWirelessClientList2))</script>!\n");
		return 0;
	}
	pInfo = (WLAN_STA_INFO_T *)buff;

	//printf("2.4G ClientList Num:%d\n",pInfo->Num);

	for (i=0; i<pInfo->Num ; i++){
		if (pInfo->Entry[i].Aid){


			nBytesSent += websWrite(wp,
	   		T("<tr class=\"listtableText\" align=\"center\"><td>%02x:%02x:%02x:%02x:%02x:%02x</td>"
			"<td>MCS%d</td>"
			"<td>%s</td>"
			"<td>%s</td>"
			"</tr>"),

			pInfo->Entry[i].Addr[0],pInfo->Entry[i].Addr[1],pInfo->Entry[i].Addr[2],pInfo->Entry[i].Addr[3],pInfo->Entry[i].Addr[4],pInfo->Entry[i].Addr[5]
			,pInfo->Entry[i].TxRate.field.MCS
			,GetBW(pInfo->Entry[i].TxRate.field.BW)
			,((pInfo->Entry[i].Psm) ? "ON" : "OFF")
			);
			found++;
		}
	}

	#endif

	if (found == 0) {
		nBytesSent += websWrite(wp,
		T("<tr class=\"listtableText\" align=\"center\"><td>---</td>"
		"<td>---</td>"
		"<td>---</td>"
		"<td>---</td>"
		"</tr>"));
	}

	return nBytesSent;
}
void formWirelessTbl(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));
	if (submitUrl[0])
		websRedirect(wp, submitUrl);
	else{
		submitUrl = websGetVar(wp, T("vapurl"), T(""));
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
	}

}
/////////////////////////////////////////////////////////////////////////////
void formWlSiteSurvey(webs_t wp, char_t *path, char_t *query)
{
 	char_t *submitUrl, *refresh, *connect, *done, *strSel;
 	char_t *strSSID, *strChan;
	int status, idx, encrypt;
	unsigned char res, *pMsg=NULL;
	int apEnable=1, len;
	char tmpBuf[100], data[16384];
	tmpBuf[0]='\0';
	int slength=0 ,wait_time, nBytesSent=0;
	char_t *strVal, *wlidx;	

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	done = websGetVar(wp, T("done"), T(""));
	if ( done[0] ) {
		strSSID = websGetVar(wp, T("selSSID"), T(""));
		strChan = websGetVar(wp, T("selChan"), T(""));
		printf("siteform ssid=%s\n",strSSID);
		if( strSSID[0]){

			int chan=0;
			CharFilter2(strSSID);
			if ( apmib_set(MIB_REPEATER_SSID, (void *)strSSID) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlSiteSurvey13))</script>"));
				goto ss_err;
			}
			printf("siteform ssid=%s\n",strSSID);


			chan=strtol(strChan, (char**)NULL, 10);
			printf("siteform channel=%d\n",chan);
			if ( apmib_set( MIB_WLAN_CHAN_NUM, (void *)&chan) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
				goto ss_err;
			}
			 apmib_set(MIB_AP_ROUTER_SWITCH, (void *)&apEnable);


		}

	}

	strVal = websGetVar(wp, T("ifname"), T(""));
	if(strVal[0])
		sprintf(WLAN_IF,"%s",strVal);

	refresh = websGetVar(wp, T("refresh"), T(""));
	if ( refresh[0] ) {

		//sprintf(tmpBuf, "iwpriv wlan1 set_mib ssid2scan=");
		//system(tmpBuf);
		//issue scan request

		wait_time = 0;
		memset(&data, 0, sizeof(data));

		if (getWlSiteSurveyRequest(WLAN_IF,  &data) < 0)
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspFormWlSiteSurvey1))</script>"));
			return nBytesSent;
		}

		while (1)
		{
			if (wait_time++ > 5)
			{
				break;
			}
			sleep(1);
		}

		pStatus->number = 0;

		if ((len = getWlSiteSurveyResult(WLAN_IF, &data, sizeof(data))) < 0)
		{
			nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
			free(pStatus);
			pStatus = NULL;
			return nBytesSent;
		}

		sitesurveyed = 1;
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		return;
	}

	connect = websGetVar(wp, T("connect"), T(""));
	if ( connect[0] ) {
		printf("======connect not ready======\n");
	}
	return;

ss_err:
	ERR_MSG(wp, tmpBuf);
}
void dump_hex(char *p, int len)
{
        int n = 0;

        while (len--){
                n++ ;
                printf("%02x%s",*p++ & 0xff,len ? ( (n & 7) ? " " : "-") : "") ;
                if ((n % 16) == 0)
                printf ("\n%x  ", n);
        }
        printf ("\n");
}
//////
int wlSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, wait_time=0 ,j,z, kakaso, apCount, len, ap_mode, RelApCount=0, IQfinish=0, intOne=0;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[16384],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];
	int GoSiteSurvey=0;
/*
	system("ifconfig apcli0 down");
	sleep(1);
	system("killall udhcpc");	
*/
	apmib_get(MIB_WIZ_MODE, (void *)&intOne);
	apmib_get(MIB_IQSET_DISABLE, (void *)&IQfinish); // fixed Bridge Mode iQSetup Wizard sitesurvey will show 5G/2.4G Interface Disable!!
	if (IQfinish == 1)
	{
		if (intOne == 3)
		{
			sleep(3);
		}
	}

	if (pStatus==NULL)
	{
		pStatus = calloc(1, sizeof(SS_STATUS_T));
		if ( pStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}

#ifdef __TARGET_BOARD__
	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(WLAN_IF,  &data) < 0)
	{
		nBytesSent = websWrite(wp, T("<script>document.write(showText(aspFormWlSiteSurvey1GBAND))</script>"));
		return nBytesSent;
	}

	while (1)
	{
		if (wait_time++ > 5)
		{
			break;
		}
		sleep(1);
	}

	pStatus->number = 0;

	if ((len = getWlSiteSurveyResult(WLAN_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(pStatus);
		pStatus = NULL;
		return nBytesSent;
	}

#endif
	printf("========== Get Site Survey AP List ==========\n");
	// kernel file: cmm_info.c
	if(sizeof(data) > 0){
#ifdef __TARGET_BOARD__
		int slength=0, j=0;
		int single_length, extern_length;
		char buf[50];
		FILE *fp;
		int length;
		int type_decided=0;
		struct site_survey_t{
			char channel[4];
			char ssid[33];
			char bssid[20];
			char security[23];
			char signal[9];
			char rssi[6];
			char mode[7];
			char extch[7];
			char type[3];
			char wps[4];
			char dpid[5];
		}site_survey, *psite_survey;

		apCount = i = 0;
		op = sp = &data;

		single_length = 112;	// assume the sitesurvey table at least have NT label
		extern_length = 0;

		op += single_length;

		while (*(op + extern_length) != '\n')
		{
			extern_length ++;
			if (extern_length > 9)
			{
				break;
			}
		}
		single_length += extern_length;		// + \n
		sp += single_length + 1;
		len -= single_length + 1;

		printf("total len = %d single_length = %d \n%s", len, single_length, sp);
		// santy check
		// 1. valid char data
		// 2. rest length is larger than per line length ==> (1+8+8+35+19+8+1)
		//    Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType
		while (len >= single_length)
		{

			memset (&SiteSurvey[i], 0x00, sizeof(SiteSurvey[i]));
			psite_survey = (struct site_survey_t *) sp;


		
			memcpy (SiteSurvey[i].ssid, psite_survey->ssid, 32);
			memcpy (SiteSurvey[i].channel, psite_survey->channel, 2);
		
			sscanf (psite_survey->bssid, "%02x:%02x:%02x:%02x:%02x:%02x", (int *)&SiteSurvey[i].bssid[0], (int *)&SiteSurvey[i].bssid[1],
										      (int *)&SiteSurvey[i].bssid[2], (int *)&SiteSurvey[i].bssid[3],
 										      (int *)&SiteSurvey[i].bssid[4], (int *)&SiteSurvey[i].bssid[5]);
			memcpy (SiteSurvey[i].signal, psite_survey->signal, 3);
			memcpy (SiteSurvey[i].rssi, psite_survey->rssi, 3);
			memcpy (SiteSurvey[i].wirelessmode, psite_survey->mode, 8);
			memcpy (SiteSurvey[i].networktype, psite_survey->type, 2);

			psite_survey->security[sizeof(psite_survey->security) - 1] = 0x00;
			psite_survey->ssid[sizeof(psite_survey->ssid) - 1] = 0x00;
			for (z = 33; z > 0; z--)
			{
				if(isspace(SiteSurvey[i].ssid[z]) || SiteSurvey[i].ssid[z]=='\0')
					SiteSurvey[i].ssid[z] = '\0';
				else
					break;
			}

			if (strstr (psite_survey->security, "NONE")){
				strcpy (SiteSurvey[i].webstatus, "NONE");
				strcpy (SiteSurvey[i].authmode, "OPEN");

			}else if (strstr (psite_survey->security, "WEP")){
				strcpy(SiteSurvey[i].webstatus, "WEP");
				strcpy (SiteSurvey[i].authmode, "OPEN");

			}else if (strstr (psite_survey->security, "WPAPSK/TKIP")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPAPSK/AES") || strstr (psite_survey->security, "WPAPSK/TKIPAES")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIP") || strstr (psite_survey->security, "WPA2PSK/TKIP")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");
			}else{
				strcpy (SiteSurvey[i].webstatus, "NONE");
				strcpy (SiteSurvey[i].authmode, "OPEN");
			}

			memcpy (SiteSurvey[i].extch, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].extch, " NONE "))
				strcpy (SiteSurvey[i].extch, "0");
			else if (strstr (SiteSurvey[i].extch, " ABOVE"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, " BELOW"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");
			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY SiteSurveyBUF[100]={0};
		int x,y;
		for (x=0; x<i; x++) {
			for (y=x;y<i;y++){
				if (atoi(SiteSurvey[y].signal) > atoi(SiteSurvey[x].signal)) {
					SiteSurveyBUF[x] = SiteSurvey[y];
					SiteSurvey[y] = SiteSurvey[x];
					SiteSurvey[x] = SiteSurveyBUF[x];
				}
			}
		}
		#endif

		apCount = i;
		pStatus->number = i;
		RelApCount = apCount;
		

			for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
			{
				pStatus->bssdb[i] = SiteSurvey[i];
    				sprintf(tmpBuf,"%s",pStatus->bssdb[i].ssid);
				int GbandZeroPower = atoi(pStatus->bssdb[i].signal);

				if(strlen(tmpBuf)>1 && GbandZeroPower != 0)	//EDX yihong, fixed if SSID is one string, system cannot pop in web UI. 
				{
					GoSiteSurvey=1;
				}
				else if(strlen(tmpBuf) == 1 && GbandZeroPower != 0){
					if(isspace(tmpBuf[0]))
					{
						GoSiteSurvey=0;
					}
					else
					{
						GoSiteSurvey=1;
					}
				}
				else
				{
					GoSiteSurvey=0;
				}
				
				//printf("##### %d strlen=%d\n",i, strlen(tmpBuf));
				char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
				int PowerGband = atoi(pStatus->bssdb[i].signal);
				int PowerUpGband = ScanListSignalScaleMapping92CD(PowerGband);
#endif
//				if(strlen(tmpBuf)>1){
				if(GoSiteSurvey == 1){
					nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
					nBytesSent += websWrite(wp, T("<tr style=\"background-color:#BBCCDE;\" align=\"center\"><td align=center width=\"10%%\" class=\"\"><input type=\"radio\" name=\"select%d\" id=\"select%d\" value=\"sel%d\" onClick=\"connectClick('sel%d')\"></td>\n"),i,i,i,i);  //Select
					nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"72%%\">\n"));

					nBytesSent += websWrite(wp, T("        <font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
					nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
					nBytesSent += websWrite(wp, T("					<script>document.forms['formWlSiteSurvey'].elements['ssid'+%d].value=\""),i);
					nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
					nBytesSent += websWrite(wp, T("\"     </script>\n"));
					nBytesSent += websWrite(wp, T("    </td>\n"));
#if defined(_SIGNAL_FORMULA_)
					if(PowerUpGband <= 40)
						nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font color=\"#DF0101\" size=\"2\">%d%%</font>"),PowerUpGband);  //Signal
					else
						nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">%d%%</font>"),PowerUpGband);  //Signal
					nBytesSent += websWrite(wp, T("    </td>\n"));
					nBytesSent += websWrite(wp, T("</tr>\n"));
					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpGband); 
#else
					if(atoi(pStatus->bssdb[i].signal) <= 40)
						nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font color=\"#DF0101\" size=\"2\">%s%%</font>"),pStatus->bssdb[i].signal);  //Signal
					else
						nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">%s%%</font>"),pStatus->bssdb[i].signal);  //Signal
					nBytesSent += websWrite(wp, T("    </td>\n"));
					nBytesSent += websWrite(wp, T("</tr>\n"));
					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].signal); 
#endif
					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, pStatus->bssdb[i].channel); //Channel
					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"\">\n"),i);  //encryption
					nBytesSent += websWrite(wp, T("	<script>document.forms['formWlSiteSurvey'].elements['encryption'+%d].value=\"%s\""),i ,pStatus->bssdb[i].authmode);
					nBytesSent += websWrite(wp, T("	</script>\n"));

					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"\">\n"),i);
					nBytesSent += websWrite(wp, T("	<script>document.forms['formWlSiteSurvey'].elements['ciphersutie'+%d].value=\"%s\""),i ,pStatus->bssdb[i].webstatus);
					nBytesSent += websWrite(wp, T("	</script>\n"));


					nBytesSent += websWrite(wp, T("<tr>\n"));
					nBytesSent += websWrite(wp, T("       <td style=\"display:none\" align=center class=\"table2\">\n"));
			
					nBytesSent += websWrite(wp, T("<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
					nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);
					nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,pStatus->bssdb[i].bssid[0],pStatus->bssdb[i].bssid[1],pStatus->bssdb[i].bssid[2],pStatus->bssdb[i].bssid[3],pStatus->bssdb[i].bssid[4],pStatus->bssdb[i].bssid[5]); 
					websWrite(wp, T("       </td>\n"));
					websWrite(wp, T("</tr>\n"));
				}
				else{
					RelApCount -= 1;
				}
			}

			if(apCount < 6){
				int add_tr_count;
				add_tr_count = 6 - apCount;
				for(i = 0 ; i < add_tr_count ; i++)
				{
					nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
					nBytesSent += websWrite(wp, T("<tr style=\"background-color:#FFFFFF;\" align=\"center\">\n"));
					nBytesSent += websWrite(wp, T("<td colspan=\"3\" >&nbsp;</td></tr>\n"));
				}
			}
			nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">\n"),apCount);
#else
		//for debug
		apCount=2;
		nBytesSent += websWrite(wp, T("<input type='hidden' name='apCount' value='%d'>\n"),apCount);
		for(i = 0 ; i < apCount ; i++)
		{
			nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr style=\"background-color:#BBCCDE;\" align=\"center\"><td align=center width=\"10%%\" class=\"\"><input type=\"radio\" name=\"select%d\" id=\"select%d\" value=\"sel%d\" onClick=\"connectClick('sel%d')\"></td>\n"),i,i,i,i);  //Select
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"72%%\">\n"));

				nBytesSent += websWrite(wp, T("        <font size=\"2\">SSID%d</font>\n"),i);    // SSID
				nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
				nBytesSent += websWrite(wp, T("					<script>document.forms['formWlSiteSurvey'].elements['ssid'+%d].value=\"SSID%d\";</script>\n"),i,i);
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">100%%</font>"));  //Signal
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"100%%\">\n"),i); 
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"10\">\n"),i); //Channel
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"WPA2PSK\">\n"),i);  //encryption
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"AES\">\n"),i);

				nBytesSent += websWrite(wp, T("<tr>\n"));
				nBytesSent += websWrite(wp, T("       <td style=\"display:none\" align=center>\n"));

				nBytesSent += websWrite(wp, T("<font size=\"2\">BELOW</font>\n"));
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"secchan%d\" value=\"2\">\n"),i);
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"74:DA:38:03:13:23\">\n"),i); 
				websWrite(wp, T("       </td>\n"));
				websWrite(wp, T("</tr>\n"));
		}

		if(apCount < 6){
			int add_tr_count;
			add_tr_count = 6 - apCount;
			for(i = 0 ; i < add_tr_count ; i++)
			{
				nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr style=\"background-color:#FFFFFF;\" align=\"center\">\n"));
				nBytesSent += websWrite(wp, T("<td colspan=\"3\" >&nbsp;</td></tr>\n"));
			}
		}
#endif
	}
	return nBytesSent;
}
/////
int wlSurveyOnlyTbl(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, wait_time=0 ,j,z, kakaso, apCount, len, ap_mode;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[16384],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];

	if (pStatus==NULL)
	{
		pStatus = calloc(1, sizeof(SS_STATUS_T));
		if ( pStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}

	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(WLAN_IF,  &data) < 0)
	{
		nBytesSent = websWrite(wp, T("<script>document.write(showText(aspFormWlSiteSurvey1))</script>"));
		return nBytesSent;
	}

	while (1)
	{
		if (wait_time++ > 5)
		{
			break;
		}
		sleep(1);
	}

	pStatus->number = 0;

	if ((len = getWlSiteSurveyResult(WLAN_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(pStatus);
		pStatus = NULL;
		return nBytesSent;
	}

	printf("========== Get Site Survey AP List ==========\n");
	// kernel file: cmm_info.c
	if(sizeof(data) > 0){
		int slength=0, j=0;
		int single_length, extern_length;
		char buf[50];
		FILE *fp;
		int length;
		int type_decided=0;
		struct site_survey_t{
			char channel[4];
			char ssid[33];
			char bssid[20];
			char security[23];
			char signal[9];
			char rssi[6];
			char mode[7];
			char extch[7];
			char type[3];
			char wps[4];
			char dpid[5];
		}site_survey, *psite_survey;

		apCount = i = 0;
		op = sp = &data;

		single_length = 112;	// assume the sitesurvey table at least have NT label
		extern_length = 0;

		op += single_length;

		while (*(op + extern_length) != '\n')
		{
			extern_length ++;
			if (extern_length > 9)
			{
				break;
			}
		}
		single_length += extern_length;		// + \n
		sp += single_length + 1;
		len -= single_length + 1;

		printf("total len = %d single_length = %d \n%s", len, single_length, sp);
		// santy check
		// 1. valid char data
		// 2. rest length is larger than per line length ==> (1+8+8+35+19+8+1)
		//    Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType
		while (len >= single_length)
		{

			memset (&SiteSurvey[i], 0x00, sizeof(SiteSurvey[i]));
			psite_survey = (struct site_survey_t *) sp;

			memcpy (SiteSurvey[i].channel, psite_survey->channel, 2);
			memcpy (SiteSurvey[i].ssid, psite_survey->ssid, 32);

			sscanf (psite_survey->bssid, "%02x:%02x:%02x:%02x:%02x:%02x", (int *)&SiteSurvey[i].bssid[0], (int *)&SiteSurvey[i].bssid[1],
										      (int *)&SiteSurvey[i].bssid[2], (int *)&SiteSurvey[i].bssid[3],
 										      (int *)&SiteSurvey[i].bssid[4], (int *)&SiteSurvey[i].bssid[5]);
			memcpy (SiteSurvey[i].signal, psite_survey->signal, 3);
			memcpy (SiteSurvey[i].rssi, psite_survey->rssi, 3);
			memcpy (SiteSurvey[i].wirelessmode, psite_survey->mode, 8);
			memcpy (SiteSurvey[i].networktype, psite_survey->type, 2);

			psite_survey->security[sizeof(psite_survey->security) - 1] = 0x00;
			psite_survey->ssid[sizeof(psite_survey->ssid) - 1] = 0x00;
			
			
			for (z = 33; z > 0; z--)
			{
				if(isspace(SiteSurvey[i].ssid[z]) || SiteSurvey[i].ssid[z]=='\0')
					SiteSurvey[i].ssid[z] = '\0';
				else
					break;
			}

						if (strstr (psite_survey->security, "NONE")){
				strcpy (SiteSurvey[i].authmode, "OPEN");
				strcpy (SiteSurvey[i].webstatus, "NONE");

			}else if (strstr (psite_survey->security, "WEP")){
				strcpy(SiteSurvey[i].authmode, "WEP");
				strcpy(SiteSurvey[i].webstatus, "NONE");

			}else if (strstr (psite_survey->security, "WPAPSK/TKIP")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPAPSK/AES") || strstr (psite_survey->security, "WPAPSK/TKIPAES")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIP") || strstr (psite_survey->security, "WPA2PSK/TKIP")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");
			}else{
				strcpy (SiteSurvey[i].authmode, "OPEN");
				strcpy (SiteSurvey[i].webstatus, "NONE");
			}

			memcpy (SiteSurvey[i].extch, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].extch, " NONE "))
				strcpy (SiteSurvey[i].extch, "0");
			else if (strstr (SiteSurvey[i].extch, " ABOVE"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, " BELOW"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");
			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY SiteSurveyBUF[100]={0};
		int x,y;
		for (x=0; x<i; x++) {
			for (y=x;y<i;y++){
				if (atoi(SiteSurvey[y].signal) > atoi(SiteSurvey[x].signal)) {
					SiteSurveyBUF[x] = SiteSurvey[y];
					SiteSurvey[y] = SiteSurvey[x];
					SiteSurvey[x] = SiteSurveyBUF[x];
				}
			}
		}
		#endif

		apCount = i;
		pStatus->number = i;

		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">"),apCount);

			for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
			{
				pStatus->bssdb[i] = SiteSurvey[i];
    				sprintf(tmpBuf,"%s",pStatus->bssdb[i].ssid);
				char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
				int PowerGband = atoi(pStatus->bssdb[i].signal);
				int PowerUpGband = ScanListSignalScaleMapping92CD(PowerGband);
#endif
				nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr class=\"listtableText\" align=\"center\">\n"));  //Select
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"37%%\">\n"));

				nBytesSent += websWrite(wp, T("        <font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
				nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
				nBytesSent += websWrite(wp, T("					<script>document.forms['formWlSiteSurvey'].elements['ssid'+%d].value=\""),i);
				nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
				nBytesSent += websWrite(wp, T("\"     </script>\n"));
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"21%%\"><font size=\"2\">%02X:%02X:%02X:%02X:%02X:%02X</font>"),pStatus->bssdb[i].bssid[0],pStatus->bssdb[i].bssid[1],pStatus->bssdb[i].bssid[2],pStatus->bssdb[i].bssid[3],pStatus->bssdb[i].bssid[4],pStatus->bssdb[i].bssid[5]);  //MAC
				nBytesSent += websWrite(wp, T("		</td>\n"));
				
				pStatus->bssdb[i].wirelessmode[8]='\0';
				
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"12%%\"><font size=\"2\">%s</font>"),pStatus->bssdb[i].wirelessmode);  //b/g/n
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"6%%\"><font size=\"2\">%s</font>"),pStatus->bssdb[i].channel);  //channel
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"6%%\"><font size=\"2\">%s</font>"),"AP");  //type
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%s</font>"),pStatus->bssdb[i].authmode);  //authmode
				nBytesSent += websWrite(wp, T("    </td>\n"));
#if defined(_SIGNAL_FORMULA_)
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%d%%</font>"),PowerUpGband);  //Signal
#else
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%s%%</font>"),pStatus->bssdb[i].signal);  //Signal
#endif
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));

				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"band%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].wirelessmode); 
#if defined(_SIGNAL_FORMULA_)
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpGband); 
#else
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].signal); 
#endif
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,pStatus->bssdb[i].bssid[0],pStatus->bssdb[i].bssid[1],pStatus->bssdb[i].bssid[2],pStatus->bssdb[i].bssid[3],pStatus->bssdb[i].bssid[4],pStatus->bssdb[i].bssid[5]); 

				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, pStatus->bssdb[i].channel); //Channel
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].authmode);  //encryption
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].webstatus);

				nBytesSent += websWrite(wp, T("<tr>\n"));
				nBytesSent += websWrite(wp, T("       <td style=\"display:none\" align=center class=\"table2\">\n"));

				nBytesSent += websWrite(wp, T("<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);

				websWrite(wp, T("       </td>\n"));
				websWrite(wp, T("</tr>\n"));
			}
	}
	return nBytesSent;
}
/////////////////////////////////////////////////////////////////////////////
#ifndef HOME_GATEWAY	// Radius server
void formRadius(webs_t wp, char_t *path, char_t *query)
{
	char_t *strUsrName, *strUsrPass, *strAddUsr, *strDelUsr, *strDelAllUsr;
	char_t *strCltIp, *strCltPass, *strAddClt, *strDelClt, *strDelAllClt;
	char_t *strVal, *submitUrl, *strApply;
//	char_t *strFrom, *strTo;
	char tmpBuf[100];
	int entryNum, intVal, i;
	RSER_CLT_T cltEntry;	// for radius server client
	RSER_USR_T usrEntry;	// for radius server user
//	struct in_addr curIpAddr, curSubnet;
	void *pEntry;
	int num_id, get_id, add_id, del_id, delall_id, enable_id;

	strAddUsr = websGetVar(wp, T("addRSerUsr"), T(""));
	strDelUsr = websGetVar(wp, T("delSelRSerUsr"), T(""));
	strDelAllUsr = websGetVar(wp, T("delAllRSerUsr"), T(""));

	strAddClt = websGetVar(wp, T("addRSerClt"), T(""));
	strDelClt = websGetVar(wp, T("delSelRSerClt"), T(""));
	strDelAllClt = websGetVar(wp, T("delAllRSerClt"), T(""));

	if (strAddUsr[0] || strDelUsr[0] || strDelAllUsr[0]) {
		num_id = MIB_RSER_USR_NUM;
		get_id = MIB_RSER_USR;
		add_id = MIB_RSER_USR_ADD;
		del_id = MIB_RSER_USR_DEL;
		delall_id = MIB_RSER_USR_DELALL;
		enable_id = MIB_RSER_ENABLED;
		memset(&usrEntry, '\0', sizeof(usrEntry));
		pEntry = (void *)&usrEntry;
	}
	else {
		num_id = MIB_RSER_CLT_NUM;
		get_id = MIB_RSER_CLT;
		add_id = MIB_RSER_CLT_ADD;
		del_id = MIB_RSER_CLT_DEL;
		delall_id = MIB_RSER_CLT_DELALL;
		enable_id = MIB_RSER_ENABLED;
		memset(&cltEntry, '\0', sizeof(cltEntry));
		pEntry = (void *)&cltEntry;
	}

	// Set enable flag
	if (strAddUsr[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set(enable_id, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformDdnsErrFlag))</script>"));
			goto setErr_radius;
		}
	}
//*********************************** Add Radius usr ***********************************
	if (strAddUsr[0]) {
		strUsrName = websGetVar(wp, T("rserUsrName"), T(""));
		strUsrPass = websGetVar(wp, T("rserUsrPass"), T(""));

		if (!strUsrName[0] && !strUsrPass[0])
			goto setOk_radius;

		if (!strUsrName[0] || !strUsrPass[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformFilterErrInput))</script>"));
			goto setErr_radius;
		}

		if ( strUsrName[0] ) {
			if (strlen(strUsrName) > 32) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRadiusl2))</script>"));
				goto setErr_radius;
			}
			else
				strcpy(usrEntry.username, strUsrName);
		}

		if ( strUsrPass[0] ) {
			if (strlen(strUsrPass) > 16) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRadiusl3))</script>"));
				goto setErr_radius;
			}
			else
				strcpy(usrEntry.password, strUsrPass);
		}
	}

//*********************************** Add Radius client ***********************************
	if (strAddClt[0]) {
		strCltIp = websGetVar(wp, T("rserCltIp"), T(""));
		strCltPass = websGetVar(wp, T("rserCltPass"), T(""));

		if (!strCltIp[0] && !strCltPass[0])
			goto setOk_radius;

		if (!strCltIp[0] || !strCltPass[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformFilterErrInput))</script>"));
			goto setErr_radius;
		}

		fixed_inet_aton(strCltIp, cltEntry.ipAddr);

		if ( strCltPass[0] ) {
			if (strlen(strCltPass) > 16) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRadiusl3))</script>"));
				goto setErr_radius;
			}
			else
				strcpy(cltEntry.password, strCltPass);
		}
	}

	if (strAddUsr[0] || strAddClt[0]) {

		if ( !apmib_get(num_id, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformQoSErrGetNum))</script>"));
			goto setErr_radius;
		}
		if (strAddUsr[0]) {
			if ( (entryNum + 1) > 96) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspformQoSInvAddEntry))</script>"));
				goto setErr_radius;
			}
		}
		if (strAddClt[0]) {
			if ( (entryNum + 1) > 16) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspformQoSInvAddEntry))</script>"));
				goto setErr_radius;
			}
		}
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(del_id, pEntry);
		if ( apmib_set(add_id, pEntry) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformQoSErrAddEntry))</script>"));
			goto setErr_radius;
		}
	}

	// Delete entry
	if (strDelUsr[0] || strDelClt[0]) {
		if ( !apmib_get(num_id, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetNum))</script>"));
			goto setErr_radius;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)pEntry) = (char)i;
				if ( !apmib_get(get_id, pEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetTbl))</script>"));
					goto setErr_radius;
				}
				if ( !apmib_set(del_id, pEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrDelEntry))</script>"));
					goto setErr_radius;
				}
			}
		}
	}

	// Delete all entry
	if (strDelAllUsr[0] || strDelAllClt[0]) {
		if ( !apmib_set(delall_id, pEntry)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrDelAll))</script>"));
			goto setErr_radius;
		}
	}
setOk_radius:
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
//	sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH, _CONFIG_SCRIPT_PROG, "ap", "all");
	//system("/bin/Radiusd.sh stop");
	//system("/bin/Radiusd.sh start&");
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif

	if (strApply[0]) {
		OK_MSG(wp, submitUrl);
	}
	else {
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		else
			websDone(wp, 200);
	}

  	return;
setErr_radius:
	ERR_MSG(wp, tmpBuf);
  	return;
}
/////////////////////////////////////////////////////////////////////////////
int RadiusCltList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	RSER_CLT_T entry;
	char	*ip;

	if ( !apmib_get(MIB_RSER_CLT_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_RSER_CLT, (void *)&entry))
			return -1;

		ip = fixed_inet_ntoa(entry.ipAddr);

		nBytesSent += websWrite(wp, T("<tr>"
		"<td align=center class=\"table2\"><font size=\"2\">%d</td>\n"
		"<td align=center class=\"table2\"><font size=\"2\">%s</td><input type=\"hidden\" name=\"ip%d\" value=\"%s\">\n"
   		"<td align=center class=\"table2\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
				i, ip, i ,ip,i);
	}
	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"sipnum\" value=\"%d\">\n"),i-1);
	return nBytesSent;
}
/////////////////////////////////////////////////////////////////////////////
int RadiusUsrList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	RSER_USR_T entry;
//	char	*type, *ip, portFrom[20], portTo[20];

	if ( !apmib_get(MIB_RSER_USR_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_RSER_USR, (void *)&entry))
			return -1;

		nBytesSent += websWrite(wp, T("<tr>"
		"<td align=center class=\"table2\"><font size=\"2\">%d</td>\n"
		"<td align=center class=\"table2\"><font size=\"2\">%s</td><input type=\"hidden\" name=\"user%d\" value=\"%s\">\n"
   		"<td align=center class=\"table2\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
				i, entry.username, i, entry.username, i);
	}
	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"usernum\" value=\"%d\">\n"),i-1);
	return nBytesSent;
}
#endif
/////////////////////////////////////////////////////////////////////////////



void formWdsEncrypt(webs_t wp, char_t *path, char_t *query)
{

   char_t *wepKey;
   char_t *strKeyLen, *strFormat, *strKeyId, *strEnabled;
	char key[30];
	int enabled, keyLen, ret, i;
	WEP_T wep;

   	char_t *submitUrl, *strEncrypt, *strVal, *strEnRadius, *strWpaCipher;
	char tmpBuf[100];
	ENCRYPT_T encrypt;
	int enableRS=0, intVal, getPSK=0, len, intTkip, intAes;
	unsigned long reKeyTime;
	SUPP_NONWAP_T suppNonWPA;
	struct in_addr inIp;

	//Set security method
   	strEncrypt = websGetVar(wp, T("method"), T(""));
//	printf("strEncrypt=%s\n");

	if (!strEncrypt[0]) {
 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt1))</script>"));
		goto setErr_encrypt;
	}
	encrypt = strEncrypt[0] - '0';
	if (apmib_set( MIB_WLAN_WDS_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}

	if (encrypt==2 || encrypt==3) {
		// WPA authentication
		if (encrypt==2) {
			intVal = WPA_AUTH_PSK;
			getPSK = 1;
		}
		else if (encrypt==3) {
			intVal = WPA_AUTH_AUTO;
			enableRS = 1;
		}
		if ( apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
			goto setErr_encrypt;
		}

		// cipher suite
		strVal = websGetVar(wp, T("wdsWpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if (strVal[0]) {
			if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
			if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;

			if ( apmib_set(MIB_WLAN_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_WLAN_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
		}
	}

	//Follow Realtek, Set WLAN_ENCRYPT
	strWpaCipher = websGetVar(wp, T("wdsWpaCipher"), T(""));
	if (strEncrypt[0]=='0')	encrypt=0;
	else if (strEncrypt[0]=='1') encrypt=1;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='1')) encrypt=2;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='2')) encrypt=2;
	else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='3')) encrypt=2;
	if (apmib_set( MIB_WLAN_WDS_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
		goto setErr_encrypt;
	}
//*********************************** Wep setting ***********************************
	if (encrypt==ENCRYPT_WEP) {
		strEnabled = websGetVar(wp, T("wepEnabled"), T(""));
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( enabled ) {
			strKeyLen = websGetVar(wp, T("length"), T(""));
			if (!strKeyLen[0]) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt2))</script>"));
				goto setErr_encrypt;
			}
			if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt3))</script>"));
				goto setErr_encrypt;
			}
			if (strKeyLen[0] == '1')
				wep = WEP64;
			else if (strKeyLen[0] == '2')
				wep = WEP128;
		}
		else
			wep = WEP_DISABLED;

		if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt5))</script>"));
			goto setErr_encrypt;
		}

		if (wep == WEP_DISABLED)
			goto setwep_ret;

		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt5))</script>"));
			goto setErr_encrypt;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt6))</script>"));
			goto setErr_encrypt;
		}

		i = strFormat[0] - '0' - 1;
		if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_encrypt;
		}

		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;
			else
				keyLen = WEP128_KEY_LEN*2;
		}
			strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt8))</script>"));
				goto setErr_encrypt;
			}
			i = strKeyId[0] - '0' - 1;
			if ( !apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_encrypt;
			}
		}

		wepKey = websGetVar(wp, T("key1"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
					goto setErr_encrypt;
				}
			}
		}
		wepKey = websGetVar(wp, T("key2"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt13))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt14))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY2, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt15))</script>"));
					goto setErr_encrypt;
				}
			}
		}

		wepKey = websGetVar(wp, T("key3"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt16))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt17))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY3, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt18))</script>"));
					goto setErr_encrypt;
				}
			}
		}
		wepKey = websGetVar(wp, T("key4"), T(""));
		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_encrypt;
			}
			if ( !isAllStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt19))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY4, (void *)key);
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt20))</script>"));
					goto setErr_encrypt;
				}
			}
		}
	}

//*********************************** pre-shared key setting ***********************************
	if ( getPSK ) {
		int oldFormat, oldPskLen, i;

		strVal = websGetVar(wp, T("wdsPskFormat"), T(""));
		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt22))</script>"));
			goto setErr_encrypt;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt23))</script>"));
			goto setErr_encrypt;
		}

		// remember current psk format and length to compare to default case "****"
		apmib_get(MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_WLAN_WDS_WPA_PSK, (void *)tmpBuf);
		oldPskLen = strlen(tmpBuf);

		strVal = websGetVar(wp, T("wdsPskValue"), T(""));
		len = strlen(strVal);

		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto rekey_time;
		}

		if ( apmib_set(MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}

		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_encrypt;
			}
		}
		if ( !apmib_set(MIB_WLAN_WDS_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
	}
//****************************************  ****************************************
rekey_time:
get_wepkey:
		// get 802.1x WEP key length
		strVal = websGetVar(wp, T("wepKeyLen"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("wep64")))
				intVal = WEP64;
			else if ( !gstrcmp(strVal, T("wep128")))
				intVal = WEP128;
			else {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt15))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_WLAN_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_encrypt;
			}
		}
//-----------------------------------------------------------------------------------------------
setwep_ret:
	apmib_update(CURRENT_SETTING);


//	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
//#ifdef _TAG_IN_PAGE_
//	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
//	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
//	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
//#endif

//	OK_MSG(wp, submitUrl);
//	return;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	char_t *strApply;
	char tmpBufapply[200];
	strApply = websGetVar(wp, T("isApply"),T(""));
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			OK_MSG3(tmpBuf, newurl);
		}
		else
		{
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			OK_MSG3(tmpBufapply ,submitUrl);
		}
		system("/bin/reboot.sh 3 &");
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REPLACE_MSG(submitUrl);
	}

	return;


setErr_encrypt:
	ERR_MSG(wp, tmpBuf);
}

/////////////////////////////////////////////////////////////////////////////
int wdsList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i;
	WDS_INFO_Tp pInfo;
	char *buff;

	buff = calloc(1, sizeof(WDS_INFO_T)*MAX_STA_NUM);
	if ( buff == 0 ) {
		printf("<script>document.write(showText(aspWirelessClientList1))</script>\n");
		return 0;
	}

	if ( getWdsInfo(WLAN_IF, buff) < 0 ) {
		printf("<script>document.write(showText(aspWirelessClientList2))</script>\n");
		return 0;
	}

	for (i=0; i<MAX_WDS_NUM; i++) {
		pInfo = (WDS_INFO_Tp)&buff[i*sizeof(WDS_INFO_T)];

		if (pInfo->state == STATE_WDS_EMPTY)
			break;

		nBytesSent += websWrite(wp,
	   		"<tr bgcolor=#b7b7b7><td><font size=2>%02x:%02x:%02x:%02x:%02x:%02x</td>"
			"<td><font size=2>%d</td>"
	     		"<td><font size=2>%d</td>"
			"<td><font size=2>%d</td>"
			"<td><font size=2>%d%s</td>",
			pInfo->addr[0],pInfo->addr[1],pInfo->addr[2],pInfo->addr[3],pInfo->addr[4],pInfo->addr[5],
			pInfo->tx_packets, pInfo->tx_errors, pInfo->rx_packets,
			pInfo->txOperaRate/2, ((pInfo->txOperaRate%2) ? ".5" : "" ));
	}

	free(buff);

	return nBytesSent;
}

#ifdef _MSSID_
void formMultipleSSID(webs_t wp, char_t *path, char_t *query)
{
int val;
char_t *ssid1, *ssid2, *ssid3, *ssid4, *ssid5, *ssid6;

ssid1=websGetVar(wp,T("SSID1"),T(""));
ssid2=websGetVar(wp,T("SSID2"),T(""));
ssid3=websGetVar(wp,T("SSID3"),T(""));
ssid4=websGetVar(wp,T("SSID4"),T(""));
ssid5=websGetVar(wp,T("SSID5"),T(""));
ssid6=websGetVar(wp,T("SSID6"),T(""));

if(gstrcmp(ssid1,T(""))) apmib_set(MIB_WLAN_SSID_1, (void *)websGetVar(wp,T("SSID1"),T("")));
if(gstrcmp(ssid2,T(""))) apmib_set(MIB_WLAN_SSID_2, (void *)websGetVar(wp,T("SSID2"),T("")));
if(gstrcmp(ssid3,T(""))) apmib_set(MIB_WLAN_SSID_3, (void *)websGetVar(wp,T("SSID3"),T("")));
if(gstrcmp(ssid4,T(""))) apmib_set(MIB_WLAN_SSID_4, (void *)websGetVar(wp,T("SSID4"),T("")));
if(gstrcmp(ssid5,T(""))) apmib_set(MIB_WLAN_SSID_5, (void *)websGetVar(wp,T("SSID5"),T("")));
if(gstrcmp(ssid6,T(""))) apmib_set(MIB_WLAN_SSID_6, (void *)websGetVar(wp,T("SSID6"),T("")));

val=atoi(websGetVar(wp,T("mirror1"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_1, (void *)&val);
val=atoi(websGetVar(wp,T("mirror2"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_2, (void *)&val);
val=atoi(websGetVar(wp,T("mirror3"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_3, (void *)&val);
val=atoi(websGetVar(wp,T("mirror4"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_4, (void *)&val);
val=atoi(websGetVar(wp,T("mirror5"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_5, (void *)&val);
val=atoi(websGetVar(wp,T("mirror6"),T("")));apmib_set(MIB_WLAN_SSID_MIRROR_6, (void *)&val);

if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID1"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID2"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID3"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID4"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID5"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID6"),T("")));apmib_set(MIB_WLAN_HIDDEN_SSID_6, (void *)&val);}

if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable1"),T("")));apmib_set(MIB_WLAN_WMM_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable2"),T("")));apmib_set(MIB_WLAN_WMM_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable3"),T("")));apmib_set(MIB_WLAN_WMM_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable4"),T("")));apmib_set(MIB_WLAN_WMM_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable5"),T("")));apmib_set(MIB_WLAN_WMM_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable6"),T("")));apmib_set(MIB_WLAN_WMM_6, (void *)&val);}

if(gstrcmp(ssid1,T(""))) {val = atoi(websGetVar(wp,T("txrate1"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val = atoi(websGetVar(wp,T("txrate2"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val = atoi(websGetVar(wp,T("txrate3"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val = atoi(websGetVar(wp,T("txrate4"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val = atoi(websGetVar(wp,T("txrate5"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val = atoi(websGetVar(wp,T("txrate6"),T("")));val = 1 << (val-1);apmib_set(MIB_WLAN_FIX_RATE_6, (void *)&val);}

#if defined(_ENVLAN_)
if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("vlanid1"),T("")));apmib_set(MIB_WLAN_VLANID_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("vlanid2"),T("")));apmib_set(MIB_WLAN_VLANID_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("vlanid3"),T("")));apmib_set(MIB_WLAN_VLANID_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("vlanid4"),T("")));apmib_set(MIB_WLAN_VLANID_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("vlanid5"),T("")));apmib_set(MIB_WLAN_VLANID_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("vlanid6"),T("")));apmib_set(MIB_WLAN_VLANID_6, (void *)&val);}
#endif //_ENVLAN_

#ifdef _LAN_WAN_ACCESS_
unsigned int LanWanValue = 0;
apmib_get( MIB_LAN_WAN_ACCESS, (void *)&LanWanValue);
LanWanValue &= 0xFFFF000F;

if(gstrcmp(ssid1,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue1"),T(""))) << 4;}
if(gstrcmp(ssid2,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue2"),T(""))) << 8;}
if(gstrcmp(ssid3,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue3"),T(""))) << 12;}
apmib_set(MIB_LAN_WAN_ACCESS,(void *)&LanWanValue);
#endif //_LAN_WAN_ACCESS_

apmib_update(CURRENT_SETTING);
OK_MSG(wp, websGetVar(wp, T("submit-url"), T("")));
}
#endif //_MSSID_

#ifdef _WISP_

void formStaDrvSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *str, *strSupportAllSecurity, *iqsetupclose, *strVal;
	char tmp[100];
	struct in_addr inIp;
	int val,intVal,i;
	int len;
	int band; //EDX setwer
	str = websGetVar(wp, T("stadrv_band"), T(""));  // 2.4G, 5G
	band = atoi(str);
	str = websGetVar(wp, T("stadrv_type"), T(""));  // enable,disable
	val=atoi(str);
	char tmpBuf[100];
	//EDX setwer
	
	stadrvenable=val;
	if (val == 1){  // enable
	  	if (band == 0)
				val = 1;   // 2.4g
			else
				val = 3;   // 5g
  	}
	//EDX setwer end
	apmib_set(MIB_OP_MODE, (void *)&val);
	if ( !val )
		goto setwisp_ret;   //EDX patrick add, if disable, no need set, update flash

	if (band == 0){
		if(val == 1){
			apmib_get(MIB_AP_MODE, (void *)&val);
			if(val==1 || val==2 || val==6){
				val=0;
				apmib_set(MIB_AP_MODE, (void *)&val);
				apmib_set( MIB_REPEATER_ENABLED, (void *)&val);
			}
		}
	}else{
		//EDX setwer
	    	if(val == 3){
				apmib_get(MIB_INIC_AP_MODE, (void *)&val);
		      if(val==1 || val==2 || val==6){
		      	val=0;
					apmib_set(MIB_INIC_AP_MODE, (void *)&val);
					apmib_set(MIB_INIC_REPEATER_ENABLED, (void *)&val);
		      	}
	    	}
		//EDX setwer
    }


	str = websGetVar(wp, T("stadrv_clonemac"), T(""));
	if ( str[0] ) { // EDX patrick add
		string_to_hex(str, tmp, 12);
		apmib_set(MIB_STADRV_CLONEMAC, (void *)tmp);
	}

	str = websGetVar(wp, T("stadrv_ssid"), T(""));
	if ( str[0] ) { // EDX patrick add
		apmib_set(MIB_STADRV_SSID, (void *)str);
	}

	str = websGetVar(wp, T("stadrv_band"), T(""));  // 2.4G, 5G
	if ( str[0] ) { // EDX patrick add
		val=atoi(str);
		apmib_set(MIB_STADRV_BAND, (void *)&val);
		str = websGetVar(wp, T("stadrv_chan"), T(""));
		char_t *strChanWidth;
		strChanWidth = websGetVar(wp, T("secchan"), T(""));

		if ( str[0] ) {
				intVal=atoi(str);
				printf(">> wisp Site MIB_STADRV_CHAN_NUM=%d\n",intVal);
				apmib_set(MIB_STADRV_CHAN_NUM, (void *)&intVal);

				if(val==1) { //5G WISP
					printf(">> wisp Site MIB_INIC_CHAN_NUM=%d\n",intVal);
					apmib_set(MIB_INIC_CHAN_NUM, (void *)&intVal);
				}
				else if (val==0) { //24G WISP
					printf(">> wisp Site MIB_WLAN_CHAN_NUM=%d\n",intVal);
					apmib_set(MIB_WLAN_CHAN_NUM, (void *)&intVal);
				}

				if (strChanWidth[0]) {
					printf("\nwisp Site Survey secchan=%s\n",strChanWidth);

					if(val==1) { //5G WISP
						if (!gstrcmp(strChanWidth, T("0"))) {val = 0;}
						else if (!gstrcmp(strChanWidth, T("1"))) {val = 1;}
						else if (!gstrcmp(strChanWidth, T("2"))) {val = 2;}
						else if (!gstrcmp(strChanWidth, T("3"))) {val = 3;}
						else if (!gstrcmp(strChanWidth, T("4"))) {val = 4;}
						else {
							DEBUGP(tmp, T("Invalid Channel Width 0"));
							goto setErr_encrypt;
						}
						if ( apmib_set(MIB_INIC_N_CHANNEL_WIDTH, (void *)&val) == 0) {
							DEBUGP(tmp, T("Invalid Channel Width 1"));
							goto setErr_encrypt;
						}
					}
					else if (val==0) { //24G WISP
						if (!gstrcmp(strChanWidth, T("0"))) {val = 0;}
						else if (!gstrcmp(strChanWidth, T("1"))) {val = 1;}
						else if (!gstrcmp(strChanWidth, T("2"))) {val = 2;}
						else {
							DEBUGP(tmp, T("Invalid Channel Width 0"));
							goto setErr_encrypt;
						}
						if ( apmib_set(MIB_WLAN_N_CHANNEL_WIDTH, (void *)&val) == 0) {
							DEBUGP(tmp, T("Invalid Channel Width 1"));
							goto setErr_encrypt;
						}
					}
				}
		}

	}
	str = websGetVar(wp, T("stadrv_encrypttype"), T(""));  // disable,wep.wpa-psk
	val=atoi(str);
	apmib_set(MIB_STADRV_ENCRYPT_TYPE, (void *)&val);
	
	strSupportAllSecurity=websGetVar(wp, T("supportAllSecurity"), T(""));
	switch(val)
	{
		case 0:
			break;
		case 1:
			str = websGetVar(wp, T("stadrv_weplength"), T(""));  //64bit, 128bit
			val=atoi(str);
			len=val;
			apmib_set(MIB_STADRV_WEP_LENGTH, (void *)&val);

			str = websGetVar(wp, T("stadrv_wepformat"), T(""));  //64bit:5,10 128bit:13,26
			val=atoi(str);
			apmib_set(MIB_STADRV_WEP_FORMAT1, (void *)&val);

			str = websGetVar(wp, T("key1"), T(""));
			if ((str[0]) && (!isAllStar(str))) //EDX patrick add
			{
				if (val==0)
				{
					if (len==0)
						sprintf(tmp, "%.2x%.2x%.2x%.2x%.2x", str[0], str[1], str[2], str[3], str[4]);
					else
						sprintf(tmp, "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7], str[8], str[9], str[10], str[11], str[12]);
					apmib_set(MIB_STADRV_WEPKEY1, (void *)tmp);
				}
				else
					apmib_set(MIB_STADRV_WEPKEY1, (void *)str);
			}

			str = websGetVar(wp, T("stadrv_defaultkey"), T(""));  // key1
			val=atoi(str);
			apmib_set(MIB_STADRV_WEP_DEFAULT_KEY, (void *)&val);

			break;
		case 2: 
			//EDX Robert 2014_02_21 Add Support All Security WPA (TKIP/AES)
			if(strSupportAllSecurity[0]){
				str = websGetVar(wp, T("stadrv_WPAcipher"), T("")); 
				val=atoi(str);
				apmib_set(MIB_STADRV_WPA_CIPHER, (void *)&val);

				str = websGetVar(wp, T("stadrv_WPApskformat"), T(""));  
				val=atoi(str);
				apmib_set(MIB_STADRV_PSK_FORMAT, (void *)&val);

				str = websGetVar(wp, T("stadrv_WPApskkey"), T(""));
			}
			else{	
				str = websGetVar(wp, T("stadrv_wpacipher"), T(""));  // tkip, aes
				val=atoi(str);
				apmib_set(MIB_STADRV_WPA_CIPHER, (void *)&val);

				str = websGetVar(wp, T("stadrv_pskformat"), T(""));  //Passphrase, hex_64cha
				val=atoi(str);
				apmib_set(MIB_STADRV_PSK_FORMAT, (void *)&val);

				str = websGetVar(wp, T("stadrv_pskkey"), T(""));
			}
			
			if ((str[0]) && (!isAllStar(str))) //EDX patrick add
			{ 
				if (val==1) { // hex
					if (!string_to_hex(str, tmp, MAX_PSK_LEN)) {
		 				DEBUGP(tmp, T("WISP KEY ERROR!"));
						goto setErr_encrypt;
					}
				}
				apmib_set(MIB_STADRV_PSKKEY, (void *)str);
			}
			break;
		case 3: //EDX Robert 2014_02_21 Add Support All Security WPA2 (TKIP/AES)
			if(strSupportAllSecurity[0])
			{
				str = websGetVar(wp, T("stadrv_WPA2cipher"), T("")); 
				val=atoi(str);
				apmib_set(MIB_STADRV_WPA_CIPHER, (void *)&val);

				str = websGetVar(wp, T("stadrv_WPA2pskformat"), T(""));  
				val=atoi(str);
				apmib_set(MIB_STADRV_PSK_FORMAT, (void *)&val);

				str = websGetVar(wp, T("stadrv_WPA2pskkey"), T(""));
			}
			
			if ((str[0]) && (!isAllStar(str))) //EDX patrick add
			{ 
				if (val==1) { // hex
					if (!string_to_hex(str, tmp, MAX_PSK_LEN)) {
		 				DEBUGP(tmp, T("WISP KEY ERROR!"));
						goto setErr_encrypt;
					}
				}
				apmib_set(MIB_STADRV_PSKKEY, (void *)str);
			}
			break;
		default:
			break;
	}

	#ifdef _IQv2_  //EDX Hahn add
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

setwisp_ret: //EDX patrick add
	apmib_update(CURRENT_SETTING);



	str = websGetVar(wp, T("submit-url"), T(""));

	#ifdef _Edimax_
	char_t *strApply;
	strApply = websGetVar(wp, T("isApply"),T(""));
	char_t *strCONNTEST;
	char_t *wiz_wispManu, *WISP_enable5g, *WISP_enable2g, *WISPMANU;

	WISP_enable5g = websGetVar(wp, T("WISP_enable5g"), T(""));
	WISP_enable2g = websGetVar(wp, T("WISP_enable2g"), T(""));

	wiz_wispManu = websGetVar(wp, T("wiz_wispManu"), T(""));
	WISPMANU = websGetVar(wp, T("WISPMANU"), T(""));

	if ((!gstrcmp(wiz_wispManu, T("0")))) //iQsetup by Auto  
	{
		strVal = websGetVar(wp, T("rootAPmac"), T(""));		
		if(strVal[0]){
			if((!gstrcmp(WISP_enable5g, T("1")))){
				for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "rootAPmac;systemcode".
				{
					if (strVal[i]==59)
					{
						strVal[i]='\0';
						break;
					}
				}
				sprintf(tmpBuf, "echo '%s' > /tmp/apclii0_rootMAC", strVal);
			}
			else{
				for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "rootAPmac;systemcode".
				{
					if (strVal[i]==59)
					{
						strVal[i]='\0';
						break;
					}
				}
				sprintf(tmpBuf, "echo '%s' > /tmp/apcli0_rootMAC", strVal);
			}
			system(tmpBuf);
		}

		int pid;
		pid = fork();
		if (pid) {
			waitpid(pid, NULL, 0);
			sleep(3);
		}
		else if (pid == 0) {
			if (str[0])
				REPLACE_MSG(str);
			exit(0);
		}

		if((!gstrcmp(WISP_enable5g, T("1")))){
			if(WISPMANU[0])
				CONNECT_TEST(7); //5g wisp connect_test hand
			else
				CONNECT_TEST(4); //5g wisp connect_test auto
		}
		else{
			if(WISPMANU[0])
				CONNECT_TEST(6); //2.4g wisp connect_test hand
			else
				CONNECT_TEST(3); //2.4g wisp connect_test auto
		}
	}	
	else
	{
	#endif
			//iQsetup by hand  
			if(strApply[0]){
				if (lanipChanged == 1) {
					char tmpBufnewip[200];
					unsigned char buffer[100];
					unsigned char newurl[150];
					if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
						return -1;

					sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
					sprintf(tmpBufnewip, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
					OK_MSG3(tmpBufnewip, newurl);
				}
				else
				{			
					char tmpBufapply[200];
					sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
					OK_MSG3(tmpBufapply ,str);
				}
				system("/bin/reboot.sh 3 &");
			}
			else{
				system("echo 1 > /tmp/show_reboot_info");
				REPLACE_MSG(str);
			}
	#ifdef _IQv2_
	}
	#endif
	return;

setErr_encrypt:
	ERR_MSG(wp, tmp);
}
#endif

int CONNECT_TEST(int type){ //EDX patrick add 20130501
	if(type==2) {
		connectIF=2;
		system("rm -f /tmp/apcli0IP 2> /dev/null");
		if(!ssid2scan_fail){
			SSIDexist=1;
			system("connectTest.sh apcli0 &");
		}
		else{	
			// The ssid dose not exist.
			SSIDexist=0;
			system("echo 'apcli0' > /tmp/ConnectTestIF");
		}
	}
	else if(type==5) {
		connectIF=5;
		system("rm -f /tmp/apclii0IP 2> /dev/null");
		if(!ssid2scan_fail5g){
			SSIDexist=1;
			system("connectTest.sh apclii0 &");
		}
		else{ 
			// The ssid dose not exist.			
			SSIDexist=0;
			system("echo 'apclii0' > /tmp/ConnectTestIF");
		}		
	}
	else if(type==3) {
		if(!ssid2scan_fail){
			SSIDexist=1;
			system("connectTest.sh wisp apcli0 &");
		}
		else{
			system("echo '0' > /tmp/wispConnect");
			SSIDexist=1;
		}
	}
	else if(type==4) {
		if(!ssid2scan_fail5g){
			SSIDexist=1;
			system("connectTest.sh wisp apclii0 &");
		}
		else{
			system("echo '0' > /tmp/wispConnect");
			SSIDexist=0;
		}
	}
	else if(type==7) { //5g connect_test hand
		if(!ssid2scan_fail5g){
			SSIDexist=1;
			system("connectTest.sh wisp apclii0 hand &");
		}
		else{
			system("echo '0' > /tmp/wispConnect");
			SSIDexist=0;
		}
	}
	else if(type==6) { //2.4g connect_test hand
		if(!ssid2scan_fail){
			SSIDexist=1;
			system("connectTest.sh wisp apcli0 hand &");
		}
		else{
			system("echo '0' > /tmp/wispConnect");
			SSIDexist=0;
		}
	}
	else return -1;
}

void formWlbasic(webs_t wp, char_t *path, char_t *query)  //EDX patrick add
{
	char_t *submitUrl, *strSSID, *strSSID1, *strChan, *strDisabled, *strBand,*strABand, *strAutoClone, *strVal, *strWisp, *strnextEnable ,*strSupportAllSecurity;
	char_t *strMode, *strMac1, *strMac2, *strMac3, *strMac4, *strMac5, *strMac6, *strWlanMac, *strWpsStatus;
	char_t *iqsetupclose, *isRepeaterBasicUI, *strWirelessDisable;
	int chan, disabled, mode, mv, val, wisp,txval, wlmodval;
	NETWORK_TYPE_T net;
	char tmpBuf[100];
	int intVal=0,intValWPA=0,intValWPA2=0;
	int tmp;

	char_t *wepKey;
	char_t *strKeyLen, *strFormat, *strKeyId, *strEnabled;
	char key[30];
	int enabled, keyLen, ret, i;
	WEP_T wep;

	char_t *strEncrypt, *strEnRadius, *strWpaCipher,*strWpa2Cipher, *str1xMode, *strDisabled1, *repUseCrossBand;
	ENCRYPT_T encrypt;
	int enableRS=0, getPSK=0, len, intTkip, intAes, intVal2;
	unsigned long reKeyTime;

	int intOne = 1, intZero = 0;
	char_t *use_apmode;
	use_apmode = websGetVar(wp, T("use_apmode"), T(""));
	#ifdef _TEXTFILE_CONFIG_
	if (use_apmode[0]) {
		apmib_set( MIB_IQSET_DISABLE, (void *)&intOne); 	// close iqsetup
		apmib_set( MIB_DHCP, (void *)&intZero);  	// DHCP_DISABLED=0, DHCP_CLIENT=1, DHCP_SERVER=2
		apmib_set( MIB_AP_MODE, (void *)&intZero);// 0=apmode
		apmib_set( MIB_WIZ_MODE, (void *)&intOne); // 0=router mode, 1=ap mode
		apmib_set( MIB_WAN_MODE, (void *)&intZero);
		apmib_set( MIB_OP_MODE, (void *)&intZero);
		#if defined(_DHCP_SWITCH_)
		apmib_set(MIB_DHCP_SWITCH, (void *)&intOne);  // 1=get IP from root AP, 0=static ip
		#endif
	}
	#endif
	//------------------- MSSID index ------------------//
	char_t *strM;
	int ssidIndex, enableMSSID=0;
	strM = websGetVar(wp, T("mssidSelect"), T(""));
	if(strM[0])
		ssidIndex = strtol( strM, (char **)NULL, 10);
	else
		ssidIndex = 0;

	strM = websGetVar(wp, T("enableMSSID"), T(""));
	strDisabled = websGetVar(wp, T("wlanDisabled"), T(""));
	strDisabled1 = websGetVar(wp, T("wlan1Enable"), T(""));

	printf("---------------------formWlbasic -------------------\n");
	printf("strDisabled=%s\n",strDisabled);
	//------------------ check SSID on/off  ------------------//
	if(ssidIndex == 0){

		apmib_get( MIB_WLAN_DISABLED, (void *)&disabled);

		if (strDisabled[0]) {
			if ( !gstrcmp(strDisabled, T("yes"))) disabled = 1;
			else disabled = 0;
		}
		if (strDisabled1[0]) {
			if ( !gstrcmp(strDisabled1, T("1"))) disabled = 0;
			else disabled = 1;
		}

		if ( apmib_set( MIB_WLAN_DISABLED, (void *)&disabled) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
			goto setErr_basic;
		}

		printf("ssidIndex=%d, SSIDDisable=%d\n",ssidIndex, disabled);
		
		if(disabled)
		{
			/*if ( apmib_set( MIB_WPS_ENABLE, (void *)&intZero) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
				goto setErr_basic;
			}
			if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&intZero) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
				goto setErr_basic;
			}*/
			goto setwlan_ret; // if disable, no need set, go to update flash
		}
		
	}
	else{
		//------------------ set  MSSID on/off 	------------------//
		#ifdef _MSSID_
		if(strM[0]) {
			if(!gstrcmp(strM, T("1"))) enableMSSID=1;
			else enableMSSID=0;

			if(enableMSSID) enableMSSID += ssidIndex;
			switch(ssidIndex)
			{
				case 1: apmib_set(MIB_WLAN_SSID_MIRROR_1, (void *)&enableMSSID); break;
				case 2: apmib_set(MIB_WLAN_SSID_MIRROR_2, (void *)&enableMSSID); break;
				case 3: apmib_set(MIB_WLAN_SSID_MIRROR_3, (void *)&enableMSSID); break;
				case 4: apmib_set(MIB_WLAN_SSID_MIRROR_4, (void *)&enableMSSID); break;
				default: break; 
			}
			printf("ssidIndex:%d  MIRROR:%d\n",ssidIndex, enableMSSID);
			if (!enableMSSID)
				goto setwlan_ret;   // if disable, no need set, go to update flash
		}
	
		#endif
	}


	printf("2.4G Wirelsss full setting !\n",ssidIndex);

	isRepeaterBasicUI = websGetVar(wp, T("isRepeaterBasicUI"), T(""));


	//------------------ WLAN VLAN ID ------------------//
	#ifdef _ENVLAN_
	strVal = websGetVar(wp, T("vlanid"), T(""));
	if(strVal[0])
	{
		val = strtol( strVal, (char **)NULL, 10);

		switch(ssidIndex)
		{
			case 1: apmib_set(MIB_WLAN_VLANID_1, (void *)&val); break;
			case 2: apmib_set(MIB_WLAN_VLANID_2, (void *)&val); break;
			case 3: apmib_set(MIB_WLAN_VLANID_3, (void *)&val); break;
			case 4: apmib_set(MIB_WLAN_VLANID_4, (void *)&val); break;
			default: break; 
		}
	}
	#endif

	//------------------ NOFORWARD on | off in same SSID ------------------//
	char_t *wireless_block_relay;
	#ifdef _WL_SSID_STA_ISOLATION_
	apmib_get(MIB_WIZ_MODE, (void *)&val);
	if(val == 1) //ap mode
	{
		wireless_block_relay = websGetVar(wp, T("IsolationEnabled"), T(""));
		if(wireless_block_relay[0])
			val = strtol( wireless_block_relay, (char **)NULL, 10);
	}
	else
	{
	#endif
		wireless_block_relay = websGetVar(wp, T("wireless_block_relay"), T(""));
		if(wireless_block_relay[0])
		{
			if( !strcmp(wireless_block_relay, "on")) val=1;
			else  val=0;
		}
	#ifdef _WL_SSID_STA_ISOLATION_
	}
	#endif
	
	if(wireless_block_relay[0]){
		#ifdef _MSSID_NOFORWARD_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_NOFORWARD, (void *)&val); break;
			case 1: apmib_set(MIB_WLAN_NOFORWARD1, (void *)&val); break;
			case 2: apmib_set(MIB_WLAN_NOFORWARD2, (void *)&val); break;
			case 3: apmib_set(MIB_WLAN_NOFORWARD3, (void *)&val); break;
			case 4: apmib_set(MIB_WLAN_NOFORWARD4, (void *)&val); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_NOFORWARD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("apmib set WLAN_NOFORWARD error !"));
			goto setErr_basic;
		}
		#endif
	}

	//------------------ set AP mode  ------------------//
	strMode = websGetVar(wp, T("apMode"), T(""));
	if (strMode[0]) {	
		errno=0;
		mode = strtol( strMode, (char **)NULL, 10);

		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup2))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set( MIB_AP_MODE, (void *)&mode) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup3))</script>"));
			goto setErr_basic;
		}
	}

	//------------------ set AP band  ------------------//
	strBand = websGetVar(wp, T("band"), T(""));
	if ( strBand[0] ) {	
		if (strBand[0] < '0' || strBand[0] > '6') {
  			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup4))</script>"));
			goto setErr_basic;
		}
		val = (strBand[0] - '0' + 1);
		if ( apmib_set( MIB_WLAN_BAND, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_basic;
		}
	}


#ifdef _WIFI_11N_STANDARD_
	if (val == 2) {
		apmib_get(MIB_WLAN_ENCRYPT, (void *)&intVal);
		if (intVal != ENCRYPT_WPA2) {
			intVal = ENCRYPT_DISABLED;
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);
			apmib_set(MIB_SECURITY_MODE, (void *)&intVal);
		}
		else {
			intVal = 0;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intVal);
			intVal = 2;
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intVal);
		}
	}
#endif


	//------------------ set AP Control sideband offset ------------------//
	#ifdef _AUTO_CHAN_SELECT_
	ssid2scan_fail5g=0;
	ssid2scan_fail=0;
	wep_used=0;
	#endif

	char_t *strChanWidth;
	strChanWidth = websGetVar(wp, T("secchan"), T(""));
	if (strChanWidth[0]) {
		#ifdef _AUTO_CHAN_SELECT_
		if ( strcmp(strChanWidth,"19") ) // fake value for manual config SSID
		{
		#endif
			printf("select secchan=%s\n",strChanWidth);
			if (!gstrcmp(strChanWidth, T("0"))) {val = 0;}
			else if (!gstrcmp(strChanWidth, T("1"))) {val = 1;}
			else if (!gstrcmp(strChanWidth, T("2"))) {val = 2;}
			else {
				DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
				goto setErr_basic;
			}
			if ( apmib_set(MIB_WLAN_N_CHANNEL_WIDTH, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Invalid Channel Width 1"));
				goto setErr_basic;
			}
		#ifdef _AUTO_CHAN_SELECT_
		}
		#endif
	}
	
	strChan = websGetVar(wp, T("chan"), T(""));
	if ( strChan[0] ) {
			printf("select chan=%s\n",strChan);
			errno=0;
			chan = strtol( strChan, (char **)NULL, 10);
			if (errno) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup16))</script>"));
				goto setErr_basic;
			}

			if ( apmib_set( MIB_WLAN_CHAN_NUM, (void *)&chan) == 0) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
				goto setErr_basic;
			}
	}

	//------------------ set hidden SSID ------------------//
	char_t *strHiddenSSID;
	strHiddenSSID = websGetVar(wp, T("hiddenSSID"), T(""));
	if (strHiddenSSID[0]) {	
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup20))</script>"));
			goto setErr_basic;
		}
	
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val); break;
			case 1: apmib_set(MIB_WLAN_HIDDEN_SSID_1, (void *)&val); break;
			case 2: apmib_set(MIB_WLAN_HIDDEN_SSID_2, (void *)&val); break;
			case 3: apmib_set(MIB_WLAN_HIDDEN_SSID_3, (void *)&val); break;
			case 4: apmib_set(MIB_WLAN_HIDDEN_SSID_4, (void *)&val); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_basic;
		}		
		#endif
	}
	strHiddenSSID = websGetVar(wp, T("hiddenSSID5g"), T(""));
	if (strHiddenSSID[0]) {	
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("get hiddenSSID5g fail!"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("set MIB_INIC_HIDDEN_SSID fail!"));
			goto setErr_basic;
		}		
	}


	#ifdef UNIVERSAL_REPEATER
	if(ssidIndex==0 && !isRepeaterBasicUI[0]){
		// Universal Repeater
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
		if (mode == 6 || mode == 2 ) {
			val = (mode == 2) ? 0 : 1;
			if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup6))</script>"));
				goto setErr_basic;
			}

			if (mode == 2) {
				strSSID = websGetVar(wp, T("ssid"), T(""));
			}
			else {
				strSSID = websGetVar(wp, T("repeaterSSID"), T(""));
			}

			if ( strSSID[0] ) {
				int ii;
				for(ii=MAX_SSID_LEN-1;ii>0 && isspace(strSSID[ii]) ;ii--)
				{
					strSSID[ii]='\0';
				}
				if ( apmib_set(MIB_REPEATER_SSID, (void *)strSSID) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
						goto setErr_basic;
				}
			}
		}
		else {
			val = 0;
			if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformStatsErrRep))</script>"));
			goto setErr_basic;
			}
		}

		if (mode == 2 || mode == 3 || mode == 4 || mode == 6 || wisp == 1) {
			apmib_get(MIB_WLAN_ENCRYPT, (void *)&tmp);
			if (tmp == 6) {
				val = 4;
				apmib_set(MIB_WLAN_ENCRYPT, (void *)&val);
				val = 0;
				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&val);
			}
		}

		//AP client mode type
		#ifndef _RESERVE_ENCRYPTION_SETTING_
		if (mode == 1 || mode ==2) {
			if (strMode[0]=='1')
				net = ADHOC;
			if (strMode[0]=='2')
				net = INFRASTRUCTURE;
			if ( apmib_set(MIB_WLAN_NETWORK_TYPE, (void *)&net) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
				goto setErr_basic;
			}
			//------------------------------------------------------------ Added by Tommy
			if (apmib_set( MIB_SECURITY_MODE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
				goto setErr_basic;
			}
			if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_WLAN_AUTH_TYPE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup12))</script>"));
				goto setErr_basic;
			}

			//------------------------------------------------------------
		}
		#endif
	}
	#endif
	// set auto MAC Clone
	strAutoClone = websGetVar(wp, T("autoMacClone"), T(""));
	if (strAutoClone[0]) {
		if (!gstrcmp(strAutoClone, T("no")))
			val = 0;
		else if (!gstrcmp(strAutoClone, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup13))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_AUTO_MAC_CLONE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup14))</script>"));
			goto setErr_basic;
		}
	}

	strSSID = websGetVar(wp, T("ssid"), T(""));
	repUseCrossBand = websGetVar(wp, T("repUseCrossBand"), T(""));
	if ( strSSID[0] && !repUseCrossBand[0]) {
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_SSID, (void *)strSSID); break;
			case 1: apmib_set(MIB_WLAN_SSID_1, (void *)strSSID); break;
			case 2: apmib_set(MIB_WLAN_SSID_2, (void *)strSSID); break;
			case 3: apmib_set(MIB_WLAN_SSID_3, (void *)strSSID); break;
			case 4: apmib_set(MIB_WLAN_SSID_4, (void *)strSSID); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_basic;
		}
		#endif
	}
	if ( strSSID[0] && repUseCrossBand[0]) {
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("MIB_INIC_SSID!"));
			goto setErr_basic;
		}
	}
	strSSID = websGetVar(wp, T("ssid5g"), T(""));
	if ( strSSID[0] ) {
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("set MIB_INIC_SSID fail!"));
			goto setErr_basic;
		}
	}

	if(ssidIndex==0){
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
		if ( mode == 1 || mode ==2 || mode==6 || wisp == 1 ) {
			strWlanMac = websGetVar(wp, T("wlanMac"), T(""));
			if (strWlanMac[0]) {
				if (strlen(strWlanMac)!=12 || !string_to_hex(strWlanMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup18))</script>"));
					goto setErr_basic;
				}
				if ( !apmib_set(MIB_WLAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup19))</script>"));
					goto setErr_basic;
				}

				if (strcmp(strWlanMac,"000000000000")) {
					for (mv=5; mv>=3; mv--) {
						tmpBuf[mv] = (tmpBuf[mv]-1);
						if ( (tmpBuf[mv]+1) != 0 )
							break;
					}
				}
				else {
					if (!string_to_hex("000000000000", tmpBuf, 12)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup20))</script>"));
						goto setErr_basic;
					}
				}

				if ( !apmib_set(MIB_ELAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup21))</script>"));
					goto setErr_basic;
				}
			}
		}
//**************************************** WDS setting ****************************************
		strMac1 = websGetVar(wp, T("wlLinkMac1"), T(""));
		if (strMac1[0]) {
			if (strlen(strMac1)!=12 || !string_to_hex(strMac1, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup22))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC1, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup23))</script>"));
				goto setErr_basic;
			}
		}
		strMac2 = websGetVar(wp, T("wlLinkMac2"), T(""));
		if (strMac2[0]) {
			if (strlen(strMac2)!=12 || !string_to_hex(strMac2, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup24))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC2, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup25))</script>"));
				goto setErr_basic;
			}
		}
		strMac3 = websGetVar(wp, T("wlLinkMac3"), T(""));
		if (strMac3[0]) {
			if (strlen(strMac3)!=12 || !string_to_hex(strMac3, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup26))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC3, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup27))</script>"));
				goto setErr_basic;
			}
		}
		strMac4 = websGetVar(wp, T("wlLinkMac4"), T(""));
		if (strMac4[0]) {
			if (strlen(strMac4)!=12 || !string_to_hex(strMac4, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup28))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC4, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup29))</script>"));
				goto setErr_basic;
			}
		}
		strMac5 = websGetVar(wp, T("wlLinkMac5"), T(""));
		if (strMac5[0]) {
			if (strlen(strMac5)!=12 || !string_to_hex(strMac5, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup30))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC5, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup31))</script>"));
				goto setErr_basic;
			}
		}
		strMac6 = websGetVar(wp, T("wlLinkMac6"), T(""));
		if (strMac6[0]) {
			if (strlen(strMac6)!=12 || !string_to_hex(strMac6, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup32))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC6, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup33))</script>"));
				goto setErr_basic;
			}
		}

	}

	 /*##################################################################################################*/
   /*######################################  Set security  ############################################*/
	 /*##################################################################################################*/


#if 1  //xxxxxxxxx

#ifdef _WEP_OPEN_SHARED_MODE_
	char_t *strWepMode;
#endif
  
	char_t *str_force_enable2g5g_ap;
	int force_enable2g5g_ap=0;
	str_force_enable2g5g_ap = websGetVar(wp, T("force_enable2g5g_ap"), T(""));
	if (str_force_enable2g5g_ap[0]) {
		force_enable2g5g_ap = strtol( str_force_enable2g5g_ap, (char **)NULL, 10);
		system("echo 1 > /tmp/force_enable2g5g_ap");
	}
	
	SUPP_NONWAP_T suppNonWPA;
	struct in_addr inIp;


	//Set security method
  strEncrypt = websGetVar(wp, T("method"), T(""));
	if (!strEncrypt[0]) {
 		//DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt1))</script>"));
		//goto setErr_basic;
		printf("skip set Encrypt!!\n");
		goto setwlan_ret;
	}
	encrypt = strEncrypt[0] - '0';

	#ifdef _MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_SECURITY_MODE, (void *)&encrypt); break;
		case 1: apmib_set(MIB_SECURITY_MODE_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_SECURITY_MODE_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_SECURITY_MODE_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_SECURITY_MODE_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_basic;
	}
	#endif

	if(force_enable2g5g_ap){
	  if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("set MIB_INIC_SECURITY_MODE fail!"));
		goto setErr_basic;
	  }
	}
	
	#if 0//defined(_Edimax_) || defined(_Customer_)
	//For Wlan WDS
	if (apmib_set( MIB_WLAN_WDS_ENCRYPT, (void *)&encrypt) == 0) {
  	DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}
	#endif

	//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
	strSupportAllSecurity=websGetVar(wp, T("supportAllSecurity"), T(""));
	if(!strSupportAllSecurity[0])
	{
		//Set 802.1x Enable
		 strEnRadius = websGetVar(wp, T("enRadius"), T(""));
		if ( ((encrypt==0 || encrypt==1) && !gstrcmp(strEnRadius, T("ON"))) || encrypt==3) {
			intVal = 1;
			enableRS = 1;
		}
		else {intVal = 0;}
	
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal); break;
			case 1: apmib_set( MIB_WLAN_ENABLE_1X_1, (void *)&intVal); break;
			case 2: apmib_set( MIB_WLAN_ENABLE_1X_2, (void *)&intVal); break;
			case 3: apmib_set( MIB_WLAN_ENABLE_1X_3, (void *)&intVal); break;
			case 4: apmib_set( MIB_WLAN_ENABLE_1X_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
			goto setErr_basic;
		}
		#endif
	}



#ifndef HOME_GATEWAY
	//set internal or external RADIUS
	str1xMode = websGetVar(wp, T("sel1xMode"), T(""));
	if (str1xMode[0]) {
		if (!gstrcmp(str1xMode, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set(MIB_1X_MODE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set 802.1x Mode failed!"));
			goto setErr_basic;
		}
	}
#endif

	// support nonWPA client
	strVal = websGetVar(wp, T("nonWpaSupp"), T(""));
	if (strVal[0]) {
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_WLAN_ENABLE_SUPP_NONWPA, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt2))</script>"));
			goto setErr_basic;
		}
		if ( intVal ) {
			suppNonWPA = SUPP_NONWPA_NONE;
			strVal = websGetVar(wp, T("nonWpaWep"), T(""));
			if ( !gstrcmp(strVal, T("ON")))
				suppNonWPA |= SUPP_NONWPA_WEP;

			strVal = websGetVar(wp, T("nonWpa1x"), T(""));
			if ( !gstrcmp(strVal, T("ON"))) {
				suppNonWPA |= SUPP_NONWPA_1X;
				enableRS = 1;
			}
			if ( apmib_set( MIB_WLAN_SUPP_NONWPA, (void *)&suppNonWPA) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt3))</script>"));
				goto setErr_basic;
			}
		}
	}
	
	//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
	if(!strSupportAllSecurity[0])
	{
		if (encrypt==2 || encrypt==3) {
			// WPA authentication
			if (encrypt==2) {
				intVal = WPA_AUTH_PSK; //2
				getPSK = 1;
			}
			else if (encrypt==3) {
	//			intVal = WPA_AUTH_AUTO;
				intVal = 1;
				enableRS = 1;
			}

			if(ssidIndex==0){
				if ( apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
					goto setErr_basic;
				}
			}

			// cipher suite
			strVal = websGetVar(wp, T("wpaCipher"), T(""));
			if (strVal[0]) {
				intVal = strVal[0] - '0';
				if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;
				//WPA_CIPHER_TKIP=1, WPA_CIPHER_AES=2, WPA_CIPHER_MIXED=3 

				#ifdef _MSSID_
				printf("ssidIndex:%d intTkip:%d intAes:%d\n",ssidIndex, intTkip ,intAes);
				switch(ssidIndex)
				{
					case 0: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes); break;
					case 1: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_1, (void *)&intAes); break;
					case 2: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_2, (void *)&intAes); break;
					case 3: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_3, (void *)&intAes); break;
					case 4: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_4, (void *)&intAes); break;
					default: break; 
				}
				#else
				if ( apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				if ( apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				#endif
				#if 0//defined(_Edimax_) || defined(_Customer_)
				//For Wlan wds
				if ( apmib_set(MIB_WLAN_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_encrypt;
				}
				if ( apmib_set(MIB_WLAN_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_encrypt;
				}
				#endif

			}
		} //if (encrypt==2 || encrypt==3)

		//Follow Realtek, Set WLAN_ENCRYPT
		strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));
	//strEncrypt=method
	}
	
	#ifdef _AUTO_CHAN_SELECT_
	#ifdef __TARGET_BOARD__
	if(strEncrypt[0]=='0' && ssidIndex==0 ){
		sprintf(tmpBuf, "echo '' > /tmp/ConnectTestKey", wepKey);
		system(tmpBuf);
		printf("%s\n",tmpBuf);
	}
	#endif
	#endif

	//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
	if(strSupportAllSecurity[0]){
		if (strEncrypt[0]=='0')	 //Disable
			encrypt=0;			
		else if (strEncrypt[0]=='1') //WEP
			encrypt=1; 
		else if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA
		{
			getPSK = 1;
			strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));
			if (strWpaCipher[0]) 
			{
				intValWPA = strWpaCipher[0] - '0';
				if (intValWPA==1)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intValWPA==2) intAes=WPA_CIPHER_AES; else intAes=0;

				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);
				
				if(force_enable2g5g_ap){
				  apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip);
				  apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);
				}
			}
			if (strEncrypt[0]=='2') encrypt=2;
			else 	encrypt=4;
		}
		/*else if (strEncrypt[0]=='3') //WPA2
		{
			getPSK = 1;
			strWpa2Cipher = websGetVar(wp, T("wpa2Cipher"), T(""));
			if (strWpa2Cipher[0]) 
			{
				//intValWPA=0;
				//intValWPA2 = strWpa2Cipher[0] - '0';
				intValWPA = strWpaCipher[0] - '0';
				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);
			}
			encrypt=4;
		}*/
		else if (strEncrypt[0]=='4') //WPA RADIUS
			encrypt=2;	
		else if (strEncrypt[0]=='5') //WPA2 RADIUS
			encrypt=4;

		//printf("SECURITY_MODE=%s WLAN_ENCRYPT=%d getPSK=%d\n",strEncrypt,encrypt,getPSK);
	}
	else
	{
		if (strEncrypt[0]=='0')	encrypt=0;
		else if (strEncrypt[0]=='1') encrypt=1;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='1')) encrypt=2;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='2')) encrypt=4;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='3')) encrypt=6;
	}	
	

	#ifdef _MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);break;
		case 1: apmib_set(MIB_WLAN_ENCRYPT_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_WLAN_ENCRYPT_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_WLAN_ENCRYPT_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_WLAN_ENCRYPT_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
		goto setErr_basic;
	}
	#endif
	
	if(force_enable2g5g_ap){
	  if (apmib_set( MIB_INIC_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("set MIB_INIC_ENCRYPT fail!"));
		goto setErr_basic;
	  }
	}
//ENCRYPT_DISABLED=0, ENCRYPT_WEP=1, ENCRYPT_PSK=3, ENCRYPT_ARS=4
//*********************************** Wep setting ***********************************
	if (encrypt==ENCRYPT_WEP) {
		strEnabled = websGetVar(wp, T("wepEnabled"), T(""));
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( enabled ) {
			strKeyLen = websGetVar(wp, T("length"), T(""));
			if (!strKeyLen[0]) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt2))</script>"));
				goto setErr_basic;
			}
			if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt3))</script>"));
				goto setErr_basic;
			}
			if (strKeyLen[0] == '1')
				wep = WEP64;
			else if (strKeyLen[0] == '2')
				wep = WEP128;
			else
				wep = WEP152;
		}
		else
			wep = WEP_DISABLED;

		//WEP_DISABLED=0, WEP64=1, WEP128=2, WEP152=3
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WEP, (void *)&wep); break;
			case 1: apmib_set(MIB_WLAN_WEP_1, (void *)&wep); break;
			case 2: apmib_set(MIB_WLAN_WEP_2, (void *)&wep); break;
			case 3: apmib_set(MIB_WLAN_WEP_3, (void *)&wep); break;
			case 4: apmib_set(MIB_WLAN_WEP_4, (void *)&wep); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt5))</script>"));
			goto setErr_basic;
		}
		#endif
		
		if(force_enable2g5g_ap){
		  if ( apmib_set( MIB_INIC_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("set MIB_INIC_WEP fail!"));
			goto setErr_basic;
		  }
		}
		
		if (wep == WEP_DISABLED)
			goto setwlan_ret;

		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt5))</script>"));
			goto setErr_basic;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt6))</script>"));
			goto setErr_basic;
		}

		i = strFormat[0] - '0' - 1;

		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i); break;
			case 1: apmib_set( MIB_WLAN_WEP_KEY_TYPE_1, (void *)&i); break;
			case 2: apmib_set( MIB_WLAN_WEP_KEY_TYPE_2, (void *)&i); break;
			case 3: apmib_set( MIB_WLAN_WEP_KEY_TYPE_3, (void *)&i); break;
			case 4: apmib_set( MIB_WLAN_WEP_KEY_TYPE_4, (void *)&i); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_basic;
		}
		#endif
		
		if(force_enable2g5g_ap){
		  if ( apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("set MIB_INIC_WEP_KEY_TYPE fail!"));
			goto setErr_basic;
		  }
		}
		
		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;   //WEP64_KEY_LEN=5
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;  //WEP128_KEY_LEN=13
			else
				keyLen = WEP128_KEY_LEN*2;
		}
		else {
			if (strFormat[0]=='1')
				keyLen = WEP152_KEY_LEN;
			else
				keyLen = WEP152_KEY_LEN*2;
		}
		strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt8))</script>"));
				goto setErr_basic;
			}
			i = strKeyId[0] - '0' - 1;

			#ifdef _MSSID_
			switch(ssidIndex)
			{
				case 0: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i); break;
				case 1: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_1, (void *)&i); break;
				case 2: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_2, (void *)&i); break;
				case 3: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_3, (void *)&i); break;
				case 4: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_4, (void *)&i); break;
				default: break; 
			}
			#else
			if ( !apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_basic;
			}
			#endif
			if(force_enable2g5g_ap){
			    if ( !apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("SET MIB_INIC_WEP_DEFAULT_KEY FAIL"));
				goto setErr_basic;
			  }
			}
			
		}


		wepKey = websGetVar(wp, T("key1"), T(""));

		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_basic;
			}
			if ( !isAllStar(wepKey) ) {
			
				#ifdef _AUTO_CHAN_SELECT_
				char tmpWepKey[200];
				char buildWepKey[200]={0};
				strcpy(tmpWepKey,wepKey);
				strcpy(ConnectTestKey, wepKey);
				wep_used=1;
				//CharFilter0(tmpWepKey);
				#ifdef __TARGET_BOARD__
				if(ssidIndex==0){
					for(i=0; i<strlen(tmpWepKey); i++) //EDX, Ken 2015_09_16 add, Avoid tmpWepKey is "wepKey;systemcode".
					{
						sprintf(buildWepKey, "%s%X", buildWepKey, tmpWepKey[i]);
					}
					sprintf(tmpBuf, "echo %s > /tmp/ConnectTestKey",(void *)buildWepKey);//Robert 2014_01_06
					system(tmpBuf);
					printf("%s\n",tmpBuf);
				}	
				#endif
				#endif

				printf("ssidIndex=%d, 2.4g WEP len(1:64 bit, 2:128 bit)=%d Format(1:ascii, 2:hex)=%s\n",ssidIndex,wep,strFormat);
				if (strFormat[0] == '1') // ascii
				{
					#ifdef _TEXTFILE_CONFIG_
					if ( !apmib_set(MIB_TMPSTRING, (void *)wepKey)) {
						strcpy(tmpBuf, T("MIB_TMPSTRING error!")); 
						goto setErr_basic;
					}
					#endif
					strcpy(key, wepKey);
				}
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_basic;
					}
				}

				#ifdef _MSSID_
				switch(ssidIndex)
				{
					case 0: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
						else ret=0;
						break;
					case 1: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_1, (void *)key);
						else ret=0;
						break;
					case 2: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_2, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_2, (void *)key);
						else ret=0;
						break;
					case 3: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_3, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_3, (void *)key);
						else ret=0;
						break;
					case 4: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_4, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_4, (void *)key);
						else ret=0;
						break;
					default: break; 
				}
				#else
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
				else
					ret=0;
				#endif
					
					
				if(force_enable2g5g_ap){
				  if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
				  else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
				  else
					ret=0;
				}
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
					goto setErr_basic;
				}
			}
		}

	} //if (encrypt==ENCRYPT_WEP) {

//*********************************** pre-shared key setting ***********************************

	if ( getPSK ) {
		int oldFormat, oldPskLen, i;
		
		//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
		if(strSupportAllSecurity[0])
		{			
			if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA
				strVal = websGetVar(wp, T("wpaPskFormat"), T(""));
			/*else if (strEncrypt[0]=='3') //WPA2
			{
				strVal = websGetVar(wp, T("wpa2PskFormat"), T(""));
			}*/
		}
		else
		{
			strVal = websGetVar(wp, T("pskFormat"), T(""));
		}

		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt22))</script>"));
			goto setErr_basic;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt23))</script>"));
			goto setErr_basic;
		}

		// remember current psk format and length to compare to default case "****"
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf); break;
			case 1: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_1, (void *)tmpBuf); break;
			case 2: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_2, (void *)tmpBuf); break;
			case 3: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_3, (void *)tmpBuf); break;
			case 4: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_4, (void *)tmpBuf); break;
			default: break; 
		}
		#else
		apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf);
		#endif
		
		oldPskLen = strlen(tmpBuf);

		//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
		if(strSupportAllSecurity[0]){			
			if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA		
				strVal = websGetVar(wp, T("wpaPskValue"), T(""));
			/*else if (strEncrypt[0]=='3') //WPA2
			{		
				strVal = websGetVar(wp, T("wpa2PskValue"), T(""));
			}*/
		}
		else{
			strVal = websGetVar(wp, T("pskValue"), T(""));
		}
		

		len = strlen(strVal);
		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto rekey_time;
		}

		#ifdef _AUTO_CHAN_SELECT_
		if(strVal[0])
		{
			char tmpStrVal[200];
			char buildstrVal[200]={0};
			strcpy(tmpStrVal, strVal);
			strcpy(ConnectTestKey, strVal);
			CharFilter0(tmpStrVal);
			if(ssidIndex==0){
				#ifdef __TARGET_BOARD__
				for(i=0; i<strlen(tmpStrVal); i++) //EDX, Ken 2015_09_07 add, Avoid tmpWepKey is "wepKey;systemcode".
				{
					sprintf(buildstrVal, "%s%X", buildstrVal, tmpStrVal[i]);
				}
				sprintf(tmpBuf, "echo \"%s\" > /tmp/ConnectTestKey",(void *)buildstrVal); //Robert 2014_01_06
				system(tmpBuf);
				printf("%s\n",tmpBuf);
				#endif
			}
		}
		#endif

		//printf("ssidIndex=%d, 2.4g pskformate=%d\n",ssidIndex,intVal);
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal); break;
			case 1: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&intVal); break;
			case 2: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&intVal); break;
			case 3: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&intVal); break;
			case 4: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_basic;
		}
		#endif
		
		
		if(force_enable2g5g_ap){
		  if ( apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("set MIB_INIC_WPA_PSK_FORMAT fail!"));
			goto setErr_basic;
		  }
		}
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan WDS
		if ( apmib_set(MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}
		#endif

		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_basic;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_basic;
			}
		}
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal); break;
			case 1: apmib_set(MIB_WLAN_WPA_PSK_1, (void *)strVal); break;
			case 2: apmib_set(MIB_WLAN_WPA_PSK_2, (void *)strVal); break;
			case 3: apmib_set(MIB_WLAN_WPA_PSK_3, (void *)strVal); break;
			case 4: apmib_set(MIB_WLAN_WPA_PSK_4, (void *)strVal); break;
			default: break; 
		}
		#else
		if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_basic;
		}
		#endif
		
		if(force_enable2g5g_ap){
		   if ( !apmib_set(MIB_INIC_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("set MIB_INIC_WPA_PSK fail!"));
			goto setErr_basic;
		    } 
		}
		
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan wds
		if ( !apmib_set(MIB_WLAN_WDS_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
		#endif


	}


rekey_time:
		
//*********************************** RADIUS Server setting ***********************************
	if (enableRS == 1) { // if 1x enabled, get RADIUS server info
		#ifndef HOME_GATEWAY
		 if (!str1xMode[0]) {
		 #endif
		// set Radius Port
		strVal = websGetVar(wp, T("radiusPort"), T(""));
		if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt7))</script>"));
				goto setErr_basic;
			}
			if (!string_to_dec(strVal, &intVal) || intVal<=0 || intVal>65535) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt8))</script>"));
				goto setErr_basic;
			}
	
			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_PORT, (void *)&intVal); break;
				case 1: apmib_set(MIB_WLAN_RS_PORT_1, (void *)&intVal); break;
				case 2: apmib_set(MIB_WLAN_RS_PORT_2, (void *)&intVal); break;
				case 3: apmib_set(MIB_WLAN_RS_PORT_3, (void *)&intVal); break;
				case 4: apmib_set(MIB_WLAN_RS_PORT_4, (void *)&intVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_PORT, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt9))</script>"));
				goto setErr_basic;
			}
			#endif
		//Radius Ip
			strVal = websGetVar(wp, T("radiusIP"), T(""));
			if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt10))</script>"));
				goto setErr_basic;
			}
			if ( !inet_aton(strVal, &inIp) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt11))</script>"));
				goto setErr_basic;
			}

			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_IP, (void *)&inIp); break;
				case 1: apmib_set(MIB_WLAN_RS_IP_1, (void *)&inIp); break;
				case 2: apmib_set(MIB_WLAN_RS_IP_2, (void *)&inIp); break;
				case 3: apmib_set(MIB_WLAN_RS_IP_3, (void *)&inIp); break;
				case 4: apmib_set(MIB_WLAN_RS_IP_4, (void *)&inIp); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_IP, (void *)&inIp)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt12))</script>"));
				goto setErr_basic;
			}
			#endif
			//Radius password
			strVal = websGetVar(wp, T("radiusPass"), T(""));
			if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt13))</script>"));
				goto setErr_basic;
			}

			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal); break;
				case 1: apmib_set(MIB_WLAN_RS_PASSWORD_1, (void *)strVal); break;
				case 2: apmib_set(MIB_WLAN_RS_PASSWORD_2, (void *)strVal); break;
				case 3: apmib_set(MIB_WLAN_RS_PASSWORD_3, (void *)strVal); break;
				case 4: apmib_set(MIB_WLAN_RS_PASSWORD_4, (void *)strVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_basic;
			}
			#endif
			/*int session_time = 5;
			if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME, (void *)&session_time)) {   //session_timeout
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_encrypt;
			}

			session_time = 5;
			if ( !apmib_set(MIB_WLAN_RS_INTERVAL_TIME, (void *)&session_time)) {   //idle_timeout
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_encrypt;
			}*/

		#ifndef HOME_GATEWAY
		}
		#endif

get_wepkey:
		// get 802.1x WEP key length
		strVal = websGetVar(wp, T("wepKeyLen"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("wep64")))
				intVal = WEP64;
			else if ( !gstrcmp(strVal, T("wep128")))
				intVal = WEP128;
			else {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt15))</script>"));
				goto setErr_basic;
			}
			if ( apmib_set(MIB_WLAN_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_basic;
			}
		}
	}

//*********************************** WPS_CONFIG setting ***********************************


	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
	//printf("--> 2.4 wpsStatus:%s\n", strWpsStatus);
	if (strWpsStatus[0] && !gstrcmp(strWpsStatus, T("1"))){
		val = 1;
		if ( apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_WPS_CONFIG_STATUS, (void *)&val) == 0)
		{
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_basic;
		}
		val = 0;
		if ( apmib_set(MIB_WPS_DISPLAY_KEY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
	}



setwlan_ret:

	#ifdef _IQv2_
	//char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);
	submitUrl = websGetVar(wp, T("submit-url"), T("")); //xxxxxxxxx


	if (!gstrcmp(submitUrl, T("/wiz_rep5g.asp"))){
		
		strSSID = websGetVar(wp, T("ssid"), T(""));		
		if(strSSID[0])
			system("echo \"1\" > /tmp/wifi_changed");
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh 2> /dev/null");  // for iQsetup scan ssid
		system("/bin/up_wlan.sh &");
		#endif
	}


#ifdef _AUTO_CHAN_SELECT_
	char_t *strCONNTEST;
	strCONNTEST = websGetVar(wp, T("doConnectTest"), T(""));
	if ( strCONNTEST[0] && (!gstrcmp(strCONNTEST, T("1"))) ) { //if CONN_TEST_TRUE=0 means dont do connect test

		strVal = websGetVar(wp, T("rootAPmac"), T(""));		
		if(strVal[0]){
			for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "UserName;systemcode".
			{
				if (strVal[i]==59)
				{
					strVal[i]='\0';
					break;
				}
			}
			sprintf(tmpBuf, "echo '%s' > /tmp/apcli0_rootMAC", strVal);
			system(tmpBuf);
		}

		system("killall connectTest.sh 2> /dev/null");
		system("rm -f /tmp/connectTestdone 2> /dev/null");

		int pid;
		pid = fork();
		if (pid) {
			waitpid(pid, NULL, 0);
			sleep(3);
		}
		else if (pid == 0) {
			if (submitUrl[0])
			{
				REPLACE_MSG(submitUrl);
			}
			exit(0);
		}
		CONNECT_TEST(2);	//2=wlan1 test 5=wlan0 test 7=wlan0+wlan1 test
	}
	else{
#endif
		char_t *strApply;
		char tmpBufapply[200];
		strApply = websGetVar(wp, T("isApply"),T(""));


		if(strApply[0]){

			if (lanipChanged == 1) {
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBuf, newurl);
			}
			else
			{			
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			system("echo 1 > /tmp/show_reboot_info");
			REPLACE_MSG(submitUrl);
		}

#ifdef _AUTO_CHAN_SELECT_
	}
#endif
	return;

setErr_basic:
	ERR_MSG(wp, tmpBuf);
	return;
#endif
}  //formWlbasic end
void formWlEnableSwitch(webs_t wp, char_t *path, char_t *query)
{
  char_t *submitUrl, *strWirelessEnable, *strWirelessDisable, *strnextEnable;
	int val,i;
	char tmpBuf[100];
	FILE *fp;

	#if 1  //EW7438AC Sleep mode support
	char_t  *sleep_time;
	sleep_time = websGetVar(wp, T("sleep_time_length"), T(""));
	if(sleep_time[0]){
		REDIRECT_PAGE(wp, "/aIndex.asp");
		for(i=0; i<strlen(sleep_time); i++)//EDX, Ken 2015_09_16 add, Avoid sleep_time is "sleep_time;systemcode".
		{
			if (sleep_time[i]==59)
			{
				sleep_time[i]='\0';
				break;
			}
		}
		val= atoi(sleep_time);
		sprintf(tmpBuf, "echo %d > /tmp/sleep_time_length", val);
		system(tmpBuf);
		system("/bin/sleepmode.sh &");
		//submitUrl = websGetVar(wp, T("submit-url"), T(""));
		return;
	}
	#endif
	//------------------- MSSID index ------------------//
	char_t *strM;
	int ssidIndex, enableMSSID;
	strM = websGetVar(wp, T("mssidSelect"), T(""));
	if(strM[0])
		ssidIndex = strtol( strM, (char **)NULL, 10);
	else
		ssidIndex = 0;

	strWirelessDisable = websGetVar(wp, T("wlDisabled"), T(""));

	if (strWirelessDisable[0]) {
		
		if (!gstrcmp(strWirelessDisable, T("yes")))
			val = (ssidIndex != 0) ? 0 : 1;
		else if (!gstrcmp(strWirelessDisable, T("no")))
			val = (ssidIndex != 0) ? 1 : 0;
		else {
			DEBUGP(tmpBuf, T("apmib set WLAN_DISABLED Error!"));
			goto setErr_advance;
		}
		
		printf("2.4g ssidIndex=%d, val=%d\n", ssidIndex, val);
		
		#ifdef _MSSID_
		if(ssidIndex == 0)
			apmib_set(MIB_WLAN_DISABLED, (void *)&val);
		else{
			if(val) val += ssidIndex;
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_DISABLED, (void *)&val); break;
				case 1: apmib_set(MIB_WLAN_SSID_MIRROR_1, (void *)&val); break;
				case 2: apmib_set(MIB_WLAN_SSID_MIRROR_2, (void *)&val); break;
				case 3: apmib_set(MIB_WLAN_SSID_MIRROR_3, (void *)&val); break;
				case 4: apmib_set(MIB_WLAN_SSID_MIRROR_4, (void *)&val); break;
				default: break; 
			}
		}
		#else
		if ( apmib_set(MIB_WLAN_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_advance;
		}
		#endif
	}
	apmib_update(CURRENT_SETTING);

	submitUrl = websGetVar(wp, T("submit-url"), T(""));
	REDIRECT_PAGE(wp, submitUrl);

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}

#ifdef _WIFI_ROMAING_
void formWlbasicREP(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strSSID, *strSSID1, *strChan, *strDisabled, *strBand,*strABand, *strAutoClone, *strVal, *strWisp, *strnextEnable ,*strSupportAllSecurity;
	char_t *strMode, *strMac1, *strMac2, *strMac3, *strMac4, *strMac5, *strMac6, *strWlanMac, *strWpsStatus;
	char_t *iqsetupclose, *strWirelessDisable;
	int chan, disabled, mode, mv, val, wisp,txval, wlmodval;
	NETWORK_TYPE_T net;
	char tmpBuf[100];
	int intVal=0,intValWPA=0,intValWPA2=0;
	int tmp;

	char_t *wepKey;
	char_t *strKeyLen, *strFormat, *strKeyId, *strEnabled;
	char key[30];
	int enabled, keyLen, ret, i;
	WEP_T wep;

	char_t *strEncrypt, *strEnRadius, *strWpaCipher,*strWpa2Cipher, *str1xMode, *strDisabled1, *repUseCrossBand;
	ENCRYPT_T encrypt;
	int enableRS=0, getPSK=0, len, intTkip, intAes, intVal2;
	unsigned long reKeyTime;

	//------------------- MSSID index ------------------//
	int ssidIndex, enableMSSID=0;
	ssidIndex = 0;
	printf("---------------------formWlbasicREP -------------------\n");

	//------------------ set AP mode  ------------------//
	strMode = websGetVar(wp, T("apMode"), T(""));
	if (strMode[0]) {	
		errno=0;
		mode = strtol( strMode, (char **)NULL, 10);

		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup2))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set( MIB_AP_MODE, (void *)&mode) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup3))</script>"));
			goto setErr_basic;
		}
	}

	//------------------ set AP band  ------------------//
	strBand = websGetVar(wp, T("band"), T(""));
	if ( strBand[0] ) {	
		if (strBand[0] < '0' || strBand[0] > '6') {
  			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup4))</script>"));
			goto setErr_basic;
		}
		val = (strBand[0] - '0' + 1);
		if ( apmib_set( MIB_WLAN_BAND, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_basic;
		}
	}


#ifdef _WIFI_11N_STANDARD_
	if (val == 2) {
		apmib_get(MIB_WLAN_ENCRYPT, (void *)&intVal);
		if (intVal != ENCRYPT_WPA2) {
			intVal = ENCRYPT_DISABLED;
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);
			apmib_set(MIB_SECURITY_MODE, (void *)&intVal);
		}
		else {
			intVal = 0;
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intVal);
			intVal = 2;
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intVal);
		}
	}
#endif


	//------------------ set AP Control sideband offset ------------------//
	#ifdef _AUTO_CHAN_SELECT_
	ssid2scan_fail5g=0;
	ssid2scan_fail=0;
	wep_used=0;
	#endif
	strChan = websGetVar(wp, T("chan"), T(""));
	if ( strChan[0] ) {
		printf("select chan=%s\n",strChan);
		errno=0;
		chan = strtol( strChan, (char **)NULL, 10);
		if (errno) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup16))</script>"));
			goto setErr_basic;
		}

		if ( apmib_set( MIB_WLAN_CHAN_NUM, (void *)&chan) == 0) {
	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
			goto setErr_basic;
		}
	}

	//------------------ set hidden SSID ------------------//
	char_t *strHiddenSSID;
	strHiddenSSID = websGetVar(wp, T("hiddenSSID"), T(""));
	if (strHiddenSSID[0]) {
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup20))</script>"));
			goto setErr_basic;
		}

		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_basic;
		}

	}


	#ifdef UNIVERSAL_REPEATER
	if(ssidIndex==0){
		// Universal Repeater
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
		if (mode == 6 || mode == 2 ) {
			val = (mode == 2) ? 0 : 1;
			if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup6))</script>"));
				goto setErr_basic;
			}

			if (mode == 2) {
				strSSID = websGetVar(wp, T("ssid"), T(""));
			}
			else {
				strSSID = websGetVar(wp, T("repeaterSSID"), T(""));
			}

			if ( strSSID[0] ) {
				int ii;
				for(ii=MAX_SSID_LEN-1;ii>0 && isspace(strSSID[ii]) ;ii--)
				{
					strSSID[ii]='\0';
				}
				if ( apmib_set(MIB_REPEATER_SSID, (void *)strSSID) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
						goto setErr_basic;
				}
			}
		}
		else {
			val = 0;
			if ( apmib_set( MIB_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformStatsErrRep))</script>"));
			goto setErr_basic;
			}
		}

		if (mode == 2 || mode == 3 || mode == 4 || mode == 6 || wisp == 1) {
			apmib_get(MIB_WLAN_ENCRYPT, (void *)&tmp);
			if (tmp == 6) {
				val = 4;
				apmib_set(MIB_WLAN_ENCRYPT, (void *)&val);
				val = 0;
				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&val);
			}
		}

		//AP client mode type
		#ifndef _RESERVE_ENCRYPTION_SETTING_
		if (mode == 1 || mode ==2) {
			if (strMode[0]=='1')
				net = ADHOC;
			if (strMode[0]=='2')
				net = INFRASTRUCTURE;
			if ( apmib_set(MIB_WLAN_NETWORK_TYPE, (void *)&net) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
				goto setErr_basic;
			}
			//------------------------------------------------------------ Added by Tommy
			if (apmib_set( MIB_SECURITY_MODE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_WLAN_ENABLE_1X, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
				goto setErr_basic;
			}
			if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_WLAN_AUTH_TYPE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup12))</script>"));
				goto setErr_basic;
			}

			//------------------------------------------------------------
		}
		#endif
	}
	#endif

	strSSID = websGetVar(wp, T("ssid"), T(""));
	repUseCrossBand = websGetVar(wp, T("repUseCrossBand"), T(""));
	if ( strSSID[0] && !repUseCrossBand[0]) {
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_SSID, (void *)strSSID); break;
			case 1: apmib_set(MIB_WLAN_SSID_1, (void *)strSSID); break;
			case 2: apmib_set(MIB_WLAN_SSID_2, (void *)strSSID); break;
			case 3: apmib_set(MIB_WLAN_SSID_3, (void *)strSSID); break;
			case 4: apmib_set(MIB_WLAN_SSID_4, (void *)strSSID); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_basic;
		}
		#endif
	}
	if ( strSSID[0] && repUseCrossBand[0]) {
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("MIB_INIC_SSID!"));
			goto setErr_basic;
		}
	}
	
	

	if(ssidIndex==0){
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
//**************************************** WDS setting ****************************************
		strMac1 = websGetVar(wp, T("wlLinkMac1"), T(""));
		if (strMac1[0]) {
			if (strlen(strMac1)!=12 || !string_to_hex(strMac1, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup22))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC1, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup23))</script>"));
				goto setErr_basic;
			}
		}
		strMac2 = websGetVar(wp, T("wlLinkMac2"), T(""));
		if (strMac2[0]) {
			if (strlen(strMac2)!=12 || !string_to_hex(strMac2, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup24))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC2, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup25))</script>"));
				goto setErr_basic;
			}
		}
		strMac3 = websGetVar(wp, T("wlLinkMac3"), T(""));
		if (strMac3[0]) {
			if (strlen(strMac3)!=12 || !string_to_hex(strMac3, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup26))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC3, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup27))</script>"));
				goto setErr_basic;
			}
		}
		strMac4 = websGetVar(wp, T("wlLinkMac4"), T(""));
		if (strMac4[0]) {
			if (strlen(strMac4)!=12 || !string_to_hex(strMac4, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup28))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC4, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup29))</script>"));
				goto setErr_basic;
			}
		}
		strMac5 = websGetVar(wp, T("wlLinkMac5"), T(""));
		if (strMac5[0]) {
			if (strlen(strMac5)!=12 || !string_to_hex(strMac5, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup30))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC5, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup31))</script>"));
				goto setErr_basic;
			}
		}
		strMac6 = websGetVar(wp, T("wlLinkMac6"), T(""));
		if (strMac6[0]) {
			if (strlen(strMac6)!=12 || !string_to_hex(strMac6, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup32))</script>"));
				goto setErr_basic;
			}
			if ( !apmib_set(MIB_WL_LINKMAC6, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup33))</script>"));
				goto setErr_basic;
			}
		}

	}

	/*##################################################################################################*/
	/*######################################  Set security  ############################################*/
	/*##################################################################################################*/


#if 1  //xxxxxxxxx

#ifdef _WEP_OPEN_SHARED_MODE_
	char_t *strWepMode;
#endif
  
	SUPP_NONWAP_T suppNonWPA;
	struct in_addr inIp;


	//Set security method
	strEncrypt = websGetVar(wp, T("method"), T(""));
	if (!strEncrypt[0]) {
 		//DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt1))</script>"));
		//goto setErr_basic;
		printf("skip set Encrypt!!\n");
		goto setwlan_ret;
	}
	encrypt = strEncrypt[0] - '0';
printf("strEncrypt=%s\n",strEncrypt);
	#ifdef _MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_SECURITY_MODE, (void *)&encrypt); break;
		case 1: apmib_set(MIB_SECURITY_MODE_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_SECURITY_MODE_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_SECURITY_MODE_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_SECURITY_MODE_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_basic;
	}
	#endif


#ifndef HOME_GATEWAY
	//set internal or external RADIUS
	str1xMode = websGetVar(wp, T("sel1xMode"), T(""));
	if (str1xMode[0]) {
		if (!gstrcmp(str1xMode, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set(MIB_1X_MODE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set 802.1x Mode failed!"));
			goto setErr_basic;
		}
	}
#endif

	// support nonWPA client
	strVal = websGetVar(wp, T("nonWpaSupp"), T(""));
	
	//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
	strSupportAllSecurity=websGetVar(wp, T("supportAllSecurity"), T(""));
	if(!strSupportAllSecurity[0])
	{
		if (encrypt==2 || encrypt==3) {
			// WPA authentication
			if (encrypt==2) {
				intVal = 2;
				getPSK = 1;
			}
			else if (encrypt==3) {
				intVal = 2;
				getPSK = 1;
			}

			if(ssidIndex==0){
				if ( apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
					goto setErr_basic;
				}
			}

			// cipher suite
			strVal = websGetVar(wp, T("wpaCipher"), T(""));
			if (strVal[0]) {
				intVal = strVal[0] - '0';
				if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;
				//WPA_CIPHER_TKIP=1, WPA_CIPHER_AES=2, WPA_CIPHER_MIXED=3 

				#ifdef _MSSID_
				printf("ssidIndex:%d intTkip:%d intAes:%d\n",ssidIndex, intTkip ,intAes);
				switch(ssidIndex)
				{
					case 0: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes); break;
					case 1: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_1, (void *)&intAes); break;
					case 2: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_2, (void *)&intAes); break;
					case 3: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_3, (void *)&intAes); break;
					case 4: apmib_set(MIB_WLAN_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE_4, (void *)&intAes); break;
					default: break; 
				}
				#else
				if ( apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				if ( apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				#endif

			}
		} //if (encrypt==2 || encrypt==3)

		//Follow Realtek, Set WLAN_ENCRYPT
		strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));
	//strEncrypt=method
	}
	
	#ifdef _AUTO_CHAN_SELECT_
	#ifdef __TARGET_BOARD__
	if(strEncrypt[0]=='0' && ssidIndex==0 ){
		sprintf(tmpBuf, "echo '' > /tmp/ConnectTestKey", wepKey);
		system(tmpBuf);
		printf("%s\n",tmpBuf);
	}
	#endif
	#endif

	//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
	if(strSupportAllSecurity[0]){
		if (strEncrypt[0]=='0')	 //Disable
			encrypt=0;			
		else if (strEncrypt[0]=='1') //WEP
			encrypt=1; 
		else if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA
		{
			getPSK = 1;
			strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));
			if (strWpaCipher[0]) 
			{
				intValWPA = strWpaCipher[0] - '0';
				if (intValWPA==1)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intValWPA==2) intAes=WPA_CIPHER_AES; else intAes=0;

				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);
			}
			if (strEncrypt[0]=='2') encrypt=2;
			else 	encrypt=4;
		}
		else if (strEncrypt[0]=='4') //WPA RADIUS
			encrypt=2;	
		else if (strEncrypt[0]=='5') //WPA2 RADIUS
			encrypt=4;
	}
	else
	{
		if (strEncrypt[0]=='0')	encrypt=0;
		else if (strEncrypt[0]=='1') encrypt=1;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='1')) encrypt=2;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='2')) encrypt=4;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='3')) encrypt=6;
	}	
	

	#ifdef _MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);break;
		case 1: apmib_set(MIB_WLAN_ENCRYPT_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_WLAN_ENCRYPT_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_WLAN_ENCRYPT_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_WLAN_ENCRYPT_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
		goto setErr_basic;
	}
	#endif
//ENCRYPT_DISABLED=0, ENCRYPT_WEP=1, ENCRYPT_PSK=3, ENCRYPT_ARS=4
//*********************************** Wep setting ***********************************
	if (encrypt==ENCRYPT_WEP) {
		strEnabled = websGetVar(wp, T("wepEnabled"), T(""));
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( enabled ) {
			strKeyLen = websGetVar(wp, T("length"), T(""));
			if (!strKeyLen[0]) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt2))</script>"));
				goto setErr_basic;
			}
			if (strKeyLen[0]!='1' && strKeyLen[0]!='2' && strKeyLen[0]!='3') {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt3))</script>"));
				goto setErr_basic;
			}
			if (strKeyLen[0] == '1')
				wep = WEP64;
			else if (strKeyLen[0] == '2')
				wep = WEP128;
			else
				wep = WEP152;
		}
		else
			wep = WEP_DISABLED;

		//WEP_DISABLED=0, WEP64=1, WEP128=2, WEP152=3
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WEP, (void *)&wep); break;
			case 1: apmib_set(MIB_WLAN_WEP_1, (void *)&wep); break;
			case 2: apmib_set(MIB_WLAN_WEP_2, (void *)&wep); break;
			case 3: apmib_set(MIB_WLAN_WEP_3, (void *)&wep); break;
			case 4: apmib_set(MIB_WLAN_WEP_4, (void *)&wep); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt5))</script>"));
			goto setErr_basic;
		}
		#endif
		if (wep == WEP_DISABLED)
			goto setwlan_ret;

		strFormat = websGetVar(wp, T("format"), T(""));
		if (!strFormat[0]) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt5))</script>"));
			goto setErr_basic;
		}

		if (strFormat[0]!='1' && strFormat[0]!='2') {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt6))</script>"));
			goto setErr_basic;
		}

		i = strFormat[0] - '0' - 1;

		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i); break;
			case 1: apmib_set( MIB_WLAN_WEP_KEY_TYPE_1, (void *)&i); break;
			case 2: apmib_set( MIB_WLAN_WEP_KEY_TYPE_2, (void *)&i); break;
			case 3: apmib_set( MIB_WLAN_WEP_KEY_TYPE_3, (void *)&i); break;
			case 4: apmib_set( MIB_WLAN_WEP_KEY_TYPE_4, (void *)&i); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_basic;
		}
		#endif
		if (wep == WEP64) {
			if (strFormat[0]=='1')
				keyLen = WEP64_KEY_LEN;   //WEP64_KEY_LEN=5
			else
				keyLen = WEP64_KEY_LEN*2;
		}
		else if (wep == WEP128) {
			if (strFormat[0]=='1')
				keyLen = WEP128_KEY_LEN;  //WEP128_KEY_LEN=13
			else
				keyLen = WEP128_KEY_LEN*2;
		}
		else {
			if (strFormat[0]=='1')
				keyLen = WEP152_KEY_LEN;
			else
				keyLen = WEP152_KEY_LEN*2;
		}
		strKeyId = websGetVar(wp, T("defaultTxKeyId"), T(""));
		if ( strKeyId[0] ) {
			if ( strKeyId[0]!='1' && strKeyId[0]!='2' && strKeyId[0]!='3' && strKeyId[0]!='4' ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt8))</script>"));
				goto setErr_basic;
			}
			i = strKeyId[0] - '0' - 1;

			#ifdef _MSSID_
			switch(ssidIndex)
			{
				case 0: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i); break;
				case 1: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_1, (void *)&i); break;
				case 2: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_2, (void *)&i); break;
				case 3: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_3, (void *)&i); break;
				case 4: apmib_set( MIB_WLAN_WEP_DEFAULT_KEY_4, (void *)&i); break;
				default: break; 
			}
			#else
			if ( !apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_basic;
			}
			#endif
		}


		wepKey = websGetVar(wp, T("key1"), T(""));

		if  (wepKey[0]) {
			if (strlen(wepKey) != keyLen) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt10))</script>"));
				goto setErr_basic;
			}
			if ( !isAllStar(wepKey) ) {
			
				#ifdef _AUTO_CHAN_SELECT_
				char tmpWepKey[200];
				char buildWepKey[200]={0};
				strcpy(tmpWepKey,wepKey);
				strcpy(ConnectTestKey, wepKey);
				wep_used=1;
				//CharFilter0(tmpWepKey);
				#ifdef __TARGET_BOARD__
				if(ssidIndex==0){
					for(i=0; i<strlen(tmpWepKey); i++) //EDX, Ken 2015_09_16 add, Avoid tmpWepKey is "wepKey;systemcode".
					{
						sprintf(buildWepKey, "%s%X", buildWepKey, tmpWepKey[i]);
					}
					sprintf(tmpBuf, "echo %s > /tmp/ConnectTestKey",(void *)buildWepKey);//Robert 2014_01_06
					system(tmpBuf);
					printf("%s\n",tmpBuf);
				}	
				#endif
				#endif

				printf("ssidIndex=%d, 2.4g WEP len(1:64 bit, 2:128 bit)=%d Format(1:ascii, 2:hex)=%s\n",ssidIndex,wep,strFormat);
				if (strFormat[0] == '1') // ascii
				{
					#ifdef _TEXTFILE_CONFIG_
					if ( !apmib_set(MIB_TMPSTRING, (void *)wepKey)) {
						strcpy(tmpBuf, T("MIB_TMPSTRING error!")); 
						goto setErr_basic;
					}
					#endif
					strcpy(key, wepKey);
				}
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_basic;
					}
				}

				#ifdef _MSSID_
				switch(ssidIndex)
				{
					case 0: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
						else ret=0;
						break;
					case 1: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_1, (void *)key);
						else ret=0;
						break;
					case 2: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_2, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_2, (void *)key);
						else ret=0;
						break;
					case 3: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_3, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_3, (void *)key);
						else ret=0;
						break;
					case 4: 
						if (wep == WEP64) apmib_set(MIB_WLAN_WEP64_KEY1_4, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_WLAN_WEP128_KEY1_4, (void *)key);
						else ret=0;
						break;
					default: break; 
				}
				#else
				if (wep == WEP64)
					ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
				else
					ret=0;
				#endif
				if (!ret) {
		 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt12))</script>"));
					goto setErr_basic;
				}
			}
		}

	} //if (encrypt==ENCRYPT_WEP) {

//*********************************** pre-shared key setting ***********************************

	if ( getPSK ) {
		int oldFormat, oldPskLen, i;
		
		//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
		if(strSupportAllSecurity[0])
		{			
			if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA
				strVal = websGetVar(wp, T("wpaPskFormat"), T(""));
			/*else if (strEncrypt[0]=='3') //WPA2
			{
				strVal = websGetVar(wp, T("wpa2PskFormat"), T(""));
			}*/
		}
		else
		{
			strVal = websGetVar(wp, T("pskFormat"), T(""));
		}

		if (!strVal[0]) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt22))</script>"));
			goto setErr_basic;
		}
		intVal = strVal[0] - '0';
		if (intVal != 0 && intVal != 1) {
 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt23))</script>"));
			goto setErr_basic;
		}

		// remember current psk format and length to compare to default case "****"
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf); break;
			case 1: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_1, (void *)tmpBuf); break;
			case 2: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_2, (void *)tmpBuf); break;
			case 3: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_3, (void *)tmpBuf); break;
			case 4: apmib_get(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_4, (void *)tmpBuf); break;
			default: break; 
		}
		#else
		apmib_get(MIB_WLAN_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf);
		#endif
		oldPskLen = strlen(tmpBuf);

		//EDX Robert 2014_02_20 Add Support All Security => WPA-TKIP WPA-AES WPA2-TKIP WPA2-AES
		if(strSupportAllSecurity[0]){			
			if (strEncrypt[0]=='2' || strEncrypt[0]=='3') //WPA		
				strVal = websGetVar(wp, T("wpaPskValue"), T(""));
			/*else if (strEncrypt[0]=='3') //WPA2
			{		
				strVal = websGetVar(wp, T("wpa2PskValue"), T(""));
			}*/
		}
		else{
			strVal = websGetVar(wp, T("pskValue"), T(""));
		}
		
		len = strlen(strVal);
		if (oldFormat == intVal && len == oldPskLen ) {
			for (i=0; i<len; i++) {
				if ( strVal[i] != '*' )
					break;
			}
			if (i == len)
				goto rekey_time;
		}

		#ifdef _AUTO_CHAN_SELECT_
		if(strVal[0])
		{
			char tmpStrVal[200];
			char buildstrVal[200]={0};
			strcpy(tmpStrVal, strVal);
			strcpy(ConnectTestKey, strVal);
			CharFilter0(tmpStrVal);
			if(ssidIndex==0){
				#ifdef __TARGET_BOARD__
				for(i=0; i<strlen(tmpStrVal); i++) //EDX, Ken 2015_09_07 add, Avoid tmpWepKey is "wepKey;systemcode".
				{
					sprintf(buildstrVal, "%s%X", buildstrVal, tmpStrVal[i]);
				}
				sprintf(tmpBuf, "echo \"%s\" > /tmp/ConnectTestKey",(void *)buildstrVal); //Robert 2014_01_06
				system(tmpBuf);
				printf("%s\n",tmpBuf);
				#endif
			}
		}
		#endif

		//printf("ssidIndex=%d, 2.4g pskformate=%d\n",ssidIndex,intVal);
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal); break;
			case 1: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&intVal); break;
			case 2: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&intVal); break;
			case 3: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&intVal); break;
			case 4: apmib_set(MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_basic;
		}
		#endif
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan WDS
		if ( apmib_set(MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}
		#endif

		if (intVal==1) { // hex
			if (len!=MAX_PSK_LEN || !string_to_hex(strVal, tmpBuf, MAX_PSK_LEN)) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_basic;
			}
		}
		else { // passphras
			if (len==0 || len > (MAX_PSK_LEN-1) ) {
 				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt25))</script>"));
				goto setErr_basic;
			}
		}
		#ifdef _MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal); break;
			case 1: apmib_set(MIB_WLAN_WPA_PSK_1, (void *)strVal); break;
			case 2: apmib_set(MIB_WLAN_WPA_PSK_2, (void *)strVal); break;
			case 3: apmib_set(MIB_WLAN_WPA_PSK_3, (void *)strVal); break;
			case 4: apmib_set(MIB_WLAN_WPA_PSK_4, (void *)strVal); break;
			default: break; 
		}
		#else
		if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_basic;
		}
		#endif
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan wds
		if ( !apmib_set(MIB_WLAN_WDS_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
		#endif


	}


rekey_time:
		
//*********************************** RADIUS Server setting ***********************************
	if (enableRS == 1) { // if 1x enabled, get RADIUS server info
		#ifndef HOME_GATEWAY
		 if (!str1xMode[0]) {
		 #endif
		// set Radius Port
		strVal = websGetVar(wp, T("radiusPort"), T(""));
		if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt7))</script>"));
				goto setErr_basic;
			}
			if (!string_to_dec(strVal, &intVal) || intVal<=0 || intVal>65535) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt8))</script>"));
				goto setErr_basic;
			}
	
			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_PORT, (void *)&intVal); break;
				case 1: apmib_set(MIB_WLAN_RS_PORT_1, (void *)&intVal); break;
				case 2: apmib_set(MIB_WLAN_RS_PORT_2, (void *)&intVal); break;
				case 3: apmib_set(MIB_WLAN_RS_PORT_3, (void *)&intVal); break;
				case 4: apmib_set(MIB_WLAN_RS_PORT_4, (void *)&intVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_PORT, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt9))</script>"));
				goto setErr_basic;
			}
			#endif
		//Radius Ip
			strVal = websGetVar(wp, T("radiusIP"), T(""));
			if (!strVal[0]) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt10))</script>"));
				goto setErr_basic;
			}
			if ( !inet_aton(strVal, &inIp) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt11))</script>"));
				goto setErr_basic;
			}

			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_IP, (void *)&inIp); break;
				case 1: apmib_set(MIB_WLAN_RS_IP_1, (void *)&inIp); break;
				case 2: apmib_set(MIB_WLAN_RS_IP_2, (void *)&inIp); break;
				case 3: apmib_set(MIB_WLAN_RS_IP_3, (void *)&inIp); break;
				case 4: apmib_set(MIB_WLAN_RS_IP_4, (void *)&inIp); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_IP, (void *)&inIp)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt12))</script>"));
				goto setErr_basic;
			}
			#endif
			//Radius password
			strVal = websGetVar(wp, T("radiusPass"), T(""));
			if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt13))</script>"));
				goto setErr_basic;
			}

			#ifdef _MULTIPLE_WLAN_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal); break;
				case 1: apmib_set(MIB_WLAN_RS_PASSWORD_1, (void *)strVal); break;
				case 2: apmib_set(MIB_WLAN_RS_PASSWORD_2, (void *)strVal); break;
				case 3: apmib_set(MIB_WLAN_RS_PASSWORD_3, (void *)strVal); break;
				case 4: apmib_set(MIB_WLAN_RS_PASSWORD_4, (void *)strVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_basic;
			}
			#endif
			/*int session_time = 5;
			if ( !apmib_set(MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME, (void *)&session_time)) {   //session_timeout
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_encrypt;
			}

			session_time = 5;
			if ( !apmib_set(MIB_WLAN_RS_INTERVAL_TIME, (void *)&session_time)) {   //idle_timeout
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt14))</script>"));
				goto setErr_encrypt;
			}*/

		#ifndef HOME_GATEWAY
		}
		#endif

get_wepkey:
		// get 802.1x WEP key length
		strVal = websGetVar(wp, T("wepKeyLen"), T(""));
		if (strVal[0]) {
			if ( !gstrcmp(strVal, T("wep64")))
				intVal = WEP64;
			else if ( !gstrcmp(strVal, T("wep128")))
				intVal = WEP128;
			else {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt15))</script>"));
				goto setErr_basic;
			}
			if ( apmib_set(MIB_WLAN_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_basic;
			}
		}
	}

//*********************************** WPS_CONFIG setting ***********************************


	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
	//printf("--> 2.4 wpsStatus:%s\n", strWpsStatus);
	if (!gstrcmp(strWpsStatus, T("1"))){
		val = 1;
		if ( apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_WPS_CONFIG_STATUS, (void *)&val) == 0)
		{
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_basic;
		}
		val = 0;
		if ( apmib_set(MIB_WPS_DISPLAY_KEY, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
	}



setwlan_ret:

	#ifdef _IQv2_
	//char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);
	submitUrl = websGetVar(wp, T("submit-url"), T("")); //xxxxxxxxx


	if (!gstrcmp(submitUrl, T("/wiz_rep5g.asp"))){
		
		strSSID = websGetVar(wp, T("ssid"), T(""));		
		if(strSSID[0])
			system("echo \"1\" > /tmp/wifi_changed");
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh 2> /dev/null");  // for iQsetup scan ssid
		system("/bin/up_wlan.sh &");
		#endif
	}


#ifdef _AUTO_CHAN_SELECT_
	char_t *strCONNTEST;
	strCONNTEST = websGetVar(wp, T("doConnectTest"), T(""));
	if ( strCONNTEST[0] && (!gstrcmp(strCONNTEST, T("1"))) ) { //if CONN_TEST_TRUE=0 means dont do connect test

		strVal = websGetVar(wp, T("rootAPmac"), T(""));		
		if(strVal[0]){
			for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "UserName;systemcode".
			{
				if (strVal[i]==59)
				{
					strVal[i]='\0';
					break;
				}
			}
			sprintf(tmpBuf, "echo '%s' > /tmp/apcli0_rootMAC", strVal);
			system(tmpBuf);
		}

		system("killall connectTest.sh 2> /dev/null");
		system("rm -f /tmp/connectTestdone 2> /dev/null");

		int pid;
		pid = fork();
		if (pid) {
			waitpid(pid, NULL, 0);
			sleep(3);
		}
		else if (pid == 0) {
			if (submitUrl[0])
			{
				REPLACE_MSG(submitUrl);
			}
			exit(0);
		}
		CONNECT_TEST(2);	//2=wlan1 test 5=wlan0 test 7=wlan0+wlan1 test
	}
	else{
#endif
		char_t *strApply;
		char tmpBufapply[200];
		strApply = websGetVar(wp, T("isApply"),T(""));


		if(strApply[0]){

			if (lanipChanged == 1) {
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBuf, newurl);
			}
			else
			{			
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			system("echo 1 > /tmp/show_reboot_info");
			REPLACE_MSG(submitUrl);
		}

#ifdef _AUTO_CHAN_SELECT_
	}
#endif
	return;

setErr_basic:
	ERR_MSG(wp, tmpBuf);
	return;
#endif
}  //formWlbasicREP end
#endif
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
void formWIRESch(webs_t wp, char_t *path, char_t *query)
{
	char_t *strEnabled24G, *strEnabled5G;
	char_t *strAddSch, *strDelAllSch, *strDelSch;
	char_t *strSelInterface, *strApply, *submitUrl, *iqsetupclose;
	int selectInterface, intDay, intHour, wireInteNUM, intVal;
	char tmpBuf[100];
	char defDay[25]  ="000000000000000000000000";
	char checkDay[25]="zzzzzzzzzzzzzzzzzzzzzzzz";
	WIRESCH_T wireSch;
	EDIT_T Edit;

	strEnabled24G = websGetVar(wp, T("wlanSchEnabled24G"), T(""));
	if (strEnabled24G[0]) {
		intVal=atoi(strEnabled24G);
		apmib_set( MIB_WLAN_SCH_ENABLED, (void *)&intVal);
	}

	strEnabled5G  = websGetVar(wp, T("wlanSchEnabled5G"), T(""));
	if (strEnabled5G[0]) {
		intVal=atoi(strEnabled5G);
		apmib_set( MIB_INIC_SCH_ENABLED, (void *)&intVal);
	}


	strSelInterface = websGetVar(wp, T("selInterface"), T(""));
	selectInterface=atoi(strSelInterface);

	/* Add entry */
	if (strSelInterface[0]) {
		strAddSch = websGetVar(wp, T("addSchdule"), T(""));
		if (strAddSch[0]) {
			printf("===> strAddSch---selectInterface--[%s],[%d]\n",strSelInterface,selectInterface);

			if ( !apmib_get(MIB_WIRE_SCH_NUM, (void *)&wireInteNUM))
			{
							websError(wp, 400, T("Get table wireSch error!\n"));
						return -1;
			}
			if(selectInterface<=wireInteNUM)
			{
				WIRESCH_T sel_wireSch;
				char WEEKSchArray[7][25];
				char_t *tmpWEEKSchArray[7];
			
				tmpWEEKSchArray[0] = websGetVar(wp, T("addSunday"), T(""));
				tmpWEEKSchArray[1] = websGetVar(wp, T("addMonday"), T(""));
				tmpWEEKSchArray[2] = websGetVar(wp, T("addTuesday"), T(""));
				tmpWEEKSchArray[3] = websGetVar(wp, T("addWednesday"), T(""));
				tmpWEEKSchArray[4] = websGetVar(wp, T("addThursday"), T(""));
				tmpWEEKSchArray[5] = websGetVar(wp, T("addFriday"), T(""));
				tmpWEEKSchArray[6] = websGetVar(wp, T("addSaturday"), T(""));
			
				*((char *)&sel_wireSch) = (char)selectInterface;
				if ( !apmib_get(MIB_WIRE_SCH, (void *)&sel_wireSch)) 
					return -1;

				strcpy(WEEKSchArray[0],sel_wireSch.Sunday);//Sunday
				strcpy(WEEKSchArray[1],sel_wireSch.Monday);//Monday
				strcpy(WEEKSchArray[2],sel_wireSch.Tuesday);//Tuesday
				strcpy(WEEKSchArray[3],sel_wireSch.Wednesday);//Wednesday
				strcpy(WEEKSchArray[4],sel_wireSch.Thursday);//Thursday
				strcpy(WEEKSchArray[5],sel_wireSch.Friday);//Friday
				strcpy(WEEKSchArray[6],sel_wireSch.Saturday);//Saturday

				printf("-------- formWIRESch --------\n");		
				printf("tmpWEEKSchArray[0]    ====>[%s]\n",tmpWEEKSchArray[0]);
				printf("tmpWEEKSchArray[1]    ====>[%s]\n",tmpWEEKSchArray[1]);
				printf("tmpWEEKSchArray[2]    ====>[%s]\n",tmpWEEKSchArray[2]);
				printf("tmpWEEKSchArray[3]    ====>[%s]\n",tmpWEEKSchArray[3]);
				printf("tmpWEEKSchArray[4]    ====>[%s]\n",tmpWEEKSchArray[4]);
				printf("tmpWEEKSchArray[5]    ====>[%s]\n",tmpWEEKSchArray[5]);
				printf("tmpWEEKSchArray[6]    ====>[%s]\n",tmpWEEKSchArray[6]);

				printf("sel_wireSch.Interface ====>[%s]\n",sel_wireSch.Interface);
				printf("sel_wireSch.Sunday    ====>[%s]\n",sel_wireSch.Sunday);
				printf("sel_wireSch.Monday    ====>[%s]\n",sel_wireSch.Monday);
				printf("sel_wireSch.Tuesday   ====>[%s]\n",sel_wireSch.Tuesday);
				printf("sel_wireSch.Wednesday ====>[%s]\n",sel_wireSch.Wednesday);
				printf("sel_wireSch.Thursday  ====>[%s]\n",sel_wireSch.Thursday);
				printf("sel_wireSch.Friday    ====>[%s]\n",sel_wireSch.Friday);
				printf("sel_wireSch.Saturday  ====>[%s]\n",sel_wireSch.Saturday);
				printf("-------- formWIRESch --------\n");	
			
				for(intDay=6;intDay>=0;intDay--) //Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday ==> 0,1,2,3,5,6
				{
					for(intHour=23;intHour>=0;intHour--)
					{
						WEEKSchArray[intDay][intHour]=CharLogicGatesComputing(WEEKSchArray[intDay][intHour],tmpWEEKSchArray[intDay][intHour],1);
					}
				}

				if( !strcmp(WEEKSchArray[0],checkDay) && !strcmp(WEEKSchArray[1],checkDay) && !strcmp(WEEKSchArray[2],checkDay) && !strcmp(WEEKSchArray[3],checkDay) && !strcmp(WEEKSchArray[4],checkDay) && !strcmp(WEEKSchArray[5],checkDay) && !strcmp(WEEKSchArray[6],checkDay) )
				{
					printf("WEEKSchArray Error!!\n");
					strcpy(wireSch.Interface,sel_wireSch.Interface);
					strcpy(wireSch.Sunday,sel_wireSch.Sunday);//Sunday
					strcpy(wireSch.Monday,sel_wireSch.Monday);//Monday
					strcpy(wireSch.Tuesday,sel_wireSch.Tuesday);//Tuesday
					strcpy(wireSch.Wednesday,sel_wireSch.Wednesday);//Wednesday
					strcpy(wireSch.Thursday,sel_wireSch.Thursday);//Thursday
					strcpy(wireSch.Friday,sel_wireSch.Friday);//Friday
					strcpy(wireSch.Saturday,sel_wireSch.Saturday);//Saturday
				}
				else
				{
					strcpy(wireSch.Interface,sel_wireSch.Interface);
					strcpy(wireSch.Sunday,WEEKSchArray[0]);//Sunday
					strcpy(wireSch.Monday,WEEKSchArray[1]);//Monday
					strcpy(wireSch.Tuesday,WEEKSchArray[2]);//Tuesday
					strcpy(wireSch.Wednesday,WEEKSchArray[3]);//Wednesday
					strcpy(wireSch.Thursday,WEEKSchArray[4]);//Thursday
					strcpy(wireSch.Friday,WEEKSchArray[5]);//Friday
					strcpy(wireSch.Saturday,WEEKSchArray[6]);//Saturday
				}

				Edit.selEntry = (char *)&sel_wireSch;
				Edit.editEntry = (char *)&wireSch;
				apmib_set(MIB_WIRE_SCH_EDIT, (void *)&Edit);
			}
		}

		/* Delete entry */
		strDelSch = websGetVar(wp, T("deleteSelSch"), T(""));
		if (strDelSch[0]) {
			printf("===> strDelSch---selectInterface--[%s],[%d]\n",strSelInterface,selectInterface);

			if ( !apmib_get(MIB_WIRE_SCH_NUM, (void *)&wireInteNUM))
			{
							websError(wp, 400, T("Get table wireSch error!\n"));
						return -1;
			}

			if(selectInterface<=wireInteNUM)
			{
				WIRESCH_T sel_wireSch;
				char WEEKSchArray[7][25];
				char_t *tmpWEEKSchArray[7];
		
				tmpWEEKSchArray[0] = websGetVar(wp, T("totalSelSunday"), T(""));
				tmpWEEKSchArray[1] = websGetVar(wp, T("totalSelMonday"), T(""));
				tmpWEEKSchArray[2] = websGetVar(wp, T("totalSelTuesday"), T(""));
				tmpWEEKSchArray[3] = websGetVar(wp, T("totalSelWednesday"), T(""));
				tmpWEEKSchArray[4] = websGetVar(wp, T("totalSelThursday"), T(""));
				tmpWEEKSchArray[5] = websGetVar(wp, T("totalSelFriday"), T(""));
				tmpWEEKSchArray[6] = websGetVar(wp, T("totalSelSaturday"), T(""));

				*((char *)&sel_wireSch) = (char)selectInterface;
				if ( !apmib_get(MIB_WIRE_SCH, (void *)&sel_wireSch)) 
					return -1;

				strcpy(WEEKSchArray[0],sel_wireSch.Sunday);//Sunday
				strcpy(WEEKSchArray[1],sel_wireSch.Monday);//Monday
				strcpy(WEEKSchArray[2],sel_wireSch.Tuesday);//Tuesday
				strcpy(WEEKSchArray[3],sel_wireSch.Wednesday);//Wednesday
				strcpy(WEEKSchArray[4],sel_wireSch.Thursday);//Thursday
				strcpy(WEEKSchArray[5],sel_wireSch.Friday);//Friday
				strcpy(WEEKSchArray[6],sel_wireSch.Saturday);//Saturday

				printf("-------- formWIRESch --------\n");		
				printf("tmpWEEKSchArray[0]    ====>[%s]\n",tmpWEEKSchArray[0]);
				printf("tmpWEEKSchArray[1]    ====>[%s]\n",tmpWEEKSchArray[1]);
				printf("tmpWEEKSchArray[2]    ====>[%s]\n",tmpWEEKSchArray[2]);
				printf("tmpWEEKSchArray[3]    ====>[%s]\n",tmpWEEKSchArray[3]);
				printf("tmpWEEKSchArray[4]    ====>[%s]\n",tmpWEEKSchArray[4]);
				printf("tmpWEEKSchArray[5]    ====>[%s]\n",tmpWEEKSchArray[5]);
				printf("tmpWEEKSchArray[6]    ====>[%s]\n",tmpWEEKSchArray[6]);

				printf("sel_wireSch.Interface ====>[%s]\n",sel_wireSch.Interface);
				printf("sel_wireSch.Sunday    ====>[%s]\n",sel_wireSch.Sunday);
				printf("sel_wireSch.Monday    ====>[%s]\n",sel_wireSch.Monday);
				printf("sel_wireSch.Tuesday   ====>[%s]\n",sel_wireSch.Tuesday);
				printf("sel_wireSch.Wednesday ====>[%s]\n",sel_wireSch.Wednesday);
				printf("sel_wireSch.Thursday  ====>[%s]\n",sel_wireSch.Thursday);
				printf("sel_wireSch.Friday    ====>[%s]\n",sel_wireSch.Friday);
				printf("sel_wireSch.Saturday  ====>[%s]\n",sel_wireSch.Saturday);
				printf("-------- formWIRESch --------\n");	

				for(intDay=6;intDay>=0;intDay--) //Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday ==> 0,1,2,3,5,6
				{
					for(intHour=23;intHour>=0;intHour--)
					{
						WEEKSchArray[intDay][intHour]=CharLogicGatesComputing(WEEKSchArray[intDay][intHour],tmpWEEKSchArray[intDay][intHour],2);
					}
				}

				if( !strcmp(WEEKSchArray[0],checkDay) && !strcmp(WEEKSchArray[1],checkDay) && !strcmp(WEEKSchArray[2],checkDay) && !strcmp(WEEKSchArray[3],checkDay) && !strcmp(WEEKSchArray[4],checkDay) && !strcmp(WEEKSchArray[5],checkDay) && !strcmp(WEEKSchArray[6],checkDay) )
				{
					printf("WEEKSchArray Error!!\n");
					strcpy(wireSch.Interface,sel_wireSch.Interface);
					strcpy(wireSch.Sunday,sel_wireSch.Sunday);//Sunday
					strcpy(wireSch.Monday,sel_wireSch.Monday);//Monday
					strcpy(wireSch.Tuesday,sel_wireSch.Tuesday);//Tuesday
					strcpy(wireSch.Wednesday,sel_wireSch.Wednesday);//Wednesday
					strcpy(wireSch.Thursday,sel_wireSch.Thursday);//Thursday
					strcpy(wireSch.Friday,sel_wireSch.Friday);//Friday
					strcpy(wireSch.Saturday,sel_wireSch.Saturday);//Saturday
				}
				else
				{
					strcpy(wireSch.Interface,sel_wireSch.Interface);
					strcpy(wireSch.Sunday,WEEKSchArray[0]);//Sunday
					strcpy(wireSch.Monday,WEEKSchArray[1]);//Monday
					strcpy(wireSch.Tuesday,WEEKSchArray[2]);//Tuesday
					strcpy(wireSch.Wednesday,WEEKSchArray[3]);//Wednesday
					strcpy(wireSch.Thursday,WEEKSchArray[4]);//Thursday
					strcpy(wireSch.Friday,WEEKSchArray[5]);//Friday
					strcpy(wireSch.Saturday,WEEKSchArray[6]);//Saturday
				}
				Edit.selEntry = (char *)&sel_wireSch;
				Edit.editEntry = (char *)&wireSch;
				apmib_set(MIB_WIRE_SCH_EDIT, (void *)&Edit);
			}
		}

		/* Delete all entry */
		strDelAllSch = websGetVar(wp, T("deleteAllSch"), T(""));
		if ( strDelAllSch[0]) {
			printf("===> strDelAllSch---selectInterface--[%s],[%d]\n",strSelInterface,selectInterface);

			if ( !apmib_get(MIB_WIRE_SCH_NUM, (void *)&wireInteNUM))
			{
							websError(wp, 400, T("Get table wireSch error!\n"));
						return -1;
			}
			if(selectInterface<=wireInteNUM)
			{
				WIRESCH_T sel_wireSch;

				*((char *)&sel_wireSch) = (char)selectInterface;
				if ( !apmib_get(MIB_WIRE_SCH, (void *)&sel_wireSch)) 
					return -1;

				strcpy(wireSch.Interface,sel_wireSch.Interface);
				strcpy(wireSch.Sunday,defDay);//Sunday
				strcpy(wireSch.Monday,defDay);//Monday
				strcpy(wireSch.Tuesday,defDay);//Tuesday
				strcpy(wireSch.Wednesday,defDay);//Wednesday
				strcpy(wireSch.Thursday,defDay);//Thursday
				strcpy(wireSch.Friday,defDay);//Friday
				strcpy(wireSch.Saturday,defDay);//Saturday

				Edit.selEntry = (char *)&sel_wireSch;
				Edit.editEntry = (char *)&wireSch;
				apmib_set(MIB_WIRE_SCH_EDIT, (void *)&Edit);
			}
		}
	}
#ifdef _MULTI_WIZMODE_
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
#endif

	apmib_update(CURRENT_SETTING);
	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

	char tmpBufapply[200];
	if(strApply[0]){
		if (lanipChanged == 1) {
			unsigned char buffer[100];
			unsigned char newurl[150];
			if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
				return -1;

			sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
			sprintf(tmpBuf, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
			OK_MSG3(tmpBuf, newurl);
		}
		else
		{			
			sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
			OK_MSG3(tmpBufapply ,submitUrl);
		}
		system("/bin/reboot.sh 3 &");
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REPLACE_MSG(submitUrl);
	}
	return;
}
int WIRESchList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, j, wireInteNUM, intVal,tmpIntVal, intDay, intHour, intMin, wireSchCount;
	int MinArray[6]={32,16,8,4,2,1};
	int initState=2, SchState, last_SchState, each_hour;
	int wifi_on=0, wifi_off=1, need_do_something=0;
	int WirelessON[100],WirelessOFF[100],WirelessONNum=0,WirelessOFFNum=0;
	int selectInterface=atoi(argv[0]); // 1(2.4G),2(5G)
	WIRESCH_T wireSch;
	char WEEKArray[7][10]= {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	char WEEKSchArray[7][25],InterfaceArray[10];

	SchState = initState;
	last_SchState = initState;

	if ( !apmib_get(MIB_WIRE_SCH_NUM, (void *)&wireInteNUM)){
		websError(wp, 400, T("Get table wireSch error!\n"));
			return -1;
	}
	if(selectInterface<=wireInteNUM){
		//for ( i=1; i<=wireInteNUM; i++)
		//{
			WirelessONNum=0,WirelessOFFNum=0;
			//*((char *)&wireSch) = (char)i;
			*((char *)&wireSch) = (char)selectInterface;
			if ( !apmib_get(MIB_WIRE_SCH, (void *)&wireSch)) 
				return -1;
			strcpy(InterfaceArray,wireSch.Interface);
			strcpy(WEEKSchArray[0],wireSch.Sunday);//Sunday
			strcpy(WEEKSchArray[1],wireSch.Monday);//Monday
			strcpy(WEEKSchArray[2],wireSch.Tuesday);//Tuesday
			strcpy(WEEKSchArray[3],wireSch.Wednesday);//Wednesday
			strcpy(WEEKSchArray[4],wireSch.Thursday);//Thursday
			strcpy(WEEKSchArray[5],wireSch.Friday);//Friday
			strcpy(WEEKSchArray[6],wireSch.Saturday);//Saturday

			printf("-------- WIRESchList --------\n");		
			printf("wireSch.Interface     ====>[%s]\n",wireSch.Interface);
			printf("wireSch.Sunday        ====>[%s]\n",wireSch.Sunday);
			printf("wireSch.Monday        ====>[%s]\n",wireSch.Monday);
			printf("wireSch.Tuesday       ====>[%s]\n",wireSch.Tuesday);
			printf("wireSch.Wednesday     ====>[%s]\n",wireSch.Wednesday);
			printf("wireSch.Thursday      ====>[%s]\n",wireSch.Thursday);
			printf("wireSch.Friday        ====>[%s]\n",wireSch.Friday);
			printf("wireSch.Saturday      ====>[%s]\n",wireSch.Saturday);
			printf("-------- WIRESchList --------\n");

			for(intDay=6;intDay>=0;intDay--){
				for(intHour=23;intHour>=0;intHour--){
					each_hour = CharToInt_x64(WEEKSchArray[intDay][intHour]); // each_hour[x:00, x:10, x:20, x:30, x:40, x:50]
					for(intMin=5;intMin>=0;intMin--){
						need_do_something = each_hour & MinArray[intMin];
						if(need_do_something) SchState = wifi_off;
						else SchState = wifi_on;


						if(last_SchState == initState)  
							last_SchState = SchState; 	// start record SchState

						if(SchState != last_SchState){
							if(SchState == wifi_off){
								WirelessON[WirelessONNum]=((intDay*144) + (intHour*6) + intMin) + 1;
								#if 0
								printf("WirelessON[%d]-",WirelessON[WirelessONNum]);
								printf("[%s] ",WEEKArray[ WirelessON[WirelessONNum]/144 ] );
								printf("[%d Hour]",(WirelessON[WirelessONNum]%144) / 6 );
								printf("[%d Min]\n",(( WirelessON[WirelessONNum]%144 ) % 6) *10 );
								#endif
								WirelessONNum++;
							}
							else{
								WirelessOFF[WirelessOFFNum]=((intDay*144) + (intHour*6) + intMin) + 1;
								#if 0
								printf("WirelessOFF[%d]-",WirelessOFF[WirelessOFFNum]);
								printf("[%s] ",WEEKArray[ WirelessOFF[WirelessOFFNum]/144 ] );
								printf("[%d Hour]",( WirelessOFF[WirelessOFFNum]%144) / 6 );
								printf("[%d Min]\n",(( WirelessOFF[WirelessOFFNum]%144 ) % 6) *10 );
								#endif
								WirelessOFFNum++;
							}
							last_SchState = SchState; // if SchState chaneged -> record SchState
						}
					}
				}
			}


			// check count of WIFI ON/OFF time point 
			if(WirelessOFFNum == 0 && WirelessONNum == 0)
				goto show_null_table;
			if( WirelessONNum != WirelessOFFNum ){
				if(WirelessONNum > WirelessOFFNum){ 
					WirelessOFF[WirelessOFFNum]=0; // one of rules is [Sunday 0:00 - Sunday x:xx], Add missing [Sunday 0:00]-off 
					WirelessOFFNum++;
				}
				else{  
					WirelessON[WirelessONNum]=0; 	// one of rules is [Saturday x:xx - Sunday 0:00], Add missing [Sunday 0:00]-on
					WirelessONNum++; 
				}
				if( WirelessONNum != WirelessOFFNum ){
					printf("Internet Schedule Table Error!!!\n");
					goto show_null_table;
				}
			}

			//If Saturday 23:50 need do something, WirelessON[] need shift to right for mapping [Sunday 0:00]-on to [Saturday x:xx]-off in WirelessOFF[].
			each_hour = CharToInt_x64(wireSch.Saturday[23]);
			need_do_something = each_hour & MinArray[5]; // MinArray[5]=x:50
			if(need_do_something && WirelessONNum >= 2){
				printf("WirelessON[] shift to right\n");
				tmpIntVal=WirelessON[WirelessONNum-1];
				for(intVal=WirelessONNum-1;intVal>=1;intVal--){
					WirelessON[intVal]=WirelessON[intVal-1];
				}
				WirelessON[0]=tmpIntVal;
			}


			nBytesSent += websWrite(wp, T("\n<!-- ========================================================================================== -->\n"));
			nBytesSent += websWrite(wp, T("  <input type=\"hidden\" id=\"SchInterface%d\" name=\"SchInterface%d\" value=\"%s\">\n"),selectInterface,selectInterface,wireSch.Interface);

			//for(j=0;j<WirelessOFFNum;j++){
			for(j=WirelessOFFNum-1;j>=0;j--){
				if(j%2) nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
				else nBytesSent += websWrite(wp, T("<tr style=\"background-color: #FFFFFF;\">\n"));
				nBytesSent += websWrite(wp, T("	  <td align=center class=\"dnsTableText\"><font size=\"2\"><script>dw(%s)</script>&nbsp;-&nbsp;%02d:%02d</font></td>\n"),WEEKArray[WirelessOFF[j]/144],(WirelessOFF[j]%144)/6,((WirelessOFF[j]%144)%6)*10);
				nBytesSent += websWrite(wp, T("	  <td align=center class=\"dnsTableText\"><font size=\"2\"><script>dw(%s)</script>&nbsp;-&nbsp;%02d:%02d</font></td>\n"),WEEKArray[ WirelessON[j]/144],  (WirelessON[j]%144)/6,  ((WirelessON[j]%144)%6)*10);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"weekS%d_%d\" name=\"weekS%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,WirelessOFF[j]/144);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"timeHS%d_%d\" name=\"timeHS%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,(WirelessOFF[j]%144) / 6);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"timeMS%d_%d\" name=\"timeMS%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,(( WirelessOFF[j]%144 ) % 6) );
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"weekE%d_%d\" name=\"weekE%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,WirelessON[j]/144);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"timeHE%d_%d\" name=\"timeHE%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,( WirelessON[j]%144) / 6 );
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"timeME%d_%d\" name=\"timeME%d_%d\" value=\"%d\">\n"),selectInterface,j,selectInterface,j,(( WirelessON[j]%144 ) % 6)  );
				nBytesSent += websWrite(wp, T("<!-- =================== -->\n"));
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selSunday%d_%d\" name=\"selSunday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selMonday%d_%d\" name=\"selMonday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selTuesday%d_%d\" name=\"selTuesday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selWednesday%d_%d\" name=\"selWednesday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selThursday%d_%d\" name=\"selThursday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selFriday%d_%d\" name=\"selFriday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"selSaturday%d_%d\" name=\"selSunday%d_%d\" value=\"\">\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	  <td align=center class=\"dnsTableText\"><input type=\"checkbox\" id=\"select%d_%d\" name=\"select%d_%d\" value=\"ON\"></td>\n"),selectInterface,j,selectInterface,j);
				nBytesSent += websWrite(wp, T("	</tr>\n"));
			}

show_null_table:
			if(WirelessOFFNum==0){
				nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
				nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
			}

			nBytesSent += websWrite(wp, T("	  <!-- =========================================================================== -->\n"));
			wireSchCount = WirelessOFFNum;

			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"wireSchCount%d\" name=\"wireSchCount%d\" value=\"%d\">\n"),selectInterface,selectInterface,wireSchCount);

			nBytesSent += websWrite(wp, T("	  <!-- =========================================================================== -->\n"));
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Sunday_x64\"    name=\"Sunday_x64\"    value=\"%s\">\n"),wireSch.Sunday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Monday_x64\"    name=\"Monday_x64\"    value=\"%s\">\n"),wireSch.Monday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Tuesday_x64\"   name=\"Tuesday_x64\"   value=\"%s\">\n"),wireSch.Tuesday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Wednesday_x64\" name=\"Wednesday_x64\" value=\"%s\">\n"),wireSch.Wednesday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Thursday_x64\"  name=\"Thursday_x64\"  value=\"%s\">\n"),wireSch.Thursday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Friday_x64\"    name=\"Friday_x64\"    value=\"%s\">\n"),wireSch.Friday);
			nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"Saturday_x64\"  name=\"Saturday_x64\"  value=\"%s\">\n"),wireSch.Saturday);
		}
		nBytesSent += websWrite(wp, T("\n<!-- ========================================================================================== -->\n"));

#ifndef __TARGET_BOARD__	
	nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"wireSchCount2\" name=\"wireSchCount2\" value=\"0\">\n"));
	nBytesSent += websWrite(wp, T("	  <input type=\"hidden\" id=\"wireSchCount1\" name=\"wireSchCount1\" value=\"0\">\n"));
#endif
	return nBytesSent;
}
#endif

#ifdef _Edimax_ //EDX, Robert Add 2015_05_29
/////////////////////////////////////////////////////////////////////////////
//For wiz_5in1 wisp mode web ui sitesurvey
int wispSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, wait_time=0 ,j,z, kakaso, apCount, len, ap_mode;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[16384],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];

	if (pStatus==NULL)
	{
		pStatus = calloc(1, sizeof(SS_STATUS_T));
		if ( pStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}

	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(WLAN_IF,  &data) < 0)
	{
		nBytesSent = websWrite(wp, T("<script>document.write(showText(aspFormWlSiteSurvey1))</script>"));
		return nBytesSent;
	}

	while (1)
	{
		if (wait_time++ > 5)
		{
			break;
		}
		sleep(1);
	}

	pStatus->number = 0;

	if ((len = getWlSiteSurveyResult(WLAN_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(pStatus);
		pStatus = NULL;
		return nBytesSent;
	}

	printf("========== Get Site Survey AP List ==========\n");
	// kernel file: cmm_info.c
	if(sizeof(data) > 0){
		int slength=0, j=0;
		int single_length, extern_length;
		char buf[50];
		FILE *fp;
		int length;
		int type_decided=0;
		struct site_survey_t{
			char channel[4];
			char ssid[33];
			char bssid[20];
			char security[23];
			char signal[9];
			char rssi[6];
			char mode[7];
			char extch[7];
			char type[3];
			char wps[4];
			char dpid[5];
		}site_survey, *psite_survey;

		apCount = i = 0;
		op = sp = &data;

		single_length = 112;	// assume the sitesurvey table at least have NT label
		extern_length = 0;

		op += single_length;

		while (*(op + extern_length) != '\n')
		{
			extern_length ++;
			if (extern_length > 9)
			{
				break;
			}
		}
		single_length += extern_length;		// + \n
		sp += single_length + 1;
		len -= single_length + 1;

		printf("total len = %d single_length = %d \n%s", len, single_length, sp);
		// santy check
		// 1. valid char data
		// 2. rest length is larger than per line length ==> (1+8+8+35+19+8+1)
		//    Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType
		while (len >= single_length)
		{

			memset (&SiteSurvey[i], 0x00, sizeof(SiteSurvey[i]));
			psite_survey = (struct site_survey_t *) sp;

			memcpy (SiteSurvey[i].channel, psite_survey->channel, 2);
			memcpy (SiteSurvey[i].ssid, psite_survey->ssid, 32);

			sscanf (psite_survey->bssid, "%02x:%02x:%02x:%02x:%02x:%02x", (int *)&SiteSurvey[i].bssid[0], (int *)&SiteSurvey[i].bssid[1],
										      (int *)&SiteSurvey[i].bssid[2], (int *)&SiteSurvey[i].bssid[3],
 										      (int *)&SiteSurvey[i].bssid[4], (int *)&SiteSurvey[i].bssid[5]);
			memcpy (SiteSurvey[i].signal, psite_survey->signal, 3);
			memcpy (SiteSurvey[i].rssi, psite_survey->rssi, 3);
			memcpy (SiteSurvey[i].wirelessmode, psite_survey->mode, 8);
			memcpy (SiteSurvey[i].networktype, psite_survey->type, 2);

			psite_survey->security[sizeof(psite_survey->security) - 1] = 0x00;
			psite_survey->ssid[sizeof(psite_survey->ssid) - 1] = 0x00;
			
			
			for (z = 33; z > 0; z--)
			{
				if(isspace(SiteSurvey[i].ssid[z]) || SiteSurvey[i].ssid[z]=='\0')
					SiteSurvey[i].ssid[z] = '\0';
				else
					break;
			}

						if (strstr (psite_survey->security, "NONE")){
				strcpy (SiteSurvey[i].authmode, "OPEN");
				strcpy (SiteSurvey[i].webstatus, "NONE");

			}else if (strstr (psite_survey->security, "WEP")){
				strcpy(SiteSurvey[i].authmode, "WEP");
				strcpy(SiteSurvey[i].webstatus, "NONE");

			}else if (strstr (psite_survey->security, "WPAPSK/TKIP")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPAPSK/AES") || strstr (psite_survey->security, "WPAPSK/TKIPAES")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIP") || strstr (psite_survey->security, "WPA2PSK/TKIP")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/AES") || strstr (psite_survey->security, "WPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPA1PSKWPA2PSK/TKIPAES")){
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
				strcpy(SiteSurvey[i].webstatus, "AES");
			}else{
				strcpy (SiteSurvey[i].authmode, "OPEN");
				strcpy (SiteSurvey[i].webstatus, "NONE");
			}

			memcpy (SiteSurvey[i].extch, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].extch, " NONE "))
				strcpy (SiteSurvey[i].extch, "0");
			else if (strstr (SiteSurvey[i].extch, " ABOVE"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, " BELOW"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");
			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY SiteSurveyBUF[100]={0};
		int x,y;
		for (x=0; x<i; x++) {
			for (y=x;y<i;y++){
				if (atoi(SiteSurvey[y].signal) > atoi(SiteSurvey[x].signal)) {
					SiteSurveyBUF[x] = SiteSurvey[y];
					SiteSurvey[y] = SiteSurvey[x];
					SiteSurvey[x] = SiteSurveyBUF[x];
				}
			}
		}
		#endif

		apCount = i;
		pStatus->number = i;

		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">"),apCount);

		for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
		{
	pStatus->bssdb[i] = SiteSurvey[i];
	sprintf(tmpBuf,"%s",pStatus->bssdb[i].ssid);
	char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
	int PowerGband = atoi(pStatus->bssdb[i].signal);
	int PowerUpGband = ScanListSignalScaleMapping92CD(PowerGband);
#endif
	nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
	nBytesSent += websWrite(wp, T("<tr class=\"listtableText\" align=\"center\">\n"));

	nBytesSent += websWrite(wp, T("	<td align=center width=\"10%%\" class=\"\">\n"));
	nBytesSent += websWrite(wp, T("		<input type=\"radio\" name=\"select\" value=\"sel%d\" onClick=\"enableConnect('sel%d')\">\n"),i,i);  
	nBytesSent += websWrite(wp, T("	</td>\n")); //Select

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
	nBytesSent += websWrite(wp, T("		<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
	nBytesSent += websWrite(wp, T("		<script>document.forms['formWlSiteSurvey'].elements['ssid'+%d].value=\""),i);
	nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
	nBytesSent += websWrite(wp, T("\" </script>\n"));
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%02X:%02X:%02X:%02X:%02X:%02X</font>"),pStatus->bssdb[i].bssid[0],pStatus->bssdb[i].bssid[1],pStatus->bssdb[i].bssid[2],pStatus->bssdb[i].bssid[3],pStatus->bssdb[i].bssid[4],pStatus->bssdb[i].bssid[5]);  //MAC
	nBytesSent += websWrite(wp, T("	</td>\n"));
	
	pStatus->bssdb[i].wirelessmode[8]='\0';
	
	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),pStatus->bssdb[i].wirelessmode);  //b/g/n
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),pStatus->bssdb[i].channel);  //channel
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),"AP");  //type
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),pStatus->bssdb[i].authmode);  //authmode
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
#if defined(_SIGNAL_FORMULA_)
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%d%%</font>"),PowerUpGband);  //Signal
#else
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s%%</font>"),pStatus->bssdb[i].signal);  //Signal
#endif
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("</tr>\n"));

	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"band%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].wirelessmode);
#if defined(_SIGNAL_FORMULA_)	
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpGband); 
#else
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].signal); 
#endif
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,pStatus->bssdb[i].bssid[0],pStatus->bssdb[i].bssid[1],pStatus->bssdb[i].bssid[2],pStatus->bssdb[i].bssid[3],pStatus->bssdb[i].bssid[4],pStatus->bssdb[i].bssid[5]); 
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, pStatus->bssdb[i].channel); //Channel
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].authmode);  //encryption
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"%s\">\n"),i,pStatus->bssdb[i].webstatus);

	nBytesSent += websWrite(wp, T("<tr>\n"));
	nBytesSent += websWrite(wp, T("	<td style=\"display:none\" align=center class=\"table2\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
	nBytesSent += websWrite(wp, T("		<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);
	nBytesSent += websWrite(wp, T("	</td>\n"));
	nBytesSent += websWrite(wp, T("</tr>\n"));
		}
	}
	return nBytesSent;
}
#endif

