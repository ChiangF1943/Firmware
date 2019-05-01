#include <stm32f4xx.h>
#include <function_conf.h>

uint16_t tim12_counter = 0;
	

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		AdisReadData();
	}
}


void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
		
		Load_tx_buff_u2();
		Start_u2_tran();
		
	}
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
		
		tim7_flag += 1;
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



