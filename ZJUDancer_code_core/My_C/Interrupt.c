
#include <function_conf.h>
#include "stdarg.h"
#include "stdio.h"



void USART2_IRQHandler(void)
{
	static uint8_t tx_pointer = 0;
	uint8_t temp;
	
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_ClearFlag(USART2, USART_FLAG_TC);
		
		if(tx_pointer < buff_u2_len)
		{
			temp = tx_buff_u2[tx_pointer++];
			USART2->DR = temp;
			//USART6->DR = temp;
		}
		else
			tx_pointer = 1;
	}
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		
		
	}
	
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART2);
		
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
