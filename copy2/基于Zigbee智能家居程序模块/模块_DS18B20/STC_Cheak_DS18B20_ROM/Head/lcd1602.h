#ifndef	_LCD1602_H
#define _LCD1602_H

/* LCD1602����IO�� */
#define RS_LCD				P22			/* RS */
#define LCD_COMMAND 		0				/* RS = L (command) */	
#define	LCD_DATA    		1				/* RS = H (data)    */
#define RW_LCD				P21			/* R/W */
#define E_LCD				P20		/*  E  */
#define BUS_LCD				P0				/* DB0--DB7 */


#define	CHK_BUSY			(bit)1
#define	UNCHK_BUSY			(!CHK_BUSY)
/* LCD1602����ָ�� */
#define LCD_CLEAR_SCREEN	0x01			/* ���������ʾ����RAM����ʾRAM��ַ���������� */	
#define LCD_HOMING			0x02			/* ��ʾRAM���ݲ��䣬��ʾ����RAM�ĵ�ַ���������� */
#define MODE_SET_0			0x04			/* ��д���ݺ󣬹�������ƣ���ʾ�����ƶ� */
#define MODE_SET_1			0x05			/* д���ݵ���ʾ����RAM��������ʾ�������ƶ� */
#define MODE_SET_2			0x06			/* ��д���ݺ󣬹�������ƣ���ʾ�����ƶ� */
#define	MODE_SET_3			0x07			/* д���ݵ���ʾ����RAM����ʾ�������ƶ� */
#define OFF_DISPLAY			0x08			/* ��ʾ������ʾ */
#define OFF_CURSOR			0x0C			/* ��ʾ���ݣ�����ʾ��꣬�ֲ���˸ */
#define CHAR_BLINK			0x0D			/* ��ʾ���ݣ�����ʾ��꣬����˸ */
#define ON_CURSOR			0x0E			/* ��ʾ���ݣ���ʾ��꣬��겻��˸ */
#define CURSOR_BLINK		0x0F			/* ��ʾ���ݣ���ʾ��꣬�����˸ */
#define CURSOR__LEFT 		0x10			/* ��������ƶ�һλ */
#define	CURSOR_RIGHT		0x14			/* ��������ƶ�һλ */
#define	ALL_LEFT			0x18			/* ������ʾ�������ƶ�һλ���������ƶ� */
#define	ALL_RIGHT			0x1C			/* ������ʾ�������ƶ�һλ���������ƶ� */
#define	SET_8BITS			0x38			/* �趨��ʾ��Ϊ8bit��˫�У�5*7���� */
#define CGRAM_SET			0x40			/* CG RAM�Ŀ�ʼ��ַ���趨�� */
#define	DDRAM_SET			0x80			/* ��ʾ����RAM�Ŀ�ʼ��ַ���趨�� */

#define LINE_CHARNUM		(2*16+1)		/*LCD1602����ַ���������˫����������ַ�������β��һ���������������ַ���Ϊ16*2+1*/

extern void Initial_LCD(void);
extern void Command_LCD(unsigned char command,bit BusyFlag);
//����˵��:	������ʾ������
//����˵��:	X��������00--15;
//			Y������Y=0����һ��Y=1���ж�
extern void LocatPos(unsigned char X,unsigned char Y);
extern void DispOneChar(unsigned char X,unsigned char Y,unsigned char charactor);
extern void String_LCD(char *string);
extern unsigned char ReadLCD(void);
/**********д����ʱ��Ҫд��һ��0x30����0x30|5������д���5**********/
extern void WriteToLCD(unsigned char LCD_data);	//����д�����õ�
#endif
/*------------------------------------------end--------------------------------------------------*/