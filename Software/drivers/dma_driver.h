#ifndef DMA_DRIVER_H
#define DMA_DRIVER_H

#include <stdint.h>

void memcpy ( void * dest, const void * src, uint32_t n);
// void memset ( void *dest, uint8_t val, uint32_t n);
void memset ( void *dest, uint32_t val, uint32_t n);


#endif