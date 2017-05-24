/*********************************************************************************************************
*	Model	Name	:	��������ģ��
*	File	Name	:	uart.c
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

#include "uart.h"
#include "intrins.h"
//unsigned char xdata __g_uart_buf[UART_BUF_SIZE] = {0};
unsigned char  __g_uart_buf[5] = {0};
uint8_t __g_uart_recieve_counter = 0;
uint8_t g_uart_sta = 0;

void uart_isr() interrupt 4	 //�жϽ��ճ���
{
	static unsigned char frame_head_suc = 0;
	unsigned char res = 0;
	if(RI) {			   /* �ж��Ƿ�����꣬������ɺ���Ӳ����RIλ */
		RI = 0;
//		__g_uart_buf[__g_uart_recieve_counter++] = SBUF;
//		if (__g_uart_recieve_counter >= UART_BUF_SIZE) {
//			__g_uart_recieve_counter = 0;
//		}
			res = SBUF;
			if(!(g_uart_sta & 0x80))
			{
				if(!(g_uart_sta & 0x40))
				{
					if(res == 0xAA) {
						g_uart_sta |= 0x40; 
						__g_uart_buf[0] = 0xAA;
						g_uart_sta++;
					} else {
						g_uart_sta &= ~0x40;
					}
				} else {
					__g_uart_buf[(g_uart_sta & 0x0F)] = res;
					g_uart_sta ++;
					if((g_uart_sta & 0x0F) == 5) {
						g_uart_sta |= 0x80;
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
void uart_send_byte (uint8_t _data)
{
	SBUF = _data;	/* ���������ͻ��� */
	while (!TI);		/* �ȴ�������� */
	TI = 0;
}

/**
* \brief ���ڽ��յ��ֽ�
*
* \param ��
*
* \return ���ؽ��յ�����
*/
//uint8_t uart_get_byte (void)
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
void uart_send_buf (uint8_t *p_buf, uint8_t _len)
{
	while (_len--) {
	
		uart_send_byte (*p_buf++);
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}


void uart_send_string (uint8_t *s)
{

    while (*s)              //Check the end of the string
    {
        uart_send_byte(*s++);     //Send current char and increment string ptr
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
//uint8_t uart_recieve_buf (uint8_t *p_buf, uint8_t _len)
//{
//	uint8_t i = 0;
//	if (__g_uart_recieve_counter >= _len) {
//		
//		for( i = 0; i< _len; i++) {
//		 p_buf[i] = __g_uart_buf[i];
//		}
//		__g_uart_recieve_counter = 0;
//		g_uart_sta = 0;
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
void uart_init (void)
{
	
	SCON = 0x50;	/* 010 1 0000B��������ʽ1, 8λUART�������ʿɱ䣬������� */
	TMOD = 0x20;	/* T1�����ڷ�ʽ2 */
	TCON = 0x40;	/* ��ʱ��1��ʼ���� */
//	TH1 = 250;		/* T1������ֵ */
//	TL1 = 250;

	#if UART_SPEED_X12
	AUXR = (AUXR | (1 << 6));
	
	/* ����������˫����������˵������BAUD/2;((FCLK/12/32/(BAUD/2)))����6 */
	TH1  = TL1 =(-(FCLK / 32 / (BAUD / 2)));		
	#else			
	
  /* ((FCLK/12/32/BAUD/2))����1.5 */	
	TH1  = TL1 =(-(FCLK / 12 / 32 / (BAUD / 2)));
	#endif
	
	/* SMOD��1,˫������ */
	PCON = 0x80;	
	RI   = 0;
	TI   = 0;
	
	ES = 1;
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
void uart_send_frame ( float temp, uint8_t control_sta)
{
	uint8_t send_frame[5] = {0};
	
	/* ֡ͷ */
	send_frame[0] = 0xAA;
	
	/* �¶ȵ����� */
	send_frame[1] = (uint8_t)temp;
	
	/* �¶ȵ�С�� */
	send_frame[2] = ((uint16_t)(temp * 10)) % 10;
	
	/* ���ƻ�״̬��Ϣ�� */
	send_frame[3] = control_sta;
	
	/* У��� */
	send_frame[4] = send_frame[0] + send_frame[1] + send_frame[2] + send_frame[3];
	
	uart_send_buf(send_frame, 5);
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
*					-1��У�����
*/
int8_t uartf_reciev_frame(float *temp, uint8_t *control_sta)
{

	uint8_t res = 0;

	if (g_uart_sta & 0x80) {
		
		res = __g_uart_buf[0] + __g_uart_buf[1] + __g_uart_buf[2] + __g_uart_buf[3];
		
		if (res == __g_uart_buf[4]) {
			*temp = __g_uart_buf[1] + __g_uart_buf[2]/10.0;
			*control_sta = __g_uart_buf[3];
			g_uart_sta = 0;
			return 1;
		} else {
			g_uart_sta = 0; /* ���յ�5���ֽں󣬾Ϳ��Խ���������� */
			return -1;	/* У����� */
		}
	}
		return 0;
}




/********************************************************************************************************
*	Discription		:	ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��UART1
*						ʵ���ض���ֻ��Ҫ���2������:
*						char putchar(char c);
*						int fgetc(FILE *f);
*						char getchar(void)
*						���cģ���޶�Ӧ��h�ļ���
*********************************************************************************************************/


///*
//*********************************************************************************************************
//*	�� �� ��: putchar
//*	����˵��: �ض��� putchar ������ ��������ʹ��printf�����Ӵ���1��ӡ���
//*	��    ��: �ȴ����͵��ַ�
//*	�� �� ֵ: �����ѷ��͵��ַ�
//*********************************************************************************************************
//*/
//char putchar(char c)
//{
//	/* ����һ���ַ� c ��UART1 */
//	uart_send_byte(c);

//	return (c);
//}

///*
//*********************************************************************************************************
//*	�� �� ��: getchar
//*	����˵��: �ض���C���е� getchar ����,��������ʹ��scanff�����Ӵ���1��������
//*	��    ��: ��
//*	�� �� ֵ: �����Ѷ������ַ�
//*********************************************************************************************************
//*/
//char getchar(void)
//{
//	return uart_get_byte();
//}