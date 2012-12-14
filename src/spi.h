#ifndef SPI_H_
#define SPI_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define SPI_PORT PORTB
#define SPI_CS_PORT PORTC
#define SPI_DIR DDRB
#define SPI_CS_DIR DDRC
#define SPI_CS_PIN1 PC6
#define SPI_CS_PIN2 PC4
#define SPI_SCK_PIN PB7
#define SPI_MOSI_PIN PB5
#define SPI_MISO_PIN PB6

#define SPI_CMD_SIZE 2

void initSPI(void);
uint8_t send (volatile unsigned char* out[SPI_CMD_SIZE]);

#endif
