#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "touch.h"	  
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usart2.h"
#include "AS608.h"
#include "timer.h"
#include "main.h"

//ALIENTEK ս��V3&��Ӣ STM32������ ��չʵ��14
//ATK-AS608ָ��ʶ��ģ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 

#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ���

SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чָ�Ƹ���
u8** kbd_tbl;
const  u8* kbd_menu[15]={"ɾָ��"," : ","¼ָ��","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//������
const  u8* kbd_delFR[15]={"����"," : ","���ָ��","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//������

void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��
void ShowErrMessage(u8 ensure);//��ʾȷ���������Ϣ
void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl);//�����������
u8  AS608_get_keynum(u16 x,u16 y);//��ȡ������
u16 GET_NUM(void);//��ȡ��ֵ
int checknum=0;
int flagR1=0;
int main(void)
{    
	u8 ensure;
	u8 key_num;
	char *str;	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  	//��ʼ����ʱ����
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	PS_StaGPIO_Init();	//��ʼ��FR��״̬����
	BEEP_Init();  			//��ʼ��������
	KEY_Init();					//������ʼ�� 
 	LCD_Init();					//LCD��ʼ�� 	
	W25QXX_Init();			//��ʼ��W25Q128
	tp_dev.init();			//��ʼ��������
	usmart_dev.init(72);		//��ʼ��USMART
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[1],"1:",1);  //����FLASH.
	
	delay_init();
	//TIM4_1sConfig();
	TIM5_100msConfig();//����ʱ��Ϊ1s���ӳ�
	LED_Init();
	Esp_Init();
	LED_Init();
	TIM3ocConfig();//Ϊ�������PWM
	
	//ADCr_Config();  //�Դ�����ADC����
	//TIM3_Int_Init(7199,9999); //ADC2 ����
	//TIM2ocConfig(7199,39999);//�Ƚ����4·PWM
	delay_ms(2000);
	Espconnected();
	
	
	POINT_COLOR=RED;
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ
		delay_ms(200);	
	}
	if(!(tp_dev.touchtype&0x80))//����ǵ�����
	{
		Show_Str_Mid(0,30,"�Ƿ���д�����У׼",16,240);
		POINT_COLOR=BLUE;
		Show_Str_Mid(0,60,"��:KEY2   ��:KEY0",16,240);	
		while(1)
		{
			key_num=KEY_Scan(0);
			if(key_num==KEY0_PRES)
				break;
			if(key_num==KEY2_PRES)
			{
				LCD_Clear(WHITE);
				TP_Adjust();  	 //��ĻУ׼ 
				TP_Save_Adjdata();//����У׼����
				break;				
			}
		}
	}
	/*����ָ��ʶ��ʵ�����*/
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	Show_Str_Mid(0,0,"AS608ָ��ʶ��ģ����Գ���",16,240);	    			    	 
	Show_Str_Mid(0,20,"����ԭ�� @ALIENTEK",16,240);				    	 
 	POINT_COLOR=BLUE;
	Show_Str_Mid(0,40,"��AS608ģ������....",16,240);	
	while(PS_HandShake(&AS608Addr))//��AS608ģ������
	{
		delay_ms(400);
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"δ��⵽ģ��!!!",16,240);
		delay_ms(800);
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"��������ģ��...",16,240);		  
	}
	LCD_Fill(30,40,240,100,WHITE);
	Show_Str_Mid(0,40,"ͨѶ�ɹ�!!!",16,240);
	str=mymalloc(SRAMIN,30);
	sprintf(str,"������:%d   ��ַ:%x",usart2_baund,AS608Addr);
	Show_Str(0,60,240,16,(u8*)str,16,0);
	ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	if(ensure!=0x00)
		ShowErrMessage(ensure);//��ʾȷ���������Ϣ	
	ensure=PS_ReadSysPara(&AS608Para);  //������ 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"������:%d     �Աȵȼ�: %d",AS608Para.PS_max-ValidN,AS608Para.PS_level);
		Show_Str(0,80,240,16,(u8*)str,16,0);
	}
	else
		ShowErrMessage(ensure);	
	myfree(SRAMIN,str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//�����������
	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)Del_FR();		//ɾָ��
			if(key_num==3)Add_FR();		//¼ָ��									
		}
		if(PS_Sta)	 //���PS_Sta״̬���������ָ����
		{
			press_FR();//ˢָ��			
		}	

		if(checknum>=3)
		{
			delay_ms(1000);
			if(flagR1==0){
			Esp_SendChar("#1,0,0,0#", 11);		flagR1++;}
		else if(flagR1!=0) {
			Esp_SendChar("#0,0,0,0#", 11);flagR1--;}
			LED_B;//��������
			checknum=0;
		}
			if(heartnumber>30)
		{
			heartnumber=0;	
			Esp_SendChar(Heart,1);		
		}
		if(GPIO_ReadOutputDataBit( GPIOB,GPIO_Pin_5)==RESET&&flagR==0){
			Esp_SendChar("#1,0,0,0#", 11);		flagR++;}
		else if(GPIO_ReadOutputDataBit( GPIOB,GPIO_Pin_5)==SET&&flagR!=0) {
			Esp_SendChar("#0,0,0,0#", 11);flagR--;}
			
			if(GPIO_ReadOutputDataBit( GPIOE,GPIO_Pin_5)==RESET&&flagG==0){
			Esp_SendChar("#0,1,0,0#", 11);		flagG++;}//'\0'ռ�����ֽڣ��س���'/n'Ҳռ�������ַ�
		else if(GPIO_ReadOutputDataBit( GPIOE,GPIO_Pin_5)==SET&&flagG!=0) {
			Esp_SendChar("#0,0,0,0#", 11);flagG--;}
			
			if(GPIO_ReadOutputDataBit( GPIOB,GPIO_Pin_8)==SET&&flagB==0){
			Esp_SendChar("#0,0,1,0#", 11);		flagB++;}
		else if(GPIO_ReadOutputDataBit( GPIOB,GPIO_Pin_8)==RESET&&flagB!=0) {
			Esp_SendChar( "#0,0,0,0#", 11);flagB--;}
		
			//������
			if( flagDoor==1&&flagD==0){
			Esp_SendChar("#0,0,0,1#", 11);flagD++;}
			else if(flagDoor==0&&flagD!=0) {
			Esp_SendChar("#0,0,0,0#", 11);flagD--;}
	} 	
}


//���ذ������棨�ߴ�x,yΪ240*150��
//x,y:������ʼ���꣨240*320�ֱ��ʵ�ʱ��x����Ϊ0��
void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl)
{
	u16 i;
	POINT_COLOR=RED;
	kbd_tbl=kbtbl;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+150);						   
	LCD_DrawRectangle(x+80,y,x+160,y+150);	 
	LCD_DrawRectangle(x,y+30,x+240,y+60);
	LCD_DrawRectangle(x,y+90,x+240,y+120);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i==1)//�������2����:������Ҫ�м���ʾ
			Show_Str(x+(i%3)*80+2,y+7+30*(i/3),80,30,(u8*)kbd_tbl[i],16,0);	
		else
			Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),(u8*)kbd_tbl[i],16,80);
	} 
}
//����״̬����
//x,y:��������
//key:��ֵ(0~14)
//sta:״̬��0���ɿ���1�����£�
void AS608_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>16)return;
	if(sta &&keyx!=1)//�������2����:������Ҫ���
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,GREEN);
	else if(keyx!=1)
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,WHITE);
	if(keyx!=1)//���ǡ�����
		Show_Str_Mid(x+j*80,y+7+30*i,(u8*)kbd_tbl[keyx],16,80);	
}
//�õ�������������
//x,y:��������
//����ֵ:��1~15,��Ӧ������
u8 AS608_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)//������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*30+30)&&tp_dev.y[0]>(y+i*30))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					AS608_key_staset(x,y,key_x-1,0);
					key_x=key;
					AS608_key_staset(x,y,key_x-1,1);
				}
				break;
			}
	  }  
	}else if(key_x) 
	{
		AS608_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
//��ȡ������ֵ
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)return 0xFFFF;//�����ء���
			if(key_num==3)return 0xFF00;//		
			if(key_num>3&&key_num<13&&num<99)//��1-9����(��������3λ��)
				num =num*10+key_num-3;		
			if(key_num==13)num =num/10;//��Del����			
			if(key_num==14&&num<99)num =num*10;//��0����
			if(key_num==15)return num;  //��Enter����
		}
		LCD_ShowNum(80+15,170+7,num,6,16);
	}	
}
//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
}
//¼ָ��
void Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"�밴ָ��",16,240);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					BEEP=1;
					ensure=PS_GenChar(CharBuffer1);//��������
					BEEP=0;
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"ָ������",16,240);
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"�밴�ٰ�һ��ָ��",16,240);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					BEEP=1;
					ensure=PS_GenChar(CharBuffer2);//��������
					BEEP=0;
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"ָ������",16,240);
						i=0;
						processnum=2;//����������
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"�Ա�����ָ��",16,240);
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"�Աȳɹ�,����ָ��һ��",16,240);
					processnum=3;//�������Ĳ�
				}
				else 
				{
					LCD_Fill(0,100,lcddev.width,160,WHITE);
					Show_Str_Mid(0,100,"�Ա�ʧ�ܣ�������¼��ָ��",16,240);
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;

			case 3:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"����ָ��ģ��",16,240);
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"����ָ��ģ��ɹ�",16,240);
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"�����봢��ID,��Enter����",16,240);
				Show_Str_Mid(0,120,"0=< ID <=299",16,240);
				do
					ID=GET_NUM();
				while(!(ID<AS608Para.PS_max));//����ID����С��ָ�����������ֵ
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{			
					LCD_Fill(0,100,lcddev.width,160,WHITE);					
					Show_Str_Mid(0,120,"¼��ָ�Ƴɹ�",16,240);
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					LCD_ShowNum(56,80,AS608Para.PS_max-ValidN,3,16);
					delay_ms(1500);
					LCD_Fill(0,100,240,160,WHITE);
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(400);
		if(i==5)//����5��û�а���ָ���˳�
		{
			LCD_Fill(0,100,lcddev.width,160,WHITE);
			break;	
		}				
	}
}

//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		BEEP=1;//�򿪷�����	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			BEEP=0;//�رշ�����	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"ˢָ�Ƴɹ�",16,240);				
				str=mymalloc(SRAMIN,50);
				sprintf(str,"ȷ�д���,ID:%d  ƥ��÷�:%d",seach.pageID,seach.mathscore);
				Show_Str_Mid(0,140,(u8*)str,16,240);
				myfree(SRAMIN,str);
				TIM_SetCompare2(TIM3,25);
			}
			else 
			{ShowErrMessage(ensure);			
				checknum++;
			}		
	  }
		else
			ShowErrMessage(ensure);
	 BEEP=0;//�رշ�����
	 delay_ms(600);
	 LCD_Fill(0,100,lcddev.width,160,WHITE);
	}
		
}

//ɾ��ָ��
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	Show_Str_Mid(0,100,"ɾ��ָ��",16,240);
	Show_Str_Mid(0,120,"������ָ��ID��Enter����",16,240);
	Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	num=GET_NUM();//��ȡ���ص���ֵ
	if(num==0xFFFF)
		goto MENU ; //������ҳ��
	else if(num==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(num,1);//ɾ������ָ��
	if(ensure==0)
	{
		LCD_Fill(0,120,lcddev.width,160,WHITE);
		Show_Str_Mid(0,140,"ɾ��ָ�Ƴɹ�",16,240);		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	LCD_ShowNum(56,80,AS608Para.PS_max-ValidN,3,16);
MENU:	
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);
}







