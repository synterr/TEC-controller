#include "stm32f4xx.h"                  // Device header
#include "MCP9808.h"
#include "i2c.h"
#include "tools.h"

void MCP_Init(void)
{
  i2c_init();
  MCP_set_resolution(RES_0625_DEGREE);
}

void MCP_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
	i2c_start ();
	i2c_address (Address);
	i2c_write (Reg);
	i2c_write (Data);
	i2c_stop ();
}

void MCP_WriteMulti (uint8_t Address, uint8_t *Data, uint8_t Size)
{
	i2c_start ();
	i2c_address (Address);
  i2c_writemulti (Data, Size);
  i2c_stop ();
}

void MCP_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
  i2c_start(); // send START command
  i2c_address (Address & 0xFE); //WRITE Command (see Section 4.1.4 “Address Byte”)
  //also, make sure bit 0 is cleared ‘0’
  i2c_write(0x05); // Write TA Register Address
  
  i2c_start(); //Repeat START
  i2c_read(Address | 0x01, buffer, size);
 
  i2c_stop(); // send STOP command
}

float MCP_caculate_temp(uint8_t temp_upper, uint8_t temp_lower) {
    float temp = 0;

  if (temp_upper & SIGN_BIT) {
        temp_upper &= 0x0f;
        temp = 256 - (temp_upper * 16 + temp_lower * 0.0625);
        temp *= -1;
  }
  else {
    temp_upper &= 0x0f;
    temp = temp_upper * 16 + temp_lower * 0.0625;
  }
  
  return temp;
}

float MCP_get_temp(void) {
  
  uint8_t buff[2];
  MCP_Read(MCP_ADRESS, TEMP_REG, buff, 2);
  return MCP_caculate_temp(buff[0], buff[1]);
}

void MCP_set_resolution(uint8_t res){
  MCP_Write(MCP_ADRESS, RES_REG, res);
}

