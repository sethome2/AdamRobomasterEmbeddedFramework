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

//#define USE_3508_AS_SHOOT_MOTOR

enum shoot_speed
{
    SHOOT_STOP = 0,
    SHOOT_17 = 1000,
    SHOOT_23 = 3000,
    SHOOT_30 = 4000,
};

typedef struct
{
    /* data */
    enum shoot_speed speed_level;

    int remainingBullets;

    //�������
    float shoot_speed[2]; //Ħ�����ٶ�

    struct
    {
        /* data */
        float now;
        float set;
    }trigger_location; //�������λ��

} shoot_t;

extern shoot_t shoot;

//�ⲿ����
void shoot_init(void);    //��ʼ��
void shoot_update(void);  //���²�������ٶȵ�
void shoot_pid_cal(void); //����pid
int shoot_Bullets(int n); //����N���ӵ�
void shoot_reload(void);  //�ӵ����¼���

#endif

//end of file
