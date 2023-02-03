#ifndef _IP_CT_URL_BLOCKING_H_
#define _IP_CT_URL_BLOCKING_H_

#define HTTP_PORT 80

#define MAX_URL_LIST	32
#define MAX_URL_LEN		34
#define MAX_BUF_SIZE	(MAX_URL_LIST * MAX_URL_LEN)

#define URL_FILENAME "url_blocking_list"

#define DUMP_TUPLE_RAW(x)                                              \
       DEBUGP("tuple %p: %u %u.%u.%u.%u:0x%08x -> %u.%u.%u.%u:0x%08x\n",\
       (x), (x)->dst.protonum,                                         \
       NIPQUAD((x)->src.ip), ntohl((x)->src.u.all),                    \
       NIPQUAD((x)->dst.ip), ntohl((x)->dst.u.all))
		
#endif
