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
 *			 LCD12864不能显示“数”字，说的是0xfd的bug，要下一个补丁，但我下了，没有修复
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-10  duanlangtaosha, first implementation.
 * \endinternal
 */

#ifndef __LCD12864_H
#define __LCD12864_H

//---包含头文件---//
#include "STC12C5A60S2.h"
#include "string.h"
#include "stdio.h"

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/* ---如果使用画图模式定义这个---*/

/** \brief打开图片显示的函数，不用就注销，节约Flash */
//#define LCD12864_PICTURE			

/** \brief 打开把整数打印成字符串的函数，不用就注销，节约Flash */
#define LCD12864_WRITE_UINT_STRING	

/** \brief 打开把float型小数打印成字符串的函数，不用就注销，节约Flash */
#define LCD12864_WRITE_FLOAT_STRING	

/** \brief 如果是1则是并口，如果是0则是非并口 */
#define PARALLEL	0			

/* ---定义使用的IO口--- */
	#if PARALLEL
		#define Read_LCD12864_DATAPORT P0	  /**< \brief 数据IO口 */
		#define Write_LCD12864_DATAPORT(x) P0 = x
	#else						/**< \brief  如果是不连续的IO口则需一个一个的赋值 */
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

//sbit LCD12864_RS  =  P2^2;   /**< \brief（数据命令）寄存器选择输入  */
//sbit LCD12864_RW  =  P2^1;   /**< \brief 液晶读/写控制 */
//sbit LCD12864_EN  =  P2^0;   /**< \brief 液晶使能控制 */
//sbit LCD12864_PSB =  P2^3;   /**< \brief 串/并方式控制 */
//sbit LCD12864_RST =  P2^5;	 /**< \brief 复位端 */
		
		
sbit LCD12864_RS  =  P1^4;   /**< \brief（数据命令）寄存器选择输入  */
sbit LCD12864_RW  =  P1^1;   /**< \brief 液晶读/写控制 */
sbit LCD12864_EN  =  P1^0;   /**< \brief 液晶使能控制 */
sbit LCD12864_PSB =  P2^6;   /**< \brief 串/并方式控制 */
sbit LCD12864_RST =  P2^7;	 /**< \brief 复位端 */


/********************************************************************************/
/********************************************************************************/

/**
* \brief LCD12864初始化
*
* \param	无
*
* \return 无
*/
void lcd12864_init (void);

/**
* \brief LCD12864写指令操作
*
* \param[in] cmd : 传人相应的指令
*
* \return 无
*/
void lcd12864_write_cmd(uchar cmd);

/**
* \brief LCD12864写数据操作
*
* \param[in] dat : 传人相应的数据
*
* \return 无
*/
void lcd12864_write_data(uchar dat);

/**
* \brief LCD12864读取数据
*
* \param 无
*
* \return 读取到的数据
*/
uchar lcd12864_read_data (void);

/**
* \brief LCD12864清屏（在画图模式下）
*
* \note 在画图模式下，LCD12864的01H命令不能清屏，所以要自己写一个清屏函数
*
* \param 无
*
* \return 无
*/
void lcd12864_clear_screen (void);

/**
* \brief LCD12864画点
*
* \param[in] x : x坐标
* \param[in] y : y坐标
*
* \return 无
*/
void lcd12864_drow_point (uchar x, uchar y);

/**
* \brief LCD12864画图
*
* \param[in] a : 图片数组的地址
*
* \return 无
*/
void LCD12864_DrowPic (uchar *a);

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
void lcd12864_set_window(uchar x, uchar y);

/**
* \brief LCD12864写字符串函数
*
* \note 直接写入字符串无需引入数组，如lcd12864_write_string("断浪淘沙");
*
* \param[in] string : 字符串地址
*
* \return 无
*/
void lcd12864_write_string (char *string);

/**
* \brief LCD12864写整数函数
*
* \note 送人一个整数把整数打印到LCD12864
*
* \param[in] value : 送人的整数
*
* \return 无
*/
//void lcd12864_write_uint_string(unsigned int value );

/**
* \brief LCD12864写浮点数函数
*
* \note 送人一个foat型的小数打印到LCD12864
*
* \param[in] value : 送人浮点数
*
* \return 无
*/
//void lcd12864_write_float_string(float value);
//#endif

#endif
