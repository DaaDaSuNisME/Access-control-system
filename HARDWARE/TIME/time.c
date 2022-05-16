#include "time.h"
#include "led.h"

int OC1pul=2000;
int OC2pul=2000;
int OC3pul=4000;
int OC4pul=5000;

float percent1=0.3;
float percent2=0.8;
float percent3=0.5;
float percent4=0.5;

void TIM4_1sConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_ConfigStructure;
	NVIC_InitTypeDef NVICforTIM4Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); /*除了TIM1,8，其他的TIM定时器全部都是在APB1上的*/
	
	TIM_ConfigStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_ConfigStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_ConfigStructure.TIM_Prescaler=7199;
	TIM_ConfigStructure.TIM_Period=9999;
	TIM_TimeBaseInit(TIM4,&TIM_ConfigStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVICforTIM4Structure.NVIC_IRQChannel=TIM4_IRQn;
	NVICforTIM4Structure.NVIC_IRQChannelPreemptionPriority=0;
	NVICforTIM4Structure.NVIC_IRQChannelSubPriority=3;
	NVICforTIM4Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICforTIM4Structure);
	
	TIM_Cmd(TIM4,ENABLE);
}


void TIM5_100msConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_ConfigStructure;
	NVIC_InitTypeDef NVICforTIM4Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); /*除了TIM1,8，其他的TIM定时器全部都是在APB1上的*/
	
	TIM_ConfigStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_ConfigStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_ConfigStructure.TIM_Prescaler=7199;
	TIM_ConfigStructure.TIM_Period=9999;
	TIM_TimeBaseInit(TIM5,&TIM_ConfigStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	NVICforTIM4Structure.NVIC_IRQChannel=TIM5_IRQn;
	NVICforTIM4Structure.NVIC_IRQChannelPreemptionPriority=0;
	NVICforTIM4Structure.NVIC_IRQChannelSubPriority=3;
	NVICforTIM4Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICforTIM4Structure);
	
	TIM_Cmd(TIM5,ENABLE);
}

void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_Cmd(TIM3, ENABLE);
	
}



void simplePwmTIMx_InitandCmd2(TIM_TypeDef* TIMx,int mseconds,int ocx,float percent)
{
	TIM_TimeBaseInitTypeDef simplePwmTIMxstruct;
	TIM_OCInitTypeDef simplePWmOcStruct;
	
		if(TIMx==TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	else if (TIMx==TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	else if (TIMx==TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	else if (TIMx==TIM5)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	simplePwmTIMxstruct.TIM_CounterMode = TIM_CounterMode_Up;
	simplePwmTIMxstruct.TIM_Period = 10*mseconds-1;
	simplePwmTIMxstruct.TIM_Prescaler = 7200-1;
	
	simplePWmOcStruct.TIM_OCMode=TIM_OCMode_PWM1;
	simplePWmOcStruct.TIM_OutputState=TIM_OutputState_Enable;
	simplePWmOcStruct.TIM_Pulse=percent*10*mseconds-1;
	simplePWmOcStruct.TIM_OCPolarity=TIM_OCPolarity_High;

	TIM_TimeBaseInit(TIMx,&simplePwmTIMxstruct);
	
	switch(ocx)
	{
		case 1:{TIM_OC1Init(TIMx, &simplePWmOcStruct);TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);};break;
		case 2:{TIM_OC2Init(TIMx, &simplePWmOcStruct);TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);};break;
		case 3:{TIM_OC3Init(TIMx, &simplePWmOcStruct);TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);};break;
		case 4:{TIM_OC4Init(TIMx, &simplePWmOcStruct);TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);};break;
	}
	

	TIM_CtrlPWMOutputs(TIMx,ENABLE);
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	TIM_Cmd(TIMx,ENABLE);
	
}

void TIM2ocConfig(int psc,int arr)
{
	TIM_OCInitTypeDef simplePWmOcStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	simplePWmOcStruct.TIM_OCMode=TIM_OCMode_Toggle;
	simplePWmOcStruct.TIM_OutputState=TIM_OutputState_Enable;
	simplePWmOcStruct.TIM_Pulse=OC1pul-1;
	simplePWmOcStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &simplePWmOcStruct);
	
	simplePWmOcStruct.TIM_OutputState=TIM_OutputState_Enable;
	simplePWmOcStruct.TIM_Pulse=OC2pul-1;
	TIM_OC2Init(TIM2, &simplePWmOcStruct);
	
	simplePWmOcStruct.TIM_OutputState=TIM_OutputState_Enable;
	simplePWmOcStruct.TIM_Pulse=OC3pul-1;
	TIM_OC3Init(TIM2, &simplePWmOcStruct);
	
	simplePWmOcStruct.TIM_OutputState=TIM_OutputState_Enable;
	simplePWmOcStruct.TIM_Pulse=OC4pul-1;
	TIM_OC4Init(TIM2, &simplePWmOcStruct);
	
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);//输出比较模式输出占空比不同的PWM要手动装载，不进行预装载
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  /*    初始化Update中断之后定时器不运行，如法进入中断     */
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_Cmd(TIM2,ENABLE);
	
	
}

void TIM3ocConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①使能定时器 3 时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|
RCC_APB2Periph_AFIO, ENABLE); //①使能 GPIO 和 AFIO 复用功能时钟
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5 
	
//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
//初始化 TIM3
TIM_TimeBaseStructure.TIM_Period = 199; //设置在自动重装载周期值
TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置预分频值
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //③初始化 TIMx
//初始化 TIM3 Channel2 PWM 模式
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式 2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
TIM_OC2Init(TIM3, &TIM_OCInitStructure); //④初始化外设 TIM3 OC2
TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器
TIM_Cmd(TIM3, ENABLE);
	
}
