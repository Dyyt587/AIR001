/*
 * @Author: dyyt
 * @Date: 2023-08-12 16:11:04
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-13 14:11:02
 * @FilePath: \Air001_rtt-main\Src\interface_iic.c
 * @Description:
 */
#include "iic.h"
#include "air001xx_hal.h"
#include "stdlib.h"
#include "string.h"
#include "rtthread.h"
#include "encode.h"


I2C_HandleTypeDef I2cHandle;
#define I2C_ADDRESS        0x08             /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK   100000             /* 通讯速度100K */
#define I2C_DUTYCYCLE    I2C_DUTYCYCLE_2    /* 占空比 */

void interface_init(void)
{

  I2cHandle.Instance             = I2C;                       /* I2C */
  I2cHandle.Init.ClockSpeed      = I2C_SPEEDCLOCK;            /* I2C通讯速度 */
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE;             /* I2C占空比 */
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;               /* I2C地址 */
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;   /* 禁止广播呼叫 */
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;     /* 允许时钟延长 */
  
    if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
    {
        while(1);
    }


//    while (HAL_I2C_Slave_Receive_IT(&I2cHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
//    {
//        while(1);
//    }
}