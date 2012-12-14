#include "baro.h"
#include "spi.h"

unsigned char get_pressure(unsigned short int pitot)
{
	return receive();
}
