#ifndef _USART1_H
#define _USART1_H

#include "main.h"
#include <stdbool.h>

#define USART1RxLEN 200

typedef struct
{
    u8 receive;
    u8 RxBuff[USART1RxLEN];
    u8 RxLen;
    u8 Buff[USART1RxLEN];
    bool ChangFlag;
    float Angle_Value[3];

    float PID[3];
}USART1_Structure;
extern USART1_Structure USART1_Stru;

void Up_Angle(uint16_t a1,uint16_t a2,uint16_t a3);
void USART1_Init(u32 baudrate);



#endif
