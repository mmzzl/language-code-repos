#ifndef _GPIO_H_
#define _GPIO_H_
#include "Arduino.h"

uint8_t key_scan(void);
void keyx_check(GPIO_PinState sta, uint8_t num, uint8_t *flag, uint8_t *result);
uint16_t key_cnt[8] = {0};


typedef enum
{
  GPIO_Pin_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

#endif