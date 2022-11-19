/**
 * @file Error_detect.c
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Error_detect.h"
#include "global_status.h"
#include "time.h"

struct Error_detect_t Error_detect;

void Error_detect_init(void)
{
}
void Error_detect_flush(void)
{
    if (Get_sys_time_ms() - Error_detect.remote.last_time > 1000)
		{
				Global_status.err[REMOTE_ERR] = 1;
        Error_detect.remote.flag = 1;
    }
		else
		{
				Global_status.err[REMOTE_ERR] = 0;
        Error_detect.remote.flag = 0;
		}
}

void Error_detect_motor(can_id ID)
{
    uint16_t tmp = get_motor_data(ID).given_current;
    if (Error_detect.motor.last_given_current[ID] == tmp)
        Error_detect.motor.err_cnt[ID]++;
    Error_detect.motor.last_given_current[ID] = tmp;

    if (Error_detect.motor.err_cnt[ID] > 2 && Error_detect.motor.flag[ID] == 0)
    {
        Error_detect.motor.err_cnt[ID] = 0;
        Error_detect.motor.flag[ID] = 1;

        Global_status_set_err(MOTOR_ERR, 1);
    }
    else
    {
        Error_detect.motor.flag[ID] = 0;

        Global_status_set_err(MOTOR_ERR, 0);
    }
}

void Error_detect_remote(void)
{
    Error_detect.remote.last_time = Get_sys_time_ms();
}
