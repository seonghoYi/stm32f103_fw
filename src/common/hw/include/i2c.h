#ifndef SRC_COMMON_HW_INCLUDE_I2C_H_
#define SRC_COMMON_HW_INCLUDE_I2C_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_I2C
#define			I2C_CH_MAX			HW_I2C_CH_MAX





bool i2cInit();
bool i2cBegin(uint8_t ch, uint32_t freq_khz);
uint32_t i2cWrite(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size);
uint32_t i2cRead(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size);
uint32_t i2cMemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);
uint32_t i2cMemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_I2C_H_ */
