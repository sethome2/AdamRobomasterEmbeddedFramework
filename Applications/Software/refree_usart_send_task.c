#include "refree_usart_send_task.h"
#include "UART_data_transmit.h"
//#include "infantry_mode_tasks.h"
#include "main.h"
#include "cmsis_os.h"
//#include "CAN_receive.h"

//#include "bsp_usart.h"
//#include "detect_task.h"

#include "CRC8_CRC16.h"
#include "fifo.h"
#include "protocol.h"
#include "referee.h"

#define send_max_len     200
unsigned char CliendTxBuffer[send_max_len];

//��ֲ����

/**
  * @brief  �ϴ��Զ�������
  * @param  void
  * @retval void
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
  
void JUDGE_Show_Data0(void);
void JUDGE_Show_Data1(void);
void JUDGE_Show_Data2(void);
void char_ui_loop(void);

//void JUDGE_Show_dele_Data0(void);
//void JUDGE_Show_dele_Data1(void);
//void JUDGE_Show_dele_Data2(void);
void JUDGE_Show_dele_Data(uint8_t graphic_delete,uint8_t layer);



/**
  * @brief  ����ϵͳ��ͻ��˷��͵�freertos����
  * @param  void
  * @retval void
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */

/**
  * @brief  �ж��Լ�������
  * @param  void
  * @retval RED   BLUE
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
#define RED 0
#define BLUE 1
bool_t is_red_or_blue(void);
/**
  * @brief  �ж�����ID��ѡ��ͻ���ID
  * @param  void
  * @retval RED 0   BLUE 1
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
uint8_t get_client_id(void);
void referee_usart_send_task(void const * argument)
{ 
//      JUDGE_Show_Data0();
//  osDelay(100);
//      JUDGE_Show_Data1();
//  osDelay(100);
//      JUDGE_Show_Data2();
  char_ui_init();
   while(1)
    {
      osDelay(10);  
      char_ui_loop();
//          JUDGE_Show_Char_Data0();   
//      JUDGE_Show_Data0();   
//      osDelay(10);  
//      JUDGE_Show_Data1();
//      osDelay(10); 
//      JUDGE_Show_Data2();
//      osDelay(10);
//      char_ui_init();
//      osDelay(10); 
      
//      JUDGE_Show_Data0();
//      JUDGE_Show_Char_Data();
//       void JUDGE_Show_Char_Data(void);
//      JUDGE_Show_dele_Data0();
       //JUDGE_Show_Data0();
//     
//      osDelay(100);  
//      JUDGE_Show_Data1();
//      osDelay(100); 
//      JUDGE_Show_Data2();
//        JUDGE_Show_Data();
//        JUDGE_Show_Char_Data();  
    }
}

/**
  * @brief  �ϴ��Զ�������
  * @param  void
  * @retval void
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
  
// void JUDGE_Show_Data(void)
//{
//	static uint8_t datalength,i;
//	ext_SendClientData_t ShowData;
//  
//	/*֡ͷ*/
//	ShowData.FrameHeader.SOF = 0xA5;
//	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_custom_graphic_seven_t);//��С��Ӧ
//	ShowData.FrameHeader.seq = 0;
//	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
//	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
//	/*������*/
//	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
//	/*���ݶ�ͷ�ṹ*/
//	ShowData.dataFrameHeader.data_cmd_id = 0x103;//�����ͻ��˵�cmd���ͻ��˻���5��ͼ��
//	ShowData.dataFrameHeader.send_ID 	 = 0x3;//�����ߵ�ID ��������
//	ShowData.dataFrameHeader.receiver_ID = 0x103;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� ���������ͻ���
//	/*���ݶ�*/  
//  //��0��
//  ShowData.graphdata.grapic_data_struct[0].graphic_name[0] = 0;
//  ShowData.graphdata.grapic_data_struct[0].graphic_name[1] = 0;
//	ShowData.graphdata.grapic_data_struct[0].graphic_name[2] = 0;
//	ShowData.graphdata.grapic_data_struct[0].operate_tpye = 1;
//	ShowData.graphdata.grapic_data_struct[0].graphic_tpye =1;
//	ShowData.graphdata.grapic_data_struct[0].layer = 0;
//	ShowData.graphdata.grapic_data_struct[0].color = 2;
//	ShowData.graphdata.grapic_data_struct[0].width = 6;
//	ShowData.graphdata.grapic_data_struct[0].start_x = 550;
//	ShowData.graphdata.grapic_data_struct[0].start_y =550;
//	ShowData.graphdata.grapic_data_struct[0].end_x =700;
//	ShowData.graphdata.grapic_data_struct[0].end_y =700;	
//  //��1��
////  ShowData.graphdata.grapic_data_struct[0].graphic_name[0] = 0;
////  ShowData.graphdata.grapic_data_struct[0].graphic_name[1] = 0;
////	ShowData.graphdata.grapic_data_struct[0].graphic_name[2] = 0;
////	ShowData.graphdata.grapic_data_struct[0].operate_tpye = 1;
////	ShowData.graphdata.grapic_data_struct[0].graphic_tpye = 5;
////	ShowData.graphdata.grapic_data_struct[0].layer = 1;
////	ShowData.graphdata.grapic_data_struct[0].color = 2;
////	ShowData.graphdata.grapic_data_struct[0].width = 6;
////	ShowData.graphdata.grapic_data_struct[0].start_x = 550;
////	ShowData.graphdata.grapic_data_struct[0].start_y =550;
////	ShowData.graphdata.grapic_data_struct[0].end_x =700;
////	ShowData.graphdata.grapic_data_struct[0].end_y =700;
////  float floatdata = 1.23f;
////  uint32_t tempdata = (uint32_t)(floatdata);
////  ShowData.graphdata.grapic_data_struct[0].radius = floatdata;
////  ShowData.graphdata.grapic_data_struct[0].end_x = floatdata;
////  ShowData.graphdata.grapic_data_struct[0].end_y = floatdata;
//  
//	//���д�����ݶ�
//	memcpy(	
//			CliendTxBuffer + 5, 
//			(uint8_t*)&ShowData.CmdID, 
//			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.graphdata))
//		  );			
//			
//	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
//  //DMA���� 
//	datalength = sizeof(ShowData);
//  //usart6_tx_dma_enable(CliendTxBuffer, datalength);  
//     
//} 
/**
  * @brief  �ϴ��Զ�������
  * @param  void
  * @retval void
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
  
 void JUDGE_Show_Char_Data(uint8_t index ,char *strdata, uint8_t operate_tpye, uint8_t layer,uint8_t color,uint8_t width, uint8_t size, uint8_t start_x ,uint8_t start_y)
{
	static uint8_t datalength;
	ext_SendClientChraterData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_custom_character_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x0110;//�����ͻ��˵�cmd���ͻ��˻����ַ�
	ShowData.dataFrameHeader.send_ID 	 = SEND_ID;//�����ߵ�ID ��������
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� ���������ͻ���
	/*���ݶ�*/  
  //��0��
  ShowData.character.grapic_data_struct.graphic_name[0]= 0;
  ShowData.character.grapic_data_struct.graphic_name[1] = index;
  ShowData.character.grapic_data_struct.graphic_name[2] = 0;
  ShowData.character.grapic_data_struct.operate_tpye = operate_tpye;
	ShowData.character.grapic_data_struct.graphic_tpye =7;
	ShowData.character.grapic_data_struct.layer = layer;
	ShowData.character.grapic_data_struct.color = color;
	ShowData.character.grapic_data_struct.width = width;
	ShowData.character.grapic_data_struct.start_angle = size;
	ShowData.character.grapic_data_struct.end_angle = strlen(strdata);
	ShowData.character.grapic_data_struct.start_x =start_x;
	ShowData.character.grapic_data_struct.start_y =start_x;
  memset((char *)ShowData.character.data, 0, sizeof(ShowData.character.data[0])*30);
  snprintf((char *)ShowData.character.data, 29, "%s", strdata);
  
//  memcpy(ShowData.character.data,strdata,sizeof(strdata)+1);

 
  
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.character))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  ////usart6_tx_dma_enable(CliendTxBuffer, datalength);  
	UART_send_data(UART6_data,CliendTxBuffer,datalength);     
} 

void JUDGE_Show_Char_Data0(uint8_t operate_tpye,uint8_t color)
{
	static uint8_t datalength;
	ext_SendClientChraterData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_custom_character_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x0110;//�����ͻ��˵�cmd���ͻ��˻����ַ�
	ShowData.dataFrameHeader.send_ID 	 = SEND_ID;//�����ߵ�ID ��������
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� ���������ͻ���
	/*���ݶ�*/  
  //��0��
  char strdata[] = "EDGE";
  ShowData.character.grapic_data_struct.graphic_name[0]= 0;
  ShowData.character.grapic_data_struct.graphic_name[1] = 0;
  ShowData.character.grapic_data_struct.graphic_name[2] = 0;
  ShowData.character.grapic_data_struct.operate_tpye = operate_tpye;
	ShowData.character.grapic_data_struct.graphic_tpye =7;
	ShowData.character.grapic_data_struct.layer = 3;
	ShowData.character.grapic_data_struct.color = color;
	ShowData.character.grapic_data_struct.width = 10;
	ShowData.character.grapic_data_struct.start_angle = 50;
	ShowData.character.grapic_data_struct.end_angle = strlen(strdata);
	ShowData.character.grapic_data_struct.start_x =300;
	ShowData.character.grapic_data_struct.start_y =900;
  memset((char *)ShowData.character.data, 0, sizeof(ShowData.character.data[0])*30);
  snprintf((char *)ShowData.character.data, 29, "%s", strdata);
  
//  memcpy(ShowData.character.data,strdata,sizeof(strdata)+1);

 
  
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.character))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  ////usart6_tx_dma_enable(CliendTxBuffer, datalength);  
	UART_send_data(UART6_data,CliendTxBuffer,datalength);     
     
} 
void JUDGE_Show_Char_Data1(uint8_t operate_tpye,uint8_t color)
{
	static uint8_t datalength;
	ext_SendClientChraterData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_custom_character_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x0110;//�����ͻ��˵�cmd���ͻ��˻����ַ�
	ShowData.dataFrameHeader.send_ID 	 = SEND_ID;//�����ߵ�ID ��������
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� ���������ͻ���
	/*���ݶ�*/  
  //��0��
  char strdata[] = "TOP";
  ShowData.character.grapic_data_struct.graphic_name[0]= 2;
  ShowData.character.grapic_data_struct.graphic_name[1] = 0;
  ShowData.character.grapic_data_struct.graphic_name[2] = 0;
  ShowData.character.grapic_data_struct.operate_tpye = operate_tpye;
	ShowData.character.grapic_data_struct.graphic_tpye =7;
	ShowData.character.grapic_data_struct.layer = 4;
	ShowData.character.grapic_data_struct.color = color;
	ShowData.character.grapic_data_struct.width = 10;
	ShowData.character.grapic_data_struct.start_angle = 50;
	ShowData.character.grapic_data_struct.end_angle = strlen(strdata);
	ShowData.character.grapic_data_struct.start_x =300;
	ShowData.character.grapic_data_struct.start_y =800;
  memset((char *)ShowData.character.data, 0, sizeof(ShowData.character.data[0])*30);
  snprintf((char *)ShowData.character.data, 29, "%s", strdata);
  
//  memcpy(ShowData.character.data,strdata,sizeof(strdata)+1);

 
  
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.character))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  //usart6_tx_dma_enable(CliendTxBuffer, datalength);  
  UART_send_data(UART6_data,CliendTxBuffer,datalength);      
} 
void JUDGE_Show_Char_Data2(uint8_t operate_tpye,uint8_t color)
{
	static uint8_t datalength;
	ext_SendClientChraterData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_custom_character_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x0110;//�����ͻ��˵�cmd���ͻ��˻����ַ�
	ShowData.dataFrameHeader.send_ID 	 = SEND_ID;//�����ߵ�ID ��������
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ��� ���������ͻ���
	/*���ݶ�*/  
  //��0��
  char strdata[] = "CAP";
  ShowData.character.grapic_data_struct.graphic_name[0]= 0;
  ShowData.character.grapic_data_struct.graphic_name[1] = 3;
  ShowData.character.grapic_data_struct.graphic_name[2] = 0;
  ShowData.character.grapic_data_struct.operate_tpye = operate_tpye;
	ShowData.character.grapic_data_struct.graphic_tpye =7;
	ShowData.character.grapic_data_struct.layer = 5;
	ShowData.character.grapic_data_struct.color = color;
	ShowData.character.grapic_data_struct.width = 10;
	ShowData.character.grapic_data_struct.start_angle = 50;
	ShowData.character.grapic_data_struct.end_angle = strlen(strdata);
	ShowData.character.grapic_data_struct.start_x =300;
	ShowData.character.grapic_data_struct.start_y =700;
  memset((char *)ShowData.character.data, 0, sizeof(ShowData.character.data[0])*30);
  snprintf((char *)ShowData.character.data, 29, "%s", strdata);
  
//  memcpy(ShowData.character.data,strdata,sizeof(strdata)+1);

 
  
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.character))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  ////usart6_tx_dma_enable(CliendTxBuffer, datalength);  
  UART_send_data(UART6_data,CliendTxBuffer,datalength);       
} 

void JUDGE_Show_Data0(void)
{
	static uint8_t datalength;
	ext_SendClientData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x101;//�����ͻ��˵�cmd���ͻ��˻���5��ͼ��
	ShowData.dataFrameHeader.send_ID = SEND_ID;//�����ߵ�ID
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ���
	/*���ݶ�*/
  ShowData.drawdata.graphic_name[0] = 0;
	ShowData.drawdata.graphic_name[1] = 1;
	ShowData.drawdata.graphic_name[2] = 0;
	ShowData.drawdata.operate_tpye = 1;
	ShowData.drawdata.graphic_tpye =1;
	ShowData.drawdata.layer = 0;
	ShowData.drawdata.color = 0;
	ShowData.drawdata.width = 2;
	ShowData.drawdata.start_x = 946;
	ShowData.drawdata.start_y =483;
	ShowData.drawdata.end_x =1013;
	ShowData.drawdata.end_y =417;	
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.drawdata))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  ////usart6_tx_dma_enable(CliendTxBuffer, datalength);  
  UART_send_data(UART6_data,CliendTxBuffer,datalength);       
}

void JUDGE_Show_Data1(void)
{
	static uint8_t datalength;
	ext_SendClientData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x101;//�����ͻ��˵�cmd���ͻ��˻���5��ͼ��
	ShowData.dataFrameHeader.send_ID = SEND_ID;//�����ߵ�ID
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ���
	/*���ݶ�*/
  ShowData.drawdata.graphic_name[0] = 0;
	ShowData.drawdata.graphic_name[1] = 0;
	ShowData.drawdata.graphic_name[2] = 1;
	ShowData.drawdata.operate_tpye = 1;
	ShowData.drawdata.graphic_tpye =1;
	ShowData.drawdata.layer = 1;
	ShowData.drawdata.color = 1;
	ShowData.drawdata.width = 2;
	ShowData.drawdata.start_x = 947;
	ShowData.drawdata.start_y =473;
	ShowData.drawdata.end_x =993;
	ShowData.drawdata.end_y =434;	
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.drawdata))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  ////usart6_tx_dma_enable(CliendTxBuffer, datalength);  
  UART_send_data(UART6_data,CliendTxBuffer,datalength);       
}

void JUDGE_Show_Data2(void)
{
	static uint8_t datalength;
	ext_SendClientData_t ShowData;
  
	/*֡ͷ*/
	ShowData.FrameHeader.SOF = 0xA5;
	ShowData.FrameHeader.data_length= sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t);//��С��Ӧ
	ShowData.FrameHeader.seq = 0;
	memcpy(CliendTxBuffer, &ShowData.FrameHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));//д��֡ͷCRC8У����
	/*������*/
	ShowData.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	/*���ݶ�ͷ�ṹ*/
	ShowData.dataFrameHeader.data_cmd_id = 0x101;//�����ͻ��˵�cmd���ͻ��˻���5��ͼ��
	ShowData.dataFrameHeader.send_ID = SEND_ID;//�����ߵ�ID
	ShowData.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ���
	/*���ݶ�*/
  ShowData.drawdata.graphic_name[0] = 0;
	ShowData.drawdata.graphic_name[1] = 1;
	ShowData.drawdata.graphic_name[2] = 1;
	ShowData.drawdata.operate_tpye = 1;
	ShowData.drawdata.graphic_tpye =1;
	ShowData.drawdata.layer = 2;
	ShowData.drawdata.color = 2;
	ShowData.drawdata.width = 2;
	ShowData.drawdata.start_x = 953;
	ShowData.drawdata.start_y =461;
	ShowData.drawdata.end_x =982;
	ShowData.drawdata.end_y =437;	
	//���д�����ݶ�
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.drawdata))
		  );			
			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(ShowData));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(ShowData);
  //usart6_tx_dma_enable(CliendTxBuffer, datalength);  
  UART_send_data(UART6_data,CliendTxBuffer,datalength);       
}

void JUDGE_Show_dele_Data(uint8_t graphic_delete, uint8_t layer)//uint8_t operta ,uint8_t layer 
{
  static uint8_t datalength;
  delete_data dele;
  dele.deleteHeader.SOF = 0xA5;
	dele.deleteHeader.data_length= sizeof(ext_student_interactive_header_data_t)+sizeof(graphic_delete_t);//��С��Ӧ
  dele.deleteHeader .seq  = 0;
	memcpy(CliendTxBuffer, &dele.deleteHeader, sizeof(frame_header_struct_t));//д��֡ͷ����
	append_CRC8_check_sum(CliendTxBuffer, sizeof(frame_header_struct_t));
  dele.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;//�����˼佻����������ID
	dele.dataFrameHeader .data_cmd_id  = 0x0100;//�����ͻ��˵�cmd��ɾ��ͼ��
	dele.dataFrameHeader.send_ID = SEND_ID;//�����ߵ�ID
	dele.dataFrameHeader.receiver_ID = RECEIVER_ID;//�ͻ��˵�ID��ֻ��Ϊ�����߻����˶�Ӧ�Ŀͻ���
  dele.graphic_delete.operate_tpye = graphic_delete;
  dele.graphic_delete.layer = layer;
  memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&dele.CmdID, 
			(sizeof(dele.CmdID)+ sizeof(dele.dataFrameHeader)+ sizeof(dele.graphic_delete ))
		  );			
	append_CRC16_check_sum(CliendTxBuffer,sizeof(dele));//д�����ݶ�CRC16У����	
  //DMA���� 
	datalength = sizeof(dele);
  //usart6_tx_dma_enable(CliendTxBuffer, datalength);  
	UART_send_data(UART6_data,CliendTxBuffer,datalength);     
}

/**
  * @brief  �ж��Լ�������
  * @param  void
  * @retval RED   BLUE
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
bool_t is_red_or_blue()
{
  const ext_game_robot_state_t *robot_state = get_robot_state_point();
  //uint8_t id= robot_state->robot_id;
	
	if(robot_state->robot_id > 10)
	{
		return BLUE;
	}
	else 
	{
		return RED;
	}
}

/**
  * @brief  �ж�����ID��ѡ��ͻ���ID
  * @param  void
  * @retval RED 0   BLUE 1
  * @attention  ���ݴ��,�����ɺ�ͨ�����ڷ��͵�����ϵͳ
  */
uint8_t get_client_id(void)
{
  bool_t color  = is_red_or_blue();
  static uint8_t client_id;
  uint8_t robot= get_robot_id();
	if(color == BLUE)
	{
		client_id = 0x0167 + (robot-103);//����ͻ���ID
    return client_id;
	}
	else if(color == RED)
	{
		client_id = 0x0100 + robot;//����ͻ���ID
    return client_id;
	}
	return 255;//modif by sethome
}

//(char *strdata, uint8_t operate_tpye, uint8_t layer,uint8_t color,uint8_t width, uint8_t size, uint8_t start_x ,uint8_t start_y)
void char_ui_init(void)
{
  JUDGE_Show_Char_Data0(1,4);
  osDelay(10); 
  JUDGE_Show_Char_Data1(1,4);
  osDelay(10); 
  JUDGE_Show_Char_Data2(1,4);
  osDelay(10); 
//  char str1[] = "EDGE";
//  char str2[] = "TOP";
//  char str3[] = "CAP";
//  JUDGE_Show_Char_Data(2,str1,1,3,4,10,50, (uint8_t)700,(uint8_t)700);
//  osDelay(10); 
//  JUDGE_Show_Char_Data(3,str2,1,5,4,10,50, (uint8_t)1474 ,(uint8_t)782);
//  osDelay(10); 
//  JUDGE_Show_Char_Data(4,str3,1,6,2,10,50, (uint8_t)1474 ,(uint8_t)856);
//  osDelay(10); 
}
void char_ui_loop(void)
{
	/*
   const super_cap_measure_t *super_cap_info = get_super_cap_measure_point();//��ȡ��������ָ��
    if(mode.TOP)
    {
      JUDGE_Show_Char_Data1(2,2);
    }else if(!mode.TOP)
    {
      JUDGE_Show_Char_Data1(2,4);
    }
    osDelay(10);
    if(mode.EDGE)
    {
      JUDGE_Show_Char_Data0(2,2);
    }
    else if(!mode.EDGE)
    {
      JUDGE_Show_Char_Data0(2,4);
    } 
    osDelay(10);
    if(super_cap_info->cap_voltage > 17.0f)
    {
      JUDGE_Show_Char_Data2(2,2);
    }
    else if(super_cap_info->cap_voltage > 12.0f)
    {
      JUDGE_Show_Char_Data2(2,1);
    }
    else
    {
      JUDGE_Show_Char_Data2(2,4);
    }
    osDelay(10);
*/
}

