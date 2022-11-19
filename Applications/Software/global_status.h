/**
 * @file global_status.h
 * @author sethome
 * @brief ȫ��״̬��
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright sethome Copyright (c) 2022
 *
 */

#include "stdint.h"

#define __GLOBAL_STATUS_H__
#ifdef __GLOBAL_STATUS_H__

// ������
enum err_e
{
    GIMBAL_ERR = 0,
    CHASSIS_ERR,
    SHOOT_ERR,
    CAP_ERR,
    VISION_ERR,
    REMOTE_ERR,
    PC_ERR,
    OTHER_ERR,
    MOTOR_ERR,
};

// �ʺ�ʮ������״̬�����
struct GlobalStatus_t
{
    // ��̨���Ӿ�״̬
    enum gimbal_visual_e
    {
        SHUTDOWN = 0,              // ͣ��
        GIMBAL_FLOW,               // ����ģʽ
        GIMBAL_HANDLNESS,          // ��ͷģʽ
        SELF_SIGHT_WITH_FLOW,      // ��������
        SELF_SIGHT_WITH_HANDLNESS, // ��ͷ����
        GYRO,                      // ����
        GYRO_WITH_SELF_SIGHT,      // С��������
    } gimbal_with_visual;

    enum cap_e
    {
        STOP = 0, // ��ʱ����Ӧ�ڳ��
        ACC,      // ���ڶ�ʱ����
        FULL,     //ȫ����Ӧ
    } cap;

    enum team_e
    {
        BLUE_TEAM = 0,
        RED_TEAM,
    } team;

    uint8_t err[8];
};

extern struct GlobalStatus_t Global_status;

void change_gimbal_visual_status(enum gimbal_visual_e in);
void change_cap_status(enum cap_e in);
enum gimbal_visual_e read_gimbal_visual_status(void);
enum cap_e read_cap_status(void);

void Global_status_set_err(enum err_e err, uint8_t status);

#endif
// end of file
