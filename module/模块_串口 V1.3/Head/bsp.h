/*********************************************************************************************************
*	Model	Name	:	BSPģ��
*	File	Name	:	bsp.h
*	Chip			:	STC89C52RC
*	OSC				:	11.0592M
*	Made	By		:	������ɳ
*	Vision			:	1.0
*	Time			:	2016-8-5
*	Discription		:	Ӳ���ײ��������򼯺�
*	Copyright (C), 2016-2017, ������ɳ QQ:891650837
*
*********************************************************************************************************/


#ifndef _BSP_H_
#define _BSP_H

#include "reg52.h"
#include "string.h"
#include "stdio.h"

#define FCLK      11059200uL		 /* ���徧���Ƶ��ʹ, 11.0592MHz */
#define BAUD      9600u		    	 /* ���崮�ڲ����ʶ��� */

/* Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/* Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/* ÿ�����͵���������ֵ */
#define U8_MAX     (255)
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535u)
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

#include "bsp_uart.h"
#include "bsp_tm7705.h"

void bsp_DelayMS(uint16_t n);

void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
