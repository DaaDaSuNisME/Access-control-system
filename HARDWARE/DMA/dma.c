#include "dma.h"

volatile int CurrDataCountEnd=1;
const uint32_t BufferSrc[12]={0x12312313,0x12312333,0x12343533,0x12334533,
0x00000001,0x00000002,0x00000003,0x00000004,
0x00000005,0x00000006,0x00000007,0x12345678};
uint32_t BufferDst[12];
uint32_t DMA_Rece_Buf[1024];

void DMA1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef DMA_NVIC_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Channel6);
	
	DMA_InitStructure.DMA_BufferSize=12;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)BufferSrc;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)BufferDst;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;
	
	DMA_NVIC_Structure.NVIC_IRQChannel=DMA1_Channel6_IRQn;//��ΪDMAѡ���ͨ����CHannel6
	DMA_NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	DMA_NVIC_Structure.NVIC_IRQChannelPreemptionPriority=1;
	DMA_NVIC_Structure.NVIC_IRQChannelSubPriority=3;
	
	NVIC_Init(&DMA_NVIC_Structure);
	
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
	
	
	DMA_Cmd(DMA1_Channel6,ENABLE);
	
}

void DMAusart_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Channel5);
	
	DMA_InitStructure.DMA_BufferSize=BufferMaxSize;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)Usart.DMA_Rece_Buf;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//��Ϊԭ��ַΪ���ڣ����ڵ����ݿ��Ϊ8
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel5,ENABLE);
	/*   
	����ʹ��DMA���еĴ��ڽ������ݣ���ý�DMA��USARTһͬ���г�ʼ��������ʹ�õ���USART�����߿���IDLE�ж�
	����DMA�Ķ��壬����Ҫ�����κ�DMA���жϺ�NVIC��
	*/
}


//void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
//{ 
//    DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1��ָʾ��ͨ��    
//    DMA_SetCurrDataCounter(DMA_CHx,1024);//DMAͨ����DMA����Ĵ�С
//    DMA_Cmd(DMA_CHx, ENABLE);  //��USART1 TX DMA1��ָʾ��ͨ��  
//} 
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1��ָʾ��ͨ��    
    DMA_SetCurrDataCounter(DMA_CHx,1024);//DMAͨ����DMA����Ĵ�С
    DMA_Cmd(DMA_CHx, ENABLE);  //��USART1 TX DMA1��ָʾ��ͨ��  
} 

