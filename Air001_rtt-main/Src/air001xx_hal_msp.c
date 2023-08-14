/**
  ******************************************************************************
  * @file    air001xx_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

extern DMA_HandleTypeDef HdmaCh1;
extern DMA_HandleTypeDef HdmaCh2;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief 初始化全局MSP
  */
void HAL_MspInit(void)
{
	  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  /* 使能LSI时钟 */
  __HAL_RCC_LSI_ENABLE();

  /* 等待直到LSI READY置位 */
  while (READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) == 0U);
}
/**
  * @brief 初始化SPI的MSP
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  /* SPI1 初始化 */
  if (hspi->Instance == SPI1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();                   /* GPIOB时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();                   /* GPIOA时钟使能 */
    __HAL_RCC_SYSCFG_CLK_ENABLE();                  /* 使能SYSCFG时钟 */
    __HAL_RCC_SPI1_CLK_ENABLE();                    /* SPI1时钟使能 */
    __HAL_RCC_DMA_CLK_ENABLE();                     /* DMA时钟使能 */
    HAL_SYSCFG_DMA_Req(1);                          /* SPI1_TX DMA_CH1 */
    HAL_SYSCFG_DMA_Req(0x200);                      /* SPI1_RX DMA_CH2 */
    /*
      PA5-SCK  (AF0)
      PA6-MISO(AF0)
      PA7-MOSI(AF0)
      PA4-NSS(AF0)
    */
    /* SPI CS*/
    // GPIO_InitStruct.Pin = GPIO_PIN_4;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    // if (hspi->Init.CLKPolarity == SPI_POLARITY_LOW)
    // {
    //   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    // }
    // else
    // {
    //   GPIO_InitStruct.Pull = GPIO_PULLUP;
    // }
    // GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    /*GPIO配置为SPI：SCK/MISO/MOSI*/
    GPIO_InitStruct.Pin       = GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*中断配置*/
    HAL_NVIC_SetPriority(SPI1_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    /*DMA_CH1配置*/
    HdmaCh1.Instance                 = DMA1_Channel1;
    HdmaCh1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    HdmaCh1.Init.PeriphInc           = DMA_PINC_DISABLE;
    HdmaCh1.Init.MemInc              = DMA_MINC_ENABLE;
    if (hspi->Init.DataSize <= SPI_DATASIZE_8BIT)
    {
      HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      HdmaCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    }
    else
    {
      HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      HdmaCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    }
    HdmaCh1.Init.Mode                = DMA_NORMAL;
    HdmaCh1.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    /*DMA初始化*/
    HAL_DMA_Init(&HdmaCh1);
    /*DMA句柄关联到SPI句柄*/
    __HAL_LINKDMA(hspi, hdmatx, HdmaCh1);

    /*DMA_CH2配置*/
    HdmaCh2.Instance                 = DMA1_Channel2;
    HdmaCh2.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    HdmaCh2.Init.PeriphInc           = DMA_PINC_DISABLE;
    HdmaCh2.Init.MemInc              = DMA_MINC_ENABLE;
    if (hspi->Init.DataSize <= SPI_DATASIZE_8BIT)
    {
      HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      HdmaCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    }
    else
    {
      HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      HdmaCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    }
    HdmaCh2.Init.Mode                = DMA_NORMAL;
    HdmaCh2.Init.Priority            = DMA_PRIORITY_LOW;
    /*DMA初始化*/
    HAL_DMA_Init(&HdmaCh2);
    /*DMA句柄关联到SPI句柄*/
    __HAL_LINKDMA(hspi, hdmarx, HdmaCh2);
    /*DMA中断设置*/
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  }

}

/**
  * @brief 反初始化I2C的MSP
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* 复位SPI外设 */
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();

    /* 关闭外设和GPIO时钟 */
    /* 取消配置SPI SCK*/
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

    HAL_NVIC_DisableIRQ(SPI1_IRQn);

    HAL_DMA_DeInit(&HdmaCh1);
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);

  }
}
/**
  * @brief 初始TIM相关MSP
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

}


/**
  * @brief 初始化I2C的MSP
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_SYSCFG_CLK_ENABLE();                              /* SYSCFG时钟使能 */
  __HAL_RCC_DMA_CLK_ENABLE();                                 /* DMA时钟使能 */
  __HAL_RCC_I2C_CLK_ENABLE();                                 /* I2C时钟使能 */
  __HAL_RCC_GPIOA_CLK_ENABLE();                               /* GPIOF时钟使能 */

  /**I2C引脚配置
  Pa3     ------> I2C1_SCL
  Pa2     ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;                     /* 开漏 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;                         /* 上拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_I2C;                   /* 复用为I2C */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);                     /* GPIO初始化 */
  /*复位I2C*/
  __HAL_RCC_I2C_FORCE_RESET();
  __HAL_RCC_I2C_RELEASE_RESET();
  /* I2C1 interrupt Init */
  HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);                      /* 中断优先级设置 */
  HAL_NVIC_EnableIRQ(I2C1_IRQn);                              /* 使能I2C中断 */

  /* 配置DMA请求映像 */
  HAL_SYSCFG_DMA_Req(9);                                      /* DMA1_MAP选择为IIC_TX */
  HAL_SYSCFG_DMA_Req(0xA00);                                  /* DMA2_MAP选择为IIC_RX */

  /* 配置DMA */
  /* 配置DMA句柄（用于发送） */
  HdmaCh1.Instance                 = DMA1_Channel1;           /* 选择DMA通道1 */
  HdmaCh1.Init.Direction           = DMA_MEMORY_TO_PERIPH;    /* 方向为从存储器到外设 */
  HdmaCh1.Init.PeriphInc           = DMA_PINC_DISABLE;        /* 禁止外设地址增量 */
  HdmaCh1.Init.MemInc              = DMA_MINC_ENABLE;         /* 使能存储器地址增量 */
  HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;     /* 外设数据宽度为8位 */
  HdmaCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;     /* 存储器数据宽度位8位 */
  HdmaCh1.Init.Mode                = DMA_NORMAL;              /* 禁止循环模式 */
  HdmaCh1.Init.Priority            = DMA_PRIORITY_VERY_HIGH;  /* 通道优先级为很高 */

  HAL_DMA_Init(&HdmaCh1);                                     /* 初始化DMA通道1 */
  __HAL_LINKDMA(hi2c, hdmatx, HdmaCh1);                       /* DMA1关联IIC_TX */

  /* 配置DMA句柄（用于接收） */
  HdmaCh2.Instance                 = DMA1_Channel2;           /* 选择DMA通道1 */
  HdmaCh2.Init.Direction           = DMA_PERIPH_TO_MEMORY;    /* 方向为从外设到存储 */
  HdmaCh2.Init.PeriphInc           = DMA_PINC_DISABLE;        /* 禁止外设地址增量 */
  HdmaCh2.Init.MemInc              = DMA_MINC_ENABLE;         /* 使能存储器地址增量 */
  HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;     /* 外设数据宽度为8位 */
  HdmaCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;     /* 存储器数据宽度位8位 */
  HdmaCh2.Init.Mode                = DMA_NORMAL;              /* 禁止循环模式 */
  HdmaCh2.Init.Priority            = DMA_PRIORITY_HIGH;       /* 通道优先级为高 */

  HAL_DMA_Init(&HdmaCh2);                                     /* 初始化DMA通道1 */
  __HAL_LINKDMA(hi2c, hdmarx, HdmaCh2);                       /* DMA1关联IIC_RX */

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 1);             /* 中断优先级设置 */
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);                     /* 使能DMA通道1中断 */

  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 1);           /* 中断优先级设置 */
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);                   /* 使能DMA通道2_3中断 */
}
/************************ (C) COPYRIGHT AirM2M *****END OF FILE******************/
