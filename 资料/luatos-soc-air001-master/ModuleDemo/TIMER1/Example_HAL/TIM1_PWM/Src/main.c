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
#define  PERIOD_VALUE       (uint32_t)(50 - 1)
#define  PULSE1_VALUE       10
#define  PULSE2_VALUE       20
#define  PULSE3_VALUE       30
#define  PULSE4_VALUE       40

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef sConfig;

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
  
  TimHandle.Instance = TIM1;                                                  /* 选择TIM1 */
  TimHandle.Init.Period            = 50;                                      /* 自动重装载值 */
  TimHandle.Init.Prescaler         = 800 - 1;                                 /* 预分频为800-1 */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;                  /* 时钟不分频 */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;                      /* 向上计数 */
  TimHandle.Init.RepetitionCounter = 1 - 1;                                   /* 不重复计数 */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;          /* 自动重装载寄存器没有缓冲 */
  /* 基础时钟初始化 */
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.OCMode       = TIM_OCMODE_PWM1;                                     /* 输出配置为PWM1 */
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;                                 /* OC通道输出高电平有效 */
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;                                  /* 输出快速使能关闭 */
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;                                /* OCN通道输出高电平有效 */
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;                              /* 空闲状态OC1N输出低电平 */
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;                               /* 空闲状态OC1输出低电平 */

  sConfig.Pulse = PULSE1_VALUE;                                               /* CC1值为10，占空比=10/50=20% */
  /* 配置通道1 */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Pulse = PULSE2_VALUE;                                               /* CC2值为20,占空比=20/50=40% */
  /* 配置通道2 */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Pulse = PULSE3_VALUE;                                               /* CC3值为30,占空比=30/50=60% */
  /* 配置通道3 */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  sConfig.Pulse = PULSE4_VALUE;                                               /* CC4值为40,占空比=40/50=80% */
  /* 配置通道4 */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 开启所有通道输出 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  while (1)
  {
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
