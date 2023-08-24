/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-11 15:25:50
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-11 20:44:23
 * @FilePath: \Air001_rtt-main_ing\Src\Motor_PWM.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "Motor_PWM.h"

#define  PERIOD_VALUE       (uint32_t)(50 - 1)
#define  PULSE1_VALUE       5
#define  PULSE2_VALUE       9999
#define  PULSE3_VALUE       5
#define  PULSE4_VALUE       5


TIM_HandleTypeDef  TimHandle;
TIM_HandleTypeDef  TimHandle14;


void Error_Handler_Motor(void)
{
	while(1);
}
void motor_Init(void)
{
	TIM_OC_InitTypeDef sConfig;

	  GPIO_InitTypeDef   GPIO_InitStruct;
  /* TIM1时钟使能 */
  __HAL_RCC_TIM1_CLK_ENABLE();
  /* GPIOA时钟使能 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        /* 复用模式 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;            /* 引脚上拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Alternate = GPIO_AF0_TIM14;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*GPIOA0初始化*/
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*GPIOA1初始化*/
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
    TimHandle.Instance = TIM1;                                         /* 选择TIM1 */
    TimHandle.Init.Period = 10000;                                        /* 自动重装载值 */
    TimHandle.Init.Prescaler = 2 - 1;                                /* 预分频为800-1 */
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             /* 时钟不分频 */
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;                   /* 向上计数 */
    TimHandle.Init.RepetitionCounter = 1 - 1;                          /* 不重复计数 */
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* 自动重装载寄存器没有缓冲 */
    /* 基础时钟初始化 */
    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
    {
        Error_Handler_Motor();
    }

    sConfig.OCMode = TIM_OCMODE_PWM1;              /* 输出配置为PWM1 */
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;      /* OC通道输出高电平有效 */
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;       /* 输出快速使能关闭 */
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;    /* OCN通道输出高电平有效 */
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET; /* 空闲状态OC1N输出低电平 */
    sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;   /* 空闲状态OC1输出低电平 */

    sConfig.Pulse = PULSE1_VALUE; /* CC1值为10，占空比=10/50=20% */
    /* 配置通道1 */
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler_Motor();
    }
    sConfig.Pulse = PULSE2_VALUE; /* CC2值为20,占空比=20/50=40% */
    /* 配置通道2 */
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler_Motor();
    }
    sConfig.Pulse = PULSE3_VALUE; /* CC3值为30,占空比=30/50=60% */
    /* 配置通道3 */
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
    {
        /* Configuration Error */
        Error_Handler_Motor();
    }
    sConfig.Pulse = PULSE4_VALUE; /* CC4值为40,占空比=40/50=80% */
    /* 配置通道4 */
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler_Motor();
    }

    /* 开启所有通道输出 */
    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler_Motor();
    }
    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler_Motor();
    }
    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler_Motor();
    }
    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler_Motor();
    }
		
		
		
		
		
		
//		TimHandle14.Instance = TIM14;                                         /* 选择TIM1 */
//    TimHandle14.Init.Period = 10000;                                        /* 自动重装载值 */
//    TimHandle14.Init.Prescaler = 2 - 1;                                /* 预分频为800-1 */
//    TimHandle14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             /* 时钟不分频 */
//    TimHandle14.Init.CounterMode = TIM_COUNTERMODE_UP;                   /* 向上计数 */
//    TimHandle14.Init.RepetitionCounter = 1 - 1;                          /* 不重复计数 */
//    TimHandle14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* 自动重装载寄存器没有缓冲 */
//    /* 基础时钟初始化 */
//    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
//    {
//        Error_Handler_Motor();
//    }

//    sConfig.OCMode = TIM_OCMODE_PWM1;              /* 输出配置为PWM1 */
//    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;      /* OC通道输出高电平有效 */
//    sConfig.OCFastMode = TIM_OCFAST_DISABLE;       /* 输出快速使能关闭 */
//    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;    /* OCN通道输出高电平有效 */
//    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET; /* 空闲状态OC1N输出低电平 */
//    sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;   /* 空闲状态OC1输出低电平 */

//    sConfig.Pulse = PULSE1_VALUE; /* CC1值为10，占空比=10/50=20% */
//    /* 配置通道1 */
//    if (HAL_TIM_PWM_ConfigChannel(&TimHandle14, &sConfig, TIM_CHANNEL_1) != HAL_OK)
//    {
//        Error_Handler_Motor();
//    }
//		
		
		
}

/**
 * @description: 设置电机PB3 PWM
 * @param {uint32_t} value
 * @return {*}
 */
void Motor_Set_CH2(uint32_t value)
{
__HAL_TIM_SET_COMPARE(&TimHandle,TIM_CHANNEL_2,value);
}



/**
 * @description: 设置电机IN1 PWM
 * @param {uint32_t} value
 * @return {*}
 */
void Motor_Set_CH3(uint32_t value)
{
__HAL_TIM_SET_COMPARE(&TimHandle,TIM_CHANNEL_3,value);
}
/**
 * @description: 设置电机IN2 PWM
 * @param {uint32_t} value
 * @return {*}
 */
void Motor_Set_CH4(uint32_t value)
{
__HAL_TIM_SET_COMPARE(&TimHandle,TIM_CHANNEL_4,value);
}
