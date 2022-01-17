#include "ap.h"



void apInit()
{
	lcdBegin(_DEF_LCD1);

}


void apMain()
{
	while(1)
	{

		lcdPrintf(_DEF_LCD1, 0, 0, WHITE, "Hello, world!");
		lcdUpdateFrame(_DEF_LCD1);
	}
}
