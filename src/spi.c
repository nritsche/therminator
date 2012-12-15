#include "spi.h"
#include "Processors.h"
#include "lcd.h"

volatile unsigned char spi_data;
volatile unsigned char data_sent;

void initSPI()
{
	// set PB5(MOSI), PB7(SCK) as output
	// also PB4(SS) to ensure SPI functionality
	SPI_DIR  |= (1<<SPI_MOSI_PIN)|(1<<SPI_SCK_PIN)|(1<<PB4);

	// and MISO as input
	SPI_DIR &= ~(1<<SPI_MISO_PIN);

   // enable SPI Interrupt and SPI in Master Mode with SCK = CK/128
   // with MSB transmitted first, clock HIGH when idle and
   // clock phase CPHA set to 1 ?
	SPCR0 |= (1<<SPIE0)|(1<<SPE0)|(1<<MSTR0)|(1<<SPR10)|(1<<SPR00)|(1<<CPOL0)|(1<<CPHA0);
	
	// clear SPIF bit in SPSR
	spi_data = SPSR0;
	spi_data = SPDR0;

	clear_to_send = 1;
}

// called when next byte can be send
ISR(SPI_STC_vect)
{
	if (!data_sent) {
		data_sent = 1;
		SPDR0  = spi_data;	// send char
		lcdWriteString ("wrote data to SPDR0\n");
	}
	else
		clear_to_send = 1;	
}

//initiate transmit
uint8_t send (volatile unsigned char cmd, volatile unsigned char data)
{
	if (clear_to_send) {
		clear_to_send = 0;

		spi_data = data;
		data_sent = 0;
		SPDR0 = cmd;
		//lcdWriteString ("wrote cmd to SPDR0\n");

		return 1;
	}
	else {
		return 0;
	}
}

unsigned char receive ()
{
	if (clear_to_send) {
		//lcdWriteString ("read from SPDR0\n");
			return SPDR0;
	}
	else
		return 0x1F;
}
