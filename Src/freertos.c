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
#include "PWM_control.h"
#include "GPIO_output.h"

#include "TF_MINI_PLUS_LaserRanging.h"
//#include "guard_chassis.h" //哨兵底盘
#include "chassis_move.h" //普通底盘
#include "gimbal.h"
#include "shoot.h"
#include "math.h"
#include "USB_VirCom.h"
#include "cap_ctl.h"

#include "time.h"

#include "global_status.h"

#include "Arm.h"

#include "NUC_communication.h"

#include "MYACTUATOR_RMD_X.h"
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
/* Definitions for flashLED */
osThreadId_t flashLEDHandle;
const osThreadAttr_t flashLED_attributes = {
    .name = "flashLED",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for CAN_sendTask */
osThreadId_t CAN_sendTaskHandle;
const osThreadAttr_t CAN_sendTask_attributes = {
    .name = "CAN_sendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime7,
};
/* Definitions for FastTestTask */
osThreadId_t FastTestTaskHandle;
const osThreadAttr_t FastTestTask_attributes = {
    .name = "FastTestTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for RemoteTask */
osThreadId_t RemoteTaskHandle;
const osThreadAttr_t RemoteTask_attributes = {
    .name = "RemoteTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for ChassisTask */
osThreadId_t ChassisTaskHandle;
const osThreadAttr_t ChassisTask_attributes = {
    .name = "ChassisTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime1,
};
/* Definitions for gimbalTask */
osThreadId_t gimbalTaskHandle;
const osThreadAttr_t gimbalTask_attributes = {
    .name = "gimbalTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime6,
};
/* Definitions for NUCcontrolTask */
osThreadId_t NUCcontrolTaskHandle;
const osThreadAttr_t NUCcontrolTask_attributes = {
    .name = "NUCcontrolTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityAboveNormal7,
};
/* Definitions for errorDetectTask */
osThreadId_t errorDetectTaskHandle;
const osThreadAttr_t errorDetectTask_attributes = {
    .name = "errorDetectTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for statusTask */
osThreadId_t statusTaskHandle;
const osThreadAttr_t statusTask_attributes = {
    .name = "statusTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void flashLEDTask_callback(void *argument);
void CAN_sendTask_callback(void *argument);
void FastTestTask_callback(void *argument);
void RemoteTask_callback(void *argument);
void ChassisTask_callback(void *argument);
void gimbalTask_callback(void *argument);
void NUCcontrolTask_callback(void *argument);
void errorDetectTask_callback(void *argument);
void statusTask_callback(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
  /* creation of flashLED */
  flashLEDHandle = osThreadNew(flashLEDTask_callback, NULL, &flashLED_attributes);

  /* creation of CAN_sendTask */
  CAN_sendTaskHandle = osThreadNew(CAN_sendTask_callback, NULL, &CAN_sendTask_attributes);

  /* creation of FastTestTask */
  FastTestTaskHandle = osThreadNew(FastTestTask_callback, NULL, &FastTestTask_attributes);

  /* creation of RemoteTask */
  RemoteTaskHandle = osThreadNew(RemoteTask_callback, NULL, &RemoteTask_attributes);

  /* creation of ChassisTask */
  ChassisTaskHandle = osThreadNew(ChassisTask_callback, NULL, &ChassisTask_attributes);

  /* creation of gimbalTask */
  gimbalTaskHandle = osThreadNew(gimbalTask_callback, NULL, &gimbalTask_attributes);

  /* creation of NUCcontrolTask */
  NUCcontrolTaskHandle = osThreadNew(NUCcontrolTask_callback, NULL, &NUCcontrolTask_attributes);

  /* creation of errorDetectTask */
  errorDetectTaskHandle = osThreadNew(errorDetectTask_callback, NULL, &errorDetectTask_attributes);

  /* creation of statusTask */
  statusTaskHandle = osThreadNew(statusTask_callback, NULL, &statusTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_flashLEDTask_callback */
/**
 * @brief  Function implementing the flashLED thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_flashLEDTask_callback */
void flashLEDTask_callback(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN flashLEDTask_callback */
  /* Infinite loop */
  for (;;)
  {
    if (Global_status.team == RED_TEAM)
      led_show(RED);
    else if (Global_status.team == BLUE_TEAM)
      led_show(BLUE);

    osDelay(80);
    led_show(BLANK);
    osDelay(80);
  }
  /* USER CODE END flashLEDTask_callback */
}

/* USER CODE BEGIN Header_CAN_sendTask_callback */
/**
 * @brief Function implementing the CAN_sendTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_CAN_sendTask_callback */
void CAN_sendTask_callback(void *argument)
{
  /* USER CODE BEGIN CAN_sendTask_callback */
  /* Infinite loop */
  for (;;)
  {
    //原子化任务
    //停止任务调度
    vTaskSuspendAll();

    //CAN1_send_current();
    //CAN2_send_current();

    //重启调度
    if (!xTaskResumeAll())
    {
			taskYIELD();
    }

    osDelay(10);
  }
  /* USER CODE END CAN_sendTask_callback */
}

/* USER CODE BEGIN Header_FastTestTask_callback */
/**
 * @brief Function implementing the FastTestTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_FastTestTask_callback */
void FastTestTask_callback(void *argument)
{
  /* USER CODE BEGIN FastTestTask_callback */
  /* Infinite loop */
  for (;;)
  {
    // gimbal.pitch.set = RC_data.rc.ch[0] / 3.0;
		cap_update();
    osDelay(10);
  }
  /* USER CODE END FastTestTask_callback */
}

/* USER CODE BEGIN Header_RemoteTask_callback */
/**
 * @brief Function implementing the RemoteTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RemoteTask_callback */
float test1 = 0;
void RemoteTask_callback(void *argument)
{
  /* USER CODE BEGIN RemoteTask_callback */
  /* Infinite loop */

  float x_s = 0.0f, y_s = 0.0f, r_s = 0.0f; // 底盘参数

  // float pitch = 0.0f, yaw = 0.0f;

  float manual_d1 = 0, manual_d2 = 0;

  for (;;)
  {

    // Arm_status.a_d1.angle = RC_data.rc.ch[3] / 20.0f;
    // Arm_status.a_d2.angle = RC_data.rc.ch[4] / 8.0f;

    // 从遥控器给底盘赋值
    x_s = (float)RC_data.rc.ch[0] / 200.0f;
    y_s = (float)RC_data.rc.ch[1] / 200.0f;
    r_s = (float)RC_data.rc.ch[2] / 200.0f;

    if (Global_status.arm == AUTO) // 只有在更新时才响应
    {
      if (Global_status.location == get_GND)
        ARM_set_angle(17, 84);
      else if (Global_status.location == get_Station)
        ARM_set_angle(25, 19);
      else if (Global_status.location == put)
        ARM_set_angle(65, 49);

      //移动d2
      ARM_set_MAN_angle(0, RC_data.rc.ch[4] / 110);
			ARM_set_s_offset(0);
    }
    else if (Global_status.arm == MANUAL) // 手动模式下，底盘锁定
    {
      y_s = r_s = 0; // 锁定前进和旋转，左右维持
      ARM_set_MAN_angle(RC_data.rc.ch[1] / 110, -(RC_data.rc.ch[3] / 110));
			ARM_set_s_offset(RC_data.rc.ch[4] / 6.0f);
    }
		
		if(Global_status.pump == OFF)
			set_PIN_bool(2,0);
		else
			set_PIN_bool(2,1);
		
    // gimbal_set(pitch, yaw);
    chassis_moto_speed_calc(x_s, y_s, r_s);
    osDelay(10);
  }
  /* USER CODE END RemoteTask_callback */
}

/* USER CODE BEGIN Header_ChassisTask_callback */
/**
 * @brief Function implementing the ChassisTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ChassisTask_callback */
void ChassisTask_callback(void *argument)
{
  /* USER CODE BEGIN ChassisTask_callback */

  /* Infinite loop */
  for (;;)
  {
			osDelay(10);
  }
  /* USER CODE END ChassisTask_callback */
}

/* USER CODE BEGIN Header_gimbalTask_callback */
/**
 * @brief Function implementing the gimbalTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_gimbalTask_callback */
void gimbalTask_callback(void *argument)
{
  /* USER CODE BEGIN gimbalTask_callback */

  //工程机器为机械臂进程

  /* Infinite loop */
  for (;;)
  {

    ARM_update();
    //    gimbal_updata();
    //    gimbal_pid_cal();

    //    shoot_update();
    //    shoot_pid_cal();

    osDelay(5);
  }
  /* USER CODE END gimbalTask_callback */
}

/* USER CODE BEGIN Header_NUCcontrolTask_callback */
/**
 * @brief Function implementing the NUCcontrolTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_NUCcontrolTask_callback */
void NUCcontrolTask_callback(void *argument)
{
  /* USER CODE BEGIN NUCcontrolTask_callback */
  /* Infinite loop */
  STM32_data_t toNUC;
  unsigned char data[128];
  for (;;)
  {
    //和NUC py一下
    toNUC.mode = '1';
    if (Global_status.team == BLUE_TEAM)
      toNUC.Team = 1;

    encodeSTM32(&toNUC, data, 128);
    VirCom_send(data, sizeof(STM32_data_t));

    //和MATLAB py一下
    //		char matlab[128];
    //		unsigned char len = 0;
    //		len = sprintf(matlab,"%f %f %f %f %f %f %f %f %f",IMU_data.accel[0],IMU_data.accel[1],IMU_data.accel[2],IMU_data.gyro[0],IMU_data.gyro[1],IMU_data.gyro[2],IMU_data.mag[0],IMU_data.mag[1],IMU_data.mag[2]);
    //    VirCom_send(matlab,len);

    osDelay(10);
  }
  /* USER CODE END NUCcontrolTask_callback */
}

/* USER CODE BEGIN Header_errorDetectTask_callback */
/**
 * @brief Function implementing the errorDetectTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_errorDetectTask_callback */
void errorDetectTask_callback(void *argument)
{
  /* USER CODE BEGIN errorDetectTask_callback */
  /* Infinite loop */
  int err_cnt = 0;

  for (;;)
  {
    // 循环检测电机值，如果和上次不相同，则在线，遥控器同理

    osDelay(500);
    err_cnt = 0;
  }
  /* USER CODE END errorDetectTask_callback */
}

/* USER CODE BEGIN Header_statusTask_callback */
/**
 * @brief Function implementing the statusTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_statusTask_callback */
void statusTask_callback(void *argument)
{
  /* USER CODE BEGIN statusTask_callback */
  /* Infinite loop */
  int8_t last_rc_s1 = 0;
  for (;;)
  {
    // 在此切换状态

    // 如果s1的状态改变
    if (last_rc_s1 != RC_data.rc.s[1])
    {
      if (switch_is_down(RC_data.rc.s[1]))
        Global_status.location = get_GND;
      else if (switch_is_mid(RC_data.rc.s[1]))
        Global_status.location = get_Station;
      else if (switch_is_up(RC_data.rc.s[1]))
        Global_status.location = put;
    }
    else
      Global_status.location = STAND_BY;
    last_rc_s1 = RC_data.rc.s[1];

    if (switch_is_up(RC_data.rc.s[0]))
    {
      Global_status.arm = AUTO;
      Global_status.pump = ON;
    }
    else if (switch_is_mid(RC_data.rc.s[0]))
    {
      Global_status.arm = MANUAL;
      Global_status.pump = OFF;
    }
    else if (switch_is_down(RC_data.rc.s[0]))
    {
      Global_status.arm = MANUAL;
      Global_status.pump = ON;
    }

    osDelay(10);
  }
  /* USER CODE END statusTask_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
