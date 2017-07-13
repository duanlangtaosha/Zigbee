/*************	���س�������	**************/
#define MAIN_Fosc		22118400L	//������ʱ��
#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	BaudRate1		115200UL	//ѡ������
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 ��װֵ�� ��Ӧ300KHZ
#include	"STC15Fxxxx.H"
/*************	���ر�������	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//���ջ���
u8	TX1_Cnt;	//���ͼ���
u8	RX1_Cnt;	//���ռ���
bit	B_TX1_Busy;	//����æ��־
/*************	�˿����Ŷ���	**************/
unsigned char two_lab=0;
sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P3^7;//LED3
sbit DK1=P3^3;//�̵���
sbit BEEP=P3^4;//������
sbit K1=P1^3;//����1
sbit K2=P1^2;//����2
sbit K3=P1^4;//����3


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define O_PF    0X00  //��ͷ1�ֽڹ̶�0X55 
#define O_LEN_L 0X01  //�����������ֽ�
#define O_LEN_H 0X02  //�����������ֽ� ע��ת���� ��������ֻ����һ�����ݴ���
#define O_CMD_T 0X03  //��������
#define O_CMD_C 0X04  //��������
#define O_CIX_L 0X05  //���������б�ŵ��ֽ�
#define O_CIX_H 0X06  //���������б�Ÿ��ֽ�
#define O_EXMSH 0X07  //��չ��Ϣ���ֽ�
#define O_EXMSL 0X08  //��չ��Ϣ���ֽ�
#define O_RESTA 0X09  //���ݰ�״̬��Ϣ���ɹ� ʧ�� ���� δ֪
#define O_DEVID 0x0A  //8�ֽ��豸ʶ��
   //1 BYTS TL_LEN	  //��õ��豸��ʱͨѶ���Ƴ���
   //N BYTS TK 		  //TL_LEN��ͨѶ���ƴ�
   //N BYTS DATAS	  //�ͻ������ݰ���
   //CRC_L    1BYTE	  //CRC16 ���ֽ�
   //CRC_H    1BYTE	  //CRC16 ���ֽ�
   //PACK_END 1BYTE	  //��β1�ֽڹ̶�0X55

////////////////////////////////////////////////////////////////////////���϶����˰��еĸ������ľ���λ��//////////////////////////////////

char led1bl,led2bl,led3bl;

code unsigned char AT_RST[]="AT+RST"; 	                                             
code unsigned char AT_MODE[]="AT+CWMODE=3";
code unsigned char AT_CWJAP[]="AT+CWJAP=\"360we\",\"zty0012001\"";
code unsigned char CIPSTART[]="AT+CIPSTART=\"UDP\",\"cloud.ai-thinker.com\",5001";
//////////////////////////////////////////////////////////////////////���ϲ��ַ���ָ���õ�Ƭ�������ڲ�����////////////////////////////////
code unsigned char CIPSEND_LEN[]="AT+CIPSEND=38";//ATָ�������38�ֽڵ����ݵ�������//////////////////////
unsigned char at_send_len_ox[16];                //����һ��ָ��ֻ�ܷ���38�ľ�̬�����������ø�RAM�洢�ϱ�CODE��ATָ��"AT+CIPSEND=38"�����޸ĺ����38��Ϊ������
unsigned char send_buf[64];	                     //���ͻ�����
unsigned char recd_buf[64];						 //���ջ�����
data unsigned char temp_buf[64];				 //�ӹ�������
unsigned char toke[8]={0x24,0x41,0xD6,0x39,0x48,0x83,0xAC,0x00};//���豸�ڷ������ϻ�õ����ư�

/////////////////////////////////////////////////////////////////���沿�ֶ�����CRC16У�麯���õ��ı��///////////////////////////////////
code unsigned int crc_table[256]=
{               /* CRC��ʽ�� */
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
/////////////////////////////////////////////////////////////////���沿�ֶ�����CRC16У�麯���õ��ı��///////////////////////////////////
bit t_o=0;
void make_AT_SEND(unsigned char a_len)//�����ұ�������ָ�������a_len �ĳ�10����  "AT+CIPSEND=38"
{
  unsigned char aa=0;
  for(aa=0;aa<11;aa++)
  {
    at_send_len_ox[aa]=CIPSEND_LEN[aa];	 //����"AT+CIPSEND= ��RAM  �����ʮ���Ʋ���������Ĳ�������
  }
  t_o=0;                                 //ȥ��ǰ���0�����緢��38���ֽڣ�038��ǰ���0�Ϳ���ȥ���ˡ�
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
//���溯�����CRCУ���� ���ñ�׼CRC16 ��ʼCRC=0XFFFF  �������ʽ���� 8005 ��1021
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

#define O_PF    0X00  //��ͷ1�ֽڹ̶�0X55 
#define O_LEN_L 0X01  //�����������ֽ�
#define O_LEN_H 0X02  //�����������ֽ� ע��ת���� ��������ֻ����һ�����ݴ���
#define O_CMD_T 0X03  //��������
#define O_CMD_C 0X04  //��������
#define O_CIX_L 0X05  //���������б�ŵ��ֽ�
#define O_CIX_H 0X06  //���������б�Ÿ��ֽ�
#define O_EXMSH 0X07  //��չ��Ϣ���ֽ�
#define O_EXMSL 0X08  //��չ��Ϣ���ֽ�
#define O_RESTA 0X09  //���ݰ�״̬��Ϣ���ɹ� ʧ�� ���� δ֪
#define O_DEVID_START 0x0A  //8�ֽ��豸ʶ��  ���ֽ���ǰ
#define O_TK_LEN      0X12 //1 BYTS TL_LEN	  //��õ��豸��ʱͨѶ���Ƴ���
#define O_TOKE_START  0X13
#define O_DATAS_START 0X1B
   //N BYTS DATAS	  //�ͻ������ݰ���
   //CRC_L    1BYTE	  //CRC16 ���ֽ�
   //CRC_H    1BYTE	  //CRC16 ���ֽ�
   //PACK_END 1BYTE	  //��β1�ֽڹ̶�0X55

unsigned char esp_dev_id[8]={0,0,0,0,0,0,0X1E,0XE6};         //    8266��8���ֽڵ�����ID�ţ����ڷ��������ݿ��в鵽��Ψһ��ʾһ����������¼������Ҫһ������ID,����������һ���������룬��ô�򵥵�¼
unsigned char esp_TOK_id[8]={0,0,0,0,0,0,0,0};				 //    �������ָ��������������ư���������ط�Ҳ�����ˣ���ȫ������һ��������ɵ�
unsigned char esp_user_data[14]={0,0,0,0,0,0,0X1E,0XE6};	 //    �ͻ��ľ����ݸ����������Ժܴ���Ϊ���Ƭ�����ޣ�����һ������źţ����弸���ֽڹ��ˣ�ע���ڵ�¼��ʱ���������������룡
unsigned char temp_cd[]="TEMP:123;";						 //    һ�����ݰ���ǰ���ǰ���ʽ���壬�����ǿͻ������������ﶨ��һ������Ҫ���͵��¶�����
unsigned char need_seed_len=0;								 //    ȫ�ֱ����������ܹ���Ҫ�������ڵ�����


//���������������һ�����͵����ݸ�ʽ���뿴���ݸ�ʽ�ĵ�����ȫ�����ýṹ����ɣ�����������ݣ����ϵ�������������ݰ�
//���͵����ݰ���Ŀǰֻ�е�¼���ݰ������ϱ��¶����ݰ������������������ݰ����ϱ����ݰ����Գ䵱����������һ�����������ţ��Ƿ��͵�¼�������¶Ȱ�
//����������ڲ���������ID�����ư����Լ��ͻ������ݣ��Լ��ͻ����ݳ���

void make_send_pack(unsigned char ms_opt,unsigned char *dev_id,unsigned char *toke_id,unsigned char *use_data,unsigned char use_data_len)
{
  unsigned char a,b,i=0;
  unsigned esp_crc=0;
  send_buf[0]=0x55;			                             //��ͷ��0X55�̶�
  
  send_buf[O_LEN_L]=(O_DATAS_START+use_data_len+3)%0xff; //�����ݰ����������ݳ��ȣ���ͷ����β���ǵ���û�о���ת��ǰ�İ���
  send_buf[O_LEN_H]=(O_DATAS_START+use_data_len+3)/0xff;

  if(ms_opt==0)											 //������ڲ����ж��Ƿ��͵�¼��·���������Ƿ������ݰ���ƽ̨
  send_buf[O_CMD_T]=0XA0;// 0XA0 ��·���� 0XAA ���ݴ��� 0XAC ʵʱ���ָ�� 0XF0 �ն˲��� 
  else if (ms_opt==1)
  send_buf[O_CMD_T]=0XAA;// 0XA0 ��·���� 0XAA ���ݴ��� 0XAC ʵʱ���ָ�� 0XF0 �ն˲��� 

  if(ms_opt==0)					                        //0X00�����¼����
  send_buf[O_CMD_C]=0X00;// reg  option
  else if (ms_opt==1)
  send_buf[O_CMD_C]=0XEE;					            //0XEE���������Ǵ��豸����ƽ̨�ķ���

  send_buf[O_CIX_L]=0XF3;// CMD INDEXL	                //�������б�ţ���ʱ���ã�������Ϊ�Է�Ӧ������ݰ��ű�ʾ
  send_buf[O_CIX_H]=0XC0;//	CMD INDEXL					//�������б�ţ���ʱ���ã�������Ϊ�Է�Ӧ������ݰ��ű�ʾ
  send_buf[O_EXMSH]=0XC0;//	EXTERN MESSAGE1				//��չ����ʱ����
  send_buf[O_EXMSL]=0XF3;//	EXTERN MESSAGE2				//��չ����ʱ����

  send_buf[O_RESTA]=0X02;//	CMD_STA 00 OK 01 FAIL 02 SEND 03 NO SUP	 //�������ݰ���״̬���Ƿ��ͻ���Ӧ��ɹ�����ʧ��

  for(i=0;i<8;i++)
  send_buf[O_DEVID+i]=*(dev_id+(7-i)); // �����豸��ΨһID�ŵ����ݰ���


  send_buf[O_TK_LEN] =8;	           //��������������ư���8���ֽ�


  for(i=0;i<8;i++)
  send_buf[O_TOKE_START+i]=*(toke_id+i);//8���ֽ����ư�����ʼ���ư�Ϊ00 ���������������һ�����ư�������豸���豸ÿ��ͨѶҪЯ��������ư�


  for(i=0;i<use_data_len;i++)
  send_buf[O_DATAS_START+i]=*(use_data+i); // �ͻ�������������¼��ʱ������������ı�
  
  temp_buf[0]=0x55;						   //��β

  esp_crc=GetRevCrc_16(send_buf,O_DATAS_START+use_data_len);//�õ�ת��֮ǰ�������ݰ�CRC��������Բ���CRC���ݸ�ʽ�����CRC�����ת��֮ǰ����������

  for(a=1,b=1;a<(O_DATAS_START+use_data_len);a++)           //����ȥ��ͷ�����е������к�����0X55������ת���0X54��0X01����0X54 ���0X54��02������ת�����ݰ�
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
 }	 ///////////////////////////////////////////////////////////���ϵ����ת�����ݰ��г���ͷ��CRC֮ǰ��ȫ��������///////////////////////////////////////////////////////////////////////
 //55 28 00 AA EE F3 C0 C0 F3 02 E6 1E 00 00 00 00 00 00 08 24 41 D6 39 48 83 AC 00 54 02 45 4D 50 3A 32 35 35 3B 00 D1 52 55
 //55 28 00 AA EE F3 C0 C0 F3 02 E6 1E 00 00 00 00 00 00 08 24 41 D6 39 48 83 AC 00 54 02 45 4D 50 3A 32 35 35 3B 00 D1 52 55

 temp_buf[b]=(esp_crc>>8);
 b+=1;
 temp_buf[b]=(esp_crc&0x00ff);	 
 b+=1;
    ///////////////////////////////////////////////////////////�����������CRCУ��////////////////////////////////////////
 temp_buf[b]=0x55;	//��β
 b+=1;
 temp_buf[b]=0x0d;
 b+=1;
 temp_buf[b]=0x0a;
 b+=1;				//���Ϲ��ɻس�
 need_seed_len=b;	//���˹��������Ҫ���͵�ȫ������ ����ATָ����Ҫ�Ļ���
}
bit have_data=0;
unsigned char rec_len=0;


void Delay1(unsigned long ms)//���ӳٺ�������λ�Ǻ���
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

void at_uart_send_str(unsigned char *str)//����AT�ַ���������
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
void at_uart_send_buf(unsigned char *str,unsigned char len)//�������ݻ������ķ��ַ�����Ϣ����������Ϣ������
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
unsigned int time=0; //ÿ��30���һ��������1��Ȼ������������Ϊ�¶������ϱ�����ƽ̨��ת���ֻ���
void main(void)
{
	char tt,i=0;;
	//////////////////////////////////////////////////////////////////////////////////////���沿��Ϊ��ʱ���Լ����ڳ�ʼ��/////////////////////
	B_TX1_Busy = 0;
	RX1_Cnt = 0;
	TX1_Cnt = 0;
	S1_8bit();				//8λ����
	S1_USE_P30P31();		//UART1 ʹ��P30 P31��	Ĭ��
	AUXR &= ~(1<<4);	//Timer stop		������ʹ��Timer2����
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (u8)(Timer2_Reload >> 8);
	TL2 = (u8)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//�������
	ES  = 1;	//�����ж�
	EA = 1;		//����ȫ���ж�
	P3M1 = 0x00;
    P3M0 = 0xFF;
	RX1_Cnt=0;
	DK1=0;
	BEEP=0;
	///////////////////////////////////////////////////////////////////////////////////���ϲ�����Ҫ��ɴ��ڵĳ�ʼ��////////////////////////////
	for(;;)
	{	//////////////////////////////////////////////////////////�������䷢��ATָ����������������ӵ����ſ�������������/////////////////////
		Delay2(2000);
        at_uart_send_str(AT_MODE);
		Delay2(2000);
        at_uart_send_str(AT_CWJAP);
		Delay2(10000);
        at_uart_send_str(CIPSTART);
		Delay2(2000);
		//////////////////////////////////////////////////////////���������䷢��ATָ����������������ӵ����ſ�������������/////////////////////
		make_send_pack(0,esp_dev_id,esp_TOK_id,esp_user_data,8);//�����¼���ݰ�
		make_AT_SEND(need_seed_len);                            //��Ҫ����WIFI���͵����ֽ������10���ƣ���̬���ɷ�������ATָ��
        at_uart_send_str(at_send_len_ox);                       //������õ�AT�������ݵ��������Ķ�̬�������ݳ���
		Delay2(2000);
		at_uart_send_buf(temp_buf,need_seed_len);				//����WIFIģ�鷢�͹���õĵ�¼��
		while(1)
		{
			for(tt=0;tt<60;tt++)
			{
			Delay2(200);
			Delay2(200);
			}
		    make_send_pack(1,esp_dev_id,toke,temp_cd,10);    //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
		    make_AT_SEND(need_seed_len); 					 //��̬���췢��ATָ��
			at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
		    Delay2(2000);
		    at_uart_send_buf(temp_buf,need_seed_len);	     //����WIFI��������

			LED1=0;
			Delay2(200);
		    LED1=1;
			Delay2(200);
															//ÿ30������е�����һ�Ρ�����һ���¶���ֵ
			time++;
			temp_cd[5]=(((time%1000)/100)+'0');
			temp_cd[6]=(((time%100)/10)+'0');
		    temp_cd[7]=(((time%10))+'0');
		}
	}
	Delay2(2000);
}

/********************* UART1�жϺ���************************/
bit a_vec=0;
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		if(SBUF==0x55)	 //////////////////////////////////////����������ݹ̶������ݰ���ʽ����ȡ����Ҫ������/////////////////////////////////
		a_vec=1;
		if(a_vec)
		{
		 recd_buf[RX1_Cnt] = SBUF;		//����һ���ֽ�
		 if(RX1_Cnt<62)
		   RX1_Cnt++;
		 else
		   RX1_Cnt=0;
		 if(recd_buf[RX1_Cnt]==0X55)
		   two_lab++;
		 if(two_lab==2)//////////////////////////////////////////ȡ��2��OX55֮�����Ч����////////////////////
		 {
		   have_data=1;
		   two_lab=0;
		   rec_len=RX1_Cnt;
		   RX1_Cnt=0;
		   a_vec=0;
		    if((recd_buf[33])==':')	      /////////////////////////////////////�򵥵õ��ֻ��Ŀ�ָ��////////////////////
			{

			if((recd_buf[34])=='0')
			{
			  LED1=0;	  //����ָ��
			  toke[7]=recd_buf[19];	     //////////////���Ƶ���ģ�������ư����ǵ�¼��������ʱ��ֻ����һ�Σ� ���ʱ�򴮿ڴ���δ֪״̬����˾����ѵ�һ��������
			  toke[6]=recd_buf[20];		 //////////////���ڽ��յ��ĵ�һ�����ݰ����Ƿ������ظ������ư�������Ƭ����һǡ�ö���������һ����¼�Ϸ�������ֻҪ�ֻ���
			  toke[5]=recd_buf[21];		 //////////////һ�����ƿ���ָ�ģ�飬�����ָ��Ĺ̶�λ�ã�����19λ�ô����ŵľ��ǿ��ܶ������ư���
			  toke[4]=recd_buf[22];		 //////////////������������ư����Ժ�ģ�鷢�͵���ƽ̨�����ݣ���Ҫ�õ�������ư�
			  toke[3]=recd_buf[23];
			  toke[2]=recd_buf[24];
			  toke[1]=recd_buf[25];
			  toke[0]=recd_buf[26];
			}
			else if((recd_buf[34])=='1')
		      LED1=1;  //�ص�ָ��
			}
		 }
		 }
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}
