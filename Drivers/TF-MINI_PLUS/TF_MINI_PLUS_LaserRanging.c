/*
 * @Author: sethome
 * @Date: 2021-12-23 00:24:57
 * @LastEditTime: 2021-12-23 10:49:11
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Drivers\TF-MINI_PLUS\TF_MINI_PLUS_LaserRanging.c
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
