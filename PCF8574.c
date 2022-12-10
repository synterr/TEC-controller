#include "stm32f4xx.h"                  // Device header
#include "PCF8574.h"
#include "i2c.h"
#include "tools.h"

void PCF_Init(void)
{
  i2c_init();
}

void PCF_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
	i2c_start ();
	i2c_address (Address);
	//i2c_write (Reg);
	i2c_write (Data);
	i2c_stop ();
}

void PCF_WriteMulti (uint8_t Address, uint8_t *Data, uint8_t Size)
{
	i2c_start ();
	i2c_address (Address);
  i2c_writemulti (Data, Size);
  i2c_stop ();
}

void PCF_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	i2c_start ();
	i2c_address (Address);
	i2c_write (Reg);
	i2c_start ();  // repeated start
	i2c_read (Address+0x01, buffer, size);
	i2c_stop ();
}




