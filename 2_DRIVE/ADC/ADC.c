#include "ADC.h"
#include "delay.h"

void ADC_Initialize(void)
{
	ADC_GPIO_Init();
	ADC_Channel_Initialize();
}
/**
  * @brief ADCͨ����ʼ��
	* @param none
  * @retval none
  * @note  ����ת������������ADCת��
  */ 
void ADC_Channel_Initialize(void)
{

	ADC_InitTypeDef ADC_InitStructure;

	
	/* ����ADCCLK��Ƶ���� ADCCLK = PCLK2/6���� 72MHz/6 = 12MHz */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_DeInit(ADC1);  //��ADC1��Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��������ADCת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//����ת��ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ʼ��ADC
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);	//��λADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼���� 
}

/**
  * @brief ADC_GPIO�ڳ�ʼ��
	* @param none
  * @retval none
  * @note  ����ת������������ADCת��
  */ 
void ADC_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��GPIOA��ADC1,AFIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

	  /* ���� PA.00 (ADC1_IN0) ��Ϊģ���������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}
/**
  * @brief ���ADCֵ
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:ͨ��ֵ ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADCת��ֵ
  * @note 
  */
uint16_t Get_Adc(ADC_TypeDef* ADCx, uint8_t ch)   
{
	/* ADC�Ĺ���ͨ�������ã�һ�����У�����ʱ��239.5����	*/
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//��ʼת��
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));	//�ȴ�ת������
	return ADC_GetConversionValue(ADCx);						//����ADC1ת�����
}

/**
  * @brief ��ȡ����10��ADC������ƽ��ֵ
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:ͨ��ֵ ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADCת����ƽ��ֵ
  * @note 
  */
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx, uint8_t ch)
{
	uint32_t adc_val = 0;
	uint8_t i;
	
	for(i = 0; i < 10; i++)
	{
		adc_val += Get_Adc(ADCx, ch);
		delay_ms(5);
	}
	return adc_val/10;
} 	 

float Get_Voltage(u16 ADCvalue)
{
 return 3.3 * ADCvalue / 4096 ;
}