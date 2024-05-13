#include "motorSpeedInput.h"

MotorSpeedStructure MotorSpeedStru1;

void TIM1Input_Capture_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM1_ICInitStructure = { 0 };

    //����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  			//PA8 ���֮ǰ����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA8 ����  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_0);						//PA8 ����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  			//PA9 ���֮ǰ����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA9 ����  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  			//PA10 ���֮ǰ����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//PA10 ����  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //��ʼ����ʱ��1 TIM1
    TIM_TimeBaseStructure.TIM_Period = arr; 							//�趨�������Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 							//Ԥ��Ƶ��   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 					//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM1���벶�����
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//CC1S=01CC1ͨ��������Ϊ���룬IC1ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//ӳ�䵽TI1��,��ӳ�䵽TI2��ΪTIM_ICSelection_IndirectTI
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//���������Ƶ,����Ƶ��ÿ�����ض�����һ�β��� 
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;							//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_2;          			//CC1S=01	ѡ������� IC2ӳ�䵽TI2��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//ӳ�䵽TI2��
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//���������Ƶ,����Ƶ 
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;							//IC2F=0000 ���������˲��������˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  			//TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	//��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  							//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3, ENABLE);//��������У���������������жϣ�,����CC1IE�����ж�	

    TIM_Cmd(TIM1, ENABLE); 										//ʹ�ܶ�ʱ��1
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
















