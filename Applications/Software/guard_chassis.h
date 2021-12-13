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

void guard_chassis_move_init(void);              //�ڱ����̳�ʼ��
void guard_chassis_pid_calc(void);               //�ڱ�����PID��ʼ��
void guard_chassis_set_location(float location); //�ڱ��趨λ��
void guard_chassis_offset(void);                 //�趨����ֵ�����ص�ԭ��
void guard_chassis_updata_location(void);        //�ڱ�����λ��
#endif

//end of file
