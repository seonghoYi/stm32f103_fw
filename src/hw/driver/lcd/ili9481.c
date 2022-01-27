#include "lcd/ili9481.h"
#include "lcd/ili9481_regs.h"





#ifdef _USE_HW_ILI9481

#define NOP		asm volatile("NOP")


void delay_us(uint32_t us)
{
	if (us <= 0)
	{
		return;
	}

	for(int i = 0; i < us * 20; i++)
	{
		NOP;
	}
}




typedef struct
{
	GPIO_TypeDef *port;
	uint32_t 			pin;
	uint8_t 			mode;
	GPIO_PinState on_state;
	GPIO_PinState off_state;
	bool 					init_value;
} gpio_tbl_t;


const gpio_tbl_t gpio_tbl[ILI9481_GPIO_MAX_CH]=
{
		{GPIOA, GPIO_PIN_0, _DEF_OUTPUT, GPIO_PIN_RESET, GPIO_PIN_SET, _DEF_LOW},
		{GPIOA, GPIO_PIN_1, _DEF_OUTPUT, GPIO_PIN_RESET, GPIO_PIN_SET, _DEF_LOW},
		{GPIOA, GPIO_PIN_4, _DEF_OUTPUT, GPIO_PIN_RESET, GPIO_PIN_SET, _DEF_LOW},
		{GPIOB, GPIO_PIN_0, _DEF_OUTPUT, GPIO_PIN_RESET, GPIO_PIN_SET, _DEF_LOW},
		{GPIOC, GPIO_PIN_1, _DEF_OUTPUT, GPIO_PIN_RESET, GPIO_PIN_SET, _DEF_LOW},
		{GPIOA, GPIO_PIN_9, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOC, GPIO_PIN_7, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOA, GPIO_PIN_10, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOB, GPIO_PIN_3, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOB, GPIO_PIN_5, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOB, GPIO_PIN_4, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOB, GPIO_PIN_10, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
		{GPIOA, GPIO_PIN_8, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_LOW},
};




bool gpioPinMode(uint8_t ch, uint8_t mode)
{
	bool ret = true;


	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (ch >= ILI9481_GPIO_MAX_CH)
	{
		return false;
	}


	switch(mode)
	{
	case _DEF_INPUT:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		break;
	case _DEF_OUTPUT:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		break;
	}

  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pin = gpio_tbl[ch].pin;

  HAL_GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);

  return ret;
}


void gpioPinWrite(uint8_t ch, bool value)
{
	if (ch >= ILI9481_GPIO_MAX_CH)
	{
		return;
	}

	if(value)
	{
		HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
	}
	else
	{
		HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
	}
}



bool gpioPinRead(uint8_t ch)
{
	bool ret = false;

	if (ch >= ILI9481_GPIO_MAX_CH)
	{
		return false;
	}

	if (HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin) == gpio_tbl[ch].on_state)
	{
		ret = true;
	}
	return ret;
}




bool ili9481InitGPIO()
{
	bool ret = true;


	for(int i = 0; i < ILI9481_GPIO_MAX_CH; i++)
	{
		ret &= gpioPinMode(i, gpio_tbl[i].mode);
		gpioPinWrite(i, gpio_tbl[i].init_value);
	}

	return ret;
}


bool ili9481BusMode(uint8_t mode)
{
	bool ret = true;

	for(int i = _DEF_ILI9481_D0; i < ILI9481_GPIO_MAX_CH; i++)
	{
		ret &= gpioPinMode(i, mode);
	}

	return ret;
}

void ili9481WriteData(uint8_t data);
void ili9481WriteCommand(uint8_t com);
uint8_t ili9481ReadData(void);
bool ili9481Reset();
bool ili9481SetAddrWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
bool ili9481SetRotation(uint8_t r);

void ili9481SetPixel(uint32_t x, uint32_t y);




void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i,j;

  gpioPinWrite(_DEF_ILI9481_RS, _DEF_LOW);
  gpioPinWrite(_DEF_ILI9481_CS, _DEF_HIGH);

  l=l+x;
  ili9481SetAddrWindow(x, y, l, y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
  	ili9481WriteData(c);
  }
  gpioPinWrite(_DEF_ILI9481_CS, _DEF_LOW);
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i,j;

  gpioPinWrite(_DEF_ILI9481_RS, _DEF_LOW);
  gpioPinWrite(_DEF_ILI9481_CS, _DEF_HIGH);
  l=l+y;
  ili9481SetAddrWindow(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  {
  	ili9481WriteData(c);
  }
  gpioPinWrite(_DEF_ILI9481_CS, _DEF_LOW);
}


void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}



bool ili9481Init()
{
	bool ret = true;

	ret &= ili9481InitGPIO();
	ret &= ili9481Reset();


	for(int i = 0; i < ILI9481_WIDTH; i++)
	{
		ili9481SetPixel(i, 240);
	}

	return ret;
}



bool ili9481DriverInit(lcd_driver_t *lcd_driver)
{
	bool ret = true;
	lcd_driver->init 					= ili9481Init;
	lcd_driver->reset 				= ili9481Reset;
	lcd_driver->setWindow 		= ili9481SetAddrWindow;
	lcd_driver->getWidth;
	lcd_driver->getHeight;
	lcd_driver->setCallBack;
	lcd_driver->sendbuffer;

	return ret;
}



bool ili9481Reset()
{
	bool ret = true;
	gpioPinWrite(_DEF_ILI9481_RST, _DEF_LOW);
	delay(5);
	gpioPinWrite(_DEF_ILI9481_RST, _DEF_HIGH);
	delay(100);
	gpioPinWrite(_DEF_ILI9481_RST, _DEF_LOW);
	delay(5);
	gpioPinWrite(_DEF_ILI9481_RD, _DEF_LOW);
	delay(5);
	gpioPinWrite(_DEF_ILI9481_WR, _DEF_LOW);
	delay(5);
	gpioPinWrite(_DEF_ILI9481_CS, _DEF_HIGH);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_EXIT_SLEEP_MODE);
	delay(20);

	ili9481WriteCommand(ILI9481_COM_POWER_SETTING);
	delay(5);
	ili9481WriteData(0x07);
	delay(5);
	ili9481WriteData(0x42);
	delay(5);
	ili9481WriteData(0x18);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_VCOM_CONTROL);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x07);
	delay(5);
	ili9481WriteData(0x10);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_POWER_SET_FOR_NORMAL_MODE);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0x02);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_PANEL_DRIVING_SETTING);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x3B);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x02);
	delay(5);
	ili9481WriteData(0x11);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_FRAME_RATE_INVERSION_CONTROL);
	delay(5);
	ili9481WriteData(0x03);
	delay(5);

	/*
	ili9481WriteCommand(ILI9481_COM_SET_ADDRESS_MODE);
	delay(5);
	ili9481WriteData(0x0A);
	delay(5);
	*/

	ili9481SetRotation(2);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_PIXEL_FORMAT);
	delay(5);
	ili9481WriteData(0x55);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_COLUMN_ADDRESS);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0x3F);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_PAGE_ADDRESS);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0xE0);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_GAMMA_SETTING);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x32);
	delay(5);
	ili9481WriteData(0x36);
	delay(5);
	ili9481WriteData(0x45);
	delay(5);
	ili9481WriteData(0x06);
	delay(5);
	ili9481WriteData(0x16);
	delay(5);
	ili9481WriteData(0x37);
	delay(5);
	ili9481WriteData(0x75);
	delay(5);
	ili9481WriteData(0x77);
	delay(5);
	ili9481WriteData(0x54);
	delay(5);
	ili9481WriteData(0x0C);
	delay(5);
	ili9481WriteData(0x00);

	delay(120);


	ili9481WriteCommand(ILI9481_COM_SET_DISPLAY_ON);


	return ret;
}


void ili9481WriteData(uint8_t data)
{
	gpioPinWrite(_DEF_ILI9481_RS, _DEF_LOW);


	ili9481BusMode(_DEF_OUTPUT);
	gpioPinWrite(_DEF_ILI9481_D0, data & 0x0001);
	gpioPinWrite(_DEF_ILI9481_D1, data & 0x0002);
	gpioPinWrite(_DEF_ILI9481_D2, data & 0x0004);
	gpioPinWrite(_DEF_ILI9481_D3, data & 0x0008);
	gpioPinWrite(_DEF_ILI9481_D4, data & 0x0010);
	gpioPinWrite(_DEF_ILI9481_D5, data & 0x0020);
	gpioPinWrite(_DEF_ILI9481_D6, data & 0x0040);
	gpioPinWrite(_DEF_ILI9481_D7, data & 0x0080);

	gpioPinWrite(_DEF_ILI9481_WR, _DEF_HIGH);
	delay_us(1);
	gpioPinWrite(_DEF_ILI9481_WR, _DEF_LOW);
}


uint8_t ili9481ReadData()
{
	uint8_t ret = 0;

	gpioPinWrite(_DEF_ILI9481_RS, _DEF_LOW);

	gpioPinWrite(_DEF_ILI9481_RD, _DEF_HIGH);
	delay_us(1);


	ili9481BusMode(_DEF_INPUT);
	ret |= gpioPinRead(_DEF_ILI9481_D0) << 0x0000;
	ret |= gpioPinRead(_DEF_ILI9481_D1) << 0x0001;
	ret |= gpioPinRead(_DEF_ILI9481_D2) << 0x0002;
	ret |= gpioPinRead(_DEF_ILI9481_D3) << 0x0003;
	ret |= gpioPinRead(_DEF_ILI9481_D4) << 0x0004;
	ret |= gpioPinRead(_DEF_ILI9481_D5) << 0x0005;
	ret |= gpioPinRead(_DEF_ILI9481_D6) << 0x0006;
	ret |= gpioPinRead(_DEF_ILI9481_D7) << 0x0007;


	gpioPinWrite(_DEF_ILI9481_RD, _DEF_LOW);

	return ret;
}


void ili9481WriteCommand(uint8_t com)
{
	gpioPinWrite(_DEF_ILI9481_RS, _DEF_HIGH);


	ili9481BusMode(_DEF_OUTPUT);
	gpioPinWrite(_DEF_ILI9481_D0, com & 0x0001);
	gpioPinWrite(_DEF_ILI9481_D1, com & 0x0002);
	gpioPinWrite(_DEF_ILI9481_D2, com & 0x0004);
	gpioPinWrite(_DEF_ILI9481_D3, com & 0x0008);
	gpioPinWrite(_DEF_ILI9481_D4, com & 0x0010);
	gpioPinWrite(_DEF_ILI9481_D5, com & 0x0020);
	gpioPinWrite(_DEF_ILI9481_D6, com & 0x0040);
	gpioPinWrite(_DEF_ILI9481_D7, com & 0x0080);

	gpioPinWrite(_DEF_ILI9481_WR, _DEF_HIGH);
	delay_us(1);
	gpioPinWrite(_DEF_ILI9481_WR, _DEF_LOW);
}


bool ili9481SetAddrWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	ili9481WriteCommand(ILI9481_COM_SET_COLUMN_ADDRESS);
	ili9481WriteData(x >> 8);
	ili9481WriteData(x & 0xFF);
	ili9481WriteData((x + w - 1) >> 8);
	ili9481WriteData((x + w - 1) & 0xFF);
	ili9481WriteCommand(ILI9481_COM_SET_PAGE_ADDRESS);
	ili9481WriteData(y >> 8);
	ili9481WriteData(y & 0xFF);
	ili9481WriteData((y + h - 1) >> 8);
	ili9481WriteData((y + h - 1) & 0xFF);
	ili9481WriteCommand(ILI9481_COM_WRITE_MEMORY_START);
	return true;
}


bool ili9481SetRotation(uint8_t r)
{
	bool ret = true;


	uint8_t com = 0x00;

	//0x20: page/column order
	//0x40: column address order
	//0x80: page address order
	//0x08: RGB order

	switch(r)
	{
	case 0:
		com = 0x80;
		break;
	case 1:
		com = 0x80 | 0x40 | 0x20;
		break;
	case 2:
		com = 0x40;
		break;
	case 3:
		com = 0x20;
		break;
	}

	ili9481WriteCommand(ILI9481_COM_SET_ADDRESS_MODE);
	delay(5);
	ili9481WriteData(com | 0x08);

	return ret;
}


void ili9481SetPixel(uint32_t x, uint32_t y)
{
	gpioPinWrite(_DEF_ILI9481_CS, _DEF_HIGH);
	ili9481SetAddrWindow(x, y, 1, 1);
	ili9481WriteData(0xF8);
	ili9481WriteData(0x00);
	gpioPinWrite(_DEF_ILI9481_CS, _DEF_LOW);
}












#endif
