#ifndef MAIN_H_
#define MAIN_H_

#include "lcd.h"
#include "spi.h"
#include "baro.h"
#include <avr/interrupt.h>
#include "AnalogSensors.h"

#define CHANNELS 7
#define FIRST_OUT_CH 20

uint16_t channel[CHANNELS]; //current input servo-positions

#endif
