#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"


/* 电机参数结构体 */
typedef struct
{
	uint8_t state;          /*电机状态*/
	float current;          /*电机电流*/
	float volatage;         /*电机电压*/
	float power;            /*电机功率*/
	float speed;            /*电机实际速度*/
	float location;         /*电机位置*/
	int32_t motor_pwm;      /*设置比较值大小 */
	u16 LocationSpeed;      /*位置环输出*/
} Motor_TypeDef;
extern Motor_TypeDef g_motor_data1;  /*电机参数变量*/
extern Motor_TypeDef g_motor_data2;  /*电机参数变量*/
extern Motor_TypeDef g_motor_data3;  /*电机参数变量*/

void TIM4_InterruptInit(u16 arr, u16 psc);






#endif
