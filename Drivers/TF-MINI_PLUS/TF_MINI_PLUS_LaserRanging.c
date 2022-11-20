/**
 * @file TF_MINI_PLUS_LaserRanging.c
 * @author sethome
 * @brief 北醒TF-MINI-PLUS激光测距模块驱动
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */
#include "TF_MINI_PLUS_LaserRanging.h"

struct TF_MINI_PLUS_LaserRanging TF_LaserRanging;

//更新数据
void updataTF_MINI_PLUS_LaserRanging(unsigned char rx[], int num)
{
    int checksum = 0;

    if (rx[0] != 0x59 || rx[1] != 0x59 || num != 9) //错误检查
        return;
    for (int i = 0; i < 8; i++)
        checksum += rx[i];

    if (rx[8] == (checksum % 256))
    {
        TF_LaserRanging.distance = rx[2] + rx[3] * 256;
        TF_LaserRanging.strength = rx[4] + rx[5] * 256;
        TF_LaserRanging.temp = (float)(rx[6] + rx[7] * 256) / 8.0f - 256.0f;
    }
}
