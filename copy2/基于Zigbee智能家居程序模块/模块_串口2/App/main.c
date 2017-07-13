/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series MCU UART2 (8-bit/9-bit)Demo ---------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include "stc12c5a60s2.h"
#include "intrins.h"
#include "uart2.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

//#define FOSC 11059200L      //System frequency
//#define BAUD 115200         //UART baudrate

/*Define UART parity mode*/
//#define NONE_PARITY     0   //None parity
//#define ODD_PARITY      1   //Odd parity
//#define EVEN_PARITY     2   //Even parity
//#define MARK_PARITY     3   //Mark parity
//#define SPACE_PARITY    4   //Space parity

//#define PARITYBIT EVEN_PARITY   //Testing even parity

/*Declare SFR associated with the UART2 */
//sfr AUXR  = 0x8e;           //Auxiliary register
//sfr S2CON = 0x9a;           //UART2 control register
//sfr S2BUF = 0x9b;           //UART2 data buffer
//sfr BRT   = 0x9c;           //Baudrate generator
//sfr IE2   = 0xaf;           //Interrupt control 2

//#define S2RI  0x01          //S2CON.0
//#define S2TI  0x02          //S2CON.1
//#define S2RB8 0x04          //S2CON.2
//#define S2TB8 0x08          //S2CON.3

bit busy;

void SendData(BYTE dat);
void SendString(char *s);

void main()
{
	
	float a;
	unsigned char b;
	uart2_init();
	uart2_send_string("STC12C5A60S2\r\nUart2 Test !\r\n");
    while(1)
		{
		uart2f_reciev_frame(&a, &b);
		}
}

/*----------------------------
UART2 interrupt service routine
----------------------------*/
//void Uart2() interrupt 8 using 1
//{
//    if (S2CON & S2RI)
//    {
//        S2CON &= ~S2RI;     //Clear receive interrupt flag
//        P0 = S2BUF;         //P0 show UART data
//        P2 = (S2CON & S2RB8);//P2.2 show parity bit
//    }
//    if (S2CON & S2TI)
//    {
//        S2CON &= ~S2TI;     //Clear transmit interrupt flag
//        busy = 0;           //Clear transmit busy flag
//    }
//}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(BYTE _data)
{
//    while (busy);           //Wait for the completion of the previous data is sent
//    ACC = dat;              //Calculate the even parity bit P (PSW.0)
//    if (P)                  //Set the parity bit according to P
//    {
//#if (PARITYBIT == ODD_PARITY)
//        S2CON &= ~S2TB8;    //Set parity bit to 0
//#elif (PARITYBIT == EVEN_PARITY)
//        S2CON |= S2TB8;     //Set parity bit to 1
//#endif
//    }
//    else
//    {
//#if (PARITYBIT == ODD_PARITY)
//        S2CON |= S2TB8;     //Set parity bit to 1
//#elif (PARITYBIT == EVEN_PARITY)
//        S2CON &= ~S2TB8;    //Set parity bit to 0
//#endif
//    }
//    busy = 1;
//    S2BUF = ACC;            //Send data to UART2 buffer
	
	
	S2BUF = _data;	/* 发送数据送缓冲 */
	while (!(S2CON & 0x02));		/* 等待发送完成 */
	S2CON &= ~0x02;
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}

