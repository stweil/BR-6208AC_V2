#include <memory.h>
#include <ctype.h>

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <sys/ioctl.h>
//#include <signal.h>
//#include <sys/ioctl.h>
#include <net/if.h>
//#include <linux/wireless.h>
//#include <dirent.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "agent.h"

//#define WLAN_IF     "eth2"
//#define WLAN_IF     "wlan0"

//#define WLAN_IF     "ra0"
#define BRIDGE_IF     "br0"
//#define SIOCGIWRTLGETBSSDB          0x8B34	// get bss data base
//#define SIOCGIWRTLSCANREQ           0x8B33	// scan request
//#define SIOCGIWNAME                 0x8B01    /* get name == wireless protocol */
//#define SIOCGIFFLAGS                0x8913	/* get flags			         */
//#define SIOCGIFHWADDR               0x8927	/* Get hardware address		     */
//#define SIOCGIFADDR                 0x8915	/* get PA address		         */

//#define SIOCGIWRTLSTAINFO           0x8B30    // get station table information
//#define SIOCGIWRTLSTANUM            0x8B31    // get the number of stations in table
//#define SIOCGIWRTLJOINREQ           0x8B35    // join request
//#define SIOCGIWRTLJOINREQSTATUS     0x8B36    // get status of join request
//#define SIOCGIWRTLGETBSSINFO        0x8B37    // get currnet bss info
//#define SIOCGIWRTLGETWDSINFO        0x8B38
//#define SIOCGIWRTLSTAINFO_NEW       0x8B50

//#define SIOCDEVPRIVATE              0x8BE0
//#define SIOCIWFIRSTPRIV             SIOCDEVPRIVATE
//#define RTPRIV_IOCTL_GET_MAC_TABLE  (SIOCIWFIRSTPRIV + 0x0F)
//#define RTPRIV_IOCTL_GSITESURVEY    (SIOCIWFIRSTPRIV + 0x0D)
//#define RTPRIV_IOCTL_SET            (SIOCIWFIRSTPRIV + 0x02)
//#define OID_802_11_BSSID_LIST       0x0609
//#define RTPRIV_IOCTL_GSITESURVEY    (SIOCIWFIRSTPRIV + 0x0D)

#define MAX_BROADCAST_STRING_SIZE   1514

#define ID_VENDOR   19
// 01=Edimax, 02=.., 03=Asante, 04=InterLink, 05=..-..
// 06=NETmate, 07=MOSION, 08=Planet, 09=Arlotto, 10=EMINENT
// 11=BlackBox, 12=DDC, 13=..-EDIMAX, 14=..-.., 15=..-NETmate
// 16=1stWave, 17=Blitzz, 18=Hawking, 19=Jensen

const unsigned char G_Signature[6] = {'J', 'E', 'N', 'S', 'E', 'N'};
// unsigned char G_MacId[MAC_ID_LEN];

/***************************************************************************/

int main(int argc, char** argv)
{
    int nSockfd;
    int nRecv_Len;
    struct sockaddr_in addr;
    unsigned char zsRecv_Buffer[MAX_BROADCAST_STRING_SIZE];
    FILE *fp;
    struct sockaddr_in addrSender;
    int nSenderAddrSize = sizeof(struct sockaddr_in);

    //check
    fp = fopen("/tmp/agentenabledflag", "r");
    if(fp != NULL) exit(0);
      
    //set socket
    nSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(nSockfd < 0)
    {
        printf("socket() error!\n");
        exit(0);
    }
   
    // Display Version
	printf("Jensen Agent Version:1.0.2.0\n");    
    /*
    ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int));
    if(ret < 0)
    {
        printf("setsockopt() error!\n");
        exit(0);
    }
    */
    
    //init local address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(AGENT_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(nSockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("bind() error!\n");
	    exit(0);
    }
    else
    {
        fp = fopen("/tmp/agentenabledflag", "w");
	    if(fp == NULL)
	    {
	        exit(0);
	    }
	    else
	    {
	        fclose(fp);
	    }
    }
    
    while(1)
    {
        memset(zsRecv_Buffer, 0, MAX_BROADCAST_STRING_SIZE);
        // nRecv_Len = recv(nSockfd, zsRecv_Buffer, MAX_BROADCAST_STRING_SIZE, 0);
        nRecv_Len = recvfrom(nSockfd, zsRecv_Buffer, MAX_BROADCAST_STRING_SIZE, 0, (struct sockaddr *)&addrSender, &nSenderAddrSize);
        if(nRecv_Len > 0)
        {
            dispatch(zsRecv_Buffer, nRecv_Len, nSockfd, &addrSender);
        }
    } 
    return 0;
}
/***************************************************************************/

void dispatch(unsigned char *zpBuf, int nLen, int nSockfd, struct sockaddr_in *addrSender)
{
    //int i;
	unsigned char zsMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    if(HEADER_SIZE > nLen) 
    {
        return;
    }
    
    PSTRU_Header aHeader = (PSTRU_Header)zpBuf;
		
    // check Mac ID
	if (memcmp(aHeader->MacID, zsMAC, MAC_ID_LEN) != 0)
	{
        return;
	}
        
    // Check Signature
    if (memcmp(aHeader->Signature, G_Signature, SIGNATURE_LEN) != 0)
	{
        return;
	}
		
	if((aHeader->CheckSum[0] != (0xff - aHeader->FuncType)) ||
        (aHeader->CheckSum[1] != (0xff - aHeader->Function)) ||
        (aHeader->FuncType != REQUEST))
	{
        return;
	}
    
    if (aHeader->Function == SEARCH_AP)
    {
        searchAP(addrSender);
    }
}

/***************************************************************************/

int searchAP(struct sockaddr_in *addrSender)
{
    STRU_Reply_Search  aReplay;
    struct sockaddr hwaddr;
    struct in_addr addrIP;
    struct in_addr addrSubnet;
    int nIdx;
    FILE *fp;
    char zsLine[80];
    int nLength = 0;
    char *zpGetInfo[2] = {"USER_NAME=", "USER_PASSWORD="};
        
    memset(&aReplay, 0, sizeof(STRU_Reply_Search));

    // Get MacID myself    
    if(getMacID(&hwaddr) == 0)
    {
        memcpy(aReplay.Header.MacID, hwaddr.sa_data, MAC_ID_LEN);
    }
    else
    {
        printf("get Mac ID fail!\n");
    }
    
    // Set Signature
    memcpy(aReplay.Header.Signature, G_Signature, SIGNATURE_LEN);
    
    // Set Vendor ID
    aReplay.VendorID = ID_VENDOR;
    
    // Get IP Addr
    if(getIPAddr(&addrIP) == 0)
    {
        memcpy(aReplay.IPAddr, &addrIP.s_addr, IP_ADDRESS_LEN);
    }
    else
    {
        printf("get IP Address fail!\n");
    }
    
    // Get Subnet
    if(getSubnetMask(&addrSubnet) == 0)
    {
        memcpy(aReplay.Subnet, &addrSubnet.s_addr, IP_ADDRESS_LEN);
    }
    else
    {
        printf("get Subnet fail!\n");
    }
	
    // Set Device Model
    memcpy(aReplay.DeviceModel, "Unknown", sizeof("Unknown"));

#if defined(_Customer_)
    memcpy(aReplay.DeviceModel, "AL3000DG", sizeof("AL3000DG"));
#endif

	//Get Username and password
    system("flash all > /tmp/getinfo.txt");
    fp = fopen("/tmp/getinfo.txt", "r");
    while((fgets(zsLine, 80, fp)) != NULL)
    {
        for(nIdx = 0; nIdx < 2; nIdx++)
	    {
	        if(memcmp(zpGetInfo[nIdx], zsLine, strlen(zpGetInfo[nIdx])) == 0)
	        {
	            if((nLength + strlen(zsLine)) > sizeof(aReplay.Reserve)) break;
		        memcpy(aReplay.Reserve + nLength, zsLine, strlen(zsLine));
		        nLength += strlen(zsLine);
	        }
	    }
    }
    fclose(fp);	
    system("rm /tmp/getinfo.txt");
    
    // Add Sender IP
    sprintf(zsLine, "SENDER_IP_ADDR=%s\n", inet_ntoa(addrSender->sin_addr));
    if((nLength + strlen(zsLine)) < sizeof(aReplay.Reserve)) 
    {
        memcpy(aReplay.Reserve + nLength, zsLine, strlen(zsLine));
        nLength += strlen(zsLine);
    }
        
    aReplay.Header.FuncType = RESPONSE;
    aReplay.Header.Function = SEARCH_AP;
    aReplay.Header.CheckSum[0] = 0xff - aReplay.Header.FuncType;
    aReplay.Header.CheckSum[1] = 0xff - aReplay.Header.Function;
    
    return senddata(&aReplay, sizeof(STRU_Reply_Search), addrSender);
}
/***************************************************************************/

int senddata(void *buf, int nLen, struct sockaddr_in *addrSender)
{
    int fd;
    int result;
    int index;
    struct sockaddr_ll dest;
    struct ifreq ifr;
    struct packet_header *pkt_hdr;
    struct sockaddr_in;
    unsigned char packet[2000];
		
    pkt_hdr = (struct packet_header *)packet;
	
    memset(&ifr, 0, sizeof(ifr));
    memset(&dest, 0, sizeof(dest));
    memset(&packet, 0, 2000);

    if ((fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0)
    {
        perror("socket");
	    return -1;
    }

    strncpy(ifr.ifr_name, BRIDGE_IF, sizeof(IFNAMSIZ));

    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl SIOCGIFINDEX");
	    return -1;
    }
    index = ifr.ifr_ifindex;

    /*
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl SIOCGIFADDR");
	return -1;
    }
    addr = ((struct sockaddr_in *)&ifr.ifr_addr);
    */
	
    dest.sll_family = PF_PACKET;
    dest.sll_protocol = htons(ETH_P_IP);
    dest.sll_ifindex = index;
    dest.sll_halen = 6;
    memset(dest.sll_addr, 0xff, 6);
	
    pkt_hdr->ip.protocol = IPPROTO_UDP;
    pkt_hdr->ip.saddr = INADDR_ANY; //addr->sin_addr.s_addr;
    pkt_hdr->ip.daddr = INADDR_BROADCAST;
    pkt_hdr->ip.version = 4;
    pkt_hdr->ip.ttl = 128;
    pkt_hdr->ip.tot_len = htons(sizeof(pkt_hdr->udp) + sizeof(pkt_hdr->ip) + nLen);
    pkt_hdr->ip.ihl = sizeof(pkt_hdr->ip) >> 2;
    pkt_hdr->ip.check = 0x0000;
	
    pkt_hdr->udp.source = htons(AGENT_PORT);
    pkt_hdr->udp.dest = addrSender->sin_port;
    pkt_hdr->udp.len = htons(sizeof(pkt_hdr->udp) + nLen);
    pkt_hdr->udp.check = 0x0000;
    
    memcpy(packet + sizeof(struct packet_header), buf, nLen);
	
    pkt_hdr->udp.check = checksum((unsigned short*)&pkt_hdr->udp, sizeof(pkt_hdr->udp) + nLen);
    pkt_hdr->ip.check = checksum((unsigned short*)&pkt_hdr->ip, sizeof(pkt_hdr->udp) + sizeof(pkt_hdr->ip) + nLen);
    pkt_hdr->udp.check = 0x0000;

//#if defined(_7206APG_) || defined(_7206PDG_) || defined(_7209APG_) || defined(_EW7228APN_) // Realtek
//    result = sendto(fd, packet, pkt_hdr->ip.tot_len, 0, (struct sockaddr *) &dest, sizeof(dest));
//#else // Ralink
    result = sendto(fd, packet, (sizeof(pkt_hdr->udp) + sizeof(pkt_hdr->ip) + nLen), 0, (struct sockaddr *)&dest, sizeof(dest));
//#endif
    
    if (result <= 0)
    {
        perror("sendto Error");
    }
	
    close(fd);
    return 0;
}
/***************************************************************************/

unsigned short checksum(unsigned short *buffer, int size)
{ 
    unsigned long cksum = 0;
    while(size > 1)
    {  
        cksum += *buffer ++;
	    size -= sizeof(unsigned short);
    }          
    if(size)
    {
         cksum += *(unsigned char *)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return(unsigned short) (~cksum);
}
/***************************************************************************/

int getInAddr( char *interface, ADDR_T type, void *pAddr )
{
    struct ifreq ifr;
    int skfd, found=0;
    struct sockaddr_in *addr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
	return (0);

    if (type == HW_ADDR) 
    {
    	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) 
    	{
		    memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
		    found = 1;
	    }
    }
    else if (type == IP_ADDR) 
    {
	    if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) 
	    {
		    addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		    *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		    found = 1;
	    }
    }
    else if (type == SUBNET_MASK) 
    {
	    if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) 
	    {
		    addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		    *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		    found = 1;
	    }
    }
    close( skfd );
    return found;
}
/***************************************************************************/

int getMacID(void *hwaddr)
{
    if(BRIDGE_IF && getInAddr(BRIDGE_IF, HW_ADDR, hwaddr))
    {	
        return 0;
    }
    return -1;
}
/***************************************************************************/

int getIPAddr(void *intaddr)
{	
    if(BRIDGE_IF && getInAddr(BRIDGE_IF, IP_ADDR, intaddr))
    {	
        return 0;
    }
    return -1;
}
/***************************************************************************/
int getSubnetMask(void *intaddr)
{
    if(BRIDGE_IF && getInAddr(BRIDGE_IF, SUBNET_MASK, intaddr))
    {	
	    return 0;
    }
    return -1;
}
/***************************************************************************/

