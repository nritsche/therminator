#include "baro.h"
#include "spi.h"

unsigned char get_pressure()
{
	return receive();
}
