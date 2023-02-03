/*
 * LED interface for WP3200
 *
 * Copyright (C) 2002, by Allen Hung
 *
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>                       
#include <linux/reboot.h>
#include <asm/rt2880/surfboardint.h>
#include <linux/interrupt.h>

#include "led.h"

#define BUF_LEN		30
#define NAME			"ralink_gpio"
#define RALINK_GPIO_MAX_INFO (RALINK_GPIO_NUMBER)

irqreturn_t ralink_gpio_irq_handler(int , void *);
void ralink_gpio_notify_user(int );

ralink_gpio_reg_info ralink_gpio_info[RALINK_GPIO_MAX_INFO];
ralink_gpio_reg_info info;
struct LED_DATA  {
    char sts_buf[BUF_LEN+1];
    unsigned long sts;
};
//add by kyle 2007/12/12 for gpio interrupt
int ralink_gpio_irqnum = 0;
pid_t ra_pid=0;
u32 ralink_gpio_intp = 0;
u32 ralink_gpio_edge = 0;
#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
int mii_pid = 0;
int send_mii_sig=0;
static char eth_detect_flag = '0';
#endif
#if defined(_BR6278AC_) //EDX Robert Add,2016_08_02
int gpo_39_data=0;
#endif
struct LED_DATA led_data[LED_DEV_NUM];

u32 ralink_gpio3924_intp = 0;
u32 ralink_gpio3924_edge = 0;
u32 ralink_gpio7140_intp = 0;
u32 ralink_gpio7140_edge = 0;
u32 ralink_gpio72_intp = 0;
u32 ralink_gpio72_edge = 0;

extern unsigned long volatile jiffies;
static struct timer_list blink_timer[LED_DEV_NUM];
// sam 01-30-2004 for watchdog
static struct timer_list watchdog;
// Kyle 20080806 for upgrade reboot.
static struct timer_list rebootTimer;
// end sam
#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
static struct timer_list mii_timer;
#endif
static char cmd_buf[BUF_LEN+1];
extern void machine_restart(char *);

// sam for ps 3205U -- using CSx1 (0xb0e00000)
// bit map as following
// BIT   1      2      3      4      5   
//     POWER  WLEN   PORT1  PORT2  PORT3
//
// value 0 --> led on
// value 1 --> led off

#define _ROUTER_

//volatile unsigned char* gpioA_pddr =(unsigned char*)(0xA0300000);
//volatile unsigned char* gpioA_pdcon=(unsigned char*)(0xA0300000);
//volatile unsigned char* gpioB_pddr =(unsigned char*)(0xA0300000);
//volatile unsigned char* gpioB_pdcon=(unsigned char*)(0xA0300000);

//volatile unsigned int * wdt_lr  = (unsigned int*)(0x1e8c0000+0xC1800000);
//volatile unsigned int * wdtg_cvr= (unsigned int*)(0x1e8c0004+0xC1800000);
//volatile unsigned int * wdt_con = (unsigned int*)(0x1e8c0008+0xC1800000);





// sam 1-30-2004 LED status 
// bit map as following
// BIT 4:0  Link status   -->PHY Link ->1 = up, 0 = down
#define LINK_STATUS     (*(unsigned long *)0xb2000014)
#define WATCHDOG_VAL    (*(unsigned long *)0xb20000c0)
#define WATCHDOG_PERIOD 500 // unit ms
#define EXPIRE_TIME     300 // unit 10 ms
#define CLEAR_TIMEER    0xffffa000l  // bit 14:0 -> count up timer, write 0 to clear
#define ENABLE_WATCHDOG 0x80000000l  // bit 31 -> 1 enable , 0 disable watchdog
#define WATCHDOG_SET_TMR_SHIFT 16    // bit 16:30 -> watchdog timer set
// end sam

 
//------------------------------------------------------------
static void turn_led(int id, int on)
{
	switch ( on ) {
	case 0:
		if ((id >= 0) && (id <= 23)) {*(volatile u32 *)(RALINK_REG_PIODATA) |= (1 << (id));}
		else if ((id >= 24) && (id <= 39)) {*(volatile u32 *)(RALINK_REG_PIO3924DATA) |= (1 << (id - 24));}
		else if ((id >= 40) && (id <= 71)) {*(volatile u32 *)(RALINK_REG_PIO7140DATA) |= (1 << (id - 40));}
		else if (id == 72) {*(volatile u32 *)(RALINK_REG_PIO72DATA) |= (1 << (id - 72));}
		else {printk("You have the wrong id(%d)\n", id);}
		break; // LED OFF
	case 1:
		if ((id >= 0) && (id <= 23)) {*(volatile u32 *)(RALINK_REG_PIODATA) &= ~(1 << (id));}
		else if ((id >= 24) && (id <= 39)) {*(volatile u32 *)(RALINK_REG_PIO3924DATA) &= ~(1 << (id - 24));}
		else if ((id >= 40) && (id <= 71)) {*(volatile u32 *)(RALINK_REG_PIO7140DATA) &= ~(1 << (id - 40));}
		else if (id == 72) {*(volatile u32 *)(RALINK_REG_PIO72DATA) &= ~(1 << (id - 72));}
		else {printk("You have the wrong id(%d)\n", id);}
		break; // LED ON
	case 2:
#if defined(_BR6278AC_) //EDX Robert Add,2016_08_02
		if(id==39)
		{
			if(gpo_39_data==0)
				*(volatile u32 *)(RALINK_REG_PIO3924DATA) |= (0x00008000); //GPO#39
			else
				*(volatile u32 *)(RALINK_REG_PIO3924DATA) &= ~(0x00008000); //GPO#39
			gpo_39_data=((gpo_39_data==1) ? 0:1);
		}
		else
		{
#endif
			if ((id >= 0) && (id <= 23)) {*(volatile u32 *)(RALINK_REG_PIODATA) ^= (1 << (id));}
			else if ((id >= 24) && (id <= 39)) {*(volatile u32 *)(RALINK_REG_PIO3924DATA) ^= (1 << (id - 24));}
			else if ((id >= 40) && (id <= 71)) {*(volatile u32 *)(RALINK_REG_PIO7140DATA) ^= (1 << (id - 40));}
			else if (id == 72) {*(volatile u32 *)(RALINK_REG_PIO72DATA) ^= (1 << (id - 72));}
			else {printk("You have the wrong id(%d)\n", id);}
#if defined(_BR6278AC_) //EDX Robert Add,2016_08_02
		}
#endif
		break; // LED inverse
	}
}


static int led_flash[30]={20,10,100,5,5,150,100,5,5,50,20,50,50,20,60,5,20,10,30,10,5,10,50,2,5,5,5,70,10,50};//Erwin
static unsigned int    wlan_counter;    //Erwin
static void blink_wrapper(unsigned long  id)
{
    u_long sts = led_data[id].sts;

    if ( (sts & LED_BLINK_CMD) == LED_BLINK_CMD )  {
	unsigned long period = sts & LED_BLINK_PERIOD;
	if(period == 0xffff)		// BLINK random
	{
		blink_timer[id].expires = jiffies + 3*led_flash[wlan_counter%30]*HZ/1000;
		wlan_counter++;
	}
	else
		blink_timer[id].expires = jiffies + (period * HZ / 1000);
	turn_led(id, 2);
	add_timer(&blink_timer[id]);
    }
    else if ( sts == LED_ON || sts == LED_OFF )
	turn_led(id, sts==LED_ON ? 1 : 0);
}
//------------------------------------------------------------
#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
extern u32 mii_mgr_read(u32 , u32 , u32 *);
/*vance 2009.04.30
 * ethernet port detecter 
*/
static void mii_wrapper(unsigned long  data)
{
	struct task_struct *p = NULL;
	u32 temp;
	int i,tid,reg;
	mii_mgr_read(3,1,&temp);
	reg = (temp>>5)&1;
	if(reg != data)
	{
		if(reg == 1) //connect 
		{
			p = find_task_by_vpid(mii_pid);
			printk(KERN_NOTICE NAME ": sending a SIGURG to process %d\n",mii_pid);
			if(send_mii_sig == 1){
				send_sig(SIGURG, p, 0);
			}
		}
   		
		if(reg == 0)
		{
			p = find_task_by_vpid(mii_pid);
			printk(KERN_NOTICE NAME ": sending a SIGIO to process %d\n",mii_pid);
			if(send_mii_sig == 1){
				send_sig(SIGIO, p, 0);
			}
		}
	}
    	
	mii_timer.data = reg;
	mii_timer.expires = jiffies + (2000 * HZ / 1000);
	add_timer(&mii_timer);
}
#endif
//------------------------------------------------------------

static void get_token_str(char *str, char token[][21], int token_num)
{
    int t, i;

    for ( t = 0 ; t < token_num ; t++ )  {
    	memset(token[t], 0, 21);
    	while ( *str == ' ' )  str++;
    	for ( i = 0 ; str[i] ; i++ )  {
    	    if ( str[i] == '\t' || str[i] == ' ' || str[i] == '\n' )  break;
    	    if ( i < 20 )  token[t][i] = str[i];
    	}
    	str += i;
    }
}

//------------------------------------------------------------
static void set_led_status_by_str(int id)
{
  char token[3][21], *p;
  get_token_str(led_data[id].sts_buf, token, 3);

	#ifdef _KREBOOT_
	//add by Kyle 2008.07.29 for Upgrade reboot function.
	if ( !strcmp(token[0], "REBOOT") ) 
	{
		printk("Reboot System2 !!\n");
		#if defined(_REVERSE_BUTTON_)			
			if(id==_HW_BUTTON_WPS_){
		#else
			if(id==_HW_BUTTON_RESET_){
		#endif
			machine_restart(0);
		}
		
	}
	#endif	
	if ( strcmp(token[0], "LED") ) {
		goto set_led_off;
	}

	if ( !strcmp(token[1], "ON") )  {
		turn_led(id, 1);
		led_data[id].sts = LED_ON;
	}
	else if ( !strcmp(token[1], "OFF") )  {
		goto set_led_off;
	}
	else if ( !strcmp(token[1], "BLINK") ) {
  	unsigned int period = 0;
  	p = token[2];
  	if ( !strcmp(p, "FAST") )
  	    period = LED_BLINK_FAST & LED_BLINK_PERIOD;
  	else if ( !strcmp(p, "SLOW") )
  	    period = LED_BLINK_SLOW & LED_BLINK_PERIOD;
  	else if ( !strcmp(p, "EXTRA_SLOW") )
  	    period = LED_BLINK_EXTRA_SLOW & LED_BLINK_PERIOD;
  	else if ( !strcmp(p, "RANDOM") )
  	    period = LED_BLINK_RANDOM & LED_BLINK_PERIOD;
  	else if ( !strcmp(p, "OFF") )
		  goto set_led_off;
		else if ( *p >= '0' && *p <= '9' )  {
			while ( *p >= '0' && *p <= '9' )
			  period = period * 10 + (*p++) - '0';
		}
		else
			period = LED_BLINK & LED_BLINK_PERIOD;
	
		if ( period == 0 )
		  goto set_led_off;
		
		sprintf(led_data[id].sts_buf, "LED BLINK %d\n", period);
		led_data[id].sts = LED_BLINK_CMD + period;
		turn_led(id, 2);
	 	// Set timer for next blink
		del_timer(&blink_timer[id]);
		blink_timer[id].function = blink_wrapper;
		blink_timer[id].data = id;
		init_timer(&blink_timer[id]);
		      
		blink_timer[id].expires = jiffies + (1000 * HZ / 1000);
		     
		add_timer(&blink_timer[id]);
  }
  else{
    goto set_led_off;
	}
  return;

  set_led_off:
    strcpy(led_data[id].sts_buf, "LED OFF\n");
    led_data[id].sts = LED_OFF;
    turn_led(id, 0); 	 		

}
int usb_led_func(int id, int on_off)
{
	if(on_off)
		strcpy(led_data[id].sts_buf, "LED ON\n");
	else
		strcpy(led_data[id].sts_buf, "LED OFF\n");

	set_led_status_by_str(id);
}

#ifdef _ETHPORT_DETECT_
static int write_eth_detect_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;
		
	//DPRINTK("file: %08x, buffer: %s, count: %lu, data: %08x\n",
	//	(unsigned int)file, buffer, count, (unsigned int)data);

	if (buffer && !copy_from_user(&eth_detect_flag, buffer, 1)) {
		if (eth_detect_flag == '0') {
			send_mii_sig = 0;
		}
		else if (eth_detect_flag == '1') {
			send_mii_sig = 1;
		}
		else {
			send_mii_sig = 0;
		}
		return count;
	}
	else
		return -EFAULT;
}
#endif
#ifdef _WIFI_REPEATER_
#define RT2882_REG(x)          (*((volatile u32 *)(x)))
static int read_mode_sw_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data)
{
	int len;
	char flag;

	int mode, mode2;

	#ifdef _EW7438AC_
	mode = (RT2882_REG(RALINK_REG_PIO3924DATA) & (0x00000080));  //GPIO31
	mode2 = (RT2882_REG(RALINK_REG_PIO7140DATA) & (0x00800000)); //GPIO63
	if (mode + mode2 == 0x80)
		flag = '2'; // sleepMode Mode
	else if (mode + mode2 == 0x800080)
		flag = '6';// greenMode Mode
	else if (mode + mode2 == 0x800000)
		flag = '0';// NormalMode Mode
	else
		flag = 'E';// unknow -> error
	#else
	mode = (RT2882_REG(RALINK_REG_PIO7140DATA) & (0x00400000));  //GPIO62
	mode2 = (RT2882_REG(RALINK_REG_PIO7140DATA) & (0x00800000)); //GPIO63
	if (mode + mode2 == 0x400000)
		flag = '2'; // sleepMode Mode
	else if (mode + mode2 == 0xC00000)
		flag = '6';// greenMode Mode
	else if (mode + mode2 == 0x800000)
		flag = '0';// NormalMode Mode
	else
		flag = 'E';// unknow -> error
	#endif

	//printk("mode=%d, mode2=%d mode+mode2=%d\n", mode, mode2, mode+mode2);

	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

	

	return len;
}
static int read_reset_button_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data)
{
	int len;
	char flag;
	int mode;

	#ifdef _EW7438AC_
	mode = (RT2882_REG(RALINK_REG_PIO3924DATA) & (0x00000002)); //GPIO25
	#else
	mode = (RT2882_REG(RALINK_REG_PIO7140DATA) & (0x00100000)); //GPIO60
	#endif

	//printk("button mode=%d\n", mode);

	#ifdef _EW7438AC_
	if (mode == 0x2)
	#else
	if (mode == 0x100000)
	#endif
		flag = '0';// Reset Button
	else
		flag = '1';// Push Reset Button

	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}
#endif

//----------------------------------------------------------------------
static int led_read_proc(char *buf, char **start, off_t fpos, int length, int *eof, void *data)
{
  int len, dev;

  for ( len = dev = 0 ; dev < LED_DEV_NUM ; dev++ )  {
  	len += sprintf(buf+len, "%d: %s", dev, led_data[dev].sts_buf);
  }
  len = strlen(buf) - fpos;
  if ( len <= 0 ) {
		*start = buf;
		*eof = 1;
		return 0;
  }
  *start = buf + fpos;
  if ( len <= length )   *eof = 1;
  return len < length ? len : length;
}
static void reboot_wrapper(unsigned long period)
{
		machine_restart(0);
}
//add by kyle 2007/12/12 for gpio interrupt
static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int id = (int)file->private_data;
	unsigned long  tmp;
	cmd &= RALINK_GPIO_DATA_MASK;
	switch ( cmd )  {
		case RALINK_GPIO_REG_IRQ:
			printk(KERN_ERR NAME ":ioctl GPIO REG\n");
			copy_from_user(&info, (ralink_gpio_reg_info *)arg, sizeof(info));
			printk ("info.irq = %d\n", info.irq);
			if (0 <= info.irq && info.irq < RALINK_GPIO_NUMBER) {
				ralink_gpio_info[info.irq].pid = info.pid;
				if (info.irq <= 23) {
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIORENA));
					tmp |= (0x1 << info.irq);
					*(volatile u32 *)(RALINK_REG_PIORENA) = cpu_to_le32(tmp);
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOFENA));
					tmp |= (0x1 << info.irq);
					*(volatile u32 *)(RALINK_REG_PIOFENA) = cpu_to_le32(tmp);
				} else if (info.irq <= 39) {
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924RENA));
					tmp |= (0x1 << (info.irq-24));
					*(volatile u32 *)(RALINK_REG_PIO3924RENA) = cpu_to_le32(tmp);
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924FENA));
					tmp |= (0x1 << (info.irq-24));
					*(volatile u32 *)(RALINK_REG_PIO3924FENA) = cpu_to_le32(tmp);
				} else if (info.irq <= 71) {
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140RENA));
					tmp |= (0x1 << (info.irq-40));
					*(volatile u32 *)(RALINK_REG_PIO7140RENA) = cpu_to_le32(tmp);
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140FENA));
					tmp |= (0x1 << (info.irq-40));
					*(volatile u32 *)(RALINK_REG_PIO7140FENA) = cpu_to_le32(tmp);
				} else {
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72RENA));
					tmp |= (0x1 << (info.irq-72));
					*(volatile u32 *)(RALINK_REG_PIO72RENA) = cpu_to_le32(tmp);
					tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72FENA));
					tmp |= (0x1 << (info.irq-72));
					*(volatile u32 *)(RALINK_REG_PIO72FENA) = cpu_to_le32(tmp);
				}		
#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
				mii_pid = info.pid;
#endif	
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOEDGE));
				tmp |=  0x3<<(info.irq*2);
				*(volatile u32 *)(RALINK_REG_PIOEDGE) = cpu_to_le32(tmp);
			}else {
				printk(KERN_ERR NAME ": irq number(%d) out of range\n",
						info.irq);
			}
		break;
#ifdef _KREBOOT_
		//add by Kyle 2008.02.29 for Upgrade reboot function.
		case REBOOT:
			rebootTimer.function = reboot_wrapper;
			rebootTimer.data = 3000;
			init_timer(&rebootTimer);
			rebootTimer.expires = jiffies + (3000 * HZ / 1000);
			add_timer(&rebootTimer);
			printk("Add Reboot timer !!\n");
			//machine_restart(0);
		break;
#endif		
    }
    return 0;
}

//----------------------------------------------------------------------
static long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int id = (int)file->private_data;
    switch ( cmd )  {
      case LED_ON:
    	strcpy(led_data[id].sts_buf, "LED ON\n");
	set_led_status_by_str(id);
	break;
      case LED_OFF:
    	strcpy(led_data[id].sts_buf, "LED OFF\n");
			set_led_status_by_str(id);
			break;
      default:
        if ( (cmd & LED_BLINK_CMD) != LED_BLINK_CMD )
		{
	    break;
		}
      case LED_BLINK:
      case LED_BLINK_FAST:
      case LED_BLINK_SLOW:
      case LED_BLINK_EXTRA_SLOW:
      case LED_BLINK_RANDOM:
        sprintf(led_data[id].sts_buf, "LED BLINK %d\n", (int)(cmd & LED_BLINK_PERIOD));
	set_led_status_by_str(id);
	break;
    }
    return 0;
}

static int led_open(struct inode *inode, struct file *file)
{
    int led_id = MINOR(inode->i_rdev);
//    unsigned long led_bit = 1 << (led_id);

    if ( led_id >= LED_DEV_NUM )
        return -ENODEV;
/* sam 12/02/2003
    GPIO_SEL_I_O &= ~led_bit;   // 0 to GPIO
    GPIO_O_EN |= (led_bit << 16);   // 0 to Output
*/	
	
    file->private_data = (void*)led_id;
    return 0;
}

static ssize_t led_read(struct file *file, char *buf, size_t count, loff_t *fpos)
{
    int  rem, len;
    int  id = (int)file->private_data;
    char *p = led_data[id].sts_buf;

    len = strlen(p);
    rem = len - *fpos;
    if ( rem <= 0 )  {
    	*fpos = len;
    	return 0;
    }
    if ( rem > count )   rem = count;
    memcpy(buf, p+(*fpos), rem);
    *fpos += rem;
    return rem;
}

//static ssize_t led_write(struct file *file, char *buf, size_t count, loff_t *fpos)
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *fpos)
{
    int  len;
    int  id = (int)file->private_data;
    char *p = id == REG_MINOR ? cmd_buf : led_data[id].sts_buf;
    memset(p, 0, BUF_LEN);

    p += *fpos;
    len = 0;

	
    while ( count > 0 )  
	{
		
    	if ( *fpos < BUF_LEN )  
		{
    	    int c = *buf++;
            p[len] = c>='a' && c<='z' ? c-'a'+'A' : c;
        }
    	(*fpos)++;
	    len++;
    	count--;
    }
	// sam
    set_led_status_by_str(id);
	(*fpos) = 0;
	//
	
    return len;
}

static int led_flush(struct file *file, fl_owner_t flid)
{
    int  id = (int)file->private_data;

    if ( file->f_mode & FMODE_WRITE )
	{
    	set_led_status_by_str(id);
	}
    return 0;
}

static struct file_operations led_fops = {
    .read  = led_read,
    .write = led_write,
    .flush = led_flush,
    .unlocked_ioctl = led_ioctl,
    .open =  led_open,
};

//----------------------------------------------

static int dump_content(char *buf)
{
    return 0;
}

static ssize_t gpio_read(struct file *file, char *buf, size_t count, loff_t *fpos)
{
	int  rem, len;
	int  id = (int)file->private_data, gpio_bit;
	char temp[80*10];
	unsigned long gpio_regval = 0;

#if defined(_REVERSE_BUTTON_)

	if(id==_HW_BUTTON_WPS_)
		id=_HW_BUTTON_RESET_;
	else if(id==_HW_BUTTON_RESET_)
		id=_HW_BUTTON_WPS_;
#endif

#if defined(_EW7438AC_)
	gpio_regval = le32_to_cpu(*(volatile u32 *) (RALINK_REG_PIO3924DATA));
	id = id - 24; //GPIO 24-39
#elif defined(_EW7478AC_)
	gpio_regval = le32_to_cpu(*(volatile u32 *) (RALINK_REG_PIO7140DATA));
	id = id - 40;
#else
	gpio_regval = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
#endif
	gpio_bit = 1 << id;

	if ( id < GPIO_DEV_NUM )  {

		switch(id){
#if 0
			#if defined(_ARSWITCH_) || defined(_WIRELESS_SWITCH_)
			case _HW_BUTTON_SWITCH_://ap router switch
				#if defined(_ARSWITCH_) || defined(_WIRELESS_SWITCH_)
					#ifdef _WIRELESS_SWITCH_REVERSE_2_
						len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 0 : 1); //EDX shakim
					#else
						len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 0);
					#endif
				#else
					len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 1);
				#endif
		
				break;
			#endif
			case _HW_BUTTON_WPS_://wps
				#if defined(_WPS_INDEPEND_) || defined(_REVERSE_BUTTON_)
					len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 0);
				#else
					len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 1);
				#endif
				break;
#endif		
			case _HW_BUTTON_RESET_://reset
				#if defined(_REVERSE_BUTTON_)
					#if defined(_WPS_INDEPEND_) 
						len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 0);
					#else
						len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 1);
					#endif
				#else
					len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 0);
				#endif
				break;	
			
			default:
				len = sprintf(temp, "%d\n", (gpio_regval & gpio_bit) ? 1 : 0);
				break;
		}
	}

    	else   // REG device
        	len = dump_content(temp);

    rem = len - *fpos;
    if ( rem <= 0 )  {
    	*fpos = len;
    	return 0;
    }
    if ( rem > count )   rem = count;
    memcpy(buf, temp+(*fpos), rem);
    *fpos += rem;
    return rem;
}

static int gpio_open(struct inode *inode, struct file *file)
{
    int id = MINOR(inode->i_rdev);
		//printk("@@@@@@@ gpio_open id=%d  GPIO_DEV_NUM=%d\n",id,GPIO_DEV_NUM);
    if ( id >= GPIO_DEV_NUM && id != REG_MINOR )
        return -ENODEV;
    file->private_data = (void*)id;
    return 0;
}

static struct file_operations gpio_fops = {
    .read  = gpio_read,
    .open  = gpio_open,
    .unlocked_ioctl = gpio_ioctl,
    .write = led_write,
};

//----------------------------------------------
//----------------------------------------------
static int init_status;

#define INIT_REGION	        0x01
#define INIT_LED_REGISTER	0x02
#define INIT_LED_PROC_READ	0x04
#define INIT_GPIO_REGISTER	0x08
#define INIT_LAN_STATUS_REGISTER 0x10
#define INIT_WATCHDOG_REGISTER 0x20

static void led_exit(void)
{
    int id;
    for ( id = 0 ; id < LED_DEV_NUM ; id++ )  {
        del_timer(&blink_timer[id]);
        turn_led(id, 1);
    }
    if ( init_status & INIT_LED_PROC_READ )
    	remove_proc_entry("driver/led", NULL);
    	
    if ( init_status & INIT_LED_REGISTER )
    	unregister_chrdev(LED_MAJOR, "led");

    if ( init_status & INIT_GPIO_REGISTER )
    	unregister_chrdev(GPIO_MAJOR, "gpio");
    
    if( init_status & INIT_WATCHDOG_REGISTER)
       del_timer(&watchdog);
}



//add by kyle 2007/12/12 for gpio interrupt
/*
 * 1. save the PIOINT and PIOEDGE value
 * 2. clear PIOINT by writing 1
 * (called by interrupt handler)
 */
void ralink_gpio_save_clear_intp(void) {
	ralink_gpio_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOINT));
	ralink_gpio_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIOEDGE));
	*(volatile u32 *)(RALINK_REG_PIOINT) = cpu_to_le32(0x00FFFFFF);
	*(volatile u32 *)(RALINK_REG_PIOEDGE) = cpu_to_le32(0x00FFFFFF);

	ralink_gpio3924_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924INT));
	ralink_gpio3924_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924EDGE));
	*(volatile u32 *)(RALINK_REG_PIO3924INT) = cpu_to_le32(0x0000FFFF);
	*(volatile u32 *)(RALINK_REG_PIO3924EDGE) = cpu_to_le32(0x0000FFFF);
	ralink_gpio7140_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140INT));
	ralink_gpio7140_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO7140EDGE));
	*(volatile u32 *)(RALINK_REG_PIO7140INT) = cpu_to_le32(0xFFFFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO7140EDGE) = cpu_to_le32(0xFFFFFFFF);	
	
	ralink_gpio72_intp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72INT));
	ralink_gpio72_edge = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO72EDGE));
	*(volatile u32 *)(RALINK_REG_PIO72INT) = cpu_to_le32(0x00FFFFFF);
	*(volatile u32 *)(RALINK_REG_PIO72EDGE) = cpu_to_le32(0x00FFFFFF);
}


#ifdef _BR6477AC_
//add by kyle 2007/12/12 for gpio interrupt
irqreturn_t ralink_gpio_irq_handler(int irq, void *irqaction)
{
	int resetButton=_HW_BUTTON_RESET_;
#if 0
	int wpsButton=_HW_BUTTON_WPS_;
	#if defined(_ARSWITCH_) || defined(_WIRELESS_SWITCH_)
	int switchButton1=_HW_BUTTON_SWITCH_;
	#endif
#endif

	#if defined (_REVERSE_BUTTON_)
		resetButton=_HW_BUTTON_WPS_;
		wpsButton=_HW_BUTTON_RESET_;
	#endif

	struct gpio_time_record {
		unsigned long falling;
		unsigned long rising;
	};
	static struct gpio_time_record record[RALINK_GPIO_NUMBER];
	unsigned long now;
	int i;

	ralink_gpio_save_clear_intp();
	now = jiffies;

	for (i = 0; i < 24; i++) {
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
#if defined(_WIRELESS_SWITCH_)
				if (i == _HW_BUTTON_SWITCH_){
					ralink_gpio_notify_user(3);
					printk(KERN_NOTICE NAME ":Wireless Switch!\n");
				}
#endif	
#if 0 //EDX
				if (time_before(now, record[i].falling + 200L)) {
					//one click
					printk("i=%d, one click\n", i);
					ralink_gpio_notify_user(1);
				}
				else {
					//press for several seconds
					printk("i=%d, push several seconds\n", i);
					ralink_gpio_notify_user(2);
				}
#endif //EDX
				if (time_before(now, record[i].falling + 500L)) {
					#if !defined(_COREGA_WPS_TRIGGER_CONDITION_) || !defined(_HW_MACFILTER_IPHONE_)
						//one click
						#if !defined (_WPS_ACTIVE_FALLING_EDGE_)
							#if defined(_WPS_INDEPEND_)
								if(i==wpsButton)
								{
									ralink_gpio_notify_user(1);
									printk(KERN_NOTICE NAME ":WPS Function!\n");
								}
							#else
								if(i==resetButton)
								{
									ralink_gpio_notify_user(1);
									printk(KERN_NOTICE NAME ":WPS Function!\n");
								}
							#endif
						#endif //_WPS_ACTIVE_FALLING_EDGE_
					#endif
				} else {
					//press for several seconds
					printk(KERN_NOTICE NAME ":press for 5 seconds!\n");
				}
			}
		}
		else { //falling edge
			if (record[i].falling != 0 && time_before_eq(now, record[i].falling + 40L)) {
				/*
				 * If the interrupt comes in a short period,
				 * it might be floating. We ignore it.
				 */
			} else {
				record[i].falling = now;
				if(i==resetButton) {
					ralink_gpio_notify_user(2);
#if defined(_COREGA_WPS_TRIGGER_CONDITION_)
					ralink_gpio_notify_user(1);
#endif
				}
#if defined(_HW_MACFILTER_IPHONE_) || defined(_WPS_ACTIVE_FALLING_EDGE_)
				if(i==wpsButton) {
					ralink_gpio_notify_user(1);
					printk(KERN_NOTICE NAME ":WPS Function!\n");
				}
#endif
#if defined(_WIRELESS_SWITCH_)
				if (i == switchButton1) {
					ralink_gpio_notify_user(3);
				}
#endif
			}
		}
		break;
	}
	for (i = 24; i < 40; i++) {
		if (! (ralink_gpio3924_intp & (1 << (i - 24))))
			continue;
		ralink_gpio_irqnum = i;
		if (ralink_gpio3924_edge & (1 << (i - 24))) {
			if (record[i].rising != 0 && time_before_eq(now,
						record[i].rising + 40L)) {
			}
			else {
				record[i].rising = now;
				if (time_before(now, record[i].falling + 200L)) {
					printk("i=%d, one click\n", i);
					ralink_gpio_notify_user(1);
				}
				else {
					printk("i=%d, push several seconds\n", i);
					ralink_gpio_notify_user(2);
				}
			}
		}
		else {
			record[i].falling = now;
		}
		break;
	}
	for (i = 40; i < 72; i++) {
		if (! (ralink_gpio7140_intp & (1 << (i - 40))))
			continue;
		ralink_gpio_irqnum = i;
		if (ralink_gpio7140_edge & (1 << (i - 40))) {
			if (record[i].rising != 0 && time_before_eq(now,
						record[i].rising + 40L)) {
			}
			else {
				record[i].rising = now;
				if (time_before(now, record[i].falling + 200L)) {
					printk("i=%d, one click\n", i);
					ralink_gpio_notify_user(1);
				}
				else {
					printk("i=%d, push several seconds\n", i);
					ralink_gpio_notify_user(2);
				}
			}
		}
		else {
			record[i].falling = now;
		}
		break;
	}
	for (i = 72; i < RALINK_GPIO_NUMBER; i++) {
		if (! (ralink_gpio72_intp & (1 << (i - 72))))
			continue;
		ralink_gpio_irqnum = i;
		if (ralink_gpio72_edge & (1 << (i - 72))) {
			if (record[i].rising != 0 && time_before_eq(now,
						record[i].rising + 40L)) {
			}
			else {
				record[i].rising = now;
				if (time_before(now, record[i].falling + 200L)) {
					printk("i=%d, one click\n", i);
					ralink_gpio_notify_user(1);
				}
				else {
					printk("i=%d, push several seconds\n", i);
					ralink_gpio_notify_user(2);
				}
			}
		}
		else {
			record[i].falling = now;
		}
		break;
	}
	return IRQ_HANDLED;
}
#else // EW7438AC or EW7478AC
irqreturn_t ralink_gpio_irq_handler(int irq, void *irqaction)
{
	//int wpsButton=_HW_BUTTON_WPS_;
	//int resetButton = _HW_BUTTON_RESET_;
	//int switchButton1=_HW_BUTTON_SWITCH_;
	//#if defined (_REVERSE_BUTTON_)
	//	resetButton=_HW_BUTTON_WPS_;
	//	wpsButton=_HW_BUTTON_RESET_;
	//#endif

	struct gpio_time_record
	{
		unsigned long falling;
		unsigned long rising;
	};
	static struct gpio_time_record record[RALINK_GPIO_NUMBER];
	unsigned long now;
	int i, edge;

	ralink_gpio_save_clear_intp();
	now = jiffies;

	for (i = 0; i < RALINK_GPIO_NUMBER; i++)
	{
		// See if interrupt occurs, and what is its edge.
		switch (i)
		{
			case 0 ... 23:
				if (!(ralink_gpio_intp & (1 << (i - 0))))
					continue;
				edge = ralink_gpio_edge & (1 << (i - 0));
				break;
			case 24 ... 39:
				if (!(ralink_gpio3924_intp & (1 << (i - 24))))
					continue;
				edge = ralink_gpio3924_edge & (1 << (i - 24));
				break;
			case 40 ... 71:
				if (!(ralink_gpio7140_intp & (1 << (i - 40))))
					continue;
				edge = ralink_gpio7140_edge & (1 << (i - 40));
				break;
			default:
				if (!(ralink_gpio72_intp & (1 << (i - 72))))
					continue;
				edge = ralink_gpio72_edge & (1 << (i - 72));
				break;
		}

		printk("<%s> GPIO %2d is %s.\n", __FILE__, i, edge ? "rising" : "falling");
		ralink_gpio_irqnum = i;
		if (edge)
		{
			if (record[i].rising != 0 && time_before_eq(now, record[i].rising + 40L))
			{
				printk("<%s> Time too short, skip.\n", __FILE__);
				continue;
			}
			record[i].rising = now;
		}
		else
		{
			if (record[i].falling != 0 && time_before_eq(now, record[i].falling + 40L))
			{
				printk("<%s> Time too short, skip.\n", __FILE__);
				continue;
			}
			record[i].falling = now;
		}
		// Jobs of each gpio
		switch (i)
		{
			case _HW_BUTTON_RESET_:
				printk("<%s> Reset button is %s.\n", __FILE__, edge ? "released" : "pressed");
				if (edge)
				{
					if (time_before(now, record[i].falling + 1250L))
						ralink_gpio_notify_user(1);

					printk("<%s> Press second = %d\n", __FILE__, (now - record[i].falling) / HZ);
				}
				else
				{
					ralink_gpio_notify_user(2);
				}
				break;
			default:
				printk("<%s> Unknown button (GPIO=%d) is %s.\n", __FILE__, i, edge ? "released" : "pressed");
				break;
		}
	}
	return IRQ_HANDLED;
}
#endif


static int __init led_init(void)
{
	int error;
	printk("Init GPIO Interrupt\n");	
	int i=0;

	#ifdef _ETHPORT_DETECT_
	struct proc_dir_entry *res;
	res = create_proc_entry("ETH_DETECT", 0, NULL);
	if (res)
	{
		res->read_proc = NULL;
		res->write_proc = write_eth_detect_proc;
		
	}
	else
	{
		printk("Ralink GPIO Driver, create ETH_DETECT failed!\n");
	}
	#else
	struct proc_dir_entry *res;
	#endif
	#ifdef _WIFI_REPEATER_
	res = create_proc_entry("MODE_SW", 0, NULL);
	if (res)
	{
		res->read_proc = read_mode_sw_proc;
		res->write_proc = NULL;
	}
	else
	{
		printk("Ralink GPIO Driver, create MODE_SW failed!\n");
	}
	res = create_proc_entry("RESET_BUTTON", 0, NULL);
	if (res)
	{
		res->read_proc = read_reset_button_proc;
		res->write_proc = NULL;
	}
	else
	{
		printk("Ralink GPIO Driver, create RESET_BUTTON failed!\n");
	}
	#endif

	//enable gpio interrupt
	*(volatile u32 *)(RALINK_REG_INTENA) = cpu_to_le32(RALINK_INTCTL_PIO);
	for (i = 0; i < RALINK_GPIO_NUMBER; i++) {
		ralink_gpio_info[i].irq = i;
		ralink_gpio_info[i].pid = 0;
	}

	printk("***********Init LED Driver Finishing*****************\n");	 
	int result, id;
	init_status = 0;
	
	//----- register device (LED)-------------------------
	result = register_chrdev(LED_MAJOR, "led", &led_fops);
	if ( result < 0 )   {
		printk(KERN_ERR "led: can't register char device\n" );
		led_exit();
		return result;
	}

	init_status |= INIT_LED_REGISTER;
	//----- register device (GPIO)-------------------------
	result = register_chrdev(GPIO_MAJOR, "gpio", &gpio_fops);
	if ( result < 0 )   {
		printk(KERN_ERR "gpio: can't register char device\n" );
		led_exit();
		return result;
	}
	init_status |= INIT_GPIO_REGISTER;

	//------ read proc -------------------
	if ( !create_proc_read_entry("driver/led", 0, 0, led_read_proc, NULL) )  {
		printk(KERN_ERR "led: can't create /proc/driver/led\n");
		led_exit();
		return -ENOMEM;
	}
	init_status |= INIT_LED_PROC_READ;
	//------------------------------


	for ( id = 0; id < LED_DEV_NUM; id++ )
	{
		#ifdef _BR6477AC_
		if (id==-1 ){
		#elif defined(_EW7478AC_)
		if (id==66 || id==67 || id==68 || id==69 || id==70 || id==71 ){
		#elif defined(_BR6278AC_)
		if (id==41 || id==42 || id==43 || id==44  || id==39){
		#else
		if (id==24 || id==27 || id==28 || id==29 || id==32 || id==33 ){
		#endif
			strcpy(led_data[id].sts_buf, "LED ON\n" );
			set_led_status_by_str(id);
		}
	}


#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
	del_timer(&mii_timer);
	mii_timer.function = mii_wrapper;
	u32 temp;
	mii_mgr_read(3,1,&temp);
	mii_timer.data = (temp>>5)&1;
	mii_timer.expires = jiffies + (2000 * HZ / 1000);    
	init_timer(&mii_timer);
	add_timer(&mii_timer);
#endif	

	printk("Ralink_gpio_init_irq \n");
	error = request_irq(SURFBOARDINT_GPIO, ralink_gpio_irq_handler, IRQF_DISABLED, NAME, NULL);
	if (error)
		return error;
	printk(KERN_INFO "LED & GPIO & LAN Status Driver LED_VERSION \n");	
	return 0;
}


//add by kyle 2007/12/12 for gpio interrupt
void ralink_gpio_notify_user(int usr)
{
	struct task_struct *p = NULL;

	if (ralink_gpio_irqnum < 0 || RALINK_GPIO_MAX_INFO <= ralink_gpio_irqnum) {
		printk(KERN_ERR NAME ": gpio irq number out of range\n");
		return;
	}

	//don't send any signal if pid is 0 or 1
	if ((int)ralink_gpio_info[ralink_gpio_irqnum].pid < 2)
		return;
	p = find_task_by_vpid(ralink_gpio_info[ralink_gpio_irqnum].pid);
	if (NULL == p) {
		printk(KERN_ERR NAME ": no registered process to notify\n");
		return;
	}

	if ( usr == 1 ) {
		printk(KERN_NOTICE NAME ": sending a SIGUSR1 to process %d\n",
				ralink_gpio_info[ralink_gpio_irqnum].pid);
		send_sig(SIGUSR1, p, 0);
	}
	else if (usr == 2) {
		printk(KERN_NOTICE NAME ": sending a SIGUSR2 to process %d\n",
				ralink_gpio_info[ralink_gpio_irqnum].pid);
		send_sig(SIGUSR2, p, 0);
	}//for wireless H/W switch function
#if defined(_WIRELESS_SWITCH_) 
	else if ( usr == 3 ) {
		printk(KERN_NOTICE NAME ": sending a SIGIO to process %d\n",
				ralink_gpio_info[ralink_gpio_irqnum].pid);
		send_sig(SIGIO, p, 0);
	}
#endif
}


#if 0 // RexHua move to module_init
void __init ralink_gpio_init_irq(void)
{
	printk("\n\n\n ralink_gpio_init_irq \n\n\n");
	setup_irq(SURFBOARDINT_GPIO, &ralink_gpio_irqaction);
}
#endif

module_init(led_init);
module_exit(led_exit);


