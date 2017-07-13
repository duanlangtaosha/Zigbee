#include "stc12c5a60s2.h"
#include "uart.h"
//#include "ds1302.h"
extern void Set_RTC(void);
extern void Read_RTC(void);

extern unsigned char l_tmpdate[7];
//extern unsigned char data timeNow[7];
int main()
{
	uart_init();
//	init_ds1302();
//	set_ds1302time();
	Set_RTC();  
	while(1)
	{
		Read_RTC();
//		unsigned char timenow[7] = {0};
//		
//		get_ds1302_time(timenow);
		printf("%c",l_tmpdate[1]);
	}
	
}