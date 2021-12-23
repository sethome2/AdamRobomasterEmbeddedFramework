/*
 * @Author: sethome
 * @Date: 2021-12-06 10:27:58
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Hardware\USB_VirCom.c
 */
#include "usbd_cdc_if.h"
#include "USB_VirCom.h"

void VirCom_send(uint8_t data[], uint16_t len)
{
    CDC_Transmit_FS(data, len);
}

void VirCom_rev(uint8_t data[], uint16_t len)
{
    VirCom_send(data, len);
}

