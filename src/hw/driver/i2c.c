#include "i2c.h"


#ifdef _USE_HW_I2C



typedef struct
{
	bool is_init;


	I2C_HandleTypeDef *p_hi2c;
} i2c_tbl_t;


i2c_tbl_t i2c_tbl[I2C_CH_MAX];


I2C_HandleTypeDef hi2c1;



bool i2cInit()
{
	for(int i = 0; i < I2C_CH_MAX; i++)
	{
		i2c_tbl[i].is_init = false;
		i2c_tbl[i].p_hi2c = NULL;
	}

	return true;
}

bool i2cBegin(uint8_t ch)
{
	bool ret = false;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;


	switch(ch)
	{
		case _DEF_I2C1:
			p_handle->Instance = I2C1;
			p_handle->Init.ClockSpeed = 100000;
			p_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
			p_handle->Init.OwnAddress1 = 0;
			p_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
			p_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
			p_handle->Init.OwnAddress2 = 0;
			p_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
			p_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;



		  if (HAL_I2C_Init(p_handle) != HAL_OK)
		  {
		    Error_Handler();
		  }

		break;
		default:
		break;
	}

	return ret;
}

uint32_t i2cWrite(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Master_Transmit(p_handle, dev_addr, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}

uint32_t i2cRead(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Master_Receive(p_handle, dev_addr, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}

	return ret;
}

uint32_t i2cMemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Mem_Write(p_handle, dev_addr, mem_addr, mem_addr_size, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}

uint32_t i2cMemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Mem_Read(p_handle, dev_addr, mem_addr, mem_addr_size, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}



void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}


#endif
