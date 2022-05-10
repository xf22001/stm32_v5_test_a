/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define FAN3_FAULT_Pin GPIO_PIN_2
#define FAN3_FAULT_GPIO_Port GPIOE
#define FAN4_FAULT_Pin GPIO_PIN_3
#define FAN4_FAULT_GPIO_Port GPIOE
#define FAN5_FAULT_Pin GPIO_PIN_4
#define FAN5_FAULT_GPIO_Port GPIOE
#define LED_YELLOW_PLUG1_Pin GPIO_PIN_5
#define LED_YELLOW_PLUG1_GPIO_Port GPIOE
#define LED_YELLOW_PLUG2_Pin GPIO_PIN_6
#define LED_YELLOW_PLUG2_GPIO_Port GPIOE
#define LED_RED_PLUG1_Pin GPIO_PIN_8
#define LED_RED_PLUG1_GPIO_Port GPIOI
#define POWERDISTRIBUTION_5_Pin GPIO_PIN_13
#define POWERDISTRIBUTION_5_GPIO_Port GPIOC
#define VTRANS_1_12V_24_Pin GPIO_PIN_9
#define VTRANS_1_12V_24_GPIO_Port GPIOI
#define VTRANS_2_12V_24_Pin GPIO_PIN_10
#define VTRANS_2_12V_24_GPIO_Port GPIOI
#define LED_GREEN_POWER2_Pin GPIO_PIN_11
#define LED_GREEN_POWER2_GPIO_Port GPIOI
#define LED_RED_PLUG2_Pin GPIO_PIN_0
#define LED_RED_PLUG2_GPIO_Port GPIOF
#define LED_GREEN_POWER1_Pin GPIO_PIN_1
#define LED_GREEN_POWER1_GPIO_Port GPIOF
#define EPO_Pin GPIO_PIN_2
#define EPO_GPIO_Port GPIOF
#define TEMP__PLUG1_Pin GPIO_PIN_3
#define TEMP__PLUG1_GPIO_Port GPIOF
#define TEMP__PLUG1F4_Pin GPIO_PIN_4
#define TEMP__PLUG1F4_GPIO_Port GPIOF
#define BAT1_V_Pin GPIO_PIN_5
#define BAT1_V_GPIO_Port GPIOF
#define TEMP__PLUG2_Pin GPIO_PIN_6
#define TEMP__PLUG2_GPIO_Port GPIOF
#define TEMP__PLUG2F7_Pin GPIO_PIN_7
#define TEMP__PLUG2F7_GPIO_Port GPIOF
#define CHARGER1_V_Pin GPIO_PIN_8
#define CHARGER1_V_GPIO_Port GPIOF
#define INS_R_V_PLUG1_Pin GPIO_PIN_9
#define INS_R_V_PLUG1_GPIO_Port GPIOF
#define INS_R_V_PLUG2_Pin GPIO_PIN_10
#define INS_R_V_PLUG2_GPIO_Port GPIOF
#define CHARGER2_V_Pin GPIO_PIN_0
#define CHARGER2_V_GPIO_Port GPIOC
#define BAT2_V_Pin GPIO_PIN_1
#define BAT2_V_GPIO_Port GPIOC
#define AMB_TEMP_Pin GPIO_PIN_2
#define AMB_TEMP_GPIO_Port GPIOC
#define CP_VOLTAGE_PAR_MODE_Pin GPIO_PIN_3
#define CP_VOLTAGE_PAR_MODE_GPIO_Port GPIOC
#define DC_METER1_TX_Pin GPIO_PIN_0
#define DC_METER1_TX_GPIO_Port GPIOA
#define DC_METER1_RX_Pin GPIO_PIN_1
#define DC_METER1_RX_GPIO_Port GPIOA
#define CP1_VOLTAGE_Pin GPIO_PIN_2
#define CP1_VOLTAGE_GPIO_Port GPIOA
#define FUSE1_OPEN_Pin GPIO_PIN_2
#define FUSE1_OPEN_GPIO_Port GPIOH
#define FUSE2_OPEN_Pin GPIO_PIN_3
#define FUSE2_OPEN_GPIO_Port GPIOH
#define ETH_CS_Pin GPIO_PIN_4
#define ETH_CS_GPIO_Port GPIOA
#define ETH_SCK_Pin GPIO_PIN_5
#define ETH_SCK_GPIO_Port GPIOA
#define ETH_MISO_Pin GPIO_PIN_6
#define ETH_MISO_GPIO_Port GPIOA
#define ETH_MOSI_Pin GPIO_PIN_7
#define ETH_MOSI_GPIO_Port GPIOA
#define BRG_RLY_DRV_Pin GPIO_PIN_1
#define BRG_RLY_DRV_GPIO_Port GPIOB
#define BRG_RLY_DRV_BACK_Pin GPIO_PIN_11
#define BRG_RLY_DRV_BACK_GPIO_Port GPIOF
#define SW_D0_Pin GPIO_PIN_12
#define SW_D0_GPIO_Port GPIOF
#define SW_D1_Pin GPIO_PIN_13
#define SW_D1_GPIO_Port GPIOF
#define SW_D2_Pin GPIO_PIN_14
#define SW_D2_GPIO_Port GPIOF
#define SW_D3_Pin GPIO_PIN_15
#define SW_D3_GPIO_Port GPIOF
#define SW_D4_Pin GPIO_PIN_0
#define SW_D4_GPIO_Port GPIOG
#define FAN1_RLY_DRV_Pin GPIO_PIN_1
#define FAN1_RLY_DRV_GPIO_Port GPIOG
#define DOOR1_OPEN_Pin GPIO_PIN_7
#define DOOR1_OPEN_GPIO_Port GPIOE
#define CP_RELAY_Pin GPIO_PIN_8
#define CP_RELAY_GPIO_Port GPIOE
#define MARQUEE1__Pin GPIO_PIN_9
#define MARQUEE1__GPIO_Port GPIOE
#define MARQUEE2__Pin GPIO_PIN_10
#define MARQUEE2__GPIO_Port GPIOE
#define ETH_RST_Pin GPIO_PIN_11
#define ETH_RST_GPIO_Port GPIOE
#define SYS_LED_Pin GPIO_PIN_12
#define SYS_LED_GPIO_Port GPIOE
#define DC_METER1_LED_Pin GPIO_PIN_13
#define DC_METER1_LED_GPIO_Port GPIOE
#define DC_METER2_LED_Pin GPIO_PIN_14
#define DC_METER2_LED_GPIO_Port GPIOE
#define LED_4G_Pin GPIO_PIN_15
#define LED_4G_GPIO_Port GPIOE
#define POWERDISTRIBUTION_1_BACK_Pin GPIO_PIN_10
#define POWERDISTRIBUTION_1_BACK_GPIO_Port GPIOB
#define POWERDISTRIBUTION_2_BACK_Pin GPIO_PIN_11
#define POWERDISTRIBUTION_2_BACK_GPIO_Port GPIOB
#define POWERDISTRIBUTION_3_BACK_Pin GPIO_PIN_6
#define POWERDISTRIBUTION_3_BACK_GPIO_Port GPIOH
#define EEP_SCL_Pin GPIO_PIN_7
#define EEP_SCL_GPIO_Port GPIOH
#define EEP_SDA_Pin GPIO_PIN_8
#define EEP_SDA_GPIO_Port GPIOH
#define CHARGING_LED_SCLK1_Pin GPIO_PIN_9
#define CHARGING_LED_SCLK1_GPIO_Port GPIOH
#define CHARGING_LED_LCLK1_Pin GPIO_PIN_10
#define CHARGING_LED_LCLK1_GPIO_Port GPIOH
#define CHARGING_LED_DS1_Pin GPIO_PIN_11
#define CHARGING_LED_DS1_GPIO_Port GPIOH
#define CHARGING_LED_SCLK2_Pin GPIO_PIN_12
#define CHARGING_LED_SCLK2_GPIO_Port GPIOH
#define CHARGING_LED_LCLK2_Pin GPIO_PIN_14
#define CHARGING_LED_LCLK2_GPIO_Port GPIOB
#define CHARGING_LED_DS2_Pin GPIO_PIN_15
#define CHARGING_LED_DS2_GPIO_Port GPIOB
#define DC_METER2_TX_Pin GPIO_PIN_8
#define DC_METER2_TX_GPIO_Port GPIOD
#define DC_METER2_RX_Pin GPIO_PIN_9
#define DC_METER2_RX_GPIO_Port GPIOD
#define POWERDISTRIBUTION_4_BACK_Pin GPIO_PIN_10
#define POWERDISTRIBUTION_4_BACK_GPIO_Port GPIOD
#define MAIN_RLY_PLUG1_Pin GPIO_PIN_11
#define MAIN_RLY_PLUG1_GPIO_Port GPIOD
#define BMSPOWER_PLUG1_Pin GPIO_PIN_12
#define BMSPOWER_PLUG1_GPIO_Port GPIOD
#define PLUG1_LOCK_1_Pin GPIO_PIN_13
#define PLUG1_LOCK_1_GPIO_Port GPIOD
#define PLUG1_LOCK_2_Pin GPIO_PIN_14
#define PLUG1_LOCK_2_GPIO_Port GPIOD
#define K1_PLUG1_Pin GPIO_PIN_15
#define K1_PLUG1_GPIO_Port GPIOD
#define POWERDISTRIBUTION_5_BACK_Pin GPIO_PIN_2
#define POWERDISTRIBUTION_5_BACK_GPIO_Port GPIOG
#define K3_PLUG1_Pin GPIO_PIN_3
#define K3_PLUG1_GPIO_Port GPIOG
#define MAIN_RLY_PLUG1_BACK_Pin GPIO_PIN_4
#define MAIN_RLY_PLUG1_BACK_GPIO_Port GPIOG
#define PLUG1_LOCK_BACK_Pin GPIO_PIN_5
#define PLUG1_LOCK_BACK_GPIO_Port GPIOG
#define K1_PLUG2_Pin GPIO_PIN_6
#define K1_PLUG2_GPIO_Port GPIOG
#define K3_PLUG2_Pin GPIO_PIN_7
#define K3_PLUG2_GPIO_Port GPIOG
#define CONTACTOR_DRV_Pin GPIO_PIN_8
#define CONTACTOR_DRV_GPIO_Port GPIOG
#define LCD_TX_Pin GPIO_PIN_6
#define LCD_TX_GPIO_Port GPIOC
#define LCD_RX_Pin GPIO_PIN_7
#define LCD_RX_GPIO_Port GPIOC
#define DISCHARGE_RLY_PLUG1_Pin GPIO_PIN_9
#define DISCHARGE_RLY_PLUG1_GPIO_Port GPIOC
#define DISCHARGE_RLY_PLUG2_Pin GPIO_PIN_8
#define DISCHARGE_RLY_PLUG2_GPIO_Port GPIOA
#define RFID_TX_Pin GPIO_PIN_9
#define RFID_TX_GPIO_Port GPIOA
#define RFID_RX_Pin GPIO_PIN_10
#define RFID_RX_GPIO_Port GPIOA
#define PAR_EXT_TX1_Pin GPIO_PIN_13
#define PAR_EXT_TX1_GPIO_Port GPIOH
#define PAR_EXT_TX2_Pin GPIO_PIN_14
#define PAR_EXT_TX2_GPIO_Port GPIOH
#define PAR_EXT_TX3_Pin GPIO_PIN_15
#define PAR_EXT_TX3_GPIO_Port GPIOH
#define SPI_CS_1_Pin GPIO_PIN_0
#define SPI_CS_1_GPIO_Port GPIOI
#define SPI_SCK_1_Pin GPIO_PIN_1
#define SPI_SCK_1_GPIO_Port GPIOI
#define SPI_MISO_1_Pin GPIO_PIN_2
#define SPI_MISO_1_GPIO_Port GPIOI
#define SPI_MOSI_1_Pin GPIO_PIN_3
#define SPI_MOSI_1_GPIO_Port GPIOI
#define PAR_EXT_RX1_Pin GPIO_PIN_15
#define PAR_EXT_RX1_GPIO_Port GPIOA
#define PAR_EXT_RX2_Pin GPIO_PIN_10
#define PAR_EXT_RX2_GPIO_Port GPIOC
#define PAR_EXT_RX3_Pin GPIO_PIN_11
#define PAR_EXT_RX3_GPIO_Port GPIOC
#define EXTEND_TX_Pin GPIO_PIN_12
#define EXTEND_TX_GPIO_Port GPIOC
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define EXTEND_RX_Pin GPIO_PIN_2
#define EXTEND_RX_GPIO_Port GPIOD
#define CC1_PLUG2_Pin GPIO_PIN_3
#define CC1_PLUG2_GPIO_Port GPIOD
#define CC1_PLUG1_Pin GPIO_PIN_4
#define CC1_PLUG1_GPIO_Port GPIOD
#define RX_4G_Pin GPIO_PIN_5
#define RX_4G_GPIO_Port GPIOD
#define MAIN_RLY_PLUG2_Pin GPIO_PIN_9
#define MAIN_RLY_PLUG2_GPIO_Port GPIOG
#define MAIN_RLY_PLUG2_BACK_Pin GPIO_PIN_10
#define MAIN_RLY_PLUG2_BACK_GPIO_Port GPIOG
#define BMSPOWER_PLUG2_Pin GPIO_PIN_11
#define BMSPOWER_PLUG2_GPIO_Port GPIOG
#define PLUG2_LOCK_1_Pin GPIO_PIN_12
#define PLUG2_LOCK_1_GPIO_Port GPIOG
#define PLUG2_LOCK_2_Pin GPIO_PIN_13
#define PLUG2_LOCK_2_GPIO_Port GPIOG
#define PLUG2_LOCK_BACK_Pin GPIO_PIN_14
#define PLUG2_LOCK_BACK_GPIO_Port GPIOG
#define SPICAN_CS_Pin GPIO_PIN_15
#define SPICAN_CS_GPIO_Port GPIOG
#define SPICAN_MISO_Pin GPIO_PIN_4
#define SPICAN_MISO_GPIO_Port GPIOB
#define SPICAN_MOSI_Pin GPIO_PIN_5
#define SPICAN_MOSI_GPIO_Port GPIOB
#define SPICAN_INT_Pin GPIO_PIN_6
#define SPICAN_INT_GPIO_Port GPIOB
#define SPICAN_INT_EXTI_IRQn EXTI9_5_IRQn
#define SPICAN_INT0_Pin GPIO_PIN_7
#define SPICAN_INT0_GPIO_Port GPIOB
#define SPICAN_INT0_EXTI_IRQn EXTI9_5_IRQn
#define SPICAN_INT1_Pin GPIO_PIN_8
#define SPICAN_INT1_GPIO_Port GPIOB
#define SPICAN_INT1_EXTI_IRQn EXTI9_5_IRQn
#define SPICAN_CLKO_Pin GPIO_PIN_9
#define SPICAN_CLKO_GPIO_Port GPIOB
#define FAN1_FAULT_Pin GPIO_PIN_0
#define FAN1_FAULT_GPIO_Port GPIOE
#define FAN2_FAULT_Pin GPIO_PIN_1
#define FAN2_FAULT_GPIO_Port GPIOE
#define POWERDISTRIBUTION_1_Pin GPIO_PIN_4
#define POWERDISTRIBUTION_1_GPIO_Port GPIOI
#define POWERDISTRIBUTION_2_Pin GPIO_PIN_5
#define POWERDISTRIBUTION_2_GPIO_Port GPIOI
#define POWERDISTRIBUTION_3_Pin GPIO_PIN_6
#define POWERDISTRIBUTION_3_GPIO_Port GPIOI
#define POWERDISTRIBUTION_4_Pin GPIO_PIN_7
#define POWERDISTRIBUTION_4_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
