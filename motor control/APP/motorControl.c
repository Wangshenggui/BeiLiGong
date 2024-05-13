#include "motorControl.h"


LowFilter M1LowFilter;
LowFilter M2LowFilter;
LowFilter M3LowFilter;

CloserStructure M1Closer;
CloserStructure M2Closer;
CloserStructure M3Closer;

float closerToTarget(CloserStructure* Closer)
{
    if (Closer->current < Closer->target) 
    {
        Closer->current++;
    }
    else if (Closer->current > Closer->target) 
    {
        Closer->current--;
    }

    // ������һ����Ҫ���ٵĲ�ֵ
    Closer->diff = Closer->target - Closer->current;
    Closer->diff = Closer->diff * Closer->closer_k;//diff/100

    if (Closer->diff != 0) 
    {
        if (Closer->diff > 0) 
        {
            Closer->current += Closer->diff;
        }
        else 
        {
            Closer->current -= -Closer->diff;
        }
    }

    return Closer->current;
}

// һ�׵�ͨ�˲�������
static float lowPassFilter(LowFilter* Filter, float input_value, float alpha)
{
    float one_minus_alpha = 1.0f - alpha;
    Filter->output = (one_minus_alpha * input_value) + (alpha * Filter->prev_output);
    Filter->prev_output = Filter->output;
    return Filter->output;
}


void Motor1Control()
{
    //----------------------------------------------------------------------------------------
    g_motor_data1.location = (float)ADC_Value[0];/* ��ȡ��ǰ��λ��ֵ������λ�ñջ����� */

    //������������ֹ����
    M1Closer.current=g_location_pid1.SetPoint;
    M1Closer.target=USART1_Stru.Angle_Value[0];
    g_location_pid1.SetPoint=closerToTarget(&M1Closer);
    //��ͨ�˲����ӳٿ���
    g_motor_data1.location=lowPassFilter(&M1LowFilter,g_motor_data1.location,M1LowFilter.Filter_k);
    
    if(ADC_Value[0] - USART1_Stru.Angle_Value[0] <= 5 && ADC_Value[0] - USART1_Stru.Angle_Value[0] >= -5)
    {
        g_motor_data1.motor_pwm = 0;
        MotorSetPWMValue(MotorN1, g_motor_data1.motor_pwm);
        return;
    }
    else if(ADC_Value[0] < 1104 && ADC_Value[0] > 2904)
    {
        g_motor_data1.motor_pwm = 0;
        MotorSetPWMValue(MotorN1, g_motor_data1.motor_pwm);
        return;
    }
    else
    {
        g_motor_data1.motor_pwm = increment_pid_ctrl(&g_location_pid1, g_motor_data1.location);    /* λ�û�PID���ƣ��⻷�� */
    }
    
#if PID_MODE==1//ʹ�ô���
    if (g_motor_data1.motor_pwm >= 80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data1.motor_pwm = 80;
    if (g_motor_data1.motor_pwm <= -80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data1.motor_pwm = -80;

    g_speed_pid1.SetPoint = g_motor_data1.motor_pwm;/* ����Ŀ���ٶȣ��⻷�����Ϊ�ڻ����� */
    g_motor_data1.motor_pwm = increment_pid_ctrl(&g_speed_pid1, g_motor_data1.speed);          /* �ٶȻ�PID���ƣ��ڻ��� */
#endif

    if (g_motor_data1.motor_pwm >= 80)           /* ����ռ�ձ� */
        g_motor_data1.motor_pwm = 80;
    if (g_motor_data1.motor_pwm <= -80)           /* ����ռ�ձ� */
        g_motor_data1.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN1, -g_motor_data1.motor_pwm);
}

void Motor2Control()
{
    //----------------------------------------------------------------------------------------
    g_motor_data2.location = (float)ADC_Value[1];/* ��ȡ��ǰ��λ��ֵ������λ�ñջ����� */

    //������������ֹ����
    M2Closer.current=g_location_pid2.SetPoint;
    M2Closer.target=USART1_Stru.Angle_Value[1];
    g_location_pid2.SetPoint=closerToTarget(&M2Closer);
    //��ͨ�˲����ӳٿ���
    g_motor_data2.location=lowPassFilter(&M2LowFilter,g_motor_data2.location,M2LowFilter.Filter_k);
    
    if(ADC_Value[1] - USART1_Stru.Angle_Value[1] <= 5 && ADC_Value[1] - USART1_Stru.Angle_Value[1] >= -5)
    {
        g_motor_data2.motor_pwm = 0;
        MotorSetPWMValue(MotorN2, g_motor_data2.motor_pwm);
        return;
    }
    else if(ADC_Value[1] < 1104 && ADC_Value[1] > 2904)
    {
        g_motor_data2.motor_pwm = 0;
        MotorSetPWMValue(MotorN2, g_motor_data2.motor_pwm);
        return;
    }
    else
    {
        g_motor_data2.motor_pwm = increment_pid_ctrl(&g_location_pid2, g_motor_data2.location);    /* λ�û�PID���ƣ��⻷�� */
    }
    
#if PID_MODE==1//ʹ�ô���
    if (g_motor_data2.motor_pwm >= 80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data2.motor_pwm = 80;
    if (g_motor_data2.motor_pwm <= -80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data2.motor_pwm = -80;

    g_speed_pid2.SetPoint = g_motor_data2.motor_pwm;/* ����Ŀ���ٶȣ��⻷�����Ϊ�ڻ����� */
    g_motor_data2.motor_pwm = increment_pid_ctrl(&g_speed_pid2, g_motor_data2.speed);          /* �ٶȻ�PID���ƣ��ڻ��� */
#endif

    if (g_motor_data2.motor_pwm >= 80)           /* ����ռ�ձ� */
        g_motor_data2.motor_pwm = 80;
    if (g_motor_data2.motor_pwm <= -80)           /* ����ռ�ձ� */
        g_motor_data2.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN2, -g_motor_data2.motor_pwm);
}

void Motor3Control()
{
    //----------------------------------------------------------------------------------------
    g_motor_data3.location = (float)ADC_Value[2];/* ��ȡ��ǰ��λ��ֵ������λ�ñջ����� */

    //������������ֹ����
    M3Closer.current=g_location_pid3.SetPoint;
    M3Closer.target=USART1_Stru.Angle_Value[2];
    g_location_pid3.SetPoint=closerToTarget(&M3Closer);
    //��ͨ�˲����ӳٿ���
    g_motor_data3.location=lowPassFilter(&M3LowFilter,g_motor_data3.location,M3LowFilter.Filter_k);
    
    if(ADC_Value[2] - USART1_Stru.Angle_Value[2] <= 5 && ADC_Value[2] - USART1_Stru.Angle_Value[2] >= -5)
    {
        g_motor_data3.motor_pwm = 0;
        MotorSetPWMValue(MotorN3, g_motor_data3.motor_pwm);
        return;
    }
    else if(ADC_Value[2] < 1104 && ADC_Value[2] > 2904)
    {
        g_motor_data3.motor_pwm = 0;
        MotorSetPWMValue(MotorN3, g_motor_data3.motor_pwm);
        return;
    }
    else
    {
        g_motor_data3.motor_pwm = increment_pid_ctrl(&g_location_pid3, g_motor_data3.location);    /* λ�û�PID���ƣ��⻷�� */
    }
    
#if PID_MODE==1//ʹ�ô���
    if (g_motor_data3.motor_pwm >= 80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data3.motor_pwm = 80;
    if (g_motor_data3.motor_pwm <= -80)/* �����⻷�����Ŀ���ٶȣ� */
        g_motor_data3.motor_pwm = -80;

    g_speed_pid3.SetPoint = g_motor_data3.motor_pwm;/* ����Ŀ���ٶȣ��⻷�����Ϊ�ڻ����� */
    g_motor_data3.motor_pwm = increment_pid_ctrl(&g_speed_pid3, g_motor_data3.speed);          /* �ٶȻ�PID���ƣ��ڻ��� */
#endif

    if (g_motor_data3.motor_pwm >= 80)           /* ����ռ�ձ� */
        g_motor_data3.motor_pwm = 80;
    if (g_motor_data3.motor_pwm <= -80)           /* ����ռ�ձ� */
        g_motor_data3.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN3, -g_motor_data3.motor_pwm);
}





