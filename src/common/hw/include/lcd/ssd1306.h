#ifndef SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_
#define SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_


#include "hw_def.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_SSD1306
#include "lcd.h"



bool ssd1306Init(void);
bool ssd1306DriverInit(lcd_driver_t *lcd_driver);






#endif


#ifdef __cplusplus
}
#endif
#endif /* SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_ */
