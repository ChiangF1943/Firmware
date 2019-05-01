#include <stm32f4xx.h>
#include <function_conf.h>

uint16_t tim12_counter = 0;
	


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		
		tim_int_counter[2]++;
		//Adis_Start();
		//AdisReadData();
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
	static uint16_t width = 0;
	static int8_t dir_flag = 0;
	
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		
		if(dir_flag == 0)
			width = width + 1;
		if(dir_flag == 1)
			width = width - 1;
		
		if(width >= 2000)
			dir_flag = 1;
		if(width <= 2)
			dir_flag = 0;
		
		TIM_SetCompare1(TIM4, width);
		TIM_SetCompare2(TIM4, width);
		TIM_SetCompare3(TIM4, width);
		TIM_SetCompare4(TIM4, width);
		//TIM4->CCR1 = width;
		//TIM4->CCR2 = width;
		//TIM4->CCR3 = width;
		//TIM4->CCR4 = width;
		
	}
}


void TIM5_IRQHandler(void)
{
	static uint16_t width = 0;
	static int8_t dir_flag = 0;
	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		
		if(dir_flag == 0)
			width = width + 1;
		if(dir_flag == 1)
			width = width - 1;
		
		if(width >= 2000)
			dir_flag = 1;
		if(width <= 2)
			dir_flag = 0;
		
		TIM_SetCompare1(TIM5, width);
		TIM_SetCompare2(TIM5, width);
		TIM_SetCompare3(TIM5, width);
		TIM_SetCompare4(TIM5, width);
		
		
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
			LED3_OFF;
			LED4_OFF;
			
			FAN1_OFF;
			FAN2_OFF;
			LCD_LED_OFF;
		}
		
		else if(tim12_counter == 2)    // °ëÃë´¦Àíº¯Êý
		{
			LED1_ON;
			LED2_ON;
			LED3_ON;
			LED4_ON;
			
			FAN1_ON;
			FAN2_ON;
			LCD_LED_ON;
			
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



