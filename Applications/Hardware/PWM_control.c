/**
 * @file PWM_control.c
 * @author sethome (you@domain.com)
 * @brief PWM控制
 * 		  PS: IO1-8(6,7默认为输入，不修改不可行)
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "PWM_control.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

//定时器变量
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;

//初始化
void PWM_servo_control_init()
{
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim8);

	//从IO1-IO8
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	// HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	// HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}

//初始化snail电机
void PWM_snail_init(uint8_t channel)
{
	set_PIN_PWM(channel, 1200);
}
void PWM_snaill_set(uint8_t channel, uint16_t CCR)
{
	set_PIN_PWM(channel, CCR);
}
void PWM_snaill_change_vector(uint8_t channel)
{
	set_PIN_PWM(channel, 2000);
	HAL_Delay(10);
	set_PIN_PWM(channel, 20000);
}

//设定舵机角度
void set_servo_angle(uint8_t channel, float angle) //统一给180舵机
{
	uint16_t CCR = (1500.0 / 180.0) * angle + 500;
	switch (channel)
	{
	case PIN_1:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, CCR);
		break;
	case PIN_2:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, CCR);
		break;
	case PIN_3:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, CCR);
		break;
	case PIN_4:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, CCR);
		break;
	case PIN_5:
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, CCR);
		break; /*
	 case PIN_6:
		 __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, CCR);
		 break;
	 case PIN_7:
		 __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, CCR);
		 break;*/
	}
}

//设定PWM的占空比
void set_PIN_PWM(uint8_t channel, uint16_t CCR)
{
	switch (channel)
	{
	case PIN_1:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, CCR);
		break;
	case PIN_2:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, CCR);
		break;
	case PIN_3:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, CCR);
		break;
	case PIN_4:
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, CCR);
		break;
	case PIN_5:
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, CCR);
		break; /*
	 case PIN_6:
		 __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, CCR);
		 break;
	 case PIN_7:
		 __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, CCR);
		 break;*/
	}
}
// end of file
