/*
 blueRC UART- and TWI(I2C)-interface library definitions

 Carsten Bruns (carst.bruns@gmx.de)
 */

#ifndef INTERFACES_H_
#define INTERFACES_H_

#include <avr/io.h>
#include "Processors.h"  //include processor specific definitions

extern void new_data_received(uint8_t *data); //called when received a new package, have to be implemented in main program

//blueRC protocol byte definitions
#define STOP_BYTE 128
#define PING_ANSWER 130
#define FILTER_CHANGE_BYTE 192
#define PING_REQUEST 254
#define SERVO_BYTE 255
#define SERVO_CH_BYTE 250
#define SENSOR_BYTE 129
#define EEPROM_WRITE_TX 131
#define EEPROM_READ_TX 132
#define EEPROM_READ_ANSWER_TX 251
#define EEPROM_WRITE_RX 253
#define EEPROM_READ_RX 252
#define EEPROM_READ_ANSWER_RX 133

#define UART_SEND_RESET 100 //time after which UART status is reseted to not sending
#define TWI_SEND TWCR = TWCR | ((1<<TWINT) | (1<<TWSTA)) //sends a start on TWI interface
#define TWI_BUS_CHECKS_AFTER_STOP 10  //amount of bus checks after stop (hardware-bug)
#define FILTER_NONE 0  //filter type definitions
#define FILTER_TX 1
#define FILTER_RX 2

struct { //interfaces flags
	unsigned volatile UART_new_data :1; //new data received flags
	unsigned volatile TWI_new_data :1;
	unsigned UART_active_buffer :1; //flag for current active receive buffer (every interface has two independent buffers)
	unsigned TWI_active_buffer :1;
	unsigned volatile TWI_sending :1;
	unsigned filter_type :2;  //filter type flag
	unsigned tx_or_rx :1; //flag for transmitter(0) or receiver(1) system setting
} iflags_0;

struct { //UART low level mode flags (low level mode is for direct UART access, if sth. else then blueRC packages have to be received/sended)
	unsigned UART_LowLevel_mode :1; //flag for activating low level mode
	unsigned UART_LowLevel_slowdown :1; //flag to slow down UART output in low level mode
	unsigned volatile UART_LowLevel_new_data :1; //new data flag in low level mode
} iflags_1;

uint8_t volatile UART_sending;

//receive buffers for UART and TWI interfaces
uint8_t UART_last_command_0[COMMAND_BUFFER_SIZE];
uint8_t UART_last_command_1[COMMAND_BUFFER_SIZE];
uint8_t volatile UART_LowLevel_data;
uint8_t UART_receive_pos;
#if (TWI_AVAILABLE == 1)
uint8_t TWI_last_command_0[COMMAND_BUFFER_SIZE];
uint8_t TWI_last_command_1[COMMAND_BUFFER_SIZE];
uint8_t TWI_receive_pos;
#endif

//send buffers for UART and TWI interfaces, implemented as ring buffers
uint8_t UART_send_buffer[SEND_BUFFER_SIZE];
uint8_t UART_send_max_pos;  //last byte containing data in UART send buffer
uint8_t UART_send_pos;  //last sended byte in UART send buffer
#if (TWI_AVAILABLE == 1)
uint8_t TWI_send_buffer[SEND_BUFFER_SIZE];
uint8_t TWI_send_max_pos;  //last byte containing data in TWI send buffer
uint8_t TWI_send_pos;  //last sended byte in TWI send buffer

uint8_t TWI_open_checks; //amount of open bus checks after last stop on TWI (hardware bug)
#endif

void send(uint8_t *data);
void perform_twi_bus_checks(void);
void twi_send(uint8_t *data);
void uart_send(uint8_t *data);
void update_interfaces(void);
void set_filter(uint8_t filter);
void init_interfaces(uint8_t tx_or_rx);

#endif /* INTERFACES_H_ */
