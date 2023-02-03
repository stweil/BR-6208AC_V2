#ifndef _MYTEST_H
#define _MYTEST_H

#include <linux/ip.h>
#include <linux/udp.h>
#include "cmdx.h"

struct packet_header 
{
	struct iphdr    ip;
	struct udphdr   udp;
};

typedef enum 
{ 
    IP_ADDR, 
    SUBNET_MASK, 
    DEFAULT_GATEWAY, 
    HW_ADDR, 
    DHCPGATEWAYIP_ADDR, 
    DHCPNAMESERVER_ADDR 
} ADDR_T;

void dispatch(unsigned char *zpBuf, int nLen, int nSockfd, struct sockaddr_in *addrSender);
int searchAP(struct sockaddr_in *addrSender);
unsigned short checksum(unsigned short *buffer, int size);
int senddata(void *buf, int nLen, struct sockaddr_in *addrSender);
int getInAddr( char *interface, ADDR_T type, void *pAddr );
int getMacID(void *hwaddr);
int getIPAddr(void *intaddr);
int getSubnetMask(void *intaddr);

#endif

