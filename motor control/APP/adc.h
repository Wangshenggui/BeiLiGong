#ifndef _ADC_H
#define _ADC_H

#include "main.h"


extern u16 ADCBuf[900];
extern u16 ADC_Value[3];
extern int16_t ADC_Speed[3];

void ADC_DMA_CH1_3_Init(u32 memadr, u16 bufsize);
void Get_ADC_ConversionVal(void);
void Get_ADC_Speed(void);






#endif
