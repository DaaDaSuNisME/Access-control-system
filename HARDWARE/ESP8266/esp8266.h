#ifndef __ESP_H
#define __ESP_H
#include "main.h"

extern char IDnum[];
extern char Heart[];

extern struct BUFFER{
	char DMA_Rece_Buf[1024];
	uint8_t U1RevCnt;
}Usart;
extern struct BUFFER Esp;

extern volatile int heartnumber;
extern volatile int check;

extern char *ledCmd[7];
extern int flagR;  
extern int flagG;
extern int flagB;
extern int extiFlag;
extern volatile int flagDoor;
extern int flagD;


#define     ESP8266_CH_ENABLE()                 GPIO_SetBits (GPIOA, GPIO_Pin_4)
#define     ESP8266_CH_DISABLE()                GPIO_ResetBits (GPIOA, GPIO_Pin_4)

#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits (GPIOA, GPIO_Pin_15)
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits (GPIOA, GPIO_Pin_15)



void Esp_Init(void);
void Esp8266_GpioInit(void);
void Espconnected(void);
void Get_ESP82666_Cmd( char * cmd );
#endif
