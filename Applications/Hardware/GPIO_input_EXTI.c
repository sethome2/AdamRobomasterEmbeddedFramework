//2021-7-12 sethome 
//IO7，启用上沿中断（低电平变高电平时中断）
#define __GPIO_INPUT_EXTI_C__
#ifdef __GPIO_INPUT_EXTI_C__

#include "global_status.h"
#include "GPIO_input_EXTI.h"
#include "main.h"
#include "Encoder.h"
#include "stm32f4xx_hal_gpio.h"
#include "LED_control.h"

//重载HAL库中断函数
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	if(GPIO_Pin == IO7_Pin)//为IO7被中断时
	{
		//update_Encoder(&Arm_encoder,read_IO6());
	}
	if(GPIO_Pin == KEY_Pin)// 用户按键被按下时
	{
		//led_show(YELLOW);
		if(Global_status.team == BLUE_TEAM)
			Global_status.team = RED_TEAM;
		else if(Global_status.team == RED_TEAM)
			Global_status.team = BLUE_TEAM;
	}
}

//读取IO6的电平状态
int read_IO6()//返回0/1
{
	return HAL_GPIO_ReadPin(IO6_GPIO_Port,IO6_Pin);
}

#endif
