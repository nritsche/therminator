#include "main.h"
#include <util/delay.h>
#include "blueRC-lib/Interfaces.h"

/*
  test output of received servo data
 */
void lcdServoOut(void) {
	lcdClear();
	for (uint8_t ch = 0; ch < CHANNELS; ch++) {
		lcdWriteString("Servo ");
		lcdWriteChar(ch + '1');
		lcdWriteString(": ");
		char buf[6];
		itoa(channel[ch],buf,10);
		lcdWriteString(buf);
		lcdWriteChar('\n');
	}
}

void send_servo_data(uint16_t *positions) {
	uint8_t buffer[CHANNELS*2+30];
	uint8_t *b = buffer;

	*b = SERVO_CH_BYTE;
	b++;
	*b = FIRST_OUT_CH;
	b++;
	for (uint8_t i = 0; i < CHANNELS;i++) {
		*b = ((*positions / 128) & 0b01111111);
		b++;
		*b = (*positions & 0b01111111);
		positions++;
		b++;
	}
	*b = STOP_BYTE;

	send(buffer);
}

/*
 new blueRC data received
 */
void new_data_received(uint8_t *data) {
	if (*data == SERVO_BYTE || *data == SERVO_CH_BYTE) {
		uint8_t i;
		int8_t ch = 0;
		if (*data == SERVO_BYTE) {
			i = 1;
		} else {
			i = 2;
			ch -= (data[1] - 1);
		}

		while ((data[i] != STOP_BYTE) && (data[i + 1] != STOP_BYTE)
				&& ((i - 1) / 2 <= CHANNELS)) {
			if (ch >= 0) {
				channel[ch] = (data[i] * 128) + data[i + 1];
			}
			ch++;
			i += 2;
		}
		lcdServoOut(); //TODO: remove this test output
		channel[0] += (analog_sensor_values[ACC_X]-550)*50;
		send_servo_data(channel);
	}
}

int main(void) {
	sei();
	init_interfaces(1);  //initialize blueRC lib
	UCSR0B &= ~(1 << RXEN0); //disable UART which was activated by blueRC lib
	UCSR0B &= ~(1 << TXEN0);
	lcdInit();

	init_baro();
	initSPI();
	init_analog_sensors();
	lcdWriteString("Hi");

	while (1) {
		update_interfaces();
	}
}
