#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"
#include "esp8266.h"

#include "app_home.h"
#include "app_menu.h"
#include "app_other.h"
#include "app_elc.h"
#include "app_setting.h"
#include "app_time_set.h"
#include "app_temp_set.h"

unsigned char choose_page = 0;

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main (void)
{

	lcd12864_init ();
	P2M1 = 0x00;
	P2M0 = 0xFF;
	lcd12864_set_window(0, 1);
	lcd12864_write_string("智能家居系统");
	lcd12864_set_window(1, 1);
	lcd12864_write_string("系统启动中...");
	uart_init();

//	lcd12864_init ();
	
	init_ds18b20();
	init_ds1302();
	
	uart2_init();
	Delay2000ms();
	init_esp_8266_setting();

	while (1) {
		
//		unsigned char buuu[5]={0x11 ,0x22, 0x33,0x44,0x55};
//		float a = 18.5,c = 0.0;
//		unsigned char b = 0x01, d = 0;
		
		switch (choose_page) {
		
		case 0:
			home () ;
			break;
		case 1:
			menu();
			break;
		case 2:
			elec_app();
			break;
			
		case 3:
			setting();
			break;
		case 4:
			other();
			break;
		case 5:
			date_time_setting();
			break;
		case 6:
			temp_setting();
			break;
			default:
				break;
		
		}
	}

}



