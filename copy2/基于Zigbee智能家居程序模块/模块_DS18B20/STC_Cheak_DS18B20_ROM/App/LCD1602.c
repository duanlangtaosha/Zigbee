#include ".\Head\stc12c5a60s2.h"
#include ".\Head\intrins.h"
#include ".\Head\LCD1602.h"

static void CheckBusy_LCD(void);
static void WriteToLCD(unsigned char LCD_data);
static void DelayMS(unsigned char MS_Counter);
static void TCGRAM(unsigned char DispAdr,unsigned char code *SpecialPtr);
//unsigned char code ASCII_CODE[] = 	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//unsigned char code Speci_Char[] = 	{
//										0x06,0x02,0x0A,0x0A,0x0A,0x11,0x11,0x00,	/* �ַ� '��' */
//				    					0x04,0x04,0x04,0x1F,0x04,0x04,0x04,0x00 	/* �ַ�	'ʮ' */
//								 		};
/*************************************************************************************************
						void InitialLCD(void)
����˵��:	�趨��ʾΪ8bits���趨LCDΪ˫����ʾ��5��7������
����˵��:	�ڵ�Դ������4.5V֮��10ms��æµ��ʶ����Ϊ1��һֱ����ʼ�����̽���������Ϊ0
**************************************************************************************************/
void Initial_LCD(void)
{
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* ����Ϊ16*2��ʾ��5*7�ַ���8bit���ݽӿڣ�����æ 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* ����Ϊ16*2��ʾ��5*7�ַ���8bit���ݽӿڣ�����æ 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,UNCHK_BUSY);		/* ����Ϊ16*2��ʾ��5*7�ַ���8bit���ݽӿڣ�����æ 	*/
	DelayMS(5);
	Command_LCD(SET_8BITS,CHK_BUSY);		/* ����Ϊ16*2��ʾ��5*7�ַ���8bit���ݽӿڣ���ʼ��æ 	*/
	Command_LCD(OFF_DISPLAY,CHK_BUSY);		/* �ر�LCD1602 										*/
	Command_LCD(LCD_CLEAR_SCREEN,CHK_BUSY);	/* ���� 											*/
	Command_LCD(MODE_SET_2,CHK_BUSY);		/* ��д���ݺ󣬹�����ƣ���Ļ������ 				*/
	Command_LCD(OFF_CURSOR,CHK_BUSY);		/* ��LCD1602 										*/





//	TCGRAM(00,Speci_Char);					/* ���������ַ����ַ���ַ�����00 					*/
//	TCGRAM(01,Speci_Char+8);				/* ���������ַ����ַ���ַ�����01 					*/
}
/*************************************************************************************************
						void String_LCD(char *string)
����˵��:	��LCD����ʾ�ַ�
����˵��:	
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
����˵��:	������ʾ������
����˵��:	X��������00--15;
			Y������Y=0����һ��Y=1���ж�
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
����˵��:	д���ݵ�LCD�������У��������ݵĵ�ַ��Ҫ��ǰһ�����趨���ĸ���ַ���������ô��ĵط��ǽ�
			�ַ���д��LCD���ڹ���λ�û���ʾ���Ӧ�����塣
����˵��:	1.����:��Ҫ��ʾ���ַ����޷����ַ���
**************************************************************************************************/
void WriteToLCD(unsigned char LCD_data)
{
	CheckBusy_LCD();						/* ��æ�ź� 							*/	
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
����˵��:	дָ�LCD������
����˵��:	1.����:����ָ��
			2.���ڳ�ʼ��LCD1602��ǰ��������ò�æ�źţ�����Ҫ�ж��Ƿ���Ҫ��æ�źš�
**************************************************************************************************/
void Command_LCD(unsigned char command,bit BusyFlag )
{
	if(BusyFlag)							/* �ж��Ƿ���Ҫ��æ�ź� 				*/
	{
		CheckBusy_LCD();
	}
	RS_LCD		=	0;						/* д����ʱ�� 							*/
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
����˵��:	���LCDæ��־
����˵��:	1.LCDÿ��д����������ݣ��ڲ�����Ҫʱ�䴦������ÿ������/д����֮ǰ�������ȼ��æ��־��
			ȷ���ռ�󣬲ſɼ�����LCD�����������/д�����ݺ�����ᱻ��©��
			2.��ʼ��LCD1602��ǰ20ms������ʾģʽʱ�����ü��æ�źš�
			3.�˺�������LCD�����𻵵�����£����ܻ�һֱ��ѭ��������Ҫ�򿪿��Ź����м��ӡ�
**************************************************************************************************/
void CheckBusy_LCD(void)
{
	BUS_LCD		=	0xff;
	RS_LCD 		=	0;						/* ��״̬ʱ�� 								*/
	RW_LCD 		=	1;
	E_LCD 		=	1;
	_nop_();
	_nop_();
	_nop_();
	while(BUS_LCD&0x80);					/* ��״̬�ֵ�bit7���ж��Ƿ�æ 				*/
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


