#include "spi.h"
#include "Processors.h"

//volatile unsigned char* outgoing[SPI_CMD_SIZE];
volatile unsigned char incoming[SPI_CMD_SIZE];
volatile short int received=0;
volatile unsigned char* send_ptr;
volatile unsigned char data_sent;
volatile unsigned char spi_data;

void initSPI()
{
	// set PB4(/SS), PB5(MOSI), PB7(SCK) as output 
	SPI_DIR  |= (1<<PB4)|(1<<PB5)|(1<<PB7);

	// and MISO as input
	SPI_DIR &= ~(1<<SPI_MISO_PIN);

   // enable SPI Interrupt and SPI in Master Mode with SCK = CK/16
   // with MSB transmitted first, clock HIGH when idle and
   // clock phase CPHA set to 1 ?
	SPCR0 |= (1<<SPIE0)|(1<<SPE0)|(1<<MSTR0)|(1<<SPR10)|(1<<SPR00)|(1<<CPOL0)|(1<<CPHA0);
	
	// clear SPIF bit in SPSR
	volatile char IOReg = SPSR0;
	IOReg = SPDR0;
	clear_to_send = 1;
}

// called every spi clock cycle
ISR(SPI_STC_vect)
{
	if (!data_sent) {
		SPDR0  = spi_data;	// send char
		data_sent = 1;
	}
	else
		clear_to_send = 1;	
}

//initiate transmit
uint8_t send (volatile unsigned char cmd, volatile unsigned char data)
{
	if (clear_to_send) {
		clear_to_send = 0;

		SPDR0 = cmd;

		spi_data = data;
		return 1;
	}
	else {
		return 0;
	}
}

unsigned char receive ()
{
	if (clear_to_send) {
			return SPDR0;
	}
	else
		return 0x00;
}
