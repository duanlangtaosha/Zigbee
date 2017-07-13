#include ".\Head\stc12c5a60s2.h"
#include ".\Head\intrins.h"
#include ".\Head\LCD1602.h"
#include ".\Head\ds18b20.h"
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

float a = 0;
int main (void)
{
	Initial_LCD();
	
 	while(1)
	{
//		 Display_DS18B20_ROM();	   //��ʾһ��İ����DS18B20��ROM��

		/*****��ʾ����DS18B20������ͬһ�������ϵ��¶���ʾ������********/
//		 LocatPos(0,0);
		
	
		 a=rd_temperature_f();
		 WriteToLCD(0x30|(unsigned char)a/10);
		 WriteToLCD(0x30|(unsigned char)a%10);
		 Delay500ms();
	}
	return 0;
}