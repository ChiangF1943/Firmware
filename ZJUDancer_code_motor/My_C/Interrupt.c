
#include <function_conf.h>
#include "stdarg.h"
#include "stdio.h"


void USART1_IRQHandler(void)
{
	static uint8_t rx_buff[58] = {0};
	static uint8_t pointer = 0;
	uint8_t ch_temp = 0;
	uint32_t checksum_cal = 0, checksum_word = 0;
	uint8_t i = 0, j = 0;
	uint32_t packet_type;
	
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
		//USART6->DR = ch_temp;
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
		
		if(pointer > 4 && pointer < 58)
		{
			rx_buff[pointer++] = ch_temp;
			if(pointer == 58)
			{
				checksum_cal = check_sum_reverse_to_uint32(rx_buff, 0, 57-4);
				checksum_word = byte4_to_uint32(rx_buff, 54);
				if(checksum_cal == checksum_word)
				{
					packet_type = byte4_to_uint32(rx_buff, 4);
					
					action_flag = rx_buff[11];
					
					if(packet_type == packet_type_position)
					{
						action_flag = rx_buff[11];
						for(i = 0, j = 12; i < 20; i++)
						{
							position_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					}
					else if(packet_type == packet_type_speed_limit)
						for(i = 0, j = 12; i < 20; i++)
						{
							speed_limit_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_torque_limit)
						for(i = 0, j = 12; i < 20; i++)
						{
							torque_limit_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_led)
						for(i = 0, j = 12; i < 20; i++)
						{
							led_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_torque_enable)
						for(i = 0, j = 12; i < 20; i++)
						{
							torque_enable_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_p_gain)
						for(i = 0, j = 12; i < 20; i++)
						{
							p_gain_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_i_gain)
						for(i = 0, j = 12; i < 20; i++)
						{
							i_gain_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					else if(packet_type == packet_type_d_gain)
						for(i = 0, j = 12; i < 20; i++)
						{
							d_gain_buff[i] = ((uint16_t)(rx_buff[j]) << 8) | ((uint16_t)(rx_buff[j+1]));
							j = j+2;
						}
					
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
			temp = uart2_tx_buff[tx_pointer++];
			USART2->DR = temp;
			//USART6->DR = temp;
		}
		else
		{
			tx_pointer = 1;
			B485_1_R;
		}
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
	static uint8_t tx_pointer = 0;
	uint8_t temp;
	
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		
		if(tx_pointer < buff_u3_len)
		{
			temp = uart3_tx_buff[tx_pointer++];
			USART3->DR = temp;
			
		}
		else
		{
			tx_pointer = 1;
			B485_2_R;
		}
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

void UART4_IRQHandler(void)
{
//	static uint8_t rx_buff[28] = {0};
	static uint8_t tx_pointer = 0;
//	uint8_t ch_temp = 0;
//	uint32_t checksum_cal = 0, checksum_word = 0;
	
	static uint8_t rx_tmp = 0;    //for servo test
	
	
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		USART_ClearFlag(UART4, USART_FLAG_TC);
		
		if(tx_pointer < buff_u4_len)
			UART4->DR = uart4_tx_buff[tx_pointer++];
		else
		{
			tx_pointer = 1;
			B485_3_R;
		}
	}
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		USART_ClearFlag(UART4, USART_FLAG_RXNE);
		
		rx_tmp = UART4->DR;      // for servo test
		Usart6_Tran((char) rx_tmp );  //for servo test
		
	}
	
	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	{
		USART_ReceiveData(UART4);
		
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




