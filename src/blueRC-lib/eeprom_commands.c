/*
 blueRC library for performing eeprom read and write actions
 
 Carsten Bruns (carst.bruns@gmx.de)
 */

#include "eeprom_commands.h"
#include "Interfaces.h"

/*
 waits till the interfaces are not sending anymore, blocks the program for this time!
 */
void wait_for_interfaces(void) {
	uint8_t s = 0;
	while (UART_sending != 0 || iflags_0.TWI_sending != 0) {
		s++;
		if (UART_sending > 0 && s > 15) {
			s = 0;
			perform_twi_bus_checks();
			UART_sending--;
		}
	}
}

/*
 answers a eeprom read command
 
 @param data the eeprom read command
 */
void eeprom_read_command(uint8_t *data) {
	uint8_t ees_data[EES_BUFFER_SIZE];
	if (iflags_0.tx_or_rx == 0) { //set right answer command
		ees_data[0] = EEPROM_READ_ANSWER_TX;
	} else {
		ees_data[0] = EEPROM_READ_ANSWER_RX;
	}
	uint16_t adr = data[1] * 128 + data[2];
	uint16_t end_adr = data[3] * 128 + data[4];
	if (end_adr == 0) { //if length 0 read the whole eeprom
		end_adr = E2END;
	} else {
		end_adr += adr - 1;
	}
	if (end_adr > E2END) {
		end_adr = E2END;
	}
	while (adr <= end_adr) { //more bytes to send
		uint8_t i;
		ees_data[1] = adr / 128;
		ees_data[2] = adr % 128;
		for (i = 2; (adr <= end_adr) && (i < EES_BUFFER_SIZE / 2); i++) { //read eeprom data to send buffer
			eeprom_busy_wait();
			uint8_t read = eeprom_read_byte((uint8_t*) adr);
			ees_data[2 * i - 1] = (read & 0b11110000) / 16;
			ees_data[2 * i] = read & 0b00001111;
			adr++;
		}
		ees_data[2 * i - 1] = STOP_BYTE;
		wait_for_interfaces();
		send(ees_data); //send the answer package
	}
}

/*
 answers a eeprom write command
 
 @param data the eeprom write command
 */
void eeprom_write_command(uint8_t *data) {
	uint16_t adr = data[1] * 128 + data[2];
	data += 3;
	while (*data != STOP_BYTE) { //more bytes available in received data
		uint8_t write = *data * 16;
		data++;
		if (*data == STOP_BYTE) {
			break;
		}
		write += *data;
		data++;
		eeprom_busy_wait();
		eeprom_write_byte((uint8_t*) adr, write); //write byte to eeprom
		if (adr < E2END) {
			adr++;
		}
	}
}

/*
 checks if a command is a eeprom command and handles it if it should (eeprom pin low)
 
 @param data the command to check
 @return 1 if the package was a eeprom command and was handled, otherwise 0
 */
uint8_t check_eeprom_command(uint8_t *data) {
	uint8_t isEEPROMcommand = 0;
	if ((EEPROM_PORT & (1 << EEPROM_PIN)) == 0) {  //eeprom pin low
		if (iflags_0.tx_or_rx == 0) {  //transmitter system
			if (*data == EEPROM_WRITE_TX) {
				eeprom_write_command(data);
				isEEPROMcommand = 1;
			} else {
				if (*data == EEPROM_READ_TX) {
					eeprom_read_command(data);
					isEEPROMcommand = 1;
				}
			}
		} else { //receiver system
			if (*data == EEPROM_WRITE_RX) {
				eeprom_write_command(data);
				isEEPROMcommand = 1;
			} else {
				if (*data == EEPROM_READ_RX) {
					eeprom_read_command(data);
					isEEPROMcommand = 1;
				}
			}
		}
	}
	return isEEPROMcommand;
}
