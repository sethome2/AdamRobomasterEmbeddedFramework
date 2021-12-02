//2021-7-12 sethome 
//IO7为浮空输入，启用上沿中断（低电平变高电平时中断）
#define __GPIO_INPUT_EXTI_C__
#ifdef __GPIO_INPUT_EXTI_C__

#include "GPIO_input_EXTI.h"
#include "main.h"
#include "Encoder.h"
#include "stm32f4xx_hal_gpio.h"

//重载HAL库中断函数
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	if(GPIO_Pin == IO7_Pin)//为IO7被中断时
	{
		update_Encoder(&Arm_encoder,read_IO6());
	}
}

//读取IO6的电平状态
int read_IO6()//返回0/1
{
	return HAL_GPIO_ReadPin(IO6_GPIO_Port,IO6_Pin);
}

#endif
