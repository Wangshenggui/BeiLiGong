#ifndef __PID_H
#define __PID_H

#include "main.h"

#define PID_MODE    0               /* 0��������1������ */



/******************************************************************************************/
/* PID��ز��� */
#define  INCR_LOCT_SELECT  0         /* 0��λ��ʽ��1������ʽ */

#if INCR_LOCT_SELECT    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

///* ����λ�û�PID������غ� */
//#define  L_KP1      0.04f             /* P���� */
//#define  L_KI1      0.0000f             /* I���� */
//#define  L_KD1      0.000f             /* D���� */

//#define  L_KP2      0.251f             /* P���� */
//#define  L_KI2      0.000000f             /* I���� */
//#define  L_KD2      0.000f             /* D���� */

//#define  L_KP3      0.251f             /* P���� */
//#define  L_KI3      0.000000f             /* I���� */
//#define  L_KD3      0.000f             /* D���� */


#if PID_MODE==1//ʹ�ô���
///* �����ٶ�PID������غ� */
//#define  S_KP1      0.9f             /* P���� */
//#define  S_KI1      0.0f            /* I���� */
//#define  S_KD1      0.5f             /* D���� */

//#define  S_KP2      0.9f             /* P���� */
//#define  S_KI2      0.0f            /* I���� */
//#define  S_KD2      0.5f             /* D���� */

//#define  S_KP3      0.9f             /* P���� */
//#define  S_KI3      0.0f            /* I���� */
//#define  S_KD3      0.5f             /* D���� */
#endif

#else                   //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

///* ����λ�û�PID������غ� */
//#define  L_KP1      0.251f             /* P���� */
//#define  L_KI1      0.000000f             /* I���� */
//#define  L_KD1      0.000f             /* D���� */

//#define  L_KP2      0.251f             /* P���� */
//#define  L_KI2      0.000000f             /* I���� */
//#define  L_KD2      0.000f             /* D���� */

//#define  L_KP3      0.251f             /* P���� */
//#define  L_KI3      0.000000f             /* I���� */
//#define  L_KD3      0.000f             /* D���� */


#if PID_MODE==1//ʹ�ô���
///* �����ٶ�PID������غ� */
//#define  S_KP1      0.9f             /* P���� */
//#define  S_KI1      0.0f            /* I���� */
//#define  S_KD1      0.5f             /* D���� */

//#define  S_KP2      0.9f             /* P���� */
//#define  S_KI2      0.0f            /* I���� */
//#define  S_KD2      0.5f             /* D���� */

//#define  S_KP3      0.9f             /* P���� */
//#define  S_KI3      0.0f            /* I���� */
//#define  S_KD3      0.5f             /* D���� */
#endif

#endif

/* PID�ṹ�� */
typedef struct
{
    __IO float  SetPoint;            /* Ŀ��ֵ */
    __IO float  ActualValue;         /* �������ֵ */
    __IO float  SumError;            /* ����ۼ� */
    __IO float  Proportion;          /* �������� P */
    __IO float  Integral;            /* ���ֳ��� I */
    __IO float  Derivative;          /* ΢�ֳ��� D */
    __IO float  Error;               /* Error[1] */
    __IO float  LastError;           /* Error[-1] */
    __IO float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_location_pid1;  /* λ�û�PID�����ṹ�� */
extern PID_TypeDef  g_speed_pid1;     /* �ٶȻ�PID�����ṹ�� */

extern PID_TypeDef  g_location_pid2;  /* λ�û�PID�����ṹ�� */
extern PID_TypeDef  g_speed_pid2;     /* �ٶȻ�PID�����ṹ�� */

extern PID_TypeDef  g_location_pid3;  /* λ�û�PID�����ṹ�� */
extern PID_TypeDef  g_speed_pid3;     /* �ٶȻ�PID�����ṹ�� */

/******************************************************************************************/

void pid_init(void);                 /* pid��ʼ�� */
int32_t increment_pid_ctrl(PID_TypeDef* PID, float Feedback_value);      /* pid�ջ����� */

#endif
