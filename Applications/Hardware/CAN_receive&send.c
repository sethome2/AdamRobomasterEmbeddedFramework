/**
 * @file CAN_receive&send.c
 * @author sethome
 * @brief (CAN_motor & surper_cap) control & send_rev
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "CAN_receive&send.h"

//第三方驱动
#include "MYACTUATOR_RMD_X.h"

#include "math.h"
#include "cmsis_os.h"

//电机数据
motor_measure_t motor_data[22];

//CAN寄存器及控制器
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2; //定义原型在can.c文件

//电机数据读取
void get_motor_measure(motor_measure_t *ptr, uint8_t data[])
{
  (ptr)->last_ecd = (ptr)->ecd;
  (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);
  (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);
  (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);
  (ptr)->temperate = (data)[6];
}

//使用函数指针
//电机数据处理
void process_motor_data(motor_measure_t *motor_data)
{
  //计算总圈数
  if (motor_data->last_ecd > 6000 && motor_data->ecd < 3000)
    motor_data->ecd_cnt += ((ECD_MAX - motor_data->last_ecd) + motor_data->ecd);
  else if (motor_data->last_ecd < 3000 && motor_data->ecd > 6000)
    motor_data->ecd_cnt -= ((ECD_MAX - motor_data->ecd) + motor_data->last_ecd);
  else
    motor_data->ecd_cnt += (motor_data->ecd - motor_data->last_ecd);
}

//HAL库中断回调指针
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header; //CAN 数据指针
  uint8_t rx_data[8];            //获取到的数据

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //取得信息
	
	RMD_decode_reply(rx_header.StdId, rx_data);
	
  if (hcan == &hcan1) //CAN1/2判断
  {
    get_motor_measure(&motor_data[rx_header.StdId - CAN_ID1], rx_data);
    process_motor_data(&motor_data[rx_header.StdId - CAN_ID1]);
  }
  else
  {
    get_motor_measure(&motor_data[CAN_1_6020_7 + 1 + rx_header.StdId - CAN_ID1], rx_data);
    process_motor_data(&motor_data[CAN_1_6020_7 + 1 + rx_header.StdId - CAN_ID1]);
  }
}

//返回马达数据指针（还不够安全。。。）
motor_measure_t get_motor_data(can_id motorID) //获取马达数据
{
  motor_measure_t *target_data = &motor_data[motorID];
  return *target_data;
}

//设置马达电流
void set_motor(int16_t val, can_id motorID) //设定马达电流
{
  motor_data[motorID].set = val;
}

//CAN1发送电流
void CAN1_send_current() //发送电机控制电流
{
  uint8_t can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;

  //发送前4个
  tx_message.StdId = CAN_1_4_SIGN_ID;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;

  can_send_data[0] = (motor_data[CAN_1_1].set >> 8);
  can_send_data[1] = motor_data[CAN_1_1].set;

  can_send_data[2] = (motor_data[CAN_1_2].set >> 8);
  can_send_data[3] = motor_data[CAN_1_2].set;

  can_send_data[4] = (motor_data[CAN_1_3].set >> 8);
  can_send_data[5] = motor_data[CAN_1_3].set;

  can_send_data[6] = (motor_data[CAN_1_4].set >> 8);
  can_send_data[7] = motor_data[CAN_1_4].set;
  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);

  //发送后4个
  tx_message.StdId = CAN_5_8_SIGN_ID;

  can_send_data[0] = (motor_data[CAN_1_5].set >> 8);
  can_send_data[1] = motor_data[CAN_1_5].set;

  can_send_data[2] = (motor_data[CAN_1_6].set >> 8);
  can_send_data[3] = motor_data[CAN_1_6].set;

  can_send_data[4] = (motor_data[CAN_1_7].set >> 8);
  can_send_data[5] = motor_data[CAN_1_7].set;

  can_send_data[6] = (motor_data[CAN_1_8].set >> 8);
  can_send_data[7] = motor_data[CAN_1_8].set;
  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);

#ifdef USE_CAN_1_6020

  tx_message.StdId = CAN_6020_SIGN_ID;

  can_send_data[0] = (motor_data[CAN_1_6020_5].set >> 8);
  can_send_data[1] = motor_data[CAN_1_6020_5].set;

  can_send_data[2] = (motor_data[CAN_1_6020_6].set >> 8);
  can_send_data[3] = motor_data[CAN_1_6020_6].set;

  can_send_data[4] = (motor_data[CAN_1_6020_7].set >> 8);
  can_send_data[5] = motor_data[CAN_1_6020_7].set;

  can_send_data[6] = 0;
  can_send_data[7] = 0;

#ifdef USE_NOP_DELAY
  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
  {
  };
#endif
#ifdef USE_FREERTOS_DELAY
  osDelay(1); //延时1ms
#endif

  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
#endif
}

//CAN2 发送电流
void CAN2_send_current() //发送电机控制电流
{
  uint8_t can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;

  //发送前4个
  tx_message.StdId = CAN_1_4_SIGN_ID;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;

  can_send_data[0] = (motor_data[CAN_2_1].set >> 8);
  can_send_data[1] = motor_data[CAN_2_1].set;

  can_send_data[2] = (motor_data[CAN_2_2].set >> 8);
  can_send_data[3] = motor_data[CAN_2_2].set;

  can_send_data[4] = (motor_data[CAN_2_3].set >> 8);
  can_send_data[5] = motor_data[CAN_2_3].set;

  can_send_data[6] = (motor_data[CAN_2_4].set >> 8);
  can_send_data[7] = motor_data[CAN_2_4].set;
  HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);

  //发送后4个
  tx_message.StdId = CAN_5_8_SIGN_ID;

  can_send_data[0] = (motor_data[CAN_2_5].set >> 8);
  can_send_data[1] = motor_data[CAN_2_5].set;

  can_send_data[2] = (motor_data[CAN_2_6].set >> 8);
  can_send_data[3] = motor_data[CAN_2_6].set;

  can_send_data[4] = (motor_data[CAN_2_7].set >> 8);
  can_send_data[5] = motor_data[CAN_2_7].set;

  can_send_data[6] = (motor_data[CAN_2_8].set >> 8);
  can_send_data[7] = motor_data[CAN_2_8].set;
  HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);

#ifdef USE_CAN_2_6020
  tx_message.StdId = CAN_6020_SIGN_ID;

  can_send_data[0] = (motor_data[CAN_2_6020_5].set >> 8);
  can_send_data[1] = motor_data[CAN_2_6020_5].set;

  can_send_data[2] = (motor_data[CAN_2_6020_6].set >> 8);
  can_send_data[3] = motor_data[CAN_2_6020_6].set;

  can_send_data[4] = (motor_data[CAN_2_6020_7].set >> 8);
  can_send_data[5] = motor_data[CAN_2_6020_7].set;

  can_send_data[6] = 0;
  can_send_data[7] = 0;

#ifdef USE_NOP_DELAY
  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) == 0)
  {
  };
    // uint32_t delay = 250 * 168 / 4; //延时250ns
    // do
    // {
    //   __NOP();
    // } while (delay--);
#endif
#ifdef USE_FREERTOS_DELAY
  osDelay(1); //延时1ms
#endif
  HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
#endif
}
void decode_as_3508(can_id motorID)
{
  //计算出轴速度
  motor_data[motorID].round_speed  = motor_data[motorID].speed_rpm / 60.0f / M3508_P;
  //计算总角度
  motor_data[motorID].angle_cnt = motor_data[motorID].ecd_cnt / ECD_MAX / M3508_P * 360.00f;
}

void decode_as_2006(can_id motorID)
{
  //计算出轴速度
  motor_data[motorID].round_speed = motor_data[motorID].speed_rpm / 60.0f / M2006_P;
  //计算总角度
  motor_data[motorID].angle_cnt = motor_data[motorID].ecd_cnt / ECD_MAX / M2006_P * 360.00f;
}

void decode_as_6020(can_id motorID)
{
  //计算出轴速度
  motor_data[motorID].round_speed = motor_data[motorID].speed_rpm / ECD_MAX * 360.0f;
  //计算总角度
  motor_data[motorID].angle_cnt = motor_data[motorID].ecd_cnt / ECD_MAX * 360.00f;
}
