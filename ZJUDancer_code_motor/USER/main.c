
#include <function_conf.h>

uint32_t voltage_uint32 = 0;
uint8_t tx_buff[28] = {0};

uint8_t id_buff[30] = {1,2,3,4,5,
					  6,7,8,9,10,
					  11,12,13,14,15,
					  16,17,18,21,22};

uint16_t position_buff[30] = {0};
uint16_t speed_limit_buff[30] = {0};
uint16_t torque_limit_buff[30] = {0};
uint16_t led_buff[30] = {0};
uint16_t torque_enable_buff[30] = {0};
uint16_t p_gain_buff[30] = {0};
uint16_t i_gain_buff[30] = {0};
uint16_t d_gain_buff[30] = {0};

uint8_t action_flag = 0;
uint8_t tim7_flag = 0;

uint8_t uart2_tx_buff[400] = {0};
uint8_t uart3_tx_buff[400] = {0};
uint8_t uart4_tx_buff[400] = {0};
//uint8_t uart6_tx_buff[400] = {0};
uint8_t buff_u2_len = 0;
uint8_t buff_u3_len = 0;
uint8_t buff_u4_len = 0;
//uint8_t buff_u6_len = 0;
uint16_t buff_u2_load_pointer;
uint16_t buff_u3_load_pointer;
uint16_t buff_u4_load_pointer;
//uint16_t buff_u6_load_pointer;



int main(void)
{	

//	uint8_t ch = 0;
//	uint8_t i = 0;
	
	SystemInit();
	//SYSCLK_Init(256, 8);
	Delayms(100);
	//RCC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Led_Init();
	
	Delayms(200);
	
	Usart1_Init(1000000);
	Usart1_Interp_Config();
	
	Bus_485_Con_Init();
	
	Usart2_Init(1000000);
	//Usart2_Interp_Config();
	Usart3_Init(1000000);
	//Usart3_Interp_Config();
	Usart4_Init(1000000);
	//Usart4_Interp_Config();
	
	Usart6_Init(256000);
	
	Motor_V0_Speed_Limit(ID_ALL, 60);
	
	Motor_V0_Torque(ID_ALL, 1);//上电卡住的动作
	Motor_V0_Speed_Limit(ID_ALL, 50);
	Delayms(500);
	//while(1)
	{
		//Motor_V0_LED(ID_ALL, 1);
		//Motor_V0_Torque(ID_ALL, 0);
		//Motor_V0_Position(ID_ALL, 2040);
		//Motor_V0_Action(ID_ALL);
		//Delayms(500);
		
		
// 		Motor_V0_LED(ID_ALL, 0);
// 		Motor_V0_Torque(ID_ALL, 1);
// 		Motor_V0_Position(ID_ALL, 600);
// 		Motor_V0_Action(ID_ALL);
// 		Delayms(500);
	}
	
	
	DMA_Uart2_Tx_Config();
	DMA_Uart3_Tx_Config();
	DMA_Uart4_Tx_Config();
	DMA_Uart6_Tx_Config();
	
	//TIM2_Init(500);
	TIM3_Init(4);     //load data to motor for 485
	TIM12_Init(500);  
	TIM5_Init(200);
	
	Delayms(1000);


	//Set_Motor_Torque_Enable_All(1);
	//Delayms(1000);
	//Set_Motor_Torque_Enable_All(0);
	//Set_Motor_Speed_Limit(50);
	//Set_Motor_Torque_Limit(1023);
	while(1)
	{
		//Set_Motor_Position(100);
		//Delayms(3000);
		//Set_Motor_Position(900);
		//Delayms(3000);
	}
	
	//while(1);
	
}






