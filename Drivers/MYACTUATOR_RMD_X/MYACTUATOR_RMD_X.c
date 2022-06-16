/*
 * @Author: sethome
 * @Date: 2022-02-27 23:20:14
 * @LastEditTime: 2022-03-01 21:18:24
 * @LastEditors: Please set LastEditors
 * @Description: 脉塔智能的示例驱动程序
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha 工程\Drivers\MYACTUATOR_RMD_X\MYACTUATOR_RMD_X.c
 */

#include "MYACTUATOR_RMD_X.h"
#include "CAN_receive&send.h"

RMD_X RMD_motor[8];

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2; //定义原型在can.c文件

//方便接下来的发送
void send2CAN(RMD_X_ID_CMD motorID, RMD_X_ID_CMD ID, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7)
{
	uint8_t can_send_data[8] = {ID, data1, data2, data3, data4, data5, data6, data7};
	static CAN_TxHeaderTypeDef tx_message;
	uint32_t send_mail_box;

	tx_message.StdId = motorID;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
}

//使用宏优化
#define TO_Q24(a) (uint32_t)(a * 16777216)
#define FROM_Q24(a) (flaot)((float)a / 16777216.0f)

void RMD_config(RMD_X *motor, float location_p, float location_i, float speed_p, float speed_i, float current_p, float current_i)
{
	motor->location_p = location_p;
	motor->location_i = location_i;
	motor->speed_p = speed_p;
	motor->speed_i = speed_i;
	motor->current_p = current_p;
	motor->current_i = current_i;
}

/**
 * @brief 设置PID参数
 * 
 * @param motor 马达结构体 
 * @param ID ID
 */
void RMD_setup(RMD_X *motor, RMD_X_ID_CMD ID)
{
	send2CAN(ID, PID_SET, 0, motor->location_p, motor->location_i, motor->speed_p, motor->speed_i, motor->current_p, motor->current_i);
}

void RMD_write_current(RMD_X_ID_CMD ID, int16_t current) // -2000 ~ 2000
{
	send2CAN(ID, WRITE_CURRENT, 0, 0, 0, *(uint8_t *)(&current), *(uint8_t *)(&current + 1), 0, 0);
}

void RMD_write_angle(RMD_X_ID_CMD ID, float angle)
{
	int32_t angleInt = angle * 100 * 36; // 减速比为36
	send2CAN(ID, WRITE_LOCATION1, 0, 0, 0, (uint8_t)(angleInt), (uint8_t)(angleInt >> 8), (uint8_t)(angleInt >> 16), (uint8_t)(angleInt >> 24));
}
void RMD_write_angle_with_speed(RMD_X_ID_CMD ID, uint16_t speed, float angle)
{
	int32_t angleInt = angle * 100 * 36; // 减速比为36
	send2CAN(ID, WRITE_LOCATION2, 0, *(uint8_t *)(&speed), *(uint8_t *)(&speed + 1), *(uint8_t *)(&angleInt), *(uint8_t *)(&angleInt + 1), *(uint8_t *)(&angleInt + 2), *(uint8_t *)(&angleInt + 3));
}

void RMD_run(RMD_X_ID_CMD ID)
{
	send2CAN(ID, MOTOR_RUN, 0, 0, 0, 0, 0, 0, 0);
}

void RMD_decode_reply(uint16_t CAN_ID, uint8_t data[8])
{
	if (CAN_ID <= 0x140 && CAN_ID >= 0x140 + 33)
		return;

	RMD_X *motor = &RMD_motor[CAN_ID - 0x141];

	switch (data[0])
	{
	case WRITE_LOCATION1:
		motor->ecd = data[6] + data[7] * 255;
		break;
	case WRITE_LOCATION2:
		motor->ecd = data[6] + data[7] * 255;
		break;

	case WRITE_CURRENT:
    motor->speed = data[4] + data[5] * 255;
		motor->ecd = data[6] + data[7] * 255;
		break;

	default:
		break;
	}
}
