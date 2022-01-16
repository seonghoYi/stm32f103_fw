#include <lcd/ssd1306.h>
#include "i2c.h"





#ifdef _USE_HW_SSD1306


#define _DEF_SSD1306						_DEF_I2C1
#define _DEF_SSD1306_I2C_ADDR		(0x3C << 1)






static bool is_init = false;


bool 			ssd1306Reset(void);
bool 			ssd1306SetWidth(uint32_t x);
bool 			ssd1306SetHeight(uint32_t y);
uint32_t 	ssd1306GetWidth(void);
uint32_t 	ssd1306GetHeight(void);
bool			ssd1306SendBuffer(uint8_t *buf);


bool ssd1306WriteCommand(uint8_t cmd)
{
	bool ret = false;
	if(i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x00, 1, &cmd, 1) != 0)
	{
		ret = true;
	}

	return ret;
}

bool ssd1306WriteData(uint8_t *buf)
{
	bool ret = false;
	for (int i = 0; i < 8; i++)
	{
		ssd1306WriteCommand(0xB0 + i);
		ssd1306WriteCommand(0x00);
		ssd1306WriteCommand(0x10);

		ret &= i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x40, 1, &buf[SSD1306_WIDTH * i], SSD1306_WIDTH);
	}
	return ret;
}




bool ssd1306Init(void)
{
	ssd1306Reset();
	is_init = true;
	return true;
}


bool ssd1306DriverInit(lcd_driver_t *lcd_driver)
{
	lcd_driver->init 				= ssd1306Init;
	lcd_driver->reset 			= ssd1306Reset;
	lcd_driver->setWidth 		= ssd1306SetWidth;
	lcd_driver->setHeight 	= ssd1306SetHeight;
	lcd_driver->getWidth 		= ssd1306GetWidth;
	lcd_driver->getHeight 	= ssd1306GetHeight;
	lcd_driver->sendbuffer 	= ssd1306SendBuffer;

	return true;
}


bool ssd1306Reset(void)
{
	bool ret = true;

	i2cBegin(_DEF_I2C1, 100);


	ssd1306WriteCommand(0xAE); //Off display

	ssd1306WriteCommand(0x20); //Set memory addressing mode
	ssd1306WriteCommand(0x02); //00b: Horizontal addressing mode 01b: Vertical addressing mode 10b: Page addressing mode

	ssd1306WriteCommand(0xB0); //Set page start address

	ssd1306WriteCommand(0xC8); //Set COM output direction

	ssd1306WriteCommand(0x00); //Set low column address
	ssd1306WriteCommand(0x10); //Set high column address

	ssd1306WriteCommand(0x40); //Set start line address

	ssd1306WriteCommand(0x81);
	ssd1306WriteCommand(0x7F); //Set contrast control

	ssd1306WriteCommand(0xA1); //Set segment re-map

	ssd1306WriteCommand(0xA6); //Set normal color

	ssd1306WriteCommand(0xA8); //Set multiplex ratio
	ssd1306WriteCommand(0x3F);

	ssd1306WriteCommand(0xA4); //Set display RAM content

	ssd1306WriteCommand(0xD3); //Set display offset
	ssd1306WriteCommand(0x00); //Offset

	ssd1306WriteCommand(0xD5); //Set display clock divide ratio
	ssd1306WriteCommand(0xF0); //Set divide ratio

	ssd1306WriteCommand(0xD9); //Set precharge period
	ssd1306WriteCommand(0x22);

	ssd1306WriteCommand(0xDA);
	ssd1306WriteCommand(0x12); //Set COM pins hardware config

	ssd1306WriteCommand(0xDB); //Set vcomh
	ssd1306WriteCommand(0x20); //0.77vcc

	ssd1306WriteCommand(0x8D); //Set DC-DC enable
	ssd1306WriteCommand(0x14);

	ssd1306WriteCommand(0xAF); //On display




	return ret;
}

bool ssd1306SetWidth(uint32_t x)
{

}


bool ssd1306SetHeight(uint32_t y)
{

}

uint32_t ssd1306GetWidth(void)
{

}

uint32_t ssd1306GetHeight(void)
{

}

bool ssd1306SendBuffer(uint8_t *buf)
{
	return ssd1306WriteData(buf);
}




#endif
