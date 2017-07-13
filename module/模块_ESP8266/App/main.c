#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart2.h"

extern esp_8266_send_com ( uint8_t *com, uint8_t *ack, uint16_t waittime);
extern void init_esp_8266_setting ( void);


void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


extern uint8_t g_wifi_ok ;
extern uint8_t g_uart2_sta;
extern uint8_t  xdata __g_uart2_buf[UART2_BUF_SIZE];
void main()
{
	
	uart2_init();
	Delay2000ms();
	init_esp_8266_setting();
    while(1)
		{
			if (g_wifi_ok) {
				
				if (g_uart2_sta & 0x80) {
				
					if (__g_uart2_buf[1] == 0x01) {
						uart2_send_string("123456");
					}
					g_uart2_sta = 0x00;
				}
				
			}
		}
}


