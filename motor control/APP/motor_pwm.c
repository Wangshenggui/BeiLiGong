#include "motor_pwm.h"
#include "Motor.h"



void TIM3_PWM_CH1_3_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = { 0 };
    TIM_OCInitTypeDef TIM_OCInitStructure = { 0 };

    //����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //CH1,CH2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //CH3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //��ʱ������
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    //ͨ��1����Ƚ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    //ͨ��2����Ƚ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    //ͨ��3����Ƚ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}



//���PWM����(n=1 2 3--value=0-99)
void MotorSetPWMValue(u8 n, int value)
{
    if (value < 0)
    {
        value = -value;
        MotorZF_Control(n, CCW);
    }
    else
    {
        value = value;
        MotorZF_Control(n, CW);
    }

    if (value > 100)
    {
        value = 100;
    }

    if (n == 1)
        TIM3->CH1CVR = value;
    else if (n == 2)
        TIM3->CH2CVR = value;
    else if (n == 3)
        TIM3->CH3CVR = value;
    else
        return;
}






