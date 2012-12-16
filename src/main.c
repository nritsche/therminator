#include "main.h"
#include <util/delay.h>
#include "AnalogSensors.h"

uint8_t convertToAscii(uint8_t digit) {
	if (digit < 10)
		return digit+48;
	else
		return digit+55;
}

void print_12bit_value(uint16_t value) {
	uint8_t tmp = value / 256;
	lcdWriteChar(convertToAscii(tmp));
	tmp = value % 256;
	tmp /= 16;
	lcdWriteChar(convertToAscii(tmp));
	tmp = value % 16;
	lcdWriteChar(convertToAscii(tmp));
	lcdWriteChar('\n');
}

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("ich werde nie BB sein\n");

    init_baro();
	initSPI();
	init_analog_sensors();
	unsigned int i = 0;
	unsigned char pressure[2];
	pressure[0] = 0x00;
	pressure[1] = 0x00;
	
	while (1) {
		/*pressure[0] = get_pressure(0);
		lcdWriteString("read 0x");
		uint8_t tmp = pressure[0] / 16;
		lcdWriteChar(convertToAscii(tmp));
		tmp = pressure[0] % 16;
		lcdWriteChar(convertToAscii(tmp));
		lcdWriteChar('\n');*/

		/*if (pressure[0] == 0xBB) {
			lcdWriteString("WHO_AM_I ");
			lcdWriteString(pressure);
			lcdWriteChar('\n');
		}
		else if (pressure[0] == 0xFF)
			lcdWriteString ("ERROR: pit not 0 or 1\n");
		else if (pressure[0] == 0x1F)
			lcdWriteString("try snd while !clr_to_send\n");
		else if (pressure[0] == 0x00)
			lcdWriteString ("read 00 from SPDR0\n");
		else 
			lcdWriteString ("check main.c\n");*/

		//_delay_ms(50);

		lcdWriteString("ACC_X: 0x");
		print_12bit_value(analog_sensor_values[ACC_X]);
		lcdWriteString("ACC_Y: 0x");
		print_12bit_value(analog_sensor_values[ACC_Y]);
		lcdWriteString("ACC_Z: 0x");
		print_12bit_value(analog_sensor_values[ACC_Z]);
		lcdWriteString("GYR_X: 0x");
		print_12bit_value(analog_sensor_values[GYR_X]);
		lcdWriteString("GYR_Y: 0x");
		print_12bit_value(analog_sensor_values[GYR_Y]);
		lcdWriteString("GYR_Z: 0x");
		print_12bit_value(analog_sensor_values[GYR_Z]);
		_delay_ms(200);

		i++;
		if (i == 1) {
			i = 0;
			lcdClear();
		}
	}
}
