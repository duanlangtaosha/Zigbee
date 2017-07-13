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
 * \brief LCD12864 display module
 *
 * \note ʵ�ֺ��ֵĴ�ӡ��������printf������ӡ�����ֺͺ��֣�Ҳ������LCD12864_WriteSring
 *			 ֧�ֻ�ͼ�����㹤�̣��޷��ף�ע:��ʱ���Կ�����lcd12864_delayms���޸�
 *			 printf��֧��\n\r�Ļ��С�
 *			 ֧��sprintf��������С��ת�����ַ����������:1.2356����ת�����ַ������͵�1.2356
 *
 *       �����������,֧�ֲ�������IO�ڸ�ֵ��δ���ԣ�
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-10  duanlangtaosha, first implementation.
 * \endinternal
 */

#include "lcd12864.h"
//#include "dl_stdint.h"

/**
* \brief LCD12864��ʱms������
*
* \param[in] c : ��ʱ���ٺ��룬�����һ��uint��ֵ
*
* \return ��
*/
void lcd12864_delayms (uint c)
{
    uchar a,b;
	for (; c > 0; c--) {
	    for(b = 199; b > 0; b--) {
	        for(a = 1; a > 0; a--);
		}
	}
}

/**
* \brief LCD12864��æ����
*
* \param none
*
* \retval 0 : ��æʧ�ܣ��ȴ���ʱ
*         1 : ��æ�ɹ�����ʱLCD12864����
*/

uchar lcd12864_busy (void)
{
	uchar i = 0;

	LCD12864_RS = 0;  /* ѡ������ */
	LCD12864_RW = 1;	/* ѡ���ȡ */

	LCD12864_EN = 1;
	lcd12864_delayms(1);

//	while((LCD12864_DATAPORT & 0x80) == 0x80)	/* ����ȡ����ֵ */
	while((Read_LCD12864_DATAPORT & 0x80) == 0x80) {
		i++;
		if(i > 100) {
			LCD12864_EN = 0;
			return 0;	   /* �����ȴ�ʱ�䷵��0��ʾʧ�� */
		}
	}

	LCD12864_EN = 0;

	return 1;
}

/**
* \brief LCD12864дָ�����
*
* \param[in] cmd : ������Ӧ��ָ��
*
* \return ��
*/
void lcd12864_write_cmd(uchar cmd)
{
	uchar i;
	i = 0;
	while( lcd12864_busy() == 0) {
		lcd12864_delayms(1);
		i++;
		if( i > 100) {
			return;	   /* �����ȴ��˳� */
		}	
	}
	
	LCD12864_RS = 0;     /* ѡ������ */
	LCD12864_RW = 0;     /* ѡ��д�� */
	LCD12864_EN = 0;     /* ��ʼ��ʹ�ܶ� */

//	LCD12864_DATAPORT = cmd;   /* �������� */
	Write_LCD12864_DATAPORT(cmd);

	LCD12864_EN = 1;		   /* дʱ�� */
	lcd12864_delayms(5);
	LCD12864_EN = 0;    					
}

/**
* \brief LCD12864д���ݲ���
*
* \param[in] dat : ������Ӧ������
*
* \return ��
*/
void lcd12864_write_data(uchar dat)
{
	uchar i;
	i = 0;
	while ( lcd12864_busy() == 0) {
		lcd12864_delayms(1);
		i++;
		if( i > 100) {
			return;	   /* �����ȴ��˳� */
		}	
	}

	LCD12864_RS = 1;     /* ѡ������ */
	LCD12864_RW = 0;     /* ѡ��д�� */
	LCD12864_EN = 0;     /* ��ʼ��ʹ�ܶ� */

//	LCD12864_DATAPORT = dat;   /* �������� */
	Write_LCD12864_DATAPORT(dat);

	LCD12864_EN = 1;		   /* дʱ�� */
	lcd12864_delayms(5);
	LCD12864_EN = 0;    								
}

/**
* \brief LCD12864��ȡ����
*
* \param ��
*
* \return ��ȡ��������
*/
#ifdef LCD12864_PICTURE

uchar lcd12864_read_data (void)
{
	uchar i = 0, lcd12864_read_data = 0;

	i = 0;
	while ( lcd12864_busy() == 0) {
		lcd12864_delayms(1);
		i++;
		if ( i > 100) {
			return 0;	   /* �����ȴ��˳� */
		}	
	}

	LCD12864_RS = 1;       /* ѡ������ */
	LCD12864_RW = 1;
	LCD12864_EN = 0;
	lcd12864_delayms(1);  /* �ȴ� */

	LCD12864_EN = 1;
	lcd12864_delayms(1);
	lcd12864_read_data = Read_LCD12864_DATAPORT;
	LCD12864_EN = 0;

	return lcd12864_read_data;
}

#endif

/**
* \brief LCD12864��ʼ��
*
* \param	��
*
* \return ��
*/
void lcd12864_init (void)
{
	LCD12864_PSB = 1;	  /* ѡ�������� */
	LCD12864_RST = 1;	  /* ��λ */

	lcd12864_write_cmd(0x30);  /* ѡ�����ָ����� */
	lcd12864_write_cmd(0x0c);  /* ��ʾ�����ع��   */
	lcd12864_write_cmd(0x01);  /* ���LCD12864����ʾ���� */
}


/**
* \brief LCD12864�������ڻ�ͼģʽ�£�
*
* \note �ڻ�ͼģʽ�£�LCD12864��01H���������������Ҫ�Լ�дһ����������
*
* \param ��
*
* \return ��
*/
#ifdef LCD12864_PICTURE

void lcd12864_clear_screen (void)
{
	uchar i,j;

	lcd12864_write_cmd(0x34);	  /*������չָ� */

	for(i = 0; i < 32; i++) {		 /* ��ΪLCD��������32������д��ʮ���� */
	
		lcd12864_write_cmd(0x80+i);	/* ��д��������Y��ֵ */
		lcd12864_write_cmd(0x80);		/* ��д�������X��ֵ */
		for(j=0;j<32;j++) {		  /*
													  /* ��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
								            /* ��д��32����Ϊ��д�������ֽ�֮����������
													  */
			lcd12864_write_data(0xFF);	  /* ����1�����ԾͲ����ٴ�д���ַ�ˡ� */
		}
	}

	lcd12864_write_cmd(0x36);	 /* 0x36��չָ������򿪻�ͼ��ʾ */
	lcd12864_write_cmd(0x30);	 /* �ָ�����ָ� */
}

#endif

/**
* \brief LCD12864����
*
* \param[in] x : x����
* \param[in] y : y����
*
* \return ��
*/
#ifdef LCD12864_PICTURE

void lcd12864_drow_point (uchar x, uchar y)
{
	uint bt = 0, lcd12864_read_data = 0;
	uchar x_adr, y_adr, h_bit, l_bit;

	/* --Y�����һ��64������12864��Ϊ�ϰ������°���������32�����°����ˣ�--*/
	/* --��ַ���ִ��㿪ʼ�ˡ����Զ�32���������õ�Y������꣬Ȼ�����80H�Ļ���ַ--*/
	y_adr = 0x80 + y % 32;		//����Y��ĵ�ַ	
	
	/* --��Y����31ʱ���°�����X��ˮƽλ�õĵ�ַ���Ǵ�88H��ʼ�ģ���������16λһ--*/
	/* --����ַ�����Զ�16��ģ�����ϰ����ĵ�ַ�Ǵ�80H��ʼ��--*/ 
	if(y > 31) {
		x_adr = 0x88 + x / 16;	/* ����X��ĵ�ַ */
	}
	
	else {
		x_adr = 0x80 + x / 16;
	}
	
	/* --���X��ˮƽλ�ã��������ĵ�����16λ�����������һ��λ--*/
	/* --���Զ������������--*/
	bt = 0x8000 >> (x % 16);

	lcd12864_write_cmd(0x34);	  /* ������չָ� */

	lcd12864_write_cmd(y_adr);	  /* ��ȡ���ݵ�ʱ��Ҫ��д����ȡ���ݵĵ�ַ */
	lcd12864_write_cmd(x_adr);

	lcd12864_read_data();		  /* ��ȡ�ĵ�һ���ֽڲ�Ҫ */
	lcd12864_read_data = lcd12864_read_data();	  /* �ӵڶ����ֽڿ�ʼ���� */
	lcd12864_read_data <<= 8;
	lcd12864_read_data |= lcd12864_read_data();
	bt = bt | lcd12864_read_data;
	h_bit = bt >> 8;
	l_bit = bt;

	lcd12864_write_cmd(y_adr);	   /* д����ʱ�򣬴���д���ַ����Ϊ��ַ�Ѿ��ı� */
	lcd12864_write_cmd(x_adr);

	lcd12864_write_data(h_bit);
	lcd12864_write_data(l_bit);

	lcd12864_write_cmd(0x36);	   /* ����ʾ */
	lcd12864_write_cmd(0x30);	   /* �رջ�ͼָ�	 */
}
#endif


/*******************************************************************************
* �� �� ��         : lcd12864_clear_screen
* ��������		   : �ڻ�ͼģʽ�£�LCD12864��01H���������������Ҫ�Լ�дһ����
*                  * ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

/**
* \brief LCD12864��ͼ
*
* \param[in] a : ͼƬ����ĵ�ַ
*
* \return ��
*/
#ifdef LCD12864_PICTURE

void LCD12864_DrowPic (uchar *a)	   
{

	unsigned char i,j;

	lcd12864_clear_screen();

	lcd12864_write_cmd(0x34);  /* ������չָ������رջ�ͼ��ʾ��*/
	for(i=0;i<32;i++) {			  /* ��ΪLCD��������32������д��ʮ���� */	
		lcd12864_write_cmd(0x80+i);		/* ��д��������Y��ֵ */
		lcd12864_write_cmd(0x80);		  /* ��д�������X��ֵ */
		for(j = 0; j < 16; j++) {		/* ��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
								                 * ��д��32����Ϊ��д�������ֽ�֮����������
														     * ����1�����ԾͲ����ٴ�д���ַ�ˡ�
														     */
		  lcd12864_write_data(*a); 	      
			a++;
		}
	}  
	for(i = 0; i < 32; i++)	{		  /* ��ΪLCD��������32������д��ʮ���� */
	
		
		lcd12864_write_cmd(0x80+i);		  /* ��д��������Y��ֵ */
		lcd12864_write_cmd(0x88);		  /* ��д�������X��ֵ */
		for(j = 0; j < 16; j++)	{	  /* ��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
								             * ��д��32����Ϊ��д�������ֽ�֮����������
														 * ����1�����ԾͲ����ٴ�д���ַ��
														 */
			
			lcd12864_write_data(*a); 	      
			a++;
		}
	}	

	lcd12864_write_cmd(0x36);	   /* ����ʾ */
	lcd12864_write_cmd(0x30);  	 /* ת�ػ���ָ� */
}

#endif

/**
* \brief LCD12864���ô���
*
* \note �����ڻ���ָ��ģʽ��������ʾ���ꡣע�⣺x�������У�y��������
*
* \param[in] x : �����꣨�ܹ���4��, x = 0, 1, 2, 3��
* \param[in] y : ������
*
* \return ��
*/
void lcd12864_set_window(uchar x, uchar y)
{
	uchar pos = 0;

	/* ��һ�еĵ�ַ��80H */
	if (x == 0) {
		x = 0x80;
	}
		
	/* �ڶ��еĵ�ַ��90H */
	else if (x == 1) {
		x = 0x90;	
	}
	
	/* �����еĵ�ַ��88H */
	else if (x == 2) {
		x = 0x88;
	}
	
	else if (x == 3) {
		x = 0x98;
	}
	
	pos = x + y;
	lcd12864_write_cmd(pos);
}

/*
*********************************************************************************************************
*	�� �� ��: putchar
*	����˵��: �ض��� putchar ������ ��������ʹ��printf������ӡ�����LCD12864
*	��    ��: �ȴ����͵��ַ�
*	�� �� ֵ: �����ѷ��͵��ַ�
*********************************************************************************************************
*/

/**
* \brief printf��������װ�أ�printf��Ĵ�ӡ��LCD12864��
*
* \note �ض��� putchar ������ ��������ʹ��printf������ӡ�����LCD12864
*
* \param[in] c : �ַ�
*
* \return ��Ӧ���ַ�
*/
char putchar(char c)
{
	/* ����һ���ַ� c ��UART1 */
	lcd12864_write_data(c);

	return (c);
}

/*
*********************************************************************************************************
*	�� �� ��: getchar
*	����˵��: �ض���C���е� getchar ����,��������ʹ��scanff�����Ӵ�����������
*	��    ��: ��
*	�� �� ֵ: �����Ѷ������ַ�
*********************************************************************************************************
*/
//char getchar(void)
//{
//	//return UART_GetByte();
//	return 0;
//}


/**
* \brief LCD12864д�ַ�������
*
* \note ֱ��д���ַ��������������飬��lcd12864_write_string("������ɳ");
*
* \param[in] string : �ַ�����ַ
*
* \return ��
*/
void lcd12864_write_string (char *string)
{
	char *p = string;
	while (*p != '\0') {
		lcd12864_write_data(*p);
		p++;
	}
}

/**
* \brief LCD12864д��������
*
* \note ����һ��������������ӡ��LCD12864
*
* \param[in] value : ���˵�����
*
* \return ��
*/
#ifdef LCD12864_WRITE_UINT_STRING
void lcd12864_write_uint_string(unsigned int value )
{
	char uintbuf[10] = {0};
	sprintf(uintbuf, "%d", (int)value);
	lcd12864_write_string(uintbuf);
}
#endif

/*******************************************************************************
* �� �� ��         : LCD12864_WriteFloatString
* ��������		   	: ����һ��Float�͵�С����ӡ��LCD12864
* ��    ��         	: float value
* ��    ��         	: ��
*******************************************************************************/

/**
* \brief LCD12864д����������
*
* \note ����һ��foat�͵�С����ӡ��LCD12864
*
* \param[in] value : ���˸�����
*
* \return ��
*/
#ifdef LCD12864_WRITE_FLOAT_STRING
void lcd12864_write_float_string(float value)
{
	char floatbuf[10] = {0};
	char *p = floatbuf;
	sprintf(floatbuf,"%.2f", (float)value);		//ȡС�������λ
	lcd12864_write_string(floatbuf);
	
}
#endif
