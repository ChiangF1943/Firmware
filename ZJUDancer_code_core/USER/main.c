
#include <function_conf.h>


uint8_t tx_buff_u2[28] = {0};
uint8_t buff_u2_len = 0;
uint8_t tim7_flag = 0;

uint16_t uart1_tx_rate = 0;
uint16_t uart1_tx_count = 0;
uint16_t uart1_rx_rate = 0;
uint16_t uart1_rx_count = 0;


int main(void)
{	
	SystemInit();
	//SYSCLK_Init(256, 8);
	//RCC_Config();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
		Led_Init();
		Delayms(200);
		TIM12_Init(500);
		
		Usart6_Init(256000);
		SPI2_Init();
		SPI2_Interp_config();
		TIM3_Init(5);  //陀螺仪5ms周期
		TIM6_Init(10); //和上位机通信10ms周期,控制串口2
		TIM7_Init(1000); //1000ms
		
		tim7_flag = 0;
		
		Usart2_Init(576000);
		Usart2_Interp_Config();
		
	while(1)
	{
		//AdisReadData();
		//printf("acc x = %f, y = %f, z = %f\r\n", accf[0], accf[1], accf[2]);
		//printf("voltage = %d\r\n", voltage_uint32);
		//printf("voltage_uint32 = %d\r\n", voltage_uint32);
	}
	
}
  





