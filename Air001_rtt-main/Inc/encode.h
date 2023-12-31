/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-12 15:32:47
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-13 14:01:10
 * @FilePath: \Air001_rtt-main\Inc\encode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ENCODE_H_
#define _ENCODE_H_

#include "air001xx_hal.h"
#include "air001_Core_Board.h"

#include "rtthread.h"

#include "motor_pwm.h"

/*
一圈脉冲：4
减速器一圈：400
*/
typedef struct
{
    int count_old;
	  int count;
	
    float speed;
	  float angle; 
	  uint8_t cycle;
	  uint8_t pluse;
	  int reduction_ratio;
	
}ENCODE;

/*
PID相关变量
*/
 struct PID {
  int Kp; //比例系数
  int Ki; //积分系数
  int Kd; //微分系数
  int setpoint; //设定值
  int input ; //输入值
  int output;  //输出值
  int error; //偏差值
  int error_last ; //上一次的偏差值
  int error_sum; //偏差累积值
};
 
extern struct PID pid_speed;
extern struct PID pid_angle;

void Encode_Handler();

void encode_Init(int singlePluse ,int reduction_ratio);

float get_Angle();
float get_Speed();

void set_Cycle(int cycle);
void set_Reduction_Ratio(int reduction_ratio);
void set_Single_Pluse(int pluse);
void set_Angle_Zero();

void motor_Contral(int pwm);

void Set_Speed(float Speed);
void Set_Angle(float Angle);
#endif