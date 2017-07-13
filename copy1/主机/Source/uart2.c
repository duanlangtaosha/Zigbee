/*********************************************************************************************************
*	Model	Name	:	��������ģ��
*	File	Name	:	uart2.c
*	Chip			:	STC89C52RC&STC12C5A60S2��עSTC89C52������1Tģʽ��
*	OSC				:	11.0592M
*	Made	By		:	������ɳ
*	Vision			:	1.3
*	Discription		:	ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��UART1������ģ��֧��
*						printf�Ĵ�ӡ�����ڡ�
*						������	:115200
*						����	: 11.0592M
*	Time			:	2016-8-5
*	Copyright (C), 2016-2017, ������ɳ QQ:891650837
*
*********************************************************************************************************/

#include "uart2.h"
uint8_t  xdata __g_uart2_buf[UART2_BUF_SIZE] = {0};
uint8_t __g_uart2_recieve_counter = 0;
uint8_t g_uart2_sta = 0;

extern uint8_t g_wifi_ok ;
extern uint8_t g_home ;
extern unsigned char choose_page;
void uart2_isr() interrupt 8 using 1	 //�жϽ��ճ���
{
	uint8_t res = 0;
	if(S2CON & S2RI) {			   /* �ж��Ƿ�����꣬������ɺ���Ӳ����RIλ */
		S2CON &= ~S2RI;

			res = S2BUF;

		if (g_wifi_ok == 0) {
			
			 if (g_uart2_sta > UART2_BUF_SIZE-1) {
				g_uart2_sta = 0;
			 }
			__g_uart2_buf[(g_uart2_sta)] = res;
			g_uart2_sta ++;
			 

		 } else {
		 
				if(!(g_uart2_sta & 0x80))
				{
					if(!(g_uart2_sta & 0x40))
					{
						if(res == 0xAA) {
							g_uart2_sta = 0x40; 
							__g_uart2_buf[0] = 0xAA;
							g_uart2_sta++;
						} else {
							g_uart2_sta &= ~0x40;
						}
					} else {
						__g_uart2_buf[(g_uart2_sta & 0x0F)] = res;
						g_uart2_sta ++;
						if((g_uart2_sta & 0x0F) == 2) {
							g_uart2_sta |= 0x80;
							
							g_home = 0;
							choose_page = 2;
						}
					}
			
			 } 
		 }
	}
}


/**
* \brief ���ڷ��͵��ֽ�
*
* \param[in] _data : Ҫ���͵�����
*
* \return ��
*/
void uart2_send_byte (uint8_t _data)
{
	S2BUF = _data;	/* ���������ͻ��� */
	while (!(S2CON & 0x02));		/* �ȴ�������� */
	S2CON &= ~0x02;
}

/**
* \brief ���ڽ��յ��ֽ�
*
* \param ��
*
* \return ���ؽ��յ�����
*/
//uint8_t uart2_get_byte (void)
//{
//	uint8_t c = 0;

//	while (RI == 0);
//	c = SBUF;
//	RI = 0;	

//	return c;
//}

/**
* \brief ���ڷ���һ���ֽ�����
*
* \param[in] p_buf : �����͵����ݻ�����
* \param[in] len   : �������Ĵ�С
*
* \return ��
*/
void uart2_send_buf (uint8_t *p_buf, uint8_t _len)
{
	while (_len--) {
	
		uart2_send_byte (*p_buf++);
	}
}

/**
* \brief ���ڽ������ݻ���
*
* \param[in] p_buf : �������ݻ�����
* \param[in] len   : �������Ĵ�С
*
* \retval 1 : ��ȷ
*         0 : ����
*/
//uint8_t uart2_recieve_buf (uint8_t *p_buf, uint8_t _len)
//{
//	uint8_t i = 0;
//	if (__g_uart2_recieve_counter >= _len) {
//		
//		for( i = 0; i< _len; i++) {
//		 p_buf[i] = __g_uart2_buf[i];
//		}
//		__g_uart2_recieve_counter = 0;
//		
//		return 1;
//	} else {
//		return 0;
//	}
//}

/**
* \brief ���ڳ�ʼ��
*
* \param ��
*
* \return ��
*/
void uart2_init (void)
{
		EA = 0;
    S2CON = 0x50;           //8-bit variable UART
    BRT = -(FOSC / 32 / BAUDS2);  //Set auto-reload vaule of baudrate generator
    AUXR |= 0x14;            //Baudrate generator work in 1T mode
	
		IPH2 |= 0x01;
		IP2  |= 0x01;
	
	
    IE2 = 0x01;             //Enable UART2 interrupt
    EA = 1;    
	
}

/**
* \brief ��������֡��Ϣ
*
* \note  ���͵�֡��Ϣ�� D[0](֡ͷ) + D1(�¶ȵ�����) +D2(�¶�С��) + 
*                       D[3](���ƺ�״̬��) + D[4](У���)
*
* \param[in] temp        : �¶���
* \param[in] control_sta : ���ƺ�״̬��
*
* \return ��
*/
//void uart2_send_frame ( float temp, uint8_t control_sta)
//{
//	uint8_t send_frame[5] = {0};
//	
//	/* ֡ͷ */
//	send_frame[0] = 0xAA;
//	
//	/* �¶ȵ����� */
//	send_frame[1] = (uint8_t)temp;
//	
//	/* �¶ȵ�С�� */
//	send_frame[2] = ((uint16_t)(temp * 10)) % 10;
//	
//	/* ���ƻ�״̬��Ϣ�� */
//	send_frame[3] = control_sta;
//	
//	/* У��� */
//	send_frame[4] = send_frame[0] + send_frame[1] + send_frame[2] + send_frame[3];
//	
//	uart2_send_buf(send_frame, 5);
//}

void uart2_send_string(char *s)
{
    while (*s)              //Check the end of the string
    {
        uart2_send_byte(*s++);     //Send current char and increment string ptr
    }
}


/**
* \brief ��������֡��Ϣ
*
* \note  ���͵�֡��Ϣ�� D[0](֡ͷ) + D1(�¶ȵ�����) +D2(�¶�С��) + 
*                       D[3](���ƺ�״̬��) + D[4](У���)
*
* \param[in] temp        : �¶���
* \param[in] control_sta : ���ƺ�״̬��
*
* \retval 0 : ����
*         1 : ��ȷ
*/
//uint8_t uart2f_reciev_frame(float *temp, uint8_t *control_sta)
//{
//	uint8_t r_frame[5] = {0};
//	
//	if (uart2_recieve_buf (r_frame, 5)) { /* ���մ���֡��Ϣ */
//	
//		if (r_frame[0] == 0xAA) {
//			if (r_frame[4] == (uint8_t)(r_frame[0] + r_frame[1] + r_frame[2] + r_frame[3])) {
//				uart2_send_string("abc\r\n");
//				*temp = r_frame[1] + r_frame[2]/10.0;
//				
//				*control_sta = r_frame[3];
//				
//				return 1;
//			} else {
//				return 0;	/* У����� */
//			}
//			
//		} else {
//			return 0;
//		}
// }
//	return 0;
//}


