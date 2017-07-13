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
 * \brief DS18B20�ӿڳ���
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  duanlangtaosha, first implementation.
 * \endinternal
 */

#ifndef __ds18b20_h__
#define __ds18b20_h__

/** \brief ����DS18B20�Ľӿ� */
#define PIN_DS18B20 P37

/**
* \brief DS18B20��ʼ��
*
* \param ��
*
* \return ���س�ʼ����״̬
*
*/
bit init_ds18b20(void);

/**
* \brief ��DS18B20��д������
*
* \param[in] dat : Ҫд�������
*
* \return ��
*
*/
void write_ds18b20 (unsigned char dat);

/**
* \brief ��ȡDS1820���¶�����(����)
*
* \param ��
*
* \return ���ش�С�����¶�,������1��
*
*/
 unsigned char rd_temperature_i(void);
 
/**
* \brief ��ȡDS1820���¶�����(������)
*
* \param ��
*
* \return ���ش�С�����¶�,������0.0625��
*
*/
float rd_temperature_f (void);
 
 /**
* \brief ���DS18B20��ROMֵ����һ�޶����64bit���豸����
*
* \param[out] p_buf : ����8���ֽڵ�ROM��ֵ���ŵ�һ��8�ֽڵ������У�
*
* \return ��
*
*/
void read_ds18b20_rom (unsigned char *p_buf);

/**
* \brief ƥ��ROM,����Ƕ��ͨ�ž�Ҫ��ROMֵ�����豸
*
* \param[in] num_ds18b20 : �����豸�ţ�����ж���豸�ʹ�1��ʼ
* \param[in] p_rom_buf   : �洢��֪��DS18B20��ROMֵ������һ����ά������
*
* \note test[x][8] = { {0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??}, {}, {}.....}
*
* \return ��
*
*/
void match_rom(unsigned char num_ds18b20, unsigned char *p_rom_buf);		  
#endif										