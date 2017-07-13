#ifndef __ds18b20_h__
#define __ds18b20_h__

#define PIN_DS18B20 P14



extern void DS18B20_Mode(void);
extern float rd_temperature_f(void);
#endif										