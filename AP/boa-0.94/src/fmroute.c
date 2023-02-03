/*
 *      Web server handler routines for management (password, save config, f/w update)
 *
 *      Authors: sc_yang <sc_yang@realtek.com.tw>
 *
 *      $Id
 *
 */
#ifdef ROUTE_SUPPORT
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/reboot.h>
#include <unistd.h>

#include "../webs.h"
#ifndef _BOA_
#include "../um.h"
#endif
#include "apmib.h"
#include "apform.h"
#include "utility.h"

#ifdef HOME_GATEWAY
void formRoute(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strAddRoute, *strDelRoute, *strVal, *strDelAllRoute;
	char_t *strIp, *strMask, *strGateway, *strRip;
	char tmpBuf[100];
	char str1[30], str2[30], str3[30];
	int entryNum, intVal, i;
	STATICROUTE_T entry, checkentry;
	unsigned int * a1, *a2, *b1, *b2;
	int ok_msg =0 ;
#ifndef NO_ACTION
	int pid;
#endif
	strAddRoute = websGetVar(wp, T("addRoute"), T(""));
	strDelRoute = websGetVar(wp, T("deleteSelRoute"), T(""));
	strDelAllRoute = websGetVar(wp, T("deleteAllRoute"), T(""));
	strRip	= websGetVar(wp, T("ripSetup"), T(""));
	memset(&entry, '\0', sizeof(entry));
	if(strRip[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
                        intVal = 1;
                else
                        intVal = 0;	
		if ( apmib_set( MIB_RIP_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormPreventionSetup1))</script>"));
			goto setErr_route;
		}
		strVal = websGetVar(wp, T("ripWanTx"), T(""));
		if(strVal[0]){
			intVal = atoi(strVal);
			if ( apmib_set( MIB_RIP_WAN_TX, (void *)&intVal) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute2))</script>"));
				goto setErr_route;
			}
		}
		strVal = websGetVar(wp, T("ripWanRx"), T(""));
		if(strVal[0]){
			intVal = atoi(strVal);
			if ( apmib_set( MIB_RIP_WAN_RX, (void *)&intVal) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute3))</script>"));
				goto setErr_route;
			}
		}

		strVal = websGetVar(wp, T("ripLanTx"), T(""));
		if(strVal[0]){
			intVal = atoi(strVal);
			if ( apmib_set( MIB_RIP_LAN_TX, (void *)&intVal) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute4))</script>"));
				goto setErr_route;
			}
		}
		strVal = websGetVar(wp, T("ripLanRx"), T(""));
		if(strVal[0]){
			intVal = atoi(strVal);
			if ( apmib_set( MIB_RIP_LAN_RX, (void *)&intVal) == 0) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute5))</script>"));
				goto setErr_route;
			}
		}
		ok_msg = 1;
	}
	/* Add new static route table */
	if (strAddRoute[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_STATICROUTE_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc1))</script>"));
			goto setErr_route;
		}

		strIp = websGetVar(wp, T("ipAddr"), T(""));
		strMask= websGetVar(wp, T("subnet"), T(""));
		strGateway= websGetVar(wp, T("gateway"), T(""));
		
		if (!strIp[0] && !strMask[0] && !strGateway[0])
			goto setOk_route;

		fixed_inet_aton(strIp,entry.dstAddr);
		fixed_inet_aton(strMask, entry.netmask);
		fixed_inet_aton(strGateway, entry.gateway);

		if ( !apmib_get(MIB_STATICROUTE_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_route;
		}

		if ( (entryNum + 1) > MAX_ROUTE_NUM) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc5))</script>"));
			goto setErr_route;
		}
		a1 = (unsigned int *) entry.dstAddr ;
		a2 = (unsigned int *) entry.netmask;
		for(i=1; i <= entryNum ; i++) {
			*((char *)&checkentry) = (char)i;
			if ( !apmib_get(MIB_STATICROUTE, (void *)&checkentry)){
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormVpnSetup16))</script>"));
				goto setErr_route;
			}
			b1 = (unsigned int *) checkentry.dstAddr ;
			b2 = (unsigned int *) checkentry.netmask;
			if((*a1 & *a2) == (*b1&*b2)){
				sprintf(tmpBuf, T("<script>document.write(showText(aspFormRoute9))</script> %d!"),i);
				goto setErr_route;
			}
		}
		sprintf(tmpBuf, "route add -net %s netmask %s gw %s", strIp, strMask, strGateway);
		if(system(tmpBuf) != 0){
			strcpy(tmpBuf, "<script>document.write(showText(aspFormRoute10))</script>\n");
			goto setErr_route ;
		}
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_STATICROUTE_DEL, (void *)&entry);
		if ( apmib_set(MIB_STATICROUTE_ADD, (void *)&entry) == 0) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute11))</script>"));
			goto setErr_route;
		}
	}

	/* Delete entry */
	if (strDelRoute[0]) {
		if ( !apmib_get(MIB_STATICROUTE_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute12))</script>"));
			goto setErr_route;
		}

		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);
			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {
				*((char *)&entry) = (char)i;
				if ( !apmib_get(MIB_STATICROUTE, (void *)&entry)) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute18))</script>"));
					goto setErr_route;
				}
				strcpy(str1, fixed_inet_ntoa(entry.dstAddr));
				strcpy(str2, fixed_inet_ntoa(entry.netmask));
				strcpy(str3, fixed_inet_ntoa(entry.gateway));
				sprintf(tmpBuf, "route del -net %s netmask %s gw %s\n", str1, str2, str3);
				system(tmpBuf);
				if ( !apmib_set(MIB_STATICROUTE_DEL, (void *)&entry)) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute19))</script>"));
					goto setErr_route;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAllRoute[0]) {
		if ( !apmib_get(MIB_STATICROUTE_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormWlAc4))</script>"));
			goto setErr_route;
		}
		for(i=1; i <= entryNum ; i++) {
			*((char *)&entry) = (char)i;
			if ( !apmib_get(MIB_STATICROUTE, (void *)&entry)){
				strcpy(tmpBuf, T("<script>document.write(showText(aspFormVpnSetup16))</script>"));
				goto setErr_route;
			}
			strcpy(str1, fixed_inet_ntoa(entry.dstAddr));
			strcpy(str2, fixed_inet_ntoa(entry.netmask));
			strcpy(str3, fixed_inet_ntoa(entry.gateway));
			sprintf(tmpBuf, "route del -net %s netmask %s gw %s\n", str1, str2, str3);
			system(tmpBuf);

		}
		if ( !apmib_set(MIB_STATICROUTE_DELALL, (void *)&entry)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspFormRoute22))</script>"));
			goto setErr_route;
		}
	}

setOk_route:
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
	if(ok_msg){
		pid = fork();
		if (pid) {
			waitpid(pid, NULL, 0);
		}
		else if (pid == 0) {
			snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _ROUTE_SCRIPT_PROG);
			execl( tmpBuf, _CONFIG_SCRIPT_PROG, NULL);
			exit(1);
		}
	}
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	if(ok_msg){
#if defined(_Customer_)
                char_t *strnextEnable;
                strnextEnable = websGetVar(wp, T("new_apply"), T(""));
                if (strnextEnable[0])
                {
                        system("/bin/reset.sh gw 2");
                }
                REPLACE_MSG(submitUrl);
#else
		OK_MSG (wp, submitUrl);	
#endif
		return ;
	}
	if (submitUrl[0])
		websRedirect(wp, submitUrl);
	else
		websDone(wp, 200);
  	return;

setErr_route:
	ERR_MSG(wp, tmpBuf);
}

int staticRouteList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	STATICROUTE_T entry;
	char	ip[30], netmask[30], gateway[30], *tmpStr;

	if ( !apmib_get(MIB_STATICROUTE_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	nBytesSent += websWrite(wp, T("<tr>"
      	"<td align=center width=\"25%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Destination IP Address</b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Netmask </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Gateway </b></font></td>\n"
      	"<td align=center width=\"15%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Select</b></font></td></tr>\n"));

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_STATICROUTE, (void *)&entry))
			return -1;

		tmpStr = fixed_inet_ntoa(entry.dstAddr);
		strcpy(ip, tmpStr);
		tmpStr = fixed_inet_ntoa(entry.netmask);
		strcpy(netmask, tmpStr);
		tmpStr = fixed_inet_ntoa(entry.gateway);
		strcpy(gateway, tmpStr);

		nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center width=\"25%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"15%%\" bgcolor=\"#C0C0C0\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
				ip, netmask, gateway, i);
	}
	return nBytesSent;
}

#include <net/route.h>
int kernelRouteList(int eid, webs_t wp, int argc, char_t **argv)
{
	char buff[256];
	int nl = 0;
	struct in_addr dest;
	struct in_addr gw;
	struct in_addr mask;
	int flgs, ref, use, metric, mtu, win, ir;
	char flags[64];
	unsigned long int d, g, m;

	char sdest[16], sgw[16];
	int	nBytesSent=0;

	FILE *fp = fopen("/proc/net/route", "r");
	nBytesSent += websWrite(wp, T("<tr>"
      	"<td align=center width=\"25%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Destination </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Gateway </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Genmask </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Flags </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Metric </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Ref </b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Use </b></font></td>\n"
      	"<td align=center width=\"15%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Iface </b></font></td></tr>\n"));

	while (fgets(buff, sizeof(buff), fp) != NULL) {
		if (nl) {
			int ifl = 0;
			while (buff[ifl] != ' ' && buff[ifl] != '\t' && buff[ifl] != '\0')
				ifl++;
			buff[ifl] = 0;	/* interface */
			if (sscanf(buff + ifl + 1, "%lx%lx%X%d%d%d%lx%d%d%d",
					   &d, &g, &flgs, &ref, &use, &metric, &m, &mtu, &win,
					   &ir) != 10) {
				printf("Unsuported kernel route format\n");
			}
			ifl = 0;	/* parse flags */
			if (flgs & RTF_UP) {
				if (flgs & RTF_REJECT)
					flags[ifl++] = '!';
				else
					flags[ifl++] = 'U';
				if (flgs & RTF_GATEWAY)
					flags[ifl++] = 'G';
				if (flgs & RTF_HOST)
					flags[ifl++] = 'H';
				if (flgs & RTF_REINSTATE)
					flags[ifl++] = 'R';
				if (flgs & RTF_DYNAMIC)
					flags[ifl++] = 'D';
				if (flgs & RTF_MODIFIED)
					flags[ifl++] = 'M';
				flags[ifl] = 0;
				dest.s_addr = d;
				gw.s_addr = g;
				mask.s_addr = m;
				strcpy(sdest, inet_ntoa(dest));
				strcpy(sgw, inet_ntoa(gw));

	nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center width=\"25%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-16s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-16s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-16s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-6s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-6d</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%-2d</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%7d</td>\n"
      			"<td align=center width=\"15%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td></tr>\n"),
			sdest, sgw, inet_ntoa(mask), flags, metric, ref, use, buff);
			}
		}
		nl++;
	}
	return nBytesSent;
}
#endif

#endif //ROUTE_SUPPORT
