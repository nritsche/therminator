#ifndef LCD_H_ 
#define LCD_H_

#include "fonts.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>

#define LCDWIDTH  	128
#define LCDHEIGHT		64
#define ROP_COPY		0
#define ROP_PAINT		1
#define ROP_INVERT	2
#define REVERSED		1

#define LCD_PORT		PORTD
#define LCD_DDR		DDRD
#define LCD_PIN		PIND
#define LCD_CS_BIT	5
#define LCD_RST_BIT  6
#define LCD_A0_BIT	7
#define LCD_SCL_BIT  4
#define LCD_SDA_BIT  1
#define LCD_CS			PD5
#define LCD_RST		PD6
#define LCD_A0	 		PD7
#define LCD_SCL		PD4
#define LCD_SDA		PD1
#define LCD_CS_DIR	DDRD5
#define LCD_RST_DIR  DDRD6
#define LCD_A0_DIR	DDRD7
#define LCD_SCL_DIR  DDRD4
#define LCD_SDA_DIR  DDRD1

// lcd commands
#define CMD_DISPLAY_OFF				0xAE
#define CMD_DISPLAY_ON				0xAF
#define CMD_SET_START_LINE			0x40
#define CMD_SET_PAGE					0xB0
#define CMD_SET_COLUMN_UPPER		0x10
#define CMD_SET_COLUMN_LOWER		0x00
#define CMD_SET_ADC_NORMAL			0xA0  // RTL or LTR
#define CMD_SET_ADC_REVERSE		0xA1
#define CMD_SET_DISP_NORMAL		0xA6
#define CMD_SET_DISP_REVERSE		0xA7  // negative display
#define CMD_SET_ALLPTS_OFF			0xA4
#define CMD_SET_ALLPTS_ON			0xA5
#define CMD_SET_BIAS_9				0xA2 
#define CMD_SET_BIAS_7				0xA3
#define CMD_RMW						0xE0
#define CMD_RMW_CLEAR				0xEE
#define CMD_INTERNAL_RESET		 	0xE2
#define CMD_SET_COM_NORMAL		 	0xC0  
// Reverse makes (0,0) from down-left and invert the font [Vertical FLIP]
#define CMD_SET_COM_REVERSE		0xC8
#define CMD_SET_POWER_CONTROL	 	0x28
#define CMD_SET_RESISTOR_RATIO	0x20
#define CMD_SET_CONTRAST			0x81
#define CMD_SET_STATIC_OFF			0xAC
#define CMD_SET_STATIC_ON		  	0xAD
#define CMD_SET_STATIC_REG		 	0x0
#define CMD_SET_BOOSTER_FIRST	 	0xF8
#define CMD_SET_BOOSTER_234		0
#define CMD_SET_BOOSTER_5		  	1
#define CMD_SET_BOOSTER_6		  	3

void lcdInit(void);
void lcdEnable(void);
void lcdDisable(void);
void lcdSelectFont(const fontdescriptor_t *font);
void lcdSetPixel(uint8_t x, uint8_t y, uint8_t on);
void lcdSetPos(uint8_t line, uint8_t column);
void lcdClear(void);
void lcdWriteChar(char c);
void lcdWriteString(char *s);

static const unsigned char _initSeq[] PROGMEM = {
	CMD_SET_BIAS_9,				// set bias to 1/9
	CMD_SET_ADC_NORMAL,			// SEG output direction = normal
	CMD_SET_COM_REVERSE,	 		// COM output direction = reversed
	CMD_SET_START_LINE,		  	// start line = 0
	CMD_SET_DISP_NORMAL,	 		// Normal Display
	CMD_SET_ALLPTS_OFF,		  	// display all points = OFF
	CMD_SET_POWER_CONTROL | 7, // Power Control Set
	CMD_SET_RESISTOR_RATIO | 4,// set ra/rb
	CMD_SET_CONTRAST,		 		// set contrast
	0x20,					 			// -> to 31
	CMD_DISPLAY_ON,				// Display on
	0x00,					 			// (terminator)
};

#endif
