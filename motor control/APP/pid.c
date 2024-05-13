#include "pid.h"
#include <math.h>



PID_TypeDef g_location_pid1;             /* 位置环PID参数结构体 */
PID_TypeDef g_speed_pid1;                /* 速度环PID参数结构体 */

PID_TypeDef g_location_pid2;             /* 位置环PID参数结构体 */
PID_TypeDef g_speed_pid2;                /* 速度环PID参数结构体 */

PID_TypeDef g_location_pid3;             /* 位置环PID参数结构体 */
PID_TypeDef g_speed_pid3;                /* 速度环PID参数结构体 */

extern float PP;
extern float II;
extern float DD;

extern float sPP;
extern float sII;
extern float sDD;
//pid初始化
void pid_init(void)
{
    /* 初始化位置环PID参数 */
    g_location_pid1.SetPoint = 0.0;       /* 目标值 */
    g_location_pid1.ActualValue = 0.0;    /* 期望输出值 */
    g_location_pid1.SumError = 0.0;       /* 积分值 */
    g_location_pid1.Error = 0.0;          /* Error[1] */
    g_location_pid1.LastError = 0.0;      /* Error[-1] */
    g_location_pid1.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid1.Proportion = L_KP1;    /* 比例常数 Proportional Const */
    //    g_location_pid1.Integral = L_KI1;      /* 积分常数 Integral Const */
    //    g_location_pid1.Derivative = L_KD1;    /* 微分常数 Derivative Const */

    g_location_pid1.Proportion = PP;    /* 比例常数 Proportional Const */
    g_location_pid1.Integral = II;      /* 积分常数 Integral Const */
    g_location_pid1.Derivative = DD;    /* 微分常数 Derivative Const */

#if PID_MODE==1//使用串级
    /* 初始化速度环PID参数 */
    g_speed_pid1.SetPoint = 0.0;          /* 目标值 */
    g_speed_pid1.ActualValue = 0.0;       /* 期望输出值 */
    g_speed_pid1.SumError = 0.0;          /* 积分值 */
    g_speed_pid1.Error = 0.0;             /* Error[1] */
    g_speed_pid1.LastError = 0.0;         /* Error[-1] */
    g_speed_pid1.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid1.Proportion = S_KP1;       /* 比例常数 Proportional Const */
    //    g_speed_pid1.Integral = S_KI1;         /* 积分常数 Integral Const */
    //    g_speed_pid1.Derivative = S_KD1;       /* 微分常数 Derivative Const */

    g_speed_pid1.Proportion = sPP;       /* 比例常数 Proportional Const */
    g_speed_pid1.Integral = sII;         /* 积分常数 Integral Const */
    g_speed_pid1.Derivative = sDD;       /* 微分常数 Derivative Const */
#endif

    ////////////////////////////////////////////////
    /* 初始化位置环PID参数 */
    g_location_pid2.SetPoint = 0.0;       /* 目标值 */
    g_location_pid2.ActualValue = 0.0;    /* 期望输出值 */
    g_location_pid2.SumError = 0.0;       /* 积分值 */
    g_location_pid2.Error = 0.0;          /* Error[1] */
    g_location_pid2.LastError = 0.0;      /* Error[-1] */
    g_location_pid2.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid2.Proportion = L_KP2;    /* 比例常数 Proportional Const */
    //    g_location_pid2.Integral = L_KI2;      /* 积分常数 Integral Const */
    //    g_location_pid2.Derivative = L_KD2;    /* 微分常数 Derivative Const */

    g_location_pid2.Proportion = PP;    /* 比例常数 Proportional Const */
    g_location_pid2.Integral = II;      /* 积分常数 Integral Const */
    g_location_pid2.Derivative = DD;    /* 微分常数 Derivative Const */

#if PID_MODE==1//使用串级
    /* 初始化速度环PID参数 */
    g_speed_pid2.SetPoint = 0.0;          /* 目标值 */
    g_speed_pid2.ActualValue = 0.0;       /* 期望输出值 */
    g_speed_pid2.SumError = 0.0;          /* 积分值 */
    g_speed_pid2.Error = 0.0;             /* Error[1] */
    g_speed_pid2.LastError = 0.0;         /* Error[-1] */
    g_speed_pid2.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid2.Proportion = S_KP2;       /* 比例常数 Proportional Const */
    //    g_speed_pid2.Integral = S_KI2;         /* 积分常数 Integral Const */
    //    g_speed_pid2.Derivative = S_KD2;       /* 微分常数 Derivative Const */

    g_speed_pid2.Proportion = sPP;       /* 比例常数 Proportional Const */
    g_speed_pid2.Integral = sII;         /* 积分常数 Integral Const */
    g_speed_pid2.Derivative = sDD;       /* 微分常数 Derivative Const */
#endif

    ////////////////////////////////////////////////
    /* 初始化位置环PID参数 */
    g_location_pid3.SetPoint = 0.0;       /* 目标值 */
    g_location_pid3.ActualValue = 0.0;    /* 期望输出值 */
    g_location_pid3.SumError = 0.0;       /* 积分值 */
    g_location_pid3.Error = 0.0;          /* Error[1] */
    g_location_pid3.LastError = 0.0;      /* Error[-1] */
    g_location_pid3.PrevError = 0.0;      /* Error[-2] */
    //    g_location_pid3.Proportion = L_KP3;    /* 比例常数 Proportional Const */
    //    g_location_pid3.Integral = L_KI3;      /* 积分常数 Integral Const */
    //    g_location_pid3.Derivative = L_KD3;    /* 微分常数 Derivative Const */

    g_location_pid3.Proportion = PP;    /* 比例常数 Proportional Const */
    g_location_pid3.Integral = II;      /* 积分常数 Integral Const */
    g_location_pid3.Derivative = DD;    /* 微分常数 Derivative Const */

#if PID_MODE==1//使用串级
    /* 初始化速度环PID参数 */
    g_speed_pid3.SetPoint = 0.0;          /* 目标值 */
    g_speed_pid3.ActualValue = 0.0;       /* 期望输出值 */
    g_speed_pid3.SumError = 0.0;          /* 积分值 */
    g_speed_pid3.Error = 0.0;             /* Error[1] */
    g_speed_pid3.LastError = 0.0;         /* Error[-1] */
    g_speed_pid3.PrevError = 0.0;         /* Error[-2] */
    //    g_speed_pid3.Proportion = S_KP3;       /* 比例常数 Proportional Const */
    //    g_speed_pid3.Integral = S_KI3;         /* 积分常数 Integral Const */
    //    g_speed_pid3.Derivative = S_KD3;       /* 微分常数 Derivative Const */

    g_speed_pid3.Proportion = sPP;       /* 比例常数 Proportional Const */
    g_speed_pid3.Integral = sII;         /* 积分常数 Integral Const */
    g_speed_pid3.Derivative = sDD;       /* 微分常数 Derivative Const */
#endif
}



/**
 * @brief       pid闭环控制
 * @param       *PID：PID结构体变量地址
 * @param       Feedback_value：当前实际值
 * @retval      期望输出值
 */

#define IntegralMax 3000
#define IntegralMin -3000

int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value)
{
    // 计算偏差
    PID->Error = (float)(PID->SetPoint - Feedback_value);

    // 计算积分项对目标值的影响
    float integral_effect = PID->SumError * PID->Integral;

    // 根据积分项对目标值的影响调整限幅范围
    float adjusted_integral_max = IntegralMax - integral_effect;
    float adjusted_integral_min = IntegralMin - integral_effect;

    // 计算位置积分项
    PID->SumError += PID->Error;

    // 限制积分项
    if (PID->SumError > adjusted_integral_max)
    {
        PID->SumError = adjusted_integral_max;
    }
    else if (PID->SumError < adjusted_integral_min)
    {
        PID->SumError = adjusted_integral_min;
    }

    float position_integral = PID->Integral * PID->SumError;

    // 计算比例环节和微分环节
    float proportional_term = PID->Proportion * PID->Error;
    float derivative_term = PID->Derivative * (PID->Error - PID->LastError);

    // 计算控制器输出
    PID->ActualValue = proportional_term + position_integral + derivative_term;

    // 更新上一次的偏差
    PID->LastError = PID->Error;

    // 返回计算后输出的数值
    return ((int32_t)(PID->ActualValue));
}







//int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value)
//{
//    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* 计算偏差 */

//#if  INCR_LOCT_SELECT                                                       /* 增量式PID */
//    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                          /* 比例环节 */
//        + (PID->Integral * PID->Error)                                             /* 积分环节 */
//        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));  /* 微分环节 */

//    PID->PrevError = PID->LastError;                                        /* 存储偏差，用于下次计算 */
//    PID->LastError = PID->Error;
//#else                                                                       /* 位置式PID */
//    PID->SumError += PID->Error;

//    PID->ActualValue = (PID->Proportion * PID->Error)                       /* 比例环节 */
//        + (PID->Integral * PID->SumError)                    /* 积分环节 */
//        + (PID->Derivative * (PID->Error - PID->LastError)); /* 微分环节 */
//    PID->LastError = PID->Error;
//#endif
//    return ((int32_t)(PID->ActualValue));                                   /* 返回计算后输出的数值 */
//}




