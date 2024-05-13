#ifndef _MOTORCONTROL_H
#define _MOTORCONTROL_H

#include "system.h"
#include "pid.h"
#include "timer.h"
#include "adc.h"
#include "usart1.h"
#include "motor_pwm.h"

//����Ŀ��ֵ�ٶȣ�ԽС����ʱ��Խ��������ᵼ�³���
#define CLOSER_K 0.1050f
//�ӳٿ���ϵ����Խ���ӳ�Խ��
#define FILTER_K 0.00f

//���˽Ƕȶ�Ӧ��ADCֵ�������ת�Ƕ�0-90����Ӳ���йأ�\
y={SWIVEL_Angle/(MAX_ANGLE-MIN_ANGLE)}*(x-MIN_ANGLE)
#define MAX_ANGLE 2300z
#define MIN_ANGLE 1800
#define SWIVEL_Angle 30.0f

//һ�׵�ͨ
typedef struct
{
    float prev_output;
    float output;
    float Filter_k;//�ӳٿ���ϵ����Խ���ӳ�Խ��
} LowFilter;
extern LowFilter M1LowFilter;
extern LowFilter M2LowFilter;
extern LowFilter M3LowFilter;

//����������
typedef struct
{
    float current;//��ǰֵ
    float target;//Ŀ��ֵ
    float closer_k;//����ϵ��
    float diff;//��ֵ
} CloserStructure;
extern CloserStructure M1Closer;
extern CloserStructure M2Closer;
extern CloserStructure M3Closer;



float closerToTarget(CloserStructure* Closer);
    
void Motor1Control(void);
void Motor2Control(void);
void Motor3Control(void);





#endif
