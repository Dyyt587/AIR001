
#include "encode.h"
//#include "Motor_PWM.h"
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

#define defaultcycle 10
#define defaultpluse 4
#define defaultreduction_ratio 100

ENCODE encode;

rt_timer_t encodeTimer;

struct rt_timer static_timer;


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
	int count = encode.count - encode.count_old;
   
    // 计算转速(count),数据类型转换
	
    encode.speed = (float)encode.count_old / (encode.reduction_ratio * encode.cycle) * 1000*60;// 转的圈数/时间*1000*60,将原来的r/ms转换成r/min

    // Serial.println(speedl);
    // count置零
    encode.count = 0;
	
		encode.count_old  = encode.count;

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

