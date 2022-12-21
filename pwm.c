#include "stm32f4xx.h"                  // Device header
#include "pwm.h"

// TIM1 CH1 on PA8

static uint16_t pwm_val = 1023; //Higher values means lower power (inverted)

void PWM_init(void) {
  
  RCC->APB2ENR = RCC_APB2ENR_TIM1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable GPIOA CLOCK
  GPIOA->MODER |= (2<<16);              //  alternate function for PA8
  GPIOA->OTYPER &= ~(1 << 8);
  GPIOA->PUPDR  &= ~(3 << 16);
  GPIOA->AFR[1] |= (1<<0);    // Bits (3:2:1:0) = 0:0:0:1 --> AF1 for pin PA8;


 TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
 TIM1->CCER = TIM_CCER_CC1E;
 TIM1->BDTR = TIM_BDTR_MOE;
 TIM1->PSC = 1;
 TIM1->ARR = 1023;
 TIM1->CCR1 = pwm_val;
 TIM1->EGR = TIM_EGR_UG;
 TIM1->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
 
}

void PWM_set_val(uint16_t val){
  if (val > 1023)
    val = 1023;

  pwm_val = val;
  TIM1->CCR1 = 1023-pwm_val; //Higher values means lower power (inverted)
}

uint16_t PWM_get_val(void){
  return pwm_val;
}