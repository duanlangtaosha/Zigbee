#include "stc12C5A60S2.h"
#include "intrins.h"
#include "ds1302.h"

/** \brief 初始化时间		 秒     分	 时	  日	 月,星期	 年 */
unsigned char code __g_initial_time[7] = {0x55, 0x33, 0x10, 0x26, 0x05, 5, 0x17};

//unsigned char data timeNow[7];

void delay1us()		//@12.000MHz
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

/**
* \brief SPI写单字节函数
*
* \param[in] dat : 要写入的数据
*
* \return 无
*         
*/
void ds1302_write_spi (unsigned char dat)
{
	unsigned char i = 0;
//	EA=0;
	for (i = 0; i < 8; i++) {
		
		
		DS1302_SCLK = 0;
		DS1302_IO = dat & 0x01;
		
		delay1us();
		dat >>= 1;
		DS1302_SCLK = 1;
		
		delay1us();
		
		
	}	
//	EA=1;
}

/**
* \brief SPI读单字节函数
*
* \param[in] dat : 要写入的数据
*
* \return 无
*         
*/
unsigned char ds1302_read_spi (void)
{
	unsigned char i = 0, dat = 0;
//	EA=0;
	for (i = 0; i < 8; i++) {
	
		DS1302_SCLK = 0;
		dat >>= 1;
		delay1us();
		if (DS1302_IO) {
			dat|= 0x80;
		}
		
		DS1302_SCLK = 1;
		delay1us();
	}
//	EA=1;
   return dat;
}

/**
* \brief 向DS1302相应的地址写入数据
*
* \param[in] addr : 要写入的地址
* \param[in] dat  : 要写入的数据
*
* \return 无
*         
*/
void ds1302_write_data(unsigned char addr, unsigned char dat)
{
	DS1302_CE   = 0;
		_nop_();
	DS1302_SCLK = 0;

	delay1us();
	DS1302_CE = 1;
	
	ds1302_write_spi(addr);
	ds1302_write_spi(dat);
	
	
//	DS1302_SCLK = 1;
	DS1302_CE   = 0;
	
	delay1us();
	delay1us();
	delay1us();
	delay1us();	
}

/**
* \brief 读取DS1302相应地址中的值
*
* \param[in] addr : 要读取的地址
*
* \return 读出的数据
*         
*/
unsigned char ds1302_read_data (unsigned char addr)
{
	unsigned char dat = 0;
  DS1302_CE   = 0;
	_nop_();
	_nop_();
	DS1302_SCLK = 0;
	delay1us();
	DS1302_CE = 1;
	_nop_();
	_nop_();
	
	ds1302_write_spi(addr);
	dat = ds1302_read_spi();
	
	DS1302_CE   = 0;
	_nop_();
	_nop_();
	DS1302_SCLK = 1;
	
	delay1us();
	delay1us();
	delay1us();
	delay1us();	
	return dat;
}

/**
* \brief 设置DS1302的初始时间
*
* \param[in] p_init_time : 初始化时间数组
*
* \return 无
*         
*/
void set_ds1302_initial_time (unsigned char code *p_init_time)
{
	unsigned char i = 0;

	ds1302_write_data(0x8e, 0x00);	/* 写保护关闭 */
	for(i = 0; i < 7; i++) {
	
		ds1302_write_data(0x80 + i * 2, p_init_time[i]);
	}
	ds1302_write_data(0x8e, 0x80);	/* 写保护开	*/

}

void set_ds1302_time (unsigned char  *p_init_time)
{
	unsigned char i = 0;

	ds1302_write_data(0x8e, 0x00);	/* 写保护关闭 */
	for(i = 0; i < 7; i++) {
	
		ds1302_write_data(0x80 + i * 2, p_init_time[i]);
	}
	ds1302_write_data(0x8e, 0x80);	/* 写保护开	*/

}


/**
* \brief 获取DS1302的时钟值
*
* \param p_get_time ：一个7字节数组，存储 D[0]秒 + D[1]分 + D2时+ D[3]日 + 
*                                         D[4]月 + D[5]星期 + D[6]年
*
* \return 无
*         
*/
void get_ds1302_time (unsigned char *p_get_time)
{
	unsigned char i = 0; 
	
//	EA = 0;							  /* 这个很重要 */
		
	for (i = 0; i < 7; i++) {
	
		p_get_time[i] = ds1302_read_data(0x81 + i * 2);
		ds1302_read_data(0x81 + i * 2);	   /* 执行一个空操作 */
	}
//	EA = 1;
}

/**
* \brief 初始化DS1302
*
* \param 无
*
* \return 无
*         
*/
void init_ds1302(void)
{
	unsigned char temp = 0;
	DS1302_CE   = 0;
	DS1302_SCLK = 0;
	ds1302_write_data(0x8e, 0x00);
	ds1302_write_data(0x90, 0xa6);
//	ds1302_write_data(0x80,0x00);	 //开时钟	这个加上则秒每次开电都是从0开始
	ds1302_write_data(0x8e, 0x80);
	
	temp = ds1302_read_data(81);
	if(temp & 0x80) {
	 	set_ds1302_initial_time(__g_initial_time);
	}
	
}
