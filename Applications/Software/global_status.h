/**
 * @file global_status.h
 * @author sethome
 * @brief 全局状态机
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright sethome Copyright (c) 2022
 *
 */

#include "stdint.h"

#define __GLOBAL_STATUS_H__
#ifdef __GLOBAL_STATUS_H__

// 适合十几个简单状态的情况
struct GlobalStatus_t
{
    // 云台和视觉状态
    enum gimbal_visual_e
    {
        SHUTDOWN = 0,              // 停机
        GIMBAL_FLOW,               // 跟随模式
        GIMBAL_HANDLNESS,          // 无头模式
        SELF_SIGHT_WITH_FLOW,      // 跟随自瞄
        SELF_SIGHT_WITH_HANDLNESS, // 无头自瞄
        GYRO,                      // 自瞄
        GYRO_WITH_SELF_SIGHT,      // 小陀螺自瞄
    } gimbal_with_visual;

    enum cap_e
    {
        STOP = 0, // 此时电容应在充电
        ACC,      // 用于短时加速
        FULL,     //全力响应
    } cap;
		
		
    enum team_e
    {
        BLUE_TEAM = 0,
        RED_TEAM,
    }team;
};

extern struct GlobalStatus_t Global_status;

void change_gimbal_visual_status(enum gimbal_visual_e in);
void change_cap_status(enum cap_e in);
enum gimbal_visual_e read_gimbal_visual_status(void);
enum cap_e read_cap_status(void);

#endif
//end of file
