
#include <function_conf.h>

unsigned char tim_sec, tim_min, tim_hour;

uint16_t tim_int_counter[13];
uint16_t tim_int_rate[13];

uint8_t systick_over_flag = 0;




//================== TIM3_Init =======================//
//   0 < period_ms < 65535 / 2
void TIM3_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 2;                //周期period_ms
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	    //1ms加1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM3, 1000, TIM_PSCReloadMode_Immediate);*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);    //使能TIM4溢出中断
	
	TIM_Cmd(TIM3, ENABLE);   //使能TIM4
}
//<<<<<<<<<<<<<<<<<<<<<< TIM3_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//




void reflesh_time(void)
{
	tim_sec++;
	if(tim_sec == 60)
	{
		tim_sec = 0;
		tim_min++;
	}
	
	if(tim_min == 60)
	{
		tim_min = 0;
		tim_hour++;
	}	
}


//================== TIM6_Init =======================//
//   0 < period_us < 65535 / 2 us
void TIM6_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period = period_ms*10;                //秒溢出
	TIM_TimeBaseStructure.TIM_Prescaler = (8400 - 1);	    //1us加1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM7, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);    //使能TIM6溢出中断
	
	TIM_Cmd(TIM6, ENABLE);   //使能TIM7
}

//<<<<<<<<<<<<<<<<<<<<<<< TIM6_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//



//================== TIM7_Init =======================//
//   0 < period_us < 65535 / 2 ms
void TIM7_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	TIM_DeInit(TIM7);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 2;    //1秒溢出
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	 //1ms加1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM7, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);    //使能TIM7溢出中断
	
	TIM_Cmd(TIM7, ENABLE);   //不使能TIM7
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM7_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//



//================== TIM12_Init =====wyatt==================//
//   0 < period_us < 65535 / 10 ms
void TIM12_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	TIM_DeInit(TIM12);
	TIM_TimeBaseStructure.TIM_Period = period_ms*10;                //1秒溢出
	TIM_TimeBaseStructure.TIM_Prescaler = (2 * 4200 - 1);	    //1ms加1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM11, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM12, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);    //使能TIM12溢出中断
	
	TIM_Cmd(TIM12, ENABLE);   //使能TIM12
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM12Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//




unsigned short Nop(unsigned short steps)
{
	unsigned int i;
	for(i = 0; i < steps; i++);
	return i;
}

void SysTick_Handler(void)
{
	systick_over_flag = 1;
}

void Delayus(unsigned int us)
{
	systick_over_flag = 0;
	SysTick_Config(168*us);
	while(systick_over_flag == 0);
	systick_over_flag = 0;
}

void Delayms(unsigned int ms)
{
	uint32_t i = 0;
	for(i = 0; i < ms; i++)
		Delayus(1000);
}
