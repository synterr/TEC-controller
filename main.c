#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"                  // Device header
#include "tools.h"
#include "gpio.h"
#include "uart.h"
#include "i2c-lcd.h"
#include "PCF8574.h"
#include "MCP9808.h"
#include "commands.h"
#include "regulator.h"
#include "pwm.h"

void EXTI15_10_IRQHandler(void);

static volatile uint32_t msTicks;                                 // counts 1ms timeTicks


//void SysTick_Handler(void);
void Delay (uint32_t dlyTicks);
void SystemCoreClockConfigure(void);
/*----------------------------------------------------------------------------
 * SysTick_Handler:
 *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++;
//}

/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {
	
	// Setting max 84 MHz system clock !!!
  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_ICEN;                            // Instruction cache enable
  FLASH->ACR |= FLASH_ACR_DCEN;                            // Data cache enable
  FLASH->ACR |= FLASH_ACR_LATENCY_5WS;                     // Flash 5 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK (AHB prescaler)
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                        // APB1 = HCLK/2 (PCLK1 is 42MHz max)
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // APB2 = HCLK/1

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

  // PLL configuration:  VCO = HSI/M * N,  Sysclk = VCO/P
  RCC->PLLCFGR = ( 8ul                   |                 // PLL_M =  8
                 (84ul <<  6)            |                 // PLL_N = 84
                 (  2ul << 16)            |                // PLL_P =   2
                 (RCC_PLLCFGR_PLLSRC_HSI) |                // PLL_SRC = HSI
                 (  8ul << 24)             );              // PLL_Q =   8

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}

/*----------------------------------------------------------------------------
 * main: blink LED and check button state
 *----------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockConfigure();                              // configure HSI as System Clock
  SystemCoreClockUpdate();
  //SysTick_Config(SystemCoreClock / 1000);                  // SysTick 1 msec interrupts
  
  gpio_init(GPIO_PIN_LED);
  //gpio_init(GPIO_PIN_WSLEDS);
  gpio_init_input(GPIO_PIN_BTN);
  
  PCF_Init();
  MCP_Init();
  
  PWM_init();
  
  UART2_init();
  uart_clear_buffer();
  ///////spi_init();
  gpio_up(GPIO_PIN_LED);
  gpio_down(GPIO_PIN_LED);
  
  NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_EnableIRQ(USART2_IRQn);
  
  lcd_init();
  
  long cnt = 0;
  while(1) {
    cnt++;
    
    float PIDcontrol = 0;
    //Regulator loop
    if(cnt % 1000 == 0){
      reg_curtemp_set(MCP_get_temp());
      
    }
    //Display loop
    if(cnt % 10000 == 0){
      PIDcontrol = doPID(reg_curtemp_get(), reg_settemp_get());
      if (PIDcontrol > 10)
        PIDcontrol = 10;
      if (PIDcontrol < -7)
        PIDcontrol = -7;
      PWM_set_val(600-60*PIDcontrol);
      
      char t[10]; //size of the number
      gpio_up(GPIO_PIN_LED);
      float_to_string(reg_curtemp_get(), t); 
      lcd_put_cur(0, 0);
      lcd_send_string("T-cur: ");
      lcd_send_string(t);
      lcd_send_string("      ");
      float_to_string(PIDcontrol, t); 
      lcd_put_cur(2, 0);
      lcd_send_string("Error: ");
      lcd_send_string(t);
      lcd_send_string("      ");
    }
    if(cnt % 10000 != 0)
      gpio_down(GPIO_PIN_LED);
    
    //UART buffer loop
    if(cnt % 1000 == 0)
    {
      if (uart_get_buffer_len() > 0 && uart_get_buffer()[uart_get_buffer_len() - 1] == '\0')
      {
        commands_execute(uart_get_buffer(), uart_get_buffer_len());
        uart_clear_buffer();
      }
    }
  }
}

void EXTI15_10_IRQHandler(void) {
  if (EXTI->PR & (1 << BTN_PINPOS)) {
    /* clear the interrupt flag by writing a 1 */
    EXTI->PR |= (1 << BTN_PINPOS);
    gpio_up(GPIO_PIN_LED);
  }
}

