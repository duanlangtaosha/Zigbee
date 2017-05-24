/*********************************************************************************************
Project		:	独立按键扫描
Function	:	支持最多添加8位独立按键
Test chip	:	STC89C52RC,STC12C5A60S2
Osc			:	12M
Made  by	:	断浪淘沙
Time		:	2016-7-16
Vision		:	V1.0
*********************************************************************************************/
#ifndef __key_h__
#define __key_h__

/***************独立按键端口的映射*************/
#define PIN_KEY0 P36  /**< \brief 按键0的端口定义 */
#define PIN_KEY1 P35  /**< \brief 按键1的端口定义 */
#define PIN_KEY2 P34  /**< \brief 按键2的端口定义 */
#define PIN_KEY3 P33  /**< \brief 按键3的端口定义 */

#define SHAKE_TIME	10 /**< \brief 按键消抖，如果还抖动就通过调节这个的大小 */

/** \brief 4个键代码 */
typedef enum	  
{
 	EN_KEY0=1,
	EN_KEY1,
	EN_KEY2,
	EN_KEY3
}EN_KEY;

/**
* \brief 按键扫描
*
* \param 无
*
* \retval 0       :  无按键按下
*         EN_KEY0 ： 按键0按下
*         EN_KEY1 ： 按键1按下
*         EN_KEY2 ： 按键2按下
*					EN_KEY3 ： 按键3按下
*/
unsigned char keyscan (void);
#endif