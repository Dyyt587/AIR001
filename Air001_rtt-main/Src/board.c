#include <rtthread.h>

/* board.c */

/* timer 定时器中断服务函数调用 rt_os_tick_callback function，cortex-m 架构使用 SysTick_Handler() */
void rt_os_tick_callback(void)
{
  rt_interrupt_enter(); /* 进入中断时必须调用 */

  rt_tick_increase();  /* RT-Thread 系统时钟计数 */

  rt_interrupt_leave(); /* 退出中断时必须调用 */
}

/* cortex-m 架构使用 SysTick_Handler() */
void SysTick_Handler()
{
//    HAL_IncTick();
    rt_os_tick_callback();
}

void rt_hw_board_init(void)
{
  /*
   * TODO 1: OS Tick Configuration
   * Enable the hardware timer and call the rt_os_tick_callback function
   * periodically with the frequency RT_TICK_PER_SECOND.
   */

  /* 1、系统、时钟初始化 */
	/**
  * @brief   系统时钟配置函数
  * @param   无
  * @retval  无
  */



  HAL_Init(); // 初始化 HAL 库
	
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;        /* 选择RCC振荡器为HSI */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* 开启HSI */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* 配置HSI输出时钟为24MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* 关闭HSE */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                      /* 关闭PLL */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;              /* 选择PLL源为HSI */
  /* 初始化RCC振荡器 */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* 初始化CPU,AHB,APB总线时钟 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;                                      /* SYSCLK的源选择为HSE */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                             /* APH时钟不分频 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                                              /* APB时钟2分频 */
  /* 初始化RCC系统时钟(FLASH_LATENCY_0=24M以下;FLASH_LATENCY_1=48M) */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
  rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
  rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

