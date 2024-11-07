#include <Arduino.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "SevSeg.h"

const int OUT_CTRL = 3;
const int GREEN_LED_PIN = 5;
const int TEMP_SET_PIN = A1;
const int TEMP_SENSOR_PIN = A0;
const int LEFT_PUSHBUTTON_PIN = A3;
const int RIGHT_PUSHBUTTON_PIN = A2;
const int BALL_SW_PIN = A4;
const bool LAST_BALL_SW_STATUS = 0;
const int SETBIAS_PIN = A6;
const int64_t AUTOOFF_TIME = 5LL * 60LL * 1000LL;
const int DISPLAY_PERIOD = 2000;
#endif