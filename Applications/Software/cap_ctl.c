#include "cap_ctl.h"
#include "CAN_receive&send.h"

#include "string.h"

cap_t cap;

typedef struct
{
	uint8_t setPower;
	uint16_t cacheEnergy;
}can_send_t;

extern CAN_HandleTypeDef hcan1;

void cap_handle_message(uint8_t data[8])
{
	static float rev_vol;
	
	memcpy(&rev_vol,data,sizeof(rev_vol));
	
	cap.remain_vol = rev_vol;
}

void cap_update()
{
	static uint8_t can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;
	
	tx_message.StdId = 0x006;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;
	
	can_send_t send_data;
	
	send_data.setPower = cap.set_max_power;
	send_data.cacheEnergy = cap.cache_energy;
	
	memcpy(can_send_data,&send_data,sizeof(can_send_t));
	
	HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
}

int cap_set_power(uint8_t set)
{
	if(set > 200 || set < 50)
		return -1;
	cap.set_max_power = set;
	cap_update();
	return 0;
}

float cap_get_remain_vol()
{
	return cap.remain_vol;
}

float cap_get_predict_energy()
{
	return cap.prediect_energy;
}
