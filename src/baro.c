#include "baro.h"
#include "spi.h"
#include "lcd.h"

void init_baro()
{
	// set cs pins as output
	SPI_CS_DIR |= ((1<<SPI_CS_PIN1) | (1<<SPI_CS_PIN2));

	// set both CS high (they are low-active)
	SPI_CS_PORT |= ((1<<SPI_CS_PIN1) | (1<<SPI_CS_PIN2));
}

unsigned char get_pressure(unsigned short int pitot)
{
	if (pitot && pitot != 1)
		return 0xFF;

	//PIN2 is defined 4 and PIN1 is defined 6
	SPI_CS_PORT &= ~(1 << (SPI_CS_PIN2 + pitot * 2));
	
	while (!send (BARO_CMD | WHO_AM_I, 0x00));
	unsigned char p;
	
	while (!clear_to_send);

	// at carsten: wie wärs mal mit einer sleeptime als test HIER
	// #include <util/delay.h> _delay_ms(const);
	
	p = receive();

	// set CS high again
	SPI_CS_PORT |= (1 << (SPI_CS_PIN1 + pitot * 2));
	return p;
}
