#include "GPIO_output.h"
#include "PWM_control.h"

#include "main.h"
#include "stm32f4xx_hal_gpio.h"

//设定PIN的开关量
//IO1-8(6,7默认为输入，不修改不可行)
void set_PIN_bool(uint8_t channel, uint8_t bool_v) //设置PIN引脚的开关状态
{
	if(channel >= 1 && channel <= 5)
	{	
		if(bool_v == 1)
			set_PIN_PWM(channel,40000);
		else
			set_PIN_PWM(channel,0);
	}
	else
	{
		if(bool_v == 1)
			HAL_GPIO_WritePin(IO8_GPIO_Port,IO8_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(IO8_GPIO_Port,IO8_Pin,GPIO_PIN_RESET);
	}
}
