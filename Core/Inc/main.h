/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l0xx_hal.h"

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
#define KEY_MID_Pin GPIO_PIN_0
#define KEY_MID_GPIO_Port GPIOA
#define KEY_MID_EXTI_IRQn EXTI0_1_IRQn
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_UP_Pin GPIO_PIN_14
#define KEY_UP_GPIO_Port GPIOC
#define KEY_UP_EXTI_IRQn EXTI4_15_IRQn
#define KEY_MID_Pin GPIO_PIN_0
#define KEY_MID_GPIO_Port GPIOA
#define KEY_MID_EXTI_IRQn EXTI0_1_IRQn
#define KEY_RIGHT_Pin GPIO_PIN_2
#define KEY_RIGHT_GPIO_Port GPIOA
#define KEY_RIGHT_EXTI_IRQn EXTI2_3_IRQn
#define KEY_DOWN_Pin GPIO_PIN_3
#define KEY_DOWN_GPIO_Port GPIOA
#define KEY_DOWN_EXTI_IRQn EXTI2_3_IRQn
#define CODEC_INT_Pin GPIO_PIN_4
#define CODEC_INT_GPIO_Port GPIOA
#define CODEC_INT_EXTI_IRQn EXTI4_15_IRQn
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOA
#define I2C_SCL_Pin GPIO_PIN_0
#define I2C_SCL_GPIO_Port GPIOB
#define KEY_LEFT_Pin GPIO_PIN_1
#define KEY_LEFT_GPIO_Port GPIOB
#define KEY_LEFT_EXTI_IRQn EXTI0_1_IRQn
#define CHARGING_Pin GPIO_PIN_2
#define CHARGING_GPIO_Port GPIOB
#define CHRG_DONE_Pin GPIO_PIN_8
#define CHRG_DONE_GPIO_Port GPIOA
#define SYS_PWR_Pin GPIO_PIN_9
#define SYS_PWR_GPIO_Port GPIOA
#define SYS_CODEC_PWR_Pin GPIO_PIN_10
#define SYS_CODEC_PWR_GPIO_Port GPIOA
#define BL_PREV_Pin GPIO_PIN_11
#define BL_PREV_GPIO_Port GPIOA
#define BL_NEXT_Pin GPIO_PIN_12
#define BL_NEXT_GPIO_Port GPIOA
#define BL_PLAY_Pin GPIO_PIN_15
#define BL_PLAY_GPIO_Port GPIOA
#define BL_PWR_Pin GPIO_PIN_3
#define BL_PWR_GPIO_Port GPIOB
#define BL_AUEN_Pin GPIO_PIN_4
#define BL_AUEN_GPIO_Port GPIOB
#define BL_CNED_Pin GPIO_PIN_5
#define BL_CNED_GPIO_Port GPIOB
#define SYS_RST_N_Pin GPIO_PIN_8
#define SYS_RST_N_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
