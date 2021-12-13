/*
 * @Author: sethome
 * @Date: 2021-12-02 11:48:50
 * @LastEditTime: 2021-12-09 10:47:14
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\guard_chassis.h
 */
#define __GUARD_CHASSIS_H__
#ifdef __GUARD_CHASSIS_H__

#include "CAN_receive&send.h"

#include "struct_typedef.h"

struct guard_chassis_status
{
    struct
    {
        float set;
        float last;
        float now;
        float offset;
    } speed;
    float acc;

    struct
    {
        float set;
        float last;
        float now;
        float offset;
    } location;
};

extern struct guard_chassis_status chassis;

void guard_chassis_move_init(void);              //哨兵底盘初始化
void guard_chassis_pid_calc(void);               //哨兵底盘PID初始化
void guard_chassis_set_location(float location); //哨兵设定位置
void guard_chassis_offset(void);                 //设定归零值，并回到原点
void guard_chassis_updata_location(void);        //哨兵更新位置
#endif

//end of file
