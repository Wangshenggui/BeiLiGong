#include "adc.h"
#include <stdio.h>



//У׼ֵ
static int16_t ADC_CalibrationValue;
//DMA�洢
u16 ADCBuf[900];
//ADC����
u16 ADC_Value[3];

//ADC�ٶ�
int16_t ADC_Speed[3];

void ADC_DMA_CH1_3_Init(u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = { 0 };
    ADC_InitTypeDef ADC_InitStructure = { 0 };
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    //�������ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //ʱ�ӷ�Ƶ
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    //����DMA
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr; //�ڴ�洢����ַ�������һ������
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMAת��ģʽΪSRCģʽ����������Ƶ��ڴ�
    DMA_InitStructure.DMA_BufferSize = bufsize; // DMA�����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����һ�����ݺ��豸��ַ��ֹ���ƣ�����DMA���������ģʽ��
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�رս���һ�����ݺ�Ŀ���ڴ��ַ���ƣ�����DMA���ڴ����ģʽ��
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����������ݳ���
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    //��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //����ADC
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�޴���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 3;//����ͨ��
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));

    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 3, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    ADC_CalibrationValue = Get_CalibrationValue(ADC1);
}

//��ȡADCֵ
void Get_ADC_ConversionVal()
{
    uint64_t temp1 = 0, temp2 = 0, temp3 = 0;
    u16 v1_n = 0, v2_n = 0, v3_n = 0;

    for (u16 i = 0; i < 900; i += 3)
    {
        if ((ADC_CalibrationValue + ADCBuf[i]) > 4095 || ADCBuf[i] == 4095)
        {
            temp1 += 4095;
            v1_n++;
        }
        else
        {
            temp1 += ADCBuf[i] + ADC_CalibrationValue;
            v1_n++;
        }

        if ((ADC_CalibrationValue + ADCBuf[i + 1]) > 4095 || ADCBuf[i + 1] == 4095)
        {
            temp2 += 4095;
            v2_n++;
        }
        else
        {
            temp2 += ADCBuf[i + 1] + ADC_CalibrationValue;
            v2_n++;
        }

        if ((ADC_CalibrationValue + ADCBuf[i + 2]) > 4095 || ADCBuf[i + 2] == 4095)
        {
            temp3 += 4095;
            v3_n++;
        }
        else
        {
            temp3 += ADCBuf[i + 2] + ADC_CalibrationValue;
            v3_n++;
        }
    }
    ADC_Value[0] = temp1 / v1_n;
    ADC_Value[1] = temp2 / v2_n;
    ADC_Value[2] = temp3 / v3_n;
//    printf("%d,%d,%d\r\n",ADC_Value[0], ADC_Value[1],ADC_Value[2]);
}

void Get_ADC_Speed()
{
    static int16_t temp1 = 0;
    float speed1;

    static int16_t temp2 = 0;
    float speed2;

    static int16_t temp3 = 0;
    float speed3;

    Get_ADC_ConversionVal();//��ȡADCֵ
    speed1 = temp1 - ADC_Value[0];
    temp1 = ADC_Value[0];
    ADC_Speed[0] = speed1;

    speed2 = temp2 - ADC_Value[1];
    temp2 = ADC_Value[1];
    ADC_Speed[1] = speed2;

    speed3 = temp3 - ADC_Value[2];
    temp3 = ADC_Value[2];
    ADC_Speed[2] = speed3;
}






