#include "Arm_Task.h"
#include "UART_data_transmit.h"
#include "stdio.h"
#include "stdlib.h"
#include "Stxxid-Data-Transmission.h"
#include "string.h"
#include "stdio.h"
#include "referee.h"
//DMA���Ʊ���
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;

//���ڿ��Ʊ���
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

//����������+DMA���ϣ�������������
transmit_data UART1_data;
transmit_data UART6_data;

//ĳStxxid-Data-Transmission��������ʱ���ܣ�
struct serial_data_set_t UART1;             //UART1�Ľ���״̬
struct serial_data_set_t UART6;             //UART2�Ľ���״̬     
extern struct database_t transmit_database; //�����������ݿ�


//��ʼ������
void uart_init(void)
{
  UART_DMA_rxtx_start(&UART1_data,&huart1, &hdma_usart1_rx, &hdma_usart1_tx);
  UART_DMA_rxtx_start(&UART6_data,&huart6, &hdma_usart6_rx, &hdma_usart6_tx);
}

//�d(?��?`)o��ܰ��ʾ��ʹ�� DMA + �жϿ��У����������ˡ������Ǳ��� �̶�256�ֽڣ���ʱû�и����Զ���Ĵ���
//DMA�������ж�����
void UART_DMA_rxtx_start(transmit_data *data, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_usart_tx)
{
  data->huart = huart;                                  //���ڿ��Ʊ���
  data->hdma_usart_rx = hdma_usart_rx;                  //DMA���ջ���
  data->hdma_usart_tx = hdma_usart_tx;                  //DMA���ͻ���
	
  HAL_UART_Receive_DMA(data->huart,data->rev_data,256); //ʹ��DMA

  __HAL_UART_ENABLE_IT(data->huart, UART_IT_RXNE);      //���������ж�
  __HAL_UART_ENABLE_IT(data->huart, UART_IT_IDLE);      //���������ж�
  __HAL_DMA_ENABLE(hdma_usart_rx);                      //����DMA����
}




//�������ݣ����ݱ��ͷ��ˣ���Ȼ�����ղ�����
void UART_send_data(transmit_data uart, uint8_t data[], uint16_t size)
{
  HAL_UART_Transmit_DMA(uart.huart, data, size);//���ީd(?��?`)o
}

//�����UARTx_IRQHandler�£���UARTȫ���жϺ���
//=.=��Ȼ�������HAL_UART_RxCpltCallbackҲ���ǲ���
void UART_rx_IRQHandler(transmit_data *uart)
{
  uint16_t len = 256;//�õ������ݳ���
	
  if (uart->huart->Instance->SR & UART_FLAG_RXNE) //���ܵ��ֽ��ж�
  {
    //�����Ҫ�Ե����ַ������ڴ˴�
  }
  else if (uart->huart->Instance->SR & UART_FLAG_IDLE) //���Ϊ�����ж�
  {
		HAL_UART_DMAStop(uart->huart); //ʧЧDMA
		
    len = 256 - __HAL_DMA_GET_COUNTER(uart->hdma_usart_rx);//�����õ��ֽڳ���
		
    //�˴����ô�������зз
    //�������� = uart->rev_data ,���� = len
    //�Է�����ʾ���� UART_send_data(UART1_data,uart->rev_data,len);
		
    if (uart->huart == &huart1)//����һ���ݴ���
    {
			UART_send_data(UART1_data,uart->rev_data,len);
			
      referee_data_solve(uart->rev_data);
		}
    else if (uart->huart == &huart6)//���ڶ�������˿ӡ�����ݴ���
    {
			UART_send_data(UART6_data,uart->rev_data,len);
		}

		//�������
		for(int p = 0;p < 256;p++)
			uart->rev_data[p] = 0;
		
		__HAL_UART_CLEAR_IDLEFLAG(uart->huart); //��������жϱ�־
		__HAL_UART_CLEAR_PEFLAG(uart->huart);   //����ȴ���־
		HAL_UART_Receive_DMA(uart->huart, uart->rev_data, 256);   //����ʹ��DMA������
  }
	
	__HAL_UART_CLEAR_PEFLAG(uart->huart);//����ȴ���־
}

//end of file
