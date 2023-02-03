/**************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************
 *
 * $Id: gpio.c,v 1.6.2.1 2007-11-06 09:17:21 winfred Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "led.h"

#if defined(_Customer_) || defined (_EW7478AC_) || defined (_BR6278AC_)
#define GPIO_DEV	"/dev/reset_but"
#else
#define GPIO_DEV	"/dev/wps_but"
#endif

#define WLAN_INTERFACE	"ra0"
#define INIC_INTERFACE	"rai0"

#if defined(_Customer_) || defined (_EW7478AC_)
#define RESET_BUTTON	44
#else
#define RESET_BUTTON	4
#endif

#if defined(_WIRELESS_SWITCH_)
#define SWITCH_BUTTON	2
#endif

#if defined (_WPS_INDEPEND_)
#define WPS_BUTTON	1
#endif

static int interface_select = 0;
static int test_button = 0;
static int button_count = 0;
static int wlan_unconfig = 0, inic_unconfig = 0;
static int call_wps = 1;

void wps_function()
{
	char buffer[128];

	FILE *fp;

	if (interface_select == 0){

		if (wlan_unconfig != 2){

			memset (buffer, 0x00, sizeof(buffer));
			sprintf (buffer, "wpstool %s getCurrentWscProfile", WLAN_INTERFACE);
	
			if ( (fp = popen(buffer, "r")) != NULL){
				if ( fgets (buffer, sizeof(buffer), fp) > 0){
					if (wlan_unconfig == 0)
						wlan_unconfig = atoi (&buffer[0]);
					else{
						if (wlan_unconfig != atoi (&buffer[0])){
							wlan_unconfig = atoi (&buffer[0]);
							memset (buffer, 0x00, sizeof(buffer));
							sprintf (buffer, "wpstool %s get_Wsc_Profile&", WLAN_INTERFACE);
							system (buffer);
						}
					}
				}
				pclose (fp);
			}
		}

		if (inic_unconfig != 2){
			memset (buffer, 0x00, sizeof(buffer));
			sprintf (buffer, "wpstool %s getCurrentWscProfile", INIC_INTERFACE);
	
			if ( (fp = popen(buffer, "r")) != NULL){
		
				if ( fgets (buffer, sizeof(buffer), fp) > 0){
					if (inic_unconfig == 0)
						inic_unconfig = atoi (&buffer[0]);
					else{
						if (inic_unconfig != atoi (&buffer[0])){
							inic_unconfig = atoi (&buffer[0]);
							memset (buffer, 0x00, sizeof(buffer));
							sprintf (buffer, "wpstool %s get_Wsc_Profile&", INIC_INTERFACE);
							system (buffer);
						}
					}
				}
				pclose (fp);
			}
		}

		if ((wlan_unconfig != 2) || (inic_unconfig != 2))
			alarm (5);

	}else if (interface_select == 1){ // if pressed once

		call_wps = 1;

		#if defined(_WIFI_REPEATER_)
		if( (fp=fopen("/tmp/SleepMode", "r")) != NULL ){
			printf("\n[ received signal: Disable Sleep Mode ]\n\n");
			system ("/bin/scriptlib_util.sh setUpWireless up Aband sleepMode");
			system ("/bin/scriptlib_util.sh setUpWireless up Gband sleepMode 3");
			fclose(fp);
			call_wps = 0;
		}
		if( (fp=fopen("/tmp/Gschedule", "r")) != NULL ){
			printf("\n[ received signal: Disable Gband schedule ]\n\n");
			system ("/bin/scriptlib_util.sh setUpWireless up Gband schedule");
			fclose(fp);
			call_wps = 0;
		}
		if( (fp=fopen("/tmp/Aschedule", "r")) != NULL ){
			printf("\n[ received signal: Disable Aband schedule ]\n\n");
			system ("/bin/scriptlib_util.sh setUpWireless up Aband schedule");
			fclose(fp);
			call_wps = 0;
		}
		if(call_wps){
			system("/bin/interrupt.sh null wps &");
			printf("\n[ received signal: interrupt.sh null wps & ]\n\n");
		}
		#else
		system("/bin/interrupt.sh null wps &");
		printf("\n[ received signal: interrupt.sh null wps & ]\n\n");
		#endif

	}else if (interface_select > 1){  // if pressed twice
		sprintf (buffer, "killall wpstool");
		system (buffer);
		printf(" received signal : killall wpstool\n");
//		#if defined(_WIFI_ROMAING_)
//		system("/bin/Button_FW_upgrade.sh &");
//		printf("====Start Auto FW upgrade====\n");
//		#endif
	}

	interface_select = 0;
}

void signal_handler(int signum)
{
	char buffer[128];

	if (signum == SIGUSR1){
		interface_select ++;
		alarm(1);
	}
	else if (signum == SIGUSR2)
		system("/bin/interrupt.sh null reset &");
#if defined(_WIRELESS_SWITCH_)
	else if (signum == SIGIO){
		sprintf (buffer, "/bin/interrupt.sh %s wireless &", WLAN_INTERFACE);
		system (buffer);
	}
#endif
#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
	else if (signum == SIGURG)
	{
		system("echo 'LED ON' > /dev/LAN_LED");
		system("killall ReNewIP.sh");
		system("/bin/ReNewIP.sh &");
#ifdef _ETH_TO_AP_
		system("kill -9 `pidof DetectEthernet.sh`");
		system("/bin/DetectEthernet.sh &");
#endif
	}
	else if (signum == SIGIO)
	{
		system("echo 'LED OFF' > /dev/LAN_LED");
		system("kill -9 `pidof ReNewIP.sh`");
#ifdef _WIFI_ROMAING_
		system("kill -9 `pidof DetectEthernet.sh`");
#endif
	}
//		system("/bin/scriptlib_util.sh reconnect_dhcpc &");
#endif
	else
		printf("%d", signum);
	printf(" received signal: %d \n", signum);
}
int gpio_reg_info(void)
{
	int fd;
	ralink_gpio_reg_info info;

	fd = open(GPIO_DEV, O_RDONLY);
	if (fd < 0) {
		perror(GPIO_DEV);
		return -1;
	}
	info.pid = getpid();
	//register reset button interrupt.
#if defined(_REVERSE_BUTTON_)
	info.irq = _HW_BUTTON_WPS_;
#else
	info.irq = _HW_BUTTON_RESET_;

#endif

	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0) {
		perror("ioctl reset button");
		close(fd);
		return -1;
	}
#if defined_WPS_INDEPEND_
	//register wps button interrupt.
#if defined(_REVERSE_BUTTON_)
	info.irq = _HW_BUTTON_RESET_;
#else
	info.irq = _HW_BUTTON_WPS_;

#endif

	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0) {
		perror("ioctl wps button");
		close(fd);
		return -1;
	}
#endif
#if defined(_WIRELESS_SWITCH_)
	info.irq = _HW_BUTTON_SWITCH_;
	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0) {
		perror("ioctl wireless switc");
		close(fd);
		return -1;
	}
#endif

	close(fd);
}


void mu_handler(int signum)
{
	char buf[32];
	
	if (signum == SIGUSR1){
		button_count |= RESET_BUTTON;

	}else if (signum == SIGUSR2){
#if defined (_WPS_INDEPEND_)
		button_count |= WPS_BUTTON;
#else
		button_count |= RESET_BUTTON;
#endif
	
	}else if (signum == SIGIO){
#if defined(_WIRELESS_SWITCH_)
		button_count |= SWITCH_BUTTON;
#endif
	}

	sprintf (buf, "echo %d > /tmp/mu_button", button_count);
	system (buf);
	printf(" received signal: %d \n", signum);
}

void gpio_mu_intr (void)
{
	//issue a handler to handle SIGUSR1
	signal(SIGUSR1, mu_handler);
	signal(SIGUSR2, mu_handler);
#if defined(_WIRELESS_SWITCH_)
	signal(SIGIO, mu_handler);
#endif

#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
	signal(SIGURG, mu_handler);
	signal(SIGIO, mu_handler);
#endif

	//wait for signal
	while ( 1 ){
		pause();
	}
}

void gpio_test_intr(void)
{
	//issue a handler to handle SIGUSR1
	signal(SIGUSR1, signal_handler);

	signal(SIGUSR2, signal_handler);

#if defined(_WIRELESS_SWITCH_)
	signal(SIGIO, signal_handler);
#endif

#ifdef _ETHPORT_DETECT_ //vance 2009.04.30 for detect ethernet port
	signal(SIGURG, signal_handler);
	signal(SIGIO, signal_handler);
#endif
	signal(SIGALRM, wps_function);

	// first run wait system init done
	alarm (10);

	//wait for signal
	while ( 1 ){
		pause();
	}
}

void usage(char *cmd)
{
	printf ("%s function wlan_configed inic_configed\n", cmd);
	printf ("function: 0 or 1 ; 0 ==> normal, 1==> manufacture \n");
	printf ("wlan_configed: 0 or 1 ; 0 ==> unconfiged, 1==> configed \n");
	printf ("inic_configed: 0 or 1 ; 0 ==> unconfiged, 1==> configed \n");
	exit(0);
}
int main(int argc, char *argv[])
{
	char buffer[128];
	FILE *fp;

	test_button = atoi (argv[1]);

	if ( (argc == 2) && (test_button == 1)){
		system ("echo 0 > /tmp/mu_button");

	}else if ( (argc == 4) && (test_button == 0)){
		if (atoi(argv[2]) == 1)
			wlan_unconfig = 2;

		if (atoi(argv[3]) == 1)
			inic_unconfig = 2;
			
	}else{
		usage(argv[0]);

	}

	gpio_reg_info();
	
	if (test_button){
		gpio_mu_intr();
	}else{
		gpio_test_intr();
	}
	return 0;
}

