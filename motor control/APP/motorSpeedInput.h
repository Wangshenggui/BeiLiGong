#ifndef _MOTORSPEEDINPUT_H
#define _MOTORSPEEDINPUT_H

#include "main.h"


typedef struct
{
    int16_t Speed;
    int32_t Location;
    int32_t LastLocation;
}MotorSpeedStructure;
extern MotorSpeedStructure MotorSpeedStru1;


void TIM1Input_Capture_Init(u16 arr, u16 psc);
int16_t GetMotorSpeed(void);


#endif
