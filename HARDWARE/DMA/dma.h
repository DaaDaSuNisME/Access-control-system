#ifndef __DMA_H
#define __DMA_H
#include "main.h"

void DMA1_Config(void);
void DMAusart_Config(void);

extern volatile int CurrDataCountEnd;
extern const uint32_t BufferSrc[12];
extern uint32_t BufferDst[12];
extern uint32_t DMA_Rece_Buf[1024];
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
#endif
