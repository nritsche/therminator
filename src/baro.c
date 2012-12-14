#include "baro.h"
#include "spi.h"

void init_baro()
{
	// set cs pins as output
	SPI_CS_DIR |= ((1<<SPI_CS_PIN1) | (1<<SPI_CS_PIN2));
}

unsigned char get_pressure(unsigned short int pitot)
{
	SPI_CS_PORT |= (1<<SPI_CS_PIN1);
	unsigned char p = receive();
	SPI_CS_PORT &= ~(1<<SPI_CS_PIN1);
	return p;
}
