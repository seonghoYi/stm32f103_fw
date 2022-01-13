#ifndef SRC_COMMON_HW_INCLUDE_LCD_H_
#define SRC_COMMON_HW_INCLUDE_LCD_H_

#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_LCD




typedef struct
{
	bool 			(*init)(void);
	bool 			(*reset)(void);
	bool 			(*setWidth)(uint32_t x);
	bool 			(*setHeight)(uint32_t y);
	uint32_t 	(*getWidth)(void);
	uint32_t 	(*getHeight)(void);

} lcd_driver_t;





bool lcdInit(void);









#endif

#ifdef __cplusplus
}
#endif



#endif /* SRC_COMMON_HW_INCLUDE_LCD_H_ */
