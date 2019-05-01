#include <stm32f4xx.h>

#include <function_conf.h>

uint32_t voltage_uint32 = 0;

int main(void)
{	
	uint32_t number = 0;
	SystemInit();
	RCC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Led_Init();
	Fan_Init();
	LCD_LED_Init();
	Delayms(100);
	
	Usart1_Init(256000);
	Usart1_Interp_Config();
	Usart5_Init(256000);
	//Usart5_Interp_Config();
	//Usart6_Init(115200);
	
	//SPI1_Init();
	//SPI1_Interp_config();
	//SPI2_Init();
	//HMC_Init();
	
	/*
	while(1)
	{
		AdisReadData();
		printf("acx = %5.3f, acy = %5.3f, acz = %5.3f ", accf[0], accf[1], accf[2]);
		printf("gyx = %5.3f, gyy = %5.3f, gyz = %5.3f \n", gyrof[0], gyrof[1], gyrof[2]);
		Delayms(1000);
	}
	*/
	
	//TIM2_Init(5000);
	TIM12_Init(500);
	
	TIM4_PWM_Init(2000, 42);
	TIM5_PWM_Init(2000, 42);
	
	TIM4_IT_Init();
	TIM5_IT_Init();
	
	port_init();
    Ht1621_Init();
	
	while(1)
	{
		print_lcd(voltage_uint32);
		Delayms(1);
		number++;
		
	}
	
	while(1)
	{
		if(cpu_count_flag == 1)
		{
			cpu_count_flag = 0;
			cpu_count = 0;
		}
		cpu_count++;
	    
	}
	
	
}


