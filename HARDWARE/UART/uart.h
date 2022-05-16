#ifndef __UART_H
#define __UART_H
#include "main.h"

#define DMA_espSRC USART->DR 
#define DMA_espDST espMonmery;

void Uart_esp8266Config(void);
void Esp_SendChar(char *ch,uint8_t len);
void USART1_SendChar(char *ch,uint8_t len);
#endif
