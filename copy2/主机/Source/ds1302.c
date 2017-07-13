#include "stc12C5A60S2.h"
#include "intrins.h"
#include "ds1302.h"

/** \brief ��ʼ��ʱ��		 ��     ��	 ʱ	  ��	 ��,����	 �� */
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
* \brief SPIд���ֽں���
*
* \param[in] dat : Ҫд�������
*
* \return ��
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
* \brief SPI�����ֽں���
*
* \param[in] dat : Ҫд�������
*
* \return ��
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
* \brief ��DS1302��Ӧ�ĵ�ַд������
*
* \param[in] addr : Ҫд��ĵ�ַ
* \param[in] dat  : Ҫд�������
*
* \return ��
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
* \brief ��ȡDS1302��Ӧ��ַ�е�ֵ
*
* \param[in] addr : Ҫ��ȡ�ĵ�ַ
*
* \return ����������
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
* \brief ����DS1302�ĳ�ʼʱ��
*
* \param[in] p_init_time : ��ʼ��ʱ������
*
* \return ��
*         
*/
void set_ds1302_initial_time (unsigned char code *p_init_time)
{
	unsigned char i = 0;

	ds1302_write_data(0x8e, 0x00);	/* д�����ر� */
	for(i = 0; i < 7; i++) {
	
		ds1302_write_data(0x80 + i * 2, p_init_time[i]);
	}
	ds1302_write_data(0x8e, 0x80);	/* д������	*/

}

void set_ds1302_time (unsigned char  *p_init_time)
{
	unsigned char i = 0;

	ds1302_write_data(0x8e, 0x00);	/* д�����ر� */
	for(i = 0; i < 7; i++) {
	
		ds1302_write_data(0x80 + i * 2, p_init_time[i]);
	}
	ds1302_write_data(0x8e, 0x80);	/* д������	*/

}


/**
* \brief ��ȡDS1302��ʱ��ֵ
*
* \param p_get_time ��һ��7�ֽ����飬�洢 D[0]�� + D[1]�� + D2ʱ+ D[3]�� + 
*                                         D[4]�� + D[5]���� + D[6]��
*
* \return ��
*         
*/
void get_ds1302_time (unsigned char *p_get_time)
{
	unsigned char i = 0; 
	
//	EA = 0;							  /* �������Ҫ */
		
	for (i = 0; i < 7; i++) {
	
		p_get_time[i] = ds1302_read_data(0x81 + i * 2);
		ds1302_read_data(0x81 + i * 2);	   /* ִ��һ���ղ��� */
	}
//	EA = 1;
}

/**
* \brief ��ʼ��DS1302
*
* \param ��
*
* \return ��
*         
*/
void init_ds1302(void)
{
	unsigned char temp = 0;
	DS1302_CE   = 0;
	DS1302_SCLK = 0;
	ds1302_write_data(0x8e, 0x00);
	ds1302_write_data(0x90, 0xa6);
//	ds1302_write_data(0x80,0x00);	 //��ʱ��	�����������ÿ�ο��綼�Ǵ�0��ʼ
	ds1302_write_data(0x8e, 0x80);
	
	temp = ds1302_read_data(81);
	if(temp & 0x80) {
	 	set_ds1302_initial_time(__g_initial_time);
	}
	
}
