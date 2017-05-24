#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"

#define LED P17

void main (void)
{

	uart_init();

	init_ds18b20();

	
	while (1) {
		
		float temp = 0.0;
		static unsigned char control = 0x00;
		
		unsigned char keycode = keyscan();


		/* 接收控制指令 */
		if (uartf_reciev_frame(&temp, &control))
		{
			if (control & 0x01) {
				LED = 0;
			} else {
				LED = 1;
			}
	}
		if (LED == 0) {
			control |= 0x01;
		} else {
			control &= ~0x01;
		}
    temp = rd_temperature_i();
		uart_send_frame(temp, control);	
		
		if (keycode == EN_KEY0) {
			LED = !LED;
		}
}
}

