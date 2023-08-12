#ifndef WS2812_SPI_DMA_H
#define WS2812_SPI_DMA_H

#include "main.h"
#include "stdint.h"
#include "string.h"
 
 typedef struct {
  uint8_t R;
  uint8_t G;
  uint8_t B;
}RGBColor_TypeDef;	//颜色结构体

void WS2812Init(void);
void RGB_Reflash(void);
void Set_LEDColor(uint16_t LedId, RGBColor_TypeDef Color);

 

#endif
