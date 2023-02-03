#ifndef _IP_CT_STARCRAFT_H_
#define _IP_CT_STARCRAFT_H_

#define SC_PORT 6112

#define MAX_CLIENTS	256

typedef struct _sc_info{
	u_int32_t	ip;
	u_int16_t	port;
	u_int32_t	battle_net;
//	u_int32_t	server;
//	u_int16_t	sport;
}SC_INFO,*pSC_INFO;

//kyle
extern unsigned int (*ip_nat_sc_hook)(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       unsigned int hooknum);
			       
#define DUMP_TUPLE_RAW(x)                                              \
       DEBUGP("tuple %p: %u %u.%u.%u.%u:0x%08x -> %u.%u.%u.%u:0x%08x\n",\
       (x), (x)->dst.protonum,                                         \
       NIPQUAD((x)->src.ip), ntohl((x)->src.u.all),                    \
       NIPQUAD((x)->dst.ip), ntohl((x)->dst.u.all))

#define PRINT_TUPLE_RAW(x)                                              \
       printk("tuple %p: %u %u.%u.%u.%u:0x%08x -> %u.%u.%u.%u:0x%08x\n",\
       (x), (x)->dst.protonum,                                         \
       NIPQUAD((x)->src.ip), ntohl((x)->src.u.all),                    \
       NIPQUAD((x)->dst.ip), ntohl((x)->dst.u.all))
		
u_int32_t find_battle_by_client( u_int32_t ip );

#endif
