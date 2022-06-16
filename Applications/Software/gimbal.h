/*
 * @Author: your name
 * @Date: 2021-12-06 11:35:13
 * @LastEditTime: 2021-12-23 12:08:46
 */
#define __GIMBAL_H__
#ifdef __GIMBAL_H__

#include "pid.h"

enum gimbal_status_e
{
    LOCATION = 0,
    SPEED,
};

struct gimbal_status
{
    //设定云台控制模式
    enum gimbal_status_e pitch_status;
    enum gimbal_status_e yaw_status;

    struct
    {
        float set, now, last, offset;
        float stable;
    } pitch;
    float pitch_speed;
    float set_pitch_speed;

    struct
    {
        float set, now, last, offset;
        float stable;
    } yaw;
    float yaw_speed;
    float set_yaw_speed;

    // struct
    // {
    //     float now, last;
    //     float stable;
    // } roll;
};

extern struct gimbal_status gimbal;

extern pid_t pitch_speed_pid;

//外部调用
void gimbal_init(void);                         //初始化云台
void gimbal_set_offset(float pitch, float yaw); //初始化零点

void gimbal_set(float pitch, float yaw); //设置角度
void gimbal_set_pitch(float pitch);      //设定picth角度

void gimbal_set_speed(float pitch, float yaw); //设定速度
void gimbal_set_yaw_speed(float yaw);          //设定yaw速度

void gimbal_pid_cal(void); //云台PID计算
void gimbal_updata(void);  //更新云台数据

void gimbal_clear_cnt(void);
// end of file

#endif
