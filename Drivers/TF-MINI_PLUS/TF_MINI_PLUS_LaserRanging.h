/*
 * @Author: sethome
 * @Date: 2021-12-23 00:25:56
 * @LastEditTime: 2021-12-23 00:36:28
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Drivers\TF-MINI_PLUS\TF_MINI_PLUS_LaserRanging.h
 */

struct TF_MINI_PLUS_LaserRanging
{
    /* data */
    int distance;
    int strength;
    float temp;
};

extern struct TF_MINI_PLUS_LaserRanging TF_LaserRanging;

void updataTF_MINI_PLUS_LaserRanging(unsigned char rx[], int num);//更新激光测距数据（串口）
