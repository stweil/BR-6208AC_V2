 /*
 *      Web server handler routines for TCP/IP stuffs
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: fmtcpip.c,v 1.2 2005/05/13 07:53:53 tommy Exp $
 *
 */

/*-- System inlcude files --*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <time.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <net/if.h>
#ifndef _BOA_
#include "../um.h"
#endif
#include <sys/types.h>

/*-- Local inlcude files --*/
#include "../webs.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"

static char superName[MAX_NAME_LEN]={0};//, superPass[MAX_NAME_LEN]={0};
//static char userName[MAX_NAME_LEN]={0}, userPass[MAX_NAME_LEN]={0};
#define DEFAULT_GROUP           T("administrators")
#define ACCESS_URL              T("/")
#ifndef __TARGET_BOARD__
#define _LITTLE_ENDIAN_
#endif

#define _DHCPD_PROG_NAME	T("udhcpd")
#define _DHCPD_PID_PATH		T("/var/run")
#define _DHCPC_PROG_NAME	T("udhcpc")
#define _DHCPC_PID_PATH		T("/etc/udhcpc")
#define _PATH_DHCPS_LEASES	T("/var/lib/misc/udhcpd.leases")


/*-- Macro declarations --*/
#ifdef _LITTLE_ENDIAN_
#define ntohdw(v) ( ((v&0xff)<<24) | (((v>>8)&0xff)<<16) | (((v>>16)&0xff)<<8) | ((v>>24)&0xff) )

#else
#define ntohdw(v) (v)
#endif

#define RECONNECT_MSG(url) { \
	websHeader(wp); \
	websWrite(wp, T("<head><link rel=\"stylesheet\" href=\"set.css\"></head>\n")); \
	websWrite(wp, T("<body class=\"background\"><blockquote><h4 class=\"textcolor\">Change setting successfully!<BR><BR>"\
                "<form><input type=button value=\"  OK  \" OnClick=window.location.replace(\"%s\")></form></blockquote></body>"), url);\
	websFooter(wp); \
	websDone(wp, 200); \
}

/*-- Forward declarations --*/
#ifdef HOME_GATEWAY
static int getOneDhcpClient(char **ppStart, unsigned long *size, char *ip, char *mac, char *liveTime);
#endif

static int getPid(char *filename);

int lanipChanged = 0; // Tommy Added
int main_menu=0;
int sub_menu=0;
int third_menu=0;

#ifdef _OK_MSG_CHARSET_ISO_8859_1_
char charset[20]="iso-8859-1";
#else
char charset[20]="utf-8";
#endif

#ifdef HOME_GATEWAY
//////////////////////////////////////////////////////////////////////////////
static int getOneDhcpClient(char **ppStart, unsigned long *size, char *ip, char *mac, char *liveTime)
{
	struct dhcpOfferedAddr {
			u_int32_t expires;      /* host order */
        	u_int32_t yiaddr;       /* network order */
        	u_int8_t chaddr[28];
	};

	struct dhcpOfferedAddr entry;

	if ( *size < sizeof(entry) )
		return -1;
	entry = *((struct dhcpOfferedAddr *)*ppStart);
	*ppStart = *ppStart + sizeof(entry);
	*size = *size - sizeof(entry);

	if (entry.expires == 0)
		return 0;

	strcpy(ip, inet_ntoa(*((struct in_addr *)&entry.yiaddr)) );

	snprintf(mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x",
			entry.chaddr[0],entry.chaddr[1],entry.chaddr[2],entry.chaddr[3],
			entry.chaddr[4], entry.chaddr[5]);


	snprintf(liveTime, 10, "%lu", (unsigned long)ntohl(entry.expires));

	return 1;
}
static int getOneDhcpClientinfo(char **ppStart, unsigned long *size, char *ip, char *mac, char *liveTime,char *hostname)
{
	
	struct dhcpOfferedAddr {
		u_int32_t expires;      /* host order */
       	u_int32_t yiaddr;       /* network order */
       	u_int8_t chaddr[6];
       	u_int8_t host[22];
	};
	struct dhcpOfferedAddr entry;

	if ( *size < sizeof(entry) )
		return -1;
	entry = *((struct dhcpOfferedAddr *)*ppStart);
	*ppStart = *ppStart + sizeof(entry);
	*size = *size - sizeof(entry);

	if (entry.expires == 0)
		return 0;
	strcpy(ip, inet_ntoa(*((struct in_addr *)&entry.yiaddr)) );
	snprintf(mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x",
			entry.chaddr[0],entry.chaddr[1],entry.chaddr[2],entry.chaddr[3],
			entry.chaddr[4], entry.chaddr[5]);
	sprintf(hostname,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",entry.host[0],entry.host[1],entry.host[2],entry.host[3],entry.host[4],entry.host[5],entry.host[6],entry.host[7],entry.host[8],entry.host[9],entry.host[10],entry.host[11],entry.host[12],entry.host[13],entry.host[14],entry.host[15],entry.host[16],entry.host[17],entry.host[18],entry.host[19],entry.host[20],entry.host[21]);
	snprintf(liveTime, 10, "%lu", (unsigned long)ntohl(entry.expires));

	return 1;
}
#endif
///////////////////////////////////////////////////////////
static int getPid(char *filename)
{
	struct stat status;
	char buff[100];
	FILE *fp;

	if ( stat(filename, &status) < 0)
		return -1;
	fp = fopen(filename, "r");
	if (!fp) {
#ifndef _BOA_
        	error(E_L, E_LOG, T("Read pid file error!\n"));
#endif
		return -1;
   	}
	fgets(buff, 100, fp);
	fclose(fp);

	return (atoi(buff));
}

// Static DHCP lease ----------------------------------------------
int SDHCPList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	SDHCP_T entry;
	char tmpBuf[100];
	char tmpBuf1[100];
	char *ip;

	if ( !apmib_get(MIB_STATIC_DHCP_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}



	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_STATIC_DHCP, (void *)&entry))
			return -1;

		snprintf(tmpBuf, 100, T("%02x:%02x:%02x:%02x:%02x:%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);

		snprintf(tmpBuf1,100, T("%02x%02x%02x%02x%02x%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);

		ip = fixed_inet_ntoa(entry.ipAddr);
		if ( !strcmp(ip, "0.0.0.0"))
			ip = "----";
	
		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"7%%\">\n"));
		nBytesSent += websWrite(wp, T("    <font size=\"2\">%d</font>\n"),i);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"45%%\">\n"));
		nBytesSent += websWrite(wp, T("    <font size=\"2\">%s</font><input type=\"hidden\" name=\"smac%d\" value=\"%s\">\n"),tmpBuf,i,tmpBuf1);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"35%%\">\n"));
		nBytesSent += websWrite(wp, T("    <font size=\"2\">%s</font><input type=\"hidden\" name=\"sip%d\" value=\"%s\">\n"),ip,i,ip);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"13%%\">\n"));
		nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" name=\"select%d\" value=\"ON\">\n"),i);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"smacnum\" value=\"%d\">\n"),i-1);
	return nBytesSent;
}
//-----------------------------------------------------------------
// Static DHCP lease ----------------------------------------------
void formSDHCP(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddMac, *strDelMac, *strDelAllMac, *strVal, *submitUrl, *strEnabled, *strApply;
	char tmpBuf[100];
	int entryNum, i, enabled;
	SDHCP_T entry;

	strAddMac = websGetVar(wp, T("addSDHCPMac"), T(""));
	strDelMac = websGetVar(wp, T("deleteSelDhcpMac"), T(""));
	strDelAllMac = websGetVar(wp, T("deleteAllDhcpMac"), T(""));
	strEnabled = websGetVar(wp, T("SDHCPEnabled"), T(""));

	if (strAddMac[0]) {
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;
		if ( apmib_set( MIB_STATIC_DHCP_ENABLED, (void *)&enabled) == 0) {
  			DEBUGP(tmpBuf, T("Set enabled flag error!"));
			goto setErr_sdhcp;
		}

		strVal = websGetVar(wp, T("mac"), T(""));
		printf("mac=%s\n", strVal);
		if ( !strVal[0] ) {
//			DEBUGP(tmpBuf, T("Error! No mac address to set."));
			goto setsdhcp_ret;
		}
		if (strlen(strVal)!=12 || !string_to_hex(strVal, entry.macAddr, 12)) {
			DEBUGP(tmpBuf, T("Error! Invalid MAC address."));
			goto setErr_sdhcp;
		}

		printf("mac=%02x:%02x:%02x:%02x:%02x:%02x\n", entry.macAddr[0], entry.macAddr[1],entry.macAddr[2],entry.macAddr[3],entry.macAddr[4],entry.macAddr[5]);
		strVal = websGetVar(wp, T("ip"), T(""));
		printf("ip=%s\n", strVal);
		if (!strVal[0]) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformFilterErrInput))</script>"));
			goto setErr_sdhcp;
		}

		fixed_inet_aton(strVal, entry.ipAddr);

		printf("ip=%02x.%02x.%02x.%02x\n", entry.ipAddr[0], entry.ipAddr[1],entry.ipAddr[2],entry.ipAddr[3]);
		if ( !apmib_get(MIB_STATIC_DHCP_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("Get entry number error!"));
			goto setErr_sdhcp;
		}

		if ( (entryNum + 1) > MAX_STATIC_DHCP_NUM) {

			strcpy(tmpBuf, T("Cannot add new entry because table is full!"));
			goto setErr_sdhcp;
		}

		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_STATIC_DHCP_DEL, (void *)&entry);
		if ( apmib_set(MIB_STATIC_DHCP_ADD, (void *)&entry) == 0) {
			DEBUGP(tmpBuf, T("Add table entry error!"));
			goto setErr_sdhcp;
		}
	}

	/* Delete entry */
	if (strDelMac[0]) {
		if ( !apmib_get(MIB_STATIC_DHCP_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("Get entry number error!"));
			goto setErr_sdhcp;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)&entry) = (char)i;
				if ( !apmib_get(MIB_STATIC_DHCP, (void *)&entry)) {
					DEBUGP(tmpBuf, T("Get table entry error!"));
					goto setErr_sdhcp;
				}
				if ( !apmib_set(MIB_STATIC_DHCP_DEL, (void *)&entry)) {
					DEBUGP(tmpBuf, T("Delete table entry error!"));
					goto setErr_sdhcp;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAllMac[0]) {
		if ( !apmib_set(MIB_STATIC_DHCP_DELALL, (void *)&entry)) {
			DEBUGP(tmpBuf, T("Delete all table error!"));
			goto setErr_sdhcp;
		}
	}

setsdhcp_ret:
	apmib_update(CURRENT_SETTING);

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

#ifdef _7207CG_
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		else
			websDone(wp, 200);

	  	return;
#else
	// Tommy Modified on 0602
	if (strApply[0]) {
#ifdef _TAG_IN_PAGE_
main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
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
		OK_MSG(wp,  submitUrl);
#endif
	}
	else {
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		else
			websDone(wp, 200);
	}

  	return;
#endif



setErr_sdhcp:
	ERR_MSG(wp, tmpBuf);
}
//-----------------------------------------------------------------

///////////////////////////////////////////////////////////////////
void formTcpipSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strIp, *strMask, *strGateway, *strDhcp, *strStp, *submitUrl, *strMac, *strIpChanged, *strMode, *strDNS;
	char_t	*strDomainName, *strDhcpGatewayIP, *strDhcpNameServerIP, *strLeaseTime, *strDisIp;
	char_t  *strUser, *strPassword, *userid, *nextUserid, *strOPassword;	//password
	struct in_addr inIp, inMask, inGateway, inDhcpGatewayIP, inDhcpNameServerIP, dns1, dns2, dns3;
	DHCP_T dhcp, curDhcp;
	char tmpBuf[128];
	unsigned long longVal;
	int stp, DisIp=0, intVal;
	char_t	*strdhcpRangeStart, *strdhcpRangeEnd;
	struct in_addr dhcpRangeStart, dhcpRangeEnd;
	DNS_TYPE_T dns, dns_old;
	int dns_changed=0;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));
	strIpChanged = websGetVar(wp, T("ipChanged"), T("")); // Tommy Added
	//EDX Timmy start
	if ( strIpChanged[0] ){
        	lanipChanged = atoi(strIpChanged);
	}
	//EDX Timmy end




//**************************************** DNS Setting ****************************************
	strMode = websGetVar(wp, T("dnsMode"), T(""));
	if ( strMode[0] ) {
		system("rm -f /tmp/showConnectResult2 2> /dev/null"); // let wiz_ip.asp show flash setting
		struct in_addr dns1_old, dns2_old, dns3_old;
		if (!strcmp(strMode, T("dnsAuto")))
			dns = DNS_AUTO;
		else if (!strcmp(strMode, T("dnsManual")))
			dns = DNS_MANUAL;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup3))</script>"));
			goto setErr_tcpip;
		}

		if ( !apmib_get(MIB_WAN_DNS_MODE, (void *)&dns_old)) {
	  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup4))</script>"));
			goto setErr_tcpip;
		}
		if (dns != dns_old)
			dns_changed = 1;

		// Set DNS to MIB
		if ( !apmib_set(MIB_WAN_DNS_MODE, (void *)&dns)) {
	  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup2))</script>"));
			goto setErr_tcpip;
		}

		//if ( dns == DNS_MANUAL ) {
			if ( !apmib_get(MIB_WAN_DNS1, (void *)&dns1_old)) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup3))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_get(MIB_WAN_DNS2, (void *)&dns2_old)) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup3))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_get(MIB_WAN_DNS3, (void *)&dns3_old)) {
	  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup3))</script>"));
				goto setErr_tcpip;
			}


			// If DHCP server is enabled in LAN, update dhcpd.conf

			strDNS = websGetVar(wp, T("dns1"), T(""));

			if ( strDNS[0] ) {
				if ( !inet_aton(strDNS, &dns1) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup1))</script>"));
					goto setErr_tcpip;
				}

				if ( !apmib_set(MIB_WAN_DNS1, (void *)&dns1)) {
	  				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup2))</script>"));
					goto setErr_tcpip;
				}
			}
			else {
				if ( !apmib_get(MIB_WAN_DNS1, (void *)&dns1) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup3))</script>"));
					goto setErr_tcpip;
				}
			}
			strDNS = websGetVar(wp, T("dns2"), T(""));
			if ( strDNS[0] ) {
				if ( !inet_aton(strDNS, &dns2) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup1))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_DNS2, (void *)&dns2)) {
	  				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup2))</script>"));
					goto setErr_tcpip;
				}
			}
			else {
				if ( !apmib_get(MIB_WAN_DNS2, (void *)&dns2) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup5))</script>"));
					goto setErr_tcpip;
				}
			}
			strDNS = websGetVar(wp, T("dns3"), T(""));
			if ( strDNS[0] ) {
				if ( !inet_aton(strDNS, &dns3) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup1))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_DNS3, (void *)&dns3)) {
	  				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup2))</script>"));
					goto setErr_tcpip;
				}
			}
			else {
				if ( !apmib_get(MIB_WAN_DNS3, (void *)&dns3) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup6))</script>"));
					goto setErr_tcpip;
				}
			}

			if ( *((long *)&dns1) != *((long *)&dns1_old) ||
				 *((long *)&dns2) != *((long *)&dns2_old) ||
				  *((long *)&dns3) != *((long *)&dns3_old) )
				dns_changed = 1;
		//}
	}

//**************************************** password ****************************************
	strUser = "admin";
	strPassword = websGetVar(wp, T("newpass"), T(""));
	strOPassword = websGetVar(wp, T("oldpass"), T(""));
	apmib_get(MIB_USER_PASSWORD, (void *)tmpBuf);
	if ( strOPassword[0] || strPassword[0]) {
		if (  strcmp(strOPassword,tmpBuf )) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup1))</script>"));
	       	goto setErr_tcpip;
		}
	if ( strUser[0] ) {
		// Check if user name is the same as supervisor name
		if ( !apmib_get(MIB_SUPER_NAME, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup2))</script>"));
			goto setErr_tcpip;
		}
		if ( !strcmp(strUser, tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup3))</script>"));
			goto setErr_tcpip;
		}
#ifndef _BOA_
		// Check if supervisor account exist. if not, create it
		if ( !umGroupExists(DEFAULT_GROUP) )
			if ( umAddGroup(DEFAULT_GROUP, (short)PRIV_ADMIN, AM_BASIC, FALSE, FALSE) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup4))</script>"));
				goto setErr_tcpip;
			}
		if ( !umAccessLimitExists(ACCESS_URL) )
			if ( umAddAccessLimit(ACCESS_URL, AM_FULL, (short)0, DEFAULT_GROUP) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup5))</script>"));
				goto setErr_tcpip;
			}
		if ( !umUserExists("super"))
			if ( umAddUser("super", "AP@xuniL", DEFAULT_GROUP, FALSE, FALSE) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup6))</script>"));
				goto setErr_tcpip;
			}

		// Add new one
		if ( umUserExists(strUser))
			umDeleteUser(strUser);

		if ( umAddUser(strUser, strPassword, DEFAULT_GROUP, FALSE, FALSE) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup7))</script>"));
			goto setErr_tcpip;
		}
#endif
	}
#ifndef _BOA_
	else {
		umDeleteAccessLimit("/");
		umDeleteUser(superName);
		umDeleteGroup(DEFAULT_GROUP);
	}
		// Delete current user account
		userid = umGetFirstUser();
		while (userid) {
			if ( gstrcmp(userid, superName) && gstrcmp(userid, strUser)) {
				nextUserid = umGetNextUser(userid);
				if ( umDeleteUser(userid) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup8))</script>"));
					goto setErr_tcpip;
				}
				userid = nextUserid;
				continue;
			}
			userid = umGetNextUser(userid);
		}
#ifdef HOME_GATEWAY  /* Modified by Tommy  */
		if (umCommit(NULL) != 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup9))</script>"));
			goto setErr_tcpip;
		}
#endif
#endif
		// Set user account to MIB
		if ( !apmib_set(MIB_USER_NAME, (void *)strUser) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup10))</script>"));
			goto setErr_tcpip;
		}

		if ( !apmib_set(MIB_USER_PASSWORD, (void *)strPassword) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup11))</script>"));
			goto setErr_tcpip;
		}
	}

	strLeaseTime = websGetVar(wp, T("leaseTime"), T(""));
	if (strLeaseTime[0]) {
		if ( !(longVal=strtoul(strLeaseTime,NULL,0))) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup12))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_LAN_LEASETIME, (void *)&longVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup13))</script>"));
			goto setErr_tcpip;
		}
	}
//---------------------------------   Client Mode   ----------------------------------------------
	// Set client mode ip type
	strDisIp = websGetVar(wp, T("clIpDisabled"), T(""));
	if (strDisIp[0]) {
		if (strDisIp[0] == '0')
			DisIp = 0;
		else
			DisIp = 1;
		if ( !apmib_set(MIB_CLIENT_IP_DISABLED, (void *)&DisIp)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup14))</script>"));
			goto setErr_tcpip;
		}
	}
//----------------------------------------------------------------------------------------
	// Set STP
	strStp = websGetVar(wp, T("stp"), T(""));
	if (strStp[0]) {
		if (strStp[0] == '0')
			stp = 0;
		else
			stp = 1;
		if ( !apmib_set(MIB_STP_ENABLED, (void *)&stp)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup15))</script>"));
			goto setErr_tcpip;
		}
	}

	// Set clone MAC address
	strMac = websGetVar(wp, T("macAddr"), T(""));
	if (strMac[0]) {
		if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
			goto setErr_tcpip;
		}
		if ( !apmib_set(MIB_ELAN_MAC_ADDR, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup17))</script>"));
			goto setErr_tcpip;
		}
		if ( !apmib_set(MIB_WLAN_MAC_ADDR, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup18))</script>"));
			goto setErr_tcpip;
		}
	}
//**************************************** DHCP Server ****************************************

#if defined(_DHCP_SWITCH_) //EDX patrick add 20130509
	char_t	*ipMode;
	ipMode = websGetVar(wp, T("ipmode"), T(""));
	int intZero=0;
	if (ipMode[0]) 
	{
		if ( ipMode[0] == '0')
			dhcp = 0; //Use the following IP address
		else
			dhcp = 1;  //Obtain an IP address automatically

		if ( !apmib_set(MIB_DHCP, (void *)&intZero)) {
  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup15))</script>"));
			goto setErr_tcpip;
		}

		if ( !apmib_set(MIB_DHCP_SWITCH, (void *)&dhcp)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup15))</script>"));
			goto setErr_tcpip;
		}
	}
#endif

	// Read current DHCP setting for reference later
	if ( !apmib_get( MIB_DHCP, (void *)&curDhcp) ) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup19))</script>"));
		goto setErr_tcpip;
	}

	strDhcp = websGetVar(wp, T("dhcp"), T(""));
	if ( strDhcp[0] ) {
		if ( strDhcp[0]!='0' && strDhcp[0]!='1' && strDhcp[0]!='2' ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup20))</script>"));
			goto setErr_tcpip;
		}
		// set to MIB
		if ( strDhcp[0] == '0' )
			dhcp = DHCP_DISABLED;
		else if (strDhcp[0] == '1')
			dhcp = DHCP_CLIENT;
		else
			dhcp = DHCP_SERVER;

		if ( !apmib_set(MIB_DHCP, (void *)&dhcp)) {
	  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup21))</script>"));
			goto setErr_tcpip;
		}
	}
	else
		dhcp = curDhcp;

	#ifdef _AP_DHCP_SPACE_
		char *strApDhcp;
		DHCP_T ap_dhcp;
		strApDhcp = websGetVar(wp, T("apdhcp"), T(""));
		if(strApDhcp[0])
		{
			if ( strApDhcp[0] == '0' )
				ap_dhcp = DHCP_DISABLED;
			else if (strApDhcp[0] == '1')
				ap_dhcp = DHCP_CLIENT;
			else
				ap_dhcp = DHCP_SERVER;
		}
		apmib_set(MIB_AP_DHCP, (void *)&ap_dhcp);
	#endif

#ifdef _CONVERT_IP_ADDR_
 	int AP_ROUTER_MODE;
#endif
	if ( dhcp == DHCP_DISABLED || dhcp == DHCP_SERVER ) {
//---------------------------------------------------------------------------------------
		strDomainName = websGetVar(wp, T("DomainName"), T(""));
		if (strDhcp[0]!='0'){
   	     if ( apmib_set(MIB_DOMAIN_NAME, (void *)strDomainName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup22))</script>"));
		        goto setErr_tcpip;
			}
		}
		strDhcpGatewayIP = websGetVar(wp, T("DhcpGatewayIP"), T(""));
		if ( strDhcpGatewayIP[0] ) {
			if ( !inet_aton(strDhcpGatewayIP, &inDhcpGatewayIP) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set( MIB_DHCPGATEWAYIP_ADDR, (void *)&inDhcpGatewayIP)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
				goto setErr_tcpip;
			}
		}

		strDhcpNameServerIP = websGetVar(wp, T("DhcpNameServerIP"), T(""));
		if ( strDhcpGatewayIP[0] ) {
			if ( !inet_aton(strDhcpNameServerIP, &inDhcpNameServerIP) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set( MIB_DHCPNAMESERVER_ADDR, (void *)&inDhcpNameServerIP)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
				goto setErr_tcpip;
			}
		}
//**************************************** Ip Settings ****************************************

		#ifdef _IQv2_
		int wizMode, wl5gRepEnable, iqsetupFinish;
		apmib_get( MIB_WIZ_MODE, (void *)&wizMode);
		apmib_get( MIB_INIC_REPEATER_ENABLED, (void *)&wl5gRepEnable);
		apmib_get( MIB_IQSET_DISABLE, (void *)&iqsetupFinish);
		#endif


		if (DisIp != 1) {	// DHCP Client
			strIp = websGetVar(wp, T("ip"), T(""));
			if ( strIp[0] ) {
				if ( !inet_aton(strIp, &inIp) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set( MIB_IP_ADDR, (void *)&inIp)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
					goto setErr_tcpip;
				}
			}
			else { // get current used IP
				
				#ifdef _IQv2_
				if((wizMode==0) || (wizMode==4))
				{
					#ifdef __TARGET_BOARD__
					if ( !getInAddr(BRIDGE_IF, IP_ADDR, (void *)&inIp) ) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup25))</script>"));
						goto setErr_tcpip;
					}
					#endif
				}
				#else
				if ( !getInAddr(BRIDGE_IF, IP_ADDR, (void *)&inIp) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup25))</script>"));
					goto setErr_tcpip;
				}
				#endif
				
			}
			strMask = websGetVar(wp, T("mask"), T(""));
			if ( strMask[0] ) {
				if ( !inet_aton(strMask, &inMask) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup26))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_SUBNET_MASK, (void *)&inMask)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup27))</script>"));
					goto setErr_tcpip;
				}
			}
			else { // get current used netmask
				
				#ifdef _IQv2_
				if((wizMode==0) || (wizMode==4))
				{
					#ifdef __TARGET_BOARD__
					if ( !getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&inMask )) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup28))</script>"));
						goto setErr_tcpip;
					}	
					#endif
				}
				#else
				if ( !getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&inMask )) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup28))</script>"));
					goto setErr_tcpip;
				}
				#endif
			
			}
			strGateway = websGetVar(wp, T("gateway"), T(""));
	//		if ( dhcp == DHCP_DISABLED && strGateway[0] ) {
			if ( strGateway[0] ) {
				if ( !inet_aton(strGateway, &inGateway) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup29))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_DEFAULT_GATEWAY, (void *)&inGateway)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup30))</script>"));
					goto setErr_tcpip;
				}
			}
		}
//****************************************  ****************************************
		#ifdef _AP_DHCP_SPACE_
		if ( ap_dhcp == DHCP_SERVER || dhcp == DHCP_SERVER ) {
		#else
		if ( dhcp == DHCP_SERVER ) {
		#endif
			// Get/Set DHCP client range
			strdhcpRangeStart = websGetVar(wp, T("dhcpRangeStart"), T(""));
			if ( strdhcpRangeStart[0] ) {
				if ( !inet_aton(strdhcpRangeStart, &dhcpRangeStart) ) {
					DEBUGP(tmpBuf, T("Invalid DHCP client start address!"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_DHCP_CLIENT_START, (void *)&dhcpRangeStart)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup32))</script>"));
					goto setErr_tcpip;
				}
			}
			strdhcpRangeEnd = websGetVar(wp, T("dhcpRangeEnd"), T(""));
			if ( strdhcpRangeEnd[0] ) {
				if ( !inet_aton(strdhcpRangeEnd, &dhcpRangeEnd) ) {
					DEBUGP(tmpBuf, T("Invalid DHCP client end address!"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_DHCP_CLIENT_END, (void *)&dhcpRangeEnd)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup34))</script>"));
					goto setErr_tcpip;
				}
			}

			// update DHCP server config file
			if ( strdhcpRangeStart[0] && strdhcpRangeEnd[0] ) {
				unsigned long start, end, mask, ip, diff;

				start = *((unsigned long *)&dhcpRangeStart);
				end = *((unsigned long *)&dhcpRangeEnd);
				diff =  ((end) - (start)) ;
				ip = *((unsigned long *)&inIp);
				mask = *((unsigned long *)&inMask);
				if (diff <= 0 ||
//					diff > 256*3 ||
					(ip&mask) != (start&mask) ||
					(ip&mask) != (end& mask) ) {
					DEBUGP(tmpBuf, T("Invalid DHCP client range!"));
					goto setErr_tcpip;
				}
			}
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

	#ifdef _HIGHSPEED_WIFI_REPEATER_
	strDhcp = websGetVar(wp, T("HS24_DHCPswitch"), T(""));
	if ( strDhcp[0] )
	{
		if ( strDhcp[0] == '0' )
			dhcp = DHCP_DISABLED;			
		else if (strDhcp[0] == '1')
			dhcp = DHCP_CLIENT;
		else
			dhcp = DHCP_SERVER;
		// set to MIB
		apmib_set(MIB_DHCP, (void *)&dhcp);
	}

	strLeaseTime = websGetVar(wp, T("HS24_leaseTime"), T(""));
	if (strLeaseTime[0]) {
		longVal=strtoul(strLeaseTime,NULL,0);
		apmib_set(MIB_LAN_LEASETIME, (void *)&longVal);
	}

	strDhcpGatewayIP = websGetVar(wp, T("HS24_DhcpGatewayIP"), T(""));
	if ( strDhcpGatewayIP[0] ) {
		inet_aton(strDhcpGatewayIP, &inDhcpGatewayIP);
		apmib_set( MIB_DHCPGATEWAYIP_ADDR, (void *)&inDhcpGatewayIP);
	}

	// Get/Set DHCP client range
	strdhcpRangeStart = websGetVar(wp, T("HS24_dhcpRangeStart"), T(""));
	if ( strdhcpRangeStart[0] ) {
		inet_aton(strdhcpRangeStart, &dhcpRangeStart);
		apmib_set(MIB_DHCP_CLIENT_START, (void *)&dhcpRangeStart);

	}
	strdhcpRangeEnd = websGetVar(wp, T("HS24_dhcpRangeEnd"), T(""));
	if ( strdhcpRangeEnd[0] ) {
		inet_aton(strdhcpRangeEnd, &dhcpRangeEnd);
		apmib_set(MIB_DHCP_CLIENT_END, (void *)&dhcpRangeEnd);
	}
	#endif

	apmib_update(CURRENT_SETTING);	// update configuration to flash


	char_t *gowifi, *strUrl;
	gowifi = websGetVar(wp, T("goToWifi"), T(""));
	if (gowifi[0])
	{
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		websRedirect(wp, strUrl);
		return ;
	}
	
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
			REBOOT_MSG(wp,tmpBufapply ,submitUrl);
		}
		system("/bin/reboot.sh 3 &");
	}
	else{
		system("echo 1 > /tmp/show_reboot_info");
		REDIRECT_PAGE(wp, submitUrl);
	}
	return;

setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef HOME_GATEWAY
void formPreventionSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strpodEnable, *strpingEnable, *strscanEnable, *strsynEnable,*submitUrl, *strBasic;
	char_t	*strpodPack, *strpodTime, *strpodBur, *strsynPack, *strsynTime, *strsynBur, *strscanNum;

	char_t	*strApply;

	int intVal, val;
	char tmpBuf[128];
	unsigned long longVal;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

//**************************************** Ping of Death ****************************************
	strpodEnable = websGetVar(wp, T("podEnable"), T(""));
	if ( !gstrcmp(strpodEnable, T("ON")))
		intVal = 1;
	else
		intVal = 0;
	if ( apmib_set( MIB_POD_ENABLED, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_tcpip;
	}
	strpodPack = websGetVar(wp, T("podPack"), T(""));
	if (strpodPack[0]) {
		if ( !string_to_dec(strpodPack, &val) || val<0 || val>255) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup2))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_POD_PACK, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup3))</script>"));
			goto setErr_tcpip;
		}
	}
	strpodTime = websGetVar(wp, T("podTime"), T(""));
	if (strpodTime[0]) {
		errno=0;
		val = strtol( strpodTime, (char **)NULL, 10);
		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup4))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set( MIB_POD_TIME, (void *)&val) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup5))</script>"));
			goto setErr_tcpip;
		}
	}
	strpodBur = websGetVar(wp, T("podBur"), T(""));
	if (strpodBur[0]) {
		if ( !string_to_dec(strpodBur, &val) || val<0 || val>255) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup6))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_POD_BUR, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup7))</script>"));
			goto setErr_tcpip;
		}
	}

//*********************************** Discard Ping From WAN ***********************************
#ifndef _DIS_PING_INDEP_
	strpingEnable = websGetVar(wp, T("pingEnable"), T(""));
	if ( !gstrcmp(strpingEnable, T("ON")))
		intVal = 1;
	else
		intVal = 0;
	if ( apmib_set( MIB_PING_ENABLED, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_tcpip;
	}
#endif
//**************************************** Port Scan ****************************************
	strscanEnable = websGetVar(wp, T("scanEnable"), T(""));
	if ( !gstrcmp(strscanEnable, T("ON")))
		intVal = 1;
	else
		intVal = 0;
	if ( apmib_set( MIB_SCAN_ENABLED, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_tcpip;
	}

	strBasic = websGetVar(wp, T("basic"), T(""));
	if (!strBasic[0]) {
		strscanNum = websGetVar(wp, T("scanNumVal"), T(""));
		longVal=strtoul(strscanNum,NULL,0);
		if ( apmib_set(MIB_SCAN_NUM, (void *)&longVal) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup8))</script>"));
			goto setErr_tcpip;
		}
	}
//**************************************** Sync Flood ****************************************
	strsynEnable = websGetVar(wp, T("synEnable"), T(""));
	if ( !gstrcmp(strsynEnable, T("ON")))
		intVal = 1;
	else
		intVal = 0;
	if ( apmib_set( MIB_SYN_ENABLED, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_tcpip;
	}

	strsynPack = websGetVar(wp, T("synPack"), T(""));
	if (strsynPack[0]) {
		if ( !string_to_dec(strsynPack, &val) || val<0 || val>255) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup9))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_SYN_PACK, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup10))</script>"));
			goto setErr_tcpip;
		}
	}

	strsynTime = websGetVar(wp, T("synTime"), T(""));
	if (strsynTime[0]) {
		errno=0;
		val = strtol( strsynTime, (char **)NULL, 10);
		if (errno) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup11))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set( MIB_SYN_TIME, (void *)&val) == 0) {
   			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup12))</script>"));
			goto setErr_tcpip;
		}
	}
	strsynBur = websGetVar(wp, T("synBur"), T(""));
	if (strsynBur[0]) {
		if ( !string_to_dec(strsynBur, &val) || val<0 || val>255) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup13))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_SYN_BUR, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup14))</script>"));
			goto setErr_tcpip;
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

	apmib_update(CURRENT_SETTING);	// update to flash

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

setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
	return;
}
void formUPNPSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strupnpEnable, *submitUrl;
	int val;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strupnpEnable = websGetVar(wp, T("upnpEnable"), T(""));
	if (strupnpEnable[0]) {
		if (!gstrcmp(strupnpEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strupnpEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormUPNPSetup1))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_UPNP_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormUPNPSetup2))</script>"));
			goto setErr_tcpip;
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

	apmib_update(CURRENT_SETTING);	// update to flash

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
setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
}

void formHWNATSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strupnpEnable, *submitUrl;
	int val;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strupnpEnable = websGetVar(wp, T("HwNatEnable"), T(""));
	if (strupnpEnable[0]) {
		if (!gstrcmp(strupnpEnable, T("no")))
			val = 0;
		else if (!gstrcmp(strupnpEnable, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormUPNPSetup1))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_FAST_NAT_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormUPNPSetup2))</script>"));
			goto setErr_tcpip;
		}
	}

	apmib_update(CURRENT_SETTING);	// update to flash

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
setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// Lance 2004.2.9
void formPSSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strPTR, *submitUrl;
	int val;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strPTR = websGetVar(wp, T("PSEnable"), T(""));
	if (strPTR[0]) {
		if (!gstrcmp(strPTR, T("no")))
			val = 0;
		else if (!gstrcmp(strPTR, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup1))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_PS_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup2))</script>"));
			goto setErr_tcpip;
		}
	}
	strPTR = websGetVar(wp, T("PS_IPPEnable"), T(""));
	if (strPTR[0]) {
		if (!gstrcmp(strPTR, T("no")))
			val = 0;
		else if (!gstrcmp(strPTR, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup3))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_PS_IPPENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup4))</script>"));
			goto setErr_tcpip;
		}
	}
	strPTR = websGetVar(wp, T("PS_LPREnable"), T(""));
	if (strPTR[0]) {
		if (!gstrcmp(strPTR, T("no")))
			val = 0;
		else if (!gstrcmp(strPTR, T("yes")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup5))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_PS_LPRENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup6))</script>"));
			goto setErr_tcpip;
		}
	}
    strPTR = websGetVar(wp, T("PSName"), T(""));
    if ( strPTR[0] ) {
	    if ( apmib_set(MIB_PS_NAME, (void *)strPTR) == 0) {
    	    DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup7))</script>"));
            goto setErr_tcpip;
        }
    }
    strPTR = websGetVar(wp, T("PS_Port1Name"), T(""));
    if ( strPTR[0] ) {
	    if ( apmib_set(MIB_PS_PORT1NAME, (void *)strPTR) == 0) {
    	    DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup8))</script>"));
            goto setErr_tcpip;
        }
    }
    strPTR = websGetVar(wp, T("PS_Port2Name"), T(""));
    if ( strPTR[0] ) {
	    if ( apmib_set(MIB_PS_PORT2NAME, (void *)strPTR) == 0) {
    	    DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPSSetup9))</script>"));
            goto setErr_tcpip;
        }
    }

	apmib_update(CURRENT_SETTING);	// update to flash

#ifndef NO_ACTION
	//system("/bin/pserver.sh"); // Tommy
	//system("/bin/reboot.sh");
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
setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
}
// End - Lance
/////////////////////////////////////////////////////////////////////////////////////////////////
void formTELBPSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strtelBPIPAddr, *submitUrl, *strVal;
	char_t  *strName, *strPassword, *strwanMode, *strnextEnable, *strApply;
	int intVal, Mode;
	struct in_addr intelBPIPAddr;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strtelBPIPAddr = websGetVar(wp, T("telBPIPAddr"), T(""));
	if ( strtelBPIPAddr[0] ) {
		if ( !inet_aton(strtelBPIPAddr, &intelBPIPAddr) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_telstra;
		}
		if ( !apmib_set( MIB_TELBPIP_ADDR, (void *)&intelBPIPAddr)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_telstra;
		}
	}
	strName = websGetVar(wp, T("telBPUserName"), T(""));
		if ( strName[0] ) {
			if ( apmib_set(MIB_TELBP_USER, (void *)strName) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTELBPSetup1))</script>"));
				goto setErr_telstra;
			}
		}
	strPassword = websGetVar(wp, T("telBPPassword"), T(""));
		if ( strPassword[0] ) {
			if ( apmib_set(MIB_TELBP_PASSWORD, (void *)strPassword) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTELBPSetup2))</script>"));
				goto setErr_telstra;
			}
		}
	strVal = websGetVar(wp, T("telBPEnabled"), T(""));
	if ( !gstrcmp(strVal, T("ON")))
		intVal = 1;
	else
		intVal = 0;

	if ( apmib_set(MIB_TELBP_ENABLED, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_telstra;
	}
	strwanMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strwanMode[0] ) {
		if ( !gstrcmp(strwanMode, T("4")))
			Mode = 4;
	   if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_telstra;
		}
	}
	apmib_update(CURRENT_SETTING);	// update to flash

	// run script
#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all"); // Tommy
	// system(tmpBuf);
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strApply=websGetVar(wp,T("isApply"),T(""));
	strnextEnable = websGetVar(wp, T("redirect"), T(""));
	if ( (strnextEnable[0] || wizardEnabled == 1) && !strApply[0]){
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

setErr_telstra:
	ERR_MSG(wp, tmpBuf);

}
/////////////////////////////////////////////////////////////////////////////////////////////
void formDDNSSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*submitUrl;
	char_t  *strName, *strPassword, *strAccount, *strEnable, *strpvidSel;
	char_t	*strHostID, *strAddress, *strdhispass, *strdhisSelect;
	char_t  *strAuthP1, *strAuthP2, *strAuthQ1, *strAuthQ2;
//	struct in_addr inAddress;
	char tmpBuf[128];
	int val;
	unsigned long longVal;
/*#ifndef NO_ACTION
	int pid;
#endif*/

/*	int dns_changed=0;
	char *arg;
*/
	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strEnable = websGetVar(wp, T("ddnsEnable"), T(""));
	if (strEnable[0]) {
		if (!gstrcmp(strEnable, T("0"))){
			val = 0;

		}
		else if (!gstrcmp(strEnable, T("1")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspError))</script>"));
			goto setErr_tcpip;
		}



		if ( apmib_set(MIB_DDNS_ENABLE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup1))</script>"));
			goto setErr_tcpip;
		}
		val = 1;
		if ( apmib_set(MIB_DDNS_STATUS, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup1))</script>"));
			goto setErr_tcpip;
		}


	}

	strpvidSel = websGetVar(wp, T("ddnspvidSel"), T(""));
	if ( strpvidSel[0] ) {
		if ( apmib_set(MIB_DDNS_PVIDSEL, (void *)strpvidSel) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup2))</script>"));
			goto setErr_tcpip;
		}
	}

	if ( strcmp(strpvidSel,"dhis") && strcmp(strpvidSel,"aiaimamor")) {

		strName = websGetVar(wp, T("ddnsName"), T(""));
		if ( strName[0] ) {
			if ( apmib_set(MIB_DDNS_NAME, (void *)strName) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup3))</script>"));
				goto setErr_tcpip;
			}
		}
		strAccount = websGetVar(wp, T("ddnsAccount"), T(""));
		if ( strAccount[0] ) {
			if ( apmib_set(MIB_DDNS_ACCOUNT, (void *)strAccount) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup4))</script>"));
				goto setErr_tcpip;
			}
		}
		strPassword = websGetVar(wp, T("ddnsPass"), T(""));
		if ( strPassword[0] ) {
			if ( apmib_set(MIB_DDNS_PASS, (void *)strPassword) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup5))</script>"));
				goto setErr_tcpip;
			}
		}
    }
    else {
//--------------------------------------------------------------------------------------
		strHostID = websGetVar(wp, T("dhisHostID"), T(""));
		if (strHostID[0]) {
			longVal=strtoul(strHostID,NULL,0);
			if ( apmib_set(MIB_DHIS_HOSTID, (void *)&longVal) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup6))</script>"));
				goto setErr_tcpip;
			}
		}
		strAddress = websGetVar(wp, T("dhisISAddr"), T(""));
		if ( strAddress[0] ) {
// Lance 2003.11.06
//			if ( !inet_aton(strAddress, &inAddress) ) {
//				DEBUGP(tmpBuf, T("Invalid IP-address value!"));
//				goto setErr_tcpip;
//			}
//			if ( !apmib_set( MIB_DHIS_ISADDR, (void *)&inAddress)) {
			if ( !apmib_set( MIB_DHIS_ISADDR, (void *)strAddress)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup7))</script>"));
				goto setErr_tcpip;
			}
		}
		strdhisSelect = websGetVar(wp, T("dhisSelect"), T(""));
		if ( !gstrcmp(strdhisSelect, T("1")))
			val = 1;
		if ( !gstrcmp(strdhisSelect, T("0")))
			val = 0;
		if (strdhisSelect[0]) {
			if ( apmib_set( MIB_DHIS_SELECT, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup8))</script>"));
				goto setErr_tcpip;
			}
		}


		if ( val == 0) {

 			strdhispass = websGetVar(wp, T("dhispass"), T(""));
			if ( strdhispass[0] ) {
				if ( apmib_set(MIB_DHIS_PASSWORD, (void *)strdhispass) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup9))</script>"));
					goto setErr_tcpip;
				}
			}
		}
		else {
 			strAuthP1 = websGetVar(wp, T("dhisAuthP1"), T(""));
			if ( apmib_set(MIB_DHIS_AUTH_P1, (void *)strAuthP1) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup10))</script>"));
				goto setErr_tcpip;
			}
 			strAuthP2 = websGetVar(wp, T("dhisAuthP2"), T(""));
			if ( apmib_set(MIB_DHIS_AUTH_P2, (void *)strAuthP2) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup11))</script>"));
				goto setErr_tcpip;
			}
 			strAuthQ1 = websGetVar(wp, T("dhisAuthQ1"), T(""));
			if ( apmib_set(MIB_DHIS_AUTH_Q1, (void *)strAuthQ1) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup12))</script>"));
				goto setErr_tcpip;
			}
 			strAuthQ2 = websGetVar(wp, T("dhisAuthQ2"), T(""));
			if ( apmib_set(MIB_DHIS_AUTH_Q2, (void *)strAuthQ2) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDDNSSetup13))</script>"));
				goto setErr_tcpip;
			}
		}
	}
//--------------------------------------------------------------------------------------

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}
	#endif

	apmib_update(CURRENT_SETTING);	// update to flash

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

setErr_tcpip:
	ERR_MSG(wp, tmpBuf);

}
void formDynIpSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strdynIPHostName, *strMac, *strMode, *submitUrl;
	int Mode;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page

	strMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strMode[0] ) {
		if ( !gstrcmp(strMode, T("0")))
			Mode = 0;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_dynip;
		}
	}
	strdynIPHostName = websGetVar(wp, T("dynIPHostName"), T(""));
	if ( apmib_set(MIB_DYNIP_HOSTNAME, (void *)strdynIPHostName) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup2))</script>"));
		goto setErr_dynip;
	}
	// Set clone MAC address
	strMac = websGetVar(wp, T("macAddr"), T(""));
	if (strMac[0]) {
		if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc2))</script>"));
			goto setErr_dynip;
		}
		if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
			goto setErr_dynip;
		}
	}

	apmib_update(CURRENT_SETTING);	// update to flash

#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH , _CONFIG_SCRIPT_PROG, "gw", "all"); // Tommy
	// system(tmpBuf);
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
setErr_dynip:
	ERR_MSG(wp, tmpBuf);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void formStcIpSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strIp, *strMask, *strGateway, *strDNS, *strMode, *submitUrl;
	struct in_addr inIp, inMask, inGateway, dns1;
	int Mode;
	char tmpBuf[128];

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strMode[0] ) {
		if ( !gstrcmp(strMode, T("1")))
			Mode = 1;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_stcip;
		}
	}
	strIp = websGetVar(wp, T("ip"), T(""));
	if ( strIp[0] ) {
		if ( !inet_aton(strIp, &inIp) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_stcip;
		}
		if ( !apmib_set(MIB_WAN_IP_ADDR, (void *)&inIp)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_stcip;
		}
	}
	strMask = websGetVar(wp, T("mask"), T(""));
	if ( strMask[0] ) {
		if ( !inet_aton(strMask, &inMask) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup26))</script>"));
			goto setErr_stcip;
		}
		if ( !apmib_set(MIB_WAN_SUBNET_MASK, (void *)&inMask)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup27))</script>"));
			goto setErr_stcip;
		}
	}
	strGateway = websGetVar(wp, T("gateway"), T(""));
	if ( strGateway[0] ) {
		if ( !inet_aton(strGateway, &inGateway) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup29))</script>"));
			goto setErr_stcip;
		}
		if ( !apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&inGateway)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup30))</script>"));
			goto setErr_stcip;
		}
	}
	strDNS = websGetVar(wp, T("dns1"), T(""));
	if ( strDNS[0] ) {
		if ( !inet_aton(strDNS, &dns1) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup1))</script>"));
			goto setErr_stcip;
		}
		if ( !apmib_set(MIB_WAN_DNS1, (void *)&dns1)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup2))</script>"));
			goto setErr_stcip;
		}
	}
	else {
		if ( !apmib_get(MIB_WAN_DNS1, (void *)&dns1) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormStcIpSetup3))</script>"));
			goto setErr_stcip;
		}
	}

	apmib_update(CURRENT_SETTING);	// update to flash

#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH , _CONFIG_SCRIPT_PROG, "gw", "all"); // Tommy
	// system(tmpBuf);
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
setErr_stcip:
	ERR_MSG(wp, tmpBuf);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void formL2TPSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strIPAddr, *strIPMaskAddr, *strDfGateway, *strGateway, *strMac, *submitUrl, *strnextEnable, *strApply;
	char_t  *strName, *strPassword, *strType, *strIpMode, *strConnect, *strwanMode, *strMTU, *strHostName,*HK_HWRN2;
	struct in_addr inIPAddr, inIPMaskAddr, inDfGateway;
	char tmpBuf[128];
	int val, Mode;
	int buttonState=0, flag=1, applyEnable;
	char *ptr;

	#ifdef _Customer_ //Robart Add, Use L2TP must disable hw_nat
		val = 0;
		apmib_set( MIB_FAST_NAT_ENABLE, (void *)&val);
	#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
        strConnect = websGetVar(wp, T("L2TPConnect"), T(""));
	if (strConnect[0])
		buttonState = 1;

        strConnect = websGetVar(wp, T("L2TPDisconnect"), T(""));
	if (strConnect[0])
		buttonState = 2;

	strIpMode = websGetVar(wp, T("L2TPIpMode"), T(""));
	if (strIpMode[0]) {
		if (!gstrcmp(strIpMode, T("0")))
			val = 0;
		else if (!gstrcmp(strIpMode, T("1")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspError))</script>"));
			goto setErr_l2tp;
		}
		if ( apmib_set(MIB_L2TP_IPMODE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup1))</script>"));
			goto setErr_l2tp;
		}
	}
	//Obtain an IP address automatically
	if (!gstrcmp(strIpMode, T("0"))) {
		strHostName = websGetVar(wp, T("HostName"), T(""));
		if ( apmib_set(MIB_DYNIP_HOSTNAME, (void *)strHostName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup2))</script>"));
			goto setErr_l2tp;
		}
		#if defined(_Edimax_)
			strMac = websGetVar(wp, T("macAddr"), T(""));
		#else
			strMac = websGetVar(wp, T("Mac"), T(""));
		#endif
		if (strMac[0]) {
			if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
				goto setErr_l2tp;
			}
			if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
				goto setErr_l2tp;
			}
		}
	}
	else
	{
		#if defined(_Edimax_)
			strMac = websGetVar(wp, T("macAddr1"), T(""));
			if (strMac[0]) 
			{
				if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
					goto setErr_l2tp;
				}
				if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
					goto setErr_l2tp;
				}
			}
		#endif
	}

#ifdef _DUALL_WAN_ACCESS_

	int duallAccessEnable=0;

	char_t *strDuallAccessEnable;
	strDuallAccessEnable=websGetVar(wp, T("enableDuallAccess"), T(""));
	if ( strDuallAccessEnable[0] && !gstrcmp(strDuallAccessEnable, T("ON")))
		duallAccessEnable = 1;
	apmib_set( MIB_DUALL_ACCESS_ENABLE, (void *)&duallAccessEnable);

	#ifdef _DUAL_WAN_IGMP_
	int DUAL_WAN_IGMP=0;
	char_t *strDUAL_WAN_IGMP;
	strDUAL_WAN_IGMP=websGetVar(wp, T("DUAL_WAN_IGMP"), T(""));
	if (strDUAL_WAN_IGMP[0]) {
		if (!gstrcmp(strDUAL_WAN_IGMP, T("0")))
			DUAL_WAN_IGMP = 0;
		else
			DUAL_WAN_IGMP = 1;
		apmib_set(MIB_DUAL_WAN_IGMP, (void *)&DUAL_WAN_IGMP);
	}
	#endif
#endif

#if defined (_AUTO_DNS_)
	strIpMode = websGetVar(wp, T("DNSMode"), T(""));
	if (strIpMode[0])
	{
		if (!gstrcmp(strIpMode, T("0"))) val = 0;
		else val = 1;
		apmib_set(MIB_WAN_DNS_MODE, (void *)&val);
	}

	strcpy(tmpBuf,"0.0.0.0");

	if(val==0)
	{
		inet_aton(tmpBuf, &inIPAddr);
		apmib_set( MIB_WAN_DNS1, (void *)&inIPAddr);
		apmib_set( MIB_WAN_DNS2, (void *)&inIPAddr);
		apmib_set( MIB_WAN_DNS3, (void *)&inIPAddr);
	}
	else
	{
		strIPAddr = websGetVar(wp, T("dns1"), T(""));
		if(strIPAddr[0])
		{
			inet_aton(strIPAddr, &inIPAddr);
			apmib_set( MIB_WAN_DNS1, (void *)&inIPAddr);
		}

		strIPAddr = websGetVar(wp, T("dns2"), T(""));
		if(strIPAddr[0])
		{
			inet_aton(strIPAddr, &inIPAddr);
			apmib_set( MIB_WAN_DNS2, (void *)&inIPAddr);
		}

		strIPAddr = websGetVar(wp, T("dns3"), T(""));
		if(strIPAddr[0])
		{
			inet_aton(strIPAddr, &inIPAddr);
			apmib_set( MIB_WAN_DNS3, (void *)&inIPAddr);
		}
	}
#endif


	strIPAddr = websGetVar(wp, T("L2TPIPAddr"), T(""));
	if ( strIPAddr[0] ) {
		if ( !inet_aton(strIPAddr, &inIPAddr) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_l2tp;
		}
		if ( !apmib_set( MIB_L2TP_IP_ADDR, (void *)&inIPAddr)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_l2tp;
		}
	}
	strIPMaskAddr = websGetVar(wp, T("L2TPMaskAddr"), T(""));
	if ( strIPMaskAddr[0] ) {
		if ( !inet_aton(strIPMaskAddr, &inIPMaskAddr) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_l2tp;
		}
		if ( !apmib_set( MIB_L2TP_MAKEADDR, (void *)&inIPMaskAddr)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_l2tp;
		}
	}
	strDfGateway = websGetVar(wp, T("L2TPDefGateway"), T(""));
	if ( strDfGateway[0] ) {
		if ( !inet_aton(strDfGateway, &inDfGateway) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_l2tp;
		}
		if ( !apmib_set( MIB_L2TP_DEFGATEWAY, (void *)&inDfGateway)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_l2tp;
		}
	}
	strGateway = websGetVar(wp, T("L2TPGateway"), T(""));
	if ( strGateway[0] ) {
		if ( apmib_set( MIB_L2TP_GATEWAY, (void *)strGateway) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup3))</script>"));
			goto setErr_l2tp;
		}
	}

	strName = websGetVar(wp, T("L2TPUserName"), T(""));
	tmpBuf[0]='\0';
	flag = 1;
	while ( flag ) {
		if ((ptr = strchr(strName, '#')) != NULL) {
			*ptr ='\0';
			ptr++;
			strcat( tmpBuf, strName);
			strcat( tmpBuf, "\\#");
			strName = ptr;
		}
		else {
			strcat( tmpBuf, strName);
			flag = 0;
		}
	}
	if ( tmpBuf[0] ) {
		if ( apmib_set( MIB_L2TP_USER, (void *)tmpBuf) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup4))</script>"));
			goto setErr_l2tp;
		}
	}

	strPassword = websGetVar(wp, T("L2TPPassword"), T(""));
	if ( strPassword[0] ) {
		if ( apmib_set( MIB_L2TP_PASSWORD, (void *)strPassword) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup5))</script>"));
			goto setErr_l2tp;
		}
	}

	strMTU = websGetVar(wp, T("L2TPMTU"), T(""));
	if (strMTU[0]) {
		if ( !string_to_dec(strMTU, &val) || val< 512 || val >1492) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup6))</script>"));
			goto setErr_l2tp;
		}
		if ( apmib_set(MIB_L2TP_MTU, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup7))</script>"));
			goto setErr_l2tp;
		}
	}

	strType = websGetVar(wp, T("L2TPConnectType"), T(""));
	if ( strType[0] ) {
		PPP_CONNECT_TYPE_T type;
		if ( strType[0] == '0' )
			type = CONTINUOUS;
		else if ( strType[0] == '1' )
			type = CONNECT_ON_DEMAND;
		else if ( strType[0] == '2' )
			type = MANUAL;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup8))</script>"));
			goto setErr_l2tp;
		}
		if ( apmib_set( MIB_L2TP_CONNECT_TYPE, (void *)&type) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup9))</script>"));
			goto setErr_l2tp;
		}
		if (type != CONTINUOUS) {
			char_t *strTime;
			strTime = websGetVar(wp, T("L2TPIdleTime"), T(""));
			if ( strTime[0] ) {
				int time;
				time = strtol(strTime, (char**)NULL, 10) * 60;
				if ( apmib_set(MIB_L2TP_IDLE_TIME, (void *)&time) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup10))</script>"));
					goto setErr_l2tp;
				}
			}
		}
	}
	strwanMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strwanMode[0] ) {
		if ( !gstrcmp(strwanMode, T("6")))
			Mode = 6;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_l2tp;
		}
	}
//*******************************************************************************
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif

#ifdef _IQv2_
char_t *iqsetupclose;
iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
if (iqsetupclose[0]){
	int close_iQsetup=1;
	apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
}
#endif

	if ( buttonState == 1 ) { // connect button is pressed
		int wait_time=30;

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
//		sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all");
//		system(tmpBuf);
    #ifdef _L2TPD_
		system("/bin/l2tp_2.sh connect"); // Richie 2009.7.10
    #else
		system("/bin/l2tp.sh connect"); // Lance 2004.2.3
    #endif
		while (wait_time-- >0) {
			if (isConnectPPP())
				break;
			sleep(1);
		}
		if (isConnectPPP())
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup11))</script>\n"));
		else
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup12))</script>\n"));
		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}

	if ( buttonState == 2 ) { // disconnect button is pressed

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
//		sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all");
//		system(tmpBuf);
		system("/bin/disconnect.sh pppoe 0 all"); // Lance 2004.2.3
		#if defined(_Customer_)
				strcpy(tmpBuf, T("L2TP disconnected."));
		#else
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup13))</script>\n"));
		#endif
		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}
	apmib_update(CURRENT_SETTING);	// update to flash

	// run script

#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all"); // Tommy
	// system(tmpBuf);
#endif
	strApply=websGetVar(wp,T("isApply"),T(""));
//EDX start
	if(!strcmp(strApply, "1"))
	   applyEnable=1;
	else
	   applyEnable=0;
//EDX end
	strnextEnable = websGetVar(wp, T("redirect"), T(""));
	if ((strnextEnable[0] || wizardEnabled==1) && (applyEnable == 0)){
		system("/bin/scriptlib_util.sh reloadFlash");
		REPLACE_MSG(submitUrl);
	}
	else{
//		#if defined(_Customer_)
//                char_t *strnextEnable;
//                strnextEnable = websGetVar(wp, T("new_apply"), T(""));
//                if (strnextEnable[0])
//                {
//                        system("/bin/reset.sh gw 2");
//                }
//                REPLACE_MSG(submitUrl);
//		#else
//			OK_MSG(wp, submitUrl);
//		#endif
#if 1
		if(strApply[0]){
			if (lanipChanged == 1) {
				char tmpBufnewip[200];
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBufnewip, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufnewip, newurl);
			}
			else
			{			
				char tmpBufapply[200];
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp,tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			#ifdef __TARGET_BOARD__
			system("echo 1 > /tmp/show_reboot_info");
			#endif
			REDIRECT_PAGE(wp, submitUrl);
		}
		return;
#endif
	}
	return;
setErr_l2tp:
	ERR_MSG(wp, tmpBuf);

}
void formPPPoESetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strName, *strPassword, *strpppServName, *strpppMTU, *submitUrl;
	char_t  *strType, *strConnect, *strwanMode;
	char tmpBuf[100];
	int val, Mode, flag=1;
	int buttonState=0;
	int buttonState1=0;
	char *ptr;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
    strConnect = websGetVar(wp, T("pppConnect"), T(""));
	if (strConnect[0])
		buttonState = 1;

	//alex 2007/02/01
    strConnect = websGetVar(wp, T("pppConnect1"), T(""));
	if (strConnect[0])
		buttonState1 = 1;

    strConnect = websGetVar(wp, T("pppDisconnect"), T(""));
	if (strConnect[0])
		buttonState = 2;

	//alex 2007/02/01
    strConnect = websGetVar(wp, T("pppDisconnect1"), T(""));
	if (strConnect[0])
		buttonState1 = 2;

	strwanMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strwanMode[0] ) {
		if ( !gstrcmp(strwanMode, T("2")))
			Mode = 2;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_ppp;
		}
	}
	strpppServName = websGetVar(wp, T("pppServName"), T(""));
	if (Mode == 2) {
		if ( apmib_set(MIB_PPP_SERVNAME, (void *)strpppServName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup14))</script>"));
			goto setErr_ppp;
		}
	}

	strpppMTU = websGetVar(wp, T("pppMTU"), T(""));
	if (strpppMTU[0]) {
		if ( !string_to_dec(strpppMTU, &val) || val< 512 || val >1492) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup6))</script>"));
			goto setErr_ppp;
		}
		if ( apmib_set(MIB_PPP_MTU, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup7))</script>"));
			goto setErr_ppp;
		}
	}

	strName = websGetVar(wp, T("pppUserName"), T(""));
	tmpBuf[0]='\0';
	flag = 1;
	while ( flag ) {
		if ((ptr = strchr(strName, '#')) != NULL) {
			*ptr ='\0';
			ptr++;
			strcat( tmpBuf, strName);
			strcat( tmpBuf, "\\#");
			strName = ptr;
		}
		else {
			strcat( tmpBuf, strName);
			flag = 0;
		}
	}
	if ( tmpBuf[0] ) {
		if ( apmib_set(MIB_PPP_USER, (void *)tmpBuf) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup14))</script>"));
			goto setErr_ppp;
		}
	}

	strPassword = websGetVar(wp, T("pppPassword"), T(""));
	if ( strPassword[0] ) {
		if ( apmib_set(MIB_PPP_PASSWORD, (void *)strPassword) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup15))</script>"));
			goto setErr_ppp;
		}
	}

	strType = websGetVar(wp, T("pppConnectType"), T(""));
	if ( strType[0] ) {
		PPP_CONNECT_TYPE_T type;
		if ( strType[0] == '0' )
			type = CONTINUOUS;
		else if ( strType[0] == '1' )
			type = CONNECT_ON_DEMAND;
		else if ( strType[0] == '2' )
			type = MANUAL;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup16))</script>"));
			goto setErr_ppp;
		}
		if ( apmib_set(MIB_PPP_CONNECT_TYPE, (void *)&type) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup17))</script>"));
			goto setErr_ppp;
		}
		if (type != CONTINUOUS) {
			char_t *strTime;
			strTime = websGetVar(wp, T("pppIdleTime"), T(""));
			if ( strTime[0] ) {
				int time;
				time = strtol(strTime, (char**)NULL, 10) * 60;
				if ( apmib_set(MIB_PPP_IDLE_TIME, (void *)&time) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup18))</script>"));
					goto setErr_ppp;
				}
			}
		}
	}
//**************************************************************************************
	if ( buttonState == 1 ) { // connect button is pressed
		int wait_time=30;

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
		sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH , _CONFIG_SCRIPT_PROG, "gw", "all");
		system(tmpBuf);
		while (wait_time-- >0) {
			if (isConnectPPP())
				break;
			sleep(1);
		}
		if (isConnectPPP())
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup11))</script>\n"));
		else
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup12))</script>\n"));

		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}

	if ( buttonState == 2 ) { // disconnect button is pressed

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
		sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH , _CONFIG_SCRIPT_PROG, "gw", "all");
		system(tmpBuf);
		strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup19))</script>\n"));
		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}
	apmib_update(CURRENT_SETTING);	// update to flash

#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s/%s %s %s", _CONFIG_SCRIPT_PATH , _CONFIG_SCRIPT_PROG, "gw", "all"); // Tommy
	// system(tmpBuf);
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

setErr_ppp:
	ERR_MSG(wp, tmpBuf);

}
////////////////////////////////////////////////////////////////////////////////
void formPPTPSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*strpptpIPAddr, *strpptpIPMaskAddr, *strpptpDfGateway, *strpptpGateway, *submitUrl, *strnextEnable, *HK_HWRN2, *strApply;
	char_t  *strName, *strPassword, *strType, *strIpMode, *strConnect;
	char_t  *strConntID, *strBEZEQEnable, *strwanMode, *strMTU;
	char_t  *strHostName, *strMac;
	struct in_addr inpptpIPAddr, inpptpIPMaskAddr, inpptpDfGateway, inpptpGateway;
	char tmpBuf[128];
	int val, Mode, flag=1;
	int buttonState=0, applyEnable;
	char *ptr;

	#ifdef _Customer_ //Robart Add, Use PPTP must disable hw_nat
		val = 0;
		apmib_set( MIB_FAST_NAT_ENABLE, (void *)&val);
	#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
        strConnect = websGetVar(wp, T("pptpConnect"), T(""));
	if (strConnect[0])
		buttonState = 1;

        strConnect = websGetVar(wp, T("pptpDisconnect"), T(""));
	if (strConnect[0])
		buttonState = 2;

	strIpMode = websGetVar(wp, T("pptpIpMode"), T(""));
	if (strIpMode[0]) {
		if (!gstrcmp(strIpMode, T("0")))
			val = 0;
		else if (!gstrcmp(strIpMode, T("1")))
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspError))</script>"));
			goto setErr_pptp;
		}
		if ( apmib_set(MIB_PPTP_IPMODE, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup1))</script>"));
			goto setErr_pptp;
		}
	}
	//Obtain an IP address automatically
	if (!gstrcmp(strIpMode, T("0"))) {
		strHostName = websGetVar(wp, T("HostName"), T(""));
		if ( apmib_set(MIB_DYNIP_HOSTNAME, (void *)strHostName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup2))</script>"));
			goto setErr_pptp;
		}
		#if defined(_Edimax_)
			strMac = websGetVar(wp, T("macAddr"), T(""));
		#else
			strMac = websGetVar(wp, T("Mac"), T(""));
		#endif
		if (strMac[0]) {
			if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
				goto setErr_pptp;
			}
			if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
				goto setErr_pptp;
			}
		}
	}
	else
	{
		#if defined(_Edimax_)
			strMac = websGetVar(wp, T("macAddr1"), T(""));
			if (strMac[0]) 
			{
				if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
					goto setErr_pptp;
				}
				if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
					goto setErr_pptp;
				}
			}
		#else
			strMac = websGetVar(wp, T("Mac_static"), T(""));
			if (strMac[0]) {
				if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
					goto setErr_pptp;
				}
				if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
					goto setErr_pptp;
				}
			}
		#endif
	}
	strpptpIPAddr = websGetVar(wp, T("pptpIPAddr"), T(""));
	if ( strpptpIPAddr[0] ) {
		if ( !inet_aton(strpptpIPAddr, &inpptpIPAddr) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_pptp;
		}
		if ( !apmib_set( MIB_PPTPIP_ADDR, (void *)&inpptpIPAddr)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_pptp;
		}
	}
	strpptpIPMaskAddr = websGetVar(wp, T("pptpIPMaskAddr"), T(""));
	if ( strpptpIPMaskAddr[0] ) {
		if ( !inet_aton(strpptpIPMaskAddr, &inpptpIPMaskAddr) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_pptp;
		}
		if ( !apmib_set( MIB_PPTPIPMAKE_ADDR, (void *)&inpptpIPMaskAddr)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_pptp;
		}
	}
	strpptpDfGateway = websGetVar(wp, T("pptpDfGateway"), T(""));
	if ( strpptpDfGateway[0] ) {
		if ( !inet_aton(strpptpDfGateway, &inpptpDfGateway) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_pptp;
		}
		if ( !apmib_set( MIB_PPTPDFGATEWAY, (void *)&inpptpDfGateway)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_pptp;
		}
	}

#ifdef _PPTP_FQDN_
	strpptpGateway = websGetVar(wp, T("pptpGateway"), T(""));
	if ( strpptpGateway[0] ) {
		if ( apmib_set( MIB_PPTPGATEWAY, (void *)strpptpGateway) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_pptp;
		}
	}
#else
	strpptpGateway = websGetVar(wp, T("pptpGateway"), T(""));
	if ( strpptpGateway[0] ) {
		if ( !inet_aton(strpptpGateway, &inpptpGateway) ) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
			goto setErr_pptp;
		}
		if ( !apmib_set( MIB_PPTPGATEWAY, (void *)&inpptpGateway)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
			goto setErr_pptp;
		}
	}
#endif

#if defined(_DUALL_WAN_ACCESS_)

	int duallAccessEnable=0;

	char_t *strDuallAccessEnable;
	strDuallAccessEnable=websGetVar(wp, T("enableDuallAccess"), T(""));
	if ( strDuallAccessEnable[0] && !gstrcmp(strDuallAccessEnable, T("ON")))
		duallAccessEnable = 1;
	apmib_set( MIB_DUALL_ACCESS_ENABLE, (void *)&duallAccessEnable);

	#ifdef _DUAL_WAN_IGMP_
	int DUAL_WAN_IGMP=0;
	char_t *strDUAL_WAN_IGMP;
	strDUAL_WAN_IGMP=websGetVar(wp, T("DUAL_WAN_IGMP"), T(""));
	if (strDUAL_WAN_IGMP[0]) {
		if (!gstrcmp(strDUAL_WAN_IGMP, T("0")))
			DUAL_WAN_IGMP = 0;
		else
			DUAL_WAN_IGMP = 1;
		apmib_set(MIB_DUAL_WAN_IGMP, (void *)&DUAL_WAN_IGMP);
	}
	#endif
#endif

#if defined (_AUTO_DNS_)
	strIpMode = websGetVar(wp, T("DNSMode"), T(""));
	if (strIpMode[0])
	{
		if (!gstrcmp(strIpMode, T("0"))) val = 0;
		else val = 1;
		apmib_set(MIB_WAN_DNS_MODE, (void *)&val);
	}

	strcpy(tmpBuf,"0.0.0.0");

	if(val==0)
	{
		inet_aton(tmpBuf, &inpptpIPAddr);
		apmib_set( MIB_WAN_DNS1, (void *)&inpptpIPAddr);
		apmib_set( MIB_WAN_DNS2, (void *)&inpptpIPAddr);
		apmib_set( MIB_WAN_DNS3, (void *)&inpptpIPAddr);
	}
	else
	{
		strpptpIPAddr = websGetVar(wp, T("dns1"), T(""));
		if(strpptpIPAddr[0])
		{
			inet_aton(strpptpIPAddr, &inpptpIPAddr);
			apmib_set( MIB_WAN_DNS1, (void *)&inpptpIPAddr);
		}

		strpptpIPAddr = websGetVar(wp, T("dns2"), T(""));
		if(strpptpIPAddr[0])
		{
			inet_aton(strpptpIPAddr, &inpptpIPAddr);
			apmib_set( MIB_WAN_DNS2, (void *)&inpptpIPAddr);
		}

		strpptpIPAddr = websGetVar(wp, T("dns3"), T(""));
		if(strpptpIPAddr[0])
		{
			inet_aton(strpptpIPAddr, &inpptpIPAddr);
			apmib_set( MIB_WAN_DNS3, (void *)&inpptpIPAddr);
		}
	}
#endif



	strName = websGetVar(wp, T("pptpUserName"), T(""));
	tmpBuf[0]='\0';
	flag = 1;
	while ( flag ) {
		if ((ptr = strchr(strName, '#')) != NULL) {
			*ptr ='\0';
			ptr++;
			strcat( tmpBuf, strName);
			strcat( tmpBuf, "\\#");
			strName = ptr;
		}
		else {
			strcat( tmpBuf, strName);
			flag = 0;
		}
	}
	if ( tmpBuf[0] ) {
		if ( apmib_set(MIB_PPTP_USER, (void *)tmpBuf) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup1))</script>"));
			goto setErr_pptp;
		}
	}

	strPassword = websGetVar(wp, T("pptpPassword"), T(""));
	if ( strPassword[0] ) {
		if ( apmib_set(MIB_PPTP_PASSWORD, (void *)strPassword) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup2))</script>"));
			goto setErr_pptp;
		}
	}

	strMTU = websGetVar(wp, T("pptpMTU"), T(""));
	if (strMTU[0]) {
		if ( !string_to_dec(strMTU, &val) || val< 512 || val >1492) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup6))</script>"));
			goto setErr_pptp;
		}
		if ( apmib_set(MIB_PPTP_MTU, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup7))</script>"));
			goto setErr_pptp;
		}
	}
//-------------------------------------------------------------------------------------
	strType = websGetVar(wp, T("pptpConnectType"), T(""));
	if ( strType[0] ) {
		PPP_CONNECT_TYPE_T type;
		if ( strType[0] == '0' )
			type = CONTINUOUS;
		else if ( strType[0] == '1' )
			type = CONNECT_ON_DEMAND;
		else if ( strType[0] == '2' )
			type = MANUAL;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup3))</script>"));
			goto setErr_pptp;
		}
		if ( apmib_set(MIB_PPTP_CONNECT_TYPE, (void *)&type) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup4))</script>"));
			goto setErr_pptp;
		}
		if (type != CONTINUOUS) {
			char_t *strTime;
			strTime = websGetVar(wp, T("pptpIdleTime"), T(""));
			if ( strTime[0] ) {
				int time;
				time = strtol(strTime, (char**)NULL, 10) * 60;
				if ( apmib_set(MIB_PPTP_IDLE_TIME, (void *)&time) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup5))</script>"));
					goto setErr_pptp;
				}
			}
		}
	}
	strConntID = websGetVar(wp, T("pptpConntID"), T(""));
	if ( apmib_set(MIB_PPTP_CONNT_ID, (void *)strConntID) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup6))</script>"));
		goto setErr_pptp;
	}
	strBEZEQEnable = websGetVar(wp, T("pptpBEZEQEnable"), T(""));
	if ( !gstrcmp(strBEZEQEnable, T("ON")))
		val = 1;
	else
		val = 0;
	if ( apmib_set( MIB_PPTP_BEZEQ_ENABLE, (void *)&val) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
		goto setErr_pptp;
	}
	strwanMode = websGetVar(wp, T("wanMode"), T(""));
	if ( strwanMode[0] ) {
		if ( !gstrcmp(strwanMode, T("3")))
			Mode = 3;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_pptp;
		}
	}
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif

#ifdef _IQv2_
char_t *iqsetupclose;
iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
if (iqsetupclose[0]){
	int close_iQsetup=1;
	apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
}
#endif
//--------------------------------------------------------------------------------
	if ( buttonState == 1 ) { // connect button is pressed
		int wait_time=30;

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
//		sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all");
//		system(tmpBuf);
		system("/bin/pptp.sh connect"); // Lance 2004.2.3
		while (wait_time-- >0) {
		if (isConnectPPP())
			break;
			sleep(1);
		}
		if (isConnectPPP())
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup11))</script>\n"));
		else
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup12))</script>\n"));
		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}

	if ( buttonState == 2 ) { // disconnect button is pressed

		apmib_update(CURRENT_SETTING);	// update to flash
	#ifndef NO_ACTION
//		sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all");
//		system(tmpBuf);
		system("/bin/disconnect.sh pppoe 0 all"); // Lance 2004.2.3
		#if defined(_Customer_)
				strcpy(tmpBuf, T("PPTP disconnected.")); 
		#else
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormPPTPSetup7))</script>\n"));
		#endif
		OK_MSG1(tmpBuf, submitUrl);
	#endif
		return;
	}
	apmib_update(CURRENT_SETTING);	// update to flash
	// run script
#ifndef NO_ACTION
	// sprintf(tmpBuf, "%s %s", "/bin/init.sh gw", "all"); // Tommy
	// system(tmpBuf);
#endif


	strApply=websGetVar(wp, T("isApply"),T(""));
//EDX start
	if(!strcmp(strApply, "1"))
	   applyEnable=1;
	else
	   applyEnable=0;
//EDX end
	strnextEnable = websGetVar(wp, T("redirect"), T(""));
	if ((strnextEnable[0] || wizardEnabled==1) && (applyEnable == 0)){
		system("/bin/scriptlib_util.sh reloadFlash");
		REPLACE_MSG(submitUrl);
	}
	else
	{
//		#if defined(_Customer_)
//                char_t *strnextEnable;
//                strnextEnable = websGetVar(wp, T("new_apply"), T(""));
//                if (strnextEnable[0])
//                {
//                        system("/bin/reset.sh gw 2");
//                }
//                REPLACE_MSG(submitUrl);
//		#else
//			OK_MSG(wp, submitUrl);
//		#endif
#if 1
		if(strApply[0]){
			if (lanipChanged == 1) {
				char tmpBufnewip[200];
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBufnewip, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufnewip, newurl);
			}
			else
			{			
				char tmpBufapply[200];
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp,tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			#ifdef __TARGET_BOARD__
			system("echo 1 > /tmp/show_reboot_info");
			#endif
			REDIRECT_PAGE(wp, submitUrl);
		}
		return;
#endif
	}
	return;

setErr_pptp:
	ERR_MSG(wp, tmpBuf);

}
////////////////////////////////////////////////////////////////////////////////
void formWanTcpipSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *strIp, *strMask, *strGateway, *strDNS, *strMode, *submitUrl,*strApply;
	char_t *strConnect, *strMac, *strnextEnable, *renewEnable,*HK_HWRN2;
	char_t *strdynIPHostName, *strpppServName, *strpppMTU, *strEnTtl, *strMTU;
	int val, oldtype;
	struct in_addr inIp, inMask,dns1, dns2, dns3, inGateway;
	DHCP_T dhcp, curDhcp;
	DNS_TYPE_T dns, dns_old;
	char tmpBuf[128];
	char_t *strwanMode;
	int Mode, applyEnable;
	int dns_changed=0, flag=1, intMTU;
	char *arg;
	int buttonState=0;
	int buttonState1=0;
	int buttonState2=0;
	char *ptr;
	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strwanMode = websGetVar(wp, T("wanMode"), T(""));


#ifdef _WANTTL_
	char_t *strTTLValue, *strTTLType;
	int intVal=0;
	strTTLType =websGetVar(wp, T("ttlType"), T(""));
	strTTLValue =websGetVar(wp, T("ttlValue"), T(""));

	intVal = strtol( strTTLType, (char **)NULL, 10);
	if ( apmib_set( MIB_TTL_TYPE, (void *)&intVal) == 0) {
		DEBUGP(tmpBuf, T("<script>document.write(Set TTL type error!)</script>"));
		goto setErr_tcpip;
	}

	if (strTTLValue[0]){
			intVal = strtol( strTTLValue, (char **)NULL, 10);
  			if ( apmib_set(MIB_TTL_VALUE, (void *)&intVal) == 0) {
								DEBUGP(tmpBuf, T("<script>document.write(Set TTL value error!))</script>"));
		        goto setErr_tcpip;
				}
	}
#endif
	if (strwanMode[0]) {
		if ( !gstrcmp(strwanMode, T("0")))    Mode = 0;
		if ( !gstrcmp(strwanMode, T("1")))    Mode = 1;
      if ( !gstrcmp(strwanMode, T("2")))    Mode = 2;
		if ( apmib_set( MIB_WAN_MODE, (void *)&Mode) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup1))</script>"));
			goto setErr_tcpip;
		}
	}

	#ifdef _INDEPEND_MTU_
	if(Mode==0 || Mode==1){
		if(Mode)
			strMTU=websGetVar(wp, T("stipMTU"), T(""));
		else
			strMTU=websGetVar(wp, T("dhcpMTU"), T(""));

		intMTU = strtol( strMTU, (char **)NULL, 10);
		if ( apmib_set( MIB_INDEPEND_MTU, (void *)&intMTU) == 0) {
			DEBUGP(tmpBuf, T("apmib set MIB_INDEPEND_MTU fail!"));
			goto setErr_tcpip;
		}
	}
	#endif
//****************************************Dual WAN Access *********************************************
#ifdef _DUALL_WAN_ACCESS_
	int DHCP_MODE=0,STATIC_MODE=1;
	int duallAccessEnable=0,duallAccessType=0;

	char_t *strDuallAccessEnable, *strDuallAccessType;
	strDuallAccessEnable=websGetVar(wp, T("enableDuallAccess"), T(""));
	strDuallAccessType=	websGetVar(wp, T("duallAccessMode"), T(""));
	if ( strDuallAccessEnable[0] && !gstrcmp(strDuallAccessEnable, T("ON")))
		duallAccessEnable = 1;
	if ( strDuallAccessType[0] && !gstrcmp(strDuallAccessType, T("1")))
		duallAccessType=1;
	apmib_set( MIB_DUALL_ACCESS_ENABLE, (void *)&duallAccessEnable);
	apmib_set( MIB_DUALL_ACCESS_MODE, (void *)&duallAccessType);


	if (duallAccessEnable==1 && Mode==2){
		if (duallAccessType==1){
			strIp = websGetVar(wp, T("ip"), T(""));
			if ( strIp[0] ) {
				if ( !inet_aton(strIp, &inIp) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_IP_ADDR, (void *)&inIp)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
					goto setErr_tcpip;
				}
			}

			strMask = websGetVar(wp, T("mask"), T(""));
			if ( strMask[0] ) {
				if ( !inet_aton(strMask, &inMask) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup26))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_SUBNET_MASK, (void *)&inMask)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup27))</script>"));
					goto setErr_tcpip;
				}
			}

			strGateway = websGetVar(wp, T("gateway"), T(""));
			if ( strGateway[0] ) {
				if ( !inet_aton(strGateway, &inGateway) ) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup29))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&inGateway)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup30))</script>"));
					goto setErr_tcpip;
				}
			}
		}else if (duallAccessType==0){
			if (duallAccessType==0){
				strdynIPHostName = websGetVar(wp, T("dynIPHostName"), T(""));
				if ( apmib_set(MIB_DYNIP_HOSTNAME, (void *)strdynIPHostName) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup2))</script>"));
					goto setErr_tcpip;
				}
			}
			// Set clone MAC address
			strMac = websGetVar(wp, T("macAddr2"), T(""));
			if (strMac[0]) {
				if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
					goto setErr_tcpip;
				}
				if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
					goto setErr_tcpip;
				}
			}
		}
	}
	#ifdef _DUAL_WAN_IGMP_
	int DUAL_WAN_IGMP=0;
	char_t *strDUAL_WAN_IGMP;
	strDUAL_WAN_IGMP=websGetVar(wp, T("DUAL_WAN_IGMP"), T(""));
	if (strDUAL_WAN_IGMP[0]) {
		if (!gstrcmp(strDUAL_WAN_IGMP, T("0")))
			DUAL_WAN_IGMP = 0;
		else
			DUAL_WAN_IGMP = 1;
		apmib_set(MIB_DUAL_WAN_IGMP, (void *)&DUAL_WAN_IGMP);
	}
	#endif
#endif
//**************************************** PPPoE Setting ****************************************
   strConnect = websGetVar(wp, T("pppConnect"), T(""));
	if (strConnect[0]) {
		buttonState = 1;
		strMode = T("ppp");
		goto set_ppp;
	}

   strConnect = websGetVar(wp, T("pppDisconnect"), T(""));
	if (strConnect[0]) {
		buttonState = 2;
		strMode = T("ppp");
		goto set_ppp;
	}

	strpppServName = websGetVar(wp, T("pppServName"), T(""));
	if (Mode == 2) {
		if ( apmib_set(MIB_PPP_SERVNAME, (void *)strpppServName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup14))</script>"));
			goto setErr_tcpip;
		}
	}
	strpppMTU = websGetVar(wp, T("pppMTU"), T(""));
	if (strpppMTU[0]) {
		if ( !string_to_dec(strpppMTU, &val) || val< 512 || val >1492) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup6))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_PPP_MTU, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup7))</script>"));
			goto setErr_tcpip;
		}
	}

#ifdef _INDEPEND_DHCP_MTU_
	strpppMTU = websGetVar(wp, T("dhcpMTU"), T(""));
	if (strpppMTU[0]) {
		if ( !string_to_dec(strpppMTU, &val) || val< 512 || val >1492) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup6))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_DHCP_MTU, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup7))</script>"));
			goto setErr_tcpip;
		}
	}
#endif


	strEnTtl = websGetVar(wp, T("pppEnTtl"), T(""));
	if (strEnTtl[0]) {
		if (strEnTtl[0] == '0')
			val = 0;
		else if (strEnTtl[0] == '1')
			val = 1;
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup1))</script>"));
			goto setErr_tcpip;
		}
		if ( apmib_set(MIB_PPP_TTL_ENABLED, (void *)&val) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup2))</script>"));
			goto setErr_tcpip;
		}
	}
	
	// Set clone MAC address
	strMac = websGetVar(wp, T("macAddr1"), T(""));
	if (strMac[0]) {
		if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
			goto setErr_tcpip;
		}
		if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
			goto setErr_tcpip;
		}
	}
//**************************************** Dynamic Ip ****************************************
	if (Mode == 0) {
		strdynIPHostName = websGetVar(wp, T("dynIPHostName"), T(""));
		if ( apmib_set(MIB_DYNIP_HOSTNAME, (void *)strdynIPHostName) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup2))</script>"));
			goto setErr_tcpip;
		}
	}
	// Set clone MAC address
	#ifdef _DUALL_WAN_ACCESS_
	if ( !duallAccessEnable || (duallAccessEnable && duallAccessType))
	{

		strMac = websGetVar(wp, T("macAddr"), T(""));
		if (strMac[0]) {
			if (strlen(strMac)!=12 || !string_to_hex(strMac, tmpBuf, 12)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup16))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpBuf)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormDynIpSetup3))</script>"));
				goto setErr_tcpip;
			}
		}
	}
	#endif

	strIp = websGetVar(wp, T("DNSMode"), T(""));
	if (strIp[0])
	{
		if (!gstrcmp(strIp, T("0"))) val = 0;
		else val = 1;
		apmib_set(MIB_WAN_DNS_MODE, (void *)&val);
	}

	strcpy(tmpBuf,"0.0.0.0");

	if(val==0)
	{
		inet_aton(tmpBuf, &inIp);
		apmib_set( MIB_WAN_DNS1, (void *)&inIp);
		apmib_set( MIB_WAN_DNS2, (void *)&inIp);
		apmib_set( MIB_WAN_DNS3, (void *)&inIp);
	}
	else
	{
		strIp = websGetVar(wp, T("dns1"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS1, (void *)&inIp);
		}

		strIp = websGetVar(wp, T("dns2"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS2, (void *)&inIp);
		}

		strIp = websGetVar(wp, T("dns3"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS3, (void *)&inIp);
		}
	}


	// Read current ip mode setting for reference later
	if ( !apmib_get( MIB_WAN_DHCP, (void *)&curDhcp) ) {
		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup7))</script>"));
		goto setErr_tcpip;
	}

	strMode = websGetVar(wp, T("ipMode"), T(""));
set_ppp:
	if ( strMode[0] ) {


	strIp = websGetVar(wp, T("DNSMode"), T(""));
	if (strIp[0])
	{
		if (!gstrcmp(strIp, T("0"))) val = 0;
		else val = 1;
		apmib_set(MIB_WAN_DNS_MODE, (void *)&val);
	}

	strcpy(tmpBuf,"0.0.0.0");

	if(val==0)
	{
		inet_aton(tmpBuf, &inIp);
		apmib_set( MIB_WAN_DNS1, (void *)&inIp);
		apmib_set( MIB_WAN_DNS2, (void *)&inIp);
		apmib_set( MIB_WAN_DNS3, (void *)&inIp);
	}
	else
	{
		strIp = websGetVar(wp, T("dns1"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS1, (void *)&inIp);
		}

		strIp = websGetVar(wp, T("dns2"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS2, (void *)&inIp);
		}

		strIp = websGetVar(wp, T("dns3"), T(""));
		if(strIp[0])
		{
			inet_aton(strIp, &inIp);
			apmib_set( MIB_WAN_DNS3, (void *)&inIp);
		}
	}

		if ( !strcmp(strMode, T("autoIp")))
			dhcp = DHCP_CLIENT;
		else if ( !strcmp(strMode, T("fixedIp")))
			dhcp = DHCP_DISABLED;
		else if ( !strcmp(strMode, T("ppp"))) {
			char_t	*strName, *strPassword, *strType;
			dhcp = PPPOE;
  			strName = websGetVar(wp, T("pppUserName"), T(""));

			tmpBuf[0]='\0';
			flag = 1;
			while ( flag ) {
				if ((ptr = strchr(strName, '#')) != NULL) {
					*ptr ='\0';
					ptr++;
					strcat( tmpBuf, strName);
					strcat( tmpBuf, "\\#");
					strName = ptr;
				}
				else {
					strcat( tmpBuf, strName);
					flag = 0;
				}
			}

			if ( tmpBuf[0] ) {
				if ( apmib_set(MIB_PPP_USER, (void *)tmpBuf) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup14))</script>"));
					goto setErr_tcpip;
				}
			}
 			strPassword = websGetVar(wp, T("pppPassword"), T(""));
			if ( strPassword[0] ) {
				if ( apmib_set(MIB_PPP_PASSWORD, (void *)strPassword) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup15))</script>"));
					goto setErr_tcpip;
				}
			}
			strType = websGetVar(wp, T("pppConnectType"), T(""));

			printf("strType = %s \n" , strType);
			if ( strType[0] ) {
				PPP_CONNECT_TYPE_T type;
				if ( strType[0] == '0' )
					type = CONTINUOUS;
				else if ( strType[0] == '1' )
					type = CONNECT_ON_DEMAND;
				else if ( strType[0] == '2' )
					type = MANUAL;
				else {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup16))</script>"));
					goto setErr_tcpip;
				}

				if ( apmib_set(MIB_PPP_CONNECT_TYPE, (void *)&type) == 0) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup17))</script>"));
					goto setErr_tcpip;
				}
				if (type != CONTINUOUS) {
					char_t *strTime;
					strTime = websGetVar(wp, T("pppIdleTime"), T(""));
					if ( strTime[0] ) {
						int time;
 						time = strtol(strTime, (char**)NULL, 10) * 60;
						if ( apmib_set(MIB_PPP_IDLE_TIME, (void *)&time) == 0) {
   							DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup18))</script>"));
							goto setErr_tcpip;
						}
					}
				}
			}

		}
		else {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWanTcpipSetup8))</script>"));
			goto setErr_tcpip;
		}
		if ( !apmib_set(MIB_WAN_DHCP, (void *)&dhcp)) {
	  		DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup21))</script>"));
			goto setErr_tcpip;
		}

		if ( dhcp == PPPOE && buttonState == 1 ) { // connect button is pressed
			system("connect_button_is_pressed");
			int wait_time=30;
//---------------------------------------------------------------------------------
			apmib_update(CURRENT_SETTING);	// update to flash


#ifndef NO_ACTION

	#ifdef __TARGET_BOARD__
		system("/bin/pppoe.sh connect");
	#endif


		while (wait_time-- >0) {
			if (isConnectPPP())
				break;
			sleep(1);
		}
		if (isConnectPPP())
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup11))</script>\n"));
		else
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormL2TPSetup12))</script>\n"));

		OK_MSG1(tmpBuf, submitUrl);
#endif
			return;
		}

		if ( dhcp == PPPOE && buttonState == 2 ) { // disconnect button is pressed

			apmib_update(CURRENT_SETTING);	// update to flash
#ifndef NO_ACTION

	#ifdef __TARGET_BOARD__
			system("/bin/disconnect.sh pppoe 0 all"); // Lance 2004.2.3
	#endif

		strcpy(tmpBuf, T("PPPoE disconnected."));
		OK_MSG1(tmpBuf, submitUrl);
#endif
			return;
		}

	}
	else
		dhcp = curDhcp;

	if ( dhcp == DHCP_DISABLED ) {
		strIp = websGetVar(wp, T("ip"), T(""));
		if ( strIp[0] ) {
			if ( !inet_aton(strIp, &inIp) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup23))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set(MIB_WAN_IP_ADDR, (void *)&inIp)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup24))</script>"));
				goto setErr_tcpip;
			}
		}

		strMask = websGetVar(wp, T("mask"), T(""));
		if ( strMask[0] ) {
			if ( !inet_aton(strMask, &inMask) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup26))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set(MIB_WAN_SUBNET_MASK, (void *)&inMask)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup27))</script>"));
				goto setErr_tcpip;
			}
		}

		strGateway = websGetVar(wp, T("gateway"), T(""));
		if ( strGateway[0] ) {
			if ( !inet_aton(strGateway, &inGateway) ) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup29))</script>"));
				goto setErr_tcpip;
			}
			if ( !apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&inGateway)) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormTcpipSetup30))</script>"));
				goto setErr_tcpip;
			}
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

	apmib_update(CURRENT_SETTING);	// update to flash

	// run script
	if ( dns_changed )
		arg = "all";
	else
		arg = "wan";


	strApply = websGetVar(wp, T("isApply"),T(""));
//EDX start
	if(!strcmp(strApply, "1"))
	   applyEnable=1;
	else
	   applyEnable=0;
//EDX end
	strnextEnable = websGetVar(wp, T("redirect"), T(""));
	if ((strnextEnable[0] || wizardEnabled == 1) &&  (applyEnable == 0)){ //EDX
		system("/bin/scriptlib_util.sh reloadFlash");
		REPLACE_MSG(submitUrl);
	}
	else{

#if 1
		if(strApply[0]){
			if (lanipChanged == 1) {
				char tmpBufnewip[200];
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBufnewip, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufnewip, newurl);
			}
			else
			{			
				char tmpBufapply[200];
				sprintf(tmpBufapply, T("<script>document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp,tmpBufapply ,submitUrl);
			}
			#ifdef __TARGET_BOARD__
			system("/bin/reboot.sh 3 &");
			#endif
		}
		else{
			#ifdef __TARGET_BOARD__
			system("echo 1 > /tmp/show_reboot_info");
			#endif
			REDIRECT_PAGE(wp, submitUrl);
		}
		return;
#endif
		//OK_MSG(wp, submitUrl);
	}
	return;
setErr_tcpip:
	ERR_MSG(wp, tmpBuf);
}

int dhcpClientList(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	int nBytesSent=0;
	int element=0, ret;
	char ipAddr[40], macAddr[40], liveTime[80], *buf=NULL, *ptr, tmpBuf[100];
	struct stat status;
	int pid;
	unsigned long longVal,longVal1;

	//kyle
	system("killall -q -USR1 udhcpd");

	// siganl DHCP server to update lease file
	snprintf(tmpBuf, 100, "%s/%s.pid", _DHCPD_PID_PATH, _DHCPD_PROG_NAME);
	pid = getPid(tmpBuf);
	if ( pid > 0)
		kill(pid, SIGUSR1);
	usleep(1000);
	if ( stat(_PATH_DHCPS_LEASES, &status) < 0 )
		goto err;

	buf = malloc(status.st_size);
	if ( buf == NULL )
		goto err;
	fp = fopen(_PATH_DHCPS_LEASES, "r");
	if ( fp == NULL )
		goto err;
	fread(buf, 1, status.st_size, fp);
	fclose(fp);
	ptr = buf+8;

	while (1) {
		ret = getOneDhcpClient(&ptr, &status.st_size, ipAddr, macAddr, liveTime);
		if (ret < 0) break;
		if (ret == 0) continue;
		longVal=strtoul(liveTime,NULL,0);
		if (longVal > 2000000) sprintf(liveTime, "%s", "forever");
		
		if(!element%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\" class=\"dnsTableText\">\n"));

		nBytesSent += websWrite(wp,T("<td width='30%%'>%s</td><td width='30%%'>%s</td><td width='40%%'>%s</td></tr>"),ipAddr, macAddr, liveTime);
		element++;
	}

err:
	if (element == 0) {
		//nBytesSent += websWrite(wp,T("<tr class=table2 align=center><td><script>dw(None)</script></td><td>----</td><td>----</td></tr>"));
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	if (buf)
		free(buf);
	return nBytesSent;
}

/////////////////////////////////////////////////////////////////////////////
void formReflashClientTbl(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl;

	submitUrl = websGetVar(wp, T("submit-url"), T(""));
	if (submitUrl[0])
		websRedirect(wp, submitUrl);
}
#endif
//////////////////////////////////////////////////////////////////////////////
int isDhcpClientExist(char *name)
{
	char tmpBuf[100];
	struct in_addr intaddr;

	if ( getInAddr(name, IP_ADDR, (void *)&intaddr ) ) {
		snprintf(tmpBuf, 100, "%s/%s-%s.pid", _DHCPC_PID_PATH, _DHCPC_PROG_NAME, name);
		if ( getPid(tmpBuf) > 0)
			return 1;
	}
	return 0;
}
#ifdef _IQv2_
void saveAndReboot(webs_t wp, char_t *path, char_t *query)
{
	char_t *strRequest, *strRequest_sitesurvey, *submitUrl, *goSitesuvy, *repEnableCrossBand, *force_enable2g5g_ap;
	char_t *Ap_enable5g, *Ap_enable2g, *rep_enable5g, *rep_enable2g, *APcl_enable5g, *APcl_enable2g, *WISP_enable5g, *WISP_enable2g, *strDNS2, *strMode;

	char_t *strtmpIP;	
	struct in_addr tmpIP;

	DNS_TYPE_T dns;
	FILE *fp,*fp2;
	unsigned char tempbuf[100];
	int rdSize, intOne = 1, intZero = 0, intSix = 6, intval, intTwo=2 ;

	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
	}

	#ifdef _DHCP_SWITCH_
	struct in_addr inIp, inMask, inGateway, inDhcpGatewayIP, dhcpRangeStart, dhcpRangeEnd;
	char_t *goToRep, *strIP, *ipMode, *strDhcp, *strLeaseTime, *strDhcpGatewayIP, *strdhcpRangeStart, *strdhcpRangeEnd;
	int disable2G, disable5G, autoIP2G, autoIP5G;
	DHCP_T dhcp;
	unsigned long longVal;

	goToRep = websGetVar(wp, T("goToRep"), T(""));
	goSitesuvy = websGetVar(wp, T("goSitesuvy"), T(""));
	if(goToRep[0]) {
		if(goSitesuvy[0]) {
			#ifdef __TARGET_BOARD__
			system("killall up_wlan.sh 2> /dev/null");  // for iQsetup disable interface can sitesurvey
			system("/bin/up_wlan.sh &");
			#endif
		}

		system("rm -f /tmp/showConnectResult5 2> /dev/null"); // let wiz_ip5g.asp show flash setting

		char_t *lanipChang;
		lanipChang = websGetVar(wp, T("ipChanged"),T(""));
		if ( lanipChang[0] ){ 
			lanipChanged = atoi(lanipChang);
		}
		
		int autoip;
		ipMode = websGetVar(wp, T("ipmode5g"), T(""));
		if (ipMode[0]) {
			if ( ipMode[0] == '0')
				autoip = 0; //Use the following IP address
			else
				autoip = 1;  //Obtain an IP address automatically

			apmib_set(MIB_DHCP_SWITCH_5G, (void *)&autoip);
		}

		strIP = websGetVar(wp, T("ip5g"), T(""));
		if(strIP[0]) {
			if ( !inet_aton(strIP, &inIp) ) {
				DEBUGP(tempbuf, T("5G IP ERROR"));
				goto setErr_tcpip;
			}
			if ( !apmib_set( MIB_IP_ADDR_5G, (void *)&inIp)) {
				DEBUGP(tempbuf, T("5G IP ERROR"));
				goto setErr_tcpip;
			}
		}
		strIP = websGetVar(wp, T("mask5g"), T(""));
		if(strIP[0]){
			if ( !inet_aton(strIP, &inMask) ) {
				DEBUGP(tempbuf, T("5G MASK ERROR"));
				goto setErr_tcpip;
			}
			if ( !apmib_set( MIB_SUBNET_MASK_5G, (void *)&inMask)) {
				DEBUGP(tempbuf, T("5G MASK ERROR"));
				goto setErr_tcpip;
			}
		}
		strIP = websGetVar(wp, T("gateway5g"), T(""));
		if(strIP[0]){
			if ( !inet_aton(strIP, &inGateway) ) {
				DEBUGP(tempbuf, T("5G GATEWAY ERROR"));
				goto setErr_tcpip;
			}
			if ( !apmib_set( MIB_DEFAULT_GATEWAY_5G, (void *)&inGateway)) {
				DEBUGP(tempbuf, T("5G GATEWAY ERROR"));
				goto setErr_tcpip;
			}
		}

		strMode = websGetVar(wp, T("dnsMode"), T("")); //Robert add 2014_06_11 Set 5G DNS
		if ( strMode[0] ) {
			if (!strcmp(strMode, T("dnsAuto")))
				dns = DNS_AUTO;
			else if (!strcmp(strMode, T("dnsManual")))
				dns = DNS_MANUAL;

			// Set DNS to MIB
			apmib_set(MIB_WAN_DNS_MODE, (void *)&dns); 
			
			if (!strcmp(strMode, T("dnsManual")))
			{
				strDNS2 = websGetVar(wp, T("dns2"), T(""));
				inet_aton(strDNS2, &strDNS2);
				apmib_set( MIB_WAN_DNS2, (void *)&strDNS2);
			}
		}

		#ifdef _HIGHSPEED_WIFI_REPEATER_
		strDhcp = websGetVar(wp, T("HS5_DHCPswitch"), T(""));
		if ( strDhcp[0] )
		{
			if ( strDhcp[0] == '0' )
				dhcp = DHCP_DISABLED;			
			else if (strDhcp[0] == '1')
				dhcp = DHCP_CLIENT;
			else
				dhcp = DHCP_SERVER;
			// set to MIB
			apmib_set(MIB_DHCP, (void *)&dhcp);
		}

		strLeaseTime = websGetVar(wp, T("HS5_leaseTime"), T(""));
		if (strLeaseTime[0]) {
			longVal=strtoul(strLeaseTime,NULL,0);
			apmib_set(MIB_LAN_LEASETIME, (void *)&longVal);
		}

		strDhcpGatewayIP = websGetVar(wp, T("HS5_DhcpGatewayIP"), T(""));
		if ( strDhcpGatewayIP[0] ) {
			inet_aton(strDhcpGatewayIP, &inDhcpGatewayIP);
			apmib_set( MIB_DHCPGATEWAYIP_ADDR, (void *)&inDhcpGatewayIP);
		}

		// Get/Set DHCP client range
		strdhcpRangeStart = websGetVar(wp, T("HS5_dhcpRangeStart"), T(""));
		if ( strdhcpRangeStart[0] ) {
			inet_aton(strdhcpRangeStart, &dhcpRangeStart);
			apmib_set(MIB_DHCP_CLIENT_START, (void *)&dhcpRangeStart);
	
		}
		strdhcpRangeEnd = websGetVar(wp, T("HS5_dhcpRangeEnd"), T(""));
		if ( strdhcpRangeEnd[0] ) {
			inet_aton(strdhcpRangeEnd, &dhcpRangeEnd);
			apmib_set(MIB_DHCP_CLIENT_END, (void *)&dhcpRangeEnd);
		}
		#endif

		#ifdef _IQv2_
		char_t *iqsetupclose;
		iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
		if (iqsetupclose[0]){
			int close_iQsetup=1;
			apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup); // close iqsetup
		}
		#endif
		

		apmib_update(CURRENT_SETTING);
		strRequest = websGetVar(wp, T("isApply"),T(""));

		if(goSitesuvy[0]) 
			submitUrl = goSitesuvy;
		else
			submitUrl = websGetVar(wp, T("submit-url"),T(""));

		if(strRequest[0]){
			printf("5g lanipChanged=%d\n",lanipChanged);
			if (lanipChanged == 1) {
				char tmpBufnewip[200];
				unsigned char buffer[100];
				unsigned char newurl[150];
				if ( !apmib_get( MIB_IP_ADDR_5G,  (void *)buffer) )
					return -1;

				sprintf(newurl, "http://%s", fixed_inet_ntoa(buffer));
				sprintf(tmpBufnewip, T("<script>if(window.sysrst) document.write(showText(SystemRestarting)); else  document.write(showText(SystemRestarting));</script>"));
				REBOOT_MSG(wp, tmpBufnewip, newurl);
			}
			else{			
				char tmpBufapply[200];
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
	#endif //_DHCP_SWITCH_

	goSitesuvy = websGetVar(wp, T("goSitesuvy"), T(""));
	if (goSitesuvy[0]){
		if ( (!gstrcmp(goSitesuvy, T("/wiz_rep24g.asp"))) ||
				 (!gstrcmp(goSitesuvy, T("/wiz_rep5g.asp"))) ){
			#ifdef __TARGET_BOARD__
			system("killall up_wlan.sh 2> /dev/null"); // for iQsetup scan ssid
			system("/bin/up_wlan.sh &");
			#endif
			REPLACE_MSG(goSitesuvy);
			return;
		}
	}

	strRequest = websGetVar(wp, T("RedirectURL"), T(""));
	if (strRequest[0]){

		char_t *strWanMode;
		strWanMode=websGetVar(wp, T("wanMode"), T(""));
		if (strWanMode[0]){
			intval=atoi(strWanMode);
			apmib_set(MIB_WAN_MODE, (void *)&intval);
			apmib_update(CURRENT_SETTING);
		}

		//system("echo 0 > /tmp/pause_iQsetup"); // start detect ipsetup rule
		#ifdef _ENABLE_HW_NAT_
		int hwnatDisable;
		apmib_get( MIB_DISABLE_HW_NAT, (void *)&hwnatDisable);
		if(hwnatDisable!=1)
			system("echo 1 > /proc/hw_nat"); 		// fixed pppoe ping fail bug
		#endif
		websRedirect(wp, strRequest);
		return;
	}

	strRequest_sitesurvey = websGetVar(wp, T("RedirectURL_sitesurvey"), T(""));
	if (strRequest_sitesurvey[0]){
		websRedirect(wp, strRequest_sitesurvey);
		return;
	}
	
	int wizmode, Aband_Repeater_enable=0 , Gband_Repeater_enable=0,tempEnableCrossBand=0;
	apmib_get( MIB_WIZ_MODE, (void *)&wizmode);  // used to record IQsetup mode select		
 	strRequest = websGetVar(wp, T("isSave"), T(""));
	force_enable2g5g_ap = websGetVar(wp, T("force_enable2g5g_ap"), T(""));


	if (strRequest[0]) {
		#ifdef __TARGET_BOARD__
		system("/bin/saveiq.sh &"); //Robert 2014_01_02 update backup data
		sleep(3);
		#endif
		websWrite(wp, "HTTP/1.0 200 OK\n");
		websWrite(wp, "Content-Type: application/octet-stream;\n");

		if(wizmode==0)
			websWrite(wp, "Content-Disposition: attachment;filename=\"Router.txt\" \n");
		else if(wizmode==1)
			websWrite(wp, "Content-Disposition: attachment;filename=\"Access Point.txt\" \n");
		else if(wizmode==2)
			websWrite(wp, "Content-Disposition: attachment;filename=\"Wi-Fi Extender.txt\" \n");
		else if(wizmode==3)
			websWrite(wp, "Content-Disposition: attachment;filename=\"Wi-Fi Bridge.txt\" \n");
		else
			websWrite(wp, "Content-Disposition: attachment;filename=\"WISP.txt\" \n");

		websWrite(wp, "Pragma: no-cache\n");
		websWrite(wp, "Cache-Control: no-cache\n");
		websWrite(wp, "\n");

		unsigned char tempbuf_24G[300]="(2.4G Hz) Wi-Fi password : \n";
		unsigned char tempbuf_5G[300]="(5G Hz) Wi-Fi password : \n";
		unsigned int output_intValue;
		char output_charValue[4]="0x";
		char get24G_key[100]={0};
		char get5G_key[100]={0};

//START EDX, Ken 2015_09_010 add, "password;systemcode".
		if((fp2 = fopen("/tmp/EnableCrossBand","r"))!=NULL) {
			fgets(tempbuf, sizeof(tempbuf), fp2);
			fclose(fp2);
			tempEnableCrossBand=atoi(tempbuf);
		}
		else
		{
			tempEnableCrossBand=0;
		}

		if ((fp2 = fopen("/tmp/ConnectTestKey","r"))!=NULL) {  
			fgets(tempbuf, sizeof(tempbuf), fp2);
			fclose(fp2);
			for(intval=0;intval<(strlen(tempbuf)-1);intval=intval+2)
			{
				output_charValue[2]=tempbuf[intval];
				output_charValue[3]=tempbuf[intval+1];
				output_charValue[4]='\0';
				sscanf(output_charValue,"%x",&output_intValue); // Hexadecimal to Decimal
				sprintf(get24G_key, "%s%c",get24G_key,output_intValue);
			}
		}
		get24G_key[strlen(get24G_key)]='\n';
	
		if ((fp2 = fopen("/tmp/ConnectTestKey5g","r"))!=NULL) { 
			fgets(tempbuf, sizeof(tempbuf), fp2);
			fclose(fp2);
			for(intval=0;intval<(strlen(tempbuf)-1);intval=intval+2)
			{
				output_charValue[2]=tempbuf[intval];
				output_charValue[3]=tempbuf[intval+1];
				output_charValue[4]='\0';
				sscanf(output_charValue,"%x",&output_intValue); // Hexadecimal to Decimal
				sprintf(get5G_key, "%s%c",get5G_key,output_intValue);
			}
		}
		get5G_key[strlen(get5G_key)]='\n';

		if(!gstrcmp(force_enable2g5g_ap, T("1"))){
			tempEnableCrossBand=0;
		}

		fp=fopen("/var/run/iqlog","r");
		if (fp != NULL)
		{
			while (fgets(tempbuf, sizeof(tempbuf), fp) != NULL)
			{
				if(tempEnableCrossBand==0) {
					if(!gstrcmp(force_enable2g5g_ap, T("1"))){
						if ( get24G_key[1] != NULL) {
							if(!strcmp(tempbuf,tempbuf_24G))
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
								websWriteDataNonBlock(wp, get24G_key, strlen(get24G_key));
							}
							else if(!strcmp(tempbuf,tempbuf_5G))
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
								websWriteDataNonBlock(wp, get24G_key, strlen(get24G_key));
							}
							else
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf));
							}
						}
						else {
							if(!strcmp(tempbuf,tempbuf_24G))
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
								websWriteDataNonBlock(wp, get5G_key, strlen(get5G_key));
							}
							else if(!strcmp(tempbuf,tempbuf_5G))
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
								websWriteDataNonBlock(wp, get5G_key, strlen(get5G_key));
							}
							else
							{
								websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf));
							}
						}
					}						
					else {
						if(!strcmp(tempbuf,tempbuf_24G))
						{
							websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
							websWriteDataNonBlock(wp, get24G_key, strlen(get24G_key));

						}
						else if(!strcmp(tempbuf,tempbuf_5G))
						{
							websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
							websWriteDataNonBlock(wp, get5G_key, strlen(get5G_key));
						}
						else
						{
							websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf));
						}
					}
				}
				else{
					if(!strcmp(tempbuf,tempbuf_5G))
					{
						websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
						websWriteDataNonBlock(wp, get24G_key, strlen(get24G_key));
					}
					else if(!strcmp(tempbuf,tempbuf_24G))
					{
						websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf)-1); // Removed "\n"
						websWriteDataNonBlock(wp, get5G_key, strlen(get5G_key));
					}
					else
					{
						websWriteDataNonBlock(wp, tempbuf, strlen(tempbuf));
					}
				}
			}
			fclose(fp);
		} 
//END EDX, Ken 2015_09_010 add, "password;systemcode".
    websDone(wp, 200);
    return;
  }
	#ifdef _TEXTFILE_CONFIG_
	strRequest = websGetVar(wp, T("SaveText"), T(""));
	if (strRequest[0]) {
		#ifdef __TARGET_BOARD__
		system("/bin/saveap.sh &");
		sleep(3);
		#endif
		websWrite(wp, "HTTP/1.0 200 OK\n");
		websWrite(wp, "Content-Type: application/octet-stream;\n");

		websWrite(wp, "Content-Disposition: attachment;filename=\"AP_Config.txt\" \n");

		websWrite(wp, "Pragma: no-cache\n");
		websWrite(wp, "Cache-Control: no-cache\n");
		websWrite(wp, "\n");

		if ((fp = fopen("/var/run/aplog","r"))!=NULL) {
			while ((rdSize = fread(tempbuf, 1, 100, fp)) !=0 )
				  websWriteDataNonBlock(wp, tempbuf, rdSize);
			fclose(fp);
		}
		websDone(wp, 200);
		return;
	}
	#endif

	Ap_enable5g = websGetVar(wp, T("Ap_enable5g"), T(""));
	Ap_enable2g = websGetVar(wp, T("Ap_enable2g"), T(""));
	rep_enable5g = websGetVar(wp, T("rep_enable5g"), T(""));
	rep_enable2g = websGetVar(wp, T("rep_enable2g"), T(""));
	repEnableCrossBand = websGetVar(wp, T("repEnableCrossBand"), T(""));
	APcl_enable5g = websGetVar(wp, T("APcl_enable5g"), T(""));
	APcl_enable2g = websGetVar(wp, T("APcl_enable2g"), T(""));
	WISP_enable5g = websGetVar(wp, T("WISP_enable5g"), T(""));
	WISP_enable2g = websGetVar(wp, T("WISP_enable2g"), T(""));

  strRequest = websGetVar(wp, T("isApply"), T(""));
  if (strRequest[0]) 
	{
		if(wizmode==0){  /* Router mode */
			intval = DHCP_SERVER;
			apmib_set(MIB_DHCP, (void *)&intval); // enable DHCP server
			apmib_set( MIB_INIC_AP_MODE, (void *)&intZero);	
			apmib_set( MIB_AP_MODE, (void *)&intZero);
		}
		else if (wizmode == 1){ /* AP mode */
			#if defined (_Customer_) || defined (_BR6278AC_)
			intval = 1;
			apmib_set(MIB_AP_ROUTER_SWITCH, (void *)&intval);
			#endif
			if( (!gstrcmp(Ap_enable5g, T("1"))) && (!gstrcmp(Ap_enable2g, T("1"))) ){ //AP 5g and 24g enable
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);						
			}
			else if( (!gstrcmp(Ap_enable5g, T("1"))) && (!gstrcmp(Ap_enable2g, T("0"))) ) //AP 5g enable
			{
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);								
			}
			else{
				//AP 24g enable
				apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);								
			}
			
			apmib_set( MIB_INIC_AP_MODE, (void *)&intZero);	
			apmib_set( MIB_AP_MODE, (void *)&intZero);
			
			intval = DHCP_DISABLED;
			apmib_set(MIB_DHCP, (void *)&intval); // disable DHCP server
		}
		else if (wizmode == 3){ /* Bridge mode */
			unsigned char strSSID[100],str5GSSID[100];

			apmib_get( MIB_REPEATER_SSID, (void *)&strSSID);
			apmib_get( MIB_INIC_REPEATER_SSID, (void *)&str5GSSID);

			if( (!gstrcmp(APcl_enable5g, T("1")))){
				apmib_set( MIB_INIC_SSID, (void *)&str5GSSID);	

				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);			
										
				//apmib_set( MIB_INIC_HIDDEN_SSID, (void *)&intTwo);
				//apmib_set( MIB_WLAN_HIDDEN_SSID, (void *)&intZero);
				
				apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intOne);
				apmib_set( MIB_REPEATER_ENABLED, (void *)&intZero);
				Aband_Repeater_enable=1;
				Gband_Repeater_enable=0;
			}	
			else{
				apmib_set( MIB_WLAN_SSID, (void *)&strSSID);

				apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);

				//apmib_set( MIB_INIC_HIDDEN_SSID, (void *)&intZero);
				//apmib_set( MIB_WLAN_HIDDEN_SSID, (void *)&intTwo);

				apmib_set( MIB_REPEATER_ENABLED, (void *)&intOne); 
				apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intZero);
				Aband_Repeater_enable=0;
				Gband_Repeater_enable=1;
			}
			
			apmib_set( MIB_INIC_AP_MODE, (void *)&intTwo);	
			apmib_set( MIB_AP_MODE, (void *)&intTwo);
			//apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intZero);
			//apmib_set( MIB_REPEATER_ENABLED, (void *)&intZero);	
			intval = DHCP_DISABLED;
			apmib_set(MIB_DHCP, (void *)&intval); // disable DHCP server
		}
		else if (wizmode == 4){ /* WISP mode */
			if( (!gstrcmp(WISP_enable5g, T("1")))){
				Aband_Repeater_enable=1;
				Gband_Repeater_enable=0;
			}
			else{
				Aband_Repeater_enable=0;
				Gband_Repeater_enable=1;
			}
			apmib_set( MIB_INIC_AP_MODE, (void *)&intZero);	
			apmib_set( MIB_AP_MODE, (void *)&intZero);
			apmib_set( MIB_DHCP, (void *)&intTwo);
		}
		else{	/* Repeater mode */

			#ifdef _CROSSBAND_
			if(!gstrcmp(repEnableCrossBand, T("1")))
				apmib_set( MIB_CROSSBAND_ENABLE, (void *)&intOne);
			else
				apmib_set( MIB_CROSSBAND_ENABLE, (void *)&intZero);
			#endif

			if( (!gstrcmp(rep_enable5g, T("1"))) && (!gstrcmp(rep_enable2g, T("1"))) ){ //rep 5g and 24g enable
				Aband_Repeater_enable=1;
				Gband_Repeater_enable=1;
				apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);						
				apmib_set( MIB_REPEATER_ENABLED, (void *)&intOne);
				apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intOne);	
				apmib_set( MIB_INIC_AP_MODE, (void *)&intSix);	
				apmib_set( MIB_AP_MODE, (void *)&intSix);
			}
			else if( !gstrcmp(rep_enable5g, T("1"))) //rep 5g enable only
			{
				Aband_Repeater_enable=1;
				Gband_Repeater_enable=0;
				
				if(!gstrcmp(force_enable2g5g_ap, T("1"))){
				    apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				    apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
				}
				else{
				  #ifdef _CROSSBAND_
				  if(!gstrcmp(repEnableCrossBand, T("1"))){
					  apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
					  apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
				  }					
				  else{
				  #endif
					  apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
					  apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);								
				  #ifdef _CROSSBAND_
				  }
				  #endif
				}
				apmib_set( MIB_REPEATER_ENABLED, (void *)&intZero);
				apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intOne);			
			}
			else{ //rep 2.4g enable only
				Aband_Repeater_enable=0;
				Gband_Repeater_enable=1;
				
				if(!gstrcmp(force_enable2g5g_ap, T("1"))){
				    apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
				    apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);
				}
				else{				  
				  #ifdef _CROSSBAND_
				  if(!gstrcmp(repEnableCrossBand, T("1"))){
					  apmib_set( MIB_INIC_DISABLED, (void *)&intZero);			
					  apmib_set( MIB_WLAN_DISABLED, (void *)&intOne);			
				  }				
				  else{
				  #endif
					  apmib_set( MIB_INIC_DISABLED, (void *)&intOne);			
					  apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);	
				  #ifdef _CROSSBAND_
				  }		
				  #endif
				}
				apmib_set( MIB_REPEATER_ENABLED, (void *)&intOne);
				apmib_set( MIB_INIC_REPEATER_ENABLED, (void *)&intZero);	
			}

			intval = DHCP_DISABLED;
			apmib_set(MIB_DHCP, (void *)&intval); // disable DHCP server
			
			
			//EDX Yihong. fixed, repeater mode wps status always config(for SPEC
			if( Aband_Repeater_enable && Gband_Repeater_enable )
			{
				apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&intOne);
				apmib_set(MIB_WPS_CONFIG_STATUS, (void *)&intOne);
				
				apmib_set(MIB_INIC_WPS_CONFIG_STATUS, (void *)&intOne);
				apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&intOne);
			}
			else if (Aband_Repeater_enable && !Gband_Repeater_enable )
			{
				apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&intOne);
				apmib_set(MIB_WPS_CONFIG_STATUS, (void *)&intOne);
			}
			else{
				apmib_set(MIB_INIC_WPS_CONFIG_STATUS, (void *)&intOne);
				apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&intOne);
			}
		}


		#ifdef _HIGHSPEED_WIFI_REPEATER_
		apmib_get(MIB_DHCP_SWITCH, (void *)&autoIP2G);
		apmib_get(MIB_DHCP_SWITCH_5G, (void *)&autoIP5G);
		
		if(autoIP2G){
			if ((fp = fopen("/tmp/apcli0IP","r"))!=NULL) 
			{
				strtmpIP = websGetVar(wp, T("apcli0IP"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_IP_ADDR, (void *)&tmpIP);
				apmib_set(MIB_DHCPGATEWAYIP_ADDR, (void *)&tmpIP);

				strtmpIP = websGetVar(wp, T("apcli0_SUBNET"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_SUBNET_MASK, (void *)&tmpIP);

				strtmpIP = websGetVar(wp, T("apcli0_ROUTER"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DEFAULT_GATEWAY, (void *)&tmpIP);

				strtmpIP = websGetVar(wp, T("apcli0_DNS"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_WAN_DNS1, (void *)&tmpIP);
/*
				apmib_set(MIB_WAN_DNS_MODE, (void *)&intOne);
				apmib_set( MIB_DHCP_SWITCH, (void *)&intZero);
				apmib_set(MIB_DHCP, (void *)&intTwo);
*/		
				strtmpIP = websGetVar(wp, T("apcli0DHCP_START"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DHCP_CLIENT_START, (void *)&tmpIP);
				strtmpIP = websGetVar(wp, T("apcli0DHCP_END"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DHCP_CLIENT_END, (void *)&tmpIP);

				fclose(fp);
			}	
		}

		if(autoIP5G){
			if ((fp = fopen("/tmp/apclii0IP","r"))!=NULL) 
			{
				strtmpIP = websGetVar(wp, T("apclii0IP"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_IP_ADDR_5G, (void *)&tmpIP);
				apmib_set(MIB_DHCPGATEWAYIP_ADDR, (void *)&tmpIP);
		
				strtmpIP = websGetVar(wp, T("apclii0_SUBNET"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_SUBNET_MASK_5G, (void *)&tmpIP);

				strtmpIP = websGetVar(wp, T("apclii0_ROUTER"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DEFAULT_GATEWAY_5G, (void *)&tmpIP);

				strtmpIP = websGetVar(wp, T("apclii0_DNS"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_WAN_DNS2, (void *)&tmpIP);
/*
				apmib_set(MIB_WAN_DNS_MODE, (void *)&intOne);
				apmib_set( MIB_DHCP_SWITCH_5G, (void *)&intZero);
				apmib_set(MIB_DHCP, (void *)&intTwo);
*/
				strtmpIP = websGetVar(wp, T("apclii0DHCP_START"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DHCP_CLIENT_START, (void *)&tmpIP);
				strtmpIP = websGetVar(wp, T("apclii0DHCP_END"), T(""));	
				inet_aton(strtmpIP, &tmpIP);
				apmib_set(MIB_DHCP_CLIENT_END, (void *)&tmpIP);

				}
		}
		#endif

		apmib_set(MIB_IQSET_DISABLE, (void *)&intOne); // close iqsetup
		apmib_set(MIB_IS_RESET_DEFAULT, (void *)&intZero); // 0: avoid show login window in last.asp
		apmib_update(CURRENT_SETTING);
		system("killall -USR1 webs"); // force renew flash value, avoid show login window in last.asp
		if (wizmode == 3)
			websRedirect(wp, T("/last_APcl.asp"));
		else
			websRedirect(wp, T("/last.asp"));

		printf("\n[Aband_Repeater:%d Gband_Repeater:%d]\n\n",Aband_Repeater_enable, Gband_Repeater_enable);
		// save root AP MAC for channel change detect.
		if(Aband_Repeater_enable && Gband_Repeater_enable){
			system("/bin/rootAPmac.sh apclii0");		
			system("/bin/rootAPmac.sh apcli0");	
		}
		else if(Aband_Repeater_enable){
			system("/bin/rootAPmac.sh apclii0");			
		}
		else{
			system("/bin/rootAPmac.sh apcli0");		
		}

		#ifdef __TARGET_BOARD__
		system("/bin/reboot.sh 5 &");
		#endif

		return;
  }

setErr_tcpip:
	ERR_MSG(wp, tempbuf);

}
void setWifi(webs_t wp, char_t * path, char_t * query)
{
	int intVal, intSecurity, intEncrypt, intTkip, intAes, intOne, intTwo; 
	char_t *Ap_enable5g, *Ap_enable2g;
	intOne = 1;
	intTwo = 2;
	intVal = WPA_AUTH_PSK; // 1-RADIUS, 2-pre shared key
	intAes = WPA_CIPHER_AES; // WPA_CIPHER_TKIP=1, WPA_CIPHER_AES=2, WPA_CIPHER_MIXED=3
	intTkip = 0;
	intSecurity = 2;
	intEncrypt = 4;

	int wizMode;
	apmib_get( MIB_WIZ_MODE, (void *)&wizMode);

	Ap_enable2g = websGetVar(wp, T("Ap_enable2g"), T(""));
	if( !gstrcmp(Ap_enable2g, T("1")) || (wizMode==0) || (wizMode==4) )
	{
		//2.4G        
		apmib_set(MIB_WLAN_SSID, (void *)websGetVar(wp, T("ssid"), T("")));
		apmib_set(MIB_WLAN_WPA_PSK, (void *)websGetVar(wp, T("pskValue"), T("")));
		apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intTkip); 
		apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intAes);
		apmib_set(MIB_SECURITY_MODE, (void *)&intSecurity);
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&intEncrypt);
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal); 
		apmib_set(MIB_WPS_CONFIG_TYPE, (void *)&intOne); // Be Configured
		apmib_set(MIB_WPS_CONFIG_STATUS, (void *)&intOne); // Be Configured
	}


	Ap_enable5g = websGetVar(wp, T("Ap_enable5g"), T(""));
	if( !gstrcmp(Ap_enable5g, T("1")) || (wizMode==0) || (wizMode==4) )
	{
		//5G                                                  
		apmib_set(MIB_INIC_SSID, (void *)websGetVar(wp, T("ssid5g"), T("")));
		apmib_set(MIB_INIC_WPA_PSK, (void *)websGetVar(wp, T("pskValue5g"), T("")));
		apmib_set(MIB_INIC_WPA_CIPHER_SUITE, (void *)&intTkip); 
		apmib_set(MIB_INIC_WPA2_CIPHER_SUITE, (void *)&intAes);
		apmib_set(MIB_INIC_SECURITY_MODE, (void *)&intSecurity);
		apmib_set(MIB_INIC_ENCRYPT, (void *)&intEncrypt);
		apmib_set(MIB_INIC_WPA_AUTH, (void *)&intVal); 
		apmib_set(MIB_INIC_WPS_CONFIG_TYPE, (void *)&intOne); // Be Configured
		apmib_set(MIB_INIC_WPS_CONFIG_STATUS, (void *)&intOne); // Be Configured
	}

	system("echo \"1\" > /tmp/wifi_changed");
	apmib_update(CURRENT_SETTING);
	#ifdef __TARGET_BOARD__
	system("saveiq.sh &");
	#endif
	websRedirect(wp, T("/conclusion.asp"));
}

void setWAN(webs_t wp, char_t * path, char_t * query)
{
  char_t *strRequest, *conType, *strDHCPtoPPPOE, *strpptpGateway;
  int wanMode = atoi(websGetVar(wp, T("wanMode"), T("")));
  int tmpVal,i;
  char tmpstr[128], tmpstr1[128], tmpBuf[200];
  struct in_addr inIp;

	#ifdef _Customer_ //Robart Add, Use PPTP or L2TP must disable hw_nat
	if(wanMode>2) {tmpVal = 0;}
	else {tmpVal = 1;}
		apmib_set( MIB_FAST_NAT_ENABLE, (void *)&tmpVal);
	#endif

	apmib_set(MIB_WAN_MODE, (void *)&wanMode);
	switch(wanMode)
	{
	case 1:
		inet_aton( websGetVar(wp, T("ip"), T("0.0.0.0")), &inIp);
		apmib_set(MIB_WAN_IP_ADDR, (void *)&inIp);

		inet_aton( websGetVar(wp, T("mask"), T("0.0.0.0")), &inIp);
		apmib_set(MIB_WAN_SUBNET_MASK, (void *)&inIp);

		inet_aton( websGetVar(wp, T("gateway"), T("0.0.0.0")), &inIp);
		apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&inIp);

		break;
	case 2:
		strcpy( tmpstr1 , websGetVar(wp, T("pppUserName"), T("")) );
		for(i=0; i<strlen(tmpstr1); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "UserName;systemcode".
		{
			if (tmpstr1[i]==59)
			{
				tmpstr1[i]='\0';
				break;
			}
		}
						
		sprintf(tmpBuf, "echo %s > /tmp/UserName", tmpstr1);
		system(tmpBuf);

		memset(&tmpstr[0], 0, sizeof(tmpstr));
		tmpVal=0;
		while( tmpstr1[tmpVal] != 0 )
		{
		    if( tmpstr1[tmpVal] == '#' )
		    {
		      tmpstr[ strlen(tmpstr) ] = '\\';
		      tmpstr[ strlen(tmpstr) ] = '#';
		    }
		    else
		      tmpstr[ strlen(tmpstr) ] = tmpstr1[tmpVal];

		    tmpVal++;
		}

		strDHCPtoPPPOE=websGetVar(wp, T("DHCPtoPPPOE"), T(""));

		if(strDHCPtoPPPOE[0])
			system("echo 6 > /tmp/wanStatus");
		else
			system("echo 4 > /tmp/wanStatus");

		apmib_set(MIB_PPP_USER, (void *)tmpstr);

		apmib_set(MIB_PPP_PASSWORD, (void *)websGetVar(wp, T("pppPassword"), T("")));

		break;
	case 3:
		strcpy( tmpstr1 , websGetVar(wp, T("pptpUserName"), T("")) );
		for(i=0; i<strlen(tmpstr1); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "UserName;systemcode".
		{
			if (tmpstr1[i]==59)
			{
				tmpstr1[i]='\0';
				break;
			}
		}
		sprintf(tmpBuf, "echo %s > /tmp/UserName", tmpstr1);
		system(tmpBuf);	

		memset(&tmpstr[0], 0, sizeof(tmpstr));
		tmpVal=0;
	while( tmpstr1[tmpVal] != 0 )
	{
		if( tmpstr1[tmpVal] == '#' )
		{
			tmpstr[ strlen(tmpstr) ] = '\\';
			tmpstr[ strlen(tmpstr) ] = '#';
		}
		else
		{
			tmpstr[ strlen(tmpstr) ] = tmpstr1[tmpVal];
		}
		tmpVal++;
	}

	apmib_set(MIB_PPTP_USER, (void *)tmpstr);
	apmib_set(MIB_PPTP_PASSWORD, (void *)websGetVar(wp, T("pptpPassword"), T("")));
	//apmib_set(MIB_PPTPGATEWAY, (void *)websGetVar(wp, T("pptpGateway"), T("")));
     

#ifdef _PPTP_FQDN_
	strpptpGateway = websGetVar(wp, T("pptpGateway"), T(""));
	apmib_set(MIB_PPTPGATEWAY, (void *)strpptpGateway);
#else
	strcpy(tmpstr, websGetVar(wp, T("pptpGateway"), T("")));
	if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
	inet_aton( tmpstr, &inIp);
	apmib_set(MIB_PPTPGATEWAY, (void *)&inIp);
#endif

	tmpVal = atoi(websGetVar(wp, T("pptpIpMode"), T("0")));
	apmib_set(MIB_PPTP_IPMODE, (void *)&tmpVal);
	if(tmpVal==1)
	{
		strcpy( tmpstr, websGetVar(wp, T("pptpIPAddr"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_PPTPIP_ADDR, (void *)&inIp);

		strcpy( tmpstr, websGetVar(wp, T("pptpIPMaskAddr"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_PPTPIPMAKE_ADDR, (void *)&inIp);

		strcpy( tmpstr, websGetVar(wp, T("pptpDfGateway"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_PPTPDFGATEWAY, (void *)&inIp);
	}
	break;
    case 6:
		strcpy( tmpstr1 , websGetVar(wp, T("L2TPUserName"), T("")) );
		for(i=0; i<strlen(tmpstr1); i++)//EDX, Ken 2015_09_16 add, Avoid tmpstr1 is "UserName;systemcode".
		{
			if (tmpstr1[i]==59)
			{
				tmpstr1[i]='\0';
				break;
			}
		}
		sprintf(tmpBuf, "echo %s > /tmp/UserName", tmpstr1);
		system(tmpBuf);

		memset(&tmpstr[0], 0, sizeof(tmpstr));
		tmpVal=0;
		while( tmpstr1[tmpVal] != 0 )
		{
		if( tmpstr1[tmpVal] == '#' )
		{
		  tmpstr[ strlen(tmpstr) ] = '\\';
		  tmpstr[ strlen(tmpstr) ] = '#';
		}
		else
		  tmpstr[ strlen(tmpstr) ] = tmpstr1[tmpVal];

		tmpVal++;
		}
		apmib_set(MIB_L2TP_USER, (void *)tmpstr);
		apmib_set(MIB_L2TP_PASSWORD, (void *)websGetVar(wp, T("L2TPPassword"), T("")));
		apmib_set(MIB_L2TP_GATEWAY, (void *)websGetVar(wp, T("L2TPGateway"), T("")));

		tmpVal = atoi(websGetVar(wp, T("L2TPIpMode"), T("0")));
		apmib_set(MIB_L2TP_IPMODE, (void *)&tmpVal);
		if(tmpVal==1)
		{
		strcpy( tmpstr, websGetVar(wp, T("L2TPIPAddr"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_L2TP_IP_ADDR, (void *)&inIp);

		strcpy( tmpstr, websGetVar(wp, T("L2TPMaskAddr"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_L2TP_MAKEADDR, (void *)&inIp);

		strcpy( tmpstr, websGetVar(wp, T("L2TPDefGateway"), T("")) );
		if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
		inet_aton( tmpstr, &inIp);
		apmib_set(MIB_L2TP_DEFGATEWAY, (void *)&inIp);
		}
      
	break;
	default:    
	printf("default\n");
	break;
  }
    
  apmib_set(MIB_DYNIP_HOSTNAME, (void *)websGetVar(wp, T("dynIPHostName"), T("")));
  
  tmpVal = atoi(websGetVar(wp, T("DNSMode"), T("0")));
  apmib_set(MIB_WAN_DNS_MODE, (void *)&tmpVal);
  if(tmpVal==1)
  {
    strcpy( tmpstr, websGetVar(wp, T("dns1"), T("")) );
    if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
    inet_aton( tmpstr, &inIp);
    apmib_set(MIB_WAN_DNS1, (void *)&inIp);

    strcpy( tmpstr, websGetVar(wp, T("dns2"), T("")) );
    if( !tmpstr[0] ) strcpy(tmpstr, "0.0.0.0");
    inet_aton( tmpstr, &inIp);
    apmib_set(MIB_WAN_DNS2, (void *)&inIp);
  }

  strRequest = websGetVar(wp, T("macAddr"), T(""));
  if ( strRequest[0] && strlen(strRequest)==12 && string_to_hex(strRequest, tmpstr, 12) )
      apmib_set(MIB_WAN_MAC_ADDR, (void *)tmpstr);

  apmib_update(CURRENT_SETTING);
	//EDX Hahn 2014/01/02

	char_t *wiz_gwManu, *wiz_wispManu;
	int iqsetup_mode_value;

	wiz_gwManu = websGetVar(wp, T("wiz_gwManu"), T(""));
	wiz_wispManu = websGetVar(wp, T("wiz_wispManu"), T(""));
 
	if ( !gstrcmp(wiz_gwManu, T("1")) ){
		system("echo 1 > /tmp/wiz_gwManu"); // by hand
		websRedirect(wp, "/wifi.asp");
	}
	else if( !gstrcmp(wiz_wispManu, T("1")) ){
		system("echo 1 > /tmp/wiz_wispManu"); // by hand
		websRedirect(wp, "/wifi.asp");
	}
	else{
		//websRedirect(wp, "/probe.asp");
		#ifdef __TARGET_BOARD__

		FILE *kptr;
		int wizmode;
		apmib_get( MIB_WIZ_MODE, (void *)&wizmode);
		if (wizmode==0)
		{
			#if defined (_Customer_) || defined (_BR6278AC_)
			system("/bin/WanPortDetect.sh 4");
			#else
			system("/bin/WanPortDetect.sh 0");
			#endif
			if ((kptr = fopen("/tmp/WanPortDetect","r"))==NULL)
			{
				system("echo 2 > /tmp/pingStatus"); //disconnect
				system("echo 1 > /tmp/pppFail"); // for adhcp_and_pppoe.asp
				system("echo 1 > /tmp/after_delay_ping"); // for adhcp_and_pppoe.asp
				system("echo 1 > /tmp/DetectInternetDone");
				return;
			}
			fclose(kptr);
		}

		printf(">>> test wanMode is %d\n",wanMode);
		switch(wanMode)
		{
			case 0:
				system("setdip.sh &");
				break;
			case 1:
				system("setsip.sh &");
				break;
			case 2:
				system("disconnect.sh all");
				system("setpppoe.sh &");
				break;
			case 3:
				system("setpptp.sh &");
				break;
			case 6:
				system("setl2tp.sh &");
				break;
			default:
				printf("default\n");
				break;
		}
		#endif

		conType=websGetVar(wp, T("conType"),T(""));
		#ifdef __TARGET_BOARD__

			if(conType[0])
				system("echo 1 > /tmp/conType");
			else
				system("echo 0 > /tmp/conType");

			//system("echo 1 > /tmp/pause_iQsetup"); // pause detect ipsetup rule
			#ifdef _ENABLE_HW_NAT_
			system("echo 0 > /proc/hw_nat");       // fixed pppoe ping fail bug
			#endif

			system("/bin/scriptlib_util.sh delay_ping 30 &");
		#endif
	}

	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), " ");
	websDone(wp, 200);	
	return;
}
void chkLink(webs_t wp, char_t * path, char_t * query)
{
	unsigned char strbuf[150];

	char_t *modeSelecct, *back2iQmain, *back25in1, *strUrl, *strIp, *wizmode_value;
	struct in_addr inIp;
	int mode, apmode=0, repeatermode=6, intOne=1, intZero=0;

	modeSelecct = websGetVar(wp, T("modeSelecct"), T(""));
	if (modeSelecct[0])
	{
		mode = strtol( modeSelecct, (char **)NULL, 10);
		/** mode select **/
		apmib_set( MIB_WIZ_MODE, (void *)&mode);
		if( (mode!=2) && (mode!=3))
		{
			apmib_set( MIB_AP_MODE, (void *)&apmode); 
		}
		else
		{
			apmib_set( MIB_AP_MODE, (void *)&repeatermode);
		}
		/** init IP setting **/
		if(mode)
		{
			apmib_set( MIB_DHCP_SWITCH, (void *)&intOne);
			#ifdef _DHCP_SWITCH_
			apmib_set( MIB_DHCP_SWITCH_5G, (void *)&intOne);
			#endif
			strIp = websGetVar(wp, T("ip"), T(""));
			if ( strIp[0] ) {
				if ( inet_aton(strIp, &inIp) ) 
					apmib_set( MIB_IP_ADDR, (void *)&inIp);
			}
			#ifdef _DHCP_SWITCH_
			strIp = websGetVar(wp, T("ip5g"), T(""));
			if ( strIp[0] ) {
				if ( inet_aton(strIp, &inIp) ) 
					apmib_set( MIB_IP_ADDR_5G, (void *)&inIp);
			}
			#endif
		}
		/** init wifi setting **/
		system("rm -f /tmp/wifi_changed"); 
		apmib_set( MIB_WLAN_CHAN_NUM, (void *)&intZero);
		apmib_set( MIB_WLAN_DISABLED, (void *)&intZero);

		system("rm -f /tmp/wifi5g_changed"); 
		apmib_set( MIB_INIC_CHAN_NUM, (void *)&intZero);
		apmib_set( MIB_INIC_DISABLED, (void *)&intZero);

		strUrl = websGetVar(wp, T("submiturl"), T(""));
		apmib_update(CURRENT_SETTING);
		websRedirect(wp, strUrl);
		return ;
	}

	back2iQmain = websGetVar(wp, T("back2iQmain"), T(""));
	if (back2iQmain[0])
	{
		apmib_set( MIB_WIZ_MODE, (void *)&intZero);
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		apmib_update(CURRENT_SETTING);
		websRedirect(wp, strUrl);
		return ;
	}
	back25in1 = websGetVar(wp, T("back25in1"), T(""));
	if (back25in1[0])
	{			
		#ifdef __TARGET_BOARD__
		system("flash default");
		#endif
		wizmode_value = websGetVar(wp, T("wizmode_value"), T(""));
		
		if( (!gstrcmp(wizmode_value, T("1")))){
			system("echo 1 > /tmp/wizmode_value");
		}
		else if( (!gstrcmp(wizmode_value, T("2")))){
			system("echo 2 > /tmp/wizmode_value");
		}
		else if( (!gstrcmp(wizmode_value, T("3")))){
			system("echo 3 > /tmp/wizmode_value");
		}
		else if( (!gstrcmp(wizmode_value, T("4")))){
			system("echo 4 > /tmp/wizmode_value");
		}
#ifdef _EW7478APC_
			apmib_set( MIB_WIZ_MODE, (void *)&intOne);
#else
			apmib_set( MIB_WIZ_MODE, (void *)&intZero);
#endif

		apmib_update(CURRENT_SETTING);
		#ifdef __TARGET_BOARD__
		system("killall -SIGUSR1 webs");
		#endif
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		REPLACE_MSG(strUrl);
		return ;
	}

	char tmp='0';
	FILE *fp, *kptr;
	char_t *strRequest;
	int Wizmode;
	
	apmib_get( MIB_WIZ_MODE, (void *)&Wizmode);  // used to record IQsetup mode select
	strRequest = websGetVar(wp, T("submit-url"), T(""));
	
	if(Wizmode == 0)
	{
		#ifdef __TARGET_BOARD__
		#if defined (_Customer_) || defined (_BR6278AC_)
		system("/bin/WanPortDetect.sh 4");
		#else
		system("/bin/WanPortDetect.sh 0");
		#endif
		//sleep(1);
		if ((kptr = fopen("/tmp/WanPortDetect","r"))==NULL) {
			websRedirect(wp, "/afail.asp");
			return ;
		}
		else{
			fclose(kptr);
			websRedirect(wp, "/detect.asp");
		}
		#else	
		websRedirect(wp, "/afail.asp");
		return ;
		#endif
	}

	#ifdef __TARGET_BOARD__
	//system("echo 1 > /tmp/pause_iQsetup"); // pause detect ipsetup rule
	#ifdef _ENABLE_HW_NAT_
	system("echo 0 > /proc/hw_nat");       // fixed pppoe ping fail bug
	#endif
	#endif

	#ifdef __TARGET_BOARD__
	system("/bin/scriptlib_util.sh iQsetup_detect &");
	#endif
	websRedirect(wp, "/detect.asp");
	return ;
}
void wiz_5in1_redirect(webs_t wp, char_t * path, char_t * query)
{
	char_t *wiz_gwManu, *submiturl, *go5gSitesuvy, *go2gSitesuvy;
	char_t *wiz_wispManu, *WISP_enable5g, *WISP_enable2g;
	char_t *Ap_enable5g, *Ap_enable2g;
	char_t *rep_enable5g, *rep_enable2g;
	char_t *APcl_enable5g, *APcl_enable2g;
	char_t *OtherModeSelect, *back2iQmain, *strUrl, *strIp, *ModeSelect, *strValue;

	unsigned char 
	MSSIDName[20]="edimax.1",
	MSSIDName5g[20]="edimax5G.1",
	GuestName[20]="edimax.guest", 
	GuestName5g[20]="edimax5G.guest"; 
	
	int intValue, mode, apmode=0, APclmode=2, repeatermode=6, intOne=1, intTwo=2, intZero=0;
	int disable2G, disable5G;

	struct in_addr inIp;

	#ifdef _CROSSBAND_
	strValue = websGetVar(wp, T("EnableCrossBand"), T(""));
	if (strValue[0]){

		if (!gstrcmp(strValue, T("1")) )
			system("echo 1 > /tmp/EnableCrossBand");
		else
			system("echo 0 > /tmp/EnableCrossBand");
	
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		REPLACE_MSG(strUrl);
		return;
	}
	#endif
	strValue = websGetVar(wp, T("skip24g"), T(""));
	if (strValue[0]){
		system("echo 0 > /tmp/rep_enable2g");
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh 2> /dev/null");  // for iQsetup disable interface can sitesurvey
		system("/bin/up_wlan.sh &");	
		#endif
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		REPLACE_MSG(strUrl);
		return;
	}

	OtherModeSelect = websGetVar(wp, T("OtherModeSelect"), T(""));
	if (OtherModeSelect[0])
	{
		system("rm -f /tmp/SKipAddRoute");

		mode = strtol( OtherModeSelect, (char **)NULL, 10);
		
		/** mode select **/
		apmib_set( MIB_WIZ_MODE, (void *)&mode);

//#ifdef _WIFI_ROMAING_
//		if (mode != 3)
//		{
//			apmib_set( MIB_SINGLESIGNON, (void *)&mode);
//		}
//#endif
		/*#ifdef _Customer_
		if(mode == 0)
			system("/bin/config-vlan.sh 3 LLLLW");
		else
			system("/bin/config-vlan.sh 3 WLLLL");
		#endif*/

		/*if(mode == 2)
			apmib_set( MIB_AP_MODE, (void *)&repeatermode);
		else if (mode == 3)
			apmib_set( MIB_AP_MODE, (void *)&repeatermode); 
		else
			apmib_set( MIB_AP_MODE, (void *)&apmode);*/

		printf("mode is %d \n",mode);

		/** Guest SSID setting **/
		if(mode == 1) //AP mode
		{
			#ifdef _MSSID_
			apmib_set( MIB_WLAN_SSID_1, (void *)&MSSIDName);
			#endif
			#ifdef _INIC_MSSID_
			apmib_set( MIB_INIC_SSID_1, (void *)&MSSIDName5g);
			#endif
		}
		else
		{
			#ifdef _MSSID_
			apmib_set( MIB_WLAN_SSID_1, (void *)&GuestName); 
			#endif
			#ifdef _INIC_MSSID_
			apmib_set( MIB_INIC_SSID_1, (void *)&GuestName5g); 
			#endif
		}

		/** init IP setting **/
		if(mode)
		{
			#ifdef _DHCP_SWITCH_
			apmib_set( MIB_DHCP_SWITCH, (void *)&intOne);
			apmib_set( MIB_DHCP_SWITCH_5G, (void *)&intOne);
			#endif
			strIp = websGetVar(wp, T("ip"), T(""));
			if ( strIp[0] ) {
				if ( inet_aton(strIp, &inIp) ) 
					apmib_set( MIB_IP_ADDR, (void *)&inIp);
			}
			#ifdef _DHCP_SWITCH_
			strIp = websGetVar(wp, T("ip5g"), T(""));
			if ( strIp[0] ) {
				if ( inet_aton(strIp, &inIp) ) 
					apmib_set( MIB_IP_ADDR_5G, (void *)&inIp);
			}
			#endif
		}

		strUrl = websGetVar(wp, T("submiturl"), T(""));
		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(strUrl);
		return;
	}

	back2iQmain = websGetVar(wp, T("back2iQmain"), T(""));
	if (back2iQmain[0])
	{
		apmib_set( MIB_WIZ_MODE, (void *)&intZero);

//#ifdef _WIFI_ROMAING_
//                if (intZero != 3)
//                {
//                        apmib_set( MIB_SINGLESIGNON, (void *)&intZero);
//                }
//#endif
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(strUrl);
		return;
	}

	ModeSelect = websGetVar(wp, T("ModeSelect"), T(""));
	if (ModeSelect[0])
	{
		if(!gstrcmp(ModeSelect, T("Router"))){
			apmib_set( MIB_WIZ_MODE, (void *)&intZero);
			#ifdef _MSSID_
			apmib_set( MIB_WLAN_SSID_1, (void *)&GuestName); 
			#endif
			#ifdef _INIC_MSSID_
			apmib_set( MIB_INIC_SSID_1, (void *)&GuestName5g); 
			#endif
		}
		else if(!gstrcmp(ModeSelect, T("fistIsAP"))){
			apmib_set( MIB_WIZ_MODE, (void *)&intOne);

//#ifdef _WIFI_ROMAING_
//			if (intOne != 3)
//        	        {
//                	        apmib_set( MIB_SINGLESIGNON, (void *)&intOne);
//        	        }
//#endif	
			system("echo 1 > /tmp/wizmode_value");
		}
		else if(!gstrcmp(ModeSelect, T("Repeater"))){
			//#ifdef _Customer_
			//system("/bin/config-vlan.sh 3 WLLLL");
			//#endif
			#ifdef _EW7478WAP_
			system("echo 2 > /tmp/wizmode_value"); //ken
			#endif
			apmib_set( MIB_WIZ_MODE, (void *)&intTwo);

//#ifdef _WIFI_ROMAING_
//                        if (intTwo != 3)
//                        {
//                                apmib_set( MIB_SINGLESIGNON, (void *)&intTwo);
//                        }
//#endif
			#ifdef _MSSID_
			apmib_set( MIB_WLAN_SSID_1, (void *)&GuestName); 
			#endif
			#ifdef _INIC_MSSID_
			apmib_set( MIB_INIC_SSID_1, (void *)&GuestName5g); 
			#endif
		}
		strUrl = websGetVar(wp, T("submiturl"), T(""));
		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(strUrl);
		return;
	}

	go5gSitesuvy = websGetVar(wp, T("go5gSitesuvy"), T(""));
	go2gSitesuvy = websGetVar(wp, T("go2gSitesuvy"), T(""));
	if (go5gSitesuvy[0] || go2gSitesuvy[0]){
		#ifdef __TARGET_BOARD__
		system("killall up_wlan.sh 2> /dev/null");  // for iQsetup disable interface can sitesurvey
		system("/bin/up_wlan.sh &");
		#endif

		if (go5gSitesuvy[0]){
			apmib_update(CURRENT_SETTING);
			REPLACE_MSG(go5gSitesuvy);
		}
		else{
			apmib_update(CURRENT_SETTING);
			REPLACE_MSG(go2gSitesuvy);
		}
		return; 
	}

	submiturl = websGetVar(wp, T("submiturl"), T(""));   // hidden page

	wiz_gwManu = websGetVar(wp, T("wiz_gwManu"), T(""));
	if( wiz_gwManu[0] != NULL )
	{
		if(!gstrcmp(wiz_gwManu, T("1"))){
		 // by hand
		 system("echo 1 > /tmp/wiz_gwManu");
		 REPLACE_MSG(submiturl);
		 return; 
		}
		else{
		 // by Auto 
		 system("echo 0 > /tmp/wiz_gwManu");  
		 REPLACE_MSG(submiturl); 
		 return; 
		}
	}

	Ap_enable5g = websGetVar(wp, T("Ap_enable5g"), T(""));  //wiz_apmode_select.asp
	Ap_enable2g = websGetVar(wp, T("Ap_enable2g"), T(""));
	
	if(Ap_enable5g[0] && Ap_enable2g[0])
	{
		if (!gstrcmp(Ap_enable5g, T("1")) )
			system("echo 1 > /tmp/Ap_enable5g");
		else
			system("echo 0 > /tmp/Ap_enable5g");

		if (!gstrcmp(Ap_enable2g, T("1")) )
			system("echo 1 > /tmp/Ap_enable2g");
		else
			system("echo 0 > /tmp/Ap_enable2g");

		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(submiturl); // by Auto
		return; 
	}		

	rep_enable5g = websGetVar(wp, T("rep_enable5g"), T(""));
	rep_enable2g = websGetVar(wp, T("rep_enable2g"), T(""));
	strValue  = websGetVar(wp, T("rep_select"), T(""));
	if((rep_enable5g[0] != NULL) && (rep_enable2g[0] != NULL))
	{
		if (!gstrcmp(rep_enable5g, T("1")) )
			system("echo 1 > /tmp/rep_enable5g");	
		else
			system("echo 0 > /tmp/rep_enable5g");

		if (!gstrcmp(rep_enable2g, T("1")) )
			system("echo 1 > /tmp/rep_enable2g");
		else
			system("echo 0 > /tmp/rep_enable2g");
		#ifdef _CROSSBAND_
		system("echo -1 > /tmp/EnableCrossBand");
		#endif
		if(strValue[0]){
			#ifdef __TARGET_BOARD__
			system("killall up_wlan.sh 2> /dev/null");  // for iQsetup disable interface can sitesurvey
			system("/bin/up_wlan.sh &");	
			#endif
		}
		websRedirect(wp, submiturl);
		return; 
	}

	APcl_enable5g = websGetVar(wp, T("APcl_enable5g"), T(""));
	APcl_enable2g = websGetVar(wp, T("APcl_enable2g"), T(""));

	if( (APcl_enable5g[0] != NULL) || (APcl_enable2g[0] != NULL) )
	{
		if (!gstrcmp(APcl_enable5g, T("1")) )
		 system("echo 1 > /tmp/APcl_enable5g");
		else
		 system("echo 0 > /tmp/APcl_enable5g");

		if (!gstrcmp(APcl_enable2g, T("1")) )
		 system("echo 1 > /tmp/APcl_enable2g");
		else
		 system("echo 0 > /tmp/APcl_enable2g");

		/*apmib_get(MIB_WLAN_DISABLED, (void *)&disable2G);
		apmib_get(MIB_INIC_DISABLED, (void *)&disable5G);
		if((disable2G==1) || (disable5G==1)){*/
			#ifdef __TARGET_BOARD__
		  system("killall up_wlan.sh 2> /dev/null");  // for iQsetup disable interface can sitesurvey
		  system("/bin/up_wlan.sh &");
			#endif
	//	}

		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(submiturl); // by Auto
		return; 
	}

	wiz_wispManu = websGetVar(wp, T("wiz_wispManu"), T(""));  //wiz_WISPselect.asp use
	//submiturl = websGetVar(wp, T("submiturl"), T(""));
	if( wiz_wispManu[0] != NULL )
	{
		if ( !gstrcmp(wiz_wispManu, T("1")) ){
		 //iQsetup by hand
		 system("echo 1 > /tmp/wiz_wispManu");
		 REPLACE_MSG(submiturl);
		 return; 
		}
		else{
		 //iQsetup by Auto
		 system("echo 0 > /tmp/wiz_wispManu");  
		 REPLACE_MSG(submiturl);
		 return; 
		}
	}

	WISP_enable5g = websGetVar(wp, T("WISP_enable5g"), T(""));
	WISP_enable2g = websGetVar(wp, T("WISP_enable2g"), T(""));

	if( (WISP_enable5g[0] != NULL) || (WISP_enable2g[0] != NULL) )
	{
		if (!gstrcmp(WISP_enable5g, T("1")) )
		 system("echo 1 > /tmp/WISP_enable5g"); //5g enable
		else
		 system("echo 0 > /tmp/WISP_enable5g"); //5g disable
		
		if (!gstrcmp(WISP_enable2g, T("1")) )
		 system("echo 1 > /tmp/WISP_enable2g"); //24g enable
		else
		 system("echo 0 > /tmp/WISP_enable2g"); //24g disable

		apmib_update(CURRENT_SETTING);
		REPLACE_MSG(submiturl);
		return; 
	}		
}       
#endif   

#ifdef _DNS_PROXY_
#ifdef _EDIT_DNSPROXYURL_ //patrick add
int DNSPROXYURLList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	DNSPROXYURL_T entry;
	char tmpBuf[100];
	char tmpBuf1[100];
	char *ip;

	if ( !apmib_get(MIB_PROXYURL_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get PROXYURL entry error!\n"));
		return -1;
	}

#ifdef __TARGET_BOARD__

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_PROXYURL_TBL, (void *)&entry))
			return -1;

		ip = fixed_inet_ntoa(entry.ipAddr);
		//if ( !strcmp(ip, "0.0.0.0"))
		//	ip = "----";
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		if(i%2)
			nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
		else
			nBytesSent += websWrite(wp, T("<tr style=\"background-color: #FFFFFF;\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"8%%\">\n"));
		nBytesSent += websWrite(wp, T("    %d\n"),i);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"52%%\">\n"));
		nBytesSent += websWrite(wp, T("    %s<input type=\"hidden\" id=\"domainName%d\" name=\"domainName%d\" value=\"%s\">\n"),entry.domainname,i,i,entry.domainname);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"28%%\">\n"));
		nBytesSent += websWrite(wp, T("   %s<input type=\"hidden\" id=\"ProxyServerIP%d\" name=\"ProxyServerIP%d\" value=\"%s\">\n"),ip,i,i,ip);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"12%%\">\n"));
		nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" id=\"select%d\" name=\"select%d\" value=\"ON\">\n"),i,i);
		nBytesSent += websWrite(wp, T("  </td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
		return nBytesSent;
	}

	nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
	return nBytesSent;

#else
	i=1;
	if(0==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
		return nBytesSent;
	}
	else{
		while(1)
		{
			if(i>=6) break;

			nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
			if(i%2)
				nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
			else
				nBytesSent += websWrite(wp, T("<tr style=\"background-color: #FFFFFF;\">\n"));
			nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"8%%\">\n"));
			nBytesSent += websWrite(wp, T("    %d\n"),i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"52%%\">\n"));
			nBytesSent += websWrite(wp, T("    flashDNS%d<input type=\"hidden\" id=\"domainName%d\" name=\"domainName%d\" value=\"flashDNS%d\">\n"),i,i,i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"28%%\">\n"));
			nBytesSent += websWrite(wp, T("   192.168.88.%d<input type=\"hidden\" id=\"ProxyServerIP%d\" name=\"ProxyServerIP%d\" value=\"192.168.88.%d\">\n"),i,i,i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"12%%\">\n"));
			nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" id=\"select%d\" name=\"select%d\" value=\"ON\">\n"),i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("</tr>\n"));
			i=i+1;
		}

		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
		return nBytesSent;
	}

#endif

}
int TemporaryDNSPROXYURLList(int eid, webs_t wp, int argc, char_t **argv)
{
	char DomainName[50],IPaddress[20];
	int nBytesSent=0,i=1,j=0;

#ifdef __TARGET_BOARD__
	FILE *f=fopen("/tmp/basehosts","r");
	if (f) {
		while(1)
		{
			fscanf(f,"%s %s",&IPaddress,&DomainName);
			if(feof(f)) break;
			//printf("\"[%d]\" \"%s\" \"%s\"\n",i,IPaddress,DomainName);
			if(IPaddress[0] != '#')
			{
				nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
				if(i%2)
					nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
				else
					nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\">\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"8%%\">\n"));
				nBytesSent += websWrite(wp, T("    %d\n"),i);
				nBytesSent += websWrite(wp, T("  </td>\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"52%%\">\n"));
				nBytesSent += websWrite(wp, T("    %s<input type=\"hidden\" id=\"TempDomainName%d\" name=\"TempDomainName%d\" value=\"%s\">\n"),DomainName,i,i,DomainName);
				nBytesSent += websWrite(wp, T("  </td>\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"28%%\">\n"));
				nBytesSent += websWrite(wp, T("    %s<input type=\"hidden\" id=\"TempProxyServerIP%d\" name=\"TempProxyServerIP%d\"  value=\"%s\">\n"),IPaddress,i,i,IPaddress);
				nBytesSent += websWrite(wp, T("  </td>\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"dnsTableText\" width=\"12%%\">\n"));
				nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" id=\"TempSelect%d\" name=\"TempSelect%d\" value=\"ON\">\n"),i,i);
				nBytesSent += websWrite(wp, T("  </td>\n"));
				nBytesSent += websWrite(wp, T("</tr>\n"));
				nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"TempIndex%d\" name=\"TempIndex%d\" value=\"%d\">\n"),i,i,j);
				i=i+1;
			}
			j=j+1;
		}	
		fclose(f);
	}
	else{
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
	nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"TempRulesNum\" name=\"TempRulesNum\" value=\"%d\">\n"),i-1);
	
#else

	if(1==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	else{
		while(1)
		{
			if(i>=40) break;
			nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
			if(i%2)
				nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
			else
				nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\">\n"));
			nBytesSent += websWrite(wp, T("  <td align=center width=\"8%%\"class=\"dnsTableText\">\n"));
			nBytesSent += websWrite(wp, T("    %d\n"),i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center width=\"52%%\"class=\"dnsTableText\">\n"));
			nBytesSent += websWrite(wp, T("    www.testDomain.%d<input type=\"hidden\" id=\"TempDomainName%d\" name=\"TempDomainName%d\" value=\"www.testDomain.%d\">\n"),i,i,i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center width=\"28%%\" class=\"dnsTableText\">\n"));
			nBytesSent += websWrite(wp, T("   192.168.22.%d<input type=\"hidden\" id=\"TempProxyServerIP%d\" name=\"TempProxyServerIP%d\"  value=\"192.168.22.%d\">\n"),i,i,i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center width=\"12%%\" class=\"dnsTableText\">\n"));
			nBytesSent += websWrite(wp, T("    <input type=\"checkbox\" id=\"TempSelect%d\" name=\"TempSelect%d\" value=\"ON\">\n"),i,i);
			nBytesSent += websWrite(wp, T("  </td>\n"));
			nBytesSent += websWrite(wp, T("</tr>\n"));
			nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"TempIndex%d\" name=\"TempIndex%d\" value=\"%d\">\n"),i,i,j);
			i=i+1;
			j=j+1;
		}
	}

	nBytesSent += websWrite(wp, T("<!-- ########################################################################################## -->\n"));
	nBytesSent += websWrite(wp, T("<input type=\"hidden\" id=\"TempRulesNum\" name=\"TempRulesNum\" value=\"%d\">\n"),i-1);
#endif
	return nBytesSent;
}
void formDNSProxyrules(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAdd, *strDel, *strDelAll, *strVal, *strVal1, *submitUrl, *strEnabled, *strApply, *iqsetupclose, *strAddSelIndex, *strUserDefineEnabled;
	char tmpBuf[100],tmpBuf1[5],DomainName[50],IPaddress[20];
	int entryNum, i, j, enabled, intSelIndex[100],intSelIndexCount=0;
	DNSPROXYURL_T ProxyrulesEntry;
	strAdd = websGetVar(wp, T("addProxyrules"), T(""));
	strAddSelIndex = websGetVar(wp, T("addSelIndex"), T(""));
	strDel = websGetVar(wp, T("deleteSelProxyrules"), T(""));
	strDelAll = websGetVar(wp, T("deleteAllProxyrules"), T(""));
	strEnabled = websGetVar(wp, T("ProxyruleEnabled"), T(""));
	strUserDefineEnabled = websGetVar(wp, T("UserDefineRulesEnabled"), T(""));

	if (strEnabled[0]) {
		if ( !gstrcmp(strEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( apmib_set( MIB_DNS_PROXY_ENABLE, (void *)&enabled) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
			goto setErr_dnp;
		}

		strVal= websGetVar(wp, T("url1"), T(""));
		printf("Proxy url:%s\n",strVal);
		if (strVal[0]){
			if ( !apmib_set(MIB_DNS_PROXY_URL, (void *)strVal)) {
				DEBUGP(tmpBuf, T("MIB_DNS_PROXY_URL error!!"));
				goto setErr_dnp;
			}
		}
	}

	if (strUserDefineEnabled[0]) {
		if ( !gstrcmp(strUserDefineEnabled, T("ON")))
			enabled = 1;
		else
			enabled = 0;

		if ( apmib_set( MIB_PROXYURL_CUSTOM_ENABLED, (void *)&enabled) == 0) {
  			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
			goto setErr_dnp;
		}
	}


	if (strAdd[0]) {
		strVal = websGetVar(wp, T("ProxyServerIP"), T(""));
		strVal1 = websGetVar(wp, T("domainName"), T(""));

		if (!strVal[0] || !strVal1[0]) {
			strcpy(tmpBuf, T("Get Proxy rules error !!"));
			goto setErr_dnp;
		}
	

		printf(">> ProxyServerIP:%s\n",strVal);
		printf(">> domainName:%s\n",strVal1);

		fixed_inet_aton(strVal, ProxyrulesEntry.ipAddr);

		if (strlen(strVal1) > DN_LEN-1) {
			DEBUGP(tmpBuf, T("Domain Name length error !!"));
			goto setErr_dnp;
		}
		strcpy(ProxyrulesEntry.domainname, strVal1);
		if ( !apmib_get(MIB_PROXYURL_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_dnp;
		}

		if ( (entryNum + 1) > MAX_DNSPROXYURL_NUM) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc5))</script>"));
			goto setErr_dnp;
		}
		//apmib_set(MIB_PROXYURL_DEL, (void *)&ProxyrulesEntry);
		if ( apmib_set(MIB_PROXYURL_ADD, (void *)&ProxyrulesEntry) == 0) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
			goto setErr_dnp;
		}
	}

	#ifdef __TARGET_BOARD__
	if (strAddSelIndex[0]) {
		j=0;
		for(i=0;i<strlen(strAddSelIndex);i++)
		{
			if(strAddSelIndex[i] != ',')
			{
				tmpBuf1[j]=strAddSelIndex[i];
				j=j+1;
			}
			else
			{
				intSelIndex[intSelIndexCount] = atoi(tmpBuf1);
				intSelIndexCount = intSelIndexCount+1;
				memset(tmpBuf1, 0x00, sizeof(tmpBuf1));
				j=0;
			}
		}

		for(i=0;i<intSelIndexCount;i++)
		{
			//printf("select index ==> %d \n",intSelIndex[i]);
			FILE *f=fopen("/tmp/basehosts","r");
			j=0;
			while(1)
			{
				fscanf(f,"%s %s",&IPaddress,&DomainName);
				if(feof(f)) break;

				if( j == intSelIndex[i] )			
				{
					if (!IPaddress[0] || !DomainName[0]) {
						strcpy(tmpBuf, T("Get Proxy rules error !!"));
						goto setErr_dnp;
					}
					//printf(">> domainName:%s\n",DomainName);
					//printf(">> ProxyServerIP:%s\n",IPaddress);

					fixed_inet_aton(IPaddress, ProxyrulesEntry.ipAddr);

					if (strlen(DomainName) > DN_LEN-1) {
						DEBUGP(tmpBuf, T("Domain Name length error !!"));
						goto setErr_dnp;
					}
					strcpy(ProxyrulesEntry.domainname, DomainName);
					if ( !apmib_get(MIB_PROXYURL_NUM, (void *)&entryNum)) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
						goto setErr_dnp;
					}

					if ( (entryNum + 1) > MAX_DNSPROXYURL_NUM) {
						strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc5))</script>"));
						goto setErr_dnp;
					}
					//apmib_set(MIB_PROXYURL_DEL, (void *)&ProxyrulesEntry);
					if ( apmib_set(MIB_PROXYURL_ADD, (void *)&ProxyrulesEntry) == 0) {
						DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc6))</script>"));
						goto setErr_dnp;
					}
				}	
				j=j+1;
			}
			fclose(f);		
		}		
	}
	#endif

	/* Delete entry */
	if (strDel[0]) {

		if ( !apmib_get(MIB_PROXYURL_NUM, (void *)&entryNum)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_dnp;
		}

		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);
			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {
				*((char *)&ProxyrulesEntry) = (char)i;
				if ( !apmib_get(MIB_PROXYURL_TBL, (void *)&ProxyrulesEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspWlAcList1))</script>"));
					goto setErr_dnp;
				}
				if ( !apmib_set(MIB_PROXYURL_DEL, (void *)&ProxyrulesEntry)) {
					DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormRoute21))</script>"));
					goto setErr_dnp;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAll[0]) {

		if ( !apmib_set(MIB_PROXYURL_DELALL, (void *)&ProxyrulesEntry)) {
			DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormWlAc8))</script>"));
			goto setErr_dnp;
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
		REDIRECT_PAGE(wp,submitUrl);
	}
	return;

setErr_dnp:
	ERR_MSG(wp, tmpBuf);
}
#endif
#endif
#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
void formVPNenabled(webs_t wp, char_t *path, char_t *query)
{
	char_t *strEnabled;
	FILE *fp;
	char tmp[5], buffer[100], cmdbuffer[100];
	int intVal=0;


	system("echo 3 > /tmp/vpn_enabled_state");
	intVal=3;

	vpn_enabled_state:
		/* For jquery get data*/
		websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
		websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
		websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
		websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
		websWrite(wp, T("%s"), T("\n"));
		websWrite(wp, T("%d"), intVal);
		websDone(wp, 200);	
		return;	
}
void formVPNsetup(webs_t wp, char_t *path, char_t *query)
{
	system("echo 0 > /tmp/vpn_setup_dialogBox");
	return;
}
void formVPNuser(webs_t wp, char_t *path, char_t *query)
{
	OPENVPNUSER_T entry;
	char_t *iqsetupclose,*strAdd, *strDel, *strUsername, *strPassword, *strVal;
	int intVal, entryNum, i, account_change=0;
	char tmpBuf[200], tmpBufapply[200];

	#ifdef _MULTI_WIZMODE_
		iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
		if (iqsetupclose[0]){
			int close_iQsetup=1;
			apmib_set(MIB_LICENCE, (void *)&close_iQsetup); // close iqsetup
		}
	#endif
	strAdd = websGetVar(wp, T("addAccount"), T(""));
	strDel = websGetVar(wp, T("deleteSelAccount"), T(""));
	
	if (strAdd[0]) {
		account_change=1;
		strUsername = websGetVar(wp, T("username"), T(""));
		strPassword = websGetVar(wp, T("password"), T(""));

		if (!strUsername[0] || !strPassword[0]) {
			strcpy(tmpBuf, T("Get Username or Password error !!"));
			goto setErr_dnp;
		}
	
		printf(">> Username:%s\n",strUsername);
		printf(">> Password:%s\n",strPassword);


		if (strlen(strUsername) > OPENVPNUSER_LEN-1) {
			DEBUGP(tmpBuf, T("Username length error !!"));
			goto setErr_dnp;
		}
		if (strlen(strPassword) > OPENVPNUSER_LEN-1) {
			DEBUGP(tmpBuf, T("Password length error !!"));
			goto setErr_dnp;
		}
	
		strcpy(entry.openvpnuserName,     strUsername);
		strcpy(entry.openvpnuserPassword, strPassword);	
		entry.openvpnUserEnabled=1;
		
		if ( !apmib_get(MIB_OPENVPN_USER_NUM, (void *)&entryNum)) { 
			goto setErr_dnp;
		}
		if ( (entryNum + 1) > MIB_OPENVPN_USER_NUM) { 
			goto setErr_dnp;
		}
		if ( apmib_set(MIB_OPENVPN_USER_ADD, (void *)&entry) == 0) { 
			goto setErr_dnp;
		}
	}

	/* Delete entry */
	if (strDel[0]) {
		account_change=1;
		if ( !apmib_get(MIB_OPENVPN_USER_NUM, (void *)&entryNum)) {
			goto setErr_dnp;
		}

		for (i=entryNum; i>0; i--) {
			if (string_to_dec(strDel, &intVal)){
				if ( intVal == i ) {
					*((char *)&entry) = (char)i;
					if ( !apmib_get(MIB_OPENVPN_USER_TBL, (void *)&entry)) {
						goto setErr_dnp;
					}
					if ( !apmib_set(MIB_OPENVPN_USER_DEL, (void *)&entry)) {
						goto setErr_dnp;
					}
				}
			}
		}
	}

	apmib_update(CURRENT_SETTING);
	system("echo 0 > /tmp/vpn_setup_submit");

	if (account_change==1)
	{
		system("/bin/scriptlib_util.sh flash_to_data_structure_for_openvpn_user");
		system("/bin/scriptlib_util.sh reloadFlash");
		system("killall -SIGUSR1 webs");
	}

	/* For jquery get data*/
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), "submit_ok");
	websDone(wp, 200);	
	return;
	
	setErr_dnp:
		ERR_MSG(wp, tmpBuf);
}
void formVPNCaCrt(webs_t wp, char_t *path, char_t *query)
{
	char_t *strCa_Crt;
	strCa_Crt = websGetVar(wp, T("VPN_Ca_Crt"), T(""));
	//printf("%s\n",strCa_Crt);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/ca.crt","w");
	fprintf(fp1,"%s",strCa_Crt);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=1;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",(intVal1|intVal2)&127);//Submit the eighth bit must 0.
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNDHPem(webs_t wp, char_t *path, char_t *query)
{
	char_t *strDH_Pem;
	strDH_Pem = websGetVar(wp, T("VPN_DH_Pem"), T(""));
	//printf("%s\n",strDH_Pem);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/dh.pem","w");
	fprintf(fp1,"%s",strDH_Pem);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=2;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",intVal1|intVal2);
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNServerCrt(webs_t wp, char_t *path, char_t *query)
{
	char_t *strServer_Crt;
	strServer_Crt = websGetVar(wp, T("VPN_Server_Crt"), T(""));
	//printf("%s\n",strServer_Crt);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/server.crt","w");
	fprintf(fp1,"%s",strServer_Crt);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=4;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",intVal1|intVal2);
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNServerKey(webs_t wp, char_t *path, char_t *query)
{
	char_t *strServer_Key;

	strServer_Key = websGetVar(wp, T("VPN_Server_Key"), T(""));
	//printf("%s\n",strServer_Key);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/server.key","w");
	fprintf(fp1,"%s",strServer_Key);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=8;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",intVal1|intVal2);
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNClientCrt(webs_t wp, char_t *path, char_t *query)
{
	char_t *strClient_Crt;
	strClient_Crt = websGetVar(wp, T("VPN_Client_Crt"), T(""));
	//printf("%s\n",strClient_Crt);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/client.crt","w");
	fprintf(fp1,"%s",strClient_Crt);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=16;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",intVal1|intVal2);
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNClientKey(webs_t wp, char_t *path, char_t *query)
{
	char_t *strClient_Key;
	strClient_Key = websGetVar(wp, T("VPN_Client_Key"), T(""));
	//printf("%s\n",strClient_Key);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/client.key","w");
	fprintf(fp1,"%s",strClient_Key);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=32;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",intVal1|intVal2);
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNStaticKey(webs_t wp, char_t *path, char_t *query)
{
	char_t *strStatic_Key;
	strStatic_Key = websGetVar(wp, T("VPN_Static_Key"), T(""));
	//printf("%s\n",strStatic_Key);
	FILE *fp1;
	fp1=fopen("/tmp/vpnkeys_user/ta.key","w");
	fprintf(fp1,"%s",strStatic_Key);
	fclose(fp1);

	char tmp[10];
	int intVal1=0,intVal2=0;
	if( (fp1=fopen("/tmp/vpn_setup_submit", "r")) != NULL )
	{
		fgets(tmp,5,fp1);
		fclose(fp1);
		intVal2=atoi(tmp);
	}
	intVal1=64;
	fp1=fopen("/tmp/vpn_setup_submit","w");
	fprintf(fp1,"%d\n",(intVal1|intVal2)&127); //Submit the eighth bit must 0.
	fclose(fp1);

	/* For jquery get data*/
	char buffer[20];
	websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
	websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
	websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
	websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
	websWrite(wp, T("%s"), T("\n"));
	websWrite(wp, T("%s"), buffer);
	websDone(wp, 200);	

	return;
}
void formVPNExportClientConf(webs_t wp, char_t *path, char_t *query)
{
    char_t *strClient_conf;
    FILE *fp;
    unsigned char tempbuf[100];
    int rdSize, intVal;

		strClient_conf = websGetVar(wp, T("VPN_Client_conf"), T(""));
    if (strClient_conf[0]) {
			if (string_to_dec(strClient_conf, &intVal))
			{
				if (intVal == 1)
				{
		      websWrite(wp, "HTTP/1.0 200 OK\n");
		      websWrite(wp, "Content-Type: application/octet-stream;\n");
		      websWrite(wp, "Content-Disposition: attachment;filename=\"EDIMAX_redirect_all_traffic.ovpn\" \n");
		      websWrite(wp, "Pragma: no-cache\n");
		      websWrite(wp, "Cache-Control: no-cache\n");
		      websWrite(wp, "\n");

		      if ((fp = fopen("/tmp/openvpn/EDIMAX_redirect_all_traffic.ovpn","r"))!=NULL) {
		          while ((rdSize = fread(tempbuf, 1, 100, fp)) !=0 )
		              websWriteDataNonBlock(wp, tempbuf, rdSize);
		          fclose(fp);
		      }
		      websDone(wp, 200);
				}
				else if (intVal == 2)
				{
		      websWrite(wp, "HTTP/1.0 200 OK\n");
		      websWrite(wp, "Content-Type: application/octet-stream;\n");
		      websWrite(wp, "Content-Disposition: attachment;filename=\"EDIMAX_access_LAN_only.ovpn\" \n");
		      websWrite(wp, "Pragma: no-cache\n");
		      websWrite(wp, "Cache-Control: no-cache\n");
		      websWrite(wp, "\n");

		      if ((fp = fopen("/tmp/openvpn/EDIMAX_access_LAN_only.ovpn","r"))!=NULL) {
		          while ((rdSize = fread(tempbuf, 1, 100, fp)) !=0 )
		              websWriteDataNonBlock(wp, tempbuf, rdSize);
		          fclose(fp);
		      }
		      websDone(wp, 200);
				}
				else if (intVal == 3)
				{
		      websWrite(wp, "HTTP/1.0 200 OK\n");
		      websWrite(wp, "Content-Type: application/octet-stream;\n");
		      websWrite(wp, "Content-Disposition: attachment;filename=\"EDIMAX-client.ovpn\" \n");
		      websWrite(wp, "Pragma: no-cache\n");
		      websWrite(wp, "Cache-Control: no-cache\n");
		      websWrite(wp, "\n");

		      if ((fp = fopen("/tmp/openvpn/EDIMAX-client.ovpn","r"))!=NULL) {
		          while ((rdSize = fread(tempbuf, 1, 100, fp)) !=0 )
		              websWriteDataNonBlock(wp, tempbuf, rdSize);
		          fclose(fp);
		      }
		      websDone(wp, 200);
				}

			}
    }

    return;
}

int OpenVpnAccountList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	OPENVPNUSER_T entry;
	char tmpBuf[100];
	char tmpBuf1[100];
	char *openvpnUserEnabled, *openvpnuserName ,*openvpnuserPassword;

	if ( !apmib_get(MIB_OPENVPN_USER_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get OPENVPN_USER entry error!\n"));
		return -1;
	}

	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" style=\"FONT-SIZE: 11pt; \">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
		return nBytesSent;
	}
	else{
		for (i=1; i<=entryNum; i++) {
			*((char *)&entry) = (char)i;
			if ( !apmib_get(MIB_OPENVPN_USER_TBL, (void *)&entry))
				return -1;

			nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
			nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
			nBytesSent += websWrite(wp, T("<script>\n"));
			nBytesSent += websWrite(wp, T("	if (openvpnLoginAccountTable[%d]==1)\n"),(i*2)-2);
			nBytesSent += websWrite(wp, T(" 	 document.write(\'<td align=center class=\"choose-itemText\" width=\"15%%\">\'+showText(Connect)+\'</td>\')\n"));
			nBytesSent += websWrite(wp, T("	else\n"));
			nBytesSent += websWrite(wp, T(" 	 document.write(\'<td align=center class=\"choose-itemText\" width=\"15%%\">\'+showText(Disconnect)+\'</td>\')\n"));
			nBytesSent += websWrite(wp, T("</script>\n"));
			nBytesSent += websWrite(wp, T("  <td align=center class=\"choose-itemText\" width=\"35%%\">\n"));
			nBytesSent += websWrite(wp, T("    %s<input type=\"hidden\" id=\"UserName%d\" name=\"UserName%d\" value=\"%s\">\n"),entry.openvpnuserName,i,i,entry.openvpnuserName);
			nBytesSent += websWrite(wp, T("  </td>\n"));

			if(i==1)
			{
				nBytesSent += websWrite(wp, T("  <td align=center class=\"choose-itemText\" width=\"35%%\">--</td>\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"choose-itemText\" width=\"15%%\">\n"));
				nBytesSent += websWrite(wp, T(" 	<script>"));
				nBytesSent += websWrite(wp, T(" 		document.write(\'<input type=\"button\" value=\"\'+showText(Change)+\'\" id=\"select%d\" name=\"select%d\" onClick=\"return checkAccountClick(2,%d)\" class=\"ui-button-nolimit\">\');\n"),i,i,i);
				nBytesSent += websWrite(wp, T(" 	</script>"));
				nBytesSent += websWrite(wp, T("  </td>\n"));
			}
			else
			{
				nBytesSent += websWrite(wp, T("  <td align=center class=\"choose-itemText\" width=\"35%%\">\n"));
				nBytesSent += websWrite(wp, T("   %s<input type=\"hidden\" id=\"UserPassword%d\" name=\"UserPassword%d\" value=\"%s\">\n"),entry.openvpnuserPassword,i,i,entry.openvpnuserPassword);
				nBytesSent += websWrite(wp, T("  </td>\n"));
				nBytesSent += websWrite(wp, T("  <td align=center class=\"choose-itemText\" width=\"15%%\">\n"));
				nBytesSent += websWrite(wp, T(" 	<script>"));
				nBytesSent += websWrite(wp, T(" 		document.write(\'<input type=\"button\" value=\"\'+showText(Delete)+\'\" id=\"select%d\" name=\"select%d\" onClick=\"return checkAccountClick(1,%d)\" class=\"ui-button-nolimit\">\');\n"),i,i,i);
				nBytesSent += websWrite(wp, T(" 	</script>"));
				nBytesSent += websWrite(wp, T("  </td>\n"));
			}


			nBytesSent += websWrite(wp, T("</tr>\n"));
		}
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
		return nBytesSent;
	}

}

#endif  

#ifdef _DISCONMSG_
void formToken(webs_t wp, char_t *path, char_t *query)
{
	
	
	int val;
	char_t *submitUrl, *Aband, *Gband;

	Gband = websGetVar(wp, T("Gband"), T(""));
	Aband = websGetVar(wp, T("Aband"), T(""));

	if(Gband[0])
	{
		system("rm -rf /tmp/GbandDisconnectFirst"); 
		system("echo 1 > /tmp/GbandDisconnectAfter"); 
	}
	
	if(Aband[0])
	{
		system("rm -rf /tmp/AbandDisconnectFirst"); 
		system("echo 1 > /tmp/AbandDisconnectAfter"); 
	}

	submitUrl = websGetVar(wp, T("submit-url"), T(""));
	REPLACE_MSG(submitUrl);
	return;
}
#endif
