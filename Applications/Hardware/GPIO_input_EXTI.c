//2021-7-12 sethome 
//IO7�����������жϣ��͵�ƽ��ߵ�ƽʱ�жϣ�
#define __GPIO_INPUT_EXTI_C__
#ifdef __GPIO_INPUT_EXTI_C__

#include "GPIO_input_EXTI.h"
#include "main.h"
#include "Encoder.h"
#include "stm32f4xx_hal_gpio.h"
#include "LED_control.h"

//����HAL���жϺ���
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	if(GPIO_Pin == IO7_Pin)//ΪIO7���ж�ʱ
	{
		//update_Encoder(&Arm_encoder,read_IO6());
	}
	if(GPIO_Pin == KEY_Pin)// �û�����������ʱ
	{
		led_show(YELLOW);
	}
}

//��ȡIO6�ĵ�ƽ״̬
int read_IO6()//����0/1
{
	return HAL_GPIO_ReadPin(IO6_GPIO_Port,IO6_Pin);
}

#endif
