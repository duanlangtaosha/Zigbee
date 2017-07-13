#include "__std_type.h"
#include "lcd12864.h"
#include "ds1302.h"
#include "key.h"
#include "app_time_set.h"

extern unsigned char choose_page;
extern uint8_t g_home ;

void Timer0Init(void)		//1??@11.0592MHz
{
//	AUXR |= 0x80;		
	TMOD &= 0xF0;		
	TL0 = 0xCD;		
	TH0 = 0xD4;		
	TF0 = 0;		
	TR0 = 1;		
	
	EA = 1;
	ET0 = 1;
}

bit refresh = 0;
void isr_timer0 (void) interrupt 1
{
	static unsigned int count = 0;
	TF0 = 0;
	TL0 = 0xCD;		
	TH0 = 0xD4;	
	
	count++;
	if (500 == count) {
		count = 0;
		refresh = !refresh;
	}
}



void date_time_setting (void) 
{
	unsigned char point_select = 0;
	unsigned char ds1302_buf[7] = {0};	/* DS1302接收缓存区 */
	lcd12864_write_cmd(0x01);
	
	get_ds1302_time(ds1302_buf);
	
	lcd12864_set_window(0, 0);
	lcd12864_write_string("≡时间日期设置≡");
	
	lcd12864_set_window(1, 0);
	lcd12864_write_string("日期  ");
	
	lcd12864_set_window(2, 0);
	lcd12864_write_string("时间  ");
	
	
	/* 日期设置 */
	lcd12864_set_window(1, 3);
	
	/* 年 */
	lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[6] & 0x0F) | 0x30 );
	lcd12864_write_string("-");
	
	/* 月 */
	lcd12864_write_data((ds1302_buf[4] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[4] & 0x0F) | 0x30 );
	lcd12864_write_string("-");
	
	/* 日 */
	lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[3] & 0x0F) | 0x30 );
	
	/* 时间的设置 */
	lcd12864_set_window(2, 3);
	
	/* 时 */
	lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[2] & 0x0F) | 0x30 );
	lcd12864_write_string(":");
	
	/* 分 */
	lcd12864_write_data((ds1302_buf[1] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[1] & 0x0F) | 0x30 );
	lcd12864_write_string(":");
	
	/* 秒 */
	lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
	lcd12864_write_data((ds1302_buf[0] & 0x0F) | 0x30 );
		
	Timer0Init();
	while (g_home) {
		
		unsigned char keyval = keyscan(5);
		
		switch (point_select) {
		
			case 0 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 3);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 3);
					lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
				}
				break;
			case 1 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 3);
					lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 3);
					lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
					lcd12864_write_data((ds1302_buf[6] & 0x0F) | 0x30 );
				}
				break;
			case 2 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 4);
					lcd12864_write_string("-");
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 4);
					lcd12864_write_string("-");
					lcd12864_write_data((ds1302_buf[4] >> 4) | 0x30 );
				}
				break;
			case 3 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 5);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 5);
					lcd12864_write_data((ds1302_buf[4] & 0x0F) | 0x30 );
				}
				break;
			case 4 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 6);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 6);
					lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
				}
				break;
			case 5 :
				if ( refresh == 1) {
					lcd12864_set_window(1, 6);
					lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(1, 6);
					lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
					lcd12864_write_data((ds1302_buf[3] & 0x0F) | 0x30 );
				}
				break;
			case 6 :
				if ( refresh == 1) {
					lcd12864_set_window(2, 3);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 3);
					lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
				}
				break;
			case 7 :
				if ( refresh == 1) {
					lcd12864_set_window(2, 3);
					lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 3);
					lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
					lcd12864_write_data((ds1302_buf[2] & 0x0F) | 0x30 );
				}
				break;
			case 8 :
				if ( refresh == 1) {
					lcd12864_set_window(2, 4);
					lcd12864_write_string(":");
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 4);
					lcd12864_write_string(":");
					lcd12864_write_data((ds1302_buf[1] >> 4) | 0x30 );
				}
				break;
			case 9 :
				if ( refresh == 1) {
					lcd12864_set_window(2, 5);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 5);
					lcd12864_write_data((ds1302_buf[1] & 0x0F) | 0x30 );
				}
				break;
			case 10:
				if ( refresh == 1) {
					lcd12864_set_window(2, 6);
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 6);
					lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
				}
				break;
			case 11:
				if ( refresh == 1) {
					lcd12864_set_window(2, 6);
					lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
					lcd12864_write_data(32);
				} else {
					lcd12864_set_window(2, 6);
					lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
					lcd12864_write_data((ds1302_buf[0] & 0x0F) | 0x30 );
				}
				break;
			default:
				break;
		}
		
		if (EN_KEY2 == keyval) {
			point_select++;
			
			if (point_select == 12) {
				point_select = 0;
			}
			/* 日期设置 */
			lcd12864_set_window(1, 3);
			
			/* 年 */
			lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[6] & 0x0F) | 0x30 );
			lcd12864_write_string("-");
			
			/* 月 */
			lcd12864_write_data((ds1302_buf[4] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[4] & 0x0F) | 0x30 );
			lcd12864_write_string("-");
			
			/* 日 */
			lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[3] & 0x0F) | 0x30 );
			
			/* 时间的设置 */
			lcd12864_set_window(2, 3);
			
			/* 时 */
			lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[2] & 0x0F) | 0x30 );
			lcd12864_write_string(":");
			
			/* 分 */
			lcd12864_write_data((ds1302_buf[1] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[1] & 0x0F) | 0x30 );
			lcd12864_write_string(":");
			
			/* 秒 */
			lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[0] & 0x0F) | 0x30 );
		}
		if (EN_KEY0 == keyval) {
			switch (point_select) {
				case 0 :
					ds1302_buf[6] = (ds1302_buf[6] & 0x0F) | ((ds1302_buf[6] >> 4) + 1) << 4;
					if((ds1302_buf[6] >> 4) > 9 ) {
					ds1302_buf[6] = ds1302_buf[6] & 0x0F;
					}
					break;
				case 1 :
					ds1302_buf[6] = (ds1302_buf[6] & 0xF0) | ((ds1302_buf[6] & 0x0F) + 1) ;
					if((ds1302_buf[6] & 0x0F) > 9 ) {
					ds1302_buf[6] = ds1302_buf[6] & 0xF0;
					}
					break;
				case 2 :
					ds1302_buf[4] = (ds1302_buf[4] & 0x0F) | ((ds1302_buf[4] >> 4) + 1) << 4;
					if((ds1302_buf[4] >> 4) > 9 ) {
					ds1302_buf[4] = ds1302_buf[6] & 0x0F;
					}
					break;
				case 3 :
					ds1302_buf[4] = (ds1302_buf[4] & 0xF0) | ((ds1302_buf[4] & 0x0F) + 1) ;
					if((ds1302_buf[4] & 0x0F) > 9 ) {
					ds1302_buf[4] = ds1302_buf[4] & 0xF0;
					}
					break;								
				case 4 :
					ds1302_buf[3] = (ds1302_buf[3] & 0x0F) | ((ds1302_buf[3] >> 4) + 1) << 4;
					if((ds1302_buf[3] >> 4) > 9 ) {
					ds1302_buf[3] = ds1302_buf[3] & 0x0F;
					}
					break;		
				case 5 :
					ds1302_buf[3] = (ds1302_buf[3] & 0xF0) | ((ds1302_buf[3] & 0x0F) + 1) ;
					if((ds1302_buf[3] & 0x0F) > 9 ) {
					ds1302_buf[3] = ds1302_buf[3] & 0xF0;
					}
					break;
				case 6 :
					ds1302_buf[2] = (ds1302_buf[2] & 0x0F) | ((ds1302_buf[2] >> 4) + 1) << 4;
					if((ds1302_buf[2] >> 4) > 2 ) {
					ds1302_buf[2] = ds1302_buf[2] & 0x0F;
					}
					break;
				case 7 :
					ds1302_buf[2] = (ds1302_buf[2] & 0xF0) | ((ds1302_buf[2] & 0x0F) + 1) ;
					if((ds1302_buf[2] & 0x0F) > 9 ) {
					ds1302_buf[2] = ds1302_buf[2] & 0xF0;
					}
					break;
				case 8 :
					ds1302_buf[1] = (ds1302_buf[1] & 0x0F) | ((ds1302_buf[1] >> 4) + 1) << 4;
					if((ds1302_buf[1] >> 4) > 5 ) {
					ds1302_buf[1] = ds1302_buf[1] & 0x0F;
					}
					break;
				case 9 :
					ds1302_buf[1] = (ds1302_buf[1] & 0xF0) | ((ds1302_buf[1] & 0x0F) + 1) ;
					if((ds1302_buf[1] & 0x0F) > 9 ) {
					ds1302_buf[1] = ds1302_buf[1] & 0xF0;
					}
					break;
				case 10 :
					ds1302_buf[0] = (ds1302_buf[0] & 0x0F) | ((ds1302_buf[0] >> 4) + 1) << 4;
					if((ds1302_buf[0] >> 4) > 5 ) {
					ds1302_buf[0] = ds1302_buf[0] & 0x0F;
					}
					break;
				case 11 :
					ds1302_buf[0] = (ds1302_buf[0] & 0xF0) | ((ds1302_buf[0] & 0x0F) + 1) ;
					if((ds1302_buf[0] & 0x0F) > 9 ) {
					ds1302_buf[0] = ds1302_buf[0] & 0xF0;
					}
					break;
				default:
					break;
			}
		}
		if (EN_KEY3 == keyval) {
			choose_page = 3;
			set_ds1302_time(&ds1302_buf);
			
			TR0 = 0;
			ET0 = 0;
			break;
		}
	}
}
