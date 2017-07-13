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

//extern void Read_DS18B20_ROM(void)	;
//extern void Display_DS18B20_ROM();
//extern float  Read_DS18B20_Num(unsigned int num_ds18b20);
float a=0;
int main(void)
{
	Initial_LCD();
 	while(1)
	{
//		 Display_DS18B20_ROM();	   //显示一个陌生的DS18B20的ROM的

		/*****显示两个DS18B20挂载在同一根总线上的温度显示的数据********/
		LocatPos(0,0);
//		a=Read_DS18B20_Num(1);
//		WriteToLCD(0x30|(unsigned char)a/10);
//		WriteToLCD(0x30|(unsigned char)a%10);
//		LocatPos(0,1);
//		a=Read_DS18B20_Num(2);
//		WriteToLCD(0x30|(unsigned char)a/10);
//		WriteToLCD(0x30|(unsigned char)a%10);
		
	
		 a=rd_temperature_f();
		 WriteToLCD(0x30|(unsigned char)a/10);
		 WriteToLCD(0x30|(unsigned char)a%10);
		 Delay500ms();
	}
	return 0;
}