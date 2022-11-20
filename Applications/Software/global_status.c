/**
 * @file global_status.c
 * @author sethome 
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "global_status.h"

struct GlobalStatus_t Global_status;

void change_gimbal_visual_status(enum gimbal_visual_e in)
{
    Global_status.gimbal_with_visual = in;
}

// 建议内联电容操作函数
void change_cap_status(enum cap_e in)
{
    Global_status.cap = in;
}

enum gimbal_visual_e read_gimbal_visual_status()
{
    return Global_status.gimbal_with_visual;
}

enum cap_e read_cap_status()
{
    return Global_status.cap;
}

void Global_status_set_err(enum err_e err, uint8_t status)
{
    Global_status.err[err] = status;
}

//end of file
