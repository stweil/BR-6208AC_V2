/*
 * mtk_audio_pcm.c
 *
 *  Created on: 2013/9/6
 *      Author: MTK04880
 */

#include <linux/init.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
#include <linux/sched.h>
#endif
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <sound/core.h>
#include <linux/pci.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include "drivers/char/ralink_gdma.h"
#include "mtk_audio_driver.h"

dma_addr_t i2s_txdma_addr, i2s_rxdma_addr;
dma_addr_t i2s_mmap_addr[MAX_I2S_PAGE*2];


static int mtk_audio_pcm_open(struct snd_pcm_substream *substream);
static int mtk_pcm_new(struct snd_card *card,\
	struct snd_soc_dai *dai, struct snd_pcm *pcm);
static void mtk_pcm_free(struct snd_pcm *pcm);
static int mtk_audio_pcm_close(struct snd_pcm_substream *substream);
static snd_pcm_uframes_t mtk_audio_pcm_pointer(struct snd_pcm_substream *substream);
static int mtk_audio_pcm_trigger(struct snd_pcm_substream *substream, int cmd);
static int mtk_audio_pcm_prepare(struct snd_pcm_substream *substream);
static int mtk_audio_pcm_hw_params(struct snd_pcm_substream *substream,\
				 struct snd_pcm_hw_params *hw_params);
static int mtk_audio_pcm_copy(struct snd_pcm_substream *substream, int channel,\
		snd_pcm_uframes_t pos,void __user *buf, snd_pcm_uframes_t count);
static int mtk_audio_pcm_hw_free(struct snd_pcm_substream *substream);

static const struct snd_pcm_hardware mtk_audio_hwparam = {
	.info			= SNDRV_PCM_INFO_INTERLEAVED |SNDRV_PCM_INFO_PAUSE,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE,
	.period_bytes_min	= I2S_PAGE_SIZE,
	.period_bytes_max	= I2S_PAGE_SIZE,
	.periods_min		= 2,
	.periods_max		= 1024,
	.buffer_bytes_max	= MAX_I2S_PAGE*I2S_PAGE_SIZE,
};

static struct snd_pcm_ops mtk_pcm_ops = {

	.open = 	mtk_audio_pcm_open,
	.ioctl = 	snd_pcm_lib_ioctl,
	.hw_params = mtk_audio_pcm_hw_params,
	.hw_free = 	mtk_audio_pcm_hw_free,
	.trigger =	mtk_audio_pcm_trigger,
	.prepare = 	mtk_audio_pcm_prepare,
	.pointer = 	mtk_audio_pcm_pointer,
	.close = 	mtk_audio_pcm_close,
	.copy = mtk_audio_pcm_copy,
};
struct snd_soc_platform mtk_soc_platform = {
	.name		= "mtk-dma",
	.pcm_ops	= &mtk_pcm_ops,
	.pcm_new	= mtk_pcm_new,
	.pcm_free	= mtk_pcm_free,
};

static int mtk_audio_pcm_close(struct snd_pcm_substream *substream){
	return 0;
}

static snd_pcm_uframes_t mtk_audio_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	i2s_config_type* rtd = runtime->private_data;
	unsigned int offset = 0;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		offset = bytes_to_frames(runtime, I2S_PAGE_SIZE*rtd->tx_r_idx);
	}
	else{
		offset = bytes_to_frames(runtime, I2S_PAGE_SIZE*rtd->rx_w_idx);
	}
	return offset;
}


static int gdma_ctrl_start(struct snd_pcm_substream *substream){

	struct snd_pcm_runtime *runtime= substream->runtime;
	i2s_config_type* rtd = runtime->private_data;

	//printk("%s:%d \n",__func__,__LINE__);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
#if defined(I2S_FIFO_MODE)
#else
	GdmaI2sTx((u32) rtd->pPage0TxBuf8ptr, I2S_FIFO_WREG, 0, I2S_PAGE_SIZE, i2s_dma_tx_handler, i2s_unmask_handler);
	GdmaI2sTx((u32) rtd->pPage1TxBuf8ptr, I2S_FIFO_WREG, 1, I2S_PAGE_SIZE, i2s_dma_tx_handler, i2s_unmask_handler);
#endif

#if defined(I2S_FIFO_MODE)
#else
	GdmaUnMaskChannel(GDMA_I2S_TX0);
#endif
		rtd->bTxDMAEnable = 1;
	}
	else{
#if defined(I2S_FIFO_MODE)
#else
		GdmaI2sRx(I2S_RX_FIFO_RREG, (u32)rtd->pPage0RxBuf8ptr, 0, I2S_PAGE_SIZE, i2s_dma_rx_handler, i2s_unmask_handler);
		GdmaI2sRx(I2S_RX_FIFO_RREG, (u32)rtd->pPage1RxBuf8ptr, 1, I2S_PAGE_SIZE, i2s_dma_rx_handler, i2s_unmask_handler);
#endif

#if defined(I2S_FIFO_MODE)
#else
		GdmaUnMaskChannel(GDMA_I2S_RX0);
#endif
		rtd->bRxDMAEnable = 1;
		return 0;
	}
	return 0;
}

static int gdma_ctrl_stop(struct snd_pcm_substream *substream){

	struct snd_pcm_runtime *runtime= substream->runtime;
	i2s_config_type* rtd = runtime->private_data;

	//printk("%s:%d \n",__func__,__LINE__);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		rtd->bTxDMAEnable = 0;
	}
	else{
		rtd->bRxDMAEnable = 0;
	}
	return 0;
}

static int mtk_audio_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;

	MSG("trigger cmd:%s\n",(cmd==SNDRV_PCM_TRIGGER_START)?"START":\
			(cmd==SNDRV_PCM_TRIGGER_RESUME)?"RESUME":\
					(cmd==SNDRV_PCM_TRIGGER_PAUSE_RELEASE)?"PAUSE_RELEASE":\
							(cmd==SNDRV_PCM_TRIGGER_STOP)?"STOP":\
									(cmd==SNDRV_PCM_TRIGGER_SUSPEND)?"SUSPEND":\
											(cmd==SNDRV_PCM_TRIGGER_PAUSE_PUSH)?"PAUSE_PUSH":"default");

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		ret = gdma_ctrl_start(substream);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		ret =gdma_ctrl_stop(substream);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int mtk_audio_pcm_copy(struct snd_pcm_substream *substream, int channel,\
		snd_pcm_uframes_t pos,void __user *buf, snd_pcm_uframes_t count){
	struct snd_pcm_runtime *runtime= substream->runtime;
	i2s_config_type* rtd = runtime->private_data;
	char *hwbuf = NULL;
	do{
		hwbuf = runtime->dma_area + frames_to_bytes(runtime, pos);
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
			//printk("%s:%d dma:%x hwoff:%d f2b:%d copy to user's frame:%d \n",__func__,__LINE__,(unsigned int)runtime->dma_area,pos,frames_to_bytes(runtime, pos),frames_to_bytes(runtime, count));
			if(((rtd->tx_w_idx+4)%MAX_I2S_PAGE)!=rtd->tx_r_idx){
				if (copy_from_user(hwbuf, buf, frames_to_bytes(runtime,count)))
					return -EFAULT;
				rtd->tx_w_idx = (rtd->tx_w_idx+1)%MAX_I2S_PAGE;
				break;
			}
			else{
				//printk("buf full w:%d r:%d\n",rtd->tx_w_idx,rtd->tx_r_idx);
				interruptible_sleep_on(&(rtd->i2s_tx_qh));
			}
		}
		else{
			if(rtd->rx_r_idx!=rtd->rx_w_idx)
			{
				if (copy_to_user(buf, hwbuf, frames_to_bytes(runtime, count)))
					return -EFAULT;
				rtd->rx_r_idx = (rtd->rx_r_idx+1)%MAX_I2S_PAGE;
				break;
			}
			else
			{
				//MSG("buf empty w:%d r:%d\n",rtd->rx_w_idx,rtd->rx_r_idx);
				interruptible_sleep_on(&(rtd->i2s_rx_qh));
			}
		}
	}while(1);

	return 0;
}

static int mtk_audio_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime;
	runtime = substream->runtime;
	//runtime->stop_threshold = runtime->buffer_size = MAX_I2S_PAGE*I2S_PAGE_SIZE;
	runtime->boundary = MAX_I2S_PAGE*I2S_PAGE_SIZE;
	gdma_ctrl_start(substream);

	return 0;
}


static int mtk_audio_pcm_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *hw_params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	i2s_config_type *rtd = (i2s_config_type*)runtime->private_data;
	int ret,i;
	ret = i = 0;
	//printk("%s %d \n",__func__,__LINE__);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		/* allocate tx buffer */
		rtd->pPage0TxBuf8ptr = (u8*)pci_alloc_consistent(NULL, I2S_PAGE_SIZE*2 , &i2s_txdma_addr);
		if( rtd->pPage0TxBuf8ptr==NULL){
			MSG("Allocate Tx Page Buffer Failed\n");
			return -1;
		}
		rtd->pPage1TxBuf8ptr =  rtd->pPage0TxBuf8ptr + I2S_PAGE_SIZE;
		for(i = 0;i< MAX_I2S_PAGE;i++)
			rtd->pMMAPTxBufPtr[i] =(char *)(buf->area +i*I2S_PAGE_SIZE);
	}
	else{
		/* allocate rx buffer */
		rtd->pPage0RxBuf8ptr = (u8*)pci_alloc_consistent(NULL, I2S_PAGE_SIZE*2 , &i2s_rxdma_addr);
		if(rtd->pPage0RxBuf8ptr==NULL)
		{
			MSG("Allocate Rx Page Buffer Failed\n");
			return -1;
		}
		rtd->pPage1RxBuf8ptr = rtd->pPage0RxBuf8ptr + I2S_PAGE_SIZE;
		for(i = 0;i< MAX_I2S_PAGE;i++)
			rtd->pMMAPRxBufPtr[i] =(char *)(buf->area +i*I2S_PAGE_SIZE);
	}

	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);

	return ret;
}

static int mtk_audio_pcm_hw_free(struct snd_pcm_substream *substream)
{
	i2s_config_type* rtd = (i2s_config_type*)substream->runtime->private_data;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK){
		if(rtd->bTxDMAEnable==1){
			rtd->bTxDMAEnable = 0;
			if (rtd->nTxDMAStopped<4)
				interruptible_sleep_on(&(rtd->i2s_tx_qh));
			pci_free_consistent(NULL, I2S_PAGE_SIZE*2,rtd->pPage0TxBuf8ptr, i2s_txdma_addr);
			rtd->pPage0TxBuf8ptr = NULL;
		}
	}
	else{
		if(rtd->bRxDMAEnable==1){
			rtd->bRxDMAEnable = 0;
			if(rtd->nRxDMAStopped<2)
				interruptible_sleep_on(&(rtd->i2s_rx_qh));
			pci_free_consistent(NULL, I2S_PAGE_SIZE*2,rtd->pPage0RxBuf8ptr, i2s_txdma_addr);
			rtd->pPage0RxBuf8ptr = NULL;
		}
	}
	snd_pcm_set_runtime_buffer(substream, NULL);
	return 0;
}

static int mtk_audio_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime= substream->runtime;
	int ret = 0;

	snd_soc_set_runtime_hwparams(substream, &mtk_audio_hwparam);
	/* ensure that buffer size is a multiple of period size */
	ret = snd_pcm_hw_constraint_integer(runtime,
						SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0)
		goto out;



 out:
	return ret;
	return 0;
}

static int mtk_pcm_preallocate_dma_buffer(struct snd_pcm *pcm,
	int stream)
{
#if 1
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size = I2S_PAGE_SIZE*MAX_I2S_PAGE;

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;
#if 1
	//printk("%s: size:%d \n",__func__,size);
	buf->area = dma_alloc_coherent(pcm->card->dev, size, &buf->addr, GFP_KERNEL);
	//buf->area = kmalloc(size, GFP_KERNEL);
#else
	for( i = 0 ; i <MAX_I2S_PAGE ; i ++ ){
		if(pAudio_config->pMMAPTxBufPtr[i]==NULL)
			pAudio_config->pMMAPTxBufPtr[i] = tmpPtr;
		tmpPtr += I2S_PAGE_SIZE;
	}

#endif
	if (!buf->area)
		return -ENOMEM;

	buf->bytes = size;
	//printk("myk-pcm: preallocate_dma_buffer: area=%p, addr=%p, size=%d\n",(void *) buf->area,\
			(void *) buf->addr,size);
#else
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size = I2S_PAGE_SIZE*MAX_I2S_PAGE;
#endif
	return 0;
}

static int mtk_pcm_new(struct snd_card *card,
	struct snd_soc_dai *dai, struct snd_pcm *pcm)
{
	int ret = 0;

	if (dai->playback.channels_min) {
		ret = mtk_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_PLAYBACK);
		if (ret)
			goto out;
	}

	if (dai->capture.channels_min) {
		ret = mtk_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_CAPTURE);
		if (ret)
			goto out;
	}

out:
	//printk("%s ret:%d\n",__func__,ret);
	return ret;;
}

static void mtk_pcm_free(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream;
	struct snd_dma_buffer *buf;
	int stream;
	//printk("%s:%d \n",__func__,__LINE__);
	for (stream = 0; stream < 2; stream++) {
		substream = pcm->streams[stream].substream;
		if (!substream)
			continue;

		buf = &substream->dma_buffer;
		if (!buf->area)
			continue;
		dma_free_coherent(pcm->card->dev, buf->bytes,buf->area, buf->addr);
		//kfree(buf->area);
		buf->area = NULL;
	}
}


static int __init mtk_audio_pcm_init(void)
{
	//printk("%s \n",__func__);
	return snd_soc_register_platform(&mtk_soc_platform);
}

static void __exit mtk_audio_pcm_exit(void)
{
	snd_soc_unregister_platform(&mtk_soc_platform);
}

EXPORT_SYMBOL_GPL(mtk_soc_platform);
module_init(mtk_audio_pcm_init);
module_exit(mtk_audio_pcm_exit);

MODULE_AUTHOR("Atsushi Nemoto <anemo@mba.ocn.ne.jp>");
MODULE_DESCRIPTION("TXx9 ACLC Audio DMA driver");
MODULE_LICENSE("GPL");

