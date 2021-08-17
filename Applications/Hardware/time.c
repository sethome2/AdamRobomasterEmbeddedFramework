#include "time.h"

uint32_t Get_sys_time_ms(void)
{
	return HAL_GetTick();
}

float Get_sys_time_s(void)
{
	return HAL_GetTick() / 1000.0f;
}

//end of file
