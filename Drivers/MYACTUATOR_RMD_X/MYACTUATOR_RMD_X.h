/**
 * @file MYACTUATOR_RMD_X.h
 * @author sethome (you@domain.com)
 * @brief 脉塔智能的X系列电机驱动程序
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022 sethome
 *
 */

/* 我只适配了位置环
 * 但是你可以很轻易的适配三种模式
 * 我经可能留出了拓展空间，后续建议使用ID来快速控制各个马达
 * 
 * I only adapted the position loop command
 * But you can easily adapt the three modes
 * I left as much flexible code for expansion, and it is recommended to use ID to quickly control each motor in the future
 */

#define __MYACTUATOR_RMD_X_H__
#ifdef __MYACTUATOR_RMD_X_H__

#include "stdint.h"

typedef struct
{
	float location_p, location_i;
	float speed_p, speed_i;
	float current_p, current_i;

	int32_t ecd;

	int speed;
	int temp;
} RMD_X;

extern RMD_X RMD_motor[8];

typedef enum
{
	MOTOR_BEGIN_ID = 0x140 + 1, // 1-32往后延伸32个
	MOTOR_ID_1 = 0x141,
	MOTOR_ID_2 = 0x142,
	MOTOR_ID_3 = 0x143,
	MOTOR_ID_4 = 0x144,
	MOTOR_ID_5 = 0x145,
	MOTOR_ID_6 = 0x146,

	PID_SET = 0x31,

	READ_LOCATION = 0x60, //编码器

	WRITE_CURRENT = 0xA1,
	WRITE_LOCATION1 = 0xA3,
	WRITE_LOCATION2 = 0xA4,

	MOTOR_SHUTDOWN = 0x80,
	MOTOR_STOP = 0x81,
	MOTOR_RUN = 0x88,
} RMD_X_ID_CMD;

void RMD_config(RMD_X *motor, float location_p, float location_i, float speed_p, float speed_i, float current_p, float current_i); //设置电机
void RMD_setup(RMD_X *motor, RMD_X_ID_CMD ID);																					   //设置电机
void RMD_run(RMD_X_ID_CMD ID);																									   //启动电机
void RMD_write_current(RMD_X_ID_CMD ID, int16_t current);																		   //写入角度
void RMD_write_angle(RMD_X_ID_CMD ID, float angle);																				   //写入设定角度
void RMD_write_angle_with_speed(RMD_X_ID_CMD ID, uint16_t speed, float angle);													   //写入限制速度的角度控制
void RMD_decode_reply(uint16_t CAN_ID, uint8_t data[8]);																		   //解码信息

#endif // __MYACTUATOR_RMD_X_H__
