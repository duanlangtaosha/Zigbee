#ifndef __DS1302_H__
#define __DS1302_H__

#define DS1302_SCLK P23 /**< \brief DS1302时钟口引脚定义 */
#define DS1302_IO   P24 /**< \brief DS1302数据口引脚定义 */
#define DS1302_CE   P25 /**< \brief DS1302片选口引脚定义 */


/**
* \brief 初始化DS1302
*
* \param 无
*
* \return 无
*         
*/
void init_ds1302(void);

/**
* \brief 向DS1302相应的地址写入数据
*
* \param[in] addr : 要写入的地址
* \param[in] dat  : 要写入的数据
*
* \return 无
*         
*/
void ds1302_write_data(unsigned char addr, unsigned char dat);

/**
* \brief 读取DS1302相应地址中的值
*
* \param[in] addr : 要读取的地址
*
* \return 读出的数据
*         
*/
unsigned char ds1302_read_data (unsigned char addr);

/**
* \brief 获取DS1302的时钟值
*
* \param p_get_time ：一个7字节数组，存储 D[0]秒 + D[1]分 + D2时+ D[3]日 + 
*                                         D[4]月 + D[5]星期 + D[6]年
*
* \return 无
*         
*/
void get_ds1302_time (unsigned char *p_get_time);

/**
* \brief 设置DS1302的初始时间
*
* \param[in] p_init_time : 初始化时间数组
*
* \return 无
*         
*/
void set_ds1302_initial_time (unsigned char code *p_init_time);

void set_ds1302_time (unsigned char  *p_init_time);
#endif