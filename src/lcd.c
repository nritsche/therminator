#include "lcd.h"

static uint8_t _screen[LCDWIDTH * LCDHEIGHT / 8];
static uint8_t _curx, _cury;
static fontdescriptor_t _font;
static uint8_t _flags = 0;

static void lcdSetByte(uint8_t x, uint8_t y, uint8_t b)
{
	uint8_t *scr = _screen + x + (y / 8 * LCDWIDTH);
	if (_flags & REVERSED)
		*scr = ~b;
	else
		*scr = b;
}

static void sendByte(uint8_t byte)
{
	for (uint8_t i = 8; i; i--) {
		LCD_PORT &= ~(1 << LCD_SCL);
		if (byte & 0x80)
			LCD_PORT |= (1 << LCD_SDA);
		else
			LCD_PORT &= ~(1 << LCD_SDA);
		LCD_PORT |= (1 << LCD_SCL);
		byte <<= 1;
	}
}

static void sendCommand(uint8_t command)
{
	LCD_PORT &= ~((1 << LCD_CS) | (1 << LCD_A0));
	sendByte(command);
	LCD_PORT |= (1 << LCD_CS);
}

void lcdInit()
{
	// pins as output
	DDRD |= 0b11110010;
	
	// reset display
	LCD_PORT &= ~(1 << LCD_RST);
	_delay_ms(1);
	LCD_PORT |= (1 << LCD_RST);
	_delay_ms(1);
	
	const unsigned char* ptr = _initSeq;
	uint8_t c;
	while ((c = pgm_read_byte(ptr++)))
	sendCommand(c);
	lcdSelectFont(NULL);	 // select default font

	// use timer0 with clk/8 and overflow
	// at 256 as interrupt based output of data bytes
	// ie every 1024us one byte is send to display.
	// whole screen takes about 105ms
	TCCR0B = _BV(CS01);	  // clk/8
	lcdEnable();
}

void lcdEnable()
{
	// reset timer0 to avoid re-entrant call of ISR
	// because it is non blocking
	TCNT0 = 0;
	TIFR0 = _BV(TOV0);
	TIMSK0 |= _BV(TOIE0);	// enable interrupt on overflow
}

void lcdDisable()
{
	TIMSK0 &= ~_BV(TOIE0);  // disable overflow interrupt
}


void lcdSelectFont(const fontdescriptor_t *font)
{
	if (font == NULL)
		font = &font6x8;
	memcpy_P(&_font, font, sizeof(_font));
}

static void sendData(uint8_t data)
{
	LCD_PORT &= ~(1 << LCD_CS);
	LCD_PORT |= (1 << LCD_A0);
	sendByte(data);
	LCD_PORT |= (1 << LCD_CS);
}

static void setPos(uint8_t line, uint8_t column)
{
	sendCommand(CMD_SET_PAGE | (line & 0x07));
	sendCommand(CMD_SET_COLUMN_UPPER | (column / 16));
	sendCommand(CMD_SET_COLUMN_LOWER | (column & 0x0f));
}

__attribute__ ((section(".lowtext")))
ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
{
	static uint16_t offset;

	if (offset % LCDWIDTH == 0)
		setPos(offset / LCDWIDTH, 0);

	sendData(*(_screen + offset++));
	offset %= sizeof(_screen);
}

void lcdWriteSprite_P(PGM_P sprite, uint8_t sizeX, uint8_t sizeY, uint8_t mode)
{
	uint8_t b = 0;
	for (uint8_t i = 0; i < sizeX; i++)
	{
		for (uint8_t j = 0; j < sizeY; j++)
		{
			if (j % 8 == 0)
					b = pgm_read_byte(sprite++);

			if (mode == ROP_COPY)
			{
				if ((_cury % 8 == 0) && (sizeY - j >= 8))
				{
					lcdSetByte(_curx + i, _cury + j, b);
					j += 7; // just +7 b/c the loop increments anyway
				}
				else
					lcdSetPixel(_curx + i, _cury + j, b & 0x01);
			}
			else if (mode == ROP_PAINT)
			{
				if (b & 0x01)
					lcdSetPixel(_curx + i, _cury + j, 1);
			}
			b >>= 1;
		}
	}
}

void lcdSetPixel(uint8_t x, uint8_t y, uint8_t color)
{
	static const char PROGMEM masks[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
	uint8_t *scr = _screen + x + (y / 8 * LCDWIDTH);
	uint8_t mask = pgm_read_byte(&masks[y % 8]);
	if ((_flags & REVERSED) ^ !color)
		*scr = *scr & ~mask;
	else
		*scr = *scr | mask;
}

void lcdSetPos(uint8_t line, uint8_t column)
{
	_curx = column % LCDWIDTH;
	_cury = line * 8;
}

void lcdClear()
{
	memset(_screen, 0, sizeof(_screen));
	lcdSetPos(0, 0);
	_flags = 0;
}

void lcdWriteChar(char c)
{
	if (c == '\n')
	{
		_cury += _font.sizeY;
		_curx = 0;
	}
	else
	{
		lcdWriteSprite_P(_font.selector(c), _font.sizeX, _font.sizeY, 0);
		_curx += _font.sizeX;
	}
}

void lcdWriteString(char *s)
{
	char c;
	while ((c = *s++))
		lcdWriteChar(c);
}
