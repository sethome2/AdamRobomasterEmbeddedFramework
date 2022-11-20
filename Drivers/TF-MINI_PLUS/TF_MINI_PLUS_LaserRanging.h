/**
 * @file TF_MINI_PLUS_LaserRanging.h
 * @author sethome 
 * @brief 北醒TF-MINI-PLUS激光测距模块驱动
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define __TF_MINI_PLUS_LASERRANGING_H__
#ifdef __TF_MINI_PLUS_LASERRANGING_H__

struct TF_MINI_PLUS_LaserRanging
{
    /* data */
    int distance;
    int strength;
    float temp;
};

extern struct TF_MINI_PLUS_LaserRanging TF_LaserRanging;

void updataTF_MINI_PLUS_LaserRanging(unsigned char rx[], int num); // ????

#endif // __TF_MINI_PLUS_LASERRANGING_H__
