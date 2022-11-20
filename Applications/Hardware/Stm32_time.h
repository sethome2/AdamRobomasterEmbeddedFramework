/**
 * @file Stm32_time.h
 * @author sethome 
 * @brief STM32F4�Ŀ���ʱ�����
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define __STM32_TIME_H
#ifdef __STM32_TIME_H

#include "stm32f4xx_hal.h"
#include "stdint.h"

// ��ȡϵͳʱ��
uint32_t Get_sys_time_ms(void); // recommend
uint32_t Get_sys_time_us(void);

float Get_sys_time_s(void);

void TIM_count_100KHz(void);
#endif
// end of file
