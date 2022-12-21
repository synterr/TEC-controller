#include "stm32f4xx.h"                  // Device header
#include "commands.h"
#include "string.h"
#include "tools.h"
#include "uart.h"
#include "i2c-lcd.h"
#include "MCP9808.h"
#include "regulator.h"
#include "pwm.h"

#define PARAMS_NUM 4
#define PARAMS_LEN 21

static char param[PARAMS_NUM][PARAMS_LEN]; //command parameters
static uint8_t param_len[PARAMS_NUM];      //parameter lenght

void commands_execute (char* buff, uint8_t len) {
  commands_parse(buff, len);

  lcd_put_cur(3, 0);
  lcd_send_string(commands_param_get(0));
  
  // Set target for temperature regulator
  if (strcmp(commands_param_get(0), "SETTEMP") == 0)
  {
    lcd_put_cur(1, 0);
    lcd_send_string("T-set: ");
    lcd_send_string(commands_param_get(1));
    reg_settemp_set(string_to_float(commands_param_get(1)));
  }
  // Get target for temperature regulator
  else if (strcmp(commands_param_get(0), "GETSETT") == 0)
  {
    char t[10]; //size of the number
    float_to_string(reg_settemp_get(), t); 
    uart_send_string("SETTEMP ", 8, false);
    uart_send_string(t, 7, true);
    lcd_send_string("    ");
  }
  // Get current temperature reading
  else if (strcmp(commands_param_get(0), "GETCURT") == 0)
  {
    char t[10]; //size of the number
    float_to_string(reg_curtemp_get(), t); 
    uart_send_string("CURTEMP ", 8, false);
    uart_send_string(t, 7, true);
    lcd_send_string("    ");
  }
  // Set pwm
  else if (strcmp(commands_param_get(0), "SETPWM") == 0)
  {
    PWM_set_val(string_to_int(commands_param_get(1)));
    lcd_put_cur(2, 0);
    lcd_send_string("PWM: ");
    char t[10]; //size of the number
    int_to_string(PWM_get_val(), t); 
    lcd_send_string(t);
    lcd_send_string("    ");
  }
}

void commands_parse (char* buff, uint8_t len) {
  uint8_t cidx = 0;
  uint8_t cchr = 0;
  for (uint8_t i = 0; i < len; i++)
  {
    if (buff[i] != ' '){
      if (cidx == 0)
        param[cidx][cchr] = toupper(buff[i]); // first parameter to uppercase. Command name should not be case sensitive
      else
        param[cidx][cchr] = buff[i];
      
      if (cchr < PARAMS_LEN)
        cchr++;
    }
    else {
      param[cidx][cchr] = '\0'; // end of string fo current command at cidx
      param_len[cidx] = cchr-1;
      if (cidx < PARAMS_NUM)
        cidx++;
      cchr = 0;
    }
  }
}

char* commands_param_get(uint8_t index) {
  return param[index];
}
uint8_t commands_param_len_get(uint8_t index) {
  return param_len[index];
}