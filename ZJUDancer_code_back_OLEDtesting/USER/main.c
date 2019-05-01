#include <stm32f4xx.h>

#include <function_conf.h>
#include <oled.h>
uint32_t voltage_uint32 = 0;

int main(void)
{	
	uint8_t i;
	char xgyro[14] = "gyro-x:unknown";
	char ygyro[14] = "gyro-y:unknown";
	char voltage[14] = "    16.0 V    ";
	char team[14] = "ZJUDancer-bot1";
	float voltage1;
	float voltage2;
	uint8_t dig_vol_1 = 0;
	uint8_t dig_vol_2 = 0;
	char str_1[16], str_2[16];
	
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
	
	//port_init();
    //Ht1621_Init();
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	
	/*	t=' ';
		OLED_ShowCHinese(0,0,0);//中
				OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		*/
	
	//OLED_ShowChar(0, 0, 'C', 16);
	//OLED_ShowChar(0, 2, 'C', 16);
	//OLED_ShowChar(0, 4, 'C', 16);
	//OLED_ShowChar(0, 6, 'C', 16);
	
// 	OLED_Show_Char_Line(3, team, 14);
// 	OLED_Show_Char_Line(1, voltage, 14);
	

	voltage1 = 15.2;
	voltage2 = 15.8;
	printf_oled(1, "Bat1: %2.3f V", voltage1);
	printf_oled(3, "Bat2: %2.3f V", voltage2);
	dig_vol_1 = (uint8_t)( (voltage1 - (float)14.8) / (float)(16.8 - 14.8) * 16 );
	dig_vol_2 = (uint8_t)( (voltage2 - (float)14.8) / (float)(16.8 - 14.8) * 16 );
	for(i = 0; i < 16; i++)
	{
		if(dig_vol_1 >= i+1)
			str_1[i] = '+';
		else 
			str_1[i] = ' ';
	}
	
	for(i = 0; i < 16; i++)
	{
		if(dig_vol_2 >= i+1)
			str_2[i] = '+';
		else 
			str_2[i] = ' ';
	}
	OLED_Show_Char_Line(0, (int8_t *)str_1, 16);
	OLED_Show_Char_Line(2, (int8_t *)str_2, 16);
	
	while(1)
	{
		//print_lcd(voltage_uint32);
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


