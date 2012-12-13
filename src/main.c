#include "main.h"

int main (void)
{
	sei();
	lcdInit();

   lcdSetPos(1, 0);
   lcdWriteString("Guten Tag.");

	while(1);
}
