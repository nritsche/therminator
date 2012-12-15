#include "main.h"

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("Guten Tag.");

	initSPI();

		unsigned char pressure[1];
		while (1) {
			pressure[0] = get_pressure(0);
			if (pressure[0] == 0xBB) {
				lcdWriteString(pressure);
				lcdWriteString("i am BB");
			}
		}
}
