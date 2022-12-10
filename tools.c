#include "stm32f4xx.h"                  // Device header
#include "tools.h"


void delay_nops (uint32_t dlyTicks) {
  
  for(uint32_t dly =0; dly < dlyTicks; dly++) { __NOP(); }
}


