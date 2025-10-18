#include "dma_driver.h"
#include <rp2350/dma.h>
#include <rp2350/resets.h>
#include <stdint.h>

void memcpy ( void *dest, const void *src, uint32_t n)
{
    resets.reset_clr = RESETS_RESET_DMA_MASK;

    while( !(resets.reset_done & RESETS_RESET_DMA_MASK))
        continue;

    dma.ch0_read_addr = (uint32_t) src;
    dma.ch0_write_addr = (uint32_t) dest;
    dma.ch0_trans_count = DMA_CH0_TRANS_COUNT_MODE(0)
                         |DMA_CH0_TRANS_COUNT_COUNT(n);

    dma.ch0_ctrl_trig = 
        DMA_CH0_CTRL_TRIG_AHB_ERROR(1)
        |DMA_CH0_CTRL_TRIG_READ_ERROR(1)
        |DMA_CH0_CTRL_TRIG_WRITE_ERROR(1)
        |DMA_CH0_CTRL_TRIG_SNIFF_EN(0)
        |DMA_CH0_CTRL_TRIG_BSWAP(0)
        |DMA_CH0_CTRL_TRIG_IRQ_QUIET(0)
        |DMA_CH0_CTRL_TRIG_TREQ_SEL(0x3f)
        |DMA_CH0_CTRL_TRIG_CHAIN_TO(0)
        |DMA_CH0_CTRL_TRIG_RING_SEL(0)
        |DMA_CH0_CTRL_TRIG_RING_SIZE(0)
        |DMA_CH0_CTRL_TRIG_INCR_WRITE_REV(0)
        |DMA_CH0_CTRL_TRIG_INCR_WRITE(1)
        |DMA_CH0_CTRL_TRIG_INCR_READ_REV(0)
        |DMA_CH0_CTRL_TRIG_INCR_READ(1)
        |DMA_CH0_CTRL_TRIG_DATA_SIZE(0)
        |DMA_CH0_CTRL_TRIG_HIGH_PRIORITY(0)
        |DMA_CH0_CTRL_TRIG_EN(1);
}

void memset ( void *dest, uint32_t val, uint32_t n)
{
    uint8_t temp = (uint8_t) val;
    resets.reset_clr = RESETS_RESET_DMA_MASK;

    while( !(resets.reset_done & RESETS_RESET_DMA_MASK))
        continue;

    dma.ch2_read_addr = (uint32_t) &temp;
    dma.ch2_write_addr = (uint32_t) dest;
    dma.ch2_trans_count = DMA_CH0_TRANS_COUNT_MODE(0)
                         |DMA_CH0_TRANS_COUNT_COUNT(n);

    dma.ch2_ctrl_trig = 
        DMA_CH0_CTRL_TRIG_AHB_ERROR(1)
        |DMA_CH0_CTRL_TRIG_READ_ERROR(1)
        |DMA_CH0_CTRL_TRIG_WRITE_ERROR(1)
        |DMA_CH0_CTRL_TRIG_SNIFF_EN(0)
        |DMA_CH0_CTRL_TRIG_BSWAP(0)
        |DMA_CH0_CTRL_TRIG_IRQ_QUIET(0)
        |DMA_CH0_CTRL_TRIG_TREQ_SEL(0x3f)
        |DMA_CH0_CTRL_TRIG_CHAIN_TO(0)
        |DMA_CH0_CTRL_TRIG_RING_SEL(0)
        |DMA_CH0_CTRL_TRIG_RING_SIZE(0)
        |DMA_CH0_CTRL_TRIG_INCR_WRITE_REV(0)
        |DMA_CH0_CTRL_TRIG_INCR_WRITE(1)
        |DMA_CH0_CTRL_TRIG_INCR_READ_REV(0)
        |DMA_CH0_CTRL_TRIG_INCR_READ(0)
        |DMA_CH0_CTRL_TRIG_DATA_SIZE(0)
        |DMA_CH0_CTRL_TRIG_HIGH_PRIORITY(0)
        |DMA_CH0_CTRL_TRIG_EN(1);
}