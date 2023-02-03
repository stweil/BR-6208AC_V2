/*
 * Licensed under GNU GPL version 2 Copyright Lance Wu
 * Version: 0.0.1
 *
 * 2003.08.21
 * 	- NEW Starcraft module
 *
 */

#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include <linux/netfilter_ipv4/ip_nat_helper.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <net/checksum.h>

#include "starcraft_mod.h"

MODULE_AUTHOR("Lance Wu");
MODULE_DESCRIPTION("Netfilter NAT helper for Starcraft");
MODULE_LICENSE("GPL");

#if 0
#define DEBUGP(format, args...) printk(__FUNCTION__ ": "\
				       format, ## args)
#else
#define DEBUGP(format, args...)
#endif

extern SC_INFO sc_info[MAX_CLIENTS];

//extern u_int32_t sc_server;
extern u_int32_t sc_gateway;

u_int32_t find_ip_by_port(u_int16_t port)
{
	int i;

	for( i=0; i<MAX_CLIENTS; i++ )
	{
	    if( sc_info[i].ip == 0 )
		    return 0;
		if( sc_info[i].port == port )
	        return sc_info[i].ip;
	}

	return 0;
}

u_int16_t find_port_by_ip(u_int32_t ip)
{
	int i;

	for( i=0; i<MAX_CLIENTS; i++ )
	{
	    if( sc_info[i].ip == 0 )
		    return 0;
		if( sc_info[i].ip == ip )
	        return sc_info[i].port;
	}

	return 0;
}

/*
u_int32_t find_client_by_battle( u_int32_t ip )
{
	int i;
		    
	for( i=0; i<MAX_CLIENTS; i++ )
	{
		if( sc_info[i].battle_net == ip )
			return sc_info[i].ip; 
    }       
				    
    return 0;
}
*/
//kyle
static unsigned int ip_nat_sc(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       unsigned int hooknum){
	int dir = CTINFO2DIR(ctinfo);
	struct iphdr *iph = (*pskb)->nh.iph;
	struct tcphdr *tcph = (void *)iph + iph->ihl * 4;
	struct udphdr *udph = (void *)iph + iph->ihl * 4;
//	struct ip_conntrack_tuple repl;
	unsigned int datalen = (*pskb)->len - iph->ihl * 4 - tcph->doff * 4;
	
	u_int16_t newport;
	//kyle
	struct ip_nat_range mr;
	//struct ip_nat_multi_range mr;/*kyle*/
	u_int32_t newip;
	DEBUGP("\n");
	
    DEBUGP("from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u \n",
	    NIPQUAD(iph->saddr), ntohs(udph->source),
		NIPQUAD(iph->daddr), ntohs(udph->dest));
	
	DEBUGP("hooknum= %d, protocol= %s\n",hooknum,(iph->protocol == IPPROTO_TCP)?"TCP":"UDP");

	if( iph->protocol == IPPROTO_TCP )
	{
		DEBUGP("TCP datalen= %u\n",datalen);
		
	    if( (datalen > 20) && (ctinfo == IP_CT_IS_REPLY) && (dir == IP_CT_DIR_REPLY) )
	    {
//			unsigned int matchoff;
//			unsigned int matchlen;					  
			const unsigned char* data = (const unsigned char*)tcph + tcph->doff * 4;
		
	    	if( (*data == 0xff) && ( *(data+1) == 0x09 ) )
		    {
			    DEBUGP("\n");
			    DEBUGP("ctinfo= %d, dir= %d, protocol= %s\n",
 								ctinfo,dir, (iph->protocol == IPPROTO_TCP)?"TCP":"UDP");
			
	            if( *(data+4) == 1 )
    	        {
	    	        DEBUGP("Battle.Net answer server in %u.%u.%u.%u port: %u\n",
							            *(data+20),*(data+21),*(data+22),*(data+23),
                	                    ntohs( *(unsigned int*)(data+18) ));
					if( find_port_by_ip( *(u_int32_t*)(data+20) ) )
					{
//						matchoff = 20;
//						matchlen = 4;

						DEBUGP("It's local ip, must change ip to %u.%u.%u.%u\n",NIPQUAD(sc_gateway));
//					    return ip_nat_mangle_tcp_packet(pskb, ct, ctinfo, matchoff,
//										                    matchlen, (unsigned char*)&sc_gateway, 4);
					    return ip_nat_mangle_tcp_packet(pskb, ct, ctinfo, 20, 4, (unsigned char*)&sc_gateway, 4);
					}
				}
			}
		}
	}

	if( (iph->protocol == IPPROTO_UDP) )
	{
//	    DEBUGP("IP_CT_DIR_ORIGINAL:\n");
//	    DUMP_TUPLE_RAW(&(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple));
//	    DEBUGP("IP_CT_DIR_REPLY:\n");
//	    DUMP_TUPLE_RAW(&(ct->tuplehash[IP_CT_DIR_REPLY].tuple));

		switch(hooknum)
		{

		case NF_IP_PRE_ROUTING: // hooknum == 0
	        // check iph->saddr is client ip or not
			
			// BattleNet -> GW , mr to client by battle
			/*
			newip = find_client_by_battle(iph->saddr);
			if( (newip != 0) && (iph->daddr == sc_gateway) )
			{
				memset( &mr, 0, sizeof( struct ip_nat_multi_range ) );
				mr.rangesize = 1;
                mr.range[0].flags = IP_NAT_RANGE_MAP_IPS;
                mr.range[0].min_ip = mr.range[0].max_ip = newip;
                mr.range[0].flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
                mr.range[0].min = mr.range[0].max
				                = ((union ip_conntrack_manip_proto)
	                                { htons( SC_PORT ) });
	            DEBUGP("BattleNet->GW, mr to %u.%u.%u.%u port %u\n",NIPQUAD(newip),SC_PORT);
                return ip_nat_setup_info(ct,&mr,hooknum);
			}
			*/
			
			// someone -> GW ,  mr to client by port
//	        if( (find_battle_by_client(iph->saddr) == 0) && (iph->daddr == sc_gateway) )
	        if( (iph->daddr == sc_gateway) )
    	    {
	    	    newip = find_ip_by_port( ntohs(udph->dest) );
			
//		        if( (newip != 0) && (newip != iph->daddr) && (find_port_by_ip(iph->daddr) == 0) )
		        if( newip != 0 )
	            {		
												memset( &mr, 0, sizeof( struct ip_nat_range ) );
												mr.flags = IP_NAT_RANGE_MAP_IPS;
												mr.min_ip = mr.max_ip = newip;
											 mr.min = mr.max= ((union ip_conntrack_manip_proto){ htons( SC_PORT ) });
	            		/*kyle					
	                memset( &mr, 0, sizeof( struct ip_nat_multi_range ) );
	                mr.rangesize = 1;
                    mr.range[0].flags = IP_NAT_RANGE_MAP_IPS;
                    mr.range[0].min_ip = mr.range[0].max_ip = newip;
                    mr.range[0].flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
                    mr.range[0].min = mr.range[0].max
					                = ((union ip_conntrack_manip_proto)
	                                    { htons( SC_PORT ) });
	               kyle*/
	                DEBUGP("Someone->GW, mr to %u.%u.%u.%u port %u\n",NIPQUAD(newip),SC_PORT);

                    return ip_nat_setup_info(ct,&mr,hooknum);
//                    ip_nat_setup_info(ct,&mr,hooknum);
//					return NF_DROP;
                }
			}

            break;

		case NF_IP_POST_ROUTING: // hooknum == 4   
//			if( find_battle_by_client(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip) && (iph->saddr == sc_gateway) )
			if( iph->saddr == sc_gateway )
			{
//				DEBUGP("Is client!!\n");

				newport = find_port_by_ip(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip);
				if( newport != 0 )
				{
					if( newport != udph->source )
					{
						DEBUGP("send by %u.%u.%u.%u , so port must be %u !!\n",
										NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip),
										newport);
						udph->source = htons(newport);
						udph->check = 0;
					}
				}
				else
				{
					newport = find_port_by_ip(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.ip);
					if( newport != 0 )
					{
						if( newport != udph->source )
						{
							DEBUGP("reply by %u.%u.%u.%u , so port must be %u !!\n",
											NIPQUAD(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.ip),
											newport);
							udph->source = htons(newport);
							udph->check = 0;
						}
					}
				}
//				record_port( ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip, ntohs(udph->source) );
			}
			else
			{
				newport = find_port_by_ip(iph->saddr);
				if( newport != 0 )
				{
					if( find_port_by_ip(iph->daddr) )
					{
						newip = sc_gateway;
						
						memset( &mr, 0, sizeof( struct ip_nat_range ) );
						mr.flags = IP_NAT_RANGE_MAP_IPS;
						mr.min_ip = mr.max_ip = newip;
						/*kyle
						memset( &mr, 0, sizeof( struct ip_nat_multi_range ) );
						mr.rangesize = 1;
        		        mr.range[0].flags = IP_NAT_RANGE_MAP_IPS;
            		    mr.range[0].min_ip = mr.range[0].max_ip = newip;
	        	    	DEBUGP("client->client, mr to %u.%u.%u.%u \n",NIPQUAD(newip));
					kyle*/
//	                	return ip_nat_setup_info(ct,&mr,hooknum);
	                	ip_nat_setup_info(ct,&mr,hooknum);
						return NF_DROP;
					}
				}

				if( iph->saddr == ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip )
				{
					if( udph->source != ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all )
					{
						DEBUGP("port incorrect! change back...(%u -> %u)\n",
										ntohs(udph->source),
										ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all) );
						
						udph->source = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
						udph->check = 0;
					}
				}
			}
			break;
		}
	}		
	DEBUGP("\n");
	return NF_ACCEPT; 	       	
}

//static struct ip_nat_helper sc_helper;
static char sc_names[10];


//kyle

static void __exit ip_nat_sc_fini(void)
{
	ip_nat_sc_hook = NULL;
	/* Make sure noone calls it, meanwhile. */
	synchronize_net();
}
static int __init ip_nat_sc_init(void)
{
	BUG_ON(ip_nat_sc_hook);
	ip_nat_sc_hook = ip_nat_sc;
	return 0;
}
module_init(ip_nat_sc_init);
module_exit(ip_nat_sc_fini);
