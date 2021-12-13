/**
 * @file CAN_receive&send.h
 * @author sethome
 * @brief 
 * @version 0.1
 * @date 2021-12-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CAN_RECEIVE_H__
#define __CAN_RECEIVE_H__

#include "struct_typedef.h"
#include "main.h"

//使用6020电机的ID5-7，添加其解析，一般不建议使用
#define USE_CAN_1_6020
#define USE_CAN_2_6020
//#define USE_FREERTOS_DELAY
#define USE_NOP_DELAY

/* CAN send and receive ID */
typedef enum
{
  CAN_1_4_SIGN_ID = 0x200,
  CAN_ID1 = 0x201,
  CAN_ID2 = 0x202,
  CAN_ID3 = 0x203,
  CAN_ID4 = 0x204,

  CAN_5_8_SIGN_ID = 0x1FF,
  CAN_ID5 = 0x205,
  CAN_ID6 = 0x206,
  CAN_ID7 = 0x207,
  CAN_ID8 = 0x208,

  CAN_6020_SIGN_ID = 0x2FF,
  CAN_6020_ID5 = 0x209,
  CAN_6020_ID6 = 0x210,
  CAN_6020_ID7 = 0x211,
} can_msg_id_e;

typedef enum
{
  CAN_1_1 = 0,
  CAN_1_2,
  CAN_1_3,
  CAN_1_4,
  CAN_1_5,
  CAN_1_6,
  CAN_1_7,
  CAN_1_8,
  CAN_1_6020_5,
  CAN_1_6020_6,
  CAN_1_6020_7,

  CAN_2_1,
  CAN_2_2,
  CAN_2_3,
  CAN_2_4,
  CAN_2_5,
  CAN_2_6,
  CAN_2_7,
  CAN_2_8,
  CAN_2_6020_5,
  CAN_2_6020_6,
  CAN_2_6020_7,
} can_id;

//请在这宏定义您的电机ID(废弃，直接在调用文件中定义吧)
//6020从 CAN_1_5(对应ID1) 到 CAN_1_6020_7（对应ID7）
//3508/2006从 CAN_1_1(对应ID1) 到 CAN_1_8（对应ID7）
//勿修改can_msg_id_e
#define motor_1_1 CAN_1_1
#define motor_1_2 CAN_1_2
#define motor_1_3 CAN_1_3
#define motor_1_4 CAN_1_4
#define motor_1_5 CAN_1_5
#define motor_1_6 CAN_1_6
#define motor_1_7 CAN_1_7
#define motor_1_8 CAN_1_8
#define M6020_1_5 CAN_1_6020_5
#define M6020_1_6 CAN_1_6020_6
#define M6020_1_7 CAN_1_6020_7

#define motor_2_1 CAN_2_1
#define motor_2_2 CAN_2_2
#define motor_2_3 CAN_2_3
#define motor_2_4 CAN_2_4
#define motor_2_5 CAN_2_5
#define motor_2_6 CAN_2_6
#define motor_2_7 CAN_2_7
#define motor_2_8 CAN_2_8
#define M6020_2_5 CAN_2_6020_5
#define M6020_2_6 CAN_2_6020_6
#define M6020_2_7 CAN_2_6020_7

//马达基础参数
#define ECD_MAX 8191       //编码器最大值
#define M3508_P 19.0f      //M3508电机减速比
#define M2006_P 36.0f      //M2006电机减速比
#define MAX_CURRENT 16384  //M2006+M3508最大电流 20A / MAX_CURRENT
#define MAX_6020_VOL 30000 //6020最大电压 24V / MAX_6020_VOL

//rm motor data
//notice:默认为3508电机
//3508/2006用的是电流，6020用的是电压
typedef struct
{
  int16_t set; //设定的电流 / 电压

  //原始数据
  uint16_t ecd;          //编码器数值
  int16_t speed_rpm;     //转速
  int16_t given_current; //电调给的电流
  uint8_t temperate;     //温度（获取不到）
  int16_t last_ecd;      //上一次编码器的数值

  //计算数据
  long long ecd_cnt; //编码器计数器
  float angle_cnt;   //转过的总角度 degree
  float round_speed; //出轴旋转速度 degree/s

} motor_measure_t;

//外部调用
void CAN1_send_current(void);                   //发送电机控制电流
void CAN2_send_current(void);                   //发送电机控制电流
void set_motor(int16_t val, can_id motorID);    //设定马达电流/电压
motor_measure_t get_motor_data(can_id motorID); //获取马达信息
//对马达信息二次解码（计算），使用部分计算数据前使用，由于编码器0点，可能在高速下不准确！！！
void decode_as_3508(can_id motorID);
void decode_as_2006(can_id motorID);
void decode_as_6020(can_id motorID);

#endif
