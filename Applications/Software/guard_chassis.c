/*
 * @Author: sethome
 * @Date: 2021-12-02 11:15:52
 * @LastEditTime: 2021-12-23 11:10:27
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\guard_chassis_move.c
 */
#include "guard_chassis.h"

#include "CAN_receive&send.h"
#include "TF_MINI_PLUS_LaserRanging.h"

#include "time.h"

//algorithm
#include "pid.h"

//std
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

/* motorID 1 %++++++% 0
    ___________++++__________
    ___________++++__________
             %++++++%  */

can_id Motor1 = CAN_1_1;
can_id Motor2 = CAN_1_2;
//wheel conf
#define WHEEL_RADIUS 15.0f //cm
#define PI 3.1415926f

struct guard_chassis_status chassis;

//m/s
float wheel_rpm[2];       //轮子的转速
int16_t wheel_current[2]; //pid的计算电流

//最大速度限制
#define MAX_SPEED 2.5 //m/s
#define MAX_ACC 1.0   //m/s^2

//pid计算变量
pid_t speed_pid[2];
pid_t location_pid;

//初始化运行变量
void guard_chassis_move_init()
{
    //速度环初始化
    pid_set(&speed_pid[0], 1000, 0, 10, 2000, 0);
    pid_set(&speed_pid[1], 1000, 0, 10, 2000, 0);
    //位置环初始化
    pid_set(&location_pid, 10, 0, 1, 2000, 0);
    //时间滑动方差
    sliding_variance_init(&chassis.location.stable);
}

//计算底盘pid
void guard_chassis_pid_calc()
{
    //计算速度
    chassis.speed.set = pid_cal(&location_pid, chassis.location.now, chassis.location.set);

    //计算对应电机的电流
    wheel_current[0] = pid_cal(&speed_pid[0], get_motor_data(Motor1).speed_rpm, chassis.speed.set);
    wheel_current[1] = pid_cal(&speed_pid[1], get_motor_data(Motor2).speed_rpm, chassis.speed.set);

    //设置电流
    set_motor(wheel_current[0], Motor1);
    set_motor(wheel_current[1], Motor2);
}

void guard_chassis_set_location(float location)
{
    chassis.location.set = location;
}

void guard_chassis_offset(float location)
{
    chassis.location.offset = location;
}

void guard_chassis_updata_location()
{
    static float last_time = 0;
    float time = Get_sys_time_s();
    
    decode_as_3508(Motor1);
    decode_as_3508(Motor2);
    //计算底盘的位置
    chassis.location.last = chassis.location.now;
    //从电机获取
    //chassis.location.now = WHEEL_RADIUS * ((get_motor_data(Motor1).angle_cnt + get_motor_data(Motor1).angle_cnt) / 2.0) - chassis.location.offset;
    chassis.location.now = TF_LaserRanging.distance - chassis.location.offset;
    sliding_variance_cal(&chassis.location.stable,chassis.location.now);

    //计算底盘的速度
    chassis.speed.last = chassis.speed.now;
    chassis.speed.last = (chassis.location.now - chassis.location.set) / (time - last_time);
    
    last_time = time;
}
