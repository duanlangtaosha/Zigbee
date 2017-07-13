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
 *			 LCD12864������ʾ�������֣�˵����0xfd��bug��Ҫ��һ���������������ˣ�û���޸�
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-10  duanlangtaosha, first implementation.
 * \endinternal
 */

#ifndef __LCD12864_H
#define __LCD12864_H

//---����ͷ�ļ�---//
#include "STC12C5A60S2.h"
#include "string.h"
#include "stdio.h"

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/* ---���ʹ�û�ͼģʽ�������---*/

/** \brief��ͼƬ��ʾ�ĺ��������þ�ע������ԼFlash */
//#define LCD12864_PICTURE			

/** \brief �򿪰�������ӡ���ַ����ĺ��������þ�ע������ԼFlash */
#define LCD12864_WRITE_UINT_STRING	

/** \brief �򿪰�float��С����ӡ���ַ����ĺ��������þ�ע������ԼFlash */
#define LCD12864_WRITE_FLOAT_STRING	

/** \brief �����1���ǲ��ڣ������0���Ƿǲ��� */
#define PARALLEL	0			

/* ---����ʹ�õ�IO��--- */
	#if PARALLEL
		#define Read_LCD12864_DATAPORT P0	  /**< \brief ����IO�� */
		#define Write_LCD12864_DATAPORT(x) P0 = x
	#else						/**< \brief  ����ǲ�������IO������һ��һ���ĸ�ֵ */
//		#define DB0	P00
//		#define DB1 P01
//		#define DB2 P02
//		#define DB3 P03
//		#define DB4 P04
//		#define DB5 P05
//		#define DB6 P06
//		#define DB7 P07

			#define DB0	P07
			#define DB1 P06
			#define DB2 P05
			#define DB3 P04
			#define DB4 P03
			#define DB5 P02
			#define DB6 P01
			#define DB7 P00

//		#define Read_LCD12864_DATAPORT (DB7*128)
		#define Read_LCD12864_DATAPORT	(DB0 | (DB1 * 2) | (DB2 * 4) | (DB3 * 8)|		\
										(DB4 * 16) | (DB5 * 32) | (DB6 * 64) | (DB7 * 128))

		
		#define Write_LCD12864_DATAPORT(x)  \
		{									\
			DB0=(x >> 0) & 0x01;				\
			DB1=(x >> 1) & 0x01;				\
			DB2=(x >> 2) & 0x01;				\
			DB3=(x >> 3) & 0x01;				\
			DB4=(x >> 4) & 0x01;				\
			DB5=(x >> 5) & 0x01;				\
			DB6=(x >> 6) & 0x01;				\
			DB7=(x >> 7) & 0x01;				\
		}
	#endif

//sbit LCD12864_RS  =  P2^2;   /**< \brief����������Ĵ���ѡ������  */
//sbit LCD12864_RW  =  P2^1;   /**< \brief Һ����/д���� */
//sbit LCD12864_EN  =  P2^0;   /**< \brief Һ��ʹ�ܿ��� */
//sbit LCD12864_PSB =  P2^3;   /**< \brief ��/����ʽ���� */
//sbit LCD12864_RST =  P2^5;	 /**< \brief ��λ�� */
		
		
sbit LCD12864_RS  =  P1^4;   /**< \brief����������Ĵ���ѡ������  */
sbit LCD12864_RW  =  P1^1;   /**< \brief Һ����/д���� */
sbit LCD12864_EN  =  P1^0;   /**< \brief Һ��ʹ�ܿ��� */
sbit LCD12864_PSB =  P2^6;   /**< \brief ��/����ʽ���� */
sbit LCD12864_RST =  P2^7;	 /**< \brief ��λ�� */


/********************************************************************************/
/********************************************************************************/

/**
* \brief LCD12864��ʼ��
*
* \param	��
*
* \return ��
*/
void lcd12864_init (void);

/**
* \brief LCD12864дָ�����
*
* \param[in] cmd : ������Ӧ��ָ��
*
* \return ��
*/
void lcd12864_write_cmd(uchar cmd);

/**
* \brief LCD12864д���ݲ���
*
* \param[in] dat : ������Ӧ������
*
* \return ��
*/
void lcd12864_write_data(uchar dat);

/**
* \brief LCD12864��ȡ����
*
* \param ��
*
* \return ��ȡ��������
*/
uchar lcd12864_read_data (void);

/**
* \brief LCD12864�������ڻ�ͼģʽ�£�
*
* \note �ڻ�ͼģʽ�£�LCD12864��01H���������������Ҫ�Լ�дһ����������
*
* \param ��
*
* \return ��
*/
void lcd12864_clear_screen (void);

/**
* \brief LCD12864����
*
* \param[in] x : x����
* \param[in] y : y����
*
* \return ��
*/
void lcd12864_drow_point (uchar x, uchar y);

/**
* \brief LCD12864��ͼ
*
* \param[in] a : ͼƬ����ĵ�ַ
*
* \return ��
*/
void LCD12864_DrowPic (uchar *a);

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
void lcd12864_set_window(uchar x, uchar y);

/**
* \brief LCD12864д�ַ�������
*
* \note ֱ��д���ַ��������������飬��lcd12864_write_string("������ɳ");
*
* \param[in] string : �ַ�����ַ
*
* \return ��
*/
void lcd12864_write_string (char *string);

/**
* \brief LCD12864д��������
*
* \note ����һ��������������ӡ��LCD12864
*
* \param[in] value : ���˵�����
*
* \return ��
*/
//void lcd12864_write_uint_string(unsigned int value );

/**
* \brief LCD12864д����������
*
* \note ����һ��foat�͵�С����ӡ��LCD12864
*
* \param[in] value : ���˸�����
*
* \return ��
*/
//void lcd12864_write_float_string(float value);
//#endif

#endif
