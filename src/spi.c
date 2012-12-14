#include "spi.h"

volatile unsigned char incoming[BUFSIZE];
volatile short int received=0;

void initSPI()
{
	// set PB4(/SS), PB5(MOSI), PB7(SCK) as output 
	SPI_DIR  = (1<<PB4)|(1<<PB5)|(1<<PB7);

	// and MISO as input
	SPI_DIR &= ~(1<<SPI_MISO_PIN);

   // enable SPI Interrupt and SPI in Master Mode with SCK = CK/16
   // with MSB transmitted first, clock HIGH when idle and
   // clock phase CPHA set to 1 ?
	SPCR  = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA);

	// clear SPIF bit in SPSR
	volatile char IOReg   = SPSR;
	IOReg   = SPDR;

	__enable_interrupt();
}

// called when last byte was sent
ISR(SPI_STC_vect)
{
}
