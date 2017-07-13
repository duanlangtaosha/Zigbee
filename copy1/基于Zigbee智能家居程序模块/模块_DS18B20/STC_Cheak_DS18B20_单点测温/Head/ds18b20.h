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
 * \brief DS18B20接口程序
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  duanlangtaosha, first implementation.
 * \endinternal
 */

#ifndef __ds18b20_h__
#define __ds18b20_h__

/** \brief 定义DS18B20的接口 */
#define PIN_DS18B20 P14



//extern void DS18B20_Mode(void);
/******普通的读取一个18B20的温度**********/
extern float rd_temperature_f(void);

/******读取并显示一个陌生的DS18B20的ROM********
 多个同时挂载在总线上执行这个函数，则不知道是读
 的那一个，我测试出来的数据是，显示的ROM不是任意
 一个DS18B20的ROM，所以说只能一个一个的读取

*/
extern void Display_DS18B20_ROM(void)	;

/******读取一个DS18B20的温度(在多个DS18B20挂载到总线上)
 num_ds18b20是显示那一个DS18B20的温度，目前只支持1和2两个DS18B20
*/
extern float Read_DS18B20_Num(unsigned int num_ds18b20);


/**
* \brief DS18B20初始化
*
* \param 无
*
* \return 返回初始化的状态
*
*/
bit init_ds18b20(void);

/**
* \brief 读取DS1820的温度数据(整数)
*
* \param 无
*
* \return 返回带小数的温度,步进是1℃
*
*/
 unsigned char rd_temperature_i(void);
 
/**
* \brief 读取DS1820的温度数据(浮点数)
*
* \param 无
*
* \return 返回带小数的温度,步进是0.0625℃
*
*/
float rd_temperature_f (void);
 
 /**
* \brief 检测DS18B20的ROM值（独一无二光刻64bit的设备名）
*
* \param[out] p_buf : 保持8个字节的ROM的值（放到一个8字节的数组中）
*
* \return 无
*
*/
void read_ds18b20_rom (unsigned char *p_buf);

/**
* \brief 匹配ROM,如果是多点通信就要用ROM值区分设备
*
* \param[in] num_ds18b20 : 输入设备号，如果有多个设备就从1开始
* \param[in] p_rom_buf   : 存储已知的DS18B20的ROM值，放在一个二维数组中
*
* \note test[x][8] = { {0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??}, {}, {}.....}
*
* \return 无
*
*/
void match_rom(unsigned char num_ds18b20, unsigned char *p_rom_buf);		  
#endif										