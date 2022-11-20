/**
 * @file time.c
 * @author sethome
 * @brief STM32F4的开机时间计算
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Stm32_time.h"
#include "stm32f4xx_hal.h"

uint32_t Get_sys_time_ms(void)
{
	return HAL_GetTick();
}

float Get_sys_time_s(void)
{
	return HAL_GetTick() / 1000.0f;
}

uint32_t cnt = 0;
void TIM_count_100KHz(void)
{
	cnt+=10;
}
uint32_t Get_sys_time_us(void)
{
	return cnt;
}

//end of file
