/**
 * @file RM_JudgeSystem.h
 * @author Miraggio (w1159904119@gmail)
 * @brief 
 * @version 0.1
 * @date 2021-03-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _RM_JUDGESYSTEM_H
#define _RM_JUDGESYSTEM_H

#include "dma.h"
#include "usart.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma anon_unions

#define JudgeSystem_FUNGroundInit       \
    {                                   \
        &JudgeSystem_USART_Receive_DMA, \
            &JudgeSystem_Handler,       \
            &Check_Judge,               \
    }

typedef struct
{
    void (*JudgeSystem_USART_Receive_DMA)(UART_HandleTypeDef *huartx);
    void (*JudgeSystem_Handler)(UART_HandleTypeDef *huart);
    void (*Check_Judge)(void);
} JudgeSystem_FUN_t;
extern JudgeSystem_FUN_t JudgeSystem_FUN;

typedef struct
{
    uint8_t InfoUpdateFlag;   //��Ϣ��ȡ���±�־
    uint16_t InfoUpdateFrame; //֡��
    uint8_t OffLineFlag;      //�豸���߱�־
} RM_Judge_t;
extern RM_Judge_t RM_Judge;

float GetVision_CP(void);

//����ϵͳԤ����
#define JUDGE_20 20
#define JUDGE_21 21
#define JUDGE_VERSION JUDGE_21

#define JUDGE_DATA_ERROR 0
#define JUDGE_DATA_CORRECT 1
/*-------------------------------2020------------------------------------*/
#if JUDGE_VERSION == JUDGE_20
//__packed�ǽ���һ�ֽڶ��롣ʹ��_packedһ����Խ�����������Ϊ���ۣ����ڴ����cpu���������ں��ʵ��ֽڱ߽���������»����Ч��packed��ʹ�û��ƻ�������Ȼ�ı߽���
// 2020����ϵͳ�ٷ��ӿ�Э��
typedef struct
{
    uint8_t InfoUpdateFlag;   //��Ϣ��ȡ���±�־
    uint16_t InfoUpdateFrame; //֡��
    uint8_t OffLineFlag;      //�豸���߱�־
} RM_Judge_t;

#define JUDGESYSTEM_PACKSIZE 303u    //����ϵͳ����С
#define JUDGESYSTEM_FRAMEHEADER 0xA5 //֡ͷ
//������
#define BLUE 0
#define RED 1

//��Ϣ����
#define TRUE 1
#define FALSE 0

//���Ͷ���
void Send_Infantry(void);
void Send_Hero(void);
void Send_Sentry(void);
void Send_Engineering(void);

#define JudgeInfoType_Game_state 0x0001
#define JudgeInfoType_Game_result 0x0002
#define JudgeInfoType_Game_robot_HP 0x0003
#define JudgeInfoType_Dart_status 0x0004
#define JudgeInfoType_ICRA_buff 0x0005
#define JudgeInfoType_Event_data 0x0101
#define JudgeInfoType_Supply_projectile_action 0x0102
#define JudgeInfoType_Referee_warning 0x0104
#define JudgeInfoType_Dart_remaining_time 0x0105
#define JudgeInfoType_Game_robot_state 0x0201
#define JudgeInfoType_Power_heat_data 0x0202
#define JudgeInfoType_Game_robot_pos 0x0203
#define JudgeInfoType_Buff_musk 0x0204
#define JudgeInfoType_Robot_energy 0x0205
#define JudgeInfoType_Robot_hurt 0x0206
#define JudgeInfoType_Shoot_data 0x0207
#define JudgeInfoType__Bullet_remaining 0x0208
#define JudgeInfoType___Rfid_status 0x0209
#define JudgeInfoType___Dart_client 0x020A

//��������֡�ĳ��ȣ�FrameHeader(5-Byte)+CmdID(2-Byte)+Data(n-Byte)+FrameTail(2-Byte, CRC16, ����У��)
#define JudgeInfoLength_Game_state 12
#define JudgeInfoLength_Game_result 10
#define JudgeInfoLength_Game_robot_HP 41
#define JudgeInfoLength_Dart_status 12
#define JudgeInfoLength_ICRA_buff 12
#define JudgeInfoLength_Event_data 13
#define JudgeInfoLength_Supply_projectile_action 13
#define JudgeInfoLength_Referee_warning 11
#define JudgeInfoLength_Dart_remaining_time 10
#define JudgeInfoLength_Game_robot_state 27
#define JudgeInfoLength_Power_heat_data 25
#define JudgeInfoLength_Game_robot_pos 25
#define JudgeInfoLength_Buff_musk 10
#define JudgeInfoLength_Robot_energy 12
#define JudgeInfoLength_Robot_hurt 10
#define JudgeInfoLength_Shoot_data 15
#define JudgeInfoLength_Bullet_remaining 11
#define JudgeInfoLength_Rfid_status 13
#define JudgeInfoLength_Dart_client 21

/************************************************����ϵͳ�ṹ��*******************************************************/
typedef struct
{
    uint16_t infoUpdateFrame; //֡��
    uint8_t OffLineFlag;      //�豸���߱�־
} Judge_Monitor_t;

//1.����״̬���ݣ�Ƶ�� 1Hz ���� CmdID 0x0001
typedef struct
{
    union
    {
        uint8_t dataBuff[3];
        __packed struct
        {
            uint8_t game_type : 4;      //��������
            uint8_t game_progress : 4;  //��ǰ�����׶�
            uint16_t stage_remain_time; //��ǰ�׶�ʣ��ʱ��  ��λs
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_game_state_t;

//2.����������ݣ�0x0002������Ƶ�ʣ�������������
typedef struct
{
    union
    {
        uint8_t dataBuff[1];
        __packed struct
        {
            uint8_t winner; //0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_game_result_t;

//3.������Ѫ�����ݣ�0x0003������Ƶ�ʣ�1Hz
typedef struct
{
    union
    {
        uint8_t dataBuff[32];
        __packed struct
        {
            uint16_t red_1_robot_HP;  //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ0
            uint16_t red_2_robot_HP;  //��2���̻�����Ѫ��
            uint16_t red_3_robot_HP;  //��3����������Ѫ��
            uint16_t red_4_robot_HP;  //��4����������Ѫ��
            uint16_t red_5_robot_HP;  //��5����������Ѫ��
            uint16_t red_7_robot_HP;  //��7�ڱ�������Ѫ��
            uint16_t red_outpost_HP;  //�췽ǰ��վѪ��
            uint16_t red_base_HP;     //�췽����Ѫ��
            uint16_t blue_1_robot_HP; //��1Ӣ�ۻ�����Ѫ��
            uint16_t blue_2_robot_HP; //��2���̻�����Ѫ��
            uint16_t blue_3_robot_HP; //��3����������Ѫ��
            uint16_t blue_4_robot_HP; //��4����������Ѫ��
            uint16_t blue_5_robot_HP; //��5����������Ѫ��
            uint16_t blue_7_robot_HP; //��7�ڱ�������Ѫ��
            uint16_t blue_outpost_HP; //����ǰ��վѪ��
            uint16_t blue_base_HP;    //��������Ѫ��
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_game_robot_HP_t;

//4.���ڷ���״̬��0x0004 ���ڷ���ʱ����
typedef struct
{
    union
    {
        uint8_t dataBuff[3];
        __packed struct
        {
            uint8_t dart_belong;           //������ڵĶ���    1 �췽 2 ����
            uint16_t stage_remaining_time; //����ʱ��ʣ�����ʱ�� ��λs
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_dart_status_t;

//5.�˹�������ս���ӳ���ͽ���״̬ 0x005   1hz����
typedef struct
{
    union
    {
        uint8_t dataBuff[3];
        __packed struct
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
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_ICRA_buff_debuff_zone_status_t;

//6.�����¼����ݣ�0x0101������Ƶ�ʣ�1hz���ڷ���
typedef struct
{
    union
    {
        uint8_t dataBuff[4];
        __packed struct
        {
            uint32_t event_type; //���������ϵͳЭ��P9-10
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_event_data_t;

//7.����վ������ʶ��0x0102������Ƶ�ʣ�������������
typedef struct
{
    union
    {
        uint8_t dataBuff[4];
        __packed struct
        {
            uint8_t supply_projectile_id;   //����վ�� ID�� 1��1�Ų�����  2:2�Ų�����
            uint8_t supply_robot_id;        //���������� ID
                                            /*0Ϊ��ǰ�޻����˲�����1Ϊ�췽Ӣ�ۻ����˲�����2Ϊ�췽���̻����˲�����
			    3 / 4 / 5Ϊ�췽���������˲�����101Ϊ����Ӣ�ۻ����˲�����102Ϊ�������̻����˲�����
				             103 / 104 / 105Ϊ�������������˲���*/
            uint8_t supply_projectile_step; //�����ڿ���״̬ 0Ϊ�ر� 1Ϊ�ӵ�׼���� 2Ϊ�ӵ�����
            uint8_t supply_projectile_num;  //��������   50�� 100�� 150�� 200��
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_supply_projectile_action_t;

//8.���о������ݣ�0x0104�� ����Ƶ�ʣ����淢������
typedef struct
{
    union
    {
        uint8_t dataBuff[2];
        __packed struct
        {
            uint8_t level;         //����ȼ�
            uint8_t foul_robot_id; //���������ID  1����5������ʱ IDΪ0
                                   //  2--4������ʱ   IDΪ���������ID
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_referee_warning_t;

//9.���ڷ���ڵ���ʱ 0x0105  ����Ƶ�ʣ�1hz
typedef struct
{
    union
    {
        uint8_t dataBuff[1];
        __packed struct
        {
            uint8_t dart_remaining_time; //15s ����ʱ
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_dart_remaining_time_t;

//10.������״̬���ݣ�0x0201������Ƶ�ʣ�10Hz ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[18];
        __packed struct
        {
            uint8_t robot_id;                       //������ ID
            uint8_t robot_level;                    //�����˵ȼ�  1  2   3
            uint16_t remain_HP;                     //������ʣ��Ѫ��
            uint16_t max_HP;                        //����������Ѫ��
            uint16_t shooter_heat0_cooling_rate;    //������ 17mm ǹ��ÿ����ȴֵ
            uint16_t shooter_heat0_cooling_limit;   //������ 17mm ǹ����������
            uint16_t shooter_heat1_cooling_rate;    //������ 42mm ǹ��ÿ����ȴֵ
            uint16_t shooter_heat1_cooling_limit;   //������ 42mm ǹ����������
            uint8_t shooter_heat0_speed_limit;      //������ 17mm ǹ�������ٶ� ��λ m/s
            uint8_t shooter_heat1_speed_limit;      //������ 42mm ǹ�������ٶ� ��λ m/s
            uint8_t max_chassis_power;              //������������  ��λW
            uint8_t mains_power_gimbal_output : 1;  //gimbal������� 1Ϊ��24V����� 0Ϊ��24v�����
            uint8_t mains_power_chassis_output : 1; //chassis������� 1Ϊ��24V�����0Ϊ��24v�����
            uint8_t mains_power_shooter_output : 1; //shooter������� 1Ϊ��24V�����0Ϊ��24v�����
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_game_robot_state_t;

//11.ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz  ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[16];
        __packed struct
        {
            uint16_t chassis_volt;         //���������ѹ ��λ ����
            uint16_t chassis_current;      //����������� ��λ ����
            float chassis_power;           //����������� ��λ W ��
            int16_t chassis_power_buffer;  //���̹��ʻ��� ��λ J ����   ��ע�����¸��ݹ��������� 250J
            uint16_t shooter_heat0;        //17mm ǹ������
            uint16_t shooter_heat1;        //42mm ǹ������
            uint16_t mobile_shooter_heat2; //���� 17mmǹ������
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_power_heat_data_t;

//12. ������λ�ã�0x0203������Ƶ�ʣ�10Hz  ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[16];
        __packed struct
        {
            float x;   //λ�� x ���꣬��λ m
            float y;   //λ�� y ���꣬��λ m
            float z;   //λ�� z ���꣬��λ m
            float yaw; //λ��ǹ�ڣ���λ��
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_game_robot_pos_t;

//13.���������棺0x0204������Ƶ�ʣ�1hz  ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[1];
        __packed struct
        {
            uint8_t power_rune_buff; /*bit 0��������Ѫ����Ѫ״̬
			                              bit 1��ǹ��������ȴ����
				                          bit 2�������˷����ӳ�
				                          bit 3�������˹����ӳ�
				                                      ���� bit ����*/
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_buff_musk_t;

//14. ���л���������״̬��0x0205������Ƶ�ʣ�10Hz   ֻ�п��л��������ط���
typedef struct
{
    union
    {
        uint8_t dataBuff[3];
        __packed struct
        {
            uint8_t energy_point; //���۵�������
            uint8_t attack_time;  //�ɹ���ʱ�� ��λ s��30s �ݼ��� 0
        };
    } data;
    uint8_t infoUpdateFlag;
} aerial_robot_energy_t;

//15. �˺�״̬��0x0206������Ƶ�ʣ��˺���������   ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[1];
        __packed struct
        {
            uint8_t armor_id : 4;
            uint8_t hurt_type : 4;
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_robot_hurt_t;

//16. ʵʱ�����Ϣ��0x0207������Ƶ�ʣ�������� ���ͷ�Χ����һ������
typedef struct
{
    union
    {
        uint8_t dataBuff[6];
        __packed struct
        {
            uint8_t bullet_type; //�ӵ����� 1:17mm 2:42mm
            uint8_t bullet_freq; //�ӵ���Ƶ  ��λhz
            float bullet_speed;  //�ӵ�����  ��λ m/s
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_shoot_data_t;

//17.�ӵ�ʣ�෢���� 0x0208  ����Ƶ�ʣ�1hz ���л����ˣ��ڱ��������Լ� ICRA ���������ط��ͣ����ͷ�Χ����һ�����ˡ�
typedef struct
{
    union
    {
        uint8_t dataBuff[2];
        __packed struct
        {
            uint16_t bullet_remaining_num;
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_bullet_remaining_t;

//18.������RFID״̬ ����Ƶ��:1hz
typedef struct
{
    union
    {
        uint8_t dataBuff[4];
        __packed struct
        {
            uint32_t rfid_status; //�������ϵͳЭ��P17
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_rfid_status_t;

//19.���ڻ����˿ͻ���ָ������   0x020A ����Ƶ��:10hz
typedef struct
{
    union
    {
        uint8_t dataBuff[12];
        __packed struct
        {
            uint8_t dart_launch_opening_status; //��ǰ���ڷ����״̬ 0 �ر� 1���ڿ������߹ر� 2 �Ѿ�����
            uint8_t dart_attack_target;         //���ڵĴ��Ŀ�� Ĭ��Ϊǰ��վ 1��ǰ��վ 2������
            uint16_t target_change_time;        //�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ�룬��δ�л�Ĭ��Ϊ0��
            uint8_t first_dart_speed;           //��⵽�ĵ�һö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ0��
            uint8_t second_dart_speed;          //��⵽�ĵڶ�ö�����ٶȣ���λ 0.1m/s/LSB��δ�����Ϊ0��
            uint8_t third_dart_speed;           //��⵽�ĵ���ö�����ٶȣ���λ 0.1m /s/LSB��δ�����Ϊ0��
            uint8_t fourth_dart_speed;          //��⵽�ĵ���ö�����ٶȣ���λ 0.1m/s/LSB��δ�����Ϊ0��
            uint16_t last_dart_launch_time;     //���һ�εķ�����ڵı���ʣ��ʱ�䣬��λ�룬��ʼֵΪ0��
            uint16_t operate_launch_cmd_time;   //���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��,��ʼֵΪ0��
        };
    } data;
    uint8_t infoUpdateFlag;
} ext_dart_client_cmd_t;

//�������Զ������ݣ�������ʾ�ڲ������棬��Ƶ 10Hz CmdID 0x0100
typedef struct
{
    union
    {
        uint8_t dataBuff[13];
        struct
        {
            float data1;
            float data2;
            float data3;
            uint8_t mask;
        };
    } data;
    uint8_t infoUpdateFlag;

} Judge_SendPC_t;

/* �Զ���֡ͷ */
typedef __packed struct
{
    uint8_t SOF;
    uint16_t DataLength;
    uint8_t Seq;
    uint8_t CRC8;

} xFrameHeader;

/* �������ݽ�����Ϣ��0x0301  */
typedef __packed struct
{
    uint16_t data_cmd_id;
    uint16_t send_ID;
    uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

/* 
	�ͻ��� �ͻ����Զ������ݣ�cmd_id:0x0301������ ID:0xD180
	����Ƶ�ʣ����� 10Hz


	1.	�ͻ��� �ͻ����Զ������ݣ�cmd_id:0x0301������ ID:0xD180������Ƶ�ʣ����� 10Hz 
	�ֽ�ƫ���� 	��С 	˵�� 				��ע 
	0 			2 		���ݵ����� ID 		0xD180 
	2 			2 		���ߵ� ID 			��ҪУ�鷢���߻����˵� ID ��ȷ�� 
	4 			2 		�ͻ��˵� ID 		ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� 
	6 			4 		�Զ��帡������ 1 	 
	10 			4 		�Զ��帡������ 2 	 
	14 			4 		�Զ��帡������ 3 	 
	18 			1 		�Զ��� 8 λ���� 4 	 

*/
typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t masks;
} client_custom_data_t;

typedef __packed struct
{
    uint8_t data[10]; //���ݶ�,n��ҪС��113
} robot_interactive_data_t;

//�����˽�����Ϣ
typedef __packed struct
{
    xFrameHeader txFrameHeader;                            //֡ͷ
    uint16_t CmdID;                                        //������
    ext_student_interactive_header_data_t dataFrameHeader; //���ݶ�ͷ�ṹ
    robot_interactive_data_t interactData;                 //���ݶ�
    uint16_t FrameTail;                                    //֡β
} ext_CommunatianData_t;

//֡ͷ  ������   ���ݶ�ͷ�ṹ  ���ݶ�   ֡β
//�ϴ��ͻ���
typedef __packed struct
{
    xFrameHeader txFrameHeader;                            //֡ͷ
    uint16_t CmdID;                                        //������
    ext_student_interactive_header_data_t dataFrameHeader; //���ݶ�ͷ�ṹ
    client_custom_data_t clientData;                       //���ݶ�
    uint16_t FrameTail;                                    //֡β
} ext_SendClientData_t;

typedef struct
{
    union
    {
        uint8_t dataBuff[13];
        struct
        {
            float data1;
        };
    } data;
    uint8_t infoUpdateFlag;

} Judge_SendRobot_t;

extern ext_game_state_t ext_game_state;
extern ext_game_result_t ext_game_result;
extern ext_game_robot_HP_t ext_game_robot_HP;
extern ext_dart_status_t ext_dart_status;
extern ext_ICRA_buff_debuff_zone_status_t ext_ICRA_buff_debuff_zone_status;
extern ext_supply_projectile_action_t ext_supply_projectile_action;
extern ext_referee_warning_t ext_referee_warning;
extern ext_dart_remaining_time_t ext_dart_remaining_time;
extern ext_game_robot_state_t ext_game_robot_state;
extern ext_power_heat_data_t ext_power_heat_data;
extern ext_game_robot_pos_t ext_game_robot_pos;
extern ext_buff_musk_t ext_buff_musk;
extern aerial_robot_energy_t aerial_robot_energy;
extern ext_robot_hurt_t ext_robot_hurt;
extern ext_shoot_data_t ext_shoot_data;
extern ext_bullet_remaining_t ext_bullet_remaining;
extern ext_rfid_status_t ext_rfid_status;
extern ext_dart_client_cmd_t ext_dart_client_cmd;
extern Judge_Monitor_t Judge_Monitor;
extern Judge_SendPC_t Judge_SendData;
extern Judge_SendRobot_t Judge_SendRobot;
extern uint8_t JudgeSystem_rxBuff[JUDGESYSTEM_PACKSIZE];
extern RM_Judge_t RM_Judge;

void JudgeSystem_Init(void);
void Judge_getInfo(uint16_t dataLength);
void Judge_sendPC(void);
void Judge_sendRobot(uint16_t cmd);
_Bool is_red_or_blue(void);
void determine_ID(void);

/*-------------------------------2021------------------------------------*/
#elif JUDGE_VERSION == JUDGE_21
//2021 ����ϵͳ�ٷ��ӿ�Э��

//��Ӧͨ��Э���ʽ   frame_header(5-byte)+cmd_id(2-byte)+data(n-byte)+frame_tail(2-byte,CRC16,����У��)
#define       LEN_HEADER        5/*frame_header*/
#define       LEN_CMDID         2/*cmd_id*/
#define       LEN_TAIL          2/*frame_tail*/

//��ʼ�ֽڣ�Э��̶�Ϊ0xA5
#define      JUDGE_FRAME_HEADER            0xA5
#define      JUDGESYSTEM_PACKSIZE 		    389u		//����ϵͳ����С(354+35)

//������
#define BLUE 0
#define RED  1

//��Ϣ����
#define TRUE 1
#define FALSE 0


/***********************************������ID*************************************/
/*

ID: 0x0001          Byte: 11      ����״̬����                   ����Ƶ�� 1Hz
ID: 0x0002          Byte: 1       �����������                   ������������
ID: 0x0003          Byte: 32      ����������Ѫ������             ����Ƶ�� 1Hz
ID: 0x0004          Byte: 3       ���ڷ���״̬                   ���ڷ������
ID: 0x0005          Byte: 11      �˹�������ս���ӳ���ͷ�       ����Ƶ�� 1Hz
ID: 0x0101          Byte: 4       �����¼�����                   �¼��ı����
ID: 0x0102          Byte: 4       ���ز���վ������ʶ����         �����ı����
//ID: 0X0103          Byte: 2       ���ز���վԤԼ�ӵ�����         �����ӷ��ͣ�����10Hz��RM�Կ���δ���ţ� 
ID: 0X0104          Byte: 2       ���о�������                   ���淢������
ID: 0x0105          Byte: 1       ���ڷ���ڵ���ʱ               ����Ƶ�� 1Hz
ID: 0X0201          Byte: 27      ������״̬����                 ����Ƶ�� 10Hz
ID: 0X0202          Byte: 16      ʵʱ������������               ����Ƶ�� 50Hz
ID: 0X0203          Byte: 16      ������λ������                 ����Ƶ�� 10Hz
ID: 0X0204          Byte: 1       ��������������                 ����״̬�ı����
ID: 0X0205          Byte: 2       ���л���������״̬����         ����Ƶ�� 10Hz��ֻ�п��л��������ط���
ID: 0X0206          Byte: 1       �˺�״̬����                   �˺���������
ID: 0X0207          Byte: 7       ʵʱ�������                   �ӵ��������
ID: 0X0208          Byte: 6       �ӵ�ʣ�෢����                 ���˻����ڱ�����  ����Ƶ�� 1Hz
ID: 0X0209          Byte: 4       ������RFID״̬                 ����Ƶ�� 1Hz
ID: 0x020A          Byte: 12      ���ڻ����˿ͻ���ָ����         ����Ƶ�� 10Hz
ID: 0X0301          Byte: n       �����˼佻������               ���ͷ���������    ����Ƶ�� 10Hz
ID: 0x0302          Byte: n       �Զ���������������ݽӿ�       �ͻ��˴�������    ����Ƶ�� 30Hz
ID: 0x0303          Byte: 15      �ͻ���С��ͼ��������           ��������
ID: 0x0304          Byte: 12      ���̡������Ϣ                 ͨ��ͼ�����ڷ���

*/

#define       Judge_Game_StatusData              0x0001 
#define       Judge_Game_ResultData              0x0002 
#define       Judge_Robot_HP                     0x0003 
#define       Judge_Dart_Launch                  0x0004
#define       Judge_AI_ChallengeBuff             0x0005
#define       Judge_Event_Data                   0x0101
#define       Judge_Supply_Station               0x0102
//#define       Judge_Request_Recharge             0x0103(�Կ���δ����)
#define       Judge_Referee_Warning              0x0104
#define       Judge_Dart_Countdown               0x0105
#define       Judge_Robot_State                  0x0201
#define       Judge_Power_Heat                   0x0202
#define       Judge_Robot_Position               0x0203
#define       Judge_Robot_Buff                   0x0204
#define       Judge_Aerial_Energy                0x0205
#define       Judge_Injury_State                 0x0206
#define       Judge_RealTime_Shoot               0x0207
#define       Judge_Remaining_Rounds             0x0208
#define       Judge_Robot_RFID                   0x0209
#define       Judge_Dart_Client                  0x020A
#define       Judge_Robot_Communicate            0x0301
#define       Judge_User_Defined                 0x0302
#define       Judge_Map_Interaction              0x0303
#define       Judge_KeyMouse_Message             0x0304
#define       Judge_Client_Map                   0x0305


/***************************DATA_Length*************************/
/*Calculation:frame_header(5-byte)+cmd_id(2-byte)+data(n-byte)+frame_tail(2-byte,CRC16)*/

#define       JudgeLength_Game_StatusData        20
#define       JudgeLength_Game_ResultData        10
#define       JudgeLength_Robot_HP               41
#define       JudgeLength_Dart_Launch            12
#define       JudgeLength_AI_ChallengeBuff       20
#define       JudgeLength_Event_Data             13
#define       JudgeLength_Supply_Station         13
//#define       JudgeLength_Request_Recharge       11(�Կ���δ����)
#define       JudgeLength_Referee_Warning        11
#define       JudgeLength_Dart_Countdown         10
#define       JudgeLength_Robot_State            36
#define       JudgeLength_Power_Heat             25
#define       JudgeLength_Robot_Position         25
#define       JudgeLength_Robot_Buff             10
#define       JudgeLength_Aerial_Energy          10
#define       JudgeLength_Injury_State           10
#define       JudgeLength_RealTime_Shoot         16
#define       JudgeLength_Remaining_Rounds       15
#define       JudgeLength_Robot_RFID             13
#define       JudgeLength_Dart_Client            21
#define       JudgeLength_Robot_Commute          35
#define       JudgeLength_Robot_Map              26

/**************************************�Զ���UI������ָʾ��Ϣ����*****************************************/

#define       LED_YELLOWMeaning                 "Fric:Manual"
#define       LED_GREENMeaning                  "Magazine:OFF"
#define       LED_ORANGEMeaning                 "ChassisMode:Normal"
#define       LED_PURPLEMeaning                 "VisionMode:None"
#define       LED_PINKMeaning                   "Pitch:Default"

#define       LED_CYANMeaning                   "Resever"
#define       LED_BLACKMeaning                  "SuperCap"
#define       LED_WHITEMeaning                  "Windmill"
#define       LED_Init                          "         "

extern int YellowFlag;
extern int GreenFlag;
extern int OrangeFlag;
extern int PurpleFlag;
extern int PinkFlag;
/**************************************����ϵͳ�ṹ��*****************************************/
/* �Զ���֡ͷ */
typedef __packed struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	
}xFrameHeader;

/* ID: 0x0001    Byte: 11     ����״̬���� */
typedef struct
{
  union {
		uint8_t dataBuff[11];
		__packed struct {
			uint8_t game_type : 4;             //��������
			uint8_t game_progress : 4;         //��ǰ�����׶�
			uint16_t stage_remain_time;        //��ǰ�׶�ʣ��ʱ��  ��λs
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_status_t;

/* ID: 0x0002    Byte: 1       ����������� */
typedef struct
{
	union{
	  uint8_t dataBuff[1];
		__packed struct
		{
			uint8_t winner;//0ƽ�� 1��ʤ 2��ʤ
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_game_result_t;

/* ID: 0x0003     Byte: 32     ����������Ѫ������ */
typedef struct
{
  union
	{
		uint8_t dataBuff[32];
		__packed struct
		{
	     uint16_t red_1_robot_HP;//�� 1 Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
       uint16_t red_2_robot_HP;//�� 2 ���̻�����Ѫ��
       uint16_t red_3_robot_HP;//�� 3 ����������Ѫ��
       uint16_t red_4_robot_HP;//�� 4 ����������Ѫ��
       uint16_t red_5_robot_HP;//�� 5 ����������Ѫ��
       uint16_t red_7_robot_HP;//�� 7 ����������Ѫ��
       uint16_t red_outpost_HP;//�췽ǰ��սѪ��
       uint16_t red_base_HP;//�췽����Ѫ��
       uint16_t blue_1_robot_HP;
       uint16_t blue_2_robot_HP; 
       uint16_t blue_3_robot_HP; 
       uint16_t blue_4_robot_HP; 
       uint16_t blue_5_robot_HP;
       uint16_t blue_7_robot_HP;
       uint16_t blue_outpost_HP;
       uint16_t blue_base_HP;			
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_game_robot_HP_t;

/* ID: 0x0004      Byte: 3    ���ڷ���״̬ */
typedef struct
{
	union
	{
		uint8_t dataBuff[3];
		__packed struct
		{
			uint8_t dart_belong;//������ڵĶ��飺1���췽����2����������
			uint16_t stage_remaining_time;//����ʱ��ʣ�����ʱ�䣬��λ s
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_dart_status_t;

/* ID: 0x0005      Byte: 11      �˹�������ս���ӳ���ͷ� */
typedef struct
{
	union
	{
		uint8_t dataBuff[11];
		__packed struct
		{
			uint8_t F1_zone_status:1;
			uint8_t F1_zone_buff_debuff_status:3;
			uint8_t F2_zone_status:1;
			uint8_t F2_zone_buff_debuff_status:3;
			uint8_t F3_zone_status:1;
			uint8_t F3_zone_buff_debuff_status:3; 
			uint8_t F4_zone_status:1;
			uint8_t F4_zone_buff_debuff_status:3; 
			uint8_t F5_zone_status:1;
			uint8_t F5_zone_buff_debuff_status:3;
			uint8_t F6_zone_status:1;
			uint8_t F6_zone_buff_debuff_status:3;
			uint16_t red1_bullet_left;
			uint16_t red2_bullet_left;
			uint16_t blue1_bullet_left;
			uint16_t blue2_bullet_left;
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_ICRA_buff_debuff_zone_status_t;

/* ID: 0x0101     Byte: 4       �����¼����� */
typedef struct
{
	union
	{
		uint8_t dataBuff[4];
		__packed struct
		{
			uint32_t event_type;
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_event_data_t;

/* ID: 0x0102     Byte: 4       ���ز���վ������ʶ���� */
typedef struct
{
  union
	{
		uint8_t dataBuff[4];
		__packed struct
		{
			uint8_t supply_projectile_id;  //����վ�� ID
			uint8_t supply_robot_id;       //���������� ID
			uint8_t supply_projectile_step;//�����ڿ���״̬ 
			uint8_t supply_projectile_num; //��������
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_supply_projectile_action_t;

/* ID: 0X0104          Byte: 2       ���о������� */
typedef struct
{
	union
	{
    uint8_t dataBuff[2];
	  __packed struct
	 {
		 uint8_t level;//����ȼ���
		 uint8_t foul_robot_id; //��������� ID���и�ʱ�������� ID Ϊ 0 ���ơ�����ʱ�������� ID Ϊ��������� ID
	 };
	}data;
	uint8_t InfoUpdataFlag;
}ext_referee_warning_t;

/* ID: 0x0105          Byte: 1       ���ڷ���ڵ���ʱ  */
typedef struct
{
	union
	{
		uint8_t dataBuff[1];
		__packed struct
		{
			uint8_t dart_remaining_time;//15s ����ʱ
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_dart_remaining_time_t;

/* ID: 0X0201          Byte: 27      ������״̬���� */
typedef struct
{
	union
	{
		uint8_t dataBuff[27];
		__packed struct
		{
		  uint8_t robot_id;
      uint8_t robot_level;
      uint16_t remain_HP;//������ʣ��Ѫ��
      uint16_t max_HP;//����������Ѫ��
      uint16_t shooter_id1_17mm_cooling_rate; //������ 1 �� 17mm ǹ��ÿ����ȴֵ
      uint16_t shooter_id1_17mm_cooling_limit;//������ 1 �� 17mm ǹ����������
      uint16_t shooter_id1_17mm_speed_limit;  //������ 1 �� 17mm ǹ�������ٶ� ��λ m/s
      uint16_t shooter_id2_17mm_cooling_rate;
      uint16_t shooter_id2_17mm_cooling_limit;
      uint16_t shooter_id2_17mm_speed_limit;
      uint16_t shooter_id1_42mm_cooling_rate;
      uint16_t shooter_id1_42mm_cooling_limit;
      uint16_t shooter_id1_42mm_speed_limit;
      uint16_t chassis_power_limit;           //�����˵��̹�����������
			/*���ص�Դ��������
       0 bit��gimbal ������� 1 Ϊ�� 24V �����0 Ϊ�� 24v �����
       1 bit��chassis �������1 Ϊ�� 24V �����0 Ϊ�� 24v �����
       2 bit��shooter �������1 Ϊ�� 24V �����0 Ϊ�� 24v �����*/
      uint8_t mains_power_gimbal_output : 1;
      uint8_t mains_power_chassis_output : 1;
      uint8_t mains_power_shooter_output : 1;	
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_game_robot_status_t;

/* ID: 0X0202          Byte: 16      ʵʱ������������ */
typedef struct
{
  union
	{
		uint8_t dataBuff[16];
		__packed struct
		{
			uint16_t chassis_volt; //���������ѹ ��λ ����
      uint16_t chassis_current; //����������� ��λ ����
      float chassis_power;//����������� ��λ W ��
      uint16_t chassis_power_buffer;//���̹��ʻ��� ��λ J ���� ��ע�����¸��ݹ��������� 250J
      uint16_t shooter_id1_17mm_cooling_heat;//ǹ������
      uint16_t shooter_id2_17mm_cooling_heat;
      uint16_t shooter_id1_42mm_cooling_heat;	
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_power_heat_data_t;

/* ID: 0X0203          Byte: 16      ������λ������  */
typedef struct
{
  union
	{
		uint8_t dataBuff[16];
		__packed struct
		{
		 float x;//λ�� x ����
     float y;//λ�� y ����
     float z;//λ�� z ����
     float yaw;//λ��ǹ��
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_game_robot_pos_t;

/* ID: 0X0204          Byte: 1       �������������� */
typedef struct
{
	union
	{
		uint8_t dataBuff[1];
		__packed struct
		{
			uint8_t power_rune_buff;/*bit 0��������Ѫ����Ѫ״̬
                                bit 1��ǹ��������ȴ����
                                bit 2�������˷����ӳ�
                                bit 3�������˹����ӳ�*/
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_buff_t;
/* ID: 0X0205          Byte: 2       ���л���������״̬���� */
typedef struct
{
	union
	{
		uint8_t dataBuff[2];
		__packed struct
		{
			uint8_t attack_time;//�ɹ���ʱ�� ��λ s��30s �ݼ��� 0
		};
	}data;
	uint8_t InfoUpdataFlag;
}aerial_robot_energy_t;

/* ID: 0X0206          Byte: 1       �˺�״̬���� */
typedef struct
{
  union
	{
		uint8_t dataBuff[1];
		__packed struct
		{
		  uint8_t armor_id : 4;/*bit 0-3����Ѫ���仯����Ϊװ���˺�������װ�� ID��������ֵΪ 0-4 �Ŵ�������˵�
                             ���װ��Ƭ������Ѫ���仯���ͣ��ñ�����ֵΪ 0��*/
      uint8_t hurt_type : 4;/*bit 4-7��Ѫ���仯����,0x0 װ���˺���Ѫ��
                              0x1 ģ����߿�Ѫ��
                              0x2 �����ٿ�Ѫ��
                              0x3 ��ǹ��������Ѫ��
                              0x4 �����̹��ʿ�Ѫ��
                              0x5 װ��ײ����Ѫ*/
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_robot_hurt_t;

/* ID: 0X0207          Byte: 7       ʵʱ������� */
typedef struct
{
  union
	{
		uint8_t dataBuff[7];
		__packed struct
		{
		  uint8_t bullet_type;//�ӵ�����: 1��17mm ���� 2��42mm ����
      uint8_t shooter_id;//������� ID
      uint8_t bullet_freq;//�ӵ���Ƶ ��λ Hz
      float bullet_speed;//�ӵ����� ��λ m/s
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_shoot_data_t;

/* ID: 0X0208          Byte: 6       �ӵ�ʣ�෢���� */
typedef struct
{
  union
	{
		uint8_t dataBuff[6];
		__packed struct
		{
		  uint16_t bullet_remaining_num_17mm;//17mm �ӵ�ʣ�෢����Ŀ
      uint16_t bullet_remaining_num_42mm;//42mm �ӵ�ʣ�෢����Ŀ
			uint16_t coin_remaining_num;//ʣ��������
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_bullet_remaining_t;

/* ID: 0X0209          Byte: 4       ������RFID״̬ */
/*bit 0����������� RFID ״̬��
bit 1���ߵ������ RFID ״̬��
bit 2���������ؼ���� RFID ״̬��
bit 3����������� RFID ״̬��
bit 4��ǰ�ڸ������ RFID ״̬��
bit 5����Դ������� RFID ״̬��
bit 6����Ѫ������� RFID ״̬��
bit 7�����̻����˲�Ѫ�� RFID ״̬��
RFID ״̬����ȫ�����Ӧ������򴦷�״̬������з���ռ��ĸߵ�����㣬����
��ȡ��Ӧ������Ч����*/
typedef struct
{
	union
	{
		uint8_t dataBuff[4];
		__packed struct
		{
			uint32_t rfid_status;
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_rfid_status_t;

/* ID: 0x020A          Byte: 12      ���ڻ����˿ͻ���ָ���� */
typedef struct
{
	union
	{
		uint8_t dataBuff[12];
		__packed struct
		{
		  uint8_t dart_launch_opening_status;//��ǰ���ڷ���ڵ�״̬
      uint8_t dart_attack_target;//���ڵĴ��Ŀ�꣬Ĭ��Ϊǰ��վ
      uint16_t target_change_time;//�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ�룬��δ�л�Ĭ��Ϊ 0��
      uint8_t first_dart_speed; //��⵽�ĵ�һö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ 0
      uint8_t second_dart_speed;//��⵽�ĵڶ�ö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ 0
      uint8_t third_dart_speed; //��⵽�ĵ���ö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ 0
      uint8_t fourth_dart_speed;//��⵽�ĵ���ö�����ٶȣ���λ 0.1m/s/LSB, δ�����Ϊ 0
      uint16_t last_dart_launch_time;//���һ�εķ�����ڵı���ʣ��ʱ�䣬��λ�룬��ʼֵΪ 0��
      uint16_t operate_launch_cmd_time;	//���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��, ��ʼֵΪ 0��
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_dart_client_cmd_t;


/* 

�������ݰ���һ��ͳһ�����ݶ�ͷ�ṹ��
���ݶΰ��������� ID���������Լ������ߵ� ID ���������ݶΣ�
�����������ݵİ��ܹ������Ϊ 128 ���ֽڣ�
��ȥ frame_header,cmd_id �� frame_tail �� 9 ���ֽ��Լ����ݶ�ͷ�ṹ�� 6 ���ֽڣ�
�ʶ����͵��������ݶ����Ϊ 113�� 
ÿ�������˽����������Զ�����������������кϼƴ������� 5000 Byte�������з���Ƶ�ʷֱ𲻳���
30Hz��

������ ID��
1��Ӣ��(��)��
2������(��)��
3/4/5������(��)��
6������(��)��
7���ڱ�(��)��
9���״�վ���죩��

101��Ӣ��(��)��
102������(��)��
103/104/105������(��)��
106������(��)��
107���ڱ�(��)��
109���״�վ��������

�ͻ��� ID��
0x0101 ΪӢ�۲����ֿͻ���(��)��
0x0102�����̲����ֿͻ���((��)��
0x0103/0x0104/0x0105�����������ֿͻ���(��)��
0x0106�����в����ֿͻ���((��)��

0x0165��Ӣ�۲����ֿͻ���(��)��
0x0166�����̲����ֿͻ���(��)��
0x0167/0x0168/0x0169�����������ֿͻ��˲���(��)��
0x016A�����в����ֿͻ���(��)

*/

/* 
  ���ڴ��ڶ������ ID�������� cmd_id ����Ƶ�����Ϊ 10Hz��
  ѧ�������˼�ͨ�� cmd_id 0x0301������ ID:0x0200~0x02FF
	�������� �����˼�ͨ�ţ�0x0301��
	����Ƶ�ʣ����� 10Hz  

	�ֽ�ƫ���� 	��С 	˵�� 			           ��ע 
	   0 			   2 		���ݵ����� ID 	 0x0200~0x02FF 
										                 ���������� ID ��ѡȡ������ ID �����ɲ������Զ��� 
	
	   2 			   2 		�����ߵ� ID 	   ��ҪУ�鷢���ߵ� ID ��ȷ�ԣ� 
	
	   4 			   2 		�����ߵ� ID 	   ��ҪУ������ߵ� ID ��ȷ�ԣ�
										                 ���粻�ܷ��͵��жԻ����˵�ID 
	
	   6 			   n 		���ݶ� 			     n ��ҪС�� 113 


*/

/* ID: 0X0301          Byte: n       �����˼佻������ */
typedef __packed struct
{
  uint16_t data_cmd_id;
  uint16_t sender_ID;
  uint16_t receiver_ID;
}ext_student_interactive_header_data_t;

/* data */
typedef __packed struct
{
	uint8_t data[20];//���ݶ�nС��113
}robot_interactive_data_t;

/* 
	�ͻ��� �ͻ����Զ������ݣ�cmd_id:0x0301������ ID:0x0100   0x0101  0x0102  0x0103  0x0110  0x0104
	����Ƶ�ʣ����� 10Hz


*/

/* �ͻ���ɾ��ͼ�� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  uint8_t operate_tpye; 
  uint8_t layer; 
}ext_client_custom_graphic_delete_t;

/* ͼ������ */
typedef __packed struct
{ 
  uint8_t graphic_name[3]; 
  uint32_t operate_tpye:3; 
  uint32_t graphic_tpye:3; 
  uint32_t layer:4; 
  uint32_t color:4; 
  uint32_t start_angle:9;
  uint32_t end_angle:9;
  uint32_t width:10; 
  uint32_t start_x:11; 
  uint32_t start_y:11; 
  uint32_t radius:10; 
  uint32_t end_x:11; 
  uint32_t end_y:11; 
}graphic_data_struct_t;

typedef __packed struct
{ 
  uint8_t graphic_name[3]; 
  uint32_t operate_tpye:3; 
  uint32_t graphic_tpye:3; 
  uint32_t layer:4; 
  uint32_t color:4; 
  uint32_t start_angle:9;
  uint32_t end_angle:9;
  uint32_t width:10; 
  uint32_t start_x:11; 
  uint32_t start_y:11; 
	int32_t data;
}ClientData_struct_t;

/* �ͻ��˻���һ��ͼ�� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  graphic_data_struct_t grapic_data_struct;
}ext_client_custom_graphic_single_t;

/* �ͻ��˻��ƶ���ͼ�� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  graphic_data_struct_t grapic_data_struct[2];
}ext_client_custom_graphic_double_t;

/* �ͻ��˻������ͼ�� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  graphic_data_struct_t grapic_data_struct[5];
}ext_client_custom_graphic_five_t;

/* �ͻ��˻����߸�ͼ�� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  graphic_data_struct_t grapic_data_struct[7];
}ext_client_custom_graphic_seven_t;

/* �ͻ��˻����ַ� �����˼�ͨ�ţ�0x0301 */
typedef __packed struct
{
  graphic_data_struct_t grapic_data_struct;
  char data[30];
}ext_client_custom_character_t;


/*�״�վĿǰ���뷨����������Ҫ�ÿͻ���С��ͼ�·���Ϣ�ٽ��пͻ��˽�����Ϣ����Ȼ�����޷�ʵ��*/
/*С��ͼ����*/
/* �ͻ����·���Ϣ��0x0303 (15)*/
typedef struct
{
	union
	{
		uint8_t dataBuff[15];
		__packed struct
		{
			float target_position_x;
			float target_position_y;
			float target_position_z;
			uint8_t commd_keyboard;
			uint16_t target_robot_ID;
		};
	}data;
	uint8_t InfoUpdataFlag;
}ext_robot_command_t;

//typedef __packed struct
//{
//	xFrameHeader   							txFrameHeader;//֡ͷ
//	uint16_t								CmdID;//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	ext_robot_command_t  	 			interactData;//���ݶ�
//	uint16_t		 						FrameTail;//֡β
//}ext_MapCommunate_t;

/*�ͻ��˽�����Ϣ:0x0305*/
//typedef struct
//{
//	union
//	{
//		uint8_t dataBuff[10];
//		__packed struct
//		{
//			uint16_t target_robot_ID;
//			float target_position_x;
//			float target_position_y;
//		};
//	}data;
//	uint8_t InfoUpdataFlag;
//}ext_client_map_command_t;
typedef __packed struct
{
	uint16_t target_robot_ID;
	float target_position_x;
	float target_position_y;
	float reserved;
}ext_client_map_command_t;

typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t								CmdID;//������
	//ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_map_command_t  	 			interactData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_MapCommunate_t;

/* ͼ��ң�ؿͻ����·���Ϣ:0x0304*/
typedef __packed struct
{
  int16_t mouse_x;
  int16_t mouse_y;
  int16_t mouse_z;
  int8_t left_button_down;
  int8_t right_button_down;
  uint16_t keyboard_value;
  uint16_t reserved;
}ext_robot_keycommand_t;


/*�����˽�����Ϣ��0x0301*/
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t								CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	robot_interactive_data_t  	 			interactData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_CommunatianData_t;

//֡ͷ  ������   ���ݶ�ͷ�ṹ  ���ݶ�   ֡β

/*�ͻ��˽ṹ��*/
//�ϴ��ͻ���
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	graphic_data_struct_t cilentData[7];//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_SendClientData_t;

typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	graphic_data_struct_t cilentData[5];//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_ShowCrossHair_t;

//ɾ���ͻ���
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_graphic_delete_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_DeleteClientData_t;

//���ٵȼ��ϴ��ṹ��
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_ShootLevelData_t;

//����ģ���ϴ�
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_Cap_Energyvalue_t;

typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_Cap_Energy_t;

//̧���߶��ϴ��ṹ��
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_UpliftHeightData_t;

//���ӽǶȽṹ��
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_ClipAngeleData_t;

//������ϴ�
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	graphic_data_struct_t cilentData[5];//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_CarDistance_t;

//�ڱ�״ָ̬ʾ�ṹ��
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_SentryStatusData_t;

//����״ָ̬ʾ�ṹ��
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_DartStatusData_t;

//������ָʾ��Ϣ
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//֡ͷ
	uint16_t		 						CmdID;//������
	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_custom_character_t cilentData;//���ݶ�
	uint16_t		 						FrameTail;//֡β
}ext_LedMeaning_t;

typedef struct {
	union {
		uint8_t dataBuff[13];
		struct {
			float data;
		};
	}data;
	uint8_t InfoUpdateFlag;
}Judge_SendRobot_t;


/*�����Է���  ������*/
void Judge_sendRobot(uint16_t cmd);




/*���͸��ڱ�(����һ̨��������)*/
void Judge_SendSentry(uint16_t);
/*���͸����ڷ����(���޿��л����˵���)*/
void Judge_SendDart(uint16_t);
/*���Ͷ������жϺ������ٽ��з��ͣ�*/
void Send_Infantry(void);
void Send_Hero(void);
void Send_Sentry(void);
void Send_Engineering(void);
void Send_Dart(void);
void Send_Radar(void);
void determine_ID(void);
_Bool is_red_or_blue(void);


//�ϴ���PC��
/*ɾ�����пͻ���*/
void UI_DeleteAll(void);
/*��׼��*/
void Show_CrossHair(void);
/*���ٵȼ�*/
void ShootLv_show(void);
void ShootLv_Updata(void);
/*��������ģ��*/
void CapEnergy_Show(void);
void Cap_EnergyUpdata(void);
/*�����ͼ*/
void Car_DistanceFrame(void);
/*����̧���߶�*/
void UpliftHeight_Show(void);
void UpliftHeight_Updata(void);
/*����̧���Ƕ�*/
void ClipAngle_Show(void);
void Clip_AngleUpdata(void);
/*�ڱ�״̬��ʾ*/
void SentryStatus_Show(void);
void SentryStatus_Updata(void);
/*����״ָ̬ʾ*/
void DartStatus_Show(void);
void DartStatus_Updata(void);

/**********������״ָ̬ʾ��************/
/**��ɫ**/
void RobotStatus_LEDYellow(void);
void RobotStatus_YellowDelete(void);
void LEDYellow_Mean(void);
void LEDYellow_MeanDelete(void);
/*��ɫ�ܿ���*/
void Yellow_ShowOn(void);
void LEDYellow_MeanUpdate(void);
void Yellow_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDGreen(void);
void RobotStatus_GreenDelete(void);
void LEDGreen_Mean(void);
void LEDGreen_MeanDelete(void);
/*��ɫ�ܿ���*/
void Green_ShowOn(void);
void LEDGreen_MeanUpdate(void);
void Green_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDOrange(void);
void RobotStatus_OrangeDelete(void);
void LEDOrange_Mean(void);
void LEDOrange_MeanDelete(void);
/*��ɫ�ܿ���*/
void Orange_ShowOn(void);
void LEDOrange_MeanUpdate(void);
void Orange_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDPurple(void);
void RobotStatus_PurpleDelete(void);
void LEDPurple_Mean(void);
void LEDPurple_MeanDelete(void);
/*��ɫ�ܿ���*/
void Purple_ShowOn(void);
void LEDPurple_MeanUpdate(void);
void Purple_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDPink(void);
void RobotStatus_PinkDelete(void);
void LEDPink_Mean(void);
void LEDPink_MeanDelete(void);
/*��ɫ�ܿ���*/
void Pink_ShowOn(void);
void LEDPink_MeanUpdate(void);
void Pink_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDCyan(void);
void RobotStatus_CyanDelete(void);
void LEDCyan_Mean(void);
void LEDCyan_MeanDelete(void);
/*��ɫ�ܿ���*/
void Cyan_ShowOn(void);
void Cyan_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDBlack(void);
void RobotStatus_BlackDelete(void);
void LEDBlack_Mean(void);
void LEDBlack_MeanDelete(void);
/*��ɫ�ܿ���*/
void Black_ShowOn(void);
void Black_DeleteAll(void);
/**��ɫ**/
void RobotStatus_LEDWhite(void);
void RobotStatus_WhiteDelete(void);
void LEDWhite_Mean(void);
void LEDWhite_MeanDelete(void);
/*��ɫ�ܿ���*/
void White_ShowOn(void);
void White_DeleteAll(void);
/***************************************/

//С��ͼ������Ϣ  �ͻ��˽�����Ϣ 0x0305
void Client_MapSend(void);


void Judge_GetMessage(uint16_t Data_Length);

extern RM_Judge_t JudgeSystem;
extern Judge_SendRobot_t Judge_SendRobot;

extern ext_game_status_t      ext_game_status;
extern ext_game_result_t     ext_game_result;
extern ext_game_robot_HP_t   ext_game_robot_HP;
extern ext_dart_status_t     ext_dart_status;
extern ext_ICRA_buff_debuff_zone_status_t    ext_ICRA_buff_debuff_zone_status;
extern ext_event_data_t ext_even_data;
extern ext_supply_projectile_action_t  ext_supply_projectile_action;
extern ext_referee_warning_t         ext_referee_warning;
extern ext_dart_remaining_time_t     ext_dart_remaining_time;
extern ext_game_robot_status_t  ext_game_robot_state;
extern ext_power_heat_data_t  ext_power_heat_data;
extern ext_game_robot_pos_t  ext_game_robot_pos;
extern ext_buff_t Buff;
extern aerial_robot_energy_t  aerial_robot_energy;
extern ext_robot_hurt_t  ext_robot_hurt;
extern ext_shoot_data_t  ext_shoot_data;
extern ext_bullet_remaining_t  ext_bullet_remaining;
extern ext_rfid_status_t        ext_rfid_status;
extern ext_dart_client_cmd_t   ext_dart_client_cmd;
extern ext_CommunatianData_t     CommuData;		//����ͨ����Ϣ
extern ext_SendClientData_t      ShowData;			//�ͻ�����Ϣ

extern uint8_t JudgeSystem_rxBuff[JUDGESYSTEM_PACKSIZE];


extern uint8_t JudgeSystem_rxBuff[JUDGESYSTEM_PACKSIZE]; //??buff

#endif //�汾��
/*-----------------------------------------------------------------------*/

#endif //ͷ�ļ�
