/**
 * @file GPIO_output.h
 * @author sethome 
 * @brief gpio输出头文件
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define __GPIO_OUTPUT_H__
#ifdef __GPIO_OUTPUT_H__

#include "struct_typedef.h"

void set_PIN_bool(uint8_t channel, uint8_t bool_v); //设置PIN引脚的开关状态

#endif 
