/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-09 13:35:21
 * @LastEditors: dyyt 805207319@qq.com
 * @LastEditTime: 2023-08-09 23:04:08
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
 *
 *      M [param1] 由从机发送
 *          param1: requireAddr     从机请求获取地址，仅在动态地址分配模式下有用
 *                  error    从机反馈异常
 *                      param2: overSpeed
 *                              overCurrent
 *                              overTemperature
 *                              syetemError
 *                              overVoltage
 *                              unknownError
 *
 *      V[speed rmp]        转每分钟  由主/从机发送
 *      T[Torque N*m]       牛米      由主/从机发送
 *      P[position degree]  一圈360   由主/从机发送
 *      I[param1]           获取状态  由主/从机发送
 *          param1: all  所有信息
 *                  vbus 总线电压
 *                  version 版本号，id号，用户定义号
 *                  0000 0000 0000 0000 0000 0000 0000 0000
 *                 |  版本号 ||        唯一id              |
 *                  0000 0000 0000 0000 0000 0000 0000 0000
 *                 |            用户自定义数据             |
 *                  temperature 温度
 *
 * 实现电机转动
 * 实现电机位置闭环
 * 实现通讯协议
 * 实现动态调参
 *





*/

#include "Wire.h"
#include "WS2812FX.h"
#include "SPI.h"
#include "String.h"
#include "stdlib.h"








void sysStatueChange(void)
{}


class cmdmanger
{
    enum
    {
        none = 0U,
        S,
        M,

        V,
        T,
        P,

        I,
          reboot,    // 重启
          linkStart, // 启动动态地址分配模式
          linkEnd,   // 关闭动态地址分配模式
          address,   // 分配地址 oprntional [7位地址]
          led,       // 设置led灯显示模式

          requireAddr,
          error, //    从机反馈异常
          overSpeed,
          overCurrent,
          overTemperature,
          syetemError,
          overVoltage,
          unknownError,
          all,     // 所有信息
          vbus,    // 总线电压
          version, // 版本号，id号，用户定义号
          otherData = 99999,
    };
    typedef int cmdID;
    typedef union
    {
        float Float;
        int Int;

    } cmdData;
    cmdmanger()
    {
    }
    int getcmd(char* str, cmdID cmds[3], cmdData *data)
    {
        #define FIND_CMD(x) if(str[0]==x)
        #define FIND_PARAM1(x) if(strpbrk(str,x)==1)
        #define FIND_PARAM2(x) if(strpbrk(str,x)<64)
        FIND_CMD('S')
        {
            cmds[0]=S;

            FIND_PARAM1("reboot")
            {
                cmds[1]=reboot;
            }
            FIND_PARAM1("linkStart")
            {
                cmds[1]=linkStart;

            }
            FIND_PARAM1("linkEnd")
            {
                cmds[1]=linkEnd;
            }
            FIND_PARAM1("address")
            {
                cmds[1]=address;
                cmds[2]=otherData;
                
                data->Int=0xffffffff;
                memcopy(str[8],data->Int,4);
            }
            FIND_PARAM1("led")
            {
                cmds[1]=led;
                cmds[2]=otherData;
                //TODO:!!!!!!!!!!!!!!!!!!!
                data->Int=0xffffffff;
                memcopy(str[4],data->Int,4);
            }
           
        }else
        FIND_CMD('M')
        {
            cmds[0]=M;
            FIND_PARAM1("requireAddr")
            {
                cmds[1]=requireAddr;                
            }else
            FIND_PARAM1("led")
            {
                cmds[1]=error;
                FIND_PARAM2("overSpeed")
                {
                    cmds[2]=overSpeed;

                }else
                FIND_PARAM2("overCurrent")
                {
                    cmds[2]=overCurrent;

                }else
                FIND_PARAM2("overTemperature")
                {
                    cmds[2]=overTemperature;

                }else
                FIND_PARAM2("syetemError")
                {
                    cmds[2]=syetemError;

                }else
                FIND_PARAM2("overVoltage")
                {
                    cmds[2]=overVoltage;

                }else
                FIND_PARAM2("unknownError")
                {
                    cmds[2]=unknownError;

                }else{
                    cmds[2]=none;
                }
            }

        }else
        FIND_CMD('V')
        {
            cmds[0]=V;
            cmds[1]=none;
            data->Float=0;
            memcopy(str[1],data->Float,4);
        }else
        FIND_CMD('T')
        {
            cmds[0]=T;
            cmds[1]=none;
            data->Float=0;
            memcopy(str[1],data->Float,4);
        }else
        FIND_CMD('P')
        {
            cmds[0]=P;
            cmds[1]=none;
            
            memcopy(str[1],data->Float,4);
        }else
        FIND_CMD('I')
        {
            cmds[0]=I;
            FIND_PARAM1("all")
            {
            cmds[1]=all;
            cmds[2]=none;

            }
            FIND_PARAM1("vbus")
            {
            cmds[1]=vbus;
            cmds[2]=none;

            }
            FIND_PARAM1("version")
            {
            cmds[1]=version;
            cmds[2]=none;

            }
        }

    }
    int cmdHandle(cmdID cmds[3], cmdData *data)
    {

    }
};

/**
 * @description: 将字符串拆分成命令
 * @param {char*} str
 * @return {*}
 */
int cmd_str2cmd(char *str)
{
}

#define LED_COUNT 1
#define DATA_PIN PA_4  // MOSI/SPI2
#define CLOCK_PIN PF_0 // SCLK/SPI2

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
    while (1 < Wire.available())
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