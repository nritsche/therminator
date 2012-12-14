#include "baro.h"
#include "spi.h"

char *get_pressure()
{
	return receive();
}
