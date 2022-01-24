/*
 * @Author: sethome
 * @Date: 2021-12-06 11:32:52
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\gimbal.c
 */
#include "gimbal.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "small_tools.h"

#include "USB_VirCom.h"

struct gimbal_status gimbal;

pid_t pitch_speed_pid;
pid_t pitch_location_pid;

pid_t yaw_speed_pid;
pid_t yaw_location_pid;

//云台电机数据
#define PITCH_MOTOR CAN_2_2
#define YAW_MOTOR CAN_2_1

//稳定程度
sliding_variance pitch_variance;
sliding_variance yaw_variance;

//云台初始化
void gimbal_init()
{
    pid_set(&yaw_speed_pid, 3.45, 0.1f, 0.1f, 2000.0f, 200.0f);
    pid_set(&yaw_location_pid, 40, 0, 0.245f, 3600.0f, 0.0f);
	
    pid_set(&pitch_speed_pid, 5.5f, 0.05f, 0.1f, 2500.0f, 300.0f);
    pid_set(&pitch_location_pid, 60.0f, 0.0f, 0.0f, 3000.0f, 0.0f);

		gimbal.pitch.now = 0;
		gimbal.pitch.set = 0;
    gimbal.pitch.offset = 0;
    gimbal.pitch.stable = 0;

		gimbal.yaw.now = 0;
		gimbal.yaw.set = 0;
    gimbal.yaw.offset = 0;
    gimbal.yaw.stable = 0;
    
    sliding_variance_init(&pitch_variance);
    sliding_variance_init(&yaw_variance);
}

void gimbal_set_offset(float pitch, float yaw)
{
    gimbal.pitch.offset = pitch;
    gimbal.yaw.offset = yaw;
}

void gimbal_updata()
{
    //从陀螺仪或电机获取数据
    //yaw轴更新
    //decode_as_6020(YAW_MOTOR);
		decode_as_3508(YAW_MOTOR);
		gimbal.yaw.now = (get_motor_data(YAW_MOTOR).angle_cnt) / 3.0 - gimbal.yaw.offset;
	
    //pitch轴更新
    //decode_as_6020(PITCH_MOTOR);
    decode_as_3508(PITCH_MOTOR);
		gimbal.pitch.now = (get_motor_data(PITCH_MOTOR).angle_cnt) - gimbal.pitch.offset;

    gimbal.pitch.stable = sliding_variance_cal(&pitch_variance, gimbal.pitch.now);
    gimbal.yaw.stable = sliding_variance_cal(&yaw_variance, gimbal.yaw.now);
}

void gimbal_set(float pitch,float yaw)
{
    gimbal.pitch.set = pitch;
    gimbal.yaw.set = yaw;
}

//char arduino[128];
void gimbal_pid_cal()
{
    //位置环
    gimbal.yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
    gimbal.pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);

    //速度环
    set_motor(pid_cal(&yaw_speed_pid, get_motor_data(YAW_MOTOR).speed_rpm, gimbal.yaw_speed), YAW_MOTOR);
		set_motor(pid_cal(&pitch_speed_pid, get_motor_data(PITCH_MOTOR).speed_rpm, gimbal.pitch_speed), PITCH_MOTOR);

//		int len = sprintf(arduino,"%f,%f,%f,%d,%d\n",gimbal.pitch.now,gimbal.pitch.set,gimbal.pitch_speed,get_motor_data(PITCH_MOTOR).speed_rpm,get_motor_data(PITCH_MOTOR).set);
//		VirCom_send(arduino,len);
}
