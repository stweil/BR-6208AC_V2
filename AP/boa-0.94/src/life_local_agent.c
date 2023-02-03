
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>
#include "agent.h"
#include <jansson.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
/* Local include files */
#include "apmib.h"
#include "mibtbl.h"
#include "utility.h"

//#include "../../../define/C_DEFINE"

#  define DUAL_BRIDGE


static unsigned char g_freeMacId[MAC_ID_LEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static unsigned char g_myMacId[MAC_ID_LEN] = { 0 };
#ifdef DUAL_BRIDGE
static unsigned char g_myMacId5g[MAC_ID_LEN] = { 0 };
int g_Gband_RepON=0, g_Aband_RepON=0;
#if defined(_CROSSBAND_)
int g_crossband_enable=0;
#endif
#endif
#if defined(_WIFI_ROMAING_)
int singlesignon;
#endif
static int startLifeAgent();

int life_local_agent() {
	startLifeAgent();
	return 0;
}

static struct {
	char mac[20];
	#ifdef DUAL_BRIDGE
	char mac5g[20];
	#endif
	char fwversion[10];
	long webport;
	char model[40];
	char vendor[60];
	long pnv;
	char protocolversion[10];
#if defined(_WIFI_ROMAING_)
	char roamingmode[2];
	char roamingenable[2];
#endif
} g_devInfo;



//=============== function header ===============
static int getMacID(char *iface, void *hwaddr);
static int getIPAddr(char *iface, void *intaddr);
static int getSubnetMask(char *iface, void *intaddr);

unsigned char choose_random_shiftnum() {
	FILE *fp;
	unsigned int num;
	unsigned char n;
	if ((fp = fopen("/dev/urandom", "r")) == NULL) {
		num = (int) time(NULL);
	} else {
		fread(&num, sizeof(num), 1, fp);
		fclose(fp);
	}
	n = (num %= 7);
	n++;
	return (unsigned char) n;
}

// Right-rotate a byte n places.
unsigned char rrotate(unsigned char val, int n) {
	int i;
	unsigned int t;
	t = val;
	// First, move the value 8 bits higher.
	t = t << 8;
	for (i = 0; i < n; i++) {
		t = t >> 1;
		/* If a bit shifts out, it will be in bit 7
		 of the integer t. If this is the case,
		 put that bit on the left side. */
		if (t & 128)
			t = t | 32768; // put a 1 on left end
	}
	/* Finally, move the result back to the
	 lower 8 bits of t. */
	t = t >> 8;
	return t;
}

// Left-rotate a byte n places.
unsigned char lrotate(unsigned char val, int n) {
	unsigned int t;
	int i;
	t = val;
	for (i = 0; i < n; i++) {
		t = t << 1;
		/* If a bit shifts out, it will be in bit 8
		 of the integer t. If this is the case,
		 put that bit on the right side. */
		if (t & 256)
			t = t | 1; // put a 1 on the right end
	}
	return t; // return the lower 8 bits.
}

//encode bytes by right-rotate every byte
void rEncodeBytes(unsigned char *bytes, int len, int n) {
	int i;
	bytes[0] += n;
	for (i = 1; i < len; i++) {
		bytes[i] = rrotate(bytes[i], n);
	}
}

//decode bytes by left-rotate every byte
void lDecodeBytes(unsigned char *bytes, int len, int n) {
	int i;
	bytes[0] -= n;
	for (i = 1; i < len; i++) {
		bytes[i] = lrotate(bytes[i], n);
	}
}

//==================== function ====================
static inline int getMacID(char *iface,void *hwaddr) {
	return (getInAddr(iface, HW_ADDR, hwaddr) ? 0 : 1);
}
static inline int getIPAddr(char *iface,void *intaddr) {
	return (getInAddr(iface, IP_ADDR, intaddr) ? 0 : 1);
}
static inline int getSubnetMask(char *iface,void *intaddr) {
	return (getInAddr(iface, SUBNET_MASK, intaddr) ? 0 : 1);
}
//==================================================
static int updateSP(LIFE_STRU *pLifeStru, int *len,char *iface) {
	char buf[300] = { 0 };
	struct in_addr addrIp, addrSubnet;
	char *jsonStr = NULL;
	unsigned char buffer[100];
	FILE *fp;
	*len = 0;
	json_t *jsonRoot = json_object();
	bzero(pLifeStru, sizeof(LIFE_STRU));

	if ( !apmib_init()) {
		printf("Initialize AP MIB failed!\n");
		return -1;
	}
	apmib_get( MIB_WLAN_SSID, (void *)&buffer);
	json_object_set_new(jsonRoot, "search.name", json_string(buffer));
	//search.ip
	bzero(buf, sizeof(buf));
	if (getIPAddr(iface, &addrIp) == 0) {
		strcpy(buf, inet_ntoa(addrIp));
	} else {
		printf("<---Get IP failed--->");
		return -1;
	}
	json_object_set_new(jsonRoot, "search.ip", json_string(buf));
	#ifdef DUAL_BRIDGE
	if(!strcmp("br0", iface)){
 		if(g_Gband_RepON)
			json_object_set_new(jsonRoot, "search.mac", json_string(g_devInfo.mac));
		else
			json_object_set_new(jsonRoot, "search.mac", json_string(g_devInfo.mac5g));
	}
	else 
	#endif
		json_object_set_new(jsonRoot, "search.mac", json_string(g_devInfo.mac));
	json_object_set_new(jsonRoot, "search.fwversion", json_string(g_devInfo.fwversion));
	json_object_set_new(jsonRoot, "search.webport", json_integer(g_devInfo.webport));
	json_object_set_new(jsonRoot, "search.model", json_string(g_devInfo.model));
	// Get Subnet
	bzero(buf, sizeof(buf));
	if (getSubnetMask(iface, &addrSubnet) == 0) {
		strcpy(buf, inet_ntoa(addrSubnet));
	} else {
		printf("<---Get Subnet failed!--->");
		return -1;
	}
	json_object_set_new(jsonRoot, "search.subnet", json_string(buf));

	// Get Gateway
	sprintf(buffer, "/tmp/%s_gw", iface);
	if ((fp = fopen(buffer,"r"))!=NULL) {
		fgets(buf, 30, fp);
		fclose(fp);
	}
	else {
		printf("<---Get Gateway failed!--->");
		return -1;
	}
	
	json_object_set_new(jsonRoot, "search.gateway", json_string(buf));
	json_object_set_new(jsonRoot, "search.vendor", json_string(g_devInfo.vendor));
	json_object_set_new(jsonRoot, "search.pnv", json_integer(g_devInfo.pnv));
	json_object_set_new(jsonRoot, "search.protocolversion", json_string(g_devInfo.protocolversion));
#if defined(_WIFI_ROMAING_)
	json_object_set_new(jsonRoot, "search.roamingmode", json_string(g_devInfo.roamingmode));
	json_object_set_new(jsonRoot, "search.roamingenable", json_string(g_devInfo.roamingenable));
#endif	
	jsonStr = json_dumps(jsonRoot, 0);
	strcpy(pLifeStru->data, jsonStr);
	free(jsonStr);
	json_decref(jsonRoot);
	*len = strlen((char *) pLifeStru->data);
	sprintf(buf, "%04d", *len);
	memcpy(pLifeStru->Header.DataLen, buf, sizeof(pLifeStru->Header.DataLen));
	rEncodeBytes((unsigned char *) pLifeStru->data, *len, choose_random_shiftnum());
#if 0
	{
		char lenStr[5] = {0}, jsonStr[sizeof(pLifeStru->data)] = {0};
		memcpy(lenStr, pLifeStru->Header.DataLen, 4);
		memcpy(jsonStr, pLifeStru->data, atoi(lenStr));
#ifdef _ENCRYPT_SHIFT_BYTES_
		lDecodeBytes((unsigned char *) jsonStr, atoi(lenStr),
				(int) ((unsigned char) jsonStr[0] - '{'));
#endif
		DBG(LOG_DEBUG, "str=%s", jsonStr);
	}
#endif
	// Set My MacID
	#ifdef DUAL_BRIDGE
	if(!strcmp("br0", iface)){
		if(g_Gband_RepON)
			memcpy(pLifeStru->Header.MacID, g_myMacId, MAC_ID_LEN);
		else
			memcpy(pLifeStru->Header.MacID, g_myMacId5g, MAC_ID_LEN);
	}
	else
	#endif
		memcpy(pLifeStru->Header.MacID, g_myMacId, MAC_ID_LEN);

	pLifeStru->Header.FuncType = RESPONSE;
	pLifeStru->Header.Function = SEARCH_UNICAST;
	pLifeStru->Header.CheckSum[0] = 0xff - pLifeStru->Header.FuncType;
	pLifeStru->Header.CheckSum[1] = 0xff - pLifeStru->Header.Function;
	return 0;
}

static int initSP() {
	FILE *fp;
	bzero(&g_devInfo, sizeof(g_devInfo));
	if ((fp = fopen("/etc/version","r"))!=NULL){
		fgets(g_devInfo.fwversion, sizeof(g_devInfo.fwversion), fp);
		fclose(fp);
	}
	//unsigned char buffer[100];
	//apmib_get( MIB_WLAN_SSID, (void *)&buffer);

	#ifdef DUAL_BRIDGE
	if(g_Gband_RepON)
	#endif
		sprintf(g_devInfo.mac, "%02X%02X%02X%02X%02X%02X", g_myMacId[0], g_myMacId[1], g_myMacId[2], g_myMacId[3], g_myMacId[4], g_myMacId[5]);
	#ifdef DUAL_BRIDGE
	if(g_Aband_RepON)
		sprintf(g_devInfo.mac5g, "%02X%02X%02X%02X%02X%02X", g_myMacId5g[0], g_myMacId5g[1], g_myMacId5g[2], g_myMacId5g[3], g_myMacId5g[4], g_myMacId5g[5]);
	#endif
#if defined(_EW7478AC_)
	#if defined(_EW7478ACR_)
		strcpy(g_devInfo.model, "EW-7478ACR");
	#elif defined(_EW7478WAP_)
		strcpy(g_devInfo.model, "EW-7478WAP");
	#else
		strcpy(g_devInfo.model, "EW-7438AC");
	#endif
#endif

#if defined(_WIFI_ROMAING_)
	apmib_get( MIB_SINGLESIGNON, (void *)&singlesignon);
	if(singlesignon == 0)	//WiFiromaing not support
	{
		strcpy(g_devInfo.roamingenable, "0");	//close WiFiromaing
		strcpy(g_devInfo.roamingmode, "2");	//slave(default mode)
	}
	else if(singlesignon == 1)
	{
		strcpy(g_devInfo.roamingenable, "1");	//open WiFiromaing
		strcpy(g_devInfo.roamingmode, "1");	//master
	}
	else if(singlesignon == 2)
	{
		strcpy(g_devInfo.roamingenable, "1");	//open WiFiromaing
		strcpy(g_devInfo.roamingmode, "2");	//slave
	}
	else
	{
		strcpy(g_devInfo.roamingenable, "0");	//close WiFiromaing
		strcpy(g_devInfo.roamingmode, "2");	//slave(default mode)
	}
#endif
	g_devInfo.pnv = 0;
	strcpy(g_devInfo.protocolversion, PROTOCOL_VER);
	strcpy(g_devInfo.vendor, "EDIMAX");
	g_devInfo.webport = WEBSERVER_PORT;
	printf("Agent initSP{\nfwversion\t%s\nmac\t\t%s\nmac5g\t\t%s\nmodel\t\t%s\nvendor\t\t%s\n}\n",g_devInfo.fwversion,g_devInfo.mac,g_devInfo.mac5g,g_devInfo.model,g_devInfo.vendor);
	return 0;
}

static void dispatch(unsigned char *zpBuf, int iLen, int nSockfd, struct sockaddr_in *pInAddrSrc, LIFE_STRU *pLifeStru, char *iface) {
	int dataLen = 0;
	if (iLen >= sizeof(LIFE_HEADER)) {
		PLIFE_HEADER aHeader = (PLIFE_HEADER) zpBuf;
		printf("<---UDP packet from %s (port:%d)--->\n", inet_ntoa(pInAddrSrc->sin_addr), ntohs(pInAddrSrc->sin_port));
		if ((aHeader->CheckSum[0] != (0xff - aHeader->FuncType)) || (aHeader->CheckSum[1] != (0xff - aHeader->Function))
				|| (aHeader->FuncType != REQUEST)) {
			printf("<---Error CheckSum or FuncType=%x--->", aHeader->FuncType);
			return;
		}
		if (aHeader->Function == SEARCH_UNICAST) {
			// check MAC ID
			#ifdef DUAL_BRIDGE
			if(!strcmp("br0", iface)){
				if(g_Gband_RepON){
					if (memcmp(aHeader->MacID, g_freeMacId, MAC_ID_LEN) != 0 && memcmp(aHeader->MacID, g_myMacId, MAC_ID_LEN) != 0) {
						printf("<---MacID %02x:%02x:%02x:%02x:%02x:%02x != ff:ff:ff:ff:ff:ff or %02x:%02x:%02x:%02x:%02x:%02x--->", aHeader->MacID[0],aHeader->MacID[1], aHeader->MacID[2], aHeader->MacID[3], aHeader->MacID[4], aHeader->MacID[5], g_myMacId[0], g_myMacId[1],g_myMacId[2], g_myMacId[3], g_myMacId[4], g_myMacId[5]);
						return;
					}
				}
				else{
					if (memcmp(aHeader->MacID, g_freeMacId, MAC_ID_LEN) != 0 && memcmp(aHeader->MacID, g_myMacId5g, MAC_ID_LEN) != 0) {
						printf("<---MacID %02x:%02x:%02x:%02x:%02x:%02x != ff:ff:ff:ff:ff:ff or %02x:%02x:%02x:%02x:%02x:%02x--->", aHeader->MacID[0],aHeader->MacID[1], aHeader->MacID[2], aHeader->MacID[3], aHeader->MacID[4], aHeader->MacID[5], g_myMacId5g[0], g_myMacId5g[1],g_myMacId5g[2], g_myMacId5g[3], g_myMacId5g[4], g_myMacId5g[5]);
						return;
					}
				}
			}
			else{
			#endif
				if (memcmp(aHeader->MacID, g_freeMacId, MAC_ID_LEN) != 0 && memcmp(aHeader->MacID, g_myMacId, MAC_ID_LEN) != 0) {
					printf("<---MacID %02x:%02x:%02x:%02x:%02x:%02x != ff:ff:ff:ff:ff:ff or %02x:%02x:%02x:%02x:%02x:%02x--->", aHeader->MacID[0],aHeader->MacID[1], aHeader->MacID[2], aHeader->MacID[3], aHeader->MacID[4], aHeader->MacID[5], g_myMacId[0], g_myMacId[1],g_myMacId[2], g_myMacId[3], g_myMacId[4], g_myMacId[5]);
					return;
				}
			#ifdef DUAL_BRIDGE
			}
			#endif

			if (updateSP(pLifeStru, &dataLen, iface) == 0) {
				if (sendto(nSockfd, pLifeStru, sizeof(LIFE_HEADER) + dataLen, 0, (struct sockaddr *) pInAddrSrc, sizeof(struct sockaddr_in)) <= 0) {
					printf("<---sendto() failed(%s)--->", strerror(errno));
				}
			}

		} else if (aHeader->Function == SEARCH_BROADCAST) {
			//TODO
			printf("<---TODO--->");
		} else if (aHeader->Function == SEARCH_SETUP) {
			//TODO
			printf("<---TODO--->");
		} else {
			printf("<---Error Function %x--->", aHeader->Function);
			//DBG(LOG_ERR, "Error Function %x", aHeader->Function);
		}
	}
}
#if 0
static int getInAddr(char *interface, ADDR_T type, void *pAddr) {
	struct ifreq ifr;
	int skfd, iFound = 0;
	struct sockaddr_in *addr;
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, interface);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		printf("<---IOCTL failed(%s)--->", strerror(errno));
		//DBG(LOG_ERR, "IOCTL failed(%s)", strerror(errno));
		return 0;
	}
	if (type == HW_ADDR) {
		if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
			memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
			iFound = 1;
		} else {
			printf("<---SIOCGIFHWADDR failed(%s)--->", strerror(errno));
			//DBG(LOG_ERR, "SIOCGIFHWADDR failed(%s)", strerror(errno));
		}
	} else if (type == IP_ADDR) {
		if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
			addr = ((struct sockaddr_in *) &ifr.ifr_addr);
			*((struct in_addr *) pAddr) = *((struct in_addr *) &addr->sin_addr);
			iFound = 1;
		}
	} else if (type == SUBNET_MASK) {
		if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
			addr = ((struct sockaddr_in *) &ifr.ifr_addr);
			*((struct in_addr *) pAddr) = *((struct in_addr *) &addr->sin_addr);
			iFound = 1;
		}
	}
	close(skfd);
	return iFound;
}
#endif
static int initSocket(char *iface) {
	int sock, iOpt = 1;
	struct ifreq interface;
	struct sockaddr_in addr;
	//set socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("<---socket() failed(%s)--->", strerror(errno));
		return -1;
	}
	//set socket broadcast address
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &iOpt, sizeof(iOpt)) < 0) {
		printf("<---setsockopt() failed(%s)--->", strerror(errno));
		close(sock);
		return -1;
	}
	//set socket reuse address
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &iOpt, sizeof(iOpt)) < 0) {
		printf("<---setsockopt() failed(%s)--->", strerror(errno));
		close(sock);
		return -1;
	}
	//set interface
	strncpy(interface.ifr_ifrn.ifrn_name, iface, IFNAMSIZ);
	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *) &interface, sizeof(interface)) < 0) {
		printf("<---setsockopt() failed(%s)--->", strerror(errno));
		close(sock);
		return -1;
	}
	//init local address
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(LIFE_UDP_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	//bind socket
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		printf("<---bind() failed(%s)--->", strerror(errno));
		close(sock);
		return -1;
	}
	return sock;
}

static int startLifeAgent() {
	#ifdef DUAL_BRIDGE
	int nBr1Sock, nBrSock, ret;
	#else
	int nBrSock, ret;
	#endif
	int iInAddrLen;
	struct sockaddr_in inAddrSrc;
	struct sockaddr hwaddr;
	struct timeval timeout;
	fd_set readfds;
	LIFE_STRU lifeStru;
	unsigned char zsRecv_Buffer[MAX_BROADCAST_STRING_SIZE];
	signal(SIGPIPE, SIG_IGN);

	#ifdef DUAL_BRIDGE
	if ( !apmib_init()) {
		printf("Initialize AP MIB failed!\n");
		return -1;
	}
	apmib_get( MIB_REPEATER_ENABLED, (void *)&g_Gband_RepON);
	apmib_get( MIB_INIC_REPEATER_ENABLED, (void *)&g_Aband_RepON);
	
	apmib_get( MIB_CROSSBAND_ENABLE, (void *)&g_crossband_enable);
	printf("G Band Repeater Enable:\t\t%d\nA Band Repeater Enable:\t\t%d\ncrossband Enable:\t%d\n", g_Gband_RepON, g_Aband_RepON, g_crossband_enable);
	#endif

	//set socket
	#ifdef DUAL_BRIDGE
	if(g_Gband_RepON && g_Aband_RepON){
		if ((nBr1Sock = initSocket("br1")) < 0) {
			return -1;
		}
	}
	#endif
	if ((nBrSock = initSocket("br0")) < 0) {
		return -1;
	}
	
	printf("Agent set socket done\n");
	// Get My MacID
	#ifdef DUAL_BRIDGE
	if(g_Gband_RepON){
	#endif
		if (getMacID("apcli0", &hwaddr) == 0) { //only be apcli0
			memcpy(g_myMacId, hwaddr.sa_data, MAC_ID_LEN);
		} else {
			printf("<---Get apcli0 Mac ID fail!--->");
		}
	#ifdef DUAL_BRIDGE
	}
	if(g_Aband_RepON){
		if (getMacID("apclii0", &hwaddr) == 0) { //only be apclii0
			memcpy(g_myMacId5g, hwaddr.sa_data, MAC_ID_LEN);
		} else {
			printf("<---Get apclii0 Mac ID fail!--->");
		}
	}
	#endif
	printf("Agent set MacID done\n");

	initSP();

	while (1) {
		FD_ZERO(&readfds);
		#ifdef DUAL_BRIDGE
		if(g_Gband_RepON && g_Aband_RepON)
			FD_SET(nBr1Sock, &readfds);
		else
			nBr1Sock = -1;
		#endif
		FD_SET(nBrSock, &readfds);
		//timeout.tv_sec = 0;
		//timeout.tv_usec = 50000;
		#ifdef DUAL_BRIDGE
		if ((ret = select(nBr1Sock > nBrSock ? nBr1Sock + 1 : nBrSock + 1, &readfds, NULL, NULL, NULL)) > 0) {
		#else
		if ((ret = select(nBrSock + 1, &readfds, NULL, NULL, NULL)) > 0) {
		#endif
			#ifdef DUAL_BRIDGE
			if(g_Gband_RepON && g_Aband_RepON){
				if (FD_ISSET(nBr1Sock, &readfds)) {
					iInAddrLen = sizeof(struct sockaddr_in); //to prevent getting the IP whose value is 0.0.0.0
					ret = recvfrom(nBr1Sock, zsRecv_Buffer, MAX_BROADCAST_STRING_SIZE, 0, (struct sockaddr *) &inAddrSrc, (socklen_t *) &iInAddrLen);
					if (ret > 0) {
						dispatch(zsRecv_Buffer, ret, nBr1Sock, &inAddrSrc, &lifeStru, "br1");
					} else {
						printf("<---recvfrom() failed(%s)--->", strerror(errno));
					}
				} 
			}
			//else {
			#endif
				if (FD_ISSET(nBrSock, &readfds)) {
					iInAddrLen = sizeof(struct sockaddr_in); //to prevent getting the IP whose value is 0.0.0.0
					ret = recvfrom(nBrSock, zsRecv_Buffer, MAX_BROADCAST_STRING_SIZE, 0, (struct sockaddr *) &inAddrSrc, (socklen_t *) &iInAddrLen);
					if (ret > 0) {
						dispatch(zsRecv_Buffer, ret, nBrSock, &inAddrSrc, &lifeStru, "br0");
					} else {
						printf("<---recvfrom() failed(%s)--->", strerror(errno));
					}
				}
			#ifdef DUAL_BRIDGE
			//}
			#endif
		}
	}

	return 0;
}

