#include "motorSpeedInput.h"

MotorSpeedStructure MotorSpeedStru1;

void TIM1Input_Capture_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM1_ICInitStructure = { 0 };

    //开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  			//PA8 清除之前设置  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA8 输入  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_0);						//PA8 下拉

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  			//PA9 清除之前设置  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA9 输入  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  			//PA10 清除之前设置  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA10 输入  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化定时器1 TIM1
    TIM_TimeBaseStructure.TIM_Period = arr; 							//设定计数器自动重装值 
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 							//预分频器   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM1输入捕获参数
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//CC1S=01CC1通道被配置为输入，IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI1上,若映射到TI2上为TIM_ICSelection_IndirectTI
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//配置输入分频,不分频，每个边沿都触发一次捕获 
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;							//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_2;          			//CC1S=01	选择输入端 IC2映射到TI2上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI2上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//配置输入分频,不分频 
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;							//IC2F=0000 配置输入滤波器，不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  			//TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	//先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3, ENABLE);//允许更新中（计数器溢出计数中断）,允许CC1IE捕获中断	

    TIM_Cmd(TIM1, ENABLE); 										//使能定时器1
}


void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        if (PCout(13) == 1)
        {
            MotorSpeedStru1.Location++;
        }
        else
        {
            MotorSpeedStru1.Location--;
        }
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {
        TIM1->CH2CVR;
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
    {
        TIM1->CH3CVR;
    }
    TIM1->CNT = 0;
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3);
}

int16_t GetMotorSpeed()
{
    MotorSpeedStru1.Speed = MotorSpeedStru1.Location - MotorSpeedStru1.LastLocation;
    MotorSpeedStru1.LastLocation = MotorSpeedStru1.Location;

    return MotorSpeedStru1.Speed;
}
















