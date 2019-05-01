
#include <function_conf.h>
#include <stm32l1xx.h>

#define window_width 25
void Test_LSM(void);

int main(void)
{	

//	uint8_t ch = 0;
	uint8_t i = 0;
//	signed short magxyz[3];
	
	SystemInit();
	Delayms(500);
	NVIC_PriorityGroupConfig(2);
	LED_Init();
	
	TIM2_Init(16000, 20);
	TIM_Cmd(TIM2,ENABLE);
	
	//Test_LSM();
	
	TIM3_Init(16000, 20);
	TIM_Cmd(TIM3,ENABLE);
	
	Usart1_Init(115200);
	//Usart1_Tran('A');
	//printf("Hello");
	SpiPortInit(); 
	LsmInit();
	while(1)
	{
		//LsmMagneticReadAverage(magxyz_averge);
		//MpuAccGyroRead(accxyz, gyroxyz);
		//AdisReadData(accxyz, gyroxyz);
		//sample_count++;
		/*for(i = 0; i < 12; i++)
		{
		printf("%d  %6d;  %6d;  %6d; \n", i+1, magtempxyz[i][0], magtempxyz[i][1], magtempxyz[i][2]);
		printf("\r\n");
		
		}
		printf("\r\n");
		//printf("A  %d;  %d;  %d; \n", magxyz_averge[0], magxyz_averge[1], magxyz_averge[2]);
		//printf("\r\n");
		//printf("\r\n");
		Delayms(500);*/
		
		//Load_tx_buff();
		//Usart1_Tran_Str(tx_buff, 28);
		
		
		/*
		LsmWhoRead();
		for(i = 0; i < 12; i++)
		{
			printf("%2x  ", who[i]);
			
		}
		printf("\r\n");
		*/
		/*
		if(return_compass_flag == 1)
		{
			return_compass_flag = 0;
			
		}*/
		
	}
}

void TIM2_IRQHandler(void)
{
	static uint8_t flag = 0;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		if(flag == 1)
		{
			flag = 0;
			LED1_ON;
		}
		else
		{
			LED1_OFF;
			flag = 1;
		}
		
	}
}

void Move_Average(void)
{
	static int16_t magn[window_width][3];
	uint8_t i = 0;
	int32_t sum;
	
	for(i = 0; i < window_width - 1; i++)
	{
		magn[i][0] = magn[i+1][0];
		magn[i][1] = magn[i+1][1];
		magn[i][2] = magn[i+1][2];
		
	}
	magn[window_width-1][0] = magxyz_averge[0];
	magn[window_width-1][1] = magxyz_averge[1];
	magn[window_width-1][2] = magxyz_averge[2];
	
	for(i = 0, sum = 0; i < window_width-1; i++)
		sum += magn[i][0];
	magn_move_average[0] = -(int16_t)(sum / window_width-1);
	
	for(i = 0, sum = 0; i < window_width-1; i++)
		sum += magn[i][1];
	magn_move_average[1] = -(int16_t)(sum / window_width-1);
	
	for(i = 0, sum = 0; i < window_width-1; i++)
		sum += magn[i][2];
	magn_move_average[2] = (int16_t)(sum / window_width-1);
	
}
void TIM3_IRQHandler(void)
{
	static uint8_t flag = 0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		LsmMagneticReadAverage(magxyz_averge);
		//Move_Average();
		Load_tx_buff();
		Usart1_Tran_Str(tx_buff, 28);
		//printf("A  %d;  %d;  %d; \n", magxyz_averge[0], magxyz_averge[1], magxyz_averge[2]);
	}
}

void Test_LSM(void)
{
	uint8_t i;
	Usart1_Init(115200);
	Usart1_Tran('A');
	printf("Hello");
	SpiPortInit(); 
	LsmInit();
	while(1)
	{
		LsmMagneticReadAverage(magxyz_averge);
		//MpuAccGyroRead(accxyz, gyroxyz);
		//AdisReadData(accxyz, gyroxyz);
		//sample_count++;
		for(i = 0; i < 12; i++)
		{
		printf("%d  %6d;  %6d;  %6d; \n", i+1, magtempxyz[i][0], magtempxyz[i][1], magtempxyz[i][2]);
		printf("\r\n");
		
		}
		printf("\r\n");
		//printf("A  %d;  %d;  %d; \n", magxyz_averge[0], magxyz_averge[1], magxyz_averge[2]);
		//printf("\r\n");
		//printf("\r\n");
		Delayms(500);
		
		//Load_tx_buff();
		//Usart1_Tran_Str(tx_buff, 28);
		
		
		/*
		LsmWhoRead();
		for(i = 0; i < 12; i++)
		{
			printf("%2x  ", who[i]);
			
		}
		printf("\r\n");
		*/
		/*
		if(return_compass_flag == 1)
		{
			return_compass_flag = 0;
			
		}*/
		
	}

}

