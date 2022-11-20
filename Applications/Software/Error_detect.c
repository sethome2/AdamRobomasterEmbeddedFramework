/**
 * @file Error_detect.c
 * @author sethome
 * @brief 错误检查
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Error_detect.h"
#include "global_status.h"
#include "Stm32_time.h"

struct Error_detect_t Error_detect;

void Error_detect_init(void)
{
}

void Error_detect_flush(void)
{
    // 遥控器检测
    static uint32_t last_remote = 0;
    if (last_remote == Error_detect.remote.last_time)
    {
        Global_status.err[REMOTE_ERR] = 1;
        Error_detect.remote.flag = 1;
    }
    else
    {
        Global_status.err[REMOTE_ERR] = 0;
        Error_detect.remote.flag = 0;
    }
    last_remote = Error_detect.remote.last_time;
		
		// 电机检测
		for (int i = 0;i < CAN_2_6020_7;i++)
		{
			if ( Error_detect.motor.flag[i] == 1)
			{
				Global_status_set_err(MOTOR_ERR, 1);
				break;
			}
			Global_status_set_err(MOTOR_ERR, 0);
		}
}

/**
 * @brief 电机错误检测，在FreeRTOS中调用
 * 
 * @param ID 
 */
void Error_detect_motor(can_id ID)
{
    uint16_t tmp = get_motor_data(ID).given_current;
    if (Error_detect.motor.last_given_current[ID] == tmp)
        Error_detect.motor.err_cnt[ID]++;
		else
		{
		    Error_detect.motor.err_cnt[ID] = 0;
				Error_detect.motor.flag[ID] = 0;
		}
		
    Error_detect.motor.last_given_current[ID] = tmp;
		
    if (Error_detect.motor.err_cnt[ID] > 3 && Error_detect.motor.flag[ID] == 0)
    {
        Error_detect.motor.err_cnt[ID] = 0;
        Error_detect.motor.flag[ID] = 1;
    }
}

void Error_detect_remote(void)
{
    Error_detect.remote.last_time++;
}
