/*********************************************************************************************************
*	Model	Name	:	串口驱动模块
*	File	Name	:	uart.c
*	Chip			:	STC89C52RC&STC12C5A60S2，注STC89C52不能用1T模式的
*	OSC				:	11.0592M
*	Made	By		:	断浪淘沙
*	Vision			:	1.3
*	Discription		:	实现printf和scanf函数重定向到串口1，即支持printf信息到UART1，即该模块支持
*						printf的打印到串口。
*						波特率	:115200
*						晶振	: 11.0592M
*	Time			:	2016-8-5
*	Copyright (C), 2016-2017, 断浪淘沙 QQ:891650837
*
*********************************************************************************************************/

#include "uart.h"
#include "intrins.h"
unsigned char xdata __g_uart_buf[UART_BUF_SIZE] = {0};
//unsigned char xdata __g_uart_buf[5] = {0};
uint8_t __g_uart_recieve_counter = 0;
uint8_t uart_sta = 0;

void uart_isr() interrupt 4	 //中断接收程序
{
	static unsigned char frame_head_suc = 0;
	unsigned char res = 0;
	if(RI) {			   /* 判断是否接收完，接收完成后，由硬件置RI位 */
		RI = 0;
	
		__g_uart_buf[__g_uart_recieve_counter++] = SBUF;
		if (__g_uart_recieve_counter >= UART_BUF_SIZE) {
			__g_uart_recieve_counter = 0;
		}
		
		

//			res = SBUF;
//			if(!(uart_sta & 0x80))
//				{
//				if(!(uart_sta & 0x40))
//				{
//					if(res == 0xAA)
//						uart_sta |= 0x40;                                     
//					else uart_sta &= ~0x40;
//				}
//				else
//				{
//					__g_uart_buf[(uart_sta&0x0F)] = res;
//					uart_sta ++;
//					if((uart_sta &0x0F) == 4) 
//						uart_sta |= 0x80;
//				}
//		
//	}
}
	}


/**
* \brief 串口发送单字节
*
* \param[in] _data : 要发送的数据
*
* \return 无
*/
void uart_send_byte (uint8_t _data)
{
	SBUF = _data;	/* 发送数据送缓冲 */
	while (!TI);		/* 等待发送完成 */
	TI = 0;
}

/**
* \brief 串口接收单字节
*
* \param 无
*
* \return 返回接收的数据
*/
//uint8_t uart_get_byte (void)
//{
//	uint8_t c = 0;

//	while (RI == 0);
//	c = SBUF;
//	RI = 0;	

//	return c;
//}

/**
* \brief 串口发送一串字节数据
*
* \param[in] p_buf : 待发送的数据缓存区
* \param[in] len   : 缓存区的大小
*
* \return 无
*/
void uart_send_buf (uint8_t *p_buf, uint8_t _len)
{
	while (_len--) {
	
		uart_send_byte (*p_buf++);
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}


void uart_send_string (uint8_t *s)
{

    while (*s)              //Check the end of the string
    {
        uart_send_byte(*s++);     //Send current char and increment string ptr
    }
}

/**
* \brief 串口接收数据缓存
*
* \param[in] p_buf : 接收数据缓存区
* \param[in] len   : 缓存区的大小
*
* \retval 1 : 正确
*         0 : 错误
*/
uint8_t uart_recieve_buf (uint8_t *p_buf, uint8_t _len)
{
	uint8_t i = 0;
	if (__g_uart_recieve_counter >= _len) {
		
		for( i = 0; i< _len; i++) {
		 p_buf[i] = __g_uart_buf[i];
		}
		__g_uart_recieve_counter = 0;
		
		return 1;
	} else {
		return 0;
	}
}

/**
* \brief 串口初始化
*
* \param 无
*
* \return 无
*/
void uart_init (void)
{
	
	SCON = 0x50;	/* 010 1 0000B，工作方式1, 8位UART，波特率可变，允许接收 */
	TMOD = 0x20;	/* T1工作于方式2 */
	TCON = 0x40;	/* 定时器1开始计数 */
//	TH1 = 250;		/* T1计数初值 */
//	TL1 = 250;

	#if UART_SPEED_X12
	AUXR = (AUXR | (1 << 6));
	
	/* 由于下面是双倍速率所以说，这里BAUD/2;((FCLK/12/32/(BAUD/2)))等于6 */
	TH1  = TL1 =(-(FCLK / 32 / (BAUD / 2)));		
	#else			
	
  /* ((FCLK/12/32/BAUD/2))等于1.5 */	
	TH1  = TL1 =(-(FCLK / 12 / 32 / (BAUD / 2)));
	#endif
	
	/* SMOD置1,双倍速率 */
	PCON = 0x80;	
	RI   = 0;
	TI   = 0;
	
	ES = 1;
	EA = 1;
}

/**
* \brief 发送数据帧信息
*
* \note  发送的帧信息： D[0](帧头) + D1(温度的整数) +D2(温度小数) + 
*                       D[3](控制和状态量) + D[4](校验和)
*
* \param[in] temp        : 温度量
* \param[in] control_sta : 控制和状态量
*
* \return 无
*/
void uart_send_frame ( float temp, uint8_t control_sta)
{
	uint8_t send_frame[5] = {0};
	
	/* 帧头 */
	send_frame[0] = 0xAA;
	
	/* 温度的整数 */
	send_frame[1] = (uint8_t)temp;
	
	/* 温度的小数 */
	send_frame[2] = ((uint16_t)(temp * 10)) % 10;
	
	/* 控制或状态信息量 */
	send_frame[3] = control_sta;
	
	/* 校验和 */
	send_frame[4] = send_frame[0] + send_frame[1] + send_frame[2] + send_frame[3];
	
	uart_send_buf(send_frame, 5);
}

/**
* \brief 接收数据帧信息
*
* \note  发送的帧信息： D[0](帧头) + D1(温度的整数) +D2(温度小数) + 
*                       D[3](控制和状态量) + D[4](校验和)
*
* \param[in] temp        : 温度量
* \param[in] control_sta : 控制和状态量
*
* \retval 0 : 错误
*         1 : 正确
*/
uint8_t uartf_reciev_frame(float *temp, uint8_t *control_sta)
{
	uint8_t r_frame[5] = {0};
	
	if (uart_recieve_buf (r_frame, 5)) { /* 接收串口帧信息 */
	
		if (r_frame[0] == 0xAA) {
			if (r_frame[4] == (uint8_t)(r_frame[0] + r_frame[1] + r_frame[2] + r_frame[3])) {

				*temp = r_frame[1] + r_frame[2]/10.0;
				
				*control_sta = r_frame[3];
				
				return 1;
			} else {
				return 0;	/* 校验错误 */
			}
			
		} else {
			return 0;
		}
 }
	return 0;
}




/********************************************************************************************************
*	Discription		:	实现printf和scanf函数重定向到串口1，即支持printf信息到UART1
*						实现重定向，只需要添加2个函数:
*						char putchar(char c);
*						int fgetc(FILE *f);
*						char getchar(void)
*						这个c模块无对应的h文件。
*********************************************************************************************************/


///*
//*********************************************************************************************************
//*	函 数 名: putchar
//*	功能说明: 重定义 putchar 函数， 这样可以使用printf函数从串口1打印输出
//*	形    参: 等待发送的字符
//*	返 回 值: 返回已发送的字符
//*********************************************************************************************************
//*/
//char putchar(char c)
//{
//	/* 发送一个字符 c 到UART1 */
//	uart_send_byte(c);

//	return (c);
//}

///*
//*********************************************************************************************************
//*	函 数 名: getchar
//*	功能说明: 重定义C库中的 getchar 函数,这样可以使用scanff函数从串口1输入数据
//*	形    参: 无
//*	返 回 值: 返回已读到的字符
//*********************************************************************************************************
//*/
//char getchar(void)
//{
//	return uart_get_byte();
//}