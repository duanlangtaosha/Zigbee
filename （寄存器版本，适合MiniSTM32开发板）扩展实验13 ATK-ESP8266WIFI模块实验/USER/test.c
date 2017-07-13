#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h"  
#include "malloc.h"
#include "mmc_sd.h"  
#include "flash.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"
#include "usart2.h"
#include "touch.h"
#include "common.h"

//ALIENTEKս��STM32��������չʵ��6
//ATK-ESP8266 WIFIģ�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

int main(void)
{
	u8 key,fontok=0; 
  Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);			//��ʱ��ʼ��
	uart_init(72,115200); 	//����1��ʼ�� 
	USART2_Init(36,115200);  //����3��ʼ��
	LCD_Init();				//��ʼ��Һ�� 
	LED_Init();         	//LED��ʼ�� 	
	KEY_Init();				//������ʼ��	 
	usmart_dev.init(72);	//usmart��ʼ��	
	key=KEY_Scan(0);  

	atk_8266_test();		//����ATK_ESP8266����
}

