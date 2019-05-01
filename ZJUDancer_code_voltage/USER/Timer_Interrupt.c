#include <stm32f4xx.h>
#include <function_conf.h>

uint16_t tim12_counter = 0;
	


void TIM2_IRQHandler(void)
{
	uint16_t voltage_uint16 = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		
		voltage_uint16 = AD7171_Read();
		voltage = (float)voltage_uint16 * (float)2 / (float)65536 * (float)4.096 / ((float)(1000+220) / (float)(2000+2000+1000+220));
		//printf("voltage = %f\n", voltage);
	}
	
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		
	}
}



void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		
		//TIM_SetCompare1(TIM4, width);
		
	}
}



void TIM5_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		
		
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
		
	}
}

void TIM7_IRQHandler(void)
{


	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);

	}
	
}

void TIM8_UP_TIM13_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM8, TIM_FLAG_Update);
		
		TIM_Cmd(TIM8, DISABLE);
		TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);
		
	}
}


void TIM8_BRK_TIM12_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM12, TIM_FLAG_Update);
		
		
		
		tim12_counter += 1;
		if(tim12_counter == 1)
		{
			LED1_OFF;
			LED2_OFF;
			

		}
		
		else if(tim12_counter == 2)    // ∞Î√Î¥¶¿Ì∫Ø ˝
		{
			LED1_ON;
			LED2_ON;
			
			tim12_counter = 0;
			
			cpu_usage = 100 - (double)cpu_count / 9880520.0 * 100;
			cpu_count_flag = 1;
			cpu_count = 0;
			
			//printf("acx = %5.3f, acy = %5.3f, acz = %5.3f ", accf[0], accf[1], accf[2]);
			//printf("gyx = %5.3f, gyy = %5.3f, gyz = %5.3f \n", gyrof[0], gyrof[1], gyrof[2]);
			//printf("cpu_usage = %5.3f", cpu_usage);
			
			//printf("\n");
			//===============
		}
		
		
		else
			tim12_counter= 0;

		
		
	}
}



