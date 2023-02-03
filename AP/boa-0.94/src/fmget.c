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

#define FW_VERSION	fwVersion

#ifdef _AUTO_CHAN_SELECT_
extern int ssid2scan_fail; //edx patrick. if 1: skip connect test
extern int ssid2scan_fail5g; //edx patrick. if 1: skip connect test
extern int wep_used;
#endif

extern int connectIF;

#ifdef _AUTO_FW_UPGRADE_
extern int isServerAlive;
extern char zip_url[128], fw_url[128],correct_md5[125],downloadfw_md5[125],fw_version[30],product_name[100];
#endif

extern char *fwVersion;	// defined in version.c
extern int First, Changed;
//extern int need_reboot;		//removed by Erwin

#ifdef HOME_GATEWAY
char staTbl[2][7][50];
char usbTbl[3][5][50];
#endif
///////////////////////////////////////////
#if WIRELESS_EXT <= 11
#ifndef SIOCDEVPRIVATE
#define SIOCDEVPRIVATE                              0x8BE0
#endif
#define SIOCIWFIRSTPRIV								SIOCDEVPRIVATE
#endif

#define RT_PRIV_IOCTL								(SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET							(SIOCIWFIRSTPRIV + 0x02)


// Ioctl value for WPS UPnP daemon and LLTD daemon must be identical in RT61 and RT2860
#define RT_OID_SYNC_RT61                            0x0D010750


// for consistency with RT61
#define RT_OID_WSC_QUERY_STATUS						((RT_OID_SYNC_RT61 + 0x01) & 0xffff)
#define RT_OID_WSC_PIN_CODE							((RT_OID_SYNC_RT61 + 0x02) & 0xffff)
#define RT_OID_WSC_UUID								((RT_OID_SYNC_RT61 + 0x03) & 0xffff)
#define RT_OID_WSC_SET_SELECTED_REGISTRAR			((RT_OID_SYNC_RT61 + 0x04) & 0xffff)
#define RT_OID_WSC_EAPMSG							((RT_OID_SYNC_RT61 + 0x05) & 0xffff)
#define RT_OID_WSC_MANUFACTURER						((RT_OID_SYNC_RT61 + 0x06) & 0xffff)
#define RT_OID_WSC_MODEL_NAME						((RT_OID_SYNC_RT61 + 0x07) & 0xffff)
#define RT_OID_WSC_MODEL_NO							((RT_OID_SYNC_RT61 + 0x08) & 0xffff)
#define RT_OID_WSC_SERIAL_NO						((RT_OID_SYNC_RT61 + 0x09) & 0xffff)
#define RT_OID_WSC_MAC_ADDRESS						((RT_OID_SYNC_RT61 + 0x10) & 0xffff)

int getMyName(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name;
	struct in_addr	intaddr;
	struct sockaddr hwaddr;
	unsigned char *pMacAddr;
	unsigned long sec, mn, hr, day, longVal;
	unsigned char buffer[100];
	int intVal, flag=1;
 	struct user_net_device_stats stats;
	DHCP_T dhcp;
	bss_info bss;
	char *ptr;
#ifdef HOME_GATEWAY
	unsigned char tmp[100];
	char *ptr1;
	OPMODE_T opmode=-1;
	char_t *iface;
#endif
   	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
   		websError(wp, 400, T("Insufficient args\n"));
   		return -1;
   	}

	return websWrite(wp, T("RexHua"));
}

/***********************      Added By Yeahone!      ***********************/

int showSomeString(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *buffer= "<h1><script type='text/javascript'>document.write(showText(textString1))</script></h1>";
	websWrite(wp, T("%s"), buffer);
}

int showWebsPasswd(int eid, webs_t wp, int argc, char_t **argv)
{
	char buffer[100];

	apmib_get(MIB_USER_NAME, (void *)buffer);
	websWrite(wp, T("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"));
	websWrite(wp, T("The User Name is : "));
	websWrite(wp, T("%s<br>"), buffer);
	apmib_get(MIB_USER_PASSWORD, (void *)buffer);
	websWrite(wp, T("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"));
	websWrite(wp, T("The User Password is : "));
	websWrite(wp, T("%s<br>"), buffer);
	apmib_get(MIB_SUPER_NAME, (void *)buffer);
	websWrite(wp, T("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"));
	websWrite(wp, T("The Super Name is : "));
	websWrite(wp, T("%s<br>"), buffer);
	apmib_get(MIB_SUPER_PASSWORD, (void *)buffer);
	websWrite(wp, T("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"));
	websWrite(wp, T("The Super Password is : "));
	websWrite(wp, T("%s<br>"), buffer);
}

/***********************      Added By Yeahone!      ***********************/
#if defined(_GENERNAL_RANDOMNUMBER_)
int generateRandomNum(void)
{
        int Num;
        srand(time(NULL));
        Num = rand();
        return Num;
}
#endif

#ifdef _MSSID_
/*
void wep_key_show_all(webs_t wp,int handler, ...)
{
	int val=0,i=0,ii=0;
	int arg1;
	char buffer[100]={0};
	char tempkey[100]={0};

	apmib_get( handler, (void *)&val);
	va_list key_list;
	va_start(key_list,handler);
	while(arg1=va_arg(key_list,int))
	{
		memset(buffer, 0, sizeof(buffer));
		memset(tempkey,0, sizeof(tempkey));
		apmib_get(arg1, (void *)buffer);

		//ascii
		if (val == 0)
		{
			#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
				CharFilter2(buffer);
			#endif
			for(i=0;i<sizeof(buffer);i++)
			{
				sprintf(tempkey,"%s%c",tempkey,buffer[i]);
				if(buffer[i] == '\0' ) break;
			}
		}
		else // hex
		{
			if (ii<4)     for(i=0;i< 5;i++) sprintf(tempkey,"%s%02x",tempkey,buffer[i]);
			else if(ii<8)	for(i=0;i<13;i++) sprintf(tempkey,"%s%02x",tempkey,buffer[i]);
			ii++;
		}
		websWrite(wp, T("\"%s\","), tempkey);
	}
	va_end(key_list);

	return;
}
*/
#endif

int getInfo(int eid, webs_t wp, int argc, char_t **argv)
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

	intVal = oem_getInfo(eid, wp, name);
	if (intVal != -1)
		return intVal;

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

#ifdef _TIME_COPY_
   	else if ( !strcmp(name, T("year"))) {

   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_year+ 1900));

   			return websWrite(wp, T("%s"), buffer);

   		}
   		else if ( !strcmp(name, T("month"))) {
   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_mon+1));
   			return websWrite(wp, T("%s"), buffer);
   		}
   		else if ( !strcmp(name, T("day"))) {
   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_mday));
   			return websWrite(wp, T("%s"), buffer);
   		}
   		else if ( !strcmp(name, T("hour"))) {
   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_hour));
   			return websWrite(wp, T("%s"), buffer);
   		}
   		else if ( !strcmp(name, T("minute"))) {
   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_min));
   			return websWrite(wp, T("%s"), buffer);
   		}
   		else if ( !strcmp(name, T("second"))) {
   			time(&current_time);
   			tm_time = localtime(&current_time);
   			sprintf(buffer,"%d", (tm_time->tm_sec));
   			return websWrite(wp, T("%s"), buffer);
   		}
#endif
	else if ( !strcmp(name, T("DownLoadURL"))) {
		#ifdef _DNS_PROXY_
		apmib_get( MIB_DNS_PROXY_URL,  (void *)buffer) ;
		//CharFilter5(buffer);
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#else
		websWrite(wp, T("new Array(\"\")"));
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("autochannel"))) {  //EDX patrick, if select auto chan, status show driver mib value
		FILE *fp1;
		#ifndef __TARGET_BOARD__
		system("echo \"11\" > /tmp/AutoChenGband");
		#else
		system(" iwconfig ra0 |  grep -n 'Channel=' | cut -f 2 -d '=' | cut -f 1 -d ' '  > /tmp/AutoChenGband");
		#endif
		if ((fp1 = fopen("/tmp/AutoChenGband","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			int intChan;
			if ( !apmib_get( MIB_WLAN_CHAN_NUM,  (void *)&intChan) ) 
    		return -1;
			sprintf(buffer, "%d", intChan );
			return websWrite(wp, buffer);
		}
  }
	else if ( !strcmp(name, T("ra0IF"))) {
		#ifdef __TARGET_BOARD__
		FILE *fp;
		char cmdbuffer[100];
		int wizmode, brigerMode=3;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);

		if(wizmode == brigerMode)
			snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh currnetIFstatus apcli0");
		else
			snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh currnetIFstatus ra0");

		if ((fp = popen(cmdbuffer, "r")) != NULL) {
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}				
		#else
		sprintf(buffer, "down");
		return websWrite(wp, buffer);
		#endif
	}
	else if ( !strcmp(name, T("rai0IF"))) {
		#ifdef __TARGET_BOARD__
		FILE *fp;
		char cmdbuffer[100];
		int wizmode, brigerMode=3;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);

		if(wizmode == brigerMode)		
			snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh currnetIFstatus apclii0");
		else
			snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh currnetIFstatus rai0");

		if ((fp = popen(cmdbuffer, "r")) != NULL) {
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}				
		#else
		sprintf(buffer, "down");
		return websWrite(wp, buffer);
		#endif
	}
	else if ( !strcmp(name, T("connectStatus2G"))) {
		#ifdef __TARGET_BOARD__
		int isDisable, useCrossBand=0;
		FILE *fp;
		char cmdbuffer[100];
		apmib_get( MIB_WLAN_DISABLED,  (void *)&isDisable);
		#if defined(_CROSSBAND_)
		apmib_get( MIB_CROSSBAND_ENABLE,  (void *)&useCrossBand);
		#endif

		if(isDisable){
			sprintf(buffer, "disable");
			return websWrite(wp, buffer);
		}
		else{
			if(useCrossBand)
				snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apclii0");
			else
				snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apcli0");
			if ((fp = popen(cmdbuffer, "r")) != NULL) {
				fgets(buffer, sizeof(buffer), fp);
				pclose(fp);
				if (buffer[strlen(buffer)-1]== '\n')
					buffer[strlen(buffer)-1] = '\0';
				return websWrite(wp, buffer);
			}				
		}
		#else
		sprintf(buffer, "44");
		return websWrite(wp, buffer);
		#endif
	}
	else if ( !strcmp(name, T("connectStatus5G"))) {
		#ifdef __TARGET_BOARD__
		int isDisable, useCrossBand=0;
		FILE *fp;
		char cmdbuffer[100];
		apmib_get( MIB_INIC_DISABLED,  (void *)&isDisable);
		#if defined(_CROSSBAND_)
		apmib_get( MIB_CROSSBAND_ENABLE,  (void *)&useCrossBand);
		#endif

		if(isDisable){
			sprintf(buffer, "disable");
			return websWrite(wp, buffer);
		}
		else{
			if(useCrossBand)
				snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apcli0");
			else
				snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apclii0");
			if ((fp = popen(cmdbuffer, "r")) != NULL) {
				fgets(buffer, sizeof(buffer), fp);
				pclose(fp);
				if (buffer[strlen(buffer)-1]== '\n')
					buffer[strlen(buffer)-1] = '\0';
				return websWrite(wp, buffer);
			}				
		}
		#else
		sprintf(buffer, "55");
		return websWrite(wp, buffer);
		#endif
	}
	else if ( !strcmp(name, T("get_apcli0_rssi"))) {  
		#ifdef __TARGET_BOARD__
		FILE *fp;
		char cmdbuffer[100];
		snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apcli0");
		if ((fp = popen(cmdbuffer, "r")) != NULL) {
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}				
		#else
		int intChan=100;
		sprintf(buffer, "%d", intChan );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("get_apclii0_rssi"))) {  
		#ifdef __TARGET_BOARD__
		FILE *fp;
		char cmdbuffer[100];
		snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetUplinkRssistrength apclii0");
		if ((fp = popen(cmdbuffer, "r")) != NULL) {
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}				
		#else
		int intChan=100;
		sprintf(buffer, "%d", intChan );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("cableStatus"))) {  
		FILE *fp;
		char cmdbuffer[100];
		#ifdef __TARGET_BOARD__
	
		#ifdef _EW7478AC_
		snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetphyStat 3 1");
		#else
		snprintf(cmdbuffer, sizeof(cmdbuffer), "/bin/scriptlib_util.sh GetphyStat 4 1");
		#endif
		if ((fp = popen(cmdbuffer, "r")) != NULL) {
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			printf("cableStatus:%s\n", buffer);
			return websWrite(wp, buffer);
		}				
		else{
			sprintf(buffer, "0");
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "connect");
		return websWrite(wp, buffer);
		#endif

	}
	else if ( !strcmp(name, T("get_connect_distinguish"))) {
		FILE *fp1;
		int connect;
		if ((fp1 = fopen("/tmp/connect_distinguish","r"))!=NULL) { 
			fgets(buffer, 30, fp1);
			connect = atoi(buffer);
			fclose(fp1);
			return websWrite(wp, T("%d"), connect);
		}
	}
	else if ( !strcmp(name, T("fw_state"))) {  
		FILE *fp;
		printf(">> get fw_state\n");
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/Invalidfw", "r")) != NULL ){
			system("rm -f /tmp/Invalidfw");
			sprintf( buffer, "%s" ,"0");
			fclose(fp);
		}
		else if( (fp=fopen("/tmp/BootComplete", "r")) != NULL ){
			system("rm -f /tmp/BootComplete");
			sprintf( buffer, "%s" ,"1");
			fclose(fp);
		}
		else
			sprintf( buffer, "%s" ,"2");
		return websWrite(wp, T("%s"), buffer);
		#else
		sprintf( buffer, "%s" ,"1");
		return websWrite(wp, T("%s"), buffer);
		#endif
	}
	else if ( !strcmp(name, T("Config_state"))) {  
		FILE *fp;
		printf(">> get fw_state\n");
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/InvalidConfig", "r")) != NULL ){
			system("rm -f /tmp/InvalidConfig");
			sprintf( buffer, "%s" ,"0");
			fclose(fp);
		}
		else if( (fp=fopen("/tmp/BootComplete", "r")) != NULL ){
			system("rm -f /tmp/BootComplete");
			sprintf( buffer, "%s" ,"1");
			fclose(fp);
		}
		else
			sprintf( buffer, "%s" ,"2");
		return websWrite(wp, T("%s"), buffer);
		#else
		sprintf( buffer, "%s" ,"1");
		return websWrite(wp, T("%s"), buffer);
		#endif
	}	
	else if ( !strcmp(name, T("wps_key_val")) )
	{
		int val2=0, val3=0, wep;
		if ( !apmib_get( MIB_SECURITY_MODE, (void *)&intVal) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WEP,  (void *)&wep) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val2) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WPA_PSK_FORMAT, (void *)&val3) )
			return -1;

 		memset(buffer, 0, sizeof(buffer));

		if ( intVal == 1 )
		{
			if( wep == 1){
				apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
					return 0;
				}
			}
			else{
				apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
					return 0;
				}
			}
		}
		else if( intVal == 2)
			apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);
		else if ( intVal == 3 )
			apmib_get(MIB_WLAN_WDS_WPA_PSK,  (void *)buffer);


		CharFilter6(buffer);
		

		websWrite(wp, T("%s"), buffer);

		return 0;
	}
	else if ( !strcmp(name, T("rep_wps_key_val")) )
	{
		int val2=0, val3=0, wep;
		if ( !apmib_get( MIB_SECURITY_MODE, (void *)&intVal) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WEP,  (void *)&wep) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val2) )
			return -1;

		if ( !apmib_get( MIB_WLAN_WPA_PSK_FORMAT, (void *)&val3) )
			return -1;

 		memset(buffer, 0, sizeof(buffer));

		if ( intVal == 1 )
		{
			if( wep == 1){
				apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
					return 0;
				}
			}
			else{
				apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);
				if(val2){
					websWrite(wp, T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
					return 0;
				}
			}
		}
		else if( intVal == 2 || intVal == 3)
			apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);

		CharFilter6(buffer);
		websWrite(wp, T("%s"), buffer);

		return 0;
	}
	else if ( !strcmp(name, T("wpaEncrypt"))) { //EDX Robert 2014_02_20 Add Support All Security
			//1.WPA=> TKIP or AES , 2.WPA2=> TKIP or AES 
		int val;  
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&val);  
			websWrite(wp, T("new Array(\"%d\","), val); 
		apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&val);
			websWrite(wp, T("\"%d\")"), val);
		return 0;
	}
#ifdef _IQv2_
	else if ( !strcmp(name, T("wepasciistring"))) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_WEPASCII_STRING,  (void *)buffer) )
			return -1;
		CharFilter6(buffer);
   	return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("wepasciistring"))) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_WEPASCII_STRING_5G,  (void *)buffer) )
			return -1;
		CharFilter6(buffer);
   	return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("wizmode_value"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/wizmode_value", "r")) != NULL )
		    fgets(tmp,2,fp);
		fclose(fp);
		return websWrite(wp, T("%c"), tmp[0]);
		#else
#ifdef _EW7478WAP_
		return websWrite(wp, T("%s"), "2");
#else
		return websWrite(wp, T("%s"), "1");
#endif
		#endif
	}
	else if ( !strcmp(name, T("wiz_gwManu"))) {  // UI select iQ Setup wizard or Configure manually
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/wiz_gwManu", "r")) != NULL )
		    fgets(tmp,2,fp);
		fclose(fp);
		return websWrite(wp, T("%c"), tmp[0]);
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("afterTest"))) {  // for adhcp_and_pppoe.asp
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/after_delay_ping", "r")) != NULL ){
			fclose(fp);
			return websWrite(wp, T("%s"), "1");
		}
		else{
			return websWrite(wp, T("%s"), "0");
		}
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("pppFail"))) {  // for adhcp_and_pppoe.asp
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/pppFail", "r")) != NULL ){
			fclose(fp);
			return websWrite(wp, T("%s"), "1");
		}
		else{
			return websWrite(wp, T("%s"), "0");
		}
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("Ap_enable5g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/Ap_enable5g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("Ap_enable2g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/Ap_enable2g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");	
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("wiz_wispManu"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/wiz_wispManu", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("WISP_enable5g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/WISP_enable5g", "r")) != NULL){
		  fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("WISP_enable2g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/WISP_enable2g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("wispConnect"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/wispConnect", "r")) != NULL){
			fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "0");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("pingStatus"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/pingStatus", "r")) != NULL){
			fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "2");
		#else
		return websWrite(wp, T("%s"), "2");
		#endif
	}
	else if ( !strcmp(name, T("conType"))) {
		FILE *fp;
		char tmp[2];
		if( (fp=fopen("/tmp/conType", "r")) != NULL){
			fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "0");
	}
	else if ( !strcmp(name, T("wifi_changed"))) {
		FILE *fp1;
		int intChan=0;

		if ((fp1 = fopen("/tmp/wifi_changed","r"))!=NULL) {
			intChan=1;
			//fgets(buffer, 30, fp1);
			sprintf(buffer, "%d", intChan );
			fclose(fp1);
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%d", intChan );
			return websWrite(wp, buffer);
		}
	}
	 else if ( !strcmp(name, T("ConnectKey"))) {
		FILE *fp;
		int i;
		unsigned int output_intValue;
		char output_charValue[4]="0x";
		char tmpBuf[100]={0};
		char PskBuf[150];

		if ((fp = fopen("/tmp/ConnectTestKey","r"))!=NULL) { 
			fgets(PskBuf, sizeof(PskBuf), fp);
			fclose(fp);

			for(i=0;i<(strlen(PskBuf)-1);i=i+2) // EDX, Ken 2015_09_010 add, Avoid buffer is "2.4G password;systemcode".
			{
				output_charValue[2]=PskBuf[i];
				output_charValue[3]=PskBuf[i+1];
				output_charValue[4]='\0';
				sscanf(output_charValue,"%x",&output_intValue); // Hexadecimal to Decimal
				sprintf(tmpBuf, "%s%c",tmpBuf,output_intValue);

			}
		}
		else {
			tmpBuf[0]='\0';
		}

		CharFilter5(tmpBuf);
		websWrite(wp, T("new Array(\"%s\")"), tmpBuf);
		return 0;
	}
	else if ( !strcmp(name, T("ConnectKey5g"))) {
		FILE *fp;
		int i;
		unsigned int output_intValue;
		char output_charValue[4]="0x";
		char tmpBuf[100]={0};
		char PskBuf[150];
		if ((fp = fopen("/tmp/ConnectTestKey5g","r"))!=NULL) { 
			fgets(PskBuf, sizeof(PskBuf), fp);
			fclose(fp);

			for(i=0;i<(strlen(PskBuf)-1);i=i+2) // EDX, Ken 2015_09_010 add, Avoid buffer is "5G password;systemcode".
			{
				output_charValue[2]=PskBuf[i];
				output_charValue[3]=PskBuf[i+1];
				output_charValue[4]='\0';
				sscanf(output_charValue,"%x",&output_intValue); // Hexadecimal to Decimal
				sprintf(tmpBuf, "%s%c",tmpBuf,output_intValue);
			}
		}
		else {
			tmpBuf[0]='\0';
		}
		CharFilter5(tmpBuf);
		websWrite(wp, T("new Array(\"%s\")"), tmpBuf);
		return 0;
	}
	else if ( !strcmp(name, T("rep_enable5g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/rep_enable5g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else	
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("rep_enable2g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/rep_enable2g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("rep_enableCrossBand"))){
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/EnableCrossBand", "r")) != NULL ){
			fgets(buffer, 70, fp);
			fclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
			return websWrite(wp, T("%s"), "0");
		#else
		return websWrite(wp, T("%s"), "0");
		#endif
	}
	else if ( !strcmp(name, T("force_enable2g5g_ap"))){
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/force_enable2g5g_ap", "r")) != NULL ){
			fgets(buffer, 70, fp);
			fclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
			return websWrite(wp, T("%s"), "0");
		#else
		return websWrite(wp, T("%s"), "0");
		#endif
	}
	else if ( !strcmp(name, T("APcl_enable5g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/APcl_enable5g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("APcl_enable2g"))) {
		FILE *fp;
		char tmp[2];
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/APcl_enable2g", "r")) != NULL){
	    fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "1");
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("SKipAddRoute"))) {  //EDX patrick, 2:2.4G connection Test 5:5G connection Test
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/SKipAddRoute","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apcli0IP"))) {
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0IP","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0IP"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0IP","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
	}
//=======================_HIGHSPEED_WIFI_REPEATER_============================

	else if ( !strcmp(name, T("apcli0_ROUTER"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0_ROUTER","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0_ROUTER"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0_ROUTER","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }	else if ( !strcmp(name, T("apcli0_DNS"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0_DNS","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0_DNS"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0_DNS","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apcli0_SUBNET"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0_SUBNET","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0_SUBNET"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0_SUBNET","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apcli0_DHCPstart"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0_DHCPstart","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0_DHCPstart"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0_DHCPstart","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apcli0_DHCPend"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apcli0_DHCPend","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
	else if ( !strcmp(name, T("apclii0_DHCPend"))) { 
		FILE *fp1;
		#ifdef __TARGET_BOARD__
		if ((fp1 = fopen("/tmp/apclii0_DHCPend","r"))!=NULL) {
			fgets(buffer, 30, fp1);
			fclose(fp1);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else
		{
			sprintf(buffer, "%s", "" );
			return websWrite(wp, buffer);
		}
		#else
		sprintf(buffer, "%s", "" );
		return websWrite(wp, buffer);
		#endif
  }
//================================_HIGHSPEED_WIFI_REPEATER_ end=================================




	else if ( !strcmp(name, T("wlmac_last6"))) {
		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02X%02X%02X"), buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("5Gwlmac_last6"))) {
		if ( !apmib_get( MIB_HW_INIC_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02X%02X%02X"), buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("Gssidtbl"))) {
		apmib_get(MIB_WLAN_SSID,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pskValueAll"))) {
		#ifdef _MSSID_
		apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_1,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);

		apmib_get(MIB_WLAN_WPA_PSK_2,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_3,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_4,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_5,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_6,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_7,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\")"), buffer);
		#else
		apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#endif
   		return 0;
	}
#endif
	else if ( !strcmp(name, T("userName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_USER_NAME,  (void *)buffer) )
			return -1;

		ptr1 = buffer;
		tmp[0] = '\0';

		while ( flag ) {
			if ((ptr = strchr(ptr1, '#')) != NULL) {
				*(ptr-1) ='\0';
				ptr++;
				strcat( tmp, ptr1);
				strcat( tmp, "#");
				ptr1 = ptr;
			}
			else {
				strcat( tmp, ptr1);
				flag = 0;
			}
		}
		return websWrite(wp, T("%s"), tmp);
	}

	else if ( !strcmp(name, T("userPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_USER_PASSWORD,  (void *)buffer) )
			return -1;

		ptr1 = buffer;
		tmp[0] = '\0';

		while ( flag ) {
			if ((ptr = strchr(ptr1, '#')) != NULL) {
				*(ptr-1) ='\0';
				ptr++;
				strcat( tmp, ptr1);
				strcat( tmp, "#");
				ptr1 = ptr;
			}
			else {
				strcat( tmp, ptr1);
				flag = 0;
			}
		}
		return websWrite(wp, T("%s"), tmp);
	}

#ifdef _DHCP_SERVER_WITH_GW_DNS_
	else if ( !strcmp(name, T("dhcpGW"))) {
		if ( !apmib_get( MIB_DHCP_SERVER_GW, (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"),inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("dhcpDNS1"))) {
		if ( !apmib_get( MIB_DHCP_SERVER_DNS1, (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"),inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("dhcpDNS2"))) {
		if ( !apmib_get( MIB_DHCP_SERVER_DNS2, (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"),inet_ntoa(intaddr));
	}
#endif
   	else if ( !strcmp(name, T("clientnum"))) {
		apmib_get( MIB_WLAN_DISABLED, (void *)&intVal);
		if (intVal == 1)	// disable
			intVal = 0;
		else {
			WLAN_STA_INFO_T *pInfo;
			char buff[WEBS_BUFSIZE] ;
			getWlStaInfo(WLAN_IF,  (WLAN_STA_INFO_Tp)buff );
			pInfo = (WLAN_STA_INFO_T *)buff;
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
#if defined(_MSSID_)
   	else if ( !strcmp(name, T("clientnum1"))) {
		apmib_get( MIB_WLAN_DISABLED, (void *)&intVal);
		if (intVal == 1)	// disable
			intVal = 0;
		else {
			WLAN_STA_INFO_T *pInfo;
			char buff[WEBS_BUFSIZE] ;
			getWlStaInfo(WLAN_IF,  (WLAN_STA_INFO_Tp)buff );
			pInfo = (WLAN_STA_INFO_T *)buff;
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
		apmib_get( MIB_WLAN_DISABLED, (void *)&intVal);
		if (intVal == 1)	// disable
			intVal = 0;
		else {
			WLAN_STA_INFO_T *pInfo;
			char buff[WEBS_BUFSIZE] ;
			getWlStaInfo(WLAN_IF,  (WLAN_STA_INFO_Tp)buff );
			pInfo = (WLAN_STA_INFO_T *)buff;
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
		apmib_get( MIB_WLAN_DISABLED, (void *)&intVal);
		if (intVal == 1)	// disable
			intVal = 0;
		else {
			WLAN_STA_INFO_T *pInfo;
			char buff[WEBS_BUFSIZE] ;
			getWlStaInfo(WLAN_IF,  (WLAN_STA_INFO_Tp)buff );
			pInfo = (WLAN_STA_INFO_T *)buff;
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
#endif
#ifdef _WATCH_DOG_
else if ( !strcmp(name, T("watchdog_interval"))) {
		if ( !apmib_get( MIB_WATCH_DOG_INTERVAL,  (void *)&intVal) )	return -1;
		return websWrite(wp, T("%d"), intVal);
}
else if ( !strcmp(name, T("watchdog_host"))) {
		if ( !apmib_get( MIB_WATCH_DOG_HOST,  (void *)&intaddr) )	return -1;
		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
}

#endif
//************************************ Repeater Status *******************Wise*****************
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
   			return websWrite(wp, "1.02");
		}
	}
	else if ( !strcmp(name, T("bootVersion"))) {
		FILE *fp;
		int fd, seek_start, offset;
		char seek_buffer[512], isFound = 0;

		if ((fp = fopen("/tmp/boot_version","r"))!=NULL) {
			fgets(buffer, 30, fp);
				if(buffer[strlen(buffer)-1]== '\n' )
					buffer[strlen(buffer)-1] ='\0';
			fclose(fp);
		}else {

			fd = open (BOOT_DEVICE_NAME, O_RDONLY);
			if (fd < 0){
				sprintf (buffer, "3.5.0.0.0");

			}else{
				// search 1b000 ~ 1d000, change it if need!
				lseek (fd, BOOT_SEEK_START, SEEK_SET);

				seek_start = 0;
				while ((seek_start < BOOT_SEEK_LEN) && (isFound == 0)){
					read (fd, &seek_buffer, sizeof(seek_buffer));
					seek_start += sizeof(seek_buffer);

					for (offset = 0; offset < 512; offset += 4){
						if ( !memcmp (&seek_buffer[offset], "UBoo", 4)){
							seek_start -= 512 - offset;

							lseek (fd, BOOT_SEEK_START + seek_start , SEEK_SET);
							read (fd, &seek_buffer, sizeof(seek_buffer));
							isFound = 1;
							break;
						}
					}
				}

				if (isFound == 0){
					sprintf (buffer, "echo 3.5.0.0.0 > /tmp/boot_version");
					system (buffer);
					sprintf (buffer, "3.5.0.0");
				}else{
					sprintf (buffer, "echo %s > /tmp/boot_version", seek_buffer + strlen (BOOT_STRING));
					system (buffer);
					strncpy (buffer, seek_buffer + strlen (BOOT_STRING), sizeof(buffer));
				}
				close (fd);
			}
		}
   		return websWrite(wp, buffer);
	}

	else if ( !strcmp(name, T("wlMacAddr"))) {
		FILE *fp;
// { Rex For Ralink
		sprintf (buffer, "ifconfig %s | grep HWaddr > /var/wlMactmp.tmp", WLAN_IF);
		system (buffer);
// } End
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
	else if ( !strcmp(name, T("tx1Gain"))) {
		FILE *fp;
		char buf[256];
		sprintf (buffer, "/bin/rftest.sh %s BAND gband", WLAN_IF);
		system (buffer);
		sprintf (buffer, "/bin/rftest.sh %s READTX1", WLAN_IF);
		system (buffer);

		sprintf (buf, "");
		if ((fp = fopen ("/var/rftest", "r")) != NULL){
			fgets (buf, 256, fp);
                                fgets(buf, 200, fp);
				fclose (fp);
				
			return websWrite (wp, T("%s"), buf);
		}else{
			return websWrite (wp, T("%s"), "error");
		}
        }
	else if ( !strcmp(name, T("tx2Gain"))) {
		FILE *fp;
		char buf[256];
		system("echo gband > /tmp/rfband");
		sprintf (buffer, "/bin/rftest.sh %s READTX2", WLAN_IF);
		system (buffer);

		sprintf (buf, "");
		if ((fp = fopen ("/var/rftest", "r")) != NULL){
			fgets (buf, 256, fp);
                                fgets(buf, 200, fp);
				fclose (fp);
				
			return websWrite (wp, T("%s"), buf);
		}else{
			return websWrite (wp, T("%s"), "error");
		}
        }
	else if ( !strcmp(name, T("e2pDE"))) {
		FILE *fp;
		char buf[100];
		
		sprintf (buffer, "iwpriv %s e2p DE |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", WLAN_IF);
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

		sprintf (buffer, "iwpriv %s e2p E2 |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", WLAN_IF);
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

		sprintf (buffer, "iwpriv %s e2p EA |grep :0x |cut -f 2 -d :| cut -b 3-6 > /var/rftest", WLAN_IF);
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
#ifdef _Edimax_
	else if ( !strcmp(name, T("getnetName"))) 
	{
		FILE *fp;
		char buf[100];
		#ifdef __TARGET_BOARD__
		if ((fp = fopen("/var/netName.var","r"))!=NULL) {
		#else
		if ((fp = fopen("netName.var","r"))!=NULL) {
		#endif

			while (fgets(buf, 100, fp) > 0){
				websWrite(wp, T("%s"), buf);
			}

			fclose(fp);

			return websWrite(wp,T(""),buf);
		}else {
   			return websWrite(wp,T(""),"");
		}
	}
#endif
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
	else if ( !strcmp(name, T("getDefined"))) {
				FILE *fp;
		char buf[100];
		char buf2[100];
		char *buf3;
		int i;

		#ifdef __TARGET_BOARD__
			if ((fp = fopen("/web/FUNCTION_SCRIPT","r"))!=NULL) {
		#else
			if ((fp = fopen("../../../define/FUNCTION_SCRIPT","r"))!=NULL) {
		#endif
				while (fgets(buf, 100, fp) > 0)
				{
					strcpy(buf2, "var ");
					strcat(buf2,strtok (buf,"="));
					buf3=strtok (NULL,"\n");

					if(buf3[0]=='\"')	strcat(buf2,"=");
					else			strcat(buf2,"=\"");

					strcat(buf2,buf3);

					if(buf3[strlen(buf3)-1]=='\"')	strcat(buf2,";");
					else				strcat(buf2,"\";");
//					strcat(buf2,"=\"");
//					strcat(buf2,strtok (NULL,"\n"));
//					strcat(buf2,"\";");
					websWrite(wp, T("%s\n"), buf2);
				}

			//readsize=fread(buf,1,15000, fp);
			//printf("readsize=%d,buffer= %s",readsize,buf);

				fclose(fp);
   			return websWrite(wp,T(""),buf);
		}else {
   			return websWrite(wp, T("%s"), "Null");
		}
	}


//*****************Function Defined END *************************************************


//*****************Country Code *************************************************

  else if ( !strcmp(name, T("getCountryCode"))) {
  	#ifdef _DFS_
			if ( !apmib_get( MIB_HW_COUNTRY_CODE,  (void *)buffer) )
				return websWrite(wp, T("%s"),  "\"\"");
			return websWrite(wp, T("%s"), buffer);
		#else
			return websWrite(wp, T("%s"),  "\"\"");
		#endif
	}


//************************************ ddns status *********************************
	else if ( !strcmp(name, T("ddnsStatus"))) {

		FILE *fp;
		char buf[100];
		int val=0;
		int val2=0;
		if(!apmib_get(MIB_DDNS_ENABLE, (void *)&val2))
			 val2=0;

		if ( val2 != 0 && ((fp = fopen("/var/ddnsstatus.var","r"))!=NULL)) {
			if ( !apmib_get( MIB_DDNS_STATUS, (void *)&val) )
				val=0;
			printf("debug val=%d",val);
				if (val==1){
					sleep(11);
					val=0;
					apmib_set(MIB_DDNS_STATUS, (void *)&val);
					apmib_update(CURRENT_SETTING);

				}
			fgets(buf, 100, fp);
			fclose(fp);



   			return websWrite(wp,T("%s"),buf);
		}else {
   			return websWrite(wp, T("%s"), "unConnection. ");
		}
	}

//************************************ station of wireless *************************************
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
	else if ( !strcmp(name, T("name")) ) {
		buffer[0]='\0';
		#ifdef HOME_GATEWAY
			#if defined(_BR6225N_) && defined(_Customer_) && defined(__TARGET_BOARD__)//大中華6225要依照flash hw setting變更model name (這個會顯示在status頁面)
				apmib_get(MIB_HW_MODEL_NAME,  (void *)buffer);
			#elif defined(_PRODUCT_NAME_)
				sprintf(buffer,"%s",_PRODUCT_NAME_);
			#else
				ProductName_is_not_defined_Please_see_Set_App_Defined
			#endif
			//#else
			//		if ( !apmib_get( MIB_ALIAS_NAME,  (void *)buffer) )
			//			return -1;
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("model")) ) {
		buffer[0]='\0';
		sprintf(buffer,"%s",_MODEL_);
		return websWrite(wp, T("%s"), buffer);
	}
 	else if ( !strcmp(name, T("ssid"))) {

		if ( !apmib_get( MIB_WLAN_SSID,  (void *)buffer) )
			return -1;
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
		return websWrite(wp, buffer);
	}
#ifdef _DISCONMSG_	
 	else if ( !strcmp(name, T("inicssid"))) {

		if ( !apmib_get( MIB_INIC_SSID,  (void *)buffer) )
			return -1;
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
		return websWrite(wp, buffer);
	}
#endif	
#ifdef _DALGFTP_
	else if ( !strcmp(name, T("algPorts")) ) {

		if ( !apmib_get( MIB_ALG_FTP_PORTS, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal);
		return websWrite(wp, buffer);


	}

#endif


	else if ( !strcmp(name, T("vlanidAll")) ) {
		int val;
		#if defined(_ENVLAN_) && defined(_MSSID_)
		apmib_get( MIB_WLAN_VLANID_1, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_VLANID_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		websWrite(wp, T("\"%s\""), "");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("txrateAll")) ) {
		int val;
		#ifdef _MSSID_
		apmib_get( MIB_WLAN_FIX_RATE, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_FIX_RATE_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_WLAN_FIX_RATE, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("WmmCapableAll")) ) {
		int val;
		#ifdef _MSSID_
		apmib_get( MIB_WLAN_WMM, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WMM_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_WLAN_WMM, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("hiddenSSIDAll")) ) {
		int val;
		#ifdef _MSSID_
		apmib_get( MIB_WLAN_HIDDEN_SSID, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_HIDDEN_SSID_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_WLAN_HIDDEN_SSID, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("ssidMirrorAll")) ) {
		int val;
		#ifdef _MSSID_
		apmib_get( MIB_WLAN_SSID_MIRROR_1, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_SSID_MIRROR_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		websWrite(wp, T("\"%s\""), "");
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("gusetssidenable")) ) {

		if ( !apmib_get( MIB_WLAN_SSID_MIRROR_1, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal);
		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("noForwardAll")) ) {
		int val;
		#ifdef _MSSID_NOFORWARD_ 
		apmib_get( MIB_WLAN_NOFORWARD, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_NOFORWARD7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
		#else
		apmib_get( MIB_WLAN_NOFORWARD, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("defaultKeyIdAll")) ) {
		int val;
#ifdef _MSSID_
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_1, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_2, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_3, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_4, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_5, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_6, (void *)&val);		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY_7, (void *)&val);		websWrite(wp, T("\"%d\")"), val);
#else
		apmib_get( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

 	else if ( !strcmp(name, T("pskValueAll"))) {
#ifdef _MSSID_
		apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);	CharFilter2(buffer);	websWrite(wp, T("new Array(\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_1,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_2,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_3,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_4,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_5,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_6,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_WPA_PSK_7,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("\"%s\")"), buffer);
#else
		apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);	CharFilter2(buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
#endif
   		return 0;
	}
	else if ( !strcmp(name, T("pskValueArr"))) {
		apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer);	
		//CharFilter2(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
   	return 0;
	}
	else if ( !strcmp(name, T("pskValue1Arr"))) {
		apmib_get(MIB_WLAN_WPA_PSK_1,  (void *)buffer);	
		//CharFilter2(buffer);		
		websWrite(wp, T("new Array(\"%s\")"), buffer);
   	return 0;
	}
	else if ( !strcmp(name, T("ssid1Arr"))) {
		#ifdef _MSSID_
		apmib_get(MIB_WLAN_SSID_1,(void *)buffer); 
		//CharFilter6(buffer); 
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("ssidArr"))) {
		apmib_get(MIB_WLAN_SSID,(void *)buffer); 
		//CharFilter6(buffer); 
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pskFormatAll"))) {
		int val;
#ifdef _MSSID_
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT, (void *)&val);		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_1, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_2, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_3, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_4, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_5, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_6, (void *)&val);	websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT_7, (void *)&val);	websWrite(wp, T("\"%d\")"), val);
#else
		apmib_get( MIB_WLAN_WPA_PSK_FORMAT, (void *)&val);		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}
 	else if ( !strcmp(name, T("wepValueAll"))) {
#ifdef _MSSID_
 		int val;
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 	 		websWrite(wp, T("new Array(\"%s\","), buffer);

 	 		memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);		websWrite(wp, T("new Array(\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_1, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_1,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_2, (void *)&val);
		if ( val == 0){
			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_2,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_3, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_3,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_4, (void *)&val);
		if ( val == 0){
			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
		}
		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_4,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_5, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_5,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_5,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_5,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_6, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_6,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_6,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_6,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
 		apmib_get( MIB_WLAN_WEP_KEY_TYPE_7, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY1_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4_7,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\")"), buffer);
 		}
 		else{
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY1_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4_7,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4_7,  (void *)buffer);	websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\")"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}
#else
		int val;
		apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val);
 		if (val == 0){
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 	 		websWrite(wp, T("new Array(\"%s\","), buffer);

 	 		memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP64_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY2,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY3,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\","), buffer);

 			memset(buffer, 0, sizeof(buffer));
 			apmib_get(MIB_WLAN_WEP128_KEY4,  (void *)buffer);
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
 	 		CharFilter2(buffer);
#endif
 			websWrite(wp, T("\"%s\")"), buffer);
		}
 		else{
 			memset(buffer, 0, sizeof(buffer));
 	 		apmib_get(MIB_WLAN_WEP64_KEY1,  (void *)buffer);		websWrite(wp, T("new Array(\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP64_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY1,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY2,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY3,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\","), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 			sprintf(buffer,"");
 			apmib_get(MIB_WLAN_WEP128_KEY4,  (void *)buffer);		websWrite(wp, T("\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\")"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
 		}

#endif //_MSSID_
   		return 0;

 	}
	else if ( !strcmp(name, T("wpaCipherAll"))) {
		int iTkip, iAes;
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iTkip);		apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iAes);
#ifdef _MSSID_
		websWrite(wp, T("new Array(\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_1, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_1, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_2, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_2, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_3, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_3, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_4, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_4, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_5, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_5, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_6, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_6, (void *)&iAes);
		websWrite(wp, T("\"%d\","), iAes+iTkip);
		apmib_get( MIB_WLAN_WPA_CIPHER_SUITE_7, (void *)&iTkip);	apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE_7, (void *)&iAes);
		websWrite(wp, T("\"%d\")"), iAes+iTkip);
#else
		websWrite(wp, T("new Array(\"%d\")"), iAes+iTkip);
#endif
		return 0;
	}

	else if ( !strcmp(name, T("keyTypeAll"))) {
		int val;
		apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val);
#ifdef _MSSID_
		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_1, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_2, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_3, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_4, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_5, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_6, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_WEP_KEY_TYPE_7, (void *)&val);
		websWrite(wp, T("\"%d\")"), val);
#else
		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

   	else if ( !strcmp(name, T("secModeAll"))) {
		apmib_get( MIB_SECURITY_MODE,  (void *)buffer) ;
#ifdef _MSSID_
		websWrite(wp, T("new Array(\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_1,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_2,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_3,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_4,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_5,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_6,  (void *)buffer) ;
		websWrite(wp, T("\"%d\","), (int)buffer[0]);
		apmib_get( MIB_SECURITY_MODE_7,  (void *)buffer) ;
		websWrite(wp, T("\"%d\")"), (int)buffer[0]);
#else
		websWrite(wp, T("new Array(\"%d\")"), (int)buffer[0]);
#endif
		return 0;
	}
   	else if ( !strcmp(name, T("ssidAll"))) {
#ifdef _MSSID_
		apmib_get(MIB_WLAN_SSID,  (void *)buffer);	CharFilter6(buffer);	websWrite(wp, T("new Array(\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_1,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_2,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_3,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_4,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_5,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_6,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\","), buffer);
		apmib_get(MIB_WLAN_SSID_7,  (void *)buffer);	CharFilter6(buffer);		websWrite(wp, T("\"%s\")"), buffer);
#else
		apmib_get(MIB_WLAN_SSID,(void *)buffer); CharFilter6(buffer); websWrite(wp, T("new Array(\"%s\")"), buffer);
#endif
		return 0;
	}

	else if ( !strcmp(name, T("enable1XAll"))) {
		int val;
		apmib_get( MIB_WLAN_ENABLE_1X, (void *)&val);
#ifdef _MSSID_
		websWrite(wp, T("new Array(\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_1, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_2, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_3, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_4, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_5, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_6, (void *)&val);
		websWrite(wp, T("\"%d\","), val);
		apmib_get( MIB_WLAN_ENABLE_1X_7, (void *)&val);
		websWrite(wp, T("\"%d\")"), val);
#else
		websWrite(wp, T("new Array(\"%d\")"), val);
#endif
		return 0;
	}

	else if ( !strcmp(name, T("1xMode"))) {
		int val;
		apmib_get( MIB_1X_MODE, (void *)&val);
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
		sprintf (buffer, "/bin/wlan.sh %s", WLAN_IF);
		system (buffer);
		
		return websWrite(wp, T("%s"), " ");
	}

#endif
	else if ( !strcmp(name, T("bssid_drv"))) {
        if ( !apmib_get(MIB_WLAN_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				                        buffer[2], buffer[3], buffer[4], buffer[5]);

	}
	else if ( !strcmp(name, T("ssid_drv"))) {

        if ( !apmib_get( MIB_WLAN_SSID,  (void *)buffer) )
        	return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			#if defined(_Customer_)
			CharFilter1(buffer);
			#else
			CharFilter2(buffer);
			#endif
		#endif
		return websWrite(wp, buffer);
		//return websWrite(wp, T("%s"), buffer);
}
	else if ( !strcmp(name, T("ssid_drv2"))) {	// for stadev.asp status show ssid

    if ( !apmib_get( MIB_WLAN_SSID,  (void *)buffer) )
    	return -1;
		CharFilter6(buffer);
		return websWrite(wp, buffer);
}
#ifdef _MSSID_
	else if ( !strcmp(name, T("ssid1_drv"))) {
    if ( !apmib_get( MIB_WLAN_SSID_1,  (void *)buffer) )
    	return -1;

		CharFilter6(buffer);
		return websWrite(wp, buffer);
}
#endif

	else if ( !strcmp(name, T("state_drv"))) {
		char *pMsg;
		if ( getWlBssInfo(WLAN_IF, &bss) < 0)
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
        if ( !apmib_get( MIB_WLAN_CHAN_NUM,  (void *)&intVal) )
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
#ifdef _DISCONMSG_
	else if ( !strcmp(name, T("inic_channel_drv"))) {
        if ( !apmib_get( MIB_INIC_CHAN_NUM,  (void *)&intVal) )
			return -1;
	  	if (intVal==0)
			sprintf(buffer, "%s", "auto" );
		else
			sprintf(buffer, "%d", intVal );
		return websWrite(wp, buffer);


//		if (bss.channel)
//			sprintf(buffer, "%d", bss.channel);
//		else
//			buffer[0] = '\0';
//		return websWrite(wp, T("%s"), buffer);
	}
#endif
   	else if ( !strcmp(name, T("channel"))) {

		if ( !apmib_get( MIB_WLAN_CHAN_NUM,  (void *)&intVal) )
			return -1;

		if (intVal==0)
			sprintf(buffer, "%s", "auto" );
		else
			sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}

	else if ( !strcmp (name, T("auto_channel"))){
		intVal = getAPChannel (WLAN_IF, &buffer);
		
		if (intVal == 0)
			sprintf(buffer, "%s", "auto" );
		else
			sprintf (buffer, "%d", intVal);
                return websWrite(wp, buffer);
	}

	#ifdef _AUTO_FW_UPGRADE_
	else if ( !strcmp(name, T("fw_info")) ) {
#ifdef __TARGET_BOARD__
		if(isServerAlive){
			websWrite(wp, T("new Array(\"%s\","), fw_version);
			printf("fw_version:%s\n", fw_version); 

			websWrite(wp, T("\"%s\","), zip_url);
			printf("zip_url:%s\n", zip_url); 

			websWrite(wp, T("\"%s\")"), fw_url);
			printf("fw_url:%s\n", fw_url); 
		}
		else
		{
			websWrite(wp, T("new Array(\"NA\","));
			websWrite(wp, T("\"NA\","));
			websWrite(wp, T("\"NA\")"));
		}
		return 0;
	
#else
		websWrite(wp, T("new Array(\"2.00\","));
		websWrite(wp, T("\"NA\","));
		websWrite(wp, T("\"NA\")"));
		return 0;
#endif
	}
#endif
 	else if ( !strcmp(name, T("wepAll"))) {
		apmib_get( MIB_WLAN_WEP, (void *)&intVal);
#ifdef _MSSID_
		websWrite(wp, T("new Array(\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_1, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_2, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_3, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_4, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_5, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_6, (void *)&intVal);
		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_WEP_7, (void *)&intVal);
		websWrite(wp, T("\"%d\")"), intVal);
#else
		websWrite(wp, T("new Array(\"%d\")"), intVal);
#endif
		return 0;
   	}

   	else if ( !strcmp(name, T("wep_key_is_empty"))) {  //0=empty     1=not empty
   websWrite(wp, T("new Array("));
		apmib_get( MIB_WLAN_WEP64_KEY1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
#ifdef _MSSID_
		apmib_get( MIB_WLAN_WEP64_KEY1_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_1, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_2, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_3, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_4, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_5, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_6, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY1_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY2_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY3_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP64_KEY4_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY1_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY2_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY3_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
		apmib_get( MIB_WLAN_WEP128_KEY4_7, buffer); websWrite(wp, T("\"%d\","), ((!strcmp(buffer,""))?0:1));
#endif
		websWrite(wp, T("\"\")"));
		return 0;
   	}

   	else if ( !strcmp(name, T("wep"))) {
   		WEP_T wep;
		if ( !apmib_get( MIB_WLAN_WEP,  (void *)&wep) )
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
		#ifdef __TARGET_BOARD__
		FILE *fp;
		int wizmode;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);
		if(wizmode != 2){
			if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr ) )
				return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
			else
				return websWrite(wp, T("0.0.0.0"));
		}
		else{

			if ((fp = fopen("/tmp/whereIsLanNow","r"))!=NULL) {
				fgets(buffer, sizeof(buffer), fp);
				fclose(fp);
				if (buffer[strlen(buffer)-1]== '\n')
					buffer[strlen(buffer)-1] = '\0';
			}
			else
				sprintf(buffer,"%s","br0");

			if ( getInAddr(buffer, IP_ADDR, (void *)&intaddr ) )
				return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
			else
				return websWrite(wp, T("0.0.0.0"));
		}
		#else
		return websWrite(wp, T("192.168.2.xx"));
		#endif
	}
	else if ( !strcmp(name, T("brx-gateway"))) {
		#ifdef __TARGET_BOARD__
		FILE *fp, fp1;
		char bufbrx[10]={0};

		if ((fp = fopen("/tmp/whereIsLanNow","r"))!=NULL) {
			fgets(bufbrx, sizeof(bufbrx), fp);
			fclose(fp);
			if (bufbrx[strlen(bufbrx)-1]== '\n')
				bufbrx[strlen(bufbrx)-1] = '\0';
		}
		else sprintf(bufbrx,"%s","br0");

		sprintf(buffer,"/tmp/%s_gw",bufbrx);

		if ((fp = fopen(buffer,"r"))!=NULL) {
			fgets(buffer, sizeof(buffer), fp);
			fclose(fp);
			if (buffer[strlen(buffer)-1]== '\n')
				buffer[strlen(buffer)-1] = '\0';
			return websWrite(wp, buffer);
		}
		else{
			return websWrite(wp, T("0.0.0.0"));
		}
		#else
		return websWrite(wp, T("192.168.2.x"));
		#endif
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
		if ( !apmib_get(MIB_WLAN_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}

#if defined(HOME_GATEWAY) || defined(_AP_WITH_TIMEZONE_)
//**************************************** Time Zone ****************************************
	else if ( !strcmp(name, T("TimeZoneSel"))) {
		if ( !apmib_get( MIB_TIME_ZONE_SEL, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("startMonth"))) {
		if ( !apmib_get( MIB_START_MONTH, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("startDay"))) {
		if ( !apmib_get( MIB_START_DAY, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	#if defined (_Customer_)
	else if ( !strcmp(name, T("startHour"))) {
		if ( !apmib_get( MIB_START_HOUR, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("endHour"))) {
		if ( !apmib_get( MIB_END_HOUR, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	#endif
	else if ( !strcmp(name, T("endMonth"))) {
		if ( !apmib_get( MIB_END_MONTH, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("endDay"))) {
		if ( !apmib_get( MIB_END_DAY, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("TimeServerAddr"))) {
		if ( !apmib_get(MIB_TIMESERVER_ADDR,   (void *)&intaddr ) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
	}
#endif
#ifdef _TIMESERVER_
	else if ( !strcmp(name, T("ServerSel"))) {
		if ( !apmib_get( MIB_SERVER_SEL, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif
#if _SNTP_A_M_CTL_ //vance 2009.02.20
	else if ( !strcmp(name, T("staticTime"))) {
			if ( !apmib_get( MIB_STATIC_SYSTEM_TIME, (void *)&buffer) )
				return -1;
			return websWrite(wp, T("%s"), buffer);
		}
	else if ( !strcmp(name, T("timeserverDomain"))) {
		if ( !apmib_get( MIB_TIMESERVER_DOMAIN, (void *)&buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
#endif

#ifdef _TIMESERVER_
	else if ( !strcmp(name, T("NTPSaddr"))) {
			if ( !apmib_get( MIB_NTPS_ADDR, (void *)&buffer) )
				return -1;
			return websWrite(wp, T("%s"), buffer);
	}
#endif

#ifdef HOME_GATEWAY
	else if ( !strcmp(name, T("hostpassword")) ) {	//no use
		buffer[0]='\0';
		if ( !apmib_get(MIB_USER_PASSWORD,  (void *)buffer) )
			return -1;

			CharFilter6(buffer);
			websWrite(wp, T("new Array(\"%s\")"), buffer);
			return 0;
   		//return websWrite(wp, buffer);
	}

//************************************** Remote Management **************************************
	else if ( !strcmp(name, T("reManHostAddr"))) {
		if ( !apmib_get(MIB_REMANHOST_ADDR,  (void *)&intaddr ) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("reManPort"))) {
		if ( !apmib_get( MIB_REMAN_PORT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
//**************************************** Dynamic Ip ****************************************
	else if ( !strcmp(name, T("dynIPHostName")) ) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_DYNIP_HOSTNAME,  (void *)buffer) )
			return -1;
			
		//#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_) 
		//	CharFilter2(buffer);
		//#endif
		//return websWrite(wp, buffer);
		CharFilter5(buffer); //patrick
		return websWrite(wp, T("%s"), buffer);
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
//**************************************** Static Ip ****************************************
	else if ( !strcmp(name, T("wan-ip-rom"))) {
		if ( !apmib_get( MIB_WAN_IP_ADDR, (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"),inet_ntoa(intaddr));
	}
   	else if ( !strcmp(name, T("wan-mask-rom"))) {
		if ( !apmib_get( MIB_WAN_SUBNET_MASK,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
 	else if ( !strcmp(name, T("wan-gateway-rom"))) {
		if ( !apmib_get( MIB_WAN_DEFAULT_GATEWAY,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T("0.0.0.0"));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
//**************************************** PPPoE Setting ****************************************
   	else if ( !strcmp(name, T("pppServName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPP_SERVNAME,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("pppMTU"))) {
		if ( !apmib_get( MIB_PPP_MTU, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#ifdef _INDEPEND_DHCP_MTU_
	else if ( !strcmp(name, T("dhcpMTU"))) {
		if ( !apmib_get( MIB_DHCP_MTU, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif
#ifdef _INDEPEND_MTU_
	else if ( !strcmp(name, T("independMTU"))) {
		if ( !apmib_get( MIB_INDEPEND_MTU, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif
   	else if ( !strcmp(name, T("pppUserName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPP_USER,  (void *)buffer) )
			return -1;

		ptr1 = buffer;
		tmp[0] = '\0';
		while ( flag ) {
			if ((ptr = strchr(ptr1, '#')) != NULL) {
				*(ptr-1) ='\0';
				ptr++;
				strcat( tmp, ptr1);
				strcat( tmp, "#");
				ptr1 = ptr;
			}
			else {
				strcat( tmp, ptr1);
				flag = 0;
			}
		}
		CharFilter5(tmp);
		return websWrite(wp, T("%s"), tmp);
	}
  	else if ( !strcmp(name, T("pppPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPP_PASSWORD,  (void *)buffer) )
			return -1;
		CharFilter5(buffer);
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("wan-ppp-idle"))) {
		if ( !apmib_get( MIB_PPP_IDLE_TIME,  (void *)&intVal) )
			return -1;

		sprintf(buffer, "%d", intVal/60 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("pptpIPAddr"))) {
		if ( !apmib_get(MIB_PPTPIP_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("pptpIPMaskAddr"))) {
		if ( !apmib_get(MIB_PPTPIPMAKE_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("pptpDfGateway"))) {
		if ( !apmib_get(MIB_PPTPDFGATEWAY,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}

#ifdef _PPTP_FQDN_
  	else if ( !strcmp(name, T("pptpGateway"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPTPGATEWAY,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
#else
	else if ( !strcmp(name, T("pptpGateway"))) {
		if ( !apmib_get(MIB_PPTPGATEWAY,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
#endif
   	else if ( !strcmp(name, T("pptpUserName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPTP_USER,  (void *)buffer) )
			return -1;

		ptr1 = buffer;
		tmp[0] = '\0';
		while ( flag ) {
			if ((ptr = strchr(ptr1, '#')) != NULL) {
				*(ptr-1) ='\0';
				ptr++;
				strcat( tmp, ptr1);
				strcat( tmp, "#");
				ptr1 = ptr;
			}
			else {
				strcat( tmp, ptr1);
				flag = 0;
			}
		}

		CharFilter5(tmp); //patrick
		return websWrite(wp, T("%s"), tmp);
	}
  	else if ( !strcmp(name, T("pptpPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPTP_PASSWORD,  (void *)buffer) )
			return -1;
		CharFilter5(buffer); //patrick
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("wan-pptp-idle"))) {
		if ( !apmib_get( MIB_PPTP_IDLE_TIME,  (void *)&intVal) )
			return -1;

		sprintf(buffer, "%d", intVal/60 );
   		return websWrite(wp, buffer);
	}
   	else if ( !strcmp(name, T("pptpConntID"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_PPTP_CONNT_ID,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("pptpMTU"))) {
		if ( !apmib_get( MIB_PPTP_MTU, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
//**************************************** L2TP Setting ****************************************
	else if ( !strcmp(name, T("L2TPIPAddr"))) {
		if ( !apmib_get(MIB_L2TP_IP_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("L2TPMaskAddr"))) {
		if ( !apmib_get(MIB_L2TP_MAKEADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("L2TPDefGateway"))) {
		if ( !apmib_get(MIB_L2TP_DEFGATEWAY,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
  	else if ( !strcmp(name, T("L2TPGateway"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_L2TP_GATEWAY,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
   	else if ( !strcmp(name, T("L2TPUserName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_L2TP_USER,  (void *)buffer) )
			return -1;

		/*#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_) //patrick
			CharFilter2(buffer);
		#endif

		return websWrite(wp, buffer);*/

		ptr1 = buffer;
		tmp[0] = '\0';
		while ( flag ) {
			if ((ptr = strchr(ptr1, '#')) != NULL) {
				*(ptr-1) ='\0';
				ptr++;
				strcat( tmp, ptr1);
				strcat( tmp, "#");
				ptr1 = ptr;
			}
			else {
				strcat( tmp, ptr1);
				flag = 0;
			}
		}
		CharFilter5(tmp); //patrick
		return websWrite(wp, T("%s"), tmp);
	}
  	else if ( !strcmp(name, T("L2TPPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_L2TP_PASSWORD,  (void *)buffer) )
			return -1;
		CharFilter5(buffer); //patrick
		return websWrite(wp, T("%s"), buffer);
	}
   	else if ( !strcmp(name, T("L2TPIdleTime"))) {
		if ( !apmib_get( MIB_L2TP_IDLE_TIME,  (void *)&intVal) )
			return -1;

		sprintf(buffer, "%d", intVal/60 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("L2TPMTU"))) {
		if ( !apmib_get( MIB_L2TP_MTU, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
//************************************** TelStra Big Pond **************************************
	else if ( !strcmp(name, T("telBPIPAddr"))) {
		if ( !apmib_get(MIB_TELBPIP_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
   	else if ( !strcmp(name, T("telBPUserName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_TELBP_USER,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
  	else if ( !strcmp(name, T("telBPPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_TELBP_PASSWORD,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%s"), buffer);
	}
//*********************************** Application layer gateway ***********************************
	else if ( !strcmp(name, T("appLayerGateway"))) {
		if ( !apmib_get( MIB_APP_LAYER_GATEWAY, (void *)&longVal) )
			return -1;
		sprintf(buffer, "%lu", longVal );
   		return websWrite(wp, buffer);
	}
//*********************************** LAN WAN Access ***********************************
	else if ( !strcmp(name, T("lanWanAccess"))) {
		#ifdef _LAN_WAN_ACCESS_
		if ( !apmib_get( MIB_LAN_WAN_ACCESS, (void *)&longVal) )
			return -1;
		sprintf(buffer, "%lu", longVal );

		#else
			sprintf(buffer,"\"\"");
		#endif



   		return websWrite(wp, buffer);
	}
//**************************************** DDNS Setting ****************************************
   	else if ( !strcmp(name, T("ddnsName"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DDNS_NAME,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
   	else if ( !strcmp(name, T("ddnspvidSel"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DDNS_PVIDSEL,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
   	else if ( !strcmp(name, T("ddnsAccount"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DDNS_ACCOUNT,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("ddnsPass"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DDNS_PASS,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
#ifdef _DDNS_UID_BY_MAC_
  	else if ( !strcmp(name, T("ddnsEdimaxUid"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DDNS_UID,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
#endif
	else if ( !strcmp(name, T("dhisHostID"))) {
		if ( !apmib_get( MIB_DHIS_HOSTID, (void *)&longVal) )
			return -1;
		sprintf(buffer, "%lu", longVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("dhisISAddr"))) {
		if ( !apmib_get(MIB_DHIS_ISADDR,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("dhispass"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DHIS_PASSWORD,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("dhisAuthP1"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DHIS_AUTH_P1,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("dhisAuthP2"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DHIS_AUTH_P2,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("dhisAuthQ1"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DHIS_AUTH_Q1,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
  	else if ( !strcmp(name, T("dhisAuthQ2"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_DHIS_AUTH_Q2,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
//**************************************** DoS ****************************************
	else if ( !strcmp(name, T("podPack"))) {
		if ( !apmib_get( MIB_POD_PACK, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("podBur"))) {
		if ( !apmib_get( MIB_POD_BUR, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("synPack"))) {
		if ( !apmib_get( MIB_SYN_PACK, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("synBur"))) {
		if ( !apmib_get( MIB_SYN_BUR, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("scanNum"))) {
		if ( !apmib_get( MIB_SCAN_NUM, (void *)&longVal) )
			return -1;
		sprintf(buffer, "%lu", longVal );
   		return websWrite(wp, buffer);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
 	else if ( !strcmp(name, T("mask"))) {

		#ifdef __TARGET_BOARD__
		FILE *fp;
		int wizmode;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);
		if(wizmode != 2){
			if ( getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&intaddr ))
				return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
			else
				return websWrite(wp, T("0.0.0.0"));
		}
		else{

			if ((fp = fopen("/tmp/whereIsLanNow","r"))!=NULL) {
				fgets(buffer, sizeof(buffer), fp);
				fclose(fp);
				if (buffer[strlen(buffer)-1]== '\n')
					buffer[strlen(buffer)-1] = '\0';
			}
			else
				sprintf(buffer,"%s","br0");

			if ( getInAddr(buffer, SUBNET_MASK, (void *)&intaddr ))
				return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
			else
				return websWrite(wp, T("0.0.0.0"));
		}
		#else
		return websWrite(wp, T("255.255.255.x"));
		#endif
	}
 	else if ( !strcmp(name, T("gateway"))) {
#ifdef HOME_GATEWAY
		DHCP_T dhcp;
  		apmib_get( MIB_DHCP, (void *)&dhcp);
		if ( dhcp == DHCP_SERVER ) {
		// if DHCP server, default gateway is set to LAN IP
			if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr ) )
				return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
			else
				return websWrite(wp, T("0.0.0.0"));
	}
	else
#endif

		if ( getDefaultRoute(BRIDGE_IF, &intaddr) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
//**************************************** LAN Setting****************************************
	else if ( !strcmp(name, T("ip-rom")))
	{
		#ifdef _IP_SWITCH_
		if( apmib_get( MIB_AP_ROUTER_SWITCH, (void *)&intVal))
		{
			if(!intVal)
			{
				if ( !apmib_get( MIB_IP_ADDR_ROUTER,  (void *)&intaddr) )
					return -1;
   			return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}
			else
			{
				if ( !apmib_get( MIB_IP_ADDR_AP,  (void *)&intaddr) )
					return -1;
   			return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}

		}
		#else
		if ( !apmib_get( MIB_IP_ADDR,  (void *)&intaddr) )
			return -1;
		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
 		#endif
	}
   	else if ( !strcmp(name, T("mask-rom"))) {
		if ( !apmib_get( MIB_SUBNET_MASK,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("gateway-rom"))) {
		if ( !apmib_get( MIB_DEFAULT_GATEWAY,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T("0.0.0.0"));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	#ifdef _DHCP_SWITCH_
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
   else if ( !strcmp(name, T("dhcpRangeStart"))) {
   	#ifdef _IP_SWITCH_
		if( apmib_get( MIB_AP_ROUTER_SWITCH, (void *)&intVal))
		{
			if(!intVal)
			{
				if ( !apmib_get( MIB_DHCP_CLIENT_ROUTER_START,  (void *)&intaddr) )
					return -1;

				if( !strcmp(inet_ntoa(intaddr), T("0.0.0.0")))
					if ( !apmib_get( MIB_DHCP_CLIENT_START,  (void *)&intaddr) )
						return -1;

				return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}
			else
			{
				if ( !apmib_get( MIB_DHCP_CLIENT_AP_START,  (void *)&intaddr) )
					return -1;

				if( !strcmp(inet_ntoa(intaddr), T("0.0.0.0")))
					if ( !apmib_get( MIB_DHCP_CLIENT_START,  (void *)&intaddr) )
						return -1;

				return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}
		}
		#else
		if ( !apmib_get( MIB_DHCP_CLIENT_START,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
  		#endif
	}
  	else if ( !strcmp(name, T("dhcpRangeEnd"))) {
   	#ifdef _IP_SWITCH_
		if( apmib_get( MIB_AP_ROUTER_SWITCH, (void *)&intVal))
		{
			if(!intVal)
			{
				if ( !apmib_get( MIB_DHCP_CLIENT_ROUTER_END,  (void *)&intaddr) )
					return -1;

				if( !strcmp(inet_ntoa(intaddr), "0.0.0.0"))
					if ( !apmib_get( MIB_DHCP_CLIENT_END,  (void *)&intaddr) )
						return -1;

				return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}
			else
			{
				if ( !apmib_get( MIB_DHCP_CLIENT_AP_END,  (void *)&intaddr) )
					return -1;

				if( !strcmp(inet_ntoa(intaddr), "0.0.0.0"))
					if ( !apmib_get( MIB_DHCP_CLIENT_END,  (void *)&intaddr) )
						return -1;

				return websWrite(wp, T("%s"), inet_ntoa(intaddr));
			}
		}
		#else
		if ( !apmib_get( MIB_DHCP_CLIENT_END,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	#endif
	}
	else if ( !strcmp(name, T("leaseTime"))) {
		if ( !apmib_get( MIB_LAN_LEASETIME, (void *)&longVal) )
			return -1;
		sprintf(buffer, "%lu", longVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("DhcpGatewayIP"))) {
		if ( !apmib_get(MIB_DHCPGATEWAYIP_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}

	else if ( !strcmp(name, T("DhcpNameServerIP"))) {
		if ( !apmib_get(MIB_DHCPNAMESERVER_ADDR,  (void *)&intaddr) )
			return -1;
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
   	if ( !strcmp(name, T("DomainName")) ) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_DOMAIN_NAME,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
//********************* TTL Function add by Kyle 2008/03/07. *******************
#ifdef _WANTTL_
  	else if ( !strcmp(name, T("getTTLValue"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_TTL_VALUE,  (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
#endif
//***************************************************************************
 	else if ( !strcmp(name, T("dhcp-current")) ) {
   		if ( !apmib_get( MIB_DHCP, (void *)&dhcp) )
			return -1;

		if (dhcp==DHCP_CLIENT) {
			if (!isDhcpClientExist(BRIDGE_IF) &&
					!getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr))
				return websWrite(wp, T("Getting IP from DHCP server..."));
			if (isDhcpClientExist(BRIDGE_IF))
				return websWrite(wp, T("DHCP"));
		}
		return websWrite(wp, T("Fixed IP"));
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
//			iface = WLAN_IF;
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
//			iface = WLAN_IF;
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
//			iface = WLAN_IF;
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
//			iface = WLAN_IF;
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
//				iface = WLAN_IF;
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
	else if ( !strcmp(name, T("saveSystemLog"))) {
		#ifdef _EXPORT_SYS_LOG_
		#ifdef __TARGET_BOARD__
		system("/bin/export_syslog.sh");
		#endif
		#endif
		return 0;
	}
#ifdef _WIFI_ROMAING_
	else if ( !strcmp(name, T("saveGAdevicelog"))) {
		system("/bin/GA_device_log.sh");
		return 0;
	}
	else if ( !strcmp(name, T("SlaveNum"))) {
		FILE *fp2, *fp3, *fp4;
		int val;
		char tmp[5]={0};

		if ((fp2 = fopen("/tmp/CheckFW","r"))!=NULL)
		{
			printf("== /tmp/CheckFW No need excute SlaveNum.sh ==\n");
			fclose(fp2);
		}
		else
		{
			if ((fp3 = fopen("/tmp/DownloadFW","r"))!=NULL) 
			{
				printf("== /tmp/DownloadFW No need excute SlaveNum.sh ==\n");
				fclose(fp3);
			}
			else
			{
				system("/bin/SlaveNum.sh");
			}
		}

		if ((fp4 = fopen("/tmp/SlaveNum","r"))!=NULL)
		{		
			fgets(tmp, sizeof(tmp), fp4);
			val = atoi(tmp);
			fclose(fp4);
			printf("== /tmp/SlaveNum ==\n");
			websWrite(wp, T("%d"), val);
		}
		else
		{
			websWrite(wp, T("0"));
		}

		return 0;
	}
	else if ( !strcmp(name, T("OneTime"))) {
		FILE *fp5;
		int val;
		char bufferFW[5] = {0};

		if ((fp5 = fopen("/tmp/OneTime","r"))!=NULL) {
			fgets(bufferFW, sizeof(bufferFW), fp5);
			val = atoi(bufferFW);
			system("rm -rf /tmp/OneTime");
			fclose(fp5);
			websWrite(wp, T("%d"), val);
		}
		else
		{
			websWrite(wp, T("0"));
		}	

		return 0;
	}
	else if ( !strcmp(name, T("SlaveRealNumber"))) {
		FILE *fp6;
		int val;
		char buffer[5] = {0};

		if ((fp6 = fopen("/tmp/SlaveRealNumber","r"))!=NULL) {
			fgets(buffer, sizeof(buffer), fp6);
			val = atoi(buffer);
			system("rm -rf /tmp/SlaveRealNumber");
			fclose(fp6);
			websWrite(wp, T("%d"), val);
		}
		else
		{
			websWrite(wp, T("0"));
		}

		return 0;
	}
	else if ( !strcmp(name, T("Rload"))) {
		FILE *fp1, *fp;
		int val, stop=0;
		char buffer[5] = {0};

		if ((fp = fopen("/tmp/OneTime","r"))!=NULL)
		{
			stop=1;
			fclose(fp);
		}
		else
		{
			stop=0;
		}

		if ((fp1 = fopen("/tmp/Rload","r"))!=NULL) {
			fgets(buffer, sizeof(buffer), fp1);
			val = atoi(buffer);
			if (stop==0)
			{	
				system("rm -rf /tmp/Rload");
			}
			fclose(fp1);
			websWrite(wp, T("%d"), val);
		}
		else
		{
			websWrite(wp, T("0"));
		}	

		return 0;
	}
	else if ( !strcmp(name, T("GoToUpgrade"))) {
		FILE *fp7;
		int val;
		char buffer[5] = {0};

		if ((fp7 = fopen("/tmp/GoToUpgrade","r"))!=NULL) {
			fgets(buffer, sizeof(buffer), fp7);
			val = atoi(buffer);
			system("rm -rf /tmp/GoToUpgrade");
			fclose(fp7);
			websWrite(wp, T("%d"), val);
		}
		else
		{
			websWrite(wp, T("0"));
		}

		return 0;
	}
#endif
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
	else if ( !strcmp(name, T("sta-ip"))) {
		int i;
		strcpy(buffer, staTbl[0][1]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
	else if (!strcmp(name, T("sta-mask"))) {
		int i;
		strcpy(buffer, staTbl[0][2]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("sta-gateway"))) {
		if ( getDefaultRoute(BRIDGE_IF, &intaddr) )
			return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
		else
			return websWrite(wp, T("0.0.0.0"));
	}
	else if (!strcmp(name, T("sta-mac"))) {
		int i;
		strcpy(buffer, staTbl[0][3]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
	else if (!strcmp(name, T("sta-dns1"))) {
		int i;
		strcpy(buffer, staTbl[0][4]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
	else if (!strcmp(name, T("sta-dns2"))) {
		int i;
		strcpy(buffer, staTbl[0][5]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
	else if (!strcmp(name, T("sta-dns3"))) {
		int i;
		strcpy(buffer, staTbl[0][6]);
		i=strlen(buffer)-1;
		while(i>=0 && isspace(buffer[i])) {buffer[i]='\0'; i--;}
		return websWrite(wp, T("%s"), buffer);
	}
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
		FILE *fp;
		int wizmode;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);
		if(wizmode != 2){
			if ( getInAddr(BRIDGE_IF, HW_ADDR, (void *)&hwaddr ) ) {
				pMacAddr = hwaddr.sa_data;
				return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
			}
			else
				return websWrite(wp, T("00:00:00:00:00:00"));
		}
		else{
			if ((fp = fopen("/tmp/whereIsLanNow","r"))!=NULL) {
				fgets(buffer, sizeof(buffer), fp);
				fclose(fp);
				if (buffer[strlen(buffer)-1]== '\n')
					buffer[strlen(buffer)-1] = '\0';
			}
			else
				sprintf(buffer,"%s","br0");

			if ( getInAddr(buffer, HW_ADDR, (void *)&hwaddr ) ) {
				pMacAddr = hwaddr.sa_data;
				return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
			}
			else
				return websWrite(wp, T("00:00:00:00:00:00"));
		}
	}
	else if ( !strcmp(name, T("wladdr"))) {
		if (!apmib_get (MIB_HW_WLAN_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("inicaddr"))) {
		if (!apmib_get (MIB_HW_INIC_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("inicaddr1"))) {
		if (!apmib_get (MIB_HW_INIC_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]+0x1);
	}
	else if ( !strcmp(name, T("inicaddr2"))) {
		if (!apmib_get (MIB_HW_INIC_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]+0x2);
	}
	else if ( !strcmp(name, T("inicaddr3"))) {
		if (!apmib_get (MIB_HW_INIC_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]+0x3);
	}
	else if ( !strcmp(name, T("inicaddr4"))) {
		if (!apmib_get (MIB_HW_INIC_ADDR, (void *)buffer))
			return -1;
		return websWrite (wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]+0x4);
	}
	else if ( !strcmp(name, T("bridgeMac"))) {
		if ( !apmib_get(MIB_ELAN_MAC_ADDR,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
#ifdef _DISCONMSG_	
	else if (!strcmp(name, T("br0_24"))) {
		if ( getInAddr("br0_24", HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}	
	else if (!strcmp(name, T("br0_5"))) {
		if ( getInAddr("br0_5", HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}	
#endif	
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
		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlmac1"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x0, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep ra1 | cut -b 39-55 > /tmp/wlmac1");
	
		if ((fp1 = fopen("/tmp/wlmac1","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 
	}
	else if ( !strcmp(name, T("wlmac2"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x1, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep ra2 | cut -b 39-55 > /tmp/wlmac2");
	
		if ((fp1 = fopen("/tmp/wlmac2","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 	
	}
	else if ( !strcmp(name, T("wlmac3"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x2, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep ra3 | cut -b 39-55 > /tmp/wlmac3");
	
		if ((fp1 = fopen("/tmp/wlmac3","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 	
	}
	else if ( !strcmp(name, T("wlmac4"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x3, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep ra4 | cut -b 39-55 > /tmp/wlmac4");
	
		if ((fp1 = fopen("/tmp/wlmac4","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 	
	}
	else if ( !strcmp(name, T("inicmac"))) {
		if ( !apmib_get( MIB_HW_INIC_ADDR, (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("inicmac1"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x0, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep rai1 | cut -b 39-55 > /tmp/inicmac1");
	
		if ((fp1 = fopen("/tmp/inicmac1","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 	
	}
	else if ( !strcmp(name, T("inicmac2"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x1, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep rai2 | cut -b 39-55 > /tmp/inicmac2");
	
		if ((fp1 = fopen("/tmp/inicmac2","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 
	}
	else if ( !strcmp(name, T("inicmac3"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x2, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep rai3 | cut -b 39-55 > /tmp/inicmac3");
	
		if ((fp1 = fopen("/tmp/inicmac3","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 
	}
	else if ( !strcmp(name, T("inicmac4"))) {
//		if ( !apmib_get( MIB_HW_WLAN_ADDR, (void *)buffer) )
//			return -1;
//		return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), buffer[0]+0x2, buffer[1],
//				buffer[2], 0x3, buffer[4], buffer[5]);
		int k,h;
		FILE *fp1;
		char strMAC[20];
		system("ifconfig -a |grep rai4 | cut -b 39-55 > /tmp/inicmac4");
	
		if ((fp1 = fopen("/tmp/inicmac4","r"))!=NULL) {
			fgets(strMAC, 50, fp1);
			fclose(fp1);
		}
		strcpy(buffer,strMAC);
		h=strlen(buffer)-1;
		while(h>=0 && isspace(buffer[h])) { buffer[h]='\0'; h--; }
		return websWrite(wp, T("%s"), buffer); 
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
		if ( !apmib_get( MIB_WLAN_TRAN_RATE,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
//*********************************** Wireless Advance Setting ***********************************
//RexHua
  else if ( !strcmp(name, T("wlanTxPower"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_WLAN_TX_POWER,  (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("useCrossBand"))) {
		buffer[0]='\0';
		#if defined(_CROSSBAND_)
		if ( !apmib_get( MIB_CROSSBAND_ENABLE,  (void *)&intVal) )
			return -1;
		#else
		intVal=0;
		#endif
		sprintf(buffer, "%d", intVal );
   	return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("fragThreshold"))) {
		if ( !apmib_get( MIB_WLAN_FRAG_THRESHOLD, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("rtsThreshold"))) {
		if ( !apmib_get( MIB_WLAN_RTS_THRESHOLD, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("beaconInterval"))) {
		if ( !apmib_get( MIB_WLAN_BEACON_INTERVAL, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("dtimPeriod"))) {
		if ( !apmib_get( MIB_WLAN_DTIM_PERIOD, (void *)&intVal) )
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
	else if ( !strcmp(name, T("lanTxPacketNum"))) {
		if ( getStats(ELAN_IF, &stats, 0) < 0)
			stats.tx_packets = 0;
		sprintf(buffer, "%d", (int)stats.tx_packets);
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("lanRxPacketNum"))) {
		if ( getStats(ELAN_IF, &stats, 0) < 0)
			stats.rx_packets = 0;
		sprintf(buffer, "%d", (int)stats.rx_packets);
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("wlanTxPacketNum"))) {
#ifdef _WISP_WITH_STA_
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 2){
			if ( getStats(WLAN_STA_IF, &stats, 1) < 0)
				stats.tx_packets = 0;
			else
				stats.tx_packets = stats.tx_bytes / 1024;
		}
		else{
			if ( getStats(WLAN_IF, &stats, 0) < 0)
				stats.tx_packets = 0;
		}
#else
		if ( getStats(WLAN_IF, &stats, 0) < 0)
			stats.tx_packets = 0;
#endif
		sprintf(buffer, "%d", (int)stats.tx_packets);
   		return websWrite(wp, buffer);

	}
	else if ( !strcmp(name, T("wlanRxPacketNum"))) {
#ifdef _WISP_WITH_STA_
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 2){
			if ( getStats(WLAN_STA_IF, &stats, 1) < 0)
				stats.rx_packets = 0;
			else
				stats.rx_packets = stats.rx_bytes / 1024;

		}else{
			if ( getStats(WLAN_IF, &stats, 0) < 0)
				stats.rx_packets = 0;
		}
#else
		if ( getStats(WLAN_IF, &stats, 0) < 0)
			stats.rx_packets = 0;
#endif
		sprintf(buffer, "%d", (int)stats.rx_packets);
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("bssid"))) {
		apmib_get( MIB_WLAN_DISABLED, (void *)&intVal);
		if ( intVal == 0 &&  getInAddr(WLAN_IF, HW_ADDR, (void *)&hwaddr ) ) {
			pMacAddr = hwaddr.sa_data;
			return websWrite(wp, T("%02x:%02x:%02x:%02x:%02x:%02x"), pMacAddr[0], pMacAddr[1],
				pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		else
			return websWrite(wp, T("00:00:00:00:00:00"));
	}
#if defined(_MSSID_)
	else if ( !strcmp(name, T("bssid1"))) {
		int k,h;
                FILE *fp1;
		char strMAC[20];
		system("ifconfig |grep rai1 | cut -b 39-55 > /tmp/SSIDMAC");
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
		system("ifconfig |grep rai2 | cut -b 39-55 > /tmp/SSIDMAC");
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
		system("ifconfig |grep rai3 | cut -b 39-55 > /tmp/SSIDMAC");
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
		if ( !apmib_get(MIB_WL_LINKMAC1,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac2"))) {
		if ( !apmib_get(MIB_WL_LINKMAC2,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac3"))) {
		if ( !apmib_get(MIB_WL_LINKMAC3,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac4"))) {
		if ( !apmib_get(MIB_WL_LINKMAC4,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac5"))) {
		if ( !apmib_get(MIB_WL_LINKMAC5,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
	else if ( !strcmp(name, T("wlLinkMac6"))) {
		if ( !apmib_get(MIB_WL_LINKMAC6,  (void *)buffer) )
			return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1],
						buffer[2], buffer[3], buffer[4], buffer[5]);
	}
//-----------------------------------------------------------------------------------------

#ifdef HOME_GATEWAY
	else if ( !strcmp(name, T("wanTxPacketNum"))) {
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 0)
			sprintf (_WAN_IF_, "%s", WAN_IF);
		else if (opmode == 1)
			sprintf (_WAN_IF_, "%s", WLAN_IF);
		else if (opmode == 2)
			sprintf (_WAN_IF_, "%s", WLAN_STA_IF);
		else if (opmode == 3)
			sprintf (_WAN_IF_, "%s", INIC_IF);
		else if (opmode == 4)
			sprintf (_WAN_IF_, "%s", INIC_STA_IF);
		else
			sprintf (_WAN_IF_, "%s", WAN_IF);

		iface = _WAN_IF_;

		if ((opmode == 2) || (opmode == 4)){
			if ( getStats(iface, &stats, 1) < 0)
				stats.tx_packets = 0;
			else
				stats.tx_packets = stats.tx_bytes / 1024;
		}else{
			if ( getStats(iface, &stats, 0) < 0)
				stats.tx_packets = 0;
		}
		sprintf(buffer, "%d", (int)stats.tx_packets);
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("wanRxPacketNum"))) {
		if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
			return -1;
		if(opmode == 0)
			sprintf (_WAN_IF_, "%s", WAN_IF);
		else if (opmode == 1)
			sprintf (_WAN_IF_, "%s", WLAN_IF);
		else if (opmode == 2)
			sprintf (_WAN_IF_, "%s", WLAN_STA_IF);
		else if (opmode == 3)
			sprintf (_WAN_IF_, "%s", INIC_IF);
		else if (opmode == 4)
			sprintf (_WAN_IF_, "%s", INIC_STA_IF);
		else
			sprintf (_WAN_IF_, "%s", WAN_IF);

		iface = _WAN_IF_;

		if ((opmode == 2) || (opmode == 4)){
			if ( getStats(iface, &stats, 1) < 0)
				stats.rx_packets = 0;
			else
				stats.rx_packets = stats.rx_bytes / 1024;
		}else{
			if ( getStats(iface, &stats, 0) < 0)
				stats.rx_packets = 0;
		}
		sprintf(buffer, "%d", (int)stats.rx_packets);
   		return websWrite(wp, buffer);
	}
	// Lance 2004.2.10
    else if ( !strcmp(name, T("PSName")) ) {
        buffer[0]='\0';
        if ( !apmib_get(MIB_PS_NAME,  (void *)buffer) )
            return -1;
        return websWrite(wp, buffer);
    }
	// Lance 2004.2.16
    else if ( !strcmp(name, T("PS_Port1Name")) ) {
        buffer[0]='\0';
        if ( !apmib_get(MIB_PS_PORT1NAME,  (void *)buffer) )
            return -1;
        return websWrite(wp, buffer);
    }
    else if ( !strcmp(name, T("PS_Port2Name")) ) {
        buffer[0]='\0';
        if ( !apmib_get(MIB_PS_PORT2NAME,  (void *)buffer) )
            return -1;
        return websWrite(wp, buffer);
    }
#endif


#ifdef WLAN_WPA


	else if ( !strcmp(name, T("rsIp"))) {
		if ( !apmib_get( MIB_WLAN_RS_IP,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("rsPort"))) {
		if ( !apmib_get( MIB_WLAN_RS_PORT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
 	else if ( !strcmp(name, T("rsPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_WLAN_RS_PASSWORD,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsIp"))) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_IP,  (void *)&intaddr) )
			return -1;
		if (!memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4))
			return websWrite(wp, T(""));
   		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
	else if ( !strcmp(name, T("accountRsPort"))) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_PORT, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsPassword"))) {
		buffer[0]='\0';
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_PASSWORD,  (void *)buffer) )
			return -1;
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTime"))) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeDay"))) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal/86400 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeHr"))) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", (intVal%86400)/3600 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeMin"))) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", ((intVal%86400)%3600)/60 );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("groupRekeyTimeSec"))) {
		if ( !apmib_get( MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", ((intVal%86400)%3600)%60 );
   		return websWrite(wp, buffer);
	}
	
	else if ( !strcmp(name, T("rsIpall"))) {
		#ifdef _MULTIPLE_WLAN_RS_SETTING_
		apmib_get( MIB_WLAN_RS_IP,  (void *)&intaddr);   memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("new Array(\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_WLAN_RS_IP_1,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_WLAN_RS_IP_2,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_WLAN_RS_IP_3,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\","), inet_ntoa(intaddr));
		apmib_get( MIB_WLAN_RS_IP_4,  (void *)&intaddr); memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("\"%s\")"), inet_ntoa(intaddr));
		#else
		apmib_get( MIB_WLAN_RS_IP,  (void *)&intaddr);   memcmp((unsigned char *)&intaddr, "\x0\x0\x0\x0", 4); websWrite(wp, T("new Array(\"%s\")"), inet_ntoa(intaddr));
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("rsPortall")) ) {
		#ifdef _MULTIPLE_WLAN_RS_SETTING_
		apmib_get( MIB_WLAN_RS_PORT, (void *)&intVal);			websWrite(wp, T("new Array(\"%d\","), intVal);
		apmib_get( MIB_WLAN_RS_PORT_1, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_RS_PORT_2, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_RS_PORT_3, (void *)&intVal);		websWrite(wp, T("\"%d\","), intVal);
		apmib_get( MIB_WLAN_RS_PORT_4, (void *)&intVal);		websWrite(wp, T("\"%d\")"), intVal);
		#else
		apmib_get( MIB_WLAN_RS_PORT, (void *)&intVal);			websWrite(wp, T("new Array(\"%d\")"), intVal);
		#endif
		return 0;
	}
	else if ( !strcmp(name, T("rsPasswordall"))) {
		#ifdef _MULTIPLE_WLAN_RS_SETTING_
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD,   (void *)buffer);		websWrite(wp, T("new Array(\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD_1, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD_2, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD_3, (void *)buffer);		websWrite(wp, T("\"%s\","), buffer);
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD_4, (void *)buffer);		websWrite(wp, T("\"%s\")"), buffer);
		#else
		buffer[0]='\0'; apmib_get( MIB_WLAN_RS_PASSWORD,   (void *)buffer);		websWrite(wp, T("new Array(\"%s\")"), buffer);
		#endif
		return 0;
	}

 	else if ( !strcmp(name, T("wdsPskValue"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_WLAN_WDS_WPA_PSK,  (void *)buffer) )
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
		if ( !apmib_get(MIB_WLAN_WDS_WPA_PSK,  (void *)buffer) )
			return -1;
		
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("wanStatus"))) {
		FILE *fp;
		char tmp[2];

		if( (fp=fopen("/tmp/wanStatus", "r")) != NULL){
			fgets(tmp, 2, fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
			return websWrite(wp, T("%s"), "0");
	}
	else if ( !strcmp(name, T("current_lan_ip")) )
	{
		if ( getInAddr(BRIDGE_IF, IP_ADDR, (void *)&intaddr ) )
		return websWrite(wp, T("%s"), inet_ntoa(intaddr) );
	}
 	else if ( !strcmp(name, T("pskValue"))) {
		int i;
		buffer[0]='\0';
		if ( !apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer) )
			return -1;
		for (i=0; i<strlen(buffer); i++)
			buffer[i]='*';
		buffer[i]='\0';
   		return websWrite(wp, buffer);
	}
#ifdef _MSSID_ //vance
 	else if ( !strcmp(name, T("pskValue1"))) {
 			int i;
 			buffer[0]='\0';
 			if ( !apmib_get(MIB_WLAN_WPA_PSK_1,  (void *)buffer) )
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
		if ( !apmib_get(MIB_WPS_PING_CODE,  (void *)buffer) )
			return -1;
   return websWrite(wp, buffer);
	}
	//**************************************** get wps pincode , kyle ****************************************
	else if ( !strcmp(name, T("pinCode"))) {

		FILE *fp1;
		sprintf (buffer, "wpstool %s Pincode > /var/pincode.var", WLAN_IF);
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
		if ( !apmib_get(MIB_WLAN_WPA_PSK,  (void *)buffer) )
			return -1;
//EDX shakim start
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
//EDX shakim end
   		return websWrite(wp, buffer);
	}
#ifdef _MSSID_ //vance
	else if ( !strcmp(name, T("wpsPskValue1"))) {
		buffer[0]='\0';
		if ( !apmib_get(MIB_WLAN_WPA_PSK_1,  (void *)buffer) )
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
		if ( !apmib_get( MIB_WLAN_ACCOUNT_UPDATE_DELAY, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("rsInterval"))) {
		if ( !apmib_get( MIB_WLAN_RS_INTERVAL_TIME, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("accountRsInterval"))) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME, (void *)&intVal) )
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

	else if ( !strcmp(name, T("modelname")) )
	{
		#if defined(_BR6225N_) && (defined(_Customer_) || defined(_Customer_)) && defined(__TARGET_BOARD__) //大中華6225要依照flash hw setting變更model name (這個會顯示在flash.asp)
			apmib_get(MIB_HW_MODEL_NAME,  (void *)buffer);
			return websWrite(wp, buffer);
		#else
			return websWrite(wp, _MODEL_);
		#endif
	}
	else if ( !strcmp(name, T("modulename")))
	{
		#ifdef __TARGET_BOARD__
			system("cat /web/FUNCTION_SCRIPT | grep ^_MODE_= | cut -f 2 -d \\\" > /tmp/modenametmp");
		#else
			system("cat ../web/FUNCTION_SCRIPT | grep ^_MODE_= | cut -f 2 -d \\\" > /tmp/modenametmp");
		#endif

		FILE *ptr;
		ptr=fopen("/tmp/modenametmp","r");
		fgets(buffer,100,ptr);
		fclose(ptr);

		// remove space characters behind buffer[]
		intVal=strlen(buffer);
		while( buffer[intVal]=='\0' || buffer[intVal]==' ' || buffer[intVal]=='\n' || buffer[intVal]=='\t')
		{
			buffer[intVal]='\0';
			intVal=intVal-1;
		}

		system("rm -rf /tmp/modenametmp");
		return websWrite(wp, buffer);
	}
#ifdef UNIVERSAL_REPEATER
	else if ( !strcmp(name, T("repeaterSSID"))) {
		if ( !apmib_get( MIB_REPEATER_SSID, (void *)&buffer) )
			return -1;
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
#endif
   		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("repeaterSSIDAll"))) {       //Robert 2014_01_06
		apmib_get( MIB_REPEATER_SSID,  (void *)buffer) ;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter6(buffer);
		#endif
		websWrite(wp, T("new Array(\"%s\")"), buffer);
		return 0;
	}
#endif

	else if ( !strcmp(name, T("show_reboot"))) {
		FILE *fp;
		char tmp[2];
		int intinfo=1;
		#ifdef __TARGET_BOARD__
		if( (fp=fopen("/tmp/show_reboot_info", "r")) != NULL )
		{
			fgets(tmp,2,fp);
			fclose(fp);
			return websWrite(wp, T("%c"), tmp[0]);
		}
		else
		{
			return websWrite(wp, T("%s"), "0");
		}
		#else
		return websWrite(wp, T("%s"), "1");
		#endif
	}
	else if ( !strcmp(name, T("wiredIPTVAll")) ) {
		int val;
		websWrite(wp, T("new Array(\"0\",\"0\",\"0\",\"0\",\"0\")"), val);
		return 0;
	}
#ifdef _WISP_
	else if ( !strcmp(name, T("stadrv_type")))
	{
//		if ( !apmib_get( MIB_STADRV_TYPE,  (void *)&intVal) ) return -1;
		if ( !apmib_get( MIB_OP_MODE,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("stadrv_ssid")))
	{
		if ( !apmib_get( MIB_STADRV_SSID,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
        return websWrite(wp, buffer);
//		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("stadrv_chan")))
	{
		if ( !apmib_get( MIB_STADRV_CHAN_NUM,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("stadrv_autochan")))
	{
		FILE *fp1;
		#ifndef __TARGET_BOARD__
		system("echo \"11\" > /tmp/stadrv_autochannel");
		#else
		//system("iwpriv wlan0 get_mib channel | cut -f 2 -d : > /tmp/get_wlan0_autochan");
		#endif
		if ((fp1 = fopen("/tmp/stadrv_autochannel","r"))!=NULL) {
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
	else if ( !strcmp(name, T("stadrv_band")))
	{
		if ( !apmib_get( MIB_STADRV_BAND,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("stadrv_encrypttype")))
	{
		if ( !apmib_get( MIB_STADRV_ENCRYPT_TYPE,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
	else if ( !strcmp(name, T("stadrv_weplength")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_LENGTH,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
  else if ( !strcmp(name, T("stadrv_wepformat1")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_FORMAT1,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepformat2")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_FORMAT2,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepformat3")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_FORMAT3,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepformat4")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_FORMAT4,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_defaultkey")))
	{
		if ( !apmib_get( MIB_STADRV_WEP_DEFAULT_KEY,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepkey1")))
	{
		if ( !apmib_get( MIB_STADRV_WEPKEY1,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepkey2")))
	{
		if ( !apmib_get( MIB_STADRV_WEPKEY2,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepkey3")))
	{
		if ( !apmib_get( MIB_STADRV_WEPKEY3,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
   else if ( !strcmp(name, T("stadrv_wepkey4")))
	{
		if ( !apmib_get( MIB_STADRV_WEPKEY4,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
   else if ( !strcmp(name, T("stadrv_wpacipher")))
	{
		if ( !apmib_get( MIB_STADRV_WPA_CIPHER,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
  		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_pskformat")))
	{
		if ( !apmib_get( MIB_STADRV_PSK_FORMAT,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
 		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_pskkey")))
	{
		if ( !apmib_get( MIB_STADRV_PSKKEY,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("wisp_pskkey"))){ 
		if ( !apmib_get( MIB_STADRV_PSKKEY,  (void *)buffer) ) return -1;
		return websWrite(wp, T("new Array(\"%s\")"), CharFilter6(buffer));
	}
 else if ( !strcmp(name, T("stadrv_wantype")))
	{
		if ( !apmib_get( MIB_STADRV_WANTYPE,  (void *)&intVal) ) return -1;
		sprintf(buffer, "%d", intVal );
 		return websWrite(wp, buffer);
	}
   else if ( !strcmp(name, T("stadrv_hostname")))
	{
		if ( !apmib_get( MIB_STADRV_HOSTNAME,  (void *)buffer) ) return -1;
		#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)
			CharFilter2(buffer);
		#endif
		return websWrite(wp, T("%s"), buffer);
	}
	else if ( !strcmp(name, T("stadrv_clonemac")))
	{
		if ( !apmib_get(MIB_STADRV_CLONEMAC,  (void *)buffer) ) return -1;
		return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
	}
   else if ( !strcmp(name, T("stadrv_ip")))
	{
		if ( !apmib_get( MIB_STADRV_IP,  (void *)&intaddr) ) return -1;
  		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
   else if ( !strcmp(name, T("stadrv_mask")))
	{
		if ( !apmib_get( MIB_STADRV_MASK,  (void *)&intaddr) ) return -1;
  		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
   else if ( !strcmp(name, T("stadrv_gateway")))
	{
		if ( !apmib_get( MIB_STADRV_GATEWAY,  (void *)&intaddr) ) return -1;
  		return websWrite(wp, T("%s"), inet_ntoa(intaddr));
	}
#endif
#ifdef _SDMZ_
   else if ( !strcmp(name, T("dmz_mac")))
    {
	   if ( !apmib_get(MIB_DMZ_MAC_ADDR,  (void *)buffer) ) return -1;
	   return websWrite(wp, T("%02x%02x%02x%02x%02x%02x"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
    }
	else if ( !strcmp(name, T("refresh_time")))
	{
		if(!apmib_get( MIB_SUPERDMZ_REFRESHTIME, (void *)&intVal)) return -1;
      return websWrite(wp, T("%d"), intVal);
	}
#endif
#ifdef _CONVERT_IP_ADDR_
        else if ( !strcmp(name, T("convert-ip-rom"))) {
                if ( !apmib_get( MIB_CONVERT_IP_ADDR, (void *)&intaddr) )
                        return -1;
                return websWrite(wp, T("%s"),inet_ntoa(intaddr));
        }
#endif
#ifdef _EZ_QOS_
  else if ( !strcmp(name, T("EZQoS_DownBandWidth")) ) {
	  system("cat /tmp/wget | grep ^Band | cut -d = -f 2 > /tmp/wget2");

	  memset(buffer, 0, sizeof(buffer));

    FILE *ptr;
		if ((ptr = fopen("/tmp/wget2","r"))!=NULL)
			fgets(buffer,20,ptr);
	  fclose(ptr);

		// remove space characters behind buffer[]
		intVal=strlen(buffer);
		while( buffer[intVal]=='\0' || buffer[intVal]==' ' || buffer[intVal]=='\n' || buffer[intVal]=='\t')
		{
			buffer[intVal]='\0';
			intVal=intVal-1;
		}

    system("rm -rf /tmp/wget2");
    return websWrite(wp, buffer);
	}
  else if ( !strcmp(name, T("EZQoS_UpBandWidth")) ) {
		system("cat /tmp/ftpput | grep ^Band | cut -d = -f 2 > /tmp/ftpput2");

		unsigned char buffer2[20];
		memset(buffer2, 0, sizeof(buffer2));

    FILE *ptr;
    if ((ptr = fopen("/tmp/ftpput2","r"))!=NULL)
			fgets(buffer2,20,ptr);
    fclose(ptr);

		// remove space characters behind buffer[]
		intVal=strlen(buffer2);
		while( buffer2[intVal]=='\0' || buffer2[intVal]==' ' || buffer2[intVal]=='\n' || buffer2[intVal]=='\t')
		{
			buffer2[intVal]='\0';
			intVal=intVal-1;
		}

 		system("rm -rf /tmp/ftpput2");
   	return websWrite(wp, buffer2);
    }
/*	else if ( !strcmp(name, T("EZQosConnections"))) {
		if ( !apmib_get( MIB_EZ_QOS_CONNECTIONS, (void *)&intVal) )
			return -1;
		sprintf(buffer, "%d", intVal );
 		return websWrite(wp, buffer);
	}*/
#endif
//******************************2011-08-08
#ifdef _Customer_
	else if ( !strcmp(name, T("EdimaxCHN_Page_Header")) ) {
		websWrite(wp, T("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"));
                websWrite(wp, T("<html>\n"));
                websWrite(wp, T("       <head>\n"));
                websWrite(wp, T("               <link rel=\"stylesheet\" href=\"/style.css\">\n"));
                websWrite(wp, T("               <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\">\n"));
		    websWrite(wp, T("			<meta http-equiv=\"cache-control\" content=\"no-cache,must-revalidate\">\n"));
		    websWrite(wp, T("			<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"));
		    websWrite(wp, T("			<meta http-equiv=\"Expires\" content=\"-1\">\n"));
                websWrite(wp, T("               <title>EDIMAX Technology</title>\n"));
                websWrite(wp, T("               <script type=\"text/javascript\" src=\"/file/multilanguage.var\"></script>\n"));
                websWrite(wp, T("               <script type=\"text/javascript\" src=\"/file/function.js\"></script>\n"));
                websWrite(wp, T("               <script type=\"text/javascript\" src=\"/javascript.js\"></script>\n"));
                websWrite(wp, T("               <script type=\"text/javascript\" src=\"/FUNCTION_SCRIPT\"></script>\n"));
                return 0;
	}
        else if ( !strcmp(name, T("EdimaxCHN_Body_Header")) ) {
                websWrite(wp, T("       </head>\n"));
                websWrite(wp, T("       <body>\n"));
                websWrite(wp, T("               <script>\n"));
                websWrite(wp, T("                       Write_Sitemap("));
                websWrite(wp, T("showText(quickSetup),"));
                websWrite(wp, T("showText(admin),"));
                websWrite(wp, T("showText(WAN),"));  //EDX Steven
                websWrite(wp, T("showText(LAN),"));
                websWrite(wp, T("showText(lanSet),"));
                websWrite(wp, T("showText(firmwareUp),"));
                websWrite(wp, T("showText(nat),"));
                websWrite(wp, T("showText(natPort),"));
                websWrite(wp, T("showText(virtualServer),"));
                websWrite(wp, T("showText(setLanguage),"));
                websWrite(wp, T("showText(firewall),"));
                websWrite(wp, T("showText(wireless),"));
                websWrite(wp, T("showText(basicSet),"));
                websWrite(wp, T("showText(upnp),"));
                websWrite(wp, T("showText(fwDmz),"));
                websWrite(wp, T("showText(accessControl),"));
                websWrite(wp, T("showText(urlBlock),"));
                websWrite(wp, T("showText(DoS),"));
                websWrite(wp, T("showText(SystemStatus),"));
                websWrite(wp, T("showText(SysTimeZone),"));
		    websWrite(wp, T("showText(Configuration),"));
                websWrite(wp, T("showText(MenuEncryption),"));
                websWrite(wp, T("showText(wlAdvSettings),"));
                websWrite(wp, T("showText(QuickSystemTimeZone),"));
                websWrite(wp, T("showText(QuickWAN),"));
                websWrite(wp, T("showText(WPS),"));
                websWrite(wp, T("showText(passSet),"));
                websWrite(wp, T("showText(RemoteM),"));
                websWrite(wp, T("showText(restart),"));
                websWrite(wp, T("showText(qos),"));
                websWrite(wp, T("showText(WISP),"));
                websWrite(wp, T("showText(fwCtrl),"));
                websWrite(wp, T("showText(wlshedule_s2),"));
                websWrite(wp, T("showText(alg),"));
                websWrite(wp, T("showText(statSystem),"));
                websWrite(wp, T("showText(statSecurity),"));	
                websWrite(wp, T("showText(DDNS1),"));
                websWrite(wp, T("showText(DNS),"));
		    websWrite(wp, T("showText(ezqos),"));
		    websWrite(wp, T("showText(iQSetup),"));
		    websWrite(wp, T("showText(wireless_5G),"));
		    websWrite(wp, T("showText(wol),"));
		int fw_val;
                apmib_get( MIB_FIREWALL_ENABLE, (void *)&fw_val);
                websWrite(wp, T("%d"), fw_val);
                websWrite(wp, T(");\n"));
                websWrite(wp, T("               </script>\n"));
                websWrite(wp, T("                       <div id=\"wrapper\">\n"));
                websWrite(wp, T("                               <div id=\"main\">\n"));
                return 0;
        }
		else if ( !strcmp(name, T("EdimaxCHN_Body_Header1")) ) {
                websWrite(wp, T("       </head>\n"));
                websWrite(wp, T("       <body>\n"));
                websWrite(wp, T("               <script>\n"));
                websWrite(wp, T("                       Write_Sitemap("));
                websWrite(wp, T("showText(quickSetup),"));
                websWrite(wp, T("showText(admin),"));
                websWrite(wp, T("showText(WAN),"));  //EDX Steven
                websWrite(wp, T("showText(LAN),"));
                websWrite(wp, T("showText(lanSet),"));
                websWrite(wp, T("showText(firmwareUp),"));
                websWrite(wp, T("showText(nat),"));
                websWrite(wp, T("showText(natPort),"));
                websWrite(wp, T("showText(virtualServer),"));
                websWrite(wp, T("showText(setLanguage),"));
                websWrite(wp, T("showText(firewall),"));
                websWrite(wp, T("showText(wireless),"));
                websWrite(wp, T("showText(basicSet),"));
                websWrite(wp, T("showText(upnp),"));
                websWrite(wp, T("showText(fwDmz),"));
                websWrite(wp, T("showText(accessControl),"));
                websWrite(wp, T("showText(urlBlock),"));
                websWrite(wp, T("showText(DoS),"));
                websWrite(wp, T("showText(SystemStatus),"));
                websWrite(wp, T("showText(SysTimeZone),"));
		    websWrite(wp, T("showText(Configuration),"));
                websWrite(wp, T("showText(MenuEncryption),"));
                websWrite(wp, T("showText(wlAdvSettings),"));
                websWrite(wp, T("showText(QuickSystemTimeZone),"));
                websWrite(wp, T("showText(QuickWAN),"));
                websWrite(wp, T("showText(WPS),"));
                websWrite(wp, T("showText(passSet),"));
                websWrite(wp, T("showText(RemoteM),"));
                websWrite(wp, T("showText(restart),"));
                websWrite(wp, T("showText(qos),"));
                websWrite(wp, T("showText(WISP),"));
                websWrite(wp, T("showText(fwCtrl),"));
                websWrite(wp, T("showText(wlshedule_s2),"));
                websWrite(wp, T("showText(alg),"));
                websWrite(wp, T("showText(statSystem),"));
                websWrite(wp, T("showText(statSecurity),"));	
                websWrite(wp, T("showText(DDNS1),"));
                websWrite(wp, T("showText(DNS),"));
		    websWrite(wp, T("showText(ezqos),"));
		    websWrite(wp, T("showText(iQSetup),"));
		    websWrite(wp, T("showText(wireless_5G),"));
		    websWrite(wp, T("showText(wol),"));
		int fw_val;
                apmib_get( MIB_FIREWALL_ENABLE, (void *)&fw_val);
                websWrite(wp, T("%d"), fw_val);
                websWrite(wp, T(");\n"));
                websWrite(wp, T("               </script>\n"));
                websWrite(wp, T("                       <div id=\"wrapper\">\n"));
                websWrite(wp, T("                               <div id=\"main\">\n"));
                return 0;
	}
        else if ( !strcmp(name, T("EdimaxCHN_Body_Middle")) ) {
                websWrite(wp, T("                               </div>\n"));
                websWrite(wp, T("                               <div id=\"help\">\n"));
                return 0;
        }
	else if ( !strcmp(name, T("EdimaxCHN_Body_Tail")) ) {
                websWrite(wp, T("                               </div>\n"));
                websWrite(wp, T("                       </div>\n"));
                websWrite(wp, T("               </div>\n"));
                websWrite(wp, T("       </body>\n"));
                websWrite(wp, T("</html>\n"));
                return 0;
        }
        else if ( !strcmp(name, T("EdimaxCHN_Date")) ) {
                FILE *ptr_date;
                char string_date[35];
                if( (ptr_date=popen("date", "r"))!=NULL )
                {
                        fgets(string_date, 34, ptr_date);
                        websWrite(wp, string_date);
                        pclose(ptr_date);
                }
                else
                        websWrite(wp, T("N/A"));
                return 0;
        }
#endif

//******************************
#ifdef _GENERNAL_RANDOMNUMBER_
	else if ( !strcmp(name, T("generate-random-num"))){
		websWrite(wp,T("%d"),generateRandomNum());
		return 0;
	}
#endif
#ifdef _MCAST_RATE_SUPPORT_
	else if ( !strcmp (name, T("mcastenable"))){
		if ( !apmib_get(MIB_MCAST_RATE_ENABLED,  (void *)&intVal) )
			return -1;
		sprintf (buffer, "%d", intVal);
		return websWrite(wp, buffer);
	}

	else if ( !strcmp(name, T("mcastinfo"))) {
		if ( !apmib_get(MIB_MCAST_RATE,  (void *)&intVal) )
			return -1;
		sprintf (buffer, "%d", intVal);
		return websWrite(wp, buffer);
	}
#endif

#ifdef _IPV6_DSLITE_SUPPORT_
        else if (!strcmp(name, T("IPv6CGNIP"))) {
                apmib_get(MIB_IPV6_CGN_IP, (void *)buffer);
                if (!memcmp((unsigned char *)&buffer, "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0", 16))
                        return websWrite(wp, T(""));
                return websWrite(wp, T("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x"), buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
        }
#endif //_IPV6_DSLITE_SUPPORT_
#ifdef _IPV6_6RD_SUPPORT_
        else if (!strcmp(name, T("IPv66rdServer"))) {
                apmib_get(MIB_IPV6_6RD_SERVER, (void *)buffer);
                if (buffer[0] == '\0')
                        return websWrite(wp, T("6rd.comcast.net"));
                return websWrite(wp, T("%s"), buffer);
        }
        else if (!strcmp(name, T("IPv66rdPrefix"))) {
                apmib_get(MIB_IPV6_6RD_PREFIX, (void *)buffer);
                if (buffer[0] == '\0')
                        return websWrite(wp, T("2001:55c"));
                return websWrite(wp, T("%s"), buffer);
        }
#endif //_IPV6_6RD_SUPPORT_
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
			else if ( !strcmp(name, T("wlanSchEnabled_24G")) ) {
				apmib_get(MIB_WLAN_SCH_ENABLED, (void *)&intVal);
				return websWrite(wp, T("%d"), intVal);
			}
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
	else if ( !strcmp(name, T("openvpnENABLED")) ) {
		int val;
		apmib_get( MIB_OPENVPN_ENABLED, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	else if ( !strcmp(name, T("openvpnTotalAccountNum")) ) {
		int val;
		apmib_get( MIB_OPENVPN_USER_NUM, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Interface -> Type 0:TAP 1:TUN
	else if ( !strcmp(name, T("openvpnInterType")) ) {
		int val;
		apmib_get( MIB_OPENVPN_INTER_TYPE, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Protocol -> 0:TCP 1:UDP
	else if ( !strcmp(name, T("openvpnProtocol")) ) {
		int val;
		apmib_get( MIB_OPENVPN_PROTOCOL, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Server Port -> Default:443
	else if ( !strcmp(name, T("openvpnPORT")) ) {
		int val;
		apmib_get( MIB_OPENVPN_SER_PORT, (void *)&val); 	if (val == 0) {val = 443;} 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Authorization Mode -> 0:TLS 1:Static key
	else if ( !strcmp(name, T("openvpnAuthMode")) ) {
		int val;
		apmib_get( MIB_OPENVPN_AUTH_MODE, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//UsernamePassword Auth. Only -> 0:NO 1:YES
	else if ( !strcmp(name, T("openvpnOnlyAuthUser")) ) {
		int val;
		apmib_get( MIB_OPENVPN_ONLY_AUTH_USER, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//VPN Subnet/Netmask (TUN)
	else if ( !strcmp(name, T("openvpnSubnetNetmask")) ) {
		struct in_addr subnet, netmask;
		apmib_get( MIB_OPENVPN_SUBNET, (void *)&subnet); 
		websWrite(wp, T("new Array(\"%s\","), inet_ntoa(subnet));
		apmib_get( MIB_OPENVPN_NETMASK, (void *)&netmask);
		websWrite(wp, T("\"%s\")"), inet_ntoa(netmask));
		return 0;
	}
	//VPN START-IP/END-IP (TAP)
	else if ( !strcmp(name, T("openvpnStartAndEndIP")) ) {
		struct in_addr startIP, endIP;
		apmib_get( MIB_OPENVPN_START_IP, (void *)&startIP);
		websWrite(wp, T("new Array(\"%s\","), inet_ntoa(startIP));
		apmib_get( MIB_OPENVPN_END_IP, (void *)&endIP);
		websWrite(wp, T("\"%s\")"),  inet_ntoa(endIP));
		return 0;
	}
	//Direct Client Traffic -> 0:NO 1:YES
	else if ( !strcmp(name, T("openvpnRedirectTraffic")) ) {
		int val;
		apmib_get( MIB_OPENVPN_REDIRECT_TRAFFIC, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Push To LAN To Clients -> 0:NO 1:YES
	else if ( !strcmp(name, T("openvpnPushLAN")) ) {
		int val;
		apmib_get( MIB_OPENVPN_PUSH_LAN, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	//Respond DNS -> 0:NO 1:YES
	else if ( !strcmp(name, T("openvpnResDNS")) ) {
		int val;
		apmib_get( MIB_OPENVPN_RES_DNS, (void *)&val); 	websWrite(wp, T("new Array(\"%d\")"), val);
		return 0;
	}
	// Note: openvpnClientConf value
	// 1=(EDIMAX_redirect_all_traffic.ovpn exist)
	// 2=(EDIMAX_access_LAN_only.ovpn exist)
	// 4=(EDIMAX-client.ovpn exist)
	//
	else if ( !strcmp(name, T("openvpnClientConf")) ) {
		int val=0;
		FILE *fp;
		if ((fp = fopen("/tmp/openvpn/EDIMAX_redirect_all_traffic.ovpn","r"))!=NULL) {
			fclose(fp);
			val=val+1;
		}
		if ((fp = fopen("/tmp/openvpn/EDIMAX_access_LAN_only.ovpn","r"))!=NULL) {
			fclose(fp);
			val=val+2;
		}
		if ((fp = fopen("/tmp/openvpn/EDIMAX-client.ovpn","r"))!=NULL) {
			fclose(fp);
			val=val+4;
		}
		websWrite(wp, T("%d"), val);
		return 0;
	}
	else if ( !strcmp(name, T("openvpnCaCrt"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/ca.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/ca.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnDHPem"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/dh.pem","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/dh.pem","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnServerCrt"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/server.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/server.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnServerKey"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/server.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/server.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnClientCrt"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/client.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/client.crt","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnClientKey"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/client.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/client.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("openvpnStaticKey"))) {
		FILE *fp1;
		if ((fp1 = fopen("/tmp/vpnkeys_user/ta.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		else if ((fp1 = fopen("/tmp/openvpn/keys/def/ta.key","r"))!=NULL) {
			while (fgets(buffer, 100, fp1) > 0)
				websWrite(wp, T("%s"), buffer);
		fclose(fp1);
		}
		return 0;
	}
	else if ( !strcmp(name, T("vpn_setup_submit"))) {
		FILE *fp;
		char tmp[10];

		if( (fp=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
		{
			fgets(tmp,5,fp);
			fclose(fp);
			//printf("%s",tmp);
			return websWrite(wp, T("%d"), atoi(tmp));
		}
		else
		{
			//printf("%s\n","0");
			return websWrite(wp, T("%s"), "0");
		}
	}
	else if ( !strcmp(name, T("vpn_setup_dialogBox"))) {
		FILE *fp;
		char tmp[10];

		if( (fp=fopen("/tmp/vpn_setup_dialogBox", "r")) != NULL )
		{
			fgets(tmp,5,fp);
			fclose(fp);
			//printf("%s",tmp);
			return websWrite(wp, T("%d"), atoi(tmp));
		}
		else
		{
			//printf("%s\n","0");
			return websWrite(wp, T("%s"), "0");
		}
	}
	else if ( !strcmp(name, T("vpn_enabled_state"))) {
		// Note: vpn_enabled_state value  "1~3 enabled openvpn service" "4- start openvpn server"
		// 0=(enabled check ok or disabled) , 1=(wan port not connected) , 2=(non-public ip) , 3=(no expected error)
		// 4=(start openvpn server success) , 5=(start openvpn server failure)
		FILE *fp;
		char tmp[10];
		int tmpVal;
		if( (fp=fopen("/tmp/vpn_enabled_state", "r")) != NULL )
		{
			fgets(tmp,5,fp);
			fclose(fp);
			tmpVal=atoi(tmp);
			
			if (tmpVal <= 3)
			{
				system("echo 0 > /tmp/vpn_enabled_state");
			}
			return websWrite(wp, T("%d"), tmpVal);
		}
		else
		{
			//printf("%s\n","0");
			return websWrite(wp, T("%s"), "0");
		}
	}
	else if ( !strcmp(name, T("enable_Edimax_DDNS"))) {
		FILE *fp;
		char tmp[10];

		if( (fp=fopen("/tmp/enable_Edimax_DDNS", "r")) != NULL )
		{
			fgets(tmp,5,fp);
			fclose(fp);
			//printf("%s",tmp);
			return websWrite(wp, T("%d"), atoi(tmp));
		}
		else
		{
			//printf("%s\n","0");
			return websWrite(wp, T("%s"), "0");
		}
	}
	else if ( !strcmp(name, T("openvpnLoginAccountTable")) ) {
		#ifdef __TARGET_BOARD__
		FILE *f;
		int val=1,tmpVal,openvpnTotalUser=8;
		char Path[100]="",charVal[5]="";
		char PathHead[] = "/tmp/openvpn/account/user";
		char PathTail1[] = "/LOGIN";
		char PathTail2[] = "/IP";

		websWrite(wp, T("%s"), "new Array(");
		for(val=1;val<=openvpnTotalUser;val++)
		{
			tmpVal=0;
			sprintf(charVal, "%d", val);

			Path[0]='\0';
			strcat(Path,PathHead);
			strcat(Path,charVal);
			strcat(Path,PathTail1);

			f=fopen(Path,"r");
			while(1)
			{
				fscanf(f,"%s",&buffer);
				if(feof(f)) break;
				websWrite(wp, T("\"%s\""), buffer);
			}
			fclose(f);
			websWrite(wp, T("%s"),",\"");

			Path[0]='\0';
			strcat(Path,PathHead);
			strcat(Path,charVal);
			strcat(Path,PathTail2);

			f=fopen(Path,"r");
			while(1)
			{
				fscanf(f,"%s",&buffer);
				if(feof(f)) break;
				if(tmpVal != 0) websWrite(wp, T("%s")," ");
				websWrite(wp, T("%s"), buffer);
				tmpVal=1;
			}
			fclose(f);
			
			if (val!=8)
			{
				websWrite(wp, T("%s"),"\",");
			}
			else
			{
				websWrite(wp, T("%s"),"\"");
			}
		}
		websWrite(wp, T("%s"), ")");
		#else
		websWrite(wp, T("%s"), "new Array(\"0\",\"\",\"0\",\"\",\"0\",\"\",\"0\",\"\",\"0\",\"\",\"0\",\"\",\"0\",\"\",\"0\",\"\")");
		#endif
		return 0;
	}
#endif

#ifdef _DUAL_WAN_IGMP_
	else if (!strcmp(name, T("DUAL_WAN_IGMP"))) {
		apmib_get(MIB_DUAL_WAN_IGMP, (void *)&intVal);
		return websWrite(wp, T("%d"), intVal);
	}
#endif

	else{
		return websWrite(wp, "");
	}
 	return -1;
}

/////////////////////////////////////////////////////////////////////////////
#ifndef _BOA_
int getIdValue(id, eid) {
	char_t buffer[50];
	int val;
	if ( !apmib_get( id, (void *)&val) )
		return -1;
	sprintf(buffer, "%d", val);
	ejSetResult(eid, buffer);
	return 0;
}
#endif
/////////////////////////////////////////////////////////////////////////////
#ifndef _BOA_
int getIndex(int eid, webs_t wp, int argc, char_t **argv)
#else
int getIndex(char *varName, char *indexValue)
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
/*
	val = oem_getIndex(eid, wp, name);
	if (val == 0)
		return val;
*/
//************************************* System setting *************************************

	if ( !strcmp(name, T("licence")) ) {
 		if ( !apmib_get( MIB_LICENCE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#ifdef _ADV_CONTROL_
	else if ( !strcmp(name, T("manualavdcontrol"))) {
		if ( !apmib_get( MIB_ADVCONTROL_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#endif
	#if defined(_DHCP_SWITCH_)    //EDX patrick add
	else if ( !strcmp(name, T("dhcpswitch"))) {
		if ( !apmib_get( MIB_DHCP_SWITCH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("dhcpswitch-5g"))) {
		if ( !apmib_get( MIB_DHCP_SWITCH_5G, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#endif
#ifdef _AUTO_FW_UPGRADE_
        else if ( !strcmp(name, T("isServerAlive"))) {
                sprintf(buffer, "%d", isServerAlive);
                ejSetResult(eid, buffer);
                return 0;
        }
#endif
	else if ( !strcmp(name, T("enable_dns_proxy"))) {	
		#ifdef _DNS_PROXY_
		if ( !apmib_get( MIB_DNS_PROXY_ENABLE, (void *)&val) )
			return -1;
		#else
		val = 0;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("DNSProxyrulesNum")) ) {
		#ifdef _EDIT_DNSPROXYURL_ 
		if ( !apmib_get( MIB_PROXYURL_NUM, (void *)&val) )
			return -1;
		#else
		val = 5;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enable_user_define_rules"))) {
		#ifdef _EDIT_DNSPROXYURL_ 
		if ( !apmib_get( MIB_PROXYURL_CUSTOM_ENABLED, (void *)&val) )
			return -1;
		#else
		val = 0;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("DNSProxyTempRulesNum"))) {
		char DomainName[50],IPaddress[20];
		val=0;
		#ifdef __TARGET_BOARD__
		FILE *f=fopen("/tmp/basehosts","r");
			if (f) {
				while(1)
				{
					fscanf(f,"%s %s",&IPaddress,&DomainName);
					if(feof(f)) break;
					if(IPaddress[0] != '#')
					{
						val=val+1;
					}
				}
				fclose(f);
			}
		#else
		val=40;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	#ifdef _IQv2_
	else if ( !strcmp(name, T("wizmode"))) {
		#ifdef __TARGET_BOARD__
		if ( !apmib_get( MIB_WIZ_MODE, (void *)&val) )
			return -1;
		#else
			val=2;
		#endif

		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("isRestDefault"))) {
		if ( !apmib_get( MIB_IS_RESET_DEFAULT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("IQsetupDisable"))) {
		#ifdef __TARGET_BOARD__
		if ( !apmib_get( MIB_IQSET_DISABLE, (void *)&val) ) 
			return -1;
		#else
			val=1;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _WIFI_ROMAING_
	else if ( !strcmp(name, T("SINGLESIGNON"))) {
		if ( !apmib_get( MIB_SINGLESIGNON, (void *)&val) ) 
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("SINGLESIGNON_FREEZE"))) {
		if ( !apmib_get( MIB_SINGLESIGNON_FREEZE, (void *)&val) ) 
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("DeviceSinglesignon"))) {
                system("gddbc -g /airfores/singlesignon  > /tmp/DevSinglesignon");
		FILE *fp;
		char tmp[5];
		int Sing = 0;
		if((fp = fopen("/tmp/DevSinglesignon","r"))!= NULL){
			fgets(tmp,sizeof(tmp)-1,fp);
			fclose(fp);
			tmp[strlen(tmp)-1]=0;
			Sing = atoi(tmp);
			sprintf(buffer, "%d", Sing);
		}
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("MASTERIP"))) {
                system("gddbc -g /airfores/singlesignoninfo/connmaster/ip  > /tmp/MIP");
		FILE *fp;
		char tmp[20];
		if((fp = fopen("/tmp/MIP","r"))!= NULL){
			fgets(tmp,sizeof(tmp)-1,fp);
			fclose(fp);
			tmp[strlen(tmp)-1]=0;
			sprintf(buffer, "%s", tmp);
		}else{
			sprintf(buffer, "%s", "edimaxext.setup");
		}
		ejSetResult(eid, buffer);
		return 0;

	}
#endif
	else if ( !strcmp(name, T("showConnectResult2"))) {
		val=0;
		FILE *fp;
		#ifdef __TARGET_BOARD__
		if((fp = fopen("/tmp/showConnectResult2","r"))!= NULL){
			val=1;
			fclose(fp);
		}
		#else
			val=1;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("showConnectResult5"))) {
		val=0;
		FILE *fp;
		#ifdef __TARGET_BOARD__
		if((fp = fopen("/tmp/showConnectResult5","r"))!= NULL){
			val=1;
			fclose(fp);
		}
		#else
			val=1;
		#endif
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
	else if ( !strcmp(name, T("igmpEnable"))) {
		if ( !apmib_get( MIB_IGMP_PROXY_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
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
#ifdef _TIMESERVER_
	else if ( !strcmp(name, T("time_type"))) {
		if ( !apmib_get( MIB_TIME_TYPE,  (void *)&val) )	return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ntpsconfig"))) {
		if ( !apmib_get( MIB_NTPS_CONFIG,  (void *)&val) )	return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
	//EDX Timmy for WISP and root AP had same LAN IP 
	else if ( !strcmp(name, T("IPcheck")) ) {
		FILE *fp;
		char tmp[10];
		if((fp = fopen("/tmp/NewLanIP","r"))!= NULL){
			fgets(tmp,sizeof(tmp)-1,fp);
			fclose(fp);
			tmp[strlen(tmp)-1]=0;
			sprintf(buffer, "%s", tmp);
			system("rm -f /tmp/NewLanIP");
		}else
			sprintf(buffer, "%d", 0);	
		printf("xxxxxxxxxxxxxxxx%sxxxxxxxxxxxxxx\n",buffer);
		ejSetResult(eid, buffer);
		return 0;
		
	}
	//EDX Timmy end
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
		if ( !apmib_get( MIB_WLAN_WDS_ENCRYPT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsWpaAuth"))) {
		if ( !apmib_get( MIB_WLAN_WDS_WPA_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsWpaCipher"))) {
		int iTkip, iAes;
		if ( !apmib_get( MIB_WLAN_WDS_WPA_CIPHER_SUITE, (void *)&iTkip) )
			return -1;
		if ( !apmib_get( MIB_WLAN_WDS_WPA2_CIPHER_SUITE, (void *)&iAes) )
			return -1;
		if (iTkip==WPA_CIPHER_TKIP && iAes==0) val=1;
		else if (iTkip==0 && iAes==WPA_CIPHER_AES) val=2;
		else if (iTkip==WPA_CIPHER_TKIP && iAes==WPA_CIPHER_AES) val=3;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wdsPskFormat"))) {
		if ( !apmib_get( MIB_WLAN_WDS_WPA_PSK_FORMAT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
  else if ( !strcmp(name, T("disProtection"))) {
		if ( !apmib_get( MIB_WLAN_PROTECTION_DISABLED, (void *)&val) )
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
		if ( !apmib_get( MIB_WLAN_NAT25_MAC_CLONE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("band")) ) {
		if ( !apmib_get( MIB_WLAN_BAND, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("fixTxRate")) ) {
		if ( !apmib_get( MIB_WLAN_FIX_RATE, (void *)&val) )
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
			if ( !apmib_get( MIB_WLAN_AC_SWITCH, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
	else if ( !strcmp(name, T("wlanac1sw")) ) {
				if ( !apmib_get( MIB_WLAN_AC1_SWITCH, (void *)&val) )
					return -1;
				sprintf(buffer, "%d", val);
				ejSetResult(eid, buffer);
				return 0;
			}
	else if ( !strcmp(name, T("wlanac2sw")) ) {
				if ( !apmib_get( MIB_WLAN_AC2_SWITCH, (void *)&val) )
					return -1;
				sprintf(buffer, "%d", val);
				ejSetResult(eid, buffer);
				return 0;
			}
	else if ( !strcmp(name, T("wlanac3sw")) ) {
				if ( !apmib_get( MIB_WLAN_AC3_SWITCH, (void *)&val) )
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
	else if ( !strcmp(name, T("wlanRateMode")) ) {
		if ( !apmib_get( MIB_WLAN_RATE_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanCts")) ) {
		if ( !apmib_get( MIB_WLAN_CTS, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanBurst")) ) {
		if ( !apmib_get( MIB_WLAN_BURST, (void *)&val) )
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
//			iface = WLAN_IF;
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

#if 0
//#ifdef _Customer_
			FILE *fp;
			char c='0';
		system("cat /var/flash.inc | grep -i WAN_MODE | cut -d= -f2 > /var/current_wan_type.var");
		if ((fp = fopen("/var/current_wan_type.var","r"))!=NULL) {
			c=fgetc(fp);
			fclose(fp);
		}else
			c='0';
		sprintf(buffer, "%c", c);
		ejSetResult(eid, buffer);
#else
		if ( !apmib_get( MIB_WAN_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
#endif
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
	else if ( !strcmp(name, T("EZQosNum")) ) {	//EZ QoS
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
	else if ( !strcmp(name, T("sitesurveyed")) ) {
		sprintf(buffer, "%d", sitesurveyed);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("sitesurveyed5G")) ) {
		sprintf(buffer, "%d", sitesurveyed5G);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("wpsEnable"))) {
		if ( !apmib_get( MIB_WPS_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsDisplayKey"))) {
		if ( !apmib_get( MIB_WPS_DISPLAY_KEY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsProxyEnable"))) {
		if ( !apmib_get( MIB_WPS_PROXY_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigMode"))) {
		if ( !apmib_get( MIB_WPS_INTERNAL_REG, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigType"))) {
		if ( !apmib_get( MIB_WPS_CONFIG_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpsConfigStatus"))) {
		if ( !apmib_get( MIB_WPS_CONFIG_STATUS, (void *)&val) )
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
		if ( !apmib_get( MIB_1X_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("channel")) ) {
		if ( !apmib_get( MIB_WLAN_CHAN_NUM, (void *)&chan) )
			return -1;
		sprintf(buffer, "%d", chan);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("regDomain")) ) {
		if ( !apmib_get( MIB_HW_REG_DOMAIN, (void *)&domain) )
			return -1;
		sprintf(buffer, "%d", (int)domain);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wep")) ) {
		if ( !apmib_get( MIB_WLAN_WEP, (void *)&wep) )
			return -1;
		sprintf(buffer, "%d", (int)wep);
		ejSetResult(eid, buffer);
   	    	return 0;
	}
#ifdef _MSSID_
	else if ( !strcmp(name, T("wep1")) ) {
		if ( !apmib_get( MIB_WLAN_WEP_1, (void *)&wep) )
			return -1;
		sprintf(buffer, "%d", (int)wep);
		ejSetResult(eid, buffer);
   	    	return 0;
	}
#endif
	else if ( !strcmp(name, T("defaultKeyId")) ) {
		if ( !apmib_get( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&val) )
			return -1;
		val++;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanSetTx")) ) {
		if ( !apmib_get( MIB_WLAN_SET_TX, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("keyType")) ) {
		if ( !apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		 ejSetResult(eid, buffer);
		return 0;
	}
  	else if ( !strcmp(name, T("authType"))) {
		if ( !apmib_get( MIB_WLAN_AUTH_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val) ;
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("operRate"))) {
		if ( !apmib_get( MIB_WLAN_SUPPORTED_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("basicRate"))) {
		if ( !apmib_get( MIB_WLAN_BASIC_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", (int)val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("preamble"))) {
		if ( !apmib_get( MIB_WLAN_PREAMBLE_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("hiddenSSID"))) {
		if ( !apmib_get( MIB_WLAN_HIDDEN_SSID, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _MSSID_ //vance 2008.12.30
	else if ( !strcmp(name, T("hiddenSSID1"))) {
			if ( !apmib_get( MIB_WLAN_HIDDEN_SSID_1, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
#endif
	// RexHua
	else if ( !strcmp(name, T("wlanTurbo"))) {
		if ( !apmib_get( MIB_WLAN_TURBO, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("NfixTxRate"))) {
		if ( !apmib_get( MIB_WLAN_N_FIX_RATE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("NChanWidth"))) {
		if ( !apmib_get( MIB_WLAN_N_CHANNEL_WIDTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanWmm"))) {
		if ( !apmib_get( MIB_WLAN_WMM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#if defined(HOME_GATEWAY) || defined(_AP_WLAN_NOFORWARD_)
		//kyle
		else if ( !strcmp(name, T("noForward"))) {
		if ( !apmib_get( MIB_WLAN_NOFORWARD, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

	else if ( !strcmp(name, T("wmFilterNum"))) {
		if ( !apmib_get( MIB_WLAN_AC_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanDisabled"))){
		if ( !apmib_get( MIB_WLAN_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAcNum")) ) {
		if ( !apmib_get( MIB_WLAN_AC_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAcEnabled"))) {
		if ( !apmib_get( MIB_WLAN_AC_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#ifdef _MULTIPLE_WLAN_ACCESS_CONTROL_ //vance 2008.10.24
	//SSID 2
	else if ( !strcmp(name, T("wlanAc2Num")) ) {
		if ( !apmib_get( MIB_WLAN_AC2_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc2Enabled"))) {
		if ( !apmib_get( MIB_WLAN_AC2_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	//SSID 3
	else if ( !strcmp(name, T("wlanAc3Num")) ) {
		if ( !apmib_get( MIB_WLAN_AC3_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc3Enabled"))) {
		if ( !apmib_get( MIB_WLAN_AC3_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	//SSID 4
	else if ( !strcmp(name, T("wlanAc4Num")) ) {
		if ( !apmib_get( MIB_WLAN_AC4_NUM, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wlanAc4Enabled"))) {
		if ( !apmib_get( MIB_WLAN_AC4_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#endif
	else if ( !strcmp(name, T("rateAdaptiveEnabled"))) {
		if ( !apmib_get( MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("netType"))) {
		if ( !apmib_get( MIB_WLAN_NETWORK_TYPE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("iappDisabled"))) {
		if ( !apmib_get( MIB_WLAN_IAPP_DISABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("block"))) {
		if ( !apmib_get( MIB_WLAN_BLOCK_RELAY, (void *)&val) )
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
		if ( !apmib_get( MIB_WLAN_BAND, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
//-----------------------------------Erwin Add 28.08----------------------------------------------
	else if ( !strcmp(name, T("apMode"))) {
		if ( !apmib_get( MIB_AP_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _AUTO_CHAN_SELECT_
	else if ( !strcmp(name, T("ssid2scan_fail"))) {
		sprintf(buffer, "%d", ssid2scan_fail);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("ssid2scan_fail5g"))) {
		sprintf(buffer, "%d", ssid2scan_fail5g);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("connect_key_is_wep"))) {
		sprintf(buffer, "%d", wep_used);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("connectIF"))) {
		sprintf(buffer, "%d", connectIF);
		ejSetResult(eid, buffer);
		return 0;
	}
#endif

	else if ( !strcmp(name, T("secMode"))) {
		if ( !apmib_get( MIB_SECURITY_MODE, (void *)&val) )
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
	#if defined (_AUTO_DAYLIGHT_ENABLE_)
	else if ( !strcmp(name,T("AutoDayLightEnable"))) {
		if ( !apmib_get( MIB_AUTO_DAYLIGHT_ENABLE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	#endif		
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
		if ( !apmib_get( MIB_SECURITY_MODE, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
#ifdef _MSSID_ //vance
	else if ( !strcmp(name, T("encrypt1"))) {
			if ( !apmib_get( MIB_SECURITY_MODE_1, (void *)&val) )
				return -1;
			sprintf(buffer, "%d", val);
			ejSetResult(eid, buffer);
			return 0;
		}
#endif
	else if ( !strcmp(name, T("enable1X"))) {
		if ( !apmib_get( MIB_WLAN_ENABLE_1X, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enableSuppNonWpa"))) {
		if ( !apmib_get( MIB_WLAN_ENABLE_SUPP_NONWPA, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("suppNonWpa"))) {
		if ( !apmib_get( MIB_WLAN_SUPP_NONWPA, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpaAuth"))) {
		if ( !apmib_get( MIB_WLAN_WPA_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("wpaCipher"))) {
		int iTkip, iAes;
		if ( !apmib_get( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iTkip) )
			return -1;
		if ( !apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iAes) )
			return -1;
		if (iTkip==WPA_CIPHER_TKIP && iAes==0) val=1;
		else if (iTkip==0 && iAes==WPA_CIPHER_AES) val=2;
		else if (iTkip==WPA_CIPHER_TKIP && iAes==WPA_CIPHER_AES) val=3;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("pskFormat"))) {
		if ( !apmib_get( MIB_WLAN_WPA_PSK_FORMAT, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}

	else if ( !strcmp(name, T("accountRsEnabled"))) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("accountRsUpdateEnabled"))) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_UPDATE_ENABLED, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("enableMacAuth"))) {
		if ( !apmib_get( MIB_WLAN_ENABLE_MAC_AUTH, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("rsRetry")) ) {
		if ( !apmib_get( MIB_WLAN_RS_RETRY, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val);
		ejSetResult(eid, buffer);
		return 0;
	}
	else if ( !strcmp(name, T("accountRsRetry")) ) {
		if ( !apmib_get( MIB_WLAN_ACCOUNT_RS_RETRY, (void *)&val) )
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
		id = MIB_REPEATER_ENABLED;
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

#ifdef _TIMESERVER_
	else if ( !strcmp(name, T("ServerAuto"))) {
		if ( !apmib_get( MIB_SERVER_AUTO, (void *)&val) )
			return -1;
		sprintf(buffer, "%d", val );
		ejSetResult(eid, buffer);
		return 0;
	}
#endif
	else{
		ejSetResult(eid, "");
		return 0;
	}
		return -1;
}

#ifdef HOME_GATEWAY
/////////////////////////////////////////////////////////////////////////////
int isConnectPPP()
{
	struct stat status;

	if ( stat("/etc/ppp/link0", &status) < 0)
		return 0;

	return 1;
}
#endif
