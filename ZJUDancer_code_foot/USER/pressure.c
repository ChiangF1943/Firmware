#include "pressure.h"

__IO uint16_t uhADC1ConvertedValue[4] = {0,0,0,0};
double uwADC1ConvertedVoltage[4] = {0,0,0,0};
double RealValue[4] = {0,0,0,0};
double buf[4][Max_buf];
double weight[4]={0.90,0.89,0.90,0.96};
double offset[4]={0.09,0.0,0.32,0.00};
int i,j=0,k;
volatile unsigned char delay_flag = 0;

int Pressure_Read(void)
{
	double sum_weight;
	ADC_Config();
	ADC_SoftwareStartConv(ADC1);
	
		for(i=0;i<4;i++)
		{
			uwADC1ConvertedVoltage[i] =(double )(uhADC1ConvertedValue[i] )*3300/0xFFF/1000;
			RealValue[i]=(buf[i][Max_buf-1]/(Max_buf-1)-offset[i])/weight[i];
			buf[i][j]=uwADC1ConvertedVoltage[i];
			buf[i][Max_buf-1]=0;
			for(k=0;k<(Max_buf-1);k++)
			{
				buf[i][Max_buf-1]+=buf[i][k];
			}
			//if(i==2)		
			printf("ADC[%d] = %lf KG     ",i,RealValue[i]);
		Delayms(100);
		}
		j++;
		if(j==Max_buf)
			j=0;
		sum_weight=0;
		for(i=0;i<4;i++)
		{
			sum_weight+=RealValue[i];
		}
		printf("sum = %lf KG     ",sum_weight);
		printf("\n");
	
}

static void ADC_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADCx, DMA and GPIO clocks ****************************************/ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	

  /* DMA2 Stream0 channel2 configuration *****************************
	*********/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADC1ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//LED
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);

  /* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 4;
	ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration **************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_144Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC3 DMA */
	ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC3 */
	ADC_Cmd(ADC1, ENABLE);

}