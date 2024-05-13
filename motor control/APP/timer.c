#include "timer.h"
#include "pid.h"
#include "motorSpeedInput.h"
#include "motor_pwm.h"
#include "adc.h"
#include "usart1.h"
#include "motorControl.h"


Motor_TypeDef g_motor_data1;  /*电机参数变量*/
Motor_TypeDef g_motor_data2;  /*电机参数变量*/
Motor_TypeDef g_motor_data3;  /*电机参数变量*/

void TIM4_InterruptInit(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;							//中断优先级NVIC设置

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 			//时钟使能

    //定时器TIM4初始化
    TIM_TimeBaseStructure.TIM_Period = arr;		 				//自动重装载寄存器周期的值1/CK_CNT=1us,1000x1us=1ms	
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 						//设置用来作为TIMx时钟频率除数的预分频值CK_CNT=CK_INT/(71+1)=1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//TIM_CKD_DIV1是.h文件中已经定义好的，TIM_CKD_DIV1=0，也就是时钟分频因子为0
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);						 //使能指定的TIM4中断,允许更新中断

    TIM_Cmd(TIM4, ENABLE);  										//使能TIM

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  				//TIM4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  		//先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			//从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  								//初始化NVIC寄存器
}


void TIM4_IRQHandler(void)   										//TIM3中断
{
    static u16 i = 0;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  			//检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 				//清除TIMx更新中断标志，立即重启定时器
        i++;
        if (i == 10)
        {
            i = 0;
            //获取ADC信息
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





