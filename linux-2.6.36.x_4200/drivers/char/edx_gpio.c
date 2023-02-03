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
#include <asm/rt2880/surfboardint.h>
#include <linux/interrupt.h>
#include "led.h"
#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtl8367b_asicdrv_port.h>
#include <smi.h>
#include <rtl8367b_asicdrv_phy.h>
#include "edx_events.h"
#include <edxtools.h>

#define	ARYSIZE(a)				((sizeof(a)) / (sizeof(a[0])))

#define	RAW_NOTIFY

#ifdef RAW_NOTIFY
RAW_NOTIFIER_HEAD(edx_gpio_chain);
RAW_NOTIFIER_HEAD(edx_wifi_chain);
#endif

irqreturn_t ralink_gpio_irq_handler(int , void *);

int ralink_gpio_irqnum = 0;
u32 ralink_gpio_intp = 0;
u32 ralink_gpio_edge = 0;

u32 ralink_gpio3924_intp = 0;
u32 ralink_gpio3924_edge = 0;
u32 ralink_gpio7140_intp = 0;
u32 ralink_gpio7140_edge = 0;
u32 ralink_gpio72_intp = 0;
u32 ralink_gpio72_edge = 0;

// FIXME	//EDX Randall start
rtk_api_ret_t (*edx_rtk_api)(rtk_port_t port, rtk_port_phy_ability_t *pAbility);
rtk_api_ret_t (*link_get)(rtk_port_t port, rtk_port_phy_reg_t reg, rtk_port_phy_data_t *pData);
rtk_api_ret_t (*link_set)(rtk_port_t port, rtk_port_phy_reg_t reg, rtk_port_phy_data_t regData);
rtk_api_ret_t (*led_set)(rtk_led_group_t group, rtk_portmask_t portmask);
rtk_api_ret_t (*led_mode_set)(rtk_led_mode_t mode);
//end

extern unsigned long volatile jiffies;
extern void machine_restart(char *);
static int edx_open(struct inode *inode, struct file *file);
static long edx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t edx_read(struct file *file, char *buf, size_t count, loff_t *fpos);
static ssize_t edx_write(struct file *file, const char __user *buf, size_t count, loff_t *fpos);

static struct file_operations edx_fops = {
    .read  = edx_read,
    .open  = edx_open,
    .unlocked_ioctl = edx_ioctl,
    .write = edx_write,
};

static struct led_map_t {
	int		id, gpio;
	int		blink, turn;
	int		period[2];
	struct timer_list	tmr;
	int		tmr_flag;
} led_map[] = {
	{ LED_WPS, 10},
	{ LED_2G, 10},
	{ LED_POWER, 7},
	{ LED_FW, 8},
	{ LED_5G, 14},
	{ LED_XX, 10}
};

//-----------------------------------------------------------
// All functions included in this area are used for edx_events
// Start of edx_events helper functions
//-----------------------------------------------------------
#if 0
static struct edx_proc_t	edx_proc;

void edx_register_proc(struct edx_proc_t *p)
{
	edx_proc = *p;
}
#endif

static int edx_open(struct inode *inode, struct file *file)
{
    int id = MINOR(inode->i_rdev);
    if (id >= EDX_DEV_NUM)
        return -ENODEV;
    file->private_data = (void*)id;
	return 0;
}

static void DO_LED(struct led_map_t *pled, int da)
{
	if (da)
		*(volatile u32 *)(RALINK_REG_PIODATA) |=  (1 << pled->gpio);
	else
		*(volatile u32 *)(RALINK_REG_PIODATA) &= ~(1 << pled->gpio);
}

static void led_timer_callback(unsigned long data)
{
	struct led_map_t *pled = (struct led_map_t *)data;

	DO_LED(pled, (pled->turn ^= 1));
	mod_timer(&pled->tmr,
			jiffies + msecs_to_jiffies(pled->period[pled->turn]));
}

static void start_timer(struct led_map_t *pled)
{
	if (pled->tmr_flag == 0) {
		pled->turn = 0;
		setup_timer(&pled->tmr, led_timer_callback, (unsigned long)pled);
		pled->tmr_flag = 1;
	}
	DO_LED(pled, pled->turn);
	mod_timer(&pled->tmr, jiffies + msecs_to_jiffies(pled->period[pled->turn]));
}

static void stop_timer(struct led_map_t *pled)
{
	if (pled->tmr_flag) {
		del_timer(&pled->tmr);
		pled->tmr_flag = 0;
	}
}

static int do_led_ctrl(struct edx_gpio_arg_t *pargs, unsigned long arg)
{
	int		i;
	struct led_map_t *pled = NULL;

	arg = arg;		// make compiler happy
	for (i = 0; i < ARRAYSIZE(led_map); i++) {
		if (pargs->args.led.led_id == led_map[i].id)
			pled = &led_map[i];
	}

	if (pled == NULL)
		return -EINVAL;

	switch (pargs->args.led.action) {
		case EDX_LED_ON:
			stop_timer(pled);
			DO_LED(pled, 0);
			break;
		case EDX_LED_OFF:
			stop_timer(pled);
			DO_LED(pled, 1);
			break;
		case EDX_LED_BLINK:
			if ((pargs->args.led.ton != 0) && (pargs->args.led.toff != 0)) {
				pled->period[0] = pargs->args.led.ton;
				pled->period[1] = pargs->args.led.toff;
				start_timer(pled);
			} else
				stop_timer(pled);
			break;
		default:
			break;
	}

	return 0;
}

static int do_switch_ctrl(struct edx_gpio_arg_t *pargs, unsigned long arg)
{
	//int id = (int)file->private_data;	//EDX Randall start
	rtk_port_phy_ability_t ability;
	unsigned int regdata, linkReg;
	rtk_portmask_t          ledmask;
	
	memset (&ability, 0x00, sizeof(ability));
	pargs->args.sw.action &= RALINK_GPIO_DATA_MASK;
	switch (pargs->args.sw.action) {
	case SPEED:
		switch(pargs->args.sw.speed){
			case 0:
				ability.Half_10 = 1;
				break;
			case 1:
				ability.Full_10 = 1;
				break;
			case 2:	
				ability.Half_100 = 1;
				if(led_mode_set)
					led_mode_set(LED_MODE_1);
				break;
			case 3:	
				ability.Full_100 = 1;
				if(led_mode_set)
					led_mode_set(LED_MODE_1);
				break;
			case 4:	
				ability.Full_1000 = 1;
				if(led_mode_set)
					led_mode_set(LED_MODE_0);
				break;
		}
		if (edx_rtk_api) {
			edx_rtk_api (0, &ability);
			edx_rtk_api (1, &ability);
			edx_rtk_api (2, &ability);
			edx_rtk_api (3, &ability);
			edx_rtk_api (4, &ability);
		}
		break;
	case LED:
		switch(pargs->args.sw.led){
		case 0:
			ledmask.bits[0] = 0x01;
			break;
		case 1:
			ledmask.bits[0] = 0x1F;
			break;
		}
		if (led_set){
			led_set(LED_GROUP_1, ledmask);
		}
		break;
	case LINK:
		switch(pargs->args.sw.link){
		case 0:
			linkReg = 0x0800;
			break;
		case 1:
			linkReg = 0x9000;
			break;
		}
		if(link_get && link_set){
			link_get(0, 0, &regdata);
			regdata = (regdata | linkReg);
			link_set(0, 0, regdata);
			link_get(1, 0, &regdata);
			regdata = (regdata | linkReg);
			link_set(1, 0, regdata);
			link_get(2, 0, &regdata);
			regdata = (regdata | linkReg);
			link_set(2, 0, regdata);
			link_get(3, 0, &regdata);
			regdata = (regdata | linkReg);
			link_set(3, 0, regdata);
			link_get(4, 0, &regdata);
			regdata = (regdata | linkReg);
			link_set(4, 0, regdata);
		}
		break;
	}	//end
	return 0;
}

static int do_gpio_ctrl(struct edx_gpio_arg_t *pargs, unsigned long arg)
{
	arg = arg;		// make compiler happy

	if (pargs->args.gpio.dir == GPIO_DIR_IN) {
		pargs->args.gpio.val = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
		pargs->args.gpio.val = (pargs->args.gpio.val >> pargs->args.gpio.num) & 0x01;
	} else {
	   	if (pargs->args.gpio.val)
			*(volatile u32 *)(RALINK_REG_PIODATA) |= (1 << pargs->args.gpio.num);
		else
			*(volatile u32 *)(RALINK_REG_PIODATA) &= ~(1 << pargs->args.gpio.num);
	}
	return 0;
}

static long edx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int id = (int)file->private_data;
	struct edx_gpio_arg_t	uargs;
	
	id = id;	// make compiler happy
	
	if (copy_from_user(&uargs, (char __user *)arg, sizeof(uargs)))
		return -EFAULT;

	switch (cmd) {
		case SWITCH_CTRL:
			do_switch_ctrl(&uargs, arg);
			break;
		case LED_CTRL:
			do_led_ctrl(&uargs, arg);
			break;
		case GPIO_CTRL:
			do_gpio_ctrl(&uargs, arg);
			if (uargs.args.gpio.dir == GPIO_DIR_IN) {
				if (copy_to_user((char __user *)arg, &uargs, sizeof(uargs)))
					return -EFAULT;
			}
			break;
	}
	return 0;
}

static ssize_t edx_read(struct file *file, char *buf, size_t count, loff_t *fpos)
{
	return 0;
}

static ssize_t edx_write(struct file *file, const char __user *buf, size_t count, loff_t *fpos)
{
	return 0;
}

static int edx_notify_user(int edge, unsigned long last, unsigned long now, unsigned long tfrom, unsigned long tto, int irq)
{
	struct gpio_args_t	args;

#ifdef RAW_NOTIFY
	args.num = irq;
	args.last = last;
	args.now = now;
	args.tfrom = tfrom;
	args.tto = tto;
	args.edge = edge;
	return raw_notifier_call_chain(&edx_gpio_chain, edge, &args);
#else
	if (edx_proc.edx_gpio) {
		args.num = irq;
		args.last = last;
		args.now = now;
		args.tfrom = tfrom;
		args.tto = tto;
		args.edge = edge;
		edx_proc.edx_gpio(&args);
	}
#endif
	return 0;
}
//------------------------------------------------------------
// End of edx_events helper functions
//------------------------------------------------------------

#if 0
static void reboot_wrapper(unsigned long period)
{
	machine_restart(0);
}
#endif

void ralink_gpio_save_clear_intp(void)
{
	ralink_gpio_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOINT));
	ralink_gpio_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOEDGE));
	*(volatile u32 *)(RALINK_REG_PIOINT) = cpu_to_le32(0x00FFFFFF);
	*(volatile u32 *)(RALINK_REG_PIOEDGE) = cpu_to_le32(0x00FFFFFF);
#if defined (RALINK_GPIO_HAS_2722)
	ralink_gpio2722_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO2722INT));
	ralink_gpio2722_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO2722EDGE));
	*(volatile u32 *)(RALINK_REG_PIO2722INT) = cpu_to_le32(0x0000FFFF);
	*(volatile u32 *)(RALINK_REG_PIO2722EDGE) = cpu_to_le32(0x0000FFFF);
#elif defined (RALINK_GPIO_HAS_4524)
	ralink_gpio3924_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924INT));
	ralink_gpio3924_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924EDGE));
	*(volatile u32 *)(RALINK_REG_PIO3924INT) = cpu_to_le32(0x0000FFFF);
	*(volatile u32 *)(RALINK_REG_PIO3924EDGE) = cpu_to_le32(0x0000FFFF);
	ralink_gpio4540_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO4540INT));
	ralink_gpio4540_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO4540EDGE));
	*(volatile u32 *)(RALINK_REG_PIO4540INT) = cpu_to_le32(0x00000FFF);
	*(volatile u32 *)(RALINK_REG_PIO4540EDGE) = cpu_to_le32(0x00000FFF);
#elif defined (RALINK_GPIO_HAS_5124)
	ralink_gpio3924_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924INT));
	ralink_gpio3924_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924EDGE));
	*(volatile u32 *)(RALINK_REG_PIO3924INT) = cpu_to_le32(0x0000FFFF);
	*(volatile u32 *)(RALINK_REG_PIO3924EDGE) = cpu_to_le32(0x0000FFFF);
	ralink_gpio5140_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140INT));
	ralink_gpio5140_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140EDGE));
	*(volatile u32 *)(RALINK_REG_PIO5140INT) = cpu_to_le32(0x00000FFF);
	*(volatile u32 *)(RALINK_REG_PIO5140EDGE) = cpu_to_le32(0x00000FFF);
#elif defined (RALINK_GPIO_HAS_9524) || defined (RALINK_GPIO_HAS_7224)
	ralink_gpio3924_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924INT));
	ralink_gpio3924_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924EDGE));
	*(volatile u32 *)(RALINK_REG_PIO3924INT) = cpu_to_le32(0x0000FFFF);
	*(volatile u32 *)(RALINK_REG_PIO3924EDGE) = cpu_to_le32(0x0000FFFF);
	ralink_gpio7140_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140INT));
	ralink_gpio7140_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140EDGE));
	*(volatile u32 *)(RALINK_REG_PIO7140INT) = cpu_to_le32(0xFFFFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO7140EDGE) = cpu_to_le32(0xFFFFFFFF);
#if defined (RALINK_GPIO_HAS_7224)
	ralink_gpio72_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72INT));
	ralink_gpio72_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72EDGE));
	*(volatile u32 *)(RALINK_REG_PIO72INT) = cpu_to_le32(0x00FFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO72EDGE) = cpu_to_le32(0x00FFFFFF);
#else
	ralink_gpio9572_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9572INT));
	ralink_gpio9572_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9572EDGE));
	*(volatile u32 *)(RALINK_REG_PIO9572INT) = cpu_to_le32(0x00FFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO9572EDGE) = cpu_to_le32(0x00FFFFFF);
#endif
#elif defined (RALINK_GPIO_HAS_9532)
	ralink_gpio6332_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO6332INT));
	ralink_gpio6332_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO6332EDGE));
	*(volatile u32 *)(RALINK_REG_PIO6332INT) = cpu_to_le32(0xFFFFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO6332EDGE) = cpu_to_le32(0xFFFFFFFF);
	ralink_gpio6332_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO6332INT));
	ralink_gpio6332_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO6332EDGE));

	ralink_gpio9564_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9564INT));
	ralink_gpio9564_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9564EDGE));
	*(volatile u32 *)(RALINK_REG_PIO9564INT) = cpu_to_le32(0xFFFFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO9564EDGE) = cpu_to_le32(0xFFFFFFFF);
	ralink_gpio9564_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9564INT));
	ralink_gpio9564_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO9564EDGE));
#endif
}

//
// Don't blame me !
// The original code fragment is coded by Ralink and it really SUCKS !!
// I re-organize some code by macro definition
// I think it can be more clear if I have enough time. -- SJC
//
#define	NOSUCK(a, b, c, d)	\
	for (i = (a); i < (b); i++) { \
		if (! ((c) & (1 << (i - (a))))) \
			continue; \
		ralink_gpio_irqnum = i; \
		if ((d) & (1 << (i - (a)))) { \
			if (record[i].rising != 0 && time_before_eq(now, record[i].rising + 40L)) { \
			} else { \
				record[i].rising = now; \
				record[i].tto = get_seconds(); \
				edx_notify_user(2, now, record[i].falling, record[i].tfrom, record[i].tto, i); \
			} \
		} else { \
			if (!time_before_eq(now, record[i].rising + 40L)) { \
				record[i].falling = now; \
				record[i].tto = record[i].tfrom = get_seconds(); \
				edx_notify_user(1, now, record[i].falling, record[i].tfrom, record[i].tto, i); \
			} \
		} \
		break; \
	}
	
static struct gpio_time_record {
	unsigned long falling;
	unsigned long rising;
	unsigned long tfrom;
	unsigned long tto;
} record[RALINK_GPIO_NUMBER];

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
void ralink_gpio_irq_handler(unsigned int irq, struct irqaction *irqaction)
#else
irqreturn_t ralink_gpio_irq_handler(int irq, void *irqaction)
#endif
{
	unsigned long now;
	int i;

	ralink_gpio_save_clear_intp();
	now = jiffies;
#if defined (RALINK_GPIO_HAS_2722)
#if 0	// Here I leave these line of code is just for refering the "comments"
	for (i = 0; i < 22; i++) {
		if (! (ralink_gpio_intp & (1 << i)))
			continue;
		ralink_gpio_irqnum = i;
		if (ralink_gpio_edge & (1 << i)) { //rising edge
			if (record[i].rising != 0 && time_before_eq(now,
						record[i].rising + 40L)) {
				/*
				 * If the interrupt comes in a short period,
				 * it might be floating. We ignore it.
				 */
			}
			else {
				record[i].rising = now;
				if (time_before(now, record[i].falling + 200L)) {
					//one click
					ralink_gpio_notify_user(1);
				}
				else {
					//press for several seconds
					ralink_gpio_notify_user(2);
				}
			}
		}
		else { //falling edge
			record[i].falling = now;
		}
		break;
	}
#endif
	NOSUCK(0, 22, ralink_gpio_intp, ralink_gpio_edge);
	NOSUCK(22, 28, ralink_gpio2722_intp, ralink_gpio2722_edge);
#elif defined (RALINK_GPIO_HAS_9532)
	NOSUCK(0, 32, ralink_gpio_intp, ralink_gpio_edge);
	NOSUCK(32, 64, ralink_gpio6332_intp, ralink_gpio6332_edge);
	NOSUCK(64, RALINK_GPIO_NUMBER, ralink_gpio9564_intp, ralink_gpio9564_edge);
#else
	NOSUCK(0, 24, ralink_gpio_intp, ralink_gpio_edge);
#if defined (RALINK_GPIO_HAS_4524)
	NOSUCK(24, 40, ralink_gpio3924_intp, ralink_gpio3924_edge);
	NOSUCK(40, RALINK_GPIO_NUMBER, ralink_gpio4540_intp, ralink_gpio4540_edge);
#elif defined (RALINK_GPIO_HAS_5124)
	NOSUCK(24, 40, ralink_gpio3924_intp, ralink_gpio3924_edge);
	NOSUCK(40, RALINK_GPIO_NUMBER, ralink_gpio5140_intp, ralink_gpio5140_edge);
#elif defined (RALINK_GPIO_HAS_9524) || defined (RALINK_GPIO_HAS_7224)
	NOSUCK(24, 40, ralink_gpio3924_intp, ralink_gpio3924_edge);
	NOSUCK(40, 72, ralink_gpio7140_intp, ralink_gpio7140_edge);
#if defined (RALINK_GPIO_HAS_7224)
	NOSUCK(72, RALINK_GPIO_NUMBER, ralink_gpio72_intp, ralink_gpio72_edge);
#else
	NOSUCK(72, RALINK_GPIO_NUMBER, ralink_gpio9572_intp, ralink_gpio9572_edge);
#endif
#endif
#endif

	return IRQ_HANDLED;
}

static int set_output_pin(int pin)
{
	unsigned long	tmp;

	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
	tmp |= cpu_to_le32(0x1 << pin);
	*(volatile u32 *)(RALINK_REG_PIODIR) = tmp;
	return 0;
}

static int set_input_pin(int pin)
{
	unsigned long	tmp;

	// direction = input
	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
	tmp &= ~cpu_to_le32(0x1 << pin);
	*(volatile u32 *)(RALINK_REG_PIODIR) = tmp;

	// enable falling and raising edge
	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIORENA));
	tmp |= (0x1 << pin);
	*(volatile u32 *)(RALINK_REG_PIORENA) = cpu_to_le32(tmp);
	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOFENA));
	tmp |= (0x1 << pin);
	*(volatile u32 *)(RALINK_REG_PIOFENA) = cpu_to_le32(tmp);
	return 0;
}

static int __init edx_gpio_init(void)
{
	int		i, ret = 0;
	u32		gpiomode;

	printk("Initializing EDX GPIO...");	

	ret = register_chrdev(EDX_MAJOR, EDX_DEVNAME, &edx_fops);
	if (ret < 0)   {
		printk(KERN_ERR "fail to register char device\n");
		return ret;
	}

	memset(record, 0, sizeof(record));
	for (i = 0; i < ARYSIZE(record); i++)
		record[i].rising = jiffies;

	gpiomode = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
#if !defined (CONFIG_RALINK_RT2880)
	gpiomode &= ~0x1C;  //clear bit[2:4]UARTF_SHARE_MODE
#endif
#if defined (CONFIG_RALINK_MT7620)
	gpiomode &= ~0x2000;  //clear bit[13] WLAN_LED
#endif
	gpiomode |= RALINK_GPIOMODE_DFT;
	*(volatile u32 *)(RALINK_REG_GPIOMODE) = cpu_to_le32(gpiomode);

	*(volatile u32 *)(RALINK_REG_INTENA) = cpu_to_le32(RALINK_INTCTL_PIO);

	set_input_pin(1);		// RST
	set_input_pin(2);		// WPS
	set_input_pin(9);		// MANUAL_RSTN
	set_input_pin(12);		// Router mode
	set_input_pin(13);		// Auto mode

	set_output_pin(7);			// Power LED
	set_output_pin(8);			// Status LED
	set_output_pin(10);		// 2.4G WPS
	set_output_pin(14);		// 5G WPS

	edx_rtk_api = NULL;	//EDX Randall start
	link_get = NULL;
	link_set = NULL;
	led_set = NULL;
	led_mode_set = NULL;	//end
	
	printk("Done\n");

	return request_irq(SURFBOARDINT_GPIO, ralink_gpio_irq_handler, IRQF_DISABLED, EDX_DEVNAME, NULL);
}

static void edx_gpio_exit(void)
{
	// Normal mode
	*(volatile u32 *)(RALINK_REG_GPIOMODE) &= ~RALINK_GPIOMODE_DFT;
	// Disable all interrupt
	*(volatile u32 *)(RALINK_REG_INTDIS) = cpu_to_le32(RALINK_INTCTL_PIO);
   	unregister_chrdev(EDX_MAJOR, EDX_DEVNAME);
}

#ifdef RAW_NOTIFY
int register_edx_gpio_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&edx_gpio_chain, nb);
}

int unregister_edx_gpio_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&edx_gpio_chain, nb);
}
//EDX wade start
int register_edx_wifi_notifier(struct notifier_block *nb)
{
        return raw_notifier_chain_register(&edx_wifi_chain, nb);
}
int unregister_edx_wifi_notifier(struct notifier_block *nb)
{
        return raw_notifier_chain_unregister(&edx_wifi_chain, nb);
}
int edx_wifi_notifier_call_chain(int event, void *arg)
{
	return raw_notifier_call_chain(&edx_wifi_chain, event, arg);
}
//EDX wade end

EXPORT_SYMBOL(register_edx_gpio_notifier);
EXPORT_SYMBOL(unregister_edx_gpio_notifier);

//EDX wade start
EXPORT_SYMBOL(register_edx_wifi_notifier);
EXPORT_SYMBOL(unregister_edx_wifi_notifier);
EXPORT_SYMBOL(edx_wifi_notifier_call_chain);
//EDX wade end
#else
EXPORT_SYMBOL(edx_register_proc);
#endif

EXPORT_SYMBOL(led_set);
EXPORT_SYMBOL(led_mode_set);
EXPORT_SYMBOL(link_get);
EXPORT_SYMBOL(edx_rtk_api);
EXPORT_SYMBOL(link_set);

module_init(edx_gpio_init);
module_exit(edx_gpio_exit);
