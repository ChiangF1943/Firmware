
#include <function_conf.h>
#include "stdarg.h"
#include "stdio.h"


void USART1_IRQHandler(void)
{
	static uint8_t rx_buff[200] = {0};
	static uint8_t pointer = 0;
	uint8_t ch_temp = 0;
//	uint32_t checksum_cal = 0, checksum_word = 0;
	uint8_t i = 0, j = 0;
	
	static uint8_t tx_pointer = 0;
	uint8_t temp;
	
	//uint8_t type = 0;
	//static uint8_t pc_type = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		
		if(tx_pointer < buff_u1_len)
		{
			temp = tx_buff_u1[tx_pointer++];
			USART1->DR = temp;
			//USART6->DR = temp;
		}
		else
		{
			tx_pointer = 1;
			uart1_tx_rate++;
		}
		
	}
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		
		ch_temp = USART1->DR;
		//USART6->DR = ch_temp;
		if(pointer < 2)
		{
			rx_buff[pointer++] = ch_temp;
			return;
		}
		if(pointer == 2)
		{
			if( (rx_buff[0] == 0xff) && (rx_buff[1] == 0xff) && (ch_temp == 0x01) )
				rx_buff[pointer++] = ch_temp;
			else
			{
				rx_buff[0] = rx_buff[1];
				rx_buff[1] = rx_buff[2];
				rx_buff[2] = ch_temp;
				
			}
			return;
			
		}
		
		if(pointer > 2 && pointer < 45)
		{
			rx_buff[pointer++] = ch_temp;
			if(pointer >= 45)
			{
				for(i = 0, j = 4; i < 20; i++)
				{
					//position_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
					if(i != 18)
						position_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
					else if(i == 18)
						position_buff[i] = (((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1])));
					j = j+2;
					action_flag = 1;
				}
				
				pointer = 0;
			}
		}
	}
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART1);
		
	}
	
}

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

void USART3_IRQHandler(void)
{
	static uint8_t rx_buff[28] = {0};
	static uint8_t pointer = 0;
	uint8_t ch_temp = 0;
	uint32_t checksum_cal = 0, checksum_word = 0;
	
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
		
		ch_temp = USART3->DR;
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
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(USART3);
		
	}
	
}

void UART4_IRQHandler(void)
{
//	static uint8_t rx_buff[28] = {0};
	static uint8_t tx_pointer = 0;
//	uint8_t ch_temp = 0;
//	uint32_t checksum_cal = 0, checksum_word = 0;
	
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		USART_ClearFlag(UART4, USART_FLAG_TC);
		
		if(tx_pointer < buff_u4_len)
			UART4->DR = tx_buff_u4[tx_pointer++];
		else
			tx_pointer = 1;
	}
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		USART_ClearFlag(UART4, USART_FLAG_RXNE);
		
	}
	
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(UART4);
		
	}
	
}




void UART5_IRQHandler(void)
{
	static uint8_t rx_buff[28] = {0};
	static uint8_t pointer = 0;
	uint8_t ch_temp = 0;
	uint32_t checksum_cal = 0, checksum_word = 0;
	uint8_t i = 0;

	if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_TC);
		USART_ClearFlag(UART5, USART_FLAG_TC);
		
	}
	
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		USART_ClearFlag(UART5, USART_FLAG_RXNE);
		ch_temp = UART5->DR;
		//uart5测试
		//UART5->DR = ch_temp;
		
		/////////////
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
				{
					for(i = 0; i < 6; i++)
						magnraw_LSM[i] = rx_buff[4+i];
					magnxyz[0] = (uint16_t)(magnraw_LSM[0]) << 8 | (uint16_t)(magnraw_LSM[1]);
					magnxyz[1] = (uint16_t)(magnraw_LSM[2]) << 8 | (uint16_t)(magnraw_LSM[3]);
					magnxyz[2] = (uint16_t)(magnraw_LSM[4]) << 8 | (uint16_t)(magnraw_LSM[5]);
					
				}
				pointer = 0;
				
			}
		}
	}
	
	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)//×￠òa￡?2??üê1ó?if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)à′?D??
	{
		USART_ReceiveData(UART5);
		
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




