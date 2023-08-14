#ifndef AIR001_CORE_BOARD_H
#define AIR001_CORE_BOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "air001xx_hal.h"
#include <rtthread.h>
  /** @addtogroup BSP
   * @{
   */

  /** @defgroup air001_Core_Board
   * @brief 本节包含导出的类型、内容和函数
   * 使用Air001 Core Board 板所需的类型和函数。
   * @{
   */

  /** @defgroup air001_Core_Board_Exported_Types 出口的类型
   * @{
   */
  typedef enum
  {
    LED1 = 0,
    LED_RED = LED1
  } Led_TypeDef;

  typedef enum
  {
    BUTTON_USER = 0,
    /* Alias */
    BUTTON_KEY = BUTTON_USER
  } Button_TypeDef;

  typedef enum
  {
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EXTI = 1
  } ButtonMode_TypeDef;

#define LEDn 1

#define LED1_PIN GPIO_PIN_0
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) \
  do                                    \
  {                                     \
    LED1_GPIO_CLK_ENABLE();             \
  } while (0U)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   LED1_GPIO_CLK_DISABLE())

#define BUTTONn 1

/**
 * @brief 用户按钮
 */
#define USER_BUTTON_PIN GPIO_PIN_4
#define USER_BUTTON_GPIO_PORT GPIOF
#define USER_BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE() __HAL_RCC_GPIOF_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn EXTI4_15_IRQn

/* 别名 */
#define KEY_BUTTON_PIN USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE() USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE() USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn USER_BUTTON_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__) \
  do                                       \
  {                                        \
    if ((__INDEX__) == 0)                  \
      USER_BUTTON_GPIO_CLK_ENABLE();       \
  } while (0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__) (((__INDEX__) == 0) ? USER_BUTTON_GPIO_CLK_DISABLE() : 0)

#ifdef HAL_UART_MODULE_ENABLED
// debug printf redirect config
#define DEBUG_USART_BAUDRATE 115200

#define DEBUG_USART USART1
#define DEBUG_USART_CLK_ENABLE()                           \
  do                                                       \
  {                                                        \
    __IO uint32_t tmpreg = 0x00U;                          \
    SET_BIT(RCC->APBENR2, RCC_APBENR2_USART1EN);           \
    /* RCC外围时钟启用后的延迟 */                \
    tmpreg = READ_BIT(RCC->APBENR2, RCC_APBENR2_USART1EN); \
    UNUSED(tmpreg);                                        \
  } while (0U)

#define __GPIOA_CLK_ENABLE()                            \
  do                                                    \
  {                                                     \
    __IO uint32_t tmpreg = 0x00U;                       \
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);           \
    /* RCC外围时钟启用后的延迟 */             \
    tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN); \
    UNUSED(tmpreg);                                     \
  } while (0U)
#define __GPIOF_CLK_ENABLE()                            \
  do                                                    \
  {                                                     \
    __IO uint32_t tmpreg = 0x00U;                       \
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOFEN);           \
    /* RCC外围时钟启用后的延迟 */             \
    tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOFEN); \
    UNUSED(tmpreg);                                     \
  } while (0U)

#define DEBUG_USART_RX_GPIO_PORT GPIOF
#define DEBUG_USART_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_USART_RX_PIN GPIO_PIN_0
#define DEBUG_USART_RX_AF GPIO_AF8_USART1

#define DEBUG_USART_TX_GPIO_PORT GPIOF
#define DEBUG_USART_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_USART_TX_PIN GPIO_PIN_1
#define DEBUG_USART_TX_AF GPIO_AF8_USART1

#define DEBUG_USART_IRQHandler USART1_IRQHandler
#define DEBUG_USART_IRQ USART1_IRQn
#endif
  /************************************************************/

#ifdef HAL_UART_MODULE_ENABLED
  extern UART_HandleTypeDef DebugUartHandle;
#endif

  /** @defgroup Functions
   * @{
   */
  uint32_t BSP_GetVersion(void);

  void BSP_LED_Init(Led_TypeDef Led);
  void BSP_LED_DeInit(Led_TypeDef Led);
  void BSP_LED_On(Led_TypeDef Led);
  void BSP_LED_Off(Led_TypeDef Led);
  void BSP_LED_Toggle(Led_TypeDef Led);

  void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
  void BSP_PB_DeInit(Button_TypeDef Button);
  uint32_t BSP_PB_GetState(Button_TypeDef Button);

  int BSP_USART_Config(void);
#define DEBUG_USART_Config BSP_USART_Config
  /**
   * @}
   */

  /**
   * @}
   */

#ifdef __cplusplus
}
#endif

#endif
