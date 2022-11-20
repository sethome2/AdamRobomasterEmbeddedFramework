/**
 * @file shoot.h
 * @author sethome 
 * @brief ����ģ��
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
