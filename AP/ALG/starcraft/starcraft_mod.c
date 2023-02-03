/*
 * Licensed under GNU GPL version 2 Copyright Lance Wu 
 * Version: 0.0.1
 *
 * 2003.08.21
 * 	- NEW Starcraft module
 *
 */

#include <linux/config.h>
#include <linux/module.h>

#include <linux/ip.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <linux/ctype.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include "starcraft_mod.h"

#include <net/checksum.h>

MODULE_AUTHOR("Lance Wu ");
MODULE_DESCRIPTION("Netfilter connection tracking module for Starcraft");
MODULE_LICENSE("GPL");
#define DEBUGP printk
/*
#if 0
#define DEBUGP(format, args...) printk(__FUNCTION__ ": " \
				       format, ## args)
#else
#define DEBUGP(format, args...)
#endif

*/
//kyle
unsigned int (*ip_nat_sc_hook)(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       unsigned int hooknum);
EXPORT_SYMBOL_GPL(ip_nat_sc_hook);
//kyle
static struct ip_conntrack_helper starcraft;
static char sc_names[10];

SC_INFO sc_info[MAX_CLIENTS];
EXPORT_SYMBOL_GPL(sc_info);
u_int16_t sc_info_count;
//u_int32_t sc_server;
u_int32_t sc_gateway;
EXPORT_SYMBOL_GPL(sc_gateway);

int record_client(u_int32_t ip, u_int32_t battle)
{
	u_int16_t i;
	
	for( i=0; i<MAX_CLIENTS; i++ )
	{
		if( sc_info[i].ip == ip )
		{
			if( sc_info[i].battle_net != battle )
			{
	            sc_info[i].battle_net = battle;
	            DEBUGP("modify ip-%d: %u.%u.%u.%u BattleNet-> %u.%u.%u.%u port-> %u\n",
				                            i,
				                            NIPQUAD(sc_info[i].ip),
				                            NIPQUAD(sc_info[i].battle_net),
											sc_info[i].port);
			    return 1;
            }
            return 0;
		}
        if( sc_info[i].ip == 0 )
		{
	        sc_info[i].ip = ip;
	        sc_info[i].battle_net = battle;
	        DEBUGP("record ip-%d: %u.%u.%u.%u BattleNet: %u.%u.%u.%u port: %u\n",
			                            i,
			                            NIPQUAD(sc_info[i].ip),
			                            NIPQUAD(sc_info[i].battle_net),
										sc_info[i].port);
	        return 1;
        }
    }
	
    sc_info[sc_info_count].ip = ip;
    sc_info[sc_info_count].battle_net = battle;
	
    DEBUGP("change ip-%d: %u.%u.%u.%u BattleNet: %u.%u.%u.%u port -> %u\n",
			                sc_info_count,
			                NIPQUAD(sc_info[sc_info_count].ip),
			                NIPQUAD(sc_info[sc_info_count].battle_net),
							sc_info[sc_info_count].port);
	
    sc_info_count++;
    if( sc_info_count == MAX_CLIENTS )
	    sc_info_count = 0;
	return 1;
}

/*
int IsUsedBattle( u_int32_t ip, u_int32_t battle )
{    
	int i;
			         
	for( i=0; i<MAX_CLIENTS; i++ )
	{        
		if( sc_info[i].ip == 0 )
			return 0;           
        if( sc_info[i].battle_net == battle )
        {                       
	        if( sc_info[i].ip == ip )
		        return 0;
		    return 1;
        }   
    }   
    return 0;
}
*/  
/*
int record_port( u_int32_t ip, u_int16_t port )
{                           
	int i;                  
			                            
    for( i=0; i<MAX_CLIENTS; i++ )
    {   
	    if( sc_info[i].ip == ip )
	    {
		    if( sc_info[i].port != port )
		    {
			    sc_info[i].port = port;
			    DEBUGP("record ip-%d: %u.%u.%u.%u port %u\n",
                                     i,
                                     NIPQUAD(sc_info[i].ip),
                                     sc_info[i].port);
			}
            return 1;
        }
        if( sc_info[i].ip == 0 )
	        return 0;
	}

	return 0;
}
*/
/*	
u_int32_t find_battle_by_client( u_int32_t ip )
{
	int i;

	for( i=0; i<MAX_CLIENTS; i++ )
	{
	    if( sc_info[i].ip == ip )
		    return sc_info[i].battle_net;
		if( sc_info[i].ip == 0 )
		    return 0;
	}

	return 0;
}
*/

static int sc_help(struct sk_buff **pskb,
		struct ip_conntrack *ct,
		enum ip_conntrack_info ctinfo)
{
	int dir;
	int ret;

	struct tcphdr _tcph, *tcph;

	unsigned int hooknum;
	size_t tcplen;
        struct iphdr *iph;
        size_t len;
	/* Not whole TCP header? */
  iph=(*pskb)->nh.iph;
	tcph = skb_header_pointer(*pskb,iph->ihl*4,sizeof(_tcph),&_tcph);
	if (!tcph)
		return NF_ACCEPT;

	len = (*pskb)->len; 
  tcplen = len - iph->ihl * 4;
	unsigned int datalen = tcplen - tcph->doff * 4;
	ret = NF_ACCEPT;
	dir = CTINFO2DIR(ctinfo);
//  struct udphdr *udph = (void *)iph + iph->ihl * 4;
 	 // unsigned int tcplen = len - iph->ihl * 4; /*kyle
//	const unsigned char* data = (const unsigned char*)tcph + tcph->doff * 4;
//	unsigned int udplen = udph->len;
//  struct ip_conntrack_expect exp;

//    unsigned int port;
//	int found=0;
//	int i,x;

    DEBUGP("\n");
   	DEBUGP("ctinfo= %d, dir= %d, protocol= %s\n",
		    ctinfo,dir, (iph->protocol == IPPROTO_TCP)?"TCP":"UDP");
    DEBUGP("from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
   	        NIPQUAD(iph->saddr),ntohs(tcph->source),
       	    NIPQUAD(iph->daddr),ntohs(tcph->dest));
	
    if( iph->protocol == IPPROTO_TCP )
    {
		/*
	    if( IsUsedBattle( iph->saddr, iph->daddr) == 1 )
	    {
		    DEBUGP("BattleNet [%u.%u.%u.%u] is using by another ip, DROP...\n",NIPQUAD(iph->daddr));
			DEBUGP("\n");
			return NF_DROP;
		}
		*/

		if( (datalen != 0) && (ctinfo == IP_CT_ESTABLISHED) && (dir == IP_CT_DIR_ORIGINAL) )
		{
			if( sc_gateway == 0 )
			{
				sc_gateway = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
				DEBUGP("GW: %u.%u.%u.%u\n", NIPQUAD(sc_gateway));
	        }
	
			record_client(iph->saddr, iph->daddr);
/*			
			if( datalen >= 20 )
			{
				if( (*data == 0xff) && ( *(data+1) == 0x1c ) && ( datalen > 24 ))
				{
//					sc_server = iph->saddr;
					DEBUGP("Create Server [%s] in %u.%u.%u.%u\n",data+24, NIPQUAD(iph->saddr) );
	            }
	        }				
*/
		}
	}   
	/*
	unsigned int (*ip_nat_sc_hook)(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,struct ip_conntrack *ct
			       unsigned int hooknum,
			       struct ip_conntrack_expect *exp);
			       */
	//kyle	start
		if (ct->status & IPS_SRC_NAT_DONE)
			hooknum = NF_IP_POST_ROUTING; /* IP_NAT_MANIP_SRC */
		else if (ct->status & IPS_DST_NAT_DONE)
			hooknum = NF_IP_PRE_ROUTING;  /* IP_NAT_MANIP_D*/
		else
			hooknum=NF_IP_POST_ROUTING;/* Missing NAT flags */
			
	/* Now, NAT might want to mangle the packet, and register the
	 * (possibly changed) expectation itself. */
	if (ip_nat_sc_hook)
        ret = ip_nat_sc_hook(pskb,ctinfo,ct,hooknum);

	//kyle end
	DEBUGP("\n");

	return ret;
	//return NF_ACCEPT;
}

static void __exit fini(void)
{
//	int i;

	ip_conntrack_helper_unregister( &starcraft );
	DEBUGP("unregistering helper for starcraft modules\n");
}

static int __init init(void)
{
	int i, ret;
	char *tmpname;

//    sc_server = 0;
    sc_gateway = 0;
    sc_info_count = 0;
    memset(sc_info,0, sizeof(SC_INFO) * MAX_CLIENTS);

	for( i=0; i<MAX_CLIENTS; i++ )
		sc_info[i].port = SC_PORT + i;
					
	memset( &starcraft, 0, sizeof(struct ip_conntrack_helper) );

	starcraft.tuple.src.u.all = htons( SC_PORT );
	starcraft.mask.src.u.all = htons(0xFF00);

	starcraft.max_expected = 1;
	starcraft.timeout = 1;
	starcraft.me = THIS_MODULE;
	starcraft.help = sc_help;

	tmpname = &sc_names[0];
	sprintf(tmpname, "starcraft");

	ret = ip_conntrack_helper_register( &starcraft );
		
	if( ret  ) 
	{
		DEBUGP("ERROR registering helper for starcraft\n");
		fini();
		return(ret);
	}
	DEBUGP("register helper for starcraft SUCCESS!!\n");
	
	return(0);
}

module_init(init);
module_exit(fini);
