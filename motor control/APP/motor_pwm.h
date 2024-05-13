#ifndef _MOTOR_PWM_H
#define _MOTOR_PWM_H

#include "main.h"

#define MotorN1 1
#define MotorN2 2
#define MotorN3 3

void TIM3_PWM_CH1_3_Init(u16 arr, u16 psc);
void MotorSetPWMValue(u8 n, int value);






#endif
