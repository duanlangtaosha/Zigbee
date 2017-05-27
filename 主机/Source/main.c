#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"
#include "esp8266.h"

unsigned char choose_page = 0;

/*主页*/
extern uint8_t g_uart_sta;



extern uint8_t g_wifi_ok ;
extern uint8_t g_uart2_sta;
extern uint8_t  xdata __g_uart2_buf[UART2_BUF_SIZE];


void home (void) 
{
	
	lcd12864_write_cmd(0x01);

  lcd12864_set_window(0, 0);						
	lcd12864_write_string("主机温度:");
	lcd12864_set_window(0, 7);						
	lcd12864_write_string("℃");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("从机温度:");
	lcd12864_set_window(1, 7);						
	lcd12864_write_string("℃");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("日期:");
	lcd12864_set_window(2, 3);	
	lcd12864_write_string("");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("时间:");
	lcd12864_set_window(3, 3);
	lcd12864_write_string("");
	while (1) {
		unsigned char ds1302_buf[7] = {0};	/* DS1302接收缓存区 */
		unsigned char keyval = keyscan(5);
		unsigned char temp = rd_temperature_i();	/* 读取温度 */
		
		float device_temp = 0.0;
		unsigned char control = 0;
		if (EN_KEY0 == keyval) {
		
			choose_page = 1; /* 进入主菜单*/
			break;
		}
			lcd12864_set_window(0, 5);
			lcd12864_write_data((temp / 10) | 0x30 );
			lcd12864_write_data((temp % 10) | 0x30 );
		
		
			if (1 == uartf_reciev_frame(&device_temp, &control)) {
				temp = (unsigned char)device_temp;
				lcd12864_set_window(1, 5);
				lcd12864_write_data((temp / 10) | 0x30 );
				lcd12864_write_data((temp % 10) | 0x30 );
			}
//			uart_send_byte(g_uart_sta);
			get_ds1302_time(ds1302_buf);
			
			/* 日期设置 */
			lcd12864_set_window(2, 3);
			
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
			lcd12864_set_window(3, 3);
			
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
			
			
			
			
			
			
			
			
			
			if (g_wifi_ok) {
				
				if (g_uart2_sta & 0x80) {
				
//					if (__g_uart2_buf[1] == 0x01) {

						uart_send_frame(15.0, __g_uart2_buf[1]);
//					}
					g_uart2_sta = 0x00;
				}
				
			}
			
			
	}
}

/*主菜单*/
void menu (void)
{
	static unsigned char menu_save = 2;
	lcd12864_write_cmd(0x01);
	choose_page++;
	lcd12864_set_window(0, 0);
	lcd12864_write_string("≡≡主菜单≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("1.家电");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("2.设置");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("3.其他");
	
	while (1) {

		unsigned char keyval = keyscan(5);	
		if (EN_KEY1 == keyval) {	/* 菜单下选择 */
					
			choose_page++;
			menu_save = choose_page;
			if (choose_page == 5) {
				choose_page = 2;
				menu_save = choose_page;
			}
			
		} else if (EN_KEY2 == keyval) { /* 菜单上选择 */
			choose_page--;
			menu_save = choose_page;
			if (choose_page == 1) {
				choose_page = 4;
				menu_save = 4;
			}
		} else if (EN_KEY3 == keyval) {
			choose_page = 0;
			break;
		} else if (EN_KEY0 == keyval) {
			choose_page = menu_save;
				break;
		}
		
		switch (menu_save) {
			case 2:
				lcd12864_set_window(3, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(1, 3);
				lcd12864_write_data(17);
			break;
			
			case 3:
				lcd12864_set_window(1, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(3, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(17);
				break;
			case 4:
				lcd12864_set_window(1, 3);
				lcd12864_write_data(32);	
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(3, 3);
				lcd12864_write_data(17);
				break;
		}
		
	}
}


/* 家电 */
void elec_app (void)
{
	unsigned char elc_num = 0;
	
	unsigned char elc_sta = 0;
  lcd12864_write_cmd(0x01);

	lcd12864_set_window(0, 1);
	lcd12864_write_string("≡≡家电≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("E1:OFF");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("E2:OFF");
	lcd12864_set_window(1, 4);
	lcd12864_write_string("E3:OFF");
	lcd12864_set_window(2, 4);
	lcd12864_write_string("E4:OFF");
	while (1) {
		float temp = 10.0;
		unsigned char keyval = keyscan(2);
		
		/* 获取当前从机家用电器的状态 */
		if (1 == uartf_reciev_frame(&temp, &elc_sta)) {
			if(elc_sta & 0x01) {
				lcd12864_set_window(1, 0);
				lcd12864_write_string("E1:ON ");
			} else {
				lcd12864_set_window(1, 0);
				lcd12864_write_string("E1:OFF");
			}
			
			if(elc_sta & 0x02) {
				lcd12864_set_window(2, 0);
				lcd12864_write_string("E2:ON ");
			} else {
				lcd12864_set_window(2, 0);
				lcd12864_write_string("E2:OFF");
			}
			
			if(elc_sta & 0x04) {
				lcd12864_set_window(1, 4);
				lcd12864_write_string("E3:ON ");
			} else {
				lcd12864_set_window(1, 4);
				lcd12864_write_string("E3:OFF");
			}

			if(elc_sta & 0x08) {
				lcd12864_set_window(2, 4);
				lcd12864_write_string("E4:ON ");
			} else {
				lcd12864_set_window(2, 4);
				lcd12864_write_string("E4:OFF");
			}
		} else {
			
			/* 不加上这句校验和就会永远的出错 */
//			uart_send_string("eorr\r\n");
		}
		
		
		/* 返回上一页 */
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		} else if (EN_KEY1 == keyval) {
			elc_num++;
			if(elc_num == 5) {
				elc_num = 1;
			}
				switch (elc_num) {

				case 1:
					lcd12864_set_window(2, 7);
					lcd12864_write_data(32);	
				
					lcd12864_set_window(1, 3);
					lcd12864_write_data(17);
					
					break;
				case 2:
					
					lcd12864_set_window(1, 3);
					lcd12864_write_data(32);
				
					lcd12864_set_window(2, 3);
					lcd12864_write_data(17);
					break;
				case 3:
					lcd12864_set_window(2, 3);
					lcd12864_write_data(32);
				
					lcd12864_set_window(1, 7);
					lcd12864_write_data(17);
					break;
				case 4:
					lcd12864_set_window(1, 7);
					lcd12864_write_data(32);
				
					lcd12864_set_window(2, 7);
					lcd12864_write_data(17);
					break;
				default:
					break;
			}
		} else if (EN_KEY0 == keyval) {
				switch (elc_num) {
				case 1:
					if (elc_sta & 0x01) {
						elc_sta &= ~0x01;
					} else {
						elc_sta |= 0x01;
					}
					uart_send_frame(0.0, elc_sta);
							
					break;
				case 2:
					if (elc_sta & 0x02) {
						elc_sta &= ~0x02;
					} else {
						elc_sta |= 0x02;
					}
					uart_send_frame(0.0, elc_sta);

					break;
				case 3:
					if (elc_sta & 0x04) {
						elc_sta &= ~0x04;
					} else {
						elc_sta |= 0x04;
					}
					uart_send_frame(0.0, elc_sta);
					break;
				case 4:
					if (elc_sta & 0x08) {
						elc_sta &= ~0x08;
					} else {
						elc_sta |= 0x08;
					}
					uart_send_frame(0.0, elc_sta);
					break;
				default:
					break;
			}
		}
	}

}

void setting (void)
{
	unsigned char point_select = 0;
	lcd12864_write_cmd(0x01);
	lcd12864_set_window(0, 1);
	lcd12864_write_string("≡≡设置≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("1.时间");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("2.温度");
	while (1) {
		unsigned char keyval = keyscan(5);
//		lcd12864_set_window(0, 0);
//		lcd12864_write_string("设置");
		
		if (EN_KEY0 == keyval) {
			if (point_select == 0) {
				choose_page = 5;
			} else {
			
			}
			break;
		}

				
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
	}
}

void other (void )
{
	lcd12864_write_cmd(0x01);
	while (1) {
		
		unsigned char keyval = keyscan(5);
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
		lcd12864_set_window(0, 0);
		lcd12864_write_string("其他");
	
	}

}



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
//	TH0 = (65536 - 1000) >> 8;
//	TL0 = (65536- 1000);
	
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
	while (1) {
		
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
		
//		if ( refresh == 1) {
//			lcd12864_set_window(1, 3);
//			lcd12864_write_data(32);
//		} else {
//			lcd12864_set_window(1, 3);
//			lcd12864_write_data(0x32);
//		}
		
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
			choose_page = 1;
			set_ds1302_time(&ds1302_buf);
			
			TR0 = 0;
			ET0 = 0;
			break;
		}
	}
}


void main (void)
{


	P2M1 = 0x00;
	P2M0 = 0xFF;
	
	uart_init();
	uart2_init();
	
	lcd12864_init ();
	
	
	init_ds18b20();
	init_ds1302();
	init_esp_8266_setting();
	
	
	while (1) {
		
		unsigned char *p = "abcd";
		unsigned char buuu[5]={0x11 ,0x22, 0x33,0x44,0x55};
		float a = 18.5,c = 0.0;
		unsigned char b = 0x01, d = 0;


//		if(uartf_reciev_frame(&c, &d))
//		{
//		
//		uart_send_byte(c/10 + 0x30);
//		uart_send_byte((unsigned char)c%10 + 0x30);
//		uart_send_byte((unsigned char)(c*10)%10 +0x30);
//		uart_send_byte('\r');
//		uart_send_byte('\n');
//		uart_send_byte(d + 0x30);
//		}
//		uart2_send_string("abcdefg");

		
		switch (choose_page) {
		
		case 0:
			home () ;
//		date_time_setting();
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
			default:
				break;
		
		}

		
	}

}



