#include <stm32f4xx.h>
#include <function_conf.h>

uint16_t tim12_counter = 0;
	


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		
		
	}
	
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		AdisReadData();
	}
}

void TIM4_IRQHandler(void)
{
	static uint8_t step = 0;
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		
		if((step % 2) == 0)
			Load_tx_buff_u2(packet_type_position);
		else if(step == 1)
			Load_tx_buff_u2(packet_type_speed_limit);
		else if(step == 3)
			Load_tx_buff_u2(packet_type_torque_limit);
		else if(step == 5)
			Load_tx_buff_u2(packet_type_led);
		else if(step == 7)
			Load_tx_buff_u2(packet_type_torque_enable);
		else if(step == 9)
			Load_tx_buff_u2(packet_type_p_gain);
		else if(step == 11)
			Load_tx_buff_u2(packet_type_i_gain);
		else if(step == 13)
			Load_tx_buff_u2(packet_type_d_gain);
		
		if(step < 13)
			step += 1;
		else
			step = 0;
		
		Start_u2_tran();
	}
}

void TIM5_IRQHandler(void)
{
	uint8_t i;
	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
		
		for(i = 0; i < 20; i++)
			printf("%d ", position_buff[i]);
		printf("\r\n");
		
// 		printf("voltage = %d", voltage_uint32);
// 		printf("\r\n");
// 		printf("accxyz = %d; %d; %d;", accxyz[0], accxyz[1], accxyz[2]);
// 		printf("\r\n");
// 		printf("gyroxyz = %d; %d; %d;", gyroxyz[0], gyroxyz[1], gyroxyz[2]);
// 		printf("\r\n");
// 		printf("magnxyz = %d; %d; %d;", magnxyz[0], magnxyz[1], magnxyz[2]);
// 		printf("\r\n");
// 		printf("\r\n");
		
		Load_tx_buff_u4();
		Start_u4_tran();
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
		
		Load_tx_buff_u1();
		Start_u1_tran();
		
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

void TIM8_UP_TIM13_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM8, TIM_FLAG_Update);
		
		TIM_Cmd(TIM8, DISABLE);
		TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);
		
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) 
{
	static uint8_t step = 0;
	if(TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM11, TIM_FLAG_Update);
		
		if(step == 1)
			Set_Motor_Position(1000);
		else
			Set_Motor_Position(0);
		
		step += 1;
		if(step > 1)
			step = 0;
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
			Set_Motor_Led_All(0);
		}
		
		else if(tim12_counter == 2)    // ∞Î√Î¥¶¿Ì∫Ø ˝
		{
			LED1_ON;
			LED2_ON;
			Set_Motor_Led_All(1);
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



