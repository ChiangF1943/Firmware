#include <stm32f4xx.h>
#include <function_conf.h>


	
void TIM2_IRQHandler(void)
{
	uint8_t i = 0;
	static uint8_t flag = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		
		if(flag == 0)
		{
			for(i = 0; i < 20; i++)
				led_buff[i] = 1;
			flag = 1;
		}
		else 
		{
			for(i = 0; i < 20; i++)
			led_buff[i] = 0;
			flag = 0;
		}
	}
	
}

void TIM3_IRQHandler(void)
{
	static uint8_t step = 0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		
		USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
		
		B485_1_S;
		B485_2_S;
		B485_3_S;
		if( (step % 2) == 0)
		{
			if(action_flag == 1)
			{
				Reset_uart2_tx_buff();
				Motor_Position_Load_Uart2();
				Motor_Action_Load_Uart2();
				DMA_Uart2_Tx_Start();
				
				Reset_uart3_tx_buff();
				Motor_Position_Load_Uart3();
				Motor_Action_Load_Uart3();
				DMA_Uart3_Tx_Start();
				
				Reset_uart4_tx_buff();
				Motor_Position_Load_Uart4();
				Motor_Action_Load_Uart4();
				DMA_Uart4_Tx_Start();
			}
			
		}
		
		
		if( step == 1)
		{
			Reset_uart2_tx_buff();
			Motor_Led_Load_Uart2();
			DMA_Uart2_Tx_Start();
			
			Reset_uart3_tx_buff();
			Motor_Led_Load_Uart3();
			DMA_Uart3_Tx_Start();
			
			Reset_uart4_tx_buff();
			Motor_Led_Load_Uart4();
			DMA_Uart4_Tx_Start();
			
			printf_buff_reset();
			myprintf("ENTER!!!");
		  myprintf("\r\n");
			
			
			Reset_uart4_tx_buff();
			Motor_Present_Position_Load_Uart4();
			DMA_Uart4_Tx_Start();
			
			myprintf("Leave!!!");
		  myprintf("\r\n");
			myprintf_start_tran();
		}
		
		if( step == 3)
		{
			Reset_uart2_tx_buff();
			Motor_Torque_Enable_Load_Uart2();
			DMA_Uart2_Tx_Start();
			
			Reset_uart3_tx_buff();
			Motor_Torque_Enable_Load_Uart3();
			DMA_Uart3_Tx_Start();
			
			Reset_uart4_tx_buff();
			Motor_Torque_Enable_Load_Uart4();
			DMA_Uart4_Tx_Start();
		}
		
		if( step == 5)
		{
			Reset_uart2_tx_buff();
			Motor_Speed_Limit_Load_Uart2();
			DMA_Uart2_Tx_Start();
			
			Reset_uart3_tx_buff();
			Motor_Speed_Limit_Load_Uart3();
			DMA_Uart3_Tx_Start();
			
			Reset_uart4_tx_buff();
			Motor_Speed_Limit_Load_Uart4();
			DMA_Uart4_Tx_Start();
		}
		
		if( step == 7)
		{
			
			Reset_uart2_tx_buff();
			Motor_Torque_Limit_Load_Uart2();
			DMA_Uart2_Tx_Start();
			
			Reset_uart3_tx_buff();
			Motor_Torque_Limit_Load_Uart3();
			DMA_Uart3_Tx_Start();
			
			Reset_uart4_tx_buff();
			Motor_Torque_Limit_Load_Uart4();
			DMA_Uart4_Tx_Start();
		}
		
		//step += 1;
		if(step >= 8)
			step = 0;
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
	uint8_t i = 0;
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
		
		printf_buff_reset();
		
		myprintf("action_flag = %d ", action_flag);
		myprintf("\r\n");
		
		//myprintf("position_buff ");
		//for(i = 0; i < 20; i++)
			//myprintf("%d ", position_buff[i]);
		//myprintf("\r\n1");
		
		/*myprintf("speed_limit_buff ");
		for(i = 0; i < 20; i++)
			myprintf("%d ", speed_limit_buff[i]);
		myprintf("\r\n");
		
		myprintf("torque_limit_buff ");
		for(i = 0; i < 20; i++)
			myprintf("%d ", torque_limit_buff[i]);
		myprintf("\r\n");
		*/
		/*
		myprintf("led_buff  ");
		for(i = 0; i < 20; i++)
			myprintf("%d ", led_buff[i]);
		myprintf("\r\n");
		*/
		/*
		myprintf("torque_enable_buff ");
		for(i = 0; i < 20; i++)
			myprintf("%d ", torque_enable_buff[i]);
		myprintf("\r\n");
		*/
		//myprintf("\r\n");
		myprintf_start_tran();
		
		/*
		printf("p_gain_buff  ");
		for(i = 0; i < 20; i++)
			printf("%d ", p_gain_buff[i]);
		print_enter();
		
		printf("i_gain_buff  ");
		for(i = 0; i < 20; i++)
			printf("%d ", i_gain_buff[i]);
		print_enter();
		
		printf("d_gain_buff  ");
		for(i = 0; i < 20; i++)
			printf("%d ", d_gain_buff[i]);
		print_enter();
		print_enter();
		*/
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
		tim7_flag = 1;
		
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
	static uint16_t tim12_counter = 0;
	
	if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM12, TIM_FLAG_Update);
		
		tim12_counter += 1;
		if(tim12_counter == 1)
		{
			LED1_OFF;
			LED2_OFF;
		
		}
		
		else if(tim12_counter == 2)    // °ëÃë´¦Àíº¯Êý
		{
			LED1_ON;
			LED2_ON;
			
			tim12_counter = 0;
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



