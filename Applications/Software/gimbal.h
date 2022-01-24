/*
 * @Author: your name
 * @Date: 2021-12-06 11:35:13
 * @LastEditTime: 2021-12-23 12:08:46
 */
#define __GIMBAL_H__
#ifdef __GIMBAL_H__

#include "pid.h"

struct gimbal_status
{
    struct
    {
        float set, now, last, offset;
        float stable;
    } pitch;
    float pitch_speed;

    struct
    {
        float set, now, last, offset;
        float stable;
    } yaw;
    float yaw_speed;

    // struct
    // {
    //     float now, last;
    //     float stable;
    // } roll;
};

extern struct gimbal_status gimbal;

extern pid_t pitch_speed_pid;

//外部调用
void gimbal_init(void);                         //云台初始化
void gimbal_set_offset(float pitch, float yaw); //设置云台初始值
void gimbal_set(float pitch,float yaw); //设置云台角度
void gimbal_pid_cal(void);//云台PID计算
void gimbal_updata(void);                       //更新云台数据
//end of file

#endif
