#ifndef __CAN_RECEIVE__
#define __CAN_RECEIVE__

#include "struct_typedef.h"
#include "main.h"

#define CAN_1 1
#define CAN_2 2

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
	
	CAN_6020_SIGN_ID = 0x204,
	
} can_msg_id_e;

//请在这宏定义您的电机ID
//勿修改can_msg_id_e
//motorID
#define chassis_FR CAN_ID1
#define chassis_FL CAN_ID2
#define chassis_BL CAN_ID3
#define chassis_BR CAN_ID4
#define rotator1   CAN_ID5
#define rotator2   CAN_ID6
#define RollTrack  CAN_ID7
#define motor_8    CAN_ID8

//马达基础参数
#define ECD_MAX 8191  //编码器最大值
#define M3508_P 19.0f //M3508电机减速比
#define M2006_P 36.0f //M2006电机减速比

//待写 sethome
//马达类型修改 默认为M3508
//#define CAN1_0_M2006
//#define CAN1_1_M2006
//#define CAN1_2_M2006
//#define CAN1_3_M2006
//#define CAN1_4_M2006
//#define CAN1_5_M2006
//#define CAN1_6_M2006
//#define CAN1_7_M2006

//#define CAN2_0_M2006
//#define CAN2_1_M2006
//#define CAN2_2_M2006
//#define CAN2_3_M2006
//#define CAN2_4_M2006
//#define CAN2_5_M2006
//#define CAN2_6_M2006
//#define CAN2_7_M2006

//rm motor data
typedef struct
{
  int16_t set_current; //设定的电流

  uint16_t ecd;          //编码器
  int16_t speed_rpm;     //转速
  int16_t given_current; //电调给的电流
  uint8_t temperate;     //温度（获取不到）
  int16_t last_ecd;      //上一次编码器的数值

  long long ecd_cnt; //编码器计数器
  float angle_cnt;   //转过的总角度
  float round_speed; //出轴旋转速度

} motor_measure_t;

typedef struct
{
  int16_t set_voltage; //设定的电压

  uint16_t ecd;          //编码器
  int16_t speed_rpm;     //转速
  int16_t given_current; //电调给的电流
  uint8_t temperate;     //温度（获取不到）
  int16_t last_ecd;      //上一次编码器的数值

  long long ecd_cnt; //编码器计数器
  float angle_cnt;   //转过的总角度
  float round_speed; //出轴旋转速度

} motor_6020_measure_t;

//外部调用
void CAN1_send_current(void);                                        //发送电机控制电流
void CAN2_send_current(void);                                        //发送电机控制电流
void set_motor_current(int16_t current, uint8_t CAN_x, int motorID); //设定马达电流
motor_measure_t get_motor_data(uint8_t CAN_x, int motorID);          //获取马达信息

#endif
