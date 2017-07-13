/*************	本地常量声明	**************/
#define MAIN_Fosc		22118400L	//定义主时钟
#define	RX1_Lenth		32			//串口接收缓冲长度
#define	BaudRate1		115200UL	//选择波特率
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 重装值， 对应300KHZ
#include	"STC15Fxxxx.H"
/*************	本地变量声明	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//接收缓冲
u8	TX1_Cnt;	//发送计数
u8	RX1_Cnt;	//接收计数
bit	B_TX1_Busy;	//发送忙标志
/*************	端口引脚定义	**************/
unsigned char two_lab=0;
sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P3^7;//LED3
sbit DK1=P3^3;//继电器
sbit BEEP=P3^4;//蜂鸣器
sbit K1=P1^3;//按键1
sbit K2=P1^2;//按键2
sbit K3=P1^4;//按键3
char led1bl,led2bl,led3bl;
code unsigned char AT_RST[]="AT+RST"; 
code unsigned char AT_MODE[]="AT+CWMODE=3";
code unsigned char AT_CWJAP[]="AT+CWJAP=\"360we\",\"zty0012001\"";
code unsigned char CIPSTART[]="AT+CIPSTART=\"UDP\",\"cloud.ai-thinker.com\",5001";
code unsigned char CIPSEND_LEN[]="AT+CIPSEND=38";
unsigned char test_buff[38]={0x55,0x26,0x00,0xA0,0x00,0xA8,0x16,0x16,0xA8,0x02,0xE6,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0xE6,0xBD,0x5B,0x55};
unsigned send_buf[64];
unsigned recd_buf[64];
bit have_data=0;
unsigned char rec_len=0;
void Delay1(unsigned long ms)
{
	unsigned char i, j,k;
	for(k=0;k<ms;k++)
	{
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
}
void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*100;i++);
}
void at_uart_send_str(unsigned char *str)
{
  unsigned char *st_p=str;
  do{
     SBUF=*st_p;
	 st_p++;
	 Delay1(1);
	}while(*st_p);
	SBUF='\r';
	Delay1(1);
	SBUF='\n';
	Delay1(1);
}
void at_uart_send_buf(unsigned char *str,unsigned char len)
{
  unsigned char *st_p=str;
  
  while(len){
     SBUF=*st_p;
	 st_p++;
	 Delay1(1);
	 len--;
	}while(*st_p);
	SBUF='\r';
	Delay1(1);
	SBUF='\n';
	Delay1(1);
}
void main(void)
{
	char i=0;
	B_TX1_Busy = 0;
	RX1_Cnt = 0;
	TX1_Cnt = 0;
	S1_8bit();				//8位数据
	S1_USE_P30P31();		//UART1 使用P30 P31口	默认
	AUXR &= ~(1<<4);	//Timer stop		波特率使用Timer2产生
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (u8)(Timer2_Reload >> 8);
	TL2 = (u8)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断
	P3M1 = 0x00;
    P3M0 = 0xFF;
	RX1_Cnt=0;
	DK1=0;
	BEEP=0;
	for(;;)//AT+CWMODE=2 设置成路由模式
	{
		//SBUF='A';Delay1(1);
		Delay2(2000);
        at_uart_send_str(AT_MODE);
		Delay2(2000);
        at_uart_send_str(AT_CWJAP);
		Delay2(4000);
        at_uart_send_str(CIPSTART);
		Delay2(2000);
        at_uart_send_str(CIPSEND_LEN);
		Delay2(2000);
		at_uart_send_buf(test_buff,38);
		while(1)
		{
		  if(have_data)
		  {
		    have_data=0;
			LED1=0;
			Delay2(200);
		    LED1=1;
		  }
		}
	}
	Delay2(2000);
}

/********************* UART1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	char i,a,b,c;
	if(RI)
	{
		RI = 0;
		LED1=0;
		LED1=1;
		recd_buf[RX1_Cnt] = SBUF;		//保存一个字节
		if(RX1_Cnt<62)
		RX1_Cnt++;
		else
		RX1_Cnt=0;
		 if(recd_buf[RX1_Cnt]==0X55)
		   two_lab++;
		 if(two_lab==2)
		 {
		   have_data=1;
		   two_lab=0;
		   rec_len=RX1_Cnt;
		   RX1_Cnt=0;
		 }
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}
