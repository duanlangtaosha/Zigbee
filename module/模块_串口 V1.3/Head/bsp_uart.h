/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_uart.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
