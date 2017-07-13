#include "uart2.h"
#include "intrins.h"
#include "string.h" 

#define ESP8266_RST P32

extern uint8_t g_uart2_sta;
extern uint8_t xdata __g_uart2_buf[UART2_BUF_SIZE];


static void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void delay_ms (uint8_t time) 
{
		unsigned char i, j;
	unsigned char t;
	for (t = 0; t < time; t++) {
			_nop_();
			i = 11;
			j = 190;
			do
			{
				while (--j);
			} while (--i);
	}


}

uint8_t* esp_8266_check_cmd(uint8_t *str)
{
	
	char *strx = 0;

//	uart2_send_string("2222\r\n");
	strx = strstr(( char*)__g_uart2_buf,( char*)str);
	return (uint8_t*)strx;
}


uint8_t esp_8266_send_com ( uint8_t *com, uint8_t *ack, uint16_t waittime)
{
	uart2_send_string(com);
	uart2_send_string("\r\n");
	
	if (ack && waittime) {
	
		while (--waittime) {
			delay_ms(1);
			
			if (esp_8266_check_cmd(ack)) {
				g_uart2_sta = 0;
				break;	//响应正确
			}
		}
		
		if (0 == waittime) {
			/* 加上这句会有ERROR */
//			g_uart2_sta = 0; /////////////////////////////////////////////////////////////
			return 1;
		}
		
	}
	return 0;
}


//uint8_t xdata g_staip_buf[32] = {0};
//uint8_t xdata g_apip_buf[32] = {0};
//extern uint8_t g_uart2_sta;

//void esp_8266_get_ip (void)
//{
//	uint8_t  *p = NULL;
//	uint8_t *p1 = NULL;
//	uint8_t  *p2 = NULL;
//	uint8_t *ipbuf = NULL;
//	uint8_t  *buf = NULL;
//	
//	uint8_t xdata p_buf[32] = {0};
//	uint8_t xdata p1_buf[32] = {0};
//	uint8_t xdata p2_buf[32] = {0};
//	uint8_t xdata ipbuf_buf[32] = {0};
//	uint8_t xdata buf_buf[32] = {0};
//	
//	p = p_buf;
//	p1=p1_buf;
//	p2=p2_buf;
//	ipbuf = ipbuf_buf;
//	buf = buf_buf;

//	g_uart2_sta = 0;
//	if (esp_8266_send_com("AT+CIFSR","OK",1000)) {
//	
//	} else {
////		                          APIP,
////															APIP,"
//		p = esp_8266_check_cmd("APIP,\"");
////		p = esp_8266_check_cmd("AP");
//		p1 = (u8*)strstr((char*)(p + 6), "\"");

//		p2 = p1;
//		*p1 = 0;
//		ipbuf = p + 6;
//		
//		/* apip的IP地址 */
////		sprintf((char*)buf,"AP IP:%s 端口:%s",ipbuf,(u8*)portnum);
//		sprintf((uint8_t*)g_apip_buf,"%s",ipbuf);
////		sprintf((uint8_t*)g_apip_buf,"%s",p);
////		sprintf((uint8_t*)g_apip_buf,"%s","abcdddddddd");
//		
//		p = (u8*)strstr((char*)(p2 + 1), "STAIP,\"");
//		p1 = (u8*)strstr((char*)(p + 7), "\"");

//		*p1 = 0;
//		ipbuf = p + 7;
//		
//		sprintf((char*)g_staip_buf, "%s",ipbuf);
//	
//	}
//}

uint8_t g_wifi_ok = 0;
void init_esp_8266_setting ( void)
{
	uint8_t res = 0;
	
	ES = 0;
	ESP8266_RST = 0;
	delay_ms(100);
	ESP8266_RST = 1;
	Delay2000ms();
	Delay2000ms();
	/* 设置模式 */
	esp_8266_send_com("AT+CWMODE=3", "OK", 1000);

	/* 复位 */
	esp_8266_send_com("AT+RST", "ready", 5000);
	
	esp_8266_send_com("AT+CIPAP=\"192.168.4.1\"", "OK", 2000);
	Delay2000ms();
	Delay2000ms();
	
//	esp_8266_send_com("AT+CIPSTA=\"192.168.1.200\"", "OK", 2000);
	Delay2000ms();
	Delay2000ms();
	
	esp_8266_send_com("AT+CWSAP=\"LWL\",\"12345678\",1,4", "OK", 1000);
	Delay2000ms();

//	esp_8266_send_com("AT+CWJAP=\"MERCURY_A78A\",\"zlgmcu18\"", "OK", 1000);
	Delay2000ms();

	esp_8266_send_com("AT+CIPMUX=1", "OK", 1000);
	Delay2000ms();

	res = esp_8266_send_com("AT+CIPSERVER=1,8086", "OK", 1000);
	Delay2000ms();
//	esp_8266_get_ip ();
	if (res == 0) {
		g_wifi_ok = 1;
		g_uart2_sta = 0;
	}
	ES = 1;
}

