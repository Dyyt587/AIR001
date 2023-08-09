/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-09 13:35:21
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-09 15:44:07
 * @FilePath: \AIR001\AIR001.ino
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */











/**
 * 实现ws2812
 * 实现iic通讯
 * 实现电机转动
 * 实现电机位置闭环
 * 实现通讯协议
 * 实现动态调参
 *





*/


#include "Wire.h"



void setup()
{
    Serial.begin(115200);
    Wire.begin();
}

void loop()
{
    Wire.beginTransmission(8);
    Wire.write('8');
    Wire.endTransmission();

}