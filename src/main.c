#include "main.h"
#include <util/delay.h>

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("ich werde nie BB sein");

    init_baro();
	initSPI();
	unsigned int i = 0;
	char pressure[2];
	pressure[0] = 0x00;
	pressure[1] = 0x00;
	
	while (1) {
		pressure[0] = get_pressure(0);
		if (pressure[0] == 0xBB) {
			lcdWriteString("WHO_AM_I ");
			lcdWriteString(pressure);
			lcdWriteChar('\n');
		}
		else if (pressure[0] == 0xFF)
			lcdWriteString ("ERROR: pitot not 0 or 1\n");
		else if (pressure[0] == 0x1F)
			lcdWriteString("tried to sent while not clr_to_send\n");
		else if (pressure[0] == 0x00)
			lcdWriteString ("read 00 from SPDR0\n");
		else 
			lcdWriteString ("one case not thought of! check main.c\n");

		_delay_ms(1000);
		i++;
		if (i == 2) {
			i = 0;
			lcdClear();
		}
	}
}
