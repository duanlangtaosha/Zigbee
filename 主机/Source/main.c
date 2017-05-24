#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"


unsigned char choose_page = 0;

/*主页*/
extern uint8_t g_uart_sta;
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
		unsigned char ds1302_buf[7] = {0};
		unsigned char keyval = keyscan();
		unsigned char temp = rd_temperature_i();
//		unsigned char temp = 0;
		
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
				
				//********************************************************************************************
//				uart_send_byte(control);
				uart_send_byte('a');
//				g_uart_sta = 0;
			}
			uart_send_byte(g_uart_sta);
			get_ds1302_time(ds1302_buf);
			lcd12864_set_window(3, 3);
			lcd12864_write_data((ds1302_buf[2] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[2] % 10) | 0x30 );
			lcd12864_write_string(":");
			lcd12864_write_data((ds1302_buf[1] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[1] % 10) | 0x30 );
			lcd12864_write_string(":");
			lcd12864_write_data((ds1302_buf[0] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[0] % 10) | 0x30 );
			
	}
}

/*主菜单*/
void menu (void)
{
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

		unsigned char keyval = keyscan();	
		if (EN_KEY1 == keyval) {	/* 菜单下选择 */
					
			choose_page++;
			if (choose_page == 5) {
				choose_page = 2;
			}
			
		} else if (EN_KEY2 == keyval) { /* 菜单上选择 */
			choose_page--;
			if (choose_page == 1) {
				choose_page = 4;
			}
		} else if (EN_KEY3 == keyval) {
			choose_page = 0;
			break;
		} else if (EN_KEY0 == keyval) {
				break;
		}
		
		switch (choose_page) {
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


void elec_app (void)
{
	unsigned char elc_num = 0;
	
//	float temp = 0.0;
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
		unsigned char keyval = keyscan();
		
		/* 获取当前从机家用电器的状态 */
		if (1 == uartf_reciev_frame(&temp, &elc_sta)) {
			
			//****************************************************************************
//			uart_send_byte(elc_sta);
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
					

					break;
				case 3:

					break;
				case 4:

					break;
				default:
					break;
			}
		}
	}

}

void setting (void)
{
	lcd12864_write_cmd(0x01);
	while (1) {
		unsigned char keyval = keyscan();
		lcd12864_set_window(0, 0);
		lcd12864_write_string("设置");
		
		
				
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
		
		unsigned char keyval = keyscan();
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
		lcd12864_set_window(0, 0);
		lcd12864_write_string("其他");
	
	}

}




void main (void)
{

	uart_init();
	uart2_init();
	lcd12864_init ();
	init_ds18b20();
	init_ds1302();
	
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
			default:
				break;
		
		}

		
	}

}



