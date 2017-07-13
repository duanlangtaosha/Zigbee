#include "__std_type.h"
#include "lcd12864.h"
#include "key.h"
#include "uart.h"
#include "uart2.h"
#include "app_elc.h"


extern uint8_t  xdata __g_uart2_buf[UART2_BUF_SIZE];
extern uint8_t g_wifi_ok ;
extern uint8_t g_uart2_sta;
extern uint8_t g_home ;
extern unsigned char choose_page;
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
		
			if (g_wifi_ok) {
				
				if (g_uart2_sta & 0x80) {
				
						uart_send_frame(15.0, __g_uart2_buf[1]);
					g_uart2_sta = 0x00;
				}
				
			}
		
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
			g_home = 1;
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