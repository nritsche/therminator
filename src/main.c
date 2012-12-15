#include "main.h"
#include <util/delay.h>

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("Guten Tag.");

	initSPI();
unsigned int i = 0;
		unsigned char pressure[1];
		while (1) {
			pressure[0] = get_pressure(0);
			if (pressure[0] == 0xBB) {
				lcdWriteString(pressure);
				lcdWriteString("i am BB");
			}
			lcdWriteString(".\n");
			_delay_ms(1000);
			i++;
			if (i == 6) {
				i = 0;
				lcdClear();
			}
		}
}
