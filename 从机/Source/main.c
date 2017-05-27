#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"

#define LED  P17
#define FAN  P16
#define BEEP P15

void main (void)
{

	
	uart_init();

	init_ds18b20();

	P1M1 = 0x00;
	P1M0 = 0xFF;
	
	LED = 1;
	FAN = 0;
	BEEP = 0;
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
			
			if (control & 0x02) {
				FAN = 1;
			} else {
				FAN = 0;
			} 
			
			if (control & 0x04) {
				BEEP = 1;
			} else {
				BEEP = 0;
			}
	}
		if (LED == 0) {
			control |= 0x01;
		} else {
			control &= ~0x01;
		}
		
		if (FAN == 1) {
			control |= 0x02;
		} else {
			control &= ~0x02;
		}
		
		if (BEEP == 1) {
			control |= 0x04;
		} else {
			control &= ~0x04;
		}
		
		
    temp = rd_temperature_i();
		uart_send_frame(temp, control);	
		
		if (keycode == EN_KEY0) {
			LED = !LED;
		} else if (keycode == EN_KEY1) {
			FAN = !FAN;
		} else if (keycode == EN_KEY2) {
			BEEP = !BEEP;
		}
		
}
}

