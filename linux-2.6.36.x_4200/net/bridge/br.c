/*
 *	Generic parts
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/llc.h>
#include <net/llc.h>
#include <net/stp.h>

#include "br_private.h"

#if defined(_IPV6_BRIDGE_) || defined(_IGMP_L2_PROXY_)
struct proc_dir_entry *procwanIfx=NULL;

#define MAX_IFX_NAME 32
char _wanIfx_[MAX_IFX_NAME]="eth2.2";

static int br_wanIfxread_proc(char *page, char **start, off_t off,
                int count, int *eof, void *data)
{

      int len;

      //printk( "procfile_read called, wanIfx=%s\n", _wanIfx_);

      len = sprintf(page, "%s\n", _wanIfx_);

      if (len <= off+count) *eof = 1;
      *start = page + off;
      len -= off;
      if (len>count) len = count;
      if (len<0) len = 0;

      return len;

}
static int br_wanIfxwrite_proc(struct file *file, const char *buffer,
                      unsigned long count, void *data)
{
	char temp[MAX_IFX_NAME];
	int i;

      if (count < 2)
            return -EFAULT;

/*      if (buffer && !copy_from_user(_wanIfx_, buffer, MAX_IFX_NAME)) {
            return count;
      }*/

      if (buffer && !copy_from_user(temp, buffer, MAX_IFX_NAME)) {

    	  for(i=0; i<MAX_IFX_NAME-1 && temp[i]!='\n' && temp[i]!='\0'; i++)
        	  _wanIfx_[i]=temp[i];

          _wanIfx_[i]=0;

          return count;
      }
      return -EFAULT;
}

#endif

#ifdef IGMP_SNOOPING
struct proc_dir_entry *procigmp=NULL;
int igmpsnoopenabled=1;


static int br_igmpread_proc(char *page, char **start, off_t off,
                int count, int *eof, void *data)
{

      int len;

      len = sprintf(page, "%c\n", igmpsnoopenabled + '0');


      if (len <= off+count) *eof = 1;
      *start = page + off;
      len -= off;
      if (len>count) len = count;
      if (len<0) len = 0;
      return len;

}

static int br_igmpwrite_proc(struct file *file, const char *buffer,
                      unsigned long count, void *data)
{
      unsigned char br_tmp;
      if (count < 2)
            return -EFAULT;

      if (buffer && !copy_from_user(&br_tmp, buffer, 1)) {
            igmpsnoopenabled = br_tmp - '0';
            return count;
      }
      return -EFAULT;
}

#endif

#ifdef MCAST_TO_UNICAST

struct proc_dir_entry *procIgmpProxy = NULL;

int IGMPProxyOpened = 0;

//#define IGMP_Proxy_DBG

static int br_igmpProxyRead_proc(char *page, char **start, off_t off, 
		int count, int *eof, void *data)
{
#ifdef	IGMP_Proxy_DBG
	  printk("br_igmpProxyRead_proc()\n");	
#endif	  
      int len;
      len = sprintf(page, "%c\n", IGMPProxyOpened + '0');

      if (len <= off+count) *eof = 1;
      *start = page + off;
      len -= off;
      if (len>count) len = count;
      if (len<0) len = 0;
      return len;

}
static int br_igmpProxyWrite_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	#ifdef IGMP_Proxy_DBG
	printk("\nbr_igmpProxyWrite_proc()\n");	
	printk("count=%d\n",count);
	printk("buffer=%s\n",buffer);	  	  
	#endif	
    unsigned char chartmp; 
	  
    if (count > 1) {	//call from shell
      	if (buffer && !copy_from_user(&chartmp, buffer, 1)) {
	    	IGMPProxyOpened = chartmp - '0';
			#ifdef	IGMP_Proxy_DBG			
			printk("IGMPProxyOpened=%d\n",IGMPProxyOpened);
			#endif
			
	    }
	}else if(count==1){//call from demon(demon direct call br's ioctl)
			//memcpy(&chartmp,buffer,1);
			if(buffer){
				get_user(chartmp,buffer);	
		    	IGMPProxyOpened = chartmp - '0';
			}else
				return -EFAULT;
			#ifdef	IGMP_Proxy_DBG			
			printk("IGMPProxyOpened=%d\n",IGMPProxyOpened);
			#endif			

	}else{
		#ifdef	IGMP_Proxy_DBG			
		printk("br_igmpProxyWrite_proc fail\n");
		#endif			
		return -EFAULT;
	}
	return count;
}
#endif

int (*br_should_route_hook)(struct sk_buff *skb);

static const struct stp_proto br_stp_proto = {
	.rcv	= br_stp_rcv,
};

static struct pernet_operations br_net_ops = {
	.exit	= br_net_exit,
};

static int __init br_init(void)
{
	int err;
//EDX start
#if defined(_IPV6_BRIDGE_) || defined(_IGMP_L2_PROXY_)
	procwanIfx = create_proc_entry("br_wanIfx", 0, NULL);
	if (procwanIfx) {
		procwanIfx->read_proc = (read_proc_t*)&br_wanIfxread_proc;
		procwanIfx->write_proc =  (write_proc_t*)&br_wanIfxwrite_proc;
	}
#endif

#if defined(_IPV6_BRIDGE_)
	procipv6 = create_proc_entry("br_ipv6", 0, NULL);
	if (procipv6) {
		procipv6->read_proc = (read_proc_t*)&br_ipv6read_proc;
		procipv6->write_proc = (write_proc_t*)&br_ipv6write_proc;
	}
#endif

#ifdef IGMP_SNOOPING
	procigmp = create_proc_entry("br_igmpsnoop", 0, NULL);
	if (procigmp) {
		procigmp->read_proc = (read_proc_t*)&br_igmpread_proc;
		procigmp->write_proc = (write_proc_t*)&br_igmpwrite_proc;
	}
#endif

#ifdef MCAST_TO_UNICAST	
		procIgmpProxy = create_proc_entry("br_igmpProxy", 0, NULL);
		if (procIgmpProxy) {
		    procIgmpProxy->read_proc = (read_proc_t*)&br_igmpProxyRead_proc;
		    procIgmpProxy->write_proc = (write_proc_t*)&br_igmpProxyWrite_proc;
		}
#endif

	err = stp_proto_register(&br_stp_proto);
	if (err < 0) {
		pr_err("bridge: can't register sap for STP\n");
		return err;
	}

	err = br_fdb_init();
	if (err)
		goto err_out;

	err = register_pernet_subsys(&br_net_ops);
	if (err)
		goto err_out1;

	err = br_netfilter_init();
	if (err)
		goto err_out2;

	err = register_netdevice_notifier(&br_device_notifier);
	if (err)
		goto err_out3;

	err = br_netlink_init();
	if (err)
		goto err_out4;

	brioctl_set(br_ioctl_deviceless_stub);

#if defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE)
	br_fdb_test_addr_hook = br_fdb_test_addr;
#endif
	return 0;
err_out4:
	unregister_netdevice_notifier(&br_device_notifier);
err_out3:
	br_netfilter_fini();
err_out2:
	unregister_pernet_subsys(&br_net_ops);
err_out1:
	br_fdb_fini();
err_out:
	stp_proto_unregister(&br_stp_proto);
	return err;
}

static void __exit br_deinit(void)
{
	stp_proto_unregister(&br_stp_proto);

	br_netlink_fini();
	unregister_netdevice_notifier(&br_device_notifier);
	brioctl_set(NULL);

	unregister_pernet_subsys(&br_net_ops);

	rcu_barrier(); /* Wait for completion of call_rcu()'s */

	br_netfilter_fini();
#if defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE)
	br_fdb_test_addr_hook = NULL;
#endif

	br_fdb_fini();
}

EXPORT_SYMBOL(br_should_route_hook);

module_init(br_init)
module_exit(br_deinit)
MODULE_LICENSE("GPL");
MODULE_VERSION(BR_VERSION);
