#include "main.h"
#include "system.h"
#include "led.h"
#include "motor_pwm.h"
#include "usart1.h"
#include <stdio.h>
#include "motorSpeedInput.h"
#include "timer.h"
#include "pid.h"
#include "Motor.h"
#include <stdbool.h>
#include "key.h"
#include "iwdg.h"
#include "adc.h"
#include "motorControl.h"

extern uint16_t Target1;
extern uint16_t Target2;
extern uint16_t Target3;

int main(void)
{
    
//    SCB->VTOR = FLASH_BASE | 0x5000;
    
    static u16 iwdg_cnt = 0;

    SystemCoreClockUpdate();
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    USART1_Init(57600);
    //20ms��ʱ��
//    TIM4_InterruptInit(500 - 1, 720 - 1);
//    TIM4_InterruptInit(10000 - 1, 72 - 1);
    MotorInit();//�����ʼ��
    pid_init();//PID������
    Key_Init();//���԰���
    //ADC��ʼ��
    ADC_DMA_CH1_3_Init((u32)ADCBuf, 900);
    while(!ADCBuf[899]);//�ȴ�ADC��һ�βɼ����
    Get_ADC_ConversionVal();//��ȡADC�Ƕ���Ϣ
    //��ʼ��һЩ����-------------------------------------------------------------------------
    M1Closer.closer_k=CLOSER_K;
    M2Closer.closer_k=CLOSER_K;
    M3Closer.closer_k=CLOSER_K;
    M1LowFilter.Filter_k=FILTER_K;
    M2LowFilter.Filter_k=FILTER_K;
    M3LowFilter.Filter_k=FILTER_K;
    /*��ֹ�ϵ���ת*/
    USART1_Stru.Angle_Value[0]=ADC_Value[0];
    USART1_Stru.Angle_Value[1]=ADC_Value[1];
    USART1_Stru.Angle_Value[2]=ADC_Value[2];
    //��ǰ����ͨ�˲�
    M1LowFilter.output=USART1_Stru.Angle_Value[0]*1;
    M2LowFilter.output=USART1_Stru.Angle_Value[1]*1;
    M3LowFilter.output=USART1_Stru.Angle_Value[2]*1;
    M1LowFilter.prev_output=USART1_Stru.Angle_Value[0]*1;
    M2LowFilter.prev_output=USART1_Stru.Angle_Value[1]*1;
    M3LowFilter.prev_output=USART1_Stru.Angle_Value[2]*1;
    //��ǰ�����������
    M1Closer.current=USART1_Stru.Angle_Value[0]*1;
    M1Closer.target=USART1_Stru.Angle_Value[0]*1;
    M2Closer.current=USART1_Stru.Angle_Value[1]*1;
    M2Closer.target=USART1_Stru.Angle_Value[1]*1;
    M3Closer.current=USART1_Stru.Angle_Value[2]*1;
    M3Closer.target=USART1_Stru.Angle_Value[2]*1;
    //�����������
    g_location_pid1.SetPoint=USART1_Stru.Angle_Value[0]*1;
    g_location_pid2.SetPoint=USART1_Stru.Angle_Value[1]*1;
    g_location_pid3.SetPoint=USART1_Stru.Angle_Value[2]*1;
    
    while(1)
    {
        g_motor_data1.location = (float)ADC_Value[0];/* ��ȡ��ǰ��λ��ֵ������λ�ñջ����� */
        //������������ֹ����
        M1Closer.current=g_location_pid1.SetPoint;
        M1Closer.target=USART1_Stru.Angle_Value[0];
        g_location_pid1.SetPoint=closerToTarget(&M1Closer);
        
        g_motor_data2.location = (float)ADC_Value[1];
        //������������ֹ����
        M2Closer.current=g_location_pid2.SetPoint;
        M2Closer.target=USART1_Stru.Angle_Value[1];
        g_location_pid2.SetPoint=closerToTarget(&M2Closer);
        
        g_motor_data3.location = (float)ADC_Value[2];/* ��ȡ��ǰ��λ��ֵ������λ�ñջ����� */
        //������������ֹ����
        M3Closer.current=g_location_pid3.SetPoint;
        M3Closer.target=USART1_Stru.Angle_Value[2];
        g_location_pid3.SetPoint=closerToTarget(&M3Closer);
        
        if(USART1_Stru.Angle_Value[0] == g_location_pid1.SetPoint \
            && USART1_Stru.Angle_Value[1] == g_location_pid2.SetPoint \
        && USART1_Stru.Angle_Value[2] == g_location_pid3.SetPoint)
        {
            break;
        }
        
        delay_ms(10);
    }
    TIM4_InterruptInit(20000 - 1, 72 - 1);
    //��ʼ��һЩ����-------------------------------------------------------------------------
    //���Ź���ʼ��
    IWDG_Feed_Init(IWDG_Prescaler_64, 625);//1s IWDG reset
    
    led_init();
    while (1)
    {
        iwdg_cnt++;
        //ι��
        if (iwdg_cnt == 5)
        {
            iwdg_cnt = 0;
            IWDG->CTLR = ((uint16_t)0xAAAA);
        }
        delay_ms(5);
        
        static uint8_t x=0;
        x++;
        if(x==10)
        {
            x=0;
            PAout(3)=!PAout(3);
        }
        
        
        
//        printf("%d,%d,%d,%d\r\n",\
//        ADC_Value[1],\
//        (int)USART1_Stru.Angle_Value[1],\
//            -g_motor_data2.motor_pwm,\
//        (int)g_location_pid2.SetPoint\
//            );
        
        printf("%d,%d,%d,%d\r\n",\
        ADC_Value[2],\
        (int)USART1_Stru.Angle_Value[2],\
            -g_motor_data3.motor_pwm,\
        (int)g_location_pid3.SetPoint\
            );
        
        
//        printf("%d,%d,%d\r\n",\
//        ADC_Value[0],\
//        ADC_Value[1],\
//        ADC_Value[2]\
//            );
        
        Get_ADC_Speed();
    }
}






