#include "baro.h"
#include "spi.h"
#include "lcd.h"

void init_baro()
{
	// set cs pins as output
	SPI_CS_DIR |= ((1<<SPI_CS_PIN1) | (1<<SPI_CS_PIN2));

	// set both CS high
	SPI_CS_PORT |= (1<<SPI_CS_PIN1);// | (1<<SPI_CS_PIN2));
	SPI_CS_PORT &= ~(1<<SPI_CS_PIN2);
}

unsigned char get_pressure(unsigned short int pitot)
{
	//SPI_CS_PORT &= ~(1<<SPI_CS_PIN1);
	
	while (!send (BARO_CMD | WHO_AM_I, 0x00));
	unsigned char p;
	
	while (!clear_to_send);
	
	p = receive();

	//SPI_CS_PORT |= (1<<SPI_CS_PIN1);
	return p;
}
