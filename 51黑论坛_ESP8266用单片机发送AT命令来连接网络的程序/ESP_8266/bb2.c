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
#define O_PF    0X00  //包头1字节固定0X55 
#define O_LEN_L 0X01  //整个包长低字节
#define O_LEN_H 0X02  //整个包长高字节 注意转义码 两个这里只计算一个数据处理！
#define O_CMD_T 0X03  //命令类型
#define O_CMD_C 0X04  //具体命令
#define O_CIX_L 0X05  //本命令序列编号低字节
#define O_CIX_H 0X06  //本命令序列编号高字节
#define O_EXMSH 0X07  //扩展信息高字节
#define O_EXMSL 0X08  //扩展信息低字节
#define O_RESTA 0X09  //数据包状态信息，成功 失败 请求 未知
#define O_DEVID 0x0A  //8字节设备识别
   //1 BYTS TL_LEN	  //获得的设备临时通讯令牌长度
   //N BYTS TK 		  //TL_LEN个通讯令牌串
   //N BYTS DATAS	  //客户的数据包串
   //CRC_L    1BYTE	  //CRC16 低字节
   //CRC_H    1BYTE	  //CRC16 高字节
   //PACK_END 1BYTE	  //包尾1字节固定0X55

//

char led1bl,led2bl,led3bl;
code unsigned char AT_RST[]="AT+RST"; 
code unsigned char AT_MODE[]="AT+CWMODE=3";
code unsigned char AT_CWJAP[]="AT+CWJAP=\"360we\",\"zty0012001\"";
code unsigned char CIPSTART[]="AT+CIPSTART=\"UDP\",\"cloud.ai-thinker.com\",5001";
code unsigned char CIPSEND_LEN[]="AT+CIPSEND=4,38";
unsigned char at_send_len_ox[16];
unsigned char test_buff[38]={0x55,0x26,0x00,0xA0,0x00,0xA8,0x16,0x16,0xA8,0x02,0xE6,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0xE6,0xBD,0x5B,0x55};
data unsigned char send_buf[64];
pdata unsigned char recd_buf[64];
unsigned char temp_buf[64];

unsigned char toke[8]={0,0,0,0,0,0,0,0,};
code unsigned int crc_table[256]=
{               /* CRC余式表 */
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
bit t_o=0;
void make_AT_SEND(unsigned char a_len)
{
  unsigned char aa=0;
  for(aa=0;aa<11;aa++)
  {
    at_send_len_ox[aa]=CIPSEND_LEN[aa];
  }
  t_o=0;
  if((a_len/100))
  {
  at_send_len_ox[aa]=a_len/100+'0';
  aa++;
  t_o=1;
  }
  if((a_len%100)/10)
  {
  at_send_len_ox[aa]=(a_len%100)/10+'0';
  aa++;
  t_o=1;
  }
  else if(t_o)
  {
    at_send_len_ox[aa]=0+'0';
	aa++;
  }
  at_send_len_ox[aa]=(a_len%10)+'0';
  aa++;
  at_send_len_ox[aa]=0;
}
unsigned int GetRevCrc_16(unsigned char * pData, int nLength)
{
  unsigned int cRc_16 = 0xffff;
  unsigned char temp;

  while(nLength-- > 0)
  {
    temp = cRc_16&0xff; 
    cRc_16 = (cRc_16 >> 8) ^ crc_table[(temp ^ *pData++) & 0xFF];
  }

  return cRc_16;    
}
#define O_PF    0X00  //包头1字节固定0X55 
#define O_LEN_L 0X01  //整个包长低字节
#define O_LEN_H 0X02  //整个包长高字节 注意转义码 两个这里只计算一个数据处理！
#define O_CMD_T 0X03  //命令类型
#define O_CMD_C 0X04  //具体命令
#define O_CIX_L 0X05  //本命令序列编号低字节
#define O_CIX_H 0X06  //本命令序列编号高字节
#define O_EXMSH 0X07  //扩展信息高字节
#define O_EXMSL 0X08  //扩展信息低字节
#define O_RESTA 0X09  //数据包状态信息，成功 失败 请求 未知
#define O_DEVID_START 0x0A  //8字节设备识别  低字节在前
#define O_TK_LEN      0X12 //1 BYTS TL_LEN	  //获得的设备临时通讯令牌长度
#define O_TOKE_START  0X13
#define O_DATAS_START 0X1B
   //N BYTS DATAS	  //客户的数据包串
   //CRC_L    1BYTE	  //CRC16 低字节
   //CRC_H    1BYTE	  //CRC16 高字节
   //PACK_END 1BYTE	  //包尾1字节固定0X55
unsigned char esp_dev_id[8]={0,0,0,0,0,0,0X1E,0XE6};
unsigned char esp_TOK_id[8]={0,0,0,0,0,0,0,0};
unsigned char esp_user_data[8]={0,0,0,0,0,0,0X1E,0XE6};
unsigned char need_seed_len=0;
void make_send_pack(unsigned char ms_opt,unsigned char *dev_id,unsigned char *toke_id,unsigned char *use_data,unsigned char use_data_len)
{
  unsigned char a,b,i=0;
  unsigned esp_crc=0;
  send_buf[0]=0x55;

  send_buf[O_LEN_L]=(O_DATAS_START+use_data_len+3)%0xff;
  send_buf[O_LEN_H]=(O_DATAS_START+use_data_len+3)/0xff;

  send_buf[O_CMD_T]=0XA0;// 0XA0 链路操作 0XAA 数据传输 0XAC 实时检测指令 0XF0 终端操作 
  send_buf[O_CMD_C]=0X00;// reg  option
  send_buf[O_CIX_L]=0XA8;// CMD INDEXL
  send_buf[O_CIX_H]=0X16;//	CMD INDEXL
  send_buf[O_EXMSH]=0X16;//	EXTERN MESSAGE1
  send_buf[O_EXMSL]=0XA8;//	EXTERN MESSAGE2
  send_buf[O_RESTA]=0X02;//	CMD_STA 00 OK 01 FAIL 02 SEND 03 NO SUP

  for(i=0;i<8;i++)
  send_buf[O_DEVID+i]=*(dev_id+(7-i));

  send_buf[O_TK_LEN] =8;
  for(i=0;i<8;i++)
  send_buf[O_TOKE_START+i]=*(toke_id+i);

  for(i=0;i<use_data_len;i++)
  send_buf[O_DATAS_START+i]=*(use_data+i); // 数据区，登录的时候放数据密码文本

  temp_buf[0]=0x55;
  for(a=1,b=1;a<(O_DATAS_START+use_data_len);a++)
  {
    if(send_buf[a]==0x55)
	{
	  temp_buf[b]=0x54;
	  b+=1;
	  temp_buf[b]=0x01;
	  b+=1;
	}
	else if(send_buf[a]==0x54)
	{
	  temp_buf[b]=0x54;
	  b+=1;
	  temp_buf[b]=0x02;
	  b+=1;
	}
	else
	{
	temp_buf[b]=send_buf[a];
	b+=1;
	}
 }
 esp_crc=GetRevCrc_16(temp_buf,O_DATAS_START+use_data_len);
 temp_buf[b]=(esp_crc>>8);
 b+=1;
 temp_buf[b]=(esp_crc&0x00ff);	 
 b+=1;
 temp_buf[b]=0x55;
 b+=1;
 need_seed_len=b;
}
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
	char i=0;;
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
		make_send_pack(0,esp_dev_id,esp_TOK_id,esp_user_data,8);
		make_AT_SEND(need_seed_len);                     //将要发送的总字节数变成10进制动态修改AT发送指令	   at_uart_send_str(at_send_len_ox);
        at_uart_send_str(at_send_len_ox);                //动态发送数据长度
		Delay2(2000);
		at_uart_send_buf(temp_buf,need_seed_len);
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
