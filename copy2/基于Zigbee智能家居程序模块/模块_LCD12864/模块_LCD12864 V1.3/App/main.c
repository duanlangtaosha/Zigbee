#include "stc12c5a60s2.h"
#include "./Head/lcd12864.h"
#include "stdio.h"
int main()
{
	unsigned char string_num[10]={0};
	unsigned int a=10116,b=3;
	float c=1.234;
	lcd12864_init();

	
	while(1)
	{
		lcd12864_set_window(0, 2);
		printf("断浪淘沙");
		lcd12864_set_window(1, 3);
		printf("V1.3");
		lcd12864_set_window(2, 0);
		
		lcd12864_write_string("打印整数 :");	
		lcd12864_write_uint_string(125);
		lcd12864_set_window(3, 0);
		lcd12864_write_string("打印小数 :");
		lcd12864_write_float_string(3.145);
//		printf("%d",1234u);		//如果打印数字的话，必须是无符号的，否则如：只写一个1的话，就会打出256
//		
//		
//		lcd12864_SetWindow(1,0);
//		printf("V1.1");
////		lcd12864_DrowPic(Photo1);
////		lcd12864_DrowPoint(127,63);		//再右下角画上一个点
////		printf("%s",string_num);
//		sprintf(string_num,"%d",(unsigned int)(a+b));
//		lcd12864_WriteString(string_num);
//		sprintf(string_num,"%f",c);
////		printf("%s",string_num);
////		lcd12864_WriteString(string_num);
//		lcd12864_WriteFloatString(3.145);
//		
//		lcd12864_SetWindow(2,0);
//		lcd12864_WriteUINTString(125);
		while(1);
	}
}