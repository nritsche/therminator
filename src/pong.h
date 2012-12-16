/*
  pong.h
*/

#ifndef PONG_H_
#define PONG_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include "AnalogSensors.h"
#include "lcd.h"

#define sizeX 64
#define sizeY 64
#define speed 2
#define csize 10
#define gameovertime 50;

int8_t speedx, speedy;
int8_t cursor;
uint8_t ballx, bally, gameover;
uint16_t acc_cal;
uint16_t points;

void init_pong( void);
void pong_next( void);

#endif /* PONG_H_ */
