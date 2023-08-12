/*
 * @Author: dyyt
 * @Date: 2023-08-12 16:11:04
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-12 19:06:31
 * @FilePath: \Air001_rtt-main\Src\interface_iic.c
 * @Description:
 */
#include "interface_iic.h"
#include "air001xx_hal_gpio.h"
#include "air001xx_hal_i2c.h"
#include "stdio.h"

extern DMA_HandleTypeDef HdmaCh1;
extern DMA_HandleTypeDef HdmaCh2;

uint8_t aRxBuffer[16] = {0};
typedef enum
{
    None,
    Rx_Started,
    Rx_Cplt,
} Interface_Rx_State_e;
Interface_Rx_State_e RxBuffer_State = None;
int RxBuffer_index = 0;
typedef struct
{
    char mode; // 0待机，1速度，2位置,
    float speed;
    float position;

} Control_t;
Control_t control;
// 本函数内部使用，故不窜入参数
void Rx_Pack_Callback(void)
{
    float param[2] = {0};
    char cmd;
    memcpy(param, &aRxBuffer[RxBuffer_index - 2 * sizeof(param)], 2 * sizeof(param));
    cmd = aRxBuffer[RxBuffer_index - 2 * sizeof(param) - 1];
}

void interface_init(void)
{
    /* I2C初始化 */
    I2cHandle.Instance = I2C;                                 /* I2C */
    I2cHandle.Init.ClockSpeed = I2C_SPEEDCLOCK;               /* I2C通讯速度 */
    I2cHandle.Init.DutyCycle = I2C_DUTYCYCLE;                 /* I2C占空比 */
    I2cHandle.Init.OwnAddress1 = I2C_ADDRESS;                 /* I2C地址 */
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* 禁止广播呼叫 */
    I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* 允许时钟延长 */
    if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
    {
        Error_Handler();
    }
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_SYSCFG_CLK_ENABLE(); /* SYSCFG时钟使能 */
    __HAL_RCC_DMA_CLK_ENABLE();    /* DMA时钟使能 */
    __HAL_RCC_I2C_CLK_ENABLE();    /* I2C时钟使能 */
    __HAL_RCC_GPIOF_CLK_ENABLE();  /* GPIOF时钟使能 */

    /**I2C引脚配置
    PF0     ------> I2C1_SCL
    PF1     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; /* 开漏 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;     /* 上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_I2C; /* 复用为I2C */
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);    /* GPIO初始化 */
    /*复位I2C*/
    __HAL_RCC_I2C_FORCE_RESET();
    __HAL_RCC_I2C_RELEASE_RESET();
    /* I2C1中断初始化 */
    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0); /* 中断优先级设置 */
    HAL_NVIC_EnableIRQ(I2C1_IRQn);         /* 使能I2C中断 */

    /* DMA配置 */
    HAL_SYSCFG_DMA_Req(9);     /* DMA1_MAP选择为IIC_TX */
    HAL_SYSCFG_DMA_Req(0xA00); /* DMA2_MAP选择为IIC_RX */

    /* 配置DMA请求映像 */
    /* 配置DMA句柄（用于发送） */
    HdmaCh1.Instance = DMA1_Channel1;                       /* 选择DMA通道1 */
    HdmaCh1.Init.Direction = DMA_MEMORY_TO_PERIPH;          /* 方向为从存储器到外设 */
    HdmaCh1.Init.PeriphInc = DMA_PINC_DISABLE;              /* 禁止外设地址增量 */
    HdmaCh1.Init.MemInc = DMA_MINC_ENABLE;                  /* 使能存储器地址增量 */
    HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 外设数据宽度为8位 */
    HdmaCh1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* 存储器数据宽度位8位 */
    HdmaCh1.Init.Mode = DMA_NORMAL;                         /* 禁止循环模式 */
    HdmaCh1.Init.Priority = DMA_PRIORITY_VERY_HIGH;         /* 通道优先级为很高 */

    HAL_DMA_Init(&HdmaCh1);               /* 初始化DMA通道1 */
    __HAL_LINKDMA(hi2c, hdmatx, HdmaCh1); /* DMA1关联IIC_TX */

    /* 配置DMA句柄（用于接收） */
    HdmaCh2.Instance = DMA1_Channel2;                       /* 选择DMA通道1 */
    HdmaCh2.Init.Direction = DMA_PERIPH_TO_MEMORY;          /* 方向为从外设到存储 */
    HdmaCh2.Init.PeriphInc = DMA_PINC_DISABLE;              /* 禁止外设地址增量 */
    HdmaCh2.Init.MemInc = DMA_MINC_ENABLE;                  /* 使能存储器地址增量 */
    HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 外设数据宽度为8位 */
    HdmaCh2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    /* 存储器数据宽度位8位 */
    HdmaCh2.Init.Mode = DMA_NORMAL;                         /* 禁止循环模式 */
    HdmaCh2.Init.Priority = DMA_PRIORITY_HIGH;              /* 通道优先级为高 */

    HAL_DMA_Init(&HdmaCh2);               /* 初始化DMA通道1 */
    __HAL_LINKDMA(hi2c, hdmarx, HdmaCh2); /* DMA1关联IIC_RX */

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0); /* 中断优先级设置 */
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);         /* 使能DMA通道1中断 */

    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0); /* 中断优先级设置 */
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);         /* 使能DMA通道2_3中断 */

    while (HAL_I2C_Slave_Receive_DMA(&I2cHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (aRxBuffer[RxBuffer_index] = 'S')
    { // 寻找包头
        RxBuffer_State = Rx_Started;
    }
    else
    {
        RxBuffer_index = 0;
        while (HAL_I2C_Slave_Receive_DMA(&I2cHandle, &(aRxBuffer[RxBuffer_index]), 1) != HAL_OK)
        {
            Error_Handler();
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
    while (HAL_I2C_Slave_Receive_DMA(&I2cHandle, &(aRxBuffer[RxBuffer_index]), 1) != HAL_OK)
    {
        Error_Handler();
    }
}