#include "__std_type.h"
#include "lcd12864.h"
#include "key.h"
#include "app_menu.h"


extern unsigned char choose_page;
extern uint8_t g_home ;
/*主菜单*/
void menu (void)
{
	static unsigned char menu_save = 2;
	lcd12864_write_cmd(0x01);
	choose_page++;
	lcd12864_set_window(0, 1);
	lcd12864_write_string("≡≡菜单≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("1.家电");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("2.设置");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("3.其他");
	
	while (g_home) {

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