#include "__std_type.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "key.h"
#include "eeprom.h"
#include "app_temp_set.h"


extern uint8_t g_home ;
extern unsigned char choose_page;

//char g_temp = 31;
void temp_setting (void) 
{
		unsigned char temp = iap_read_byte(0x0000);
		lcd12864_write_cmd(0x01);
		lcd12864_set_window(0, 1);
		lcd12864_write_string("≡报警温度≡");
		lcd12864_set_window(1, 7);
		lcd12864_write_string("℃");
	while (g_home) {
		unsigned char keyval = keyscan(5);
		if (EN_KEY3 == keyval) {
			iap_erase_sector(0x0000);
			iap_program_byte(0x0000, temp);
			choose_page = 3;
			break;
		}	else if (EN_KEY1 == keyval) {
			temp++;
			if (temp > 100) {
				temp = 0;
			} 
		} else if (EN_KEY2 == keyval) {
			temp--;
			if (temp < 0) {
				temp = 100;
			}
		}
		lcd12864_set_window(1, 0);
		lcd12864_write_string("温度上限: ");
		lcd12864_write_data((temp / 10) | 0x30);
		lcd12864_write_data((temp % 10) | 0x30);
		

	}
}