#include "esp8266.h"

struct BUFFER Esp={0};
struct BUFFER Usart={0};

char IDnum[]="137A8E85GC2L40A0";
char Heart[]="Q";
volatile int heartnumber=0; //FOR Server
volatile int check=0;
char *ledCmd[7] = { "红","绿","蓝","白","关","开","锁" };
int flagR=0;  
int flagG=0;
int flagB=0;
int extiFlag=0;
volatile int flagDoor=0;
int flagD=0;
	
void Esp8266_GpioInit(void);
void Esp_Init(void)
	
{
		Esp8266_GpioInit();
		Uart_esp8266Config();
		ESP8266_RST_HIGH_LEVEL();
		ESP8266_CH_ENABLE();//把CH和RST全部拉高开启ESP8266模块
}
	
void Esp8266_GpioInit(void)
{
	GPIO_InitTypeDef Esp_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	Esp_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	Esp_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_15;
	Esp_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&Esp_InitStructure);
}
//void USART3_SendChar(uint16_t ch)
//{
//	USART_SendData(USART3,ch);
//	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET);
//}


void Espconnected(void)
{
	char RST[]="AT+RST\r\n";
	char CWMODE[]="AT+CWMODE=3\r\n";
	char CWJAP[]="AT+CWJAP=\"ZDSNDM\",\"qwer12345\"\r\n";
	char CIPMUX[]="AT+CIPMUX=0\r\n";
	char TCPLINK[]="AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n";
	char CIPSEND1[]="AT+CIPMODE=1\r\n";
	char CIPSEND2[]="AT+CIPSEND\r\n";
	printf("正在配置服务器连接到云端......\r\n");
	printf("正在配置服务器连接到云端......\r\n\n\n\n\n\n");
	printf("请等待20s......\r\n\n\n\n");
	delay_ms(1500);
	Esp_SendChar(CWMODE,13);
	delay_ms(1500);
	Esp_SendChar(RST,8);
	delay_ms(1500);delay_ms(1500);
	Esp_SendChar(CWJAP,31);
	delay_ms(1500);delay_ms(1500);delay_ms(1500);delay_ms(1500);
	Esp_SendChar(CIPMUX,13);
	delay_ms(1500);
	Esp_SendChar(TCPLINK,40);
	delay_ms(1500);delay_ms(1500);delay_ms(1500);
	Esp_SendChar(CIPSEND1,14);
	delay_ms(1500);
	Esp_SendChar(CIPSEND2,12);
	delay_ms(1500);	
	Esp_SendChar(IDnum,16);
	delay_ms(1500);delay_ms(1500);
	Esp_SendChar(Heart,1);
	delay_ms(1500);	

}



void Get_ESP82666_Cmd( char* cmd )
{
	uint8_t i;
	for(i = 0;i < 7; i++)
	{
     if( (_Bool)strstr( cmd, ledCmd[i] ))
		 break;
	}
	switch(i)
    {
      case 0:
        LED_R;
      break;
      case 1:
        LED_G;
      break;
      case 2:
        LED_B;
      break;
      case 3:
        LED_W;
      break;
      case 4:
        LED_OFF;
      break;
			case 5:
			//ESP8266_SendString ( ENABLE, "DOOR OPEND", 8, Multiple_ID_0 );
			{printf("DOOR open");
				TIM_SetCompare2(TIM3,25);
        flagDoor=1;}
      break;
			case 6:
			//ESP8266_SendString ( ENABLE, "DOOR CLOSED", 8, Multiple_ID_0 );
			{printf("DOOR closed");
				TIM_SetCompare2(TIM3,5);
				flagDoor=0;}
      break;
      default:
        
        break;      
    }   
}
