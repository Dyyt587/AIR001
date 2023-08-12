#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#include "air001xx_hal.h"

#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOA
#define TIMx_GPIO_PORT_CHANNEL2        GPIOB
#define TIMx_GPIO_PORT_CHANNEL3        GPIOA
#define TIMx_GPIO_PORT_CHANNEL4        GPIOA
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_8
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_3
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_0
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_1



void motor_Init(void);
void Motor_Set_CH2(uint32_t value);
void Motor_Set_CH3(uint32_t value);
void Motor_Set_CH4(uint32_t value);
#endif