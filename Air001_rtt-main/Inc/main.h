/**
  ******************************************************************************
  * @file    main.h
  * @author  MCU Application Team
  * @brief   Header for main.c file.
  *          This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) AirM2M.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "air001xx_hal.h"
#include "air001_Core_Board.h"


extern DMA_HandleTypeDef HdmaCh1;
extern DMA_HandleTypeDef HdmaCh2;
extern DMA_HandleTypeDef HdmaCh3;
extern SPI_HandleTypeDef Spi1Handle;
extern uint8_t DmaOnFlag, DmaTxRxFlag;

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported variables prototypes ---------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
extern I2C_HandleTypeDef I2cHandle;
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT AirM2M *****END OF FILE******************/
