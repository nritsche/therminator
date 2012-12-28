/*
 blueRC processor specific definitions
 (different register names, buffer sizes, disable features, etc..)
 
 Carsten Bruns (carst.bruns@gmx.de)
 */

#ifndef PROCESSORS_H_
#define PROCESSORS_H_

#if defined (__AVR_ATmega8__)
#define TWI_AVAILABLE 1  //TWI availability
#define INCLUDE_BTM_CONFIG 1  //include BTM222 configuration
#define ENABLE_MODE_SELECTION 1  //activate mode selection
#define IMPLEMENT_EEPROM_COMMANDS 1  //implement eeprom commands
#define INCLUDE_ADC_SENSORS 1  //include ADC voltage sensors
//register names
#define UDR0 UDR
#define UBRR0L UBRRL
#define UCSR0B UCSRB
#define RXEN0 RXEN
#define TXEN0 TXEN
#define RXCIE0 RXCIE
#define TXCIE0 TXCIE
#define TIMSK0 TIMSK
#define TIMSK1 TIMSK
#define TCCR0B TCCR0
#define EICRA MCUCR
#define EIMSK GICR
#define USART_RX_vect USART_RXC_vect
#define USART_TX_vect USART_TXC_vect

//TWI pins and ports
#define I2C_DATA_PORT PINC
#define I2C_DATA_PIN 4
#define I2C_CLOCK_PORT PINC
#define I2C_CLOCK_PIN 5

//buffer sizes
#define COMMAND_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 128
#elif ((defined (__AVR_ATmega88__)) ||  (defined (__AVR_ATmega328P__)))
#define TWI_AVAILABLE 1  //TWI availability
#define INCLUDE_BTM_CONFIG 1  //include BTM222 configuration
#define ENABLE_MODE_SELECTION 1  //activate mode selection
#define IMPLEMENT_EEPROM_COMMANDS 1  //implement eeprom commands
#define INCLUDE_ADC_SENSORS 1  //include ADC voltage sensors
//TWI pins and ports
#define I2C_DATA_PORT PINC
#define I2C_DATA_PIN 4
#define I2C_CLOCK_PORT PINC
#define I2C_CLOCK_PIN 5

//buffer sizes
#define COMMAND_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 128
#elif (defined (__AVR_ATmega324P__))
#define TWI_AVAILABLE 1  //TWI availability
#define INCLUDE_BTM_CONFIG 1  //include BTM222 configuration
#define ENABLE_MODE_SELECTION 1  //activate mode selection
#define IMPLEMENT_EEPROM_COMMANDS 1  //implement eeprom commands
#define INCLUDE_ADC_SENSORS 1  //include ADC voltage sensors
//TWI pins and ports
#define I2C_DATA_PORT PINC
#define I2C_DATA_PIN 1
#define I2C_CLOCK_PORT PINC
#define I2C_CLOCK_PIN 0

//register names
#define USART_RX_vect USART0_RX_vect
#define USART_TX_vect USART0_TX_vect

//buffer sizes
#define COMMAND_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 128
#elif defined (__AVR_AT90S2313__)
#define TWI_AVAILABLE 0  //TWI availability
#define INCLUDE_BTM_CONFIG 0  //include BTM222 configuration
#define ENABLE_MODE_SELECTION 0  //activate mode selection
#define IMPLEMENT_EEPROM_COMMANDS 0  //implement eeprom commands
#define INCLUDE_ADC_SENSORS 0  //include ADC voltage sensors
//register names
#define UDR0 UDR
#define UBRR0L UBRR
#define UCSR0B UCR
#define RXEN0 RXEN
#define TXEN0 TXEN
#define RXCIE0 RXCIE
#define TXCIE0 TXCIE
#define TIMSK0 TIMSK
#define TIMSK1 TIMSK
#define TCCR0B TCCR0
#define EICRA MCUCR
#define EIMSK GICR
#define USART_RX_vect UART_RX_vect
#define USART_TX_vect UART_TX_vect
#define TIMER0_OVF_vect TIMER0_OVF0_vect
#define TIMER1_OVF_vect TIMER1_OVF1_vect

//buffer sizes
#define COMMAND_BUFFER_SIZE 26
#define SEND_BUFFER_SIZE 2  //ATTENTION: NO SEND BUFFER, NEVER SEND STH. WITH THIS DEVICE!!
#else
# error "The selected device is not supported by Interface-library!"
#endif

#endif /* PROCESSORS_H_ */
