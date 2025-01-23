#ifndef  GUI_H
#define  GUI_H
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "map"
#include "font.h"
#include "clock.h"
#include "images.h"
#include "network.h"
#include "images/astronaut.h"

void main_gui();
void tft_init();
void showWeatherIcon(const char* weatherCondition);

#endif