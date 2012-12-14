#include "spi.h"

//volatile unsigned char* outgoing[SPI_CMD_SIZE];
volatile unsigned char incoming[SPI_CMD_SIZE];
volatile short int received=0;
volatile short int clear_to_send=0;
volatile unsigned char* send_ptr;

void initSPI()
{
	// set PB4(/SS), PB5(MOSI), PB7(SCK) as output 
	SPI_DIR  = (1<<PB4)|(1<<PB5)|(1<<PB7);

	// and MISO as input
	SPI_DIR &= ~(1<<SPI_MISO_PIN);

   // enable SPI Interrupt and SPI in Master Mode with SCK = CK/16
   // with MSB transmitted first, clock HIGH when idle and
   // clock phase CPHA set to 1 ?
	SPCR0 = (1<<SPIE0)|(1<<SPE0)|(1<<MSTR0)|(1<<SPR00)|(1<<CPOL0)|(1<<CPHA0);

	// clear SPIF bit in SPSR
	volatile char IOReg = SPSR0;
	IOReg = SPDR0;
}

// called every spi clock cycle
ISR(SPI_STC_vect)
{
	send_ptr++; // ptr to next char
	
	if (*send_ptr != 0)
		SPDR0  = *send_ptr;	// send char
	else
		clear_to_send = 1;	
}

//initiate transmit
uint8_t send (volatile unsigned char* out[SPI_CMD_SIZE])
{
	if (clear_to_send) {
		clear_to_send = 0;
		// set ptr to start of string
		send_ptr = out[0];
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
