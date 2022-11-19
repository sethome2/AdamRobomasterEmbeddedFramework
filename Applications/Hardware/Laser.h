/**
 * @file Laser.h
 * @author sethome
 * @brief 
 * @version 0.1
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __LASER_H__
#define __LASER_H__

#include "stdint.h"

extern uint8_t Laser_status;

void Laser_init(void);
void Laser_set(uint8_t status);

#endif /* __LASER_H__ */
