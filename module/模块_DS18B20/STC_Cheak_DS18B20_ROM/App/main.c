#include ".\Head\stc12c5a60s2.h"
#include ".\Head\intrins.h"
#include ".\Head\LCD1602.h"
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

//extern void Read_DS18B20_ROM(void)	;
extern void Display_DS18B20_ROM();

int main(void)
{
	Initial_LCD();
 	while(1)
	{
		 Display_DS18B20_ROM();
		 Delay500ms();
	}
	return 0;
}