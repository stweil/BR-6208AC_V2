/*
 * Copyright (C) 2011-2021 by Acelink networks
 * Author: Morris Shih (morris@edimax.com.tw)
 *
 * This software may be used and distributed
 * according to the terms of the GNU General Public License,
 * incorporated herein by reference.
 *
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>                       
#include <linux/reboot.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <asm/rt2880/surfboardint.h>
#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtl8367b_asicdrv_port.h>
#include <smi.h>
#include <rtl8367b_asicdrv_phy.h>
//#include "edx_events.h"
#include "edxtools.h"
#include "../../net/bridge/br_private.h"
#include "../../net/bridge/br_edx.h"

int (*wifi_proc)(struct notifier_block *this, unsigned long event, void *ptr);
EXPORT_SYMBOL(wifi_proc);
#if	1	//EDX Randall
int (*wps_proc)(struct notifier_block *this, unsigned long event, void *ptr);
EXPORT_SYMBOL(wps_proc);
#endif

void (*edx_aband_led_ctrl)(int onoff);
EXPORT_SYMBOL(edx_aband_led_ctrl);

int edx_raw_notifier_chain_register(struct raw_notifier_head *nh, struct notifier_block *nb)
{
	return raw_notifier_chain_register(nh, nb);
}

int edx_raw_notifier_chain_unregister(struct raw_notifier_head *nh, struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(nh, nb);
}

int edx_raw_notifier_call_chain(struct raw_notifier_head *nh, int event, void *arg)
{
	return raw_notifier_call_chain(nh, event, arg);
}

EXPORT_SYMBOL(edx_raw_notifier_chain_register);
EXPORT_SYMBOL(edx_raw_notifier_chain_unregister);
EXPORT_SYMBOL(edx_raw_notifier_call_chain);

rtk_api_ret_t (*edx_rtk_api)(rtk_port_t port, rtk_port_phy_ability_t *pAbility);
rtk_api_ret_t (*link_get)(rtk_port_t port, rtk_port_phy_reg_t reg, rtk_port_phy_data_t *pData);
rtk_api_ret_t (*link_set)(rtk_port_t port, rtk_port_phy_reg_t reg, rtk_port_phy_data_t regData);
rtk_api_ret_t (*led_set)(rtk_led_group_t group, rtk_portmask_t portmask);
rtk_api_ret_t (*led_mode_set)(rtk_led_mode_t mode);

EXPORT_SYMBOL(led_set);
EXPORT_SYMBOL(led_mode_set);
EXPORT_SYMBOL(link_get);
EXPORT_SYMBOL(edx_rtk_api);
EXPORT_SYMBOL(link_set);

#ifdef EDX_TABLES
int (*edx_tables_if)(struct net_bridge_port *p, struct sk_buff *skb);
int (*edx_tables_m2u)(struct net_bridge *br, struct sk_buff *skb, int type);
int (*edx_tables_deliver)(struct sk_buff *skb, struct net_device *dev, const unsigned char *dest);
void (*edx_tables_multicast)(struct net_bridge *br, struct net_bridge_fdb_entry *dst, struct sk_buff *skb, void (*__packet_hook)(struct net_bridge_port *p, struct sk_buff *skb));
int (*edx_change_mac)(struct sk_buff *skb);
int	EdxM2UEnabled = 0;
#endif

#ifdef EDX_MAC2IP
int (*edx_mac2ip_peep)(struct net_bridge_fdb_entry *fdb, struct sk_buff *skb);
#endif

#ifdef EDX_CHILDNET
// childnet
int (*edx_childnet)(struct sk_buff *skb);
EXPORT_SYMBOL(edx_childnet);
#endif

#ifdef EDX_NOTIFY	// EDX SJC
RAW_NOTIFIER_HEAD(edx_wifi_chain);
struct wifi_args_t {
	int		node_num;
	char	ifname[33];
};

#define IW_ASSOC_EVENT_FLAG                         0x0200
#define IW_STA_LINKUP_EVENT_FLAG                    0x020F
#define IW_WPS_END_EVENT_FLAG                       0x021B

int EDXEvent(unsigned int Event, char *pifname)
{
	struct wifi_args_t	args;

	if (wifi_proc) {
		args.node_num = 1;
		strncpy(args.ifname, pifname, sizeof(args.ifname));
		if (Event == IW_ASSOC_EVENT_FLAG) {
			wifi_proc(&edx_wifi_chain, 3, &args);
//		} else if (Event == IW_DISASSOC_EVENT_FLAG) {
//			wifi_proc(&edx_wifi_chain, 4, &args);
		} else if (Event == IW_STA_LINKUP_EVENT_FLAG) {
			wifi_proc(&edx_wifi_chain, 6, &args);
		} else if (Event == IW_WPS_END_EVENT_FLAG) {
			printk("%s enrollee success\n", pifname);
			wifi_proc(&edx_wifi_chain, 0, &args);
		}
	}
	return 0;
}

EXPORT_SYMBOL(EDXEvent);
#endif	// EDX_NOTIFY

int edx_dev_ioctl(struct net *net, unsigned int cmd, void __user *arg)
{
	return dev_ioctl(net, cmd, arg);
}
EXPORT_SYMBOL(edx_dev_ioctl);

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry	*edx_proc_dir;
#endif

static int __init init_farm(void)
{
	edx_aband_led_ctrl = NULL;
	wifi_proc = NULL;
#ifdef EDX_TABLES
	edx_tables_if = NULL;
	edx_tables_m2u = NULL;
	edx_tables_deliver = NULL;
	edx_tables_multicast = NULL;
	edx_change_mac = NULL;
#endif
#ifdef EDX_MAC2IP
	edx_mac2ip_peep = NULL;
#endif
#ifdef EDX_CHILDNET
	edx_childnet = NULL;
#endif
#ifdef CONFIG_PROC_FS
	if ((edx_proc_dir = proc_mkdir(EDX_PROC_TOP_DIR, NULL)) == NULL) {
		printk("unable to mkdir for proc,fail !\n");
		return -ENOMEM;
	}
#endif
	return 0;
}

static void __exit deinit_farm(void)
{
#ifdef CONFIG_PROC_FS
	if (edx_proc_dir)
		remove_proc_entry(EDX_PROC_TOP_DIR, NULL);
#endif
}

module_init(init_farm);
module_exit(deinit_farm);

#ifdef EDX_TABLES
EXPORT_SYMBOL(EdxM2UEnabled);
EXPORT_SYMBOL(edx_tables_if);
EXPORT_SYMBOL(edx_tables_m2u);
EXPORT_SYMBOL(edx_tables_deliver);
EXPORT_SYMBOL(edx_tables_multicast);
EXPORT_SYMBOL(edx_change_mac);
#endif

#ifdef EDX_MAC2IP
EXPORT_SYMBOL(edx_mac2ip_peep);
#endif

