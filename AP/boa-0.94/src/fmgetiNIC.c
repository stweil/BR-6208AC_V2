/*
 *      Web server handler routines for get info and index (getinfo(), getindex())
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: fmget.c,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../webs.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/wireless.h>


#ifdef HOME_GATEWAY
char staTbl[2][7][50];
char usbTbl[3][5][50];
#endif



int getiNICInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name;
	struct in_addr	intaddr;
	struct sockaddr hwaddr;
	unsigned char *pMacAddr;
	unsigned long sec, mn, hr, day, longVal;
	unsigned char buffer[100], tmp[100];
	int intVal, flag=1;
 	struct user_net_device_stats stats;
	DHCP_T dhcp;
	bss_info bss;
	char *ptr, *ptr1;
	time_t current_time;
	struct tm * tm_time;
#ifdef HOME_GATEWAY
	OPMODE_T opmode=-1;
	char_t *iface;
#endif
   	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
   		websError(wp, 400, T("Insufficient args\n"));
   		return -1;
   	}

   	if ( !strcmp(name, T("uptime")))
	{
		FILE* fpt;
		char buffer[100]={0};
		int i;
        	if ((fpt = fopen("/proc/uptime","r"))!=NULL)
		{
	        	fgets(buffer,100, fpt);
			for( i=0; i<100; i++ )
			{
				if( buffer[i] == '.' )
					buffer[i] = 0;
				if( (buffer[i] == 0) || (buffer[i] == ' ') )
					break;
			}
			fclose(fpt);
			sec = atoi(buffer);
		}
		else
			sec = (unsigned long) time(NULL);

//		ez=sec;
		day = sec / 86400;

#ifdef _RTL8181_     // busybox 1.02 don't need this
//        day -= 10957; 		// day counted from 1970-2000
#endif
		sec %= 86400;
		hr = sec / 3600;
		sec %= 3600;
		mn = sec / 60;
		sec %= 60;
		return websWrite(wp, T("%dday:%dh:%dm:%ds"),day, hr, mn, sec);
	}
else if ( !strcmp(name, T("Repeater_Stat"))) {
#if	defined(_REPEATER_STAT_) && defined(__TARGET_BOARD__)
		system("cat /etc/repeater_status > /tmp/Repeater_Status1");
		FILE *fp;
		char c='0';
		fp=fopen("/tmp/Repeater_Status1", "r");
		c=fgetc(fp);
		fclose(fp);
		system("rm -rf /tmp/Repeater_Status1");
		return websWrite(wp, "%c",c);
#else
		return websWrite(wp, "0");
#endif
	}
#ifdef _Customer_
   	else if ( !strcmp(name, T("TimeZoneSelected"))) {
		apmib_get( MIB_TIMEZONE_SELECTED, (void *)&intVal);
		sprintf(buffer, "%d", intVal );
		return websWrite(wp, buffer);
	}
#endif
	else if ( !strcmp(name, T("fwVersion"))) {
		FILE *fp;
		if ((fp = fopen("/etc/version","r"))!=NULL) {
			fgets(buffer, 30, fp);
				if(buffer[strlen(buffer)-1]== '\n' )
					buffer[strlen(buffer)-1] ='\0';
			fclose(fp);
   			return websWrite(wp, buffer);
		}else {
   		return websWrite(wp, "final version");
		}
	}
	else if ( !strcmp(name, T("wlMacAddr"))) {
		FILE *fp;
		sprintf (buffer, "ifconfig %s | grep HWaddr > /var/wlMactmp.tmp", INIC_IF);
		system (buffer);
		system("cut /var/wlMactmp.tmp -b 39-55 > /var/wlMac.tmp");
		if ((fp = fopen("/var/wlMac.tmp","r"))!=NULL) {
			fgets(buffer, 30, fp);
			fclose(fp);
			system("rm -f /var/wlMactmp.tmp /var/wlMac.tmp");
   			return websWrite(wp, buffer);
		}else {
   			return websWrite(wp, "00:50:fc:00:50:fc");
		}
	}
//*********************************************Read e2p value****kyle*****************************
	else if ( !strcmp(name, T("readE2P"))) {

		FILE *fp;
		char buf[100];
		sprintf(buf,"");
		if ((fp = fopen("/var/rftest2","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "");
		}
	}
//*********************************************TX1 TX2 Gain*****kyle*****************************

	else if ( !strcmp(name, T("tx1Gain"))) {
		FILE *fp;
		char buf[100];

		sprintf (buffer, "/bin/rftest.sh %s BAND aband", INIC_IF);
		system (buffer);
		sprintf (buffer, "/bin/rftest.sh %s READTX1", INIC_IF);
		system (buffer);

		sprintf(buf,"");
		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
	else if ( !strcmp(name, T("tx2Gain"))) {
		FILE *fp;
		char buf[100];

		system("echo aband > /tmp/rfband");
		sprintf (buffer, "/bin/rftest.sh %s READTX2", INIC_IF);
		system (buffer);

		sprintf(buf,"");
		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer2= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
	else if ( !strcmp(name, T("tx3Gain"))) {
		FILE *fp;
		char buf[100];

		system("echo aband > /tmp/rfband");
		sprintf (buffer, "/bin/rftest.sh %s READTX3", INIC_IF);
		system (buffer);

		sprintf(buf,"");
		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer2= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
	else if ( !strcmp(name, T("e2pDE"))) {
		FILE *fp;
		char buf[100];

		sprintf (buffer, "iwpriv %s e2p DE |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", INIC_IF);
		system (buffer);

		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer2= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
	else if ( !strcmp(name, T("e2pE2"))) {
		FILE *fp;
		char buf[100];

		sprintf (buffer, "iwpriv %s e2p E2 |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", INIC_IF);
		system (buffer);

		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer2= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
	else if ( !strcmp(name, T("e2pEB"))) {
		FILE *fp;
		char buf[100];

		sprintf (buffer, "iwpriv %s e2p EA |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", INIC_IF);
		system (buffer);

		if ((fp = fopen("/var/rftest","r"))!=NULL) {
			fgets(buf, 100, fp);
			//printf("buffer2= %s",buf);
			fclose(fp);

   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "error");
		}
	}
//*********************************************LINK Status*****kyle 2007/11/29********************
	else if ( !strcmp(name, T("lanLinkStatus"))) {
			system("/bin/linkdetect.sh lan > var/linkStatus.var");
		FILE *fp;
		char buf[100];
		if ((fp = fopen("/var/linkStatus.var","r"))!=NULL) {
				while (fgets(buf, 100, fp) > 0){
					websWrite(wp, T("%s"), buf);

				}
				fclose(fp);
   			return websWrite(wp,T(""),buf);
		}else {
   			return websWrite(wp, T("%s"), "var Port0=0 \n var Port1=0 \n var Port2=0 \n Port3=0 \n Port4=0");
		}
	}

//*********************************************Net Name Scan*****kyle*****************************

	else if ( !strcmp(name, T("netNameScan"))) {
				FILE *fp;
		char buf[100];
		char command[100];
		char lanIP[30];
		int readsize=0;

		#ifdef __TARGET_BOARD__
			if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr ) )
				sprintf(lanIP, "%s", inet_ntoa(intaddr) );
			else
				sprintf(lanIP, "%s", "0.0.0.0" );

			int i=strlen(lanIP);
			while(lanIP[i]!='.') i--;
			lanIP[i]='\0';

			sprintf(command,"/bin/nbtscan -a -r %s.1-50/24 > /var/netName.var",lanIP); system(command);
			sprintf(command,"/bin/nbtscan -a -r %s.51-100/24 >> /var/netName.var",lanIP); system(command);
			sprintf(command,"/bin/nbtscan -a -r %s.101-150/24 >> /var/netName.var",lanIP); system(command);
			sprintf(command,"/bin/nbtscan -a -r %s.151-200/24 >> /var/netName.var",lanIP); system(command);
			sprintf(command,"/bin/nbtscan -a -r %s.201-254/24 >> /var/netName.var",lanIP); system(command);
		//system("/bin/nbtscan -a -r 192.168.2.0/24 > /var/netName.var");
		#else
			system("../../nbtscan-1.5.1a/nbtscan -a -r 192.168.2.0/24 > netName.var");
		#endif

		#ifdef __TARGET_BOARD__
			if ((fp = fopen("/var/netName.var","r"))!=NULL) {
		#else
			if ((fp = fopen("netName.var","r"))!=NULL) {
		#endif

				while (fgets(buf, 100, fp) > 0){
					websWrite(wp, T("%s"), buf);

				}

			//readsize=fread(buf,1,15000, fp);
			//printf("readsize=%d,buffer= %s",readsize,buf);

				fclose(fp);

   			return websWrite(wp,T(""),buf);
		}else {
   			return websWrite(wp, T("%s"), "Null");
		}
	}
//********************************For refresh***************************************
	else if ( !strcmp(name, T("netRefreshScan")))
	{
		FILE *fp;
		char buf[100];
		char buf1[100];
		char lanIP[30];
		struct in_addr intaddr;
		#ifdef __TARGET_BOARD__
			if ((fp = fopen("/var/netName.var","r"))!=NULL)
			{
		#else
			if ((fp = fopen("netName.var","r"))!=NULL)
			{
		#endif
				if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr )) sprintf(lanIP, "%s", inet_ntoa(intaddr) );
				else if ( getInAddr("eth0", IP_ADDR, (void *)&intaddr )) sprintf(lanIP, "%s", inet_ntoa(intaddr));
				else sprintf(lanIP, "0.0.0.0");


				sprintf(buf1,"new Array(\"%s\",",lanIP);
				system(lanIP);
				while(fgets(buf, 100, fp) > 0 )
				{
					if( strncmp(buf,buf1,strlen(buf1)) )
					websWrite(wp, T("%s"), buf);
				}

				fclose(fp);

   			return websWrite(wp,T(""),buf);
			}
			else
			{
   			return websWrite(wp, T(""));
			}
	}

//************************************ station of wireless *************************************

	#if defined(_DHCP_SWITCH_)    //EDX patrick add
	else if ( !strcmp(name, T("ip-rom-5g"))) {
		if ( !apmib_get( MIB_IP_ADDR_5G, (void *)&intaddr) )
			return -1;
 		return websWrite(wp, T("%s"),inet_ntoa(intaddr));
	}
 	else if ( !strcmp(name, T("mask-rom-5g"))) {
		if ( !apmib_get( MIB_SUBNET_MASK_5G,  (void *)&intaddr) )
			return -1;
 		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
 	else if ( !strcmp(name, T("gateway-rom-5g"))) {
		if ( !apmib_get( MIB_DEFAULT_GATEWAY_5G,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T("0.0.0.0"));
		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	#endif

#ifndef HOME_GATEWAY
 	else if ( !strcmp(name, T("staPsk"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_STA_WPA_PSK,  (void *)buffer) )
			return -1;
		for (i=0; i<strlen(buffer); i++)
			buffer[i]='*';
		buffer[i]='\0';
   		return websWrite(wp, buffer);
	}
   	else if ( !strcmp(name, T("identity"))) {
		if ( !apmib_get( MIB_STA_ID,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("md5Pass"))) {
		if ( !apmib_get( MIB_STA_PASS,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("eapId"))) {
		if ( !apmib_get( MIB_STA_EAP_ID,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("eapPass"))) {
		if ( !apmib_get( MIB_STA_EAP_PASS,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("cltPass"))) {
		if ( !apmib_get( MIB_STA_CLIENT_PASS,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("serPass"))) {
		if ( !apmib_get( MIB_STA_SERVER_PASS,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("staProto"))) {
		if ( !apmib_get( MIB_STA_PROTOCOL,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("staRts"))) {
		if ( !apmib_get( MIB_STA_RTS, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("staFrag"))) {
		if ( !apmib_get( MIB_STA_FRAGMENT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif
//************************************ Wireless basic Setting *************************************
   	else if ( !strcmp(name, T("ssid"))) {

		if ( !apmib_get( MIB_INIC_SSID,  (void *)buffer) )
			return -1;
			CharFilter6(buffer);
		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("Assidtbl"))) {
		apmib_get(MIB_INIC_SSID,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("repAssidtbl"))) {
		apmib_get(MIB_INIC_REPEATER_SSID,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("vlanidAll")) ) {
		int val;
		#if defined(_INIC_ENVLAN_) && defined(_INIC_MSSID_)
		apmib_get( MIB_INIC_VLANID_1, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_VLANID_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		websWrite(wp, T("\"%s\""), "");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("txrateAll")) ) {
		int val;
		#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_FIX_RATE, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_FIX_RATE_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_INIC_FIX_RATE, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("WmmCapableAll")) ) {
		int val;
		#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_WMM, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_WMM_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WMM_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_INIC_WMM, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("hiddenSSIDAll")) ) {
		int val;
		#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_HIDDEN_SSID, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_HIDDEN_SSID_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_INIC_HIDDEN_SSID, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("ssidMirrorAll")) ) {
		int val;
		#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_SSID_MIRROR_1, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_SSID_MIRROR_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		websWrite(wp, T("\"%s\""), "");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("gusetssidenable")) ) {
		if ( !apmib_get( MIB_INIC_SSID_MIRROR_1, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal);
		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("noForwardAll")) ) {
		int val;
		#ifdef _INICMSSID_NOFORWARD_
		apmib_get( MIB_INIC_NOFORWARD, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_NOFORWARD7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_INIC_NOFORWARD, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("defaultKeyIdAll")) ) {
		int val;
#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
#else
		apmib_get( MIB_INIC_WEP_DEFAULT_KEY, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

 	else if ( !strcmp(name, T("pskValueAll"))) {
#ifdef _INIC_MSSID_
		apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_1,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_2,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_3,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_4,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_5,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_6,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_WPA_PSK_7,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\")"), buffer);
#else
		apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
#endif
   		return 0;
	}
	else if ( !strcmp(name, T("pskValueArr"))) {
		apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer);	
		//CharFilter2(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
   	return 0;
	}
	else if ( !strcmp(name, T("pskValue1Arr"))) {
		apmib_get(MIB_INIC_WPA_PSK_1,  (void *)buffer);	
		//CharFilter2(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
   	return 0;
	}
	else if ( !strcmp(name, T("pskFormatAll"))) {
		int val;
#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_WPA_PSK_FORMAT, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_1, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_2, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_3, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_4, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_5, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_6, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WPA_PSK_FORMAT_7, (void *)&val);	websWrite(wp, T("\"%d\")"), val);
#else
		apmib_get( MIB_INIC_WPA_PSK_FORMAT, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}
	else if ( !strcmp (name, T("wep64keyall"))){
		unsigned char key[32];
		int key_type;

		if ( !apmib_get(MIB_INIC_WEP64_KEY1, (void *)key) ) 
			return -1;
		if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE, (void *)&key_type)) 
			return -1;
		if (key_type == 0) sprintf (buffer, "%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4]);
		else sprintf (buffer, "%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4]);

		#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%s\","), CharFilter2(buffer));

		if ( !apmib_get(MIB_INIC_WEP64_KEY1_1, (void *)key) ) 
			return -1;
		if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_1, (void *)&key_type)) 
			return -1;
		if (key_type == 0) sprintf (buffer, "%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4]);
		else sprintf (buffer, "%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4]);
		websWrite(wp, T("\"%s\","), CharFilter2(buffer));

		if ( !apmib_get(MIB_INIC_WEP64_KEY1_2, (void *)key) ) return -1;
		if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_2, (void *)&key_type)) return -1;
		if (key_type == 0) sprintf (buffer, "%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4]);
		else sprintf (buffer, "%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4]);
		websWrite(wp, T("\"%s\","), CharFilter2(buffer));
		
		if ( !apmib_get(MIB_INIC_WEP64_KEY1_3, (void *)key) ) return -1;
		if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_3, (void *)&key_type)) return -1;
		if (key_type == 0) sprintf (buffer, "%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4]);
		else sprintf (buffer, "%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4]);
		websWrite(wp, T("\"%s\","), CharFilter2(buffer));
		
		if ( !apmib_get(MIB_INIC_WEP64_KEY1_4, (void *)key) ) return -1;
		if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_4, (void *)&key_type)) return -1;
		if (key_type == 0) sprintf (buffer, "%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4]);
		else sprintf (buffer, "%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4]);
		websWrite(wp, T("\"%s\")"), CharFilter2(buffer));
		#else
		websWrite(wp, T("new Array(\"%s\")"), CharFilter2(buffer));
		#endif
	}
	else if ( !strcmp (name, T("wep128keyall"))){
			unsigned char key[32];
			int key_type;

			if ( !apmib_get(MIB_INIC_WEP128_KEY1, (void *)key) ) return -1;
			if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE, (void *)&key_type)) return -1;
			if (key_type == 0) sprintf (buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			else sprintf (buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);

			#ifdef _INIC_MSSID_
			websWrite(wp, T("new Array(\"%s\","), CharFilter2(buffer));

			if ( !apmib_get(MIB_INIC_WEP128_KEY1_1, (void *)key) ) return -1;
			if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_1, (void *)&key_type)) return -1;
			if (key_type == 0) sprintf (buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			else sprintf (buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			websWrite(wp, T("\"%s\","), CharFilter2(buffer));
			
			if ( !apmib_get(MIB_INIC_WEP128_KEY1_2, (void *)key) ) return -1;
			if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_2, (void *)&key_type)) return -1;
			if (key_type == 0) sprintf (buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			else sprintf (buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			websWrite(wp, T("\"%s\","), CharFilter2(buffer));
			
			if ( !apmib_get(MIB_INIC_WEP128_KEY1_3, (void *)key) ) return -1;
			if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_3, (void *)&key_type)) return -1;
			if (key_type == 0) sprintf (buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			else sprintf (buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			websWrite(wp, T("\"%s\","), CharFilter2(buffer));
			
			if ( !apmib_get(MIB_INIC_WEP128_KEY1_4, (void *)key) ) return -1;
			if ( !apmib_get(MIB_INIC_WEP_KEY_TYPE_4, (void *)&key_type)) return -1;
			if (key_type == 0) sprintf (buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			else sprintf (buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],key[2],key[3],key[4],key[5],key[6],key[7],key[8],key[9],key[10],key[11], key[12]);
			websWrite(wp, T("\"%s\")"), CharFilter2(buffer));
		#else
		websWrite(wp, T("new Array(\"%s\")"), CharFilter2(buffer));
		#endif
	}
 	else if ( !strcmp(name, T("wepValueAll"))) {
#ifdef _INIC_MSSID_
 		int val;
 		apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 	 		websWrite(wp, T("new Array(\"%s\","), buffer);

 	 		memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);		websWrite(wp, T("new Array(\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_1, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_2, (void *)&val);
		if ( val == 0){
			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_3, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
		apmib_get( MIB_INIC_WEP_KEY_TYPE_4, (void *)&val);
		if ( val == 0){
			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
		}
		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_5, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_6, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_INIC_WEP_KEY_TYPE_7, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY1_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\")"), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY1_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\")"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}

#else
 		int val;
 		apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 	 		websWrite(wp, T("new Array(\"%s\","), buffer);

 	 		memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP64_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_INIC_WEP128_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\")"), buffer);
 		}
 		else{
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);		websWrite(wp, T("new Array(\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP64_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_INIC_WEP128_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\")"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}

#endif //_INIC_MSSID_
   		return 0;

 	}
	else if ( !strcmp(name, T("wpaCipherAll"))) {
		int iTkip, iAes;
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE, (void *)&iTkip);		apmib_get( MIB_INIC_WPA2_CIPHER_SUITE, (void *)&iAes);
#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_1, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_1, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_2, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_2, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_3, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_3, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_4, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_4, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_5, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_5, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_6, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_6, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE_7, (void *)&iTkip);	apmib_get( MIB_INIC_WPA2_CIPHER_SUITE_7, (void *)&iAes);
		websWrite(wp, T("\"%d\")"), iAes+iTkip);
#else
		websWrite(wp, T("new Array(\"%d\")"), iAes+iTkip);
#endif
		return 0;
	}
	else if ( !strcmp(name, T("wps_key_val")) )
	{
		int val2=0, val3=0, wep;
		if ( !apmib_get( MIB_INIC_SECURITY_MODE, (void *)&intVal) )
			return -1;

		if ( !apmib_get( MIB_INIC_WEP,  (void *)&wep) )
			return -1;

		if ( !apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val2) )
			return -1;

		if ( !apmib_get( MIB_INIC_WPA_PSK_FORMAT, (void *)&val3) )
			return -1;

 		memset(buffer, 0, sizeof(buffer));

		if ( intVal == 1 )
		{
			if( wep == 1){
				apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
					return 0;
				}
			}
			else{
				apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
					return 0;
				}
			}
		}
		else if( intVal == 2)
			apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer);
		else if ( intVal == 3 )
			apmib_get(MIB_INIC_WDS_WPA_PSK,  (void *)buffer);


		CharFilter6(buffer);
		

		websWrite(wp, T("%s"), buffer);

		return 0;
	}
	else if ( !strcmp(name, T("rep_wps_key_val")) )
	{
		int val2=0, val3=0, wep;
		if ( !apmib_get( MIB_INIC_SECURITY_MODE, (void *)&intVal) )
			return -1;

		if ( !apmib_get( MIB_INIC_WEP,  (void *)&wep) )
			return -1;

		if ( !apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val2) )
			return -1;

		if ( !apmib_get( MIB_INIC_WPA_PSK_FORMAT, (void *)&val3) )
			return -1;

 		memset(buffer, 0, sizeof(buffer));

		if ( intVal == 1 )
		{
			if( wep == 1){
				apmib_get(MIB_INIC_WEP64_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
					return 0;
				}
			}
			else{
				apmib_get(MIB_INIC_WEP128_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
					return 0;
				}
			}
		}
		else if( intVal == 2 || intVal == 3)
			apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer);

		CharFilter6(buffer);
		websWrite(wp, T("%s"), buffer);

		return 0;
	}
	else if ( !strcmp(name, T("wpaEncrypt"))) { //EDX Robert 2014_02_20 Add Support All Security
			//1.WPA=> TKIP or AES , 2.WPA2=> TKIP or AES 
		int val;  
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE, (void *)&val);  
			websWrite(wp, T("new Array(\"%d\","), val); 
		apmib_get( MIB_INIC_WPA2_CIPHER_SUITE, (void *)&val);
			websWrite(wp, T("\"%d\")"), val);
		return 0;
	}
	else if ( !strcmp(name, T("wpa_wpa2_Cipher"))) {
		int iTkip, iAes;
		apmib_get( MIB_INIC_WPA_CIPHER_SUITE, (void *)&iTkip);		
		apmib_get( MIB_INIC_WPA2_CIPHER_SUITE, (void *)&iAes);
		websWrite(wp, T("new Array(\"%d\","), iTkip);
		websWrite(wp, T("\"%d\")"), iAes);
		return 0;
	}
	else if ( !strcmp(name, T("keyTypeAll"))) {
		int val;
		apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val);
#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_1, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_2, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_3, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_4, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_5, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_6, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_WEP_KEY_TYPE_7, (void *)&val);
		websWrite(wp, T("\"%d\")"), val);
#else
		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

   	else if ( !strcmp(name, T("secModeAll"))) {
		apmib_get( MIB_INIC_SECURITY_MODE,  (void *)buffer) ;
#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_1,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_2,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_3,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_4,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_5,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_6,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_INIC_SECURITY_MODE_7,  (void *)buffer) ;
		websWrite(wp, T("\"%d\")"), (int)buffer[0]);
#else
		websWrite(wp, T("new Array(\"%d\")"), (int)buffer[0]);
#endif
		return 0;
	}
   	else if ( !strcmp(name, T("ssidAll"))) {
#ifdef _INIC_MSSID_
		apmib_get(MIB_INIC_SSID,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_1,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_2,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_3,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_4,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_5,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_6,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_INIC_SSID_7,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\")"), buffer);
#else
		apmib_get(MIB_INIC_SSID,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
#endif
		return 0;
	}
	else if ( !strcmp(name, T("ssid1Arr"))) {
		#ifdef _INIC_MSSID_
		apmib_get(MIB_INIC_SSID_1,  (void *)buffer);	
		//CharFilter6(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("ssidArr"))) {
		apmib_get(MIB_INIC_SSID,  (void *)buffer);	
		//CharFilter6(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enable1XAll"))) {
		int val;
		apmib_get( MIB_INIC_ENABLE_1X, (void *)&val);
#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_1, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_2, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_3, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_4, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_5, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_6, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_INIC_ENABLE_1X_7, (void *)&val);
		websWrite(wp, T("\"%d\")"), val);
#else
		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

	else if ( !strcmp(name, T("1xMode"))) {
		int val;
		apmib_get( MIB_INIC_1X_MODE, (void *)&val);
		websWrite(wp, T("%d"), val);
		return 0;
	}



   	else if ( !strcmp(name, T("staSsid"))) {
		if ( !apmib_get( MIB_STA_SSID,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
//enable wlan for hama wlanMP.asp
#ifdef _Customer_
   	else if ( !strcmp(name, T("wlanUp"))) {
		sprintf (buffer, "/bin/wlan.sh %s", INIC_IF);
		system (buffer);
		return websWrite(wp, T("%s"), " ");
	}

#endif
//************************************ Wireless Information *************************************
	else if ( !strcmp(name, T("bssid_drv"))) {
        if ( !apmib_get(MIB_INIC_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				                        buffer[2], buffer[3], buffer[4], buffer[5]);

	}
	else if ( !strcmp(name, T("ssid_drv"))) {

        if ( !apmib_get( MIB_INIC_SSID,  (void *)buffer) )
        	return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
					CharFilter2(buffer);
		#endif
		return websWrite(wp, buffer);
		//return websWrite(wp, T("%s"), buffer);
}
	else if ( !strcmp(name, T("ssid_drv2"))) {	// for stadev.asp status show ssid

        if ( !apmib_get( MIB_INIC_SSID,  (void *)buffer) )
        	return -1;
		return websWrite(wp, buffer);
}
#ifdef _INIC_MSSID_
	else if ( !strcmp(name, T("ssid1_drv"))) {
    if ( !apmib_get( MIB_INIC_SSID_1,  (void *)buffer) )
    	return -1;
			CharFilter6(buffer);
		return websWrite(wp, buffer);
}
#endif

	else if ( !strcmp(name, T("state_drv"))) {
		char *pMsg;
		if ( getWlBssInfo(INIC_IF, &bss) < 0)
			return websWrite(wp, "state_drv");		//Erwin Modify 04.09
		switch (bss.state) {
		case STATE_DISABLED:
			pMsg = T("Disabled");
			break;
		case STATE_IDLE:
			pMsg = T("Idle");
			break;
		case STATE_STARTED:
			pMsg = T("Started");
			break;
		case STATE_CONNECTED:
			pMsg = T("Connected");
			break;
		case STATE_SCANNING:
			pMsg = T("Scanning");
			break;
		default:
			pMsg=NULL;
		}
		return websWrite(wp, T("%s"), pMsg);
	}
	else if ( !strcmp(name, T("channel_drv"))) {
        if ( !apmib_get( MIB_INIC_CHAN_NUM,  (void *)&intVal) )
			return -1;
	  	if (intVal==0)
			sprintf(buffer, "%s", "auto" );
		else
			sprintf(buffer, "%d", intVal );
		return websWrite(wp, buffer);


		if (bss.channel)
			sprintf(buffer, "%d", bss.channel);
		else
			buffer[0] = '\0';

		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("stadrv_iNICautochan")))
	{
		FILE *fp1;
		#ifndef __TARGET_BOARD__
		system("echo \"36\" > /tmp/stadrv_iNICautochannel");
		#else
		//system("iwpriv wlan0 get_mib channel | cut -f 2 -d : > /tmp/get_wlan0_autochan");
		#endif
		if ((fp1 = fopen("/tmp/stadrv_iNICautochannel","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			if ( !apmib_get( MIB_STADRV_CHAN_NUM,  (void *)&intVal) ) return -1;
			sprintf(buffer, "%d", intVal );
				return websWrite(wp, buffer);	
		}
	}
	else if ( !strcmp(name, T("autochannel"))) {  //EDX patrick, if select auto chan, status show driver mib value
		FILE *fp1;
		#ifndef __TARGET_BOARD__
		system("echo \"36\" > /tmp/AutoChenAband");
		#else
		system(" iwconfig rai0 |  grep -n 'Channel=' | cut -f 2 -d '=' | cut -f 1 -d ' '  > /tmp/AutoChenAband");
		#endif
		if ((fp1 = fopen("/tmp/AutoChenAband","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			int intChan;
			if ( !apmib_get( MIB_INIC_CHAN_NUM,  (void *)&intChan) ) 
    		return -1;
			sprintf(buffer, "%d", intChan );
			return websWrite(wp, buffer);
		}
  }

   	else if ( !strcmp(name, T("channel"))) {

		if ( !apmib_get( MIB_INIC_CHAN_NUM,  (void *)&intVal) )
			return -1;

//		if (intVal==0)
//			sprintf(buffer, "%s", "auto" );
//		else
			sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}

   	else if ( !strcmp(name, T("wepAll"))) {
		apmib_get( MIB_INIC_WEP, (void *)&intVal);
#ifdef _INIC_MSSID_
		websWrite(wp, T("new Array(\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_1, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_2, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_3, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_4, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_5, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_6, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_WEP_7, (void *)&intVal);
		websWrite(wp, T("\"%d\")"), intVal);
#else
		websWrite(wp, T("new Array(\"%d\")"), intVal);
#endif
		return 0;
   	}

   	else if ( !strcmp(name, T("wep_key_is_empty"))) {  //0=empty     1=not empty
   websWrite(wp, T("new Array("));
		apmib_get( MIB_INIC_WEP64_KEY1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
#ifdef _INIC_MSSID_
		apmib_get( MIB_INIC_WEP64_KEY1_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY1_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY2_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY3_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP64_KEY4_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY1_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY2_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY3_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_INIC_WEP128_KEY4_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
#endif
		websWrite(wp, T("\"\")"));
		return 0;
   	}

   	else if ( !strcmp(name, T("wep"))) {
   		WEP_T wep;
		if ( !apmib_get( MIB_INIC_WEP,  (void *)&wep) )
			return -1;

		if ( wep == WEP_DISABLED )
			strcpy( buffer, T("Disabled") );
		else if ( wep == WEP64 )
			strcpy( buffer, T("64bits") );
		else if ( wep == WEP128)
			strcpy( buffer, T("128bits") );
   		return websWrite(wp, buffer);
   	}

  	else if ( !strcmp(name, T("defIp"))) {
		FILE *fp;
		system("flash get DEF_IP_ADDR | cut -f 2 -d \\' > /var/defip.tmp");
		if ((fp = fopen("/var/defip.tmp","r"))!=NULL) {
			fgets(buffer, 30, fp);
			fclose(fp);
			system("rm -f /var/defip.tmp");
   			return websWrite(wp, buffer);
		}else {
   			return websWrite(wp, "192.168.2.1");
		}
	}
  	else if ( !strcmp(name, T("ip"))) {
		if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr ) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
#ifdef _CONVERT_IP_ADDR_
  	else if ( !strcmp(name, T("convert_ip"))) {
		if ( apmib_get( MIB_CONVERT_IP_ADDR, (void *)&intaddr ) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
	}
#endif
	else if ( !strcmp(name, T("cloneMac"))) {
		int i;
		FILE *fp;
		char buf[100]={0},macaddr[20]={0},comment[50]={0};
#ifdef __TARGET_BOARD__
#ifndef _BOA_ 
                sprintf(comment,"cat /proc/net/arp | grep %s > /var/abc.txt",wp->ipaddr);
#else
                sprintf(comment,"cat /proc/net/arp | grep %s > /var/abc.txt",wp->remote_ip_addr);
#endif

		system(comment);
		if ((fp = fopen("/var/abc.txt","r"))!=NULL)
		{
			ptr=NULL;
			while (fgets(buf,100, fp) > 0){
				for (i=0;i<=1;i++) {
					if ((ptr = strchr(buf, ' ')) != NULL)
						while (*ptr == ' ' ) ptr++;
					sprintf(buf,"%s",ptr);
				}
			}

			fclose(fp);
			system("rm -f /var/abc.txt");
			if(ptr!=NULL) snprintf(macaddr, 18, "%s", ptr);
		}
#else
		snprintf(macaddr, 18, "%s", "00:33:44:33:22:00");
#endif
		for (i=0;i<=5;i++) {
			if ((ptr = strchr(macaddr, ':')) != NULL) {
				*ptr='\0';
				ptr++;
				strcat(macaddr,ptr);
			}
		}
		if (!strcmp(macaddr,"(null)"))
			return websWrite(wp, T("%s"), "000000000000");
		else
			return websWrite(wp, T("%s"), macaddr);
	}
	else if ( !strcmp(name, T("wlanMac"))) {
		if ( !apmib_get(MIB_INIC_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wanMac"))) {
		#if defined(_HWNICMAC_TO_WANDYNAMICMAC_)
		if ( !apmib_get(MIB_HW_NIC1_ADDR,  (void *)buffer) )
		#else
		if ( !apmib_get(MIB_WAN_MAC_ADDR,  (void *)buffer) )
		#endif
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
	}
#ifdef  HOME_GATEWAY
  	else if ( !strcmp(name, T("wan-ip"))) {
  		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if ( dhcp == PPPOE || dhcp == PPTP) {
			iface = PPPOE_IF;
			if ( !isConnectPPP() )
				iface = NULL;
		}
//		else if (opmode == WISP_MODE)
//			iface = INIC_IF;
		else
			iface = _WAN_IF_;

		if ( iface && getInAddr(iface, IP_ADDR, (void *)&intaddr ) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
   	else if ( !strcmp(name, T("wan-mask"))) {
		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if ( dhcp == PPPOE || dhcp == PPTP) {
			iface = PPPOE_IF;
			if ( !isConnectPPP() )
				iface = NULL;
		}
//		else if (opmode == WISP_MODE)
//			iface = INIC_IF;
		else
			iface = _WAN_IF_;

		if ( iface && getInAddr(iface, SUBNET_MASK, (void *)&intaddr ))
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
   	else if ( !strcmp(name, T("wan-gateway"))) {
		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if ( dhcp == PPPOE || dhcp == PPTP) {
			iface = PPPOE_IF;
			if ( !isConnectPPP() )
				iface = NULL;
		}
//		else if (opmode == WISP_MODE)
//			iface = INIC_IF;
		else
			iface = _WAN_IF_;
		if ( iface && getDefaultRoute(iface, &intaddr) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
	else if ( !strcmp(name, T("wan-hwaddr"))) {
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
//		if(opmode == WISP_MODE)
//			iface = INIC_IF;
//		else
			iface = _WAN_IF_;
		if ( getInAddr(iface, HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],
				pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}
//**************************************** DDNS Setting ****************************************
  	else if ( !strcmp(name, T("wan-dns1"))) {
		if ( !apmib_get( MIB_WAN_DNS1,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
  	else if ( !strcmp(name, T("wan-dns2"))) {
		if ( !apmib_get( MIB_WAN_DNS2,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
  	else if ( !strcmp(name, T("wan-dns3"))) {
		if ( !apmib_get( MIB_WAN_DNS3,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
 	else if ( !strcmp(name, T("wanDhcp-current")) ) {
   	if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		if ( dhcp == DHCP_CLIENT) {
  			if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
				return -1;
//			if(opmode == WISP_MODE)
//				iface = INIC_IF;
//			else
				iface = _WAN_IF_;
		 	if (!isDhcpClientExist(iface))
				return websWrite(wp, T("Getting IP from DHCP server..."));
			else
				return websWrite(wp, T("DHCP"));
		}
		else if ( dhcp == DHCP_DISABLED )
			return websWrite(wp, T("Fixed IP"));
		else if ( dhcp ==  PPPOE ) {
			if ( isConnectPPP())
				return websWrite(wp, T("PPPoE Connected"));
			else
				return websWrite(wp, T("PPPoE Disconnected"));
		}
	}
	else if ( !strcmp(name, T("syslog"))) {
		FILE *fp1;
		system("/bin/syslog.sh");
		if ((fp1 = fopen("/var/log/syslog","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}

	else if ( !strcmp(name, T("dhcplog"))) {
		#ifdef _DEBUG_DHCPC_

		FILE *fp1;

		if ((fp1 = fopen("/var/dhcp_debug.var","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		#else
			websWrite(wp, T("%s"), "\"\"");
		#endif

		return 0;
	}

//**************************************** routing table ****************************************
	else if ( !strcmp(name, T("routingTable"))) {
		FILE *fp1;
		int line=0;
		system("route > /var/routing.var");
		if ((fp1 = fopen("/var/routing.var","r"))!=NULL) {

			while (fgets(buffer, 100, fp1) > 0){
				if(line>0)
					websWrite(wp, T("%s"), buffer);
				line++;
			}
		fclose(fp1);
		}
		return 0;
	}
//**************************************** memory Display ****************************************
	else if ( !strcmp(name, T("getmem"))) {
		FILE *fp1;
		system("cat proc/meminfo > /var/memLog.var");
		if ((fp1 = fopen("/var/memLog.var","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
//**************************************** Security Log ****************************************
	else if ( !strcmp(name, T("seclog"))) {
		FILE *fp1;
		system("/bin/seclog.sh");
		if ((fp1 = fopen("/var/log/security","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}

//************************************** Internet Connection *************/*************************
	else if ( !strcmp(name, T("sta-current"))) {

		int k,h;
		FILE *fp1;
		system("wan-status.sh");
		if ((fp1 = fopen("/var/run/wanstat","r"))!=NULL) {
			for (k=0;k<7;k++) {
				fgets(staTbl[0][k], 50, fp1);
			}
			fclose(fp1);
		}
		strcpy(buffer,staTbl[0][0]);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer);

	}
	else if ( !strcmp(name, T("sta-ip"))) //Erwin modify 17.09
	{
		int k;
		strcpy(buffer, staTbl[0][1]);
		k=strlen(buffer)-1;
		while( k>=0 && isspace(buffer[k]) ) { buffer[k]='\0'; k--; }
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("sta-mask")))
		return websWrite(wp, T("%s"), staTbl[0][2]);
	else if ( !strcmp(name, T("sta-gateway"))) {
		if ( getDefaultRoute(BRIDGE_IF, &intaddr) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
	else if ( !strcmp(name, T("sta-mac")))
		return websWrite(wp, T("%s"), staTbl[0][3]);
	else if ( !strcmp(name, T("sta-dns1")))
		return websWrite(wp, T("%s"), staTbl[0][4]);
	else if ( !strcmp(name, T("sta-dns2")))
		return websWrite(wp, T("%s"), staTbl[0][5]);
#ifdef _USBLOG_
//**************************************** USB Connect/Disconnect Log ****************************************
	else if ( !strcmp(name, T("USBLog"))){
//		#define MAX_USBLOG_NUMBER 300
		struct _device_log
		{
			time_t time;
			char event[25];
			char device_name[50];
			char ip[20];
		} device_log;
		FILE *fp2;
		char temp[200], temp1[11];
		int i,j;


		memset( &device_log, 0, sizeof(device_log) );
		i=0;
      system("cat /proc/usblog* > /tmp/usblog");
		websWrite(wp, T("new Array(\n"));

		if((fp2=fopen("/tmp/usblog","r"))!=NULL)
		{
			while( fgets( temp, 200, fp2 )>0)
			{
				if( isdigit(temp[0]) )
				{
					strncpy(temp1, temp+0, 10);
					device_log.time=atol(temp1);

					strncpy(device_log.event, temp+11, 1);
//					j=sizeof(device_log.event)-1;
					switch(atoi(device_log.event))
					{
						case 1: strcpy(device_log.event, "USB Connected"); break;
						case 2: strcpy(device_log.event, "USB Disconnected"); break;
						case 3: strcpy(device_log.event, "Host used"); break;
						case 4: strcpy(device_log.event, "Host not used"); break;
						default:strcpy(device_log.event, "Unknown"); break;
					}

					strncpy(device_log.device_name, temp+13, 30);
					j=sizeof(device_log.device_name)-1;
					device_log.device_name[j]='\0';
					while(j>=0 && ( isspace(device_log.device_name[j]) || device_log.device_name[j]=='\0' ) ) { device_log.device_name[j]='\0'; j--; }

					strncpy(device_log.ip, temp+44, 15);
					j=sizeof(device_log.ip)-1;
					device_log.ip[j]='\0';
					while(j>=0 && ( isspace(device_log.ip[j]) || device_log.ip[j]=='\0' ) ) { device_log.ip[j]='\0'; j--;}


					strftime(temp, 200, "%y/%m/%d_%T", localtime(&device_log.time));

					websWrite(wp, T("new Array(\"%s\",\"%s\",\"%s\",\"%s\"),\n"),
						temp,
						device_log.event,
						device_log.device_name,
						device_log.ip);

					i++;
				}
			}
			fclose(fp2);
		}
		websWrite(wp, T("new Array("")"));
		websWrite(wp, T(");"));

		return 0;

	}

#endif
	//-----------------------------------------------------------------------------
#endif
	else if ( !strcmp(name, T("hwaddr"))) {
		if ( getInAddr(BRIDGE_IF, HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}
	else if ( !strcmp(name, T("bridgeMac"))) {
		if ( !apmib_get(MIB_ELAN_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
//*************************************** Get HW_MAC flash value ***************************************
//20060417 pippen
	else if ( !strcmp(name, T("nic0mac"))) {
		if ( !apmib_get( MIB_HW_NIC0_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("nic1mac"))) {
		if ( !apmib_get( MIB_HW_NIC1_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlmac"))) {
		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("flashregDomain"))) {
		if ( !apmib_get( MIB_HW_REG_DOMAIN, (void *)buffer) )
			return -1;
		return websWrite(wp, buffer);
	}
	/* Advance setting stuffs */
//******************************wireless setting for inprocomm ******************************
  	else if ( !strcmp(name, T("wlanTranRate"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_INIC_TRAN_RATE,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
//*********************************** Wireless Advance Setting ***********************************
//RexHua
  	else if ( !strcmp(name, T("wlanTxPower"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_INIC_TX_POWER,  (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}

	else if ( !strcmp(name, T("fragThreshold"))) {
		if ( !apmib_get( MIB_INIC_FRAG_THRESHOLD, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("rtsThreshold"))) {
		if ( !apmib_get( MIB_INIC_RTS_THRESHOLD, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("beaconInterval"))) {
		if ( !apmib_get( MIB_INIC_BEACON_INTERVAL, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("dtimPeriod"))) {
		if ( !apmib_get( MIB_INIC_DTIM_PERIOD, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("model"))) {

	}// kyle for jensen auto wpa key
	else if ( !strcmp(name, T("autowpakey"))) {
		FILE *fp;
			char buf[10];
			char buf2[100];
			char brf3[20];

	#ifdef _AUTOWPA_

		if ( !apmib_get( MIB_HW_NIC0_ADDR, (void *)buffer) )
			return -1;
		sprintf(brf3, "%02x%02x%02x%02x%02x%02x", buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);



			printf("DEBUG: mac=%s\n",buffer);
			sprintf(buf2,"/bin/AutoWPA %s > /var/autowpakey.var",brf3);
			printf("DEBUG: cmd=%s",buf2);
			system(buf2);
		if ((fp = fopen("/var/autowpakey.var","r"))!=NULL) {
			fgets(buf, 10, fp);
			fclose(fp);
		}else
			sprintf(buf, "%s", " ");
	#else
			sprintf(buf,"\"\"");
	#endif
		return websWrite(wp, buf);

	}
// kyle for Logitec auto wep key
	else if ( !strcmp(name, T("autowepkey"))) {
			FILE *fp;
				char buf[10];
				char buf2[100];
				char brf3[20];
		#ifdef _WEP_MAC_

			if ( !apmib_get( MIB_HW_NIC0_ADDR, (void *)buffer) )
				return -1;
			sprintf(brf3, "%02x%02x%02x%02x%02x%02x", buffer[0], buffer[1],
					buffer[2], buffer[3], buffer[4], buffer[5]);

				sprintf(buf2,"/bin/AutoWPA %s pwepmac > /var/autowepkey.var",brf3);

				system(buf2);
			if ((fp = fopen("/var/autowepkey.var","r"))!=NULL) {
				fgets(buf, 100, fp);
				fclose(fp);
			}else
				sprintf(buf, "%s", " ");
		#else
			sprintf(buf,"\"\"");
		#endif



		return websWrite(wp, buf);

	}
// TODO: Following counter need to get from driver in the future
	else if ( !strcmp(name, T("wlanTxPacketNum"))) {
#ifdef _WISP_WITH_STA_
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 4){
			if ( getStats(INIC_STA_IF, &stats, 1) < 0)
				stats.tx_packets = 0;
			else
				stats.tx_packets = stats.tx_packets / 1024;
		}else{
			if ( getStats(INIC_IF, &stats, 0) < 0)
				stats.tx_packets = 0;
		}
#else
		if ( getStats(INIC_IF, &stats, 0) < 0)
			stats.tx_packets = 0;
#endif
		sprintf(buffer, "%d", (int)stats.tx_packets);
   		return websWrite(wp, buffer);

	}
	else if ( !strcmp(name, T("wlanRxPacketNum"))) {
#ifdef _WISP_WITH_STA_
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 4){
			if ( getStats(INIC_STA_IF, &stats, 1) < 0)
				stats.rx_packets = 0;
			else
				stats.rx_packets = stats.rx_packets / 1024;

		}else{
			if ( getStats(INIC_IF, &stats, 0) < 0)
				stats.rx_packets = 0;
		}
#else
		if ( getStats(INIC_IF, &stats, 0) < 0)
			stats.rx_packets = 0;
#endif
		sprintf(buffer, "%d", (int)stats.rx_packets);
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("bssid"))) {
		apmib_get( MIB_INIC_DISABLED, (void *)&intVal);
		if ( intVal == 0 &&  getInAddr(INIC_IF, HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],
				pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}
        else if ( !strcmp(name, T("clientnum"))) {
                apmib_get( MIB_INIC_DISABLED, (void *)&intVal);
                if (intVal == 1)        // disable
                        intVal = 0;
                else {
                        INIC_STA_INFO_T *pInfo;
                        char buff[WEBS_BUFSIZE];
                        getWlStaInfo(INIC_IF,  (INIC_STA_INFO_Tp)buff );
                        pInfo = (INIC_STA_INFO_T *)buff;
			intVal = 0;
			int tmpval;
			for ( tmpval=0; tmpval<pInfo->Num ; tmpval++)
                        {
                                if (pInfo->Entry[tmpval].ApIdx == 0)
                                        intVal++;
                        }
                }
                sprintf(buffer, "%d", intVal );
                return websWrite(wp, buffer);
        }

#if defined(_INIC_MSSID_)
        else if ( !strcmp(name, T("clientnum1"))) {
                apmib_get( MIB_INIC_DISABLED, (void *)&intVal);
                if (intVal == 1)        // disable
                        intVal = 0;
                else {
                        INIC_STA_INFO_T *pInfo;
                        char buff[WEBS_BUFSIZE];
                        getWlStaInfo(INIC_IF,  (INIC_STA_INFO_Tp)buff );
                        pInfo = (INIC_STA_INFO_T *)buff;
			intVal = 0;
			int tmpval;
			for ( tmpval=0; tmpval<pInfo->Num ; tmpval++)
                        {
                                if (pInfo->Entry[tmpval].ApIdx == 1)
                                        intVal++;
                        }
                }
                sprintf(buffer, "%d", intVal );
                return websWrite(wp, buffer);
        }
        else if ( !strcmp(name, T("clientnum2"))) {
                apmib_get( MIB_INIC_DISABLED, (void *)&intVal);
                if (intVal == 1)        // disable
                        intVal = 0;
                else {
                        INIC_STA_INFO_T *pInfo;
                        char buff[WEBS_BUFSIZE];
                        getWlStaInfo(INIC_IF,  (INIC_STA_INFO_Tp)buff );
                        pInfo = (INIC_STA_INFO_T *)buff;
			intVal = 0;
			int tmpval;
			for ( tmpval=0; tmpval<pInfo->Num ; tmpval++)
                        {
                                if (pInfo->Entry[tmpval].ApIdx == 2)
                                        intVal++;
                        }
                }
                sprintf(buffer, "%d", intVal );
                return websWrite(wp, buffer);
        }
        else if ( !strcmp(name, T("clientnum3"))) {
                apmib_get( MIB_INIC_DISABLED, (void *)&intVal);
                if (intVal == 1)        // disable
                        intVal = 0;
                else {
                        INIC_STA_INFO_T *pInfo;
                        char buff[WEBS_BUFSIZE];
                        getWlStaInfo(INIC_IF,  (INIC_STA_INFO_Tp)buff );
                        pInfo = (INIC_STA_INFO_T *)buff;
			intVal = 0;
			int tmpval;
			for ( tmpval=0; tmpval<pInfo->Num ; tmpval++)
                        {
                                if (pInfo->Entry[tmpval].ApIdx == 3)
                                        intVal++;
                        }
                }
                sprintf(buffer, "%d", intVal );
                return websWrite(wp, buffer);
        }
        else if ( !strcmp(name, T("bssid1"))) {
                int k,h;
                FILE *fp1;
                char strMAC[20];
                system("ifconfig |grep ra1 | cut -b 39-55 > /tmp/SSIDMAC");
                if ((fp1 = fopen("/tmp/SSIDMAC","r"))!=NULL) {
                        fgets(strMAC, 50, fp1);
                        fclose(fp1);
                }
                strcpy(buffer,strMAC);
                h=strlen(buffer)-1;
                while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
                return websWrite(wp, T("%s"), buffer);
        }
        else if ( !strcmp(name, T("bssid2"))) {
                int k,h;
                FILE *fp1;
                char strMAC[20];
                system("ifconfig |grep ra2 | cut -b 39-55 > /tmp/SSIDMAC");
                if ((fp1 = fopen("/tmp/SSIDMAC","r"))!=NULL) {
                        fgets(strMAC, 50, fp1);
                        fclose(fp1);
                }
                strcpy(buffer,strMAC);
                h=strlen(buffer)-1;
                while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
                return websWrite(wp, T("%s"), buffer);
        }
        else if ( !strcmp(name, T("bssid3"))) {
                int k,h;
                FILE *fp1;
                char strMAC[20];
                system("ifconfig |grep ra3 | cut -b 39-55 > /tmp/SSIDMAC");
                if ((fp1 = fopen("/tmp/SSIDMAC","r"))!=NULL) {
                        fgets(strMAC, 50, fp1);
                        fclose(fp1);
                }
                strcpy(buffer,strMAC);
                h=strlen(buffer)-1;
                while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
                return websWrite(wp, T("%s"), buffer);
        }
#endif

//************************************** Wireless WDS **************************************
	else if ( !strcmp(name, T("wlLinkMac1"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC1,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac2"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC2,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac3"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC3,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac4"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC4,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac5"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC5,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac6"))) {
		if ( !apmib_get(MIB_INIC_WL_LINKMAC6,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
#ifdef WLAN_WPA
	else if ( !strcmp(name, T("rsIp"))) {
		if ( !apmib_get( MIB_INIC_RS_IP,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("rsPort"))) {
		if ( !apmib_get( MIB_INIC_RS_PORT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
 	else if ( !strcmp(name, T("rsPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_INIC_RS_PASSWORD,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsIp"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_IP,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("accountRsPort"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_PORT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_PASSWORD,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTime"))) {
		if ( !apmib_get( MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeDay"))) {
		if ( !apmib_get( MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal/86400 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeHr"))) {
		if ( !apmib_get( MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", (intVal%86400)/3600 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeMin"))) {
		if ( !apmib_get( MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", ((intVal%86400)%3600)/60 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeSec"))) {
		if ( !apmib_get( MIB_INIC_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", ((intVal%86400)%3600)%60 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("rsIpall"))) {
		#ifdef _MULTIPLE_INIC_RS_SETTING_
		apmib_get( MIB_INIC_RS_IP,  (void *)&intaddr);   memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("new Array(\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_INIC_RS_IP_1,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_INIC_RS_IP_2,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_INIC_RS_IP_3,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_INIC_RS_IP_4,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\")"), inet_ntoa(intaddr));
		#else
		apmib_get( MIB_INIC_RS_IP,  (void *)&intaddr);   memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("new Array(\"%s\")"), inet_ntoa(intaddr));
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("rsPortall")) ) {
		#ifdef _MULTIPLE_INIC_RS_SETTING_
		apmib_get( MIB_INIC_RS_PORT, (void *)&intVal);			websWrite(wp, T("new Array(\"%d\","), intVal);
		apmib_get( MIB_INIC_RS_PORT_1, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_RS_PORT_2, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_RS_PORT_3, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_INIC_RS_PORT_4, (void *)&intVal);		websWrite(wp, T("\"%d\")"), intVal);
		#else
		apmib_get( MIB_INIC_RS_PORT, (void *)&intVal);			websWrite(wp, T("new Array(\"%d\")"), intVal);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("rsPasswordall"))) {
		#ifdef _MULTIPLE_INIC_RS_SETTING_
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD,   (void *)buffer);		websWrite(wp, T("new Array(\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD_1, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD_2, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD_3, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD_4, (void *)buffer);		websWrite(wp, T("\"%s\")"), buffer);
		#else
		buffer[0]='\0'; apmib_get( MIB_INIC_RS_PASSWORD,   (void *)buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#endif
		return 0;
	}
 	else if ( !strcmp(name, T("wdsPskValue"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_INIC_WDS_WPA_PSK,  (void *)buffer) )
			return -1;
		for (i=0; i<strlen(buffer); i++)
			buffer[i]='*';
		buffer[i]='\0';
		//return websWrite(wp, buffer);
		CharFilter5(buffer);
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsPskValue_pci"))) {
		if ( !apmib_get(MIB_INIC_WDS_WPA_PSK,  (void *)buffer) )
			return -1;
		
   		return websWrite(wp, buffer);
	}
 	else if ( !strcmp(name, T("pskValue"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer) )
			return -1;
//		#ifdef _Customer_
//		for (i=0; i<  ((strlen(buffer)>39)?40:strlen(buffer))  ; i++)
//		#else
		for (i=0; i<strlen(buffer); i++)
//		#endif
			buffer[i]='*';
		buffer[i]='\0';
   		return websWrite(wp, buffer);
	}
#ifdef _INIC_MSSID_ //vance
 	else if ( !strcmp(name, T("pskValue1"))) {
 			int i;
 			buffer[0]='\0';
 			if ( !apmib_get(MIB_INIC_WPA_PSK_1,  (void *)buffer) )
 				return -1;
 	//		#ifdef _Customer_
 	//		for (i=0; i<  ((strlen(buffer)>39)?40:strlen(buffer))  ; i++)
 	//		#else
 			for (i=0; i<strlen(buffer); i++)
 	//		#endif
 				buffer[i]='*';
 			buffer[i]='\0';
 	   		return websWrite(wp, buffer);
 		}
 #endif
	//**************************** pinNumber from client**************
	 	else if ( !strcmp(name, T("pinNumber"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_INIC_WPS_PING_CODE,  (void *)buffer) )
			return -1;
   return websWrite(wp, buffer);
	}
	//**************************************** get wps pincode , kyle ****************************************
	else if ( !strcmp(name, T("pinCode"))) {
		FILE *fp1;

		sprintf (buffer, "wpstool %s Pincode > /var/pincode.var", INIC_IF);
		system (buffer);

		if ((fp1 = fopen("/var/pincode.var","r"))!=NULL) {
			if(	fgets(buffer, 100, fp1) <= 0)
				sprintf( buffer, "%s" ,"0");

				if(buffer[strlen(buffer)-1] == '\n')
					buffer[strlen(buffer)-1] ='\0';

			fclose(fp1);

			}else{
				sprintf( buffer, "%s" ,"0");
			}
			return websWrite(wp, T("%s"), buffer);

	}
 	else if ( !strcmp(name, T("wpsPskValue"))) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_INIC_WPA_PSK,  (void *)buffer) )
			return -1;
//EDX shakim start
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
//EDX shakim end
   		return websWrite(wp, buffer);
	}
#ifdef _INIC_MSSID_ //vance
	else if ( !strcmp(name, T("wpsPskValue1"))) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_INIC_WPA_PSK_1,  (void *)buffer) )
			return -1;
//EDX shakim start
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
//EDX shakim end
   		return websWrite(wp, buffer);
	}
#endif
	else if ( !strcmp(name, T("accountRsUpdateDelay"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_UPDATE_DELAY, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("rsInterval"))) {
		if ( !apmib_get( MIB_INIC_RS_INTERVAL_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsInterval"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_INTERVAL_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif

#ifdef HOME_GATEWAY // For Guarantee QoS
	else if ( !strcmp(name, T("maxdownbandwidth"))) {
		if ( !apmib_get( MIB_MAX_DOWNLOAD_BANDWIDTH, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("maxupbandwidth"))) {
		if ( !apmib_get( MIB_MAX_UPLOAD_BANDWIDTH, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif

#ifdef UNIVERSAL_REPEATER
	else if ( !strcmp(name, T("repeaterSSID"))) {
		if ( !apmib_get( MIB_INIC_REPEATER_SSID, (void *)&buffer) )
			return -1;
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
   		return websWrite(wp, buffer);
	}
#endif

#ifdef _MCAST_RATE_SUPPORT_
	else if ( !strcmp(name, T("mcastinfo"))) {
		if ( !apmib_get(MIB_INIC_MCAST_RATE,  (void *)&intVal) )
			return -1;
		sprintf (buffer, "%d", intVal);
		return websWrite(wp, buffer);
	}
#endif
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
	else if ( !strcmp(name, T("wlanSchEnabled_5G")) ) {
		apmib_get(MIB_INIC_SCH_ENABLED, (void *)&intVal);
		return websWrite(wp, T("%d"), intVal);
	}
#endif

	else{
		return websWrite(wp, "");
	}
 	return -1;
}
/*
/////////////////////////////////////////////////////////////////////////////
int getIdValue(id, eid) {
	char_t buffer[50];
	int val;
	if ( !apmib_get( id, (void *)&val) )
		return -1;
	sprintf(buffer, "%d", val);
	ejSetResult(eid, buffer);
	return 0;
}
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef _BOA_
int getiNICIndex(int eid, webs_t wp, int argc, char_t **argv)
#else
int getiNICIndex(char *varName, char *indexValue)
#endif
{
	char_t *name, buffer[50];
	int chan, val;
	REG_DOMAIN_T domain;
	WEP_T wep;
	DHCP_T dhcp;

#ifdef HOME_GATEWAY
	OPMODE_T opmode=-1;
	char_t *iface=NULL;
#ifdef VPN_SUPPORT
	IPSECTUNNEL_T entry;
#endif
#endif

#ifdef UNIVERSAL_REPEATER
  int id;
#endif

#ifndef _BOA_
   	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
   		websError(wp, 400, T("Insufficient args\n"));
   		return -1;
   	}
#else
        name = varName;
#endif

//************************************* System setting *************************************
	if ( !strcmp(name, T("licence")) ) {
 		if ( !apmib_get( MIB_LICENCE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlDev"))) {	// 0-disabled 1-realtek 2-intersil 3-ipn
			ejSetResult(eid, "4");
			return 0;
	}
	else if ( !strcmp(name, T("psDev"))) {
		#ifdef _OFF_PS_DEVICE_
			ejSetResult(eid, "0");
		#elif _ON_PS_DEVICE_
			ejSetResult(eid, "1");
		#endif
			return 0;
	}
	else if ( !strcmp(name, T("sDev"))) {
		#ifdef _OFF_S_DEVICE_
			ejSetResult(eid, "0");
		#elif _ON_S_DEVICE_
			ejSetResult(eid, "1");
		#endif
			return 0;
	}
	else if ( !strcmp(name, T("apMachType"))) {
		#ifndef _IS_GATEWAY_
			ejSetResult(eid, "1");
		#endif
			return 0;
	}
	else if ( !strcmp(name, T("vsionType"))) {
		#ifdef _planet_
			ejSetResult(eid, "3");
		#else
			ejSetResult(eid, "6");
		#endif
			return 0;
	}
	else if ( !strcmp(name, T("modelType"))) {
		#ifdef _micronet_
			ejSetResult(eid, "1");
		#else
			ejSetResult(eid, "0");
		#endif
			return 0;
	}
	else if ( !strcmp(name, T("l2tpGwMode"))) {
		#if  defined(_edimax20bw_) || defined(_edimax_)  || defined(_edimaxEZ_)
			ejSetResult(eid, "1");
		#else
			ejSetResult(eid, "0");
		#endif
			return 0;
	}
//************************************* STAION of ENCRYPT *************************************
	else if ( !strcmp(name, T("dhcp")) ) {
 		if ( !apmib_get( MIB_DHCP, (void *)&dhcp) )
			return -1;
		sprintf(buffer, "%d", (int)dhcp);
		ejSetResult(eid, buffer);
		return 0;
	}
  	else if ( !strcmp(name, T("dhcp-current")) ) {
   		if ( !apmib_get( MIB_DHCP, (void *)&dhcp) )
			return -1;
		if ( dhcp == DHCP_CLIENT && !isDhcpClientExist(BRIDGE_IF))
			dhcp = DHCP_DISABLED;
		sprintf(buffer, "%d", (int)dhcp);
		ejSetResult(eid, buffer);
		return 0;
	}
 	else if ( !strcmp(name, T("stp")) ) {
   		if ( !apmib_get( MIB_STP_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _WATCH_DOG_
 	else if ( !strcmp(name, T("watchdog_enable"))) {

 			if ( !apmib_get( MIB_WATCH_DOG_ENABLE,  (void *)&val) )	return -1;
 			sprintf(buffer, "%d", val);
 			ejSetResult(eid, buffer);
 		return 0;
 	}
#endif

#ifdef _AP_DHCP_SPACE_
	else if ( !strcmp(name, T("apdhcp")) ) {
 		if ( !apmib_get( MIB_AP_DHCP, (void *)&dhcp) )
			return -1;
		sprintf(buffer, "%d", (int)dhcp);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ARindex")) ) {
		FILE *fp;
		system("cat /dev/switch_but > /tmp/getAR");
		if((fp=fopen("/tmp/getAR","r"))!=NULL)
		{
			fgets(buffer,2,fp);
			ejSetResult(eid, buffer);
		}
		return 0;
	}
#endif
#ifdef _TIME_ZONE_TYPE_
 	else if ( !strcmp(name, T("time_type"))) {
 	 	 			if ( !apmib_get( MIB_TIME_TYPE,  (void *)&val) )	return -1;
 	 	 			sprintf(buffer, "%d", val);
 	 	 			ejSetResult(eid, buffer);
 	 	 		return 0;
 	 }
#endif

	else if ( !strcmp(name, T("systime")) ) {		//Erwin Add 09.23
		FILE* fpt;
		char *rdptr;
//		system("date +%Y,%m,%d,%H,%M,%S > timetmp.txt");

#ifdef __TARGET_BOARD__
		system("/bin/date.sh");
#else
		system("../../script/date.sh");
#endif
       if ((fpt = fopen("/var/time.tmp","r"))!=NULL)
        {
			fread(buffer, 1, 100, fpt);
			if ( (rdptr = strchr(buffer, '\n'))!=NULL)
				*rdptr = '\0';
			fclose(fpt);
			system("rm -f /var/time.tmp");
		}
		ejSetResult(eid, buffer);
		return 0;
	}
//************************************** WiFi test **************************************
	else if ( !strcmp(name, T("Customer"))) {
		if ( !apmib_get( MIB_WIFI_TEST, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//************************************** Country Reg **************************************
	else if ( !strcmp(name, T("countryReg"))) {
		#ifdef _DFS_
			if ( !apmib_get( MIB_HW_COUNTRY_REG, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
		#else
			ejSetResult(eid, "\"\"");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("countryRegAband"))) {
		#ifdef _DFS_
			if ( !apmib_get( MIB_HW_COUNTRY_REG_ABAND, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
		#else
			ejSetResult(eid, "\"\"");
		#endif
		return 0;
	}

//************************************ STATION of ENCRYPT ************************************
#ifndef HOME_GATEWAY
	else if ( !strcmp(name, T("enCltCer")) ) {
		if ( !apmib_get( MIB_CLT_CER_FILE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enSerCer")) ) {
		if ( !apmib_get( MIB_SER_CER_FILE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staSecMode")) ) {
		if ( !apmib_get( MIB_STA_SEC_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("adhocSecMode")) ) {
		if ( !apmib_get( MIB_ADHOC_SEC_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staKeyType")) ) {
		if ( !apmib_get( MIB_STA_WEP_KEY_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staEncrypt"))) {
		if ( !apmib_get( MIB_STA_ENCRYPT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staWep")) ) {
		if ( !apmib_get( MIB_STA_WEP, (void *)&wep) )
			return -1;
		sprintf(buffer, "%d", (int)wep);
		ejSetResult(eid, buffer);
   	    	return 0;
	}
	else if ( !strcmp(name, T("staDefKeyId")) ) {
		if ( !apmib_get( MIB_STA_WEP_DEFKEY, (void *)&val) )
			return -1;
		val++;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staWpaCipher"))) {
		if ( !apmib_get( MIB_STA_WPA_CIPHER_SUITE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staPskFormat"))) {
		if ( !apmib_get( MIB_STA_WPA_PSK_FORMAT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("staEnRadius"))) {
		if ( !apmib_get( MIB_STA_ENRADIUS, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staEapTp"))) {
		if ( !apmib_get( MIB_STA_EAP_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staEnCltCer"))) {
		if ( !apmib_get( MIB_STA_ENCLT_CER, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staEnSerCer"))) {
		if ( !apmib_get( MIB_STA_ENSER_CER, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("cltCerFile"))) {
		if ( !apmib_get( MIB_CLT_CER_FILE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("serCerFile"))) {
		if ( !apmib_get( MIB_SER_CER_FILE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("staPream"))) {
		if ( !apmib_get( MIB_STA_PREAMBLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//************************************** RADIUS Server **************************************
	else if ( !strcmp(name, T("rserEnabled"))) {
		if ( !apmib_get( MIB_RSER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("rserCltNum")) ) {
		if ( !apmib_get( MIB_RSER_CLT_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("rserUsrNum")) ) {
		if ( !apmib_get( MIB_RSER_USR_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
//**************************************** WEP of WDS ****************************************
	else if ( !strcmp(name, T("wdsEncrypt"))) {
		if ( !apmib_get( MIB_INIC_WDS_ENCRYPT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsWpaAuth"))) {
		if ( !apmib_get( MIB_INIC_WDS_WPA_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsWpaCipher"))) {
		int iTkip, iAes;
		if ( !apmib_get( MIB_INIC_WDS_WPA_CIPHER_SUITE, (void *)&iTkip) )
			return -1;
		if ( !apmib_get( MIB_INIC_WDS_WPA2_CIPHER_SUITE, (void *)&iAes) )
			return -1;
		if (iTkip==WPA_CIPHER_TKIP && iAes==0) val=1;
		else if (iTkip==0 && iAes==WPA_CIPHER_AES) val=2;
		else if (iTkip==WPA_CIPHER_TKIP && iAes==WPA_CIPHER_AES) val=3;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsPskFormat"))) {
		if ( !apmib_get( MIB_INIC_WDS_WPA_PSK_FORMAT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
  else if ( !strcmp(name, T("disProtection"))) {
		if ( !apmib_get( MIB_INIC_PROTECTION_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
  }
  else if ( !strcmp(name, T("wispWanId")) ) {
		if ( !apmib_get( MIB_WISP_WAN_ID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("opMode")) ) {
		if ( !apmib_get( MIB_OP_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("wlanMacClone"))) {
		if ( !apmib_get( MIB_INIC_NAT25_MAC_CLONE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("band")) ) {
		if ( !apmib_get( MIB_INIC_BAND, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("aband")) ) {
		#ifdef _DBAND_
			if ( !apmib_get( MIB_ABAND_MODE, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", (int)val) ;
			 ejSetResult(eid, buffer);
		#else
			ejSetResult(eid, "\"\"");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("dualmode")) ) {
		#ifdef _DBAND_
			if ( !apmib_get( MIB_DUAL_BAND_SWITCH, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", (int)val) ;
			 ejSetResult(eid, buffer);
		#else

			ejSetResult(eid, "\"\"");
		#endif
		return 0;
	}


	else if ( !strcmp(name, T("fixTxRate")) ) {
		if ( !apmib_get( MIB_INIC_FIX_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
//**************************************  MAC Filter **************************************
	else if ( !strcmp(name, T("macFilterNum")) ) {
		if ( !apmib_get( MIB_MACFILTER_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("macDenyEnabled")) ) {
		if ( !apmib_get( MIB_MACDENY_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#if defined(_WLAN_AC_SWITCH_)
	else if ( !strcmp(name, T("wlanacsw")) ) {
			if ( !apmib_get( MIB_INIC_AC_SWITCH, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
	else if ( !strcmp(name, T("wlanac1sw")) ) {
				if ( !apmib_get( MIB_INIC_AC1_SWITCH, (void *)&val) )
					return -1;
				sprintf(buffer, "%d", val);
				ejSetResult(eid, buffer);
				return 0;
			}
	else if ( !strcmp(name, T("wlanac2sw")) ) {
				if ( !apmib_get( MIB_INIC_AC2_SWITCH, (void *)&val) )
					return -1;
				sprintf(buffer, "%d", val);
				ejSetResult(eid, buffer);
				return 0;
			}
	else if ( !strcmp(name, T("wlanac3sw")) ) {
				if ( !apmib_get( MIB_INIC_AC3_SWITCH, (void *)&val) )
					return -1;
				sprintf(buffer, "%d", val);
				ejSetResult(eid, buffer);
				return 0;
			}
#endif
#ifdef _LAN_WAN_ACCESS_
	else if ( !strcmp(name, T("lanWanAccess"))) {

		if ( !apmib_get( MIB_LAN_WAN_ACCESS, (void *)&val) )
			return -1;
		sprintf(buffer, "%lu", val );
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
//******************************wireless setting for inprocomm ******************************
	else if ( !strcmp(name, T("wlanRateMode")) ) {
		if ( !apmib_get( MIB_INIC_RATE_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanCts")) ) {
		if ( !apmib_get( MIB_INIC_CTS, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanBurst")) ) {
		if ( !apmib_get( MIB_INIC_BURST, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
#ifdef HOME_GATEWAY
	else if ( !strcmp(name, T("wanDhcp")) ) {
		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
		sprintf(buffer, "%d", (int)dhcp);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wanDhcp-current")) ) {
		if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
			return -1;
  	if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
//		if(opmode == WISP_MODE)
//			iface = INIC_IF;
//		else
			iface = _WAN_IF_;
		if ( dhcp == DHCP_CLIENT && !isDhcpClientExist(iface))
			dhcp = DHCP_DISABLED;
		sprintf(buffer, "%d", (int)dhcp);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wanDNS")) ) {
		DNS_TYPE_T dns;
		if ( !apmib_get( MIB_WAN_DNS_MODE, (void *)&dns) )
			return -1;
		sprintf(buffer, "%d", (int)dns);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wanMode"))) {
		if ( !apmib_get( MIB_WAN_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}

//************************************** PPPoE Setting **************************************
	else if ( !strcmp(name, T("pppConnectType")) ) {
		PPP_CONNECT_TYPE_T type;
		if ( !apmib_get( MIB_PPP_CONNECT_TYPE, (void *)&type) )
			return -1;
		sprintf(buffer, "%d", (int)type);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pppConnectStatus")) ) {
		sprintf(buffer, "%d", isConnectPPP());
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pppEnTtl"))) {
		if ( !apmib_get( MIB_PPP_TTL_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** PPTP Setting ***************************************
	else if ( !strcmp(name, T("pptpIpMode"))) {
		if ( !apmib_get( MIB_PPTP_IPMODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pptpConnectType")) ) {
		PPP_CONNECT_TYPE_T type;
		if ( !apmib_get( MIB_PPTP_CONNECT_TYPE, (void *)&type) )
			return -1;
		sprintf(buffer, "%d", (int)type);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pptpBEZEQEnable")) ) {
		PPP_CONNECT_TYPE_T type;
		if ( !apmib_get( MIB_PPTP_BEZEQ_ENABLE, (void *)&type) )
			return -1;
		sprintf(buffer, "%d", (int)type);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** L2TP Setting ***************************************
	else if ( !strcmp(name, T("L2TPIpMode"))) {
		if ( !apmib_get( MIB_L2TP_IPMODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("L2TPConnectType")) ) {
		PPP_CONNECT_TYPE_T type;
		if ( !apmib_get( MIB_L2TP_CONNECT_TYPE, (void *)&type) )
			return -1;
		sprintf(buffer, "%d", (int)type);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** Telstra Big Pond ***************************************
	else if ( !strcmp(name, T("telBPEnabled"))) {
		if ( !apmib_get( MIB_TELBP_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//************************************** Port Forwarding **************************************
	else if ( !strcmp(name, T("portFwEnabled"))) {
		if ( !apmib_get( MIB_PORTFW_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("portFwNum")) ) {
		if ( !apmib_get( MIB_PORTFW_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//******************************************************************************************
/*	else if ( !strcmp(name, T("ipFilterNum")) ) {	//no use
		if ( !apmib_get( MIB_IPFILTER_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("portFilterNum")) ) {	//no sue
		if ( !apmib_get( MIB_PORTFILTER_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ipFilterEnabled"))) {	//no use
		if ( !apmib_get( MIB_IPFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("portFilterEnabled"))) {	//no use
		if ( !apmib_get( MIB_PORTFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
*/
//************************************** Special Application **************************************
	else if ( !strcmp(name, T("triggerPortEnabled"))) {
		if ( !apmib_get( MIB_TRIGGERPORT_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("triggerPortNum")) ) {
		if ( !apmib_get( MIB_TRIGGERPORT_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("macFilterEnabled"))) {
		if ( !apmib_get( MIB_MACFILTER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("dmzEnabled"))) {
		if ( !apmib_get( MIB_DMZ_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//************************************** QoS **************************************
	else if ( !strcmp(name, T("wan1QosNum")) ) {	//WAN1
		if ( !apmib_get( MIB_WAN1_QOS_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wan1QosEnabled")) ) {
		if ( !apmib_get( MIB_WAN1_QOS_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#if defined(_EZ_QOS_)
	else if ( !strcmp(name, T("EZQosNum")) ) {
		if ( !apmib_get( MIB_EZ_QOS_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("EZQosEnabled")) ) {
		if ( !apmib_get( MIB_EZ_QOS_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
  else if ( !strcmp(name, T("EZQos_Mode"))){
    if (!apmib_get( MIB_EZ_QOS_MODE, (void *)&val) )
    	return -1;
    sprintf(buffer, "%d", val);
    ejSetResult(eid, buffer);
    return 0;
    }
#endif

	else if ( !strcmp(name, T("newpage")) ) {
		sprintf(buffer, "%d", newpage);
		if(newpage == 1)
		{
			apmib_reinit();
			First = 1;
			Changed = 0;
		}
		newpage = 1;
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("Changed")) ) {
		if ( Changed == 1 && First == 1 )
		{
			sprintf(buffer, "%d", 1);
			First = 0;
		} else
			sprintf(buffer, "%d", 0);
		ejSetResult(eid, buffer);
		return 0;
	}
/*	else if ( !strcmp(name, T("apply_enabled"))) {		//removed by Erwin
		sprintf(buffer, "%d", need_reboot);
		ejSetResult(eid, buffer);
		return 0;
	}
*/
//************************************** Static Routing **************************************
	else if ( !strcmp(name, T("sroutEnabled"))) {
		if ( !apmib_get( MIB_SROUT_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("sroutNum")) ) {
		if ( !apmib_get( MIB_SROUT_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wizardEnabled")) ) {
		sprintf(buffer, "%d", wizardEnabled);
//		wizardEnabled = 0;
//		printf("\nbuffer=%s\n",buffer);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("genEnabled")) ) {
		sprintf(buffer, "%d", wizardEnabled);
		wizardEnabled = 0;
//		printf("\nbuffer=%s\n",buffer);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
// Tommy
	else if ( !strcmp(name, T("lanipChanged")) ) {
		sprintf(buffer, "%d", lanipChanged);
		ejSetResult(eid, buffer);
		return 0;
	}
////*************************************Kyle 2007.03.12 for WPS*************************************

	else if ( !strcmp(name, T("wpsEnable"))) {
		if ( !apmib_get( MIB_INIC_WPS_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsDisplayKey"))) {
		if ( !apmib_get( MIB_INIC_WPS_DISPLAY_KEY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsProxyEnable"))) {
		if ( !apmib_get( MIB_INIC_WPS_PROXY_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigMode"))) {
		if ( !apmib_get( MIB_INIC_WPS_INTERNAL_REG, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigType"))) {
		if ( !apmib_get( MIB_INIC_WPS_CONFIG_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigStatus"))) {
		if ( !apmib_get( MIB_INIC_WPS_CONFIG_STATUS, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsStatus"))) {
			FILE *fp;
			char c='0';
		if ((fp = fopen("/var/wpsStatus.var","r"))!=NULL) {
			c=fgetc(fp);
			fclose(fp);
		}else
			c='0';
		if(c<'0' || c>'34')
				c='0';
		sprintf(buffer, "%c", c);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsTime"))) {
			FILE *fp;
			char buf[10];
		if ((fp = fopen("/var/wpsTime.var","r"))!=NULL) {
			fgets(buf, 10, fp);
			fclose(fp);
		}else
			sprintf(buf, "%s", "0");


		sprintf(buffer, "%s", buf);
		ejSetResult(eid, buffer);
		return 0;
	}

//****************************************************************************************************************************************************
// Tommy

//************************************** Wireless Setting **************************************
	else if ( !strcmp(name, T("1xMode"))) {
		if ( !apmib_get( MIB_INIC_1X_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

 	else if ( !strcmp(name, T("channelA"))) {
		#ifdef _DBAND_
			if ( !apmib_get( MIB_ABAND_CHANNEL, (void *)&chan) )
				return -1;
			sprintf(buffer, "%d", chan);

		#else
			sprintf(buffer, "%d", 0 );
		#endif

			ejSetResult(eid, buffer);
			return 0;;

   	}
	else if ( !strcmp(name, T("channel")) ) {
		if ( !apmib_get( MIB_INIC_CHAN_NUM, (void *)&chan) )
			return -1;
		sprintf(buffer, "%d", chan);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("regDomain")) ) {
		if ( !apmib_get( MIB_HW_COUNTRY_REG_ABAND, (void *)&domain) )
			return -1;
		sprintf(buffer, "%d", (int)domain);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wep")) ) {
		if ( !apmib_get( MIB_INIC_WEP, (void *)&wep) )
			return -1;
		sprintf(buffer, "%d", (int)wep);
		ejSetResult(eid, buffer);
   	    	return 0;
	}
#ifdef _INIC_MSSID_
	else if ( !strcmp(name, T("wep1")) ) {
		if ( !apmib_get( MIB_INIC_WEP_1, (void *)&wep) )
			return -1;
		sprintf(buffer, "%d", (int)wep);
		ejSetResult(eid, buffer);
   	    	return 0;
	}
#endif
	else if ( !strcmp(name, T("defaultKeyId")) ) {
		if ( !apmib_get( MIB_INIC_WEP_DEFAULT_KEY, (void *)&val) )
			return -1;
		val++;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanSetTx")) ) {
		if ( !apmib_get( MIB_INIC_SET_TX, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("keyType")) ) {
		if ( !apmib_get( MIB_INIC_WEP_KEY_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
  	else if ( !strcmp(name, T("authType"))) {
		if ( !apmib_get( MIB_INIC_AUTH_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("operRate"))) {
		if ( !apmib_get( MIB_INIC_SUPPORTED_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("basicRate"))) {
		if ( !apmib_get( MIB_INIC_BASIC_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("preamble"))) {
		if ( !apmib_get( MIB_INIC_PREAMBLE_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("hiddenSSID"))) {
		if ( !apmib_get( MIB_INIC_HIDDEN_SSID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _INIC_MSSID_ //vance 2008.12.30
	else if ( !strcmp(name, T("hiddenSSID1"))) {
			if ( !apmib_get( MIB_INIC_HIDDEN_SSID_1, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
#endif
	// RexHua
	else if ( !strcmp(name, T("wlanTurbo"))) {
		if ( !apmib_get( MIB_INIC_TURBO, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("NfixTxRate"))) {
		if ( !apmib_get( MIB_INIC_N_FIX_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("NChanWidth"))) {
		if ( !apmib_get( MIB_INIC_N_CHANNEL_WIDTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanWmm"))) {
		if ( !apmib_get( MIB_INIC_WMM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanDFS"))) {
		if ( !apmib_get( MIB_INIC_DFS, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanBF"))) {
		if ( !apmib_get( MIB_INIC_BF, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#if defined(HOME_GATEWAY) || defined(_AP_WLAN_NOFORWARD_)
		//kyle
		else if ( !strcmp(name, T("noForward"))) {
		if ( !apmib_get( MIB_INIC_NOFORWARD, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

	else if ( !strcmp(name, T("wmFilterNum"))) {
		if ( !apmib_get( MIB_INIC_AC_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanDisabled"))){
		if ( !apmib_get( MIB_INIC_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAcNum")) ) {
		if ( !apmib_get( MIB_INIC_AC_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAcEnabled"))) {
		if ( !apmib_get( MIB_INIC_AC_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#ifdef _INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
	//SSID 2
	else if ( !strcmp(name, T("wlanAc2Num")) ) {
		if ( !apmib_get( MIB_INIC_AC2_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc2Enabled"))) {
		if ( !apmib_get( MIB_INIC_AC2_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	//SSID 3
	else if ( !strcmp(name, T("wlanAc3Num")) ) {
		if ( !apmib_get( MIB_INIC_AC3_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc3Enabled"))) {
		if ( !apmib_get( MIB_INIC_AC3_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	//SSID 4
	else if ( !strcmp(name, T("wlanAc4Num")) ) {
		if ( !apmib_get( MIB_INIC_AC4_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc4Enabled"))) {
		if ( !apmib_get( MIB_INIC_AC4_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#endif
	else if ( !strcmp(name, T("rateAdaptiveEnabled"))) {
		if ( !apmib_get( MIB_INIC_RATE_ADAPTIVE_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("netType"))) {
		if ( !apmib_get( MIB_INIC_NETWORK_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("iappDisabled"))) {
		if ( !apmib_get( MIB_INIC_IAPP_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("block"))) {
		if ( !apmib_get( MIB_INIC_BLOCK_RELAY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("autoMacClone"))) {
		if ( !apmib_get( MIB_AUTO_MAC_CLONE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("band"))) {
		if ( !apmib_get( MIB_INIC_BAND, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//-----------------------------------Erwin Add 28.08----------------------------------------------
	else if ( !strcmp(name, T("apMode"))) {
		if ( !apmib_get( MIB_INIC_AP_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("secMode"))) {
		if ( !apmib_get( MIB_INIC_SECURITY_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("clIpDisabled"))) {
		if ( !apmib_get( MIB_CLIENT_IP_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#if defined (HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
//**************************************** Time Zone ****************************************
	else if ( !strcmp(name, T("DayLightEnable"))) {
		if ( !apmib_get( MIB_DAYLIGHT_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
#if _SNTP_A_M_CTL_ //vance 2009.02.20
//**************************************** Time Zone ****************************************
	else if ( !strcmp(name, T("TimeServerEnable"))) {
		if ( !apmib_get( MIB_TIMESERVER_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
#ifdef HOME_GATEWAY
//*************************************** Remote Managemen ***************************************
	else if ( !strcmp(name, T("reMangEnable"))) {
		if ( !apmib_get( MIB_REMANG_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** UPnP Setting ***************************************
	else if ( !strcmp(name, T("upnpEnable"))) {
		if ( !apmib_get( MIB_UPNP_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//********************************** PrinterServer Setting **********************************
// Lance 2004.2.9
	else if ( !strcmp(name, T("PSEnable"))) {
		if ( !apmib_get( MIB_PS_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
// Lance 2004.2.16
	else if ( !strcmp(name, T("PS_IPPEnable"))) {
		if ( !apmib_get( MIB_PS_IPPENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("PS_LPREnable"))) {
		if ( !apmib_get( MIB_PS_LPRENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
// End - Lance
//*************************************** NAT Setting ***************************************
   else if ( !strcmp(name, T("natEnable"))) {
		if ( !apmib_get( MIB_NAT_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("fastNatEnable"))) {
		if ( !apmib_get( MIB_FAST_NAT_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** IGMP Setting ***************************************
	else if ( !strcmp(name, T("igmpEnable"))) {

#if defined(_IGMP_PROXY_)
		if ( !apmib_get( MIB_IGMP_PROXY_ENABLED, (void *)&val) )
			return -1;
#else
		val=0;
#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//********************* TTL Function add by Kyle 2008/03/07. *******************
#ifdef _WANTTL_
	else if ( !strcmp(name, T("ttlType"))) {
		if ( !apmib_get( MIB_TTL_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
//*************************************** Firewall Setting ***************************************
	else if ( !strcmp(name, T("FirewallEnable"))) {
		if ( !apmib_get( MIB_FIREWALL_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** Virtual Server***************************************
	else if ( !strcmp(name, T("virtSVEnabled"))) {
		if ( !apmib_get( MIB_VSER_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("vserNum")) ) {
		if ( !apmib_get( MIB_VSER_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//**************************************** DMZ ****************************************
	else if ( !strcmp(name, T("dmzNum")) ) {
		if ( !apmib_get( MIB_DMZ_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//**************************************** URL Blocking ****************************************
	else if ( !strcmp(name, T("URLBEnabled"))) {
		if ( !apmib_get( MIB_URLB_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("URLBNum")) ) {
		if ( !apmib_get( MIB_URLB_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//**************************************** DoS ****************************************
	else if ( !strcmp(name, T("podEnable")) ) {
		if ( !apmib_get( MIB_POD_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pingEnable")) ) {
		if ( !apmib_get( MIB_PING_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("scanEnable")) ) {
		if ( !apmib_get( MIB_SCAN_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("synEnable")) ) {
		if ( !apmib_get( MIB_SYN_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("podTime")) ) {
		if ( !apmib_get( MIB_POD_TIME, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("synTime")) ) {
		if ( !apmib_get( MIB_SYN_TIME, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ACPCEnabled"))) {
		if ( !apmib_get( MIB_ACPC_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ipDenyEnabled")) ) {
		if ( !apmib_get( MIB_IPDENY_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ACPCNum")) ) {
		if ( !apmib_get( MIB_ACPC_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//*************************************** DDNS Setting ***************************************
	else if ( !strcmp(name, T("ddnsEnable"))) {
		if ( !apmib_get( MIB_DDNS_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("dhisSelect"))) {
		if ( !apmib_get( MIB_DHIS_SELECT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
//----------------------------------------------------------------------------------------------
#ifdef WLAN_WPA
	else if ( !strcmp(name, T("encrypt"))) {
		if ( !apmib_get( MIB_INIC_SECURITY_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _INIC_MSSID_ //vance
	else if ( !strcmp(name, T("encrypt1"))) {
			if ( !apmib_get( MIB_INIC_SECURITY_MODE_1, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
#endif
	else if ( !strcmp(name, T("enable1X"))) {
		if ( !apmib_get( MIB_INIC_ENABLE_1X, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enableSuppNonWpa"))) {
		if ( !apmib_get( MIB_INIC_ENABLE_SUPP_NONWPA, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("suppNonWpa"))) {
		if ( !apmib_get( MIB_INIC_SUPP_NONWPA, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpaAuth"))) {
		if ( !apmib_get( MIB_INIC_WPA_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpaCipher"))) {
		int iTkip, iAes;
		if ( !apmib_get( MIB_INIC_WPA_CIPHER_SUITE, (void *)&iTkip) )
			return -1;
		if ( !apmib_get( MIB_INIC_WPA2_CIPHER_SUITE, (void *)&iAes) )
			return -1;
		if (iTkip==WPA_CIPHER_TKIP && iAes==0) val=1;
		else if (iTkip==0 && iAes==WPA_CIPHER_AES) val=2;
		else if (iTkip==WPA_CIPHER_TKIP && iAes==WPA_CIPHER_AES) val=3;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pskFormat"))) {
		if ( !apmib_get( MIB_INIC_WPA_PSK_FORMAT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("accountRsEnabled"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("accountRsUpdateEnabled"))) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_UPDATE_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enableMacAuth"))) {
		if ( !apmib_get( MIB_INIC_ENABLE_MAC_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("rsRetry")) ) {
		if ( !apmib_get( MIB_INIC_RS_RETRY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("accountRsRetry")) ) {
		if ( !apmib_get( MIB_INIC_ACCOUNT_RS_RETRY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

// Static DHCP Lease
	else if ( !strcmp(name, T("SDHCPEnabled"))) {
		if ( !apmib_get( MIB_STATIC_DHCP_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("SDHCPNum")) ) {
		if ( !apmib_get( MIB_STATIC_DHCP_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//-------------------

#ifdef UNIVERSAL_REPEATER
	else if ( !strcmp(name, T("repeaterEnabled"))) {
		id = MIB_INIC_REPEATER_ENABLED;
		if ( !apmib_get( id, (void *)&val) )
				return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

	else if ( !strcmp(name, T("apRouterSwitch"))) {
		if ( !apmib_get( MIB_AP_ROUTER_SWITCH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("txStream"))) {
		ejSetResult(eid, "2");
		return 0;
	}

	else if ( !strcmp(name, T("pppoepass"))) {
		ejSetResult(eid, "\"\"");
	return 0;
	}
	else if ( !strcmp(name, T("ipv6bridge"))) {
		ejSetResult(eid, "\"\"");
	return 0;
	}
	#ifdef _Unicorn_
	else if ( !strcmp(name, T("QosMode"))){
		if (!apmib_get( MIB_QOS_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
	return 0;
	}
	#endif
#ifdef _LEDSWITCH_
	else if ( !strcmp(name, T("ledStatus"))) {
		if ( !apmib_get( MIB_LED_CONTROL, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

#ifdef _SDMZ_
        else if ( !strcmp(name, T("DMZ_Mode"))){
                if (!apmib_get( MIB_DMZ_MODE, (void *)&val) )
                        return -1;
                sprintf(buffer, "%d", val);
                ejSetResult(eid, buffer);
        return 0;
        }
#endif
    //*************************************** Start Dual Wan Access***************************************
       else if ( !strcmp(name, T("isDuallAccessEnable"))) {
#ifdef _DUALL_WAN_ACCESS_
            if ( !apmib_get( MIB_DUALL_ACCESS_ENABLE, (void *)&val) )
                    return -1;
            sprintf(buffer, "%d", val);
            ejSetResult(eid, buffer);
       #else
            ejSetResult(eid, "\"\"");
#endif
       return 0;
       }
        else if ( !strcmp(name, T("duallAccessMode"))) {
#ifdef _DUALL_WAN_ACCESS_
         if ( !apmib_get( MIB_DUALL_ACCESS_MODE, (void *)&val) )
                 return -1;
         sprintf(buffer, "%d", val);
         ejSetResult(eid, buffer);
#else
         ejSetResult(eid, "\"\"");
#endif
      return 0;
    }
    //*************************************** End Dual Wan Access***************************************
#ifdef _IGMP_SNOOPING_ENABLED_
	else if ( !strcmp(name, T("igmpsnoopenabled"))) {
		if ( !apmib_get( MIB_IGMP_SNOOP_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
#ifdef _WEP_OPEN_SHARED_MODE_
        else if ( !strcmp(name, T("WEP_AuthMode"))){
                if (!apmib_get( MIB_WEP_OPEN_SHARED_MODE, (void *)&val) )
                        return -1;
                sprintf(buffer, "%d", val);
                ejSetResult(eid, buffer);
        return 0;
        }
#endif
#ifdef _TX_POWER_CONTROL_
	else if ( !strcmp(name, T("tx_power")) )
	{
		if ( !apmib_get( MIB_TX_POWER, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val) ;
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("txpower_sw")) ) 
	{
		if ( !apmib_get( MIB_HW_TXPOWER_SW, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val) ;
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
#ifdef _DISABLE_WIRELESS_AP_
	else if ( !strcmp(name, T("get_ap_status")))
	{
		if ( !apmib_get(MIB_DISABLE_WIRELESS_AP, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid,buffer);
		return 0;
	}
#endif

	else{
		ejSetResult(eid, "");
		return 0;
	}
		return -1;
}
