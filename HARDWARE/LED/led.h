#ifndef __LED_H
#define __LED_H

#include "sys.h"
#define LEDR PBout(5)
#define LEDB PBout(1)
#define LEDG PBout(0)

void LED_Init(void);
void LED_ALLOFF(void);
void LED_ALLON(void);

#define LED_G GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#define LED_R GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED_B GPIO_SetBits(GPIOB,GPIO_Pin_8) //BEEP
#define LED_W LED_ALLON()
#define LED_OFF LED_ALLOFF()

#endif

