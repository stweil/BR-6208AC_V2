/*
 *      Web server handler routines for Dynamic DNS 
 *
 *      Authors: Shun-Chin  Yang	<sc_yang@realtek.com.tw>
 *
 *      $Id
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "../webs.h"
#ifndef _BOA_
#include "../um.h"
#endif
#include "apmib.h"
#include "apform.h"
#include "utility.h"

#define _DDNS_SCRIPT_PROG	T("ddns.sh")
void formDdns(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl;
	char tmpBuf[100];

	
#ifndef NO_ACTION
	int pid;
#endif
	int enabled=0 ,ddnsType=0 ;
	char *tmpStr ;
	       
	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	
	tmpStr = websGetVar(wp, T("ddnsEnabled"), T(""));  
	if(!strcmp(tmpStr, "ON"))
		enabled = 1 ;
	else 
		enabled = 0 ;

	if ( apmib_set( MIB_DDNS_ENABLED, (void *)&enabled) == 0) {
		strcpy(tmpBuf, T("<script>document.write(showText(aspformDdnsErrFlag))</script>"));
		goto setErr_ddns;
	}
	
	if(enabled){
		tmpStr = websGetVar(wp, T("ddnsType"), T(""));  
		if(tmpStr[0]){
		ddnsType = tmpStr[0] - '0' ;
	 		if ( apmib_set(MIB_DDNS_TYPE, (void *)&ddnsType) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformDdnsErrType))</script>"));
					goto setErr_ddns;
			}
		}
		tmpStr = websGetVar(wp, T("ddnsUser"), T(""));  
		if(tmpStr[0]){
			if ( apmib_set(MIB_DDNS_USER, (void *)tmpStr) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformDdnsErrUser))</script>"));
					goto setErr_ddns;
			}
		}
		tmpStr = websGetVar(wp, T("ddnsPassword"), T(""));  
		if(tmpStr[0]){
			if ( apmib_set(MIB_DDNS_PASSWORD, (void *)tmpStr) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformDdnsErrPasswd))</script>"));
					goto setErr_ddns;
			}	
		}
		tmpStr = websGetVar(wp, T("ddnsDomainName"), T(""));  
		if(tmpStr[0]){
			if ( apmib_set(MIB_DDNS_DOMAIN_NAME, (void *)tmpStr) == 0) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformDdnsErrPasswd))</script>"));
					goto setErr_ddns;
			}	
		}		
	}

	apmib_update(CURRENT_SETTING);
#ifndef NO_ACTION
	pid = find_pid_by_name("ddns.sh");
	if(pid)
		kill(pid, SIGTERM);

	pid = fork();
        if (pid)
		waitpid(pid, NULL, 0);
        else if (pid == 0) {
		snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _DDNS_SCRIPT_PROG);
		execl( tmpBuf, _DDNS_SCRIPT_PROG, "option", NULL);
               	exit(1);
       	}
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

setErr_ddns:
	ERR_MSG(wp, tmpBuf);
}
