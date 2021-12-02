/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "CAN_receive&send.h"
#include "DBUS_remote_control.h"
#include "LED_control.h"
#include "IMU_updata.h"
#include "chassis_move.h"
#include "PWM_control.h"
#include "Stxxid-Data-Transmission.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern struct IMU_t IMU_data;

/* USER CODE END Variables */
/* Definitions for ChassisTask */
osThreadId_t ChassisTaskHandle;
const osThreadAttr_t ChassisTask_attributes = {
  .name = "ChassisTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for RemoteTask */
osThreadId_t RemoteTaskHandle;
const osThreadAttr_t RemoteTask_attributes = {
  .name = "RemoteTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for SendCAN_Task */
osThreadId_t SendCAN_TaskHandle;
const osThreadAttr_t SendCAN_Task_attributes = {
  .name = "SendCAN_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for FlashLEDTask */
osThreadId_t FlashLEDTaskHandle;
const osThreadAttr_t FlashLEDTask_attributes = {
  .name = "FlashLEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ChassisTask_callback(void *argument);
void RemoteTask_callback(void *argument);
void SendCAN_Task_callback(void *argument);
void FlashLEDTask_callback(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ChassisTask */
  ChassisTaskHandle = osThreadNew(ChassisTask_callback, NULL, &ChassisTask_attributes);

  /* creation of RemoteTask */
  RemoteTaskHandle = osThreadNew(RemoteTask_callback, NULL, &RemoteTask_attributes);

  /* creation of SendCAN_Task */
  SendCAN_TaskHandle = osThreadNew(SendCAN_Task_callback, NULL, &SendCAN_Task_attributes);

  /* creation of FlashLEDTask */
  FlashLEDTaskHandle = osThreadNew(FlashLEDTask_callback, NULL, &FlashLEDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ChassisTask_callback */
/**
  * @brief  Function implementing the ChassisTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ChassisTask_callback */
void ChassisTask_callback(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN ChassisTask_callback */
  chassis_move_init();
	char a[]="Test\n";
  /* Infinite loop */
  for (;;)
  {
		CDC_Transmit_FS(a,5);
    osDelay(1000);
  }
  /* USER CODE END ChassisTask_callback */
}

/* USER CODE BEGIN Header_RemoteTask_callback */
/**
* @brief Function implementing the RemoteTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RemoteTask_callback */
void RemoteTask_callback(void *argument)
{
  /* USER CODE BEGIN RemoteTask_callback */
  
	for(;;)
	{
		
    osDelay(10);
	}
  /* USER CODE END RemoteTask_callback */
}

/* USER CODE BEGIN Header_SendCAN_Task_callback */
/**
* @brief Function implementing the SendCAN_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendCAN_Task_callback */
void SendCAN_Task_callback(void *argument)
{
  /* USER CODE BEGIN SendCAN_Task_callback */
  /* Infinite loop */
  for (;;)
  {
    //原子化任务
    //停止任务调度
    vTaskSuspendAll();

    CAN1_send_current();
    CAN2_send_current();

    //重启调度
    if (!xTaskResumeAll())
    {
      taskYIELD();
    }

    osDelay(5);
  }
  /* USER CODE END SendCAN_Task_callback */
}

/* USER CODE BEGIN Header_FlashLEDTask_callback */
/**
* @brief Function implementing the FlashLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FlashLEDTask_callback */
void FlashLEDTask_callback(void *argument)
{
  /* USER CODE BEGIN FlashLEDTask_callback */
	//用于检测系统是否卡死
  /* Infinite loop */
  for(;;)
  {
		#ifdef TEST_VERSION
			led_show(PINK);
		#endif
		#ifdef DEV_VERSION
			led_show(ORANGE);
		#endif
		#ifdef RELEASE_VERSION
			led_show(GREEN);
		#endif
    osDelay(80);
			led_show(BLANK);
		osDelay(80); 
  }
  /* USER CODE END FlashLEDTask_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
