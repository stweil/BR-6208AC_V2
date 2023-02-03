#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtl8367b_asicdrv_port.h>
#include <rtl8367b_asicdrv_phy.h>
#include <smi.h>
//EDX shakim start
extern rtk_api_ret_t rtk_switch_init(void);
extern rtk_api_ret_t rtk_port_phyAutoNegoAbility_set(rtk_port_t, rtk_port_phy_ability_t *);
extern rtk_api_ret_t rtk_vlan_init(void);
extern rtk_vlan_portPvid_set(rtk_port_t , rtk_vlan_t, rtk_pri_t );
extern rtk_api_ret_t rtk_vlan_set(rtk_vlan_t , rtk_portmask_t , rtk_portmask_t, rtk_fid_t);
extern rtk_api_ret_t rtk_vlan_get(rtk_vlan_t vid, rtk_portmask_t *pMbrmsk, rtk_portmask_t *pUntagmsk, rtk_fid_t *pFid);
extern rtk_api_ret_t rtk_l2_addr_next_get(rtk_l2_read_method_t read_method, rtk_port_t port, rtk_uint32 *pAddress, rtk_l2_ucastAddr_t *pL2_data);
//EDX shakim end

//EDX Timmy start for wan detect
extern rtk_api_ret_t rtk_port_phyStatus_get(rtk_port_t port, rtk_port_linkStatus_t *pLinkStatus, rtk_data_t *pSpeed, rtk_data_t *pDuplex);
//EDX Timmy end

static int model, led_mode, vlan, led_all;

module_param (model, int, 0);
MODULE_PARM_DESC(model, "Model define");

module_param (led_mode, int, 0);
MODULE_PARM_DESC(led_mode, "Led mode");

module_param (vlan, int, 1);
MODULE_PARM_DESC(vlan, "VLAN");

module_param (led_all, int, 0);
MODULE_PARM_DESC(led_all, "ALL Led Control");

int port_led (struct config_tbl *config)
{
	rtk_portmask_t          ledmask;
	
	if ( (config->model == WLLLL) && (config->led_mode == LED_LAN_PORT_OFF))
		ledmask.bits[0] = 0x01; /* port 0 ~ port 4 LED */

	else if ( (config->model == LLLLW) && (config->led_mode == LED_LAN_PORT_OFF))
		ledmask.bits[0] = 0x10; /* port 0 ~ port 4 LED */

	else 
		ledmask.bits[0] = 0x1F; /* port 0 ~ port 4 LED */

	if(rtk_led_enable_set(LED_GROUP_0, ledmask) != RT_ERR_OK)
		return RT_ERR_FAILED;

	if(rtk_led_enable_set(LED_GROUP_1, ledmask) != RT_ERR_OK)
		return RT_ERR_FAILED;

	if(rtk_led_enable_set(LED_GROUP_2, ledmask) != RT_ERR_OK)
		return RT_ERR_FAILED;

	switch (config->led_mode){
		case LED_ALL_OFF:
			if(rtk_led_groupConfig_set (LED_GROUP_0, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;

			if(rtk_led_groupConfig_set (LED_GROUP_1, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;

			if(rtk_led_groupConfig_set (LED_GROUP_2, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;
			break;
		
		case LED_LAN_SPPED_100: //EDX shakim
		default:
			if(rtk_led_mode_set(LED_MODE_0) != RT_ERR_OK)
				return RT_ERR_FAILED;
			break;
	}

	switch (config->led_all){
		case 1:
			if(rtk_led_groupConfig_set (LED_GROUP_0, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;

			if(rtk_led_groupConfig_set (LED_GROUP_1, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;

			if(rtk_led_groupConfig_set (LED_GROUP_2, LED_CONFIG_LEDOFF) != RT_ERR_OK)
				return RT_ERR_FAILED;
			break;
		default:
			break;
	}
	
	return RT_ERR_OK;
}

// set Full_1000 will fail ? wait to fix ...?
void port_phy_ability (int switch_model, int led_mode)
{
	rtk_port_phy_ability_t ability;
	int value = 0;
	
	rtk_port_phyEnableAll_set (ENABLED);

	memset (&ability, 0x00, sizeof(ability));
	
	switch (led_mode){
//EDX shakim start
		case LED_LAN_SPPED_100:
		case LED_ALL_OFF:
			ability.Half_100 = 1;
			break;
//EDX shakim end

		default:
		case LED_NORMAL:
			ability.AutoNegotiation = 1;
			ability.Full_1000 = 1;
			ability.FC = 1;
			ability.AsyFC = 0;
			break;
	}	
	
	if ((switch_model == RTL8367R) || (switch_model == RTL8367R_1)){
		printk ("%s %d RTL8367R\n", __FUNCTION__, __LINE__);
		rtk_port_phyAutoNegoAbility_set (0, &ability);
		rtk_port_phyAutoNegoAbility_set (1, &ability);
		rtk_port_phyAutoNegoAbility_set (2, &ability);
		rtk_port_phyAutoNegoAbility_set (3, &ability);
		rtk_port_phyAutoNegoAbility_set (4, &ability);

	}else{
		printk ("%s %d RTL8367R_VB\n", __FUNCTION__, __LINE__);
		rtk_port_phyAutoNegoAbility_set (0, &ability);
		rtk_port_phyAutoNegoAbility_set (1, &ability);
		rtk_port_phyAutoNegoAbility_set (2, &ability);
		rtk_port_phyAutoNegoAbility_set (3, &ability);
		rtk_port_phyAutoNegoAbility_set (4, &ability);

	}
}

int init_rgmii (struct config_tbl *config)
{
	rtk_port_mac_ability_t  mac_cfg;
	rtk_portmask_t		mbrmsk;
	rtk_portmask_t		untagmsk;
	uint32 mode;

	mac_cfg.forcemode = MAC_FORCE;
	mac_cfg.speed     = SPD_1000M;
	mac_cfg.duplex    = FULL_DUPLEX;
	mac_cfg.link      = 1;
	mac_cfg.nway      = 0;
	mac_cfg.rxpause   = 1;
	mac_cfg.txpause   = 1;

	/* Force Ext1 at RGMII, 1G/Full */
	if(rtk_port_macForceLinkExt0_set(MODE_EXT_RGMII, &mac_cfg) != RT_ERR_OK)
		return RT_ERR_FAILED;

	rtk_port_rgmiiDelayExt0_set (0, 0); //EDX shakim


	 /* set port 5 as CPU port */
	 rtk_cpu_enable_set(ENABLE);
	 rtk_cpu_tagPort_set(RTK_EXT_1_MAC, CPU_INSERT_TO_NONE);

	/* LED */

	if (config->vlan == ENABLED){
	

		switch (config->switch_model){

			case RTL8367R_VB:
			default:
				printk ("%s %d RTL8367R_VB\n", __FUNCTION__, __LINE__);

				if (rtk_vlan_init() != RT_ERR_OK)
					return RT_ERR_FAILED;

				// W/LLLL
				if (config->model == WLLLL){
					mbrmsk.bits[0] = 0x003E;
					untagmsk.bits[0] = 0x001E;
					rtk_vlan_set (1, mbrmsk, untagmsk, 1);
		
					mbrmsk.bits[0] = 0x0021;
					untagmsk.bits[0] = 0x0001;
					rtk_vlan_set (2, mbrmsk, untagmsk, 2);

					if (config->led_mode == LED_LAN_PORT_OFF){
						rtk_vlan_portPvid_set (0, 2, 0);
						rtk_vlan_portPvid_set (1, 3, 0);
						rtk_vlan_portPvid_set (2, 3, 0);
						rtk_vlan_portPvid_set (3, 3, 0);
						rtk_vlan_portPvid_set (4, 3, 0);
//						rtk_vlan_portPvid_set (9, 3, 0);
					}else{
						rtk_vlan_portPvid_set (0, 2, 0);
						rtk_vlan_portPvid_set (1, 1, 0);
						rtk_vlan_portPvid_set (2, 1, 0);
						rtk_vlan_portPvid_set (3, 1, 0);
						rtk_vlan_portPvid_set (4, 1, 0);
//						rtk_vlan_portPvid_set (9, 1, 0);
					}

				// LLLL/W
				}else{
					mbrmsk.bits[0] = 0x002F;
					untagmsk.bits[0] = 0x000F;
					rtk_vlan_set (1, mbrmsk, untagmsk, 1);
	
					mbrmsk.bits[0] = 0x0030;
					untagmsk.bits[0] = 0x0010;
					rtk_vlan_set (2, mbrmsk, untagmsk, 2);

					if (config->led_mode == LED_LAN_PORT_OFF){
						rtk_vlan_portPvid_set (0, 3, 0);
						rtk_vlan_portPvid_set (1, 3, 0);
						rtk_vlan_portPvid_set (2, 3, 0);
						rtk_vlan_portPvid_set (3, 3, 0);
						rtk_vlan_portPvid_set (4, 2, 0);
//						rtk_vlan_portPvid_set (9, 3, 0);
				
					}else{
						rtk_vlan_portPvid_set (0, 1, 0);
						rtk_vlan_portPvid_set (1, 1, 0);
						rtk_vlan_portPvid_set (2, 1, 0);
						rtk_vlan_portPvid_set (3, 1, 0);
						rtk_vlan_portPvid_set (4, 2, 0);
//						rtk_vlan_portPvid_set (9, 1, 0);
					}
				}
				break;
		}
	}

	return RT_ERR_OK;
}


static int switch_status_read_proc(char *buf, char **start, off_t fpos, int length, int *eof, void *data)
{
	int len;
	char buffer[64];

	sprintf (buf, "model:%d led_mode:%d vlan:%d\n", model, led_mode, vlan);
	len = strlen (buf) - fpos;

	if (len <= 0){
		*start = buf;
		*eof = 1;
		return 0;
	}

	*start = buf + fpos;
	
	if (len <= length)
		*eof = 1;

	return len < length ? len : length;
}

static int switch_read_proc(char *buf, char **start, off_t fpos, int length, int *eof, void *data)
{
	int len, dev, port;
	char buffer[64];

	rtk_port_linkStatus_t linkStatus;
	rtk_port_speed_t speed;
	rtk_port_duplex_t duplex;
	
	memset (buffer, 0x00, sizeof(buffer));
	for (port = 0; port < 5; port++){
		rtk_port_phyStatus_get (port, &linkStatus, &speed, &duplex);
//		sprintf (buffer, "%s%d:%d;", buffer, port, linkStatus);
		sprintf (buffer, "%s%d;", buffer, linkStatus);
	}
//EDX RexHua start
	rtk_uint32 Addr=0;
	rtk_l2_ucastAddr_t pL2_data;
	printk("\tport\tmac\t\t\tfid\tivl\tcvid\tefid\tstatic\n");
	while(rtk_l2_addr_next_get(3, port, &Addr, &pL2_data) == RT_ERR_OK)
	{
		Addr+=1;
		printk("\t%d\t%02x:%02x:%02x:%02x:%02x:%02x\t%x\t%x\t%x\t%x\t%x\n", pL2_data.port, pL2_data.mac.octet[0], pL2_data.mac.octet[1],pL2_data.mac.octet[2],pL2_data.mac.octet[3],pL2_data.mac.octet[4],pL2_data.mac.octet[5], pL2_data.fid, pL2_data.ivl, pL2_data.cvid, pL2_data.efid, pL2_data.is_static);
	}	
//EDX RexHua end

	sprintf (buf, "%s\n", buffer);
	len = strlen(buf) - fpos;

	if (len <= 0){
		*start = buf;
		*eof = 1;
		return 0;
	}

	*start = buf + fpos;

	if ( len <= length)
		*eof = 1;

	return len < length ? len : length;
}
//EDX Timmy start
static int write_link_break_proc(struct file *file, char *buf, int length, void *data) {
static char link_break_flag = '2';

	if (length < 2)
		return -EFAULT;

	//0: internet led off
	//1: internet led on
	if (buf && !copy_from_user(&link_break_flag, buf, 1)) {
		if (link_break_flag == '0') {
		unsigned int regdata;
		rtk_port_phyReg_get(0, 0, &regdata);
		regdata = (regdata | 0x0800);
		rtk_port_phyReg_set(0, 0, regdata);
		rtk_port_phyReg_get(1, 0, &regdata);
		regdata = (regdata | 0x0800);
		rtk_port_phyReg_set(1, 0, regdata);
		rtk_port_phyReg_get(2, 0, &regdata);
		regdata = (regdata | 0x0800);
		rtk_port_phyReg_set(2, 0, regdata);
		rtk_port_phyReg_get(3, 0, &regdata);
		regdata = (regdata | 0x0800);
		rtk_port_phyReg_set(3, 0, regdata);
		rtk_port_phyReg_get(4, 0, &regdata);
		regdata = (regdata | 0x0800);
		rtk_port_phyReg_set(4, 0, regdata);
		return 2;

		} else if (link_break_flag == '1') {
		unsigned int regdata;
		rtk_port_phyReg_get(0, 0, &regdata);
		regdata = (regdata | 0x9000);
		rtk_port_phyReg_set(0, 0, regdata);
		rtk_port_phyReg_get(1, 0, &regdata);
		regdata = (regdata | 0x9000);
		rtk_port_phyReg_set(1, 0, regdata);
		rtk_port_phyReg_get(2, 0, &regdata);
		regdata = (regdata | 0x9000);
		rtk_port_phyReg_set(2, 0, regdata);
		rtk_port_phyReg_get(3, 0, &regdata);
		regdata = (regdata | 0x9000);
		rtk_port_phyReg_set(3, 0, regdata);
		rtk_port_phyReg_get(4, 0, &regdata);
		regdata = (regdata | 0x9000);
		rtk_port_phyReg_set(4, 0, regdata);
		return 2;
		}
		/*if(!led_switch_on){
			link_break_control(link_break_counter);
		}
		return length;*/
	} else
		return -EFAULT;
}
//EDX Timmy end

//EDX Timmy start for wan detect
static int etherport_detect(char *buf, char **start, off_t fpos, int length, int *eof, void *data)
{
	int len;

	rtk_api_ret_t retVal;
	rtk_uint32 phyData;
	rtk_port_linkStatus_t linkstatus;
	rtk_data_t speed;
	rtk_data_t duplex;

rtk_port_phyStatus_get(0, &linkstatus, &speed, &duplex);

	if (linkstatus == 1)
	{
	sprintf (buf, "WAN Port Plug-in!\n");
	}
	else
	{
	sprintf (buf, "WAN Port Plug-out!\n");
	}

	if (rtk_port_phyStatus_get(0, &linkstatus, &speed, &duplex) == RT_ERR_FAILED)
	{
	printk("Function fail!\n");
	sprintf (buf, "Function fail!\n");
	return buf;
	}

	len = strlen(buf) - fpos;

	if (len <= 0){
		*start = buf;
		*eof = 1;
		return 0;
	}

	*start = buf + fpos;

	if ( len <= length)
		*eof = 1;

	return len < length ? len : length;

}
//EDX Timmy end

static void switch_exit (void)
{
	remove_proc_entry("switch", NULL);
	remove_proc_entry("switch_status", NULL);
	remove_proc_entry("link_break", NULL);
	remove_proc_entry("wan_detect", NULL);
	printk (KERN_INFO "Realtek switch driver cleanup\n");
}

static int __init switch_init (void)
{
	struct config_tbl config;
	struct proc_dir_entry *res=NULL; //EDX

	if (!create_proc_read_entry ("switch", 0, 0, switch_read_proc, NULL) ){
		printk(KERN_ERR "rgmii: can't create /proc/switch\n");
		switch_exit();
		return -ENOMEM;
	}

	if (!create_proc_read_entry ("switch_status", 0, 0, switch_status_read_proc, NULL) ){
		printk(KERN_ERR "rgmii: can't create /proc/switch\n");
		switch_exit();
		return -ENOMEM;
	}
//EDX Timmy start
	res = create_proc_entry("link_break", 0, NULL);
	if (res) {
		res->read_proc = NULL;
		res->write_proc = write_link_break_proc;
	         }

	if (!create_proc_read_entry ("wan_detect", 0, 0, etherport_detect, NULL) ){
		printk(KERN_ERR "rgmii: can't create /proc/switch\n");
		switch_exit();
		return -ENOMEM;
	}
//EDX Timmy end

	memset (&config, 0x00, sizeof(struct config_tbl));
	
	config.vlan = vlan;
	config.model = model;
	config.led_mode = led_mode;
	config.led_all = led_all;

	smi_init();
	smi_reset ();
#if 0
	if (rtk_switch_init() == RT_ERR_OK)
		switch_model = RTL8367R;
	else
		switch_model = RTL8367M;
#else
	config.switch_model = rtk_switch_init();
	
#endif
	if (config.switch_model != RTL8367R_VB){
		smi_reset ();
		config.switch_model = rtk_switch_init();
	}

	printk ("%s %d mode = %d model = %d vlan = %d switch = %04x\n", __FUNCTION__, __LINE__, model, led_mode, vlan, config.switch_model);

	init_rgmii (&config);		
	port_led (&config);
	
 	if ( (config.led_mode == LED_ALL_OFF) || (config.led_mode == LED_LAN_SPPED_100 )) //EDX shakim
		port_phy_ability (config.switch_model, config.led_mode);
#if 0
		rtk_portmask_t		mbrmsk;
		rtk_portmask_t		untagmsk;
		rtk_fid_t 		Fid;
		rtk_vlan_get (1, &mbrmsk, &untagmsk, &Fid);
		printk(" vlan 1 mbrmask=%x,  untagmsk=%x, Fid = %d\n", mbrmsk.bits[0], untagmsk.bits[0], Fid);
		rtk_vlan_get (2, &mbrmsk, &untagmsk, &Fid);
		printk(" vlan 1 mbrmask=%x,  untagmsk=%x, Fid = %d\n", mbrmsk.bits[0], untagmsk.bits[0], Fid);
#endif

	printk (KERN_INFO "(%s %d) init_rgmii done \n", __FUNCTION__, __LINE__);
	return 0;
}

module_init(switch_init);
module_exit(switch_exit);
