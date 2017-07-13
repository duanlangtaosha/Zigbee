#ifndef __EEPROM_H
#define __EEPROM_H

unsigned char iap_read_byte(unsigned int addr);
void iap_program_byte(unsigned int addr, unsigned char dat);
void iap_erase_sector(unsigned int addr);

#endif