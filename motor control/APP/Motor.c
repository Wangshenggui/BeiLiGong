#include "Motor.h"
#include "motor_pwm.h"
#include "motorSpeedInput.h"



void MotorInit(void)
{
    //正反转控制初始化
    MotorZF_GPIO_Init();
    //速度控制初始化
    TIM3_PWM_CH1_3_Init(100, 36 - 1);//20kHz
    //电机速度输入捕获
    /*               用不到
    TIM1Input_Capture_Init(0xFFFF, 72 - 1);//1us
    */

    //设置电机速度
    MotorSetPWMValue(MotorN1, 0);
    MotorSetPWMValue(MotorN2, 0);
    MotorSetPWMValue(MotorN3, 0);
}

//正反转初始化
void MotorZF_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //设置转向
    MotorZF_Control(MotorN1, CW);
    MotorZF_Control(MotorN2, CW);
    MotorZF_Control(MotorN3, CW);
}

void MotorZF_Control(u8 n, u8 ccw)
{
    if (n == 1)
    {
        PCout(15) = ccw;
    }
    else if (n == 2)
    {
        PCout(13) = ccw;
    }
    else if (n == 3)
    {
        PCout(14) = ccw;
    }
    else
        return;
}






