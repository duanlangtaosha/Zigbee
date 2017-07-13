#include "__std_type.h"
#include "lcd12864.h"
#include "key.h"
#include "app_other.h"

extern unsigned char choose_page;
extern uint8_t g_home ;

void other (void )
{
	lcd12864_write_cmd(0x01);
	while (g_home) {
		
		unsigned char keyval = keyscan(5);
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
		lcd12864_set_window(0, 3);
		lcd12864_write_string("STAIP");
		lcd12864_set_window(1, 1);
		lcd12864_write_string("192.168.1.200");
		
		lcd12864_set_window(2, 3);
		lcd12864_write_string("APIP");
		lcd12864_set_window(3, 1);
		lcd12864_write_string("192.168.4.1");
	}
}