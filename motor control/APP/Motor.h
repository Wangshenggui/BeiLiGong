#ifndef _MOTOR_H
#define _MOTOR_H

#include "main.h"

#define CW 1
#define CCW 0


void MotorZF_GPIO_Init(void);
void MotorInit(void);
void MotorZF_Control(u8 n, u8 ccw);







#endif
