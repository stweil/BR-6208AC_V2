/*  weburl --	A netfilter module to match URLs in HTTP requests 
 *  		This module can match using string match or regular expressions
 *  		Originally designed for use with Gargoyle router firmware (gargoyle-router.com)
 *
 *
 *  Copyright © 2008-2010 by Eric Bishop <eric@gargoyle-router.com>
 * 
 *  This file is free software: you may copy, redistribute and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_weburl.h>

//#include "weburl_deps/regexp.c"
#include <linux/regexp.h>
#include "weburl_deps/tree_map.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	#define ipt_register_match      xt_register_match
	#define ipt_unregister_match    xt_unregister_match
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
	#include <linux/ip.h>
#else
	#define skb_network_header(skb) (skb)->nh.raw 
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
	#include <linux/netfilter/x_tables.h>
#endif



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Bishop");
MODULE_DESCRIPTION("Match URL in HTTP requests, designed for use with Gargoyle web interface (www.gargoyle-router.com)");

string_map* compiled_map = NULL;

void dump_hex(char *p, int len)
{
        int     n = 0 ;
        while (len--) {
                n++ ;
                printf("%x%s",*p++ & 0xff,len ? ( (n & 7) ? " " : "-") : "") ;
        }
}


int weburl_strnicmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count)
	{
		if ((__res = toupper( *cs ) - toupper( *ct++ ) ) != 0 || !*cs++)
		{
			break;
		}
		count--;
	}
	return __res;
}

char *strnistr(const char *s, const char *find, size_t slen)
{
	char c, sc;
	size_t len;


	if ((c = *find++) != '\0') 
	{
		len = strlen(find);
		do
		{
			do
			{
      				if (slen < 1 || (sc = *s) == '\0')
				{
      					return (NULL);
				}
      				--slen;
      				++s;
      			}
			while ( toupper(sc) != toupper(c));
      			
			if (len > slen)
			{
      				return (NULL);
			}
      		}
		while (weburl_strnicmp(s, find, len) != 0);
      		
		s--;
      	}
      	return ((char *)s);
}


int do_match_test(unsigned char match_type,  const char* reference, char* query)
{
	int matches = 0;
	struct regexp* r;
	switch(match_type)
	{
		case WEBURL_CONTAINS_TYPE:
			matches = (strstr(query, reference) != NULL);
			break;
		case WEBURL_REGEX_TYPE:

			if(compiled_map == NULL)
			{
				compiled_map = initialize_map(0);
				if(compiled_map == NULL) /* test for malloc failure */
				{
					return 0;
				}
			}
			r = (struct regexp*)get_map_element(compiled_map, reference);
			if(r == NULL)
			{
				int rlen = strlen(reference);
				r= regcomp((char*)reference, &rlen);
				if(r == NULL) /* test for malloc failure */
				{
					return 0;
				}
				set_map_element(compiled_map, reference, (void*)r);
			}
			matches = regexec(r, query);
			break;
		case WEBURL_EXACT_TYPE:
			matches = (strstr(query, reference) != NULL) && strlen(query) == strlen(reference);
			break;
	}
	return matches;
}

int http_match(const struct ipt_weburl_info* info, const unsigned char* packet_data, int packet_length)
{
	int test = 0; 
	
	/* first test if we're dealing with a web page request */
	if(weburl_strnicmp((char*)packet_data, "GET ", 4) == 0 || weburl_strnicmp(  (char*)packet_data, "POST ", 5) == 0 || weburl_strnicmp((char*)packet_data, "HEAD ", 5) == 0)
	{
		//printk ("%s %d Found a web page request\n", __FUNCTION__, __LINE__);
		char path[625] = "";
		char host[625] = "";
		int path_start_index;
		int path_end_index;
		int last_header_index;
		char last_two_buf[2];
		int end_found;
		char* host_match;
		char* test_prefixes[6];
	
		/* get path portion of URL */
		path_start_index = (int)(strstr((char*)packet_data, " ") - (char*)packet_data);
		while( packet_data[path_start_index] == ' ')
		{
			path_start_index++;
		}
		path_end_index= (int)(strstr( (char*)(packet_data+path_start_index), " ") -  (char*)packet_data);
		if(path_end_index > 0) 
		{
			int path_length = path_end_index-path_start_index;
			path_length = path_length < 625 ? path_length : 624; /* prevent overflow */
			memcpy(path, packet_data+path_start_index, path_length);
			path[ path_length] = '\0';
		}
		
		/* get header length */
		last_header_index = 2;
		memcpy(last_two_buf,(char*)packet_data, 2);
		end_found = 0;
		while(end_found == 0 && last_header_index < packet_length)
		{
			char next = (char)packet_data[last_header_index];
			if(next == '\n')
			{
				end_found = last_two_buf[1] == '\n' || (last_two_buf[0] == '\n' && last_two_buf[1] == '\r') ? 1 : 0;
			}
			if(end_found == 0)
			{
				last_two_buf[0] = last_two_buf[1];
				last_two_buf[1] = next;
				last_header_index++;
			}
		}
		
		/* get host portion of URL */
		host_match = strnistr( (char*)packet_data, "Host:", last_header_index);
		if(host_match != NULL)
		{
			int host_end_index;
			char* port_ptr;
			host_match = host_match + 5; /* character after "Host:" */
			while(host_match[0] == ' ')
			{
				host_match = host_match+1;
			}
			
			host_end_index = 0;
			while(	host_match[host_end_index] != '\n' && 
				host_match[host_end_index] != '\r' && 
				host_match[host_end_index] != ' ' && 
				host_match[host_end_index] != ':' && 
				((char*)host_match - (char*)packet_data)+host_end_index < last_header_index 
				)
			{
				host_end_index++;
			}
			memcpy(host, host_match, host_end_index);
			host_end_index = host_end_index < 625 ? host_end_index : 624; /* prevent overflow */
			host[host_end_index] = '\0';

			
		}
	
		//printk ("%s %d host = %s path = %s\n", __FUNCTION__, __LINE__, host, path);
		

		switch(info->match_part)
		{
			case WEBURL_DOMAIN_PART:
				test = do_match_test(info->match_type, info->test_str, host);
				if(!test && strstr(host, "www.") == host)
				{
					test = do_match_test(info->match_type, info->test_str, ((char*)host+4) );	
				}
				break;
			case WEBURL_PATH_PART:
				test = do_match_test(info->match_type, info->test_str, path);
				if( !test && path[0] == '/' )
				{
					test = do_match_test(info->match_type, info->test_str, ((char*)path+1) );
				}
				break;
			case WEBURL_ALL_PART:
				test_prefixes[0] = "http://";
				test_prefixes[1] = "";
				test_prefixes[2] = NULL;

				
				int prefix_index;
				for(prefix_index=0; test_prefixes[prefix_index] != NULL && test == 0; prefix_index++)
				{
					char test_url[1250];
					test_url[0] = '\0';
					strcat(test_url, test_prefixes[prefix_index]);
					strcat(test_url, host);
					if(strcmp(path, "/") != 0)
					{
						strcat(test_url, path);
					}
					test = do_match_test(info->match_type, info->test_str, test_url);
					if(!test && strcmp(path, "/") == 0)
					{
						strcat(test_url, path);
						test = do_match_test(info->match_type, info->test_str, test_url);
					}
					
					//printk ("%s %d test_url = %s test = %d\n", __FUNCTION__, __LINE__, test_url, test);
				}
				if(!test && strstr(host, "www.") == host)
				{
					char* www_host = ((char*)host+4);
					for(prefix_index=0; test_prefixes[prefix_index] != NULL && test == 0; prefix_index++)
					{
						char test_url[1250];
						test_url[0] = '\0';
						strcat(test_url, test_prefixes[prefix_index]);
						strcat(test_url, www_host);
						if(strcmp(path, "/") != 0)
						{
							strcat(test_url, path);
						}
						test = do_match_test(info->match_type, info->test_str, test_url);
						if(!test && strcmp(path, "/") == 0)
						{
							strcat(test_url, path);
							test = do_match_test(info->match_type, info->test_str, test_url);
						}
					
//						printk ("%s %d test_url = %s test = %d\n", __FUNCTION__, __LINE__, test_url, test);
					}
				}
				break;
			
		}		


		/* 
		 * If invert flag is set, return true if this IS a web request, but it didn't match 
		 * Always return false for non-web requests
		 */
		test = info->invert ? !test : test;
	}

	return test;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
		static bool 
	#else
		static int
	#endif
	match(		const struct sk_buff *skb,
			const struct net_device *in,
			const struct net_device *out,
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
				const struct xt_match *match,
			#endif
			const void *matchinfo,
			int offset,
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
				unsigned int protoff,
			#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
				const void *hdr,
				u_int16_t datalen,
			#endif
			#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
				bool *hotdrop
			#else
				int *hotdrop
			#endif	
			)
#else
	static bool match(const struct sk_buff *skb, const struct xt_match_param *par)
#endif
{
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
		const struct ipt_weburl_info *info = (const struct ipt_weburl_info*)matchinfo;
	#else
		const struct ipt_weburl_info *info = (const struct ipt_weburl_info*)(par->matchinfo);
	#endif

	
	int test = 0;
	struct iphdr* iph;	

	/* linearize skb if necessary */
	struct sk_buff *linear_skb;
	int skb_copied;
	if(skb_is_nonlinear(skb))
	{
		linear_skb = skb_copy(skb, GFP_ATOMIC);
		skb_copied = 1;
	}
	else
	{
		linear_skb = (struct sk_buff*)skb;
		skb_copied = 0;
	}

	

	/* ignore packets that are not TCP */
	iph = (struct iphdr*)(skb_network_header(skb));
	if(iph->protocol == IPPROTO_TCP)
	{
		/* get payload */
		struct tcphdr* tcp_hdr		= (struct tcphdr*)( ((unsigned char*)iph) + (iph->ihl*4) );
		unsigned short payload_offset 	= (tcp_hdr->doff*4) + (iph->ihl*4);
		unsigned char* payload 		= ((unsigned char*)iph) + payload_offset;
		unsigned short payload_length	= ntohs(iph->tot_len) - payload_offset;

	

		/* if payload length <= 10 bytes don't bother doing a check, otherwise check for match */
		if(payload_length > 10)
		{
			test = http_match(info, payload, payload_length);
		}
	}
	
	/* free skb if we made a copy to linearize it */
	if(skb_copied == 1)
	{
		kfree_skb(linear_skb);
	}


//	printk ("%s %d returning %d from weburl\n", __FUNCTION__, __LINE__, test);
	return test;
}



#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
	static bool
	#else
	static int
	#endif
	checkentry(	const char *tablename,
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
				const void *ip,
				const struct xt_match *match,
	#else
				const struct ipt_ip *ip,
	#endif
				void *matchinfo,
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
		    		unsigned int matchsize,
	#endif
				unsigned int hook_mask
				)
#else
	static bool checkentry(const struct xt_mtchk_param *par)
#endif
{
	return 1;
}


static struct ipt_match weburl_match = 
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	{ NULL, NULL },
	"weburl",
	&match,
	&checkentry,
	NULL,
	THIS_MODULE
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
	.name		= "weburl",
	.match		= &match,
	.family		= AF_INET,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
	.matchsize	= sizeof(struct ipt_weburl_info),
#endif
	.checkentry	= &checkentry,
	.me		= THIS_MODULE,
#endif
};

static int __init init(void)
{
	compiled_map = NULL;
	return ipt_register_match(&weburl_match);

}

static void __exit fini(void)
{
	ipt_unregister_match(&weburl_match);
	if(compiled_map != NULL)
	{
		unsigned long num_destroyed;
		destroy_map(compiled_map, DESTROY_MODE_FREE_VALUES, &num_destroyed);
	}
}

module_init(init);
module_exit(fini);

