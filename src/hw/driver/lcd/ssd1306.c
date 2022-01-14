#include <lcd/ssd1306.h>
#include "i2c.h"





#ifdef _USE_HW_SSD1306


#define _DEF_SSD1306						_DEF_I2C1
#define _DEF_SSD1306_I2C_ADDR		(0x3C << 1)



static uint8_t buffer[128*64/8];




bool ssd1306Reset(void);
bool setWidth(uint32_t x);
bool setHeight(uint32_t y);
uint32_t getWidth(void);
uint32_t getHeight(void);


bool ssd1306CommandWrite(uint8_t cmd)
{
	bool ret = false;
	if(i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x00, 1, &cmd, 1) != 0)
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

	i2cBegin(_DEF_I2C1, 100);


	ssd1306CommandWrite(0xAE); //Off display

	ssd1306CommandWrite(0x20); //Set memory addressing mode
	ssd1306CommandWrite(0x00); //00b: Horizontal addressing mode 01b: Vertical addressing mode 10b: Page addressing mode

	ssd1306CommandWrite(0xB0); //Set page start address

	ssd1306CommandWrite(0xC8); //Set COM output direction

	ssd1306CommandWrite(0x00); //Set low column address
	ssd1306CommandWrite(0x10); //Set high column address

	ssd1306CommandWrite(0x40); //Set start line address

	ssd1306CommandWrite(0x81);
	ssd1306CommandWrite(0x7F); //Set contrast control

	ssd1306CommandWrite(0xA1); //Set segment re-map

	ssd1306CommandWrite(0xA6); //Set normal color

	ssd1306CommandWrite(0xA8); //Set multiplex ratio
	ssd1306CommandWrite(0x3F);

	ssd1306CommandWrite(0xA4); //Set display RAM content

	ssd1306CommandWrite(0xD3); //Set display offset
	ssd1306CommandWrite(0x00); //Offset

	ssd1306CommandWrite(0xD5); //Set display clock divide ratio
	ssd1306CommandWrite(0xF0); //Set divide ratio

	ssd1306CommandWrite(0xD9); //Set precharge period
	ssd1306CommandWrite(0x22);

	ssd1306CommandWrite(0xDA);
	ssd1306CommandWrite(0x12); //Set COM pins hardware config

	ssd1306CommandWrite(0xDB); //Set vcomh
	ssd1306CommandWrite(0x20); //0.77vcc

	ssd1306CommandWrite(0x8D); //Set DC-DC enable
	ssd1306CommandWrite(0x14);

	ssd1306CommandWrite(0xAF); //On display



	memset(buffer, 0xFF, sizeof(buffer));

	for (int i = 0; i < 8; i++)
	{
		i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x40, 1, &buffer[128 * i], 128);
	}


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
