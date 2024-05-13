#ifndef _MOTORCONTROL_H
#define _MOTORCONTROL_H

#include "system.h"
#include "pid.h"
#include "timer.h"
#include "adc.h"
#include "usart1.h"
#include "motor_pwm.h"

//靠近目标值速度，越小控制时间越长，过大会导致超调
#define CLOSER_K 0.1050f
//延迟控制系数，越大延迟越高
#define FILTER_K 0.00f

//两端角度对应的ADC值（假设可转角度0-90，与硬件有关）\
y={SWIVEL_Angle/(MAX_ANGLE-MIN_ANGLE)}*(x-MIN_ANGLE)
#define MAX_ANGLE 2300z
#define MIN_ANGLE 1800
#define SWIVEL_Angle 30.0f

//一阶低通
typedef struct
{
    float prev_output;
    float output;
    float Filter_k;//延迟控制系数，越大延迟越高
} LowFilter;
extern LowFilter M1LowFilter;
extern LowFilter M2LowFilter;
extern LowFilter M3LowFilter;

//趋近防超调
typedef struct
{
    float current;//当前值
    float target;//目标值
    float closer_k;//靠近系数
    float diff;//差值
} CloserStructure;
extern CloserStructure M1Closer;
extern CloserStructure M2Closer;
extern CloserStructure M3Closer;



float closerToTarget(CloserStructure* Closer);
    
void Motor1Control(void);
void Motor2Control(void);
void Motor3Control(void);





#endif
