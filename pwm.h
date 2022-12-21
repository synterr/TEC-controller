#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_init (void);
void PWM_set_val(uint16_t val);
uint16_t PWM_get_val(void);

#endif

