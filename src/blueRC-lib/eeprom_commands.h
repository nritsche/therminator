/*
 blueRC library for performing eeprom read and write actions header file

 Carsten Bruns (carst.bruns@gmx.de)
 */

#include <avr/io.h>
#include <avr/eeprom.h>

#ifndef EEPROM_COMMANDS_H_
#define EEPROM_COMMANDS_H_

#define EES_BUFFER_SIZE COMMAND_BUFFER_SIZE  //data buffer size

#define EEPROM_PORT PINC //eeprom select pin and port definitions
#define EEPROM_PORT2 PORTC
#define EEPROM_DDR DDRC
#define EEPROM_PIN 3

void wait_for_interfaces(void);
void eeprom_read_command(uint8_t *data);
void eeprom_write_command(uint8_t *data);
uint8_t check_eeprom_command(uint8_t *data);

#endif /* EEPROM_COMMANDS_H_ */
