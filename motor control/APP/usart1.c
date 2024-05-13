#include "usart1.h"
#include <stdio.h>
#include "pid.h"


USART1_Structure USART1_Stru = { 0 };

/* Support Printf Function Definition */
struct __FILE
{
    int handle;
};
FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{
    x = x;
}

int fputc(int data, FILE* f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART1->DATAR = (data & (uint16_t)0x01FF);
    return data;
}

//�ϴ�����
void Up_Angle(uint16_t a1,uint16_t a2,uint16_t a3)
{
    uint8_t TxCMD[100];
    
    union
    {
        float v;
        unsigned char dat[4];
    }data;

    TxCMD[0] = 0xCC;
    TxCMD[1] = 0xAA;//�Ƕȹ���

    data.v = a1;
    TxCMD[2] = data.dat[0];
    TxCMD[3] = data.dat[1];
    TxCMD[4] = data.dat[2];
    TxCMD[5] = data.dat[3];

    data.v = a2;
    TxCMD[6] = data.dat[0];
    TxCMD[7] = data.dat[1];
    TxCMD[8] = data.dat[2];
    TxCMD[9] = data.dat[3];

    data.v = a3;
    TxCMD[10] = data.dat[0];
    TxCMD[11] = data.dat[1];
    TxCMD[12] = data.dat[2];
    TxCMD[13] = data.dat[3];

    TxCMD[14] = (unsigned char)(TxCMD[2] + TxCMD[3] + TxCMD[4] + TxCMD[5]\
        + TxCMD[6] + TxCMD[7] + TxCMD[8] + TxCMD[9]\
        + TxCMD[10] + TxCMD[11] + TxCMD[12] + TxCMD[13]);
    TxCMD[15] = 0xEB;
    TxCMD[16] = 0x90;
    for (uint8_t i = 0; i < 17; i++)
    {
#ifndef SIMULATOR
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART1->DATAR = (TxCMD[i] & (uint16_t)0x01FF);
#endif
    }
}

void USART1_Init(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);
}




float PP=0.0;
float II=0.0;
float DD=0.0;


float sPP=0.0;
float sII=0.0;
float sDD=0.0;


//����֡��ʽ��\
ͷ  ������        ����1                   ����2                   ����3         У���   β\
 0   1     2     3     4     5     6     7     8     9    10    11    12    13   14   15  16\
CC  Fun  D1_1  D1_2  D1_3  D1_4  D2_1  D2_2  D2_3  D2_4  D3_1  D3_2  D3_3  D3_4  SUM  EB  90\
������ռ��--->>>0xD0-0xDF\
0xD0::����Ƕ�\
0xD1::PID����\
0xD2::NULL\
���ݴ洢��ʽΪС��ģʽfloat����
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //��ȡ���յ�������
        USART1_Stru.RxBuff[USART1_Stru.RxLen] = (uint16_t)(USART1->DATAR & (uint16_t)0x01FF);
        //֡β
        if (USART1_Stru.RxBuff[USART1_Stru.RxLen] == 0x90\
            && USART1_Stru.RxBuff[USART1_Stru.RxLen - 1] == 0xeb)
        {
            //֡ͷ
            if (USART1_Stru.RxBuff[0] == 0xAC)
            {
                //���Ƶ�������
                for (u8 i = 0; i < USART1_Stru.RxLen + 1; i++)
                {
                    USART1_Stru.Buff[i] = USART1_Stru.RxBuff[i];
                    USART1_Stru.RxBuff[i] = '\0';
                }
                
                union
                {
                    float PID;
                    char bit[4];
                }P;
                u8 i;
                for(i=0;i<4;i++)
                {
                    P.bit[i]=USART1_Stru.Buff[i+1];
                }
                PP=P.PID;
                
                union
                {
                    float PID;
                    char bit[4];
                }I;
                for(i=0;i<4;i++)
                {
                    I.bit[i]=USART1_Stru.Buff[i+1+4];
                }
                II=I.PID;
                
                union
                {
                    float PID;
                    char bit[4];
                }D;
                for(i=0;i<4;i++)
                {
                    D.bit[i]=USART1_Stru.Buff[i+1+4+4];
                }
                DD=D.PID;
                
                g_location_pid1.Proportion = PP;    /* �������� Proportional Const */
                g_location_pid1.Integral = II;      /* ���ֳ��� Integral Const */
                g_location_pid1.Derivative = DD;    /* ΢�ֳ��� Derivative Const */
                
                g_location_pid2.Proportion = PP;    /* �������� Proportional Const */
                g_location_pid2.Integral = II;      /* ���ֳ��� Integral Const */
                g_location_pid2.Derivative = DD;    /* ΢�ֳ��� Derivative Const */
                
                g_location_pid3.Proportion = PP;    /* �������� Proportional Const */
                g_location_pid3.Integral = II;      /* ���ֳ��� Integral Const */
                g_location_pid3.Derivative = DD;    /* ΢�ֳ��� Derivative Const */
            }
            else if (USART1_Stru.RxBuff[0] == 0xCC)
            {
                //���Ƶ�������
                for (u8 i = 0; i < USART1_Stru.RxLen + 1; i++)
                {
                    USART1_Stru.Buff[i] = USART1_Stru.RxBuff[i];
                    USART1_Stru.RxBuff[i] = '\0';
                }
                
                union
                {
                    float Target;
                    char bit[4];
                }Tar;
                u8 i;
                for(i=0;i<4;i++)
                {
                    Tar.bit[i]=USART1_Stru.Buff[i+1];
                }
                
                USART1_Stru.Angle_Value[0]=(uint16_t)Tar.Target;
                USART1_Stru.Angle_Value[1]=(uint16_t)Tar.Target;
                USART1_Stru.Angle_Value[2]=(uint16_t)Tar.Target;
            }
            else if (USART1_Stru.RxBuff[0] == 0xAD)
            {
                //���Ƶ�������
                for (u8 i = 0; i < USART1_Stru.RxLen + 1; i++)
                {
                    USART1_Stru.Buff[i] = USART1_Stru.RxBuff[i];
                    USART1_Stru.RxBuff[i] = '\0';
                }
                
                union
                {
                    float PID;
                    char bit[4];
                }P;
                u8 i;
                for(i=0;i<4;i++)
                {
                    P.bit[i]=USART1_Stru.Buff[i+1];
                }
                sPP=P.PID;
                
                union
                {
                    float PID;
                    char bit[4];
                }I;
                for(i=0;i<4;i++)
                {
                    I.bit[i]=USART1_Stru.Buff[i+1+4];
                }
                sII=I.PID;
                
                union
                {
                    float PID;
                    char bit[4];
                }D;
                for(i=0;i<4;i++)
                {
                    D.bit[i]=USART1_Stru.Buff[i+1+4+4];
                }
                sDD=D.PID;
                
                g_speed_pid1.Proportion = sPP;       /* �������� Proportional Const */
                g_speed_pid1.Integral = sII;         /* ���ֳ��� Integral Const */
                g_speed_pid1.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
                
                g_speed_pid2.Proportion = sPP;       /* �������� Proportional Const */
                g_speed_pid2.Integral = sII;         /* ���ֳ��� Integral Const */
                g_speed_pid2.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
                
                g_speed_pid3.Proportion = sPP;       /* �������� Proportional Const */
                g_speed_pid3.Integral = sII;         /* ���ֳ��� Integral Const */
                g_speed_pid3.Derivative = sDD;       /* ΢�ֳ��� Derivative Const */
            }
            USART1_Stru.RxLen = 0;
        }
        else
        {
            USART1_Stru.RxLen++;
        }
    }
}








