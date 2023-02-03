/*
 * udp.c - handle upd connections
 *
 * Copyright (C) 1999 Brad M. Garcia <garsh@home.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "relay.h"
#include "cache.h"
#include "query.h"
#include "domnode.h"
#include "check.h"
#include "dns.h"
#include <sys/ioctl.h>
#include <net/if.h>

#ifndef EXCLUDE_MASTER
#include "master.h"
#endif

/*
 * dnssend()						22OCT99wzk
 *
 * Abstract: A small wrapper for send()/sendto().  If an error occurs a
 *           message is written to syslog.
 *
 * Returns:  The return code from sendto().
 */
static int udp_send(int sock, srvnode_t *srv, void *msg, int len)
{
    int	rc;
    time_t now = time(NULL);
    rc = sendto(sock, msg, len, 0,
		(const struct sockaddr *) &srv->addr,
		sizeof(struct sockaddr_in));

    if (rc != len) {
	log_msg(LOG_ERR, "sendto error: %s: ",
		inet_ntoa(srv->addr.sin_addr), strerror(errno));
	return (rc);
    }
    if ((srv->send_time == 0)) srv->send_time = now;
    srv->send_count++;
    return (rc);
}

int send2current(query_t *q, void *msg, const int len) {
    /* If we have domains associated with our servers, send it to the
       appropriate server as determined by srvr */
  domnode_t *d;
  assert(q != NULL);
  assert(q->domain != NULL);

  d = q->domain;
  while ((d->current != NULL) && (udp_send(q->sock, d->current, msg, len) != len)) {
    if (reactivate_interval) deactivate_current(d);
  }
  if (d->current != NULL) {
    return len;
  } else return 0;
}
// { EDX RexHua
int recvfrom_edx(int s, u_char *pbuf, int maxsize, struct sockaddr *sin, int *addr_len, struct sockaddr *din)
{
    int     n, x=1;
    struct  msghdr  msg;
    struct  {
        struct cmsghdr    hdr;
        struct in_pktinfo info;
    } msgbuf;
    struct  iovec   iov;
    bzero(&msg, sizeof(msg));	/* make certain msg_accrightslen = 0 */
    bzero(&msgbuf, sizeof(msgbuf));

    msg.msg_name = sin;
    msg.msg_namelen = *addr_len;
    msg.msg_control = &msgbuf;
    msg.msg_controllen = sizeof(msgbuf);

    iov.iov_base = pbuf;
    iov.iov_len = maxsize;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    setsockopt(s, SOL_IP, IP_PKTINFO, &x, sizeof(x));
    
    n = recvmsg(s, &msg, 0);

    ((struct sockaddr_in *)din)->sin_addr = msgbuf.info.ipi_addr;
    
printf("dest ip = %s len=%d flags=%d\n", inet_ntoa(((struct sockaddr_in *)din)->sin_addr), msg.msg_controllen, msg.msg_flags);

    return(n);
}

int getInAddr( char *interface, void *pAddr )
{
    struct ifreq ifr;
    int skfd=0, found=0;
    struct sockaddr_in *addr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(skfd==-1)
                return 0;

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
        close( skfd );
                return (0);
        }
   if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
  	addr = ((struct sockaddr_in *)&ifr.ifr_addr);
                *((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
                found = 1;
    }
    close( skfd );
    return found;
}

int ip_name_find( struct sockaddr_in *sin, char *ifName)
{
  struct ifreq ifr;
  char interface[4][20]={"br0", "br1", "br2", "br3"};
  struct in_addr intaddr;
  int i;

  for(i=0; i<4; i++)
  {
    if(getInAddr(interface[i], (void *)&intaddr) == 1)
    {
	    if(intaddr.s_addr == sin->sin_addr.s_addr)
	    {
		    strcpy(ifName, interface[i]);
        return 1;
	    }
    }
  }
  return 0;
}


// } EDX RexHua
// msg.msg_controllen

/* handle_udprequest()
 *
 * This function handles udp DNS requests by either replying to them (if we
 * know the correct reply via master, caching, etc.), or forwarding them to
 * an appropriate DNS server.
 */
query_t *udp_handle_request()
{
    unsigned           addr_len;
    int                len;
    const int          maxsize = UDP_MAXSIZE;
    static char        msg[UDP_MAXSIZE+4];
    struct sockaddr_in from_addr;
    struct sockaddr_in daddr;
    
    int                fwd;
    domnode_t          *dptr;
    query_t *q, *prev;

    /* Read in the message */
    addr_len = sizeof(struct sockaddr_in);
//    len = recvfrom(isock, msg, maxsize, 0, (struct sockaddr *)&from_addr, &addr_len);
    len = recvfrom_edx(isock, msg, maxsize, (struct sockaddr *)&from_addr, &addr_len, (struct sockaddr *)&daddr); // EDX RexHua
    if (len < 0) {
	log_debug(1, "recvfrom error %s", strerror(errno));
	return NULL;
    }

    /* do some basic checking */
    if (check_query(msg, len) < 0) return NULL;

    /* Determine how query should be handled */
    //if ((fwd = handle_query(&from_addr, msg, &len, &dptr)) < 0)
    if ((fwd = handle_query(&from_addr, msg, &len, &dptr, &daddr)) < 0)
      return NULL; /* if its bogus, just ignore it */

    /* If we already know the answer, send it and we're done */
    if (fwd == 0) {
	char ifName[80];
#if 1 // EDX RexHua, Bin to source IF
	if(ip_name_find(&daddr, ifName))
	{
		log_debug(1, "bind to  %s", ifName);
		setsockopt(isock, SOL_SOCKET, SO_BINDTODEVICE, ifName, strlen(ifName) + 1);
	}
#endif
	if (sendto(isock, msg, len, 0, (const struct sockaddr *)&from_addr,
		   addr_len) != len) {
	    log_debug(1, "sendto error %s", strerror(errno));
	}
#if 1 // EDX RexHua, Bind to any
	{
		int intVal=0;

		setsockopt(isock, SOL_SOCKET, SO_BINDTODEVICE, &intVal, sizeof(intVal));
	}
#endif

	return NULL;
    }


    /* dptr->current should never be NULL it is checked in handle_query */

    //    dnsquery_add(&from_addr, msg, len);
    // if (!send2current(dptr, msg, len)) {

    /* rewrite msg, get id and add to list*/

    if ((prev=query_add(dptr, dptr->current, &from_addr, msg, len)) == NULL){
      /* of some reason we could not get any new queries. we have to
	 drop this packet */
      return NULL;
    }
    q = prev->next;


    if (send2current(q, msg, len) > 0) {
      /* add to query list etc etc */
      return q;
    } else {

      /* we couldn't send the query */
#ifndef EXCLUDE_MASTER
      int	packetlen;
      char	packet[maxsize+4];

      /*
       * If we couldn't send the packet to our DNS servers,
       * perhaps the `network is unreachable', we tell the
       * client that we are unable to process his request
       * now.  This will show a `No address (etc.) records
       * available for host' in nslookup.  With this the
       * client won't wait hang around till he gets his
       * timeout.
       * For this feature dnrd has to run on the gateway
       * machine.
       */
      
      if ((packetlen = master_dontknow(msg, len, packet)) > 0) {
	query_delete_next(prev);
	return NULL;
	if (sendto(isock, msg, len, 0, (const struct sockaddr *)&from_addr,
		   addr_len) != len) {
	  log_debug(1, "sendto error %s", strerror(errno));
	  return NULL;
	}
      }
#endif
    }
    return q;
}

/*
 * dnsrecv()							22OCT99wzk
 *
 * Abstract: A small wrapper for recv()/recvfrom() with output of an
 *           error message if needed.
 *
 * Returns:  A positove number indicating of the bytes received, -1 on a
 *           recvfrom error and 0 if the received message is too large.
 */
static int reply_recv(query_t *q, void *msg, int len)
{
    int	rc, fromlen;
    struct sockaddr_in from;

    fromlen = sizeof(struct sockaddr_in);
    rc = recvfrom(q->sock, msg, len, 0,
		  (struct sockaddr *) &from, &fromlen);

    if (rc == -1) {
	log_msg(LOG_ERR, "recvfrom error: %s",
		inet_ntoa(q->srv->addr.sin_addr));
	return (-1);
    }
    else if (rc > len) {
	log_msg(LOG_NOTICE, "packet too large: %s",
		inet_ntoa(q->srv->addr.sin_addr));
	return (0);
    }
    else if (memcmp(&from.sin_addr, &q->srv->addr.sin_addr,
		    sizeof(from.sin_addr)) != 0) {
	log_msg(LOG_WARNING, "unexpected server: %s",
		inet_ntoa(from.sin_addr));
	return (0);
    }

    return (rc);
}

/*
 * handle_udpreply()
 *
 * This function handles udp DNS requests by either replying to them (if we
 * know the correct reply via master, caching, etc.), or forwarding them to
 * an appropriate DNS server.
 *
 * Note that the mached query is prev->next and not prev.
 */
void udp_handle_reply(query_t *prev)
{
  //    const int          maxsize = 512; /* According to RFC 1035 */
    static char        msg[UDP_MAXSIZE+4];
    int                len;
    unsigned           addr_len;
    query_t *q = prev->next;

    log_debug(3, "handling socket %i", q->sock);
    if ((len = reply_recv(q, msg, UDP_MAXSIZE)) < 0)
      {
	log_debug(1, "dnsrecv failed: %i", len);
	query_delete_next(prev);
	return; /* recv error */
      }
    
    /* do basic checking */
    if (check_reply(q->srv, msg, len) < 0) {
      log_debug(1, "check_reply failed");
      query_delete_next(prev);
      return;
    }

    if (opt_debug) {
	char buf[256];
	snprintf_cname(msg, len, 12, buf, sizeof(buf));
	log_debug(3, "Received DNS reply for \"%s\"", buf);
    }

    dump_dnspacket("reply", msg, len);
    addr_len = sizeof(struct sockaddr_in);

    /* was this a dummy reactivate query? */
    if (q->domain != NULL) {
      /* no, lets cache the reply and send to client */
      cache_dnspacket(msg, len, q->srv);

      /* set the client qid */
      *((unsigned short *)msg) = q->client_qid;
      log_debug(3, "Forwarding the reply to the host %s",
		inet_ntoa(q->client.sin_addr));
#if 0 //EDX patrick add for wireless hijack issue
		setsockopt(isock, SOL_SOCKET, SO_BINDTODEVICE, "br1", 4);
		if (sendto(isock, msg, len, 0, (const struct sockaddr *)&q->client, addr_len) != len)
			log_debug(1, "sendto(br1) error %s", strerror(errno));

		setsockopt(isock, SOL_SOCKET, SO_BINDTODEVICE, "br0", 4);
		if (sendto(isock, msg, len, 0, (const struct sockaddr *)&q->client, addr_len) != len)
			log_debug(1, "sendto(br0) error %s", strerror(errno));

		int i=0;
		setsockopt(isock, SOL_SOCKET, SO_BINDTODEVICE, &i, sizeof(i));
#else
      if (sendto(isock, msg, len, 0,
		 (const struct sockaddr *)&q->client,
		 addr_len) != len) {
	log_debug(1, "sendto error %s", strerror(errno));
      }
#endif
    } else {
      log_debug(2, "We got a reactivation dummy reply. Cool!");
    }
      
    /* this server is obviously alive, we reset the counters */
    q->srv->send_time = 0;
    if (q->srv->inactive) log_debug(1, "Reactivating server %s",
				 inet_ntoa(q->srv->addr.sin_addr));
    q->srv->inactive = 0;
    /* remove query from list and destroy it */
    query_delete_next(prev);
}


/* send a dummy packet to a deactivated server to check if its back*/
int udp_send_dummy(srvnode_t *s) {
  static unsigned char dnsbuf[] = {
  /* HEADER */
    /* we send a lookup for localhost */
    /* will this work on a big endian system? */
    0x00, 0x00, /* ID */
    0x01, 0x00, /* QR|OC|AA|TC|RD -  RA|Z|RCODE  */
    0x00, 0x01, /* QDCOUNT */
    0x00, 0x00, /* ANCOUNT */
    0x00, 0x00, /* NSCOUNT */
    0x00, 0x00, /* ARCOUNT */

    9, 'l','o','c','a','l','h','o','s','t',0,  /* QNAME */
    0x00,0x01,   /* QTYPE A record */
    0x00,0x01   /* QCLASS: IN */

    /* in case you want to lookup root servers instead, use this: */
    /*    0x00,       */ /* QNAME:  empty */
    /*    0x00, 0x02, */ /* QTYPE:  a authorative name server */
    /*    0x00, 0x01  */ /* QCLASS: IN */
  };
  query_t *q;
  struct sockaddr_in srcaddr;

  /* should not happen */
  assert(s != NULL);

  if ((q=query_add(NULL, s, &srcaddr, dnsbuf, sizeof(dnsbuf))) != NULL) {
    int rc;
    q = q->next; /* query add returned the query 1 before in list */
    /* don't let those queries live too long */
    q->ttl = reactivate_interval;
    memset(&srcaddr, 0, sizeof(srcaddr));
    log_debug(2, "Sending dummy id=%i to %s", ((unsigned short *)dnsbuf)[0], 
	      inet_ntoa(s->addr.sin_addr));
    /*  return dnssend(s, &dnsbuf, sizeof(dnsbuf)); */
    rc=udp_send(q->sock, s, dnsbuf, sizeof(dnsbuf));
    ((unsigned short *)dnsbuf)[0]++;
    return rc;
  }
  return -1;
}
