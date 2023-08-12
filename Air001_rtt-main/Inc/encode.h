#ifndef _ENCODE_H_
#define _ENCODE_H_

#include "air001xx_hal.h"
#include "air001_Core_Board.h"

#include "rtthread.h"


void Encode_Handler();

void encode_Init(int singlePluse ,int reduction_ratio);

float get_Angle();
float get_Speed();

void set_Cycle(int cycle);
void set_Reduction_Ratio(int reduction_ratio);
void set_Single_Pluse(int pluse);
void set_Angle_Zero();


#endif