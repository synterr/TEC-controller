#ifndef MCP9808_H
#define MCP9808_H

#include <stdint.h>

#define MCP_ADRESS 0x18 << 1
#define SIGN_BIT   0X10
#define RES_REG    0X08 //for setting resolution
#define TEMP_REG   0X05 //for reading temperature

#define RES_5_DEGREE     0
#define RES_25_DEGREE    0X01
#define RES_125_DEGREE   0X02
#define RES_0625_DEGREE  0X03

void MCP_Init(void);
void MCP_Write (uint8_t Address, uint8_t Reg, uint8_t Data);
void MCP_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void MCP_WriteMulti (uint8_t Address, uint8_t *Data, uint8_t Size);
float MCP_caculate_temp(uint8_t temp_upper, uint8_t temp_lower);
float MCP_get_temp(void);
void MCP_set_resolution(uint8_t res);
#endif

