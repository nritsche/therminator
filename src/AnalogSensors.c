/*
  AnalogSensors.c
*/

#include "AnalogSensors.h"

ISR(ADC_vect) {
	uint16_t tmp = ADCL;
	tmp += ADCH*256;
	analog_sensor_values[ADMUX] = tmp;
	if (ADMUX < 7)
		ADMUX++;
	else
		ADMUX = FIRST_ADC_CH;
	ADCSRA |= (1 << ADSC);
}

void init_analog_sensors( void) {
	DIDR0 = 0xFF;	//disable all digital inputs on Port A
	ADMUX = FIRST_ADC_CH; //select first (used) adc channel
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
