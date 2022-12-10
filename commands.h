#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

void commands_execute (char* buff, uint8_t len);
void commands_parse (char* buff, uint8_t len);
char* commands_param_get(uint8_t index);

#endif

