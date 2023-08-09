/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-09 13:35:21
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-09 18:05:39
 * @FilePath: \AIR001\AIR001.ino
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */











/**
 * 实现ws2812
 * 实现iic通讯
 *  协议：
 *      S [param1][param2] 由主机发送
 *          param1: reboot 重启
 *                  linkStart   启动动态地址分配模式
 *                  linkEnd     关闭动态地址分配模式
 *                  address     分配或获取地址 oprntional [7位地址]
 *                  led         设置led灯显示模式
 *                      param2: 0 常亮，红
 *                              1 常亮，绿
 *                              2 常亮，蓝
 *                              3 
 *      M [param1] 由从机发送
 *          param1: address     从机请求获取地址，仅在动态地址分配模式下有用
 *                  error    从机反馈异常
 *                      param2: overSpeed
 *                              overCurrent
 *                              overTemperature
 *                              syetemError
 *                              overVoltage
 *                              unknownError
 *                 vbus
 *          
 *      V[speed rmp]        转每分钟  由主/从机发送
 *      T[Torque N*m]       牛米      由主/从机发送  
 *      P[position degree]  一圈360   由主/从机发送
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