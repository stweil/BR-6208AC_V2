#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/un.h>
#include <signal.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////brad add//////////////////////////////////////
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <linux/wireless.h>
////////////////////////////////////////////////////////////////////////////////brad add//////////////////////////////////////

//#include "mini_upnp.h"
#include "upnphttp.h"
//#include "upnpsoap.h"
#include "upnpreplyparse.h"
#include "minixml.h"
#include "boa.h"
#include "globals.h"
#include "apmib.h"
#include "apform.h"
#include "mibtbl.h"
#include "utility.h"
extern int isConnectPPP();
extern int get_dns(char *file, char *buffer, int mode, int index);
extern int is_ppp0_exist();
//#define DEBUG
#define DYNAMIC_ALLOC_MEM

#if 0 //unused, remove
#ifdef DEBUG
#include <syslog.h>
#define PRINT_ERR
#define DEBUG_PRINT(fmt, args...); printf(fmt, ## args);
#else
#define syslog(X, fmt, args...);
#define DEBUG_PRINT(fmt, args...);
#endif // DEBUG

#ifdef PRINT_ERR
#define DEBUG_ERR(fmt, args...); printf(fmt, ## args);
#else

#define DEBUG_ERR(fmt, args...);
#endif
#endif//unused, remove

#define DEBUG_PRINT(fmt, args...) 
#define DEBUG_ERR(fmt, args...);
#define HNAPD_PORT 9889
/* Basic authorization userid and passwd limit */
#define AUTH_MAX 64
#define IP_LEN 20
#define MAX_BUF_LEN 8192
#define MAX_DOC_LEN 200
#define UNIX_SOCKET_TIMEOUT 10
#define HNAP1_URL "http://purenetworks.com/HNAP1/"
#define FIRMWARE_VER_PATH "/tmp/fw_ver"
#define ADMIN_RELATIVE_PATH "/"
//#define UNIX_SOCK_PATH "/tmp/mysocket"
#define HNAP_CONFIG_PATH "/etc/hnapd.conf"
#define WEB_PID_FILENAME			("/var/run/webs.pid")
#define HNAP_PID_FILENAME		("/var/run/hnap.pid")
#define AUTH_REALM "TEST"
#define WAN_IF_1 ("eth1")
#define WAN_IF_2 ("ppp0")
#define _PATH_PROCNET_ROUTE	"/proc/net/route"
#define SEND_TO_IGD_PATH "/tmp/hnap_igd_send"
#define RECEIVE_FROM_IGD_PATH "/tmp/hnap_igd_rcv"

////////////////////////////////////////////////////////////////////////////////brad add//////////////////////////////////////
#define _PATH_PROCNET_DEV	"/proc/net/dev"
//#define _DHCPD_PROG_NAME	"udhcpd"

#define	E_LOG				0x2			/* Log error to log file */
#define E_L					__FILE__, __LINE__
#define READ_BUF_SIZE	50
////////////////////////////////////////////////////////////////////////////////brad add//////////////////////////////////////

/* Keep this in sync with /usr/src/linux/include/linux/route.h */
#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */

#define ERROR_POST_HEADER -1
#define ERROR_AUTH_BASIC -2
#define ROUTER_READY 1
#define ROUTER_REBOOT 2
#define ROUTER_REINIT 3
#define ROUTER_REINIT_BOA 4
#define ROUTER_REINIT_FIREWALL 5
#define ROUTER_ERROR -1

typedef struct priv_data {
#ifdef DYNAMIC_ALLOC_MEM
	char *result_str;
	char *body;
	char *tmp;
	char *tmpstr;
#else
	char result_str[MAX_BUF_LEN];
	char body[MAX_BUF_LEN];
	char tmp[2048];
	char tmpstr[2048];
#endif
	char action_name[100];
}PRIV_CTX_T, *PRIV_CTX_Tp;

PRIV_CTX_T priv;
//static mini_upnp_CTX_T hnap_ctx;
//static int auth_level;
//static char if_name[20];
//static char userid[AUTH_MAX];
static char passwd[AUTH_MAX];
static int router_status=ROUTER_READY;
static int igd_enabled;
static char ModelDescription[MAX_DOC_LEN];
static char ModelURL[MAX_DOC_LEN];
int isSetGetToggle=1;
int isSetIndex_id=0;
#if defined(SUPPORT_HNAP)
struct _soapMethods_hnap {
	char * methodName;
	void (*methodImpl)(char *h, request *wp);
};
#ifdef DYNAMIC_ALLOC_MEM
int Str_Alloc_Buffer(void)
{

	priv.result_str = (char *)malloc(MAX_BUF_LEN);
	priv.body = (char *)malloc(MAX_BUF_LEN);
	priv.tmp = (char *)malloc(2048);
	priv.tmpstr = (char *)malloc(2048);
	if (priv.result_str == NULL || priv.body == NULL || priv.tmp == NULL || 
		priv.tmpstr == NULL) {
		if (priv.result_str)
			free(priv.result_str);
		if (priv.body)
			free(priv.body);
		if (priv.tmp)
			free(priv.tmp);
		if (priv.tmpstr)
			free(priv.tmpstr);

		return -1;
	}
	return 0;
}
#endif

static void xml_sprintf(const char *tag_name, const char *value, char *buf)
{
	if (value)
		sprintf(buf, "<%s>%s</%s>", tag_name, value, tag_name);
	else
		sprintf(buf, "<%s></%s>", tag_name, tag_name);
}

static void xml_sprintf_strcat(const char *tag_name, const char *value, char *buf, char *out)
{
	if (value)
		sprintf(buf, "<%s>%s</%s>", tag_name, value, tag_name);
	else
		sprintf(buf, "<%s></%s>", tag_name, tag_name);
	strcat(out, buf);
}

static void xml_sprintf_int_strcat(const char *tag_name, const int value, char *buf, char *out)
{
	sprintf(buf, "<%s>%d</%s>", tag_name, value, tag_name);
	strcat(out, buf);
}

static void xml_add_tag(const char *tag_name, char *value, char *buf)
{
	char tmp[256];
	unsigned int len;

	len = 2*strlen(tag_name) + strlen(value) + 5;
	if (len >= 256) {
		printf("Error : buffer of xml_add_tag is too small!\n");
		return;
	}
	len = sprintf(tmp, "<%s>%s</%s>", tag_name, value, tag_name);
	strcat(buf, tmp);
}
static void SendResponse(char * h, const unsigned char ErrorOccurred, const unsigned char NeedReinitMib, request *wp)
{
	int TotalLen;
	int Total_Content=0;
	if (ErrorOccurred) {
		if (NeedReinitMib) {
			if (!apmib_reinit())
				printf("%s : Reinitializing MIB failed!\n", priv.action_name);
		}
		if(ErrorOccurred==1)
			sprintf(priv.tmpstr, "<%sResult>ERROR</%sResult>", priv.action_name, priv.action_name);
		else if(ErrorOccurred==6)
			sprintf(priv.tmpstr, "<%sResult>ERROR_ILLEGAL_KEY_VALUE</%sResult>", priv.action_name, priv.action_name);
		else if(ErrorOccurred==7)
			sprintf(priv.tmpstr, "<%sResult>ERROR_KEY_RENEWAL_BAD_VALUE</%sResult>", priv.action_name, priv.action_name);
		else if(ErrorOccurred==8)
			sprintf(priv.tmpstr, "<%sResult>ERROR_BAD_RADIUS_VALUEs</%sResult>", priv.action_name, priv.action_name);
		else if(ErrorOccurred==4)
			sprintf(priv.tmpstr, "<%sResult>ERROR_TYPE_NOT_SUPPORTED</%sResult>", priv.action_name, priv.action_name);
		else
			sprintf(priv.tmpstr, "<%sResult>ERROR</%sResult>", priv.action_name, priv.action_name);
		strcpy(priv.body,priv.tmpstr);
	}
	
	TotalLen = sprintf(priv.result_str, 
      		"<%sResponse xmlns=\"%s\">\n%s\n</%sResponse>", priv.action_name,
           	HNAP1_URL,
           	priv.body,
           	priv.action_name);
	//printf("total length=%d, and content=%s\n", TotalLen,priv.result_str);
	
	Total_Content = wp->buffer_end;
	req_write(wp, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	req_write(wp, "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\n");
	req_write(wp, "<soap:Body>\n");
	req_write(wp, priv.result_str);
	req_write(wp, "</soap:Body>\n");
	req_write(wp, "</soap:Envelope>\n");
	Total_Content = wp->buffer_end - Total_Content;
	wp->filesize = Total_Content+1;

	
	
	
	
	
#ifdef DYNAMIC_ALLOC_MEM
	if (priv.result_str){
		free(priv.result_str);
	}
	if (priv.body){
		free(priv.body);
	}
	if (priv.tmp){
		free(priv.tmp);
	}
	if (priv.tmpstr){
		free(priv.tmpstr);
	}
#endif
}
static void SetWanType(const char *str, int *wan_type,
							int *static_type)
{
	DEBUG_PRINT("<<------str [%s]------>>\n", str);
	 if (strcmp(str, "DHCP") == 0) {
		*wan_type = DHCP_CLIENT;
		*static_type = 0;
		apmib_set(MIB_WAN_IP_DYNAMIC, (void *)static_type);
	}
	else if (strcmp(str, "DHCPPPPoE") == 0) {
		*wan_type = PPPOE;
		*static_type = 0;
		apmib_set(MIB_PPPOE_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "DynamicL2TP") == 0) {
		*wan_type = L2TP;
		*static_type = 0;
		apmib_set(MIB_L2TP_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "DynamicPPTP") == 0) {
		*wan_type = PPTP;
		*static_type = 0;
		apmib_set(MIB_PPTP_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "Static") == 0) {
		*wan_type = DHCP_DISABLED;
		*static_type = 1;
		apmib_set(MIB_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "StaticL2TP") == 0) {
		*wan_type = L2TP;
		*static_type = 1;
		apmib_set(MIB_L2TP_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "StaticPPPoE") == 0) {
		*wan_type = PPPOE;
		*static_type = 1;
		apmib_set(MIB_PPPOE_WAN_IP_DYNAMIC, (void *)static_type);
	}else if (strcmp(str, "StaticPPTP") == 0) {
		*wan_type = PPTP;
		*static_type = 1;
		apmib_set(MIB_PPTP_WAN_IP_DYNAMIC, (void *)static_type);
	}
	else {
		DEBUG_PRINT("<<------UnKnown wan type [%s]------>>\n", str);
		*wan_type = DHCP_DISABLED;
		*static_type = 1;
		apmib_set(MIB_WAN_IP_DYNAMIC, (void *)static_type);
	}
	DEBUG_PRINT("<<------wan_type [%d]------>>\n", *wan_type);
	DEBUG_PRINT("<<------static_type [%d]------>>\n", *static_type);
}

static void GetWanGatewayIP(char *buf, const int wan_type,
		const int static_type, const char *name)
{
	struct in_addr ia_val;
	int found;

	if (static_type && wan_type == PPTP) {
		apmib_get(MIB_PPTP_GATEWAY, (void *)&ia_val);
		strcpy(buf, inet_ntoa(ia_val));
	}
	else if (static_type && wan_type == PPPOE) {
		apmib_get(MIB_WAN_DEFAULT_GATEWAY, (void *)&ia_val);
		strcpy(buf, inet_ntoa(ia_val));
	}
	else if (static_type && wan_type == L2TP) {
		apmib_get(MIB_L2TP_GATEWAY, (void *)&ia_val);
		strcpy(buf, inet_ntoa(ia_val));
	}
	else if (wan_type == DHCP_DISABLED) {
		apmib_get(MIB_WAN_DEFAULT_GATEWAY, (void *)&ia_val);
		strcpy(buf, inet_ntoa(ia_val));
	}
	else if (!static_type && (wan_type == PPPOE || wan_type == PPTP ||
		wan_type == L2TP)) {
		if (is_wan_connected()==LINK_UP) {
			found = getDefaultRoute( (char *)name, &ia_val );
			if (found)
				strcpy(buf, inet_ntoa(ia_val));
			else
				strcpy(buf, "0.0.0.0");
		}
		else
			strcpy(buf, "0.0.0.0");
	}
	else {
		found = getDefaultRoute( (char *)name, &ia_val );
		if (found)
			strcpy(buf, inet_ntoa(ia_val));
		else
			strcpy(buf, "0.0.0.0");
	}
}

static void GetWANMAC(char *buf)
{
	unsigned char key[20];

#if 0
	if ( getInAddr((char *)name, HW_ADDR, (void *)&hwaddr ) ) {
		pMacAddr = hwaddr.sa_data;
		sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", pMacAddr[0], pMacAddr[1],
			pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
	}
	else
	{
		strcpy(buf, "00:00:00:00:00:00");
	}
#endif
	apmib_get(MIB_WAN_MAC_ADDR,(void *)key);

	if (key[0]==0 && key[1]==0 && key[2]==0 && key[3]==0 && key[4]==0 && key[5]==0)
		apmib_get(MIB_HW_NIC1_ADDR, (void *)key);
	sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", key[0], key[1],
			key[2], key[3], key[4], key[5]);
}

static int GetConnectType(const int wan_type)
{
	int connect_type;
	
	switch (wan_type)
	{
		case PPPOE:
			apmib_get(MIB_PPP_CONNECT_TYPE, (void *)&connect_type);
			if (connect_type == CONTINUOUS)
				return 1;
			else
				return 0;
		case PPTP:
			apmib_get(MIB_PPTP_CONNECTION_TYPE, (void *)&connect_type);
			if (connect_type == CONTINUOUS)
				return 1;
			else
				return 0;
		case L2TP:
			apmib_get(MIB_L2TP_CONNECTION_TYPE, (void *)&connect_type);
			if (connect_type == CONTINUOUS)
				return 1;
			else
				return 0;
		//case BIGPOND:
			//return 1;
		default:
			return 0;
	}
}

static void GetSubnetMask(char *buf, 
		const int wan_type, const int static_type,
		const char *name)
{
	struct in_addr ia_val;
	int found;

	if (wan_type == DHCP_CLIENT || wan_type == BIGPOND) {
		found = getInAddr((char *) name, SUBNET_MASK, &ia_val );
		if (found)
			strcpy(buf, inet_ntoa(ia_val));
		else
			strcpy(buf, "0.0.0.0");
		return;
	}
	else if (!static_type && (wan_type == PPPOE || wan_type == PPTP ||
		wan_type == L2TP)) {
		if (is_wan_connected()==LINK_UP) {
			found = getInAddr((char *) name, SUBNET_MASK, &ia_val );
			if (found)
				strcpy(buf, inet_ntoa(ia_val));
			else
				strcpy(buf, "0.0.0.0");
			return;
		}
		else {
			strcpy(buf, "0.0.0.0");
			return;
		}
	}
	
	switch (wan_type)
	{
		case PPPOE:
			apmib_get(MIB_WAN_SUBNET_MASK, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));	
			return;
		case L2TP:
			apmib_get(MIB_L2TP_SUBNET_MASK, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));	
			return;
		case PPTP:
			apmib_get(MIB_PPTP_SUBNET_MASK, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));	
			return;
		case DHCP_DISABLED:
			apmib_get(MIB_WAN_SUBNET_MASK, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));	
			return;
		default:
			strcpy(buf, "0.0.0.0");
			return;
	}
}

static void GetWANIP(char *buf, const char *name)
{
	IPCon ipcon;
	char *ip;
	
	if (buf == NULL)
		return;

	ipcon = IPCon_New((char *)name);
	if (ipcon == NULL) {
		strcpy(buf, "0.0.0.0");
		return;
	}

	ip = IPCon_GetIpAddrByStr(ipcon);
	if (ip == NULL) {
		strcpy(buf, "0.0.0.0");
		IPCon_Destroy(ipcon);
		return;
	}
	strcpy(buf, ip);  
	IPCon_Destroy(ipcon);
}

static void GetServiceName(char *buf, const int wan_type)
{
	struct in_addr ia_val;
	
	if (buf == NULL)
		return;

	switch (wan_type)
	{
		case PPPOE:
			apmib_get(MIB_PPP_SERVICE, (void *)buf);
			return;
		case L2TP:
			apmib_get(MIB_L2TP_SERVER_IP, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));
			return;
		case PPTP:
			apmib_get(MIB_PPTP_SERVER_IP, (void *)&ia_val);
			strcpy(buf, inet_ntoa(ia_val));
			return;
		case DHCP_CLIENT:
		case DHCP_DISABLED:
		default:
			buf[0] = 0;
			return;
	}
}


static void GetDNS(char *buf_in)
{
	struct in_addr intaddr;
	DHCP_T dhcp;
	unsigned char buffer[500];
	int dnsMode;
	char *pDNS_filename;
	char *iface=NULL;
	unsigned int set_default=0;

	buf_in[0] = 0;
	buffer[0] = 0;
	apmib_get( MIB_WAN_DHCP, (void *)&dhcp);
	apmib_get( MIB_WAN_DNS_MODE, (void *)&dnsMode);

	if (dhcp == PPPOE || dhcp == PPTP || dhcp ==L2TP) {
		if (!isConnectPPP())
			iface = NULL;
		else
			iface = WAN_IF_2;
	}
	else
		iface = WAN_IF_1;
	if (iface && getInAddr(iface, IP_ADDR, (void *)&intaddr)) {
		if (dnsMode == DNS_AUTO &&  dhcp != DHCP_DISABLED  ) {			
			if ( dhcp == PPPOE || dhcp == PPTP || dhcp == L2TP) 	
				pDNS_filename = "/etc/ppp/resolv.conf";
			else
				pDNS_filename = "/etc/udhcpc/resolv.conf";
			get_dns(pDNS_filename, buf_in, dhcp, 0);
		}
		else {
			set_default = 1;
		}
	}
	else {	
		set_default = 1;
	}

	if (set_default) {
		apmib_get( MIB_WAN_DNS1,  (void *)&intaddr);
		if (!memcmp(&intaddr, "\x0\x0\x0\x0", 4)){
			xml_sprintf_strcat("Primary", NULL, buffer, buf_in);
		}else
			xml_sprintf_strcat("Primary", inet_ntoa(intaddr), buffer, buf_in);
		apmib_get( MIB_WAN_DNS2,  (void *)&intaddr);
		if (!memcmp(&intaddr, "\x0\x0\x0\x0", 4)){
			xml_sprintf_strcat("Secondary", NULL, buffer, buf_in);
		}else
			xml_sprintf_strcat("Secondary", inet_ntoa(intaddr), buffer, buf_in);
	}
}

static unsigned int GetMtu(const int wan_type)
{
	int mtu=0;

	switch (wan_type)
	{
		case DHCP_CLIENT:
			apmib_get(MIB_DHCP_MTU_SIZE, (void *)&mtu);
			break;
		case PPPOE:
			apmib_get(MIB_PPP_MTU_SIZE, (void *)&mtu);
			break;
		case L2TP:
			apmib_get(MIB_L2TP_MTU_SIZE, (void *)&mtu);
			break;
		case PPTP:
			apmib_get(MIB_PPTP_MTU_SIZE, (void *)&mtu);
			break;
		case DHCP_DISABLED:
			apmib_get(MIB_FIXED_IP_MTU_SIZE, (void *)&mtu);
			break;
		default:
			break;
	}
	return mtu;
}

static char *GetWanType(const int wan_type, int *static_type)
{	
	switch (wan_type)
	{
		case DHCP_CLIENT:
			apmib_get(MIB_WAN_IP_DYNAMIC, (void *)static_type);
			return "DHCP";
		case PPPOE:
			apmib_get(MIB_PPPOE_WAN_IP_DYNAMIC, (void *)static_type);
			if (!(*static_type))
				return "DHCPPPPoE";
			else
				return "StaticPPPoE";
		case L2TP:
			apmib_get(MIB_L2TP_WAN_IP_DYNAMIC, (void *)static_type);
			if (!(*static_type))
				return "DynamicL2TP";
			else
				return "StaticL2TP";
		case PPTP:
			apmib_get(MIB_PPTP_WAN_IP_DYNAMIC, (void *)static_type);
			if (!(*static_type))
				return "DynamicPPTP";
			else
				return "StaticPPTP";
		case DHCP_DISABLED:
			apmib_get(MIB_WAN_IP_DYNAMIC, (void *)static_type);
			return "Static";
		default:
			return NULL;
	}
}

static void GetWanSettingsResponse(char *h, request *wp)
{
	char *p;
	unsigned char IsErrorOccur=0;
	int idle_time;
	int wan_type;
	char *name;
	int static_type;
	struct in_addr ia_val;
	int opmode=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif
	strcpy(priv.action_name, "GetWanSettings");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWanSettingsResult>OK</GetWanSettingsResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}
	apmib_get( MIB_OP_MODE, (void *)&opmode);
	apmib_get(MIB_WAN_DHCP, (void *)&wan_type);
	DEBUG_PRINT("MIB_WAN_DHCP [%d]\n", wan_type);
	switch (wan_type)
	{
		case PPPOE:
		case L2TP:
		case PPTP:
			name = WAN_IF_2;
			break;
		case DHCP_CLIENT:
		case DHCP_DISABLED:
		default:
			name = WAN_IF_1;
			break;
	}
	if(opmode == WISP_MODE){
		if(wan_type==DHCP_CLIENT || wan_type==DHCP_DISABLED){
			name = "wlan0";
		}
	}
	p = GetWanType(wan_type, &static_type);
	if (p == NULL) {
		IsErrorOccur = 1;
		goto SendRes;
	}
	strcpy( priv.tmpstr, p);
	xml_sprintf_strcat("Type", priv.tmpstr, priv.tmp, priv.body);

	if (wan_type == PPPOE) {
		apmib_get(MIB_PPP_USER, (void *) priv.tmpstr);
		xml_sprintf_strcat("Username", priv.tmpstr, priv.tmp, priv.body);
		
		apmib_get(MIB_PPP_PASSWORD, (void *)priv.tmpstr);
		xml_sprintf_strcat("Password", priv.tmpstr, priv.tmp, priv.body);
	}
	else if (wan_type == PPTP) {
		apmib_get(MIB_PPTP_USER, (void *) priv.tmpstr);
		xml_sprintf_strcat("Username", priv.tmpstr, priv.tmp, priv.body);
		
		apmib_get(MIB_PPTP_PASSWORD, (void *)priv.tmpstr);
		xml_sprintf_strcat("Password", priv.tmpstr, priv.tmp, priv.body);
	}
	else if (wan_type == L2TP) {
		apmib_get(MIB_L2TP_USER, (void *) priv.tmpstr);
		xml_sprintf_strcat("Username", priv.tmpstr, priv.tmp, priv.body);
		
		apmib_get(MIB_L2TP_PASSWORD, (void *)priv.tmpstr);
		xml_sprintf_strcat("Password", priv.tmpstr, priv.tmp, priv.body);
	}
	else {
		xml_sprintf_strcat("Username", NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("Password", NULL, priv.tmp, priv.body);
	}

	if (wan_type == PPPOE) {
		apmib_get(MIB_PPP_IDLE_TIME, (void *)&idle_time);
		xml_sprintf_int_strcat("MaxIdleTime", idle_time, priv.tmp, priv.body);
	}
	else if (wan_type == PPTP) {
		apmib_get(MIB_PPTP_IDEL_TIME, (void *)&idle_time);
		xml_sprintf_int_strcat("MaxIdleTime", idle_time, priv.tmp, priv.body);
	}
	else if (wan_type == L2TP) {
		apmib_get(MIB_L2TP_IDEL_TIME, (void *)&idle_time);
		xml_sprintf_int_strcat("MaxIdleTime", idle_time, priv.tmp, priv.body);
	}
	else
		xml_sprintf_int_strcat("MaxIdleTime", 0, priv.tmp, priv.body);

	xml_sprintf_int_strcat("MTU", GetMtu(wan_type), priv.tmp, priv.body);

	GetServiceName(priv.tmpstr, wan_type);
	if (priv.tmpstr[0])
		xml_sprintf("ServiceName", priv.tmpstr, priv.tmp);
	else
		xml_sprintf("ServiceName", NULL, priv.tmp);
	strcat(priv.body, priv.tmp);

	if (GetConnectType(wan_type))
		xml_sprintf("AutoReconnect", "true", priv.tmp);
	else
		xml_sprintf("AutoReconnect", "false", priv.tmp);
	strcat(priv.body, priv.tmp);

	if (wan_type == DHCP_DISABLED) {
		apmib_get(MIB_WAN_IP_ADDR, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
	}
	else if (wan_type == PPPOE && static_type == 1) {
		apmib_get(MIB_PPPOE_IP, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
	}
	else if (wan_type == PPTP && static_type == 1) {
		apmib_get(MIB_PPTP_IP, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
	}
	else if (wan_type == L2TP && static_type == 1) {
		apmib_get(MIB_L2TP_IP, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
	}
	else if (!static_type && (wan_type == PPPOE ||wan_type == PPTP ||
		wan_type == L2TP)) {
		if (is_wan_connected()==LINK_UP)
			GetWANIP(priv.tmpstr, name);
		else
			strcpy(priv.tmpstr, "0.0.0.0");
	}
	else
		GetWANIP(priv.tmpstr, name);
	xml_sprintf_strcat("IPAddress", priv.tmpstr, priv.tmp, priv.body);

	GetSubnetMask(priv.tmpstr, wan_type, static_type, name);
	xml_sprintf_strcat("SubnetMask", priv.tmpstr, priv.tmp, priv.body);

	GetWanGatewayIP( priv.tmpstr, wan_type, static_type, (const char *)name);
	xml_sprintf_strcat("Gateway", priv.tmpstr, priv.tmp, priv.body);

	GetDNS(priv.tmpstr);
	xml_sprintf_strcat("DNS", priv.tmpstr, priv.tmp, priv.body);

	GetWANMAC(priv.tmpstr);
	xml_sprintf_strcat("MacAddress", priv.tmpstr, priv.tmp, priv.body); 
	
SendRes: 
	SendResponse(h, IsErrorOccur, 0, wp);
}

static void SetWanSettingsResponse(char *h, request *wp)
{ 
	char *element; 
	struct NameValueParserData data;
	char *p_NewInMessage;
	char clear_mib[1];
	int wan_type=0;
	int static_type=1;
	unsigned char IsErrorOccur=0;
	int ip;
	unsigned char NeedReinitMib=0;
	int dnsMode;
	
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif
	
	clear_mib[0] = 0;
	strcpy(priv.action_name, "SetWanSettings");

	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "Type");
	if (p_NewInMessage) {
		SetWanType(p_NewInMessage, &wan_type, &static_type);
	}
	apmib_set(MIB_WAN_DHCP, (void *)&wan_type);
	DEBUG_PRINT("MIB_WAN_DHCP [%d]\n", wan_type);

	NeedReinitMib = 1;
	
	if (wan_type == PPPOE) {
		p_NewInMessage = GetValueFromNameValueList(&data, "Username");
		if (p_NewInMessage) {
			apmib_set(MIB_PPP_USER, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_USER [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "Password");
		if (p_NewInMessage) {
			apmib_set(MIB_PPP_PASSWORD, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_PASSWORD [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "MaxIdleTime");
		if (p_NewInMessage) {
			int idle_time = atoi(p_NewInMessage);
			apmib_set(MIB_PPP_IDLE_TIME, (void *)&idle_time);
			DEBUG_PRINT("MIB_PPP_IDLE_TIME [%d]\n", idle_time);
		}
	}
	else if (wan_type == PPTP) {
		p_NewInMessage = GetValueFromNameValueList(&data, "Username");
		if (p_NewInMessage) {
			apmib_set(MIB_PPTP_USER, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_USER [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "Password");
		if (p_NewInMessage) {
			apmib_set(MIB_PPTP_PASSWORD, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_PASSWORD [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "MaxIdleTime");
		if (p_NewInMessage) {
			int idle_time = atoi(p_NewInMessage);
			apmib_set(MIB_PPTP_IDEL_TIME, (void *)&idle_time);
			DEBUG_PRINT("MIB_PPP_IDLE_TIME [%d]\n", idle_time);
		}
	}
	else if (wan_type == L2TP) {
		p_NewInMessage = GetValueFromNameValueList(&data, "Username");
		if (p_NewInMessage) {
			apmib_set(MIB_L2TP_USER, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_USER [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "Password");
		if (p_NewInMessage) {
			apmib_set(MIB_L2TP_PASSWORD, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_PASSWORD [%s]\n", p_NewInMessage);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "MaxIdleTime");
		if (p_NewInMessage) {
			int idle_time = atoi(p_NewInMessage);
			apmib_set(MIB_L2TP_IDEL_TIME, (void *)&idle_time);
			DEBUG_PRINT("MIB_PPP_IDLE_TIME [%d]\n", idle_time);
		}
	}
	

	p_NewInMessage = GetValueFromNameValueList(&data, "ServiceName");
	 if (wan_type == PPPOE) {
		if (p_NewInMessage) {
			apmib_set(MIB_PPP_SERVICE, (void *)p_NewInMessage);
			DEBUG_PRINT("MIB_PPP_SERVICE [%s]\n", p_NewInMessage);
		}
		else {
			apmib_set(MIB_PPP_SERVICE, (void *)clear_mib);
			DEBUG_PRINT("MIB_PPP_SERVICE []\n");
		}
	}
	else if (wan_type == PPTP) {
		if (p_NewInMessage) {
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 1;
				goto Send_Res;
			}
			apmib_set(MIB_PPTP_SERVER_IP, (void *)&ip);
			DEBUG_PRINT("MIB_PPTP_SERVER_IP [%s]\n", p_NewInMessage);
		}
	}
	else if (wan_type == L2TP) {
		if (p_NewInMessage) {
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 1;
				goto Send_Res;
			}
			apmib_set(MIB_L2TP_SERVER_IP, (void *)&ip);
			DEBUG_PRINT("MIB_L2TP_SERVER_IP [%s]\n", p_NewInMessage);
		}
	}
	
	p_NewInMessage = GetValueFromNameValueList(&data, "AutoReconnect");
	if (p_NewInMessage) {
		int connect_type;
		if (strcmp(p_NewInMessage, "true") == 0)
			connect_type = CONTINUOUS;
		else
			connect_type = CONNECT_ON_DEMAND;
		
		if (wan_type == PPPOE) {
			apmib_set(MIB_PPP_CONNECT_TYPE, (void *)&connect_type);
			DEBUG_PRINT("MIB_PPP_CONNECT_TYPE [%d]\n", connect_type);
		}
		else if (wan_type == PPTP) {
			apmib_set(MIB_PPTP_CONNECTION_TYPE, (void *)&connect_type);
			DEBUG_PRINT("MIB_PPTP_CONNECT_TYPE [%d]\n", connect_type);
		}
		else if (wan_type == L2TP) {
			apmib_set(MIB_L2TP_CONNECTION_TYPE, (void *)&connect_type);
			DEBUG_PRINT("MIB_PPTP_CONNECT_TYPE [%d]\n", connect_type);
		}
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "IPAddress");
	if (p_NewInMessage) {
		if ((ip = inet_addr(p_NewInMessage)) == -1) {
			IsErrorOccur = 1;
			goto Send_Res;
		}
		if (wan_type == DHCP_DISABLED) {
			apmib_set(MIB_WAN_IP_ADDR, (void *)&ip);
			DEBUG_PRINT("MIB_WAN_IP_ADDR [%s]\n", p_NewInMessage);
		}
		else if (wan_type == PPPOE && static_type == 1) {
			apmib_set(MIB_PPPOE_IP, (void *)&ip);
			DEBUG_PRINT("MIB_PPPOE_IP [%s]\n", p_NewInMessage);
		}
		else if (wan_type == PPTP && static_type == 1) {
			apmib_set(MIB_PPTP_IP, (void *)&ip);
			DEBUG_PRINT("MIB_PPTP_IP [%s]\n", p_NewInMessage);
		}
		else if (wan_type == L2TP && static_type == 1) {
			apmib_set(MIB_L2TP_IP, (void *)&ip);
			DEBUG_PRINT("MIB_L2TP_IP [%s]\n", p_NewInMessage);
		}
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "SubnetMask");
	if (p_NewInMessage) {
		if ((ip = inet_addr(p_NewInMessage)) == -1) {
			IsErrorOccur = 1;
			goto Send_Res;
		}
	}
	else
		ip = 0;
	if (wan_type == DHCP_DISABLED ||
		(wan_type == PPPOE && static_type)) {
		apmib_set(MIB_WAN_SUBNET_MASK, (void *)&ip);
		DEBUG_PRINT("MIB_WAN_SUBNET_MASK[%s]\n", p_NewInMessage);
	}
	else if (wan_type == PPTP && static_type) {
		apmib_set(MIB_PPTP_SUBNET_MASK, (void *)&ip);
		DEBUG_PRINT("MIB_PPTP_SUBNET_MASK[%s]\n", p_NewInMessage);
	}
	else if (wan_type == L2TP && static_type) {
		apmib_set(MIB_L2TP_SUBNET_MASK, (void *)&ip);
		DEBUG_PRINT("MIB_L2TP_SUBNET_MASK[%s]\n", p_NewInMessage);
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "Gateway");
	if (p_NewInMessage) {
		if ((ip = inet_addr(p_NewInMessage)) == -1) {
			IsErrorOccur = 1;
			goto Send_Res;
		}
	}
	else
		ip = 0;
	if (wan_type == DHCP_DISABLED) {
		apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&ip);
		DEBUG_PRINT("MIB_WAN_DEFAULT_GATEWAY[%s]\n", p_NewInMessage);
	}
	else if (static_type && wan_type == PPTP) {
		apmib_set(MIB_PPTP_GATEWAY, (void *)&ip);
		DEBUG_PRINT("MIB_PPTP_GATEWAY[%s]\n", p_NewInMessage);
	}
	else if (static_type && wan_type == L2TP) {
		apmib_set(MIB_L2TP_GATEWAY, (void *)&ip);
		DEBUG_PRINT("MIB_L2TP_GATEWAY[%s]\n", p_NewInMessage);
	}
	else if (static_type && wan_type == PPPOE) {
		apmib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&ip);
		DEBUG_PRINT("MIB_WAN_DEFAULT_GATEWAY[%s]\n", p_NewInMessage);
	}
	element = mini_UPnPGetFirstElement(h, strlen(h),"Primary", 7);
	if (element) {
		//printf("<<-------Primary [%s] -------->>\n", element);
		if ((ip = inet_addr(element)) == -1) {
			free(element);
			IsErrorOccur = 1;
			goto Send_Res;
		}
		if (ip != 0)
			dnsMode = DNS_MANUAL;
		else
			dnsMode = DNS_AUTO;
	}
	else {
		//printf("<<-------Primary [] -------->>\n");
		ip = 0;
		dnsMode = DNS_AUTO;
	}
	apmib_set( MIB_WAN_DNS1,  (void *)&ip);
	DEBUG_PRINT("MIB_WAN_DNS1 [%s]\n", element);
	apmib_set( MIB_WAN_DNS_MODE, (void *)&dnsMode);
	if (element)
		free(element);

	element = mini_UPnPGetFirstElement(h, strlen(h),"Secondary", 9);
	if (element) {
		//printf("<<-------Secondary [%s] -------->>\n", element);
		if ((ip = inet_addr(element)) == -1) {
			free(element);
			IsErrorOccur = 1;
			goto Send_Res;
		}
	}
	else {
		//printf("<<-------Secondary [] -------->>\n");
		ip = 0;
	}
	apmib_set( MIB_WAN_DNS2,  (void *)&ip);
	DEBUG_PRINT("MIB_WAN_DNS2 [%s]\n", element);
	if (element)
		free(element);

	p_NewInMessage = GetValueFromNameValueList(&data, "MacAddress");
	if (p_NewInMessage) {
		char key[20];

		if (!string_to_hex(p_NewInMessage, key, 12)) {
			IsErrorOccur = 1;
			goto Send_Res;
		}
		else {
		        apmib_set(MIB_WAN_MAC_ADDR,(void *)key);
		        DEBUG_PRINT("MIB_WAN_MAC_ADDR [%s]\n", p_NewInMessage);
		}
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "MTU");
	if (p_NewInMessage) {
		int mtu = atoi(p_NewInMessage);
		if(mtu>128 && mtu<=1500){
		switch (wan_type)
		{
			case DHCP_CLIENT:
				apmib_set(MIB_DHCP_MTU_SIZE, (void *)&mtu);
				DEBUG_PRINT("MIB_DHCP_MTU_SIZE [%d]\n", mtu);
				break;
			case PPPOE:
				apmib_set(MIB_PPP_MTU_SIZE, (void *)&mtu);
				DEBUG_PRINT("MIB_PPP_MTU_SIZE [%d]\n", mtu);
				break;
			case L2TP:
				apmib_set(MIB_L2TP_MTU_SIZE, (void *)&mtu);
				DEBUG_PRINT("MIB_L2TP_MTU_SIZE [%d]\n", mtu);
				break;
			case PPTP:
				apmib_set(MIB_PPTP_MTU_SIZE, (void *)&mtu);
				DEBUG_PRINT("MIB_PPTP_MTU_SIZE [%d]\n", mtu);
				break;
			case DHCP_DISABLED:
				apmib_set(MIB_FIXED_IP_MTU_SIZE, (void *)&mtu);
				DEBUG_PRINT("MIB_FIXED_IP_MTU_SIZE [%d]\n", mtu);
				break;
			default:
				break;
		}
	}
	}
	
	if (router_status == ROUTER_READY) {
		//router_status = ROUTER_REINIT;
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetWanSettingsResult>REBOOT</SetWanSettingsResult>");
	}
	else {
		IsErrorOccur = 1;
	}

Send_Res:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}


static void IsDeviceReadyResponse(char *h, request *wp)
{
	char *result_str;
	char *body;
	int TotalLen;
	int Total_Content=0;
#if 0
	//printf("<<------h->IsDeviceReadyResponse==> req_buf [%s]------>>\n", h->req_buf);
	if (checkPostHeader(h->req_buf) < 0) {
		SendError(h, 400, "Bad Request", (char *)0, "Can't parse request.");
		return;
	}

	if (auth_check(h->req_buf) < 0) {
		SendError(h, 401, "Unauthorized", AUTH_REALM, "Authorization required.");
		return;
	}
#endif
	result_str = (char *)malloc(1024);
	if (result_str == NULL) {
		return;
	}
	body = (char *)malloc(512);
	if (body == NULL) {
		free(result_str);
		return;
	}
	
	if (router_status == ROUTER_READY)
		strcpy(body,"<IsDeviceReadyResult>OK</IsDeviceReadyResult>");
	else
		strcpy(body,"<IsDeviceReadyResult>ERROR</IsDeviceReadyResult>");
	
      	TotalLen = sprintf(result_str, 
      		"<%sResponse xmlns=\"%s\">\n%s\n</%sResponse>", "IsDeviceReady",
           	HNAP1_URL,
           	body,
           	"IsDeviceReady");
           	
	Total_Content = wp->buffer_end;
	req_write(wp, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	req_write(wp, "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\n");
	req_write(wp, "<soap:Body>\n");
	req_write(wp, result_str);
	req_write(wp, "</soap:Body>\n");
	req_write(wp, "</soap:Envelope>\n");
	Total_Content = wp->buffer_end - Total_Content;
	wp->filesize = Total_Content+1;
	//printf("total size=%d\n", wp->filesize);
	isSetGetToggle=1;
	free(result_str);
	free(body);
}


static void GetDeviceSettingsResponse(char *h, request *wp)
{
	FILE *fp;
	unsigned char IsErrorOccur=0;
	char local_buf_1[40];
	char local_buf_2[60];
	 extern unsigned char *hwVersion;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif		
	strcpy(priv.action_name, "GetDeviceSettings");
	
	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetDeviceSettingsResult>OK</GetDeviceSettingsResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}

	strcpy(priv.tmp,"<Type>GatewayWithWiFi</Type>");
	strcat(priv.body, priv.tmp);

	apmib_get(MIB_DOMAIN_NAME, (void *)priv.tmpstr);
	xml_sprintf_strcat("DeviceName", priv.tmpstr, priv.tmp, priv.body);

	apmib_get(MIB_VENDOR_NAME, (void *)priv.tmpstr);
	xml_sprintf_strcat("VendorName", priv.tmpstr, priv.tmp, priv.body);

	xml_sprintf_strcat("ModelDescription", ModelDescription, priv.tmp, priv.body);

	//apmib_get(MIB_MODEL_NAME, (void *) priv.tmpstr);
	//xml_sprintf_strcat("ModelName", priv.tmpstr, priv.tmp, priv.body);
//use model number for ModelName tag, since the hnap client use this as a key (default is DIR-615);


	memset(local_buf_1, 0x00, 40);
	memset(local_buf_2, 0x00, 60);
	apmib_get(MIB_MODEL_NUM, (void *) local_buf_1);
	sprintf(local_buf_2, "<ModelName>%s rev %s</ModelName>",local_buf_1, hwVersion);  
	strcpy(priv.tmp,local_buf_2);
	strcat(priv.body, priv.tmp);




	fp = fopen(FIRMWARE_VER_PATH, "r");
	if (!fp) {
		printf("Could not open file [%s]\n", FIRMWARE_VER_PATH);
		IsErrorOccur = 1;
           	goto SendRes;
	}
	fgets(priv.tmpstr, 100, fp);
	fclose(fp);
	char *p;
	p = strchr(priv.tmpstr, '\n');
	if (p)
		*p = 0;
	p = strchr(priv.tmpstr, '\r');
	if (p)
		*p = 0;
	xml_sprintf_strcat("FirmwareVersion", priv.tmpstr, priv.tmp, priv.body);

	xml_sprintf_strcat("PresentationURL", ADMIN_RELATIVE_PATH, priv.tmp, priv.body);
	priv.tmpstr[0] = 0;
	xml_add_tag("string", HNAP1_URL"GetDeviceSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetDeviceSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWanSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetWanSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"IsDeviceReady", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWanStatus", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWLanSettings24", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetWLanSettings24", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWLanSecurity", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetWLanSecurity", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"RenewWanConnection", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"Reboot", priv.tmpstr);
	if (igd_enabled) {
		xml_add_tag("string", HNAP1_URL"AddPortMapping", priv.tmpstr);
		xml_add_tag("string", HNAP1_URL"DeletePortMapping", priv.tmpstr);
		xml_add_tag("string", HNAP1_URL"GetPortMappings", priv.tmpstr);
	}
	xml_add_tag("string", HNAP1_URL"GetMACFilters", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetMACFilters", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetMACFilters2", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetMACFilters2", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetRouterLanSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetRouterLanSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetConnectedDevices", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetNetworkStats", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWLanRadios", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWLanRadioSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetWLanRadioSettings", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"GetWLanRadioSecurity", priv.tmpstr);
	xml_add_tag("string", HNAP1_URL"SetWLanRadioSecurity", priv.tmpstr);
	xml_sprintf_strcat("SOAPActions", priv.tmpstr, priv.tmp, priv.body);

	xml_sprintf_strcat("SubDeviceURLs",  NULL, priv.tmp, priv.body);
	xml_sprintf_strcat("Tasks",  NULL, priv.tmp, priv.body);
	#if 0
	strcat(priv.body, "<Tasks>");
	
	priv.tmpstr[0] = 0;
	xml_add_tag("Name", "Wireless settings", priv.tmpstr);
	xml_add_tag("URL", "/wlan_basic.asp", priv.tmpstr);
	xml_add_tag("Type", "Browser", priv.tmpstr);
	xml_sprintf_strcat("TaskExtension", priv.tmpstr, priv.tmp, priv.body);

	priv.tmpstr[0] = 0;
	xml_add_tag("Name", "Block network access", priv.tmpstr);
	xml_add_tag("URL", "/fw_macfilter.asp", priv.tmpstr);
	xml_add_tag("Type", "Browser", priv.tmpstr);
	xml_sprintf_strcat("TaskExtension", priv.tmpstr, priv.tmp, priv.body);
	priv.tmpstr[0] = 0;
	xml_add_tag("Name", "Parental controls", priv.tmpstr);
	xml_add_tag("URL", "/fw_macfilter.asp", priv.tmpstr);
	xml_add_tag("Type", "Browser", priv.tmpstr);
	xml_sprintf_strcat("TaskExtension", priv.tmpstr, priv.tmp, priv.body);
	
	priv.tmpstr[0] = 0;
	xml_add_tag("Name", "N Router Tech Support", priv.tmpstr);
	xml_add_tag("URL", ModelURL, priv.tmpstr);
	xml_add_tag("Type", "Browser", priv.tmpstr);
	xml_sprintf_strcat("TaskExtension", priv.tmpstr, priv.tmp, priv.body);
	
	priv.tmpstr[0] = 0;
	xml_add_tag("Name", "Reboot router", priv.tmpstr);
	xml_add_tag("URL", "/util_restart.asp", priv.tmpstr);
	xml_add_tag("Type", "Browser", priv.tmpstr);
	xml_sprintf_strcat("TaskExtension", priv.tmpstr, priv.tmp, priv.body);
	strcat(priv.body, "</Tasks>");
	#endif
SendRes:
	 	SendResponse(h, IsErrorOccur, 0, wp);
	isSetGetToggle=1;
}

static void SetDeviceSettingsResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char clear_mib[1];
	char key[200];
	int NeedReboot=1;
	unsigned char IsErrorOccur=0;
	unsigned char NeedReinitMib=0;
	
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif			
	clear_mib[0] = 0;
	strcpy(priv.action_name, "SetDeviceSettings");
	
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "DeviceName");
	apmib_get(MIB_DOMAIN_NAME, (void *)key);
	if (p_NewInMessage) {
		apmib_set(MIB_DOMAIN_NAME, (void *)p_NewInMessage);
		//printf("%s : %s [%s]\n", "SetDeviceSettings", "DeviceName", p_NewInMessage);
		if (strcmp(key, p_NewInMessage) == 0)
			NeedReboot = 0;
	}
	else {
		apmib_set(MIB_DOMAIN_NAME, (void *)clear_mib);
		//printf("%s : %s []\n", "SetDeviceSettings", "DeviceName");
		if (strlen(key) == 0)
			NeedReboot = 0;
	}
	NeedReinitMib = 1;

	p_NewInMessage = GetValueFromNameValueList(&data, "AdminPassword");
	if (p_NewInMessage) {
		DEBUG_PRINT("SetDeviceSettings, pass [%s]\n", p_NewInMessage);
		strcpy(key, p_NewInMessage);
		//DealWithHNAPKey(key);
		DEBUG_PRINT("SetDeviceSettings, pass after parse [%s]\n", key);
		apmib_set(MIB_ADMIN_PASSWORD, (void *)key);
		strcpy(passwd, key);
		//printf("%s : %s [%s]\n", "SetDeviceSettings", "AdminPassword", p_NewInMessage);
	}
	else {
		apmib_set(MIB_ADMIN_PASSWORD, (void *)clear_mib);
		passwd[0] = 0;
		//printf("%s : %s []\n", "SetDeviceSettings", "AdminPassword");
	}
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetDeviceSettingsResult>REBOOT</SetDeviceSettingsResult>");
	}
	else {
		IsErrorOccur = 1;
	}

      	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
	if(isSetGetToggle==1)
		isSetGetToggle=0;
	isSetIndex_id = 0;
}

static void GetRouterLanSettingsResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i;
	struct in_addr ia_val;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif		

	strcpy(priv.action_name, "GetRouterLanSettings");

	if (router_status == ROUTER_READY) {
		sprintf(priv.result_str, "<%sResult>OK</%sResult>", priv.action_name, priv.action_name);
		strcpy(priv.body, priv.result_str);
	}
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}

	apmib_get(MIB_IP_ADDR, (void *)&ia_val);
	strcpy(priv.tmpstr, inet_ntoa(ia_val));
	xml_sprintf_strcat("RouterIPAddress", priv.tmpstr, priv.tmp, priv.body);
	
	apmib_get(MIB_SUBNET_MASK, (void *)&ia_val);
	strcpy(priv.tmpstr, inet_ntoa(ia_val));
	xml_sprintf_strcat("RouterSubnetMask", priv.tmpstr, priv.tmp, priv.body);

	apmib_get(MIB_DHCP, (void *)&i);
	if (i == DHCP_SERVER)
		xml_sprintf("DHCPServerEnabled", "true", priv.tmp);
	else
		xml_sprintf("DHCPServerEnabled", "false", priv.tmp);
	strcat(priv.body, priv.tmp);
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}

static void SetRouterLanSettingsResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char clear_mib[1];
	unsigned char IsErrorOccur=0;
	int NeedReinitMib=0;
	int router_ip=0, subnet_ip=0, dhcp_mode;
	int high_sub=0, low_sub=0;
	int ia_val;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif		

	strcpy(priv.action_name, "SetRouterLanSettings");
	clear_mib[0] = 0;
	
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "RouterIPAddress");
	if (p_NewInMessage) {
		DEBUG_PRINT("RouterIPAddress [%s]\n", p_NewInMessage);
		if ((router_ip = inet_addr(p_NewInMessage)) == -1) {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_IP_ADDR, (void *)&router_ip);
	}
	else {
		DEBUG_PRINT("RouterIPAddress []\n");
	}

	NeedReinitMib = 1;

	p_NewInMessage = GetValueFromNameValueList(&data, "RouterSubnetMask");
	if (p_NewInMessage) {
		DEBUG_PRINT("RouterSubnetMask [%s]\n", p_NewInMessage);
		if ((subnet_ip = inet_addr(p_NewInMessage)) == -1) {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_SUBNET_MASK, (void *)&subnet_ip);
	}
	else {
		DEBUG_PRINT("RouterSubnetMask []\n");
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "DHCPServerEnabled");
	if (p_NewInMessage) {
		DEBUG_PRINT("DHCPServerEnabled [%s]\n", p_NewInMessage);
		if (strcmp(p_NewInMessage, "true") == 0) {
			dhcp_mode = DHCP_SERVER;
		}
		else if (strcmp(p_NewInMessage, "false") == 0) {
			dhcp_mode = DHCP_DISABLED;
		}
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_DHCP, (void *)&dhcp_mode);
	}
	else {
		DEBUG_PRINT("DHCPServerEnabled []\n");
		IsErrorOccur = 1;
		goto SendRes;
	}

	if (dhcp_mode == DHCP_SERVER) {
		apmib_get(MIB_DHCP_CLIENT_START, (void *)&ia_val);
		if ((router_ip ^ ia_val) & subnet_ip) { //different subnet
			high_sub = (router_ip & subnet_ip);
			low_sub = (router_ip & (~subnet_ip));
			ia_val = ((low_sub+1)|high_sub);
			apmib_set(MIB_DHCP_CLIENT_START, (void *)&ia_val);
			if ((~subnet_ip) > 102)
				ia_val = ((low_sub+101)|high_sub);
			else {
				int tmp;
				tmp = (~subnet_ip) - 2;
				ia_val = ((low_sub+1+tmp)|high_sub);
			}
			apmib_set(MIB_DHCP_CLIENT_END, (void *)&ia_val);
		}
	}
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetRouterLanSettingsResult>REBOOT</SetRouterLanSettingsResult>");
	}
	else
		IsErrorOccur = 1;

SendRes:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}

static void GetWLanSettings24Response(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i=0;
	unsigned char key[20];

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "GetWLanSettings24");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWLanSettings24Result>OK</GetWLanSettings24Result>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}

	apmib_get(MIB_WLAN_DISABLED, (void *)&i);
	if (i)
		xml_sprintf("Enabled", "false", priv.tmp);
	else
		xml_sprintf("Enabled", "true", priv.tmp);
	strcat(priv.body, priv.tmp);

	apmib_get(MIB_HW_WLAN_ADDR, (void *)key);
	sprintf(priv.tmpstr, "%02x:%02x:%02x:%02x:%02x:%02x", key[0], key[1],
			key[2], key[3], key[4], key[5]);
	xml_sprintf_strcat("MacAddress", priv.tmpstr, priv.tmp, priv.body);

	apmib_get(MIB_WLAN_SSID, (void *)priv.tmpstr);
	xml_sprintf_strcat("SSID", priv.tmpstr, priv.tmp, priv.body);
	
	apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&i);
	if (i)
		xml_sprintf("SSIDBroadcast", "false", priv.tmp);
	else
		xml_sprintf("SSIDBroadcast", "true", priv.tmp);
	strcat(priv.body, priv.tmp);
	
	apmib_get(MIB_WLAN_CHAN_NUM, (void *)&i);
	xml_sprintf_int_strcat("Channel", i, priv.tmp, priv.body);
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}

static void SetWLanSettings24Response(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char clear_mib[1];
	int i=0;
	unsigned char IsErrorOccur=0;
	unsigned char NeedReinitMib=0;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "SetWLanSettings24");
	clear_mib[0] = 0;
	
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "Enabled");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "true") == 0)
			i = 0;
		else if (strcmp(p_NewInMessage, "false") == 0)
			i = 1;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_WLAN_DISABLED, (void *)&i);
	}

	NeedReinitMib = 1;
		
	p_NewInMessage = GetValueFromNameValueList(&data, "SSID");
	if (p_NewInMessage) {
		char ssid[100];
		apmib_get(MIB_WLAN_SSID, (void *)ssid);
		if (strcmp(ssid, p_NewInMessage)) {
			i = 1;
			apmib_set(MIB_WSC_CONFIGURED, (void *)&i);
		}
		apmib_set(MIB_WLAN_SSID, (void *)p_NewInMessage);
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "SSIDBroadcast");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "true") == 0)
			i = 0;
		else if (strcmp(p_NewInMessage, "false") == 0)
			i = 1;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&i);
	}

	p_NewInMessage = GetValueFromNameValueList(&data, "Channel");
	if (p_NewInMessage) {
		i = atoi(p_NewInMessage);
		if (i >= 0 && i <= 14)
			apmib_set(MIB_WLAN_CHAN_NUM, (void *)&i);
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
	}
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetWLanSettings24Result>REBOOT</SetWLanSettings24Result>");
	}
	else {
		IsErrorOccur = 1;
	}

SendRes:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}
static void WPSsetAuth_Enc(int auth, int enc, unsigned char disable1X)
{
	int intVal;

	if (disable1X) {
		intVal = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&intVal);				
	}
	
	if (auth == WSC_AUTH_OPEN) { // open
		intVal = ENCRYPT_DISABLED;	
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);	
	}
	else if (auth == WSC_AUTH_WPAPSK) { // WPA-PSK
		intVal = ENCRYPT_WPA;		
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);
			
		intVal = 0;
		if (enc == WSC_ENCRYPT_TKIP)
			intVal |= WPA_CIPHER_TKIP;
		if (enc == WSC_ENCRYPT_AES)
			intVal |= WPA_CIPHER_AES;			
		if (enc == WSC_ENCRYPT_TKIPAES)
			intVal |= WPA_CIPHER_MIXED;
						
		apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intVal);										
	}
	else if (auth == WSC_AUTH_WPA2PSK) { // WPA2-PSK		
		intVal = ENCRYPT_WPA2;	
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);									
			
		intVal = 0;
		if (enc == WSC_ENCRYPT_TKIP)
			intVal |= WPA_CIPHER_TKIP;
		if (enc == WSC_ENCRYPT_AES)
			intVal |= WPA_CIPHER_AES;		
		if (enc == WSC_ENCRYPT_TKIPAES)
			intVal |= WPA_CIPHER_MIXED;					
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intVal);			
		}
	else if (auth == WSC_AUTH_WPA2PSKMIXED) { // WPA2-MIXED
		intVal = ENCRYPT_WPA2_MIXED;		
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&intVal);			
			
		intVal = 0;
		if (enc == WSC_ENCRYPT_TKIP)
			intVal |= WPA_CIPHER_TKIP;
		if (enc == WSC_ENCRYPT_AES)
			intVal |= WPA_CIPHER_AES;		
		if (enc == WSC_ENCRYPT_TKIPAES)
			intVal |= WPA_CIPHER_MIXED;							
		apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&intVal);						
		apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&intVal);			
	}
}
static unsigned char convert_atob(char *data, int base)
{
	unsigned char tmpbuf[10];
	int bin;

	memcpy(tmpbuf, data, 2);
	tmpbuf[2]='\0';
	if (base == 16)
		sscanf(tmpbuf, "%02x", &bin);
	else
		sscanf(tmpbuf, "%02d", &bin);
	return((unsigned char)bin);
}
static int wep_string_to_hex(char *string, unsigned char *key, int key_len)
{
	int i;
	char *p;

	p = string;
	for (i=0; i<key_len; i++, p+=2) {
		if ( !isxdigit((int)*p) || !isxdigit((int)*(p+1)) )
			return 0;
		key[i] = convert_atob(p, 16);			
	}
	return 1;
}
static void DealWithHNAPKey(char *buf)
{
	char tmp[200];
	char *start;
	char *end;
	int i=0;

	if (buf == NULL)
		return;
	start = buf;
	end = start + strlen(buf);

	memset(tmp, 0, 200);
	while (start <= end) {
		if (*start == ' ') {
			tmp[i] = *start;
			i++;
			start++;
			continue;
		}

		if (strncasecmp(start, "&apos;", 6) == 0) {
			tmp[i] = '\'';
			i++; start+=6;
		}
		else if (strncasecmp(start, "&gt;", 4) == 0) {
			tmp[i] = '>';
			i++; start+=4;
		}
		else if (strncasecmp(start, "&amp;", 5) == 0) {
			tmp[i] = '&';
			i++; start+=5;
		}
		else if (strncasecmp(start, "&lt;", 4) == 0) {
			tmp[i] = '<';
			i++; start+=4;
		}
		else {
			tmp[i] = *start;
			i++;
			start++;
		}
	}
	strcpy(buf, tmp);
}

static void ReverseHNAPKey(char *buf)
{
	char tmp[200];
	char *start;
	char *tmp_start;
	char *end;

	if (buf == NULL)
		return;
	start = buf;
	tmp_start = tmp;
	end = start + strlen(buf);

	memset(tmp, 0, 200);
	while (start <= end) {
		if (*start == '\'') {
			memcpy(tmp_start, "&apos;", 6);
			start++;
			tmp_start+=6;
		}
		else if (*start == '>') {
			memcpy(tmp_start, "&gt;", 4);
			start++;
			tmp_start+=4;
		}
		else if (*start == '&') {
			memcpy(tmp_start, "&amp;", 5);
			start++;
			tmp_start+=5;
		}
		else if (*start == '<') {
			memcpy(tmp_start, "&lt;", 4);
			start++;
			tmp_start+=4;
		}
		else {
			*tmp_start = *start;
			tmp_start++;
			start++;
		}
	}
	strcpy(buf, tmp);
}

static void GetWLanSecurityResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i=0;
	unsigned char key[200];
	int SecMode;
	struct in_addr ia_val;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "GetWLanSecurity");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWLanSecurityResult>OK</GetWLanSecurityResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}

	apmib_get(MIB_WLAN_ENCRYPT, (void *)&SecMode);
	if (SecMode == ENCRYPT_DISABLED) {
		xml_sprintf_strcat("Enabled", "false", priv.tmp, priv.body);
		xml_sprintf_strcat("Type", "WPA", priv.tmp, priv.body);
		xml_sprintf_strcat("WEPKeyBits", "64", priv.tmp, priv.body);
		xml_sprintf_strcat("SupportedWEPKeyBits", "<int>64</int><int>128</int>", priv.tmp, priv.body);
		xml_sprintf_strcat("Key", "abcdefghijk", priv.tmp, priv.body);
	}
	else {
		xml_sprintf_strcat("Enabled", "true", priv.tmp, priv.body);
		
		if (SecMode == ENCRYPT_WEP) {
			xml_sprintf_strcat("Type", "WEP", priv.tmp, priv.body);

			int wep_mode;
			apmib_get( MIB_WLAN_WEP, (void *)&wep_mode);
			if (wep_mode == WEP64)
				xml_sprintf("WEPKeyBits", "64", priv.tmp);
			else if (wep_mode == WEP128)
				xml_sprintf("WEPKeyBits", "128", priv.tmp);
			else
				xml_sprintf("WEPKeyBits", NULL, priv.tmp);
			strcat(priv.body, priv.tmp);

			xml_sprintf_strcat("SupportedWEPKeyBits", "<int>64</int><int>128</int>", priv.tmp, priv.body);

			int keyId;
			apmib_get( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&keyId);
			if (wep_mode == WEP64) {
				switch (keyId)
				{
					case 0:
						apmib_get(MIB_WLAN_WEP64_KEY1, (void *)key);
						break;
					case 1:
						apmib_get(MIB_WLAN_WEP64_KEY2, (void *)key);
						break;
					case 2:
						apmib_get(MIB_WLAN_WEP64_KEY3, (void *)key);
						break;
					case 3:
						apmib_get(MIB_WLAN_WEP64_KEY4, (void *)key);
						break;
					default:
						break;
				}
				sprintf(priv.tmpstr, "%02x%02x%02x%02x%02x", key[0], key[1],
					key[2], key[3], key[4]);
			}
			else if (wep_mode == WEP128) {
				switch (keyId)
				{
					case 0:
						apmib_get(MIB_WLAN_WEP128_KEY1, (void *)key);
						break;
					case 1:
						apmib_get(MIB_WLAN_WEP128_KEY2, (void *)key);
						break;
					case 2:
						apmib_get(MIB_WLAN_WEP128_KEY3, (void *)key);
						break;
					case 3:
						apmib_get(MIB_WLAN_WEP128_KEY4, (void *)key);
						break;
					default:
						break;
				}
				sprintf(priv.tmpstr, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],
					key[2], key[3], key[4], key[5], key[6], key[7],
					key[8], key[9], key[10], key[11], key[12]);
			}
			xml_sprintf_strcat("Key", priv.tmpstr, priv.tmp, priv.body);
		}
		else {
			xml_sprintf_strcat("Type", "WPA", priv.tmp, priv.body);
			xml_sprintf_strcat("WEPKeyBits", "64", priv.tmp, priv.body);
			xml_sprintf_strcat("SupportedWEPKeyBits", "<int>64</int><int>128</int>", priv.tmp, priv.body);

			apmib_get(MIB_WLAN_WPA_PSK, (void *)key);
			DEBUG_PRINT("GetWLanSecurity, key [%s]\n", key);
			ReverseHNAPKey(key);
			DEBUG_PRINT("GetWLanSecurity, key after Reverse [%s]\n", key);
			xml_sprintf_strcat("Key", key, priv.tmp, priv.body);
		}		
	}

	apmib_get(MIB_WLAN_WPA_AUTH, (void *)&i);
	if (i ==WPA_AUTH_AUTO && SecMode != ENCRYPT_DISABLED) {
		apmib_get(MIB_WLAN_RS_IP1, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
		xml_sprintf_strcat("RadiusIP1", priv.tmpstr, priv.tmp, priv.body);

		apmib_get(MIB_WLAN_RS_PORT1, (void *)&i);
		xml_sprintf_int_strcat("RadiusPort1", i, priv.tmp, priv.body);
	
		apmib_get(MIB_WLAN_RS_IP2, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
		xml_sprintf_strcat("RadiusIP2", priv.tmpstr, priv.tmp, priv.body);

		apmib_get(MIB_WLAN_RS_PORT2, (void *)&i);
		xml_sprintf_int_strcat("RadiusPort2", i, priv.tmp, priv.body);
	}
	else {	
		xml_sprintf_strcat("RadiusIP1",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusPort1",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusIP2",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusPort2",NULL, priv.tmp, priv.body);
	}
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}
static void SetWLanSecurityResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char *p_NewInMessage_2;
	int i=0;
	unsigned char IsErrorOccur=0;
	int IsConfigured=0;
	int SecMode=ENCRYPT_DISABLED;	
	int OldSecMode;	
	int wep_mode=WEP_DISABLED;
	char key[200];
	int ip;
	char clear_mib[1];
	unsigned char NeedReinitMib=0;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "SetWLanSecurity");
	clear_mib[0] = 0;
	
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "Enabled");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "true") == 0)
			i = 1;
		else if (strcmp(p_NewInMessage, "false") == 0)
			i = 0;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
	}

	NeedReinitMib = 1;
	
	apmib_get(MIB_WLAN_ENCRYPT, (void *)&OldSecMode);
	if (i) {
		p_NewInMessage = GetValueFromNameValueList(&data, "Type");
		if (p_NewInMessage) {
			if (strcmp(p_NewInMessage, "WEP") == 0)
				SecMode = ENCRYPT_WEP;
			else if (strcmp(p_NewInMessage, "WPA") == 0)
				SecMode = ENCRYPT_WPA;
		}

		if (SecMode != OldSecMode)
			IsConfigured = 1;

		if (SecMode == ENCRYPT_WEP) {
			p_NewInMessage = GetValueFromNameValueList(&data, "WEPKeyBits");
			if (p_NewInMessage && strcmp(p_NewInMessage, "64") == 0)
				wep_mode = WEP64;
			else if (p_NewInMessage && strcmp(p_NewInMessage, "128") == 0)
				wep_mode = WEP128;
			else {
				IsErrorOccur = 1;
				goto SendRes;
			}

			apmib_set( MIB_WLAN_WEP, (void *)&wep_mode);
			p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			if (p_NewInMessage) {
				int key_len=strlen(p_NewInMessage);
				if (key_len != 10 && key_len != 26) {
					IsErrorOccur = 1;
					goto SendRes;
				}

				apmib_set(MIB_WSC_PSK, (void *)p_NewInMessage);
				
				if (!wep_string_to_hex(p_NewInMessage, key, (key_len/2))) {
					IsErrorOccur = 1;
					goto SendRes;
				}

				i = KEY_HEX;
				apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&i);
				if (wep_mode == WEP64)
					apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else
					apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
			}
			i = WSC_AUTH_OPEN;
			apmib_set(MIB_WSC_AUTH, (void *)&i);
			i = WSC_ENCRYPT_WEP;
			apmib_set(MIB_WSC_ENC, (void *)&i);
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		}
		else if (SecMode == ENCRYPT_WPA) {
			char *value;
			value = mini_UPnPGetFirstElement(h, strlen(h),"Key", 3);
			//p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			//if (p_NewInMessage) {
			if (value) {
				int TotalLen;
				DEBUG_PRINT("SetWLanSecurity, key [%s]\n", value);
				DealWithHNAPKey(value);
				DEBUG_PRINT("SetWLanSecurity, key after parse [%s]\n", value);
				TotalLen = strlen(value);
				if (TotalLen < 8 || TotalLen >64) {
					IsErrorOccur = 1;
					free(value);
					goto SendRes;
				}
				apmib_set(MIB_WSC_PSK, (void *)value);
				apmib_set(MIB_WLAN_WPA_PSK, (void *)value);
				free(value);
			}

			i = WSC_AUTH_WPAPSK;
			apmib_set(MIB_WSC_AUTH, (void *)&i);
			i = WSC_ENCRYPT_TKIP;
			apmib_set(MIB_WSC_ENC, (void *)&i);
			WPSsetAuth_Enc(WSC_AUTH_WPAPSK, WSC_ENCRYPT_TKIP, 0);
		}

		p_NewInMessage = GetValueFromNameValueList(&data, "RadiusIP1");
		if (p_NewInMessage) {
			p_NewInMessage_2 = GetValueFromNameValueList(&data, "RadiusPort1");
			if (!p_NewInMessage_2) {
				IsErrorOccur = 1;
				goto SendRes;
			}
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 1;
				goto SendRes;
			}
			apmib_set(MIB_WLAN_RS_IP1, (void *)&ip);	
			i = atoi(p_NewInMessage_2);
			apmib_set(MIB_WLAN_RS_PORT1, (void *)&i);
			i = WPA_AUTH_AUTO;
			apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
			//i = 1;
			//apmib_set(MIB_WSC_DISABLE, (void *)&i);
		}
		else {
			ip = 0;
			apmib_set(MIB_WLAN_RS_IP1, (void *)&ip);	
			i = 0;
			apmib_set(MIB_WLAN_RS_PORT1, (void *)&i);
			i = WPA_AUTH_PSK;
			apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
			//i = 0;
			//apmib_set(MIB_WSC_DISABLE, (void *)&i);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "RadiusIP2");
		if (p_NewInMessage) {
			p_NewInMessage_2 = GetValueFromNameValueList(&data, "RadiusPort2");
			if (!p_NewInMessage_2) {
				IsErrorOccur = 1;
				goto SendRes;
			}
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 1;
				goto SendRes;
			}
			apmib_set(MIB_WLAN_RS_IP2, (void *)&ip);	
			i = atoi(p_NewInMessage_2);
			apmib_set(MIB_WLAN_RS_PORT2, (void *)&i);
		}
		else {
			ip = 0;
			apmib_set(MIB_WLAN_RS_IP2, (void *)&ip);	
			i = 0;
			apmib_set(MIB_WLAN_RS_PORT2, (void *)&i);
		}
	}
	else {
		SecMode = ENCRYPT_DISABLED;
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		if (SecMode != OldSecMode)
			IsConfigured = 1;
		SecMode = WSC_AUTH_OPEN;
		apmib_set(MIB_WSC_AUTH, (void *)&SecMode);
		SecMode = WSC_ENCRYPT_NONE;
		apmib_set(MIB_WSC_ENC, (void *)&SecMode);
		i = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
		apmib_set(MIB_WSC_PSK, (void *)clear_mib);
		apmib_set(MIB_WLAN_WPA_PSK, (void *)clear_mib);
		//i = 0;
		//apmib_set(MIB_WSC_DISABLE, (void *)&i);
	}
	

	if (IsConfigured)
		apmib_set(MIB_WSC_CONFIGURED, (void *)&IsConfigured);
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetWLanSecurityResult>REBOOT</SetWLanSecurityResult>");
	}
	else
		IsErrorOccur = 1;

SendRes:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}

static void RebootResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "Reboot");
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REINIT;
		strcpy(priv.body,"<RebootResult>REBOOT</RebootResult>");
	}
	else
		IsErrorOccur = 1;
	
      	SendResponse(h, IsErrorOccur, 0, wp);
}
static int  RenewWanConnection(int RenewTimeout)
{
	char tmpBuf[200];
	DHCP_T dhcp;
	int pid, timeout;
	struct in_addr intaddr;
	apmib_get( MIB_WAN_DHCP, (void *)&dhcp);
	if (dhcp == DHCP_CLIENT) {
		DEBUG_PRINT("wan type is DHCP client\n");
			snprintf(tmpBuf, 100, "%s/%s-%s.pid", _DHCPC_PID_PATH, _DHCPC_PROG_NAME, "eth1");
			sleep(5); //for debug sean
			if ((pid=getPid(tmpBuf)) > 0) {				
				// release
				sprintf(tmpBuf, "kill -%d %d", SIGUSR2, pid);
				system(tmpBuf);

				timeout = 0;
				while (timeout++ < 5) {				
					if (getInAddr("eth1", IP_ADDR, (void *)&intaddr))
						sleep(1);
					else
						break;
				}
				if (timeout > 5)
					goto ip_fail1;

				// renew					
				sprintf(tmpBuf, "kill -%d %d", SIGUSR1, pid);
				system(tmpBuf);							
				timeout = 0;
				while (1) {
					if (getInAddr("eth1", IP_ADDR, (void *)&intaddr))
						break;
					if(timeout > RenewTimeout)
						break;
					timeout++;		
					sleep(1);
				}
				#if 0
				if (timeout > RenewTimeout){
					DEBUG_PRINT("Renew IP timeout\n"); 
					return -1;
				}	
				else{			
					DEBUG_PRINT("Renew IP successful\n"); 
					return 0;
				}
				#endif 
				return 0;
			}
			else
ip_fail1:				
				printf("Renew IP failed!\n"); 
				return -1;
	}else if ( dhcp ==  PPPOE ) {
			//Do wan pppoe renew
			PPP_CONNECT_TYPE_T type;
			apmib_get( MIB_PPP_CONNECT_TYPE, (void *)&type);
			if(type == CONNECT_ON_DEMAND){
				if(is_ppp0_exist() ==1 || getInAddr("ppp0", IP_ADDR, (void *)&intaddr )){
					system("disconnect.sh option");
					sleep(5);
				}
			}else{
				if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr )) {			
					system("disconnect.sh option");
					sleep(5);
				}
			}
			
			//Do wan pppoe connect
			if(type == MANUAL){
	        		pid = find_pid_by_name("pppoe.sh");
	        		if(pid)
		                	kill(pid, SIGTERM);
	        		pid = fork();
	        		if (pid)
		                	waitpid(pid, NULL, 0);
	        		else if (pid == 0) {
		                	snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _PPPOE_SCRIPT_PROG);
			                execl( tmpBuf, _PPPOE_SCRIPT_PROG, "connect","eth1",NULL);
				}	
			}
			timeout = 0;
			while (1) {
				if(type == CONNECT_ON_DEMAND){
					if(is_ppp0_exist() ==1)
						break;
				}else{
					if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr))
							break;
					}
				if(timeout > RenewTimeout)
						break;
				timeout++;					
				sleep(1);
			}
			//printf("the try time=%d\n", timeout);
			#if 0
			if (timeout > RenewTimeout){
				DEBUG_PRINT("PPoE Renew timeout\n"); 
				return -1;
			}else{			
				DEBUG_PRINT("PPPoE ReConnect successful!\n"); 
				return 0;
			}	
			#endif
			return 0;	
	}else if ( dhcp ==  PPTP ) {
		
			PPP_CONNECT_TYPE_T type;
			apmib_get( MIB_PPTP_CONNECTION_TYPE, (void *)&type);
			if(type == CONNECT_ON_DEMAND){
				if(is_ppp0_exist() ==1 || getInAddr("ppp0", IP_ADDR, (void *)&intaddr )){
					system("disconnect.sh all");
					sleep(5);
				}
			}else{
				if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr )) {			
					system("disconnect.sh all");
					sleep(5);
				}
			}
			
			//Do wan pptp connect
			if(type == MANUAL){
	        		pid = find_pid_by_name("pptp.sh");
	        		if(pid)
		                	kill(pid, SIGTERM);
	        		pid = fork();
	        		if (pid)
		                	waitpid(pid, NULL, 0);
	        		else if (pid == 0) {
		                	snprintf(tmpBuf, 100, "%s/%s", _CONFIG_SCRIPT_PATH, _PPTP_SCRIPT_PROG);
		                execl( tmpBuf, _PPTP_SCRIPT_PROG, "connect", "eth1",NULL);
				}	
			}
			timeout = 0;
			while (1) {
				if(type == CONNECT_ON_DEMAND){
					if(is_ppp0_exist() ==1)
						break;
				}else{
					if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr))
							break;
					}
				if(timeout > RenewTimeout)
						break;
				timeout++;					
				sleep(1);
			}
			#if 0
			if (timeout > RenewTimeout){
				DEBUG_PRINT("PPTP Renew timeout\n"); 
				return -1;
			}
			else{			
				DEBUG_PRINT("PPTP ReConnect successful!\n"); 
				return 0;
			}
			#endif
			return 0;
	}	
	else if ( dhcp ==  L2TP ) {
		
			PPP_CONNECT_TYPE_T type;
			apmib_get( MIB_L2TP_CONNECTION_TYPE, (void *)&type);
			if(type == CONNECT_ON_DEMAND){
				if(is_ppp0_exist() ==1 || getInAddr("ppp0", IP_ADDR, (void *)&intaddr )){
					system("disconnect.sh l2tp");
					sleep(5);
				}
			}else{
				if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr )) {			
					system("disconnect.sh l2tp");
					sleep(5);
				}
			}
			
			//Do wan pptp connect
			if(type == MANUAL){
	        		system("killall sleep");
				system("killall l2tpd");
				system("killall l2tp.sh");
				system("l2tp.sh connect eth1");	
			}
			timeout = 0;
			while (1) {
				if(type == CONNECT_ON_DEMAND){
					if(is_ppp0_exist() ==1)
						break;
				}else{
					if (getInAddr("ppp0", IP_ADDR, (void *)&intaddr))
							break;
					}
				if(timeout > RenewTimeout)
						break;
				timeout++;					
				sleep(1);
			}
			#if 0
			if (timeout > RenewTimeout){
				DEBUG_PRINT("L2TP Renew timeout\n"); 
				return -1;
			}
			else{			
				DEBUG_PRINT("L2TP ReConnect successful!\n"); 
				return 0;
			}
			#endif
			return 0;
	}else{
	//Static IP case do nothing!
	}
	return 0;

}


static void RenewWanConnectionResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	unsigned char IsErrorOccur=0;
	int i, isWANconnected=-1;
	int wan_type;

#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "RenewWanConnection");

	isWANconnected = is_wan_connected();
	if (isWANconnected == LINK_UP) {
		DEBUG_PRINT("WAN connected\n");
		memset(&data, 0, sizeof(struct NameValueParserData));
		ParseNameValue(h, strlen(h), &data);
		p_NewInMessage = GetValueFromNameValueList(&data, "RenewTimeout");
		if (p_NewInMessage) {
			i = atoi(p_NewInMessage);
			DEBUG_PRINT("RenewTimeout [%d]\n", i);
			apmib_get(MIB_WAN_DHCP, (void *)&wan_type);
			#if 0
			if (wan_type == PPPOE || wan_type == PPTP || wan_type == L2TP) {
				if (i < 20 || i > 120) {
				IsErrorOccur = 1;
				goto Send_Res;
			}
			}
			else {
				if (i < 10 || i > 120) {
					IsErrorOccur = 1;
					goto Send_Res;
				}
			}
			#endif
			if (RenewWanConnection(i)==0) {
				DEBUG_PRINT("wan renew ok\n");
			}
			else {
				IsErrorOccur = 1;
				goto Send_Res;
			}
		}
		else {
			IsErrorOccur = 1;
			goto Send_Res;
		}
	}
	else {
		DEBUG_PRINT("WAN disconnected\n");
	}
	
	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<RenewWanConnectionResult>OK</RenewWanConnectionResult>");
	else{
		IsErrorOccur = 1;
	}

Send_Res:
      	SendResponse(h, IsErrorOccur, 0, wp);
	if (isWANconnected == LINK_UP)
		ClearNameValueList(&data);
}
static void GetWLanRadiosResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i=0;
	unsigned char key[200];
	int intVal=0;
	int channelbound=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "GetWLanRadios");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWLanRadiosResult>OK</GetWLanRadiosResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}
	strcpy(priv.tmp,"<RadioInfos><RadioInfo>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<RadioID>2.4GHZ</RadioID>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<Frequency>2</Frequency>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<SupportedModes>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<string>802.11n</string>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<string>802.11bg</string>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<string>802.11bgn</string>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"</SupportedModes>");
	strcat(priv.body, priv.tmp);
	strcpy(priv.tmp,"<Channels>");
	strcat(priv.body, priv.tmp);

	
	apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&channelbound);
	apmib_get(MIB_HW_REG_DOMAIN, (void *)&intVal);
	if (intVal == AF || intVal == AS || intVal == AU || intVal == EU ||
			intVal == FR || intVal == IL || intVal == JP || intVal == SUA) {
				//channel 1~13
			for(i=1;i<=13;i++){
				sprintf(key, "<int>%d</int>", i);
				strcpy(priv.tmp,key);
				strcat(priv.body, priv.tmp);
			}
			strcpy(priv.tmp,"</Channels>");
			strcat(priv.body, priv.tmp);
			
			if(channelbound==1){
				for(i=1;i<=13;i++){
					if(i <= 4){
						strcpy(priv.tmp,"<WideChannels>");
						strcat(priv.body, priv.tmp);
						sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i+4);
						strcpy(priv.tmp,key);
						strcat(priv.body, priv.tmp);
						strcpy(priv.tmp,"</WideChannel>");
						strcat(priv.body, priv.tmp);
					}else{
						strcpy(priv.tmp,"<WideChannels>");
						strcat(priv.body, priv.tmp);
						sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i-4);
						strcpy(priv.tmp,key);
						strcat(priv.body, priv.tmp);
						strcpy(priv.tmp,"</WideChannel>");
						strcat(priv.body, priv.tmp);
					}				
				}
			}else{
				for(i=1;i<=13;i++){
					strcpy(priv.tmp,"<WideChannels>");
					strcat(priv.body, priv.tmp);
					sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i);
					strcpy(priv.tmp,key);
					strcat(priv.body, priv.tmp);
					strcpy(priv.tmp,"</WideChannel>");
					strcat(priv.body, priv.tmp);
				}
			}
			strcpy(priv.tmp,"</WideChannels>");
			strcat(priv.body, priv.tmp);
	}
	else {		
		//channel 1~11
		for(i=1;i<=11;i++){
				sprintf(key, "<int>%d</int>", i);
				strcpy(priv.tmp,key);
				strcat(priv.body, priv.tmp);
			}
			strcpy(priv.tmp,"</Channels>");
			strcat(priv.body, priv.tmp);
			
			if(channelbound==1){
				for(i=1;i<=11;i++){
					if(i <= 4){
						strcpy(priv.tmp,"<WideChannels>");
						strcat(priv.body, priv.tmp);
						sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i+4);
						strcpy(priv.tmp,key);
						strcat(priv.body, priv.tmp);
						strcpy(priv.tmp,"</WideChannel>");
						strcat(priv.body, priv.tmp);
					}else{
						strcpy(priv.tmp,"<WideChannels>");
						strcat(priv.body, priv.tmp);
						sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i-4);
						strcpy(priv.tmp,key);
						strcat(priv.body, priv.tmp);
						strcpy(priv.tmp,"</WideChannel>");
						strcat(priv.body, priv.tmp);
					}				
				}
			}else{
				for(i=1;i<=11;i++){
					strcpy(priv.tmp,"<WideChannels>");
					strcat(priv.body, priv.tmp);
					sprintf(key, "<Channel>%d</Channel><SecondaryChannels><int>%d</int></SecondaryChannels>", i, i);
					strcpy(priv.tmp,key);
					strcat(priv.body, priv.tmp);
					strcpy(priv.tmp,"</WideChannel>");
					strcat(priv.body, priv.tmp);
				}
			}
			strcpy(priv.tmp,"</WideChannels>");
			strcat(priv.body, priv.tmp);
	}
	
	

strcpy(priv.tmp,"<SupportedSecurity>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WEP-OPEN</SecurityType><Encryptions><string>WEP-64</string><string>WEP-128</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WEP-SHARED</SecurityType><Encryptions><string>WEP-64</string><string>WEP-128</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPA-PSK</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);


strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPA-RADIUS</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);


strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPA2-PSK</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPA2-RADIUS</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPAORWPA2-PSK</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);

strcpy(priv.tmp,"<SecurityInfo><SecurityType>WPAORWPA2-RADIUS</SecurityType><Encryptions><string>TKIP</string><string>AES</string><string>TKIPORAES</string></Encryptions></SecurityInfo>");
strcat(priv.body, priv.tmp);


strcpy(priv.tmp,"</SupportedSecurity>");
strcat(priv.body, priv.tmp);

	strcpy(priv.tmp,"</RadioInfos></RadioInfo>");
	strcat(priv.body, priv.tmp);
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}
static void GetWLanRadioSettingsResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i=0;
	unsigned char key[20];
	int channel_bound=-1;
	int wlan_band=-1;
	int wlan_channel_num=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "GetWLanRadioSettings");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWLanRadioSettingsResult>OK</GetWLanRadioSettingsResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}

	xml_sprintf("RadioID", "2.4GHZ", priv.tmp);
	strcat(priv.body, priv.tmp);
	
	apmib_get(MIB_WLAN_DISABLED, (void *)&i);
	if (i)
		xml_sprintf("Enabled", "false", priv.tmp);
	else
		xml_sprintf("Enabled", "true", priv.tmp);
	strcat(priv.body, priv.tmp);

	apmib_get(MIB_HW_WLAN_ADDR, (void *)key);
	sprintf(priv.tmpstr, "%02x:%02x:%02x:%02x:%02x:%02x", key[0], key[1],
			key[2], key[3], key[4], key[5]);
	xml_sprintf_strcat("MacAddress", priv.tmpstr, priv.tmp, priv.body);

	apmib_get(MIB_WLAN_SSID, (void *)priv.tmpstr);
	xml_sprintf_strcat("SSID", priv.tmpstr, priv.tmp, priv.body);
	
	apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&i);
	if (i)
		xml_sprintf("SSIDBroadcast", "false", priv.tmp);
	else
		xml_sprintf("SSIDBroadcast", "true", priv.tmp);
	strcat(priv.body, priv.tmp);
	
	apmib_get(MIB_WLAN_CHAN_NUM, (void *)&i);
	wlan_channel_num = i;
	xml_sprintf_int_strcat("Channel", i, priv.tmp, priv.body);
	
	apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&channel_bound);
	if(channel_bound==1){
		i=0;
	}else{
		i=20;
	}
	xml_sprintf_int_strcat("ChannelWidth", i, priv.tmp, priv.body);	
	
	if(channel_bound==0){
		xml_sprintf_int_strcat("SecondaryChannel",wlan_channel_num, priv.tmp, priv.body);
	}else if(channel_bound==1){
		if(wlan_channel_num>0 && wlan_channel_num <=4){
			xml_sprintf_int_strcat("SecondaryChannel",wlan_channel_num+4, priv.tmp, priv.body);
		}else if(wlan_channel_num > 4 && wlan_channel_num <=13) {
			xml_sprintf_int_strcat("SecondaryChannel",wlan_channel_num-4, priv.tmp, priv.body);
		}
	}
	
	apmib_get(MIB_WLAN_WMM_ENABLED, (void *)&i);
	if (i)
		xml_sprintf("Qos", "true", priv.tmp);
	else
		xml_sprintf("Qos", "false", priv.tmp);
	strcat(priv.body, priv.tmp);
	
	
	apmib_get(MIB_WLAN_BAND, (void *)&wlan_band);
		if (wlan_band == 11)
			xml_sprintf("Mode", "802.11bgn", priv.tmp);
		else if (wlan_band == 8)
			xml_sprintf("Mode", "802.11n", priv.tmp);
		else if( wlan_band== 3)
			xml_sprintf("Mode", "802.11bg", priv.tmp);
	strcat(priv.body, priv.tmp);	
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}



static void SetWLanRadioSettingsResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char clear_mib[1];
	int i=0;
	unsigned char IsErrorOccur=0;
	unsigned char NeedReinitMib=0;
	int wlan_band=0;
	int channel_bound=-1;
	int side_band=-1;
	int ori_wireless_mode=0;
	int ori_opMode=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "SetWLanRadioSettings");
	clear_mib[0] = 0;
	
	apmib_get( MIB_WLAN_MODE, (void *)&ori_wireless_mode);
	apmib_get( MIB_OP_MODE, (void *)&ori_opMode);
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "Enabled");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "true") == 0)
			i = 0;
		else if (strcmp(p_NewInMessage, "false") == 0)
			i = 1;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_WLAN_DISABLED, (void *)&i);
	}
	
	
	p_NewInMessage = GetValueFromNameValueList(&data, "MacAddress");
	if (p_NewInMessage) {
		char key[20];
		if (!string_to_hex(p_NewInMessage, key, 12)) {
			IsErrorOccur = 1;
			goto SendRes;
		}
		else {
		        apmib_set(MIB_HW_WLAN_ADDR,(void *)key);
		        DEBUG_PRINT("MIB_WAN_MAC_ADDR [%s]\n", p_NewInMessage);
		}
	}
	
	
	p_NewInMessage = GetValueFromNameValueList(&data, "Mode");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "802.11bgn") == 0)
			wlan_band = 11;
		else if (strcmp(p_NewInMessage, "802.11bg") == 0)
			 wlan_band= 3;
		else if (strcmp(p_NewInMessage, "802.11n") == 0)
			 wlan_band= 8;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_WLAN_BAND, (void *)&wlan_band);
	}
	
	p_NewInMessage = GetValueFromNameValueList(&data, "ChannelWidth");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "0") == 0)
			channel_bound = 1;
		else if (strcmp(p_NewInMessage, "20") == 0)
			 channel_bound= 0;
		else if (strcmp(p_NewInMessage, "40") == 0)
			 channel_bound= 1;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
		apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&channel_bound);
	}
	NeedReinitMib = 1;
		
	p_NewInMessage = GetValueFromNameValueList(&data, "SSID");
	if (p_NewInMessage) {
		char ssid[100];
		apmib_get(MIB_WLAN_SSID, (void *)ssid);
		if (strcmp(ssid, p_NewInMessage)) {
			i = 1;
			apmib_set(MIB_WSC_CONFIGURED, (void *)&i);
		}
		apmib_set(MIB_WLAN_SSID, (void *)p_NewInMessage);
	}
if((ori_wireless_mode !=CLIENT_MODE) &&(ori_wireless_mode !=WDS_MODE) ){
		p_NewInMessage = GetValueFromNameValueList(&data, "SSIDBroadcast");
		if (p_NewInMessage) {
			if (strcmp(p_NewInMessage, "true") == 0)
				i = 0;
			else if (strcmp(p_NewInMessage, "false") == 0)
				i = 1;
			else {
				IsErrorOccur = 1;
				goto SendRes;
			}
			apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&i);
		}
	}
if(ori_wireless_mode != CLIENT_MODE){
		p_NewInMessage = GetValueFromNameValueList(&data, "Channel");
		if (p_NewInMessage) {
			i = atoi(p_NewInMessage);
			if (i >= 0 && i <= 14)
				apmib_set(MIB_WLAN_CHAN_NUM, (void *)&i);
			else {
				IsErrorOccur = 1;
				goto SendRes;
			}
			if(channel_bound==1){
				if(i<6){
					side_band=1;
				}else{
					side_band=0;
				}
				apmib_set(MIB_WLAN_CONTROL_SIDEBAND, (void *)&side_band);
			}
		}
	}
	if((ori_wireless_mode !=CLIENT_MODE) &&(ori_wireless_mode !=WDS_MODE) ){	
		p_NewInMessage = GetValueFromNameValueList(&data, "Qos");
		if (p_NewInMessage) {
			if (strcmp(p_NewInMessage, "true") == 0)
				i = 1;
			else if (strcmp(p_NewInMessage, "false") == 0)
				i = 0;
			else {
				IsErrorOccur = 1;
				goto SendRes;
			}
			if(wlan_band==8 || wlan_band==11){
				i=1;
			}
			apmib_set(MIB_WLAN_WMM_ENABLED, (void *)&i);
		}
	}
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetWLanRadioSettingsResult>REBOOT</SetWLanRadioSettingsResult>");
	}
	else {
		IsErrorOccur = 1;
	}

SendRes:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}

static void GetWLanRadioSecurityResponse(char *h, request *wp)
{
	unsigned char IsErrorOccur=0;
	int i=0;
	unsigned char key[200];
	int SecMode;
	struct in_addr ia_val;
	int wpa_auth=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "GetWLanRadioSecurity");

	if (router_status == ROUTER_READY)
		strcpy(priv.body,"<GetWLanRadioSecurityResult>OK</GetWLanRadioSecurityResult>");
	else {
		IsErrorOccur = 1;
           	goto SendRes;
	}
  
	apmib_get(MIB_WLAN_ENCRYPT, (void *)&SecMode);
	if (SecMode == ENCRYPT_DISABLED) {
		xml_sprintf_strcat("RadioID", "2.4GHZ", priv.tmp, priv.body);
		xml_sprintf_strcat("Enabled", "false", priv.tmp, priv.body);
		xml_sprintf_strcat("Type", NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("Encryption", NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("KeyRenewal", "0", priv.tmp, priv.body);
		xml_sprintf_strcat("Key",NULL, priv.tmp, priv.body);
		
	}
	else {
		xml_sprintf_strcat("Enabled", "true", priv.tmp, priv.body);
		
		if (SecMode == ENCRYPT_WEP) {
			int wep_mode;
			int wep_auth;
			int keyId;
			int keyTytpe;
			apmib_get(MIB_WLAN_AUTH_TYPE,(void *)&wep_auth);
			if(wep_auth == AUTH_OPEN)
				xml_sprintf_strcat("Type", "WEP-OPEN", priv.tmp, priv.body);
			if(wep_auth == AUTH_SHARED)
				xml_sprintf_strcat("Type", "WEP-SHARDE", priv.tmp, priv.body);
			
			apmib_get( MIB_WLAN_WEP, (void *)&wep_mode);
			if (wep_mode == WEP64)
				xml_sprintf("Encryption", "WEP-64", priv.tmp);
			else if (wep_mode == WEP128)
				xml_sprintf("Encryption", "WEP-128", priv.tmp);
			strcat(priv.body, priv.tmp);


			
			apmib_get( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&keyId);
			apmib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&keyTytpe);
			if (wep_mode == WEP64) {
				switch (keyId)
				{
					case 0:
						apmib_get(MIB_WLAN_WEP64_KEY1, (void *)key);
						break;
					case 1:
						apmib_get(MIB_WLAN_WEP64_KEY2, (void *)key);
						break;
					case 2:
						apmib_get(MIB_WLAN_WEP64_KEY3, (void *)key);
						break;
					case 3:
						apmib_get(MIB_WLAN_WEP64_KEY4, (void *)key);
						break;
					default:
						break;
				}
				if(keyTytpe == KEY_HEX){	
					sprintf(priv.tmpstr, "%02x%02x%02x%02x%02x", key[0], key[1],
						key[2], key[3], key[4]);
				}else{
					sprintf(priv.tmpstr,"%s", key);
				}
			}
			else if (wep_mode == WEP128) {
				switch (keyId)
				{
					case 0:
						apmib_get(MIB_WLAN_WEP128_KEY1, (void *)key);
						break;
					case 1:
						apmib_get(MIB_WLAN_WEP128_KEY2, (void *)key);
						break;
					case 2:
						apmib_get(MIB_WLAN_WEP128_KEY3, (void *)key);
						break;
					case 3:
						apmib_get(MIB_WLAN_WEP128_KEY4, (void *)key);
						break;
					default:
						break;
				}
				if(keyTytpe == KEY_HEX){	
				sprintf(priv.tmpstr, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", key[0], key[1],
					key[2], key[3], key[4], key[5], key[6], key[7],
					key[8], key[9], key[10], key[11], key[12]);
				}else{
					sprintf(priv.tmpstr,"%s", key);
				}
			}
			xml_sprintf_strcat("Key", priv.tmpstr, priv.tmp, priv.body);
		}
		else {
			int wpa_encrypt=0;
			int wpa2_encrypt=0;
			int rekey_time=0;
			apmib_get(MIB_WLAN_WPA_AUTH, (void *)&wpa_auth);
			apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_encrypt);
			apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2_encrypt);
			
			if(SecMode == ENCRYPT_WPA && wpa_auth == WPA_AUTH_PSK)	
				xml_sprintf_strcat("Type", "WPA-PSK", priv.tmp, priv.body);
			else if(SecMode == ENCRYPT_WPA && wpa_auth == WPA_AUTH_AUTO)	
				xml_sprintf_strcat("Type", "WPA-RADIUS", priv.tmp, priv.body);
			else if(SecMode == ENCRYPT_WPA2 && wpa_auth == WPA_AUTH_PSK)	
				xml_sprintf_strcat("Type", "WPA2-PSK", priv.tmp, priv.body);
			else if(SecMode == ENCRYPT_WPA2 && wpa_auth == WPA_AUTH_AUTO)	
				xml_sprintf_strcat("Type", "WPA2-RADIUS", priv.tmp, priv.body);	
			else 	if(SecMode == ENCRYPT_WPA2_MIXED && wpa_auth == WPA_AUTH_AUTO)	
				xml_sprintf_strcat("Type", "WPA2-RADIUS", priv.tmp, priv.body);
			else 	if(SecMode == ENCRYPT_WPA2_MIXED && wpa_auth == WPA_AUTH_PSK)	
				xml_sprintf_strcat("Type", "WPA2-PSK", priv.tmp, priv.body);
				
			if(SecMode == ENCRYPT_WPA){
			if(wpa_encrypt == WPA_CIPHER_TKIP)
				xml_sprintf("Encryption", "TKIP", priv.tmp);
			if(wpa_encrypt == WPA_CIPHER_AES)
				xml_sprintf("Encryption", "AES", priv.tmp);
			if(wpa_encrypt == WPA_CIPHER_MIXED)
				xml_sprintf("Encryption", "TKIPORAES", priv.tmp);
			}else if(SecMode == ENCRYPT_WPA2){
				if(wpa2_encrypt == WPA_CIPHER_TKIP)
					xml_sprintf("Encryption", "TKIP", priv.tmp);
				if(wpa2_encrypt == WPA_CIPHER_AES)
					xml_sprintf("Encryption", "AES", priv.tmp);
				if(wpa2_encrypt == WPA_CIPHER_MIXED)
					xml_sprintf("Encryption", "TKIPORAES", priv.tmp);
			}else if(SecMode == ENCRYPT_WPA2_MIXED){
				xml_sprintf("Encryption", "TKIPORAES", priv.tmp);
			}
			strcat(priv.body, priv.tmp);	
			
			apmib_get(MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&rekey_time);
			sprintf(priv.tmpstr,"%d", rekey_time);
			xml_sprintf_strcat("KeyRenewal", priv.tmpstr, priv.tmp, priv.body);

			apmib_get(MIB_WLAN_WPA_PSK, (void *)key);
			sprintf(priv.tmpstr,"%s", key);
			xml_sprintf_strcat("Key", priv.tmpstr, priv.tmp, priv.body);
		}		
	}

	if (wpa_auth ==WPA_AUTH_AUTO && SecMode != ENCRYPT_DISABLED) {
		apmib_get(MIB_WLAN_RS_IP1, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
		xml_sprintf_strcat("RadiusIP1", priv.tmpstr, priv.tmp, priv.body);

		apmib_get(MIB_WLAN_RS_PORT1, (void *)&i);
		xml_sprintf_int_strcat("RadiusPort1", i, priv.tmp, priv.body);
	
		apmib_get(MIB_WLAN_RS_IP2, (void *)&ia_val);
		strcpy(priv.tmpstr, inet_ntoa(ia_val));
		xml_sprintf_strcat("RadiusIP2", priv.tmpstr, priv.tmp, priv.body);

		apmib_get(MIB_WLAN_RS_PORT2, (void *)&i);
		xml_sprintf_int_strcat("RadiusPort2", i, priv.tmp, priv.body);
		
		
		
		apmib_get(MIB_WLAN_RS_PASSWORD1, (void *)key);
		strcpy(priv.tmpstr,key);
		xml_sprintf_strcat("RadiusSecret1", priv.tmpstr, priv.tmp, priv.body);
			
		apmib_get(MIB_WLAN_RS_PASSWORD2, (void *)key);	
		strcpy(priv.tmpstr,key);
		xml_sprintf_strcat("RadiusSecret2", priv.tmpstr, priv.tmp, priv.body);	
	}
	else {	
		xml_sprintf_strcat("RadiusIP1",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusPort1","0", priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusSecret1",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusIP2",NULL, priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusPort2","0", priv.tmp, priv.body);
		xml_sprintf_strcat("RadiusSecret2",NULL, priv.tmp, priv.body);
	}
	
SendRes:
	SendResponse(h, IsErrorOccur, 0, wp);
}
static void SetWLanRadioSecurityResponse(char *h, request *wp)
{
	struct NameValueParserData data;
	char *p_NewInMessage;
	char *p_NewInMessage_2;
	char *p_NewInMessage_3;
	int i=0;
	unsigned char IsErrorOccur=0;
	int IsConfigured=0;
	int SecMode=ENCRYPT_DISABLED;	
	int OldSecMode;	
	int wep_mode=WEP_DISABLED;
	char key[200];
	int ip;
	char clear_mib[1];
	unsigned char NeedReinitMib=0;
	int wep_auth=0;
	int rekey=0;
	int wpa_auth=0;
#ifdef DYNAMIC_ALLOC_MEM	
	if(Str_Alloc_Buffer()<0)
		return;
#endif	

	strcpy(priv.action_name, "SetWLanRadioSecurity");
	clear_mib[0] = 0;
	
	memset(&data, 0, sizeof(struct NameValueParserData));
	ParseNameValue(h, strlen(h), &data);
	p_NewInMessage = GetValueFromNameValueList(&data, "Enabled");
	if (p_NewInMessage) {
		if (strcmp(p_NewInMessage, "true") == 0)
			i = 1;
		else if (strcmp(p_NewInMessage, "false") == 0)
			i = 0;
		else {
			IsErrorOccur = 1;
			goto SendRes;
		}
	}

	NeedReinitMib = 1;

	apmib_get(MIB_WLAN_ENCRYPT, (void *)&OldSecMode);
	if (i) {
		p_NewInMessage = GetValueFromNameValueList(&data, "Type");
		if (p_NewInMessage) {
			if (strcmp(p_NewInMessage, "WEP-OPEN") == 0 || strcmp(p_NewInMessage, "WEP-SHARED") == 0){
				SecMode = ENCRYPT_WEP;
				if(strcmp(p_NewInMessage, "WEP-OPEN") == 0){
					wep_auth = AUTH_OPEN;
				}
				if(strcmp(p_NewInMessage, "WEP-SHARED") == 0){
					wep_auth = AUTH_SHARED;
				}
				apmib_set(MIB_WLAN_AUTH_TYPE,(void *)&wep_auth);
			}else if (strcmp(p_NewInMessage, "WPA-PSK") == 0 || strcmp(p_NewInMessage, "WPA-RADIUS") == 0 ){
				SecMode = ENCRYPT_WPA;
			}else if (strcmp(p_NewInMessage, "WPA2-PSK") == 0 || strcmp(p_NewInMessage, "WPA2-RADIUS") == 0){
				SecMode = ENCRYPT_WPA2;
			}else if (strcmp(p_NewInMessage, "WPAORWPA2-PSK") == 0){
				SecMode = ENCRYPT_WPA2_MIXED;
			}
			else{
				IsErrorOccur = 1;
				goto SendRes;
			}
			
			if(SecMode != ENCRYPT_WEP){
				if (strcmp(p_NewInMessage, "WPA-PSK") == 0 || strcmp(p_NewInMessage, "WPA2-PSK") == 0 || strcmp(p_NewInMessage, "WPAORWPA2-PSK") == 0){
					wpa_auth = WPA_AUTH_PSK;
				}else if (strcmp(p_NewInMessage, "WPA-RADIUS") == 0 || strcmp(p_NewInMessage, "WPA2-RADIUS") == 0){
					wpa_auth = WPA_AUTH_AUTO;
				}else {
					IsErrorOccur = 4;
					goto SendRes;
				}
				apmib_set(MIB_WLAN_WPA_AUTH, (void *)&wpa_auth);
			}
		}else{
				IsErrorOccur = 4;
				goto SendRes;
		}

		if (SecMode != OldSecMode)
			IsConfigured = 1;

		if (SecMode == ENCRYPT_WEP) {
			p_NewInMessage = GetValueFromNameValueList(&data, "Encryption");
			if (p_NewInMessage && !strcmp(p_NewInMessage, "WEP-64"))
				wep_mode = WEP64;
			else if (p_NewInMessage && !strcmp(p_NewInMessage, "WEP-128"))
				wep_mode = WEP128;
			else {
				IsErrorOccur = 1;
				goto SendRes;
			}

			apmib_set( MIB_WLAN_WEP, (void *)&wep_mode);
			p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			if (p_NewInMessage) {
				int key_len=strlen(p_NewInMessage);
				int Key_type=0;
				if(wep_mode ==WEP64){
					if (key_len != 10 && key_len != 5) {
						IsErrorOccur = 1;
						goto SendRes;
					}
					if(key_len == 5){
						Key_type=KEY_ASCII;
					}
					if(key_len == 10){
						Key_type=KEY_HEX;
					}
					
					apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&Key_type);
				} 
				if(wep_mode ==WEP128){
					if (key_len != 13 && key_len != 26) {
						IsErrorOccur = 1;
						goto SendRes;
					}
					if(key_len == 13){
						Key_type=KEY_ASCII;
					}
					if(key_len == 26){
						Key_type=KEY_HEX;
					}
					
					apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&Key_type);
				}
				apmib_set(MIB_WSC_PSK, (void *)p_NewInMessage);
				if(Key_type==KEY_HEX){
					if (!wep_string_to_hex(p_NewInMessage, key, (key_len/2))) {
						IsErrorOccur = 6;
						goto SendRes;
					}
				}else{
					sprintf(key, "%s", p_NewInMessage);
				}
				if (wep_mode == WEP64)
					apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
				else
					apmib_set(MIB_WLAN_WEP128_KEY1, (void *)key);
			}else{
				IsErrorOccur = 6;
				goto SendRes;
			}
			apmib_set(MIB_WSC_AUTH, (void *)&wep_auth);
			i = WSC_ENCRYPT_WEP;
			apmib_set(MIB_WSC_ENC, (void *)&i);
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		}
		else if (SecMode == ENCRYPT_WPA && wpa_auth ==WPA_AUTH_PSK ) {
			char *value;
			int wpa_encrypt;
			value = mini_UPnPGetFirstElement(h, strlen(h),"Key", 3);
			//p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			//if (p_NewInMessage) {
			if (value) {
				int TotalLen;
				DEBUG_PRINT("SetWLanSecurity, key [%s]\n", value);
				DealWithHNAPKey(value);
				DEBUG_PRINT("SetWLanSecurity, key after parse [%s]\n", value);
				TotalLen = strlen(value);
				if (TotalLen < 8 || TotalLen >64) {
					IsErrorOccur = 1;
					free(value);
					goto SendRes;
				}
				apmib_set(MIB_WSC_PSK, (void *)value);
				apmib_set(MIB_WLAN_WPA_PSK, (void *)value);
				free(value);
			}

			i = WSC_AUTH_WPAPSK;
			apmib_set(MIB_WSC_AUTH, (void *)&i);
			
			p_NewInMessage = GetValueFromNameValueList(&data, "Encryption");
			if (p_NewInMessage && !strcmp(p_NewInMessage, "TKIP")){
				wpa_encrypt=WPA_CIPHER_TKIP;
				i = WSC_ENCRYPT_TKIP;
			}else if (p_NewInMessage && !strcmp(p_NewInMessage, "AES")){
				wpa_encrypt=WPA_CIPHER_AES;
				i = WSC_ENCRYPT_AES;
			}else{
				wpa_encrypt=WPA_CIPHER_MIXED;
				i = WSC_ENCRYPT_TKIPAES;
			}
			apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_encrypt);
			apmib_set(MIB_WSC_ENC, (void *)&i);
			WPSsetAuth_Enc(WSC_AUTH_WPAPSK, i, 0);
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		}else if (SecMode == ENCRYPT_WPA2 && wpa_auth ==WPA_AUTH_PSK) {
			char *value;
			int wpa_encrypt;
			value = mini_UPnPGetFirstElement(h, strlen(h),"Key", 3);
			//p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			//if (p_NewInMessage) {
			if (value) {
				int TotalLen;
				DEBUG_PRINT("SetWLanSecurity, key [%s]\n", value);
				DealWithHNAPKey(value);
				DEBUG_PRINT("SetWLanSecurity, key after parse [%s]\n", value);
				TotalLen = strlen(value);
				if (TotalLen < 8 || TotalLen >64) {
					IsErrorOccur = 1;
					free(value);
					goto SendRes;
				}
				apmib_set(MIB_WSC_PSK, (void *)value);
				apmib_set(MIB_WLAN_WPA_PSK, (void *)value);
				free(value);
			}

			i = WSC_AUTH_WPAPSK;
			apmib_set(MIB_WSC_AUTH, (void *)&i);
			
			p_NewInMessage = GetValueFromNameValueList(&data, "Encryption");
			if (p_NewInMessage && !strcmp(p_NewInMessage, "TKIP")){
				wpa_encrypt=WPA_CIPHER_TKIP;
				i = WSC_ENCRYPT_TKIP;
			}else if (p_NewInMessage && !strcmp(p_NewInMessage, "AES")){
				wpa_encrypt=WPA_CIPHER_AES;
				i = WSC_ENCRYPT_AES;
			}else{
				wpa_encrypt=WPA_CIPHER_MIXED;
				i = WSC_ENCRYPT_TKIPAES;
			}
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_encrypt);
			apmib_set(MIB_WSC_ENC, (void *)&i);
			WPSsetAuth_Enc(WSC_AUTH_WPAPSK, i, 0);
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		}else if (SecMode == ENCRYPT_WPA2_MIXED && wpa_auth ==WPA_AUTH_PSK) {
			char *value;
			int wpa_encrypt;
			int wpa2_encrypt;
			value = mini_UPnPGetFirstElement(h, strlen(h),"Key", 3);
			//p_NewInMessage = GetValueFromNameValueList(&data, "Key");
			//if (p_NewInMessage) {
			if (value) {
				int TotalLen;
				DEBUG_PRINT("SetWLanSecurity, key [%s]\n", value);
				DealWithHNAPKey(value);
				DEBUG_PRINT("SetWLanSecurity, key after parse [%s]\n", value);
				TotalLen = strlen(value);
				if (TotalLen < 8 || TotalLen >64) {
					IsErrorOccur = 1;
					free(value);
					goto SendRes;
				}
				apmib_set(MIB_WSC_PSK, (void *)value);
				apmib_set(MIB_WLAN_WPA_PSK, (void *)value);
				free(value);
		}
		
			i = WSC_AUTH_WPAPSK;
			apmib_set(MIB_WSC_AUTH, (void *)&i);

			p_NewInMessage = GetValueFromNameValueList(&data, "Encryption");
			if (p_NewInMessage && !strcmp(p_NewInMessage, "TKIPORAES")){
				wpa_encrypt=WPA_CIPHER_TKIP;
				wpa2_encrypt=WPA_CIPHER_AES;
				i = WSC_ENCRYPT_TKIPAES;
				apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_encrypt);
				apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2_encrypt);
				apmib_set(MIB_WSC_ENC, (void *)&i);
				WPSsetAuth_Enc(WSC_AUTH_WPAPSK, i, 0);
				
			}
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		}
			p_NewInMessage = GetValueFromNameValueList(&data, "KeyRenewal");
			if (p_NewInMessage){
				rekey = atoi(p_NewInMessage);
					apmib_set(MIB_WLAN_WPA_GROUP_REKEY_TIME, (void *)&rekey);	
			}else {
				IsErrorOccur = 7;
				goto SendRes;
			}



		p_NewInMessage = GetValueFromNameValueList(&data, "RadiusIP1");
		if (p_NewInMessage) {
			p_NewInMessage_2 = GetValueFromNameValueList(&data, "RadiusPort1");
			if (!p_NewInMessage_2) {
				IsErrorOccur = 8;
				goto SendRes;
			}
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 8;
				goto SendRes;
			}
			apmib_set(MIB_WLAN_RS_IP1, (void *)&ip);	
			i = atoi(p_NewInMessage_2);
			apmib_set(MIB_WLAN_RS_PORT1, (void *)&i);
			i = WPA_AUTH_AUTO;
			apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
			//i = 1;
			//apmib_set(MIB_WSC_DISABLE, (void *)&i);
			p_NewInMessage_3 = GetValueFromNameValueList(&data, "RadiusSecret1");
			if(p_NewInMessage_3){
				sprintf(key, "%s",p_NewInMessage_3); 
				apmib_set(MIB_WLAN_RS_PASSWORD1, (void *)key);
			}else{
				IsErrorOccur = 6;
				goto SendRes;
			}
			
			
		}
		else {
			ip = 0;
			apmib_set(MIB_WLAN_RS_IP1, (void *)&ip);	
			i = 0;
			apmib_set(MIB_WLAN_RS_PORT1, (void *)&i);
			i = WPA_AUTH_PSK;
			apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
			//i = 0;
			//apmib_set(MIB_WSC_DISABLE, (void *)&i);
		}
		p_NewInMessage = GetValueFromNameValueList(&data, "RadiusIP2");
		if (p_NewInMessage) {
			p_NewInMessage_2 = GetValueFromNameValueList(&data, "RadiusPort2");
			if (!p_NewInMessage_2) {
				IsErrorOccur = 8;
				goto SendRes;
			}
			if ((ip = inet_addr(p_NewInMessage)) == -1) {
				IsErrorOccur = 8;
				goto SendRes;
			}
			apmib_set(MIB_WLAN_RS_IP2, (void *)&ip);	
			i = atoi(p_NewInMessage_2);
			apmib_set(MIB_WLAN_RS_PORT2, (void *)&i);
			
			p_NewInMessage_3 = GetValueFromNameValueList(&data, "RadiusSecret2");
			if(p_NewInMessage_3){
				sprintf(key, "%s",p_NewInMessage_3); 
				apmib_set(MIB_WLAN_RS_PASSWORD2, (void *)key);
			}else{
				IsErrorOccur = 6;
				goto SendRes;
			}
			
		}
		else {
			ip = 0;
			apmib_set(MIB_WLAN_RS_IP2, (void *)&ip);	
			i = 0;
			apmib_set(MIB_WLAN_RS_PORT2, (void *)&i);
		}
	}
	else {
		SecMode = ENCRYPT_DISABLED;
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&SecMode);
		if (SecMode != OldSecMode)
			IsConfigured = 1;
		SecMode = WSC_AUTH_OPEN;
		apmib_set(MIB_WSC_AUTH, (void *)&SecMode);
		SecMode = WSC_ENCRYPT_NONE;
		apmib_set(MIB_WSC_ENC, (void *)&SecMode);
		i = WPA_AUTH_PSK;
		apmib_set(MIB_WLAN_WPA_AUTH, (void *)&i);
		apmib_set(MIB_WSC_PSK, (void *)clear_mib);
		apmib_set(MIB_WLAN_WPA_PSK, (void *)clear_mib);
		//i = 0;
		//apmib_set(MIB_WSC_DISABLE, (void *)&i);
	}
	

	if (IsConfigured)
		apmib_set(MIB_WSC_CONFIGURED, (void *)&IsConfigured);
	
	if (router_status == ROUTER_READY) {
		router_status = ROUTER_REBOOT;
		strcpy(priv.body,"<SetWLanRadioSecurityResult>REBOOT</SetWLanRadioSecurityResult>");
	}
	else
		IsErrorOccur = 1;

SendRes:
	SendResponse(h, IsErrorOccur, NeedReinitMib, wp);
	ClearNameValueList(&data);
}
static const struct _soapMethods_hnap soapMethods[] = 
{
	
	{ "GetDeviceSettings", GetDeviceSettingsResponse },
	{ "SetDeviceSettings", SetDeviceSettingsResponse },
	{ "IsDeviceReady", IsDeviceReadyResponse },

	{ "GetWanSettings", GetWanSettingsResponse },
	{ "SetWanSettings", SetWanSettingsResponse },
	{ "GetRouterLanSettings", GetRouterLanSettingsResponse },
	{ "SetRouterLanSettings", SetRouterLanSettingsResponse },
	{ "GetWLanSettings24", GetWLanSettings24Response },
	{ "SetWLanSettings24", SetWLanSettings24Response },
	{ "GetWLanSecurity", GetWLanSecurityResponse },
	{ "SetWLanSecurity", SetWLanSecurityResponse },
	{ "Reboot", RebootResponse },
	{ "RenewWanConnection", RenewWanConnectionResponse},
	{ "GetWLanRadios", GetWLanRadiosResponse},
	{ "GetWLanRadioSettings", GetWLanRadioSettingsResponse},
	{ "SetWLanRadioSettings", SetWLanRadioSettingsResponse},
	{ "GetWLanRadioSecurity", GetWLanRadioSecurityResponse},
	{ "SetWLanRadioSecurity", SetWLanRadioSecurityResponse},
#if 0		
	{ "GetMACFilters2", GetMACFilters2Response },
	{ "SetMACFilters2", SetMACFilters2Response },
	{ "AddPortMapping", AddPortMappingResponse },
	{ "DeletePortMapping", DeletePortMappingResponse },
	{ "GetPortMappings", GetPortMappingsResponse},
	{ "GetConnectedDevices", GetConnectedDevicesResponse},
	{ "GetNetworkStats", GetNetworkStatsResponse},
	{ "GetWanStatus", GetWanStatusResponse},
#endif
	{ 0, 0 }
};




void formSetHNAP11(request *wp, char * path, char * query)
{
	int i;
	int len;
	//printf("reach formSetHNAP11:data=%s\n", wp->post_data);
	
	//printf("the action==%s\n", wp->soap_Action);
	
	
	i = 0;
	while(soapMethods[i].methodName){
		len = strlen(soapMethods[i].methodName);
		if(strncmp(wp->soap_Action, soapMethods[i].methodName, len) == 0){
			//printf("found match for process method%s\n", soapMethods[i].methodName);
			soapMethods[i].methodImpl(wp->post_data, wp);
			break;
		}
	i++;
	}
	apmib_update(CURRENT_SETTING);
	if (router_status == ROUTER_REBOOT) { //means we response the REBOOT to utility
			router_status = ROUTER_READY;
		}
		else if (router_status == ROUTER_REINIT) {//means we get reboot command from utility, so we do software re-init
				run_init_script("all");
			router_status = ROUTER_READY;
		}
		
	return;
}

int formGetHNAP11(request *wp, int argc, char **argv)
{
		//char buffer[100];
	//char tmpBuf[100];
	//int i;
	//int len;
	soapMethods[isSetIndex_id].methodImpl(NULL, wp);
			
		if (router_status == ROUTER_REBOOT) {
			router_status = ROUTER_READY;
		}
		
		else if (router_status == ROUTER_REINIT) {
			router_status = ROUTER_READY;
		}
	return 0;

	
	
}


#endif








