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
 * \note 实现汉字的打印，可以用printf函数打印出数字和汉字，也可以用LCD12864_WriteSring
 *			 支持画图，画点工程，无反白，注:延时不对可以在lcd12864_delayms中修改
 *			 printf不支持\n\r的换行。
 *			 支持sprintf把整数和小数转换成字符串输出。如:1.2356可以转换成字符串类型的1.2356
 *
 *       并行输入输出,支持不连续的IO口赋值（未测试）
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-10  duanlangtaosha, first implementation.
 * \endinternal
 */

#include "lcd12864.h"
//#include "dl_stdint.h"

/**
* \brief LCD12864延时ms级函数
*
* \param[in] c : 延时多少毫秒，最大是一个uint的值
*
* \return 无
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
* \brief LCD12864判忙操作
*
* \param none
*
* \retval 0 : 判忙失败，等待超时
*         1 : 判忙成功，此时LCD12864空闲
*/

uchar lcd12864_busy (void)
{
	uchar i = 0;

	LCD12864_RS = 0;  /* 选择命令 */
	LCD12864_RW = 1;	/* 选择读取 */

	LCD12864_EN = 1;
	lcd12864_delayms(1);

//	while((LCD12864_DATAPORT & 0x80) == 0x80)	/* 检测读取到的值 */
	while((Read_LCD12864_DATAPORT & 0x80) == 0x80) {
		i++;
		if(i > 100) {
			LCD12864_EN = 0;
			return 0;	   /* 超过等待时间返回0表示失败 */
		}
	}

	LCD12864_EN = 0;

	return 1;
}

/**
* \brief LCD12864写指令操作
*
* \param[in] cmd : 传人相应的指令
*
* \return 无
*/
void lcd12864_write_cmd(uchar cmd)
{
	uchar i;
	i = 0;
	while( lcd12864_busy() == 0) {
		lcd12864_delayms(1);
		i++;
		if( i > 100) {
			return;	   /* 超过等待退出 */
		}	
	}
	
	LCD12864_RS = 0;     /* 选择命令 */
	LCD12864_RW = 0;     /* 选择写入 */
	LCD12864_EN = 0;     /* 初始化使能端 */

//	LCD12864_DATAPORT = cmd;   /* 放置数据 */
	Write_LCD12864_DATAPORT(cmd);

	LCD12864_EN = 1;		   /* 写时序 */
	lcd12864_delayms(5);
	LCD12864_EN = 0;    					
}

/**
* \brief LCD12864写数据操作
*
* \param[in] dat : 传人相应的数据
*
* \return 无
*/
void lcd12864_write_data(uchar dat)
{
	uchar i;
	i = 0;
	while ( lcd12864_busy() == 0) {
		lcd12864_delayms(1);
		i++;
		if( i > 100) {
			return;	   /* 超过等待退出 */
		}	
	}

	LCD12864_RS = 1;     /* 选择数据 */
	LCD12864_RW = 0;     /* 选择写入 */
	LCD12864_EN = 0;     /* 初始化使能端 */

//	LCD12864_DATAPORT = dat;   /* 放置数据 */
	Write_LCD12864_DATAPORT(dat);

	LCD12864_EN = 1;		   /* 写时序 */
	lcd12864_delayms(5);
	LCD12864_EN = 0;    								
}

/**
* \brief LCD12864读取数据
*
* \param 无
*
* \return 读取到的数据
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
			return 0;	   /* 超过等待退出 */
		}	
	}

	LCD12864_RS = 1;       /* 选择命令 */
	LCD12864_RW = 1;
	LCD12864_EN = 0;
	lcd12864_delayms(1);  /* 等待 */

	LCD12864_EN = 1;
	lcd12864_delayms(1);
	lcd12864_read_data = Read_LCD12864_DATAPORT;
	LCD12864_EN = 0;

	return lcd12864_read_data;
}

#endif

/**
* \brief LCD12864初始化
*
* \param	无
*
* \return 无
*/
void lcd12864_init (void)
{
	LCD12864_PSB = 1;	  /* 选择并行输入 */
	LCD12864_RST = 1;	  /* 复位 */

	lcd12864_write_cmd(0x30);  /* 选择基本指令操作 */
	lcd12864_write_cmd(0x0c);  /* 显示开，关光标   */
	lcd12864_write_cmd(0x01);  /* 清除LCD12864的显示内容 */
}


/**
* \brief LCD12864清屏（在画图模式下）
*
* \note 在画图模式下，LCD12864的01H命令不能清屏，所以要自己写一个清屏函数
*
* \param 无
*
* \return 无
*/
#ifdef LCD12864_PICTURE

void lcd12864_clear_screen (void)
{
	uchar i,j;

	lcd12864_write_cmd(0x34);	  /*开启拓展指令集 */

	for(i = 0; i < 32; i++) {		 /* 因为LCD有纵坐标32格所以写三十二次 */
	
		lcd12864_write_cmd(0x80+i);	/* 先写入纵坐标Y的值 */
		lcd12864_write_cmd(0x80);		/* 再写入横坐标X的值 */
		for(j=0;j<32;j++) {		  /*
													  /* 横坐标有16位，每位写入两个字节的的数据，也
								            /* 就写入32次以为当写入两个字节之后横坐标会自
													  */
			lcd12864_write_data(0xFF);	  /* 动加1，所以就不用再次写入地址了。 */
		}
	}

	lcd12864_write_cmd(0x36);	 /* 0x36扩展指令里面打开绘图显示 */
	lcd12864_write_cmd(0x30);	 /* 恢复基本指令集 */
}

#endif

/**
* \brief LCD12864画点
*
* \param[in] x : x坐标
* \param[in] y : y坐标
*
* \return 无
*/
#ifdef LCD12864_PICTURE

void lcd12864_drow_point (uchar x, uchar y)
{
	uint bt = 0, lcd12864_read_data = 0;
	uchar x_adr, y_adr, h_bit, l_bit;

	/* --Y轴最大一共64，但是12864分为上半屏和下半屏，超过32就是下半屏了，--*/
	/* --地址得又从零开始了。所以对32求余数，得到Y轴的坐标，然后加上80H的基地址--*/
	y_adr = 0x80 + y % 32;		//计算Y轴的地址	
	
	/* --当Y大于31时是下半屏，X（水平位置的地址）是从88H开始的，而且它是16位一--*/
	/* --个地址，所以对16求模。而上半屏的地址是从80H开始的--*/ 
	if(y > 31) {
		x_adr = 0x88 + x / 16;	/* 计算X轴的地址 */
	}
	
	else {
		x_adr = 0x80 + x / 16;
	}
	
	/* --求出X（水平位置），点亮的点是在16位数据里面的哪一个位--*/
	/* --所以对这个数求余数--*/
	bt = 0x8000 >> (x % 16);

	lcd12864_write_cmd(0x34);	  /* 进入拓展指令集 */

	lcd12864_write_cmd(y_adr);	  /* 读取数据的时候要先写入所取数据的地址 */
	lcd12864_write_cmd(x_adr);

	lcd12864_read_data();		  /* 读取的第一个字节不要 */
	lcd12864_read_data = lcd12864_read_data();	  /* 从第二个字节开始接收 */
	lcd12864_read_data <<= 8;
	lcd12864_read_data |= lcd12864_read_data();
	bt = bt | lcd12864_read_data;
	h_bit = bt >> 8;
	l_bit = bt;

	lcd12864_write_cmd(y_adr);	   /* 写入点的时候，从新写入地址，因为地址已经改变 */
	lcd12864_write_cmd(x_adr);

	lcd12864_write_data(h_bit);
	lcd12864_write_data(l_bit);

	lcd12864_write_cmd(0x36);	   /* 开显示 */
	lcd12864_write_cmd(0x30);	   /* 关闭绘图指令集	 */
}
#endif


/*******************************************************************************
* 函 数 名         : lcd12864_clear_screen
* 函数功能		   : 在画图模式下，LCD12864的01H命令不能清屏，所以要自己写一个清
*                  * 屏函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

/**
* \brief LCD12864画图
*
* \param[in] a : 图片数组的地址
*
* \return 无
*/
#ifdef LCD12864_PICTURE

void LCD12864_DrowPic (uchar *a)	   
{

	unsigned char i,j;

	lcd12864_clear_screen();

	lcd12864_write_cmd(0x34);  /* 开启扩展指令集，并关闭画图显示。*/
	for(i=0;i<32;i++) {			  /* 因为LCD有纵坐标32格所以写三十二次 */	
		lcd12864_write_cmd(0x80+i);		/* 先写入纵坐标Y的值 */
		lcd12864_write_cmd(0x80);		  /* 再写入横坐标X的值 */
		for(j = 0; j < 16; j++) {		/* 横坐标有16位，每位写入两个字节的的数据，也
								                 * 就写入32次以为当写入两个字节之后横坐标会自
														     * 动加1，所以就不用再次写入地址了。
														     */
		  lcd12864_write_data(*a); 	      
			a++;
		}
	}  
	for(i = 0; i < 32; i++)	{		  /* 因为LCD有纵坐标32格所以写三十二次 */
	
		
		lcd12864_write_cmd(0x80+i);		  /* 先写入纵坐标Y的值 */
		lcd12864_write_cmd(0x88);		  /* 再写入横坐标X的值 */
		for(j = 0; j < 16; j++)	{	  /* 横坐标有16位，每位写入两个字节的的数据，也
								             * 就写入32次以为当写入两个字节之后横坐标会自
														 * 动加1，所以就不用再次写入地址了
														 */
			
			lcd12864_write_data(*a); 	      
			a++;
		}
	}	

	lcd12864_write_cmd(0x36);	   /* 开显示 */
	lcd12864_write_cmd(0x30);  	 /* 转回基本指令集 */
}

#endif

/**
* \brief LCD12864设置窗口
*
* \note 设置在基本指令模式下设置显示坐标。注意：x是设置行，y是设置列
*
* \param[in] x : 行坐标（总共有4行, x = 0, 1, 2, 3）
* \param[in] y : 列坐标
*
* \return 无
*/
void lcd12864_set_window(uchar x, uchar y)
{
	uchar pos = 0;

	/* 第一行的地址是80H */
	if (x == 0) {
		x = 0x80;
	}
		
	/* 第二行的地址是90H */
	else if (x == 1) {
		x = 0x90;	
	}
	
	/* 第三行的地址是88H */
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
*	函 数 名: putchar
*	功能说明: 重定义 putchar 函数， 这样可以使用printf函数打印输出到LCD12864
*	形    参: 等待发送的字符
*	返 回 值: 返回已发送的字符
*********************************************************************************************************
*/

/**
* \brief printf函数的重装载，printf后的打印在LCD12864上
*
* \note 重定义 putchar 函数， 这样可以使用printf函数打印输出到LCD12864
*
* \param[in] c : 字符
*
* \return 相应的字符
*/
char putchar(char c)
{
	/* 发送一个字符 c 到UART1 */
	lcd12864_write_data(c);

	return (c);
}

/*
*********************************************************************************************************
*	函 数 名: getchar
*	功能说明: 重定义C库中的 getchar 函数,这样可以使用scanff函数从串口输入数据
*	形    参: 无
*	返 回 值: 返回已读到的字符
*********************************************************************************************************
*/
//char getchar(void)
//{
//	//return UART_GetByte();
//	return 0;
//}


/**
* \brief LCD12864写字符串函数
*
* \note 直接写入字符串无需引入数组，如lcd12864_write_string("断浪淘沙");
*
* \param[in] string : 字符串地址
*
* \return 无
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
* \brief LCD12864写整数函数
*
* \note 送人一个整数把整数打印到LCD12864
*
* \param[in] value : 送人的整数
*
* \return 无
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
* 函 数 名         : LCD12864_WriteFloatString
* 函数功能		   	: 送人一个Float型的小数打印到LCD12864
* 输    入         	: float value
* 输    出         	: 无
*******************************************************************************/

/**
* \brief LCD12864写浮点数函数
*
* \note 送人一个foat型的小数打印到LCD12864
*
* \param[in] value : 送人浮点数
*
* \return 无
*/
#ifdef LCD12864_WRITE_FLOAT_STRING
void lcd12864_write_float_string(float value)
{
	char floatbuf[10] = {0};
	char *p = floatbuf;
	sprintf(floatbuf,"%.2f", (float)value);		//取小数点后两位
	lcd12864_write_string(floatbuf);
	
}
#endif
