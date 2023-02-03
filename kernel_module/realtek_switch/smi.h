
#ifndef __SMI_H__
#define __SMI_H__

#include <rtk_types.h>
#include <rtk_error.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <asm/system.h>
#include <linux/reboot.h>
#include <asm/rt2880/surfboardint.h>
#include <linux/interrupt.h>
#include <asm/rt2880/rt_mmap.h>

enum {
	WLLLL = 0,
	LLLLW
};

enum {
	RTL8367M = 0,
	RTL8367R = 0x2104,		//RTL8367R chip id
	RTL8367R_1 = 0x94eb,		//RTL8367R chip id
	RTL8367R_VB = 0x1010
//	RTL8367R_VB = 0xfec4,		//init RTL8367R first the chip id will be this
};

enum {
	LED_NORMAL = 0,
	LED_LAN_SPPED_100, //EDX shakim
	LED_LAN_PORT_OFF,
	LED_ALL_OFF
};

struct config_tbl{
	int vlan;
	int model;
	int led_mode;
	int switch_model;
	int led_all;
};

extern int32 smi_reset();
extern int32 smi_init();
extern int32 smi_read(uint32 mAddrs, uint32 *rData);
extern int32 smi_write(uint32 mAddrs, uint32 rData);


//EDX shakim start
#define RT2882_REG(x)				(*((volatile u32 *)(x)))    
#define RT2880_REG_PIO_0_15_DIR			(RALINK_PIO_BASE)
#define RT2880_REG_PIO_16_31_DIR			(RALINK_PIO_BASE + 0x20)
#define RT2880_REG_PIO_0_31_DATA			(RALINK_PIO_BASE + 0x04)
#define RT2880_REG_PIO_0_31_OUTPUTEN		(RALINK_PIO_BASE + 0x014)
#define PIO_DIR_BIT(x) (x > 15 ? (((x-16)*2)):((x*2)))
//EDX shakim  end

#endif /* __SMI_H__ */
