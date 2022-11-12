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
    }team;
};

extern struct GlobalStatus_t Global_status;

void change_gimbal_visual_status(enum gimbal_visual_e in);
void change_cap_status(enum cap_e in);
enum gimbal_visual_e read_gimbal_visual_status(void);
enum cap_e read_cap_status(void);

#endif
//end of file
