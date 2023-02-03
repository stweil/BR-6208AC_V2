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
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <linux/wireless.h>
#include "../webs.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"


static SS_STATUS_Tp5G piNICStatus=NULL;
int iNICwizardEnabled = 0;
int sitesurveyed5G = 0;

#ifdef _AUTO_CHAN_SELECT_
char ConnectTestKey5g[200];
int Chan5g = 0;
int width5g = 0;
int ssid2scan_fail5g = 0;
extern int ssid2scan_fail;
extern int wep_used;
#endif

static inline int isiNICStar(char *data)
{
	int i;
	for (i=0; i<strlen(data); i++) {
		if (data[i] != '*')
			return 0;
	}
	return 1;
}


// Preamble MODE in TxD
#define INIC_MODE_CCK	0
#define INIC_MODE_OFDM   1
#define INIC_MODE_HTMIX	2
#define INIC_MODE_HTGREENFIELD	3
static inline char* iNICGetPhyMode(int Mode)
{
	switch(Mode)
	{
		case INIC_MODE_CCK:
			return "CCK";

		case INIC_MODE_OFDM:
			return "OFDM";

		case INIC_MODE_HTMIX:
			return "HTMIX";

		case INIC_MODE_HTGREENFIELD:
			return "GREEN";

		default:
			return "N/A";
	}
}

// BW
#define INIC_BW_20	0
#define INIC_BW_40	1
#define INIC_BW_BOTH		2
#define INIC_BW_10		3
static inline char* iNICGetBW(int BW)
{
#if 1
	switch(BW)
	{
		case 0:
			return "20M";
			break;
		case 1:
			return "40M";
			break;
		case 2:
			return "80M";
			break;
		case 3:
			return "10M";
			break;
		default:
			return "N/A";
			break;
	}
#else
	switch(BW)
	{
		case INIC_BW_10:
			return "10M";

		case INIC_BW_20:
			return "20M";

		case INIC_BW_40:
			return "40M";

		default:
			return "N/A";
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
void formiNICEnable(webs_t wp, char_t *path, char_t *query)
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

		if ( apmib_set(MIB_INIC_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_advance;
		}
	}
	#ifdef _TEXTFILE_CONFIG_
	strIsolationEnable = websGetVar(wp, T("mainIsoEnbled"), T(""));
	if (strIsolationEnable[0]) {
		printf("5G BLOCKRELAY=%s\n",strIsolationEnable);
		if (!gstrcmp(strIsolationEnable, T("yes")))
			val = 1;
		else if (!gstrcmp(strIsolationEnable, T("no")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("Error!"));
			goto setErr_advance;
		}

		if ( apmib_set(MIB_INIC_NOFORWARD, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("set MIB_INIC_NOFORWARD error!"));
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

	if (strnextEnable[0] || iNICwizardEnabled == 1){
		REPLACE_MSG(submitUrl);
	} else {
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
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}

void formiNICSetup(webs_t wp, char_t *path, char_t *query)
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

	if ( apmib_set( MIB_INIC_DISABLED, (void *)&disabled) == 0) {
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
		if ( apmib_set( MIB_INIC_AP_MODE, (void *)&mode) == 0) {
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
//EDX shakim start
#ifdef _SUPPORT_11AC_
		int channelWidth, preBand;
		if ( !apmib_get( MIB_INIC_N_CHANNEL_WIDTH, (void *)&channelWidth)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_wlan;
		}
		if ( val == 4 || val == 5 ) {
			if ( !apmib_get( MIB_INIC_BAND, (void *)&preBand)) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
				goto setErr_wlan;
			}
			if ( preBand != val ) {
				channelWidth = 2;	
				if ( apmib_set( MIB_INIC_N_CHANNEL_WIDTH, (void *)&channelWidth) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
					goto setErr_wlan;
				}
			}
		}
		if( val != 4 && val != 5 && channelWidth == 2) {
			channelWidth = 0;
			if ( apmib_set( MIB_INIC_N_CHANNEL_WIDTH, (void *)&channelWidth) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
				goto setErr_wlan;
			}	
		}
#endif
//EDX shakim end
		if ( apmib_set( MIB_INIC_BAND, (void *)&val) == 0) {
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
		apmib_set (MIB_INIC_MCAST_RATE, (void *)&val);
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
		apmib_set( MIB_INIC_NOFORWARD, (void *)&val);
	}
#endif
#ifdef _Customer_
        char_t *strChanWidth;
        strChanWidth = websGetVar(wp, T("iNIC_wlanNChanWidth"), T(""));      //no use
        if (strChanWidth[0]) {
                if (!gstrcmp(strChanWidth, T("0")))
                        val = 0;
                else if (!gstrcmp(strChanWidth, T("1")))
                        val = 1;
                else {
                        DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
                        goto setErr_wlan;
                }
                if ( apmib_set(MIB_INIC_N_CHANNEL_WIDTH, (void *)&val) == 0) {
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
		     if ( apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val) == 0) {
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
		if ( apmib_set( MIB_INIC_ABAND_MODE, (void *)&val) == 0) {
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


		if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup6))</script>"));
			goto setErr_wlan;
		}

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
			if ( apmib_set(MIB_INIC_REPEATER_SSID, (void *)strSSID) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
					goto setErr_wlan;
			}
		}
	}
	else {
		val = 0;
		if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_NETWORK_TYPE, (void *)&net) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
			goto setErr_wlan;
		}
#if 0	//EDX jerry

		//------------------------------------------------------------ Added by Tommy
		if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
			goto setErr_wlan;
		}

		if (apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
			goto setErr_wlan;
		}

		if (apmib_set( MIB_INIC_ENCRYPT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
			goto setErr_wlan;
		}

		if (apmib_set( MIB_INIC_AUTH_TYPE, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup12))</script>"));
			goto setErr_wlan;
		}
#endif	//EDX jerry
		//------------------------------------------------------------
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
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_wlan;
		}
	}

#if defined(_AUTO_MANUAL_SELECTED_CHAN_)
		strchtype = websGetVar(wp, T("channelseltype"), T(""));
		if (strchtype[0])
		  intChantemp = atoi(strchtype);

                if ( intChantemp == 0 && mode == 0)
                        strChan = websGetVar(wp, T("channelseltype"), T(""));
		else
			strChan = websGetVar(wp, T("chan"), T(""));
//EDX shakim start
#elif defined(_SUPPORT_11AC_)
	int valband;
	valband = (strBand[0] - '0' + 1);
	if (valband == 4 || valband == 5) {
		strChan = websGetVar(wp, T("chanAC"), T(""));
	} else {
		strChan = websGetVar(wp, T("chan"), T(""));
	}
//EDX shakim end
#else
			strChan = websGetVar(wp, T("chan"), T(""));
#endif

		//printf("Repeat Channel = %s\n",strChan);

	if ( strChan[0] ) {
		errno=0;
		chan = strtol( strChan, (char **)NULL, 10);
		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup16))</script>"));
			goto setErr_wlan;
		}
#if 0
		if ( strABand[0] ) {
			//printf("Debug aband channel=%d\n",chan);
				if ( apmib_set( MIB_ABAND_CHANNEL, (void *)&chan) == 0) {
		   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
					goto setErr_wlan;
				}
		}else{

					//printf("Debug Gband channel=%d\n",chan);
					if ( apmib_set( MIB_INIC_CHAN_NUM, (void *)&chan) == 0) {
			   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
						goto setErr_wlan;
					}

		}
#endif
		if ( apmib_set( MIB_INIC_CHAN_NUM, (void *)&chan) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
			goto setErr_wlan;
		}

	}

	strWisp = websGetVar(wp, T("wisp"), T(""));
	wisp = strtol( strWisp, (char **)NULL, 10);
	if ( mode == 1 || mode ==2 || mode==6 || wisp == 3) {
		strWlanMac = websGetVar(wp, T("wlanMac"), T(""));
		if (strWlanMac[0]) {
			if (strlen(strWlanMac)!=12 || !string_to_hex(strWlanMac, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup18))</script>"));
				goto setErr_wlan;
			}
			if ( !apmib_set(MIB_INIC_MAC_ADDR, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC1, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC2, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC3, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC4, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC5, (void *)tmpBuf)) {
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
		if ( !apmib_set(MIB_INIC_WL_LINKMAC6, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup33))</script>"));
			goto setErr_wlan;
		}
	}

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
		if ( apmib_set(MIB_INIC_DISABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable2))</script>"));
			goto setErr_wlan;
		}
	}

setwlan_ret:
//EDX shakim start
	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
		if (!gstrcmp(strWpsStatus, T("1"))){
			val = 1;
			if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_wlan;
			}
			val = 0;
			if ( apmib_set(MIB_INIC_WPS_DISPLAY_KEY, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_wlan;
			}
		}
//EDX shakim end
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	#ifdef HOME_GATEWAY
		//sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH, _CONFIG_SCRIPT_PROG, "gw", "all"); // Tommy
		//system(tmpBuf);
	#else
		//sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH, _CONFIG_SCRIPT_PROG, "ap", "all"); // Tommy
		//system(tmpBuf);
	#endif
#endif

#if 0 //EDX shakim
	//submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
//200703.29 kyle set wps configured when ssid is change.
	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
		if (!gstrcmp(strWpsStatus, T("1"))){
			val = 1;
			if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_wlan;
			}
			val = 0;
			if ( apmib_set(MIB_INIC_WPS_DISPLAY_KEY, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_wlan;
			}
		}
#endif //EDX shakim
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
    		if (strnextEnable[0] || iNICwizardEnabled == 1){
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
	}
    	return;
#else
    	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
    	strnextEnable = websGetVar(wp, T("redirect"), T(""));
    	if (strnextEnable[0] || iNICwizardEnabled == 1){
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
#endif

setErr_wlan:
	ERR_MSG(wp, tmpBuf);
}

/////////////////////////////////////////////////////////////////////////////
void formiNICAdvanceSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strAuth, *strFragTh, *strRtsTh, *strBeacon, *strPreamble, *strBlock;
	char_t *strRate, *strHiddenSSID, *strDtim, *strIapp, *strTx, *strProtection;
	char_t *strTxPower, *strWmm, *strDFS, *strBF, *strTurbo, *strNFixRate, *strChanWidth, *strNoForward;
	AUTH_TYPE_T authType;
	int val;
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
		if ( apmib_set(MIB_INIC_AUTH_TYPE, (void *)&authType) == 0) {
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
		if ( apmib_set(MIB_INIC_FRAG_THRESHOLD, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_RTS_THRESHOLD, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_BEACON_INTERVAL, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup7))</script>"));
			goto setErr_advance;
		}
	}

// RexHua
	strTxPower = websGetVar(wp, T("wlanTxPower"), T(""));	//no use
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
		if ( apmib_set(MIB_INIC_NOFORWARD, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_TURBO, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_advance;
		}
	}

	strWmm = websGetVar(wp, T("wlanWmm"), T(""));	//no use
	if (strWmm[0]) {
		printf("strWmm%s\n",strWmm);
		if (!gstrcmp(strWmm, T("yes")))
			val = 1;
		else if (!gstrcmp(strWmm, T("no")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup12))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_INIC_WMM, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
	}

	strDFS = websGetVar(wp, T("wlanDFS"), T(""));	//no use
	if (strDFS[0]) {
		if (!gstrcmp(strDFS, T("no")))
			val = 1;
		else if (!gstrcmp(strDFS, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup12))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_INIC_DFS, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
	}

	strBF = websGetVar(wp, T("wlanBF"), T(""));	//no use
	if (strBF[0]) {
		if (!gstrcmp(strBF, T("no")))
			val = 1;
		else if (!gstrcmp(strBF, T("yes")))
			val = 0;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup12))</script>"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_INIC_BF, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup13))</script>"));
			goto setErr_advance;
		}
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
//EDX shakim start
#ifdef _SUPPORT_11AC_
		else if (!gstrcmp(strChanWidth, T("2")))
			val = 2;
#endif
//EDX shakim end
		else {
			DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
			goto setErr_advance;
		}
		if ( apmib_set(MIB_INIC_N_CHANNEL_WIDTH, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("Invalid Channel Width 1"));
			goto setErr_advance;
		}
	}

	strNFixRate = websGetVar(wp, T("NtxRate"), T(""));
    if ( strNFixRate[0] ) {
		val = atoi(strNFixRate);
        if ( apmib_set(MIB_INIC_N_FIX_RATE, (void *)&val) == 0) {
        	strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup16))</script>"));
            goto setErr_advance;
		}
	}

	// set tx rate
	strRate = websGetVar(wp, T("txRate"), T(""));
	if ( strRate[0] ) {

		if ( strRate[0] == '0' ) { // auto
			val = 1;
			if ( apmib_set(MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup14))</script>"));
				goto setErr_advance;
			}
			val = atoi(strRate);


			if ( apmib_set(MIB_INIC_FIX_RATE, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup15))</script>"));
				goto setErr_advance;
			}
		}
		else
		{
			val = 0;
			if ( apmib_set(MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup14))</script>"));
				goto setErr_advance;
			}

			val = atoi(strRate);

	//		val = 1 << (val-1);

			if ( apmib_set(MIB_INIC_FIX_RATE, (void *)&val) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup15))</script>"));
				goto setErr_advance;
			}



			strRate = websGetVar(wp, T("basicrates"), T(""));
			if ( strRate[0] ) {
				val = atoi(strRate);
				if ( apmib_set(MIB_INIC_BASIC_RATE, (void *)&val) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup16))</script>"));
					goto setErr_advance;
				}
			}

			strRate = websGetVar(wp, T("operrates"), T(""));
			if ( strRate[0] ) {
				val = atoi(strRate);
				if ( apmib_set(MIB_INIC_SUPPORTED_RATE, (void *)&val) == 0) {
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
	if ( apmib_set(MIB_INIC_SUPPORTED_RATE, (void *)&val) == 0) {
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
	if ( apmib_set(MIB_INIC_BASIC_RATE, (void *)&val) == 0) {
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
			if ( apmib_set(MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set rate adaptive failed!"));
				goto setErr_advance;
			}
		}
		else  {
			val = 0;
			if ( apmib_set(MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
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
		if ( !apmib_get( MIB_INIC_HIDDEN_SSID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
			}
			if ( apmib_set(MIB_INIC_SUPPORTED_RATE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set Tx operation rate failed!"));
				goto setErr_advance;
			}
			if ( apmib_set(MIB_INIC_BASIC_RATE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("Set Tx basic rate failed!"));
				goto setErr_advance;
			}
		}
	}
	else { // set rate in operate, basic sperately
		val = 0;
		// disable rate adaptive
		if ( apmib_set(MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_SUPPORTED_RATE, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_BASIC_RATE, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_PREAMBLE_TYPE, (void *)&preamble) == 0) {
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
		if ( apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_DTIM_PERIOD, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_IAPP_DISABLED, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_BLOCK_RELAY, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_CTS, (void *)&val) == 0) {
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
		if ( apmib_set(MIB_INIC_PROTECTION_DISABLED, (void *)&val) == 0) {
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

	apmib_set(MIB_INIC_NOFORWARD, (void *)&val);
#endif
#ifdef _MCAST_RATE_SUPPORT_
{
	char *strMcastRate;
	strMcastRate = websGetVar (wp, T("McastRate"), T(""));
	if (strMcastRate[0]) {
		val = atoi (strMcastRate);
		apmib_set (MIB_INIC_MCAST_RATE, (void *)&val);
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

#ifdef WLAN_WPA
/////////////////////////////////////////////////////////////////////////////
void formiNICEncrypt(webs_t wp, char_t *path, char_t *query)
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
			if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
				goto setErr_encrypt;
			}
			intVal = 0;
			if ( apmib_set(MIB_INIC_WPS_DISPLAY_KEY, (void *)&intVal) == 0) {
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

#ifdef _INIC_MSSID_

char *temp;
int ssidID;
temp=websGetVar(wp, T("selected_ssid"), T(""));
ssidID=(int)temp[0] - '0';
switch(ssidID)
{
	case 0: apmib_set( MIB_INIC_SECURITY_MODE, (void *)&encrypt); break;
	case 1: apmib_set( MIB_INIC_SECURITY_MODE_1, (void *)&encrypt); break;
	case 2: apmib_set( MIB_INIC_SECURITY_MODE_2, (void *)&encrypt); break;
	case 3: apmib_set( MIB_INIC_SECURITY_MODE_3, (void *)&encrypt); break;
	case 4: apmib_set( MIB_INIC_SECURITY_MODE_4, (void *)&encrypt); break;
	case 5: apmib_set( MIB_INIC_SECURITY_MODE_5, (void *)&encrypt); break;
	case 6: apmib_set( MIB_INIC_SECURITY_MODE_6, (void *)&encrypt); break;
	case 7: apmib_set( MIB_INIC_SECURITY_MODE_7, (void *)&encrypt); break;
	default: break;
}

#else
	if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}
   #if defined (_Customer_) || defined (_Customer_)
        if (apmib_set( MIB_INIC_WDS_ENCRYPT, (void *)&encrypt) == 0) {
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

#ifdef _INIC_MSSID_
switch(ssidID)
{
	case 0: apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal); break;
	case 1: apmib_set( MIB_INIC_ENABLE_1X_1, (void *)&intVal); break;
	case 2: apmib_set( MIB_INIC_ENABLE_1X_2, (void *)&intVal); break;
	case 3: apmib_set( MIB_INIC_ENABLE_1X_3, (void *)&intVal); break;
	case 4: apmib_set( MIB_INIC_ENABLE_1X_4, (void *)&intVal); break;
	case 5: apmib_set( MIB_INIC_ENABLE_1X_5, (void *)&intVal); break;
	case 6: apmib_set( MIB_INIC_ENABLE_1X_6, (void *)&intVal); break;
	case 7: apmib_set( MIB_INIC_ENABLE_1X_7, (void *)&intVal); break;
	default: break;
}

#else
	if ( apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
		goto setErr_encrypt;
	}
#endif

#ifdef _Customer_
#ifdef _INIC_MSSID_ 
        strVal = websGetVar(wp, T("wlanWmm"), T(""));
        intVal = strVal[0] - '0';
switch(ssidID)
{
        case 0: apmib_set( MIB_INIC_WMM, (void *)&intVal); break;
        case 1: apmib_set( MIB_INIC_WMM_1, (void *)&intVal); break;
        case 2: apmib_set( MIB_INIC_WMM_2, (void *)&intVal); break;
        case 3: apmib_set( MIB_INIC_WMM_3, (void *)&intVal); break;
        case 4: apmib_set( MIB_INIC_WMM_4, (void *)&intVal); break;
        case 5: apmib_set( MIB_INIC_WMM_5, (void *)&intVal); break;
        case 6: apmib_set( MIB_INIC_WMM_6, (void *)&intVal); break;
        case 7: apmib_set( MIB_INIC_WMM_7, (void *)&intVal); break;
        default: break;
}
        strVal = websGetVar(wp, T("hiddenSSID"), T(""));
        intVal = strVal[0] - '0';
switch(ssidID)
{
        case 0: apmib_set( MIB_INIC_HIDDEN_SSID, (void *)&intVal); break;
        case 1: apmib_set( MIB_INIC_HIDDEN_SSID_1, (void *)&intVal); break;
        case 2: apmib_set( MIB_INIC_HIDDEN_SSID_2, (void *)&intVal); break;
        case 3: apmib_set( MIB_INIC_HIDDEN_SSID_3, (void *)&intVal); break;
        case 4: apmib_set( MIB_INIC_HIDDEN_SSID_4, (void *)&intVal); break;
        case 5: apmib_set( MIB_INIC_HIDDEN_SSID_5, (void *)&intVal); break;
        case 6: apmib_set( MIB_INIC_HIDDEN_SSID_6, (void *)&intVal); break;
        case 7: apmib_set( MIB_INIC_HIDDEN_SSID_7, (void *)&intVal); break;
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

	if ( apmib_set(MIB_INIC_1X_MODE, (void *)&intVal) == 0) {
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
	if ( apmib_set( MIB_INIC_ENABLE_SUPP_NONWPA, (void *)&intVal) == 0) {
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
		if ( apmib_set( MIB_INIC_SUPP_NONWPA, (void *)&suppNonWPA) == 0) {
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
		if ( apmib_set(MIB_INIC_WPA_AUTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
			goto setErr_encrypt;
		}

		// cipher suite
		strVal = websGetVar(wp, T("wpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if (strVal[0]) {
			if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
			if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;

#ifdef _INIC_MSSID_

switch(ssidID)
{
	case 0: apmib_set( MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip);   apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);break;
	case 1: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_1, (void *)&intAes);break;
	case 2: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_2, (void *)&intAes);break;
	case 3: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_3, (void *)&intAes);break;
	case 4: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_4, (void *)&intAes);break;
	case 5: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_5, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_5, (void *)&intAes);break;
	case 6: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_6, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_6, (void *)&intAes);break;
	case 7: apmib_set( MIB_INIC_WPA_CIPHER_SUITE_7, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_7, (void *)&intAes);break;
	default: break;
}

#else
			if ( apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
#if defined (_Customer_) || defined (_Customer_)
            if ( apmib_set(MIB_INIC_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_INIC_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
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
#ifdef _INIC_MSSID_

	switch(ssidID)
	{
		case 0:	apmib_set( MIB_INIC_ENCRYPT, (void *)&encrypt); break;
		case 1:	apmib_set( MIB_INIC_ENCRYPT_1, (void *)&encrypt); break;
		case 2:	apmib_set( MIB_INIC_ENCRYPT_2, (void *)&encrypt); break;
		case 3:	apmib_set( MIB_INIC_ENCRYPT_3, (void *)&encrypt); break;
		case 4:	apmib_set( MIB_INIC_ENCRYPT_4, (void *)&encrypt); break;
		case 5:	apmib_set( MIB_INIC_ENCRYPT_5, (void *)&encrypt); break;
		case 6:	apmib_set( MIB_INIC_ENCRYPT_6, (void *)&encrypt); break;
		case 7:	apmib_set( MIB_INIC_ENCRYPT_7, (void *)&encrypt); break;
		default: break;
	}
#else
	if (apmib_set( MIB_INIC_ENCRYPT, (void *)&encrypt) == 0) {
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

#ifdef _INIC_MSSID_

switch(ssidID)
{
	case 0:	apmib_set( MIB_INIC_WEP, (void *)&wep);  break;
	case 1:	apmib_set( MIB_INIC_WEP_1, (void *)&wep);break;
	case 2:	apmib_set( MIB_INIC_WEP_2, (void *)&wep);break;
	case 3:	apmib_set( MIB_INIC_WEP_3, (void *)&wep);break;
	case 4:	apmib_set( MIB_INIC_WEP_4, (void *)&wep);break;
	case 5:	apmib_set( MIB_INIC_WEP_5, (void *)&wep);break;
	case 6:	apmib_set( MIB_INIC_WEP_6, (void *)&wep);break;
	case 7:	apmib_set( MIB_INIC_WEP_7, (void *)&wep);break;
	default:break;
}
#else
		if ( apmib_set( MIB_INIC_WEP, (void *)&wep) == 0) {
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

#ifdef _INIC_MSSID_

switch(ssidID)
{
	case 0:	apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i);  break;
	case 1:	apmib_set( MIB_INIC_WEP_KEY_TYPE_1, (void *)&i);break;
	case 2:	apmib_set( MIB_INIC_WEP_KEY_TYPE_2, (void *)&i);break;
	case 3:	apmib_set( MIB_INIC_WEP_KEY_TYPE_3, (void *)&i);break;
	case 4:	apmib_set( MIB_INIC_WEP_KEY_TYPE_4, (void *)&i);break;
	case 5:	apmib_set( MIB_INIC_WEP_KEY_TYPE_5, (void *)&i);break;
	case 6:	apmib_set( MIB_INIC_WEP_KEY_TYPE_6, (void *)&i);break;
	case 7:	apmib_set( MIB_INIC_WEP_KEY_TYPE_7, (void *)&i);break;
	default:break;
}
#else
		if ( apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i) == 0) {
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

#ifdef _INIC_MSSID_
			switch(ssidID)
			{
				case 0:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i);  break;
				case 1:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_1, (void *)&i);break;
				case 2:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_2, (void *)&i);break;
				case 3:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_3, (void *)&i);break;
				case 4:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_4, (void *)&i);break;
				case 5:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_5, (void *)&i);break;
				case 6:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_6, (void *)&i);break;
				case 7:	apmib_set( MIB_INIC_WEP_DEFAULT_KEY_7, (void *)&i);break;
				default:break;
			}
#else
			if ( !apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_encrypt;
			}
#endif
		}




#ifdef _INIC_MSSID_

wepKey = websGetVar(wp, T("key1"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isiNICStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else				string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP64_KEY1, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP64_KEY1_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP64_KEY1_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP64_KEY1_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP64_KEY1_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP64_KEY1_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP64_KEY1_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP64_KEY1_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP128_KEY1, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP128_KEY1_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP128_KEY1_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP128_KEY1_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP128_KEY1_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP128_KEY1_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP128_KEY1_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP128_KEY1_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key2"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isiNICStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP64_KEY2, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP64_KEY2_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP64_KEY2_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP64_KEY2_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP64_KEY2_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP64_KEY2_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP64_KEY2_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP64_KEY2_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP128_KEY2, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP128_KEY2_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP128_KEY2_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP128_KEY2_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP128_KEY2_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP128_KEY2_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP128_KEY2_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP128_KEY2_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key3"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isiNICStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP64_KEY3, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP64_KEY3_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP64_KEY3_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP64_KEY3_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP64_KEY3_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP64_KEY3_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP64_KEY3_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP64_KEY3_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP128_KEY3, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP128_KEY3_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP128_KEY3_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP128_KEY3_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP128_KEY3_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP128_KEY3_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP128_KEY3_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP128_KEY3_7, (void *)&key);	break;
			default:break;
		}
}
wepKey = websGetVar(wp, T("key4"), T(""));
if( wepKey[0] && strlen(wepKey) == keyLen && !isiNICStar(wepKey) )
{
	if (strFormat[0] == '1')	strcpy(key, wepKey);
	else						string_to_hex(wepKey, key, keyLen);

	if (wep == WEP64)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP64_KEY4, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP64_KEY4_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP64_KEY4_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP64_KEY4_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP64_KEY4_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP64_KEY4_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP64_KEY4_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP64_KEY4_7, (void *)&key);	break;
			default:break;
		}
	else if	(wep == WEP128)
		switch(ssidID)
		{
			case 0: apmib_set(MIB_INIC_WEP128_KEY4, (void *)&key);	break;
			case 1: apmib_set(MIB_INIC_WEP128_KEY4_1, (void *)&key);	break;
			case 2: apmib_set(MIB_INIC_WEP128_KEY4_2, (void *)&key);	break;
			case 3: apmib_set(MIB_INIC_WEP128_KEY4_3, (void *)&key);	break;
			case 4: apmib_set(MIB_INIC_WEP128_KEY4_4, (void *)&key);	break;
			case 5: apmib_set(MIB_INIC_WEP128_KEY4_5, (void *)&key);	break;
			case 6: apmib_set(MIB_INIC_WEP128_KEY4_6, (void *)&key);	break;
			case 7: apmib_set(MIB_INIC_WEP128_KEY4_7, (void *)&key);	break;
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
				ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
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
				ret=apmib_set(MIB_INIC_WEP64_KEY2, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_INIC_WEP128_KEY2, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt14))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY2, (void *)key);
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
				ret=apmib_set(MIB_INIC_WEP64_KEY3, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_INIC_WEP128_KEY3, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt17))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY3, (void *)key);
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
				ret=apmib_set(MIB_INIC_WEP64_KEY4, (void *)key);
			else if (wep == WEP128)
				ret=apmib_set(MIB_INIC_WEP128_KEY4, (void *)key);
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
			if ( !isiNICStar(wepKey) )
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

				if (wep == WEP64)        ret=apmib_set(MIB_INIC_WEP64_KEY4, (void *)key);
				else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY4, (void *)key);

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
#ifdef _INIC_MSSID_
		//vance 2009.02.23 fixed pskvalue error
		switch(ssidID)
		{
			case 0:apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK, (void *)tmpBuf);break;
			case 1:apmib_get(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_1, (void *)tmpBuf);break;
			case 2:apmib_get(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_2, (void *)tmpBuf);break;
			case 3:apmib_get(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_3, (void *)tmpBuf);break;
			case 4:apmib_get(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_4, (void *)tmpBuf);break;
			case 5:apmib_get(MIB_INIC_WPA_PSK_FORMAT_5, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_5, (void *)tmpBuf);break;
			case 6:apmib_get(MIB_INIC_WPA_PSK_FORMAT_6, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_6, (void *)tmpBuf);break;
			case 7:apmib_get(MIB_INIC_WPA_PSK_FORMAT_7, (void *)&oldFormat);apmib_get(MIB_INIC_WPA_PSK_7, (void *)tmpBuf);break;
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
			case 0: apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK, (void *)strVal);	break;
			case 1: apmib_set(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_1, (void *)strVal);	break;
			case 2: apmib_set(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_2, (void *)strVal);	break;
			case 3: apmib_set(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_3, (void *)strVal);	break;
			case 4: apmib_set(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_4, (void *)strVal);	break;
			case 5: apmib_set(MIB_INIC_WPA_PSK_FORMAT_5, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_5, (void *)strVal);	break;
			case 6: apmib_set(MIB_INIC_WPA_PSK_FORMAT_6, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_6, (void *)strVal);	break;
			case 7: apmib_set(MIB_INIC_WPA_PSK_FORMAT_7, (void *)&intVal);	apmib_set(MIB_INIC_WPA_PSK_7, (void *)strVal);	break;
			default:break;
		}
#else
		apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_INIC_WPA_PSK, (void *)tmpBuf);
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
		if ( apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_encrypt;
		}
#if defined (_Customer_) || defined (_Customer_)
    if ( apmib_set(MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
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
		if ( !apmib_set(MIB_INIC_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_encrypt;
		}
        #if defined (_Customer_) || defined (_Customer_)
            if ( !apmib_set(MIB_INIC_WDS_WPA_PSK, (void *)strVal)) {
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
			if ( !apmib_set(MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&reKeyTime)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_WPA_GROUP_REKEY_TIME error!"));
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

			if ( !apmib_set(MIB_INIC_RS_PORT, (void *)&intVal)) {
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


			if ( !apmib_set(MIB_INIC_RS_IP, (void *)&inIp)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt12))</script>"));
				goto setErr_encrypt;
			}

			//Radius password
			strVal = websGetVar(wp, T("radiusPass"), T(""));
			if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt13))</script>"));
				goto setErr_encrypt;
			}


			if ( !apmib_set(MIB_INIC_RS_PASSWORD, (void *)strVal)) {
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
			if ( !apmib_set(MIB_INIC_RS_RETRY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_RS_RETRY error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("radiusTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid RS time value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_INIC_RS_INTERVAL_TIME, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_RS_INTERVAL_TIME error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("useAccount"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_INIC_ACCOUNT_RS_ENABLED, (void *)&intVal) == 0) {
  			DEBUGP(tmpBuf, T("Set MIB_INIC_ACCOUNT_RS_ENABLED mib error!"));
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
		if ( !apmib_set(MIB_INIC_ACCOUNT_RS_PORT, (void *)&intVal)) {
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
		if ( !apmib_set(MIB_INIC_ACCOUNT_RS_IP, (void *)&inIp)) {
			DEBUGP(tmpBuf, T("Set account RS IP-address error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountPass"), T(""));
		if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
			DEBUGP(tmpBuf, T("Account RS password length too long!"));
			goto setErr_encrypt;
		}
		if ( !apmib_set(MIB_INIC_ACCOUNT_RS_PASSWORD, (void *)strVal)) {
			DEBUGP(tmpBuf, T("Set account RS password error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountRetry"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid account RS retry value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_INIC_ACCOUNT_RS_RETRY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_ACCOUNT_RS_RETRY error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("accountTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Invalid account RS time value!"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_INIC_ACCOUNT_RS_INTERVAL_TIME, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_ACCOUNT_RS_INTERVAL_TIME error!"));
				goto setErr_encrypt;
			}
		}
		strVal = websGetVar(wp, T("accountUpdateEnabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_INIC_ACCOUNT_UPDATE_ENABLED, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set MIB_INIC_ACCOUNT_UPDATE_ENABLED mib error!"));
			goto setErr_encrypt;
		}
		strVal = websGetVar(wp, T("accountUpdateTime"), T(""));
		if (strVal[0]) {
			if ( !string_to_dec(strVal, &intVal) ) {
				DEBUGP(tmpBuf, T("Error! Invalid value of update time"));
				goto setErr_encrypt;
			}
			if ( !apmib_set(MIB_INIC_ACCOUNT_UPDATE_DELAY, (void *)&intVal)) {
				DEBUGP(tmpBuf, T("Set MIB_INIC_ACCOUNT_UPDATE_DELAY mib error!"));
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
			if ( apmib_set(MIB_INIC_WEP, (void *)&intVal) == 0) {
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
        //if ((strnextEnable[0] || iNICwizardEnabled == 1) && !strisApply[0]){
        if ( iNICwizardEnabled == 1 && !strisApply[0] ){
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
void formiNICAc(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddMac, *strDelMac, *strDelAllMac, *strVal, *submitUrl, *strEnabled, *strApply;
	char tmpBuf[100];
	int entryNum, i, enabled;
	MACFILTER_T macEntry;

	strAddMac = websGetVar(wp, T("addFilterMac"), T(""));
	strDelMac = websGetVar(wp, T("deleteSelFilterMac"), T(""));
	strDelAllMac = websGetVar(wp, T("deleteAllFilterMac"), T(""));
	strEnabled = websGetVar(wp, T("wlanAcEnabled"), T(""));

	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
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
			apmib_set( MIB_INIC_AC_SWITCH, (void *)&wlanacval);
		}
		wlanacstr1 =websGetVar(wp, T("accessctrlval1"), T(""));
		if(wlanacstr1[0])
		{
			wlanacval = atoi(wlanacstr1);
			apmib_set( MIB_INIC_AC1_SWITCH, (void *)&wlanacval);
		}
		wlanacstr2 =websGetVar(wp, T("accessctrlval2"), T(""));
		if(wlanacstr2[0])
		{
			wlanacval = atoi(wlanacstr2);
			apmib_set( MIB_INIC_AC2_SWITCH, (void *)&wlanacval);
		}
		wlanacstr3 =websGetVar(wp, T("accessctrlval3"), T(""));
		if(wlanacstr3[0])
		{
			wlanacval = atoi(wlanacstr3);
			apmib_set( MIB_INIC_AC3_SWITCH, (void *)&wlanacval);
		}

	#endif
	//printf("strIndex=%c\n",strIndex[0]);
	if (strAddMac[0]) {
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;


	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( apmib_set( MIB_INIC_AC_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( apmib_set( MIB_INIC_AC2_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( apmib_set( MIB_INIC_AC3_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( apmib_set( MIB_INIC_AC4_ENABLED, (void *)&enabled) == 0) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( apmib_set( MIB_INIC_AC_ENABLED, (void *)&enabled) == 0) {
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


	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_get(MIB_INIC_AC2_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_get(MIB_INIC_AC3_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_get(MIB_INIC_AC4_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}

	#else
		if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_ac;
		}
	#endif

		if ( (entryNum + 1) > MAX_INIC_AC_NUM) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc5))</script>"));
			goto setErr_ac;
		}

		// set to MIB. try to delete it first to avoid duplicate case
	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			apmib_set(MIB_INIC_AC_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_INIC_AC_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			apmib_set(MIB_INIC_AC2_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_INIC_AC2_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			apmib_set(MIB_INIC_AC3_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_INIC_AC3_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			apmib_set(MIB_INIC_AC4_ADDR_DEL, (void *)&macEntry);
			if ( apmib_set(MIB_INIC_AC4_ADDR_ADD, (void *)&macEntry) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
				goto setErr_ac;
			}
		}
	#else
		apmib_set(MIB_INIC_AC_ADDR_DEL, (void *)&macEntry);
		if ( apmib_set(MIB_INIC_AC_ADDR_ADD, (void *)&macEntry) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
			goto setErr_ac;
		}
	#endif


	}

	/* Delete entry */
	if (strDelMac[0]) {


	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_get(MIB_INIC_AC2_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_get(MIB_INIC_AC3_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_get(MIB_INIC_AC4_NUM, (void *)&entryNum)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_ac;
		}
	#endif


		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)&macEntry) = (char)i;


	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
				if ( strIndex[0] == '1' ) {
					if ( !apmib_get(MIB_INIC_AC_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_INIC_AC_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '2' ) {
					if ( !apmib_get(MIB_INIC_AC2_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_INIC_AC2_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '3' ) {
					if ( !apmib_get(MIB_INIC_AC3_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_INIC_AC3_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
				else if ( strIndex[0] == '4' ) {
					if ( !apmib_get(MIB_INIC_AC4_ADDR, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
						goto setErr_ac;
					}
					if ( !apmib_set(MIB_INIC_AC4_ADDR_DEL, (void *)&macEntry)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
						goto setErr_ac;
					}
				}
	#else
				if ( !apmib_get(MIB_INIC_AC_ADDR, (void *)&macEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
					goto setErr_ac;
				}
				if ( !apmib_set(MIB_INIC_AC_ADDR_DEL, (void *)&macEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
					goto setErr_ac;
				}
	#endif


			}
		}
	}

	/* Delete all entry */
	if ( strDelAllMac[0]) {


	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
		if ( strIndex[0] == '1' ) {
			if ( !apmib_set(MIB_INIC_AC_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '2' ) {
			if ( !apmib_set(MIB_INIC_AC2_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '3' ) {
			if ( !apmib_set(MIB_INIC_AC3_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
		else if ( strIndex[0] == '4' ) {
			if ( !apmib_set(MIB_INIC_AC4_ADDR_DELALL, (void *)&macEntry)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
				goto setErr_ac;
			}
		}
	#else
		if ( !apmib_set(MIB_INIC_AC_ADDR_DELALL, (void *)&macEntry)) {
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

int wliNICSiteSurveyTbl(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i=0, wait_time=0 ,j,z, kakaso, apCount=0, len, ap_mode, IQfinish=0, intOne=0, RelApCount=0;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[8192],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY5G SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];
	int GoSiteSurvey=0;
/*
        system("ifconfig apclii0 down");
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
	if (piNICStatus==NULL)
	{
		piNICStatus = calloc(1, sizeof(SS_STATUS_T5G));
		if ( piNICStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}

#ifdef __TARGET_BOARD__
	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(INIC_IF,  &data) < 0)
	{
		nBytesSent = websWrite(wp, T("<script>document.write(showText(aspFormWlSiteSurvey1ABAND))</script>"));
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

	piNICStatus->number = 0;

	if ((len = getWlSiteSurveyResult(INIC_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(piNICStatus);
		piNICStatus = NULL;
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
			#if 1
			char ch_width[7];
			#endif
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

		//printf("total len = %d single_length = %d \n%s", len, single_length, sp);
		printf("%s", sp);
		// santy check
		// 1. valid char data
		// 2. rest length is larger than per line length ==> (1+8+8+35+19+8+1)
		//    Channel+SSID+Bssid+WepStatus+AuthMode+Signal+WiressMode+NetworkType
			
		
		while (len >= single_length)
		{

			memset (&SiteSurvey[i], 0x00, sizeof(SiteSurvey[i]));
			psite_survey = (struct site_survey_t *) sp;

			//memcpy (SiteSurvey[i].channel, psite_survey->channel, 4);//2);
			sprintf(SiteSurvey[i].channel, "%d", atoi(psite_survey->channel));
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
				strcpy (SiteSurvey[i].webstatus, "NONE");
				strcpy (SiteSurvey[i].authmode, "OPEN");

			}else if (strstr (psite_survey->security, "WEP")){
				strcpy(SiteSurvey[i].webstatus, "WEP");
				strcpy (SiteSurvey[i].authmode, "OPEN");

			}else if (strstr (psite_survey->security, "WPAPSK/AES") || strstr (psite_survey->security, "WPAPSK/TKIPAES")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "AES");

			}else if (strstr (psite_survey->security, "WPAPSK/TKIP")){
				strcpy (SiteSurvey[i].authmode, "WPAPSK");
				strcpy(SiteSurvey[i].webstatus, "TKIP");

			}else if (strstr (psite_survey->security, "WPA2PSK/TKIPAES")){	//fixed RootAP(WPA2-Mix, TP-Link) DUT 2.4G show WPA2(TKIP) not WPA2(AES)
				strcpy(SiteSurvey[i].authmode, "WPA2PSK");
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
			//printf("%d, SiteSurvey[i].extch:%s\n",i ,SiteSurvey[i].extch);
			if (strstr (SiteSurvey[i].extch, "AB"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, "BE"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");

			#if 1
			memcpy (SiteSurvey[i].ch_width, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].ch_width, "8"))
				strcpy (SiteSurvey[i].ch_width, "80M");
			else if (strstr (SiteSurvey[i].ch_width, "4"))
				strcpy (SiteSurvey[i].ch_width, "40M");
			else
				strcpy (SiteSurvey[i].ch_width, "20M");
			#endif

			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY5G SiteSurveyBUF[100]={0};
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

		


		
#if defined(_WIFI_ROMAING_)
		apCount = 30;	//fixed 5G sitesurvey web server reboot issue.
#else
		apCount = 50;
#endif
		piNICStatus->number = i;
		RelApCount = apCount;
		

		for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepiNICStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
		{
			piNICStatus->bssdb[i] = SiteSurvey[i];
  			sprintf(tmpBuf,"%s",piNICStatus->bssdb[i].ssid);
			int AbandZeroPower = atoi(piNICStatus->bssdb[i].signal);

			if(strlen(tmpBuf)>1 && AbandZeroPower != 0)	//EDX yihong, fixed if SSID is one string, system cannot pop in web UI. 
			{
				GoSiteSurvey=1;
			}
			else if(strlen(tmpBuf) == 1 && AbandZeroPower != 0){
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
  				
			char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
			int PowerAband = atoi(piNICStatus->bssdb[i].signal);
			int PowerUpAband = ScanListSignalScaleMapping92CD(PowerAband);
#endif
//			if(strlen(tmpBuf)>1){
			if(GoSiteSurvey == 1){
				nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr style=\"background-color:#BBCCDE;\" align=\"center\"><td align=center width=\"10%%\" class=\"\"><input type=\"radio\" name=\"select%d\" id=\"select%d\" value=\"sel%d\" onClick=\"connectClick('sel%d')\"></td>\n"),i,i,i,i);  //Select
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"72%%\">\n"));

				nBytesSent += websWrite(wp, T("        <font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
				nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
				nBytesSent += websWrite(wp, T("					<script>document.forms['formiNICSiteSurvey'].elements['ssid'+%d].value=\""),i);
				nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
				nBytesSent += websWrite(wp, T("\"     </script>\n"));
				nBytesSent += websWrite(wp, T("    </td>\n"));
#if defined(_SIGNAL_FORMULA_)
				if(PowerUpAband <= 40)
					nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font color=\"#DF0101\" size=\"2\">%d%%</font>"),PowerUpAband);  //Signal
				else	
					nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">%d%%</font>"),PowerUpAband);  //Signal
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpAband); 
#else
				if(atoi(piNICStatus->bssdb[i].signal) <= 40)
					nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font color=\"#DF0101\" size=\"2\">%s%%</font>"),piNICStatus->bssdb[i].signal);  //Signal
				else	
					nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">%s%%</font>"),piNICStatus->bssdb[i].signal);  //Signal
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].signal); 
#endif
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, piNICStatus->bssdb[i].channel); //Channel
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"\">\n"),i);  //encryption
				nBytesSent += websWrite(wp, T("	<script>document.forms['formiNICSiteSurvey'].elements['encryption'+%d].value=\"%s\""),i ,piNICStatus->bssdb[i].authmode);
				nBytesSent += websWrite(wp, T("	</script>\n"));
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"\">\n"),i);
				nBytesSent += websWrite(wp, T("	<script>document.forms['formiNICSiteSurvey'].elements['ciphersutie'+%d].value=\"%s\""),i ,piNICStatus->bssdb[i].webstatus);
				nBytesSent += websWrite(wp, T("	</script>\n"));

				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,piNICStatus->bssdb[i].bssid[0],piNICStatus->bssdb[i].bssid[1],piNICStatus->bssdb[i].bssid[2],piNICStatus->bssdb[i].bssid[3],piNICStatus->bssdb[i].bssid[4],piNICStatus->bssdb[i].bssid[5]); 

				nBytesSent += websWrite(wp, T("<tr>\n"));
				nBytesSent += websWrite(wp, T("       <td style=\"display:none\" align=center class=\"table2\">\n"));
				nBytesSent += websWrite(wp, T("<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);
				#if 1
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"ch_width%d\" value=\"%s\">\n"),i,SiteSurvey[i].ch_width);
				#endif
				nBytesSent += websWrite(wp, T("       </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
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

		printf("\nRelApCount=%d\n",RelApCount);
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">"),apCount);


#else
		//for debug
		apCount=3;
		nBytesSent += websWrite(wp, T("<input type='hidden' name='apCount' value='%d'>\n"),apCount);
		for(i = 0 ; i < apCount ; i++)
		{
			nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr style=\"background-color:#BBCCDE;\" align=\"center\"><td align=center width=\"10%%\" class=\"\"><input type=\"radio\" name=\"select%d\" id=\"select%d\" value=\"sel%d\" onClick=\"connectClick('sel%d')\"></td>\n"),i,i,i,i);  //Select
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"72%%\">\n"));

				nBytesSent += websWrite(wp, T("        <font size=\"2\">SSID5g%d</font>\n"),i);    // SSID
				nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
				nBytesSent += websWrite(wp, T("					<script>document.forms['formiNICSiteSurvey'].elements['ssid'+%d].value=\"SSID%d\";</script>\n"),i,i);
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"18%%\"><font size=\"2\">100%%</font>"));  //Signal
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"100%%\">\n"),i); 
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"44\">\n"),i); //Channel
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
//////
int wliNICSiteSurveyonlyTbl(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, i, wait_time=0 ,j,z, kakaso, apCount, len, ap_mode;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[8192],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY5G SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];


//        system("ifconfig apclii0 down");
//        sleep(1);
//        system("killall udhcpc"); 

	if (piNICStatus==NULL)
	{
		piNICStatus = calloc(1, sizeof(SS_STATUS_T5G));
		if ( piNICStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}


	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(INIC_IF,  &data) < 0)
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

	piNICStatus->number = 0;

	if ((len = getWlSiteSurveyResult(INIC_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(piNICStatus);
		piNICStatus = NULL;
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
			#if 1
			char ch_width[7];
			#endif
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

			//memcpy (SiteSurvey[i].channel, psite_survey->channel, 2); 
			sprintf(SiteSurvey[i].channel, "%d", atoi(psite_survey->channel));
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
				strcpy (SiteSurvey[i].authmode, "WEP");
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
			//printf("%d, SiteSurvey[i].extch:%s\n",i ,SiteSurvey[i].extch);
			if (strstr (SiteSurvey[i].extch, "AB"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, "BE"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");

			#if 1
			memcpy (SiteSurvey[i].ch_width, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].ch_width, "8"))
				strcpy (SiteSurvey[i].ch_width, "80M");
			else if (strstr (SiteSurvey[i].ch_width, "4"))
				strcpy (SiteSurvey[i].ch_width, "40M");
			else
				strcpy (SiteSurvey[i].ch_width, "20M");
			#endif

			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY5G SiteSurveyBUF[100]={0};
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

#if 0
		apCount = i;
#else
		apCount = 50;
#endif
		
		piNICStatus->number = i;

		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">"),apCount);

			for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepiNICStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
			{
				piNICStatus->bssdb[i] = SiteSurvey[i];
    				sprintf(tmpBuf,"%s",piNICStatus->bssdb[i].ssid);
				char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
				int PowerAband = atoi(piNICStatus->bssdb[i].signal);
				int PowerUpAband = ScanListSignalScaleMapping92CD(PowerAband);
#endif
				nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				nBytesSent += websWrite(wp, T("<tr class=\"listtableText\" align=\"center\">\n"));  //Select
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"37%%\">\n"));

				nBytesSent += websWrite(wp, T("        <font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
				nBytesSent += websWrite(wp, T("					<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
				nBytesSent += websWrite(wp, T("					<script>document.forms['formiNICSiteSurvey'].elements['ssid'+%d].value=\""),i);
				nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
				nBytesSent += websWrite(wp, T("\"     </script>\n"));
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"21%%\"><font size=\"2\">%02X:%02X:%02X:%02X:%02X:%02X</font>"),piNICStatus->bssdb[i].bssid[0],piNICStatus->bssdb[i].bssid[1],piNICStatus->bssdb[i].bssid[2],piNICStatus->bssdb[i].bssid[3],piNICStatus->bssdb[i].bssid[4],piNICStatus->bssdb[i].bssid[5]);  //MAC
				nBytesSent += websWrite(wp, T("		</td>\n"));
				
				piNICStatus->bssdb[i].wirelessmode[8]='\0';
				
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"12%%\"><font size=\"2\">%s</font>"),piNICStatus->bssdb[i].wirelessmode);  //b/g/n
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"6%%\"><font size=\"2\">%s</font>"),piNICStatus->bssdb[i].channel);  //channel
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"6%%\"><font size=\"2\">%s</font>"),"AP");  //type
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%s</font>"),piNICStatus->bssdb[i].authmode);  //authmode
				nBytesSent += websWrite(wp, T("    </td>\n"));
#if defined(_SIGNAL_FORMULA_)
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%d%%</font>"),PowerUpAband);  //Signal
#else
				nBytesSent += websWrite(wp, T("    <td align=\"center\" width=\"9%%\"><font size=\"2\">%s%%</font>"),piNICStatus->bssdb[i].signal);  //Signal
#endif
				nBytesSent += websWrite(wp, T("    </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));

				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"band%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].wirelessmode); 
#if defined(_SIGNAL_FORMULA_)
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpAband); 
#else
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].signal); 
#endif
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,piNICStatus->bssdb[i].bssid[0],piNICStatus->bssdb[i].bssid[1],piNICStatus->bssdb[i].bssid[2],piNICStatus->bssdb[i].bssid[3],piNICStatus->bssdb[i].bssid[4],piNICStatus->bssdb[i].bssid[5]); 

				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, piNICStatus->bssdb[i].channel); //Channel
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].authmode);  //encryption
				nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].webstatus);

				nBytesSent += websWrite(wp, T("<tr>\n"));
				nBytesSent += websWrite(wp, T("       <td style=\"display:none\" align=center class=\"table2\">\n"));

				nBytesSent += websWrite(wp, T("<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);
				#if 1
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"ch_width%d\" value=\"%s\">\n"),i,SiteSurvey[i].ch_width);
				#endif
				websWrite(wp, T("       </td>\n"));
				websWrite(wp, T("</tr>\n"));
			}
	}
	return nBytesSent;
}
//////
//formiNICSiteSurvey-start
void formiNICSiteSurvey(webs_t wp, char_t *path, char_t *query)
{
 	char_t *submitUrl, *refresh, *connect, *done, *strSel;
 	char_t *strSSID, *strChan;
	int status, idx, encrypt;
	unsigned char res, *pMsg=NULL;
	int apEnable=1, len;
	char tmpBuf[100], data[8192];
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
			if ( apmib_set(MIB_INIC_REPEATER_SSID, (void *)strSSID) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlSiteSurvey13))</script>"));
				goto ss_err;
			}
			printf("siteform ssid=%s\n",strSSID);


			chan=strtol(strChan, (char**)NULL, 10);
			printf("siteform channel=%d\n",chan);
			if ( apmib_set( MIB_INIC_CHAN_NUM, (void *)&chan) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup17))</script>"));
				goto ss_err;
			}
			 apmib_set(MIB_AP_ROUTER_SWITCH, (void *)&apEnable);
		}
	}
	
	
	strVal = websGetVar(wp, T("ifname"), T(""));
	if(strVal[0])
	{
		sprintf(INIC_IF,"%s",strVal);
	}

	refresh = websGetVar(wp, T("refresh"), T(""));
	if ( refresh[0] ) {

		//sprintf(tmpBuf, "iwpriv wlan1 set_mib ssid2scan=");
		//system(tmpBuf);
		//issue scan request

		wait_time = 0;
		memset(&data, 0, sizeof(data));

		if (getWlSiteSurveyRequest(INIC_IF,  &data) < 0)
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

		piNICStatus->number = 0;

		if ((len = getWlSiteSurveyResult(INIC_IF, &data, sizeof(data))) < 0)
		{
			nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
			free(piNICStatus);
			piNICStatus = NULL;
			return nBytesSent;
		}

		sitesurveyed5G = 1;
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		return;
	}
	
//Not Modify-start
	connect = websGetVar(wp, T("connect"), T(""));
	if ( connect[0] ) {
	}
//Not Modify-end
	return;

ss_err:
	ERR_MSG(wp, tmpBuf);
}

//formiNICSiteSurvey-end

/////////////////////////////////////////////////////////////////////////////
int wirelessiNICClientList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent = 0, i, found=0, MCS=0, MCSValue=0;
	INIC_STA_INFO_T *pInfo;
	char buff[WEBS_BUFSIZE];

	apmib_get(MIB_INIC_N_FIX_RATE, (void *)&MCS);
	
	if (MCS == 0)
	{
		printf("------MCS=%d\n",MCS);
		MCSValue = 15;
	}
	else
	{
		printf("------MCSValue=%d\n",MCSValue);
		MCSValue = MCS - 1;
	}

	#ifdef __TARGET_BOARD__
	if (getInicStaInfo(INIC_IF,  (INIC_STA_INFO_Tp)buff ) < 0 ) {
		printf("<script>document.write(showText(aspWirelessClientList2))</script>!\n");
		return 0;
	}
	pInfo = (INIC_STA_INFO_T *)buff;

	for (i=0; i<pInfo->Num ; i++)
	{
		if (pInfo->Entry[i].Aid)
		{

			nBytesSent += websWrite(wp,
	   		T("<tr class=\"listtableText\" align=\"center\"><td>%02x:%02x:%02x:%02x:%02x:%02x</td>" //mac
			"<td>MCS%d</td>" // MCS
			"<td>%s</td>"    // BW
			"<td>%s</td>"    // PowerSaving
			"</tr>"),

			pInfo->Entry[i].Addr[0],pInfo->Entry[i].Addr[1],pInfo->Entry[i].Addr[2],pInfo->Entry[i].Addr[3],pInfo->Entry[i].Addr[4],pInfo->Entry[i].Addr[5]
			,MCSValue //,pInfo->Entry[i].TxRate.field.MCS
			,iNICGetBW(pInfo->Entry[i].BW)//iNICGetBW(pInfo->Entry[i].TxRate.field.BW),
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


/////////////////////////////////////////////////////////////////////////////
int wliNICAcList(int eid, webs_t wp, int argc, char_t **argv)
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
		if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '2' ) {
		if ( !apmib_get(MIB_INIC_AC2_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '3' ) {
		if ( !apmib_get(MIB_INIC_AC3_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	else if ( argv[0][0] == '4' ) {
		if ( !apmib_get(MIB_INIC_AC4_NUM, (void *)&entryNum))
		{
	  		websError(wp, 400, T("Get table entry error!\n"));
			return -1;
		}
	}
	#else
	if ( !apmib_get(MIB_INIC_AC_NUM, (void *)&entryNum))
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
			if ( !apmib_get(MIB_INIC_AC_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '2' ) {
			if ( !apmib_get(MIB_INIC_AC2_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '3' ) {
			if ( !apmib_get(MIB_INIC_AC3_ADDR, (void *)&entry)) return -1;
		}
		else if ( argv[0][0] == '4' ) {
			if ( !apmib_get(MIB_INIC_AC4_ADDR, (void *)&entry)) return -1;
		}
	#else
		if ( !apmib_get(MIB_INIC_AC_ADDR, (void *)&entry)) return -1;
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

////////////////////////////WPS /////////////////////////////////
void formiNICWpsEnable(webs_t wp, char_t *path, char_t *query)
{

	char_t *submitUrl, *strWpsEnable ;
#ifdef _Customer_
	char_t *strWpsReleaseConfig ;
	strWpsReleaseConfig =  websGetVar(wp, T("ReleaseConfig"), T(""));
#endif
	int val;
	char tmpBuf[100];
	strWpsEnable =  websGetVar(wp, T("wpsEnable"), T(""));

	printf ("%s %d strWpsEnable = %s\n", __FUNCTION__, __LINE__, strWpsEnable);

	if (!gstrcmp(strWpsEnable, T("ON")))
		val = 1;
	else
		val = 0;

	printf ("%s %d val = %d\n", __FUNCTION__, __LINE__, val);

	if ( apmib_set(MIB_INIC_WPS_ENABLE, (void *)&val) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_advance;
	}

	if( val == 1){
		val = 7;

	}else{
		val = 0;
	}

	printf ("%s %d val = %d\n", __FUNCTION__, __LINE__, val);

	if ( apmib_set(MIB_INIC_WPS_CONFIG_MODE, (void *)&val) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_advance;
	}
#ifdef _Customer_
	if(strWpsReleaseConfig[0]){
		val=0;
		if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) == 0) {
                	DEBUGP(tmpBuf, T("Setting INIC_WPS_CONFIG_TYPE error"));
                	goto setErr_advance;
        	}	
	}
#endif
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	//sprintf(tmpBuf, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG); // Tommy
	//system(tmpBuf);
#endif

	system("/bin/scriptlib_util.sh reloadFlash");
	sprintf(tmpBuf, "/bin/inic_wps.sh %s 3 0 0", INIC_IF);
	system(tmpBuf);
	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl); // Tommy
	printf ("%s %d submitUrl = %s\n", __FUNCTION__, __LINE__, submitUrl);
	websRedirect(wp, submitUrl);

	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}

////////////////////WpsStart/////////////////////////////////////
void formiNICWpsStart(webs_t wp, char_t *path, char_t *query)
{
   char_t *submitUrl, *strModeOpt ,*pingCode,*strConfigMode;
	int modeVal,configValue,i;
	char tmpBuf[100];
	char pinNumber[10];//vance
	/*
	configValue=0;
	strConfigMode = websGetVar(wp, T("confMode"), T(""));
	if (strConfigMode[0]){
		if(strConfigMode[0]=='0')
			configValue=0;
		else if(strConfigMode[0]=='1')
			configValue=1;
	}
		if ( apmib_set(MIB_WPS_INTERNAL_REG, (void *)&configValue) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}
		apmib_update(CURRENT_SETTING);
		*/
	strModeOpt = websGetVar(wp, T("configOption"), T(""));
	if (strModeOpt[0]) {
		if (!gstrcmp(strModeOpt, T("pin")))
			modeVal = 1;
		else if (!gstrcmp(strModeOpt, T("pbc")))
			modeVal = 2;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEnable1))</script>"));
			goto setErr_advance;
		}
		pingCode=websGetVar(wp, T("pinCode"), T(""));
		if(modeVal == 1 ){
			if(!pingCode[0] || configValue==1){
				sprintf(tmpBuf, "inic_wps.sh %s %d -1 0",INIC_IF, modeVal);
			}
			else{
				if (strlen(pingCode) == 4 || strlen(pingCode) == 8) //EDX, Ken 2015_09_16 add, Avoid pingCode is "systemcode".
				{
					sprintf(pinNumber, "%s",pingCode);

					for(i=0;i<strlen(pinNumber);i++)
					{
						if(pinNumber[i]<48 || 57<pinNumber[i])
						{
							DEBUGP(tmpBuf, T("<script>document.write('Input pingCode error!')</script>"));
							goto setErr_advance;
						}
					}
					sprintf(tmpBuf, "inic_wps.sh %s %d %s 0",INIC_IF, modeVal,pinNumber);
				}
				else
				{
					DEBUGP(tmpBuf, T("<script>document.write('Input pingCode error!')</script>"));
					goto setErr_advance;
				}
				
			}
		}
		else if(modeVal == 2 ){
			sprintf(tmpBuf, "inic_wps.sh %s %d 0 0", INIC_IF, modeVal);
		}
		system(tmpBuf);
	}
		if ( apmib_set(MIB_WPS_PING_CODE, (void *)&pinNumber) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_advance;
		}


//	system("iwpriv ra0 set SSID=`flash all2 | grep ^SSID= | cut -f 2 -d =`");

	apmib_reinit();

#ifndef NO_ACTION
	//sprintf(tmpBuf, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG); // Tommy
	//system(tmpBuf);
#endif


	submitUrl = websGetVar(wp, T("wlan-url"), T(""));   // hidden page
	//REPLACE_MSG(submitUrl); // Tommy
	//OK_MSG(wp, submitUrl);

	websRedirect(wp, submitUrl);
	return;
setErr_advance:
	ERR_MSG(wp, tmpBuf);
}
void formiNICWdsEncrypt(webs_t wp, char_t *path, char_t *query)
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
	if (apmib_set( MIB_INIC_WDS_ENCRYPT, (void *)&encrypt) == 0) {
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
		if ( apmib_set(MIB_INIC_WPA_AUTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt4))</script>"));
			goto setErr_encrypt;
		}

		// cipher suite
		strVal = websGetVar(wp, T("wdsWpaCipher"), T(""));
		intVal = strVal[0] - '0';
		if (strVal[0]) {
			if (intVal==1 || intVal==3)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
			if (intVal==2 || intVal==3) intAes=WPA_CIPHER_AES; else intAes=0;

			if ( apmib_set(MIB_INIC_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
				goto setErr_encrypt;
			}
			if ( apmib_set(MIB_INIC_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
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
	if (apmib_set( MIB_INIC_WDS_ENCRYPT, (void *)&encrypt) == 0) {
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

		if ( apmib_set( MIB_INIC_WEP, (void *)&wep) == 0) {
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
		if ( apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i) == 0) {
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
			if ( !apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i ) ) {
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt11))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt14))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY2, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY2, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt17))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY3, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY3, (void *)key);
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
			if ( !isiNICStar(wepKey) ) {
				if (strFormat[0] == '1') // ascii
					strcpy(key, wepKey);
				else { // hex
					if ( !string_to_hex(wepKey, key, keyLen)) {
		   				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt19))</script>"));
						goto setErr_encrypt;
					}
				}
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY4, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY4, (void *)key);
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
		apmib_get(MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_INIC_WDS_WPA_PSK, (void *)tmpBuf);
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

		if ( apmib_set(MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
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
		if ( !apmib_set(MIB_INIC_WDS_WPA_PSK, (void *)strVal)) {
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
			if ( apmib_set(MIB_INIC_WEP, (void *)&intVal) == 0) {
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
#ifdef _INIC_MSSID_
void formiNICMultipleSSID(webs_t wp, char_t *path, char_t *query)
{
int val;
char_t *ssid1, *ssid2, *ssid3, *ssid4, *ssid5, *ssid6;

ssid1=websGetVar(wp,T("SSID1"),T(""));
ssid2=websGetVar(wp,T("SSID2"),T(""));
ssid3=websGetVar(wp,T("SSID3"),T(""));
ssid4=websGetVar(wp,T("SSID4"),T(""));
ssid5=websGetVar(wp,T("SSID5"),T(""));
ssid6=websGetVar(wp,T("SSID6"),T(""));

if(gstrcmp(ssid1,T(""))) apmib_set(MIB_INIC_SSID_1, (void *)websGetVar(wp,T("SSID1"),T("")));
if(gstrcmp(ssid2,T(""))) apmib_set(MIB_INIC_SSID_2, (void *)websGetVar(wp,T("SSID2"),T("")));
if(gstrcmp(ssid3,T(""))) apmib_set(MIB_INIC_SSID_3, (void *)websGetVar(wp,T("SSID3"),T("")));
if(gstrcmp(ssid4,T(""))) apmib_set(MIB_INIC_SSID_4, (void *)websGetVar(wp,T("SSID4"),T("")));
if(gstrcmp(ssid5,T(""))) apmib_set(MIB_INIC_SSID_5, (void *)websGetVar(wp,T("SSID5"),T("")));
if(gstrcmp(ssid6,T(""))) apmib_set(MIB_INIC_SSID_6, (void *)websGetVar(wp,T("SSID6"),T("")));

val=atoi(websGetVar(wp,T("mirror1"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_1, (void *)&val);
val=atoi(websGetVar(wp,T("mirror2"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_2, (void *)&val);
val=atoi(websGetVar(wp,T("mirror3"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_3, (void *)&val);
val=atoi(websGetVar(wp,T("mirror4"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_4, (void *)&val);
val=atoi(websGetVar(wp,T("mirror5"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_5, (void *)&val);
val=atoi(websGetVar(wp,T("mirror6"),T("")));apmib_set(MIB_INIC_SSID_MIRROR_6, (void *)&val);

if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID1"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID2"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID3"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID4"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID5"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("hiddenSSID6"),T("")));apmib_set(MIB_INIC_HIDDEN_SSID_6, (void *)&val);}

if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable1"),T("")));apmib_set(MIB_INIC_WMM_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable2"),T("")));apmib_set(MIB_INIC_WMM_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable3"),T("")));apmib_set(MIB_INIC_WMM_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable4"),T("")));apmib_set(MIB_INIC_WMM_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable5"),T("")));apmib_set(MIB_INIC_WMM_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("WmmCapable6"),T("")));apmib_set(MIB_INIC_WMM_6, (void *)&val);}

if(gstrcmp(ssid1,T(""))) {val = atoi(websGetVar(wp,T("txrate1"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val = atoi(websGetVar(wp,T("txrate2"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val = atoi(websGetVar(wp,T("txrate3"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val = atoi(websGetVar(wp,T("txrate4"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val = atoi(websGetVar(wp,T("txrate5"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val = atoi(websGetVar(wp,T("txrate6"),T("")));val = 1 << (val-1);apmib_set(MIB_INIC_FIX_RATE_6, (void *)&val);}

#if defined(_ENVLAN_)
if(gstrcmp(ssid1,T(""))) {val=atoi(websGetVar(wp,T("vlanid1"),T("")));apmib_set(MIB_INIC_VLANID_1, (void *)&val);}
if(gstrcmp(ssid2,T(""))) {val=atoi(websGetVar(wp,T("vlanid2"),T("")));apmib_set(MIB_INIC_VLANID_2, (void *)&val);}
if(gstrcmp(ssid3,T(""))) {val=atoi(websGetVar(wp,T("vlanid3"),T("")));apmib_set(MIB_INIC_VLANID_3, (void *)&val);}
if(gstrcmp(ssid4,T(""))) {val=atoi(websGetVar(wp,T("vlanid4"),T("")));apmib_set(MIB_INIC_VLANID_4, (void *)&val);}
if(gstrcmp(ssid5,T(""))) {val=atoi(websGetVar(wp,T("vlanid5"),T("")));apmib_set(MIB_INIC_VLANID_5, (void *)&val);}
if(gstrcmp(ssid6,T(""))) {val=atoi(websGetVar(wp,T("vlanid6"),T("")));apmib_set(MIB_INIC_VLANID_6, (void *)&val);}
#endif //_ENVLAN_

#ifdef _LAN_WAN_ACCESS_
unsigned int LanWanValue = 0;
apmib_get( MIB_LAN_WAN_ACCESS, (void *)&LanWanValue);
LanWanValue &= 0x000FFFFF;

if(gstrcmp(ssid1,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue1"),T(""))) << 20;}
if(gstrcmp(ssid2,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue2"),T(""))) << 24;}
if(gstrcmp(ssid3,T(""))) {LanWanValue |= atoi(websGetVar(wp,T("lanWanAccessValue3"),T(""))) << 28;}
apmib_set(MIB_LAN_WAN_ACCESS,(void *)&LanWanValue);
#endif //_LAN_WAN_ACCESS_

apmib_update(CURRENT_SETTING);
OK_MSG(wp, websGetVar(wp, T("submit-url"), T("")));
}
#endif //_INIC_MSSID_

static inline int isAllStar(char *data)
{
	int i;
	for (i=0; i<strlen(data); i++) {
		if (data[i] != '*')
			return 0;
	}
	return 1;
}
/////

void formiNICbasic(webs_t wp, char_t *path, char_t *query) //EDX patrick add
{
	char_t *submitUrl, *strSSID, *strSSID1, *strChan, *strDisabled, *strBand,*strABand, *strAutoClone, *strVal, *strWisp, *strnextEnable ,*strSupportAllSecurity;
	char_t *strMode, *strMac1, *strMac2, *strMac3, *strMac4, *strMac5, *strMac6, *strWlanMac, *strWpsStatus,*repUseCrossBand;
	char_t *iqsetupclose, *isRepeaterBasicUI;
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

	char_t *strEncrypt, *strEnRadius, *strWpaCipher,*strWpa2Cipher, *str1xMode, *strDisabled1;
	ENCRYPT_T encrypt;
	int enableRS=0, getPSK=0, len, intTkip=0, intAes=0, intVal2;
	unsigned long reKeyTime;

	int intOne = 1, intZero = 0;
	char_t *use_apmode;
	use_apmode = websGetVar(wp, T("use_apmode"), T(""));
	#ifdef _TEXTFILE_CONFIG_
	if (use_apmode[0]) {
		apmib_set( MIB_IQSET_DISABLE, (void *)&intOne); 	// close iqsetup
		apmib_set( MIB_DHCP, (void *)&intZero);  	// DHCP_DISABLED=0, DHCP_CLIENT=1, DHCP_SERVER=2
		apmib_set( MIB_INIC_AP_MODE, (void *)&intZero);// 0=apmode
		apmib_set( MIB_WIZ_MODE, (void *)&intOne); // 0=router mode, 1=ap mode
		apmib_set( MIB_WAN_MODE, (void *)&intZero);
		apmib_set( MIB_OP_MODE, (void *)&intZero);
		#if defined(_DHCP_SWITCH_)
		apmib_set(MIB_DHCP_SWITCH_5G, (void *)&intOne);  // 1=get IP from root AP, 0=static ip
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
	strDisabled1 = websGetVar(wp, T("wlan0Enable"), T(""));
	
	printf("---------------------formiNICbasic---------------------\n");
	//------------------ check SSID on/off  ------------------//

	if(ssidIndex == 0){

		apmib_get( MIB_INIC_DISABLED, (void *)&disabled);

		if (strDisabled[0]) {
			if ( !gstrcmp(strDisabled, T("yes"))) disabled = 1;
			else disabled = 0;
		}

		if (strDisabled1[0]) {
			if ( !gstrcmp(strDisabled1, T("1"))) disabled = 0;
			else disabled = 1;
		}

		if ( apmib_set( MIB_INIC_DISABLED, (void *)&disabled) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
			goto setErr_basic;
		}

		printf("ssidIndex=%d, SSIDDisable=%d\n",ssidIndex, disabled);
	
		if(disabled)
		{
			/*if ( apmib_set( MIB_INIC_WPS_ENABLE, (void *)&intZero) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
				goto setErr_basic;
			}
			if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intZero) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup1))</script>"));
				goto setErr_basic;
			}*/
			goto setwlan_ret; // if disable, no need set, go to update flash
		}
		
	}
	else{
		//------------------ set  MSSID on/off 	------------------//
		#ifdef _INIC_MSSID_
	
		//printf("ssidIndex=%d, enableMSSID=%d\n",ssidIndex, enableMSSID);
		if(strM[0]) {
			if(!gstrcmp(strM, T("1"))) enableMSSID=1;
			else enableMSSID=0;

			if(enableMSSID) enableMSSID += ssidIndex;

			switch(ssidIndex)
			{
				case 1: apmib_set(MIB_INIC_SSID_MIRROR_1, (void *)&enableMSSID); break;
				case 2: apmib_set(MIB_INIC_SSID_MIRROR_2, (void *)&enableMSSID); break;
				case 3: apmib_set(MIB_INIC_SSID_MIRROR_3, (void *)&enableMSSID); break;
				case 4: apmib_set(MIB_INIC_SSID_MIRROR_4, (void *)&enableMSSID); break;
				default: break; 
			}
			printf("ssidIndex:%d, MIRROR:%d\n",ssidIndex, enableMSSID);
			if (!enableMSSID)
				goto setwlan_ret;   // if disable, no need set, go to update flash
		}
	
		#endif
	}

	
	printf("\n5G Wirelsss full setting !\n",ssidIndex);
	
	isRepeaterBasicUI = websGetVar(wp, T("isRepeaterBasicUI"), T(""));

	//------------------ WLAN VLAN ID ------------------//
	#ifdef _INIC_ENVLAN_
	strVal = websGetVar(wp, T("vlanid"), T(""));
	if(strVal[0])
	{
		val = strtol( strVal, (char **)NULL, 10);
		switch(ssidIndex)
		{
			case 1: apmib_set(MIB_INIC_VLANID_1, (void *)&val); break;
			case 2: apmib_set(MIB_INIC_VLANID_2, (void *)&val); break;
			case 3: apmib_set(MIB_INIC_VLANID_3, (void *)&val); break;
			case 4: apmib_set(MIB_INIC_VLANID_4, (void *)&val); break;
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
		#ifdef _INICMSSID_NOFORWARD_ 
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_NOFORWARD, (void *)&val); break;
			case 1: apmib_set(MIB_INIC_NOFORWARD1, (void *)&val); break;
			case 2: apmib_set(MIB_INIC_NOFORWARD2, (void *)&val); break;
			case 3: apmib_set(MIB_INIC_NOFORWARD3, (void *)&val); break;
			case 4: apmib_set(MIB_INIC_NOFORWARD4, (void *)&val); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_NOFORWARD, (void *)&val) == 0) {
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
		if ( apmib_set( MIB_INIC_AP_MODE, (void *)&mode) == 0) {
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
		if ( apmib_set( MIB_INIC_BAND, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_basic;
		}
	}


#ifdef _WIFI_11N_STANDARD_
	if (val == 2) {
		apmib_get(MIB_INIC_ENCRYPT, (void *)&intVal);
		if (intVal != ENCRYPT_WPA2) {
			intVal = ENCRYPT_DISABLED;
			apmib_set(MIB_INIC_ENCRYPT, (void *)&intVal);
			apmib_set(MIB_INIC_SECURITY_MODE, (void *)&intVal);
		}
		else {
			intVal = 0;
			apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intVal);
			intVal = 2;
			apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intVal);
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
			val = strtol( strChanWidth, (char **)NULL, 10);
			/*if (!gstrcmp(strChanWidth, T("0"))) {val = 0;}
			else if (!gstrcmp(strChanWidth, T("1"))) {val = 1;}
			else if (!gstrcmp(strChanWidth, T("2"))) {val = 2;}
			else {
				DEBUGP(tmpBuf, T("Invalid Channel Width 0"));
				goto setErr_basic;
			}*/
			if ( apmib_set(MIB_INIC_N_CHANNEL_WIDTH, (void *)&val) == 0) {
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

			if ( apmib_set( MIB_INIC_CHAN_NUM, (void *)&chan) == 0) {
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
	
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val); break;
			case 1: apmib_set(MIB_INIC_HIDDEN_SSID_1, (void *)&val); break;
			case 2: apmib_set(MIB_INIC_HIDDEN_SSID_2, (void *)&val); break;
			case 3: apmib_set(MIB_INIC_HIDDEN_SSID_3, (void *)&val); break;
			case 4: apmib_set(MIB_INIC_HIDDEN_SSID_4, (void *)&val); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormAdvanceSetup11))</script>"));
			goto setErr_basic;
		}		
		#endif
	}
	
	strHiddenSSID = websGetVar(wp, T("hiddenSSID2g"), T(""));
	if (strHiddenSSID[0]) {	
		if (!gstrcmp(strHiddenSSID, T("no")))
			val = 0;
		else if (!gstrcmp(strHiddenSSID, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("get hiddenSSID2g fail!"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("set MIB_WLAN_HIDDEN_SSID fail!"));
			goto setErr_basic;
		}		
	}

	#ifdef UNIVERSAL_REPEATER
	if(ssidIndex==0 && !isRepeaterBasicUI[0] ){
		// Universal Repeater
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
		if (mode == 6 || mode == 2 ) {
			val = (mode == 2) ? 0 : 1;
			if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
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
				if ( apmib_set(MIB_INIC_REPEATER_SSID, (void *)strSSID) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
						goto setErr_basic;
				}
			}
		}
		else {
			val = 0;
			if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformStatsErrRep))</script>"));
			goto setErr_basic;
			}
		}

		if (mode == 2 || mode == 3 || mode == 4 || mode == 6 || wisp == 3) {
			apmib_get(MIB_INIC_ENCRYPT, (void *)&tmp);
			if (tmp == 6) {
				val = 4;
				apmib_set(MIB_INIC_ENCRYPT, (void *)&val);
				val = 0;
				apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&val);
			}
		}

		//AP client mode type
		#ifndef _RESERVE_ENCRYPTION_SETTING_
		if (mode == 1 || mode ==2) {
			if (strMode[0]=='1')
				net = ADHOC;
			if (strMode[0]=='2')
				net = INFRASTRUCTURE;
			if ( apmib_set(MIB_INIC_NETWORK_TYPE, (void *)&net) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
				goto setErr_basic;
			}
			//------------------------------------------------------------ Added by Tommy
			if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_ENCRYPT, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_AUTH_TYPE, (void *)&intVal) == 0) {
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
		if ( apmib_set(MIB_INIC_AUTO_MAC_CLONE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup14))</script>"));
			goto setErr_basic;
		}
	}

	strSSID = websGetVar(wp, T("ssid"), T(""));
	repUseCrossBand = websGetVar(wp, T("repUseCrossBand"), T(""));
	if ( strSSID[0] && !repUseCrossBand[0]) {
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_SSID, (void *)strSSID); break;
			case 1: apmib_set(MIB_INIC_SSID_1, (void *)strSSID); break;
			case 2: apmib_set(MIB_INIC_SSID_2, (void *)strSSID); break;
			case 3: apmib_set(MIB_INIC_SSID_3, (void *)strSSID); break;
			case 4: apmib_set(MIB_INIC_SSID_4, (void *)strSSID); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_basic;
		}
		#endif
	}
	if ( strSSID[0] && repUseCrossBand[0]) {
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("MIB_WLAN_SSID ERROR!"));
			goto setErr_basic;
		}
	}
	strSSID = websGetVar(wp, T("ssid2g"), T(""));
	if ( strSSID[0] ) {
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("set MIB_WLAN_SSID fail!"));
			goto setErr_basic;
		}
	}


	if(ssidIndex==0){
		strWisp = websGetVar(wp, T("wisp"), T(""));
		wisp = strtol( strWisp, (char **)NULL, 10);
		if ( mode == 1 || mode ==2 || mode==6 || wisp == 3 ) {
			strWlanMac = websGetVar(wp, T("wlanMac"), T(""));
			if (strWlanMac[0]) {
				if (strlen(strWlanMac)!=12 || !string_to_hex(strWlanMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup18))</script>"));
					goto setErr_basic;
				}
				if ( !apmib_set(MIB_INIC_MAC_ADDR, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC1, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC2, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC3, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC4, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC5, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC6, (void *)tmpBuf)) {
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

	#ifdef _INIC_MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_INIC_SECURITY_MODE, (void *)&encrypt); break;
		case 1: apmib_set(MIB_INIC_SECURITY_MODE_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_INIC_SECURITY_MODE_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_INIC_SECURITY_MODE_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_INIC_SECURITY_MODE_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_basic;
	}
	#endif
	if(force_enable2g5g_ap){
	  if (apmib_set( MIB_SECURITY_MODE, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("set MIB_SECURITY_MODE fail!"));
		goto setErr_basic;
	  }
	}
	#if 0//defined(_Edimax_) || defined(_Customer_)
	//For Wlan WDS
	if (apmib_set( MIB_INIC_WDS_ENCRYPT, (void *)&encrypt) == 0) {
  	DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
		goto setErr_encrypt;
	}
	#endif

	strSupportAllSecurity=websGetVar(wp, T("supportAllSecurity"), T(""));
	if(!strSupportAllSecurity[0]) // from basic setting page
	{
		//Set 802.1x Enable
		 strEnRadius = websGetVar(wp, T("enRadius"), T(""));
		if ( ((encrypt==0 || encrypt==1) && !gstrcmp(strEnRadius, T("ON"))) || encrypt==3) {
			intVal = 1;
			enableRS = 1;
		}
		else {intVal = 0;}

		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal); break;
			case 1: apmib_set( MIB_INIC_ENABLE_1X_1, (void *)&intVal); break;
			case 2: apmib_set( MIB_INIC_ENABLE_1X_2, (void *)&intVal); break;
			case 3: apmib_set( MIB_INIC_ENABLE_1X_3, (void *)&intVal); break;
			case 4: apmib_set( MIB_INIC_ENABLE_1X_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal) == 0) {
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

		if ( apmib_set(MIB_INIC_1X_MODE, (void *)&intVal) == 0) {
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
		if ( apmib_set( MIB_INIC_ENABLE_SUPP_NONWPA, (void *)&intVal) == 0) {
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
			if ( apmib_set( MIB_INIC_SUPP_NONWPA, (void *)&suppNonWPA) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt3))</script>"));
				goto setErr_basic;
			}
		}
	}
	
	if(!strSupportAllSecurity[0]) // from basic setting page
	{
		if (encrypt==2 || encrypt==3) {
			if (encrypt==2) {
				intVal = WPA_AUTH_PSK; //2
				getPSK = 1;
			}
			else if (encrypt==3) {
				intVal = 1;
				enableRS = 1;
			}

			if(ssidIndex==0){
				if ( apmib_set(MIB_INIC_WPA_AUTH, (void *)&intVal) == 0) {
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
				#ifdef _INIC_MSSID_
				printf("ssidIndex:%d intTkip:%d intAes:%d\n",ssidIndex, intTkip ,intAes);
				switch(ssidIndex)
				{
					case 0: apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes); break;
					case 1: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_1, (void *)&intAes); break;
					case 2: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_2, (void *)&intAes); break;
					case 3: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_3, (void *)&intAes); break;
					case 4: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_4, (void *)&intAes); break;
					default: break; 
				}
				#else
				if ( apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				if ( apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				#endif
				#if 0//defined(_Edimax_) || defined(_Customer_)
				//For Wlan wds
				if ( apmib_set(MIB_INIC_WDS_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_encrypt;
				}
				if ( apmib_set(MIB_INIC_WDS_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
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
		sprintf(tmpBuf, "echo '' > /tmp/ConnectTestKey5g", wepKey);
		system(tmpBuf);
		printf("%s\n",tmpBuf);
	}
	#endif
	#endif

	//Support All Security
	if(strSupportAllSecurity[0])  // from site curvey page
	{
		if (strEncrypt[0]=='0')	 //Disable
			encrypt=0;			
		else if (strEncrypt[0]=='1') //WEP
			encrypt=1; 
		else if (strEncrypt[0]=='2' || strEncrypt[0]=='3'){ //WPA
			getPSK = 1;
			strWpaCipher = websGetVar(wp, T("wpaCipher"), T(""));

			if (strWpaCipher[0]) {
				intValWPA = strWpaCipher[0] - '0';
				if (intValWPA==1)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intValWPA==2) intAes=WPA_CIPHER_AES; else intAes=0;

				apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);
				
				if(force_enable2g5g_ap){
				  apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip);
				  apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);
				}
			}
			if (strEncrypt[0]=='2') encrypt=2;
			else 	encrypt=4;
		}
		/*else if (strEncrypt[0]=='3'){ //WPA2
			getPSK = 1;
			strWpa2Cipher = websGetVar(wp, T("wpa2Cipher"), T(""));
			if (strWpa2Cipher[0]) 
			{
				//intValWPA=0;
				intValWPA = strWpaCipher[0] - '0';
				if (intValWPA==1)	intTkip=WPA_CIPHER_TKIP; else intTkip=0;
				if (intValWPA==2) intAes=WPA_CIPHER_AES; else intAes=0;

				apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);
			}
			encrypt=4;
		}*/
		else if (strEncrypt[0]=='4') //WPA RADIUS
			encrypt=2;	
		else if (strEncrypt[0]=='5') //WPA2 RADIUS
			encrypt=4;
	}
	else{
		if (strEncrypt[0]=='0')	encrypt=0;
		else if (strEncrypt[0]=='1') encrypt=1;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='1')) encrypt=2;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='2')) encrypt=4;
		else if (((strEncrypt[0]=='2')||(strEncrypt[0]=='3')) && (strWpaCipher[0]=='3')) encrypt=6;
	}	
	

	#ifdef _INIC_MSSID_
	switch(ssidIndex){
		case 0: apmib_set(MIB_INIC_ENCRYPT, (void *)&encrypt);break;
		case 1: apmib_set(MIB_INIC_ENCRYPT_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_INIC_ENCRYPT_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_INIC_ENCRYPT_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_INIC_ENCRYPT_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_INIC_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
		goto setErr_basic;
	}
	#endif
	
	if(force_enable2g5g_ap){
	  if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
  		DEBUGP(tmpBuf, T("set MIB_WLAN_ENCRYPT fail!"));
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WEP, (void *)&wep); break;
			case 1: apmib_set(MIB_INIC_WEP_1, (void *)&wep); break;
			case 2: apmib_set(MIB_INIC_WEP_2, (void *)&wep); break;
			case 3: apmib_set(MIB_INIC_WEP_3, (void *)&wep); break;
			case 4: apmib_set(MIB_INIC_WEP_4, (void *)&wep); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt5))</script>"));
			goto setErr_basic;
		}
		#endif
		if(force_enable2g5g_ap){
		  if ( apmib_set( MIB_WLAN_WEP, (void *)&wep) == 0) {
  			DEBUGP(tmpBuf, T("set MIB_WLAN_WEP fail"));
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

		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i); break;
			case 1: apmib_set( MIB_INIC_WEP_KEY_TYPE_1, (void *)&i); break;
			case 2: apmib_set( MIB_INIC_WEP_KEY_TYPE_2, (void *)&i); break;
			case 3: apmib_set( MIB_INIC_WEP_KEY_TYPE_3, (void *)&i); break;
			case 4: apmib_set( MIB_INIC_WEP_KEY_TYPE_4, (void *)&i); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt7))</script>"));
			goto setErr_basic;
		}
		#endif
		
		if(force_enable2g5g_ap){
		  if ( apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&i) == 0) {
			DEBUGP(tmpBuf, T("set MIB_WLAN_WEP_KEY_TYPE fail!"));
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

			#ifdef _INIC_MSSID_
			switch(ssidIndex)
			{
				case 0: apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i); break;
				case 1: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_1, (void *)&i); break;
				case 2: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_2, (void *)&i); break;
				case 3: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_3, (void *)&i); break;
				case 4: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_4, (void *)&i); break;
				default: break; 
			}
			#else
			if ( !apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt9))</script>"));
				goto setErr_basic;
			}
			#endif
			if(force_enable2g5g_ap){
			    if ( !apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&i ) ) {
		 		DEBUGP(tmpBuf, T("SET MIB_WLAN_WEP_DEFAULT_KEY FAIL"));
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
				strcpy(ConnectTestKey5g, wepKey);
				wep_used=1;
				//CharFilter0(tmpWepKey);
				#ifdef __TARGET_BOARD__
				if(ssidIndex==0){
					for(i=0; i<strlen(tmpWepKey); i++) //EDX, Ken 2015_09_16 add, Avoid tmpWepKey is "wepKey;systemcode".
					{
						sprintf(buildWepKey, "%s%X", buildWepKey, tmpWepKey[i]);
					}
					sprintf(tmpBuf, "echo \"%s\" > /tmp/ConnectTestKey5g",(void *)buildWepKey);//Robert 2014_01_06
					system(tmpBuf);
					printf("%s\n",tmpBuf);
				}	
				#endif
				#endif

				printf("ssidIndex=%d, 5g WEP len(1:64 bit, 2:128 bit)=%d Format(1:ascii, 2:hex)=%s\n",ssidIndex,wep,strFormat);
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

				#ifdef _INIC_MSSID_
				switch(ssidIndex)
				{
					case 0: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
						else ret=0;
						break;
					case 1: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_1, (void *)key);
						else ret=0;
						break;
					case 2: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_2, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_2, (void *)key);
						else ret=0;
						break;
					case 3: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_3, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_3, (void *)key);
						else ret=0;
						break;
					case 4: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_4, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_4, (void *)key);
						else ret=0;
						break;
					default: break; 
				}
				#else
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
				else
					ret=0;
				#endif
					
				if(force_enable2g5g_ap){
				    if (wep == WEP64)
					  ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				    else if (wep == WEP128)
					  ret=apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
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
		if(strSupportAllSecurity[0]){			
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf); break;
			case 1: apmib_get(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_1, (void *)tmpBuf); break;
			case 2: apmib_get(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_2, (void *)tmpBuf); break;
			case 3: apmib_get(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_3, (void *)tmpBuf); break;
			case 4: apmib_get(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_4, (void *)tmpBuf); break;
			default: break; 
		}
		#else
		apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_INIC_WPA_PSK, (void *)tmpBuf);
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
		else
		{
			strVal = websGetVar(wp, T("pskValue"), T(""));
		}
		
		//printf("pskValue:%s\n",strVal);
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
			strcpy(ConnectTestKey5g, strVal);
			//CharFilter0(tmpStrVal);
			if(ssidIndex==0){
				#ifdef __TARGET_BOARD__
				for(i=0; i<strlen(tmpStrVal); i++) //EDX, Ken 2015_09_07 add, Avoid tmpWepKey is "wepKey;systemcode".
				{
					sprintf(buildstrVal, "%s%X", buildstrVal, tmpStrVal[i]);
				}
				sprintf(tmpBuf, "echo \"%s\" > /tmp/ConnectTestKey5g",(void *)buildstrVal);//Robert 2014_01_06
				system(tmpBuf);
				printf("%s\n",tmpBuf);
				#endif
			}
		}
		#endif

		//printf("ssidIndex=%d, 5g pskformate=%d\n",ssidIndex,intVal);
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal); break;
			case 1: apmib_set(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&intVal); break;
			case 2: apmib_set(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&intVal); break;
			case 3: apmib_set(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&intVal); break;
			case 4: apmib_set(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_basic;
		}
		#endif
		
		
		if(force_enable2g5g_ap){
		  if ( apmib_set(MIB_WLAN_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("set MIB_WLAN_WPA_PSK_FORMAT fail!"));
			goto setErr_basic;
		  }
		}
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan WDS
		if ( apmib_set(MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WPA_PSK, (void *)strVal); break;
			case 1: apmib_set(MIB_INIC_WPA_PSK_1, (void *)strVal); break;
			case 2: apmib_set(MIB_INIC_WPA_PSK_2, (void *)strVal); break;
			case 3: apmib_set(MIB_INIC_WPA_PSK_3, (void *)strVal); break;
			case 4: apmib_set(MIB_INIC_WPA_PSK_4, (void *)strVal); break;
			default: break; 
		}
		#else
		if ( !apmib_set(MIB_INIC_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_basic;
		}
		#endif
		
		if(force_enable2g5g_ap){
		   if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("set MIB_WLAN_WPA_PSK fail!"));
			goto setErr_basic;
		    } 
		}
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan wds
		if ( !apmib_set(MIB_INIC_WDS_WPA_PSK, (void *)strVal)) {
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
	
			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_PORT, (void *)&intVal); break;
				case 1: apmib_set(MIB_INIC_RS_PORT_1, (void *)&intVal); break;
				case 2: apmib_set(MIB_INIC_RS_PORT_2, (void *)&intVal); break;
				case 3: apmib_set(MIB_INIC_RS_PORT_3, (void *)&intVal); break;
				case 4: apmib_set(MIB_INIC_RS_PORT_4, (void *)&intVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_PORT, (void *)&intVal)) {
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

			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_IP, (void *)&inIp); break;
				case 1: apmib_set(MIB_INIC_RS_IP_1, (void *)&inIp); break;
				case 2: apmib_set(MIB_INIC_RS_IP_2, (void *)&inIp); break;
				case 3: apmib_set(MIB_INIC_RS_IP_3, (void *)&inIp); break;
				case 4: apmib_set(MIB_INIC_RS_IP_4, (void *)&inIp); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_IP, (void *)&inIp)) {
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

			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_PASSWORD, (void *)strVal); break;
				case 1: apmib_set(MIB_INIC_RS_PASSWORD_1, (void *)strVal); break;
				case 2: apmib_set(MIB_INIC_RS_PASSWORD_2, (void *)strVal); break;
				case 3: apmib_set(MIB_INIC_RS_PASSWORD_3, (void *)strVal); break;
				case 4: apmib_set(MIB_INIC_RS_PASSWORD_4, (void *)strVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_PASSWORD, (void *)strVal)) {
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
			if ( apmib_set(MIB_INIC_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_basic;
			}
		}
	}

//*********************************** WPS_CONFIG setting ***********************************


	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
	//printf("--> 5g wpsStatus:%s\n", strWpsStatus);
	if (strWpsStatus[0] && !gstrcmp(strWpsStatus, T("1"))){
		val = 1;
		if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_INIC_WPS_CONFIG_STATUS, (void *)&val) == 0)
		{
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_basic;
		}
		val = 0;
		if ( apmib_set(MIB_INIC_WPS_DISPLAY_KEY, (void *)&val) == 0) {
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


	if (!gstrcmp(submitUrl, T("/wiz_rep5g.asp"))){	//wiz_rep5g.asp??
		
		strSSID = websGetVar(wp, T("ssid"), T(""));		
		if(strSSID[0])
			system("echo \"1\" > /tmp/wifi_changed");
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh");  // for iQsetup scan ssid
		system("/bin/up_wlan.sh &");
		#endif
	}


#ifdef _AUTO_CHAN_SELECT_
	char_t *strCONNTEST;
	strCONNTEST = websGetVar(wp, T("doConnectTest"), T(""));
	if ( strCONNTEST[0] && (!gstrcmp(strCONNTEST, T("1"))) ) { //if CONN_TEST_TRUE=0 means dont do connect test
	
		strVal = websGetVar(wp, T("rootAPmac"), T(""));		
		if(strVal[0]){
			for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid strVal is "rootAPmac;systemcode".
			{
				if (strVal[i]==59)
				{
					strVal[i]='\0';
					break;
				}
			}
			sprintf(tmpBuf, "echo '%s' > /tmp/apclii0_rootMAC", strVal);
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
				REPLACE_MSG(submitUrl);
			exit(0);
		}
		CONNECT_TEST(5);	//2=wlan1 test 5=wlan0 test 7=wlan0+wlan1 test
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
				OK_MSG3(tmpBuf, newurl);
			}
			else
			{			
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				OK_MSG3(tmpBufapply ,submitUrl);
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
}  //formiNICbasic end


/////////
void formiNICEnableSwitch(webs_t wp, char_t *path, char_t *query)
{
  char_t *submitUrl, *strWirelessEnable, *strWirelessDisable, *strnextEnable;
	int val;
	char tmpBuf[100];
	FILE *fp;

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
//EDX yihong fixed, Guest SSID can not work
		if (!gstrcmp(strWirelessDisable, T("yes")))
		{
			val = (ssidIndex != 0) ? 0 : 1;
			//val = (ssidIndex != 0) ? 0 : 2;
		}
		else if (!gstrcmp(strWirelessDisable, T("no")))
		{
			val = (ssidIndex != 0) ? 1 : 0;
			//val = (ssidIndex != 0) ? 2 : 0;
		}
		else {
			DEBUGP(tmpBuf, T("apmib set WLAN_DISABLED Error!"));
			goto setErr_advance;
		}
		printf("5g ssidIndex=%d, val=%d\n", ssidIndex, val);
		#ifdef _MSSID_
		if(ssidIndex == 0)
			apmib_set(MIB_INIC_DISABLED, (void *)&val);
		else{
			if(val) val += ssidIndex;
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_DISABLED, (void *)&val); break;
				case 1: apmib_set(MIB_INIC_SSID_MIRROR_1, (void *)&val); break;
				case 2: apmib_set(MIB_INIC_SSID_MIRROR_2, (void *)&val); break;
				case 3: apmib_set(MIB_INIC_SSID_MIRROR_3, (void *)&val); break;
				case 4: apmib_set(MIB_INIC_SSID_MIRROR_4, (void *)&val); break;
				default: break; 
			}
		}
		#else
		if ( apmib_set(MIB_INIC_DISABLED, (void *)&val) == 0) {
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
void formiNICbasicREP(webs_t wp, char_t *path, char_t *query)
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
	printf("---------------------formiNICbasicREP -------------------\n");

	//------------------ set AP mode  ------------------//
	strMode = websGetVar(wp, T("apMode"), T(""));
	if (strMode[0]) {	
		errno=0;
		mode = strtol( strMode, (char **)NULL, 10);

		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup2))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set( MIB_INIC_AP_MODE, (void *)&mode) == 0) {
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
		if ( apmib_set( MIB_INIC_BAND, (void *)&val) == 0) {
   			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup5))</script>"));
			goto setErr_basic;
		}
	}


#ifdef _WIFI_11N_STANDARD_
	if (val == 2) {
		apmib_get(MIB_INIC_ENCRYPT, (void *)&intVal);
		if (intVal != ENCRYPT_WPA2) {
			intVal = ENCRYPT_DISABLED;
			apmib_set(MIB_INIC_ENCRYPT, (void *)&intVal);
			apmib_set(MIB_INIC_SECURITY_MODE, (void *)&intVal);
		}
		else {
			intVal = 0;
			apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intVal);
			intVal = 2;
			apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intVal);
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

		if ( apmib_set( MIB_INIC_CHAN_NUM, (void *)&chan) == 0) {
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

                if ( apmib_set(MIB_INIC_HIDDEN_SSID, (void *)&val) == 0) {
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
			if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
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
				if ( apmib_set(MIB_INIC_REPEATER_SSID, (void *)strSSID) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup7))</script>"));
						goto setErr_basic;
				}
			}
		}
		else {
			val = 0;
			if ( apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspformStatsErrRep))</script>"));
			goto setErr_basic;
			}
		}

		if (mode == 2 || mode == 3 || mode == 4 || mode == 6 || wisp == 3) {
			apmib_get(MIB_INIC_ENCRYPT, (void *)&tmp);
			if (tmp == 6) {
				val = 4;
				apmib_set(MIB_INIC_ENCRYPT, (void *)&val);
				val = 0;
				apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&val);
			}
		}

		//AP client mode type
		#ifndef _RESERVE_ENCRYPTION_SETTING_
		if (mode == 1 || mode ==2) {
			if (strMode[0]=='1')
				net = ADHOC;
			if (strMode[0]=='2')
				net = INFRASTRUCTURE;
			if ( apmib_set(MIB_INIC_NETWORK_TYPE, (void *)&net) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup8))</script>"));
				goto setErr_basic;
			}
			//------------------------------------------------------------ Added by Tommy
			if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup9))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_ENABLE_1X, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup10))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_ENCRYPT, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup11))</script>"));
				goto setErr_basic;
			}

			if (apmib_set( MIB_INIC_AUTH_TYPE, (void *)&intVal) == 0) {
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_SSID, (void *)strSSID); break;
			case 1: apmib_set(MIB_INIC_SSID_1, (void *)strSSID); break;
			case 2: apmib_set(MIB_INIC_SSID_2, (void *)strSSID); break;
			case 3: apmib_set(MIB_INIC_SSID_3, (void *)strSSID); break;
			case 4: apmib_set(MIB_INIC_SSID_4, (void *)strSSID); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_INIC_SSID, (void *)strSSID) == 0) {
   	 			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlanSetup15))</script>"));
				goto setErr_basic;
		}
		#endif
	}
	if ( strSSID[0] && repUseCrossBand[0]) {
		if ( apmib_set(MIB_WLAN_SSID, (void *)strSSID) == 0) {
 			DEBUGP(tmpBuf, T("MIB_WLAN_SSID ERROR!"));
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC1, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC2, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC3, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC4, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC5, (void *)tmpBuf)) {
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
			if ( !apmib_set(MIB_INIC_WL_LINKMAC6, (void *)tmpBuf)) {
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
	#ifdef _INIC_MSSID_
	switch(ssidIndex)
	{
		case 0: apmib_set(MIB_INIC_SECURITY_MODE, (void *)&encrypt); break;
		case 1: apmib_set(MIB_INIC_SECURITY_MODE_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_INIC_SECURITY_MODE_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_INIC_SECURITY_MODE_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_INIC_SECURITY_MODE_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_INIC_SECURITY_MODE, (void *)&encrypt) == 0) {
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

		if ( apmib_set(MIB_INIC_1X_MODE, (void *)&intVal) == 0) {
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
				if ( apmib_set(MIB_INIC_WPA_AUTH, (void *)&intVal) == 0) {
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

				#ifdef _INIC_MSSID_
				printf("ssidIndex:%d intTkip:%d intAes:%d\n",ssidIndex, intTkip ,intAes);
				switch(ssidIndex)
				{
					case 0: apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes); break;
					case 1: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_1, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_1, (void *)&intAes); break;
					case 2: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_2, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_2, (void *)&intAes); break;
					case 3: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_3, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_3, (void *)&intAes); break;
					case 4: apmib_set(MIB_INIC_WPA_CIPHER_SUITE_4, (void *)&intTkip); apmib_set(MIB_INIC_WPA2_CIPHER_SUITE_4, (void *)&intAes); break;
					default: break; 
				}
				#else
				if ( apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStaEncrypt21))</script>"));
					goto setErr_basic;
				}
				if ( apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes) == 0) {
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
		sprintf(tmpBuf, "echo '' > /tmp/ConnectTestKey5g", wepKey);
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

				apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip);
				apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);
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
	

	#ifdef _INIC_MSSID_
	switch(ssidIndex){
		case 0: apmib_set(MIB_INIC_ENCRYPT, (void *)&encrypt);break;
		case 1: apmib_set(MIB_INIC_ENCRYPT_1, (void *)&encrypt); break;
		case 2: apmib_set(MIB_INIC_ENCRYPT_2, (void *)&encrypt); break;
		case 3: apmib_set(MIB_INIC_ENCRYPT_3, (void *)&encrypt); break;
		case 4: apmib_set(MIB_INIC_ENCRYPT_4, (void *)&encrypt); break;
		default: break; 
	}
	#else
	if (apmib_set( MIB_INIC_ENCRYPT, (void *)&encrypt) == 0) {
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WEP, (void *)&wep); break;
			case 1: apmib_set(MIB_INIC_WEP_1, (void *)&wep); break;
			case 2: apmib_set(MIB_INIC_WEP_2, (void *)&wep); break;
			case 3: apmib_set(MIB_INIC_WEP_3, (void *)&wep); break;
			case 4: apmib_set(MIB_INIC_WEP_4, (void *)&wep); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_WEP, (void *)&wep) == 0) {
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

		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i); break;
			case 1: apmib_set( MIB_INIC_WEP_KEY_TYPE_1, (void *)&i); break;
			case 2: apmib_set( MIB_INIC_WEP_KEY_TYPE_2, (void *)&i); break;
			case 3: apmib_set( MIB_INIC_WEP_KEY_TYPE_3, (void *)&i); break;
			case 4: apmib_set( MIB_INIC_WEP_KEY_TYPE_4, (void *)&i); break;
			default: break; 
		}
		#else
		if ( apmib_set( MIB_INIC_WEP_KEY_TYPE, (void *)&i) == 0) {
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

			#ifdef _INIC_MSSID_
			switch(ssidIndex)
			{
				case 0: apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i); break;
				case 1: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_1, (void *)&i); break;
				case 2: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_2, (void *)&i); break;
				case 3: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_3, (void *)&i); break;
				case 4: apmib_set( MIB_INIC_WEP_DEFAULT_KEY_4, (void *)&i); break;
				default: break; 
			}
			#else
			if ( !apmib_set( MIB_INIC_WEP_DEFAULT_KEY, (void *)&i ) ) {
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
				strcpy(ConnectTestKey5g, wepKey);
				wep_used=1;
				//CharFilter0(tmpWepKey);
				#ifdef __TARGET_BOARD__
				if(ssidIndex==0){
					for(i=0; i<strlen(tmpWepKey); i++) //EDX, Ken 2015_09_16 add, Avoid tmpWepKey is "wepKey;systemcode".
					{
						sprintf(buildWepKey, "%s%X", buildWepKey, tmpWepKey[i]);
					}
					sprintf(tmpBuf, "echo %s > /tmp/ConnectTestKey5g",(void *)buildWepKey);//Robert 2014_01_06
					system(tmpBuf);
					printf("%s\n",tmpBuf);
				}	
				#endif
				#endif

				printf("ssidIndex=%d, 5g WEP len(1:64 bit, 2:128 bit)=%d Format(1:ascii, 2:hex)=%s\n",ssidIndex,wep,strFormat);
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

				#ifdef _INIC_MSSID_
				switch(ssidIndex)
				{
					case 0: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
						else ret=0;
						break;
					case 1: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_1, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_1, (void *)key);
						else ret=0;
						break;
					case 2: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_2, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_2, (void *)key);
						else ret=0;
						break;
					case 3: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_3, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_3, (void *)key);
						else ret=0;
						break;
					case 4: 
						if (wep == WEP64) apmib_set(MIB_INIC_WEP64_KEY1_4, (void *)key);
						else if (wep == WEP128) ret=apmib_set(MIB_INIC_WEP128_KEY1_4, (void *)key);
						else ret=0;
						break;
					default: break; 
				}
				#else
				if (wep == WEP64)
					ret=apmib_set(MIB_INIC_WEP64_KEY1, (void *)key);
				else if (wep == WEP128)
					ret=apmib_set(MIB_INIC_WEP128_KEY1, (void *)key);
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK, (void *)tmpBuf); break;
			case 1: apmib_get(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_1, (void *)tmpBuf); break;
			case 2: apmib_get(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_2, (void *)tmpBuf); break;
			case 3: apmib_get(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_3, (void *)tmpBuf); break;
			case 4: apmib_get(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&oldFormat); apmib_get(MIB_WLAN_WPA_PSK_4, (void *)tmpBuf); break;
			default: break; 
		}
		#else
		apmib_get(MIB_INIC_WPA_PSK_FORMAT, (void *)&oldFormat);
		apmib_get(MIB_INIC_WPA_PSK, (void *)tmpBuf);
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
		
printf("===========strVal=%s\n",strVal);
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
			strcpy(ConnectTestKey5g, strVal);
			CharFilter0(tmpStrVal);
			if(ssidIndex==0){
				#ifdef __TARGET_BOARD__
				for(i=0; i<strlen(tmpStrVal); i++) //EDX, Ken 2015_09_07 add, Avoid tmpWepKey is "wepKey;systemcode".
				{
					sprintf(buildstrVal, "%s%X", buildstrVal, tmpStrVal[i]);
				}
				sprintf(tmpBuf, "echo \"%s\" > /tmp/ConnectTestKey5g",(void *)buildstrVal);//Robert 2014_01_06
				system(tmpBuf);
				printf("%s\n",tmpBuf);
				#endif
			}
		}
		#endif

		//printf("ssidIndex=%d, 5g pskformate=%d\n",ssidIndex,intVal);
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal); break;
			case 1: apmib_set(MIB_INIC_WPA_PSK_FORMAT_1, (void *)&intVal); break;
			case 2: apmib_set(MIB_INIC_WPA_PSK_FORMAT_2, (void *)&intVal); break;
			case 3: apmib_set(MIB_INIC_WPA_PSK_FORMAT_3, (void *)&intVal); break;
			case 4: apmib_set(MIB_INIC_WPA_PSK_FORMAT_4, (void *)&intVal); break;
			default: break; 
		}
		#else
		if ( apmib_set(MIB_INIC_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWdsEncrypt))</script>"));
			goto setErr_basic;
		}
		#endif
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan WDS
		if ( apmib_set(MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&intVal) == 0) {
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
		#ifdef _INIC_MSSID_
		switch(ssidIndex)
		{
			case 0: apmib_set(MIB_INIC_WPA_PSK, (void *)strVal); break;
			case 1: apmib_set(MIB_INIC_WPA_PSK_1, (void *)strVal); break;
			case 2: apmib_set(MIB_INIC_WPA_PSK_2, (void *)strVal); break;
			case 3: apmib_set(MIB_INIC_WPA_PSK_3, (void *)strVal); break;
			case 4: apmib_set(MIB_INIC_WPA_PSK_4, (void *)strVal); break;
			default: break; 
		}
		#else
		if ( !apmib_set(MIB_INIC_WPA_PSK, (void *)strVal)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt6))</script>"));
			goto setErr_basic;
		}
		#endif
		#if 0//defined(_Edimax_) || defined(_Customer_)
		//For Wlan wds
		if ( !apmib_set(MIB_INIC_WDS_WPA_PSK, (void *)strVal)) {
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
	
			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_PORT, (void *)&intVal); break;
				case 1: apmib_set(MIB_INIC_RS_PORT_1, (void *)&intVal); break;
				case 2: apmib_set(MIB_INIC_RS_PORT_2, (void *)&intVal); break;
				case 3: apmib_set(MIB_INIC_RS_PORT_3, (void *)&intVal); break;
				case 4: apmib_set(MIB_INIC_RS_PORT_4, (void *)&intVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_PORT, (void *)&intVal)) {
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

			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_IP, (void *)&inIp); break;
				case 1: apmib_set(MIB_INIC_RS_IP_1, (void *)&inIp); break;
				case 2: apmib_set(MIB_INIC_RS_IP_2, (void *)&inIp); break;
				case 3: apmib_set(MIB_INIC_RS_IP_3, (void *)&inIp); break;
				case 4: apmib_set(MIB_INIC_RS_IP_4, (void *)&inIp); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_IP, (void *)&inIp)) {
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

			#ifdef _MULTIPLE_INIC_RS_SETTING_
			switch(ssidIndex)
			{
				case 0: apmib_set(MIB_INIC_RS_PASSWORD, (void *)strVal); break;
				case 1: apmib_set(MIB_INIC_RS_PASSWORD_1, (void *)strVal); break;
				case 2: apmib_set(MIB_INIC_RS_PASSWORD_2, (void *)strVal); break;
				case 3: apmib_set(MIB_INIC_RS_PASSWORD_3, (void *)strVal); break;
				case 4: apmib_set(MIB_INIC_RS_PASSWORD_4, (void *)strVal); break;
				default: break; 
			}
			#else
			if ( !apmib_set(MIB_INIC_RS_PASSWORD, (void *)strVal)) {
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
			if ( apmib_set(MIB_INIC_WEP, (void *)&intVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlEncrypt16))</script>"));
				goto setErr_basic;
			}
		}
	}

//*********************************** WPS_CONFIG setting ***********************************


	strWpsStatus = websGetVar(wp, T("wpsStatus"), T(""));   // hidden page
	//printf("--> 5g wpsStatus:%s\n", strWpsStatus);
	if (!gstrcmp(strWpsStatus, T("1"))){
		val = 1;
		if ( apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
			goto setErr_basic;
		}
		if ( apmib_set(MIB_INIC_WPS_CONFIG_STATUS, (void *)&val) == 0)
		{
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspSetFirewallFail))</script>"));
		goto setErr_basic;
		}
		val = 0;
		if ( apmib_set(MIB_INIC_WPS_DISPLAY_KEY, (void *)&val) == 0) {
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


	if (!gstrcmp(submitUrl, T("/wiz_rep5g.asp"))){	//wiz_rep5g.asp??
		
		strSSID = websGetVar(wp, T("ssid"), T(""));		
		if(strSSID[0])
			system("echo \"1\" > /tmp/wifi_changed");
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh");  // for iQsetup scan ssid
		system("/bin/up_wlan.sh &");
		#endif
	}


#ifdef _AUTO_CHAN_SELECT_
	char_t *strCONNTEST;
	strCONNTEST = websGetVar(wp, T("doConnectTest"), T(""));
	if ( strCONNTEST[0] && (!gstrcmp(strCONNTEST, T("1"))) ) { //if CONN_TEST_TRUE=0 means dont do connect test

		strVal = websGetVar(wp, T("rootAPmac"), T(""));		
		if(strVal[0]){
			for(i=0; i<strlen(strVal); i++)//EDX, Ken 2015_09_16 add, Avoid strVal is "rootAPmac;systemcode".
			{
				if (strVal[i]==59)
				{
					strVal[i]='\0';
					break;
				}
			}
			sprintf(tmpBuf, "echo '%s' > /tmp/apclii0_rootMAC", strVal);
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
		CONNECT_TEST(5);	//2=wlan1 test 5=wlan0 test 7=wlan0+wlan1 test
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
				OK_MSG3(tmpBuf, newurl);
			}
			else
			{			
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				OK_MSG3(tmpBufapply ,submitUrl);
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
}  //formiNICbasicREP end
#endif


#ifdef _Edimax_ //EDX, Robert Add 2015_05_29
/////////////////////////////////////////////////////////////////////////////
//For wiz_5in1 wisp mode web ui sitesurvey
int wispSiteSurveyTbl5G(int eid, webs_t wp, int argc, char_t **argv)
{

	int nBytesSent=0, i, wait_time=0 ,j,z, kakaso, apCount, len, ap_mode;
	char tmpBuf[150], ssidbuf[40], tmp1Buf[10], data[16384],tmpbuffer[100],orig_buffer[100], buffer[100];
	BssDscr *pBss;
	bss_info bss;
	SITE_SURVEY5G SiteSurvey[100];
	char *sp, *op, asd[200], buf[256];

	if (piNICStatus==NULL)
	{
		piNICStatus = calloc(1, sizeof(SS_STATUS_T5G));
		if ( piNICStatus == NULL )
		{
			nBytesSent = websWrite(wp, T("<script>document.write(showText(aspformSaveConfigFail))</script>"));
			return nBytesSent;
		}
	}


	// issue scan request

	wait_time = 0;
	memset(&data, 0, sizeof(data));

	if (getWlSiteSurveyRequest(INIC_IF,  &data) < 0)
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

	piNICStatus->number = 0;

	if ((len = getWlSiteSurveyResult(INIC_IF, &data, sizeof(data))) < 0)
	{
		nBytesSent = websWrite(wp, "<script>document.write(showText(aspFormWlSiteSurvey3))</script>");
		free(piNICStatus);
		piNICStatus = NULL;
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
			#if 1
			char ch_width[7];
			#endif
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

			//memcpy (SiteSurvey[i].channel, psite_survey->channel, 2); 
			sprintf(SiteSurvey[i].channel, "%d", atoi(psite_survey->channel));
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
				strcpy (SiteSurvey[i].authmode, "WEP");
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
			//printf("%d, SiteSurvey[i].extch:%s\n",i ,SiteSurvey[i].extch);
			if (strstr (SiteSurvey[i].extch, "AB"))
				strcpy (SiteSurvey[i].extch, "2");
			else if (strstr (SiteSurvey[i].extch, "BE"))
				strcpy (SiteSurvey[i].extch, "1");
			else
				strcpy (SiteSurvey[i].extch, "0");

			#if 1
			memcpy (SiteSurvey[i].ch_width, psite_survey->extch, 6);
			if (strstr (SiteSurvey[i].ch_width, "8"))
				strcpy (SiteSurvey[i].ch_width, "80M");
			else if (strstr (SiteSurvey[i].ch_width, "4"))
				strcpy (SiteSurvey[i].ch_width, "40M");
			else
				strcpy (SiteSurvey[i].ch_width, "20M");
			#endif

			len -= single_length;
			sp += single_length;
			i++;
		}

		#if 1 // sort by signal
		SITE_SURVEY5G SiteSurveyBUF[100]={0};
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
#if 0
		apCount = i;
#else
		 apCount = 50;
#endif
		piNICStatus->number = i;

		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"apCount\" value=\"%d\">"),apCount);

			for(i = 0 ; i < apCount ; i++)//Channel+SSID+Bssid+WepiNICStatus+AuthMode+Signal+WiressMode+NetworkType = 4+28+20+8+10+9+7+3
			{
				piNICStatus->bssdb[i] = SiteSurvey[i];
    				sprintf(tmpBuf,"%s",piNICStatus->bssdb[i].ssid);
				char tableclass[30]=" class=\\\"table2\\\"";
#if defined(_SIGNAL_FORMULA_)
				int PowerAband = atoi(piNICStatus->bssdb[i].signal);
				int PowerUpAband = ScanListSignalScaleMapping92CD(PowerAband);
#endif
	nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
	nBytesSent += websWrite(wp, T("<tr class=\"listtableText\" align=\"center\">\n"));

	nBytesSent += websWrite(wp, T("	<td align=center width=\"10%%\" class=\"\">\n"));
	nBytesSent += websWrite(wp, T("		<input type=\"radio\" name=\"select\" value=\"sel%d\" onClick=\"enableConnect('sel%d')\">\n"),i,i);  
	nBytesSent += websWrite(wp, T("	</td>\n")); //Select

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>\n"),tmpBuf);    // SSID
	nBytesSent += websWrite(wp, T("		<input type=\"hidden\" name=\"ssid%d\" value=\"\">\n"),i); 
	nBytesSent += websWrite(wp, T("		<script>document.forms['formiNICSiteSurvey'].elements['ssid'+%d].value=\""),i);
	nBytesSent += websWrite(wp, CharFilter2(tmpBuf));
	nBytesSent += websWrite(wp, T("\" </script>\n"));
	nBytesSent += websWrite(wp, T("</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%02X:%02X:%02X:%02X:%02X:%02X</font>"),piNICStatus->bssdb[i].bssid[0],piNICStatus->bssdb[i].bssid[1],piNICStatus->bssdb[i].bssid[2],piNICStatus->bssdb[i].bssid[3],piNICStatus->bssdb[i].bssid[4],piNICStatus->bssdb[i].bssid[5]);  //MAC
	nBytesSent += websWrite(wp, T("	</td>\n"));
	
	piNICStatus->bssdb[i].wirelessmode[8]='\0';
	
	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),piNICStatus->bssdb[i].wirelessmode);  //b/g/n
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),piNICStatus->bssdb[i].channel);  //channel
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),"AP");  //type
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>"),piNICStatus->bssdb[i].authmode);  //authmode
	nBytesSent += websWrite(wp, T("	</td>\n"));

	nBytesSent += websWrite(wp, T("	<td align=\"center\">\n"));
#if defined(_SIGNAL_FORMULA_)
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%d%%</font>"),PowerUpAband);  //Signal
#else
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s%%</font>"),piNICStatus->bssdb[i].signal);  //Signal
#endif
	nBytesSent += websWrite(wp, T("	</td>\n"));
	nBytesSent += websWrite(wp, T("</tr>\n"));

	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"band%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].wirelessmode); 
#if defined(_SIGNAL_FORMULA_)
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%d\">\n"),i,PowerUpAband); 
#else
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"signal%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].signal); 
#endif
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"bssid%d\" value=\"%02X:%02X:%02X:%02X:%02X:%02X\">\n"),i,piNICStatus->bssdb[i].bssid[0],piNICStatus->bssdb[i].bssid[1],piNICStatus->bssdb[i].bssid[2],piNICStatus->bssdb[i].bssid[3],piNICStatus->bssdb[i].bssid[4],piNICStatus->bssdb[i].bssid[5]); 
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"chan%d\" value=\"%s\">\n"),i, piNICStatus->bssdb[i].channel); //Channel
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"encryption%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].authmode);  //encryption
	nBytesSent += websWrite(wp, T("	<input type=\"hidden\" name=\"ciphersutie%d\" value=\"%s\">\n"),i,piNICStatus->bssdb[i].webstatus);

	nBytesSent += websWrite(wp, T("<tr>\n"));
	nBytesSent += websWrite(wp, T("	<td style=\"display:none\" align=center class=\"table2\">\n"));
	nBytesSent += websWrite(wp, T("		<font size=\"2\">%s</font>\n"),SiteSurvey[i].extch);
	nBytesSent += websWrite(wp, T("		<input type=\"hidden\" id=\"secchan%d\" value=\"%s\">\n"),i,SiteSurvey[i].extch);
	#if 1
	nBytesSent += websWrite(wp, T("		<input type=\"hidden\" id=\"ch_width%d\" value=\"%s\">\n"),i,SiteSurvey[i].ch_width);
	#endif
	nBytesSent += websWrite(wp, T("	</td>\n"));
	nBytesSent += websWrite(wp, T("</tr>\n"));
			}
	}
	return nBytesSent;
}
#endif

