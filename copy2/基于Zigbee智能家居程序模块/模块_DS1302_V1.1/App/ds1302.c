/*******************************************************************************
*  标题:  试验数码管显示时钟                                                   *
*																			   *
* 通过本例程了解 DS1302时钟芯片的基本原理和使用 ,理解并掌握DS1302时钟芯片	   *
* 驱动程序的编写以及实现数字字符在数码管中的显示。							   *
* 注意：JP1302跳线冒要短接。                   	                               *
* 请学员认真消化本例程，懂DS1302在C语言中的操作                       	       *
********************************************************************************/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include <intrins.h>

sbit SCK=P2^3;	//时钟	
sbit SDA=P2^4;	//数据	
sbit RST = P2^5;// DS1302复位



bit ReadRTC_Flag;//定义读DS1302标志

unsigned char l_tmpdate[7]={0,0,12,15,5,3,8};//秒分时日月周年08-05-15 12:00:00
unsigned char l_tmpdisplay[8];

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //秒分时日月周年 最低位读写位
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  


/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/												
void Write_Ds1302_byte(unsigned char temp); 
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void Read_RTC(void);//read RTC 
void Set_RTC(void); //set RTC 

/******************************************************************/
/*                   写一个字节                                   */
/******************************************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 unsigned char i;
 for (i=0;i<8;i++)     	//循环8次 写入数据
  { 
   SCK=0;
     SDA=temp&0x01;     //每次传输低字节 
     temp>>=1;  		//右移一位
     SCK=1;
   }
}   
/******************************************************************/
/*                  写入DS1302                                    */
/******************************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();                    //启动
 	Write_Ds1302_Byte(address);	//发送地址
 	Write_Ds1302_Byte(dat);		//发送数据
 	RST=0;  		            //恢复
}
/******************************************************************/
/*                   读出DS1302数据                               */
/******************************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
	_nop_();
	_nop_();
 	SCK=0;
	_nop_();
	_nop_();
 	RST=1;
	_nop_();
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{		
 		if(SDA)
 		temp|=0x80;			//每次传输低字节
		SCK=0;
		temp>>=1;			//右移一位
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 
 	RST=0;
	_nop_();	          	//以下为DS1302复位的稳定时间
 	_nop_();
	RST=0;
	SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCK=1;
	_nop_();
	_nop_();
	SDA=0;
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	return (temp);			//返回
}
/******************************************************************/
/*                   读时钟数据                                   */
/******************************************************************/
void Read_RTC(void)	        //读取 日历
{
 unsigned char i,*p;
 p=read_rtc_address; 	    //地址传递
 for(i=0;i<7;i++)		    //分7次读取 秒分时日月周年
 {
  l_tmpdate[i]=Read_Ds1302(*p);
  p++;
 }
}
/******************************************************************/
/*                  设定时钟数据                                  */
/******************************************************************/
void Set_RTC(void)		    //设定 日历
{
	unsigned char i,*p,tmp;
	for(i=0;i<7;i++){       //BCD处理
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}  
 	Write_Ds1302(0x8E,0X00);
	
 	p=write_rtc_address;	//传地址	
 	for(i=0;i<7;i++)		//7次写入 秒分时日月周年
 	{
		  Write_Ds1302(*p,l_tmpdate[i]);
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);
}


