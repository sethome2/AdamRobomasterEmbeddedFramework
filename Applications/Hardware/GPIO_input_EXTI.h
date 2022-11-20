/**
 * @file GPIO_input_EXTI.h
 * @author sethome
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define __GPIO_INPUT_EXTI_H__
#ifdef __GPIO_INPUT_EXTI_H__

#include "stm32f4xx_hal.h"

//重载HAL库中断函数
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin);

//读取IO6的电平状态
int read_IO6(void);//返回0/1
	
#endif
