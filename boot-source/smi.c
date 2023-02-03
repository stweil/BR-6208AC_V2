/*
* Copyright c                  Realtek Semiconductor Corporation, 2006 
* All rights reserved.
* 
* Program : Control  smi connected RTL8366
* Abstract : 
* Author : Yu-Mei Pan (ympan@realtek.com.cn)                
*  $Id: smi.c,v 1.2 2008-04-10 03:04:19 shiehyy Exp $
*/


#include <rtk_types.h>
//EDX jerry #include <gpio.h>
#include <smi.h>
#include <rtk_error.h>
#include <common.h>

#define MDC_MDIO_DUMMY_ID           0
#define MDC_MDIO_CTRL0_REG          31
#define MDC_MDIO_CTRL1_REG          21
#define MDC_MDIO_ADDRESS_REG        23
#define MDC_MDIO_DATA_WRITE_REG     24
#define MDC_MDIO_DATA_READ_REG      25
#define MDC_MDIO_PREAMBLE_LEN       32

#define MDC_MDIO_ADDR_OP           0x000E
#define MDC_MDIO_READ_OP           0x0001
#define MDC_MDIO_WRITE_OP          0x0003

//EDX shakim
#if defined(RT6855A_ASIC_BOARD)
#define RST_DELAY 	                20000000
#define DELAY                        3000
#else
#define RST_DELAY 	                10000000	
#define DELAY                        1500
#endif
//EDX shakim

#define CLK_DURATION(clk)            { int i; for(i=0; i<clk; i++); }
#define _SMI_ACK_RESPONSE(ok)        { /*if (!(ok)) return RT_ERR_FAILED; */}

#define gpioID				uint32

gpioID	smi_SCK;        /* GPIO used for SMI Clock Generation */
gpioID	smi_SDA;        /* GPIO used for SMI Data signal */
gpioID	smi_RST;	/* GPIO used for reset swtich */


#define ack_timer			5
#define max_register			0x018A 

// EDX jerry

//EDX shakim
#if defined (RT6855A_ASIC_BOARD)
#define _HW_SWITCH_CLOCK_		0
#define _HW_SWITCH_DATA_		1
#define _HW_SWITCH_RESET_		12 
//EDX shakim
#else
#ifdef REALTEK_SW_R_VB
#define _HW_SWITCH_CLOCK_		2
#define _HW_SWITCH_DATA_		1
#else
#define _HW_SWITCH_CLOCK_		4
#define _HW_SWITCH_DATA_		5
#endif //REALTEK_SW_R_VB
#define _HW_SWITCH_RESET_		0               //GPIO24 = GPIO24 + 0
#endif

void _rtl865x_setGpioDataBit (uint32 gpio, uint16 value)
{
	if (value){
//EDX shakim
#if defined (RT6855A_ASIC_BOARD) 
		RALINK_REG(RT2880_REG_PIO_0_31_DATA) |= cpu_to_le32(1 << gpio);	
//EDX shakim
#else
		if (gpio == _HW_SWITCH_RESET_)
			RALINK_REG(RT2880_REG_PIO_24_39_DATA) |= cpu_to_le32(1 << gpio);
		else
			RALINK_REG(RT2880_REG_PIO_0_23_DATA) |= cpu_to_le32(1 << gpio);
#endif
	}else{
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)
		RALINK_REG(RT2880_REG_PIO_0_31_DATA) &= ~(cpu_to_le32(1 << gpio));
//EDX shakim
#else
		if (gpio == _HW_SWITCH_RESET_)
			RALINK_REG(RT2880_REG_PIO_24_39_DATA) &= ~ (cpu_to_le32(1 << gpio));
		else
			RALINK_REG(RT2880_REG_PIO_0_23_DATA) &= ~(cpu_to_le32(1 << gpio));
#endif
	}
#if 0
	if (gpio == _HW_SWITCH_RESET_)
		if (value)
			printf ("(%s %d) GPIO 24= %08x, value = %d\n", __FUNCTION__, __LINE__, RALINK_REG(RT2880_REG_PIO_24_39_DATA) & cpu_to_le32(1 << gpio), value);
		else
			printf ("(%s %d) GPIO 24= %08x, value = %d\n", __FUNCTION__, __LINE__, RALINK_REG(RT2880_REG_PIO_24_39_DATA) & cpu_to_le32(1 << gpio), value);
	else
		if (value)
			printf ("(%s %d) GPIO %d= %08x, value = %d\n", __FUNCTION__, __LINE__, gpio, RALINK_REG(RT2880_REG_PIO_0_23_DATA) | cpu_to_le32(1 << gpio), value);
		else
			printf ("(%s %d) GPIO %d= %08x, value = %d\n", __FUNCTION__, __LINE__, gpio, RALINK_REG(RT2880_REG_PIO_0_23_DATA) & (~ cpu_to_le32(1 << gpio)), value);
#endif
}

void _rtl865x_getGpioDataBit(gpioID gpio, uint32 *u)
{
//	printf ("(%s %d)RT2880_REG_PIO_0_23_DATA = %08x gpio = %08x RT2880_REG_PIO_0_23_DATA = %08x *u = %d \n", __FUNCTION__, __LINE__, RALINK_REG(RT2880_REG_PIO_0_23_DATA), (cpu_to_le32(1 << gpio)), RALINK_REG(RT2880_REG_PIO_0_23_DATA) & (cpu_to_le32(1 << gpio)), *u);
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)  
	if (RALINK_REG(RT2880_REG_PIO_0_31_DATA) & (cpu_to_le32(1 << gpio)))
//EDX shakim
#else
	if (RALINK_REG(RT2880_REG_PIO_0_23_DATA) & (cpu_to_le32(1 << gpio)))
#endif
		*u = 1;
	else
		*u = 0;
#if 0
	printf ("(%s %d) GPIO %d= %08x, value = %d\n", __FUNCTION__, __LINE__, gpio, RALINK_REG(RT2880_REG_PIO_0_23_DATA) & (~ cpu_to_le32(1 << gpio)), *u);
#endif
}
// EDX jerry

void _smi_start(void)
{

#if 0	//EDX jerry
	/* change GPIO pin to Output only */
	_rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
	_rtl865x_initGpioPin(smi_SCK, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
//EDX jerry
#else
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)  
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_DATA_));	//Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_CLOCK_));	//Output mode(SCK)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_DATA_);	//Output enable mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_CLOCK_);	//Output enable mode(SCK)
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) |= cpu_to_le32(1 << _HW_SWITCH_DATA_);	//Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) |= cpu_to_le32(1 << _HW_SWITCH_CLOCK_);	//Output mode(SCK)
#endif
#endif
//EDX jerry

	/* Initial state: SCK: 0, SDA: 1 */

	_rtl865x_setGpioDataBit(smi_SCK, 0);
	_rtl865x_setGpioDataBit(smi_SDA, 1);
	CLK_DURATION(DELAY);

	/* CLK 1: 0 -> 1, 1 -> 0 */
	_rtl865x_setGpioDataBit(smi_SCK, 1);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 0);
	CLK_DURATION(DELAY);

	/* CLK 2: */
	_rtl865x_setGpioDataBit(smi_SCK, 1);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SDA, 0);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 0);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SDA, 1);
}

void _smi_writeBit(uint16 signal, uint32 bitLen)
{
    for( ; bitLen > 0; bitLen--)
    {
        CLK_DURATION(DELAY);

        /* prepare data */
        if ( signal & (1<<(bitLen-1)) ) 
            _rtl865x_setGpioDataBit(smi_SDA, 1);    
        else 
            _rtl865x_setGpioDataBit(smi_SDA, 0);    
        CLK_DURATION(DELAY);
//	printf ("(%s %d) smi_SDA write %d \n", __FUNCTION__, __LINE__, signal & (1<<(bitLen-1)) );

        /* clocking */
        _rtl865x_setGpioDataBit(smi_SCK, 1);
        CLK_DURATION(DELAY);
        _rtl865x_setGpioDataBit(smi_SCK, 0);
    }
}



void _smi_readBit(uint32 bitLen, uint32 *rData) 
{
	uint32 u;

#if 0 //EDX jerry
	/* change GPIO pin to Input only */
	_rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);
//EDX jerry
#else
//EDX shakim
#if defined (RT6855A_ASIC_BOARD) 
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) &= ~(cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_DATA_))); //Input mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN)  &= ~(cpu_to_le32(1 << _HW_SWITCH_DATA_));	//Output disable mode(SDA)
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_DATA_));	//Input mode(SDA)
#endif
#endif
//EDX jerry


#if 0
	uint32 i;
	for (i = 0; i < 200; i++){
		CLK_DURATION(DELAY);
	        _rtl865x_setGpioDataBit(smi_SCK, 1);
	        CLK_DURATION(DELAY);
        	_rtl865x_getGpioDataBit(smi_SDA, &u);
	        _rtl865x_setGpioDataBit(smi_SCK, 0);
	}
#else
    for (*rData = 0; bitLen > 0; bitLen--)
    {
        CLK_DURATION(DELAY);

        /* clocking */
        _rtl865x_setGpioDataBit(smi_SCK, 1);
        CLK_DURATION(DELAY);
        _rtl865x_getGpioDataBit(smi_SDA, &u);
        _rtl865x_setGpioDataBit(smi_SCK, 0);

        *rData |= (u << (bitLen - 1));
    }
#endif


#if 0	//EDX jerry
	/* change GPIO pin to Output only */
	_rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
//EDX jerry
#else
//EDX shakim
#if defined (RT6855A_ASIC_BOARD) 
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_DATA_)); //Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_DATA_); //Output enable mode(SDA)
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) |= cpu_to_le32(1 << _HW_SWITCH_DATA_);	//Output mode(SDA)
#endif
#endif
//EDX jerry
}



void _smi_stop(void)
{
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SDA, 0);
	_rtl865x_setGpioDataBit(smi_SCK, 1);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SDA, 1);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 1);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 0);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 1);

	/* add a click */
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 0);
	CLK_DURATION(DELAY);
	_rtl865x_setGpioDataBit(smi_SCK, 1);

#if 0 //EDX jerry
	/* change GPIO pin to Output only */
	_rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);
	_rtl865x_initGpioPin(smi_SCK, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);
//EDX jerry
#else
//EDX shakim
#if defined (RT6855A_ASIC_BOARD) 
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) &= ~(cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_DATA_)));   //Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) &= ~(cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_CLOCK_)));  //Output mode(SCK)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN)  &= ~(cpu_to_le32(1 << _HW_SWITCH_DATA_));	//Output disable mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN)  &= ~(cpu_to_le32(1 << _HW_SWITCH_CLOCK_));	//Output disable mode(SCK)	
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_DATA_));    //Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_CLOCK_));    //Output mode(SCK)
#endif //EDX shakim
#endif
//EDX jerry


}


//int32 smi_reset(uint32 port, uint32 pinRST)
int32 smi_reset()
{
	gpioID gpioId;
	int32 res;

#if 0 //EDX jerry
	printf ("(%s %d)Check Point\n", __FUNCTION__, __LINE__);
    /* Initialize GPIO port A, pin 7 as SMI RESET */
    gpioId = GPIO_ID(port, pinRST);
    res = _rtl865x_initGpioPin(gpioId, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
    if (res != RT_ERR_OK)
        return res;
    smi_RST = gpioId;
//EDX jerry
#else
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_RESET_));    //Output mode(Reset)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_RESET_);	//Output enable mode(Reset)
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_24_39_DIR) |= cpu_to_le32(1 << _HW_SWITCH_RESET_);    //Output mode(Reset)
#endif
#endif
//EDX jerry

	_rtl865x_setGpioDataBit(smi_RST, 1);
//EDX shakim	CLK_DURATION(10000000);
	CLK_DURATION(RST_DELAY); //EDX shakim
	_rtl865x_setGpioDataBit(smi_RST, 0);
//EDX shakim	CLK_DURATION(10000000);
	CLK_DURATION(RST_DELAY); //EDX shakim
	_rtl865x_setGpioDataBit(smi_RST, 1);
//EDX shakim	CLK_DURATION(10000000);
	CLK_DURATION(RST_DELAY); //EDX shakim

#if 0	//EDX jerry
	/* change GPIO pin to Input only */
	_rtl865x_initGpioPin(smi_RST, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);
//EDX jerry
#else
//	RALINK_REG(RT2880_REG_PIO_24_39_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_RESET_));    //Input mode(Reset)
#endif 
//EDX jerry

	return RT_ERR_OK;
}

//int32 smi_init(uint32 port, uint32 pinSCK, uint32 pinSDA)
int32 smi_init()
{
#if 0	//EDX jerry
    gpioID gpioId;
    int32 res;

    /* change GPIO pin to Input only */
    /* Initialize GPIO port C, pin 0 as SMI SDA0 */
    gpioId = GPIO_ID(port, pinSDA);
    res = _rtl865x_initGpioPin(gpioId, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
    if (res != RT_ERR_OK)
        return res;
    smi_SDA = gpioId;


    /* Initialize GPIO port C, pin 1 as SMI SCK0 */
    gpioId = GPIO_ID(port, pinSCK);
    res = _rtl865x_initGpioPin(gpioId, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
    if (res != RT_ERR_OK)
        return res;
    smi_SCK = gpioId;
#endif	//EDX jerry

// EDX jerry
//	printf ("************** Init GPIO Pin Start **************\n");
	//config these pins to gpio mode
//	printf ("(%s %d)RALINK_REG_GPIOMODE = %08x Init!\n", __FUNCTION__, __LINE__, RALINK_REG(RALINK_REG_GPIOMODE));	// RALINK_REG_GPIOMODE should be 5E


#ifndef RT6855A_ASIC_BOARD //EDX shakim
	RALINK_REG(RT2880_GPIOMODE_REG) |= RALINK_GPIOMODE_UARTF;
#endif //EDX shakim
//	printf ("(%s %d)RALINK_REG_GPIOMODE = %08x Init!\n", __FUNCTION__, __LINE__, RALINK_REG(RALINK_REG_GPIOMODE));	// RALINK_REG_GPIOMODE should be 5E

        // Output mode
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_CLOCK_)); //Output mode(SCK)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) |= cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_DATA_));	 //Output mode(SDA)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_CLOCK_);		 //Output enable mode(SCK)
	RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN) |= cpu_to_le32(1 << _HW_SWITCH_DATA_);			 //Output enable mode(SDA)
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) |= cpu_to_le32(1 << _HW_SWITCH_CLOCK_);	//Output mode(SCK)
	RALINK_REG(RT2880_REG_PIO_0_23_DIR) |= cpu_to_le32(1 << _HW_SWITCH_DATA_);	//Output mode(SDA)
#endif
//	RALINK_REG(RT2880_REG_PIO_24_39_DIR) |= cpu_to_le32(1 << _HW_SWITCH_RESET_);	//Output mode(Reset)

//	RALINK_REG(RT2880_REG_PIO_0_23_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_CLOCK_));	//Input mode(SCK)
//	RALINK_REG(RT2880_REG_PIO_0_23_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_DATA_));	//Input mode(SDA)
//EDX shakim
#if defined (RT6855A_ASIC_BOARD)
	RALINK_REG(RT2880_REG_PIO_0_15_DIR) &= ~(cpu_to_le32(1 << PIO_DIR_BIT(_HW_SWITCH_RESET_)));	//Input mode(Reset)
		RALINK_REG(RT2880_REG_PIO_0_31_OUTPUTEN)  &= ~(cpu_to_le32(1 << _HW_SWITCH_RESET_));	//Output disable mode(Reset)	
//EDX shakim
#else
	RALINK_REG(RT2880_REG_PIO_24_39_DIR) &= ~(cpu_to_le32(1 << _HW_SWITCH_RESET_));	//Input mode(Reset)
#endif

	smi_SCK = _HW_SWITCH_CLOCK_;
	smi_SDA = _HW_SWITCH_DATA_;
	smi_RST = _HW_SWITCH_RESET_;
// EDX jerry

	_rtl865x_setGpioDataBit(smi_SDA, 1);
	_rtl865x_setGpioDataBit(smi_SCK, 1);    
	return RT_ERR_OK;
}

int32 smi_read(uint32 mAddrs, uint32 *rData)
{
#ifdef MDC_MDIO_OPERATION

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

    /* Write address to register 23 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_ADDRESS_REG, mAddrs);

    /* Write read control code to register 21 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);

    /* Read data from register 25 */
    MDC_MDIO_READ(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_DATA_READ_REG, rData);

    return SUCCESS;
#else
    uint32 rawData=0, ACK;
    uint8  con;
    uint32 ret = RT_ERR_OK;
/*
    if ((mAddrs > max_register) || (rData == NULL))  return    RT_ERR_FAILED;
*/

    /*Disable CPU interrupt to ensure that the SMI operation is atomic. 
      The API is based on RTL865X, rewrite the API if porting to other platform.*/
//EDX jerry       rtlglue_drvMutexLock();

    _smi_start();                                /* Start SMI */

    _smi_writeBit(0x0b, 4);                     /* CTRL code: 4'b1011 for RTL8370 */

    _smi_writeBit(0x4, 3);                        /* CTRL code: 3'b100 */

    _smi_writeBit(0x1, 1);                        /* 1: issue READ command */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for issuing READ command*/
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs&0xff), 8);             /* Set reg_addr[7:0] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[7:0] */    
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs>>8), 8);                 /* Set reg_addr[15:8] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK by RTL8369 */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_readBit(8, &rawData);                    /* Read DATA [7:0] */
    *rData = rawData&0xff; 

    _smi_writeBit(0x00, 1);                        /* ACK by CPU */

    _smi_readBit(8, &rawData);                    /* Read DATA [15: 8] */

    _smi_writeBit(0x01, 1);                        /* ACK by CPU */
    *rData |= (rawData<<8);

    _smi_stop();

//EDX jerry    rtlglue_drvMutexUnlock();/*enable CPU interrupt*/

    return ret;
#endif /* end of #ifdef MDC_MDIO_OPEARTION */
}



int32 smi_write(uint32 mAddrs, uint32 rData)
{
#ifdef MDC_MDIO_OPERATION

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

    /* Write address to register 23 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_ADDRESS_REG, mAddrs);

    /* Write data to register 24 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_DATA_WRITE_REG, rData);

    /* Write data control code to register 21 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_DUMMY_ID, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);

    return SUCCESS;
#else

/*
    if ((mAddrs > 0x018A) || (rData > 0xFFFF))  return    RT_ERR_FAILED;
*/
    int8 con;
    uint32 ACK;
    uint32 ret = RT_ERR_OK;    

    /*Disable CPU interrupt to ensure that the SMI operation is atomic. 
      The API is based on RTL865X, rewrite the API if porting to other platform.*/
//EDX jerry       rtlglue_drvMutexLock();

    _smi_start();                                /* Start SMI */

    _smi_writeBit(0x0b, 4);                     /* CTRL code: 4'b1011 for RTL8370*/

    _smi_writeBit(0x4, 3);                        /* CTRL code: 3'b100 */

    _smi_writeBit(0x0, 1);                        /* 0: issue WRITE command */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for issuing WRITE command*/
    } while ((ACK != 0) && (con < ack_timer));

	if (ACK != 0)
		return RT_ERR_FAILED;
	
    _smi_writeBit((mAddrs&0xff), 8);             /* Set reg_addr[7:0] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs>>8), 8);                 /* Set reg_addr[15:8] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit(rData&0xff, 8);                /* Write Data [7:0] out */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for writting data [7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit(rData>>8, 8);                    /* Write Data [15:8] out */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                        /* ACK for writting data [15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_stop();    

//EDX jerry    rtlglue_drvMutexUnlock();/*enable CPU interrupt*/
    
    return ret;
#endif /* end of #ifdef MDC_MDIO_OPEARTION */
}
