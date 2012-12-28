/*
 blueRC UART- and TWI(I2C)-interface library

 Carsten Bruns (carst.bruns@gmx.de)
 */

#include "Interfaces.h"
#include "Frequencies.h" //include frequency dependent definitions
#if (IMPLEMENT_EEPROM_COMMANDS == 1)
#include "eeprom_commands.h" //include eeprom commands library
#else
uint8_t check_eeprom_command(uint8_t *data) { //implement empty check_eeprom_command method
	return 0;
}
#endif
#include <avr/interrupt.h>

EMPTY_INTERRUPT( __vector_default);

#if (TWI_AVAILABLE == 1)
/*
 sends data packages over TWI interface

 @param data the data to send
 */
void twi_send(uint8_t *data) {
	uint8_t i, f;

	i = 0;
	do {  //copy data package to send buffer
		f = TWI_send_max_pos + 1 + i;
		if (f > SEND_BUFFER_SIZE - 1) { //start at the beginning of the buffer if reached end (ring buffer)
			f -= SEND_BUFFER_SIZE;
		}
		TWI_send_buffer[f] = *data;
		data++;
		i++;
	}while (*(data-1) != STOP_BYTE);
	TWI_send_max_pos += i;  //set new last byte position
	if (TWI_send_max_pos > SEND_BUFFER_SIZE - 1) {
		TWI_send_max_pos -= SEND_BUFFER_SIZE;
	}

	if (iflags_0.TWI_sending == 0) { //if TWI is not sending start sending with bus checks (hardware-bug)
		iflags_0.TWI_sending = 1;
		TWI_open_checks = TWI_BUS_CHECKS_AFTER_STOP;
	}
}
#endif

/*
 checks if a command should be send to the UART or filtered

 @param command the command byte which should be checked
 @return 0 if the command should be send, otherwise 1
 */
uint8_t filter(uint8_t command) {
	if (iflags_0.filter_type == FILTER_NONE) { //no filter active
		return 0;
	} else if (iflags_0.filter_type == FILTER_TX) { //transmitter filter active
		if (command < 193) {
			return 1;
		} else {
			return 0;
		}
	} else if (iflags_0.filter_type == FILTER_RX) { //receiver filter active
		if (command < 192) {
			return 0;
		} else {
			return 1;
		}
	}
	return 1;
}

/*
 sends data packages over UART interface

 @param data the data to send
 */
void uart_send(uint8_t *data) {
	if (filter(*data) == 0) {
		uint8_t i, f;

		i = 0;
		do {  //copy data package to send buffer
			f = UART_send_max_pos + 1 + i;
			if (f > SEND_BUFFER_SIZE - 1) { //start at the beginning of the buffer if reached end (ring buffer)
				f -= SEND_BUFFER_SIZE;
			}
			UART_send_buffer[f] = *data;
			data++;
			i++;
		} while (*(data - 1) != STOP_BYTE);
		if (iflags_1.UART_LowLevel_mode == 0) { //set new last byte position in normal mode
			UART_send_max_pos += i;
		} else {
			UART_send_max_pos += i - 1; //in low level mode stop byte is not sended, subtract 1
		}
		if (UART_send_max_pos > SEND_BUFFER_SIZE - 1) {
			UART_send_max_pos -= SEND_BUFFER_SIZE;
		}

		if (UART_sending == 0) { //if UART is not sending start sending by writing first byte
			UART_sending = UART_SEND_RESET;
			UART_send_pos++;
			if (UART_send_pos > SEND_BUFFER_SIZE - 1) {
				UART_send_pos -= SEND_BUFFER_SIZE;
			}
			UDR0 = UART_send_buffer[UART_send_pos];
		}
	}
}

/*
 routine for processing new bytes on UART interface

 @param data the new byte
 */
void new_UART_data(uint8_t data) {
	if (iflags_1.UART_LowLevel_mode == 0) { //normal mode
		if (data > STOP_BYTE) {  //reset position if new start byte received
			UART_receive_pos = 0;
		}
		if (iflags_0.UART_active_buffer == 0) { //write to active buffer
			UART_last_command_0[UART_receive_pos] = data;
		} else {
			UART_last_command_1[UART_receive_pos] = data;
		}
		UART_receive_pos++;
		if (data == STOP_BYTE) { //on stop byte set new data flag and switch to other buffer
			UART_receive_pos = 0;
			iflags_0.UART_new_data = 1;
			iflags_0.UART_active_buffer = !(iflags_0.UART_active_buffer);
		} else {
			if (UART_receive_pos == COMMAND_BUFFER_SIZE) {  //buffer overflow!!
				UART_receive_pos = 0;
			}
		}
	} else { //in low level mode just set new data flag and let the main program handle everything
		UART_LowLevel_data = data;
		iflags_1.UART_LowLevel_new_data = 1;
	}
}

#if (TWI_AVAILABLE == 1)
/*
 routine for processing new bytes on TWI interface

 @param data the new byte
 */
void new_TWI_data(uint8_t data) {
	if (data > STOP_BYTE) {  //reset position if new start byte received
		TWI_receive_pos = 0;
	}
	if (iflags_0.TWI_active_buffer == 0) {  //write to active buffer
		TWI_last_command_0[TWI_receive_pos] = data;
	} else {
		TWI_last_command_1[TWI_receive_pos] = data;
	}
	TWI_receive_pos++;
	if (data == STOP_BYTE) { //on stop byte set new data flag and switch to other buffer
		TWI_receive_pos = 0;
		iflags_0.TWI_new_data = 1;
		iflags_0.TWI_active_buffer = !(iflags_0.TWI_active_buffer);
	} else {
		if (TWI_receive_pos == COMMAND_BUFFER_SIZE) {  //buffer overflow!!
			TWI_receive_pos = 0;
		}
	}
}
#endif

/*
 UART RX interrupt, calls new_UART_data
 */
ISR( USART_RX_vect) {
	new_UART_data(UDR0);
}

/*
 UART TX interrupt, checks if more data available in buffer and sends next byte
 */
ISR( USART_TX_vect) {
	UART_sending = UART_SEND_RESET;
	if (UART_send_pos != UART_send_max_pos) { //more bytes available
		UART_send_pos++;  //increase send buffer position
		if (UART_send_pos > SEND_BUFFER_SIZE - 1) { //start at the beginning of the buffer if reached end (ring buffer)
			UART_send_pos -= SEND_BUFFER_SIZE;
		}

		if (iflags_1.UART_LowLevel_mode && iflags_1.UART_LowLevel_slowdown) {
			uint8_t volatile c = 0;
			for (uint16_t a = 0; a < 10000; a++)
				if (c == 2)
					return;
		}
		UDR0 = UART_send_buffer[UART_send_pos];  //send next byte
	} else {  //no more data available
		UART_sending = 0;
	}
}

#if (TWI_AVAILABLE == 1)
/*
 TWI interrupt, handles all TWI events like new data received or sending
 */
ISR(TWI_vect) {
	uint8_t stat_code;

	stat_code = (TWSR & 0b11111000);  //mask prescaler-bits

	if (stat_code == 0x08) { //START sended successfully
		TWDR = 0b00000010;//set address 1
		TWCR = TWCR & (0xFF - (1 << TWSTA));//clear start bit
	} else {
		if ((stat_code == 0x18) || (stat_code == 0x20)) { //address sended
			TWI_send_pos++;//increase send buffer position
			if (TWI_send_pos > SEND_BUFFER_SIZE - 1) { //start at the beginning of the buffer if reached end (ring buffer)
				TWI_send_pos -= SEND_BUFFER_SIZE;
			}
			TWDR = TWI_send_buffer[TWI_send_pos];  //send data
		} else {
			if ((stat_code == 0x28) || (stat_code == 0x30)) { //data send successfully
				if (TWI_send_pos != TWI_send_max_pos) {  //send next data
					TWI_send_pos++;
					if (TWI_send_pos > SEND_BUFFER_SIZE - 1) {
						TWI_send_pos -= SEND_BUFFER_SIZE;
					}
					TWDR = TWI_send_buffer[TWI_send_pos];
				} else {  //no more data, send STOP
					TWCR = TWCR | (1 << TWSTO);
					iflags_0.TWI_sending = 0;
				}
			} else {
				if ((stat_code == 0x78) || (stat_code == 0x80) || (stat_code == 0x90)) { //received data
					new_TWI_data(TWDR);
				} else {
					if (stat_code == 0xA0) {  //STOP on TWI bus
						TWI_open_checks = TWI_BUS_CHECKS_AFTER_STOP;//check for free bus in software before next send action (hardware bug)
					}
				}
			}
		}
	}
	TWCR = TWCR | (1 << TWINT); //clear interrupt flag
}
#endif

/*
 sends data packages over all interfaces

 @param data the data to send
 */
void send(uint8_t *data) {
	uart_send(data);
#if (TWI_AVAILABLE == 1)
	twi_send(data);
#endif
}

/*
 checks a received package for special commands and passes it to the main program

 @param data the package to check
 @return 1 if the package should be forwarded over the other interfaces, otherwise 0
 */
uint8_t check_new_data_received(uint8_t *data) {
	uint8_t redirect = 1;
	if (*data == PING_REQUEST) {   //answer ping request
		uint8_t buffer[2];
		buffer[0] = PING_ANSWER;
		buffer[1] = STOP_BYTE;
		send(buffer);
	} else {
		if (*data == FILTER_CHANGE_BYTE) {  //handle filter change command
			redirect = 0;
			iflags_0.filter_type = *(data+1);
		} else {
			if (check_eeprom_command(data) == 0) { //check for eeprom commands
				new_data_received(data); //no special command, pass it to main program
			} else {
				redirect = 0;
			}
		}
	}
	return redirect;
}

/*
 updates interface library, have to be called periodically
 handles received data and package forwarding to other interfaces
 */
void update_interfaces(void) {
	if (UART_sending > 0) {
		UART_sending--;
	}
	if (iflags_0.UART_new_data == 1) { //new data on UART interface, check the data and forward to TWI
		iflags_0.UART_new_data = 0;
		if (iflags_0.UART_active_buffer == 0) {
			if (check_new_data_received(UART_last_command_1) != 0) {
#if (TWI_AVAILABLE == 1)
				twi_send(UART_last_command_1);
#endif
			}
		} else {
			if (check_new_data_received(UART_last_command_0) != 0) {
#if (TWI_AVAILABLE == 1)
				twi_send(UART_last_command_0);
#endif
			}
		}
	}

#if (TWI_AVAILABLE == 1)
	perform_twi_bus_checks();  //check if TWI bus is free (hardware bug)

	if (iflags_0.TWI_new_data == 1) { //new data on TWI interface, check the data and forward to UART
		iflags_0.TWI_new_data = 0;
		if (iflags_0.TWI_active_buffer == 0) {
			if (check_new_data_received(TWI_last_command_1) != 0) {
				if (iflags_1.UART_LowLevel_mode == 0) {
					uart_send(TWI_last_command_1);
				}
			}
		} else {
			if (check_new_data_received(TWI_last_command_0) != 0) {
				if (iflags_1.UART_LowLevel_mode == 0) {
					uart_send(TWI_last_command_0);
				}
			}
		}
	}
#endif
}

/*
 sets the filter to the given type

 @param filter the new filter type
 */
void set_filter(uint8_t filter) {
	iflags_0.filter_type = filter;
}

/*
 initializes the interfaces library

 @param tx_or_rx 0 for transmitter system, 1 for receiver system
 */
void init_interfaces(uint8_t tx_or_rx) {
#if (IMPLEMENT_EEPROM_COMMANDS == 1)
	EEPROM_DDR &= ~(1 << (EEPROM_PIN));  //eeprom select pin settings
	EEPROM_PORT2 |= (1 << (EEPROM_PIN));
#endif
	//initialize variables and flags
	iflags_0.tx_or_rx = tx_or_rx;
	iflags_0.UART_new_data = 0;
	iflags_0.TWI_new_data = 0;
	iflags_0.UART_active_buffer = 0;
	iflags_0.TWI_active_buffer = 0;
	UART_sending = 0;
	iflags_1.UART_LowLevel_mode = 0;
	iflags_1.UART_LowLevel_slowdown = 0;
	iflags_1.UART_LowLevel_new_data = 0;
	iflags_0.filter_type = FILTER_NONE;
	UART_receive_pos = 0;
	UART_send_max_pos = SEND_BUFFER_SIZE - 1;
	UART_send_pos = SEND_BUFFER_SIZE - 1;
#if (TWI_AVAILABLE == 1)
	TWI_receive_pos = 0;
	TWI_send_max_pos = SEND_BUFFER_SIZE - 1;
	TWI_send_pos = SEND_BUFFER_SIZE - 1;
	iflags_0.TWI_sending = 0;
	TWI_open_checks = TWI_BUS_CHECKS_AFTER_STOP;

	TWAR = 0b00000011;  //set slave-address to 0000001, activate general-call
	TWBR = TWI_BITRATE;//set bitrate (~50khz)
	TWSR = TWI_PRESCALER;//set TWI-prescaler
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);//activate TWI, TWI acknowledge and interrupt
#endif

	//initialize UART
	UBRR0L = UART_BAUDRATE;  //baudrate 19200
	UCSR0B |= (1 << RXEN0); //enable receiver
	UCSR0B |= (1 << TXEN0); //enable transmitter
	UCSR0B |= (1 << RXCIE0); //enable RX interrupt
	UCSR0B |= (1 << TXCIE0); //enable TX interrupt
}

#if (TWI_AVAILABLE == 1)
/*
 checks if the TWI bus is free by software
 this is needed because the Atmel has a hardware bug and sometimes not recognize a busy bus after a STOP condition
 */
void perform_twi_bus_checks( void) {
	if (iflags_0.TWI_sending && TWI_open_checks > 0) {
		if (((I2C_DATA_PORT & (1 << I2C_DATA_PIN)) == 0) || ((I2C_CLOCK_PORT & (1 << I2C_CLOCK_PIN)) == 0)) {
			TWI_open_checks = TWI_BUS_CHECKS_AFTER_STOP; //bus is busy
		} else {
			TWI_open_checks--;  //bus is free
		}
		if (TWI_open_checks == 0) {
			TWI_SEND;  //bus was free long enough, start sending now
		}
	}
}
#endif
