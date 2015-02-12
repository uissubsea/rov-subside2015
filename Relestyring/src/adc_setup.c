/*
 * adc_setup.c
 *
 *  Created on: Feb 12, 2015
 *      Author: arch
 */

#include <stm32f4xx_conf.h>
#include <stm32f4xx.h>

/* Private macro -------------------------------------------------------------*/
 #define ADCx                     ADC3
  #define ADC_CHANNEL              ADC_Channel_7
  #define ADCx_CLK                 RCC_APB2Periph_ADC3
  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
  #define GPIO_PIN                 GPIO_Pin_9
  #define GPIO_PORT                GPIOF
  #define DMA_CHANNELx             DMA_Channel_2
  #define DMA_STREAMx              DMA2_Stream0
  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)

/* Private variables ---------------------------------------------------------*/
__IO uint16_t uhADCxConvertedValue = 0;
__IO uint32_t uwADCxConvertedVoltage = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void ADC_Config()
{
	ADC_InitTypeDef ADCInitStruct;
	GPIO_InitTypeDef GPIOInitStruct;

	/* Start Peripheral clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN, ENABLE);

	GPIOInitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIOInitStruct);

	ADC_DeInit();
	ADCInitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADCInitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADCInitStruct.ADC_ContinuousConvMode = ENABLE;
	ADCInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADCInitStruct.ADC_NbrOfConversion = 1;
	ADCInitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADCInitStruct);

	ADC_Cmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_144Cycles);


}

int adc_convert(){
		ADC_SoftwareStartConv(ADC1);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		return ADC_GetConversionValue(ADC1);
}


