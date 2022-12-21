#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "regulator.h"

static float reg_set_temp = 23.0;
static float reg_cur_temp = 23.0;

static double previous_error = 0.0;
static double error = 0.0;

void reg_settemp_set(float temp){
  reg_set_temp = temp;
}

float reg_settemp_get(void){
  return reg_set_temp;
}

void reg_curtemp_set(float temp){
  reg_cur_temp = temp;
}

float reg_curtemp_get(void){
  return reg_cur_temp;
}

float reg_error_get(void){
  return error;
}

#define DT 0.1  // Sample time (seconds)

// PID control parameters
#define Kp 20.0
#define Ki 0.1
#define Kd 0.1

// Setpoint and initial conditions

  
double doPID(double measured_value, double setpoint) {
  

  // Calculate error
  double error = setpoint - measured_value;
  static double integral = 0.0;
  // Update integral
  integral += error * DT;
  
  // Calculate derivative
  double derivative = (error - previous_error) / DT;
  
  // Calculate control signal
  double control_signal = Kp * error + Ki * integral + Kd * derivative;
  
  
  // Update previous error for next iteration
  previous_error = error;

 
  return control_signal;
}