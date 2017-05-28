#include "__std_type.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "uart.h"
#include "key.h"
#include "app_home.h"
uint8_t g_home = 1;

extern unsigned char choose_page;
extern char g_temp;
void home (void) 
{
	
	lcd12864_write_cmd(0x01);

  lcd12864_set_window(0, 0);						
	lcd12864_write_string("�����¶�:");
	lcd12864_set_window(0, 7);						
	lcd12864_write_string("��");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("�ӻ��¶�:");
	lcd12864_set_window(1, 7);						
	lcd12864_write_string("��");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("����:");
	lcd12864_set_window(2, 3);	
	lcd12864_write_string("");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("ʱ��:");
	lcd12864_set_window(3, 3);
	lcd12864_write_string("");
	while (g_home) {
		unsigned char ds1302_buf[7] = {0};	/* DS1302���ջ����� */
		unsigned char keyval = keyscan(5);
		int8_t temp = rd_temperature_i();	/* ��ȡ�¶� */
		
		float device_temp = 0.0;
		unsigned char control = 0;
		if (EN_KEY0 == keyval) {
		
			choose_page = 1; /* �������˵�*/
			break;
		}
		
			/* �����¶� */
			lcd12864_set_window(0, 5);
			lcd12864_write_data((temp / 10) | 0x30 );
			lcd12864_write_data((temp % 10) | 0x30 );
		
			/* �ӻ��¶� */
			if (1 == uartf_reciev_frame(&device_temp, &control)) {
				temp = (unsigned char)device_temp;
				lcd12864_set_window(1, 5);
				lcd12864_write_data((temp / 10) | 0x30 );
				lcd12864_write_data((temp % 10) | 0x30 );
				
				if (temp > g_temp) {
					uart_send_frame(15.0, control | 0x04);
				} else {
				
					uart_send_frame(15.0, control & ~0x04);
				}
				
			}

			get_ds1302_time(ds1302_buf);
			
			/* �������� */
			lcd12864_set_window(2, 3);
			
			/* �� */
			lcd12864_write_data((ds1302_buf[6] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[6] & 0x0F) | 0x30 );
			lcd12864_write_string("-");
			
			/* �� */
			lcd12864_write_data((ds1302_buf[4] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[4] & 0x0F) | 0x30 );
			lcd12864_write_string("-");
			
			/* �� */
			lcd12864_write_data((ds1302_buf[3] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[3] & 0x0F) | 0x30 );
			
			/* ʱ������� */
			lcd12864_set_window(3, 3);
			
			/* ʱ */
			lcd12864_write_data((ds1302_buf[2] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[2] & 0x0F) | 0x30 );
			lcd12864_write_string(":");
			
			/* �� */
			lcd12864_write_data((ds1302_buf[1] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[1] & 0x0F) | 0x30 );
			lcd12864_write_string(":");
			
			/* �� */
			lcd12864_write_data((ds1302_buf[0] >> 4) | 0x30 );
			lcd12864_write_data((ds1302_buf[0] & 0x0F) | 0x30 );	
	}
}