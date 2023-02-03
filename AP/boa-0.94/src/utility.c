/*
 *      Utiltiy function to communicate with TCPIP stuffs
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: utility.c,v 1.1.1.1 2005/03/09 04:52:51 rex Exp $
 *
 */
/*-- System inlcude files --*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <signal.h>
#include <sys/ioctl.h>
//#include <net/if.h>
#include <linux/wireless.h>
#include <dirent.h>
#include <fcntl.h>
/*-- Local include files --*/
#include "apmib.h"
#include "utility.h"

/*-- Local constant definition --*/
#define _PATH_PROCNET_ROUTE	"/proc/net/route"
#define _PATH_PROCNET_DEV	"/proc/net/dev"
#define _PATH_RESOLV_CONF	"/etc/resolv.conf"

/* Keep this in sync with /usr/src/linux/include/linux/route.h */
#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */

#define READ_BUF_SIZE	50

/*-- Local routine declaration --*/
static int get_dev_fields(int type, char *bp, struct user_net_device_stats *pStats);
static char *get_name(char *name, char *p);

/*------------------------------------------------------------------*/
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int
iw_get_ext(int                  skfd,           /* Socket to the kernel */
           char *               ifname,         /* Device name */
           int                  request,        /* WE ID */
           struct iwreq *       pwrq)           /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  	//printf("ioctl ifName = %s",pwrq->ifr_name);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}


/////////////////////////////////////////////////////////////////////////////
int getWlStaNum( char *interface, int *num )
{
    int skfd;
    unsigned short staNum;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
      return -1;

    wrq.u.data.pointer = (caddr_t)&staNum;
    wrq.u.data.length = sizeof(staNum);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTANUM, &wrq) < 0)
	return -1;

    *num  = (int)staNum;

    close( skfd );

    return 0;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
int getInicStaInfo( char *interface,  INIC_STA_INFO_Tp pInfo )
{
	int skfd;
	struct iwreq wrq;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	/* Get wireless name */
	/* If no wireless name : no wireless extensions */
	if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
		return -1;

	wrq.u.data.pointer = (caddr_t)pInfo;
	wrq.u.data.length = sizeof(INIC_STA_INFO_T);
	wrq.u.data.flags=0;
	memset(pInfo, 0, sizeof(INIC_STA_INFO_T));

	if (iw_get_ext(skfd, interface, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq) < 0){
		close (skfd);
		return -1;
	}

	close(skfd);
	return 0;
}
int getWlStaInfo( char *interface,  WLAN_STA_INFO_Tp pInfo )
{
	int skfd;
	struct iwreq wrq;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	/* Get wireless name */
	/* If no wireless name : no wireless extensions */
	if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
		return -1;

	wrq.u.data.pointer = (caddr_t)pInfo;
	wrq.u.data.length = sizeof(WLAN_STA_INFO_T);
	wrq.u.data.flags=0;
	memset(pInfo, 0, sizeof(WLAN_STA_INFO_T));

	if (iw_get_ext(skfd, interface, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq) < 0){
		close (skfd);
		return -1;
	}

	close(skfd);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyResult(char *interface, unsigned char *pStatus, int iSize )
{  
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;
		 memset(pStatus, 0x00, iSize);
    wrq.u.data.pointer = pStatus;
    wrq.u.data.length = iSize;
    wrq.u.data.flags = 0;

    if (iw_get_ext(skfd, interface,RTPRIV_IOCTL_GSITESURVEY, &wrq) < 0)
	return -1;

    close( skfd );

    return wrq.u.data.length;
}

/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyResult_1(char *interface, unsigned char *pStatus  )
{  
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;
		 memset(pStatus, 0x00, sizeof(pStatus));
    wrq.u.data.pointer = pStatus;
    wrq.u.data.length = sizeof(pStatus);
    wrq.u.data.flags = 0;

    if (iw_get_ext(skfd, interface,RTPRIV_IOCTL_GSITESURVEY, &wrq) < 0)
	return -1;

    close( skfd );

    return wrq.u.data.length;
}

/////////////////////////////////////////////////////////////////////////////
int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pBss;
    wrq.u.data.length = sizeof(BssDscr);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLJOINREQ, &wrq) < 0)
	return -1;

    close( skfd );
    
//#ifdef __TARGET_BOARD__	
//    *res = wrq.u.data.pointer[0];
//#endif

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getWlJoinResult(char *interface, unsigned char *res)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)res;
    wrq.u.data.length = 1;

    if (iw_get_ext(skfd, interface, SIOCGIWRTLJOINREQSTATUS, &wrq) < 0)
	return -1;

    close( skfd );

    return 0;
}



/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyRequest(char *interface, char *pStatus)
{
    int skfd;
    struct iwreq wrq;
    unsigned char result;

#if 1	//for hidden SSID. EDX patrick
	FILE *fp1;
	char cmdbuffer[100];
	char cmdbuffer1[100];
	int i;
	unsigned int output_intValue;
	char output_charValue[4]="0x";
	char tmpBuf[100]={0};
#endif

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

		memset(pStatus, 0x00, 255);

#if 1 //for hidden SSID. EDX patrick
		if ((fp1 = fopen("/tmp/SSIDname","r"))!=NULL) {
			fgets(cmdbuffer, 40, fp1);
			fclose(fp1);

			for(i=0;i<(strlen(cmdbuffer)-1);i=i+2) // EDX, Ken 2015_09_010 add, Avoid tmpWepKey is "hidden SSID;systemcode".
			{
				output_charValue[2]=cmdbuffer[i];
				output_charValue[3]=cmdbuffer[i+1];
				output_charValue[4]='\0';
				sscanf(output_charValue,"%x",&output_intValue); // Hexadecimal to Decimal
				sprintf(tmpBuf, "%s%c",tmpBuf,output_intValue);
			}
			sprintf(cmdbuffer1, "SiteSurvey=%s", tmpBuf);
			printf("\n%s\n",cmdbuffer1);
			strcpy(pStatus, cmdbuffer1);
		}
		else
#endif
			strcpy(pStatus, "SiteSurvey=1");


		wrq.u.data.length = strlen(pStatus)+1;
		wrq.u.data.pointer = pStatus;
		wrq.u.data.flags = 0;

    if (iw_get_ext(skfd, interface, RTPRIV_IOCTL_SET, &wrq) < 0)
			return -1;

    close( skfd );
    return 0;
    


}

/////////////////////////////////////////////////////////////////////////////
int getWlBssInfo(char *interface, bss_info *pInfo)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(bss_info);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSINFO, &wrq) < 0)
	return -1;

    close( skfd );

    return 0;
}



/////////////////////////////////////////////////////////////////////////////
int getInAddr( char *interface, ADDR_T type, void *pAddr )
{
    struct ifreq ifr;
    int skfd, found=0;
    struct sockaddr_in *addr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
	return (0);

    if (type == HW_ADDR) {
    	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
		memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
		found = 1;
	}
    }
    else if (type == IP_ADDR) {
	if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    }
    else if (type == SUBNET_MASK) {
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    }
    close( skfd );
    return found;

}

/////////////////////////////////////////////////////////////////////////////
extern pid_t find_pid_by_name( char* pidName)
{
	DIR *dir;
	struct dirent *next;

	dir = opendir("/proc");
	if (!dir) {
		printf("Cannot open /proc");
		return 0;
	}

	while ((next = readdir(dir)) != NULL) {
		FILE *status;
		char filename[READ_BUF_SIZE];
		char buffer[READ_BUF_SIZE];
		char name[READ_BUF_SIZE];

		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;

		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;

		sprintf(filename, "/proc/%s/status", next->d_name);
		if (! (status = fopen(filename, "r")) ) {
			continue;
		}
		if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) {
			fclose(status);
			continue;
		}
		fclose(status);

		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(buffer, "%*s %s", name);
		if (strcmp(name, pidName) == 0) {
		//	pidList=xrealloc( pidList, sizeof(pid_t) * (i+2));
			return((pid_t)strtol(next->d_name, NULL, 0));

		}
	}
	if ( strcmp(pidName, "init")==0)
		return 1;

	return 0;
}


#if 0
/////////////////////////////////////////////////////////////////////////////
int setInAddr(char *interface, ADDR_T type, void *addr)
{
	struct ifreq ifr;
	struct sockaddr_in sin;
	int sock, cmd, ret;

	if ( type == IP_ADDR )
		cmd = SIOCSIFADDR;
	else if ( type == SUBNET_MASK )
		cmd = SIOCSIFNETMASK;
	else
		return -1;

	memset(&ifr, 0, sizeof(struct ifreq));

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, interface);
	memset(&sin, 0, sizeof(struct sockaddr));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = *((unsigned long *)addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
	if ( (ret=ioctl(sock, cmd, &ifr)) < 0)
		goto set_exit;

	// up interface
	strcpy(ifr.ifr_name, interface);
	if ((ret = ioctl(sock, SIOCGIFFLAGS, &ifr)) < 0)
		goto set_exit;

	strcpy(ifr.ifr_name, interface);
	ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
	ret = ioctl(sock, SIOCSIFFLAGS, &ifr);

set_exit:
	close(sock);

	return ret;
}
#endif

/////////////////////////////////////////////////////////////////////////////
int getDefaultRoute(char *interface, struct in_addr *route)
{
	char buff[1024], iface[16];
	char gate_addr[128], net_addr[128], mask_addr[128];
	int num, iflags, metric, refcnt, use, mss, window, irtt;
	FILE *fp = fopen(_PATH_PROCNET_ROUTE, "r");
	char *fmt;
	int found=0;
	unsigned long addr;
	int result;

	if (!fp) {
       		printf("Open %s file error.\n", _PATH_PROCNET_ROUTE);
		return 0;
    	}

	fmt = "%16s %128s %128s %X %d %d %d %128s %d %d %d";

	while (fgets(buff, 1023, fp)) {
		num = sscanf(buff, fmt, iface, net_addr, gate_addr,
		     		&iflags, &refcnt, &use, &metric, mask_addr, &mss, &window, &irtt);
		if (num < 10 || !(iflags & RTF_UP) || !(iflags & RTF_GATEWAY)/* || strcmp(iface, interface)*/)
	    		continue;
		result=strncmp(net_addr,"00000000",8);
		if (result == 0) {
			sscanf(gate_addr, "%lx", &addr );
			*route = *((struct in_addr *)&addr);

			found = 1;
			break;
		}	
	}

    	fclose(fp);
    	return found;
}

#if 0
/////////////////////////////////////////////////////////////////////////////
int deleteDefaultRoute(char *dev, void *route)
{
	struct rtentry rt;
	int skfd;
	struct sockaddr_in *pAddr;

	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));

	/* Fill in the other fields. */
 	rt.rt_flags = RTF_UP | RTF_GATEWAY;
	rt.rt_dev = dev;
	pAddr= (struct sockaddr_in *)&rt.rt_dst;
	pAddr->sin_family = AF_INET;

	pAddr= (struct sockaddr_in *)&rt.rt_gateway;
	pAddr->sin_family = AF_INET;
	pAddr->sin_addr.s_addr = *((unsigned long *)route);

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Create socket error");
		return -1;
    	}

	if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
		printf("Delete route [SIOCDELRT] error!");
		close(skfd);
		return -1;;
	}

	/* Close the socket. */
 	close(skfd);
    	return (0);
}

/////////////////////////////////////////////////////////////////////////////
int addDefaultRoute(char *dev, void *route)
{
	struct rtentry rt;
	int skfd;
	struct sockaddr_in *pAddr;

	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));

	/* Fill in the other fields. */
 	rt.rt_flags = RTF_UP | RTF_GATEWAY;
	rt.rt_dev = dev;
	pAddr= (struct sockaddr_in *)&rt.rt_dst;
	pAddr->sin_family = AF_INET;

	pAddr= (struct sockaddr_in *)&rt.rt_gateway;
	pAddr->sin_family = AF_INET;
	pAddr->sin_addr.s_addr = *((unsigned long *)route);

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Create socket error");
		return -1;
    	}

	if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
		printf("Add route [SIOCADDRT] error!");
		close(skfd);
		return -1;
	}

	/* Close the socket. */
 	close(skfd);
    	return (0);
}
#endif

/////////////////////////////////////////////////////////////////////////////
int getStats(char *interface, struct user_net_device_stats *pStats, int op_mode)
{
 	FILE *fh;
  	char buf[512];
	int type;
	char *ptr;

	// Ralink Wireless card tx packet count NOT follow /proc/net/dev
	if( ((!strcmp (interface, WLAN_IF)) || (!strcmp(interface, INIC_IF))) && (op_mode == 0)){

		sprintf (buf, "/bin/iwpriv %s stat | grep \"Tx success\" > /tmp/%sTxStat", interface, interface);
		system (buf);

		sprintf (buf, "/tmp/%sTxStat", interface);
		if ((fh = fopen(buf, "r")) != NULL) 
		{
			fgets(buf, 512, fh);
			ptr = strchr(buf, '=');
			if(ptr != NULL)
				pStats->tx_packets = strtoul(ptr+2, NULL, 0);
			else 
				pStats->tx_packets = 0;
			fclose(fh);
		}
		else
			return -1;
	
		sprintf (buf, "/bin/iwpriv %s stat | grep \"Rx success\" > /tmp/%sRxStat", interface, interface);
		system (buf);

		sprintf (buf, "/tmp/%sRxStat", interface);
		if ((fh = fopen(buf, "r")) != NULL)
		{
			fgets(buf, 512, fh);
			ptr = strchr(buf, '=');
			if(ptr != NULL)
				pStats->rx_packets = strtoul(ptr+2, NULL, 0);
			else
				pStats->rx_packets = 0;
			fclose(fh);
		}
		else
			return -1;
		return 0;

	}else{
		fh = fopen(_PATH_PROCNET_DEV, "r");
		if (!fh) {
			printf("Warning: cannot open %s\n",_PATH_PROCNET_DEV);
			return -1;
		}

		fgets(buf, sizeof buf, fh);	/* eat line */
		fgets(buf, sizeof buf, fh);

		if (strstr(buf, "compressed"))
			type = 3;
		else if (strstr(buf, "bytes"))
			type = 2;
		else
			type = 1;

		while (fgets(buf, sizeof buf, fh)) {
			char *s, name[40];
		
			s = get_name(name, buf);
			if ( strcmp(interface, name))
				continue;

			get_dev_fields(type, s, pStats);
			fclose(fh);
			
			return 0;
		}

		fclose(fh);
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
static char *get_name(char *name, char *p)
{
    while (isspace(*p))
	p++;
    while (*p) {
	if (isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return NULL;
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

int get_file (char *file, char *ptr_out, int out_size)
{
	FILE *fp;
	char buffer[256];
	int max_size, offset;

	memset (buffer, 0x00, sizeof(buffer));

	max_size = out_size;
	if ( (fp = fopen (file, "r")) != NULL){
		while (fgets (buffer, sizeof(buffer), fp) > 0){
			offset = strlen (buffer);
			max_size -= offset;
			if (out_size < 0)
				break;
			memcpy (ptr_out, buffer, offset);
			ptr_out += offset;
		}
	}

	return (out_size - max_size);
}

////////////////////////////////////////////////////////////////////////////////
static int get_dev_fields(int type, char *bp, struct user_net_device_stats *pStats)
{
    switch (type) {
    case 3:
	sscanf(bp,
	"%Lu %Lu %lu %lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,
	       &pStats->rx_compressed,
	       &pStats->rx_multicast,

	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors,
	       &pStats->tx_compressed);
	break;

    case 2:
	sscanf(bp, "%Lu %Lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,

	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_multicast = 0;
	break;

    case 1:
	sscanf(bp, "%Lu %lu %lu %lu %lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,

	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_bytes = 0;
	pStats->tx_bytes = 0;
	pStats->rx_multicast = 0;
	break;
    }
    return 0;
}

int getAPChannel (char *interface, char *pInfo)
{
#ifndef NO_ACTION
	int skfd;
	struct iwreq wrq;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	/* Get wireless name */
	if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
	/* If no wireless name : no wireless extensions */
		return 0;

	if (iw_get_ext(skfd, interface, SIOCGIWFREQ, &wrq) < 0)
		return 0;

	close (skfd);
	
	return wrq.u.freq.m;
#else
	return 0;

#endif //NO_ACTION
}

/////////////////////////////////////////////////////////////////////////////
int getWdsInfo(char *interface, char *pInfo)
{

#ifndef NO_ACTION
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = MAX_WDS_NUM*sizeof(WDS_INFO_T);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETWDSINFO, &wrq) < 0)
	return -1;

    close( skfd );
#else
    memset(pInfo, 0, MAX_WDS_NUM*sizeof(WDS_INFO_T)); 
#endif

    return 0;
}

/*      IOCTL system call */
static int re865xIoctl(char *name, unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	unsigned int args[4];
	struct ifreq ifr;
	int sockfd;
	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("fatal error socket\n");
		return -3;
	}
	strcpy((char*)&ifr.ifr_name, name);
	((unsigned int *)(&ifr.ifr_data))[0] = (unsigned int)args;
	if (ioctl(sockfd, SIOCDEVPRIVATE, &ifr)<0)
	{
		perror("device ioctl:");
		close(sockfd);
		return -1;
	}
	close(sockfd);
	return 0;
} /* end re865xIoctl */


char *fixed_inet_ntoa(char *in) {
			struct in_addr ia_value;
			memcpy( (unsigned char *)&ia_value,in, 4);
		return inet_ntoa(ia_value);
}
	int fixed_inet_aton(const char *cp,char *inp){
				struct in_addr tempIp;
			 inet_aton(cp, &tempIp);

		memcpy(inp,(unsigned char *)&tempIp,4);
		return 1;
}
//2007.03.08 kyle
#if defined(_SPECIAL_CHAR_FILTER_IN_SCRIPT_)


//Kyle Add for string replace.
//2008.07.23
static char *StrReplace(char *Str, char *OldStr, char *NewStr)
{
      int OldLen, NewLen;
      char *p, *q;
		p=Str;
		q=Str;

	  while(1){
	  	//printf("\nDEBUG0 %s\n ",q);
      if(NULL == (p = strstr(p, OldStr))){
      	//printf("\nDEBUG1 %s\n ",q);
      	break;
      }
      // printf("\nDEBUG2 %s\n ",q);
      // printf("\nDEBUG2P= %s\n ",p);
      OldLen = strlen(OldStr);
      NewLen = strlen(NewStr);
     //printf("NEW STR %s",NewStr);
      memmove(q = p+NewLen, p+OldLen, strlen(p+OldLen)+1);
      memcpy(p, NewStr, NewLen);
      p+=NewLen;
      //printf("\nDEBUG3P %s\n ",p);
	  }
      
      
      return q;
}
char *CharFilter0(char *Str){
		char tmpBuf2[1024]={0};
		strncpy(tmpBuf2,Str,strlen(Str));
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"\"","\\\"");
		StrReplace(tmpBuf2,"$","\\$");
		StrReplace(tmpBuf2,"\`","\\\`");
		sprintf(Str,"%s",tmpBuf2);
		return Str;
}
char *CharFilter(char *Str){
		char tmpBuf2[1024]={0};
		//printf("DEBUG ORG SSID: %s\n",tmpBuf);
		strncpy(tmpBuf2,Str+1,strlen(Str)-2);
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"\"","\\\"");
		StrReplace(tmpBuf2,"$","\\$");
		StrReplace(tmpBuf2,"\`","\\\`");
		//StrReplace(tmpBuf2,";","\\;");
		sprintf(Str,"\"%s\"",tmpBuf2);
		return Str;
		//printf("DEBUG NEW SSID: %s\n",tmpBuf);
}
#if defined(_Customer_)
char *CharFilter1(char *Str){		
		char tmpBuf2[1024]={0};
		strncpy(tmpBuf2,Str,strlen(Str));
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"\"","\\\"");
		StrReplace(tmpBuf2,"<","&lt;");
		StrReplace(tmpBuf2,">","&gt;");
		StrReplace(tmpBuf2,"%","%%");
		sprintf(Str,"%s",tmpBuf2);
		return Str;
}
#endif
char *CharFilter2(char *Str){
		char tmpBuf2[1024]={0};
		//printf("DEBUG ORG SSID: %s\n",tmpBuf);
		strncpy(tmpBuf2,Str,strlen(Str));
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"\"","\\\"");
		StrReplace(tmpBuf2,"%","%%");
		sprintf(Str,"%s",tmpBuf2);
		return Str;
		//printf("DEBUG NEW SSID: %s\n",tmpBuf);
}
char *CharFilter3(char *Str){
		char tmpBuf2[1024]={0};
		strncpy(tmpBuf2,Str,strlen(Str));
//		StrReplace(tmpBuf2,"<","&lt");
//		StrReplace(tmpBuf2,">","&gt");
#if defined(_Customer_)
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"%","%%");
		StrReplace(tmpBuf2,"\`","\\\`");
		StrReplace(tmpBuf2,"$","\\$");
#endif
		StrReplace(tmpBuf2,"\"","\\\"");
    sprintf(Str,"%s",tmpBuf2);
    return Str;
}
char *CharFilter4(char *Str){
    char tmpBuf2[1024]={0};
    strncpy(tmpBuf2,Str,strlen(Str));
//		StrReplace(tmpBuf2,"&lt","<");
//		StrReplace(tmpBuf2,"&gt",">");
#if defined(_Customer_)
		StrReplace(tmpBuf2,"\\\\","\\");
		StrReplace(tmpBuf2,"%%","%");
		StrReplace(tmpBuf2,"\\\`","\`");
		StrReplace(tmpBuf2,"\\$","$");
#endif
		StrReplace(tmpBuf2,"\\\"","\"");
    sprintf(Str,"%s",tmpBuf2);
    return Str;
}

char *CharFilter5(char *Str){  //patrick add
  char tmpBuf2[1024]={0};
  strncpy(tmpBuf2,Str,strlen(Str));
  StrReplace(tmpBuf2,"\"","&quot;");
	StrReplace(tmpBuf2,"\\","&#92;"); 
	StrReplace(tmpBuf2," ","&#32;"); 
	sprintf(Str,"%s",tmpBuf2);
  return Str;
}

char *CharFilter6(char *Str){  //EDX patrick add
		char tmpBuf2[1024]={0};
		strncpy(tmpBuf2,Str,strlen(Str));
		StrReplace(tmpBuf2,"\\","\\\\");
		StrReplace(tmpBuf2,"\"","\\\"");
		sprintf(Str,"%s",tmpBuf2);
		return Str;
}
char *CharFilter7(char *Str){
	char tmpBuf2[1024]={0};
	strncpy(tmpBuf2,Str,strlen(Str));
	StrReplace(tmpBuf2,"\\","\\\\");
	StrReplace(tmpBuf2,"\"","\\\"");
	sprintf(Str,"\"%s\"",tmpBuf2);
	return Str;
}
char *CharFilter8(char *Str){
	char tmpBuf2[1024]={0};
	strncpy(tmpBuf2,Str+1,strlen(Str)-2);
	StrReplace(tmpBuf2,"\\\\","\\");
	StrReplace(tmpBuf2,"\\\"","\"");
	sprintf(Str,"%s",tmpBuf2);
	return Str;
}
#endif
#if defined(_SIGNAL_FORMULA_)
char ScanListSignalScaleMapping92CD(char CurrSig)
{
    char RetSig;

    if(CurrSig >= 50)
        RetSig = 100;
    else
        RetSig  = CurrSig * 2;
    return RetSig;

}
#endif
