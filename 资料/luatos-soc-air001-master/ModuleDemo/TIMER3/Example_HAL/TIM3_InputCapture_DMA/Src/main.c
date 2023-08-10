/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
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

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_IC_InitTypeDef sICConfig;
uint32_t CC1_Capture = 0;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief   应用程序入口函数
  * @retval  int
  */
int main(void)
{
  /* 初始化所有外设，Flash接口，SysTick */
  HAL_Init();
  
  /* 系统时钟配置 */
  APP_SystemClockConfig(); 
  
  /*初始化USART*/
  DEBUG_USART_Config();
  
  TimHandle.Instance = TIM3;                                           /* 选择TIM3 */
  TimHandle.Init.Period            = 12800 - 1;                        /* 自动重装载值 */
  TimHandle.Init.Prescaler         = 1000 - 1;                         /* 预分频为1000-1 */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           /* 时钟不分频 */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;               /* 向上计数 */
  TimHandle.Init.RepetitionCounter = 1 - 1;                            /* 不重复计数 */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* 自动重装载寄存器没有缓冲 */
  /* TIM1初始化 */
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*通道1捕获配置情况*/
  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;                       /* 上升沿捕获 */
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;                    /* CC1通道配置为输入 */
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;                              /* 输入不分配 */
  sICConfig.ICFilter    = 0;                                           /* 输入无滤波 */
  /* 通道1输入捕获配置 */
  if (HAL_TIM_IC_ConfigChannel(&TimHandle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 启动通道1输入捕获 */
  if (HAL_TIM_IC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 使能指定DMA请求 */
  __HAL_TIM_ENABLE_DMA(&TimHandle, TIM_DMA_CC1);
  
  /* TIM3使能启动，并使能中断 */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 无限循环 */
  while (1)
  {
    printf("CC1_Capture = 0x%x\r\n ", CC1_Capture);
  }
}

/**
  * @brief   系统时钟配置函数
  * @param   无
  * @retval  无
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI \
                                   | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;  /* 配置HSE、HSI、LSI、LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                             /* 开启HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                             /* HSI不分频 */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;                     /* HSI校准频率8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                            /* 关闭HSE */
  /* RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz; */                                  /* HSE频率范围16~32MHz */
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                                            /* 关闭LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                                            /* 关闭LSE */
  /* RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM; */                             /* 默认LSE驱动能力 */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                                        /* 关闭PLL */
  /* RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_NONE; */                          /* PLL无时钟源 */
  /* 配置振荡器 */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;/* 配置SYSCLK、HCLK、PCLK */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                                        /* 配置系统时钟为HSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                            /* AHB时钟不分频 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                                             /* APB时钟不分频 */
  /* 配置时钟源 */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief   错误执行函数
  * @param   无
  * @retval  无
  */
void Error_Handler(void)
{
  /* 无限循环 */
  while (1)
  {
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT AirM2M *****END OF FILE******************/
