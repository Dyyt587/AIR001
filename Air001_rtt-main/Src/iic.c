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




I2C_HandleTypeDef I2cHandle;

uint8_t aRxBuffer[16] = {0};
typedef enum
{
    None,
    Rx_Started,
    Rx_Cplt,
} Interface_Rx_State_e;
Interface_Rx_State_e RxBuffer_State = None;
int RxBuffer_index = 0;
enum
{
    Standerby_Mode = 0,
    Speed_Mode,
    Angle_Mode,
    Return_Mode,
};
typedef struct
{
    char mode; // 0待机，1速度，2位置,
    float speed_target;
    float angle_target;
    float speed_current;
    float angle_current;

} Control_t;
Control_t control;
// 本函数内部使用，故不窜入参数
void Rx_Pack_Callback(void)
{
    float param[2] = {0}; // 预留一个参数
    char cmd;
    memcpy( &aRxBuffer[RxBuffer_index - 2 * sizeof(param)],param, 2 * sizeof(param));
    cmd = aRxBuffer[RxBuffer_index - 2 * sizeof(param) - 1];
    if (cmd == 'S')
    {
        control.mode = Speed_Mode;
        control.speed_target = param[0];
        Set_Speed(param[0]);//直接输出给电机驱动
    }
    else if (cmd == 'P')
    {
        control.mode = Angle_Mode;
        control.angle_target = param[0];
        Set_Angle(param[0]);
    }
        else if (cmd == 'I')
    {
        control.mode = Return_Mode;
        //TODO:完成回传数据
    }
    else
    {
        control.mode = Standerby_Mode;
        //TODO
    }
}
void interface_updata(float speed, float angle)
{
    control.speed_current = speed;
    control.angle_current = angle;
}



void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (aRxBuffer[RxBuffer_index] == 'S')
    { // 寻找包头
        RxBuffer_State = Rx_Started;
    }
    else
    {
        RxBuffer_index = 0;
        while (HAL_I2C_Slave_Receive_IT(&I2cHandle, &(aRxBuffer[RxBuffer_index]), 1) != HAL_OK)
        {
            while(1);
        }
        return;
    }
    //////////在接收过程中//////////////////////////////
    if (RxBuffer_index >= 10) // 获取到定长数据，SP[data1][data2]共10字节
    {
        // 已经完成一帧数据接收
        RxBuffer_State = Rx_Cplt;
        Rx_Pack_Callback();
        RxBuffer_index = 0;
    }
    else
    {
        // 还在接收中
        RxBuffer_index++;
    }
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
        rt_kprintf("HAL_I2C_GetState != HAL_I2C_STATE_READY\r\n");
    }
    while (HAL_I2C_Slave_Receive_IT(&I2cHandle, &(aRxBuffer[RxBuffer_index]), 1) != HAL_OK)
    {
        Error_Handler();
    }
}









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