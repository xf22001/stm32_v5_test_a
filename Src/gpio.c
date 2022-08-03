/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_YELLOW_PLUG1_Pin|LED_YELLOW_PLUG2_Pin|CP_RELAY_Pin|MARQUEE1__Pin
                          |MARQUEE2__Pin|SYS_LED_Pin|DC_METER1_LED_Pin|DC_METER2_LED_Pin
                          |LED_4G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LED_RED_PLUG1_Pin|VTRANS_1_12V_24_Pin|VTRANS_2_12V_24_Pin|LED_GREEN_POWER2_Pin
                          |SPI_CS_1_Pin|POWERDISTRIBUTION_1_Pin|POWERDISTRIBUTION_2_Pin|POWERDISTRIBUTION_3_Pin
                          |POWERDISTRIBUTION_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, POWERDISTRIBUTION_5_Pin|DISCHARGE_RLY_PLUG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED_RED_PLUG2_Pin|LED_GREEN_POWER1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ETH_CS_Pin|DISCHARGE_RLY_PLUG2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BRG_RLY_DRV_Pin|CHARGING_LED_LCLK2_Pin|CHARGING_LED_DS2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, FAN1_RLY_DRV_Pin|K3_PLUG1_Pin|K1_PLUG2_Pin|K3_PLUG2_Pin
                          |CONTACTOR_DRV_Pin|MAIN_RLY_PLUG2_Pin|BMSPOWER_PLUG2_Pin|PLUG2_LOCK_1_Pin
                          |PLUG2_LOCK_2_Pin|SPICAN_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ETH_RST_GPIO_Port, ETH_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, CHARGING_LED_SCLK1_Pin|CHARGING_LED_LCLK1_Pin|CHARGING_LED_DS1_Pin|CHARGING_LED_SCLK2_Pin
                          |PAR_EXT_TX1_Pin|PAR_EXT_TX2_Pin|PAR_EXT_TX3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, MAIN_RLY_PLUG1_Pin|BMSPOWER_PLUG1_Pin|PLUG1_LOCK_1_Pin|PLUG1_LOCK_2_Pin
                          |K1_PLUG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin */
  GPIO_InitStruct.Pin = FAN3_FAULT_Pin|FAN4_FAULT_Pin|FAN5_FAULT_Pin|DOOR1_OPEN_Pin
                          |FAN1_FAULT_Pin|FAN2_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin
                           PEPin PEPin */
  GPIO_InitStruct.Pin = LED_YELLOW_PLUG1_Pin|LED_YELLOW_PLUG2_Pin|CP_RELAY_Pin|MARQUEE1__Pin
                          |MARQUEE2__Pin|ETH_RST_Pin|SYS_LED_Pin|DC_METER1_LED_Pin
                          |DC_METER2_LED_Pin|LED_4G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PIPin PIPin PIPin PIPin
                           PIPin PIPin PIPin PIPin
                           PIPin */
  GPIO_InitStruct.Pin = LED_RED_PLUG1_Pin|VTRANS_1_12V_24_Pin|VTRANS_2_12V_24_Pin|LED_GREEN_POWER2_Pin
                          |SPI_CS_1_Pin|POWERDISTRIBUTION_1_Pin|POWERDISTRIBUTION_2_Pin|POWERDISTRIBUTION_3_Pin
                          |POWERDISTRIBUTION_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = POWERDISTRIBUTION_5_Pin|DISCHARGE_RLY_PLUG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin */
  GPIO_InitStruct.Pin = LED_RED_PLUG2_Pin|LED_GREEN_POWER1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin PFPin */
  GPIO_InitStruct.Pin = EPO_Pin|BRG_RLY_DRV_BACK_Pin|SW_D0_Pin|SW_D1_Pin
                          |SW_D2_Pin|SW_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PHPin */
  GPIO_InitStruct.Pin = FUSE1_OPEN_Pin|FUSE2_OPEN_Pin|POWERDISTRIBUTION_3_BACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = ETH_CS_Pin|DISCHARGE_RLY_PLUG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = BRG_RLY_DRV_Pin|CHARGING_LED_LCLK2_Pin|CHARGING_LED_DS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin PGPin PGPin
                           PGPin PGPin */
  GPIO_InitStruct.Pin = SW_D4_Pin|POWERDISTRIBUTION_5_BACK_Pin|MAIN_RLY_PLUG1_BACK_Pin|PLUG1_LOCK_BACK_Pin
                          |MAIN_RLY_PLUG2_BACK_Pin|PLUG2_LOCK_BACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin PGPin PGPin
                           PGPin PGPin PGPin PGPin
                           PGPin PGPin */
  GPIO_InitStruct.Pin = FAN1_RLY_DRV_Pin|K3_PLUG1_Pin|K1_PLUG2_Pin|K3_PLUG2_Pin
                          |CONTACTOR_DRV_Pin|MAIN_RLY_PLUG2_Pin|BMSPOWER_PLUG2_Pin|PLUG2_LOCK_1_Pin
                          |PLUG2_LOCK_2_Pin|SPICAN_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = POWERDISTRIBUTION_1_BACK_Pin|POWERDISTRIBUTION_2_BACK_Pin|SPICAN_CLKO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin PHPin PHPin
                           PHPin PHPin PHPin */
  GPIO_InitStruct.Pin = CHARGING_LED_SCLK1_Pin|CHARGING_LED_LCLK1_Pin|CHARGING_LED_DS1_Pin|CHARGING_LED_SCLK2_Pin
                          |PAR_EXT_TX1_Pin|PAR_EXT_TX2_Pin|PAR_EXT_TX3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = POWERDISTRIBUTION_4_BACK_Pin|CC1_PLUG2_Pin|CC1_PLUG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin
                           PDPin */
  GPIO_InitStruct.Pin = MAIN_RLY_PLUG1_Pin|BMSPOWER_PLUG1_Pin|PLUG1_LOCK_1_Pin|PLUG1_LOCK_2_Pin
                          |K1_PLUG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = PAR_EXT_RX1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PAR_EXT_RX1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = PAR_EXT_RX2_Pin|PAR_EXT_RX3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = SPICAN_INT_Pin|SPICAN_INT0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SPICAN_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SPICAN_INT1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
