#include "GPIO_output.h"
#include "PWM_control.h"

#include "main.h"
#include "stm32f4xx_hal_gpio.h"

//�趨PIN�Ŀ�����
//IO1-8(6,7Ĭ��Ϊ���룬���޸Ĳ�����)
void set_PIN_bool(uint8_t channel, uint8_t bool_v) //����PIN���ŵĿ���״̬
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
