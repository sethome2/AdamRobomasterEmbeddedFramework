/**
 * @file USB_VirCom.h
 * @author sethome
 * @brief 虚拟串口数据发送
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */
#define __USB_VIR_COM_H__
#ifdef __USB_VIR_COM_H__

#include "stdint.h"

void VirCom_send(uint8_t data[],uint16_t len);
void VirCom_rev(uint8_t data[],uint16_t len);

#endif
//end of file
