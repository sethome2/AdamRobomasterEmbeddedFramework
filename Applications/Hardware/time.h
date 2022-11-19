#define __TIME_H__
#ifdef __TIME_H__

#include "stm32f4xx_hal.h"

// 获取系统时间
uint32_t Get_sys_time_ms(void); // recommend
uint32_t Get_sys_time_us(void);

float Get_sys_time_s(void);

void TIM_count_100KHz(void);
#endif
// end of file
