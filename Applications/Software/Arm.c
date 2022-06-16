#include "Arm.h"
#include "CAN_receive&send.h"
#include "MYACTUATOR_RMD_X.h"
#include "pid.h"
#include "math.h"
#include "time.h"

#define X 0
#define Y 1

#define D1_MOTOR MOTOR_ID_1 //一号电机
#define D2_MOTOR MOTOR_ID_2 //二号电机
#define S_MOTOR CAN_1_5     // 6020末端执行机构

struct Arm_t Arm_status;

pid_t s_speed;
pid_t s_angle;

float s_offset=0;

void ARM_init()
{
  pid_set(&s_angle, 20, 0, 0, 35, 0);
  pid_set(&s_speed, 90, 0, 1, 5000, 0);

  // 初始化角度
  Arm_status.a_d1.offset = 64.0f; // offset 使其相对于地面水平
  Arm_status.a_d2.offset = 35.0f; // offset 使其相对于1d水平
	
	// ARM_set_GND_angle(-50,0);
	
	ARM_update();
}

void cal_d1_GND_angle()
{
	Arm_status.a_d1.GNDangle = Arm_status.a_d1.setAngle;

	Arm_status.a_d1.vector[Y] = sin(Arm_status.a_d1.GNDangle) * 0.4; // * L1 的长度 
	Arm_status.a_d1.vector[X] = cos(Arm_status.a_d1.GNDangle) * 0.4; // * L1 的长度 
}

void cal_d2_GND_angle()
{
	cal_d1_GND_angle();
	Arm_status.a_d2.GNDangle = Arm_status.a_d1.GNDangle - Arm_status.a_d2.setAngle;
}

void ARM_set_GND_angle(float d1,float d2)
{
	cal_d1_GND_angle();
	Arm_status.a_d1.setAngle = d1 - Arm_status.a_d1.GNDangle;
	
	cal_d2_GND_angle();
	Arm_status.a_d2.setAngle = -(d2 - Arm_status.a_d2.GNDangle);
}

void ARM_set_angle(float d1,float d2)
{
	Arm_status.a_d1.setAngle = d1;
	Arm_status.a_d2.setAngle = d2;
}

void ARM_set_MAN_angle(float d1,float d2)
{
	float add_d1 = 0,add_d2 = 0;
	
	if(d1 > 0)
		add_d1 = 0.5;
	else if (d1 < 0)
		add_d1 = -0.5;
	
	if(d2 > 0)
		add_d2 = 0.5;
	else if (d2 < 0)
		add_d2 = -0.5;
	
	Arm_status.a_d1.setAngle += add_d1;
	Arm_status.a_d2.setAngle += add_d2;
}

void ARM_set_s_offset(float set)
{
	s_offset = set;
}
float offset = 59.0f;

void ARM_update(void)
{
	// 6020 末端执行器
  decode_as_6020(S_MOTOR);
	
	Arm_status.a_s.setAngle = Arm_status.a_d2.GNDangle; // 根据d2 完成自动保持水平
	
	float single_round = get_motor_data(S_MOTOR).angle_cnt + s_offset + offset; // 垂直D2 水平校准系数
	if(single_round > 360)
		single_round -= ((int)single_round / 360) * 360.0f;
	else if(single_round < -360)
		single_round += ((int)-single_round / 360) * 360.0f;
		
	Arm_status.a_s.nowAngle = single_round;
	Arm_status.a_s.now_speed = get_motor_data(S_MOTOR).speed_rpm;
	
  Arm_status.a_s.cal_speed = pid_cal(&s_angle, Arm_status.a_s.nowAngle, Arm_status.a_s.setAngle);
	
  Arm_status.a_s.sendCurrrent = pid_cal(&s_speed, Arm_status.a_s.now_speed, Arm_status.a_s.cal_speed);
  
	set_motor(Arm_status.a_s.sendCurrrent, S_MOTOR); 
	
	// 脉塔电机
	cal_d1_GND_angle();
	cal_d2_GND_angle();
	 
	// 限制角度
	if(Arm_status.a_d1.setAngle >= 150)
		Arm_status.a_d1.setAngle = 150;
	else if(Arm_status.a_d1.setAngle <= -50)
		Arm_status.a_d1.setAngle = -50;
	
	if(Arm_status.a_d2.setAngle >= 200)
		Arm_status.a_d2.setAngle = 200;
	else if(Arm_status.a_d2.setAngle <= -200)
		Arm_status.a_d2.setAngle = -200;
	
	RMD_write_angle(D1_MOTOR, Arm_status.a_d1.setAngle + Arm_status.a_d1.offset);
  RMD_write_angle(D2_MOTOR, Arm_status.a_d2.setAngle + Arm_status.a_d2.offset);
}
