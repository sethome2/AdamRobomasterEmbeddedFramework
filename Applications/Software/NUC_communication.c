/*
 * @Author: sethome
 * @Date: 2021-12-23 16:49:58
 * @LastEditTime: 2021-12-23 17:20:29
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\NUC_communication.h
 */
#include "NUC_communication.h"

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

int decodeSTM32(STM32_data_t *target, unsigned char rx_buff[], unsigned int len)
{
    if (len != sizeof(STM32_data_t))
        return -1;

    memcpy(target, rx_buff, sizeof(STM32_data_t));
    return 0;
}
int encodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len)
{
    if (len < sizeof(STM32_data_t))
        return -1;

    memcpy(tx_buff, target, sizeof(STM32_data_t));
    return 0;
}

int decodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len)
{
    if (len != sizeof(NUC_data_t))
        return -1;

    memcpy(target, rx_buff, sizeof(NUC_data_t));
    return 0;
}
int encodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len)
{
    if (len < sizeof(NUC_data_t))
        return -1;

    memcpy(tx_buff, target, sizeof(NUC_data_t));
    return 0;
}

//end of file
