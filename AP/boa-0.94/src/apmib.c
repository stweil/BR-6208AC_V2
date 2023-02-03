/*
 *      Routines to handle MIB operation
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: apmib.c,v 1.4 2005/05/13 07:53:08 tommy Exp $
 *
 */

// include file
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "apmib.h"
#include "mibtbl.h"
char _WAN_IF_[10];
char _LAN_IF_[10];
// MAC address filtering
typedef struct _filter {
	struct _filter *prev, *next;
	char val[1];
} FILTER_T, *FILTER_Tp;

typedef struct _linkChain {
	FILTER_Tp pUsedList, pFreeList;
	int size, num, usedNum, compareLen,appendNumber;
	char *buf;
} LINKCHAIN_T, *LINKCHAIN_Tp;


// macro to remove a link list entry
#define REMOVE_LINK_LIST(entry) { \
	if ( entry ) { \
		if ( entry->prev ) \
			entry->prev->next = entry->next; \
		if ( entry->next ) \
			entry->next->prev = entry->prev; \
	} \
}

// macro to add a link list entry
#define ADD_LINK_LIST(list, entry) { \
	if ( list == NULL ) { \
		list = entry; \
		list->prev = list->next = entry; \
	} \
	else { \
		entry->prev = list; \
		entry->next = list->next; \
		list->next = entry; \
		entry->next->prev = entry; \
	} \
}

// local routine declaration
static int flash_read(char *buf, int offset, int len);
static int flash_write(char *buf, int offset, int len);
static int init_linkchain(LINKCHAIN_Tp pLinkChain, int size, int num);
static int add_linkchain(LINKCHAIN_Tp pLinkChain, char *val);
static int delete_linkchain(LINKCHAIN_Tp pLinkChain, char *val);
static void delete_all_linkchain(LINKCHAIN_Tp pLinkChain);
static int edit_linkchain(LINKCHAIN_Tp pLinkChain, char *val);
static int moveUp_linkchain(LINKCHAIN_Tp pLinkChain, char *val);
static int moveDown_linkchain(LINKCHAIN_Tp pLinkChain, char *val);
static int get_linkchain(LINKCHAIN_Tp pLinkChain, char *val, int index);

// local & global variable declaration
APMIB_Tp pMib=NULL;
//APMIB_Tp pMibDef;	//removed by Erwin
PARAM_HEADER_T hsHeader, dsHeader, csHeader;
HW_SETTING_Tp pHwSetting;

static LINKCHAIN_T wlanMacChain;
#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
static LINKCHAIN_T wlanMacChain2;
static LINKCHAIN_T wlanMacChain3;
static LINKCHAIN_T wlanMacChain4;
#endif

static LINKCHAIN_T iNICMacChain;
#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
static LINKCHAIN_T iNICMacChain2;
static LINKCHAIN_T iNICMacChain3;
static LINKCHAIN_T iNICMacChain4;
#endif

#if defined(_EDIT_DNSPROXYURL_) //EDX patrick add
static LINKCHAIN_T proxyurlChain;
#endif

static LINKCHAIN_T staticDhcpChain;
#ifdef _EZ_QOS_
static LINKCHAIN_T EZQosChain;
#endif
#ifdef HOME_GATEWAY
static LINKCHAIN_T portFwChain, ipFilterChain, portFilterChain, macFilterChain, triggerPortChain, virtSvChain, dmzChain, URLBChain, ACPCChain, sroutChain, rserCltChain, rserUsrChain, wan1QosChain;
#else
static LINKCHAIN_T rserCltChain, rserUsrChain;
#endif

#ifdef _USB_NAS_
static LINKCHAIN_T NAS_User_Chain;
#endif //_USB_NAS_

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
static LINKCHAIN_T wirelessSchChain;
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
static LINKCHAIN_T openvpnUserChain;
#endif
void dump_mem(void *ptr, int size)
{
    int i;
    printf("\n");
    for(i=0; i < size; i++)
	{
        printf("%4.2x", (unsigned )(((unsigned char*)ptr)[i]));
		if((i%16 == 0) && (i!=0))
			printf("\n");
	}
	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
char *apmib_hwconf(void)
{
	int ver;
	char *buff;
	// Read hw setting
	if ( flash_read((char *)&hsHeader, HW_SETTING_OFFSET, sizeof(hsHeader))==0 ) {
		printf("Read hw setting header failed!, Offset=%x\n", HW_SETTING_OFFSET);
		return NULL;
	}

	if ( sscanf(&hsHeader.signature[TAG_LEN], "%02d", &ver) != 1)
		ver = -1;
#ifdef __TARGET_BOARD__
	if (
			memcmp(hsHeader.signature, HW_SETTING_HEADER_TAG, TAG_LEN) || // invalid signatur
			(ver != HW_SETTING_VER) || // version is less than current
			(hsHeader.len < (sizeof(HW_SETTING_T)+1)) // length is less than current
		)
	{
		char temp_hs_ver1[3], temp_hs_ver2[3];
		strncpy(temp_hs_ver1, hsHeader.signature, 2); temp_hs_ver1[2]='\0';
		strncpy(temp_hs_ver2, HW_SETTING_HEADER_TAG, 2); temp_hs_ver2[2]='\0';
		printf("\n");
		printf("===================\n");
		printf("hdr  flash  default\n");
		printf("sig  %5s  %7s\n",temp_hs_ver1,temp_hs_ver2);
		printf("ver  %5d  %7d\n",ver, HW_SETTING_VER);
		printf("len  %5d  %7d\n",hsHeader.len,sizeof(HW_SETTING_T)+1);
		printf("===================\n");
		return NULL;
	}
#endif

	buff = calloc(1, hsHeader.len);
	if ( buff == 0 ) {
		printf("Allocate buffer failed!\n");
		return NULL;
	}

	if ( flash_read(buff, HW_SETTING_OFFSET+sizeof(hsHeader), hsHeader.len)==0 ) {
		printf("Read hw setting failed!\n");
		free(buff);
		return NULL;
	}

	if ( !CHECKSUM_OK(buff, hsHeader.len) ) {
		printf("Invalid checksum of hw setting!\n");
		free(buff);
		return NULL;
	}

	return buff;
}

////////////////////////////////////////////////////////////////////////////////
char *apmib_dsconf(void)
{
	int ver;
	char *buff;

	// Read default s/w mib
	if ( flash_read((char *)&dsHeader, DEFAULT_SETTING_OFFSET, sizeof(dsHeader))==0 ) {
		printf("Read default setting header failed!\n");
		return NULL;
	}

	if ( sscanf(&dsHeader.signature[TAG_LEN], "%02d", &ver) != 1)
		ver = -1;

	if ( memcmp(dsHeader.signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN) || // invalid signatur
		(ver != DEFAULT_SETTING_VER) || // version is less than current
		(dsHeader.len < (sizeof(APMIB_T))) ) { // length is less than current
		printf("Invalid default setting signature or version number [sig=%c%c != sig:%c%c, ver=%d != ver:%d, len=%d , APMIB_T=%d]!\n",
			dsHeader.signature[0], dsHeader.signature[1],DEFAULT_SETTING_HEADER_TAG[0],DEFAULT_SETTING_HEADER_TAG[1], ver,DEFAULT_SETTING_VER, dsHeader.len, sizeof(APMIB_T));
		return NULL;
	}

	if (ver > DEFAULT_SETTING_VER)
		printf("Default setting version is greater than current [f:%d, c:%d]!\n", ver, DEFAULT_SETTING_VER);

	buff = calloc(1, dsHeader.len + 1);
	if ( buff == 0 ) {
		printf("Allocate buffer failed!\n");
		return NULL;
	}

	if ( flash_read(buff, DEFAULT_SETTING_OFFSET+sizeof(dsHeader), dsHeader.len)==0 ) {
		printf("Read default setting failed!\n");
		free(buff);
		return NULL;
	}
	if ( !CHECKSUM_OK(buff, dsHeader.len) ) {
		printf("Invalid checksum of current setting!\n");
		free(buff);
		return NULL;
	}

	return buff;
}

////////////////////////////////////////////////////////////////////////////////
/*char *apmib_csconf(void)		//removed by Erwin
{
	int ver;
	char *buff;

	// Read current s/w mib
	if ( flash_read((char *)&csHeader, CURRENT_SETTING_OFFSET, sizeof(csHeader))==0 ) {
//		printf("Read current setting header failed!\n");
		return NULL;
	}

	if ( sscanf(&csHeader.signature[TAG_LEN], "%02d", &ver) != 1)
		ver = -1;

	if ( memcmp(csHeader.signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN) || // invalid signatur
		(ver != CURRENT_SETTING_VER) || // version is less than current
			(csHeader.len < (sizeof(APMIB_T)+1)) ) { // length is less than current
		printf("Invalid current setting signature or version number [sig=%c%c, ver=%d, len=%d]!\n",
			csHeader.signature[0], csHeader.signature[1], ver, csHeader.len);
		return NULL;
	}

//	if (ver > CURRENT_SETTING_VER)
//		printf("Current setting version is greater than current [f:%d, c:%d]!\n", ver, CURRENT_SETTING_VER);

	buff = calloc(1, csHeader.len);
	if ( buff == 0 ) {
//		printf("Allocate buffer failed!\n");
		return NULL;
	}

	if ( flash_read(buff, CURRENT_SETTING_OFFSET+sizeof(csHeader), csHeader.len)==0 ) {
//		printf("Read current setting failed!\n");
		free(buff);
		return NULL;
	}

	if ( !CHECKSUM_OK(buff, csHeader.len) ) {
//		printf("Invalid checksum of current setting!\n");
		free(buff);
		return NULL;
	}

	return buff;
}
*/
////////////////////////////////////////////////////////////////////////////////
int apmib_init(void)
{
	int i;
	char *buff;
	if ( pMib != NULL )	// has been initialized
		return 1;

	if ((buff=apmib_hwconf()) == NULL)
		return 0;
	pHwSetting = (HW_SETTING_Tp)buff;

	if ((buff=apmib_dsconf()) == NULL) {
		free(pHwSetting);
		return 0;
	}
	pMib = (APMIB_Tp)buff;

	// initialize MAC access control list
	if ( !init_linkchain(&wlanMacChain, sizeof(MACFILTER_T), MAX_WLAN_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}

	for (i=0; i<pMib->acNum; i++) {
		if ( !add_linkchain(&wlanMacChain, (char *)&pMib->acAddrArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	wlanMacChain.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )

	if ( !init_linkchain(&wlanMacChain2, sizeof(MACFILTER_T), MAX_WLAN_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->acNum2; i++) {
		if ( !add_linkchain(&wlanMacChain2, (char *)&pMib->acAddrArray2[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	wlanMacChain2.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

	if ( !init_linkchain(&wlanMacChain3, sizeof(MACFILTER_T), MAX_WLAN_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->acNum3; i++) {
		if ( !add_linkchain(&wlanMacChain3, (char *)&pMib->acAddrArray3[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	wlanMacChain3.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

	if ( !init_linkchain(&wlanMacChain4, sizeof(MACFILTER_T), MAX_WLAN_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->acNum4; i++) {
		if ( !add_linkchain(&wlanMacChain4, (char *)&pMib->acAddrArray4[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	wlanMacChain4.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;
#endif

	// initialize MAC access control list
	if ( !init_linkchain(&iNICMacChain, sizeof(MACFILTER_T), MAX_INIC_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}

	for (i=0; i<pMib->iNIC_acNum; i++) {
		if ( !add_linkchain(&iNICMacChain, (char *)&pMib->iNIC_acAddrArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	iNICMacChain.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )

	if ( !init_linkchain(&iNICMacChain2, sizeof(MACFILTER_T), MAX_INIC_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->iNIC_acNum2; i++) {
		if ( !add_linkchain(&iNICMacChain2, (char *)&pMib->iNIC_acAddrArray2[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	iNICMacChain2.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

	if ( !init_linkchain(&iNICMacChain3, sizeof(MACFILTER_T), MAX_INIC_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->iNIC_acNum3; i++) {
		if ( !add_linkchain(&iNICMacChain3, (char *)&pMib->iNIC_acAddrArray3[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	iNICMacChain3.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

	if ( !init_linkchain(&iNICMacChain4, sizeof(MACFILTER_T), MAX_INIC_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->iNIC_acNum4; i++) {
		if ( !add_linkchain(&iNICMacChain4, (char *)&pMib->iNIC_acAddrArray4[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	iNICMacChain4.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;
#endif

#ifdef _EDIT_DNSPROXYURL_  //EDX patrick add 
	if ( !init_linkchain(&proxyurlChain, sizeof(DNSPROXYURL_T), MAX_DNSPROXYURL_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->proxyurlNum; i++) {
		if ( !add_linkchain(&proxyurlChain, (char *)&pMib->proxyurlArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	proxyurlChain.compareLen = sizeof(DNSPROXYURL_T) - DN_LEN;
#endif


	if ( !init_linkchain(&staticDhcpChain, sizeof(SDHCP_T), MAX_STATIC_DHCP_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}


	for (i=0; i<pMib->SDHCPNum; i++) {
		if ( !add_linkchain(&staticDhcpChain, (char *)&pMib->SDHCPArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	staticDhcpChain.compareLen = sizeof(SDHCP_T);
#ifdef HOME_GATEWAY
///////////////////////////////////////////////////////////////////////////////////////////
	if ( !init_linkchain(&virtSvChain, sizeof(PORTFW_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->vserNum; i++) {
		if ( !add_linkchain(&virtSvChain, (char *)&pMib->vserArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	virtSvChain.compareLen = sizeof(PORTFW_T) - COMMENT_LEN;
///////////////////////////////////////////////////////////////////////////////////////////
	// initialize port forwarding table
	if ( !init_linkchain(&portFwChain, sizeof(PORTFW_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->portFwNum; i++) {
		if ( !add_linkchain(&portFwChain, (char *)&pMib->portFwArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	portFwChain.compareLen = sizeof(PORTFW_T) - COMMENT_LEN;
/////////////////////////////////////////////////////////////////////////////////////////////
	// initialize DMZ table
	if ( !init_linkchain(&dmzChain, sizeof(DMZ_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->DMZNum; i++) {
		if ( !add_linkchain(&dmzChain, (char *)&pMib->DMZArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	dmzChain.compareLen = sizeof(DMZ_T);
	// initialize Static Routing table
	if ( !init_linkchain(&sroutChain, sizeof(SROUT_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->sroutNum; i++) {
		if ( !add_linkchain(&sroutChain, (char *)&pMib->sroutArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	sroutChain.compareLen = sizeof(SROUT_T);
	// initialize URLB table
	if ( !init_linkchain(&URLBChain, sizeof(URLB_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->URLBNum; i++) {
		if ( !add_linkchain(&URLBChain, (char *)&pMib->URLBArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	//kyle 07.02.13
	//URLBChain.compareLen = sizeof(URLB_T) - COMMENT_LEN;
	URLBChain.compareLen = sizeof(URLB_T) ;
	// initialize ACPC table
	if ( !init_linkchain(&ACPCChain, sizeof(ACPC_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->ACPCNum; i++) {
		if ( !add_linkchain(&ACPCChain, (char *)&pMib->ACPCArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	ACPCChain.compareLen = sizeof(ACPC_T) - COMMENT_LEN;
/////////////////////////////////////////////////////////////////////////////////////////////
	// initialize ip-filter table
//	if ( !init_linkchain(&ipFilterChain, sizeof(IPFILTER_T), MAX_FILTER_NUM)) {
	if ( !init_linkchain(&ipFilterChain, sizeof(IPFILTER_T), 1)) {//Erwin Modify 02.09.2003
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->ipFilterNum; i++) {
		if ( !add_linkchain(&ipFilterChain, (char *)&pMib->ipFilterArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	ipFilterChain.compareLen = sizeof(IPFILTER_T) - COMMENT_LEN;
	// initialize port-filter table
//	if ( !init_linkchain(&portFilterChain, sizeof(PORTFILTER_T), MAX_FILTER_NUM)) {
	if ( !init_linkchain(&portFilterChain, sizeof(PORTFILTER_T), 1)) {//Erwin Modify 09.24
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->portFilterNum; i++) {
		if ( !add_linkchain(&portFilterChain, (char *)&pMib->vserArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	portFilterChain.compareLen = sizeof(PORTFILTER_T) - COMMENT_LEN;

	// initialize mac-filter table
	if ( !init_linkchain(&macFilterChain, sizeof(MACFILTER_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->macFilterNum; i++) {
		if ( !add_linkchain(&macFilterChain, (char *)&pMib->macFilterArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	macFilterChain.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

	// initialize trigger-port table
	if ( !init_linkchain(&triggerPortChain, sizeof(TRIGGERPORT_T), MAX_FILTER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->triggerPortNum; i++) {
		if ( !add_linkchain(&triggerPortChain, (char *)&pMib->triggerPortArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	triggerPortChain.compareLen = 5;	// length of trigger port range + proto type
	// initialize QoS of WAN1 table
	if ( !init_linkchain(&wan1QosChain, sizeof(QOS_T), MAX_QOS_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->QosNum[0]; i++) {
		if ( !add_linkchain(&wan1QosChain, (char *)&pMib->QosArray[0][i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	wan1QosChain.compareLen = sizeof(QOS_T) - COMMENT_LEN;
#if defined(_EZ_QOS_)
	// initialize QoS of EZ table
	if ( !init_linkchain(&EZQosChain, sizeof(EZQOS_T), MAX_EZQOS_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->EZQosNum; i++) {
		if ( !add_linkchain(&EZQosChain, (char *)&pMib->EZQosArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	EZQosChain.compareLen = sizeof(EZQOS_T);
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
#else	// Radius server
	// initialize Radius client table
	if ( !init_linkchain(&rserCltChain, sizeof(RSER_CLT_T), 16)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->rserCltNum; i++) {
		if ( !add_linkchain(&rserCltChain, (char *)&pMib->rserCltArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	rserCltChain.compareLen = sizeof(RSER_CLT_T);
	// initialize Radius user table
	if ( !init_linkchain(&rserUsrChain, sizeof(RSER_USR_T), 96)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->rserUsrNum; i++) {
		if ( !add_linkchain(&rserUsrChain, (char *)&pMib->rserUsrArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	rserUsrChain.compareLen = sizeof(RSER_USR_T);
#endif
//printf("bbbb 999\n");

	// initialize MAC access control list
	if ( !init_linkchain(&iNICMacChain, sizeof(MACFILTER_T), MAX_INIC_AC_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}

	for (i=0; i<pMib->iNIC_acNum; i++) {
		if ( !add_linkchain(&iNICMacChain, (char *)&pMib->iNIC_acAddrArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	iNICMacChain.compareLen = sizeof(MACFILTER_T) - COMMENT_LEN;

#ifdef _USB_NAS_
	if ( !init_linkchain(&NAS_User_Chain, sizeof(NAS_USER_T), MAX_NAS_USER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->oem.NAS_User_Num; i++) {
		if ( !add_linkchain(&NAS_User_Chain, (char *)&pMib->oem.Nas_User[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	NAS_User_Chain.compareLen = sizeof(NAS_USER_T);
#endif //_USB_NAS_

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
if ( !init_linkchain(&wirelessSchChain, sizeof(WIRESCH_T), MAX_WIRE_SCH_NUM)) {
        free(pMib);
        free(pHwSetting);
        return 0;
}
for (i=0; i<pMib->WirelessSchNum; i++) {
        if ( !add_linkchain(&wirelessSchChain, (char *)&pMib->WirelessSchArray[i]) ) {
                free(pMib);
                free(pHwSetting);
                return 0;
        }
}
wirelessSchChain.compareLen = sizeof(WIRESCH_T);
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
	if ( !init_linkchain(&openvpnUserChain, sizeof(OPENVPNUSER_T), MAX_OPENVPNUSER_NUM)) {
		free(pMib);
		free(pHwSetting);
		return 0;
	}
	for (i=0; i<pMib->openvpnUserNum; i++) {
		if ( !add_linkchain(&openvpnUserChain, (char *)&pMib->openvpnUserArray[i]) ) {
			free(pMib);
			free(pHwSetting);
			return 0;
		}
	}
	openvpnUserChain.compareLen = sizeof(OPENVPNUSER_T) - OPENVPNUSER_LEN;
#endif
	apmib_get( MIB_OP_MODE, (void *)&i);
	if(i==0){
		#ifdef _DEFAULT_WAN_IF_
			strcpy(_WAN_IF_,_DEFAULT_WAN_IF_);
		#else
			strcpy(_WAN_IF_,"eth2.2");
		#endif
	}
	else if (i == 1)
		strcpy(_WAN_IF_, WLAN_APCLI_IF);

	else if (i == 2)
		strcpy(_WAN_IF_, WLAN_STA_IF);

	else if (i == 3)
		strcpy(_WAN_IF_, INIC_APCLI_IF);

	else if (i == 4)
		strcpy(_WAN_IF_, INIC_STA_IF);

#ifdef _DEFAULT_LAN_IF_
	strcpy(_LAN_IF_,_DEFAULT_LAN_IF_);
#else
	strcpy(_LAN_IF_,"eth2.1");
#endif

	return 1;
}


///////////////////////////////////////////////////////////////////////////////
int apmib_reinit(void)
{

	printf("ReInit APMIB\n");
	if (pMib == NULL)	// has not been initialized
		return 0;

	free(pMib);
//	free(pMibDef);		//removed by Erwin
	free(pHwSetting);
	free(wlanMacChain.buf);
#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	free(wlanMacChain2.buf);
	free(wlanMacChain3.buf);
	free(wlanMacChain4.buf);
#endif

	free(iNICMacChain.buf);
#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	free(iNICMacChain2.buf);
	free(iNICMacChain3.buf);
	free(iNICMacChain4.buf);
#endif

#if defined(_EDIT_DNSPROXYURL_)
	free(proxyurlChain.buf);
#endif
	free(staticDhcpChain.buf);
#ifdef HOME_GATEWAY
	free(virtSvChain.buf);
	free(dmzChain.buf);
	free(URLBChain.buf);
	free(ACPCChain.buf);
	free(sroutChain.buf);
	free(portFwChain.buf);
	free(ipFilterChain.buf);
	free(portFilterChain.buf);
	free(macFilterChain.buf);
	free(triggerPortChain.buf);
#else
	free(rserUsrChain.buf);
	free(rserCltChain.buf);
#endif

#ifdef _USB_NAS_
	free(NAS_User_Chain.buf);
#endif //_USB_NAS_

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
  free(wirelessSchChain.buf);
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
	free(openvpnUserChain.buf);
#endif
	pMib=NULL;
//	pMibDef=NULL;	//removed by Erwin

	return apmib_init();
}


////////////////////////////////////////////////////////////////////////////////
int apmib_get(int id, void *value)
{
	int i, index;
	void *pMibTbl;
	mib_table_entry_T *pTbl;
	unsigned char ch;
	unsigned short wd;
	unsigned long dwd;

//printf("aaaaa 1-1\n");
	// search current setting mib table
	for (i=0; mib_table[i].id; i++) {
		if ( mib_table[i].id == id )
			break;
	}
//printf("aaaaa 2\n");
	if ( mib_table[i].id != 0 ) {
		pMibTbl = (void *)pMib;
		pTbl = mib_table;
	}
	else {
		// if not found, search hw setting table
		for (i=0; hwmib_table[i].id; i++) {
			if ( hwmib_table[i].id == id )
				break;
		}
		if (hwmib_table[i].id == 0) // not found
			return 0;
		pMibTbl = (void *)pHwSetting;
		pTbl = hwmib_table;
	}

//printf("aaaaa 3\n");
	switch (pTbl[i].type) {
	case BYTE_T:
//		*((int *)value) =(int)(*((unsigned char *)(((long)pMibTbl) + pTbl[i].offset)));
		memcpy((char *)&ch, ((char *)pMibTbl) + pTbl[i].offset, 1);
		*((int *)value) = (int)ch;
		break;

	case WORD_T:
//		*((int *)value) =(int)(*((unsigned short *)(((long)pMibTbl) + pTbl[i].offset)));
		memcpy((char *)&wd, ((char *)pMibTbl) + pTbl[i].offset, 2);
		*((int *)value) = (int)wd;
		break;

	case STRING_T:
		strcpy( (char *)value, (const char *)(((long)pMibTbl) + pTbl[i].offset) );
		break;

	case BYTE5_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 5);
		break;

	case BYTE6_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 6);
		break;

	case BYTE8_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 8);
		break;

	case BYTE13_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 13);
		break;
	case BYTE16_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 16);
		break;

	case DWORD_T:
		memcpy((char *)&dwd, ((char *)pMibTbl) + pTbl[i].offset, 4);
		*((int *)value) = (int)dwd;
		break;

	case INDEX_T:
		memcpy((char *)&dwd, ((char *)pMibTbl) + pTbl[i].offset, 4);
		*((int *)value) = (int)dwd;
		break;

	case INDEXDOS_T://////////////////////////////////////////////////////////////
		memcpy((char *)&dwd, ((char *)pMibTbl) + pTbl[i].offset, 4);
		*((int *)value) = (int)dwd;
		break;

#ifdef WLAN_WPA
#if 0
	case BYTE32_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 32);
		break;
#endif
#endif

	case BYTE_ARRAY_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), pTbl[i].size);
		break;

	case IA_T:
		memcpy( (unsigned char *)value, (unsigned char *)(((long)pMibTbl) + pTbl[i].offset), 4);
		break;

	case WLAC_ARRAY_T:
		//printf("DEBUG0\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&wlanMacChain, (char *)value, index );

#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	case WLAC_ARRAY2_T:
		//printf("DEBUG1\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&wlanMacChain2, (char *)value, index );
	case WLAC_ARRAY3_T:
		//printf("DEBUG2\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&wlanMacChain3, (char *)value, index );
	case WLAC_ARRAY4_T:
		//printf("DEBUG3\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&wlanMacChain4, (char *)value, index );
#endif

	case INIC_AC_ARRAY_T:
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&iNICMacChain, (char *)value, index );

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	case INIC_AC_ARRAY2_T:
		//printf("DEBUG1\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&iNICMacChain2, (char *)value, index );
	case INIC_AC_ARRAY3_T:
		//printf("DEBUG2\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&iNICMacChain3, (char *)value, index );
	case INIC_AC_ARRAY4_T:
		//printf("DEBUG3\n");
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&iNICMacChain4, (char *)value, index );
#endif

#ifdef _EDIT_DNSPROXYURL_   //EDX patrick add 
	case DNSPROXYURL_ARRAY_T:
			index = (int)( *((unsigned char *)value));
			return get_linkchain(&proxyurlChain, (char *)value, index );
#endif

	case SDHCP_ARRAY_T:
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&staticDhcpChain, (char *)value, index );
#ifdef HOME_GATEWAY
/////////////////////////////////////////////////////////////////////////////////////////
	case VSER_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&virtSvChain, (char *)value, index );
	case PORTFW_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&portFwChain, (char *)value, index );
	case DMZ_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&dmzChain, (char *)value, index );
	case URLB_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&URLBChain, (char *)value, index );
	case ACPC_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&ACPCChain, (char *)value, index );
	case SROUT_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&sroutChain, (char *)value, index );		
	case IPFILTER_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&ipFilterChain, (char *)value, index );

	case PORTFILTER_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&portFilterChain, (char *)value, index );

	case MACFILTER_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&macFilterChain, (char *)value, index );

	case TRIGGERPORT_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&triggerPortChain, (char *)value, index );

	case WAN1_QOS_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&wan1QosChain, (char *)value, index );
#if  defined(_EZ_QOS_)
	case EZ_QOS_ARRAY_T:
                index = (int)( *((unsigned char *)value));
 		return get_linkchain(&EZQosChain, (char *)value, index );
#endif

///////////////////////////////////////////////////////////////////////////////////////////
#else
	case RSER_USR_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&rserUsrChain, (char *)value, index );
	case RSER_CLT_ARRAY_T:
		index = (int)( *((unsigned char *)value));
 		return get_linkchain(&rserCltChain, (char *)value, index );
#endif

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef _USB_NAS_
	case NAS_USER_ARRAY_T:
		index = (int)( *((unsigned char *)value));
		return get_linkchain(&NAS_User_Chain, (char *)value, index );
#endif //_USB_NAS_

/////////////////////////////////////////////////////////////////////////////////////////
#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
 case WIRE_SCH_ARRAY_T:
    index = (int)( *((unsigned char *)value));
    return get_linkchain(&wirelessSchChain, (char *)value, index );
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
	case OPENVPN_USER_ARRAY_T:
			index = (int)( *((unsigned char *)value));
			return get_linkchain(&openvpnUserChain, (char *)value, index );
#endif
	}
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
/*int apmib_getDef(int id, void *value)		//removed by Erwin
{
	int ret;
	APMIB_Tp saveMib=pMib;

	pMib = pMibDef;
	ret = apmib_get(id, value);
	pMib = saveMib;
	return ret;
}
*/

////////////////////////////////////////////////////////////////////////////////
int apmib_set(int id, void *value)
{
	int i, ret;
	void *pMibTbl;
	mib_table_entry_T *pTbl;
	unsigned char ch;
	unsigned short wd;
	unsigned long dwd;
	unsigned char* tmp;
	int max_chan_num;

	if (id == MIB_WLAN_AC_ADDR_ADD) {
		ret = add_linkchain(&wlanMacChain, (char *)value);
		if ( ret )
			pMib->acNum++;

		return ret;
	}
	if (id == MIB_WLAN_AC_ADDR_DEL) {
		ret = delete_linkchain(&wlanMacChain, (char *)value);
		if ( ret )
			pMib->acNum--;

		return ret;
	}
	if (id == MIB_WLAN_AC_ADDR_DELALL) {
		delete_all_linkchain(&wlanMacChain);
		pMib->acNum = 0;
		return 1;
	}

#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	if (id == MIB_WLAN_AC2_ADDR_ADD) {
		ret = add_linkchain(&wlanMacChain2, (char *)value);
		if ( ret )
			pMib->acNum2++;

		return ret;
	}
	if (id == MIB_WLAN_AC2_ADDR_DEL) {
		ret = delete_linkchain(&wlanMacChain2, (char *)value);
		if ( ret )
			pMib->acNum2--;

		return ret;
	}
	if (id == MIB_WLAN_AC2_ADDR_DELALL) {
		delete_all_linkchain(&wlanMacChain2);
		pMib->acNum2 = 0;
		return 1;
	}

	if (id == MIB_WLAN_AC3_ADDR_ADD) {
		ret = add_linkchain(&wlanMacChain3, (char *)value);
		if ( ret )
			pMib->acNum3++;

		return ret;
	}
	if (id == MIB_WLAN_AC3_ADDR_DEL) {
		ret = delete_linkchain(&wlanMacChain3, (char *)value);
		if ( ret )
			pMib->acNum3--;

		return ret;
	}
	if (id == MIB_WLAN_AC3_ADDR_DELALL) {
		delete_all_linkchain(&wlanMacChain3);
		pMib->acNum3 = 0;
		return 1;
	}

	if (id == MIB_WLAN_AC4_ADDR_ADD) {
		ret = add_linkchain(&wlanMacChain4, (char *)value);
		if ( ret )
			pMib->acNum4++;

		return ret;
	}
	if (id == MIB_WLAN_AC4_ADDR_DEL) {
		ret = delete_linkchain(&wlanMacChain4, (char *)value);
		if ( ret )
			pMib->acNum4--;

		return ret;
	}
	if (id == MIB_WLAN_AC4_ADDR_DELALL) {
		delete_all_linkchain(&wlanMacChain4);
		pMib->acNum4 = 0;
		return 1;
	}
#endif //_MULTIPLE_WLAN_ACCESS_CONTROL_

	if (id == MIB_INIC_AC_ADDR_ADD) {
		ret = add_linkchain(&iNICMacChain, (char *)value);
		if ( ret )
			pMib->iNIC_acNum++;

		return ret;
	}
	if (id == MIB_INIC_AC_ADDR_DEL) {
		ret = delete_linkchain(&iNICMacChain, (char *)value);
		if ( ret )
			pMib->iNIC_acNum--;

		return ret;
	}
	if (id == MIB_INIC_AC_ADDR_DELALL) {
		delete_all_linkchain(&iNICMacChain);
		pMib->iNIC_acNum = 0;
		return 1;
	}

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	if (id == MIB_INIC_AC2_ADDR_ADD) {
		ret = add_linkchain(&iNICMacChain2, (char *)value);
		if ( ret )
			pMib->iNIC_acNum2++;

		return ret;
	}
	if (id == MIB_INIC_AC2_ADDR_DEL) {
		ret = delete_linkchain(&iNICMacChain2, (char *)value);
		if ( ret )
			pMib->iNIC_acNum2--;

		return ret;
	}
	if (id == MIB_INIC_AC2_ADDR_DELALL) {
		delete_all_linkchain(&iNICMacChain2);
		pMib->iNIC_acNum2 = 0;
		return 1;
	}

	if (id == MIB_INIC_AC3_ADDR_ADD) {
		ret = add_linkchain(&iNICMacChain3, (char *)value);
		if ( ret )
			pMib->iNIC_acNum3++;

		return ret;
	}
	if (id == MIB_INIC_AC3_ADDR_DEL) {
		ret = delete_linkchain(&iNICMacChain3, (char *)value);
		if ( ret )
			pMib->iNIC_acNum3--;

		return ret;
	}
	if (id == MIB_INIC_AC3_ADDR_DELALL) {
		delete_all_linkchain(&iNICMacChain3);
		pMib->iNIC_acNum3 = 0;
		return 1;
	}

	if (id == MIB_INIC_AC4_ADDR_ADD) {
		ret = add_linkchain(&iNICMacChain4, (char *)value);
		if ( ret )
			pMib->iNIC_acNum4++;

		return ret;
	}
	if (id == MIB_INIC_AC4_ADDR_DEL) {
		ret = delete_linkchain(&iNICMacChain4, (char *)value);
		if ( ret )
			pMib->iNIC_acNum4--;

		return ret;
	}
	if (id == MIB_INIC_AC4_ADDR_DELALL) {
		delete_all_linkchain(&iNICMacChain4);
		pMib->iNIC_acNum4 = 0;
		return 1;
	}
#endif //_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_

#ifdef _EDIT_DNSPROXYURL_   //EDX patrick add
	if (id == MIB_PROXYURL_ADD) {
		ret = add_linkchain(&proxyurlChain, (char *)value);
		if ( ret )
			pMib->proxyurlNum++;
		return ret;
	}
	if (id == MIB_PROXYURL_DEL) {
		ret = delete_linkchain(&proxyurlChain, (char *)value);
		if ( ret )
			pMib->proxyurlNum--;

		return ret;
	}
	if (id == MIB_PROXYURL_DELALL) {
		delete_all_linkchain(&proxyurlChain);
		pMib->proxyurlNum = 0;
		return 1;
	}
#endif
	if (id == MIB_STATIC_DHCP_ADD) {
		ret = add_linkchain(&staticDhcpChain, (char *)value);
		if ( ret )
			pMib->SDHCPNum++;

		return ret;
	}
	if (id == MIB_STATIC_DHCP_DEL) {
		ret = delete_linkchain(&staticDhcpChain, (char *)value);
		if ( ret )
			pMib->SDHCPNum--;

		return ret;
	}
	if (id == MIB_STATIC_DHCP_DELALL) {
		delete_all_linkchain(&staticDhcpChain);
		pMib->SDHCPNum = 0;
		return 1;
	}
#ifdef HOME_GATEWAY
//////////////////////////////////////////////////////////////////////////////////////////////
	if (id == MIB_VSER_ADD) {
		ret = add_linkchain(&virtSvChain, (char *)value);
		if ( ret )
			pMib->vserNum++;
		return ret;
	}
	if (id == MIB_VSER_DEL) {
		ret = delete_linkchain(&virtSvChain, (char *)value);
		if ( ret )
			pMib->vserNum--;

		return ret;
	}
	if (id == MIB_VSER_DELALL) {
		delete_all_linkchain(&virtSvChain);
		pMib->vserNum = 0;
		return 1;
	}
//////////////////////////////////////////////////////////////////////////////////////////////
	if (id == MIB_PORTFW_ADD) {
		ret = add_linkchain(&portFwChain, (char *)value);
		if ( ret )
			pMib->portFwNum++;

		return ret;
	}
	if (id == MIB_PORTFW_DEL) {
		ret = delete_linkchain(&portFwChain, (char *)value);
		if ( ret )
			pMib->portFwNum--;

		return ret;
	}
	if (id == MIB_PORTFW_DELALL) {
		delete_all_linkchain(&portFwChain);
		pMib->portFwNum = 0;
		return 1;
	}
///////////////////////////////////////////////////////////////////////////////////////////
                                //DMZ
	if (id == MIB_DMZ_ADD) {

		ret = add_linkchain(&dmzChain, (char *)value);
		if ( ret )
			pMib->DMZNum++;

		return ret;
	}
	if (id == MIB_DMZ_DEL) {
		ret = delete_linkchain(&dmzChain, (char *)value);
		if ( ret )
			pMib->DMZNum--;

		return ret;
	}
	if (id == MIB_DMZ_EDIT) {
		ret = edit_linkchain(&dmzChain, (char *)value);

		return ret;
	}
	if (id == MIB_DMZ_DELALL) {
		delete_all_linkchain(&dmzChain);
		pMib->DMZNum = 0;
		return 1;
	}
                                //Static Routing
	if (id == MIB_SROUT_ADD) {
		ret = add_linkchain(&sroutChain, (char *)value);
		if ( ret )
			pMib->sroutNum++;

		return ret;
	}
	if (id == MIB_SROUT_DEL) {
		ret = delete_linkchain(&sroutChain, (char *)value);
		if ( ret )
			pMib->sroutNum--;

		return ret;
	}
	if (id == MIB_SROUT_DELALL) {
		delete_all_linkchain(&sroutChain);
		pMib->sroutNum = 0;
		return 1;
	}
                                //URLB
	if (id == MIB_URLB_ADD) {
		ret = add_linkchain(&URLBChain, (char *)value);
		if ( ret )
			pMib->URLBNum++;
		return ret;
	}
	if (id == MIB_URLB_DEL) {
		ret = delete_linkchain(&URLBChain, (char *)value);
		if ( ret )
			pMib->URLBNum--;
		return ret;
	}
	if (id == MIB_URLB_DELALL) {
		delete_all_linkchain(&URLBChain);
		pMib->URLBNum = 0;
		return 1;
	}
                                //ACPC
	if (id == MIB_ACPC_ADD) {
		ret = add_linkchain(&ACPCChain, (char *)value);
		if ( ret )
			pMib->ACPCNum++;

		return ret;
	}
	if (id == MIB_ACPC_DEL) {
		ret = delete_linkchain(&ACPCChain, (char *)value);
		if ( ret )
			pMib->ACPCNum--;

		return ret;
	}
	if (id == MIB_ACPC_DELALL) {
		delete_all_linkchain(&ACPCChain);
		pMib->ACPCNum = 0;
		return 1;
	}
///////////////////////////////////////////////////////////////////////////////////////////
	if (id == MIB_IPFILTER_ADD) {
		ret = add_linkchain(&ipFilterChain, (char *)value);
		if ( ret )
			pMib->ipFilterNum++;

		return ret;
	}
	if (id == MIB_IPFILTER_DEL) {
		ret = delete_linkchain(&ipFilterChain, (char *)value);
		if ( ret )
			pMib->ipFilterNum--;

		return ret;
	}
	if (id == MIB_IPFILTER_DELALL) {
		delete_all_linkchain(&ipFilterChain);
		pMib->ipFilterNum = 0;
		return 1;
	}

	if (id == MIB_PORTFILTER_ADD) {
		ret = add_linkchain(&portFilterChain, (char *)value);
		if ( ret )
			pMib->portFilterNum++;

		return ret;
	}
	if (id == MIB_PORTFILTER_DEL) {
		ret = delete_linkchain(&portFilterChain, (char *)value);
		if ( ret )
			pMib->portFilterNum--;

		return ret;
	}
	if (id == MIB_PORTFILTER_DELALL) {
		delete_all_linkchain(&portFilterChain);
		pMib->portFilterNum = 0;
		return 1;
	}

	if (id == MIB_MACFILTER_ADD) {
		ret = add_linkchain(&macFilterChain, (char *)value);
		if ( ret )
			pMib->macFilterNum++;

		return ret;
	}
	if (id == MIB_MACFILTER_DEL) {
		ret = delete_linkchain(&macFilterChain, (char *)value);
		if ( ret )
			pMib->macFilterNum--;

		return ret;
	}
	if (id == MIB_MACFILTER_DELALL) {
		delete_all_linkchain(&macFilterChain);
		pMib->macFilterNum = 0;
		return 1;
	}
	if (id == MIB_TRIGGERPORT_ADD) {
		ret = add_linkchain(&triggerPortChain, (char *)value);
		if ( ret )
			pMib->triggerPortNum++;

		return ret;
	}
	if (id == MIB_TRIGGERPORT_DEL) {
		ret = delete_linkchain(&triggerPortChain, (char *)value);
		if ( ret )
			pMib->triggerPortNum--;

		return ret;
	}
	if (id == MIB_TRIGGERPORT_DELALL) {
		delete_all_linkchain(&triggerPortChain);
		pMib->triggerPortNum = 0;
		return 1;
	}
	if (id == MIB_WAN1_QOS_ADD) {	//WAN1
		ret = add_linkchain(&wan1QosChain, (char *)value);
		if ( ret )
			(pMib->QosNum[0])++;
		return ret;
	}
	if (id == MIB_WAN1_QOS_EDIT) {
		ret = edit_linkchain(&wan1QosChain, (char *)value);
		return ret;
	}
	if (id == MIB_WAN1_QOS_UP) {
		ret = moveUp_linkchain(&wan1QosChain, (char *)value);
		return ret;
	}
	if (id == MIB_WAN1_QOS_DOWN) {
		ret = moveDown_linkchain(&wan1QosChain, (char *)value);
		return ret;
	}
	if (id == MIB_WAN1_QOS_DEL) {
		ret = delete_linkchain(&wan1QosChain, (char *)value);
		if ( ret )
			pMib->QosNum[0]--;
		return ret;
	}
	if (id == MIB_WAN1_QOS_DELALL) {
		delete_all_linkchain(&wan1QosChain);
		pMib->QosNum[0] = 0;
		return 1;
	}
#if  defined(_EZ_QOS_)
	if (id == MIB_EZ_QOS_ADD) {	//EZ QoS
		ret = add_linkchain(&EZQosChain, (char *)value);
		if ( ret )
			pMib->EZQosNum++;
		return ret;
	}
	if (id == MIB_EZ_QOS_EDIT) {
		ret = edit_linkchain(&EZQosChain, (char *)value);
		return ret;
	}
	if (id == MIB_EZ_QOS_UP) {
		ret = moveUp_linkchain(&EZQosChain, (char *)value);
		return ret;
	}
	if (id == MIB_EZ_QOS_DOWN) {
		ret = moveDown_linkchain(&EZQosChain, (char *)value);
		return ret;
	}
	if (id == MIB_EZ_QOS_DEL) {
		ret = delete_linkchain(&EZQosChain, (char *)value);
		if ( ret )
			pMib->EZQosNum--;
		return ret;
	}
	if (id == MIB_EZ_QOS_DELALL) {
		delete_all_linkchain(&EZQosChain);
		pMib->EZQosNum = 0;
		return 1;
	}
#endif
///////////////////////////////////////////////////////////////////////////////////////
#else
                                //Radius User
	if (id == MIB_RSER_USR_ADD) {
		ret = add_linkchain(&rserUsrChain, (char *)value);
		if ( ret )
			pMib->rserUsrNum++;

		return ret;
	}
	if (id == MIB_RSER_USR_DEL) {
		ret = delete_linkchain(&rserUsrChain, (char *)value);
		if ( ret )
			pMib->rserUsrNum--;

		return ret;
	}
	if (id == MIB_RSER_USR_DELALL) {
		delete_all_linkchain(&rserUsrChain);
		pMib->rserUsrNum = 0;
		return 1;
	}
                                //Radius Client
	if (id == MIB_RSER_CLT_ADD) {
		ret = add_linkchain(&rserCltChain, (char *)value);
		if ( ret )
			pMib->rserCltNum++;

		return ret;
	}
	if (id == MIB_RSER_CLT_DEL) {
		ret = delete_linkchain(&rserCltChain, (char *)value);
		if ( ret )
			pMib->rserCltNum--;

		return ret;
	}
	if (id == MIB_RSER_CLT_DELALL) {
		delete_all_linkchain(&rserCltChain);
		pMib->rserCltNum = 0;
		return 1;
	}
///////////////////////////////////////////////////////////////////////////////////////////
#endif

#ifdef _USB_NAS_
	if (id == MIB_NAS_USER_ADD) {
		ret = add_linkchain(&NAS_User_Chain, (char *)value);
		if ( ret )
			pMib->oem.NAS_User_Num++;
		return ret;
	}
	if (id == MIB_NAS_USER_DEL) {
		ret = delete_linkchain(&NAS_User_Chain, (char *)value);
		if ( ret )
			pMib->oem.NAS_User_Num--;

		return ret;
	}
	if (id == MIB_NAS_USER_EDIT) {
		ret = edit_linkchain(&NAS_User_Chain, (char *)value);
		return ret;
	}
	if (id == MIB_NAS_USER_DELALL) {
		delete_all_linkchain(&NAS_User_Chain);
		pMib->oem.NAS_User_Num = 0;
		return 1;
	}
#endif //_USB_NAS_

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
if (id == MIB_WIRE_SCH_ADD) {
        ret = add_linkchain(&wirelessSchChain, (char *)value);
        if ( ret )
                pMib->WirelessSchNum++;
        return ret;
}
if (id == MIB_WIRE_SCH_EDIT) {
			ret = edit_linkchain(&wirelessSchChain, (char *)value);
			return ret;
		}
if (id == MIB_WIRE_SCH_DEL) {
        ret = delete_linkchain(&wirelessSchChain, (char *)value);
        if ( ret )
                pMib->WirelessSchNum--;
        return ret;
}
if (id == MIB_WIRE_SCH_DELALL) {
        delete_all_linkchain(&wirelessSchChain);
        pMib->WirelessSchNum = 0;
        return 1;
}
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
	if (id == MIB_OPENVPN_USER_ADD) {
		ret = add_linkchain(&openvpnUserChain, (char *)value);
		if ( ret )
			pMib->openvpnUserNum++;
		return ret;
	}
	if (id == MIB_OPENVPN_USER_EDIT) {
			ret = edit_linkchain(&openvpnUserChain, (char *)value);
			return ret;
		}
	if (id == MIB_OPENVPN_USER_DEL) {
		ret = delete_linkchain(&openvpnUserChain, (char *)value);
		if ( ret )
			pMib->openvpnUserNum--;

		return ret;
	}
	if (id == MIB_OPENVPN_USER_DELALL) {
		delete_all_linkchain(&openvpnUserChain);
		pMib->openvpnUserNum = 0;
		return 1;
	}
#endif
	for (i=0; mib_table[i].id; i++) {
		if ( mib_table[i].id == id )
			break;
	}
	if ( mib_table[i].id != 0 ) {
		pMibTbl = (void *)pMib;
		pTbl = mib_table;
	}
	else {
		// if not found, search hw setting table
		for (i=0; hwmib_table[i].id; i++) {
			if ( hwmib_table[i].id == id )
				break;
		}
		if (hwmib_table[i].id == 0) // not found
			return 0;
		pMibTbl = (void *)pHwSetting;
		pTbl = hwmib_table;
	}

	switch (pTbl[i].type) {
	case BYTE_T:
//		*((unsigned char *)(((long)pMibTbl) + pTbl[i].offset)) = (unsigned char)(*((int *)value));
		ch = (unsigned char)(*((int *)value));
		memcpy( ((char *)pMibTbl) + pTbl[i].offset, &ch, 1);
		break;

	case WORD_T:
//		*((unsigned short *)(((long)pMibTbl) + pTbl[i].offset)) = (unsigned short)(*((int *)value));
		wd = (unsigned short)(*((int *)value));
		memcpy( ((char *)pMibTbl) + pTbl[i].offset, &wd, 2);
		break;

	case STRING_T:
		if ( strlen(value)+1 > pTbl[i].size )
			return 0;
		strcpy((char *)(((long)pMibTbl) + pTbl[i].offset), (char *)value);
		break;

	case BYTE5_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 5);
		break;

	case BYTE6_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 6);
		break;

	case BYTE8_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 8);
		break;

	case BYTE13_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 13);
		break;

	case BYTE16_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 16);
		break;

	case DWORD_T:
		dwd = *(unsigned long *)value;
		memcpy( ((char *)pMibTbl) + pTbl[i].offset, &dwd, 4);
		break;

	case INDEX_T:
		dwd = *(unsigned long *)value;
		memcpy( ((char *)pMibTbl) + pTbl[i].offset, &dwd, 4);
		break;

	case INDEXDOS_T://////////////////////////////////////////////////////////////
		dwd = *(unsigned long *)value;
		memcpy( ((char *)pMibTbl) + pTbl[i].offset, &dwd, 4);
		break;
#if 0
#ifdef WLAN_WPA
	case BYTE32_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, 32);
		break;
#endif
#endif

	case BYTE_ARRAY_T:

		tmp = (unsigned char*) value;
		max_chan_num = (id == MIB_HW_TX_POWER_CCK)? MAX_CCK_CHAN_NUM: MAX_OFDM_CHAN_NUM ;
		if(tmp[2] == 0xff){ // set one channel value
			memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset + (long)tmp[0] -1), (unsigned char *)(tmp+1), 1);
		}
		else
			memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, max_chan_num);
		break;



//		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value, pTbl[i].size);
//		break;

	case IA_T:
		memcpy((unsigned char *)(((long)pMibTbl) + pTbl[i].offset), (unsigned char *)value,  4);
		break;

	case WLAC_ARRAY_T:
	case SDHCP_ARRAY_T:
#ifdef HOME_GATEWAY
	case VSER_ARRAY_T:
	case DMZ_ARRAY_T:
	case URLB_ARRAY_T:
	case ACPC_ARRAY_T:
	case SROUT_ARRAY_T:
	case PORTFW_ARRAY_T:
	case IPFILTER_ARRAY_T:
	case PORTFILTER_ARRAY_T:
	case MACFILTER_ARRAY_T:
	case TRIGGERPORT_ARRAY_T:
	case WAN1_QOS_ARRAY_T:
#if  defined(_EZ_QOS_)
	case EZ_QOS_ARRAY_T:
#endif
#else
	case RSER_CLT_ARRAY_T:	//radius client
	case RSER_USR_ARRAY_T:	//radius user
#endif
	case INIC_AC_ARRAY_T:
#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
	case INIC_AC_ARRAY_T:
	case INIC_AC_ARRAY_T:
	case INIC_AC_ARRAY_T:
#endif //_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_

#ifdef _USB_NAS_
	case NAS_USER_ARRAY_T:
#endif //_USB_NAS_

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
	case WIRE_SCH_ARRAY_T:
#endif
		return 0;
	}
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
/*int apmib_setDef(int id, void *value)		//removed by Erwin
{
	int ret;
	APMIB_Tp saveMib=pMib;

	pMib = pMibDef;
	ret = apmib_set(id, value);
	pMib = saveMib;
	return ret;
}
*/


////////////////////////////////////////////////////////////////////////////////
/* Update current used MIB into flash in current setting area
 */
int apmib_update(CONFIG_DATA_T type)
{
	int i, len;
	unsigned char checksum;
	unsigned char *data;

	if (type & HW_SETTING) {
		data = (char *)pHwSetting;
		checksum = CHECKSUM(data, hsHeader.len-1);
		data[hsHeader.len-1] = checksum;

		if ( flash_write((char *)data, HW_SETTING_OFFSET+sizeof(hsHeader), hsHeader.len)==0 ) {
			printf("write hs MIB failed!\n");
			return 0;
		}
	}

	if ((type & CURRENT_SETTING) || (type & DEFAULT_SETTING)) {
		memset( pMib->acAddrArray, '\0', MAX_WLAN_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->acNum; i++) {
			get_linkchain(&wlanMacChain, (void *)&pMib->acAddrArray[i], i+1);
		}
#if defined(_MULTIPLE_WLAN_ACCESS_CONTROL_ )
		memset( pMib->acAddrArray2, '\0', MAX_WLAN_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->acNum2; i++) {
			get_linkchain(&wlanMacChain2, (void *)&pMib->acAddrArray2[i], i+1);
		}
		memset( pMib->acAddrArray3, '\0', MAX_WLAN_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->acNum3; i++) {
			get_linkchain(&wlanMacChain3, (void *)&pMib->acAddrArray3[i], i+1);
		}
		memset( pMib->acAddrArray4, '\0', MAX_WLAN_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->acNum4; i++) {
			get_linkchain(&wlanMacChain4, (void *)&pMib->acAddrArray4[i], i+1);
		}
#endif //_MULTIPLE_WLAN_ACCESS_CONTROL_

		memset( pMib->iNIC_acAddrArray, '\0', MAX_INIC_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->iNIC_acNum; i++) {
			get_linkchain(&iNICMacChain, (void *)&pMib->iNIC_acAddrArray[i], i+1);
		}

#if defined(_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_ )
		memset( pMib->iNIC_acAddrArray2, '\0', MAX_INIC_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->iNIC_acNum2; i++) {
			get_linkchain(&iNICMacChain2, (void *)&pMib->iNIC_acAddrArray2[i], i+1);
		}
		memset( pMib->iNIC_acAddrArray3, '\0', MAX_INIC_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->iNIC_acNum3; i++) {
			get_linkchain(&iNICMacChain3, (void *)&pMib->iNIC_acAddrArray3[i], i+1);
		}
		memset( pMib->iNIC_acAddrArray4, '\0', MAX_INIC_AC_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->iNIC_acNum4; i++) {
			get_linkchain(&iNICMacChain4, (void *)&pMib->iNIC_acAddrArray4[i], i+1);
		}
#endif //_INIC_MULTIPLE_WLAN_ACCESS_CONTROL_

#ifdef _EDIT_DNSPROXYURL_   //EDX patrick add 
		memset( pMib->proxyurlArray, '\0', MAX_DNSPROXYURL_NUM*sizeof(DNSPROXYURL_T) );
		for (i=0; i<pMib->proxyurlNum; i++) {
			get_linkchain(&proxyurlChain, (void *)&pMib->proxyurlArray[i], i+1);
		}
#endif
		memset( pMib->SDHCPArray, '\0', MAX_STATIC_DHCP_NUM*sizeof(SDHCP_T) );
		for (i=0; i<pMib->SDHCPNum; i++) {
			get_linkchain(&staticDhcpChain, (void *)&pMib->SDHCPArray[i], i+1);
		}
#ifdef HOME_GATEWAY
		memset( pMib->portFwArray, '\0', MAX_FILTER_NUM*sizeof(PORTFW_T) );
		for (i=0; i<pMib->portFwNum; i++) {
			get_linkchain(&portFwChain, (void *)&pMib->portFwArray[i], i+1);
		}
////////////////////////////////////////////////////////////////////////////////////////////
		memset( pMib->DMZArray, '\0', MAX_FILTER_NUM*sizeof(DMZ_T) );
		for (i=0; i<pMib->DMZNum; i++) {
			get_linkchain(&dmzChain, (void *)&pMib->DMZArray[i], i+1);
		}
		memset( pMib->URLBArray, '\0', MAX_FILTER_NUM*sizeof(URLB_T) );
		for (i=0; i<pMib->URLBNum; i++) {
			get_linkchain(&URLBChain, (void *)&pMib->URLBArray[i], i+1);
		}
		memset( pMib->ACPCArray, '\0', MAX_FILTER_NUM*sizeof(ACPC_T) );
		for (i=0; i<pMib->ACPCNum; i++) {
			get_linkchain(&ACPCChain, (void *)&pMib->ACPCArray[i], i+1);
		}
		memset( pMib->sroutArray, '\0', MAX_FILTER_NUM*sizeof(SROUT_T) );
		for (i=0; i<pMib->sroutNum; i++) {
			get_linkchain(&sroutChain, (void *)&pMib->sroutArray[i], i+1);
		}
//		memset( pMib->ipFilterArray, '\0', MAX_FILTER_NUM*sizeof(IPFILTER_T) );
		memset( pMib->ipFilterArray, '\0', sizeof(IPFILTER_T) );
		for (i=0; i<pMib->ipFilterNum; i++) {
			get_linkchain(&ipFilterChain, (void *)&pMib->ipFilterArray[i], i+1);
		}
		memset( pMib->vserArray, '\0', MAX_FILTER_NUM*sizeof(PORTFW_T) );
		for (i=0; i<pMib->vserNum; i++) {
			get_linkchain(&virtSvChain, (void *)&pMib->vserArray[i], i+1);
		}
//		memset( pMib->portFilterArray, '\0', MAX_FILTER_NUM*sizeof(PORTFILTER_T) );
		memset( pMib->portFilterArray, '\0', sizeof(PORTFILTER_T) );
		for (i=0; i<pMib->portFilterNum; i++) {
			get_linkchain(&portFilterChain, (void *)&pMib->portFilterArray[i], i+1);
		}
		memset( pMib->macFilterArray, '\0', MAX_FILTER_NUM*sizeof(MACFILTER_T) );
		for (i=0; i<pMib->macFilterNum; i++) {
			get_linkchain(&macFilterChain, (void *)&pMib->macFilterArray[i], i+1);
		}
		memset( pMib->triggerPortArray, '\0', MAX_FILTER_NUM*sizeof(TRIGGERPORT_T) );
		for (i=0; i<pMib->triggerPortNum; i++) {
			get_linkchain(&triggerPortChain, (void *)&pMib->triggerPortArray[i], i+1);
		}
		memset( pMib->QosArray[0], '\0', MAX_QOS_NUM*sizeof(QOS_T) );
		for (i=0; i<pMib->QosNum[0]; i++) {
			get_linkchain(&wan1QosChain, (void *)&pMib->QosArray[0][i], i+1);
		}
#if  defined(_EZ_QOS_)
		memset( pMib->EZQosArray, '\0', MAX_EZQOS_NUM*sizeof(EZQOS_T) );
		for (i=0; i<pMib->EZQosNum; i++) {
			get_linkchain(&EZQosChain, (void *)&pMib->EZQosArray[i], i+1);
		}
#endif
////////////////////////////////////////////////////////////////////////////////////
#else
		memset( pMib->rserCltArray, '\0', 16*sizeof(RSER_CLT_T) );
		for (i=0; i<pMib->rserCltNum; i++) {
			get_linkchain(&rserCltChain, (void *)&pMib->rserCltArray[i], i+1);
		}
		memset( pMib->rserUsrArray, '\0', 96*sizeof(RSER_USR_T) );
		for (i=0; i<pMib->rserUsrNum; i++) {
			get_linkchain(&rserUsrChain, (void *)&pMib->rserUsrArray[i], i+1);
		}
////////////////////////////////////////////////////////////////////////////////////
#endif
		
#ifdef _USB_NAS_
		memset( pMib->oem.Nas_User, '\0', MAX_NAS_USER_NUM * sizeof(NAS_USER_T) );
		for (i=0; i<pMib->oem.NAS_User_Num; i++) {
			get_linkchain(&NAS_User_Chain, (void *)&pMib->oem.Nas_User[i], i+1);
		}
#endif //_USB_NAS_		

#if defined(_WIRELESS_SCHEDULE_V2_) //EDX, Robert Add 2015_03_31
    memset( pMib->WirelessSchArray, '\0', MAX_WIRE_SCH_NUM*sizeof(WIRESCH_T) );
    for (i=0; i<pMib->WirelessSchNum; i++) {
            get_linkchain(&wirelessSchChain, (void *)&pMib->WirelessSchArray[i], i+1);
    }
#endif

#ifdef _OPENVPN_ //EDX, Robert Add 2015_04_09
		memset( pMib->openvpnUserArray, '\0', MAX_OPENVPNUSER_NUM*sizeof(OPENVPNUSER_T) );
		for (i=0; i<pMib->openvpnUserNum; i++) {
			get_linkchain(&openvpnUserChain, (void *)&pMib->openvpnUserArray[i], i+1);
		}
#endif


			data = (unsigned char *)pMib;
			checksum = CHECKSUM(data, dsHeader.len-1);
			*(data + dsHeader.len - 1) = checksum;
			i = DEFAULT_SETTING_OFFSET + sizeof(dsHeader);
			len = dsHeader.len;


		if ( flash_write((char *)data, i, len)==0 ) {
			printf("Write flash current-setting failed!1\n");
			return 0;
		}
	}
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
/* Update default setting MIB into current setting area
 */
/*int apmib_updateDef(void)		//removed by Erwin
{
	unsigned char *data, checksum;
	PARAM_HEADER_T header;
	int i;

	memcpy(header.signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN);
	memcpy(&header.signature[TAG_LEN], &dsHeader.signature[TAG_LEN], SIGNATURE_LEN-TAG_LEN);

	header.len = dsHeader.len;
	data = (unsigned char *)pMibDef;
	checksum = CHECKSUM(data, header.len-1);
	*(data + header.len - 1) = checksum;

	i = CURRENT_SETTING_OFFSET;
	if ( flash_write((char *)&header, i, sizeof(header))==0 ) {
		printf("Write flash current-setting header failed!\n");
		return 0;
	}
	i += sizeof(header);

	if ( flash_write((char *)data, i, header.len)==0 ) {
		printf("Write flash current-setting failed!\n");
		return 0;
	}

	return 1;
}
*/

////////////////////////////////////////////////////////////////////////////////
/* Update MIB into flash current setting area
 */
int apmib_updateFlash(CONFIG_DATA_T type, char *data, int len, int force, int ver)
{
	unsigned char checksum, checksum1, *ptr=NULL;
	int i, offset=0, curLen, curVer;
	unsigned char *pMibData, *pHdr, tmpBuf[20];

	if ( type == HW_SETTING ) {
		curLen = hsHeader.len - 1;
		pMibData = (unsigned char *)pHwSetting;
		pHdr = (unsigned char *)&hsHeader;
		i = HW_SETTING_OFFSET;
	}
	else if ( type == DEFAULT_SETTING ) {
		curLen = dsHeader.len - 1;
//		pMibData = (unsigned char *)pMibDef;	//removed by Erwin
		pMibData = (unsigned char *)pMib;
		pHdr = (unsigned char *)&dsHeader;
		i = DEFAULT_SETTING_OFFSET;
	}
/*	else  {						//removed by Erwin
		curLen = csHeader.len - 1;
		pMibData = (unsigned char *)pMib;
		pHdr = (unsigned char *)&csHeader;
		i = CURRENT_SETTING_OFFSET;
	}
*/
	if (force==2) { // replace by input mib
		((PARAM_HEADER_Tp)pHdr)->len = len + 1;
		sprintf(tmpBuf, "%02d", ver);
		memcpy(&pHdr[TAG_LEN], tmpBuf, SIGNATURE_LEN-TAG_LEN);
		checksum = CHECKSUM(data, len);
		pMibData = data;
		curLen = len;
	}
	else if (force==1) { // update mib but keep not used mib
		sscanf(&((PARAM_HEADER_Tp)pHdr)->signature[TAG_LEN], "%02d", &curVer);
		if ( curVer < ver ) {
			sprintf(tmpBuf, "%02d", ver);
			memcpy(&((PARAM_HEADER_Tp)pHdr)->signature[TAG_LEN],
					tmpBuf, SIGNATURE_LEN-TAG_LEN);
		}
		checksum = CHECKSUM(data, len);
		if (curLen > len) {
			((PARAM_HEADER_Tp)pHdr)->len = curLen + 1;
			ptr = pMibData + len;
			offset = curLen - len;
			checksum1 = CHECKSUM(ptr, offset);
			checksum +=  checksum1;
		}
		else
			((PARAM_HEADER_Tp)pHdr)->len = len + 1;

		curLen = len;
		pMibData = data;
	}
	else { // keep old mib, only update new added portion
		sscanf(&((PARAM_HEADER_Tp)pHdr)->signature[TAG_LEN], "%02d", &curVer);
		if ( curVer < ver ) {
			sprintf(tmpBuf, "%02d", ver);
			memcpy(&((PARAM_HEADER_Tp)pHdr)->signature[TAG_LEN],
					tmpBuf, SIGNATURE_LEN-TAG_LEN);
		}
		if ( len > curLen ) {
			((PARAM_HEADER_Tp)pHdr)->len = len + 1;
			offset = len - curLen;
			checksum = CHECKSUM(pMibData, curLen);
			ptr = data + curLen;
			checksum1 = CHECKSUM(ptr, offset);
			checksum +=  checksum1;
		}
		else
			checksum = CHECKSUM(pMibData, curLen);
	}

	if ( flash_write((char *)pHdr, i, sizeof(PARAM_HEADER_T))==0 ) {
		printf("Write flash current-setting header failed!\n");
		return 0;
	}
	i += sizeof(PARAM_HEADER_T);

	if ( flash_write(pMibData, i, curLen)==0 ) {
		printf("Write flash current-setting failed!\n");
		return 0;
	}
	i += curLen;

	if (offset > 0) {
		if ( flash_write((char *)ptr, i, offset)==0 ) {
			printf("Write flash current-setting failed!\n");
			return 0;
		}
		i += offset;
	}

	if ( flash_write((char *)&checksum, i, sizeof(checksum))==0 ) {
		printf("Write flash current-setting checksum failed!\n");
		return 0;
	}

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////
static int flash_read(char *buf, int offset, int len)
{
	int fh;
	int ok=1;

	fh = open(FLASH_DEVICE_NAME, O_RDWR);
	if ( fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if ( read(fh, buf, len) != len)
		ok = 0;

	close(fh);

	return ok;
}


////////////////////////////////////////////////////////////////////////////////
static int flash_write(char *buf, int offset, int len)
{
	int fh;
	int ok=1;


#if 1 // EDX, RexHua for skip same data
	char *orgBuf;
	orgBuf = malloc(len);
	if (orgBuf != NULL)
	{
		flash_read( orgBuf, offset, len);
		if(!memcmp(buf, orgBuf, len))
		{
			free(orgBuf);
			printf("skip flash write!\n");
			return ok;
		}
		free(orgBuf);
	}	
#endif

	fh = open(FLASH_DEVICE_NAME, O_RDWR);

	if ( fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if ( write(fh, buf, len) != len)
		ok = 0;

	close(fh);
	sync();

	return ok;
}


///////////////////////////////////////////////////////////////////////////////
static int init_linkchain(LINKCHAIN_Tp pLinkChain, int size, int num)
{
	FILTER_Tp entry;
	int offset=sizeof(FILTER_Tp)*2;
	char *pBuf;
	int i;
	int remainder;
	remainder=size%4;
	if(remainder==0)
		pLinkChain->appendNumber=0;
	else{
		pLinkChain->appendNumber=4-remainder;
		size = (size/4+1)*4;
	}
	pBuf = calloc(num, size+offset);
//printf("size offset = %d %d\n", size, offset);

	if ( pBuf == NULL )
		return 0;

//printf("bbbb 3-3\n");
	pLinkChain->buf = pBuf;
	pLinkChain->pUsedList = NULL;
	pLinkChain->pFreeList = NULL;
	entry = (FILTER_Tp)pBuf;

//printf("bbbb 3-30 %p %p\n", pLinkChain->pFreeList, entry);
	ADD_LINK_LIST(pLinkChain->pFreeList, entry);
//printf("bbbb 3-31 %p %p\n", pLinkChain->pFreeList, entry);
	for (i=1; i<num; i++) {
		entry = (FILTER_Tp)&pBuf[i*(size+offset)];
//printf("bbbb 3-32 %p %p\n", pLinkChain->pFreeList, entry);
		ADD_LINK_LIST(pLinkChain->pFreeList, entry);
	}

//printf("bbbb 3-4\n");
	pLinkChain->size = size;
	pLinkChain->num = num;
	pLinkChain->usedNum = 0;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
static int add_linkchain(LINKCHAIN_Tp pLinkChain, char *val)
{
	FILTER_Tp entry;

	// get a free entry
	entry = pLinkChain->pFreeList;
	if (entry == NULL)
		return 0;

	if (entry->next==pLinkChain->pFreeList)
		pLinkChain->pFreeList = NULL;
	else
		pLinkChain->pFreeList = entry->next;

	REMOVE_LINK_LIST(entry);

	// copy content
	memcpy(entry->val,val, pLinkChain->size);

	// add to used list
	if (pLinkChain->pUsedList == NULL) {
		ADD_LINK_LIST(pLinkChain->pUsedList, entry);
	}
	else {
		ADD_LINK_LIST(pLinkChain->pUsedList->prev, entry);
	}
	pLinkChain->usedNum++;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
static int edit_linkchain(LINKCHAIN_Tp pLinkChain, char *val)	//Erwin add
{
	FILTER_Tp curEntry=pLinkChain->pUsedList;

	while (curEntry != NULL) {
		if ( !memcmp(curEntry->val, (unsigned char *)(((EDIT_Tp)val)->selEntry), pLinkChain->compareLen) ) {
			memcpy(curEntry->val, ((EDIT_Tp)val)->editEntry, pLinkChain->size);

			return 1;
		}
		if ( curEntry->next == pLinkChain->pUsedList )
			return 0;
		curEntry = curEntry->next;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
static int moveUp_linkchain(LINKCHAIN_Tp pLinkChain, char *val)	//Erwin add
{
	FILTER_Tp curEntry=pLinkChain->pUsedList->next;
	while (curEntry != NULL) {
		if ( !memcmp(curEntry->val, (unsigned char *)val, pLinkChain->compareLen) ) {
			if (curEntry==pLinkChain->pUsedList->next)
				pLinkChain->pUsedList=curEntry;

			curEntry->prev->prev->next = curEntry;
			curEntry->next->prev = curEntry->prev;

			curEntry->prev->next = curEntry->next;
			curEntry->next = curEntry->prev;
			curEntry->prev = curEntry->next->prev;
			curEntry->next->prev = curEntry;
			return 1;
		}
		if ( curEntry->next == pLinkChain->pUsedList )
			return 0;
		curEntry = curEntry->next;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
static int moveDown_linkchain(LINKCHAIN_Tp pLinkChain, char *val)	//Erwin add
{
	FILTER_Tp curEntry=pLinkChain->pUsedList;
	while (curEntry != NULL) {
		if ( !memcmp(curEntry->val, (unsigned char *)val, pLinkChain->compareLen) ) {
			if (curEntry==pLinkChain->pUsedList)
				pLinkChain->pUsedList=curEntry->next;

			curEntry->prev->next = curEntry->next;
			curEntry->next->next->prev = curEntry;

			curEntry->next->prev = curEntry->prev;
			curEntry->prev = curEntry->next;
			curEntry->next = curEntry->prev->next;
			curEntry->prev->next = curEntry;
			return 1;
		}
		if ( curEntry->next == pLinkChain->pUsedList->prev )
			return 0;
		curEntry = curEntry->next;
	}
}
///////////////////////////////////////////////////////////////////////////////
static int delete_linkchain(LINKCHAIN_Tp pLinkChain, char *val)
{
	FILTER_Tp curEntry=pLinkChain->pUsedList;

	while (curEntry != NULL) {

		if ( !memcmp(curEntry->val,(unsigned char *)val,pLinkChain->compareLen) ) {

				if (curEntry == pLinkChain->pUsedList) {
					if ( pLinkChain->pUsedList->next != pLinkChain->pUsedList )
						pLinkChain->pUsedList = pLinkChain->pUsedList->next;
					else
						pLinkChain->pUsedList = NULL;
				}
				REMOVE_LINK_LIST(curEntry);
				ADD_LINK_LIST(pLinkChain->pFreeList, curEntry);
				pLinkChain->usedNum--;
				return 1;
		}
		if ( curEntry->next == pLinkChain->pUsedList )
			return 0;
		curEntry = curEntry->next;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
static void delete_all_linkchain(LINKCHAIN_Tp pLinkChain)
{
	FILTER_Tp curEntry;

	if (pLinkChain->pUsedList==NULL)
		return;

	// search for matched mac address
	while (pLinkChain->pUsedList) {
		curEntry = pLinkChain->pUsedList;
		if (pLinkChain->pUsedList->next != pLinkChain->pUsedList)
			pLinkChain->pUsedList = pLinkChain->pUsedList->next;
		else
			pLinkChain->pUsedList = NULL;

		REMOVE_LINK_LIST(curEntry);
		ADD_LINK_LIST(pLinkChain->pFreeList, curEntry);
		pLinkChain->usedNum--;
	}
}

///////////////////////////////////////////////////////////////////////////////
static int get_linkchain(LINKCHAIN_Tp pLinkChain, char *val, int index)
{
	FILTER_Tp curEntry=pLinkChain->pUsedList;

	if ( curEntry == NULL || index > pLinkChain->usedNum)
 		return 0;

	while (--index > 0)
        	curEntry = curEntry->next;

	memcpy( (unsigned char *)val, curEntry->val, (pLinkChain->size-pLinkChain->appendNumber));

	return 1;
}
