#ifndef	_LCD1602_H
#define _LCD1602_H

/* LCD1602控制IO口 */
#define RS_LCD				P22			/* RS */
#define LCD_COMMAND 		0				/* RS = L (command) */	
#define	LCD_DATA    		1				/* RS = H (data)    */
#define RW_LCD				P21			/* R/W */
#define E_LCD				P20		/*  E  */
#define BUS_LCD				P0				/* DB0--DB7 */


#define	CHK_BUSY			(bit)1
#define	UNCHK_BUSY			(!CHK_BUSY)
/* LCD1602控制指令 */
#define LCD_CLEAR_SCREEN	0x01			/* 清除所有显示数据RAM，显示RAM地址计数器清零 */	
#define LCD_HOMING			0x02			/* 显示RAM内容不变，显示数据RAM的地址计数器清零 */
#define MODE_SET_0			0x04			/* 读写数据后，光标往左移，显示器不移动 */
#define MODE_SET_1			0x05			/* 写数据到显示数据RAM后，整个显示器往右移动 */
#define MODE_SET_2			0x06			/* 读写数据后，光标往右移，显示器不移动 */
#define	MODE_SET_3			0x07			/* 写数据到显示数据RAM后，显示器向左移动 */
#define OFF_DISPLAY			0x08			/* 显示器不显示 */
#define OFF_CURSOR			0x0C			/* 显示数据，不显示光标，字不闪烁 */
#define CHAR_BLINK			0x0D			/* 显示数据，不显示光标，字闪烁 */
#define ON_CURSOR			0x0E			/* 显示数据，显示光标，光标不闪烁 */
#define CURSOR_BLINK		0x0F			/* 显示数据，显示光标，光标闪烁 */
#define CURSOR__LEFT 		0x10			/* 光标向左移动一位 */
#define	CURSOR_RIGHT		0x14			/* 光标向右移动一位 */
#define	ALL_LEFT			0x18			/* 整个显示器向左移动一位，光标跟着移动 */
#define	ALL_RIGHT			0x1C			/* 整个显示器向右移动一位，光标跟着移动 */
#define	SET_8BITS			0x38			/* 设定显示器为8bit，双列，5*7字体 */
#define CGRAM_SET			0x40			/* CG RAM的开始地址的设定码 */
#define	DDRAM_SET			0x80			/* 显示数据RAM的开始地址的设定码 */

#define LINE_CHARNUM		(2*16+1)		/*LCD1602最大字符串数，以双引号引起的字符串，结尾有一个结束符，所以字符数为16*2+1*/

extern void Initial_LCD(void);
extern void Command_LCD(unsigned char command,bit BusyFlag);
//功能说明:	设置显示屏坐标
//其他说明:	X列数，从00--15;
//			Y行数，Y=0，行一；Y=1，行二
extern void LocatPos(unsigned char X,unsigned char Y);
extern void DispOneChar(unsigned char X,unsigned char Y,unsigned char charactor);
extern void String_LCD(char *string);
extern unsigned char ReadLCD(void);
/**********写数的时候要写入一个0x30，如0x30|5；就是写入的5**********/
extern void WriteToLCD(unsigned char LCD_data);	//用来写数字用的
#endif
/*------------------------------------------end--------------------------------------------------*/