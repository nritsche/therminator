/*
 blueRC frequency dependent definitions
 
 Carsten Bruns (carst.bruns@gmx.de)
 */

#ifndef FREQUENCIES_H_
#define FREQUENCIES_H_

#if (F_CPU == 8000000UL)
#define TWI_BITRATE 18;  //(~50khz)
#define  TWI_PRESCALER 0b11111001;  //TWI-prescaler 4
#define  UART_BAUDRATE 25;
#elif (F_CPU == 20000000UL)
#define TWI_BITRATE 48;  //(~50khz)
#define TWI_PRESCALER 0b11111001;  //TWI-prescaler 4
#define UART_BAUDRATE 64;
#else
# error "The selected frequency is not supported by Interface-library!"
#endif

#endif /* FREQUENCIES_H_ */
