#ifndef __PID_H
#define __PID_H

#include "main.h"

#define PID_MODE    0               /* 0：单环，1：串级 */



/******************************************************************************************/
/* PID相关参数 */
#define  INCR_LOCT_SELECT  0         /* 0：位置式，1：增量式 */

#if INCR_LOCT_SELECT    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

///* 定义位置环PID参数相关宏 */
//#define  L_KP1      0.04f             /* P参数 */
//#define  L_KI1      0.0000f             /* I参数 */
//#define  L_KD1      0.000f             /* D参数 */

//#define  L_KP2      0.251f             /* P参数 */
//#define  L_KI2      0.000000f             /* I参数 */
//#define  L_KD2      0.000f             /* D参数 */

//#define  L_KP3      0.251f             /* P参数 */
//#define  L_KI3      0.000000f             /* I参数 */
//#define  L_KD3      0.000f             /* D参数 */


#if PID_MODE==1//使用串级
///* 定义速度PID参数相关宏 */
//#define  S_KP1      0.9f             /* P参数 */
//#define  S_KI1      0.0f            /* I参数 */
//#define  S_KD1      0.5f             /* D参数 */

//#define  S_KP2      0.9f             /* P参数 */
//#define  S_KI2      0.0f            /* I参数 */
//#define  S_KD2      0.5f             /* D参数 */

//#define  S_KP3      0.9f             /* P参数 */
//#define  S_KI3      0.0f            /* I参数 */
//#define  S_KD3      0.5f             /* D参数 */
#endif

#else                   //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

///* 定义位置环PID参数相关宏 */
//#define  L_KP1      0.251f             /* P参数 */
//#define  L_KI1      0.000000f             /* I参数 */
//#define  L_KD1      0.000f             /* D参数 */

//#define  L_KP2      0.251f             /* P参数 */
//#define  L_KI2      0.000000f             /* I参数 */
//#define  L_KD2      0.000f             /* D参数 */

//#define  L_KP3      0.251f             /* P参数 */
//#define  L_KI3      0.000000f             /* I参数 */
//#define  L_KD3      0.000f             /* D参数 */


#if PID_MODE==1//使用串级
///* 定义速度PID参数相关宏 */
//#define  S_KP1      0.9f             /* P参数 */
//#define  S_KI1      0.0f            /* I参数 */
//#define  S_KD1      0.5f             /* D参数 */

//#define  S_KP2      0.9f             /* P参数 */
//#define  S_KI2      0.0f            /* I参数 */
//#define  S_KD2      0.5f             /* D参数 */

//#define  S_KP3      0.9f             /* P参数 */
//#define  S_KI3      0.0f            /* I参数 */
//#define  S_KD3      0.5f             /* D参数 */
#endif

#endif

/* PID结构体 */
typedef struct
{
    __IO float  SetPoint;            /* 目标值 */
    __IO float  ActualValue;         /* 期望输出值 */
    __IO float  SumError;            /* 误差累计 */
    __IO float  Proportion;          /* 比例常数 P */
    __IO float  Integral;            /* 积分常数 I */
    __IO float  Derivative;          /* 微分常数 D */
    __IO float  Error;               /* Error[1] */
    __IO float  LastError;           /* Error[-1] */
    __IO float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_location_pid1;  /* 位置环PID参数结构体 */
extern PID_TypeDef  g_speed_pid1;     /* 速度环PID参数结构体 */

extern PID_TypeDef  g_location_pid2;  /* 位置环PID参数结构体 */
extern PID_TypeDef  g_speed_pid2;     /* 速度环PID参数结构体 */

extern PID_TypeDef  g_location_pid3;  /* 位置环PID参数结构体 */
extern PID_TypeDef  g_speed_pid3;     /* 速度环PID参数结构体 */

/******************************************************************************************/

void pid_init(void);                 /* pid初始化 */
int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value);      /* pid闭环控制 */

#endif
