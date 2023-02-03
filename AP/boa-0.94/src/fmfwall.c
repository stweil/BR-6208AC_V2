/*
 *      Web server handler routines for firewall
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: fmfwall.c,v 1.2 2005/05/13 07:53:47 tommy Exp $
 *
 */

/*-- System inlcude files --*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/wait.h>

#include "../webs.h"
#include "apform.h"
#include "apmib.h"
#include "utility.h"

//int need_reboot = 0;		//removed by Erwin
int Changed = 0;
int First   = 1;
int newpage = 1;    // 1: new page  0: original page


#ifdef HOME_GATEWAY

int selectNum = -1;
int selectWan = -1;

URLB_T URLBEntry;	// for URL blocking
void *pEntry;

void initQos() {
	int i, num_id, add_id,intVal;
	QOS_T entry;

	memset(&entry, '\0', sizeof(entry));
#if 0 //#ifdef _Customer_
	for (i=0; i<=1; i++) {
		if (i == 0) {
			add_id = MIB_WAN1_QOS_ADD;
			num_id = MIB_WAN1_QOS_NUM;
		}
		else if (i == 1) {
			add_id = MIB_WAN2_QOS_ADD;
			num_id = MIB_WAN2_QOS_NUM;
		}
/*		else if (i == 2) {
			add_id = MIB_WAN3_QOS_ADD;
			num_id = MIB_WAN3_QOS_NUM;
		}
		else {
			add_id = MIB_WAN4_QOS_ADD;
			num_id = MIB_WAN4_QOS_NUM;
		}
*/		apmib_get( num_id, (void *)&intVal);
		if ( intVal == 0 ) {
			strcpy(entry.ruleName, "Others");
//			entry.bwidthVal = 3000;
			entry.sourType = 2;
			apmib_set( add_id, (void *)&entry);	//MIB_QOS_ADD
		}
	}
#else
	//for (i=0; i<=3; i++) {
		//if (i == 0) {
			//add_id = MIB_WAN1_QOS_ADD;
			//num_id = MIB_WAN1_QOS_NUM;
		//}
/*		else if (i == 1) {
			add_id = MIB_WAN2_QOS_ADD;
			num_id = MIB_WAN2_QOS_NUM;
		}
		else if (i == 2) {
			add_id = MIB_WAN3_QOS_ADD;
			num_id = MIB_WAN3_QOS_NUM;
		}
		else {
			add_id = MIB_WAN4_QOS_ADD;
			num_id = MIB_WAN4_QOS_NUM;
		}
*/		apmib_get( MIB_WAN1_QOS_NUM, (void *)&intVal);
		if ( intVal == 0 ) {
			strcpy(entry.ruleName, "Others");
//			entry.bwidthVal = 3000;
			entry.sourType = 2;
			apmib_set(MIB_WAN1_QOS_ADD, (void *)&entry);	//MIB_QOS_ADD
		}
	//}
#endif
//	apmib_update(CURRENT_SETTING);
}
/////////////////////////////////////////////////////////////////////////////
void formQoS(webs_t wp, char_t *path, char_t *query)
{

	char_t *strAddQos, *strDelQos, *strDelAllQos, *strApplyQos, *strEnable, *strApply, *strIsEnabled;
	char_t *strMvUp, *strMvDown, *strEditQos, *strShowQos, *strConfigWan;
	char_t *strName, *strBWVal, *strBWType, *strDUType, *strDownVal, *strDownType;
	char_t *strWanPort, *submitUrl, *strVal;
	char_t *strScType, *strScStIp, *strScEdIp, *strScMac;
	char_t *strDtStIp, *strDtEdIp;
	char_t *strTfType, *strProtl, *strScPort, *strDtPort, *strPortTmp, *strDiffSev, *strConfigNum;
	char_t *strMaxDown, *strMaxUp, *QoStype;
	char_t *strModeState;
	#ifdef _Customer_
	char_t *strfrm;
	#endif
	char tmpBuf[100];
	char *ptr1, *ptr2;
	int entryNum, i, selNum, andNum, intVal;
	int num_id, qos_id, add_id, edit_id, config_id, del_id, delall_id, send_id, enable_id;
	struct in_addr curIpAddr,entIpAddr ,curSubnet;
	unsigned long v1, v2, v3, v4;
	char scPortRange[100];
	char scPortRangeTbl[100]={0};
	char dtPortRange[100];
	char dtPortRangeTbl[100]={0};
	QOS_T entry, orgEntry;
	EDIT_T Edit;

	#ifdef _EZ_QOS_
		char_t *strEZQosMode;
	
		strEZQosMode = websGetVar(wp, T("EZQos_Mode"), T(""));

		if (strEZQosMode[0])
		{
			intVal = 1;
			if (!apmib_set(MIB_EZ_QOS_MODE, (void *)&intVal))
			{
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
				goto setErr_qos;
			}
		
			intVal = 0;
	
			if ( apmib_set( MIB_EZ_QOS_ENABLED, (void *)&intVal) == 0) 
			{//MIB_EZ_QOS_DISABLED
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
				goto setErr_qos;
			}
		}
	#endif

	strAddQos = websGetVar(wp, T("addQos"), T(""));
	strEditQos = websGetVar(wp, T("editQos"), T(""));
	strConfigWan = websGetVar(wp, T("configWan"), T(""));
	strConfigNum = websGetVar(wp, T("configNum"), T(""));
	strModeState = websGetVar(wp, T("nettype"), T(""));

//	num_id = qos_id = add_id = edit_id = config_id = del_id = delall_id = send_id = 0;
	selNum = atoi(strConfigWan);
	memset(&entry, '\0', sizeof(entry));


	//strApplyQos = websGetVar(wp, T("ApplyQos"), T(""));

	strIsEnabled = websGetVar(wp, T("isEnabled"), T(""));

	if (strIsEnabled[0]) {
		strVal = websGetVar(wp, T("enabled1"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set( MIB_WAN1_QOS_ENABLED, (void *)&intVal) == 0) {//MIB_QOS_ENABLED
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
#if 0 //#ifdef _Customer_
		strVal = websGetVar(wp, T("enabled2"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_WAN2_QOS_ENABLED, (void *)&intVal) == 0) {//MIB_QOS_ENABLED
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
#endif

	#ifdef _Customer_ //Robart Add, Enable Qos must disable hw_nat
		if(intVal==1)
		{
			intVal = 0;
			apmib_set( MIB_FAST_NAT_ENABLE, (void *)&intVal);
		}
	#endif

	}

#ifdef _Unicorn_
	if (strModeState[0]){
		intVal = atoi(strModeState);
		if (!apmib_set( MIB_QOS_MODE, (void *)&intVal)){
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrMode))</script>>"));
			goto setErr_qos;
		}
	}
#endif
	/* Add new QoS table */
	if (strAddQos[0] || strEditQos[0]) {
		for (andNum=0x001; andNum<=0x008; (andNum=andNum*2)) {
			num_id = 0;
			if ( (selNum & andNum) == 0x001 ) {	//WAN 1
				num_id = MIB_WAN1_QOS_NUM;
				qos_id = MIB_WAN1_QOS;
				add_id = MIB_WAN1_QOS_ADD;
				edit_id = MIB_WAN1_QOS_EDIT;
				del_id = MIB_WAN1_QOS_DEL;
				send_id = MIB_POLICY_SEND;
			}
			if ( num_id == 0 )	continue;

			strName = websGetVar(wp, T("ruleName"), T(""));
			strBWVal = websGetVar(wp, T("bwidthVal"), T(""));
			strBWType = websGetVar(wp, T("bwidthType"), T(""));
			strDUType = websGetVar(wp, T("downUpType"), T(""));
			strDownVal = websGetVar(wp, T("downVal"), T(""));
			strDownType = websGetVar(wp, T("downType"), T(""));
			strWanPort = websGetVar(wp, T("portValue"), T(""));

			strScType = websGetVar(wp, T("sourType"), T(""));
			//strScStIp = websGetVar(wp, T("sourStIp"), T(""));
			//strScEdIp = websGetVar(wp, T("sourEdIp"), T(""));
			strScMac = websGetVar(wp, T("sourMac"), T(""));

			//strDtStIp = websGetVar(wp, T("destStIp"), T(""));
			//strDtEdIp = websGetVar(wp, T("destEdIp"), T(""));

			strTfType = websGetVar(wp, T("trafType"), T(""));
			strProtl = websGetVar(wp, T("protocol"), T(""));
			strScPort = websGetVar(wp, T("sourPort"), T(""));
			strDtPort = websGetVar(wp, T("destPort"), T(""));
			strDiffSev = websGetVar(wp, T("diffSevType"), T(""));


			if ( strDUType[0] == '0' ) { // Tommy
				strScStIp = websGetVar(wp, T("sourStIp"), T(""));
				strScEdIp = websGetVar(wp, T("sourEdIp"), T(""));
				strDtStIp = websGetVar(wp, T("destStIp"), T(""));
				strDtEdIp = websGetVar(wp, T("destEdIp"), T(""));
				strScPort = websGetVar(wp, T("sourPort"), T(""));
				strDtPort = websGetVar(wp, T("destPort"), T(""));
			}
			else {	//Download
				strDtStIp = websGetVar(wp, T("sourStIp"), T(""));
				strDtEdIp = websGetVar(wp, T("sourEdIp"), T(""));
				strScStIp = websGetVar(wp, T("destStIp"), T(""));
				strScEdIp = websGetVar(wp, T("destEdIp"), T(""));
				strDtPort = websGetVar(wp, T("sourPort"), T(""));
				strScPort = websGetVar(wp, T("destPort"), T(""));
			}


/*			if (!strName[0] && !strBWVal[0] && !strBWType[0] && !strWanPort[0] &&
				!strScType[0] && !strScStIp[0] && !strScEdIp[0] && !strScMac[0] &&
				!strDtStIp[0] && !strDtEdIp[0] &&
				!strTfType[0] && !strProtl[0] && !strScPort[0] && !strDtPort[0] && !strDiffSev[0])
				goto setOk_qos;
*/
			if ( strName[0] ) {	//set rule name
				if (strlen(strName) > COMMENT_LEN-1) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrLong))</script>"));
					goto setErr_qos;
				}
				strcpy(entry.ruleName, strName);
			}

			if ( strDUType[0] == '0' )
			{
				if ( strBWType[0] == '0' )	//set bandwidth Type of QoS
					entry.bwidthType = 0;
				else
					entry.bwidthType = 1;



				v1=strtoul(strBWVal,NULL,0);
/*			if ( !apmib_get( send_id, (void *)&v2)) {		//MIB_POLICY_SEND
				strcpy(tmpBuf, T("Get entry number error!"));
				goto setErr_qos;
			}
			if( v1 > v2 ) {
				strcpy(tmpBuf, T("Invalid value of bandwidth!\
					   	It should be smaller than value of WAN policy's speed send."));
				goto setErr_qos;
			}*/
				entry.bwidthVal = (unsigned long)v1;
				entry.downType = 0;
				entry.downVal = (unsigned long) 0;
			}
			else
			{
				if ( strBWType[0] == '0' )	//set download bandwidth Type of QoS
					entry.downType = 0;
				else
					entry.downType = 1;

				v1=strtoul(strBWVal,NULL,0);
/*			if ( !apmib_get( send_id, (void *)&v2)) {		//MIB_POLICY_SEND
				strcpy(tmpBuf, T("Get entry number error!"));
				goto setErr_qos;
			}
			if( v1 > v2 ) {
				strcpy(tmpBuf, T("Invalid value of bandwidth!\
					   	It should be smaller than value of WAN policy's speed send."));
				goto setErr_qos;
			}*/
				entry.downVal = (unsigned long)v1;
				entry.bwidthType = 0;
				entry.bwidthVal = (unsigned long) 0;
			}


//*********************************** source address ***********************************
			if ( strScType[0] == '0' ) {
				entry.sourType = 0;	//set source address Type of QoS
				getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
				getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);


//				if (!strScStIp[0]) 	//set start ip of source address of QoS
//					strScStIp = "0.0.0.0";
				//inet_aton(strScStIp, (struct in_addr *)&entry.sourStIp);
				inet_aton(strScStIp, &entIpAddr);
				fixed_inet_aton(strScStIp,entry.sourStIp);
				//v1 = *((unsigned long *)entry.sourStIp);
  		  v1 = ((unsigned long)entIpAddr.s_addr);
  		 	v2 = *((unsigned long *)&curIpAddr);
				v3 = *((unsigned long *)&curSubnet);
				if ( !strScStIp[0] && ((v1 & v3) != (v2 & v3)) ) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSInvIpStart))</script>"));
					goto setErr_qos;
				}

//				if (!strScEdIp[0]) 	//set end ip of source address of QoS
				if (!strcmp(strScEdIp,"0.0.0.0")) 	//set end ip of source address of QoS
					strScEdIp = strScStIp;
				//inet_aton(strScEdIp, (struct in_addr *)&entry.sourEdIp);
				fixed_inet_aton(strScEdIp,entry.sourEdIp);
				v4 = *((unsigned long *)entry.sourEdIp);
				if ( !strScEdIp[0] && ((v4 & v3) != (v2 & v3)) ) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSInvIpEnd))</script>"));
					goto setErr_qos;
				}
				if ( v4 < v1 ) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSInvIpRange))</script>"));
					goto setErr_qos;
				}
			}
			else {
				entry.sourType = 1;
				if ( !strScMac[0] ) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrMacNo))</script>"));
					goto setErr_qos;
				}

				strPortTmp = strdup(strScMac);
				for (i=0;i<8; i++)	{
					if ((ptr1 = strchr(strScMac, ',')) != NULL) {
						*ptr1 = '\0';
						ptr1 ++;
					}

					if (strlen(strScMac)!=12 || !string_to_hex(strScMac, tmpBuf, 12)) {
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrMacInv))</script>"));
						goto setErr_qos;
					}
					memcpy(entry.sourMac[i], tmpBuf, 6);
//			printf("\nGGG3=%02x:%02x:%02x:%02x:%02x:%02x\n",entry.sourMac[i][0],entry.sourMac[i][1],entry.sourMac[i][2],entry.sourMac[i][3],entry.sourMac[i][4],entry.sourMac[i][5]);
					if( ptr1 == NULL)
						break;
					strScMac = ptr1;
				}
				free(strPortTmp);
			}
//*********************************** destination address ***********************************
//			if (!strDtStIp[0])		//set start ip of destination address of QoS
//				strDtStIp = "0.0.0.0";
			//inet_aton(strDtStIp, (struct in_addr *)&entry.destStIp);
			fixed_inet_aton(strDtStIp,entry.destStIp);
			v1 = *((unsigned long *)entry.destStIp);

//			if (!strDtEdIp[0])		//set end ip of destination address of QoS
			if (!strcmp(strDtEdIp,"0.0.0.0"))		//set end ip of destination address of QoS
				strDtEdIp = strDtStIp;
			//inet_aton(strDtEdIp, (struct in_addr *)&entry.destEdIp);
			fixed_inet_aton(strDtEdIp,entry.destEdIp);
			v4 = *((unsigned long *)entry.destEdIp);

			if ( v4 < v1 ) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspformQoSInvIpDest))</script>"));
				goto setErr_qos;
			}
//*********************************** source port ***********************************
			if ( strTfType[0] == '0' )	//set traffic type of QoS
				entry.trafType = 0;
			else if ( strTfType[0] == '1' )
				entry.trafType = 1;
			else if ( strTfType[0] == '2' )
				entry.trafType = 2;
			else if ( strTfType[0] == '3' )
				entry.trafType = 3;
			else if ( strTfType[0] == '4' )
				entry.trafType = 4;

			if ( strProtl[0] == '0' )	//set protocol of QoS
				entry.protl = 0;
			else if ( strProtl[0] == '1' )
				entry.protl = 1;
			else
				entry.protl = 2;  // tcp+udp

			strcpy(scPortRange,strScPort);
			for(i=0; i<strlen(scPortRange); i++) //EDX, Ken 2016_05_18 add, Fix Qos UI Restart.
			{
				sprintf(scPortRangeTbl, "%s%c", scPortRangeTbl, scPortRange[i]);
			}

			for (i=0;i<5; i++)	{

				if ((ptr1 = strchr(scPortRangeTbl, ',')) != NULL) {

					*ptr1 = '\0';

					ptr1 ++;
				}

				if ((ptr2 = strchr(scPortRangeTbl, '-')) != NULL) {
					*ptr2 = '\0';
					ptr2 ++;

					if ((!(entry.sourPort[i][0] = atoi(scPortRangeTbl))) ||
					   	(entry.sourPort[i][0] < 1) || (entry.sourPort[i][0] >65536)) {
						strcpy(tmpBuf, T("<script>document.write(showText(aspformQoSErrPortSrcFr))</script>"));
						goto setErr_qos;
					}
					if ((!(entry.sourPort[i][1]=atoi(ptr2))) ||
					   	(entry.sourPort[i][1] < 1) || (entry.sourPort[i][1] >65536))  {
						strcpy(tmpBuf, T("<script>document.write(showText(aspformQoSErrPortSrcTo))</script>"));
						goto setErr_qos;
					}
					if (entry.sourPort[i][1] < entry.sourPort[i][0]) {
						strcpy(tmpBuf, T("<script>document.write(showText(aspformQoSErrPortSrcOrd))</script>"));
						goto setErr_qos;
					}
				}
				else
					entry.sourPort[i][0]=entry.sourPort[i][1]=atol(scPortRangeTbl);
					//fix by kyle port start can't be 0 or 1
					if(entry.sourPort[i][0]==1 || entry.sourPort[i][0]==0 ){
						entry.sourPort[i][0]=2;
					}
					if(entry.sourPort[i][1]==1 || entry.sourPort[i][1]==0){
							entry.sourPort[i][1]=2;
					}

					if( ptr1 == NULL)
					break;
					strcpy(scPortRangeTbl,ptr1);
			}

//*********************************** destination port ***********************************
			strcpy(dtPortRange,strDtPort);
			for(i=0; i<strlen(dtPortRange); i++) //EDX, Ken 2016_05_18 add, Fix Qos UI Restart.
			{
				sprintf(dtPortRangeTbl, "%s%c", dtPortRangeTbl, dtPortRange[i]);
			}

			for (i=0;i<5; i++)	{
				if ((ptr1 = strchr(dtPortRangeTbl, ',')) != NULL) {
					*ptr1 = '\0';
					ptr1 ++;
				}

				if ((ptr2 = strchr(dtPortRangeTbl, '-')) != NULL) {
					*ptr2 = '\0';
					ptr2 ++;

					if ((!(entry.destPort[i][0] = atoi(dtPortRangeTbl))) ||
						   	(entry.destPort[i][0] < 2) || (entry.destPort[i][0] >65535)) {
						if(entry.destPort[i][0] < 2){
							entry.destPort[i][0]=2;
						}else{
							strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrPortDstFr))</script>"));
							goto setErr_qos;
						}
					}
					if ((!(entry.destPort[i][1]=atoi(ptr2))) ||
						   	(entry.destPort[i][1] < 2) || (entry.destPort[i][1] >65535))  {
						if(entry.destPort[i][1] < 2){
							entry.destPort[i][1]=2;
						}else{
							strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrPortDstTo))</script>"));
							goto setErr_qos;
						}
					}

					if (entry.destPort[i][1] < entry.destPort[i][0]) {
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrPortDstOrd))</script>"));
						goto setErr_qos;
					}
				}
				else
					entry.destPort[i][0]=entry.destPort[i][1]=atol(dtPortRangeTbl);
					//fix by kyle port start can't be 0 or 1
					if(entry.destPort[i][0]==1 || entry.destPort[i][0]==0)
						entry.destPort[i][0]=2;
					if(entry.destPort[i][1] == 1 || entry.destPort[i][1] == 0)
						entry.destPort[i][1]=2;

					if( ptr1 == NULL)
					break;
					strcpy(dtPortRangeTbl,ptr1);
			}
//************************************************************************
			if ( !apmib_get( num_id, (void *)&entryNum)) {		//MIB_QOS_NUM
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetNum))</script>"));
				goto setErr_qos;
			}

if ( !strEditQos[0] ) {
			if ( (entryNum + 1) > MAX_QOS_NUM) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSInvAddEntry))</script>"));
				goto setErr_qos;
			}
}
			if ( strAddQos[0] ) {

			//	printf("\n\n********add*********\n: \n bwidthVal=%d \n bwidthType=%s \n downVal=%d \n downType=%s \n sourType=%s \n sourStIp=%s \n sourEdIp=%s \n destStIp=%s \n destEdIp=%s \n trafType=%d \n diffSevType=%d \n protl=%s \n ruleName=%s",entry.bwidthVal,entry.bwidthType,entry.downVal,entry.downType,entry.sourType,fixed_inet_ntoa(entry.sourStIp),fixed_inet_ntoa(entry.sourEdIp),fixed_inet_ntoa(entry.destStIp),fixed_inet_ntoa(entry.destEdIp),entry.trafType,entry.diffSevType,entry.protl,entry.ruleName);

				// set to MIB. try to delete it first to avoid duplicate case
				apmib_set( del_id, (void *)&entry);			//MIB_QOS_DEL
				if ( apmib_set(add_id, (void *)&entry) == 0) {	//MIB_QOS_ADD
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrAddEntry))</script>"));
					goto setErr_qos;
				}
			}
			if ( strEditQos[0] ) {
				intVal = atoi(strConfigNum);
				*(( char *)&orgEntry) = (char)intVal;

				if ( !apmib_get( qos_id, (void *)&orgEntry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetTbl))</script>"));
					goto setErr_qos;
				}
				Edit.selEntry = (char *)&orgEntry;
				Edit.editEntry = (char *)&entry;
				//printf("\n******orgEntry*****: \n bwidthVal=%d \n bwidthType=%s \n downVal=%d \n downType=%s \n sourType=%s \n sourStIp=%s \n sourEdIp=%s \n destStIp=%s \n destEdIp=%s \n trafType=%d \n diffSevType=%d \n protl=%s \n ruleName=%s",orgEntry.bwidthVal,orgEntry.bwidthType,orgEntry.downVal,orgEntry.downType,orgEntry.sourType,fixed_inet_ntoa(orgEntry.sourStIp),fixed_inet_ntoa(orgEntry.sourEdIp),fixed_inet_ntoa(orgEntry.destStIp),fixed_inet_ntoa(orgEntry.destEdIp),orgEntry.trafType,orgEntry.diffSevType,orgEntry.protl,orgEntry.ruleName);
				//printf("\n\n********after edit*********\n: \n bwidthVal=%d \n bwidthType=%s \n downVal=%d \n downType=%s \n sourType=%s \n sourStIp=%s \n sourEdIp=%s \n destStIp=%s \n destEdIp=%s \n trafType=%d \n diffSevType=%d \n protl=%s \n ruleName=%s",entry.bwidthVal,entry.bwidthType,entry.downVal,entry.downType,entry.sourType,fixed_inet_ntoa(entry.sourStIp),fixed_inet_ntoa(entry.sourEdIp),fixed_inet_ntoa(entry.destStIp),fixed_inet_ntoa(entry.destEdIp),entry.trafType,entry.diffSevType,entry.protl,entry.ruleName);


				// set to MIB. try to delete it first to avoid duplicate case
//				apmib_set( del_id, (void *)&entry);	//MIB_QOS_DEL
				if ( !apmib_set( edit_id, (void *)&Edit)) {	//MIB_QOS_EDIT
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrEditTbl))</script>"));
					goto setErr_qos;
				}
			}
		}
	}

	strDelQos = websGetVar(wp, T("delSelQos"), T(""));
	strDelAllQos = websGetVar(wp, T("delAllQos"), T(""));
	strMvUp = websGetVar(wp, T("moveUpQos"), T(""));
	strMvDown = websGetVar(wp, T("moveDownQos"), T(""));
	strShowQos = websGetVar(wp, T("showQos"), T(""));

	/* config entry */
	if ( strMvUp[0] || strMvDown[0] || strDelQos[0] || strShowQos[0]) {
		if (strConfigWan[0] == '1') {
			num_id = MIB_WAN1_QOS_NUM;
			qos_id = MIB_WAN1_QOS;
		}
#if 0 //#ifdef _Customer_
		else if (strConfigWan[0] == '2') {
			num_id = MIB_WAN2_QOS_NUM;
			qos_id = MIB_WAN2_QOS;
		}
#endif
/*		else if (strConfigWan[0] == '3') {
			num_id = MIB_WAN3_QOS_NUM;
			qos_id = MIB_WAN3_QOS;
		}
		else {
			num_id = MIB_WAN4_QOS_NUM;
			qos_id = MIB_WAN4_QOS;
		}
*/
		if ( strMvUp[0] ) {			//move up entry
			if (strConfigWan[0] == '1')
				config_id = MIB_WAN1_QOS_UP;
#if 0 //#ifdef _Customer_
			else if (strConfigWan[0] == '2')
				config_id = MIB_WAN2_QOS_UP;
#endif
/*			else if (strConfigWan[0] == '3')
				config_id = MIB_WAN3_QOS_UP;
			else
				config_id = MIB_WAN4_QOS_UP;
*/		}
		else if ( strMvDown[0] ) {	//move down entry
			if (strConfigWan[0] == '1')
				config_id = MIB_WAN1_QOS_DOWN;
#if 0 //#ifdef _Customer_
			else if (strConfigWan[0] == '2')
				config_id = MIB_WAN2_QOS_DOWN;
#endif
/*			else if (strConfigWan[0] == '3')
				config_id = MIB_WAN3_QOS_DOWN;
			else
				config_id = MIB_WAN4_QOS_DOWN;
*/		}
		else if ( strDelQos[0] ) {	//del entry
			if (strConfigWan[0] == '1')
				config_id = MIB_WAN1_QOS_DEL;
#if 0 //#ifdef _Customer_
			else if (strConfigWan[0] == '2')
				config_id = MIB_WAN2_QOS_DEL;
#endif
/*			else if (strConfigWan[0] == '3')
				config_id = MIB_WAN3_QOS_DEL;
			else
				config_id = MIB_WAN4_QOS_DEL;
*/		}

		if ( !apmib_get( num_id, (void *)&entryNum)) {	//MIB_OQS_NUM
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetNum))</script>"));
			goto setErr_qos;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				if ( strShowQos[0] ) {
					selectNum = i;	//set select entry number
					selectWan = atoi(strConfigWan);
					*((char *)&entry) = (char)i;
					if ( !apmib_get( qos_id, (void *)&entry)) {	//MIB_QOS
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetTbl))</script>"));
						goto setErr_qos;
					}
					if ( entry.sourType == 2) {	// void other deleted
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSOther))</script>"));
						goto setErr_qos;
					}
				}
				else {
					*((char *)&entry) = (char)i;
					if ( !apmib_get( qos_id, (void *)&entry)) {	//MIB_QOS
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetTbl))</script>"));
						goto setErr_qos;
					}
					if ( strDelQos[0] ) {
						if ( entry.sourType == 2) {	// void other deleted
							strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSOtherD))</script>"));
							goto setErr_qos;
						}
					}
					if ( !apmib_set( config_id, (void *)&entry)) {	//Move Up, Move Down, Del
						strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrCfgTbl))</script>"));
						goto setErr_qos;
					}
				}
			}
		}
	}
	/* Delete all entry */
	if ( strDelAllQos[0]) {
		if (strConfigWan[0] == '1')
			delall_id = MIB_WAN1_QOS_DELALL;
#if 0 //#ifdef _Customer_
		else if (strConfigWan[0] == '2')
			delall_id = MIB_WAN2_QOS_DELALL;
#endif
/*		else if (strConfigWan[0] == '3')
			delall_id = MIB_WAN3_QOS_DELALL;
		else
			delall_id = MIB_WAN4_QOS_DELALL;
*/
		if ( !apmib_set(delall_id, (void *)&entry)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrDelAll))</script>"));
			goto setErr_qos;
		}
		//initQos();  // Tommy
	}

// For Guarantee QoS

	strMaxDown = websGetVar(wp, T("maxdown"), T(""));

	if (strMaxDown[0]) {
		/*
		if ( !string_to_dec(strMaxDown, &intVal) || intVal<0 || intVal>100000) {
			DEBUGP(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspInvalidMaxDownBw))</script>"));
			goto setErr_qos;
		}
			*/
		string_to_dec(strMaxDown, &intVal);
		if ( apmib_set(MIB_MAX_DOWNLOAD_BANDWIDTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set Max Download Bandwidth failed!"));
			goto setErr_qos;
		}
	}


	strMaxUp = websGetVar(wp, T("maxup"), T(""));
	if (strMaxUp[0]) {
		/*
		if ( !string_to_dec(strMaxUp, &intVal) || intVal<0 || intVal>100000) {
			DEBUGP(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspInvalidMaxUpBw))</script>"));
			goto setErr_qos;
		}
		*/
		string_to_dec(strMaxUp, &intVal);
		if ( apmib_set(MIB_MAX_UPLOAD_BANDWIDTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set Max Upload Bandwidth failed!"));
			goto setErr_qos;
		}
	}

// End For Guarantee QoS

setOk_qos:

	Changed = 1;
	newpage = 0;

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup);
	}
	#endif

	apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("submitUrl"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

	char tmpBufapply[200];
	if(submitUrl[0]){
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
	}
	else 
	{
		/* For jquery get data*/
		char buffer[20];
		sprintf( buffer, "%s" ,"submit_ok");
		websWrite(wp, T("%s"), T("HTTP/1.0 200 OK\n"));
		websWrite(wp, T("%s"), T("Pragma: no-cache\n"));
		websWrite(wp, T("%s"), T("Cache-control: no-cache\n"));
		websWrite(wp, T("%s"), T("Content-Type: text/html\n"));
		websWrite(wp, T("%s"), T("\n"));
		websWrite(wp, T("%s"), buffer);
		websDone(wp, 200);
		//strcpy(tmpBuf, T("<script>document.write(showText(qosrulesuccess));</script>"));
		//closelater_MSG(wp, tmpBuf);
	}

	return;

setErr_qos:
	ERR_MSG(wp, tmpBuf);
  	return;
}
/////////////////////////////////////////////////////////////////////////////
#ifdef _EZ_QOS_
void formEZQoS(webs_t wp, char_t *path, char_t *query)
{

	char_t *strAddQos, *strDelQos, *strDelAllQos, *strApplyQos, *strEnable, *strApply, *strIsEnabled, *strBandwidth_Type;
	char_t *strMvUp, *strMvDown, *strEditQos, *strShowQos, *strConfigWan;
	char_t *strName, *strBWVal, *strBWType, *strDUType, *strDownVal, *strDownType;
	char_t *strWanPort, *submitUrl, *strVal;
	char_t *strScType, *strScStIp, *strScEdIp, *strScMac;
	char_t *strDtStIp, *strDtEdIp;
	char_t *strTfType, *strProtl, *strScPort, *strDtPort, *strPortTmp, *strDiffSev, *strConfigNum;
	char_t *strMaxDown, *strMaxUp, *QoStype, *strOrder;
	char_t *strModeState;
//	char_t *srtEZQosConnections;

	char tmpBuf[100];
	char *ptr1, *ptr2;
	int entryNum, i, selNum, andNum, intVal;
	int num_id, qos_id, add_id, edit_id, config_id, del_id, delall_id, send_id, enable_id;
	int ezqos_GAME_int, ezqos_APP_int, ezqos_MEDIA_int, ezqos_FTP_int, ezqos_P2P_int;
	struct in_addr curIpAddr,entIpAddr ,curSubnet;
	unsigned long v1, v2, v3, v4;
	EZQOS_T entry, orgEntry, selEZQOS, newEZQOS;
	EDIT_T Edit;

	num_id = MIB_EZ_QOS_NUM;
	qos_id = MIB_EZ_QOS;
	add_id = MIB_EZ_QOS_ADD;
	edit_id = MIB_EZ_QOS_EDIT;
	del_id = MIB_EZ_QOS_DEL;
	send_id = MIB_POLICY_SEND;
	delall_id = MIB_EZ_QOS_DELALL;
	
	char_t *strEZQosMode;
	
	strEZQosMode = websGetVar(wp, T("EZQos_Mode"), T(""));

	if (strEZQosMode[0]){
		intVal = 0;
		if (!apmib_set(MIB_EZ_QOS_MODE, (void *)&intVal)){
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
		
		intVal = 0;

		if ( apmib_set( MIB_WAN1_QOS_ENABLED, (void *)&intVal) == 0) {//MIB_QOS_ENABLED
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
	}

	strIsEnabled = websGetVar(wp, T("isEnabled"), T(""));

	if (strIsEnabled[0]) {
		strVal = websGetVar(wp, T("enabled1"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set( MIB_EZ_QOS_ENABLED, (void *)&intVal) == 0) {//MIB_EZ_QOS_ENABLED
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}

		#ifdef _Customer_ //Robart Add, Enable Qos must disable hw_nat
			if(intVal==1)
			{
				intVal = 0;
				apmib_set( MIB_FAST_NAT_ENABLE, (void *)&intVal);
			}
		#endif
	}
/*
	strBandwidth_Type = websGetVar(wp, T("BandWidth_Type"), T(""));
	if (strBandwidth_Type[0]) {
		if (!gstrcmp(strBandwidth_Type, T("yes")))
			intVal = 1;
		else if (!gstrcmp(strBandwidth_Type, T("no")))
			intVal = 0;
		else {
			strcpy(tmpBuf, T("Error!"));
			goto setErr_qos;
		}
		if ( apmib_set( MIB_EZ_QOS_TYPE, (void *)&intVal) == 0) {//MIB_EZ_QOS_TYPE
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
	}
*/
	if ( !apmib_get( num_id, (void *)&entryNum)) {		//MIB_QOS_NUM
		strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetNum))</script>"));
		goto setErr_qos;
	}

	if ( (entryNum + 1) > MAX_EZQOS_NUM) {
		strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSInvAddEntry))</script>"));
		goto setErr_qos;
	}

	//if ( strAddQos[0] ) {
	strMvUp = websGetVar(wp, T("moveUpQos"), T(""));
	strMvDown = websGetVar(wp, T("moveDownQos"), T(""));

	if ( strMvUp[0] ) {			//move up entry
		config_id = MIB_EZ_QOS_UP;
	}
	else if ( strMvDown[0] ) {	//move down entry
		config_id = MIB_EZ_QOS_DOWN;
	}

	if ( !apmib_get( num_id, (void *)&entryNum)) {	//MIB_OQS_NUM
		strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetNum))</script>"));
		goto setErr_qos;
	}
	for (i=entryNum; i>0; i--) {
		snprintf(tmpBuf, 20, "select%d", i);

		strVal = websGetVar(wp, tmpBuf, T(""));
		if ( !gstrcmp(strVal, T("ON")) ) {
				*((char *)&entry) = (char)i;

				if ( !apmib_get( qos_id, (void *)&entry)) {	//MIB_QOS
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrGetTbl))</script>"));
					goto setErr_qos;
				}
				if ( !apmib_set( config_id, (void *)&entry)) {	//Move Up, Move Down, Del
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrCfgTbl))</script>"));
					goto setErr_qos;
				}
		}
	}


// For Guarantee QoS

	strMaxDown = websGetVar(wp, T("maxdown"), T(""));

	if (strMaxDown[0]) {
		string_to_dec(strMaxDown, &intVal);
		if ( apmib_set(MIB_MAX_DOWNLOAD_BANDWIDTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set Max Download Bandwidth failed!"));
			goto setErr_qos;
		}
	}

	strMaxUp = websGetVar(wp, T("maxup"), T(""));
	if (strMaxUp[0]) {
		string_to_dec(strMaxUp, &intVal);
		if ( apmib_set(MIB_MAX_UPLOAD_BANDWIDTH, (void *)&intVal) == 0) {
			DEBUGP(tmpBuf, T("Set Max Upload Bandwidth failed!"));
			goto setErr_qos;
		}
	}

// End For Guarantee QoS

// Start for Reorder EZ QoS

	if ( !apmib_get(MIB_EZ_QOS_NUM, (void *)&entryNum))
	{
		printf("Get EZQOS entry number error!");
		return;
	}

	ezqos_GAME_int=atoi(websGetVar(wp, T("ezqos_GAME"),T("9")));
	ezqos_APP_int=atoi(websGetVar(wp, T("ezqos_APP"),T("9")));
	ezqos_MEDIA_int=atoi(websGetVar(wp, T("ezqos_MEDIA"),T("9")));
	ezqos_FTP_int=atoi(websGetVar(wp, T("ezqos_FTP"),T("9")));
	ezqos_P2P_int=atoi(websGetVar(wp, T("ezqos_P2P"),T("9")));

	if ((ezqos_GAME_int!=9)&&(ezqos_APP_int!=9)&&(ezqos_MEDIA_int!=9)&&(ezqos_FTP_int!=9)&&(ezqos_P2P_int!=9))
	{
//		if (apmib_get(MIB_EZ_QOS, (void *)&selEZQOS))
//	{
		char *new_order[5];
		new_order[ezqos_GAME_int]="GAME";
		new_order[ezqos_APP_int]="APP";
		new_order[ezqos_MEDIA_int]="MEDIA";
		new_order[ezqos_FTP_int]="FTP";
		new_order[ezqos_P2P_int]="P2P";

			apmib_set( delall_id, (void *)&selEZQOS);
			for(i=0;i<5;i++)
			{
				memset(&newEZQOS, 0, sizeof(EZQOS_T));
				strcpy( newEZQOS.ruleName, new_order[i]);
				apmib_set(add_id, (void *)&newEZQOS);
			}
//		}
	}
// End for Reorder EZ QoS

setOk_qos:

	Changed = 1;
	newpage = 0;

#ifndef NO_ACTION
	apmib_update(CURRENT_SETTING);
#endif

	submitUrl = websGetVar(wp, T("submitUrl"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));

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

setErr_qos:
	ERR_MSG(wp, tmpBuf);
  	return;
}
#endif //End formEZQoS
/////////////////////////////////////////////////////////////////////////////
#ifdef _EZ_QOS_
void formEZQoSMode(webs_t wp, char_t *path, char_t *query)
{
	char_t *strEZQosMode, *submitUrl;
	char tmpBuf[100];
	int intVal;
	strEZQosMode = websGetVar(wp, T("EZQos_Mode"), T(""));

	if (strEZQosMode[0]){
		if (!gstrcmp(strEZQosMode, T("EZQoS")))
			intVal = 0;
		else if (!gstrcmp(strEZQosMode, T("QoS")))
			intVal = 1;
		if (!apmib_set(MIB_EZ_QOS_MODE, (void *)&intVal)){
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformQoSErrFlag))</script>"));
			goto setErr_qos;
		}
	}

setOk_qos:

		Changed = 1;
		newpage = 0;

	#ifndef NO_ACTION
			apmib_update(CURRENT_SETTING);
	#endif

	submitUrl = websGetVar(wp, T("submitUrl"), T(""));

	if (submitUrl[0])
		websRedirect(wp, submitUrl);
	else
		websDone(wp, 200);

	return;

	setErr_qos:
		ERR_MSG(wp, tmpBuf);
		return;
}
#endif //End formEZQoS
/////////////////////////////////////////////////////////////////////////////
#if defined(_Edimax_) //EDX Robart 2016_02_17 Add
int QosShow(int eid, webs_t wp, int argc, char_t **argv)
{
	char *strScStIp, *strScEdIp, *strDtStIp, *strDtEdIp;
	char buf[20], macTmp[150], scPtTmp[60], dtPtTmp[60], temp[COMMENT_LEN * 2 + 3];
	int	nBytesSent=0, i, qos_id=1, j, qos_num;
	QOS_T entry;

	if ( !apmib_get(MIB_WAN1_QOS_NUM, (void *)&qos_num)) { return -1; }
	if (qos_num==0) { return -1; }
	else {
		nBytesSent += websWrite(wp, T("var Qos_All_Show = new Array(\n"));
		for(qos_id=1;qos_id<=qos_num;qos_id++)
		{
			memset(&entry,   '\0', sizeof(entry));
			memset(&macTmp,  '\0', sizeof(macTmp));
			memset(&scPtTmp, '\0', sizeof(scPtTmp));
			memset(&dtPtTmp, '\0', sizeof(dtPtTmp));

			*((char *)&entry) = (char)qos_id;
			if ( !apmib_get(MIB_WAN1_QOS, (void *)&entry)) { return -1; }

			strScStIp = strdup( fixed_inet_ntoa(entry.sourStIp));
			strScEdIp = strdup( fixed_inet_ntoa(entry.sourEdIp));
			strDtStIp = strdup( fixed_inet_ntoa(entry.destStIp));
			strDtEdIp = strdup( fixed_inet_ntoa(entry.destEdIp));

			if (!strcmp(strScStIp,"0.0.0.0"))	strScStIp = strScEdIp = "";
			if (!strcmp(strScStIp,strScEdIp))	strScEdIp = "";

			if (!strcmp(strDtStIp,"0.0.0.0"))	strDtStIp = strDtEdIp = "";
			if (!strcmp(strDtStIp,strDtEdIp))	strDtEdIp = "";

			for(i=0; i<=7; i++)
			{
				sprintf(buf, "%02x%02x%02x%02x%02x%02x",
					entry.sourMac[i][0], entry.sourMac[i][1], entry.sourMac[i][2],
					 entry.sourMac[i][3], entry.sourMac[i][4], entry.sourMac[i][5]);

				if (strcmp(buf, "000000000000"))
				{
					if (i != 0) strcat(macTmp, ",");
					strcat(macTmp, buf);
				}
			}
			for (i=0; i<=4; i++)
			{
				if (entry.sourPort[i][0] == entry.sourPort[i][1]) sprintf(buf, "%d", entry.sourPort[i][0]);
				else sprintf(buf, "%d-%d", entry.sourPort[i][0], entry.sourPort[i][1]);

				if (strcmp(buf, "0-0") && strcmp(buf, "0"))
				{
					if ( i != 0) strcat(scPtTmp, ",");
					strcat(scPtTmp, buf);
				}

				if (entry.destPort[i][0] == entry.destPort[i][1]) sprintf(buf, "%d", entry.destPort[i][0]);
				else sprintf(buf, "%d-%d", entry.destPort[i][0], entry.destPort[i][1]);

				if (strcmp(buf, "0-0") && strcmp(buf, "0"))
				{
					if ( i != 0) strcat(dtPtTmp, ",");
					strcat(dtPtTmp, buf);
				}
			}

			j=0;
			for(i=0;i<COMMENT_LEN;i++)
			{
				if( entry.ruleName[i] == '\'' || entry.ruleName[i] == '\\')
				{
					temp[j]='\\';
					temp[j+1]=entry.ruleName[i];
					j=j+2;
				}
				else
				{
					temp[j]=entry.ruleName[i];
					j=j+1;
				}
			}
			if (qos_id !=1 ) {nBytesSent += websWrite(wp, T(",\n"));}
			nBytesSent += websWrite(wp, T("'%s','%lu','%d','%d',"), temp, entry.bwidthVal, entry.bwidthType, entry.sourType);
			nBytesSent += websWrite(wp, T("'%s','%s','%s','%s','%s',"), strScStIp, strScEdIp, macTmp, strDtStIp, strDtEdIp);
			nBytesSent += websWrite(wp, T("'%d','%d','%s','%s',"), entry.trafType, entry.protl, scPtTmp, dtPtTmp);
			nBytesSent += websWrite(wp, T("'%d','%d','%lu','%d'"),1, qos_id, entry.downVal, entry.downType);
		}
		nBytesSent += websWrite(wp, T("\n);"));
	}
	return nBytesSent;
}
#else
int QosShow(int eid, webs_t wp, int argc, char_t **argv)
{
	char *strScStIp, *strScEdIp, *strDtStIp, *strDtEdIp;
	char buf[20], macTmp[150], scPtTmp[60], dtPtTmp[60], temp[COMMENT_LEN * 2 + 3];
	int	nBytesSent=0, i, qos_id=0, j;
	QOS_T entry;
	memset(&entry,   '\0', sizeof(entry));
	memset(&macTmp,  '\0', sizeof(macTmp));
	memset(&scPtTmp, '\0', sizeof(scPtTmp));
	memset(&dtPtTmp, '\0', sizeof(dtPtTmp));

	if (selectWan == 1) qos_id = MIB_WAN1_QOS;

	*((char *)&entry) = (char)selectNum;

	if ( !apmib_get( qos_id, (void *)&entry))
	{
		nBytesSent += websWrite(wp, T("var qosShow = new Array('','','','','','','','','','','','','','','','','');"));
	}
	else
	{
		strScStIp = strdup( fixed_inet_ntoa(entry.sourStIp));
		strScEdIp = strdup( fixed_inet_ntoa(entry.sourEdIp));
		strDtStIp = strdup( fixed_inet_ntoa(entry.destStIp));
		strDtEdIp = strdup( fixed_inet_ntoa(entry.destEdIp));

		if (!strcmp(strScStIp,"0.0.0.0"))	strScStIp = strScEdIp = "";
		if (!strcmp(strScStIp,strScEdIp))	strScEdIp = "";

		if (!strcmp(strDtStIp,"0.0.0.0"))	strDtStIp = strDtEdIp = "";
		if (!strcmp(strDtStIp,strDtEdIp))	strDtEdIp = "";

		for(i=0; i<=7; i++)
		{
			sprintf(buf, "%02x%02x%02x%02x%02x%02x",
				entry.sourMac[i][0], entry.sourMac[i][1], entry.sourMac[i][2],
			   entry.sourMac[i][3], entry.sourMac[i][4], entry.sourMac[i][5]);

			if (strcmp(buf, "000000000000"))
			{
				if (i != 0) strcat(macTmp, ",");
				strcat(macTmp, buf);
			}
		}
		for (i=0; i<=4; i++)
		{
			if (entry.sourPort[i][0] == entry.sourPort[i][1]) sprintf(buf, "%d", entry.sourPort[i][0]);
			else                                              sprintf(buf, "%d-%d", entry.sourPort[i][0], entry.sourPort[i][1]);

			if (strcmp(buf, "0-0") && strcmp(buf, "0"))
			{
				if ( i != 0) strcat(scPtTmp, ",");
				strcat(scPtTmp, buf);
			}

			if (entry.destPort[i][0] == entry.destPort[i][1]) sprintf(buf, "%d", entry.destPort[i][0]);
			else                                              sprintf(buf, "%d-%d", entry.destPort[i][0], entry.destPort[i][1]);

			if (strcmp(buf, "0-0") && strcmp(buf, "0"))
			{
				if ( i != 0) strcat(dtPtTmp, ",");
				strcat(dtPtTmp, buf);
			}
		}

		j=0;
		for(i=0;i<COMMENT_LEN;i++)
		{
			if( entry.ruleName[i] == '\'' || entry.ruleName[i] == '\\')
			{
				temp[j]='\\';
				temp[j+1]=entry.ruleName[i];
				j=j+2;
			}
			else
			{
				temp[j]=entry.ruleName[i];
				j=j+1;
			}
		}

		nBytesSent += websWrite(wp, T("var qosShow = new Array('%s','%lu','%d','%d','%s','%s','%s','%s','%s','%d','%d','%s','%s','%d','%d','%lu','%d');"),
			/*entry.ruleName*/temp, entry.bwidthVal, entry.bwidthType,
			entry.sourType, strScStIp, strScEdIp, macTmp, strDtStIp, strDtEdIp,
		   entry.trafType, entry.protl, scPtTmp, dtPtTmp, selectWan, selectNum, entry.downVal, entry.downType);
	}
	selectNum = -1;
	selectWan = -1;
	return nBytesSent;
}
#endif
/////////////////////////////////////////////////////////////////////////////
int Wan1QosList(int eid, webs_t wp, int argc, char_t **argv)
{

	int	nBytesSent=0, entryNum=0, i=0;
	QOS_T entry;

	if ( !apmib_get(MIB_WAN1_QOS_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}


	for (i=1; i<=entryNum ; i++) {
		*((char *)&entry) = (char)i;

		if ( !apmib_get(MIB_WAN1_QOS, (void *)&entry))
			return -1;

		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\" class=\"dnsTableText\">\n"));
		nBytesSent += websWrite(wp, T("  <td width='10%%'>%d</td>\n"),i);

		if ( entry.sourType == 2 ){
			nBytesSent += websWrite(wp, T("  <td width='30%%'><font color=\"red\">%s</font></td>\n"),entry.ruleName);
			nBytesSent += websWrite(wp, T("  <td width='25%%'><font color=\"red\"><script>dw(RemainedBandwidth)</script></font></td>\n"));
			nBytesSent += websWrite(wp, T("  <td width='25%%'><font color=\"red\"><script>dw(RemainedBandwidth)</script></font></td>\n"));
		}
		else{
			nBytesSent += websWrite(wp, T("  <td width='30%%'>%s</td>\n"),entry.ruleName);
			nBytesSent += websWrite(wp, T("  <td width='25%%'>%d</td>\n"),entry.bwidthVal);
			nBytesSent += websWrite(wp, T("  <td width='25%%'>%d</td>\n"),entry.downVal);
		}
		nBytesSent += websWrite(wp, T("  <td width='10%%'><input type=\"checkbox\" id=\"select%d\" name=\"select%d\" onClick=\"move_check();\" value=\"ON\"></td>"),i,i);
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n"));
		nBytesSent += websWrite(wp, T("  <td >No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	return nBytesSent;
}

#if 0 //#ifdef _Customer_
int Wan2QosList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	QOS_T entry;

	if ( !apmib_get(MIB_WAN2_QOS_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_WAN2_QOS, (void *)&entry))
			return -1;

		if ( entry.sourType == 2 )
			nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center class='table2'><font size='2'>%d</td>\n"
			"<td align=center class='table2'><font size='2' color='red'>%s</td>\n"
			"<td align=center class='table2'><font size='2' color='red'>Remained Bandwidth</td>\n"
			"<td align=center class='table2'><font size='2' color='red'>Remained Bandwidth</td>\n"
			"<td align=center class='table2'><input type='checkbox' name='select%d' value='ON'></td></tr>\n"),
				i, entry.ruleName, i);
		else
			nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center class='table2'><font size='2'>%d</td>\n"
			"<td align=center class='table2'><font size='2'>%s</td>\n"
			"<td align=center class='table2'><font size='2'>%d</td>\n"
			"<td align=center class='table2'><font size='2'>%d</td>\n"
			"<td align=center class='table2'><input type='checkbox' name='select%d' value='ON'></td></tr>\n"),
				i, entry.ruleName, entry.bwidthVal, entry.downVal, i);
	}
	return nBytesSent;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef _EZ_QOS_
int EZQosList(int eid, webs_t wp, int argc, char_t **argv)
{

	int	nBytesSent=0, entryNum=0, i=0;
	EZQOS_T entry;

	if ( !apmib_get(MIB_EZ_QOS_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

printf("entryNum=%d \n", entryNum);
	
	for (i=1; i<=entryNum ; i++) {
		*((char *)&entry) = (char)i;

		if ( !apmib_get(MIB_EZ_QOS, (void *)&entry))
			return -1;

printf("i=%d entry.ruleName=%s entry.ruleName=%s\n", i, entry.ruleName, entry.ruleName);

		nBytesSent += websWrite(wp, T("<td id=\"EZQOS_TD_%d\" class=\"without_pic_ezqos\" onClick=\"mouseclick_item(this)\"><img title=\"%s\" style=\"cursor:pointer;\" src=\"/file/%s.gif\" height=\"100%%\" width=\"100%%\"></td>"), i, entry.ruleName, entry.ruleName);
	}

	return nBytesSent;
}
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef _EZ_QOS_
void formBandwidth_Calc(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl;
	FILE *fp;

	system("/bin/bandwidth_calc.sh");

	if (fopen("/tmp/fail_detect","r")==NULL)
		submitUrl="./bandwidth_show.asp";
	else
		submitUrl="./bandwidth_fail.asp";

	websRedirect(wp, submitUrl);
	return;
}
#endif
/////////////////////////////////////////////////////////////////////////////
void formApplyReboot(webs_t wp, char_t *path, char_t *query)
{

	system("/bin/reboot.sh");
	return;
}

/////////////////////////////////////////////////////////////////////////////
void formVirtualSv(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strAddPort, *strDelPort, *strVal, *strDelAllPort;

	char_t *strIp, *strFrom, *strTo, *strComment, *strApply;

	char tmpBuf[100];
	int entryNum, intVal, i;
	PORTFW_T entry;
	struct in_addr curIpAddr, entIpAddr,curSubnet;
	unsigned long v1, v2, v3;

	strAddPort = websGetVar(wp, T("addPortFw"), T(""));
	strDelPort = websGetVar(wp, T("deleteSelPortFw"), T(""));
	strDelAllPort = websGetVar(wp, T("deleteAllPortFw"), T(""));

	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

	memset(&entry, '\0', sizeof(entry));
	if (!strApply[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_VSER_ENABLED, (void *)&intVal) == 0) {//MIB_PORTFW_ENABLED
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrFlag))</script>")); 
			goto setErr_portfw;
		}
	}

	/* Add new port-forwarding table */
	if (strAddPort[0]) {
		strIp = websGetVar(wp, T("ip"), T(""));
		strFrom = websGetVar(wp, T("fromPort"), T(""));
		strTo = websGetVar(wp, T("toPort"), T(""));

		strComment = websGetVar(wp, T("comment"), T(""));

		if (!strIp[0] && !strFrom[0] && !strTo[0] && !strComment[0])
			goto setOk_portfw;

		if (!strIp[0]) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrIpNo))</script>"));  //ip?��?为空�?			goto setErr_portfw;
		}
		/*
		fixed_inet_aton(strIp, entry.ipAddr);
		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);
		v1 = *((unsigned long *)entry.ipAddr);
		v2 = *((unsigned long *)&curIpAddr);
		v3 = *((unsigned long *)&curSubnet);
		*/
		//alex
   inet_aton(strIp, &entIpAddr);

   fixed_inet_aton(strIp,entry.ipAddr);

   getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);

   getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);
   v1 = ((unsigned long)entIpAddr.s_addr);
 		v2 = ((unsigned long)curIpAddr.s_addr);
		v3 = ((unsigned long)curSubnet.s_addr);
		//alex

#ifdef __TARGET_BOARD__
		if ( (v1 & v3) != (v2 & v3) ) {
			strcpy(tmpBuf, T("<script type='text/javascript'>if(window.aspformVirtualSvInvIp) document.write(showText(aspformVirtualSvInvIp)); else  document.write(\"Invalid IP address! It should be set within the current subnet.\");</script>"));  //IP不在当�?子�???			
		goto setErr_portfw;
		}
#endif
		if ( !strFrom[0] ) { // if port-forwarding, from port must exist
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrNoPrivate))</script>"));  //?��??�端??�?			goto setErr_portfw;
		}
		if ( !string_to_dec(strFrom, &intVal) || intVal<1 || intVal>65535) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvInvValuePrivate))</script>"));  //私�?端口?��???			goto setErr_portfw;
		}
		entry.fromPort = (unsigned short)intVal;

		if ( !strTo[0] )
			entry.toPort = entry.fromPort;
		else {
			if ( !string_to_dec(strTo, &intVal) || intVal<1 || intVal>65535) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvInvValuePublic))</script>"));  //?�共端口?��???				goto setErr_portfw;
			}
		}
		entry.toPort = (unsigned short)intVal;

/*		if ( entry.fromPort  > entry.toPort ) {
			strcpy(tmpBuf, T("Error! Invalid port range."));
			goto setErr_portfw;
		}
*/
		strVal = websGetVar(wp, T("protocol"), T(""));
		if (strVal[0]) {

			

			if ( strVal[0] == '0' )
				entry.protoType = PROTO_BOTH;
			else if ( strVal[0] == '1' )
				entry.protoType = PROTO_TCP;
			else if ( strVal[0] == '2' )
				entry.protoType = PROTO_UDP;

			
			else {

					strcpy(tmpBuf, T("Error! Invalid protocol type."));
				
				goto setErr_portfw;
			}
		}
		else {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrPtcTypeEmpty))</script>"));  
		}

#ifdef _zio_
		switch(strVal[0])
		{
			case '3':
				entry.fromPort = (unsigned short)80;
				entry.toPort   = (unsigned short)80;
				break;
			case '4':
				entry.fromPort = (unsigned short)443;
				entry.toPort   = (unsigned short)443;
				break;
			case '5':
				entry.fromPort = (unsigned short)21;
				entry.toPort   = (unsigned short)21;
				break;
			case '6':
				entry.fromPort = (unsigned short)110;
				entry.toPort   = (unsigned short)110;
				break;
			case '7':
				entry.fromPort = (unsigned short)25;
				entry.toPort   = (unsigned short)25;
				break;
			case '8':
				entry.fromPort = (unsigned short)53;
				entry.toPort   = (unsigned short)53;
				break;
			case '9':
				entry.fromPort = (unsigned short)23;
				entry.toPort   = (unsigned short)23;
				break;
			case 'A':
				entry.fromPort = (unsigned short)500;
				entry.toPort   = (unsigned short)500;
				break;
			case 'B':
				entry.fromPort = (unsigned short)1723;
				entry.toPort   = (unsigned short)1723;
				break;
			case 'C':
				entry.fromPort = (unsigned short)9292;
				entry.toPort   = (unsigned short)9292;
				break;
			case 'D':
				entry.fromPort = (unsigned short)1720;
				entry.toPort   = (unsigned short)1720;
				break;
			default:
				break;
		}
#endif


		if ( strComment[0] ) {
			if (strlen(strComment) > COMMENT_LEN-1) {
				strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrCmtLong))</script>"));  //注�?太长
				goto setErr_portfw;
			}
			strcpy(entry.comment, CharFilter3(strComment));
		}
		if ( !apmib_get(MIB_VSER_NUM, (void *)&entryNum)) {//MIB_PORTFW_NUM
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetNum))</script>"));
			goto setErr_portfw;
		}

		if ( (entryNum + 1) > MAX_FILTER_NUM) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrFullAdd))</script>"));  //?��?进入?��??�误
			goto setErr_portfw;
		}

		// Check if there is any port overlapped
		for (i=1; i<=entryNum; i++) {
			PORTFW_T checkEntry;
			*((char *)&checkEntry) = (char)i;
			if ( !apmib_get(MIB_VSER, (void *)&checkEntry)) {//MIB_PORTFW
				strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetTbl))</script>"));  //?��?进入?��??�误
				goto setErr_portfw;
			}

			if (entry.toPort == checkEntry.toPort ) {

				strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrSetRange))</script>"));  //端口超�???				goto setErr_portfw;
			}
/*			if ( ( (entry.fromPort <= checkEntry.fromPort &&
					entry.toPort >= checkEntry.fromPort) ||
			       (entry.fromPort >= checkEntry.fromPort &&
				entry.fromPort <= checkEntry.toPort)
			     )&&
			       (entry.protoType & checkEntry.protoType) ) {
				strcpy(tmpBuf, T("Setting port range has overlapped with used port numbers!"));
				goto setErr_portfw;
			}
*/		}

		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_VSER_DEL, (void *)&entry);//MIB_PORTFW_DEL
		if ( apmib_set(MIB_VSER_ADD, (void *)&entry) == 0) {//MIB_PORTFW_ADD
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrAdd))</script>"));  //?�表?��??�号?��?�?			goto setErr_portfw;
		}
	}

	/* Delete entry */
	if (strDelPort[0]) {
		if ( !apmib_get(MIB_VSER_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetNum))</script>"));  //?��?进入?��??�误
			goto setErr_portfw;
		}

		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {
				*((char *)&entry) = (char)i;
				if ( !apmib_get(MIB_VSER, (void *)&entry)) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrGetTbl))</script>"));  //?��?表格进入?��??�误
					goto setErr_portfw;
				}
				if ( !apmib_set(MIB_VSER_DEL, (void *)&entry)) {
					strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrDelEntry))</script>"));  //?�除表格?��?�?					goto setErr_portfw;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAllPort[0]) {
		if ( !apmib_set(MIB_VSER_DELALL, (void *)&entry)) {
			strcpy(tmpBuf, T("<script>document.write(showText(aspformVirtualSvErrDelAll))</script>"));  //?�除?�?�表?�出??			goto setErr_portfw;
		}
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup);
	}
	#endif

setOk_portfw:
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

setErr_portfw:
	ERR_MSG(wp, tmpBuf);
  	return;
}
/////////////////////////////////////////////////////////////////////////////
int virtualSvList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	PORTFW_T entry;
	char	*type, *ip, portFrom[20], portTo[20];

	#ifdef __TARGET_BOARD__ //EDX, Robert 2014_12_02 add.
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

	if ( !apmib_get(MIB_VSER_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}
	for (i=1; i<=entryNum; i++)
	{
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_VSER, (void *)&entry)) return -1;

		ip = fixed_inet_ntoa(entry.ipAddr);

		if ( !strcmp(ip, "0.0.0.0")) ip = "----";

		if ( entry.protoType == PROTO_BOTH )     type = "TCP+UDP";
		else if ( entry.protoType == PROTO_TCP ) type = "TCP";
		else                                     type = "UDP";

		snprintf(portFrom, 20, "%d", entry.fromPort);
		snprintf(portTo, 20, "%d", entry.toPort);

		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n")); 
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\" class=\"dnsTableText\">\n"));

		nBytesSent += websWrite(wp, T("  <td width='5%%'>%d</td>\n"),i);
		#ifdef _NETBIOSNAME_
		nBytesSent += websWrite(wp, T("  <td width='20%%'><script>document.write(searchComName(\"%s\",0))</script></td>\n"),ip);
		#endif
	
		nBytesSent += websWrite(wp, T("  <td width='18%%'>%s</td>\n"),ip);
		nBytesSent += websWrite(wp, T("  <td width='10%%'>%s</td>\n"),portFrom);
		nBytesSent += websWrite(wp, T("  <td width='10%%'>%s</td>\n"),type);
		nBytesSent += websWrite(wp, T("  <td width='10%%'>%s</td>\n"),portTo);
		nBytesSent += websWrite(wp, T("  <td width='17%%'>&nbsp;%s</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("  <td width='10%%'><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);

	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"vsipnum\" value=\"%d\">\n"),i-1);
	return nBytesSent;
}
/////////////////////////////////////////////////////////////////////////////
void formPortFw(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl, *strAddPort, *strDelPort, *strVal, *strDelAllPort;

	char_t *strIp, *strFrom, *strTo, *strComment, *strApply;

	char tmpBuf[200];
	int entryNum, intVal, i;
	PORTFW_T entry;
	struct in_addr curIpAddr, curSubnet, entIpAddr;
	unsigned long v1, v2, v3;

	strAddPort = websGetVar(wp, T("addPortFw"), T(""));
	strDelPort = websGetVar(wp, T("deleteSelPortFw"), T(""));
	strDelAllPort = websGetVar(wp, T("deleteAllPortFw"), T(""));

	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif

	memset(&entry, '\0', sizeof(entry));
	if (!strApply[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set( MIB_PORTFW_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>if(window.aspformPortFwErrFlag) document.write(showText(aspformPortFwErrFlag)); else document.write(\"Set enabled flag error!\");</script>"));
			goto setErr_portfw;
		}
	}

	/* Add new port-forwarding table */
	if (strAddPort[0])
	{
		strIp   = websGetVar(wp, T("ip"),       T(""));
		strFrom = websGetVar(wp, T("fromPort"), T(""));
		strTo   = websGetVar(wp, T("toPort"),   T(""));


		strComment = websGetVar(wp, T("comment"), T(""));

		if (!strIp[0] && !strFrom[0] && !strTo[0] && !strComment[0])
			goto setOk_portfw;

		if (!strIp[0])
		{
			strcpy(tmpBuf, T("<script type='text/javascript'>if(window.aspformPortFwErrNoIp) document.write(showText(aspformPortFwErrNoIp)); else document.write(showText(aspformFilterErrInput));</script>"));
			goto setErr_portfw;
		}

		//alex
		inet_aton(strIp, &entIpAddr);
		fixed_inet_aton(strIp,entry.ipAddr);
		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);
		v1 = ((unsigned long)entIpAddr.s_addr);
 		v2 = ((unsigned long)curIpAddr.s_addr);
		v3 = ((unsigned long)curSubnet.s_addr);

		//alex
		#if !defined(_zio_) && defined(__TARGET_BOARD__)
		if ( (v1 & v3) != (v2 & v3) )
		{
			strcpy(tmpBuf, T("<script type='text/javascript'>if(window.aspformPortFwInvIp) document.write(showText(aspformPortFwInvIp)); else  document.write(\"Invalid IP address! It should be set within the current subnet.\");</script>"));
			goto setErr_portfw;
		}
		#endif

		if ( !strFrom[0] ) { // if port-forwarding, from port must exist
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrPortForm))</script>"));
			goto setErr_portfw;
		}
		if ( !string_to_dec(strFrom, &intVal) || intVal<1 || intVal>65535) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwInvPortForm))</script>"));
			goto setErr_portfw;
		}
		entry.fromPort = (unsigned short)intVal;

		if ( !strTo[0] )
			entry.toPort = entry.fromPort;
		else {
			if ( !string_to_dec(strTo, &intVal) || intVal<1 || intVal>65535) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrPortTo))</script>"));
				goto setErr_portfw;
			}
		}
		entry.toPort = (unsigned short)intVal;

		if ( entry.fromPort  > entry.toPort ) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrPortRange))</script>"));
			goto setErr_portfw;
		}

		strVal = websGetVar(wp, T("protocol"), T(""));
		if (strVal[0]) {
			if ( strVal[0] == '0' )
				entry.protoType = PROTO_BOTH;
			else if ( strVal[0] == '1' )
				entry.protoType = PROTO_TCP;
			else if ( strVal[0] == '2' )
				entry.protoType = PROTO_UDP;
			else {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwInvPtcType))</script>"));
				goto setErr_portfw;
			}
		}
		else {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrPtcType))</script>"));
			goto setErr_portfw;
		}


		if ( strComment[0] ) {
			if (strlen(strComment) > COMMENT_LEN-1) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrCmtLong))</script>"));
				goto setErr_portfw;
			}
			strcpy(entry.comment, CharFilter3(strComment));
		}
		if ( !apmib_get(MIB_PORTFW_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrGetNum))</script>"));
			goto setErr_portfw;
		}

		if ( (entryNum + 1) > MAX_FILTER_NUM) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(CannotCreateMoreEntries))</script>"));
			goto setErr_portfw;
		}

		// Check if there is any port overlapped
		for (i=1; i<=entryNum; i++)
		{
			PORTFW_T checkEntry;
			*((char *)&checkEntry) = (char)i;

			if ( !apmib_get(MIB_PORTFW, (void *)&checkEntry))
			{
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrGetTbl))</script>"));
				goto setErr_portfw;
			}

			if ( (
					(entry.fromPort <= checkEntry.fromPort && entry.toPort >= checkEntry.fromPort ) ||
					(entry.fromPort >= checkEntry.fromPort && entry.fromPort <= checkEntry.toPort )
			     )
			     &&
			     (entry.protoType & checkEntry.protoType)
				)
			{
				strcpy(tmpBuf, T("Setting port range has overlapped with used port numbers!"));
				goto setErr_portfw;
			}
		}

		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_PORTFW_DEL, (void *)&entry);
		if ( apmib_set(MIB_PORTFW_ADD, (void *)&entry) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrAddTbl))</script>"));
			goto setErr_portfw;
		}
	}

	/* Delete entry */
	if (strDelPort[0]) {
		if ( !apmib_get(MIB_PORTFW_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrGetNum))</script>"));
			goto setErr_portfw;
		}

		for (i=entryNum; i>0; i--) {
printf("YH~YA\n");		
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));

			if ( !gstrcmp(strVal, T("ON")) ) {
printf("OHOH\n");			
				*((char *)&entry) = (char)i;
				if ( !apmib_get(MIB_PORTFW, (void *)&entry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrGetTbl))</script>"));
					goto setErr_portfw;
				}
				if ( !apmib_set(MIB_PORTFW_DEL, (void *)&entry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrDelTbl))</script>"));
					goto setErr_portfw;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAllPort[0]) {
		if ( !apmib_set(MIB_PORTFW_DELALL, (void *)&entry)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformPortFwErrDelAll))</script>"));
			goto setErr_portfw;
		}
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup);
	}
	#endif

setOk_portfw:
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

setErr_portfw:
	ERR_MSG(wp, tmpBuf);
  	return;
}


/* ------------------ URL BLOCKING --------------------*/
void formUrlb(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddURLB, *strDelURLB, *strDelAllURLB, *strURLB, *strIsEnabled;	// for URL blocking
	//URLB_T URLBEntry;	// for URL blocking
	char_t *strVal, *submitUrl, *strApply;
	char tmpBuf[100];
	int intVal, entryNum, i;
	//void *pEntry;
	strAddURLB = websGetVar(wp, T("addURLB"), T(""));
	strDelURLB = websGetVar(wp, T("deleteSelURLB"), T(""));
	strDelAllURLB = websGetVar(wp, T("deleteAllURLB"), T(""));
	strIsEnabled = websGetVar(wp, T("isEnabled"), T(""));

	if (strAddURLB[0] || strDelURLB[0] || strDelAllURLB[0]) {
		memset(&URLBEntry, '\0', sizeof(URLBEntry));
		pEntry = (void *)&URLBEntry;
	}

	if (strIsEnabled[0]) {
		//printf("haha..............\n");
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		if ( apmib_set(MIB_URLB_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrFlag))</script>"));
			goto setErr_urlb;
		}
	}

//*********************************** Add URL Blocking ***********************************
	if (strAddURLB[0]) {
		strURLB = websGetVar(wp, T("URLB"), T(""));

		if (!strURLB[0]) {
			goto setOk_urlb;

		}

		if ( strURLB[0] ) {
			if (strlen(strURLB) > 32) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrCmtLong))</script>"));
				goto setErr_urlb;
			}
			else {  // Lance 2003.11.06
				int i;
				for( i=0; i < strlen(strURLB); i++ )
					strURLB[i] = tolower(strURLB[i]);

				strncpy(URLBEntry.URLBAddr, strURLB, sizeof(URLBEntry.URLBAddr));
			}
		}
	//}

	//if (strAddURLB[0]) {
		if ( !apmib_get(MIB_URLB_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrGetNum))</script>"));
			goto setErr_urlb;
		}
		/* if ( (entryNum + 1) > MAX_FILTER_NUM) {
			strcpy(tmpBuf, T("Cannot add new entry because table is full!"));
			goto setErr_urlb;
		} */
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_URLB_DEL, pEntry);
		if ( apmib_set(MIB_URLB_ADD, pEntry) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrAddTbl))</script>"));
			goto setErr_urlb;
		}
	}


	/* Delete entry */
	if (strDelURLB[0]) {
		if ( !apmib_get(MIB_URLB_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrGetNum))</script>"));
			goto setErr_urlb;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)pEntry) = (char)i;
				if ( !apmib_get(MIB_URLB, pEntry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrGetTbl))</script>"));
					goto setErr_urlb;
				}
				if ( !apmib_set(MIB_URLB_DEL, pEntry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrDelTbl))</script>"));
					goto setErr_urlb;
				}
			}
		}
	}

	/* Delete all entry */
	if (strDelAllURLB[0]) {
		if ( !apmib_set(MIB_URLB_DELALL, pEntry)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformUrlbErrDelAll))</script>"));
			goto setErr_urlb;
		}
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup);
	}
	#endif

setOk_urlb:
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

setErr_urlb:
	ERR_MSG(wp, tmpBuf);
	return;
}

/////////////////////////////////////////////////////////////////////////////
void formFilter(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddIp, *strAddPort, *strAddMac, *strDelPort, *strDelIp, *strDelMac;
	//char_t *strAddURLB, *strDelURLB, *strDelAllURLB, *strURLB;	// for URL blocking
	char_t *strAddDMZ, *strDelDMZ, *strDelAllDMZ, *strsIp;		// for dmz
	char_t *strAddACPC, *strDelACPC, *strDelAllACPC;		// for ip filter
	char_t *strAddSRout, *strDelSRout, *strDelAllSRout;		// for static routing
#ifdef _SDMZ_
	char_t *strDmzMode;
	char_t *strDmzMac;
	char_t *strRefreshTime;
#endif
	char_t *strDelAllPort, *strDelAllIp, *strDelAllMac, *strVal, *submitUrl, *strComment, *strApply;
	char_t *strFrom, *strTo;
	char_t *strMacDenyEnabled, *strIpDenyEnabled;
	char tmpBuf[100];
	int entryNum, intVal, i;
	IPFILTER_T ipEntry;
	DMZ_T dmzEntry;		// for dmz
	//URLB_T URLBEntry;	// for URL blocking
	ACPC_T ACPCEntry;	// for ip filter
	SROUT_T sroutEntry;	// for static routing
	PORTFILTER_T portEntry;
	MACFILTER_T macEntry;
	struct in_addr curIpAddr, curSubnet, entIpAddr;
	void *pEntry;
	unsigned long v1, v2, v3, v4;
	int num_id, get_id, add_id, del_id, delall_id, enable_id;
	char_t *strfrm;



#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif
	strAddDMZ = websGetVar(wp, T("addDMZ"), T(""));
	strDelDMZ = websGetVar(wp, T("delSelDMZ"), T(""));
	strDelAllDMZ = websGetVar(wp, T("delAllDMZ"), T(""));

	strAddIp = websGetVar(wp, T("addFilterIp"), T(""));
	strDelIp = websGetVar(wp, T("deleteSelFilterIp"), T(""));
	strDelAllIp = websGetVar(wp, T("deleteAllFilterIp"), T(""));

	strAddPort = websGetVar(wp, T("addFilterPort"), T(""));
	strDelPort = websGetVar(wp, T("deleteSelFilterPort"), T(""));
	strDelAllPort = websGetVar(wp, T("deleteAllFilterPort"), T(""));

	strAddMac = websGetVar(wp, T("addFilterMac"), T(""));
	strDelMac = websGetVar(wp, T("deleteSelFilterMac"), T(""));
	strDelAllMac = websGetVar(wp, T("deleteAllFilterMac"), T(""));

	strAddACPC = websGetVar(wp, T("addACPC"), T(""));
	strDelACPC = websGetVar(wp, T("deleteSelACPC"), T(""));
	strDelAllACPC = websGetVar(wp, T("deleteAllACPC"), T(""));

	strAddSRout = websGetVar(wp, T("addSRout"), T(""));
	strDelSRout = websGetVar(wp, T("delSelSRout"), T(""));
	strDelAllSRout = websGetVar(wp, T("delAllSRout"), T(""));


	if (strAddIp[0] || strDelIp[0] || strDelAllIp[0]) {
		num_id = MIB_IPFILTER_NUM;
		get_id = MIB_IPFILTER;
		add_id = MIB_IPFILTER_ADD;
		del_id = MIB_IPFILTER_DEL;
		delall_id = MIB_IPFILTER_DELALL;
		enable_id = MIB_IPFILTER_ENABLED;
		memset(&ipEntry, '\0', sizeof(ipEntry));
		pEntry = (void *)&ipEntry;
	}
	else if (strAddDMZ[0] || strDelDMZ[0] || strDelAllDMZ[0]) {
		num_id = MIB_DMZ_NUM;
		get_id = MIB_DMZ;
		add_id = MIB_DMZ_ADD;
		del_id = MIB_DMZ_DEL;
		delall_id = MIB_DMZ_DELALL;
		enable_id = MIB_DMZ_ENABLED;
		memset(&dmzEntry, '\0', sizeof(dmzEntry));
		pEntry = (void *)&dmzEntry;
	}
	else if (strAddPort[0] || strDelPort[0] || strDelAllPort[0]) {
		num_id = MIB_PORTFILTER_NUM;
		get_id = MIB_PORTFILTER;
		add_id = MIB_PORTFILTER_ADD;
		del_id = MIB_PORTFILTER_DEL;
		delall_id = MIB_PORTFILTER_DELALL;
		enable_id = MIB_PORTFILTER_ENABLED;
		memset(&portEntry, '\0', sizeof(portEntry));
		pEntry = (void *)&portEntry;
	}
	else if (strAddACPC[0] || strDelACPC[0] || strDelAllACPC[0]) {
		num_id = MIB_ACPC_NUM;
		get_id = MIB_ACPC;
		add_id = MIB_ACPC_ADD;
		del_id = MIB_ACPC_DEL;
		delall_id = MIB_ACPC_DELALL;
		enable_id = MIB_ACPC_ENABLED;
		memset(&ACPCEntry, '\0', sizeof(ACPCEntry));
		pEntry = (void *)&ACPCEntry;
	}
	else if (strAddSRout[0] || strDelSRout[0] || strDelAllSRout[0]) {
		num_id = MIB_SROUT_NUM;
		get_id = MIB_SROUT;
		add_id = MIB_SROUT_ADD;
		del_id = MIB_SROUT_DEL;
		delall_id = MIB_SROUT_DELALL;
		enable_id = MIB_SROUT_ENABLED;
		memset(&sroutEntry, '\0', sizeof(sroutEntry));
		pEntry = (void *)&sroutEntry;
	}
	else{
		num_id = MIB_MACFILTER_NUM;
		get_id = MIB_MACFILTER;
		add_id = MIB_MACFILTER_ADD;
		del_id = MIB_MACFILTER_DEL;
		delall_id = MIB_MACFILTER_DELALL;
		enable_id = MIB_MACFILTER_ENABLED;
		memset(&macEntry, '\0', sizeof(macEntry));
		pEntry = (void *)&macEntry;
	}

	strMacDenyEnabled = websGetVar(wp, T("macDenyEnabled"), T(""));
	if (strMacDenyEnabled[0]) {
		if (!gstrcmp(strMacDenyEnabled, T("yes")))
			intVal = 1;
		else if (!gstrcmp(strMacDenyEnabled, T("no")))
			intVal = 0;
		else {
			strcpy(tmpBuf, T("Error!"));
			goto setErr_filter;
		}
		if ( apmib_set(MIB_MACDENY_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("Set MAC filter Deny Enable failed!"));
			goto setErr_filter;
		}
	}

	strIpDenyEnabled = websGetVar(wp, T("ipDenyEnabled"), T(""));
	if (strIpDenyEnabled[0]) {
		if (!gstrcmp(strIpDenyEnabled, T("yes")))
			intVal = 1;
		else if (!gstrcmp(strIpDenyEnabled, T("no")))
			intVal = 0;
		else {
			strcpy(tmpBuf, T("Error!"));
			goto setErr_filter;
		}
		if ( apmib_set(MIB_IPDENY_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("Set IP filter Deny Enable failed!"));
			goto setErr_filter;
		}
	}
#ifdef _SDMZ_
	strDmzMode = websGetVar(wp, T("DMZ_Mode"), T(""));
	strRefreshTime = websGetVar(wp, T("Refresh_Time"), T(""));

	if (strDmzMode[0]){
		if (!gstrcmp(strDmzMode, T("DMZ")))
			intVal = 0;
		else if (!gstrcmp(strDmzMode, T("SuperDMZ")))
			intVal = 1;
		if (!apmib_set(MIB_DMZ_MODE, (void *)&intVal)){
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrSetDmzMode))</script>"));
			goto setErr_filter;
		}
	}

	if (strRefreshTime[0]){
		intVal = atoi(strRefreshTime);
		if (!apmib_set(MIB_SUPERDMZ_REFRESHTIME, (void *)&intVal)){
			strcpy(tmpBuf, T("Error! Invalid refresh time."));
			goto setErr_filter;
		}
	}

	strDmzMac = websGetVar(wp, T("DMZmac"), T(""));
	if (strDmzMac[0]) {
		if (strlen(strDmzMac)!=12 || !string_to_hex(strDmzMac, tmpBuf, 12)) {

			strcpy(tmpBuf, T("Error! Invalid MAC address."));
			goto setErr_filter;
		}
		if ( !apmib_set(MIB_DMZ_MAC_ADDR, (void *)tmpBuf)) {
			strcpy(tmpBuf, T("Set MIB_WAN_MAC_ADDR mib error!"));
			goto setErr_filter;
		}
	}
#endif

	if ( !apmib_get(num_id, (void *)&entryNum)) {
		strcpy(tmpBuf, T("Get entry number error!"));
		goto setErr_filter;
	}
/*	if ( (entryNum + 1) > MAX_FILTER_NUM) {
		strcpy(tmpBuf, T("Cannot add new entry because table is full!"));
		goto setErr_filter;
	}
*/
	// Set enable flag
	if (strAddACPC[0] || strAddDMZ[0] || strAddIp[0] || strAddPort[0] || strAddMac[0] || strAddSRout[0]) {
		char_t *strHWNatEnable;
		int hw_val;
		#if defined(_Customer_)
		char_t *strNatEnable;
		int val;

		strNatEnable = websGetVar(wp, T("natEnable"), T(""));
		if (strNatEnable[0]) {
			if (!gstrcmp(strNatEnable, T("no"))) val = 0;
			else val = 1;
				
			if ( apmib_set(MIB_NAT_ENABLE, (void *)&val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
				goto setErr_filter;
			}
		}
		#endif
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;

		strHWNatEnable = websGetVar(wp, T("HWnatEnable"), T(""));
		if (strHWNatEnable[0]){
			if (!gstrcmp(strHWNatEnable, T("no"))) hw_val = 0;
			else hw_val = 1;
				
			if ( apmib_set(MIB_FAST_NAT_ENABLE, (void *)&hw_val) == 0) {
				DEBUGP(tmpBuf, T("<script>document.write(showText(aspFormNatEnable))</script>"));
				goto setErr_filter;
			}
		}

		#ifdef _SROUT_NATOFF_
			if(strAddSRout[0])
			{
				intVal=1-intVal;
				apmib_set( MIB_NAT_ENABLE, (void *)&intVal);
				intVal=1-intVal;
			}
		#endif

		if ( apmib_set(enable_id, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("Set enabled flag error!"));
			goto setErr_filter;
		}
	}

	strComment = websGetVar(wp, T("comment"), T(""));

//*************************************** Add DMZ ****************************************
	if ( strAddDMZ[0] ) {
		char_t *strType, *strSession, *strPbIp, *strClIp;

		strType = websGetVar(wp, T("dmzType"), T(""));
		strSession = websGetVar(wp, T("session"), T(""));
		strPbIp = websGetVar(wp, T("pip"), T(""));
		strClIp = websGetVar(wp, T("cip"), T(""));

		if (!strType[0] && !strSession[0] && !strPbIp[0] && !strClIp[0] )
			goto setOk_filter;

		if (!strClIp[0]) {
			strcpy(tmpBuf, T("Error! No ip address to set."));
			goto setErr_filter;
		}



		//kyle
		inet_aton(strClIp, &entIpAddr);

		fixed_inet_aton(strClIp, dmzEntry.cipAddr);

		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);
		v1 = ((unsigned long)entIpAddr.s_addr);
 		v2 = ((unsigned long)curIpAddr.s_addr);
		v3 = ((unsigned long)curSubnet.s_addr);
		//kyle
		/*
		fixed_inet_aton(strClIp, dmzEntry.cipAddr);
		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);

		v1 = *((unsigned long *)dmzEntry.cipAddr);
		v2 = *((unsigned long *)&curIpAddr);
		v3 = *((unsigned long *)&curSubnet);
	*/
#ifdef __TARGET_BOARD__
		if ( (v1 & v3) != (v2 & v3) ) {
			strcpy(tmpBuf, T("Invalid IP address! It should be set within the current subnet."));
			goto setErr_filter;
		}
#endif
		if (strType[0] == '0') 	// type of dmz is dynamic ip
			dmzEntry.type = 0;
		else
			dmzEntry.type = 1;

		if (strSession[0] == '2')
			dmzEntry.session = 2;
		else
			dmzEntry.session = 1;

		if (!strcmp( strPbIp, "" ))
			strPbIp = "0.0.0.0";
		fixed_inet_aton(strPbIp, dmzEntry.pipAddr);
		for (i=1; i<=entryNum; i++) {	// Check if there is any ip overlapped
			DMZ_T checkEntry;
			*((char *)&checkEntry) = (char)i;
			if ( !apmib_get(get_id, (void *)&checkEntry)) {
				strcpy(tmpBuf, T("Get table entry error!"));
				goto setErr_filter;
			}
			if ( (dmzEntry.type == 1 && !memcmp(&checkEntry.pipAddr, &dmzEntry.pipAddr, 4)) || (dmzEntry.type == 0 && checkEntry.type == dmzEntry.type) ) {
				strcpy(tmpBuf, T("Setting public ip has overlapped with used ip address!"));
				goto setErr_filter;
			}
		}
	}
//*********************************** Add Static Routing ***********************************
	if (strAddSRout[0]) {
		char_t *strIp, *strMask, *strGateway, *strCount, *strFace;
		int k;
		FILE *fp;
		char wanAddr[3][50];

		strIp = websGetVar(wp, T("sroutIp"), T(""));
		strMask = websGetVar(wp, T("sroutMask"), T(""));
		strGateway = websGetVar(wp, T("sroutGateway"), T(""));
		strCount = websGetVar(wp, T("sroutCount"), T(""));
		strFace = websGetVar(wp, T("sroutFace"), T(""));

		if (!strIp[0] && !strMask[0] && !strGateway[0] && !strCount[0] && !strFace[0] )
			goto setOk_filter;

		if (!strIp[0] || !strMask[0] || !strGateway[0]) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrInput))</script>"));
			goto setErr_filter;
		}

		//kyle
   		inet_aton(strIp, &entIpAddr);

		fixed_inet_aton(strIp, sroutEntry.ipAddr);
		fixed_inet_aton(strMask, sroutEntry.maskAddr);
		fixed_inet_aton(strGateway, sroutEntry.gatewayAddr);
		//printf("sroutEntry=%u%u%u%u",sroutEntry.ipAddr[0],sroutEntry.ipAddr[1],sroutEntry.ipAddr[2],sroutEntry.ipAddr[3]);

		v1 = ((unsigned long)entIpAddr.s_addr);
		//v1 = *((unsigned long *)&sroutEntry.ipAddr);	//check IP
		v2 = *((unsigned long *)&sroutEntry.maskAddr);
		v3 = v1 & v2 ;


		memcpy(&sroutEntry.ipAddr, &v3, 4);

		if ( strFace[0] == '1' ) {
			system("wan-status.sh");
			if ((fp = fopen("/var/run/wanstat","r"))!=NULL) {
				for ( k=0 ; k<3 ; k++)
					fgets(wanAddr[k], 50, fp);
				fclose(fp);
			}
			inet_aton(wanAddr[1], &curIpAddr);
			inet_aton(wanAddr[2], &curSubnet);

			v1 = *((unsigned long *)&sroutEntry.gatewayAddr);
			v2 = *((unsigned long *)&curIpAddr);
			v3 = *((unsigned long *)&curSubnet);
		} else {
			getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
			getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);

			v1 = *((unsigned long *)&sroutEntry.gatewayAddr);
			v2 = *((unsigned long *)&curIpAddr);
			v3 = *((unsigned long *)&curSubnet);
#ifdef __TARGET_BOARD__
			if ( (v1 & v3) != (v2 & v3) ) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvIpLan))</script>"));
				goto setErr_filter;
			}
#endif
		}

		if ( !string_to_dec(strCount, &intVal) || intVal<0 || intVal>255) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvHopCount))</script>"));
			goto setErr_filter;
		}
		sroutEntry.hopCount = (unsigned short)intVal;
		if (strFace[0]){
			if ( strFace[0] == '0' )
				sroutEntry.interface = 0;
			else
				sroutEntry.interface = 1;
		}
	}

//****************************** Add access control Ip filter ******************************
	if (strAddACPC[0]) {
		char *ptr, *ptr2, *ptr3;
		int	i;
		char_t *strIndex;
		unsigned long longVal;

		strVal = websGetVar(wp, T("sipaddr"), T(""));
		strsIp = websGetVar(wp, T("eipaddr"), T(""));
		if (!strVal[0] && !strComment[0])
			goto setOk_filter;

		if (!strVal[0]) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrNoIp))</script>"));
			goto setErr_filter;
		}
		if ( !strsIp[0])
			strsIp=strVal;

		fixed_inet_aton(strVal, ACPCEntry.sipAddr);
		fixed_inet_aton(strsIp, ACPCEntry.eipAddr);
		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);

		v1 = *((unsigned long *)ACPCEntry.sipAddr);
		v4 = *((unsigned long *)ACPCEntry.eipAddr);
		v2 = *((unsigned long *)&curIpAddr);
		v3 = *((unsigned long *)&curSubnet);
#ifdef __TARGET_BOARD__
		if ( ((v1 & v3) != (v2 & v3)) || ((v4 & v3) != (v2 & v3))) {
			strcpy(tmpBuf, T("Invalid IP address! It should be set within the current subnet."));
			goto setErr_filter;
		}
#endif
		strIndex = websGetVar(wp, T("serindex"), T(""));
		ptr = websGetVar(wp, T("Port"), T(""));

		longVal=strtoul(strIndex,NULL,0);
		ACPCEntry.serindex = longVal;

		if ( (longVal == 0) && !ptr[0] )
			ACPCEntry.serindex = 0xC000;
//------------------------------------------------------------------------------------
		for (i=0;i<5; i++) {
			if ((ptr2 = strchr(ptr, ',')) != NULL) {
				*ptr2 = '\0';
				ptr2 ++;
			}

			if ((ptr3 = strchr(ptr, '-')) != NULL) {
				*ptr3 = '\0';
				ptr3 ++;

				if ((!(ACPCEntry.Port[i][0] = atoi(ptr))) ||
						(ACPCEntry.Port[i][0] < 1) || (ACPCEntry.Port[i][0] >65536)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvIpPortFr))</script>"));
					goto setErr_filter;
				}
				if ((!(ACPCEntry.Port[i][1]=atoi(ptr3))) ||
						(ACPCEntry.Port[i][1] < 1) || (ACPCEntry.Port[i][1] >65536))  {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvIpPortTo))</script>"));
					goto setErr_filter;
				}
				if (ACPCEntry.Port[i][1] < ACPCEntry.Port[i][0]) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvPortRange))</script>"));
					goto setErr_filter;
				}
			}
			else
				ACPCEntry.Port[i][0]=ACPCEntry.Port[i][1]=atol(ptr);
/*				if (i>0)
					if (ACPCEntry.Port[i-1][1] > ACPCEntry.Port[i][1])  {
						strcpy(tmpBuf, T("Error!"));
						goto setErr_filter;
					}*/
			if( ptr2 == NULL)
				break;
			ptr = ptr2;
		}
	}

//*********************************** Add Ip filter ***********************************
	if (strAddIp[0]) {
		strVal = websGetVar(wp, T("ip"), T(""));
		if (!strVal[0] && !strComment[0])
			goto setOk_filter;

		if (!strVal[0]) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrNoIp))</script>"));
			goto setErr_filter;
		}
		fixed_inet_aton(strVal, ipEntry.ipAddr);
		getInAddr(BRIDGE_IF, IP_ADDR, (void *)&curIpAddr);
		getInAddr(BRIDGE_IF, SUBNET_MASK, (void *)&curSubnet);

		v1 = *((unsigned long *)ipEntry.ipAddr);
		v2 = *((unsigned long *)&curIpAddr);
		v3 = *((unsigned long *)&curSubnet);
#ifdef __TARGET_BOARD__
		if ( (v1 & v3) != (v2 & v3) ) {
			strcpy(tmpBuf, T("Invalid IP address! It should be set within the current subnet."));
			goto setErr_filter;
		}
#endif
	}

	/* Add port filter */
	if (strAddPort[0]) {
		strFrom = websGetVar(wp, T("fromPort"), T(""));
		strTo = websGetVar(wp, T("toPort"), T(""));
		if (!strFrom[0] && !strTo[0] && !strComment[0])
			goto setOk_filter;

		if (!strFrom[0]) { // if port-forwarding, from port must exist
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrPortFr))</script>"));
			goto setErr_filter;
		}
		if ( !string_to_dec(strFrom, &intVal) || intVal<1 || intVal>65535) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvPortFr))</script>"));
			goto setErr_filter;
		}
		portEntry.fromPort = (unsigned short)intVal;

		if ( !strTo[0] )
			portEntry.toPort = portEntry.fromPort;
		else {
			if ( !string_to_dec(strTo, &intVal) || intVal<1 || intVal>65535) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrPortTo))</script>"));
				goto setErr_filter;
			}
			portEntry.toPort = (unsigned short)intVal;
		}

		if ( portEntry.fromPort  > portEntry.toPort ) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrPortRange))</script>"));
			goto setErr_filter;
		}
	}

	if (strAddACPC[0] || strAddPort[0] || strAddIp[0]) {
		strVal = websGetVar(wp, T("protocol"), T(""));
		if (strVal[0]) {
			if ( strVal[0] == '0' ) {
				if (strAddPort[0])
					portEntry.protoType = PROTO_BOTH;
				else if (strAddIp[0])
					ipEntry.protoType = PROTO_BOTH;
				else
					ACPCEntry.protoType = PROTO_BOTH;
			}
			else if ( strVal[0] == '1' ) {
				if (strAddPort[0])
					portEntry.protoType = PROTO_TCP;
				else if (strAddIp[0])
					ipEntry.protoType = PROTO_TCP;
				else
					ACPCEntry.protoType = PROTO_TCP;
			}
			else if ( strVal[0] == '2' ) {
				if (strAddPort[0])
					portEntry.protoType = PROTO_UDP;
				else if (strAddIp[0])
					ipEntry.protoType = PROTO_UDP;
				else
					ACPCEntry.protoType = PROTO_UDP;
			}
			else {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvPtcType))</script>"));
				goto setErr_filter;
			}
		}
		else {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrPtcTypeEmpty))</script>"));
			goto setErr_filter;
		}
	}

	if (strAddMac[0]) {

		strVal = websGetVar(wp, T("mac"), T(""));
		if (!strVal[0] && !strComment[0])
			goto setOk_filter;

		if ( !strVal[0] ) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrNoMac))</script>"));
			goto setErr_filter;
		}
		if (strlen(strVal)!=12 || !string_to_hex(strVal, macEntry.macAddr, 12)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterInvNoMac))</script>"));
			goto setErr_filter;
		}
	}

	if (strAddACPC[0] || strAddDMZ[0] || strAddIp[0] || strAddPort[0] || strAddMac[0] || strAddSRout[0] ) {
	//printf("\nstrComment=%d  getComment=%d\n",strlen(strComment),(COMMENT_LEN-1));
		if ( strComment[0] ) {
			if (strlen(strComment) > COMMENT_LEN-1) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrCmtLong))</script>"));
				goto setErr_filter;
			}
			if (strAddIp[0])
				strcpy(ipEntry.comment, CharFilter3(strComment));
			else if (strAddPort[0])
				strcpy(portEntry.comment, CharFilter3(strComment));
			else if (strAddMac[0])
				strcpy(macEntry.comment, CharFilter3(strComment));
			else if (strAddACPC[0])
				strcpy(ACPCEntry.comment, CharFilter3(strComment));
		}
		if ( !apmib_get(num_id, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrGetNum))</script>"));
			goto setErr_filter;
		}
		if ( (entryNum + 1) > MAX_FILTER_NUM) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrAdd))</script>"));
			goto setErr_filter;
		}
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(del_id, pEntry);
		if ( apmib_set(add_id, pEntry) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrAddTbl))</script>"));
			goto setErr_filter;
		}
	}

	/* Delete entry */
	if (strDelACPC[0] || strDelDMZ[0] || strDelPort[0] || strDelIp[0] || strDelMac[0] || strDelSRout[0]) {
		if ( !apmib_get(num_id, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrGetNum))</script>"));
			goto setErr_filter;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)pEntry) = (char)i;
				if ( !apmib_get(get_id, pEntry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrGetTbl))</script>"));
					goto setErr_filter;
				}
				if ( !apmib_set(del_id, pEntry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrDelTbl))</script>"));
					goto setErr_filter;
				}
			}
		}
	}

	/* Delete all entry */
	if (strDelAllACPC[0] || strDelAllDMZ[0] || strDelAllPort[0] || strDelAllIp[0] || strDelAllMac[0] || strDelAllSRout[0]) {
		if ( !apmib_set(delall_id, pEntry)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrDelAll))</script>"));
			goto setErr_filter;
		}
	}

	#ifdef _IQv2_
	char_t *iqsetupclose;
	iqsetupclose = websGetVar(wp, T("iqsetupclose"), T(""));
	if (iqsetupclose[0]){
		int close_iQsetup=1;
		apmib_set(MIB_IQSET_DISABLE, (void *)&close_iQsetup);
	}
	#endif

setOk_filter:
	apmib_update(CURRENT_SETTING);


	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

	char tmpBufapply[200];
	if(submitUrl[0]){
		if(strApply[0])
		{
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
			system("/bin/reboot.sh 3 &");
		}
		else{
			system("echo 1 > /tmp/show_reboot_info");
			REDIRECT_PAGE(wp, submitUrl);
		}
	}
	else 
	{
		strcpy(tmpBuf, T("<script>document.write(showText(Accesscontrolrulesuccess));</script>"));
		closelater_MSG(wp, tmpBuf);
	}
	return;	

setErr_filter:
	ERR_MSG(wp, tmpBuf);
  	return;
}

/////////////////////////////////////////////////////////////////////////////
void formTriggerPort(webs_t wp, char_t *path, char_t *query)
{
	char_t *strAddPort, *strDelAllPort, *strDelPort, *strVal, *submitUrl;
	char_t *strTriFrom, *strTriTo, *strIncFrom, *strIncTo, *strComment, *strApply;
	//kyle
	char_t *ip ,*tcpOpen ,*udpOpen , *comment;
	char tmpBuf[100];
	int entryNum, intVal, i;
	TRIGGERPORT_T entry;
/*#ifndef NO_ACTION
	int pid;
#endif*/

//	char *ptr, *ptr2, *ptr3;
//	int	i;

	strApply = websGetVar(wp, T("isApply"), T(""));   // Tommy

	memset(&entry, '\0', sizeof(entry));
	if (!strApply[0]) {
		strVal = websGetVar(wp, T("enabled"), T(""));
		if ( !gstrcmp(strVal, T("ON")))
			intVal = 1;
		else
			intVal = 0;
		if ( apmib_set(MIB_TRIGGERPORT_ENABLED, (void *)&intVal) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformFilterErrDelAll))</script>"));
			goto setErr_triPort;
		}
	}
	strAddPort = websGetVar(wp, T("addPort"), T(""));
	if (strAddPort[0]) {
		ip= websGetVar(wp, T("ip"), T(""));
		tcpOpen = websGetVar(wp, T("tcpopen"), T(""));
		udpOpen = websGetVar(wp, T("udpopen"), T(""));
		strComment = websGetVar(wp, T("comment"), T(""));
		fixed_inet_aton(ip,entry.ip);
		if (!ip[0] && !tcpOpen[0] && !udpOpen[0] &&!strComment[0])
			goto setOk_triPort;
{
	char *ptr, *ptr2, *ptr3;
	int	i ;


		 ptr = tcpOpen;
	for (i=0;i<5; i++)	{
		if ((ptr2 = strchr(ptr, ',')) != NULL) {
			*ptr2 = '\0';
			ptr2 ++;
		}

		if ((ptr3 = strchr(ptr, '-')) != NULL) {
			*ptr3 = '\0';
			ptr3 ++;

			if ((!(entry.tcp_port[i][0] = atoi(ptr))) || (entry.tcp_port[i][0] < 1) || (entry.tcp_port[i][0] >65536)) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortFr))</script>"));
				goto setErr_triPort;
			}
			if ((!(entry.tcp_port[i][1]=atoi(ptr3))) || (entry.tcp_port[i][1] < 1) || (entry.tcp_port[i][1] >65536))  {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortTo))</script>"));
				goto setErr_triPort;
			}
			if (entry.tcp_port[i][0] > entry.tcp_port[i][1]) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortRange))</script>"));
				goto setErr_triPort;
			}
		}
		else
				entry.tcp_port[i][0]=entry.tcp_port[i][1]=atol(ptr);
		if( ptr2 == NULL)
			break;
		ptr = ptr2;
	}


		 ptr2 = NULL;
		 ptr3 = NULL;
		 ptr = udpOpen;
	for (i=0;i<5; i++)	{
		if ((ptr2 = strchr(ptr, ',')) != NULL) {
			*ptr2 = '\0';
			ptr2 ++;
		}

		if ((ptr3 = strchr(ptr, '-')) != NULL) {
			*ptr3 = '\0';
			ptr3 ++;

			if ((!(entry.udp_port[i][0] = atoi(ptr))) || (entry.udp_port[i][0] < 1) || (entry.udp_port[i][0] >65536)) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortFr))</script>"));
				goto setErr_triPort;
			}
			if ((!(entry.udp_port[i][1]=atoi(ptr3))) || (entry.udp_port[i][1] < 1) || (entry.udp_port[i][1] >65536))  {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortTo))</script>"));
				goto setErr_triPort;
			}
			if (entry.udp_port[i][0] > entry.udp_port[i][1]) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortInvPortRange))</script>"));
				goto setErr_triPort;
			}
		}
		else
				entry.udp_port[i][0]=entry.udp_port[i][1]=atol(ptr);
		if( ptr2 == NULL)
			break;
		ptr = ptr2;
	}



}
		if ( strComment[0] ) {
			if (strlen(strComment) > COMMENT_LEN-1) {
				strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrCmtLong))</script>"));
				goto setErr_triPort;
			}
			strcpy(entry.comment, CharFilter3(strComment));
		}

		// get entry number to see if it exceeds max
		if ( !apmib_get(MIB_TRIGGERPORT_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrGetNum))</script>"));
			goto setErr_triPort;
		}
		if ( (entryNum + 1) > MAX_FILTER_NUM) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrAdd))</script>"));
			goto setErr_triPort;
		}
		apmib_set(MIB_TRIGGERPORT_DEL, (void *)&entry);
		if ( apmib_set(MIB_TRIGGERPORT_ADD, (void *)&entry) == 0) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrAddTbl))</script>"));
			goto setErr_triPort;
		}
	}

	/* Delete entry */
	strDelPort = websGetVar(wp, T("deleteSelPort"), T(""));
	if (strDelPort[0]) {
		if ( !apmib_get(MIB_TRIGGERPORT_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrGetNum))</script>"));
			goto setErr_triPort;
		}
		for (i=entryNum; i>0; i--) {
			snprintf(tmpBuf, 20, "select%d", i);

			strVal = websGetVar(wp, tmpBuf, T(""));
			if ( !gstrcmp(strVal, T("ON")) ) {

				*((char *)&entry) = (char)i;
				if ( !apmib_get(MIB_TRIGGERPORT, (void *)&entry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrGetTbl))</script>"));
					goto setErr_triPort;
				}
				if ( !apmib_set(MIB_TRIGGERPORT_DEL, (void *)&entry)) {
					strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrDelTbl))</script>"));
					goto setErr_triPort;
				}
			}
		}
	}

//}
	/* Delete all entry */
	strDelAllPort = websGetVar(wp, T("deleteAllPort"), T(""));
	if ( strDelAllPort[0]) {
		if ( !apmib_set(MIB_TRIGGERPORT_DELALL, (void *)&entry)) {
			strcpy(tmpBuf, T("<script type='text/javascript'>document.write(showText(aspformTriggerPortErrDelAll))</script>"));
			goto setErr_triPort;
		}
	}

setOk_triPort:
	apmib_update(CURRENT_SETTING);

#ifndef NO_ACTION
/*	pid = fork();
        if (pid) {
	      	waitpid(pid, NULL, 0);
	}
        else if (pid == 0) {
		snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _FIREWALL_SCRIPT_PROG);
		execl( tmpBuf, _FIREWALL_SCRIPT_PROG, NULL);
               	exit(1);
        }*/
	// system("/bin/firewall.sh");
#endif

	submitUrl = websGetVar(wp, T("submit-url"), T(""));   // hidden page
	// Tommy Modified on 0602
#ifdef _TAG_IN_PAGE_
	main_menu=atoi((char_t *)websGetVar(wp, T("main_menu_number"), T("0")));
	sub_menu=atoi((char_t *)websGetVar(wp, T("sub_menu_number"), T("0")));
	third_menu=atoi((char_t *)websGetVar(wp, T("third_menu_number"), T("0")));
#endif
	if (strApply[0]) {
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
	else {
		if (submitUrl[0])
			websRedirect(wp, submitUrl);
		else
			websDone(wp, 200);
	}

	return;

setErr_triPort:
	ERR_MSG(wp, tmpBuf);
  	return;
}

/////////////////////////////////////////////////////////////////////////////
int portFwList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	PORTFW_T entry;


	char	*type, portRange[20], *ip;

	#ifdef __TARGET_BOARD__ //EDX, Robert 2014_12_02 add.
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

	if ( !apmib_get(MIB_PORTFW_NUM, (void *)&entryNum))
	{
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++)
	{
		*((char *)&entry) = (char)i;

		if ( !apmib_get(MIB_PORTFW, (void *)&entry)) return -1;

		ip = fixed_inet_ntoa(entry.ipAddr);

		if ( !strcmp(ip, "0.0.0.0"))  ip = "----";

		if ( entry.protoType == PROTO_BOTH )      type = "TCP+UDP";
		else if ( entry.protoType == PROTO_TCP )  type = "TCP";
		else                                      type = "UDP";

		if ( entry.fromPort == 0)                  strcpy(portRange, "----");
		else if ( entry.fromPort == entry.toPort ) snprintf(portRange, 20, "%d", entry.fromPort);
		else                                       snprintf(portRange, 20, "%d-%d", entry.fromPort, entry.toPort);

		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n")); 
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\" class=\"dnsTableText\">\n"));

		nBytesSent += websWrite(wp, T("<td width='5%%'>%d</td>\n"),i);
   	#ifdef _NETBIOSNAME_
		nBytesSent += websWrite(wp, T("<td width='25%%'><script>document.write(searchComName(\"%s\",0))</script></td>\n"),ip);
    #endif
		nBytesSent += websWrite(wp, T("<td width='14%%'>%s</td>\n"),ip);
		nBytesSent += websWrite(wp, T("<td width='12%%'>%s</td>\n"),type);
		nBytesSent += websWrite(wp, T("<td width='12%%'>%s</td>\n"),portRange);
		nBytesSent += websWrite(wp, T("<td width='22%%'>&nbsp;%s</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("<td width='10%%'><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	return nBytesSent;
}


/////////////////////////////////////////////////////////////////////////////
/*int portFilterList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	PORTFILTER_T entry;
	char	*type, portRange[20];

	if ( !apmib_get(MIB_PORTFILTER_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	nBytesSent += websWrite(wp, T("<tr>"
      	"<td align=center width=\"30%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Port Range</b></font></td>\n"
      	"<td align=center width=\"25%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Protocol</b></font></td>\n"
	"<td align=center width=\"30%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Comment</b></font></td>\n"
      	"<td align=center width=\"15%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Select</b></font></td></tr>\n"));

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_PORTFILTER, (void *)&entry))
			return -1;

		if ( entry.protoType == PROTO_BOTH )
			type = "TCP+UDP";
		else if ( entry.protoType == PROTO_TCP )
			type = "TCP";
		else
			type = "UDP";

		if ( entry.fromPort == 0)
			strcpy(portRange, "----");
		else if ( entry.fromPort == entry.toPort )
			snprintf(portRange, 20, "%d", entry.fromPort);
		else
			snprintf(portRange, 20, "%d-%d", entry.fromPort, entry.toPort);

		nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center width=\"30%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
   			"<td align=center width=\"25%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
     			"<td align=center width=\"30%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"15%%\" bgcolor=\"#C0C0C0\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
				portRange, type, entry.comment, i);
	}
	return nBytesSent;
}
*/

/////////////////////////////////////////////////////////////////////////////
int StcRoutList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	SROUT_T entry;
	char *ip, *mask, *gateway, *interface, ipBuf[20], maskBuf[20];

	if ( !apmib_get(MIB_SROUT_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_SROUT, (void *)&entry))
			return -1;

		ip = fixed_inet_ntoa(entry.ipAddr);
		if ( !strcmp(ip, "0.0.0.0"))
			ip = "----";
		sprintf(ipBuf," %s", ip);

		mask = fixed_inet_ntoa(entry.maskAddr);
		if ( !strcmp(mask, "0.0.0.0"))
			mask = "----";
		sprintf(maskBuf," %s", mask);

		gateway = fixed_inet_ntoa(entry.gatewayAddr);
		if ( !strcmp(gateway, "0.0.0.0"))
			gateway = "----";

		if ( entry.interface ==0 )
			interface = "LAN";
		else
			interface = "WAN";


		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\" class=\"dnsTableText\">\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\" class=\"dnsTableText\">\n"));
		nBytesSent += websWrite(wp, T("<td width='5%%'>%d</td>\n"),i);
		nBytesSent += websWrite(wp, T("<td width='22%%'>%s</td>\n"),ipBuf);
		nBytesSent += websWrite(wp, T("<td width='20%%'>%s</td>\n"),maskBuf);
		nBytesSent += websWrite(wp, T("<td width='22%%'>%s</td>\n"),gateway);
		nBytesSent += websWrite(wp, T("<td width='10%%'>%d</td>\n"),entry.hopCount);
		nBytesSent += websWrite(wp, T("<td width='13%%'>%s</td>\n"),interface);
		nBytesSent += websWrite(wp, T("<td width='8%%'><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}
	return nBytesSent;
}

/////////////////////////////////////////////////////////////////////////////
int DMZList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	DMZ_T entry;
	char	 *pIp, *cIp, buf[20];
	int k, l;
	FILE *fp;
	char staTbl[7][20], staFile[15]={"/var/w1.ip"};

	#ifdef __TARGET_BOARD__ //EDX, Robert 2014_12_02 add.
	FILE *fp1;
	char command[100],IPaddress[20],tmp[2];
	int ipCount=0;
	if( (fp1=fopen("/tmp/auto_refresh_nbtscan", "r")) != NULL )
	    fgets(tmp,2,fp1);
	fclose(fp1);

	if( !strcmp(tmp, "1") )
	{
		system("cat /proc/net/arp | grep : | grep br0 | grep -v 00:00:00:00:00:00 | cut -d ' ' -f 1 > /tmp/log/br0_linkIP.log");
		FILE *fp1=fopen("/tmp/log/br0_linkIP.log","r");
		while(1)
		{
			fscanf(fp1,"%s",&IPaddress);
			if(feof(fp1)) break;

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
		fclose(fp1);
	}
	else
	{
		 system("echo 1 > /tmp/auto_refresh_nbtscan"); //EDX, Robert add, "0:Close 1:Run" auto refresh nbtscan.
	}
	#endif

	if ((fp = fopen( staFile ,"r")) != NULL) {
		for (k=0; k<7; k++) {
			fgets(staTbl[k], 20, fp);
			for(l=0; l<20; l++) {
				if(staTbl[k][l]=='\n') {
					staTbl[k][l]='\0';
					break;
				}
			}
		}
		fclose(fp);
	}

	if ( !apmib_get(MIB_DMZ_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_DMZ, (void *)&entry))
			return -1;

		pIp = fixed_inet_ntoa(entry.pipAddr);
		if ( !strcmp(pIp, "0.0.0.0")) {

			if (fopen( staFile ,"r") == NULL)
				pIp = "----";
			else
				pIp = staTbl[entry.session-1];

		}
		sprintf(buf," %s", pIp);

		cIp = fixed_inet_ntoa(entry.cipAddr);
		if ( !strcmp(cIp, "0.0.0.0"))
			cIp = "----";

#ifdef _Edimax_
		nBytesSent += websWrite(wp, T("<tr align=\"center\" class=\"listtableText\">\n"));
		nBytesSent += websWrite(wp, T("<td>%d</td>\n"),i);
		#ifdef _NETBIOSNAME_
		nBytesSent += websWrite(wp, T("<td><script>document.write(searchComName(\"%s\",0))</script></td>\n"),cIp);
		#endif
		nBytesSent += websWrite(wp, T("<td>%s</td>\n"),buf);
		nBytesSent += websWrite(wp, T("<td>%s</td>\n"),cIp);
		nBytesSent += websWrite(wp, T("<td><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T("</tr>\n"));
#else
		nBytesSent += websWrite(wp, T("<tr align=\"center\" class=\"table2\">\n"));
		nBytesSent += websWrite(wp, T("<td>%d</td>\n"),i);
		#ifdef _NETBIOSNAME_
		nBytesSent += websWrite(wp, T("<td><script>document.write(searchComName(\"%s\",0))</script></td>\n"),cIp);
		#endif
		nBytesSent += websWrite(wp, T("<td>%s</td>\n"),buf);
		nBytesSent += websWrite(wp, T("<td>%s</td>\n"),cIp);
		nBytesSent += websWrite(wp, T("<td><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T("</tr>\n"));
#endif
	}

	return nBytesSent;
}


/////////////////////////////////////////////////////////////////////////////
int ACPCList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i, j;
	ACPC_T entry;
	char	*type, sip[25], eip[25];
	char	Range[100], Index[20], tmpRange[20];
	char	serName[500], buf[60];

	if ( !apmib_get(MIB_ACPC_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_ACPC, (void *)&entry))
			return -1;
//Fixed by Kyle 2007/12/19
		 strcpy(sip,fixed_inet_ntoa(entry.sipAddr));
		 //sip = fixed_inet_ntoa(entry.sipAddr);
		if ( !strcmp(sip, "0.0.0.0"))
			sprintf(sip,"----");
			sprintf(buf,"%s", sip);
//Fixed by Kyle 2007/12/19
		strcpy(eip,fixed_inet_ntoa(entry.eipAddr));
		//eip = fixed_inet_ntoa(entry.eipAddr);
		if ( !strcmp(sip,eip)) {
			sprintf(eip, "");
			strcat(buf,eip);
		}
		else {
			strcat(buf,"-");
			strcat(buf,eip);
		}

//---------------------------------------------------------------------------------------
		snprintf(Index, 20, "%lu", entry.serindex);
		memset(serName,0,sizeof(serName));
		if (entry.serindex & 1 ) strncat(serName,"WWW",3);
		if (entry.serindex & 2 ) strncat(serName,", E-mail Sending",16);
		if (entry.serindex & 4 ) strncat(serName,", News Forums",13);
		if (entry.serindex & 8 ) strncat(serName,", E-mail Receiving",18);
		if (entry.serindex & 16 ) strncat(serName,", Secure HTTP",13);
		if (entry.serindex & 32 ) strncat(serName,", File Transfer",15);
		if (entry.serindex & 64 ) strncat(serName,", MSN Messenger",15);
		if (entry.serindex & 128 ) strncat(serName,", Telnet Service",16);
		if (entry.serindex & 256 ) strncat(serName,", AIM",5);
		if (entry.serindex & 512 ) strncat(serName,", NetMeeting",12);
		if (entry.serindex & 1024 ) strncat(serName,", DNS",5);
		if (entry.serindex & 2048 ) strncat(serName,", SNMP",6);
		if (entry.serindex & 4096 ) strncat(serName,", VPN-PPTP",10);
		if (entry.serindex & 8192 ) strncat(serName,", VPN-L2TP",10);
		if (entry.serindex & 16384 ) strncat(serName,", TCP",5);
		if (entry.serindex & 32768 ) strncat(serName,", UDP",5);
#if 0
//EDX Timmy start
//		if (entry.serindex & 64 ) strncat(serName,", MSN Messenger",15);
		if (entry.serindex & 64 ) strncat(serName,", Telnet Service",16);
		if (entry.serindex & 128 ) strncat(serName,", AIM",5);
		if (entry.serindex & 256 ) strncat(serName,", NetMeeting",12);
		if (entry.serindex & 512 ) strncat(serName,", DNS",5);
		if (entry.serindex & 1024 ) strncat(serName,", SNMP",6);
		if (entry.serindex & 2048 ) strncat(serName,", VPN-PPTP",10);
		if (entry.serindex & 4096 ) strncat(serName,", VPN-L2TP",10);
		if (entry.serindex & 8192 ) strncat(serName,", TCP",5);
		if (entry.serindex & 16384 ) strncat(serName,", UDP",5);
//EDX Timmy end
#endif
		if (serName[0]== ',') serName[0]= ' ';

		if ( entry.protoType == PROTO_BOTH )
			type = "TCP+UDP";
		else if ( entry.protoType == PROTO_TCP )
			type = "TCP";
		else
			type = "UDP";

		memset(Range,0,sizeof(Range));
		for (j=0 ; j<5 ; j++)
		{
			if ( entry.Port[j][0] != 0)
			{
				if ( entry.Port[j][0] == entry.Port[j][1] )
					snprintf(tmpRange, 20, "%d", entry.Port[j][0]);
				else
					snprintf(tmpRange, 20, "%d-%d", entry.Port[j][0], entry.Port[j][1]);
				if(j != 0)
					strcat(Range, ", ");
				strcat(Range, tmpRange);
			}
		}
		if ( !strcmp(Range,"") )
			type = "";

		/*nBytesSent += websWrite(wp, T("<tr align=\"center\" class=\"table2\">"));
		nBytesSent += websWrite(wp, T("<td ><font size=\"2\">%d</td>\n"),i);
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><font size=\"2\">%s&nbsp;</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><font size=\"2\">%s</td>\n"),buf);
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><font size=\"2\">%s</td>\n"),serName);
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><font size=\"2\">%s&nbsp;</td>\n"),type);
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><font size=\"2\">%s&nbsp;</td>\n"),Range);
		nBytesSent += websWrite(wp, T("<td align=center class=\"table2\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),i);
		*/
		
		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align=\"center\">\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align=\"center\">\n"));
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"5%%\">%d</td>\n"),i);
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"22%%\">%s&nbsp;</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"22%%\">%s</td>\n"),buf);
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"15%%\">%s</td>\n"),serName);
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"13%%\">%s&nbsp;</td>\n"),type);
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"13%%\">%s&nbsp;</td>\n"),Range);
		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width=\"10%%\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),i);

	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	return nBytesSent;
}


/////////////////////////////////////////////////////////////////////////////
/*int ipFilterList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	IPFILTER_T entry;
	char	*type, *ip;

	if ( !apmib_get(MIB_IPFILTER_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	nBytesSent += websWrite(wp, T("<tr>"
      	"<td align=center width=\"30%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Local IP Address</b></font></td>\n"
      	"<td align=center width=\"25%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Protocol</b></font></td>\n"
      	"<td align=center width=\"25%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Comment</b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Select</b></font></td></tr>\n"));

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_IPFILTER, (void *)&entry))
			return -1;

		ip = inet_ntoa(*((struct in_addr *)entry.ipAddr));
		if ( !strcmp(ip, "0.0.0.0"))
			ip = "----";

		if ( entry.protoType == PROTO_BOTH )
			type = "TCP+UDP";
		else if ( entry.protoType == PROTO_TCP )
			type = "TCP";
		else
			type = "UDP";

		nBytesSent += websWrite(wp, T("<tr>"
			"<td align=center width=\"30%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"25%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"25%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
				ip, type, entry.comment, i);
	}
	return nBytesSent;
}
*/

/////////////////////////////////////////////////////////////////////////////
int URLBList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	URLB_T entry;

	if ( !apmib_get(MIB_URLB_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_URLB, (void *)&entry))
			return -1;
 
		if(i%2)
			nBytesSent += websWrite(wp, T("<tr style=\"background-color: #F0F0F0;\">\n"));
		else
			nBytesSent += websWrite(wp, T("<tr style=\"background-color: #FFFFFF;\">\n"));

		nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"8%%\">%d</td>\n"),i);
		nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"80%%\">%s</td>\n"),entry.URLBAddr);
		nBytesSent += websWrite(wp, T("<td align=center class=\"dnsTableText\" width=\"12%%\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}

	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
		nBytesSent += websWrite(wp, T("<!-- ========================================================================================== -->\n"));
		nBytesSent += websWrite(wp, T("<input type=\"hidden\" name=\"rulesnum\" value=\"%d\">\n"),i-1);
	}



	return nBytesSent;
}

/////////////////////////////////////////////////////////////////////////////
int macFilterList(int eid, webs_t wp, int argc, char_t **argv)
{
	int nBytesSent=0, entryNum, i;
	MACFILTER_T entry;
	char tmpBuf[100];
	char tmpBuf2[100];

	#if defined(_Edimax_)
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

	if ( !apmib_get(MIB_MACFILTER_NUM, (void *)&entryNum)) {
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}

	for (i=1; i<=entryNum; i++) {
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_MACFILTER, (void *)&entry))
			return -1;

		snprintf(tmpBuf, 100, T("%02x:%02x:%02x:%02x:%02x:%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		snprintf(tmpBuf2, 100, T("%02x%02x%02x%02x%02x%02x"),
			entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
			entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);

		nBytesSent += websWrite(wp, T("\n<!-- ===================================================================== -->\n"));

		if(i%2) nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\" align='center'>\n"));
		else nBytesSent += websWrite(wp, T("<tr bgcolor=\"#FFFFFF\" align='center'>\n"));

		nBytesSent += websWrite(wp, T("<td class=\"dnsTableText\" width='5%%'>%d</td>\n"),i);

		#ifdef _NETBIOSNAME_
		nBytesSent += websWrite(wp, T("<td width='30%%' class=\"dnsTableText\"><script>document.write(searchComName(\"%s\",2))</script></td>\n"),tmpBuf2);
		#endif

		nBytesSent += websWrite(wp, T("<td width='30%%' class=\"dnsTableText\">%s</td>\n"),tmpBuf);
		nBytesSent += websWrite(wp, T("<td width='25%%' class=\"dnsTableText\">&nbsp;%s</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("<td width='10%%' class=\"dnsTableText\"><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T(" <input type=\"hidden\" id=\"Client_PC_macAddr%d\" name=\"Client_PC_macAddr%d\" value=\"%s\">\n"),i,i,tmpBuf2);
		nBytesSent += websWrite(wp, T("</tr>\n"));
		nBytesSent += websWrite(wp, T("<!-- ===================================================================== -->\n"));
	}
	if(entryNum==0){
		nBytesSent += websWrite(wp, T("<tr bgcolor=\"#F0F0F0\">\n"));
		nBytesSent += websWrite(wp, T("  <td align=\"center\" class=\"dnsTableText\">No data available in table</td>\n"));
		nBytesSent += websWrite(wp, T("</tr>\n"));
	}


	return nBytesSent;
}

/////////////////////////////////////////////////////////////////////////////
int triggerPortList(int eid, webs_t wp, int argc, char_t **argv)
{
	int	nBytesSent=0, entryNum, i;
	TRIGGERPORT_T entry;
	char	*triType, triPortRange[20], *incType, Range[100], tmpRange[20];

	char tcpRange[100], udpRange[100], *ipAddr;

	if ( !apmib_get(MIB_TRIGGERPORT_NUM, (void *)&entryNum))
	{
  		websError(wp, 400, T("Get table entry error!\n"));
		return -1;
	}


	for (i=1; i<=entryNum; i++)
	{
		*((char *)&entry) = (char)i;
		if ( !apmib_get(MIB_TRIGGERPORT, (void *)&entry)) return -1;

		ipAddr=fixed_inet_ntoa(entry.ip);
		int j;
		memset(tcpRange,0,sizeof(tcpRange));
		for (j=0 ; j<5 ; j++)
		{
			if ( entry.tcp_port[j][0] != 0)
			{
				if ( entry.tcp_port[j][0] == entry.tcp_port[j][1] )
					snprintf(tmpRange, 20, "%d", entry.tcp_port[j][0]);
				else
					snprintf(tmpRange, 20, "%d-%d", entry.tcp_port[j][0], entry.tcp_port[j][1]);
				if(j != 0)
					strcat(tcpRange, ",");
				strcat(tcpRange, tmpRange);
			}
		}
		memset(udpRange,0,sizeof(udpRange));
		for (j=0 ; j<5 ; j++)
		{
			if ( entry.udp_port[j][0] != 0)
			{
				if ( entry.udp_port[j][0] == entry.udp_port[j][1] )
					snprintf(tmpRange, 20, "%d", entry.udp_port[j][0]);
				else
					snprintf(tmpRange, 20, "%d-%d", entry.udp_port[j][0], entry.udp_port[j][1]);
				if(j != 0)
					strcat(udpRange, ",");
				strcat(udpRange, tmpRange);
			}
		}
		nBytesSent += websWrite(wp, T("<tr align=\"center\" class=\"table2\">\n"));
		nBytesSent += websWrite(wp, T("  <td>%d</td>\n"),i);

		#ifdef _NETBIOSNAME_
		#ifndef _Customer_
		nBytesSent += websWrite(wp, T("  <td><script>document.write(searchComName(\"%s\",0))</script></td>\n"),ipAddr);
		#endif
		#endif

		nBytesSent += websWrite(wp, T("  <td>%s</td>\n"),ipAddr);
		nBytesSent += websWrite(wp, T("  <td>%s</td>\n"),tcpRange);
		nBytesSent += websWrite(wp, T("  <td>%s</td>\n"),udpRange);
		nBytesSent += websWrite(wp, T("  <td>%s</td>\n"),CharFilter4(entry.comment));
		nBytesSent += websWrite(wp, T("  <td><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),i);
		nBytesSent += websWrite(wp, T("</tr>\n"));

	}
	return nBytesSent;
}

#endif // HOME_GATEWAY
