
#include <function_conf.h>

uint32_t voltage_uint32 = 0;
uint8_t tx_buff_u4[28] = {0};
uint8_t buff_u4_len = 0;

uint8_t tx_buff_u2[58] = {0};
uint8_t buff_u2_len = 0;

uint8_t tx_buff_u1[58] = {0};
uint8_t buff_u1_len = 0;


uint16_t position_buff[30] = {0};
uint16_t speed_limit_buff[30] = {0};
uint16_t torque_limit_buff[30] = {0};
uint16_t led_buff[30] = {0};
uint16_t torque_enable_buff[30] = {0};
uint16_t p_gain_buff[30] = {0};
uint16_t i_gain_buff[30] = {0};
uint16_t d_gain_buff[30] = {0};

uint8_t tim7_flag = 0;

uint8_t action_flag = 0;

uint16_t uart1_tx_rate = 0;
uint16_t uart1_tx_count = 0;
uint16_t uart1_rx_rate = 0;
uint16_t uart1_rx_count = 0;


int main(void)
{	

//	uint8_t ch = 0;
//	uint8_t i = 0;
	
	SystemInit();
	//SYSCLK_Init(256, 8);
	
	//RCC_Config();
	

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		
		Led_Init();
		
		Delayms(200);
		
		
		Usart3_Init(256000);
		Usart3_Interp_Config();
		
		Usart4_Init(256000);
		Usart4_Interp_Config();
		
		Usart6_Init(256000);
		
		SPI2_Init();
		//SPI2_Interp_config();
		
		//TIM2_Init(5000);
		TIM12_Init(500);
		//TIM11_Init(2000);
		TIM3_Init(5); 
		
		//TIM4_PWM_Init(1000, 16);
		TIM4_Init(2);
		TIM5_Init(500);
		TIM6_Init(10);
		
		TIM7_Init(1000);
		
		tim7_flag = 0;
		//Set_Motor_Torque_Limit(1023);
		//Set_Motor_Speed_Limit(100);
		
		//Set_Motor_Led_All(1);
		//Set_Motor_Torque_Enable_All(1);
		Set_Motor_Torque_Limit(1023);
		Set_Motor_Torque_Enable_All(1);
		
		Usart1_Init(115200);
		Usart1_Interp_Config();
	
		Usart2_Init(1000000);
		Usart2_Interp_Config();
		
	while(1)
	{
		if(tim7_flag == 2 )
		{
			Set_Motor_Torque_Enable_All(1);
			
		}
		if(tim7_flag == 3 )
		{
			Set_Motor_Speed_Limit(350);
			//Set_Motor_Torque_Limit(1023);
			//action_flag = 1;
		}
		if(tim7_flag == 4 )
		{
			Set_Motor_Speed_Limit(100);
			Set_Motor_Torque_Limit(1023);
			//action_flag = 1;
		}
		if(tim7_flag >= 10 )
		{
			Set_Motor_Speed_Limit(1023);
			Set_Motor_Torque_Limit(1023);
			TIM_Cmd(TIM7, DISABLE);
		}
		
		
		//ch = Usart3_Rece();
		//Usart6_Tran(ch);
		//AdisReadData();
		//printf("acc x = %f, y = %f, z = %f\n", accf[0], accf[1], accf[2]);
		//printf("voltage = %d\n", voltage_uint32);
		//printf("voltage_uint32 = %d\n", voltage_uint32);
		
		//position_buff[0] = 100;
		//Delayms(2000);
		//position_buff[0] = 0;
		//position_buff[0]++;
		//for(i = 1; i < 20; i++)
		//position_buff[i] = position_buff[i-1] + 1;
		
		
	}
	
}
  





