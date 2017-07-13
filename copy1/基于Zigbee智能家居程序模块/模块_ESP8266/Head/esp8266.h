#ifndef __esp8266_h__
#define __esp8266_h__

uint8_t esp_8266_send_com ( uint8_t *com, uint8_t *ack, uint16_t waittime);


void init_esp_8266_setting ( void);

#endif