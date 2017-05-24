/*******************************************************************************
*                            STC51 MCU Module
*                       ---------------------------
*
* Copyright (c) 2001-2012 duanlantaosha
*
*	Chip			:	STC89C52RC&STC12C5A60S2
*	OSC				:	11.0592M
*	Made	By	:	duanlangtaosha
* QQ        : 891650837
* e-mail:   : 891650837@qq.com
*******************************************************************************/

/**
 * \file
 * \brief 
 *
 * \note 串口接口文件
 *       STC89C52RC&STC12C5A60S2，注STC89C52不能用1T模式的
 *       实现printf和scanf函数重定向到串口1，即支持printf信息到UART1，即该模块支持
 *			 printf的打印到串口。
 *			 波特率	:9600,19200，28800，最高可支持115200，但115200会时不时的出现误码,9600
 *			 的是不容易出现误码的。
 * \internal
 * \par Modification history
 * - 1.00 17-05-17  duanlangtaosha, first implementation.
 * \endinternal
 */
#ifndef __uart_h__
#define __uart_h__
#include "__std_type.h"
#include "STC12C5A60S2.h"
#include "string.h"
#include "stdio.h"

#define FCLK      		11059200uL		 /* 定义晶振的频率使, 11.0592MHz */
#define BAUD      		9600 		       /* 定义串口波特率定义 */
//#define BAUD      		115200 		       /* 定义串口波特率定义 */

#define UART_SPEED_X12	1				/*若UART_SPEED_X12为1，则波特率的定时器是不分频，为0则是12分频 */

#define UART_BUF_SIZE 10

/**
* \brief 串口初始化
*
* \param 无
*
* \return 无
*/
void uart_init (void);

/**
* \brief 串口发送单字节
*
* \param[in] _data : 要发送的数据
*
* \return 无
*/
void uart_send_byte (uint8_t _data);

/**
* \brief 串口接收单字节
*
* \param 无
*
* \return 返回接收的数据
*/
uint8_t uart_get_byte (void);

/**
* \brief 串口发送一串字节数据
*
* \param[in] p_buf : 待发送的数据缓存区
* \param[in] len   : 缓存区的大小
*
* \return 无
*/
void uart_send_buf (uint8_t *p_buf, uint8_t _len);

/**
* \brief 串口接收数据缓存
*
* \param[in] p_buf : 接收数据缓存区
* \param[in] len   : 缓存区的大小
*
* \retval 1 : 正确
*         0 : 错误
*/
uint8_t uart_recieve_buf (uint8_t *p_buf, uint8_t _len);

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
void uart_send_frame ( float temp, uint8_t control_sta);

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
uint8_t uartf_reciev_frame(float *temp, uint8_t *control_sta);


void uart_send_string (uint8_t *s);

#endif