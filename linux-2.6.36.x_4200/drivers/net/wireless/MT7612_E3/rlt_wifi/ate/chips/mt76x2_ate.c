/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************


	Module Name:
	mt7662_ate.c

	Abstract:
	Specific ATE funcitons and variables for MT7662

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifdef MT76x2


#include "rt_config.h"

VOID mt76x2_ate_set_tx_rx(
    IN PRTMP_ADAPTER ad,
    IN CHAR tx,
    IN CHAR rx);


extern RTMP_REG_PAIR mt76x2_mac_g_band_cr_table[];
extern UCHAR mt76x2_mac_g_band_cr_nums;
extern RTMP_REG_PAIR mt76x2_mac_a_band_cr_table[];
extern UCHAR mt76x2_mac_a_band_cr_nums;

static void mt76x2_ate_switch_channel(RTMP_ADAPTER *ad)
{
	PATE_INFO pATEInfo = &(ad->ate);
	unsigned int latch_band, band, bw, tx_rx_setting;
	UINT32 ret, i, value, value1, restore_value, loop = 0;
	UCHAR bbp_ch_idx = 0;
	BOOLEAN band_change = FALSE;
	u8 channel = 0;

	SYNC_CHANNEL_WITH_QA(pATEInfo, &channel);


	/* determine channel flags */
	if (channel > 14)
		band = _A_BAND;
	else
		band = _G_BAND;
	
	if (!ad->MCUCtrl.power_on) {
		band_change = TRUE;
	} else {
		if (ad->LatchRfRegs.Channel > 14)
			latch_band = _A_BAND;
		else
			latch_band = _G_BAND;

		if (band != latch_band)
			band_change = TRUE;
		else
			band_change = FALSE;
	}
	
	if (ad->ate.TxWI.TXWI_N.BW == BW_80)
		bw = 2;
	else if (ad->ate.TxWI.TXWI_N.BW == BW_40)
		bw = 1;
	else
		bw = 0;

/*
	if ((ad->CommonCfg.TxStream == 1) && (ad->CommonCfg.RxStream == 1))
		tx_rx_setting = 0x101;
	else if ((ad->CommonCfg.TxStream == 2) && (ad->CommonCfg.RxStream == 1))
		tx_rx_setting = 0x201;
	else if ((ad->CommonCfg.TxStream == 1) && (ad->CommonCfg.RxStream == 2))
		tx_rx_setting = 0x102;
	else if ((ad->CommonCfg.TxStream == 2) && (ad->CommonCfg.RxStream == 2))
		tx_rx_setting = 0x202;
	else 
*/
		tx_rx_setting = 0x202;
		

#ifdef RTMP_PCI_SUPPORT
	/* mac setting per band */
	if (IS_PCI_INF(ad)) {
		if (band_change) {
			if (band == _G_BAND) {
				for(i = 0; i < mt76x2_mac_g_band_cr_nums; i++) {
					RTMP_IO_WRITE32(ad, mt76x2_mac_g_band_cr_table[i].Register,
									mt76x2_mac_g_band_cr_table[i].Value);
				}
			} else {
				for(i = 0; i < mt76x2_mac_a_band_cr_nums; i++) {
					RTMP_IO_WRITE32(ad, mt76x2_mac_a_band_cr_table[i].Register,
										mt76x2_mac_a_band_cr_table[i].Value);
				}
			}
		}
	}
#endif


	/* Fine tune tx power ramp on time based on BBP Tx delay */
	if (bw == 0)
		RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x00101001);
	else
		RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x000B0B01);


	/* tx pwr gain setting */
	//mt76x2_tx_pwr_gain(ad, channel, bw);

	/* per-rate power delta */
	mt76x2_adjust_per_rate_pwr_delta(ad, channel);

	andes_switch_channel(ad, channel, FALSE, bw, tx_rx_setting, bbp_ch_idx);

	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		/* LDPC RX */
		RTMP_BBP_IO_READ32(ad, 0x2934, &value);
		value |= (1 << 10);
		RTMP_BBP_IO_WRITE32(ad, 0x2934, value);

		/* Disable 64QAM/16QAM/QPSK CPE_d estimation */
		RTMP_BBP_IO_READ32(ad, 0x2950, &value);
		value |= (1 << 12);
		RTMP_BBP_IO_WRITE32(ad, 0x2950, value);
	}

	/* fine tune PD threshold for channel mode E */
	RTMP_BBP_IO_WRITE32(ad, 0x2394, 0x1010161C);

	/* channel smoothing threshold */
	RTMP_BBP_IO_READ32(ad, 0x2948, &value);
	value &= ~(0x1);
	value |= (0x1);
	RTMP_BBP_IO_WRITE32(ad, 0x2948, value);
	
	RTMP_BBP_IO_WRITE32(ad, 0x294C, 0xB9CB9FF9);

	mt76x2_ate_set_tx_rx(ad, 0, 0);

	/* backup mac 1004 value */
	RTMP_IO_READ32(ad, 0x1004, &restore_value);
	
	/* Backup the original RTS retry count and then set to 0 */
	RTMP_IO_READ32(ad, 0x1344, &ad->rts_tx_retry_num);

	/* disable mac tx/rx */
	value = restore_value;
	value &= ~0xC;
	RTMP_IO_WRITE32(ad, 0x1004, value);

	/* set RTS retry count = 0 */	
	RTMP_IO_WRITE32(ad, 0x1344, 0x00092B00);

	/* wait mac 0x1200, bbp 0x2130 idle */
	do {
		RTMP_IO_READ32(ad, 0x1200, &value);
		value &= 0x1;
		RTMP_BBP_IO_READ32(ad, 0x2130, &value1);
		DBGPRINT(RT_DEBUG_OFF,("%s:: Wait until MAC 0x1200 bit0 and BBP 0x2130 become 0\n", __FUNCTION__));
		RtmpusecDelay(1);
		loop++;
	} while (((value != 0) || (value1 != 0)) && (loop < 300));

	/* RXDCOC calibration */	
	CHIP_CALIBRATION(ad, RXDCOC_CALIBRATION_7662, channel);
#ifdef RTMP_PCI_SUPPORT
	RtmpOsMsDelay(10);
#endif

	if (MT_REV_LT(ad, MT76x2, REV_MT76x2E3)) {
		RTMP_IO_WRITE32(ad, 0x2308, 0xFFFF);
		RTMP_IO_WRITE32(ad, 0x2cb8, 0xc);
	}

	mt76x2_calibration(ad, channel);

	/* enable TX/RX */
	RTMP_IO_WRITE32(ad, 0x1004, 0xc);

	/* Restore RTS retry count */
	RTMP_IO_WRITE32(ad, 0x1344, ad->rts_tx_retry_num);		


#ifdef RTMP_PCI_SUPPORT
	if(IS_PCI_INF(ad)) {
		NdisAcquireSpinLock(&ad->tssi_lock);
	}
#endif

	/* TSSI Clibration */
	if ( (ad->chipCap.tssi_enable) && (pATEInfo->bAutoTxAlc) ) {
		ad->chipCap.tssi_stage = TSSI_CAL_STAGE;
		if (channel > 14) {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0101);
			else if (ad->chipCap.PAType == EXT_PA_5G_ONLY)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0101);
			else
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0001); 
		} else {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0100);
			else if ((ad->chipCap.PAType == EXT_PA_5G_ONLY) ||
					(ad->chipCap.PAType == INT_PA_2G_5G))
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0000);
			else if (ad->chipCap.PAType == EXT_PA_2G_ONLY)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0100);
			 else
				DBGPRINT(RT_DEBUG_ERROR, ("illegal PA Type(%d)\n", ad->chipCap.PAType));
		}
		ad->chipCap.tssi_stage = TSSI_TRIGGER_STAGE;
	}
	

#ifdef RTMP_PCI_SUPPORT
	if (IS_PCI_INF(ad)) {
		NdisReleaseSpinLock(&ad->tssi_lock);
	}
#endif


	/* Channel latch */
	ad->LatchRfRegs.Channel = channel;
	
	if (!ad->MCUCtrl.power_on)
		ad->MCUCtrl.power_on = TRUE;


	ATEAsicSetTxRxPath(ad);

	ATE_CHIP_RX_VGA_GAIN_INIT(ad);

	DBGPRINT(RT_DEBUG_OFF,
			("%s(): Switch to Ch#%d(%dT%dR), BBP_BW=%d\n",
			__FUNCTION__,
			channel,
			2,
			2,
			bw));
}


INT mt76x2_ate_tx_pwr_handler(
	IN RTMP_ADAPTER *ad,
	IN char index)
{
	PATE_INFO pATEInfo = &(ad->ate);
	char TxPower = 0;
	u32 value;

#ifdef RALINK_QA
	if ((pATEInfo->bQATxStart == TRUE) || (pATEInfo->bQARxStart == TRUE))
	{
		return 0;
	}
	else
#endif /* RALINK_QA */
	if (index == 0)
	{
		TxPower = pATEInfo->TxPower0;

		/* TX0 channel initial transmission gain setting */
		RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
		value = value & (~TX_ALC_CFG_0_CH_INT_0_MASK);
		value |= TX_ALC_CFG_0_CH_INT_0(TxPower);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_0, value);
	}
	else if (index == 1)
	{
		TxPower = pATEInfo->TxPower1;

		/* TX1 channel initial transmission gain setting */
		RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
		value = value & (~TX_ALC_CFG_0_CH_INT_1_MASK);
		value |= TX_ALC_CFG_0_CH_INT_1(TxPower);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_0, value);
	}
	else
	{
		DBGPRINT_ERR(("%s : Only TxPower0 and TxPower1 are adjustable !\n", __FUNCTION__));
		DBGPRINT_ERR(("%s : TxPower%d is out of range !\n", __FUNCTION__, index));
		return -1;
	}

	RTMP_IO_READ32(ad, TX_ALC_CFG_1, &value);
	value &= ~(0x3F);
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_1, value);

	RTMP_IO_READ32(ad, TX_ALC_CFG_2, &value);
	value &= ~(0x3F);
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_2, value);

	DBGPRINT(RT_DEBUG_TRACE, ("%s : (TxPower%d=%d)\n", __FUNCTION__, index, TxPower));
	
	return 0;
}	


VOID mt76x2_ate_rx_vga_init(
	IN PRTMP_ADAPTER		ad)
{
#ifdef DYNAMIC_VGA_SUPPORT
	UINT32 bbp_val;

	RTMP_BBP_IO_READ32(ad, AGC1_R8, &bbp_val);
	bbp_val = (bbp_val & 0xffff80ff) | (ad->CommonCfg.lna_vga_ctl.agc_vga_init_0 << 8);
	RTMP_BBP_IO_WRITE32(ad, AGC1_R8, bbp_val);

	//if (ad->CommonCfg.RxStream >= 2) {
	RTMP_BBP_IO_READ32(ad, AGC1_R9, &bbp_val);
	bbp_val = (bbp_val & 0xffff80ff) | (ad->CommonCfg.lna_vga_ctl.agc_vga_init_1 << 8);
	RTMP_BBP_IO_WRITE32(ad, AGC1_R9, bbp_val);
	//}
#endif /* DYNAMIC_VGA_SUPPORT */
}


VOID mt76x2_ate_set_tx_rx(
    IN PRTMP_ADAPTER ad,
    IN CHAR tx,
    IN CHAR rx)
{
	PATE_INFO   pATEInfo = &(ad->ate);
	u32 BbpValue = 0;
	u32 Value = 0 , TxPinCfg = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("%s : tx=%d, rx=%d\n", __FUNCTION__, tx, rx));

	/* store the original value of TX_PIN_CFG */
	RTMP_IO_READ32(ad, TX_PIN_CFG, &Value);

	pATEInfo->Default_TX_PIN_CFG = Value;
	Value &= ~0xF;

	switch (pATEInfo->TxAntennaSel)
	{
		case 0: /* both TX0/TX1 */
			TxPinCfg = Value | 0x0F;
			break;
		case 1: /* TX0 */
			TxPinCfg = Value | 0x03;
			break;
		case 2: /* TX1 */
			TxPinCfg = Value | 0x0C;
			break;
	}
	RTMP_IO_WRITE32(ad, TX_PIN_CFG, TxPinCfg);


	/* set TX path, pAd->TxAntennaSel : 0 -> All, 1 -> TX0, 2 -> TX1 */
	switch (ad->Antenna.field.TxPath)
	{
		case 2:
			switch (tx)
			{
				case 1: /* DAC 0 */
					/* bypass MAC control DAC selection */
					RTMP_BBP_IO_READ32(ad, IBI_R9, &BbpValue);
					BbpValue &= 0xFFFFF7FF; /* 0x2124[11]=0 */
					RTMP_BBP_IO_WRITE32(ad, IBI_R9, BbpValue);

					/* 0x2714[7:0]=0 (default) */
					RTMP_BBP_IO_READ32(ad, TXBE_R5, &BbpValue);
					BbpValue &= 0xFFFFFF00;;
					RTMP_BBP_IO_WRITE32(ad, TXBE_R5, BbpValue);

					/* 0x2080[21:20]=2'b10 */
					RTMP_BBP_IO_READ32(ad, CORE_R32, &BbpValue);
					BbpValue &= 0xFFCFFFFF;
					BbpValue |= 0x00200000;
					RTMP_BBP_IO_WRITE32(ad, CORE_R32, BbpValue);

					/* use manul mode for dac1 clock control: 
					    0x2084[11]=1,set dac1 clock enable =0 0x2084[11]=0 (default already) */
					RTMP_BBP_IO_READ32(ad, CORE_R33, &BbpValue);
					BbpValue &= 0xFFFFE1FF;
					BbpValue |= 0x800;
					RTMP_BBP_IO_WRITE32(ad, CORE_R33, BbpValue);

					break;
				case 2: /* DAC 1 */
					/* bypass MAC control DAC selection */
					RTMP_BBP_IO_READ32(ad, IBI_R9, &BbpValue);
					BbpValue &= 0xFFFFF7FF; /* 0x2124[11]=0 */
					RTMP_BBP_IO_WRITE32(ad, IBI_R9, BbpValue);

					/* 0x2714[7:0]=81 (force direct DAC0 to DAC1) */
					RTMP_BBP_IO_READ32(ad, TXBE_R5, &BbpValue);
					BbpValue &= 0xFFFFFF00;
					BbpValue |= 0x00000001;
					RTMP_BBP_IO_WRITE32(ad, TXBE_R5, BbpValue);

					/* 0x2080[21:20]=2'b01 */
					RTMP_BBP_IO_READ32(ad, CORE_R32, &BbpValue);
					BbpValue &= 0xFFCFFFFF;
					BbpValue |= 0x00100000;
					RTMP_BBP_IO_WRITE32(ad, CORE_R32, BbpValue);

					/* use manul mode for dac0 clock control:
					    0x2084[9]=1,set dac0 clock enable =0 0x2084[10]=0 (default already) */
					RTMP_BBP_IO_READ32(ad, CORE_R33, &BbpValue);
					BbpValue &= 0xFFFFE1FF;
					BbpValue |= 0x00000200;
					RTMP_BBP_IO_WRITE32(ad, CORE_R33, BbpValue);

					break;
				default: /* all DACs */
					/* bypass MAC control DAC selection */
					RTMP_BBP_IO_READ32(ad, IBI_R9, &BbpValue);
					BbpValue &= 0xFFFFF7FF; /* 0x2124[11]=0 */
					RTMP_BBP_IO_WRITE32(ad, IBI_R9, BbpValue);

					/* 0x2714[7:0]=0 (default) */
					RTMP_BBP_IO_READ32(ad, TXBE_R5, &BbpValue);
					BbpValue &= 0xFFFFFF00;
					BbpValue |= 0x00000083;
					RTMP_BBP_IO_WRITE32(ad, TXBE_R5, BbpValue);

					/* 0x2080[21:20]=0 */
					RTMP_BBP_IO_READ32(ad, CORE_R32, &BbpValue);
					BbpValue &= 0xFFCFFFFF;
					RTMP_BBP_IO_WRITE32(ad, CORE_R32, BbpValue);

					/* 0x2084[12:9]=0 */
					RTMP_BBP_IO_READ32(ad, CORE_R33, &BbpValue);
					BbpValue &= 0xFFFFE1FF;
					RTMP_BBP_IO_WRITE32(ad, CORE_R33, BbpValue);

					break;                                              
			}
			break;

		default:
			break;
	}

	switch (ad->Antenna.field.RxPath)
	{
		case 1: /* 1R */
		case 2: /* 2R */
			switch (rx)
			{
				case 1:
					// Check One Rx path, so set AGC1_OFFSET+R0 bit[4:3] = 0:0 => use 1R
					RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
					BbpValue &= 0xFFFFFFE7;				//clear bit 4:3, set bit 4:3 = 0:0
					RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);

					// Set the AGC1_OFFSET+R0 bit[1:0] = 0 :: ADC0
					RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
					BbpValue &= 0xFFFFFFFC;				//clear bit 1:0, set bit[1:0] = 0:0
					RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);
					break;
					
				case 2:
                			// Check One Rx path, so set AGC1_OFFSET+R0 bit[4:3] = 0:0 => use 1R
                			RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
            				BbpValue &= 0xFFFFFFE7;				//clear bit 4:3, set bit 4:3 = 0:0
            				RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);

                			// Set the AGC1_OFFSET+R0 bit[1:0] according to RXANT_NUM : 2R
                			RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
					BbpValue &= 0xFFFFFFFC;				//clear bit 1:0
					BbpValue |= 0x00000001;				//set bit 1:0 = 0:1
					RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);
					break;
				default:
					// Check One Rx path, so set AGC1_OFFSET+R0 bit[4:3] = 0:1 => use 2R
					RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
					BbpValue &= 0xFFFFFFE7; //clear bit 4:3
					BbpValue |= 0x00000008; //set bit 4:3 = 0:1
					RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);

					// Set the AGC1_OFFSET+R0 bit[1:0] = 3 :: All ADCs
					RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
					BbpValue &= 0xFFFFFFFC; //clear bit 1:0, set bit[1:0] = 0:0
					BbpValue |= 0x00000003; //set bit[1:0] = 3
					RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);

					if ( ad->Antenna.field.RxPath == 1 ) // 1R
					{
						// 1R AGC1_OFFSET+R0 bit[4:3:1:0]=0:0:0:0
						RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
						BbpValue &= 0xFFFFFFE4; //clear bit 4:3:1:0, set bit 4:3:1:0 = 0:0:0:0
						RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);
					}         
					else if ( ad->Antenna.field.RxPath == 2 ) // 2R
					{
						// 2R AGC1_OFFSET+R0 bit[4:3:1:0]=0:1:0:0
						RTMP_BBP_IO_READ32(ad, AGC1_R0, &BbpValue);
						BbpValue &= 0xFFFFFFE4; //clear bit 4:3:1:0
						BbpValue |= 0x00000008; //set bit 4:3:1:0 = 0:1:0:0
						RTMP_BBP_IO_WRITE32(ad, AGC1_R0, BbpValue);
					}
	            			break;
			}
			break;

		default:
		    break;              
	}

}

VOID mt76x2_ate_set_tx_rx_path(
    IN PRTMP_ADAPTER ad)
{
	PATE_INFO   pATEInfo = &(ad->ate);

	DBGPRINT(RT_DEBUG_TRACE, ("%s : Mode = %d\n", __FUNCTION__, pATEInfo->Mode));
	mt76x2_ate_set_tx_rx(ad, pATEInfo->TxAntennaSel, pATEInfo->RxAntennaSel);
}


INT	mt76x2_set_ate_tx_bw_proc(
	IN	PRTMP_ADAPTER	ad, 
	IN	PSTRING			arg)
{
	u32 core, core_r1 = 0, core_r4 = 0;
	u32 agc, agc_r0 = 0;
	u32 ret;
	u8 BBPCurrentBW;
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;

	
	BBPCurrentBW = simple_strtol(arg, 0, 10);

	if ((BBPCurrentBW == 0))
	{
		ad->ate.TxWI.TXWI_N.BW= BW_20;
	}
	else if ((BBPCurrentBW == 1))
	{
		ad->ate.TxWI.TXWI_N.BW = BW_40;
 	}
	else if ((BBPCurrentBW == 2))
	{
		ad->ate.TxWI.TXWI_N.BW = BW_80;
 	}
	else if ((BBPCurrentBW == 4))
	{
		ad->ate.TxWI.TXWI_N.BW = BW_10;
 	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_BW_Proc!! Error!! Unknow bw.\n"));
		return TRUE;
	}


	if ((ad->ate.TxWI.TXWI_N.PHYMODE == MODE_CCK) && (ad->ate.TxWI.TXWI_N.BW != BW_20))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_BW_Proc!! Warning!! CCK only supports 20MHZ!!\nBandwidth switch to 20\n"));
		ad->ate.TxWI.TXWI_N.BW = BW_20;
	}

	if ( (ad->ate.TxWI.TXWI_N.PHYMODE == MODE_VHT) && (ad->ate.TxWI.TXWI_N.BW == BW_20) )
	{
		if ((ad->ate.TxWI.TXWI_N.MCS == 9) ||(ad->ate.TxWI.TXWI_N.MCS == 25))
		{
			DBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_BW_Proc!! Warning!! VHT MCS9 not support Bandwidth switch to 20\n"));
			ad->ate.TxWI.TXWI_N.BW = BW_40;
		}
	}


	RTMP_BBP_IO_READ32(ad, CORE_R1, &core_r1);
		core = (core_r1 & (~0x18));

	RTMP_BBP_IO_READ32(ad, AGC1_R0, &agc_r0);
	agc = agc_r0 & (~0x7000);

	switch (ad->ate.TxWI.TXWI_N.BW )
	{
		case BW_80:
			core |= 0x18;
			agc |= 0x7000;
			break;
		case BW_40:
			core |= 0x10;
			agc |= 0x3000;
			break;
		case BW_20:
			core &= (~0x18);
			agc |= 0x1000;
			break;
		case BW_10:
			core |= 0x08;
			agc |= 0x1000;
			break;
	}

	if (core != core_r1) 
	{
		if (IS_MT76x0(ad))
		{
			if (ad->ate.TxWI.TXWI_N.BW == BW_80)
				core |= 0x40;
			/*
				Hold BBP in reset by setting CORE_R4[0]=1
			*/
			RTMP_BBP_IO_READ32(ad, CORE_R4, &core_r4);
			core_r4 |= 0x00000001;
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, core_r4);

			/*
				Wait 0.5 us to ensure BBP is in the idle state.
			*/
			RtmpusecDelay(10);
		}
	
		RTMP_BBP_IO_WRITE32(ad, CORE_R1, core);

		if (IS_MT76x0(ad))
		{
			/*
				Wait 0.5 us for BBP clocks to settle.
			*/
			RtmpusecDelay(10);

			/*
				Release BBP from reset by clearing CORE_R4[0].
			*/
			RTMP_BBP_IO_READ32(ad, CORE_R4, &core_r4);
			core_r4 &= ~(0x00000001);
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, core_r4);
		}
	}

	if (agc != agc_r0) {
		RTMP_BBP_IO_WRITE32(ad, AGC1_R0, agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): bw=%d, Set AGC1_R0=0x%x, agc_r0=0x%x\n", __FUNCTION__, bw, agc, agc_r0));
//		RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc);
//DBGPRINT(RT_DEBUG_OFF, ("%s(): bw=%d, After write, Get AGC1_R0=0x%x,\n", __FUNCTION__, bw, agc));
	}


	DBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_BW_Proc (BBPCurrentBW = %d)\n", ad->ate.TxWI.TXWI_N.BW));
	DBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_BW_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */
	
	return TRUE;
}	


VOID mt76x2_ate_asic_adjust_tx_power(
	IN PRTMP_ADAPTER pAd) 
{
	RTMP_CHIP_CAP *cap = &pAd->chipCap;
	ANDES_CALIBRATION_PARAM param;
	UINT32 pa_mode, tssi_slope_offset;
	UINT32 ret;


	if ((pAd->chipCap.tssi_enable) &&
			(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF | 
				fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET) == FALSE)) {
		//mt76x2_tssi_compensation(pAd, pAd->ate.Channel);


#ifdef RTMP_PCI_SUPPORT
		if(IS_PCI_INF(pAd)) {
			NdisAcquireSpinLock(&pAd->tssi_lock);
		}
#endif

		if (pAd->ate.Channel > 14) {
			if (pAd->chipCap.PAType == EXT_PA_2G_5G)
				pa_mode = 1;
			else if (pAd->chipCap.PAType == EXT_PA_5G_ONLY)
				pa_mode = 1;
			else
				pa_mode = 0;
		} else {
			if (pAd->chipCap.PAType == EXT_PA_2G_5G)
				pa_mode = 1;
			else if ((pAd->chipCap.PAType == EXT_PA_5G_ONLY) ||
					(pAd->chipCap.PAType == INT_PA_2G_5G))
				pa_mode = 0;
			else if (pAd->chipCap.PAType == EXT_PA_2G_ONLY)
				pa_mode = 1;
		} 
	
		if (pAd->ate.Channel < 14) {
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_g_band);
		} else {
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_a_band[get_chl_grp(pAd->ate.Channel )]);
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_a_band[get_chl_grp(pAd->ate.Channel )]);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_a_band[get_chl_grp(pAd->ate.Channel )]);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_a_band[get_chl_grp(pAd->ate.Channel )]);
		}

		param.mt76x2_tssi_comp_param.pa_mode = pa_mode;
		param.mt76x2_tssi_comp_param.tssi_slope_offset = tssi_slope_offset;

		/* TSSI Compensation */
		if(pAd->chipOps.Calibration != NULL)
			pAd->chipOps.Calibration(pAd, TSSI_COMPENSATION_7662, &param);

#ifdef RTMP_PCI_SUPPORT
		if (IS_PCI_INF(pAd)) {
			NdisReleaseSpinLock(&pAd->tssi_lock);
		}
#endif


	}

}


INT	mt76x2_set_ate_tx_freq_offset_proc(
	IN	PRTMP_ADAPTER	ad, 
	IN	PSTRING			arg)
{
	u32 freq_offset = 0;
	u32 value = 0;
	u32 misc_ctrl = 0;
	u8 count = 0;

	freq_offset = simple_strtol(arg, 0, 10);
	freq_offset &= 0xFF;
	ad->ate.RFFreqOffset = freq_offset;

	/* Set crystal trim1 */
	read_reg(ad, 0x40, XO_CTRL5, &value);
	value &= 0xffff80ff;
	value |= ((freq_offset & XTAL_TRIM1_MASK) << 8);
	write_reg(ad, 0x40, XO_CTRL5, value);

	/* Enable */
	read_reg(ad, 0x40, XO_CTRL6, &value);
	value &= 0xffff80ff;
	value |= (0x7f << 8);
	write_reg(ad, 0x40, XO_CTRL6, value);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_FREQOFFSET_Proc (RFFreqOffset = %d)\n", ad->ate.RFFreqOffset));
	DBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_FREQOFFSET_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */
	
	return TRUE;
}


VOID mt76x2_ate_asic_calibration(
	IN PRTMP_ADAPTER pAd, UCHAR ate_mode)
{

	DBGPRINT(RT_DEBUG_TRACE, ("%s: channel=%d ate_mode=0x%x\n", __FUNCTION__, pAd->ate.Channel, ate_mode));
	UCHAR channel = pAd->ate.Channel;

	switch ( ate_mode )
	{
		case ATE_STOP:
			if (MT_REV_LT(pAd, MT76x2, REV_MT76x2E3)) {
				RTMP_IO_WRITE32(pAd, 0x200C , 0x0700030A);
				RTMP_IO_WRITE32(pAd, 0x2394 , 0x2121262C);
			}
			break;
		case ATE_START:
			break;
		case ATE_RXFRAME:
			if (MT_REV_LT(pAd, MT76x2, REV_MT76x2E3)) {
				RTMP_IO_WRITE32(pAd, 0x200C , 0x0600030A);
				RTMP_IO_WRITE32(pAd, 0x2394 , 0x1010161C);
			}
			break;
		case ATE_TXFRAME:
			break;
		default:
			break;
	}

}


VOID mt76x2_ate_do_calibration(
	IN PRTMP_ADAPTER pAd, UINT32 cal_id, UINT32 param)
{

	switch (cal_id )
	{
		case SX_LOGEN_CALIBRATION_7662:
			/* SX Calibration */
			CHIP_CALIBRATION(pAd, SX_LOGEN_CALIBRATION_7662, param);
	
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(10);
#endif
		break;

		case LC_CALIBRATION_7662:
			/* LC Calibration */
			CHIP_CALIBRATION(pAd, LC_CALIBRATION_7662, param);
	
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(10);
#endif
		break;

		case TX_LOFT_CALIBRATION_7662:
			/* TX LOFT */
			CHIP_CALIBRATION(pAd, TX_LOFT_CALIBRATION_7662, param);

#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(140);
#endif
			break;
		case TXIQ_CALIBRATION_7662:
			/* TXIQ Clibration */
			CHIP_CALIBRATION(pAd, TXIQ_CALIBRATION_7662, param);
			
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(220);
#endif
			break;
		case DPD_CALIBRATION_7662:
			if (IS_PCI_INF(pAd)) {
				/* DPD Calibration */
				CHIP_CALIBRATION(pAd, DPD_CALIBRATION_7662, param);
			}
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(15);
#endif
			break;
		case RXIQC_FI_CALIBRATION_7662:
			/* RXIQC-FI */
			CHIP_CALIBRATION(pAd, RXIQC_FI_CALIBRATION_7662, param);
			break;

		case RXIQC_FD_CALIBRATION_7662:
			/* RXIQC-FI */
			CHIP_CALIBRATION(pAd, RXIQC_FD_CALIBRATION_7662, param);
			break;

		case RXDCOC_CALIBRATION_7662:
			/* RXDCOC calibration */	
			CHIP_CALIBRATION(pAd, RXDCOC_CALIBRATION_7662, param);
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(5);
#endif
			break;
		case RC_CALIBRATION_7662:
		/* RX LPF calibration */
			CHIP_CALIBRATION(pAd, RC_CALIBRATION_7662, param);

#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(35);
#endif
			break;
		default:
		break;
	}

}


struct _ATE_CHIP_STRUCT mt76x2ate =
{
	/* functions */
	.ChannelSwitch = mt76x2_ate_switch_channel,
	.TxPwrHandler = mt76x2_ate_tx_pwr_handler,
	.TssiCalibration = NULL,
	.ExtendedTssiCalibration = NULL /* RT5572_ATETssiCalibrationExtend */,
	.RxVGAInit = mt76x2_ate_rx_vga_init,
	.AsicSetTxRxPath = mt76x2_ate_set_tx_rx_path,
	.AdjustTxPower = mt76x2_ate_asic_adjust_tx_power,
	//.AsicExtraPowerOverMAC = DefaultATEAsicExtraPowerOverMAC,
	.Set_BW_Proc = mt76x2_set_ate_tx_bw_proc,
	.Set_FREQ_OFFSET_Proc = mt76x2_set_ate_tx_freq_offset_proc,
	.AsicCalibration = mt76x2_ate_asic_calibration,

	/* variables */
	.maxTxPwrCnt = 5,
	.bBBPStoreTXCARR = FALSE,
	.bBBPStoreTXCARRSUPP = FALSE,	
	.bBBPStoreTXCONT = FALSE,
	.bBBPLoadATESTOP = FALSE,/* ralink debug */
};

#endif /* MT76x2 */





