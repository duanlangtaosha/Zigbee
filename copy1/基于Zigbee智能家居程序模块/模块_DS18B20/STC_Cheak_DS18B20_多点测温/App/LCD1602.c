#include ".\Head\stc12c5a60s2.h"
#include ".\Head\intrins.h"
#include ".\Head\LCD1602.h"

static void CheckBusy_LCD(void);
static void WriteToLCD(unsigned char LCD_data);
static void DelayMS(unsigned char MS_Counter);
static void TCGRAM(unsigned char DispAdr,unsigned char code *SpecialPtr);
//unsigned char code ASCII_CODE[] = 	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//unsigned char code Speci_Char[] = 	{
//										0x06,0x02,0x0A,0x0A,0x0A,0x11,0x11,0x00,	/* 字符 '八' */
//				    					0x04,0x04,0x04,0x1F,0x04,0x04,0x04,0x00 	/* 字符	'十' */
//								 		};
/*************************************************************************************************
						void InitialLCD(void)
功能说明:	设定显示为8bits，设定LCD为双列显示、5×7点字体
其他说明:	在电源上升到4.5V之后10ms，忙碌标识即设为1，一直到初始化过程结束才重新为0
**************************************************************************************************/
void Initial_LCD(void)
{
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* 设置为16*2显示，5*7字符，8bit数据接口，不查忙 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* 设置为16*2显示，5*7字符，8bit数据接口，不查忙 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* 设置为16*2显示，5*7字符，8bit数据接口，不查忙 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,CHK_BUSY);		/* 设置为16*2显示，5*7字符，8bit数据接口，开始查忙 	*/
	Command_LCD(OFF_DISPLAY,CHK_BUSY);		/* 关闭LCD1602 										*/
	Command_LCD(LCD_CLEAR_SCREEN,CHK_BUSY);	/* 清屏 											*/
	Command_LCD(MODE_SET_2,CHK_BUSY);		/* 读写数据后，光标右移，屏幕不右移 				*/
	Command_LCD(OFF_CURSOR,CHK_BUSY);		/* 打开LCD1602 										*/





//	TCGRAM(00,Speci_Char);					/* 生成特殊字符，字符地址存放在00 					*/
//	TCGRAM(01,Speci_Char+8);				/* 生成特殊字符，字符地址存放在01 					*/
}
/*************************************************************************************************
						void String_LCD(char *string)
功能说明:	在LCD上显示字符
其他说明:	
**************************************************************************************************/
void String_LCD(char *string)
{
	char *strPtr = string;
	while(*strPtr !=  00)
	{
		WriteToLCD(*strPtr);
		strPtr++;
	}
}
/*************************************************************************************************
						void LocatPos(unsigned char X,unsigned char Y)
功能说明:	设置显示屏坐标
其他说明:	X列数，从00--15;
			Y行数，Y=0，行一；Y=1，行二
*************************************************************************************************/
void LocatPos(unsigned char X,unsigned char Y)
{
	unsigned char command = X & 0x0f;
	if(Y&0x01)
	command |= 0x40;
	command |= 0x80;
	Command_LCD(command,CHK_BUSY);
}
/*************************************************************************************************
						void WriteToLCD(unsigned char data)
功能说明:	写数据到LCD驱动器中，对于数据的地址需要看前一次是设定在哪个地址而定，常用带的地方是将
			字符码写入LCD后，在光标的位置会显示相对应的字体。
其他说明:	1.输入:需要显示的字符（无符号字符）
**************************************************************************************************/
void WriteToLCD(unsigned char LCD_data)
{
	CheckBusy_LCD();						/* 查忙信号 							*/	
	RS_LCD		=	1;		
	RW_LCD		=	0;
	E_LCD		=	0;
	BUS_LCD		=	LCD_data;
	E_LCD		=	1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E_LCD		=	0;
}
/*************************************************************************************************
						void Command_LCD(unsigned char command)
功能说明:	写指令到LCD驱动器
其他说明:	1.输入:控制指令
			2.由于初始化LCD1602的前几条命令不用查忙信号，所以要判断是否需要查忙信号。
**************************************************************************************************/
void Command_LCD(unsigned char command,bit BusyFlag )
{
	if(BusyFlag)							/* 判断是否需要查忙信号 				*/
	{
		CheckBusy_LCD();
	}
	RS_LCD		=	0;						/* 写命令时序 							*/
	RW_LCD		=	0;
	E_LCD		=	0;
	BUS_LCD		=	command;
	E_LCD		=	1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E_LCD		=	0;
}
/*************************************************************************************************
						void CheckBusy_LCD(void)
功能说明:	检查LCD忙标志
其他说明:	1.LCD每次写入命令或数据，内部都需要时间处理，所以每次作读/写操作之前，必须先检查忙标志，
			确定空间后，才可继续对LCD工作，否则读/写的数据后命令会被遗漏。
			2.初始化LCD1602的前20ms设置显示模式时，不用检测忙信号。
			3.此函数若在LCD驱动损坏的情况下，可能会一直死循环，故需要打开看门狗进行监视。
**************************************************************************************************/
void CheckBusy_LCD(void)
{
	BUS_LCD		=	0xff;
	RS_LCD 		=	0;						/* 读状态时序 								*/
	RW_LCD 		=	1;
	E_LCD 		=	1;
	_nop_();
	_nop_();
	_nop_();
	while(BUS_LCD&0x80);					/* 读状态字的bit7，判断是否忙 				*/
	E_LCD		=	0;						/* E = L 									*/
}
void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}
void DelayMS(unsigned char MS_Counter)
{
   while(MS_Counter--)
   {
   	   Delay1ms()	;
   }
}


