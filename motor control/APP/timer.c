#include "timer.h"
#include "pid.h"
#include "motorSpeedInput.h"
#include "motor_pwm.h"
#include "adc.h"
#include "usart1.h"
#include "motorControl.h"


Motor_TypeDef g_motor_data1;  /*�����������*/
Motor_TypeDef g_motor_data2;  /*�����������*/
Motor_TypeDef g_motor_data3;  /*�����������*/

void TIM4_InterruptInit(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;							//�ж����ȼ�NVIC����

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 			//ʱ��ʹ��

    //��ʱ��TIM4��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;		 				//�Զ���װ�ؼĴ������ڵ�ֵ1/CK_CNT=1us,1000x1us=1ms	
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 						//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��ƵֵCK_CNT=CK_INT/(71+1)=1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//TIM_CKD_DIV1��.h�ļ����Ѿ�����õģ�TIM_CKD_DIV1=0��Ҳ����ʱ�ӷ�Ƶ����Ϊ0
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);						 //ʹ��ָ����TIM4�ж�,��������ж�

    TIM_Cmd(TIM4, ENABLE);  										//ʹ��TIM

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  				//TIM4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  		//��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			//�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  								//��ʼ��NVIC�Ĵ���
}


void TIM4_IRQHandler(void)   										//TIM3�ж�
{
    static u16 i = 0;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  			//���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 				//���TIMx�����жϱ�־������������ʱ��
        i++;
        if (i == 10)
        {
            i = 0;
            //��ȡADC��Ϣ
//            Get_ADC_Speed();
            g_motor_data1.speed = ADC_Speed[0];
            g_motor_data2.speed = ADC_Speed[1];
            g_motor_data3.speed = ADC_Speed[2];
        }
        
        //111111111111111111111111
        Motor1Control();
        //222222222222222222222222
        Motor2Control();
        //333333333333333333333333
        Motor3Control();
    }
}





