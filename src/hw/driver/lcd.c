#include "lcd.h"
#include "lcd/lcd_fonts.h"


#ifdef _USE_HW_LCD



#ifdef _USE_HW_SSD1306
#include "lcd/ssd1306.h"
uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / SSD1306_NUM_PAGE];
#endif


static lcd_driver_t lcd_driver;

static lcd_font_t *font = &font_07x10;



typedef struct
{
	uint32_t pos_x;
	uint32_t pos_y;
} lcd_pixel_t;


bool lcdInit()
{
	bool ret = false;

	ret = ssd1306DriverInit(&lcd_driver);

	lcd_driver.init();


	lcdFillScreen(BLACK);
	lcdUpdateFrame();


	//lcdPutchar(0, 8, 'F', WHITE);
	lcdPrintf(0, 0, WHITE, "Hello, world!");

	//lcdDrawLine(128, 0, 0, 64, WHITE);
	//lcdDrawTriangle(64, 8, 32, 40, 96, 40, WHITE);
	//lcdDrawRectangle(10, 10, 40, 50, WHITE);
	//lcdDrawCircle(64, 32, 20, WHITE);
	lcdUpdateFrame();

	return ret;
}




bool lcdUpdateFrame(void)
{
	bool ret = false;

	ret = lcd_driver.sendbuffer((uint8_t *)&ssd1306_buffer[0]);
	memset(&ssd1306_buffer[0], 0, sizeof(ssd1306_buffer));

	return ret;
}

void lcdDrawPixel(uint32_t x, uint32_t y, uint8_t color)
{
	if(x > SSD1306_WIDTH || y > SSD1306_HEIGHT) return;


	if (color)
	{
		ssd1306_buffer[x + SSD1306_WIDTH * (y / SSD1306_NUM_PAGE)] |= 1<<(y%8);
	}
	else
	{
		ssd1306_buffer[x + SSD1306_WIDTH * (y / SSD1306_NUM_PAGE)] &= ~(1<<(y%8));
	}


}

void lcdFillScreen(uint8_t color)
{
	for(int i = 0; i < SSD1306_WIDTH; i++)
	{
		for(int j = 0; j < SSD1306_HEIGHT; j++)
		{
			lcdDrawPixel(i, j, color);
		}
	}
}



void lcdPutchar(uint32_t x, uint32_t y, char ch, uint8_t color)
{
	if(ch < 0x20 || ch > 0x7E) return;

	uint32_t b;


	for(int i = 0; i < font->height; i++)
	{
		b = font->data[(ch-0x20) * font->height + i];
		for(int j = 0; j < font->width; j++)
		{

			if((b << j) & 0x8000)
			{
				lcdDrawPixel(x + j, y + i, color);
			}
		}
	}
}


void lcdPrintf(uint32_t x, uint32_t y, uint8_t color, const char *fmt, ...)
{
	va_list arg;
	va_start (arg, fmt);
	int32_t len;

	char print_buffer[256];

	uint32_t pre_x = x;

	len = vsnprintf(print_buffer, 255, fmt, arg);
	va_end (arg);


	for(int i = 0; i < len; i++)
	{
		lcdPutchar(x, y, print_buffer[i], color);
		x+=font->width;

		if(x + font->width > SSD1306_WIDTH)
		{
			x = pre_x;
			y += font->height;
		}
	}
}



void lcdDrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t color)
{

	int32_t delta_x = abs(x0 - x1);
	int32_t delta_y = abs(y0 - y1);

  int32_t sign_x = ((x0 < x1) ? 1 : -1);
  int32_t sign_y = ((y0 < y1) ? 1 : -1);

	int32_t error = delta_x - delta_y;
	int32_t error2;


	while((x0!=x1) || (y0!=y1))
	{
		lcdDrawPixel(x0, y0, color);
		error2 = error * 2;
		if(error2 > -delta_y)
		{
			error -= delta_y;
			x0 += sign_x;
		}

		if(error2 < delta_x)
		{
			error += delta_x;
			y0 += sign_y;
		}
	}
}


void lcdDrawTriangle(uint32_t x0, uint32_t y0,uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t color)
{

	lcdDrawLine(x0, y0, x1, y1, color);
	lcdDrawLine(x1, y1, x2, y2, color);
	lcdDrawLine(x2, y2, x0, y0, color);
}


void lcdDrawRectangle(uint32_t x, uint32_t y, uint32_t line_x, uint32_t line_y, uint8_t color)
{
	lcdDrawLine(x, y, x + line_x, y, color);
	lcdDrawLine(x + line_x, y, x + line_x, y + line_y, color);
	lcdDrawLine(x + line_x, y + line_y, x, y + line_y, color);
	lcdDrawLine(x, y + line_y, x, y, color);
}


void lcdDrawCircle(uint32_t par_x, uint32_t par_y, uint32_t par_r, uint8_t color)
{
	int32_t x = -par_r;
	int32_t y = 0;
	int32_t err = 2 - 2 * par_r;
	int32_t e2;

	if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT) {
		return;
	}

		do {
			lcdDrawPixel(par_x - x, par_y + y, color);
			lcdDrawPixel(par_x + x, par_y + y, color);
			lcdDrawPixel(par_x + x, par_y - y, color);
			lcdDrawPixel(par_x - x, par_y - y, color);
			e2 = err;
			if (e2 <= y)
			{
				y++;
				err = err + (y * 2 + 1);
				if(-x == y && e2 <= x)
				{
					e2 = 0;
				}
			}
			if(e2 > x)
			{
				x++;
				err = err + (x * 2 + 1);
			}
		} while(x <= 0);
}




#endif
