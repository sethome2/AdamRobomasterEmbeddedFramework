/**
 * @file Encoder.c
 * @author sethome
 * @brief AB相编码器驱动
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */

#include "Encoder.h"
#include "Stm32_time.h"

//初始化编码器
void init_Encoder(Encode_t *target)
{
	target->count = 0;
}

//更新编码器变量
//当A相上沿时调用，给B相值
void update_Encoder(Encode_t *target,int B)
{
	if(B == 1)
		target->count++;
	else
		target->count--;
	
	//更新速度
	target->speed = (float)(1.0f / (float)(Get_sys_time_ms() - target->last_time));
	target->last_time = Get_sys_time_ms();
}
