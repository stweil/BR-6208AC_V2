
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#if 0
#define DEBUG_PRINT(fmt, args...)	printf(fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)	{}
#endif

int main(int argc, char *argv[])
{

	FILE *fp;
	int current_mode, mp_mode, last_mode, wiz_mode;
	int RunCommand=1;

	int NormalMode=0;
	int sleepMode=2;
	int greenMode=6;
	int wifi_bridge=3;

	system("echo 0 > /tmp/mp_mode");
	fp = fopen("/proc/MODE_SW", "r");
	fscanf(fp, "%d", &last_mode);
	fclose(fp);

	/*fp = fopen("/tmp/wizmodeNow", "r");
	fscanf(fp, "%d", &wiz_mode);
	fclose(fp);*/

	while(1)
	{
		fp = fopen("/tmp/mp_mode", "r");
		fscanf(fp, "%d", &mp_mode);
		fclose(fp);
		
		if (mp_mode == 0){

			fp = fopen("/proc/MODE_SW", "r");
			fscanf(fp, "%d", &current_mode);
			fclose(fp);

			if(current_mode != last_mode) { // record
				if(last_mode == sleepMode)
					system("/bin/reboot.sh");
				else	//EDX yihong for EMI test
				{
					if (current_mode == 2)
					{
						system("echo 'LED OFF' > /dev/PowerLED");
						system("echo 'LED OFF' > /dev/ABAND_LED");
						system("echo 'LED OFF' > /dev/GBAND_LED");
						system("echo 'LED OFF' > /dev/WlanLED");
					}
					else
						system("echo 'LED ON' > /dev/PowerLED");	

					last_mode = current_mode;
				}	
//				else
//					last_mode = current_mode;

				RunCommand=1;
			}

			if(RunCommand){
				if(current_mode == greenMode){
					system("/bin/scriptlib_util.sh setTxpower ra0 4");
					system("/bin/scriptlib_util.sh setTxpower rai0 4");
				}
				else{
					system("/bin/scriptlib_util.sh setTxpower ra0 1");
					system("/bin/scriptlib_util.sh setTxpower rai0 1");
					if(current_mode == sleepMode){
						system("/bin/scriptlib_util.sh setUpWireless down Aband sleepMode");
						system("/bin/scriptlib_util.sh setUpWireless down Gband sleepMode");
					}
				}
				RunCommand=0;
			}
		}
		usleep(200000);	//200ms
	}
}

