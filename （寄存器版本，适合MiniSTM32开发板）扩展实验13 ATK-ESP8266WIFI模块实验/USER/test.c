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

//ALIENTEK战舰STM32开发板扩展实验6
//ATK-ESP8266 WIFI模块测试实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

int main(void)
{
	u8 key,fontok=0; 
  Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);			//延时初始化
	uart_init(72,115200); 	//串口1初始化 
	USART2_Init(36,115200);  //串口3初始化
	LCD_Init();				//初始化液晶 
	LED_Init();         	//LED初始化 	
	KEY_Init();				//按键初始化	 
	usmart_dev.init(72);	//usmart初始化	
	key=KEY_Scan(0);  

	atk_8266_test();		//进入ATK_ESP8266测试
}

