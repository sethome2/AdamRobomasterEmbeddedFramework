/*
 * @Author: sethome
 * @Date: 2021-12-06 11:32:52
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\gimbal.c
 */
#include "gimbal.h"
#include "pid.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "small_tools.h"

struct gimbal_status gimbal;

pid_t pitch_speed_pid;
pid_t pitch_location_pid;

pid_t yaw_speed_pid;
pid_t yaw_location_pid;

//云台电机数据
#define PITCH_MOTOR CAN_2_1
#define YAW_MOTOR CAN_2_2

//稳定程度
sliding_variance pitch_variance;
sliding_variance yaw_variance;

//云台初始化
void gimbal_init()
{
    pid_set(&pitch_speed_pid, 100, 0, 10, 1000, 0);
    pid_set(&pitch_speed_pid, 100, 0, 10, 1000, 0);

    pid_set(&yaw_speed_pid, 100, 0, 10, 1000, 0);
    pid_set(&yaw_location_pid, 100, 0, 10, 1000, 0);

    gimbal.pitch.offset = 0;
    gimbal.pitch.stable = 0;

    gimbal.yaw.offset = 0;
    gimbal.yaw.stable = 0;
}

void gimbal_set_offset(float pitch, float yaw)
{
    gimbal.pitch.offset = pitch;
    gimbal.yaw.offset = yaw;
}

void gimbal_updata()
{
    //从陀螺仪或电机获取数据
    gimbal.pitch.now = IMU_data.KF_result.pitch - gimbal.pitch.offset;

    //decode_as_6020(YAW_MOTOR);
    //decode_as_3508(YAW_MOTOR);
    //gimbal.yaw.now = get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.offset;
    gimbal.yaw.now = IMU_data.KF_result.yaw - gimbal.yaw.offset;

    gimbal.roll.now = IMU_data.KF_result.roll;

    gimbal.pitch.stable = sliding_variance_cal(&pitch_variance, gimbal.pitch.now);
    gimbal.yaw.stable = sliding_variance_cal(&yaw_variance, gimbal.yaw.now);
}

void gimbal_pid_cal()
{
    static float yaw_motor_speed = 0, pitch_motor_speed = 0;

    //位置环
    yaw_motor_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
    pitch_motor_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);

    //速度环
    set_motor(pid_cal(&yaw_speed_pid, get_motor_data(YAW_MOTOR).speed_rpm, yaw_motor_speed), YAW_MOTOR);
    set_motor(pid_cal(&yaw_speed_pid, get_motor_data(PITCH_MOTOR).speed_rpm, yaw_motor_speed), YAW_MOTOR);
}
