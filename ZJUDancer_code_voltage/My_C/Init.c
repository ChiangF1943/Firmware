#include <stm32f4xx.h>

#include <function_conf.h>


unsigned int cpu_count = 0;
unsigned char  cpu_count_flag = 0;
float cpu_usage = 0;


void RCC_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | 
	                       RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | 
						   RCC_AHB1Periph_GPIOG, ENABLE);

}


void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	LED1_ON;
	LED2_ON;
	
}

void Led1_Toggle(void)
{
	static char led1_state = 0x00;
	if(led1_state)
	{
		led1_state = 0;
		LED1_OFF;
	}
	else
	{
		led1_state = 1;
		LED1_ON;
	}
}


void Led2_Toggle(void)
{
	static char led2_state = 0x00;
	if(led2_state)
	{
		led2_state = 0;
		LED2_OFF;
	}
	else
	{
		led2_state = 1;
		LED2_ON;
	}
}

void SYSCLK_Init(uint16_t PLL_N_set, uint16_t PLL_P_set)
{
	 //ErrorStatus        State;
     uint32_t           PLL_M;      
     uint32_t           PLL_N;
     uint32_t           PLL_P;
     uint32_t           PLL_Q;
 
    /*配置前将所有RCC重置为初始值*/
     RCC_DeInit();
 
     /*这里选择 外部晶振（HSE）作为 时钟源，因此首先打开外部晶振*/
     RCC_HSEConfig(RCC_HSE_ON);
     /*等待外部晶振进入稳定状态*/
     while( RCC_WaitForHSEStartUp() != SUCCESS );

     /*
     **我们要选择PLL时钟作为系统时钟，因此这里先要对PLL时钟进行配置
     */
 
     /*选择外部晶振作为PLL的时钟源*/
    
     /* 到这一步为止，已有 HSE_VALUE = 8 MHz.
        PLL_VCO input clock = (HSE_VALUE or HSI_VALUE / PLL_M)，
        根据文档，这个值被建议在 1~2MHz，因此我们令 PLL_M = 8，
        即 PLL_VCO input clock = 1MHz */
     PLL_M         =    4;  
    
     /* 到这一步为止，已有 PLL_VCO input clock = 1 MHz.
        PLL_VCO output clock = (PLL_VCO input clock) * PLL_N,
        这个值要用来计算系统时钟，我们 令 PLL_N = 336,
        即 PLL_VCO output clock = 336 MHz.*/       
     PLL_N        =    PLL_N_set;
 
     /* 到这一步为止，已有 PLL_VCO output clock = 336 MHz.
        System Clock = (PLL_VCO output clock)/PLL_P ,
        因为我们要 SystemClock = 168 Mhz，因此令 PLL_P = 2.
        */
     PLL_P         =   PLL_P_set;
 
     /*这个系数用来配置SD卡读写，USB等功能，暂时不用，根据文档，暂时先设为7*/
     PLL_Q         =    7;
    
     /* 配置PLL并将其使能，获得 168Mhz 的 System Clock 时钟*/
     RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
     RCC_PLLCmd(ENABLE);
 
     /*到了这一步，我们已经配置好了PLL时钟。下面我们配置Syetem Clock*/
     /*选择PLL时钟作为系统时钟源*/
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
	
	 /*到了这一步，我们已经配置好了系统时钟，频率为 168MHz. 下面我们可以对 AHB，APB，外设等的 时钟进行配置*/
     /*时钟的结构请参考用户手册*/
 
     /*首先配置 AHB时钟（HCLK）. 为了获得较高的频率，我们对 SYSCLK 1分频，得到HCLK*/
     RCC_HCLKConfig(RCC_HCLK_Div1);
 
     /*APBx时钟（PCLK）由AHB时钟（HCLK）分频得到，下面我们配置 PCLK*/
 
     /*APB1时钟配置. 4分频，即 PCLK1 = 42 MHz*/
     RCC_PCLK1Config(RCC_HCLK_Div4);
 
     /*APB2时钟配置. 2分频，即 PCLK2 = 84 MHz*/
     RCC_PCLK2Config(RCC_HCLK_Div2);
	 
	
}


uint16_t AD7171_Read(void)
{
	uint8_t byte[3] = {0};
	uint8_t state = 0;
	uint8_t err_flag = 0;
	uint8_t i = 0;
	
	uint16_t res;
	
	for(i = 0; i < 5; i++)
	{
		err_flag = 0;
		byte[0] = SPI1_SeRe(0x00);
		byte[1] = SPI1_SeRe(0x00);
		byte[2] = SPI1_SeRe(0x00);
		
		state = byte[2];
		//if(state & 0x80 != 0)
		//	err_flag = 1;
		//if(state & 0x20 != 0)
		//	err_flag = 1;
		//if(state & 0x10 != 0x00)
		//	err_flag = 1;
		if(state & 0x08 != 0x08)
			err_flag = 1;
		if((state & 0x04) != 0x04)
			err_flag = 1;
		if((state & 0x02) != 0x00)
			err_flag = 1;
		if((state & 0x01) != 0x01)
			err_flag = 1;
		if(err_flag == 0)
			break;
	}
	
	if(err_flag == 1)
		res = 0;
	else
	{
		res = ((uint16_t)(byte[0]) << 8) | (uint16_t)(byte[1]);
		res = res - 0x8000;
	}
	
	return res;
	
}

void Buzzer_Frequence_Set(uint8_t frequence)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	if(frequence == 0)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       //GPIOF9  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //复用功能 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //速度50MHz 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉 
		GPIO_Init(GPIOB,&GPIO_InitStructure);  
		TIM_DeInit(TIM4);
	}
	
	else
	{
		
		TIM4_PWM_Init(10000/frequence, 16);
		TIM_SetCompare1(TIM4, 10000/frequence / 50);
		
	}
	
}

void AD7171_Reset_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
}



void Buzzer2_Disable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}


void Buzzer2_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}


void max_min(float * data, uint16_t num)
{
	uint8_t i = 0, j = 0;
	float temp;
	for(i = 0; i < num; i++)
	{
		for(j = i; j < num - 1; j++)
		{
			if(data[j] >  data[j+1])
			{
				temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}
		}
	}
	
}

void Load_tx_buff(void)
{
	uint8_t i = 0;
	uint8_t checksum[4];
	
	tx_buff[0] = 0x12;
	tx_buff[1] = 0x34;
	tx_buff[2] = 0x56;
	tx_buff[3] = 0x78;
	
	tx_buff[4] = (uint8_t)(display_vol >> 24);
	tx_buff[5] = (uint8_t)(display_vol >> 16);
	tx_buff[6] = (uint8_t)(display_vol >> 8);
	tx_buff[7] = (uint8_t)(display_vol >> 0);
	
	for(i = 8; i < 28 - 4; i++)
		tx_buff[i] = 0;
	check_sum_reverse(tx_buff, 0, 28-4-1, checksum);
	tx_buff[24] = checksum[0];
	tx_buff[25] = checksum[1];
	tx_buff[26] = checksum[2];
	tx_buff[27] = checksum[3];
}


void check_sum_reverse(uint8_t * data, uint8_t num_start, uint8_t num_end, uint8_t * res_buff)
{
	uint8_t i = 0;
	uint32_t sum = 0, res = 0;
	
	for(i = num_start; i <= num_end; i++)
		sum += data[i];
	res = ~sum;
	res_buff[0] = (uint8_t)(res >> 24);
	res_buff[1] = (uint8_t)(res >> 16);
	res_buff[2] = (uint8_t)(res >> 8);
	res_buff[3] = (uint8_t)(res >> 0);
	
}






