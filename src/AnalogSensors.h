/*
  AnalogSensors.h
*/

#ifndef ANALOGSENSORS_H_
#define ANALOGSENSORS_H_

#include <avr/interrupt.h>
#include <avr/io.h>

#define ACC_X 5
#define ACC_Y 6
#define ACC_Z 7
#define GYR_X 1
#define GYR_Y 4
#define GYR_Z 2
#define BATT 3
#define FIRST_ADC_CH 1

uint16_t analog_sensor_values[8];

void init_analog_sensors( void);

#endif /* ANALOGSENSORS_H_ */
