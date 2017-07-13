/*********************************************************************************************************
*	Model	Name	:	串口驱动模块
*	File	Name	:	uart2.c
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

#include "uart2.h"
uint8_t  xdata __g_uart2_buf[UART2_BUF_SIZE] = {0};
uint8_t __g_uart2_recieve_counter = 0;
uint8_t g_uart2_sta = 0;

extern uint8_t g_wifi_ok ;
extern uint8_t g_home ;
extern unsigned char choose_page;
void uart2_isr() interrupt 8 using 1	 //中断接收程序
{
	uint8_t res = 0;
	if(S2CON & S2RI) {			   /* 判断是否接收完，接收完成后，由硬件置RI位 */
		S2CON &= ~S2RI;

			res = S2BUF;

		if (g_wifi_ok == 0) {
			
			 if (g_uart2_sta > UART2_BUF_SIZE-1) {
				g_uart2_sta = 0;
			 }
			__g_uart2_buf[(g_uart2_sta)] = res;
			g_uart2_sta ++;
			 

		 } else {
		 
				if(!(g_uart2_sta & 0x80))
				{
					if(!(g_uart2_sta & 0x40))
					{
						if(res == 0xAA) {
							g_uart2_sta = 0x40; 
							__g_uart2_buf[0] = 0xAA;
							g_uart2_sta++;
						} else {
							g_uart2_sta &= ~0x40;
						}
					} else {
						__g_uart2_buf[(g_uart2_sta & 0x0F)] = res;
						g_uart2_sta ++;
						if((g_uart2_sta & 0x0F) == 2) {
							g_uart2_sta |= 0x80;
							
							g_home = 0;
							choose_page = 2;
						}
					}
			
			 } 
		 }
	}
}


/**
* \brief 串口发送单字节
*
* \param[in] _data : 要发送的数据
*
* \return 无
*/
void uart2_send_byte (uint8_t _data)
{
	S2BUF = _data;	/* 发送数据送缓冲 */
	while (!(S2CON & 0x02));		/* 等待发送完成 */
	S2CON &= ~0x02;
}

/**
* \brief 串口接收单字节
*
* \param 无
*
* \return 返回接收的数据
*/
//uint8_t uart2_get_byte (void)
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
void uart2_send_buf (uint8_t *p_buf, uint8_t _len)
{
	while (_len--) {
	
		uart2_send_byte (*p_buf++);
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
//uint8_t uart2_recieve_buf (uint8_t *p_buf, uint8_t _len)
//{
//	uint8_t i = 0;
//	if (__g_uart2_recieve_counter >= _len) {
//		
//		for( i = 0; i< _len; i++) {
//		 p_buf[i] = __g_uart2_buf[i];
//		}
//		__g_uart2_recieve_counter = 0;
//		
//		return 1;
//	} else {
//		return 0;
//	}
//}

/**
* \brief 串口初始化
*
* \param 无
*
* \return 无
*/
void uart2_init (void)
{
		EA = 0;
    S2CON = 0x50;           //8-bit variable UART
    BRT = -(FOSC / 32 / BAUDS2);  //Set auto-reload vaule of baudrate generator
    AUXR |= 0x14;            //Baudrate generator work in 1T mode
	
		IPH2 |= 0x01;
		IP2  |= 0x01;
	
	
    IE2 = 0x01;             //Enable UART2 interrupt
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
//void uart2_send_frame ( float temp, uint8_t control_sta)
//{
//	uint8_t send_frame[5] = {0};
//	
//	/* 帧头 */
//	send_frame[0] = 0xAA;
//	
//	/* 温度的整数 */
//	send_frame[1] = (uint8_t)temp;
//	
//	/* 温度的小数 */
//	send_frame[2] = ((uint16_t)(temp * 10)) % 10;
//	
//	/* 控制或状态信息量 */
//	send_frame[3] = control_sta;
//	
//	/* 校验和 */
//	send_frame[4] = send_frame[0] + send_frame[1] + send_frame[2] + send_frame[3];
//	
//	uart2_send_buf(send_frame, 5);
//}

void uart2_send_string(char *s)
{
    while (*s)              //Check the end of the string
    {
        uart2_send_byte(*s++);     //Send current char and increment string ptr
    }
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
//uint8_t uart2f_reciev_frame(float *temp, uint8_t *control_sta)
//{
//	uint8_t r_frame[5] = {0};
//	
//	if (uart2_recieve_buf (r_frame, 5)) { /* 接收串口帧信息 */
//	
//		if (r_frame[0] == 0xAA) {
//			if (r_frame[4] == (uint8_t)(r_frame[0] + r_frame[1] + r_frame[2] + r_frame[3])) {
//				uart2_send_string("abc\r\n");
//				*temp = r_frame[1] + r_frame[2]/10.0;
//				
//				*control_sta = r_frame[3];
//				
//				return 1;
//			} else {
//				return 0;	/* 校验错误 */
//			}
//			
//		} else {
//			return 0;
//		}
// }
//	return 0;
//}


