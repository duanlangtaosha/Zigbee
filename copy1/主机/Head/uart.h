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
 * \note ���ڽӿ��ļ�
 *       STC89C52RC&STC12C5A60S2��עSTC89C52������1Tģʽ��
 *       ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��UART1������ģ��֧��
 *			 printf�Ĵ�ӡ�����ڡ�
 *			 ������	:9600,19200��28800����߿�֧��115200����115200��ʱ��ʱ�ĳ�������,9600
 *			 ���ǲ����׳�������ġ�
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

#define FCLK      		11059200uL		 /* ���徧���Ƶ��ʹ, 11.0592MHz */
#define BAUD      		9600 		       /* ���崮�ڲ����ʶ��� */
//#define BAUD      		115200 		       /* ���崮�ڲ����ʶ��� */

#define UART_SPEED_X12	1				/*��UART_SPEED_X12Ϊ1�������ʵĶ�ʱ���ǲ���Ƶ��Ϊ0����12��Ƶ */

#define UART_BUF_SIZE 10

/**
* \brief ���ڳ�ʼ��
*
* \param ��
*
* \return ��
*/
void uart_init (void);

/**
* \brief ���ڷ��͵��ֽ�
*
* \param[in] _data : Ҫ���͵�����
*
* \return ��
*/
void uart_send_byte (uint8_t _data);

/**
* \brief ���ڽ��յ��ֽ�
*
* \param ��
*
* \return ���ؽ��յ�����
*/
uint8_t uart_get_byte (void);

/**
* \brief ���ڷ���һ���ֽ�����
*
* \param[in] p_buf : �����͵����ݻ�����
* \param[in] len   : �������Ĵ�С
*
* \return ��
*/
void uart_send_buf (uint8_t *p_buf, uint8_t _len);

/**
* \brief ���ڽ������ݻ���
*
* \param[in] p_buf : �������ݻ�����
* \param[in] len   : �������Ĵ�С
*
* \retval 1 : ��ȷ
*         0 : ����
*/
uint8_t uart_recieve_buf (uint8_t *p_buf, uint8_t _len);

/**
* \brief ��������֡��Ϣ
*
* \note  ���͵�֡��Ϣ�� D[0](֡ͷ) + D1(�¶ȵ�����) +D2(�¶�С��) + 
*                       D[3](���ƺ�״̬��) + D[4](У���)
*
* \param[in] temp        : �¶���
* \param[in] control_sta : ���ƺ�״̬��
*
* \return ��
*/
void uart_send_frame ( float temp, uint8_t control_sta);

/**
* \brief ��������֡��Ϣ
*
* \note  ���͵�֡��Ϣ�� D[0](֡ͷ) + D1(�¶ȵ�����) +D2(�¶�С��) + 
*                       D[3](���ƺ�״̬��) + D[4](У���)
*
* \param[in] temp        : �¶���
* \param[in] control_sta : ���ƺ�״̬��
*
* \retval 0 : ����
*         1 : ��ȷ
*					-1��У�����
*/
int8_t uartf_reciev_frame(float *temp, uint8_t *control_sta);


void uart_send_string (uint8_t *s);

#endif