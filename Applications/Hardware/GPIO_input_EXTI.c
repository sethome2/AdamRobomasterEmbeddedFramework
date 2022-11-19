/**
 * @file GPIO_input_EXTI.c
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2021-7-12
 *
 * @copyright Copyright (c) 2022
 *
 */
// IO7，启用上沿中断（低电平变高电平时中断）
#define __GPIO_INPUT_EXTI_C__
#ifdef __GPIO_INPUT_EXTI_C__

#include "main.h"
#include "time.h"
#include "LED_control.h"
#include "global_status.h"
#include "GPIO_input_EXTI.h"
#include "Encoder.h"

//重载HAL库中断函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == IO7_Pin) //为IO7被中断时
	{
		// update_Encoder(&Arm_encoder,read_IO6());
	}
	if (GPIO_Pin == KEY_Pin) // 用户按键被按下时
	{
		static uint32_t last_time = 0;
		uint32_t now_time = Get_sys_time_ms();
		if (now_time - last_time < 200 || now_time > 1000 * 20)
		{
			last_time = now_time;
			return;
		}
		last_time = now_time;

		// 切换队伍
		if (Global_status.team == BLUE_TEAM)
			Global_status.team = RED_TEAM;
		else if (Global_status.team == RED_TEAM)
			Global_status.team = BLUE_TEAM;
	}
}

//读取IO6的电平状态
int read_IO6() //返回0/1
{
	return HAL_GPIO_ReadPin(IO6_GPIO_Port, IO6_Pin);
}

#endif
