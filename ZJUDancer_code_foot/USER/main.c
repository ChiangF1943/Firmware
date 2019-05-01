#include <stm32f4xx.h>
#include "stm32f4xx_conf.h"
#include "main.h"
#include <stdio.h>
#include "commu.h"


  #define ADC1_DR_ADDRESS          ((uint32_t)0x4001204C)
	#define Max_buf 2
	//at least 2




__IO uint16_t uhADC1ConvertedValue[4] = {0,0,0,0};
double uwADC1ConvertedVoltage[4] = {0,0,0,0};
double RealValue[4] = {0,0,0,0};
double buf[4][Max_buf];
double weight[4]={0.90,0.89,0.90,0.96};
double offset[4]={0.09,0.0,0.32,0.00};
int i,j=0,k;
volatile unsigned char delay_flag = 0;


//static __IO uint32_t TimingDelay;

static void ADC_Config(void);
static void USART_Config(void);
void TIM2_Init(void);
void Delayms(unsigned int ms);
void Comu_485_init(void);
void TIM9_Int_Init(u16 arr,u16 psc);

int main(void)
{
	double sum_weight;
	//SystemInit();
	ADC_Config();
	//USART_Config();
	USART1_Init(115200);
	Comu_485_init();
	ADC_SoftwareStartConv(ADC1);
	TIM2_Init();
	//TIM9_Int_Init(20000-1,84-1);
	
	while (1)
	{
		for(i=0;i<4;i++)
		{
			uwADC1ConvertedVoltage[i] =(double)(uhADC1ConvertedValue[i])*3300/0xFFF/1000;
			RealValue[i]=(buf[i][Max_buf-1]/(Max_buf-1)-offset[i])/weight[i];
			buf[i][j]=uwADC1ConvertedVoltage[i];
			buf[i][Max_buf-1]=0;
			for(k=0;k<(Max_buf-1);k++)
			{
				buf[i][Max_buf-1]+=buf[i][k];
			}
			//if(i==2)		
		//	printf("ADC[%d] = %lf KG     ",i,RealValue[i]);
		Delayms(10);
		}
		j++;
		if(j==Max_buf)
			j=0;
		sum_weight=0;
		for(i=0;i<4;i++)
		{
			sum_weight+=RealValue[i];
		}


if(com_flag==COMMU_GETDATA)
		{
			direction_485_ctrl(1);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			//commu_encode(v[0],v[1],v[2],v[3]);
			com_flag=COMMU_IDLE;
			commu_encode(RealValue[0],RealValue[1],RealValue[2],RealValue[3]);
			direction_485_ctrl(0);
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
		}

	//	printf("sum = %lf KG     ",sum_weight);
	//	printf("\n");
	}
	
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



// static void USART_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure;
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
// 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
// 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
// 	USART_InitStructure.USART_BaudRate = 115200;
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
// 	USART_InitStructure.USART_Parity = USART_Parity_No;
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
// 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// 	USART_Init(USART1,&USART_InitStructure);
	
// 	USART_Cmd(USART1, ENABLE);
// 	USART_ClearFlag(USART1, USART_FLAG_TC);
// 	USART_ClearFlag(USART1, USART_FLAG_RXNE);
// 	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
// 	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	
// }

void Comu_485_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //485 ctrl
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);

}

void TIM2_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 2000;
	TIM_TimeBaseStructure.TIM_Prescaler = (84 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM3, 0, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, DISABLE);

}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		delay_flag = 1;
		TIM_Cmd(TIM2, DISABLE);
	}
	
}

void Delayms(unsigned int ms)
{
	TIM2->ARR= ms * 1000;  //设定计数器自动重装值//刚好20ms
	TIM_Cmd(TIM2, ENABLE);
	while(delay_flag == 0);
	
	delay_flag = 0;
}

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
//#endif


void TIM9_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  ///使能TIM9时钟
	
 	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);//初始化TIM9
	
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM9,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn ; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM1_BRK_TIM9_IRQHandler(void) //senser
{

	if(TIM_GetITStatus(TIM9, TIM_IT_Update)!=RESET)
	{
		
// 		for(i=0;i<4;i++)
// 		{
// 			uwADC1ConvertedVoltage[i] =(double )(uhADC1ConvertedValue[i] )*3300/0xFFF/1000;
// 			RealValue[i]=(buf[i][Max_buf-1]/(Max_buf-1)-offset[i])/weight[i];
// 			buf[i][j]=uwADC1ConvertedVoltage[i];
// 			buf[i][Max_buf-1]=0;
// 			for(k=0;k<(Max_buf-1);k++)
// 			{
// 				buf[i][Max_buf-1]+=buf[i][k];
// 			}
			//if(i==2)		
		//	printf("ADC[%d] = %lf KG     ",i,RealValue[i]);
		//Delayms(10);
//		}
// 		j++;
// 		if(j==Max_buf)
// 			j=0;
// 		sum_weight=0;
// 		for(i=0;i<4;i++)
// 		{
// 			sum_weight+=RealValue[i];
// 		}
		
	}
}
