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

    // 计算下一次需要减少的差值
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

// 一阶低通滤波器函数
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
    g_motor_data1.location = (float)ADC_Value[0];/* 获取当前电位计值，用于位置闭环控制 */

    //递增靠近，防止超调
    M1Closer.current=g_location_pid1.SetPoint;
    M1Closer.target=USART1_Stru.Angle_Value[0];
    g_location_pid1.SetPoint=closerToTarget(&M1Closer);
    //低通滤波，延迟控制
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
        g_motor_data1.motor_pwm = increment_pid_ctrl(&g_location_pid1, g_motor_data1.location);    /* 位置环PID控制（外环） */
    }
    
#if PID_MODE==1//使用串级
    if (g_motor_data1.motor_pwm >= 80)/* 限制外环输出（目标速度） */
        g_motor_data1.motor_pwm = 80;
    if (g_motor_data1.motor_pwm <= -80)/* 限制外环输出（目标速度） */
        g_motor_data1.motor_pwm = -80;

    g_speed_pid1.SetPoint = g_motor_data1.motor_pwm;/* 设置目标速度，外环输出作为内环输入 */
    g_motor_data1.motor_pwm = increment_pid_ctrl(&g_speed_pid1, g_motor_data1.speed);          /* 速度环PID控制（内环） */
#endif

    if (g_motor_data1.motor_pwm >= 80)           /* 限制占空比 */
        g_motor_data1.motor_pwm = 80;
    if (g_motor_data1.motor_pwm <= -80)           /* 限制占空比 */
        g_motor_data1.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN1, -g_motor_data1.motor_pwm);
}

void Motor2Control()
{
    //----------------------------------------------------------------------------------------
    g_motor_data2.location = (float)ADC_Value[1];/* 获取当前电位计值，用于位置闭环控制 */

    //递增靠近，防止超调
    M2Closer.current=g_location_pid2.SetPoint;
    M2Closer.target=USART1_Stru.Angle_Value[1];
    g_location_pid2.SetPoint=closerToTarget(&M2Closer);
    //低通滤波，延迟控制
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
        g_motor_data2.motor_pwm = increment_pid_ctrl(&g_location_pid2, g_motor_data2.location);    /* 位置环PID控制（外环） */
    }
    
#if PID_MODE==1//使用串级
    if (g_motor_data2.motor_pwm >= 80)/* 限制外环输出（目标速度） */
        g_motor_data2.motor_pwm = 80;
    if (g_motor_data2.motor_pwm <= -80)/* 限制外环输出（目标速度） */
        g_motor_data2.motor_pwm = -80;

    g_speed_pid2.SetPoint = g_motor_data2.motor_pwm;/* 设置目标速度，外环输出作为内环输入 */
    g_motor_data2.motor_pwm = increment_pid_ctrl(&g_speed_pid2, g_motor_data2.speed);          /* 速度环PID控制（内环） */
#endif

    if (g_motor_data2.motor_pwm >= 80)           /* 限制占空比 */
        g_motor_data2.motor_pwm = 80;
    if (g_motor_data2.motor_pwm <= -80)           /* 限制占空比 */
        g_motor_data2.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN2, -g_motor_data2.motor_pwm);
}

void Motor3Control()
{
    //----------------------------------------------------------------------------------------
    g_motor_data3.location = (float)ADC_Value[2];/* 获取当前电位计值，用于位置闭环控制 */

    //递增靠近，防止超调
    M3Closer.current=g_location_pid3.SetPoint;
    M3Closer.target=USART1_Stru.Angle_Value[2];
    g_location_pid3.SetPoint=closerToTarget(&M3Closer);
    //低通滤波，延迟控制
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
        g_motor_data3.motor_pwm = increment_pid_ctrl(&g_location_pid3, g_motor_data3.location);    /* 位置环PID控制（外环） */
    }
    
#if PID_MODE==1//使用串级
    if (g_motor_data3.motor_pwm >= 80)/* 限制外环输出（目标速度） */
        g_motor_data3.motor_pwm = 80;
    if (g_motor_data3.motor_pwm <= -80)/* 限制外环输出（目标速度） */
        g_motor_data3.motor_pwm = -80;

    g_speed_pid3.SetPoint = g_motor_data3.motor_pwm;/* 设置目标速度，外环输出作为内环输入 */
    g_motor_data3.motor_pwm = increment_pid_ctrl(&g_speed_pid3, g_motor_data3.speed);          /* 速度环PID控制（内环） */
#endif

    if (g_motor_data3.motor_pwm >= 80)           /* 限制占空比 */
        g_motor_data3.motor_pwm = 80;
    if (g_motor_data3.motor_pwm <= -80)           /* 限制占空比 */
        g_motor_data3.motor_pwm = -80;
    
    MotorSetPWMValue(MotorN3, -g_motor_data3.motor_pwm);
}





