#include "stc12c5a60s2.h"
#include "uart.h"
int main()
{
//	unsigned char a[5] ={0};
	uart_init();
	while(1)
	{
		unsigned char b[3] = {0};
		unsigned char a = 0;
//		unsigned char a[5] = {0};
		float ff = 0.0;
//		printf("¶ÏÀËÌÔÉ³³ö°æ\r\n");
		
//	if(uart_recieve_buf (a, 5))
//		uart_send_buf (a, 5);
		
//		if(uartf_reciev_frame(&ff, &a)) {
//		b[0] = (uint8_t)(ff);
//		b[1] = (uint16_t)(ff * 10) % 10;
//		b[2] = a;
//			uart_send_buf (b, 3);
//		}
		uart_send_frame ( 28.5, 0x80);
		
	}
	
}