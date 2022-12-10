#ifndef PCF8574_H
#define PCF8574_H

#include <stdint.h>
void PCF_Init(void);
void PCF_Write (uint8_t Address, uint8_t Reg, uint8_t Data);
void PCF_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void PCF_WriteMulti (uint8_t Address, uint8_t *Data, uint8_t Size);
#endif

