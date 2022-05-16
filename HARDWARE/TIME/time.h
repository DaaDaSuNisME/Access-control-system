#ifndef __TIME_H
#define __TIME_H
#include "main.h"

extern int OC1pul;
extern int OC2pul;
extern int OC3pul;
extern int OC4pul;

extern float percent1;
extern float percent2;
extern float percent3;
extern float percent4;

void TIM3_Int_Init(u16, u16);
void simplePwmTIMx_InitandCmd(int timx,int mseconds,int ocx,float percent);
void simplePwmTIMx_InitandCmd2(TIM_TypeDef* TIMx,int mseconds,int ocx,float percent);
void TIM2ocConfig(int psc,int arr);
void TIM4_1sConfig(void);
void TIM3ocConfig(void);
void TIM5_100msConfig(void);
#endif


