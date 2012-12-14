#include "main.h"

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("Guten Tag.");

	initSPI();


	while(1) {
		lcdWriteString(get_pressure());
	}
}
