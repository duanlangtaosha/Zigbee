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
 * \brief ��������ɨ��ģ��
 *
 * \note ֧��������8λ��������
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-11  duanlangtaosha, first implementation.
 * \endinternal
 */

#include "stc12c5a60s2.h"
#include "key.h"

/**
* \brief ����ɨ��
*
* \param ��
*
* \retval 0       :  �ް�������
*         EN_KEY0 �� ����0����
*         EN_KEY1 �� ����1����
*         EN_KEY2 �� ����2����
*					EN_KEY3 �� ����3����
*/
unsigned char keyscan (unsigned char shake_time)
{	
	static unsigned char s_keypress = 0, s_keyok = 0, s_keyup_press = 0, s_keyup = 1;
	unsigned char pin_arry = 0;
	unsigned char copy_pin0 = 0, copy_pin1 = 0, copy_pin2 = 0, copy_pin3 = 0;
	unsigned char keycode = 0;	/* ȡ��ֵ */
	/******************�������ŵĸ�ֵ�뿽��ֵ*********************/
	PIN_KEY0 = PIN_KEY1 = PIN_KEY2 = PIN_KEY3 = 1;
	copy_pin0 = PIN_KEY0;
	copy_pin1 = PIN_KEY1;
	copy_pin2 = PIN_KEY2;
	copy_pin3 = PIN_KEY3;
	pin_arry = (copy_pin3 << 3 ) | (copy_pin2 << 2 ) | (copy_pin1 << 1 ) | (copy_pin0 ); /* �ϲ���һ�� */
/***********************���¼��****************************/	  
	 if ((0x0f != pin_arry) && (0 == s_keyok) && (1 == s_keyup)) { 	/* ȷ�ϰ���̧��󣬰��²�����Ч */
	 	s_keypress++;		
	 }
	 else {
	 	s_keypress = 0;
	 }
	 if(shake_time == s_keypress)	{  /* �������� */
		keycode = pin_arry;	 /* ȡ��ʱ�ļ�ֵ */
		s_keypress = 0;
		
		if (1 == s_keyup) {
			s_keyup = 0;
			s_keyok = 1;
			switch (keycode) {
			
				case 0x0e:	return EN_KEY0;
				case 0x0d:	return EN_KEY1;
				case 0x0b:	return EN_KEY2;
				case 0x07:	return EN_KEY3;
				default:	return 0;
			}
//			return 1;
		}
	 } 
/***********************̧����****************************/ 
	 if ((0x0f == (pin_arry & 0x0f)) && (1 == s_keyok)) {  /* �ж��ϴ��Ѿ����£��Ҵ�ʱ��̧���״̬ */
	 	s_keyup_press++;
	 }
	 else if((1 == s_keyok) && ((pin_arry & 0x0f) != 0x0f)) {
	 	s_keyup_press = 0;
	 }
	 if((shake_time == s_keyup_press)) {		/* ̧��������� */
	 	s_keyup_press = 0;  		
		if((s_keyok == 1) && ((pin_arry & 0x0f) == 0x0f)) {
		 
		 	s_keyup = 1;
			s_keyok = 0;
		 }
	 }
	 return 0;
}
