
#include <function_conf.h>
#include "stdarg.h"
#include "stdio.h"


void USART1_IRQHandler(void)
{
	static uint8_t rx_buff[28] = {0};
	static uint8_t pointer = 0;
	uint8_t ch_temp = 0;
	uint32_t checksum_cal = 0, checksum_word = 0;
		
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		
	}
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		
		ch_temp = USART1->DR;
		if(pointer < 4)
		{
			rx_buff[pointer++] = ch_temp;
			return;
		}
		if(pointer == 4)
		{
			if( (rx_buff[0] == 0x12) && (rx_buff[1] == 0x34) && (rx_buff[2] == 0x56) && (rx_buff[3] == 0x78) )
				rx_buff[pointer++] = ch_temp;
			else
			{
				rx_buff[0] = rx_buff[1];
				rx_buff[1] = rx_buff[2];
				rx_buff[2] = rx_buff[3];
				rx_buff[3] = ch_temp;
				
			}
			return;
		}
		
		if(pointer > 4 && pointer < 28)
		{
			rx_buff[pointer++] = ch_temp;
			if(pointer == 28)
			{
				checksum_cal = check_sum_reverse_to_uint32(rx_buff, 0, 27-4);
				checksum_word = byte4_to_uint32(rx_buff, 24);
				if(checksum_cal == checksum_word)
					voltage_uint32 = byte4_to_uint32(rx_buff, 4);
				pointer = 0;
			}
		}
		
	}
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART1);
		
	}
	
}



void USART3_IRQHandler(void)
{

	
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		
		
		return;
	}
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		
		
			
	}
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART3);
		
	}
	
}



//============================ USART6 INT ============================//

void USART6_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_TC);
		USART_ClearFlag(USART6, USART_FLAG_TC);
		
	}
	
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		USART_ClearFlag(USART6, USART_FLAG_RXNE);
		
	}
	
	if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART6);
	}
}



