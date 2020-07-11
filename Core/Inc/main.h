/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DoorOpen_Pin GPIO_PIN_2
#define DoorOpen_GPIO_Port GPIOE
#define GPIO_2_Pin GPIO_PIN_3
#define GPIO_2_GPIO_Port GPIOE
#define GPIO_3_Pin GPIO_PIN_4
#define GPIO_3_GPIO_Port GPIOE
#define GPIO_5_Pin GPIO_PIN_6
#define GPIO_5_GPIO_Port GPIOE
#define ADC123_IN3_Pin GPIO_PIN_3
#define ADC123_IN3_GPIO_Port GPIOA
#define ON_OFF_Pin GPIO_PIN_5
#define ON_OFF_GPIO_Port GPIOA
#define CUR_DIR_Pin GPIO_PIN_6
#define CUR_DIR_GPIO_Port GPIOA
#define BTN_4_Pin GPIO_PIN_0
#define BTN_4_GPIO_Port GPIOB
#define BTN_3_Pin GPIO_PIN_7
#define BTN_3_GPIO_Port GPIOE
#define BTN_2_Pin GPIO_PIN_8
#define BTN_2_GPIO_Port GPIOE
#define BTN_1_Pin GPIO_PIN_9
#define BTN_1_GPIO_Port GPIOE
#define SEG_DIG4_Pin GPIO_PIN_10
#define SEG_DIG4_GPIO_Port GPIOE
#define SEG_B_Pin GPIO_PIN_11
#define SEG_B_GPIO_Port GPIOE
#define SEG_DIG3_Pin GPIO_PIN_12
#define SEG_DIG3_GPIO_Port GPIOE
#define SEG_DIG2_Pin GPIO_PIN_13
#define SEG_DIG2_GPIO_Port GPIOE
#define SEG_F_Pin GPIO_PIN_14
#define SEG_F_GPIO_Port GPIOE
#define SEG_A_Pin GPIO_PIN_15
#define SEG_A_GPIO_Port GPIOE
#define SEG_DIG1_Pin GPIO_PIN_14
#define SEG_DIG1_GPIO_Port GPIOB
#define SEG_G_Pin GPIO_PIN_8
#define SEG_G_GPIO_Port GPIOD
#define SEG_C_Pin GPIO_PIN_9
#define SEG_C_GPIO_Port GPIOD
#define SEG_DP_Pin GPIO_PIN_10
#define SEG_DP_GPIO_Port GPIOD
#define SEG_D_Pin GPIO_PIN_11
#define SEG_D_GPIO_Port GPIOD
#define SEG_E_Pin GPIO_PIN_12
#define SEG_E_GPIO_Port GPIOD
#define uSD_DETECT_Pin GPIO_PIN_3
#define uSD_DETECT_GPIO_Port GPIOD
#define VCNL4200_INT_Pin GPIO_PIN_7
#define VCNL4200_INT_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
