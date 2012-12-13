/*
 * pwm.h
 *
 * Created: 30.07.2012 12:38:11
 *  Author: OliverS
 *
 * $Id: pwm.h 35 2012-08-27 10:25:23Z olischulz24@googlemail.com $
 */ 


#ifndef PWM_H_
#define PWM_H_

void pwmInit();
void pwmWrite(uint8_t channel, uint16_t value);
void pwmEnable();
void pwmDisable();

#endif /* PWM_H_ */