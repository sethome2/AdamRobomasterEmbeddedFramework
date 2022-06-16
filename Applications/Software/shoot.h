/*
 * @Author: sethome
 * @Date: 2021-12-23 12:30:06
 * @LastEditTime: 2021-12-23 16:37:02
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\shoot.h
 */
#include "stdint.h"

#define _SHOOT_H_
#ifdef _SHOOT_H_

#pragma anon_unions

#define FULL_BULLETS 500

#define USE_3508_AS_SHOOT_MOTOR

enum shoot_speed
{
    SHOOT_STOP =0,
    SHOOT_17 = 8000,
    SHOOT_23 = 0,
    SHOOT_30 = 0,
};

enum trigger_status_e     //拨弹轮控制模式
{
    LOCATIONS = 0,
    SPEEDS,
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
			  float off_set;
    }trigger_location; //拨弹电机位置
		
		float trigger_speed;//拨弹电机速度
    float set_trigger_speed;
		
    enum trigger_status_e trigger_status;

} shoot_t;

extern shoot_t shoot;

//外部调用
void shoot_init(void);    //初始化
void shoot_update(void);  //更新拨弹电机速度等
void shoot_pid_cal(void); //计算pid
int shoot_Bullets(int n); //发射N颗子弹
void shoot_reload(void);  //子弹重新计数
void trigger_set_speed(float sudu); //拨弹电机速度设定
int shoot_one(void);
void trigger_clear_cnt(void);
void trigger_set_offset(float set);
#endif

//end of file
