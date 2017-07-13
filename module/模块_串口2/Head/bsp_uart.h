/*
*********************************************************************************************************
*
*	模块名称 : 串口驱动模块
*	文件名称 : bsp_uart.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_UART_H
#define __BSP_UART_H

void bsp_InitUart(void);
void UART_SendByte(uint8_t _data);
void UART_SendBuf(uint8_t *_pBuf, uint8_t _len);
uint8_t UART_GetByte(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
