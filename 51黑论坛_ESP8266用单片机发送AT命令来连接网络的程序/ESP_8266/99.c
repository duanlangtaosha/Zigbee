/*
  ע�⣺������ʼ����0���������  ����ע���������  0X55 �������������п��ܳ��֣�����ɻ��ң���Ҫ�Ǽ���  �������ݣ��������绷����ͬ�������п��ܱ仯��ע��ͨ����
  �ڲ� ID KEY �����ʽ�Ѿ��̶�Ϊ8���ֽ�ģʽ
*/


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
/////���ǰ��ſ���ƽ̨���ݰ���ȫ����ɽṹ��ϸ�ڿɲ��հ��ſɵ���ƽ̨V1.0�汾����飬���ṹ�½�
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
#define O_DEVID 0x0A  //8�ֽ��豸ʶ��
#define O_TK_LEN      0X12 //1 BYTS TL_LEN	  //��õ��豸��ʱͨѶ���Ƴ���
#define O_TOKE_START  0X13
#define O_DATAS_START 0X1B
//N BYTS DATAS	  //�ͻ������ݰ���
//CRC_L    1BYTE	  //CRC16 ���ֽ�
//CRC_H    1BYTE	  //CRC16 ���ֽ�
//PACK_END 1BYTE	  //��β1�ֽڹ̶�0X55

////////////////////////////////////////////////////////////////////////���϶����˰��еĸ������ľ���λ��//////////////////////////////////
#define uart_rec_tcp_udp_data 0	//ϵͳ�����������������շ�״̬
#define uart_rec_csysid       1	//ϵͳ��ȡESP8266WIFIģ�����ɺ�״̬����ȡ�ĸ��ֽڵ�оƬID �� 4���ֽڵ���Ȩ��
#define uart_rec_smartlink    2	//ϵͳ�����ȡ�����˺�����״̬
#define uart_rec_bander       3	//ϵͳ���뽫��WIFI�豸 ��APP �ֻ��󶨿���״̬
//////////////////////////////////////////////////////////////////////���ϲ��ֶ�����ϵͳ�ļ���״̬///////////////////////////////////////
bit have_tok=0;
bit have_data=0;
unsigned char rec_len=0;
bit                   a_vec=0;		   //����һ����־λ�����ڳ��� ����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
unsigned char          ceng=0;         //��� �����ֽ��� �����ָ���ִ�,��̬������
unsigned char str_len_limt=16;         //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
unsigned char   str_len_num=11;        //�ַ�����
char str_ref=':';
/////////////////////////////////////////////////////////////////////���漸������������Ϊ�ڴ����й���ָ���ַ����� �޶��������ı�������ݺ�ض��ȳ�ʼ���⼸��/////
code char CYSYS_code[]="+CSYSID:CHIP";
code char PIPD_code[]="+IPD,";
code char bander_code[]="+IPD,4,26:RPL:";//���������©��
code char smartlink_code[]="SMART SUCCESS";
/////////////////////////////////////////////////////////////////////�����Ǵ�WIFIģ�鴮�����������Ƭ����������󣬱���Ƭ��Ҫ���˵ĸ���ͷ��////////////////////
unsigned char uart_rec_sta=uart_rec_csysid;// ����Ŀǰ�����ڵ�״̬����������������ݽ��գ�����SMARTLINK ���� ��ȡģ���ID��
code unsigned char AT_RST[]="AT+RST"; 	                                             
code unsigned char AT_MODE[]="AT+CWMODE=1";
code unsigned char AT_CWJAP[]="AT+CWJAP=\"360we\",\"zty0012001\"";
code unsigned char AT_CIP3[]="AT+CIPSTART=3,\"UDP\",\"cloud.ai-thinker.com\",5001,2468,0" ;
code unsigned char temp_bander[]="RPT:\"0xa1b23467\",\"0xaf321234\",\"192.168.0.123\",\"light\",\"123456a\"";
code unsigned char AT_CIPMUX[]="AT+CIPMUX=1";
code unsigned char CIPSTART[]="AT+CIPSTART=4,\"UDP\",\"cloud.ai-thinker.com\",5001,2468,0";
code unsigned char AT_CSYSID[]="AT+CSYSID";
code unsigned char AT_SMARTLINK[]="AT+CWSMARTSTART=1";
code unsigned char ZERO[]="00000000000000000000000000000000001";
code unsigned char CIPSEND_LEN[]="AT+CIPSEND=4,";
code unsigned char CIP3SEND_LEN[]="AT+CIPSEND=3,63";
/////////////////////////////////////////////////////////////////////�����ִ��ǵ�Ƭ����������ģ���AT����е���ֱ�Ӹ�WIFIģ���ָ���Щ�����������ַ�����/////////////

xdata unsigned char at_send_len_ox[20];                
idata unsigned char send_buf[64]="jisifsfhello:99999;oop";	                     //���ͻ�����
unsigned char recd_buf[64]= {0x55 ,0x25 ,0x00 ,0xAA ,0xFF ,0x00, 0x00, 0x00, 0x00 ,0x02 ,0x31, 0x0A ,0xFE ,0x00, 0x00, 0x00, 0x00 ,0x00, 0x08, 0x00 ,0x7C ,0xC8, 0x52 ,0xFE, 0x3D ,0x64 ,0x29, 0x4C ,0x49, 0x47, 0x48 ,0x54 ,0x02, 0x3A ,0x3F, 0x6C, 0x02, 0x55};						 //���ջ�����
data unsigned char temp_buf[72];				 //�ӹ�������
idata unsigned char toke[8]={0x24,0x41,0xD6,0x39,0x48,0x83,0xAC,0x00};//���豸�ڷ������ϻ�õ����ư�
unsigned char esp_dev_id[8]={0,0,0,0,0,0,0X1E,0XDE};         //    8266��8���ֽڵ�����ID�ţ����ڷ��������ݿ��в鵽��Ψһ��ʾһ����������¼������Ҫһ������ID,����������һ���������룬��ô�򵥵�¼
unsigned char esp_TOK_id[8]={0,0,0,0,0,0,0,0};				 //    �������ָ��������������ư���������ط�Ҳ�����ˣ���ȫ������һ��������ɵ�
unsigned char esp_user_data[14]={0,0,0,0,0,0,0X1E,0XDE};	 //    �ͻ��ľ����ݸ����������Ժܴ���Ϊ���Ƭ�����ޣ�����һ������źţ����弸���ֽڹ��ˣ�ע���ڵ�¼��ʱ���������������룡
unsigned char temp_cd[]="TEMP:123;";						 //    һ�����ݰ���ǰ���ǰ���ʽ���壬�����ǿͻ������������ﶨ��һ������Ҫ���͵��¶�����
unsigned char need_seed_len=0;								 //    ȫ�ֱ����������ܹ���Ҫ�������ڵ�����
bit t_o=0;													 //  �ڹ���һ����00123 ������ʱ��ȥ��ǰ���00���123 ����������0����1
code unsigned char cip3_lcport[]="2469,0";
data unsigned char chip_id[8]={'0','0','0','0','0','0','0','0'};
data unsigned char flash_id[8]={'0','0','0','0','0','0','0','0'};
pdata unsigned char pass_id[8]={'0','0','0','0','0','0','0','0'};

unsigned int time=0; //ÿ��30���һ��������1��Ȼ������������Ϊ�¶������ϱ�����ƽ̨��ת���ֻ���

xdata unsigned char ssid[32];	  //�ݴ�SSID�˻���Ϣ
xdata unsigned char password[20]; //�ݴ�ͻ�����
idata char ssid_len=0;	          //��¼SSID ����
idata char pasd_len=0;			  //��¼���볤��

bit have_id=0;					  //��¼�Ƿ���ATָ���ȡ�������ģ���ID ��KEY��Ϣ
bit have_smartlink=0;			  //��¼�Ƿ���˵� �Ӵ�������TCP UDP ����  smartlink 
bit have_bander=0;				  //��¼�ǴӴ�������������������й��˵� �ֻ���UDP������������Ϣ

unsigned char stac_a=0;		      //ȫ��ר�ñ���
unsigned char stac_b=0;			  //ȫ��ר�ñ���

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


void make_AT_CIP3(void)
//�����ϱ� ��2468 �˿ڼ������� �㲥���� �ù㲥���ݵ�IP ��ַ192.168.1.10�Ͷ˿� 48008�½�һ��3���ӣ�
//AT+CIPSTART=3,"UDP","192.168.1.10",48008,2469,0
///�½�һ������3 
{
  unsigned char a,b;
  for(a=0,b=0;a<21;a++,b++)
  temp_buf[b]=AT_CIP3[b];
  for(a=0;recd_buf[a]!=',';a++,b++)
  temp_buf[b]=recd_buf[a];
  temp_buf[b]=',';
  b++;
  a+=2;
  for(;recd_buf[a]!='"';a++,b++)
  temp_buf[b]=recd_buf[a];
  temp_buf[b]=',';
  b++;
  for(a=0;cip3_lcport[a]!=0;a++,b++)
  temp_buf[b]=cip3_lcport[a];
  temp_buf[b]=0;
}
//��16�����������16�����ַ�����10���A
char back_char(unsigned char user_d)
{
  if(user_d<10)
  return (user_d+'0');
  else 
  return (user_d-10+'A');
}
////���� ���ֻ��������¸�ʽUDP����
////RPT:"0x00FE6738","0xB8B3C281","192.168.0.123","light","123456a"
void make_bander_data()
{
  unsigned char a,b=0;
  for(a=0;temp_bander[a]!=0;a++)
  temp_buf[a]=temp_bander[a];
  temp_buf[a]=0;

  //����Ӳ���� code unsigned char temp_bander[]="RPT:\"0xa1b23467\",\"0xaf321234\",\"192.168.0.123\",\"light\",\"123456a\"";�� temp_buf
  //��������޸�RPT:"0x00FE6738","0xB8B3C281","192.168.0.123","light","123456a" �н�������豸ID����0x00FE6738�� �� KEY��0xB8B3C281���ŵ������޸ĺ��temp_buf
  temp_buf[7]=back_char(esp_dev_id[4]>>4);
  temp_buf[8]=back_char(esp_dev_id[4]&0x0f);
  temp_buf[9]=back_char(esp_dev_id[5]>>4);
  temp_buf[10]=back_char(esp_dev_id[5]&0x0f);
  temp_buf[11]=back_char(esp_dev_id[6]>>4);
  temp_buf[12]=back_char(esp_dev_id[6]&0x0f);
  temp_buf[13]=back_char(esp_dev_id[7]>>4);
  temp_buf[14]=back_char(esp_dev_id[7]&0x0f);

  temp_buf[20]=back_char(esp_user_data[7]>>4);
  temp_buf[21]=back_char(esp_user_data[7]&0x0f);
  temp_buf[22]=back_char(esp_user_data[6]>>4);
  temp_buf[23]=back_char(esp_user_data[6]&0x0f);
  temp_buf[24]=back_char(esp_user_data[5]>>4);
  temp_buf[25]=back_char(esp_user_data[5]&0x0f);
  temp_buf[26]=back_char(esp_user_data[4]>>4);
  temp_buf[27]=back_char(esp_user_data[4]&0x0f);
}

void make_AT_SEND(unsigned char a_len)   //�����ұ�������ָ�������a_len �ĳ�10���ƣ��ұ�����ָ��  "AT+CIPSEND=XX" XX�Ƿ��͵�����
{
  unsigned char aa=0;
  for(aa=0;aa<13;aa++)
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
 temp_buf[b]=0x00;
}



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
	//SBUF='\r';
	Delay1(1);
	//SBUF='\n';
	Delay1(1);
}


void change_pack()					  //�ѽ��յ������ݰ�ת�������0X55 ת���0X54 0X01 0X54 �滻��0X54 02
{
  				   for(stac_a=1,stac_b=1;recd_buf[stac_a]!=0x55;)
				   {
				     if((recd_buf[stac_a]==0x54)&&(recd_buf[stac_a+1]==0x01))
					 {
					 temp_buf[stac_b]=0x55;
					 stac_b++;
					 stac_a+=2;
					 }
					 else if((recd_buf[stac_a]==0x54)&&(recd_buf[stac_a+1]==0x02))
					 {
					 temp_buf[stac_b]=0x54;
					 stac_b++;
					 stac_a+=2;
					 }
					 else
					 {
					   temp_buf[stac_b]=recd_buf[stac_a];
					   stac_b++;
					   stac_a++;
					 }
				  }
				  temp_buf[stac_b]=0x55;
				  temp_buf[0]=0x55;
				  recd_buf[0]=temp_buf[0];
				  for(stac_a=1;temp_buf[stac_a]!=0x55;stac_a++)
				  recd_buf[stac_a]=temp_buf[stac_a];
				  recd_buf[stac_a]=0x55;
				  recd_buf[0]=0x55;
}
void init_uart(void)
{
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
}

void main(void)
{
	char tt,i,k,n,z=0;
	//////////////////////////////////////////////////////////////////////////////////////���沿��Ϊ��ʱ���Լ����ڳ�ʼ��/////////////////////
	init_uart();
	Delay2(2000);
	///////////////////////////////////////////////////////////////////////////////////���ϲ�����Ҫ��ɴ��ڵĳ�ʼ��////////////////////////////
	for(;;)
	{	 
        a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
        str_len_limt=22;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=13;           //Ҫ���������ַ�����
        str_ref=':';			  // ���˵��ַ����󣬽���Ҫ���ֵ��ַ�������ַ����ֵ�λ�ò��Ǿ���0λ��
		uart_rec_sta=uart_rec_smartlink; //�����ڴ����еĹ��˷�֧�����������ж��У����������־���ò�ͬ���ַ������˲���

		at_uart_send_str(AT_MODE);		 //����ģ�����STATION ģʽ
		Delay2(1000);
		at_uart_send_str(AT_SMARTLINK);	 //���ͽ���SMARTLINK ATָ��
                                         //PASSWORD:zty0012001	
		Delay2(2000);
		do								 //��ѭ�����SMARTLINK ������
		{
		LED1=0;
		LED2=0;
		LED3=0;
		Delay2(200);
		LED1=1;
		LED2=1;
		LED3=1;
		Delay2(200);
		if(have_smartlink)//����Ӵ����л����ȷ�� SMARTLINK �û�·�����˻�������
		{
			for(i=0;recd_buf[i]!=0x0d;i++)
			{
				ssid_len++;
				ssid[i]=recd_buf[i];			          //�������ڻ������е��û�·��������
			}
			for(i=0;recd_buf[ssid_len+11+i]!='\r';i++)
			{
				password[i]=recd_buf[ssid_len+11+i];	  //�������ڻ������е��û�·��������
				pasd_len++;
			}
		//code unsigned char AT_CWJAP[]="AT+CWJAP=\"360we\",\"zty0012001\"";
			n=0;
			for(i=0;i<63;i++)
		    temp_buf[i]=0;	        //�����ʱ������

			for(i=0;i<10;i++,n++)
			temp_buf[n]=AT_CWJAP[i];      //����ATָ�����·�����ĸ�ʽͷAT+CWJAP="

			for(i=0;i<ssid_len;i++,n++)	  //��SMARTLINK ��õ� SSID ��PASSWORLD ��� ����·��������Ҫ����������	AT+CWJAP="360we","zty0012001"
			temp_buf[n]=ssid[i];
			temp_buf[n]='"';
			n++;
			temp_buf[n]=',';
			n++;
			temp_buf[n]='"';
			n++;
			for(i=0;i<pasd_len;i++,n++)
			temp_buf[n]=password[i];
			temp_buf[n]='"';
		}
	    }while(have_smartlink==0); //��ѭ�����SMARTLINK ������

		LED1=1;
		LED2=1;
		LED3=1;
	    //////////////////////////////////////////////////////////����������ģ����豸ID/////////////////////////////////////////////////////////////
        a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
        str_len_limt=16;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=12;           //Ҫ���˵��ַ����� +CSYSID:CHIP: Ϊ��һ���ַ�����λ��: �����12���ַ�
        str_ref=':';			  // Ҫ���˵����ַ�
		uart_rec_sta=uart_rec_csysid;
		at_uart_send_str(AT_CSYSID); //�������£�+CSYSID:CHIP:00FE6738;FLASH:001640EF;KEY:81C2B3B8;
		Delay2(2000);				 //��ʱ����󣬴����бض���� ����Ҫ������ID FLASH ID ��KEY 
		if(have_id)
		{
		have_id=0;
		k=0;
		for(i=0,tt=0;i<8;i++,tt++)	//  ��ȡģ���ڲ���оƬID 			 
		chip_id[tt]=recd_buf[i];
		for(i=15,tt=0;i<23;i++,tt++)//  ��ȡģ���ڲ���FLASH ID 
		flash_id[tt]=recd_buf[i];
		for(i=28,tt=0;i<36;i++,tt++)//  ��ȡģ���ڲ���KEY
		pass_id[tt]=recd_buf[i];
        //����ȡ��16�����ַ����� 16�������ݣ�8���ֽڵ��ַ����ܵõ��ĸ��ֽڵ�16�������� �����ַ���"A2345678"���ձ��0XA2��0X34��0X56��0X78 �ĸ��ֽڴ��
	    for(i=0;i<8;i++)            
	    {
			  if((chip_id[i]>='A')&&(chip_id[i]<='F'))
			  chip_id[i]=(chip_id[i]-'A'+10);
			  else if((chip_id[i]>='a')&&(chip_id[i]<='f'))
              chip_id[i]=(chip_id[i]-'a'+10);
			  else
			  chip_id[i]-='0';
	    }
				
		for(i=0;i<8;i++)
	    esp_dev_id[i]=0;

		esp_dev_id[7]=((chip_id[6])<<4)+chip_id[7];// ���ַ���16����ת���󣬱��16���������ĸ��ֽڴ�ŵ���Ӧ�ڴ���
		esp_dev_id[6]=((chip_id[4])<<4)+chip_id[5];
		esp_dev_id[5]=((chip_id[2])<<4)+chip_id[3];
		esp_dev_id[4]=((chip_id[0])<<4)+chip_id[1];
		//����ȡ��16�����ַ����� 16�������ݣ�8���ֽڵ��ַ����ܵõ��ĸ��ֽڵ�16�������� �����ַ���"A2345678"���ձ��0XA2��0X34��0X56��0X78 �ĸ��ֽڴ��
	    for(i=0;i<8;i++)
	    {
			  if((pass_id[i]>='A')&&(pass_id[i]<='F'))
			  pass_id[i]=(pass_id[i]-'A'+10);
			  else if((pass_id[i]>='a')&&(pass_id[i]<='f'))
              pass_id[i]=(pass_id[i]-'a'+10);
			  else
			  pass_id[i]-='0';
	    }
		for(i=0;i<8;i++)
		flash_id[i]=0;

		flash_id[7]=((pass_id[6])<<4)+pass_id[7];// ���ַ���16����ת���󣬱��16���������ĸ��ֽڴ�ŵ���Ӧ�ڴ���
		flash_id[6]=((pass_id[4])<<4)+pass_id[5];
		flash_id[5]=((pass_id[2])<<4)+pass_id[3];
		flash_id[4]=((pass_id[0])<<4)+pass_id[1];

		for(i=0;i<8;i++)
	    esp_user_data[i]=flash_id[i];

		// ���ϲ�����ģ��������ڲ��ַ�����ʽ��оƬID ��KEY ������4���ֽڵģ�����16���� ��ŵ��ڴ��У��ڵ�¼��������ʱ���������Ҫ������
		//////////////////////////////////////////////////////////����������ģ����豸ID��KEY/////////////////////////////////////////////////////////////
		}
		//while(1);
		//////////////////////////////////////////////////////////�������䷢��ATָ����������������ӵ����ſ�������������/////////////////////

		a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
        str_len_limt=16;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=7;            //�ַ�����
        str_ref='"';
		uart_rec_sta=uart_rec_bander;

		Delay2(2000);
        at_uart_send_str(AT_MODE);
		Delay2(2000);
        at_uart_send_str(temp_buf);
		Delay2(10000);
		at_uart_send_str(AT_CIPMUX);
		Delay2(2000);
        at_uart_send_str(CIPSTART);	      //��UDP��ʽ���ӵ����ſɵ��ƣ����ӵ�Ŀ��5001���ſ��Ʒ���˳����� 2468 �˿ڣ�
		Delay2(2000);
		
		do
		{
		LED1=0;
		LED2=0;
		LED3=0;
		Delay2(200);
		LED1=1;
		LED2=1;
		LED3=1;
		Delay2(200);
		
		if(have_bander)
		{
		  ;
		}

		 }while(have_bander==0);        //�������������ֻ���2468�˿ڣ�2468�˿ں�����4�󶨣� ������UDPɨ����Ϣ�����RPL:"192.168.1.10","48008"  �ֻ�͸¶�Լ���IP�Ͷ˿�
		 
		 make_AT_CIP3();			   	//�����ϱ� ��2468 �˿ڼ������� �㲥���� �½�һ��3���ӣ�AT+CIPSTART=3,"UDP","192.168.1.10",48008,2469,0
		 at_uart_send_str(temp_buf);
		 Delay2(2000);
		 make_bander_data();		    //�Ӱ��ֻ����豸��ϵ�Ĵ����У��ڴ�������������õ�
		 at_uart_send_str(CIP3SEND_LEN);
		 Delay2(1000);
		 at_uart_send_str(temp_buf);
		 Delay2(2000);
		//////////////////////////////////////////////////////////���������䷢��ATָ����������������ӵ����ſ�������������/////////////////////
		at_uart_send_str(AT_MODE);
		a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
        str_len_limt=12;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=5;            //�ַ�����
        str_ref=':';
		uart_rec_sta=uart_rec_tcp_udp_data;

		make_send_pack(0,esp_dev_id,esp_TOK_id,esp_user_data,8);//�����¼���ݰ�
		make_AT_SEND(need_seed_len);                            //��Ҫ����WIFI���͵����ֽ������10���ƣ���̬���ɷ�������ATָ��
		two_lab=0;												////////////////////////////////////////////////////////////////////����55���������п��ܴ���
        at_uart_send_str(at_send_len_ox);                       //������õ�AT�������ݵ��������Ķ�̬�������ݳ���
		Delay2(2000);

	
		/*
		 for(z=0;z<8;z++)
		{
		SBUF='0';
		Delay2(1);
		}

		for(z=0;z<1;z++)
		{
		SBUF=need_seed_len;
		Delay2(1);
		}

		for(z=0;z<8;z++)
		{
		SBUF='0';
		Delay2(1);
		}
		while(1);
		*/
		at_uart_send_buf(temp_buf,need_seed_len);				//����WIFIģ�鷢�͹���õĵ�¼��
		Delay2(4000);

		//while(1);

		while(1)
		{
			for(tt=0;tt<60;tt++)
			{
			Delay2(200);
			if(0)
			{
			have_data=0;
			for(z=0;z<8;z++)
			{
			SBUF='0';
			Delay2(1);
			}
	
			for(z=0;z<40;z++)
			{
			SBUF=recd_buf[z];
			Delay2(1);
			}
	
			for(z=0;z<8;z++)
			{
			SBUF='0';
			Delay2(1);
			}
		    }
			Delay2(200);
		   }
		    make_send_pack(1,esp_dev_id,toke,temp_cd,10);    //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
		    make_AT_SEND(need_seed_len); 					 //��̬���췢��ATָ��
			at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
		    Delay2(2000);
		    at_uart_send_buf(temp_buf,need_seed_len);	     //����WIFI��������

			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;      //��������ָʾ�ƣ���������
															//ÿ30������е�����һ�Ρ�����һ���¶���ֵ
			time++;
			temp_cd[5]=(((time%1000)/100)+'0');
			temp_cd[6]=(((time%100)/10)+'0');
		    temp_cd[7]=(((time%10))+'0');
		}
	}
	Delay2(2000);
}

/********************* �ַ������˺�����ʼ����************************/
/*
bit a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
unsigned char ceng=0;         //��� �����ֽ��� �����ָ���ִ�,��̬������
unsigned char str_len_limt=12;//����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
unsigned char str_len_num=5;  //�ַ�����
char str_ref=':';
code char test_code[]="+IPD,";
*/

void fit_stb(unsigned char *str_p) // ÿ�δ��ڽ��յ�һ���ַ�������뱾���˺��������Ƿ���˵�ָ�����ַ�����
{
   if((a_vec==0)&&(ceng<str_len_limt))
   {
      if(ceng<str_len_num)
	  {
      if(SBUF==(*(str_p+ceng)))
	  ceng++; 
	  else 
	  ceng=0;
	  }
	  else
	  {
		ceng++;
		if(SBUF==str_ref)
		{
		  a_vec=1;
		  ceng=0;	
		}
	  }
	  RX1_Cnt=0;
   }
   else if(ceng>=str_len_limt)
   {
     	  a_vec=0;
		  ceng=0;
   }
}

//unsigned char 
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		if(a_vec==0)//�ϴ�δ�����˵�ָ�����ַ��������е��ַ����֣�ÿ��ֻ����һ���ַ�
		{
		 switch(uart_rec_sta)	      //����ϵͳ��״̬�����������ĸ��ַ�
		 {
		  case uart_rec_tcp_udp_data: //���ڽ���������UDP TCP �����շ�
		  fit_stb(PIPD_code);		  //�������ݽ���ͷ
		  break;
		  case  uart_rec_csysid:	  //���ڽ�����ģ���ڲ�ID ��KEY ״̬
		  fit_stb(CYSYS_code);	      //����ָ����ͷ��
		  break;
		  case uart_rec_smartlink:	  //���ڽ����ÿͻ���·�����˻�����״̬
		  fit_stb(smartlink_code);
		  break;
		  case uart_rec_bander:
		  fit_stb(bander_code);
		 }
		}
		else//////////////////////////////���˵�ָ����ͷ��
		{
		
		 recd_buf[RX1_Cnt] = SBUF;		//����һ���ֽ�
		 if(recd_buf[RX1_Cnt]==0X55)    //��¼0X55�ĸ������������κ󣬴���һ�������ݰ�����
		 two_lab++;
		 if(RX1_Cnt<62)	/////////////////////��ֹ64�ֽڵĻ��������
		 RX1_Cnt++;
		 else			///////ÿ���յ���ָ���64�ֽڣ��Ͱ�������գ�����ָ��ָ��ͷ
		 {
		   RX1_Cnt=0;
		   a_vec=0;
		   two_lab=0;
		 }
		 switch(uart_rec_sta)
		  {
		    case uart_rec_tcp_udp_data:
			//have_dd=1;
			   //LED2=0;
				if(two_lab==2)//////////////////////////////////////////�õ�һ����Ч�����ݣ�ȡ��2��OX55֮�����Ч����////////////////////
				 {
				   a_vec=0;
				   rec_len=RX1_Cnt;
				   two_lab=0;
				   RX1_Cnt=0;
                   LED1=0;
				   LED2=0;
				   LED3=0;

				   change_pack();
				   have_data=1;
				    if((recd_buf[32])==':')	      /////////////////////////////////////�򵥵õ��ֻ��Ŀ���ָ��////////////////////
					{
					 if((recd_buf[33])=='0')
					 {
					  DK1=0;
					  LED1=1;	  //�ص�ָ��
					 }
					 else if((recd_buf[33])=='1')
					 {
					  DK1=1;
				      LED1=0;    //����ָ��
					 }
					}
					if((recd_buf[3]==0xa0)&&(recd_buf[9]==0x00)&&(have_tok==0))////////////////////���TOKE��//////////////////////////////////
					{
					  have_tok=1;
					  toke[7]=recd_buf[19];	     
					  toke[6]=recd_buf[20];		
					  toke[5]=recd_buf[21];	
					  toke[4]=recd_buf[22];		 
					  toke[3]=recd_buf[23];
					  toke[2]=recd_buf[24];
					  toke[1]=recd_buf[25];
					  toke[0]=recd_buf[26];

					}
				 }
			break;
		   case  uart_rec_csysid:
		   if(RX1_Cnt>40)
		   have_id=1;
		   break;
		   case  uart_rec_smartlink:
		   if(RX1_Cnt>10)
		   have_smartlink=1;
		   break;
		   case  uart_rec_bander:
		   if(RX1_Cnt>20)
		   have_bander=1;
		   break;
		 };
	  }
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}
