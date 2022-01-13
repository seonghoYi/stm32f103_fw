#include <lcd/ssd1306.h>
#include "i2c.h"





#ifdef _USE_HW_SSD1306


#define _DEF_SSD1306						_DEF_I2C1
#define _DEF_SSD1306_I2C_ADDR		0x3C



bool ssd1306Reset(void);
bool setWidth(uint32_t x);
bool setHeight(uint32_t y);
uint32_t getWidth(void);
uint32_t getHeight(void);


bool ssd1306CommandWrite(uint8_t cmd)
{
	bool ret = false;
	if(i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x00, 1, &cmd, 1) != 0);
	{
		ret = true;
	}

	return ret;
}




bool ssd1306Init(void)
{
	ssd1306Reset();

	return true;
}


bool ssd1306DriverInit(lcd_driver_t *lcd_driver)
{
	lcd_driver->init 				= ssd1306Init;
	lcd_driver->reset 			= ssd1306Reset;
	lcd_driver->setWidth 		= setWidth;
	lcd_driver->setHeight 	= setHeight;
	lcd_driver->getWidth 		= getWidth;
	lcd_driver->getHeight 	= getHeight;

	return true;
}


bool ssd1306Reset(void)
{
	bool ret = true;

	ssd1306CommandWrite(0xA8);
	ssd1306CommandWrite(0x3F); // set mux ratio

	ssd1306CommandWrite(0xD3);
	ssd1306CommandWrite(0x00); // set display offset

	ssd1306CommandWrite(0x40); // set display start line

	ssd1306CommandWrite(0xA0); // set segment re-map

	ssd1306CommandWrite(0xC8); // set COM output scan direction

	ssd1306CommandWrite(0xDA); // set COM pins hardware configuration
	ssd1306CommandWrite(0x12);

	ssd1306CommandWrite(0x81);
	ssd1306CommandWrite(0x7F); // set contrast control

	ssd1306CommandWrite(0xA4); // disable entire display on

	ssd1306CommandWrite(0xA6); // set normal display

	ssd1306CommandWrite(0xD5); // set osc frequency
	ssd1306CommandWrite(0x80);

	ssd1306CommandWrite(0x8D); // enable charge pump regulator
	ssd1306CommandWrite(0x14);

	ssd1306CommandWrite(0xAF); // display on

	return ret;
}

bool setWidth(uint32_t x)
{

}

bool setHeight(uint32_t y)
{

}

uint32_t getWidth(void)
{

}

uint32_t getHeight(void)
{

}




#endif
