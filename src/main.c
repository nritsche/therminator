#include "main.h"

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("Guten Tag.");

	initSPI();

	while(1) {
		unsigned char pressure[1];
		pressure[0] = get_pressure(0);
		lcdWriteString(pressure);
	}
}
