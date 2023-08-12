#include "motor_driver.h"

void motor_Contral(int pwm){
    if(pwm>0){
    Motor_Set_CH3(0);
    Motor_Set_CH4(pwm);
    }


}
