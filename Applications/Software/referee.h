#ifndef __REFEREE_H__
#define __REFEREE_H__

#include "stdint.h"

typedef struct
{
    struct game_status_t
    {
        uint8_t game_type : 4;      //1：RoboMaster 机甲大师赛 2：RoboMaster 机甲大师单项赛 3：ICRA RoboMaster 人工智能挑战赛 4：RoboMaster 联盟赛 3V3 5：RoboMaster 联盟赛 1V1
        uint8_t game_progress : 4;  //0：未开始比赛 1：准备阶段 2：自检阶段 3：5s 倒计时 4：对战中 5：比赛结算中
        uint16_t stage_remain_time; //当前阶段剩余时间，单位 s
        uint64_t SyncTimeStamp;     //当前阶段剩余时间，单位 s
    } game_status;                  //1HZ

    struct game_result_t
    {
        uint8_t winner; //0 平局 1 红方胜利 2 蓝方胜利
    } game_result;      //比赛结束后发送

    struct game_robot_HP_t
    {
        uint16_t red_1_robot_HP;
        uint16_t red_2_robot_HP;
        uint16_t red_3_robot_HP;
        uint16_t red_4_robot_HP;
        uint16_t red_5_robot_HP;
        uint16_t red_7_robot_HP;
        uint16_t red_outpost_HP;
        uint16_t red_base_HP;
        uint16_t blue_1_robot_HP;
        uint16_t blue_2_robot_HP;
        uint16_t blue_3_robot_HP;
        uint16_t blue_4_robot_HP;
        uint16_t blue_5_robot_HP;
        uint16_t blue_7_robot_HP;
        uint16_t blue_outpost_HP;
        uint16_t blue_base_HP;
    } game_robot_HP; //1HZ

} referee_t;

extern referee_t referee;

//外部调用
void referee_init(void);                                                  //初始化裁判系统
int referee_decode_full_frame(unsigned char rx_data[], unsigned int len); //解析从裁判系统获取的数据

#endif

//end of file
