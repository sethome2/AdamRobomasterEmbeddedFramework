/**
 * @file Laser.c
 * @author sethome
 * @brief 
 * @version 0.1
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Laser.h"

#include "stm32f407xx.h"

#include "main.h"

uint8_t Laser_status;

void Laser_init(void)
{

}

void Laser_set(uint8_t status)
{
    if (status)
    {
        HAL_GPIO_WritePin(Laser_GPIO_Port, Laser_Pin, GPIO_PIN_SET);
        Laser_status = 1;
    }
    else
    {
        HAL_GPIO_WritePin(Laser_GPIO_Port, Laser_Pin, GPIO_PIN_RESET);
        Laser_status = 0;
    }
}
