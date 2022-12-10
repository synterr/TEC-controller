#include "stm32f4xx.h"                  // Device header
#include "commands.h"
#include "string.h"
#include "i2c-lcd.h"

#define PARAMS_NUM 4
#define PARAMS_LEN 21

static char param[PARAMS_NUM][PARAMS_LEN]; //command parameters

void commands_execute (char* buff, uint8_t len) {
  commands_parse(buff, len);

  lcd_put_cur(3, 0);
  lcd_send_string(commands_param_get(0));

  
  if (strcmp(commands_param_get(0), "SETTEMP") == 0)
  {  
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
      if (cidx < PARAMS_NUM)
        cidx++;
      cchr = 0;
    }
  }
}

char* commands_param_get(uint8_t index) {
  return param[index];
}
