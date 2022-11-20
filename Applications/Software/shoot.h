/**
 * @file shoot.h
 * @author sethome 
 * @brief 发射模块
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "stdint.h"

#define _SHOOT_H_
#ifdef _SHOOT_H_

#pragma anon_unions

#define FULL_BULLETS 500

//#define USE_3508_AS_SHOOT_MOTOR

enum shoot_speed
{
    SHOOT_STOP = 1200,
    SHOOT_17 = 1350,
    SHOOT_23 = 1450,
    SHOOT_30 = 1550,
};

typedef struct
{
    /* data */
    enum shoot_speed speed_level;

    int remainingBullets;

    //电机数据
    float shoot_speed[2]; //摩擦轮速度

    struct
    {
        /* data */
        float now;
        float set;
    }trigger_location; //拨弹电机位置

} shoot_t;

extern shoot_t shoot;

//外部调用
void shoot_init(void);    //初始化
void shoot_update(void);  //更新拨弹电机速度等
void shoot_pid_cal(void); //计算pid
int shoot_Bullets(int n); //发射N颗子弹
void shoot_reload(void);  //子弹重新计数
#endif

//end of file
