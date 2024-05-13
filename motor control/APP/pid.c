#include "pid.h"
#include <math.h>



PID_TypeDef g_location_pid1;             /* λ�û�PID�����ṹ�� */
PID_TypeDef g_speed_pid1;                /* �ٶȻ�PID�����ṹ�� */

PID_TypeDef g_location_pid2;             /* λ�û�PID�����ṹ�� */
PID_TypeDef g_speed_pid2;                /* �ٶȻ�PID�����ṹ�� */

PID_TypeDef g_location_pid3;             /* λ�û�PID�����ṹ�� */
PID_TypeDef g_speed_pid3;                /* �ٶȻ�PID�����ṹ�� */

extern float PP;
extern float II;
extern float DD;

extern float sPP;
extern float sII;
extern float sDD;
//pid��ʼ��
void pid_init(void)
{
    /* ��ʼ��λ�û�PID���� */
    g_location_pid1.SetPoint = 0.0;       /* Ŀ��ֵ */
    g_location_pid1.ActualValue = 0.0;    /* �������ֵ */
    g_location_pid1.SumError = 0.0;       /* ����ֵ */
    g_location_pid1.Error = 0.0;          /* Error[1] */
    g_location_pid1.LastError = 0.0;      /* Error[-1] */
    g_location_pid1.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid1.Proportion = L_KP1;    /* �������� Proportional Const */
    //    g_location_pid1.Integral = L_KI1;      /* ���ֳ��� Integral Const */
    //    g_location_pid1.Derivative = L_KD1;    /* ΢�ֳ��� Derivative Const */

    g_location_pid1.Proportion = PP;    /* �������� Proportional Const */
    g_location_pid1.Integral = II;      /* ���ֳ��� Integral Const */
    g_location_pid1.Derivative = DD;    /* ΢�ֳ��� Derivative Const */

#if PID_MODE==1//ʹ�ô���
    /* ��ʼ���ٶȻ�PID���� */
    g_speed_pid1.SetPoint = 0.0;          /* Ŀ��ֵ */
    g_speed_pid1.ActualValue = 0.0;       /* �������ֵ */
    g_speed_pid1.SumError = 0.0;          /* ����ֵ */
    g_speed_pid1.Error = 0.0;             /* Error[1] */
    g_speed_pid1.LastError = 0.0;         /* Error[-1] */
    g_speed_pid1.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid1.Proportion = S_KP1;       /* �������� Proportional Const */
    //    g_speed_pid1.Integral = S_KI1;         /* ���ֳ��� Integral Const */
    //    g_speed_pid1.Derivative = S_KD1;       /* ΢�ֳ��� Derivative Const */

    g_speed_pid1.Proportion = sPP;       /* �������� Proportional Const */
    g_speed_pid1.Integral = sII;         /* ���ֳ��� Integral Const */
    g_speed_pid1.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
#endif

    ////////////////////////////////////////////////
    /* ��ʼ��λ�û�PID���� */
    g_location_pid2.SetPoint = 0.0;       /* Ŀ��ֵ */
    g_location_pid2.ActualValue = 0.0;    /* �������ֵ */
    g_location_pid2.SumError = 0.0;       /* ����ֵ */
    g_location_pid2.Error = 0.0;          /* Error[1] */
    g_location_pid2.LastError = 0.0;      /* Error[-1] */
    g_location_pid2.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid2.Proportion = L_KP2;    /* �������� Proportional Const */
    //    g_location_pid2.Integral = L_KI2;      /* ���ֳ��� Integral Const */
    //    g_location_pid2.Derivative = L_KD2;    /* ΢�ֳ��� Derivative Const */

    g_location_pid2.Proportion = PP;    /* �������� Proportional Const */
    g_location_pid2.Integral = II;      /* ���ֳ��� Integral Const */
    g_location_pid2.Derivative = DD;    /* ΢�ֳ��� Derivative Const */

#if PID_MODE==1//ʹ�ô���
    /* ��ʼ���ٶȻ�PID���� */
    g_speed_pid2.SetPoint = 0.0;          /* Ŀ��ֵ */
    g_speed_pid2.ActualValue = 0.0;       /* �������ֵ */
    g_speed_pid2.SumError = 0.0;          /* ����ֵ */
    g_speed_pid2.Error = 0.0;             /* Error[1] */
    g_speed_pid2.LastError = 0.0;         /* Error[-1] */
    g_speed_pid2.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid2.Proportion = S_KP2;       /* �������� Proportional Const */
    //    g_speed_pid2.Integral = S_KI2;         /* ���ֳ��� Integral Const */
    //    g_speed_pid2.Derivative = S_KD2;       /* ΢�ֳ��� Derivative Const */

    g_speed_pid2.Proportion = sPP;       /* �������� Proportional Const */
    g_speed_pid2.Integral = sII;         /* ���ֳ��� Integral Const */
    g_speed_pid2.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
#endif

    ////////////////////////////////////////////////
    /* ��ʼ��λ�û�PID���� */
    g_location_pid3.SetPoint = 0.0;       /* Ŀ��ֵ */
    g_location_pid3.ActualValue = 0.0;    /* �������ֵ */
    g_location_pid3.SumError = 0.0;       /* ����ֵ */
    g_location_pid3.Error = 0.0;          /* Error[1] */
    g_location_pid3.LastError = 0.0;      /* Error[-1] */
    g_location_pid3.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid3.Proportion = L_KP3;    /* �������� Proportional Const */
    //    g_location_pid3.Integral = L_KI3;      /* ���ֳ��� Integral Const */
    //    g_location_pid3.Derivative = L_KD3;    /* ΢�ֳ��� Derivative Const */

    g_location_pid3.Proportion = PP;    /* �������� Proportional Const */
    g_location_pid3.Integral = II;      /* ���ֳ��� Integral Const */
    g_location_pid3.Derivative = DD;    /* ΢�ֳ��� Derivative Const */

#if PID_MODE==1//ʹ�ô���
    /* ��ʼ���ٶȻ�PID���� */
    g_speed_pid3.SetPoint = 0.0;          /* Ŀ��ֵ */
    g_speed_pid3.ActualValue = 0.0;       /* �������ֵ */
    g_speed_pid3.SumError = 0.0;          /* ����ֵ */
    g_speed_pid3.Error = 0.0;             /* Error[1] */
    g_speed_pid3.LastError = 0.0;         /* Error[-1] */
    g_speed_pid3.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid3.Proportion = S_KP3;       /* �������� Proportional Const */
    //    g_speed_pid3.Integral = S_KI3;         /* ���ֳ��� Integral Const */
    //    g_speed_pid3.Derivative = S_KD3;       /* ΢�ֳ��� Derivative Const */

    g_speed_pid3.Proportion = sPP;       /* �������� Proportional Const */
    g_speed_pid3.Integral = sII;         /* ���ֳ��� Integral Const */
    g_speed_pid3.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
#endif
}



/**
 * @brief       pid�ջ�����
 * @param       *PID��PID�ṹ�������ַ
 * @param       Feedback_value����ǰʵ��ֵ
 * @retval      �������ֵ
 */

#define IntegralMax 3000
#define IntegralMin -3000

int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value)
{
    // ����ƫ��
    PID->Error = (float)(PID->SetPoint - Feedback_value);

    // ����������Ŀ��ֵ��Ӱ��
    float integral_effect = PID->SumError * PID->Integral;

    // ���ݻ������Ŀ��ֵ��Ӱ������޷���Χ
    float adjusted_integral_max = IntegralMax - integral_effect;
    float adjusted_integral_min = IntegralMin - integral_effect;

    // ����λ�û�����
    PID->SumError += PID->Error;

    // ���ƻ�����
    if (PID->SumError > adjusted_integral_max)
    {
        PID->SumError = adjusted_integral_max;
    }
    else if (PID->SumError < adjusted_integral_min)
    {
        PID->SumError = adjusted_integral_min;
    }

    float position_integral = PID->Integral * PID->SumError;

    // ����������ں�΢�ֻ���
    float proportional_term = PID->Proportion * PID->Error;
    float derivative_term = PID->Derivative * (PID->Error - PID->LastError);

    // ������������
    PID->ActualValue = proportional_term + position_integral + derivative_term;

    // ������һ�ε�ƫ��
    PID->LastError = PID->Error;

    // ���ؼ�����������ֵ
    return ((int32_t)(PID->ActualValue));
}







//int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value)
//{
//    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* ����ƫ�� */

//#if  INCR_LOCT_SELECT                                                       /* ����ʽPID */
//    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                          /* �������� */
//        + (PID->Integral * PID->Error)                                             /* ���ֻ��� */
//        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));  /* ΢�ֻ��� */

//    PID->PrevError = PID->LastError;                                        /* �洢ƫ������´μ��� */
//    PID->LastError = PID->Error;
//#else                                                                       /* λ��ʽPID */
//    PID->SumError += PID->Error;

//    PID->ActualValue = (PID->Proportion * PID->Error)                       /* �������� */
//        + (PID->Integral * PID->SumError)                    /* ���ֻ��� */
//        + (PID->Derivative * (PID->Error - PID->LastError)); /* ΢�ֻ��� */
//    PID->LastError = PID->Error;
//#endif
//    return ((int32_t)(PID->ActualValue));                                   /* ���ؼ�����������ֵ */
//}




