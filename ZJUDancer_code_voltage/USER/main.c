
#include <function_conf.h>

float voltage = 0;
uint32_t display_vol = 0;
float buzzer1_flag = 0;
uint8_t tx_buff[28] = {0};

int main(void)
{	
	uint16_t voltage_uint16 = 0;
	float vol_buff[10] = {0};
	
	uint8_t i = 0;
	
	SystemInit();
	//SYSCLK_Init(256, 8);
	
	//RCC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Led_Init();
	
	Delayms(100);
	
	Usart1_Init(256000);
	
	Usart6_Init(256000);
	
	SPI1_Init();
	AD7171_Reset_Init();
	//Buzzer2_Disable();
	
	//TIM2_Init(5000);
	TIM12_Init(500);
	
	//TIM4_PWM_Init(1000, 16);
	//TIM4_IT_Init();
	
	/*
	while(1)
	{
		//Buzzer_Frequence_Set(30);
		Delayms(200);
		
		//Buzzer_Frequence_Set(10);
		Delayms(200);
		
		//Buzzer_Frequence_Set(0);
		Delayms(1000);
	}
	*/
	
	while(1)
	{
		//voltage_uint16 = AD7171_Read();
		//voltage = (float)voltage_uint16 * (float)2 / (float)65536 * (float)4.096 / ((float)(1000+220) / (float)(2000+2000+1000+220));
		
		
		for(i = 0; i < 10; i++)
		{
			voltage_uint16 = AD7171_Read();
			vol_buff[i] = (float)voltage_uint16 * (float)2 / (float)65536 * (float)4.096 / ((float)(1000+220) / (float)(2000+2000+1000+220));
			Delayms(80);
			
		}
		max_min(vol_buff, 10);
		
		voltage = (vol_buff[6] + vol_buff[7] + vol_buff[8] + vol_buff[9])/4;
		
		display_vol = voltage / (float)0.0001;
		Load_tx_buff();
		Usart6_Tran_Str(tx_buff, 28);
		Usart1_Tran_Str(tx_buff, 28);
		//printf("display_vol = %d\n", display_vol);
		Delayms(200);
		
	}
	
}
  





