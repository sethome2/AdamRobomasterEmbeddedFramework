/**
 * @file Error_detect.h
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ERROR_DETECT_H
#define ERROR_DETECT_H

#include "CAN_receive&send.h"

struct Error_detect_t
{
    struct
    {
        uint16_t last_given_current[CAN_2_6020_7];
        uint8_t err_cnt[CAN_2_6020_7];
        uint8_t flag[CAN_2_6020_7];
    } motor;

    struct
    {
        uint32_t last_time;
        uint8_t flag;
    } remote;
};
extern struct Error_detect_t Error_detect;

void Error_detect_init(void);
void Error_detect_flush(void);

void Error_detect_motor(can_id ID);
void Error_detect_remote(void);

#endif /* ERROR_DETECT_H */
