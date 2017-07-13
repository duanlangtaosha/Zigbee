#include "stc12c5a60s2.h"
#include "uart.h"
#include "ds1302.h"

//extern unsigned char data timeNow[7];
int main()
{
	uart_init();
	init_ds1302();
//	set_ds1302time();

	while(1)
	{
		unsigned char timenow[7] = {0};
		
		get_ds1302_time(timenow);
		printf("%c",timenow[0]);
	}
	
}