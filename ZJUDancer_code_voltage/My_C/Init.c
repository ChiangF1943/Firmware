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
 
    /*����ǰ������RCC����Ϊ��ʼֵ*/
     RCC_DeInit();
 
     /*����ѡ�� �ⲿ����HSE����Ϊ ʱ��Դ��������ȴ��ⲿ����*/
     RCC_HSEConfig(RCC_HSE_ON);
     /*�ȴ��ⲿ��������ȶ�״̬*/
     while( RCC_WaitForHSEStartUp() != SUCCESS );

     /*
     **����Ҫѡ��PLLʱ����Ϊϵͳʱ�ӣ����������Ҫ��PLLʱ�ӽ�������
     */
 
     /*ѡ���ⲿ������ΪPLL��ʱ��Դ*/
    
     /* ����һ��Ϊֹ������ HSE_VALUE = 8 MHz.
        PLL_VCO input clock = (HSE_VALUE or HSI_VALUE / PLL_M)��
        �����ĵ������ֵ�������� 1~2MHz����������� PLL_M = 8��
        �� PLL_VCO input clock = 1MHz */
     PLL_M         =    4;  
    
     /* ����һ��Ϊֹ������ PLL_VCO input clock = 1 MHz.
        PLL_VCO output clock = (PLL_VCO input clock) * PLL_N,
        ���ֵҪ��������ϵͳʱ�ӣ����� �� PLL_N = 336,
        �� PLL_VCO output clock = 336 MHz.*/       
     PLL_N        =    PLL_N_set;
 
     /* ����һ��Ϊֹ������ PLL_VCO output clock = 336 MHz.
        System Clock = (PLL_VCO output clock)/PLL_P ,
        ��Ϊ����Ҫ SystemClock = 168 Mhz������� PLL_P = 2.
        */
     PLL_P         =   PLL_P_set;
 
     /*���ϵ����������SD����д��USB�ȹ��ܣ���ʱ���ã������ĵ�����ʱ����Ϊ7*/
     PLL_Q         =    7;
    
     /* ����PLL������ʹ�ܣ���� 168Mhz �� System Clock ʱ��*/
     RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
     RCC_PLLCmd(ENABLE);
 
     /*������һ���������Ѿ����ú���PLLʱ�ӡ�������������Syetem Clock*/
     /*ѡ��PLLʱ����Ϊϵͳʱ��Դ*/
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
	
	 /*������һ���������Ѿ����ú���ϵͳʱ�ӣ�Ƶ��Ϊ 168MHz. �������ǿ��Զ� AHB��APB������ȵ� ʱ�ӽ�������*/
     /*ʱ�ӵĽṹ��ο��û��ֲ�*/
 
     /*�������� AHBʱ�ӣ�HCLK��. Ϊ�˻�ýϸߵ�Ƶ�ʣ����Ƕ� SYSCLK 1��Ƶ���õ�HCLK*/
     RCC_HCLKConfig(RCC_HCLK_Div1);
 
     /*APBxʱ�ӣ�PCLK����AHBʱ�ӣ�HCLK����Ƶ�õ��������������� PCLK*/
 
     /*APB1ʱ������. 4��Ƶ���� PCLK1 = 42 MHz*/
     RCC_PCLK1Config(RCC_HCLK_Div4);
 
     /*APB2ʱ������. 2��Ƶ���� PCLK2 = 84 MHz*/
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //���ù��� 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //�ٶ�50MHz 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //���� 
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






