#ifndef __ds18b20_h__
#define __ds18b20_h__

#define PIN_DS18B20 P14



//extern void DS18B20_Mode(void);
/******普通的读取一个18B20的温度**********/
extern float rd_temperature_f(void);

/******读取并显示一个陌生的DS18B20的ROM********
 多个同时挂载在总线上执行这个函数，则不知道是读
 的那一个，我测试出来的数据是，显示的ROM不是任意
 一个DS18B20的ROM，所以说只能一个一个的读取

*/
extern void Display_DS18B20_ROM(void)	;

/******读取一个DS18B20的温度(在多个DS18B20挂载到总线上)
 num_ds18b20是显示那一个DS18B20的温度，目前只支持1和2两个DS18B20
*/
extern float Read_DS18B20_Num(unsigned int num_ds18b20);
#endif										