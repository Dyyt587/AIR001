
#include "encode.h"

#include "stdbool.h"

 
#define defaultcycle 10
#define defaultpluse 4
#define defaultreduction_ratio 100

ENCODE encode;

rt_timer_t encodeTimer;

struct rt_timer static_timer;

struct PID pid_speed;
struct PID pid_angle;

struct rt_thread  PID_Thread;
rt_uint32_t PID_Thread_num[64];

bool _IF_SPEED = true;

#define a_speed   0.5f                // 滤波系数a（0-1）


int filter(int new_val) 
{ 
 static float value; 
 float ff=(float)new_val;
 int out=a_speed*value + (1.0f-a_speed)*ff;
 value = ff;
return out;
 }

void _ENCODE_GPIO_Init(void)
{

  GPIO_InitTypeDef  GPIO_InitStruct_EXIT,GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();                  /* 使能GPIOA时钟 */
  GPIO_InitStruct_EXIT.Mode  = GPIO_MODE_IT_FALLING;  /* GPIO模式为下降沿中断 */
  GPIO_InitStruct_EXIT.Pull  = GPIO_PULLUP;           /* 上拉 */
  GPIO_InitStruct_EXIT.Speed = GPIO_SPEED_FREQ_HIGH;  /* 速度为高速 */
  GPIO_InitStruct_EXIT.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_EXIT);

  /* 使能EXTI中断 */
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  /* 配置中断优先级 */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	
	 GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;                    /* 使能上拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;          /* GPIO速度 */  
  /* GPIO初始化 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
}

/*计算速度*/
void _countSpeed(void* a){


}

//更新PID
void _pidRun(struct PID *pid) {
	int ki_out ;
	
  pid->error = pid->setpoint - pid->input; //计算偏差值
  pid->error_sum += pid->error; //计算偏差累积值
	ki_out = pid->Ki * pid->error_sum ;
	if(ki_out>400)ki_out =400;
	
  pid->output = pid->Kp * pid->error + ki_out + pid->Kd * (pid->error - pid->error_last); //计算输出值

  pid->error_last = pid->error; //更新上一次的偏差值
  
}


void _PID_Thread_entry(void *param ){
	while (1){
		
			int count = encode.count - encode.count_old;
   
    // 计算转速(count),数据类型转换
	
    encode.speed = count * 1000*60 / (encode.reduction_ratio * encode.cycle);// 转的圈数/时间*1000*60,将原来的r/ms转换成r/min

    // Serial.println(speedl);
   
		encode.count_old  = encode.count;
		
  encode.angle=encode.count*360.0/(defaultpluse *defaultreduction_ratio);//把count转换成轮子角度
		                              
  if(_IF_SPEED)
			pid_angle.output = pid_angle.setpoint;
   else {
			pid_angle.input =  (int)(encode.angle*100);
			_pidRun(&pid_angle);
	}
  
  pid_speed.setpoint = pid_angle.output;
	
	pid_speed.input =  (int)(encode.speed*100);
  _pidRun(&pid_speed);
		

		
	motor_Contral(filter(pid_speed.output)/100);
	
	rt_thread_mdelay(encode.cycle);
	}
}


/*初始化编码器*/
void encode_Init(int singlePluse,int reduction_ratio){
	
	_ENCODE_GPIO_Init();

  encode.pluse = singlePluse;
  encode.reduction_ratio =  reduction_ratio;
  encode.cycle = defaultcycle;
  encode.count = 0;
  encode.count_old = 0;
	
  rt_timer_init (&static_timer,"encodeTimer",_countSpeed,NULL,encode.cycle,RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER );
	
  rt_timer_start (&static_timer);
  rt_thread_init(&PID_Thread,
                        "PID_Thread",
                        _PID_Thread_entry,
                        NULL,
                        PID_Thread_num,
                        sizeof (PID_Thread_num),
                        1,
                        1);
	rt_thread_startup	(&PID_Thread);										
	

}

/*外部中断使能*/

void Encode_Handler(){
 //任何引脚外部中断触发，都进入此
	assert_param(IS_GPIO_PIN(GPIO_PIN_6));//判断寄存器引脚

		if ((GPIOA->IDR & GPIO_PIN_6) != 0x00u)//判断是否为PA6
  {
    encode.count++;
  }
  else
  {
    encode.count--;
  }
	
	 __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);//清除标志位
}	

/*清除角度*/
void set_Angle_Zero(){
  encode.count = 0;
}

/*设置时钟*/
void set_Cycle(int cycle){
 encode.cycle  = cycle;
}

/*设置减速器*/
void set_Reduction_Ratio(int reduction_ratio){
  encode.reduction_ratio = reduction_ratio;
}

/*设置单圈脉冲*/
void set_Single_Pluse(int pluse){
  encode.pluse = pluse;
}

/*获取角度*/
float get_Angle(){
return (float)encode.count*360.0/400.0;
}

/*获取速度*/
float get_Speed(){
return encode.speed;
}

void motor_Contral(int pwm){
    if(pwm>0){
    Motor_Set_CH4(0);
    Motor_Set_CH3(pwm);
    }else{
    Motor_Set_CH4(-pwm);
    Motor_Set_CH3(0);
    }
}



void Set_Speed(int Speed){
 	pid_angle.setpoint  = (int)(Speed*100);
	
  _IF_SPEED = true;
}

void Set_Angle(int Angle){
  pid_angle.setpoint  = Angle*100;

    _IF_SPEED = false;

}



