#ifndef __DS1302_H__
#define __DS1302_H__

#define DS1302_SCLK P23 /**< \brief DS1302ʱ�ӿ����Ŷ��� */
#define DS1302_IO   P24 /**< \brief DS1302���ݿ����Ŷ��� */
#define DS1302_CE   P25 /**< \brief DS1302Ƭѡ�����Ŷ��� */


/**
* \brief ��ʼ��DS1302
*
* \param ��
*
* \return ��
*         
*/
void init_ds1302(void);

/**
* \brief ��DS1302��Ӧ�ĵ�ַд������
*
* \param[in] addr : Ҫд��ĵ�ַ
* \param[in] dat  : Ҫд�������
*
* \return ��
*         
*/
void ds1302_write_data(unsigned char addr, unsigned char dat);

/**
* \brief ��ȡDS1302��Ӧ��ַ�е�ֵ
*
* \param[in] addr : Ҫ��ȡ�ĵ�ַ
*
* \return ����������
*         
*/
unsigned char ds1302_read_data (unsigned char addr);

/**
* \brief ��ȡDS1302��ʱ��ֵ
*
* \param p_get_time ��һ��7�ֽ����飬�洢 D[0]�� + D[1]�� + D2ʱ+ D[3]�� + 
*                                         D[4]�� + D[5]���� + D[6]��
*
* \return ��
*         
*/
void get_ds1302_time (unsigned char *p_get_time);

/**
* \brief ����DS1302�ĳ�ʼʱ��
*
* \param[in] p_init_time : ��ʼ��ʱ������
*
* \return ��
*         
*/
void set_ds1302_initial_time (unsigned char code *p_init_time);

void set_ds1302_time (unsigned char  *p_init_time);
#endif