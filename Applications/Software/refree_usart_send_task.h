#ifndef REFEREE_USART_SEND_TASK_H
#define REFEREE_USART_SEND_TASK_H
#include "main.h"

/*ºì·½5 0x0105*/
/*À¶·½105 0x0169*/
#define SEND_ID 5
#define RECEIVER_ID 0x0105


//void referee_usart_send_task(void const * argument);
extern void JUDGE_Show_Char_Data(uint8_t index ,char *strdata, uint8_t operate_tpye, uint8_t layer,uint8_t color,uint8_t width, uint8_t size, uint8_t start_x ,uint8_t start_y);
extern void JUDGE_Show_Char_Data0(uint8_t operate_tpye,uint8_t color);
extern void JUDGE_Show_Char_Data1(uint8_t operate_tpye,uint8_t color);
extern void JUDGE_Show_Char_Data2(uint8_t operate_tpye,uint8_t color);
extern void char_ui_init(void);

#endif
