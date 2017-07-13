/*******************************************************************************
*  ����:  �����������ʾʱ��                                                   *
*																			   *
* ͨ���������˽� DS1302ʱ��оƬ�Ļ���ԭ���ʹ�� ,��Ⲣ����DS1302ʱ��оƬ	   *
* ��������ı�д�Լ�ʵ�������ַ���������е���ʾ��							   *
* ע�⣺JP1302����ðҪ�̽ӡ�                   	                               *
* ��ѧԱ�������������̣���DS1302��C�����еĲ���                       	       *
********************************************************************************/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include <intrins.h>

sbit SCK=P2^3;	//ʱ��	
sbit SDA=P2^4;	//����	
sbit RST = P2^5;// DS1302��λ



bit ReadRTC_Flag;//�����DS1302��־

unsigned char l_tmpdate[7]={0,0,12,15,5,3,8};//���ʱ��������08-05-15 12:00:00
unsigned char l_tmpdisplay[8];

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //���ʱ�������� ���λ��дλ
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  


/******************************************************************/
/*                    ��������                                    */
/******************************************************************/												
void Write_Ds1302_byte(unsigned char temp); 
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void Read_RTC(void);//read RTC 
void Set_RTC(void); //set RTC 

/******************************************************************/
/*                   дһ���ֽ�                                   */
/******************************************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 unsigned char i;
 for (i=0;i<8;i++)     	//ѭ��8�� д������
  { 
   SCK=0;
     SDA=temp&0x01;     //ÿ�δ�����ֽ� 
     temp>>=1;  		//����һλ
     SCK=1;
   }
}   
/******************************************************************/
/*                  д��DS1302                                    */
/******************************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();                    //����
 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	RST=0;  		            //�ָ�
}
/******************************************************************/
/*                   ����DS1302����                               */
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
 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{		
 		if(SDA)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		SCK=0;
		temp>>=1;			//����һλ
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 
 	RST=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
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
	return (temp);			//����
}
/******************************************************************/
/*                   ��ʱ������                                   */
/******************************************************************/
void Read_RTC(void)	        //��ȡ ����
{
 unsigned char i,*p;
 p=read_rtc_address; 	    //��ַ����
 for(i=0;i<7;i++)		    //��7�ζ�ȡ ���ʱ��������
 {
  l_tmpdate[i]=Read_Ds1302(*p);
  p++;
 }
}
/******************************************************************/
/*                  �趨ʱ������                                  */
/******************************************************************/
void Set_RTC(void)		    //�趨 ����
{
	unsigned char i,*p,tmp;
	for(i=0;i<7;i++){       //BCD����
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}  
 	Write_Ds1302(0x8E,0X00);
	
 	p=write_rtc_address;	//����ַ	
 	for(i=0;i<7;i++)		//7��д�� ���ʱ��������
 	{
		  Write_Ds1302(*p,l_tmpdate[i]);
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);
}


