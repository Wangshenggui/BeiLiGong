#include "system.h"

/*
中断优先级分配：
USART1   ：：3-3
TIM4     ：：3-2
TIM1Input：：3-0
*/

static u8  p_us = 0;
static u16 p_ms = 0;

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	p_us = SystemCoreClock / 8000000;
	p_ms = (u16)p_us * 1000;
}

void delay_us(u32 n)
{
	u32 i;

	SysTick->LOAD = n * p_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	do
	{
		i = SysTick->CTRL;
	} while ((i & 0x01) && !(i & (1 << 16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}


void delay_ms(u16 n)
{
	u32 i;

	SysTick->LOAD = (u32)n * p_ms;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	do
	{
		i = SysTick->CTRL;
	} while ((i & 0x01) && !(i & (1 << 16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}
