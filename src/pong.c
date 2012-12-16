/*
  pong.c
*/
#include "pong.h"

void init_values( void) {
	cursor = (sizeY-csize)/2;
	ballx = 2;
	bally = (sizeY-csize)/2;
	speedx = speed;
	speedy = 0;
	points = 0;
}

void init_pong( void) {
	init_values();
	acc_cal = analog_sensor_values[ACC_X];
}

void pong_next( void) {
	if (!gameover) {
	cursor += (acc_cal-analog_sensor_values[ACC_X])/8; //move cursor
	if (cursor > sizeY-1-csize)
		cursor = sizeY-1-csize;
	else if (cursor < 0)
		cursor = 0;
	ballx += speedx; //move ball;
	bally += speedy;
	if (ballx >= sizeX-speedx)
		speedx = -speedx;
	if (bally <= 0-speedy || bally >= sizeY-speedy)
		speedy = -speedy;
	if (ballx <= 0-speedx) {
		speedx = -speedx;
		if (bally < cursor || bally > cursor+csize)
			gameover = gameovertime;
		speedy = bally-(cursor+csize/2);
	}

	lcdClear();
	for (uint8_t i = 0; i <=sizeY; i++) { //draw cursor
			lcdSetPixel(sizeX-1,i,1);
			lcdSetPixel(sizeX,i,1);
		}
	for (uint8_t i = cursor; i <=cursor+csize; i++) { //draw cursor
		lcdSetPixel(0,i,1);
		lcdSetPixel(1,i,1);
	}
	lcdSetPixel(ballx,bally,1);
	lcdSetPixel(ballx+1,bally,1);
	lcdSetPixel(ballx,bally+1,1);
	lcdSetPixel(ballx+1,bally+1,1);
	lcdSetPos(0,85);
	points++;
	lcdWriteString("Score:");
	lcdSetPos(1,85);
	} else {
		lcdClear();
		lcdSetPos(2,32);
		lcdWriteString("Game Over!");
		lcdSetPos(5,32);
		lcdWriteString("Score: ");
		gameover--;
		if (gameover == 0)
		  init_values();
	}
	char score[10];
	itoa(points,score, 10);
	lcdWriteString(score);
}
