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

  /* 初始化按键 */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* 配置PA01引脚为MCO功能，输出系统时钟 */
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /* 配置系统时钟为HSI */
  APP_SystemClockConfig();

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

  /*配置时钟源HSE/HSI/LSE/LSI*/
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                       /* 开启HSI */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_4MHz;                               /* 配置HSI输出时钟为4MHz */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                       /* HSI不分频 */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                      /* 关闭HSE */
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                                                      /* 关闭LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                                                      /* 关闭LSE */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                                                  /* 关闭PLL */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)                                           /* RCC振荡器初始化 */
  {
    Error_Handler();
  }

  /* 初始化CPU,AHB,APB总线时钟 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                                         /* SYSCLK的源选择为HSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                             /* APH时钟不分频 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                                              /* APB时钟不分频 */

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)                        /* 初始化RCC系统时钟 */
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
