#include "ws2812_spi_dma.h"





uint32_t gWS2812_TimeCNT;
 
// GRB格式
SPI_HandleTypeDef Spi1Handle;
SPI_HandleTypeDef Spi1Handle2;

DMA_HandleTypeDef HdmaCh1;
DMA_HandleTypeDef HdmaCh2;
DMA_HandleTypeDef HdmaCh3;
 
 void WS2812Init(void)
 {
	 
	 
	 	Spi1Handle2.Instance               = SPI2;                       /* SPI1 */
		Spi1Handle2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;    /* 2分频 */
		Spi1Handle2.Init.Direction         = SPI_DIRECTION_1LINE;       /* 全双工 */
		Spi1Handle2.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* 时钟极性低 */
		Spi1Handle2.Init.CLKPhase          = SPI_PHASE_1EDGE ;           /* 数据采样从第一个时钟边沿开始 */
		Spi1Handle2.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI数据长度为8bit */
		Spi1Handle2.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* 先发送MSB */
		Spi1Handle2.Init.NSS               = SPI_NSS_SOFT;        /* NSS软件模式(模式) */
		Spi1Handle2.Init.Mode = SPI_MODE_MASTER;
		

		
		if (HAL_SPI_DeInit(&Spi1Handle2) != HAL_OK)
		{
			Error_Handler();
		}
		
		/*SPI初始化*/
		if (HAL_SPI_Init(&Spi1Handle2) != HAL_OK)
		{
			Error_Handler();
		}
 }
#if 0


#endif
#define LED_NUM 1	// LED灯珠个数

// 模拟bit码: 0x80为逻辑0, 0xF8为逻辑1
const uint8_t code[]={0xc0,0xF0};


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
    while(HAL_SPI_GetState(&Spi1Handle2) != HAL_SPI_STATE_READY)
		{
			printf("state%d\r\n",HAL_SPI_GetState(&Spi1Handle2));
		}
 //发送一个24bit的RGB数据
HAL_SPI_Transmit_IT(&Spi1Handle2, SPI_RGB_BUFFER, 24);
   // HAL_SPI_Transmit(&Spi1Handle2, SPI_RGB_BUFFER, 24,1);
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
