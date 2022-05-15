#ifndef __REFEREE_H__
#define __REFEREE_H__

#include "stdint.h"

typedef struct
{
  __packed struct game_status_t // ����״̬���ݣ�0x0001������Ƶ�ʣ�1Hz
  {
    uint8_t game_type : 4;      // 1��RoboMaster ���״�ʦ�� 2��RoboMaster ���״�ʦ������ 3��ICRA RoboMaster �˹�������ս�� 4��RoboMaster ������ 3V3 5��RoboMaster ������ 1V1
    uint8_t game_progress : 4;  // 0��δ��ʼ���� 1��׼���׶� 2���Լ�׶� 3��5s ����ʱ 4����ս�� 5������������
    uint16_t stage_remain_time; //��ǰ�׶�ʣ��ʱ�䣬��λ s
    uint64_t SyncTimeStamp;     //��ǰ�׶�ʣ��ʱ�䣬��λ s
  } game_status;                // 1HZ

  __packed struct game_result_t // ����������ݣ�0x0002������Ƶ�ʣ�������������
  {
    uint8_t winner; // 0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
  } game_result;    //������������

  __packed struct game_robot_HP_t // ������Ѫ�����ݣ�0x0003
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

  __packed struct // ���ڷ���״̬��0x0004
  {
    uint8_t dart_belong;           // ������� 1 Red 2 Blue
    uint16_t stage_remaining_time; // ʣ�෢��ʱ��
  } ext_dart_status_t;

  __packed struct // �˹�������ս���ӳ���ͷ���״̬��0x0005������Ƶ�ʣ�1Hz ���ڷ��ͣ����ͷ�Χ�����л�����
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

  __packed struct // ���ص��¼����ݣ�0x0101������Ƶ�ʣ��¼��ı����
  {
    uint32_t event_type;
  } ext_event_data_t;

  __packed struct // ����վ������ʶ��0x0102������Ƶ�ʣ������ı����, ���ͷ�Χ����������
  {
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
  } ext_supply_projectile_action_t;

  __packed struct // ���о�����Ϣ��cmd_id (0x0104)������Ƶ�ʣ����淢������
  {
    uint8_t level;
    uint8_t foul_robot_id;
  } ext_referee_warning_t;

  __packed struct // ���ڷ���ڵ���ʱ��cmd_id (0x0105)������Ƶ�ʣ�1Hz ���ڷ��ͣ����ͷ�Χ������������
  {
    uint8_t dart_remaining_time;
  } ext_dart_remaining_time_t;

  __packed struct // ����������״̬��0x0201������Ƶ�ʣ�10Hz
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

  __packed struct // ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz
  {
    uint16_t chassis_volt;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t chassis_power_buffer;
    uint16_t shooter_id1_17mm_cooling_heat;
    uint16_t shooter_id2_17mm_cooling_heat;
    uint16_t shooter_id1_42mm_cooling_heat;
  } ext_power_heat_data_t;

  __packed struct // ������λ�ã�0x0203������Ƶ�ʣ�10Hz
  {
    float x;
    float y;
    float z;
    float yaw;
  } ext_game_robot_pos_t;

  __packed struct // ���������棺0x0204������Ƶ�ʣ�1Hz
  {
    uint8_t power_rune_buff;
  } ext_buff_t;

  __packed struct // ���л���������״̬��0x0205������Ƶ�ʣ�10Hz
  {
    uint8_t attack_time;
  } aerial_robot_energy_t;

  __packed struct // �˺�״̬��0x0206������Ƶ�ʣ��˺���������
  {
    uint8_t armor_id : 4;
    uint8_t hurt_type : 4;
  } ext_robot_hurt_t;

  __packed struct // ʵʱ�����Ϣ��0x0207������Ƶ�ʣ��������
  {
    uint8_t bullet_type;
    uint8_t shooter_id;
    uint8_t bullet_freq;
    float bullet_speed;
  } ext_shoot_data_t;

  __packed struct // �ӵ�ʣ�෢������0x0208������Ƶ�ʣ�10Hz ���ڷ��ͣ����л����˷���
  {
    uint16_t bullet_remaining_num_17mm;
    uint16_t bullet_remaining_num_42mm;
    uint16_t coin_remaining_num; // ʣ����
  } ext_bullet_remaining_t;

  __packed struct // ������ RFID ״̬��0x0209������Ƶ�ʣ�1Hz�����ͷ�Χ����һ������
  {
    uint32_t rfid_status;
  } ext_rfid_status_t;

  __packed struct // ���ڻ����˿ͻ���ָ�����ݣ�0x020A������Ƶ�ʣ�10Hz�����ͷ�Χ����һ������
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

extern referee_t referee; // ����ϵͳ���ܽṹ��

__packed typedef struct
{
  uint8_t name[3]; // ���� ֻ��ǰһ��
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

  layer_node *next;
} layer_node;

/**
 * @brief ���ڷ���֡ͷ��cmd_id�б�
 *        �ṹ��frame_headʹ��
 */
typedef enum
{
  COM_EACH_OTHER_ID = 0x301,   //������Ϣ
  PRIVATE_CTL_ID = 0x302,      // �Զ��������
  FORM_OTHER_TEAMMATE = 0x303, // ������̨��/�״����Ϣ ?
  RADER_SEND_INFO = 0x305,     // �״﷢����Ϣ
} SEND_ID;

//********************������Ϣ����**********************
__packed typedef struct
{
  uint16_t data_cmd_id;
  uint16_t sender_ID;
  uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/**
 * @brief ������Ϣ�ķ��ͺͽ�����ID
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
  RADER_R = 7, // �췽

  HERO_B = 101,
  ENGINEER_B = 102,
  INFANTRY1_B = 103,
  INFANTRY2_B = 104,
  INFANTRY3_B = 105,
  AIR_B = 106,
  RADER_B = 107, // ����
} SEMDER_ID;

//�ⲿ����
void referee_init(void);                                            //��ʼ������ϵͳ
int referee_decode_full_frame(uint8_t rx_data[], unsigned int len); //�����Ӳ���ϵͳ��ȡ������

#endif

// end of file
