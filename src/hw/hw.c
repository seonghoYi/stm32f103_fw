#include "hw.h"




void hwInit()
{
	bspInit();
	i2cInit();
	lcdInit();
}
