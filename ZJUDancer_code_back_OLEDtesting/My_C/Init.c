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
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
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

void Fan_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	FAN1_ON;
	FAN2_ON;

}


void LCD_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LCD_LED_ON;

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

uint32_t check_sum_reverse_to_uint32(uint8_t * data, uint8_t num_start, uint8_t num_end)
{
	uint8_t i = 0;
	uint32_t sum = 0, res = 0;
	
	for(i = num_start; i <= num_end; i++)
		sum += data[i];
	res = ~sum;
	return res;
	
}


uint32_t byte4_to_uint32(uint8_t * byte, uint8_t start_num)
{
	uint32_t res = 0;
	uint32_t resuint32[4];
	resuint32[0] = ((uint32_t)(byte[start_num])) << 24;
	resuint32[1] = ((uint32_t)(byte[start_num+1])) << 16;
	resuint32[2] = ((uint32_t)(byte[start_num+2])) << 8;
	resuint32[3] = ((uint32_t)(byte[start_num+3])) << 0;
	
	res = resuint32[0] | resuint32[1] | resuint32[2] | resuint32[3];
	return res;
}




