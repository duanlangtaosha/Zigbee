#include "./Head/stc12c5a60s2.h"
#include "./Head/intrins.h"
#include "./Head/LCD1602.H"
#include "./Head/ds18b20.h"
unsigned char g_DS18B20_ROM_Buff[8]={0};
//单总线延时函数
//#ifndef STC12  
//void Delay_OneWire(unsigned int t)  //STC89C52RC
//{
//	while(t--);
//}
//#else
void Delay_OneWire(unsigned int t)  //STC12C5260S2
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}
//#endif

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		PIN_DS18B20= 0;
		PIN_DS18B20 = dat&0x01;
		Delay_OneWire(5);
		PIN_DS18B20 = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		PIN_DS18B20 = 0;
		dat >>= 1;
		PIN_DS18B20 = 1;
		if(PIN_DS18B20)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	PIN_DS18B20 = 1;
  	Delay_OneWire(12);
  	PIN_DS18B20 = 0;
  	Delay_OneWire(80); // 延时大于480us
  	PIN_DS18B20 = 1;
  	Delay_OneWire(10);  // 14
  	initflag = PIN_DS18B20;     // initflag等于1初始化失败
  	Delay_OneWire(5);
  
  	return initflag;
}

//DS18B20温度采集程序：整数
// unsigned char rd_temperature(void)
// {
//     unsigned char low,high;
//   	char temp;
//   
//   	init_ds18b20();
//   	Write_DS18B20(0xCC);
//   	Write_DS18B20(0x44); //启动温度转换
//   	Delay_OneWire(200);

//   	init_ds18b20();
//   	Write_DS18B20(0xCC);
//   	Write_DS18B20(0xBE); //读取寄存器

//   	low = Read_DS18B20(); //低字节
//   	high = Read_DS18B20(); //高字节
// /** 精度为1摄氏度 */  
//   	temp = high<<4;
//   	temp |= (low>>4);
//   
//   	return temp;
// }

//DS18B20温度采集程序：浮点数
float rd_temperature_f(void)
{
    unsigned int temp;
	float temperature;
    unsigned char low,high;
  
  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0x44); //启动温度转换
  	Delay_OneWire(200);

  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0xBE); //读取寄存器

  	low = Read_DS18B20(); //低字节
  	high = Read_DS18B20(); //高字节
/** 精度为0.0625摄氏度 */  
	temp = (high&0x0f);
	temp <<= 8;
	temp |= low;
	temperature = temp*0.0625;
  
  	return temperature;
}
//void DS18B20_Mode(void)
//{
//		unsigned char temperature=0;
//		unsigned char key_value=0;
//		Command_LCD(LCD_CLEAR_SCREEN,CHK_BUSY);	// 清屏 
//		LocatPos(0,0);
//		String_LCD("Mode:Temperature");			   //距离	   第2行
//		LocatPos(0,1);
//		String_LCD("CurrentTemp:");		
//		LocatPos(15,1);
//		String_LCD("C");
//		while(1)
//		{
//		   if(g_DS18B20_Process)
//			{
//				g_DS18B20_Process = 0;
//				temperature = (unsigned char)rd_temperature_f();  //读温度         
//				LocatPos(12,1);
//				WriteToLCD(0x30|temperature/10);
//				WriteToLCD(0x30|temperature%10);
//	        }
//			key_value=keyscan(); 
//		  if(EN_KEY0==key_value)
//		  {
//		  	g_Mode=EN_TDS_Mode;
//			break;
//		  }
//		}	
//}

/*****把DS18B20的ROM读入进一个数组里面*********/
void Read_DS18B20_ROM(void)					 
{
	unsigned char i;
	init_ds18b20();
	Write_DS18B20(0x33);
	for(i=0;i<8;i++)
	{
		g_DS18B20_ROM_Buff[i]=Read_DS18B20();  //保存序列号到一个数组中去
	}
}
/*****把DS18B20的ROM显示在LCD1602上*********/
void Display_DS18B20_ROM()
{
	unsigned char i=0,temp=0;
	Read_DS18B20_ROM();
	Command_LCD(0x80,CHK_BUSY);				    
	for(i=0;i<8;i++)
	{
		temp=g_DS18B20_ROM_Buff[i]>>4;
		if(temp<10)
		{
			WriteToLCD(0x30+temp);
		}
		else
		{
			 WriteToLCD(0x37+temp);
		}
		temp=g_DS18B20_ROM_Buff[i]&0x0f;
		if(temp<10)
		{
			WriteToLCD(0x30+temp);
		}
		else
		{
			 WriteToLCD(0x37+temp);
		}
	}	
}


