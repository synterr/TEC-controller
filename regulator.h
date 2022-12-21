#ifndef REGULATOR_H
#define REGULATOR_H

#include <stdint.h>
#include <stdbool.h>

void reg_settemp_set(float temp);
float reg_settemp_get(void);

void reg_curtemp_set(float temp);
float reg_curtemp_get(void);

double doPID(double measured_value, double setpoint);
float reg_error_get(void);

#endif

