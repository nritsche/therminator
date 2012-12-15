#include "main.h"
#include <util/delay.h>

uint8_t convertToAscii(uint8_t digit) {
	if (digit < 10)
		return digit+48;
	else
		return digit+55;
}

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("ich werde nie BB sein\n");

    init_baro();
	initSPI();
	unsigned int i = 0;
	unsigned char pressure[2];
	pressure[0] = 0x00;
	pressure[1] = 0x00;
	
	while (1) {
		pressure[0] = get_pressure(0);
		lcdWriteString("read 0x");
		uint8_t tmp = pressure[0] / 16;
		lcdWriteChar(convertToAscii(tmp));
		tmp = pressure[0] % 16;
		lcdWriteChar(convertToAscii(tmp));
		lcdWriteChar('\n');

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

		_delay_ms(50);
		i++;
		if (i == 3) {
			i = 0;
			lcdClear();
		}
	}
}
