#include "__std_type.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "key.h"
#include "app_temp_set.h"

extern uint8_t g_home ;
extern unsigned char choose_page;

char g_temp = 31;
void temp_setting (void) 
{
		lcd12864_write_cmd(0x01);
		lcd12864_set_window(0, 1);
		lcd12864_write_string("≡报警温度≡");
		lcd12864_set_window(1, 7);
		lcd12864_write_string("℃");
	while (g_home) {
		unsigned char keyval = keyscan(5);
		if (EN_KEY3 == keyval) {
			choose_page = 3;
			break;
		}	else if (EN_KEY1 == keyval) {
			g_temp++;
			if (g_temp > 100) {
				g_temp = 0;
			} 
		} else if (EN_KEY2 == keyval) {
			g_temp--;
			if (g_temp < 0) {
				g_temp = 100;
			}
		}
		lcd12864_set_window(1, 0);
		lcd12864_write_string("温度上限: ");
		lcd12864_write_data((g_temp / 10) | 0x30);
		lcd12864_write_data((g_temp % 10) | 0x30);
		

	}
}