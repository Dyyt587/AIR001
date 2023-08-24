/**
  ******************************************************************************
  * @file    air001xx_it.c
  * @author  MCU Application Team
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "air001xx_it.h"

#include "encode.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*          Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
	  rt_interrupt_enter();


  rt_interrupt_leave(); 
}

/**
  * @brief This function handles Hard fault interrupt.
  */
// void HardFault_Handler(void)
// {
//   while (1)
//   {
//   }
// }

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
	  rt_interrupt_enter(); 


  rt_interrupt_leave(); 
}

/**
  * @brief This function handles Pendable request for system service.
  */
// void PendSV_Handler(void)
// {
// }

/**
  * @brief This function handles System tick timer.
  */
// void SysTick_Handler(void)
// {
//   HAL_IncTick();
// }

/******************************************************************************/
/* AIR001xx Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file.                                          */
/******************************************************************************/
///**
//  * @brief This function handles DMA1 channel1 Interrupt .
//  */
//void DMA1_Channel1_IRQHandler(void)
//{
//	  rt_interrupt_enter(); 

//  HAL_DMA_IRQHandler(Spi1Handle.hdmatx);

//  rt_interrupt_leave(); 

//}

///**
//  * @brief This function handles DMA1 channel2 and channel3 Interrupt .
//  */
//void DMA1_Channel2_3_IRQHandler(void)
//{
//	
//	  rt_interrupt_enter(); 

//  HAL_DMA_IRQHandler(Spi1Handle.hdmarx);

//  rt_interrupt_leave(); 

//}
//	
//	
//	


void DMA1_Channel1_IRQHandler(void)
{
	rt_interrupt_enter();
  HAL_DMA_IRQHandler(I2cHandle.hdmatx);
	 rt_interrupt_leave(); 
}

/**
  * @brief This function handles DMA1 channel2 and channel3 Interrupt .
  */
void DMA1_Channel2_3_IRQHandler(void)
{
	rt_interrupt_enter();
  HAL_DMA_IRQHandler(I2cHandle.hdmarx);
	 rt_interrupt_leave(); 
}



void EXTI4_15_IRQHandler(void)
{

	rt_interrupt_enter(); 
	Encode_Handler();
  rt_interrupt_leave();
}

void I2C1_IRQHandler(void)
{
	rt_interrupt_enter(); 
//	extern I2C_HandleTypeDef I2cHandle;
  HAL_I2C_EV_IRQHandler(&I2cHandle);
  HAL_I2C_ER_IRQHandler(&I2cHandle);
	
	 rt_interrupt_leave();
}
/**
  * @brief This function handles SPI1 Interrupt .
  */

/**
  * @brief This function handles SPI1 Interrupt .
  */
void SPI2_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&Spi1Handle2);
}

//void USART1_IRQHandler(void)
//{
//	while(1){};
//     HAL_UART_IRQHandler(&DebugUartHandle);
//}
/************************ (C) COPYRIGHT AirM2M *****END OF FILE******************/
