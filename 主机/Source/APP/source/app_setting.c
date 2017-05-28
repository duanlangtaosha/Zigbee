#include "__std_type.h"
#include "lcd12864.h"
#include "key.h"
#include "app_setting.h"

extern unsigned char choose_page;
extern uint8_t g_home ;

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
	while (g_home) {
		unsigned char keyval = keyscan(5);
		
		if (EN_KEY0 == keyval) {
			if (point_select == 0) {
				choose_page = 5;
			} else {
				choose_page = 6;
			}
			break;
		} else if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		} else if (EN_KEY2 == keyval) {
			point_select++;
			
			if (point_select == 2) {
				point_select = 0;
			}
		}
		
		
		
		
		switch (point_select) {
		
			case 0:
				lcd12864_set_window(2, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(1, 3);
				lcd12864_write_data(17);
				break;
			case 1:
				lcd12864_set_window(1, 3);
				lcd12864_write_data(32);	
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(17);
				break;
			default:
				break;
		}
	}
}