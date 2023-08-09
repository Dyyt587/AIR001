/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-09 13:35:21
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-09 17:29:41
 * @FilePath: \AIR001\AIR001.ino
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */











/**
 * 实现ws2812
 * 实现iic通讯
 *  协议：
 *      A [param1]
 *          param1: reboot 重启
 *                  LinkStart 启动动态地址分配模式
 *                  LinkEnd 关闭动态地址分配模式
 * 
 *      V[speed rmp]        转每分钟
 *      T[Torque N*m]       牛米
 *      P[position degree]  一圈360
 * 实现电机转动
 * 实现电机位置闭环
 * 实现通讯协议
 * 实现动态调参
 *





*/


#include "Wire.h"
#include "WS2812FX.h"
#include "SPI.h"







#define LED_COUNT 1
#define DATA_PIN   PA_4  // MOSI/SPI2
#define CLOCK_PIN  PF_0  // SCLK/SPI2

// Create the global ws2812fx instance.
// Note: APA102 LEDs expect the pixel data in blue/green/red order, so use
// the NEO_BGR flag. For other types of LEDs change it accordingly.
WS2812FX ws2812fx = WS2812FX(LED_COUNT, DATA_PIN, NEO_RGB + NEO_KHZ800);

/**
 * @description: iic从机的处理函数
 * @param {int} callback
 * @return {*}
 */
void IIC_Handle(int callback)
{
    while(1< Wire.available())
    {
        char c = Wire.read();
        Serial.print(c);
    }
    char x = Wire.read();
    Serial.println(x);

}
void setup()
{
    Serial.begin(115200);

    Wire.setSDA(PF_0);
    Wire.setSCL(PF_1);

    Wire.begin(0x0a);
    Wire.onReceive(IIC_Handle);

}

void loop()
{
    Serial.print("tt1");
    // Wire.beginTransmission(8);
    // Wire.write('8');
    // Wire.endTransmission(true);

}