#ifndef __REFEREE_H__
#define __REFEREE_H__

#include "stdint.h"

typedef struct
{
  __packed struct game_status_t // 比赛状态数据：0x0001。发送频率：1Hz
  {
    uint8_t game_type : 4;      // 1：RoboMaster 机甲大师赛 2：RoboMaster 机甲大师单项赛 3：ICRA RoboMaster 人工智能挑战赛 4：RoboMaster 联盟赛 3V3 5：RoboMaster 联盟赛 1V1
    uint8_t game_progress : 4;  // 0：未开始比赛 1：准备阶段 2：自检阶段 3：5s 倒计时 4：对战中 5：比赛结算中
    uint16_t stage_remain_time; //当前阶段剩余时间，单位 s
    uint64_t SyncTimeStamp;     //当前阶段剩余时间，单位 s
  } game_status;                // 1HZ

  __packed struct game_result_t // 比赛结果数据：0x0002。发送频率：比赛结束后发送
  {
    uint8_t winner; // 0 平局 1 红方胜利 2 蓝方胜利
  } game_result;    //比赛结束后发送

  __packed struct game_robot_HP_t // 机器人血量数据：0x0003
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
  } game_robot_HP; // 1HZ

  __packed struct // 飞镖发射状态：0x0004
  {
    uint8_t dart_belong;           // 发射队伍 1 Red 2 Blue
    uint16_t stage_remaining_time; // 剩余发射时间
  } ext_dart_status_t;

  __packed struct // 人工智能挑战赛加成与惩罚区状态：0x0005。发送频率：1Hz 周期发送，发送范围：所有机器人
  {
    uint8_t F1_zone_status : 1;
    uint8_t F1_zone_buff_debuff_status : 3;
    uint8_t F2_zone_status : 1;
    uint8_t F2_zone_buff_debuff_status : 3;
    uint8_t F3_zone_status : 1;
    uint8_t F3_zone_buff_debuff_status : 3;
    uint8_t F4_zone_status : 1;
    uint8_t F4_zone_buff_debuff_status : 3;
    uint8_t F5_zone_status : 1;
    uint8_t F5_zone_buff_debuff_status : 3;
    uint8_t F6_zone_status : 1;
    uint8_t F6_zone_buff_debuff_status : 3;
    uint16_t red1_bullet_left;
    uint16_t red2_bullet_left;
    uint16_t blue1_bullet_left;
    uint16_t blue2_bullet_left;
  } ext_ICRA_buff_debuff_zone_status_t;

  __packed struct // 场地地事件数据：0x0101。发送频率：事件改变后发送
  {
    uint32_t event_type;
  } ext_event_data_t;

  __packed struct // 补给站动作标识：0x0102。发送频率：动作改变后发送, 发送范围：己方机器
  {
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
  } ext_supply_projectile_action_t;

  __packed struct // 裁判警告信息：cmd_id (0x0104)。发送频率：警告发生后发送
  {
    uint8_t level;
    uint8_t foul_robot_id;
  } ext_referee_warning_t;

  __packed struct // 飞镖发射口倒计时：cmd_id (0x0105)。发送频率：1Hz 周期发送，发送范围：己方机器人
  {
    uint8_t dart_remaining_time;
  } ext_dart_remaining_time_t;

  __packed struct // 比赛机器人状态：0x0201。发送频率：10Hz
  {
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t remain_HP;
    uint16_t max_HP;
    uint16_t shooter_id1_17mm_cooling_rate;
    uint16_t shooter_id1_17mm_cooling_limit;
    uint16_t shooter_id1_17mm_speed_limit;
    uint16_t shooter_id2_17mm_cooling_rate;
    uint16_t shooter_id2_17mm_cooling_limit;
    uint16_t shooter_id2_17mm_speed_limit;

    uint16_t shooter_id1_42mm_cooling_rate;
    uint16_t shooter_id1_42mm_cooling_limit;
    uint16_t shooter_id1_42mm_speed_limit;
    uint16_t chassis_power_limit;
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
  } ext_game_robot_status_t;

  __packed struct // 实时功率热量数据：0x0202。发送频率：50Hz
  {
    uint16_t chassis_volt;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t chassis_power_buffer;
    uint16_t shooter_id1_17mm_cooling_heat;
    uint16_t shooter_id2_17mm_cooling_heat;
    uint16_t shooter_id1_42mm_cooling_heat;
  } ext_power_heat_data_t;

  __packed struct // 机器人位置：0x0203。发送频率：10Hz
  {
    float x;
    float y;
    float z;
    float yaw;
  } ext_game_robot_pos_t;

  __packed struct // 机器人增益：0x0204。发送频率：1Hz
  {
    uint8_t power_rune_buff;
  } ext_buff_t;

  __packed struct // 空中机器人能量状态：0x0205。发送频率：10Hz
  {
    uint8_t attack_time;
  } aerial_robot_energy_t;

  __packed struct // 伤害状态：0x0206。发送频率：伤害发生后发送
  {
    uint8_t armor_id : 4;
    uint8_t hurt_type : 4;
  } ext_robot_hurt_t;

  __packed struct // 实时射击信息：0x0207。发送频率：射击后发送
  {
    uint8_t bullet_type;
    uint8_t shooter_id;
    uint8_t bullet_freq;
    float bullet_speed;
  } ext_shoot_data_t;

  __packed struct // 子弹剩余发射数：0x0208。发送频率：10Hz 周期发送，所有机器人发送
  {
    uint16_t bullet_remaining_num_17mm;
    uint16_t bullet_remaining_num_42mm;
    uint16_t coin_remaining_num; // 剩余金币
  } ext_bullet_remaining_t;

  __packed struct // 机器人 RFID 状态：0x0209。发送频率：1Hz，发送范围：单一机器人
  {
    uint32_t rfid_status;
  } ext_rfid_status_t;

  __packed struct // 飞镖机器人客户端指令数据：0x020A。发送频率：10Hz，发送范围：单一机器人
  {
    uint8_t dart_launch_opening_status;
    uint8_t dart_attack_target;
    uint16_t target_change_time;
    uint8_t first_dart_speed;
    uint8_t second_dart_speed;
    uint8_t third_dart_speed;
    uint8_t fourth_dart_speed;
    uint16_t last_dart_launch_time;
    uint16_t operate_launch_cmd_time;
  } ext_dart_client_cmd_t;
} referee_t;

extern referee_t referee; // 裁判系统接受结构体

__packed typedef struct
{
  uint8_t name[3]; // 名字 只用前一个
  uint32_t operate_tpye : 3;
  uint32_t graphic_tpye : 3;
  uint32_t layer : 4;
  uint32_t color : 4;
  uint32_t start_angle : 9;
  uint32_t end_angle : 9;
  uint32_t width : 10;
  uint32_t start_x : 11;
  uint32_t start_y : 11;
  uint32_t radius : 10;
  uint32_t end_x : 11;
  uint32_t end_y : 11;
} graphic_data_struct_t;

struct layer_node;

typedef struct
{
  graphic_data_struct_t graphic;
  uint8_t updata;
} layer_node;

/**
 * @brief 用于发送帧头的cmd_id列表
 *        结构体frame_head使用
 */
typedef enum
{
  COM_EACH_OTHER_ID = 0x301,   //交互信息
  PRIVATE_CTL_ID = 0x302,      // 自定义控制器
  FORM_OTHER_TEAMMATE = 0x303, // 来着云台手/雷达的信息 ?
  RADER_SEND_INFO = 0x305,     // 雷达发送信息
} SEND_ID;

//********************交互信息部分**********************
__packed typedef struct
{
  uint16_t data_cmd_id;
  uint16_t sender_ID;
  uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/**
 * @brief 交互信息的发送和接受者ID
 *
 */
typedef enum
{
  HERO_R = 1,
  ENGINEER_R = 2,
  INFANTRY1_R = 3,
  INFANTRY2_R = 4,
  INFANTRY3_R = 5,
  AIR_R = 6,
  RADER_R = 7, // 红方

  HERO_B = 101,
  ENGINEER_B = 102,
  INFANTRY1_B = 103,
  INFANTRY2_B = 104,
  INFANTRY3_B = 105,
  AIR_B = 106,
  RADER_B = 107, // 蓝方
} SEMDER_ID;

//外部调用
void referee_init(void);                                            //初始化裁判系统
int referee_decode_full_frame(uint8_t rx_data[], unsigned int len); //解析从裁判系统获取的数据

#endif

// end of file
