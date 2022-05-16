#include "uart.h"

void Uart_esp8266Config(void)
{
	USART_InitTypeDef UsartComConfigStrusture;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef espNvicConfigStructure;
	GPIO_InitTypeDef esp8266GpioConfigStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	USART_DeInit(USART3);   //在初始化之前最好都进行DeInit以免设备之前被占用已经被初始化了的问题
	DMA_DeInit(DMA1_Channel3);
	
	esp8266GpioConfigStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	esp8266GpioConfigStructure.GPIO_Pin=GPIO_Pin_10;
	esp8266GpioConfigStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&esp8266GpioConfigStructure);
	
	esp8266GpioConfigStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	esp8266GpioConfigStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&esp8266GpioConfigStructure);
	
	espNvicConfigStructure.NVIC_IRQChannel=USART3_IRQn;
	espNvicConfigStructure.NVIC_IRQChannelCmd=ENABLE;
	espNvicConfigStructure.NVIC_IRQChannelPreemptionPriority=3;
	espNvicConfigStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&espNvicConfigStructure);
	
	UsartComConfigStrusture.USART_BaudRate=115200;
	UsartComConfigStrusture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UsartComConfigStrusture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	UsartComConfigStrusture.USART_Parity=USART_Parity_No;
	UsartComConfigStrusture.USART_StopBits=USART_StopBits_1;
	UsartComConfigStrusture.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&UsartComConfigStrusture);
	

	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART3,ENABLE);


	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_BufferSize=BufferMaxSize;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&USART3->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)Esp.DMA_Rece_Buf;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//因为原地址为串口，串口的数据宽度为8
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel3,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel3,ENABLE);
	
}


void Esp_SendChar(char *ch,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++){
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//当没有发送完数据时，一直在while循环，直至数据发送完成
	USART_SendData(USART3,ch[t]);
	}while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//数据发送完成退出while循环
}

void USART1_SendChar(char *ch,uint8_t len)
{
	uint8_t t;
	for(t=0;t<len;t++){
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//当没有发送完数据时，一直在while循环，直至数据发送完成
	USART_SendData(USART1,ch[t]);
	}while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//数据发送完成退出while循环
}

