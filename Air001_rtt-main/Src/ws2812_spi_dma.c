#include "ws2812_spi_dma.h"





uint32_t gWS2812_TimeCNT;
 
// GRB格式
SPI_HandleTypeDef Spi1Handle;

DMA_HandleTypeDef HdmaCh1;
DMA_HandleTypeDef HdmaCh2;
DMA_HandleTypeDef HdmaCh3;
 
 void WS2812Init(void)
 {
   /*反初始化SPI配置*/
		Spi1Handle.Instance               = SPI1;                       /* SPI1 */
		Spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;    /* 2分频 */
		Spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;       /* 全双工 */
		Spi1Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* 时钟极性低 */
		Spi1Handle.Init.CLKPhase          = SPI_PHASE_1EDGE ;           /* 数据采样从第一个时钟边沿开始 */
		Spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI数据长度为8bit */
		Spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* 先发送MSB */
		Spi1Handle.Init.NSS               = SPI_NSS_SOFT;        /* NSS软件模式(硬件模式) */
		Spi1Handle.Init.Mode = SPI_MODE_MASTER;                         /* 配置为主机 */
		if (HAL_SPI_DeInit(&Spi1Handle) != HAL_OK)
		{
			Error_Handler();
		}
		
		/*SPI初始化*/
		if (HAL_SPI_Init(&Spi1Handle) != HAL_OK)
		{
			Error_Handler();
		}
 
 }
#if 0
 /**
  * @brief 初始化全局MSP
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief 初始化SPI的MSP
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  /* SPI1 初始化 */
  if (hspi->Instance == SPI1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();                   /* GPIOB时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();                   /* GPIOA时钟使能 */
    __HAL_RCC_SYSCFG_CLK_ENABLE();                  /* 使能SYSCFG时钟 */
    __HAL_RCC_SPI1_CLK_ENABLE();                    /* SPI1时钟使能 */
    __HAL_RCC_DMA_CLK_ENABLE();                     /* DMA时钟使能 */
    HAL_SYSCFG_DMA_Req(1);                          /* SPI1_TX DMA_CH1 */
    HAL_SYSCFG_DMA_Req(0x200);                      /* SPI1_RX DMA_CH2 */
    /*
      PA5-SCK  (AF0)
      PA6-MISO(AF0)
      PA7-MOSI(AF0)
      PA4-NSS(AF0)
    */
    /* SPI CS*/
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    if (hspi->Init.CLKPolarity == SPI_POLARITY_LOW)
    {
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else
    {
      GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    /*GPIO配置为SPI：SCK/MISO/MOSI*/
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*中断配置*/
    HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    /*DMA_CH1配置*/
    HdmaCh1.Instance                 = DMA1_Channel1;
    HdmaCh1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    HdmaCh1.Init.PeriphInc           = DMA_PINC_DISABLE;
    HdmaCh1.Init.MemInc              = DMA_MINC_ENABLE;
    if (hspi->Init.DataSize <= SPI_DATASIZE_8BIT)
    {
      HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      HdmaCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    }
    else
    {
      HdmaCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      HdmaCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    }
    HdmaCh1.Init.Mode                = DMA_NORMAL;
    HdmaCh1.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    /*DMA初始化*/
    HAL_DMA_Init(&HdmaCh1);
    /*DMA句柄关联到SPI句柄*/
    __HAL_LINKDMA(hspi, hdmatx, HdmaCh1);

    /*DMA_CH2配置*/
    HdmaCh2.Instance                 = DMA1_Channel2;
    HdmaCh2.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    HdmaCh2.Init.PeriphInc           = DMA_PINC_DISABLE;
    HdmaCh2.Init.MemInc              = DMA_MINC_ENABLE;
    if (hspi->Init.DataSize <= SPI_DATASIZE_8BIT)
    {
      HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      HdmaCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    }
    else
    {
      HdmaCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      HdmaCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    }
    HdmaCh2.Init.Mode                = DMA_NORMAL;
    HdmaCh2.Init.Priority            = DMA_PRIORITY_LOW;
    /*DMA初始化*/
    HAL_DMA_Init(&HdmaCh2);
    /*DMA句柄关联到SPI句柄*/
    __HAL_LINKDMA(hspi, hdmarx, HdmaCh2);
    /*DMA中断设置*/
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  }

}

#endif
#define LED_NUM 3	// LED灯珠个数

// 模拟bit码: 0x80为逻辑0, 0xF8为逻辑1
const uint8_t code[]={0x80,0xF8};


// 灯颜色缓存
RGBColor_TypeDef RGB_Data[LED_NUM] = {0};

/**
  * @brief			设置灯带颜色发送缓存
  * @param[in]		ID 颜色
  */
void Set_LEDColor(uint16_t LedId, RGBColor_TypeDef Color)
{
    RGB_Data[LedId].G = Color.G;
    RGB_Data[LedId].R = Color.R;
    RGB_Data[LedId].B = Color.B;
}

/**
  * @brief			SPI发送控制ws2812
  * @param[in]		待发送缓存
  */
static void SPI_Send_WS2812(uint8_t *SPI_RGB_BUFFER)
{
    // 判断上次DMA有没有传输完成
    //while(HAL_DMA_GetState(&HdmaCh1) != HAL_DMA_STATE_READY);
    // 发送一个24bit的RGB数据
    //HAL_SPI_Transmit_DMA(&Spi1Handle, SPI_RGB_BUFFER, 24);
    HAL_SPI_Transmit(&Spi1Handle, SPI_RGB_BUFFER, 24,1);
}

/**
  * @brief			控制WS2812
  * @param[in]		待发送缓存
  */
void RGB_Reflash(void)
{
	static uint8_t RGB_BUFFER[24]={0};
	uint8_t dat_b,dat_r,dat_g;
	// 将数组颜色转化为24个要发送的字节数据
    for (uint16_t i = 0; i < LED_NUM; i++) {
        dat_g = RGB_Data[i].G;
        dat_r = RGB_Data[i].R;
        dat_b = RGB_Data[i].B;
        for (uint16_t j = 0; j < 8; j++) {
            RGB_BUFFER[7-j] =code[dat_g & 0x01];
            RGB_BUFFER[15-j]=code[dat_r & 0x01];
            RGB_BUFFER[23-j]=code[dat_b & 0x01];
            dat_g >>=1;
            dat_r >>=1;
            dat_b >>=1;
        }
        SPI_Send_WS2812(RGB_BUFFER);
	}
}
