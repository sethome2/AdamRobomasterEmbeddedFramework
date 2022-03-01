/*
 * @Author: sethome
 * @Date: 2021-12-06 10:27:58
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Hardware\USB_VirCom.c
 */
#include "usbd_cdc_if.h"
#include "USB_VirCom.h"
#include "NUC_communication.h"
#include "gimbal.h"
#include "shoot.h"

NUC_data_t fromNUC;
	
void VirCom_send(uint8_t data[], uint16_t len)
{
  CDC_Transmit_FS(data, len);
}

void VirCom_rev(uint8_t data[], uint16_t len)
{
	decodeNUC(&fromNUC,data,len);
	
	//gimbal_set(fromNUC.set_pitch, fromNUC.set_yaw);
	gimbal_set(fromNUC.set_pitch + gimbal.pitch.now ,fromNUC.set_yaw + gimbal.yaw.now);
	
	if(fromNUC.shot == 0)
			shoot.speed_level = SHOOT_STOP;
	else if(fromNUC.shot == 1)
	{
			shoot.speed_level = SHOOT_17;
	}
	else if(fromNUC.shot >= 2)
	{
			shoot_Bullets(fromNUC.shot - 1);
	}
}

