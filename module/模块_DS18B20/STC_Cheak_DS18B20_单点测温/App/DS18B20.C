/*******************************************************************************
*                            STC51 MCU Module
*                       ---------------------------
*
* Copyright (c) 2001-2012 duanlantaosha
*
*	Chip			:	STC89C52RC&STC12C5A60S2
*	OSC				:	11.0592M & 12M
*	Made	By	:	duanlangtaosha
* QQ        : 891650837
* e-mail:   : 891650837@qq.com
*******************************************************************************/

/**
 * \file
 * \brief DS18B20接口程序
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  duanlangtaosha, first implementation.
 * \endinternal
 */

#include "stc12c5a60s2.h"
#include "intrins.h"
#include "LCD1602.H"
#include "ds18b20.h"

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

/* 通过单总线向DS18B20写一个字节 */


/**
* \brief 向DS18B20中写入数据
*
* \param[in] dat : 要写入的数据
*
* \return 无
*
*/
void write_ds18b20(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++) {
		PIN_DS18B20 = 0;
		PIN_DS18B20 = dat & 0x01;
		Delay_OneWire(5);
		PIN_DS18B20 = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

/**
* \brief 从DS18B20读取一个字节
*
* \param 无
*
* \return 读出的数据
*
*/
unsigned char read_ds18b20 (void)
{
	unsigned char i;
	unsigned char dat;
  
	for (i = 0; i < 8; i++) {
	
		PIN_DS18B20 = 0;
		dat >>= 1;
		PIN_DS18B20 = 1;
		if (PIN_DS18B20) {
		
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

/**
* \brief DS18B20初始化
*
* \param 无
*
* \return 返回初始化的状态
*
*/
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	PIN_DS18B20 = 1;
  	Delay_OneWire(12);
  	PIN_DS18B20 = 0;
  	Delay_OneWire(80); /* 延时大于480us */
  	PIN_DS18B20 = 1;
  	Delay_OneWire(10);  /* 14 */
  	initflag = PIN_DS18B20;     /* initflag等于1初始化失败 */
  	Delay_OneWire(5);
  
  	return initflag;
}

//DS18B20温度采集程序：整数

/**
* \brief 读取DS1820的温度数据(整数)
*
* \param 无
*
* \return 返回带小数的温度,步进是1℃
*
*/
 unsigned char rd_temperature_i(void)
 {
    unsigned char low = 0, high = 0;
   	char temp = 0;
   
   	init_ds18b20();
   	write_ds18b20(0xCC);
   	write_ds18b20(0x44); //启动温度转换
   	Delay_OneWire(200);

   	init_ds18b20();
   	write_ds18b20(0xCC);
   	write_ds18b20(0xBE); //读取寄存器

   	low = read_ds18b20(); //低字节
   	high = read_ds18b20(); //高字节
	 
		/* 精度为1摄氏度 */  
   	temp  = high << 4;
   	temp |= (low >> 4);
   
   	return temp;
 }

/**
* \brief 读取DS1820的温度数据(浮点数)
*
* \param 无
*
* \return 返回带小数的温度,步进是0.0625℃
*
*/
float rd_temperature_f (void)
{
    unsigned int temp = 0;
	  float temperature = 0.0;
    unsigned char low = 0, high = 0;
  
  	init_ds18b20();
  	write_ds18b20(0xCC);
  	write_ds18b20(0x44); /* 启动温度转换 */
  	Delay_OneWire(200);

  	init_ds18b20();
  	write_ds18b20(0xCC);
  	write_ds18b20(0xBE); /* 读取寄存器 */

  	low = read_ds18b20(); /* 低字节 */
  	high = read_ds18b20(); /* 高字节 */
	
	  /* 精度为0.0625摄氏度 */  
		temp = (high&0x0f);
		temp <<= 8;
		temp |= low;
		temperature = temp * 0.0625;
  
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


/**
* \brief 检测DS18B20的ROM值（独一无二光刻64bit的设备名）
*
* \param[out] p_buf : 保持8个字节的ROM的值（放到一个8字节的数组中）
*
* \return 无
*
*/
void read_ds18b20_rom (unsigned char *p_buf)					 
{
	unsigned char i = 0;
	
	init_ds18b20();
	write_ds18b20(0x33);
	
	for (i = 0; i < 8; i++) {
		p_buf[i] = read_ds18b20();  /* 保存序列号到一个数组中去 */
	}
}


/**
* \brief 匹配ROM,如果是多点通信就要用ROM值区分设备
*
* \param[in] num_ds18b20 : 输入设备号，如果有多个设备就从1开始
* \param[in] p_rom_buf   : 存储已知的DS18B20的ROM值，放在一个二维数组中
*
* \note test[x][8] = { {0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??}, {}, {}.....}
*
* \return 无
*
*/
void match_rom(unsigned char num_ds18b20, unsigned char *p_rom_buf)		   
{
	unsigned char j = 0, i = 0;

   	write_ds18b20(0x55);
	
	for (i = 0; i < num_ds18b20; i++) {
		for(j = 0; j < 8; j++) {
		
			write_ds18b20(p_rom_buf[j]);
		}
	}
//	if(num_ds18b20==1)
//	{
//		for(j=0;j<8;j++)
//		{
//			write_ds18b20(Code_DS18B20_ROM1[j]);
//		}
//	}
//	if(num_ds18b20==2)
//	{
//		for(j=0;j<8;j++)
//		{
//			write_ds18b20(Code_DS18B20_ROM2[j]);
//		}
//	}
}

///****现在可以读取两个DS18B20，1读取第一个，2读取第2个**********/
//float read_ds18b20_Num(unsigned int num_ds18b20)
//{
//	unsigned char low=0,high=0;
//	unsigned int temp=0;
//	float temperature=0.0;
//	
//	init_ds18b20();	      //初始化
//	write_ds18b20(0xcc);  //跳过
//	init_ds18b20();		  
//	if (num_ds18b20 == 1) {
//	
//		Match_ROM(1);	   //	 匹配rom1
//	}
//	if(num_ds18b20 == 2) {
//	
//		Match_ROM(2);	   //	  匹配rom2
//	}

//	write_ds18b20(0x44);
//	init_ds18b20();
//	write_ds18b20(0xcc);
//	init_ds18b20();
//	if (num_ds18b20 == 1) {
//	
//		Match_ROM(1);	   //
//	}
//	if (num_ds18b20 == 2)
//	{
//		Match_ROM(2);	   //
//	}

//	write_ds18b20(0xbe);  //启动读暂存器。读内部ram中9字节温度数据
//	low=read_ds18b20();
//	high=read_ds18b20();

///** 精度为0.0625摄氏度 */  
//	temp = (high & 0x0f);
//	temp <<= 8;
//	temp |= low;
//	temperature = temp * 0.0625;

//	return temperature;	
//}

/*****把DS18B20的ROM显示在LCD1602上*********/
void Display_DS18B20_ROM(void)
{
	unsigned char i=0,temp=0;
	read_ds18b20_rom(g_DS18B20_ROM_Buff);
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


