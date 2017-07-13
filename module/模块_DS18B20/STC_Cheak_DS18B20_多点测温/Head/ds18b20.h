#ifndef __ds18b20_h__
#define __ds18b20_h__

#define PIN_DS18B20 P14



//extern void DS18B20_Mode(void);
/******��ͨ�Ķ�ȡһ��18B20���¶�**********/
extern float rd_temperature_f(void);

/******��ȡ����ʾһ��İ����DS18B20��ROM********
 ���ͬʱ������������ִ�������������֪���Ƕ�
 ����һ�����Ҳ��Գ����������ǣ���ʾ��ROM��������
 һ��DS18B20��ROM������˵ֻ��һ��һ���Ķ�ȡ

*/
extern void Display_DS18B20_ROM(void)	;

/******��ȡһ��DS18B20���¶�(�ڶ��DS18B20���ص�������)
 num_ds18b20����ʾ��һ��DS18B20���¶ȣ�Ŀǰֻ֧��1��2����DS18B20
*/
extern float Read_DS18B20_Num(unsigned int num_ds18b20);
#endif										