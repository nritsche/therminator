#ifndef SPI_H_
#define SPI_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include <spi.h>

#define SPI_PORT PORTB
#define SPI_DIR DDRB
#define SPI_CS_PIN PB4
#define SPI_SCK_PIN PB7
#define SPI_MOSI_PIN PB5
#define SPI_MISO_PIN PB6

#define SPI_CMD_SIZE 2

#endif
