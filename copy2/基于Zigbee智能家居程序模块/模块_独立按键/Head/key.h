/*********************************************************************************************
Project		:	��������ɨ��
Function	:	֧��������8λ��������
Test chip	:	STC89C52RC,STC12C5A60S2
Osc			:	12M
Made  by	:	������ɳ
Time		:	2016-7-16
Vision		:	V1.0
*********************************************************************************************/
#ifndef __key_h__
#define __key_h__

/***************���������˿ڵ�ӳ��*************/
#define PIN_KEY0 P36  /**< \brief ����0�Ķ˿ڶ��� */
#define PIN_KEY1 P35  /**< \brief ����1�Ķ˿ڶ��� */
#define PIN_KEY2 P34  /**< \brief ����2�Ķ˿ڶ��� */
#define PIN_KEY3 P33  /**< \brief ����3�Ķ˿ڶ��� */

#define SHAKE_TIME	10 /**< \brief ���������������������ͨ����������Ĵ�С */

/** \brief 4�������� */
typedef enum	  
{
 	EN_KEY0=1,
	EN_KEY1,
	EN_KEY2,
	EN_KEY3
}EN_KEY;

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
unsigned char keyscan (void);
#endif