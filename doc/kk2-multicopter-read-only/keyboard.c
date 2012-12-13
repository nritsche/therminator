/*
 * keyboard.c
 *
 * Created: 30.07.2012 14:41:27
 *  Author: OliverS
 *
 * $Id: keyboard.c 32 2012-08-24 12:03:25Z olischulz24@googlemail.com $
 */ 

#include "global.h"
#include "keyboard.h"

#define ALLKEYS (KEY_1 | KEY_2 | KEY_3 | KEY_4)
static uint8_t lastKeys;
static uint16_t keyDown;

uint8_t keyboardState()
{
	return ~KEYBOARD_PIN & ALLKEYS;
}

uint8_t keyboardRead()
{
	uint8_t keys = keyboardState();
	uint8_t newkeys = keys & ~lastKeys;
	if (keys)
	{
		if (lastKeys != keys)
			keyDown = millis();
	}		
	lastKeys = keys;
	return newkeys;
}

void keyboardInit()
{
	// key pins to input
	KEYBOARD_DDR |= ALLKEYS;
	// pull up resistors on
	KEYBOARD_PORT |= ALLKEYS;
}

	