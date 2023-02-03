/*
 * Licensed under GNU GPL version 2 Copyright Lance Wu 
 * Version: 0.0.1
 *
 * 2003.07.02
 * 	- URL blocking
 *
 */

#include <linux/config.h>
#include <linux/module.h>

#include <linux/proc_fs.h>

#include <linux/ip.h>
#include <net/tcp.h>
//#include <net/udp.h>
#include <linux/ctype.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include "ip_conntrack_urlblock.h"

#include <net/checksum.h>

MODULE_AUTHOR("Lance Wu ");
MODULE_DESCRIPTION("Netfilter connection tracking module for URL blocking");
MODULE_LICENSE("GPL");

#if 0
//#define DEBUGP(format, args...) printk(__FILE__ ":" __FUNCTION__ ": " \
//				       format, ## args)
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

static struct ip_conntrack_helper url;
static char url_names[10];

char		 url_list[MAX_URL_LIST][MAX_URL_LEN];
int          url_count;


int proc_match(int len, const char *name,struct proc_dir_entry * de)
{
    if( !de || !de->low_ino )
        return 0;
    if( de->namelen != len )
        return 0;
    return !memcmp( name, de->name, len );
}

static int find_dir(char* buf, unsigned int buflen )
{
	char* ptr1;
	char* ptr2;
	const char header[] = "GET /";
//	unsigned int slen=0;
	
	ptr1 = ptr2 = buf;
	
	if( !memcmp( ptr1, header, strlen(header) ) )
	{
		return ( strlen(header) );
	}

	return 0;
}

static int find_host(char* buf, unsigned int buflen )
{
	char* ptr1;
	char* ptr2;
//	const char header[] = "GET / HTTP";
	const char header[] = "GET /";
	const char item[] = "Host: ";
	unsigned int slen=0;
//	int i=0;
//	char buffer[100]={0};
	
	ptr1 = ptr2 = buf;
	
	if( !memcmp( ptr1, header, strlen(header) ) )
	{
//		DEBUGP("find header!!\n");

		while( *ptr2 != '\r' )
			ptr2++;
		while( (*ptr2 == '\r') || (*ptr2 == '\n') )
			ptr2++;
		slen += (ptr2 - ptr1);
		if( slen >= buflen )
			return 0;
		ptr1 = ptr2;

		while( memcmp( ptr1, item, strlen(item) ) )
		{
	        while( *ptr2 != '\r' )
	            ptr2++;
			while( (*ptr2 == '\r') || (*ptr2 == '\n') )
	        	ptr2++;
	        slen += (ptr2 - ptr1);
	        if( slen > buflen )
	            return 0;
	        ptr1 = ptr2;													
		}
//		DEBUGP("find item!!\n");
		
		return ( ptr1 - buf + strlen(item) );
	}

	return 0;
}

static int url_help(
		    struct sk_buff **pskb,
			struct ip_conntrack *ct,
		    enum ip_conntrack_info ctinfo)
{
	struct iphdr *iph = (*pskb)->nh.iph; 
    struct tcphdr *tcph = (void *)iph + iph->ihl * 4;
	const char* data = (const char*)tcph + tcph->doff * 4;
    unsigned int tcplen = (*pskb)->len - iph->ihl * 4;
    int dir = CTINFO2DIR(ctinfo);
    unsigned int datalen = tcplen - tcph->doff * 4;
	int offset = 0;

	if( (dir != IP_CT_DIR_ORIGINAL) || (url_count == 0) )
	{
		return NF_ACCEPT;
	}

	DEBUGP("dir= %s\n",(dir == IP_CT_DIR_ORIGINAL)?"ORIGINAL":"REPLY");

	DEBUGP("from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
			NIPQUAD(iph->saddr),ntohs(tcph->source),
			NIPQUAD(iph->daddr),ntohs(tcph->dest));
	
	if( datalen == 0 )
	{
		return NF_ACCEPT;
	}

	if( (offset = find_dir( data, datalen )) != 0 )
	{
		int i=0;
		char buffer[MAX_URL_LEN]={0};
		char* pdata;

		pdata = data + offset;
		
		while( (*pdata != '\r') && (*pdata != '\n') )
		{
			buffer[i] = *pdata;
			i++;
			pdata++;
			if (i == MAX_URL_LEN)
				break;
		}

		for( i=0; i<MAX_URL_LIST; i++ )
		{
			if( !url_list[i][0] )
				continue;
			DEBUGP("11111111111buffer=%s,url=%s\n",buffer,url_list[i]);
			if( strstr(buffer, url_list[i]) != NULL )
			{
				return NF_DROP;
			}
		}
	}


	if( (offset = find_host( data, datalen )) != 0 )
	{
		int i=0,j;
		char buffer[MAX_URL_LEN]={0};
		char* pdata;

		pdata = data + offset;
		
		while( (*pdata != '\r') && (*pdata != '\n') )
		{
			buffer[i] = *pdata;
			i++;
			pdata++;
			if (i == MAX_URL_LEN)
				break;
		}
//		DEBUGP("Host: %s\n",buffer);

		for( i=0; i<MAX_URL_LIST; i++ )
		{
			if( url_list[i][0] == 0 )
				continue;
			for( j=0; j<MAX_URL_LEN; j++ )
			{
//				DEBUGP("2222222222buffer=%s,url=%s\n",buffer[j],url_list[i]);
				if( buffer[j] == url_list[i][0] )
				{
					if( !memcmp(&buffer[j], url_list[i], strlen(url_list[i])) )
					{
//						DEBUGP("match: list-%d [%s]\n",i,url_list[i]);
						return NF_DROP;
					}
				}
			}
		}

	}
	
	return NF_ACCEPT;
}

static void fini(void)
{
	DEBUGP("unregistering helper for URL blocking\n");
	ip_conntrack_helper_unregister( &url );
}

static int parse_data( unsigned char* buffer , int buflen)
{
	int i, j=0;
	char* ptr = buffer;
	
	url_count = 0;
	
	while( *ptr != '\0' )
	{
		if( (*ptr == '\r') || (*ptr) == '\n' )
		{
//			DEBUGP("end string\n");
			url_count++;
			j = 0;
			if( url_count == MAX_URL_LIST )
				return 1;
			while( (*ptr == '\r') || (*ptr == '\n') )
				ptr++;
		}
		url_list[url_count][j] = *ptr;
		ptr++;
		j++;
		if( j == (MAX_URL_LEN-1) )
		{
			return 1;
		}
	}


	// { RexHua add for ignor http:// from url_list	
	for(i=0; i < url_count; i++)
	{
		if((url_list[i][0] == 'h') || (url_list[i][0] == 'H'))
		 if((url_list[i][1] == 't') || (url_list[i][1] == 'T'))
		  if((url_list[i][2] == 't') || (url_list[i][2] == 'T'))
		   if((url_list[i][3] == 'p') || (url_list[i][3] == 'P'))
		    if((url_list[i][4] == ':'))
		     if((url_list[i][5] == '/'))
		      if((url_list[i][6] == '/'))
			  {
			   	for(j=0; j< MAX_URL_LEN - 7; j++)
					url_list[i][j] = url_list[i][j+7];
			  }
	}
	// } RexHua add for ignor http:// from url_list	
		
		
	return 1;
}

static int read_url_procfile(void)
{
	struct proc_dir_entry* de;
	unsigned char buffer[MAX_BUF_SIZE];
	char* start;
	int eof=0;
	int len=0;
	
    de = &proc_root;

    for( de=de->subdir; de; de=de->next )
    {
//		DEBUGP("debug: %s , len= %d\n",de->name, de->namelen);
		
        if( proc_match( strlen(URL_FILENAME), URL_FILENAME, de) )
        {
	        DEBUGP("found procfile...[%s]\n",URL_FILENAME);
			start = NULL;
            if( (len = de->read_proc( buffer, &start, 0, MAX_BUF_SIZE, &eof, de->data )) )
			{
				buffer[len] = '\0';
//				DEBUGP("len= %d filedata= %s\n",len,buffer);
				parse_data( buffer, len );
				{
					int i;
					DEBUGP("url_count= %d\n",url_count);
					for( i=0; i< url_count; i++ )
					{
						DEBUGP("data %d = %s\n",i,url_list[i]);
					}
				}
				return 0;
			}
			break;
		}
	}
    if( !de )
	    DEBUGP("procfile [%s] not found!!\n", URL_FILENAME);
	
	return 0;
}


static int __init init(void)
{
	int ret;

	url_count = 0;
	memset( url_list, 0, sizeof(url_list) );

	read_url_procfile();
	memset( &url, 0, sizeof(struct ip_conntrack_helper) );

    url.tuple.dst.protonum = IPPROTO_TCP;
    url.tuple.src.u.tcp.port = htons( HTTP_PORT );
    url.mask.dst.protonum = 0xFF;
    url.mask.src.u.tcp.port = 0xFFFF;
	url.max_expected = 1;
	url.timeout =  5 * 60;
	url.me = THIS_MODULE;
	url.help = url_help;

    DEBUGP("3344\n");
	sprintf(url_names, "url");
	url.name=url_names;

    DEBUGP("4444\n");
	
	ret = ip_conntrack_helper_register( &url );

    DEBUGP("5555\n");

	if( ret || (url_count==0 ) ) 
	{
		DEBUGP("ERROR registering helper for URL blocking %x\n");
		fini();
		return(ret);
	}
	DEBUGP("register helper for URL blocking SUCCESS!!\n");
	
	return(0);
}

module_init(init);
module_exit(fini);
