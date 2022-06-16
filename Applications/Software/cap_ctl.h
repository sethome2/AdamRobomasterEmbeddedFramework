#define __CAP_H__
#ifdef __CAP_H__

#include "stdint.h"

typedef struct 
{
	uint8_t set_max_power;
	float remain_vol;
	float prediect_energy; // 0 - 100%
	
	uint8_t cache_energy; // 从裁判系统获取缓冲能量
}cap_t;

extern cap_t cap;

void cap_handle_message(uint8_t data[8]);
void cap_update(void);
int cap_set_power(uint8_t set);
float cap_get_remain_vol(void);
float cap_get_predict_energy(void);
#endif
