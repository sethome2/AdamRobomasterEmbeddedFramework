/**
 * @file LED_control.c
 * @author sethome (you@domain.com)
 * @brief 板载LED控制
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "LED_control.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim5;

//初始化LED
void led_init()
{
	HAL_TIM_Base_Start(&htim5);               //启动定时器5
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);  //启动每一个通道
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
}

//显示颜色
void led_show(uint32_t aRGB)
{
  static uint8_t alpha;
  static uint16_t red,green,blue;

  //位移获取颜色
  alpha = (aRGB & 0xFF000000) >> 24;
  red = ((aRGB & 0x00FF0000) >> 16) * alpha;
  green = ((aRGB & 0x0000FF00) >> 8) * alpha;
  blue = ((aRGB & 0x000000FF) >> 0) * alpha;

  //使能比较值
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);  
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}
