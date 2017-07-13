/*
*	Source	Name	:	DS18B20 C语言模块
*	Function		:	功能是读取DS18B20的温度，支持一个总线上挂载多个DS18B20，支持读取一个DS18B20的ROM
*	Test	Chip	:	STC12C5A60S2
*	OSC				:	12M
*	作者			:	断浪淘沙
*	Time			:	2016-4-24
*
*/
#include "./Head/stc12c5a60s2.h"
#include "./Head/intrins.h"
#include "./Head/LCD1602.H"
#include "./Head/ds18b20.h"
unsigned char g_DS18B20_ROM_Buff[8]={0}; //可以把一个新的不知道ROM的18B20的ROM数据读取到该数组中

/**********下面这两个ROM是我做测试时测出来的两个18B20的ROM，如果遇到新的了要更改*****/
unsigned char code Code_DS18B20_ROM1[8]={0x28,0xfb,0xa8,0x45,0x06,0x00,0x00,0x29};	 //这个没有写好，笨办法，要先检测出DS18B20的ROM，填入这个数组中才有效
unsigned char code Code_DS18B20_ROM2[8]={0x28,0x4a,0xf9,0x33,0x05,0x00,0x00,0x04};


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



void Match_ROM(unsigned char num_ds18b20)		   //匹配序列号
{
	unsigned char j=0;

   	Write_DS18B20(0x55);
	if(num_ds18b20==1)
	{
		for(j=0;j<8;j++)
		{
			Write_DS18B20(Code_DS18B20_ROM1[j]);
		}
	}
	if(num_ds18b20==2)
	{
		for(j=0;j<8;j++)
		{
			Write_DS18B20(Code_DS18B20_ROM2[j]);
		}
	}
}

/****现在可以读取两个DS18B20，1读取第一个，2读取第2个**********/
float Read_DS18B20_Num(unsigned int num_ds18b20)
{
	unsigned char low=0,high=0;
	unsigned int temp=0;
	float temperature=0.0;
	
	init_ds18b20();	      //初始化
	Write_DS18B20(0xcc);  //跳过
	init_ds18b20();		  
	if(num_ds18b20==1)
	{
		Match_ROM(1);	   //	 匹配rom1
	}
	if(num_ds18b20==2)
	{
		Match_ROM(2);	   //	  匹配rom2
	}

	Write_DS18B20(0x44);
	init_ds18b20();
	Write_DS18B20(0xcc);
	init_ds18b20();
	if(num_ds18b20==1)
	{
		Match_ROM(1);	   //
	}
	if(num_ds18b20==2)
	{
		Match_ROM(2);	   //
	}

	Write_DS18B20(0xbe);  //启动读暂存器。读内部ram中9字节温度数据
	low=Read_DS18B20();
	high=Read_DS18B20();

/** 精度为0.0625摄氏度 */  
	temp = (high&0x0f);
	temp <<= 8;
	temp |= low;
	temperature = temp*0.0625;

//	value=th;
//	value=value<<8;
//	value=value|tl;
//	if(th < 0x80)
//	{	fg=0;  }
//
//	
//	if(th>= 0x80)
//	{	fg=1;
//	 	
//		value=~value+1;
//	}
//		value=value*(0.0625*10);
	    return temperature;	
}

/*****把DS18B20的ROM显示在LCD1602上*********/
void Display_DS18B20_ROM(void)
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


