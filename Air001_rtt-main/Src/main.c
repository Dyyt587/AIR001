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
#include <rtthread.h>
#include "ws2812_spi_dma.h"
#include "interface_iic.h"
#include "motor_pwm.h"

#include "encode.h"
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

// 常见颜色定义
const RGBColor_TypeDef WS2812_RED = {255, 128, 0};
const RGBColor_TypeDef WS2812_BLUE = {0, 0, 255};


extern UART_HandleTypeDef DebugUartHandle;

int ee_printf(const char *fmt, ...)
{
    char buf[12];
    va_list args;
    int len;
    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf),fmt, args);
    va_end(args);

    HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)buf, len, 0xFFFF);
    return len;
}

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{


  /* 初始化LED */
  BSP_LED_Init(LED_RED);
	WS2812Init();

	Set_LEDColor(0 , WS2812_RED );

  rt_kprintf("The current running frequency is %d Hz\r\n", HAL_RCC_GetSysClockFreq());
	
	
	/*初始化编码器*/
	encode_Init(4,100);
	
  motor_Init();
	
  pid_speed.Kp = 85;
	pid_speed.Ki = 10;
	pid_speed.Kd = 25;
	
	pid_angle.Kp = 10;

  interface_init();
  while (1)
  {
//		set_Speed(200);
//		int a = (int)get_Speed();
//    rt_kprintf("Speed:%d   \r\n",a);
//		
		Set_Angle(500);
		int a = (int)get_Angle();
    rt_kprintf("Angle:%d   \r\n",a);
		// rt_kprintf("PID:%d,%d,%d\r\n",pid_speed.Kp,pid_speed.Ki,pid_speed.Kd);
		
		 RGB_Reflash();

//		static int cnt=0;
//		Motor_Set_CH2(5000);
//		cnt+=10;
//		if(cnt>=1000)cnt=0;
//    /* 翻转LED */
    BSP_LED_Toggle(LED_RED);
    rt_thread_mdelay(50);
  }
}

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void Error_Handler(void)
{
  BSP_LED_Off(LED_RED);           /* 关闭LED */
	
	
	

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
