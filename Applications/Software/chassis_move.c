#include "chassis_move.h"
#include "math.h"
#include "CAN_receive&send.h"
#include "pid.h"
#include "UART_data_transmit.h"
#include "stdlib.h"
#include "stdio.h"

/* motorID 1 %++++++% 0
               ++++
               ++++
           2 %++++++% 3 */
//wheel conf
#define WHEEL_RADIUS 0.15240f	 //m
#define PI 3.1415926f

//car conf
#define ROLLER_DISTANCE 100    //mm  轴距
#define WHEELS_DISTANCE 100    //mm  轮距


struct chassis_status chassis;

//串口调试PID
#define DEBUG

//mm/s
float wheel_rpm[4];						 //马达速度（计算值）
float track_motor_rpm;

int16_t wheel_current[4];      //马达电流（pid计算值）
int16_t track_pid;
//限速
#define MAX_VX_SPEED 2.5  			 //m/s 
#define MAX_VY_SPEED 2.5
#define MAX_VR_SPEED 3
#define MAX_TRACK_SPEED 3

//加速度限制
#define ACC_VX       0.01       //m/s^2
#define ACC_VY       0.3   
#define ACC_VW       0xFF

//马达PID控制变量数组
pid_t motor_speed[4];
pid_t track_motor_speed;

//调试底盘马达PID使用
char buff[5] = {0,0,0,0,0};
uint8_t buff1[100];

//底盘初始化（仅针对4轮麦克纳姆轮）
void chassis_move_init()
{
	//马达PID设置初始化
	pid_set(&motor_speed[FR],8000,0,500,15000,0);
	pid_set(&motor_speed[FL],8000,0,500,15000,0);
	pid_set(&motor_speed[BL],8000,0,500,15000,0);
	pid_set(&motor_speed[BR],8000,0,500,15000,0);
	//储块马达PID设置初始化
	pid_set(&track_motor_speed,5000, 0, 20, 7000, 0);
}

//最大值限制
void val_limit(float *val,float MAX)
{
	if(fabs(*val) > MAX)
		if(*val > 0)
			*val = MAX;
		else
			*val = -MAX;
}

//限制变化
void change_limit(float last,float *now,float limit)
{
	float change = *now - last;
	if(fabs(change) > limit)
	{
		if(change > 0)
			*now = last + limit;
		else
			*now = last - limit;
	}
}

//计算底盘马达速度（注意不会发送）
void chassis_moto_speed_calc(float vx, float vy, float vw)
{
	//static float last_vx,last_vy,last_vw;//上次的速度
	
	//速度限制
  val_limit(&vx, MAX_VX_SPEED);
  val_limit(&vy, MAX_VY_SPEED);  
  val_limit(&vw, MAX_VR_SPEED);  
  
	chassis.speed.vx = vx;	chassis.speed.vy = vy;	chassis.speed.vw = vw;
	//加速度限制
	//change_limit(last_vx,&vx,ACC_VX);
	//change_limit(last_vy,&vy,ACC_VY);
	//change_limit(last_vw,&vw,ACC_VX);
	
	//last_vx = vx; last_vy = vy; last_vw = vw;
	
	//计算轮子速度
  wheel_rpm[FR] = +chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;
  wheel_rpm[FL] = +chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
  wheel_rpm[BL] = -chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
  wheel_rpm[BR] = -chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;
	
	//速度环PID计算
	wheel_current[FR] = pid_cal(&motor_speed[FR],get_motor_data(CAN_1,chassis_FR).round_speed * WHEEL_RADIUS * PI,wheel_rpm[FR]);
	wheel_current[FL] = pid_cal(&motor_speed[FL],get_motor_data(CAN_1,chassis_FL).round_speed * WHEEL_RADIUS * PI,wheel_rpm[FL]);
	wheel_current[BL] = pid_cal(&motor_speed[BL],get_motor_data(CAN_1,chassis_BL).round_speed * WHEEL_RADIUS * PI,wheel_rpm[BL]);
	wheel_current[BR] = pid_cal(&motor_speed[BR],get_motor_data(CAN_1,chassis_BR).round_speed * WHEEL_RADIUS * PI,wheel_rpm[BR]);

	//设定马达电流
	set_motor_current(wheel_current[FR],CAN_1,chassis_FR);
	set_motor_current(wheel_current[FL],CAN_1,chassis_FL);
	set_motor_current(wheel_current[BL],CAN_1,chassis_BL);
	set_motor_current(wheel_current[BR],CAN_1,chassis_BR);	
}

//计算储块马达速度（注意不会发送）
void track_moto_speed_calc(float vy)
{
	if(vy<-5||vy>5)
	{
		set_motor_current(0,CAN_1,RollTrack);
	}
	else
	{
		val_limit(&vy, MAX_TRACK_SPEED);
		track_motor_rpm=vy;
		track_pid=pid_cal(&track_motor_speed,get_motor_data(CAN_1,RollTrack).round_speed,track_motor_rpm);
		set_motor_current(track_pid,CAN_1,RollTrack);
	}
	
}




	//串口PID调试
//	#ifdef DEBUG
//		//sprintf(buff1,"%6.2f",wheel_rpm[FR]);
//		sprintf(buff1,"%6.2f",get_motor_data(CAN_1,chassis_FR).round_speed * WHEEL_RADIUS * PI);
//		buff1[15] = ',';
//		sprintf(buff1+16,"%6.2f",get_motor_data(CAN_1,chassis_FL).round_speed * WHEEL_RADIUS * PI);
//		buff1[36] = ',';
//		sprintf(buff1+36,"%6.2f",get_motor_data(CAN_1,chassis_BL).round_speed * WHEEL_RADIUS * PI);
//		buff1[50] = ',';
//		sprintf(buff1+50,"%6.2f",get_motor_data(CAN_1,chassis_BR).round_speed * WHEEL_RADIUS * PI);
//		buff1[74] = '\n';
//		UART_send_data(UART1_data,buff1,75);
//	#endif


