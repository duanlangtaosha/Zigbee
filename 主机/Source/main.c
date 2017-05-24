#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart.h"
#include "uart2.h"
#include "lcd12864.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"


unsigned char choose_page = 0;

/*主页*/
void home (void) 
{
	
	lcd12864_write_cmd(0x01);

  lcd12864_set_window(0, 0);						
	lcd12864_write_string("主机温度:");
	lcd12864_set_window(0, 7);						
	lcd12864_write_string("℃");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("从机温度:");
	lcd12864_set_window(1, 7);						
	lcd12864_write_string("℃");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("日期:");
	lcd12864_set_window(2, 3);	
	lcd12864_write_string("");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("时间:");
	lcd12864_set_window(3, 3);
	lcd12864_write_string("");
	while (1) {
		unsigned char ds1302_buf[7] = {0};
		unsigned char keyval = keyscan();
		unsigned char temp = rd_temperature_i();
		
		float device_temp = 0.0;
		unsigned char control = 0;
		if (EN_KEY0 == keyval) {
		
			choose_page = 1; /* 进入主菜单*/
			break;
		}
			lcd12864_set_window(0, 5);
			lcd12864_write_data((temp / 10) | 0x30 );
			lcd12864_write_data((temp % 10) | 0x30 );
		
		
			if (uartf_reciev_frame(&device_temp, &control)) {
				temp = (unsigned char)device_temp;
				lcd12864_set_window(1, 5);
				lcd12864_write_data((temp / 10) | 0x30 );
				lcd12864_write_data((temp % 10) | 0x30 );
				
				//********************************************************************************************
				uart_send_byte(control);
			}
			
			get_ds1302_time(ds1302_buf);
			lcd12864_set_window(3, 3);
			lcd12864_write_data((ds1302_buf[2] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[2] % 10) | 0x30 );
			lcd12864_write_string(":");
			lcd12864_write_data((ds1302_buf[1] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[1] % 10) | 0x30 );
			lcd12864_write_string(":");
			lcd12864_write_data((ds1302_buf[0] / 10) | 0x30 );
			lcd12864_write_data((ds1302_buf[0] % 10) | 0x30 );
			
	}
}

/*主菜单*/
void menu (void)
{
	lcd12864_write_cmd(0x01);
	choose_page++;
	lcd12864_set_window(0, 0);
	lcd12864_write_string("≡≡主菜单≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("1.家电");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("2.设置");
	lcd12864_set_window(3, 0);
	lcd12864_write_string("3.其他");
	
	while (1) {

		unsigned char keyval = keyscan();	
		if (EN_KEY1 == keyval) {	/* 菜单下选择 */
					
			choose_page++;
			if (choose_page == 5) {
				choose_page = 2;
			}
			
		} else if (EN_KEY2 == keyval) { /* 菜单上选择 */
			choose_page--;
			if (choose_page == 1) {
				choose_page = 4;
			}
		} else if (EN_KEY3 == keyval) {
			choose_page = 0;
			break;
		} else if (EN_KEY0 == keyval) {
				break;
		}
		
		switch (choose_page) {
			case 2:
				lcd12864_set_window(3, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(1, 3);
				lcd12864_write_data(17);
			break;
			
			case 3:
				lcd12864_set_window(1, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(3, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(17);
				break;
			case 4:
				lcd12864_set_window(1, 3);
				lcd12864_write_data(32);	
			
				lcd12864_set_window(2, 3);
				lcd12864_write_data(32);
			
				lcd12864_set_window(3, 3);
				lcd12864_write_data(17);
				break;
		}
		
	}
}


void elec_app (void)
{
	unsigned char elc_num = 0;
	
//	float temp = 0.0;
	unsigned char elc_sta = 0;
  lcd12864_write_cmd(0x01);

	lcd12864_set_window(0, 1);
	lcd12864_write_string("≡≡家电≡≡");
	lcd12864_set_window(1, 0);
	lcd12864_write_string("E1:OFF");
	lcd12864_set_window(2, 0);
	lcd12864_write_string("E2:OFF");
	lcd12864_set_window(1, 4);
	lcd12864_write_string("E3:OFF");
	lcd12864_set_window(2, 4);
	lcd12864_write_string("E4:OFF");
	while (1) {
		float temp = 10.0;
		unsigned char keyval = keyscan();
		
		/* 获取当前从机家用电器的状态 */
		if (uartf_reciev_frame(&temp, &elc_sta)) {
			
			//****************************************************************************
//			uart_send_byte(elc_sta);
			if(elc_sta & 0x01) {
				lcd12864_set_window(1, 0);
				lcd12864_write_string("E1:ON ");
			} else {
				lcd12864_set_window(1, 0);
				lcd12864_write_string("E1:OFF");
			}
			
			if(elc_sta & 0x02) {
				lcd12864_set_window(2, 0);
				lcd12864_write_string("E2:ON ");
			} else {
				lcd12864_set_window(2, 0);
				lcd12864_write_string("E2:OFF");
			}
			
			if(elc_sta & 0x04) {
				lcd12864_set_window(1, 4);
				lcd12864_write_string("E3:ON ");
			} else {
				lcd12864_set_window(1, 4);
				lcd12864_write_string("E3:OFF");
			}

			if(elc_sta & 0x08) {
				lcd12864_set_window(2, 4);
				lcd12864_write_string("E4:ON ");
			} else {
				lcd12864_set_window(2, 4);
				lcd12864_write_string("E4:OFF");
			}
		} else {
			
			/* 不加上这句校验和就会永远的出错 */
			uart_send_string("eorr\r\n");
		}
		
		
		/* 返回上一页 */
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		} else if (EN_KEY1 == keyval) {
			elc_num++;
			if(elc_num == 5) {
				elc_num = 1;
			}
				switch (elc_num) {

				case 1:
					lcd12864_set_window(2, 7);
					lcd12864_write_data(32);	
				
					lcd12864_set_window(1, 3);
					lcd12864_write_data(17);
					
					break;
				case 2:
					
					lcd12864_set_window(1, 3);
					lcd12864_write_data(32);
				
					lcd12864_set_window(2, 3);
					lcd12864_write_data(17);
					break;
				case 3:
					lcd12864_set_window(2, 3);
					lcd12864_write_data(32);
				
					lcd12864_set_window(1, 7);
					lcd12864_write_data(17);
					break;
				case 4:
					lcd12864_set_window(1, 7);
					lcd12864_write_data(32);
				
					lcd12864_set_window(2, 7);
					lcd12864_write_data(17);
					break;
				default:
					break;
			}
		} else if (EN_KEY0 == keyval) {
				switch (elc_num) {
				case 1:
					if (elc_sta & 0x01) {
						elc_sta &= ~0x01;
					} else {
						elc_sta |= 0x01;
					}
					uart_send_frame(0.0, elc_sta);
							
					break;
				case 2:
					

					break;
				case 3:

					break;
				case 4:

					break;
				default:
					break;
			}
		}
	}

}

void setting (void)
{
	lcd12864_write_cmd(0x01);
	while (1) {
		unsigned char keyval = keyscan();
		lcd12864_set_window(0, 0);
		lcd12864_write_string("设置");
		
		
				
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
	}
}

void other (void )
{
	lcd12864_write_cmd(0x01);
	while (1) {
		
		unsigned char keyval = keyscan();
		if (EN_KEY3 == keyval) {
			choose_page = 1;
			break;
		}
		lcd12864_set_window(0, 0);
		lcd12864_write_string("其他");
	
	}

}




void main (void)
{

	uart_init();
	uart2_init();
	lcd12864_init ();
	init_ds18b20();
	init_ds1302();
	
	while (1) {
		
		unsigned char *p = "abcd";
		unsigned char buuu[5]={0x11 ,0x22, 0x33,0x44,0x55};
		float a = 18.5,c = 0.0;
		unsigned char b = 0x01, d = 0;


//		if(uartf_reciev_frame(&c, &d))
//		{
//		
//		uart_send_byte(c/10 + 0x30);
//		uart_send_byte((unsigned char)c%10 + 0x30);
//		uart_send_byte((unsigned char)(c*10)%10 +0x30);
//		uart_send_byte('\r');
//		uart_send_byte('\n');
//		uart_send_byte(d + 0x30);
//		}
//		uart2_send_string("abcdefg");

		
		switch (choose_page) {
		
		case 0:
			home () ;
			break;
		case 1:
			menu();
			break;
		case 2:
			elec_app();
			break;
			
		case 3:
			setting();
			break;
		case 4:
			other();
			break;
			default:
				break;
		
		}

		
	}

}




///*------------------------------------------------------------------*/
///* --- STC MCU Limited ---------------------------------------------*/
///* --- STC12C5Axx Series MCU UART (8-bit/9-bit)Demo ----------------*/
///* --- Mobile: (86)13922805190 -------------------------------------*/
///* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
///* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
///* --- Web: www.STCMCU.com -----------------------------------------*/
///* --- Web: www.GXWMCU.com -----------------------------------------*/
///* If you want to use the program or the program referenced in the  */
///* article, please specify in which data and procedures from STC    */
///*------------------------------------------------------------------*/

//#include "reg51.h"
//#include "intrins.h"

//typedef unsigned char BYTE;
//typedef unsigned int WORD;

//#define FOSC 11059200L      //System frequency
//#define BAUD 9600           //UART baudrate

///*Define UART parity mode*/
//#define NONE_PARITY     0   //None parity
//#define ODD_PARITY      1   //Odd parity
//#define EVEN_PARITY     2   //Even parity
//#define MARK_PARITY     3   //Mark parity
//#define SPACE_PARITY    4   //Space parity

//#define PARITYBIT EVEN_PARITY   //Testing even parity

//sbit bit9 = P2^2;           //P2.2 show UART data bit9
//bit busy;

//void SendData(BYTE dat);
//void SendString(char *s);
//void Delay100ms()		//@11.0592MHz
//{
//	unsigned char i, j, k;

//	_nop_();
//	_nop_();
//	i = 5;
//	j = 52;
//	k = 195;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}

//void main()
//{
//#if (PARITYBIT == NONE_PARITY)
//    SCON = 0x50;            //8-bit variable UART
//#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
//#elif (PARITYBIT == SPACE_PARITY)
//    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
//#endif

//    TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
//    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
//    TR1 = 1;                //Timer1 start run
//    ES = 1;                 //Enable UART interrupt
//    EA = 1;                 //Open master interrupt switch

////    SendString("STC12C5A60S2\r\nUart Test !\r\n");
//    while(1){
//		SendString("STC12C5A60S2\r\nUart Test !\r\n");
//			Delay100ms();
//		}
//}

///*----------------------------
//UART interrupt service routine
//----------------------------*/
//void Uart_Isr() interrupt 4 using 1
//{
//    if (RI)
//    {
//        RI = 0;             //Clear receive interrupt flag
//        P0 = SBUF;          //P0 show UART data
//        bit9 = RB8;         //P2.2 show parity bit
//    }
//    if (TI)
//    {
//        TI = 0;             //Clear transmit interrupt flag
//        busy = 0;           //Clear transmit busy flag
//    }
//}

///*----------------------------
//Send a byte data to UART
//Input: dat (data to be sent)
//Output:None
//----------------------------*/
//void SendData(BYTE dat)
//{
//    while (busy);           //Wait for the completion of the previous data is sent
//    ACC = dat;              //Calculate the even parity bit P (PSW.0)
//    if (P)                  //Set the parity bit according to P
//    {
//#if (PARITYBIT == ODD_PARITY)
//        TB8 = 0;            //Set parity bit to 0
//#elif (PARITYBIT == EVEN_PARITY)
//        TB8 = 1;            //Set parity bit to 1
//#endif
//    }
//    else
//    {
//#if (PARITYBIT == ODD_PARITY)
//        TB8 = 1;            //Set parity bit to 1
//#elif (PARITYBIT == EVEN_PARITY)
//        TB8 = 0;            //Set parity bit to 0
//#endif
//    }
//    busy = 1;
//    SBUF = ACC;             //Send data to UART buffer
//}

///*----------------------------
//Send a string to UART
//Input: s (address of string)
//Output:None
//----------------------------*/
//void SendString(char *s)
//{
//    while (*s)              //Check the end of the string
//    {
//        SendData(*s++);     //Send current char and increment string ptr
//    }
//}


