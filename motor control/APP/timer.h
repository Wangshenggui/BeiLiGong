#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"


/* ��������ṹ�� */
typedef struct
{
	uint8_t state;          /*���״̬*/
	float current;          /*�������*/
	float volatage;         /*�����ѹ*/
	float power;            /*�������*/
	float speed;            /*���ʵ���ٶ�*/
	float location;         /*���λ��*/
	int32_t motor_pwm;      /*���ñȽ�ֵ��С */
	u16 LocationSpeed;      /*λ�û����*/
} Motor_TypeDef;
extern Motor_TypeDef g_motor_data1;  /*�����������*/
extern Motor_TypeDef g_motor_data2;  /*�����������*/
extern Motor_TypeDef g_motor_data3;  /*�����������*/

void TIM4_InterruptInit(u16 arr, u16 psc);






#endif
