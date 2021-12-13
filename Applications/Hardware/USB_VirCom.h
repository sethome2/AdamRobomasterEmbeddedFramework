/*
 * @Author: sethome
 * @Date: 2021-12-06 10:28:15
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Hardware\USB_VirCom.h
 */
#define __USB_VIR_COM_H__
#ifdef __USB_VIR_COM_H__

#include "stdint.h"

void VirCom_send(uint8_t data[],uint16_t len);
void VirCom_rev(uint8_t data[],uint16_t len);

#endif
//end of file
