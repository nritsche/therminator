#ifndef BARO_H_
#define BARO_H_

#define PRESS_OUT_XL 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A

#define SPI_CS_PORT PORTC
#define SPI_CS_DIR DDRC
#define SPI_CS_PIN1 PC6
#define SPI_CS_PIN2 PC4

unsigned char get_pressure(void);

#endif
