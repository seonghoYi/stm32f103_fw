#ifndef SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_
#define SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_

#include "hw_def.h"

#ifdef _USE_HW_ILI9481
#define 			 ILI9481_WIDTH						HW_ILI9481_WIDTH
#define				 ILI9481_HEIGHT						HW_ILI9481_HEIGHT
#define 			 ILI9481_GPIO_MAX_CH			HW_ILI9481_GPIO_MAX_CH


#include "lcd.h"


#define _DEF_ILI9481_RD			0
#define _DEF_ILI9481_WR			1
#define _DEF_ILI9481_RS			2
#define _DEF_ILI9481_CS			3
#define _DEF_ILI9481_RST		4
#define _DEF_ILI9481_D0			5
#define _DEF_ILI9481_D1			6
#define _DEF_ILI9481_D2			7
#define _DEF_ILI9481_D3			8
#define _DEF_ILI9481_D4			9
#define _DEF_ILI9481_D5			10
#define _DEF_ILI9481_D6			11
#define _DEF_ILI9481_D7			12



bool ili9481Init(void);
bool ili9481DriverInit(lcd_driver_t *lcd_driver);









#endif
#endif /* SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_ */
