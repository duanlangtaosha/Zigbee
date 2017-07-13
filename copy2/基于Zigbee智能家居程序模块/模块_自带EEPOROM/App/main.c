#include "stc12c5a60s2.h"
#include "uart.h"
#include "intrins.h"
#include "eeprom.h"

#define IAP_ADDRESS 0x0000


void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	uart_init();
	
	iap_erase_sector(IAP_ADDRESS);    //Erase current sector
	
	/* 必须要擦除当前扇区才能够写 */
	iap_program_byte(IAP_ADDRESS, 8);
	
	while (1) {
		unsigned char temp = iap_read_byte(IAP_ADDRESS);
		uart_send_byte(temp);
		Delay100ms();
	}
}


