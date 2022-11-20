/**
 * @file cap_ctl.h
 * @author sethome
 * @brief 与超级电容相关的控制
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define __CAP_H__
#ifdef __CAP_H__

#include "stdint.h"

typedef struct 
{
	uint8_t set_max_power;
	float remain_vol;
	float prediect_energy; // 0 - 100%
}cap_t;

extern cap_t cap;

void cap_handle_message(uint8_t data[8]);
int cap_set_power(uint8_t set);
float cap_get_remain_vol(void);
float cap_get_predict_energy(void);

#endif
