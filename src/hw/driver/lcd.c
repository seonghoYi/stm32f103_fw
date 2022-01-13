#include "lcd.h"
#include "lcd/ssd1306.h"


#ifdef _USE_HW_LCD


lcd_driver_t lcd_driver;




bool lcdInit()
{
	bool ret = false;

	ret = ssd1306DriverInit(&lcd_driver);

	lcd_driver.init();
	return ret;
}








#endif
