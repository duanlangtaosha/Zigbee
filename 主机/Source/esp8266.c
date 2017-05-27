#include "uart2.h"
#include "intrins.h"

extern uint8_t g_uart2_sta;
extern uint8_t xdata __g_uart2_buf[UART2_BUF_SIZE];


void Delay2000ms()		//@11.0592MHz
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
	
	char *strx=0;

//	uart2_send_string("2222\r\n");
	strx = strstr(( char*)__g_uart2_buf,( char*)str);
	return (u8*)strx;
}


uint8_t esp_8266_send_com ( uint8_t *com, uint8_t *ack, uint16_t waittime)
{
	uart2_send_string(com);
	uart2_send_string("\r\n");
	
	if (ack && waittime) {
	
		while (--waittime) {
			delay_ms(1);
			
			if (esp_8266_check_cmd(ack)) {
//				uart2_send_string("33333\r\n");
//				uart2_send_string(ack);
//				uart2_send_string("\r\n");
				g_uart2_sta = 0;
				break;	//响应正确
			}
		}
		
		if (0 == waittime) {
			return 1;
		}
		
	}
	return 0;
}

//extern void Delay2000ms()	;

uint8_t g_wifi_ok = 0;
void init_esp_8266_setting ( void)
{
	uint8_t res = 0;
	
	ES = 0;
	
	/* 设置模式 */
	esp_8266_send_com("AT+CWMODE=3", "OK", 1000);
//	Delay2000ms();
	
	/* 复位 */
	esp_8266_send_com("AT+RST", "ready", 5000);
	
//	Delay2000ms();
//	Delay2000ms();
	esp_8266_send_com("AT+CWSAP=\"LWL\",\"12345678\",1,4", "OK", 1000);
//	Delay2000ms();
//	Delay2000ms();
	esp_8266_send_com("AT+CWJAP=\"MERCURY_A78A\",\"zlgmcu18\"", "OK", 1000);
//	Delay2000ms();
//	Delay2000ms();
	esp_8266_send_com("AT+CIPMUX=1", "OK", 1000);
//	Delay2000ms();
//	Delay2000ms();
	res = esp_8266_send_com("AT+CIPSERVER=1,8086", "OK", 1000);
//	Delay2000ms();
//	Delay2000ms();
	
	if (res == 0) {
		g_wifi_ok = 1;
		g_uart2_sta = 0;
	}
	ES = 1;
}