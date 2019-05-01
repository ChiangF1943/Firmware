
#include <function_conf.h>

unsigned char tim_sec, tim_min, tim_hour;

uint16_t tim_int_counter[13];
uint16_t tim_int_rate[13];

uint8_t systick_over_flag = 0;

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


unsigned short Nop(unsigned short steps)
{
	unsigned short i;
	for(i = 0; i < steps; i++);
	return i;
}


//================== TIM2_Init =======================//
//   0 < period_us < 65535^2 / 2
void TIM2_Init(unsigned short period_us)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = period_us * 2;
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM3, 0, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
}
//<<<<<<<<<<<<<<<<<<<< TIM2_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//




//================== TIM3_Init =======================//
//   0 < period_ms < 65535 / 2
void TIM3_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 2;                //����period_ms
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM3, 1000, TIM_PSCReloadMode_Immediate);*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);    //ʹ��TIM4����ж�
	
	TIM_Cmd(TIM3, ENABLE);   //ʹ��TIM4
}

//<<<<<<<<<<<<<<<<<<<<<< TIM3_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//




//================== TIM4_Init =======================//
//   0 < period_ms < 65535 / 84
void TIM4_Init(unsigned short period_us)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = period_us * 84;                //����1000ms
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM4, 1000, TIM_PSCReloadMode_Immediate);*/
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    //ʹ��TIM4����ж�
	
	TIM_Cmd(TIM4, ENABLE);   //ʹ��TIM4
}


//<<<<<<<<<<<<<<<<<<<<<<<< TIM4_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//



//========================= TIM4 ����ΪPWM

void TIM4_IT_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    //ʹ��TIM4����ж�

    TIM_Cmd(TIM4, ENABLE);   //ʹ��TIM4
}
//===================


void TIM4_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_DeInit(TIM4);//��ʼ��TIM2�Ĵ���
	/*��Ƶ�����ڼ��㹫ʽ��
	Prescaler = (TIMxCLK / TIMx counter clock) - 1;
	Period = (TIMx counter clock / TIM3 output clock) - 1 
	TIMx counter clockΪ������Ҫ��TXM�Ķ�ʱ��ʱ�� 
	*/
	TIM_TimeBaseStructure.TIM_Period = arr; //�������ֲ��֪��TIM2��TIM5Ϊ32λ�Զ�װ��
	/*��system_stm32f4xx.c�����õ�APB1 Prescaler = 4 ,��֪
	APB1ʱ��Ϊ168M/4*2,��Ϊ���APB1��Ƶ��Ϊ1����ʱʱ��*2 
	*/
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/*��������Ƚϣ�����ռ�ձ�Ϊ20%��PWM����*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;//����CCR��ռ�ձ���ֵ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//HighΪռ�ձȸ߼��ԣ���ʱռ�ձ�Ϊ20%��Low��Ϊ�����ԣ�ռ�ձ�Ϊ80%
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	//TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	//TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	//TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	//TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	//TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	//TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ARR�Զ�װ��Ĭ���Ǵ򿪵ģ����Բ�����
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE); //ʹ��TIM2��ʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GF9����ΪTIM14 
	//GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	//GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	//GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       //GPIOF9  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //�ٶ�50MHz 

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴����� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //���� 
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9 
}










//================== TIM5_Init =======================//
//   0 < period_ms < 65535 / 2
void TIM5_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 2;                //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM5, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);    //ʹ��TIM5����ж�
	
	TIM_Cmd(TIM5, ENABLE);   //ʹ��TIM5
	
	tim_sec = 0;
	tim_min = 0;
	tim_hour = 0;
}
//<<<<<<<<<<<<<<<<<<<<<<< TIM5_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<//


//========================= TIM5 ����ΪPWM

void TIM5_IT_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);    //ʹ��TIM4����ж�

    TIM_Cmd(TIM5, ENABLE);   //ʹ��TIM4
}
//===================

void TIM5_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_DeInit(TIM5);//��ʼ��TIM2�Ĵ���
	/*��Ƶ�����ڼ��㹫ʽ��
	Prescaler = (TIMxCLK / TIMx counter clock) - 1;
	Period = (TIMx counter clock / TIM3 output clock) - 1 
	TIMx counter clockΪ������Ҫ��TXM�Ķ�ʱ��ʱ�� 
	*/
	TIM_TimeBaseStructure.TIM_Period = arr; //�������ֲ��֪��TIM2��TIM5Ϊ32λ�Զ�װ��
	/*��system_stm32f4xx.c�����õ�APB1 Prescaler = 4 ,��֪
	APB1ʱ��Ϊ168M/4*2,��Ϊ���APB1��Ƶ��Ϊ1����ʱʱ��*2 
	*/
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	/*��������Ƚϣ�����ռ�ձ�Ϊ20%��PWM����*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 200;//����CCR��ռ�ձ���ֵ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//HighΪռ�ձȸ߼��ԣ���ʱռ�ձ�Ϊ20%��Low��Ϊ�����ԣ�ռ�ձ�Ϊ80%
	
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	
	TIM_ARRPreloadConfig(TIM5, ENABLE); //ARR�Զ�װ��Ĭ���Ǵ򿪵ģ����Բ�����
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5, ENABLE); //ʹ��TIM2��ʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GF9����ΪTIM14 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;       //GPIOF9  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //�ٶ�50MHz 

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴����� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //���� 
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9 
}



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


//====================================

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
	TIM_TimeBaseStructure.TIM_Period = period_ms*10;                //�����
	TIM_TimeBaseStructure.TIM_Prescaler = (8400 - 1);	    //1us��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM7, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);    //ʹ��TIM6����ж�
	
	TIM_Cmd(TIM6, ENABLE);   //ʹ��TIM7
	
}

//<<<<<<<<<<<<<<<<<<<<<<< TIM6_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//



//================== TIM7_Init =======================//
//   0 < period_us < 65535 / 2 ms
void TIM7_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	TIM_DeInit(TIM7);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 2;    //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (42000 - 1);	 //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM7, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);    //ʹ��TIM7����ж�
	
	TIM_Cmd(TIM7, ENABLE);   //��ʹ��TIM7
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM7_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//


//================== TIM8_Init =======================//
//   0 < period_us < 65535 / 10 ms
void TIM8_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 1;                //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (168 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM8, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);    //ʹ��TIM8����ж�
	
	TIM_Cmd(TIM8, DISABLE);   //ʹ��TIM8
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM8_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//


//================== TIM9_Init =======================//
//   0 < period_us < 65535 / 10 ms
void TIM9_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	TIM_DeInit(TIM9);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 10;                //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (16800 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM9, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM9, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);    //ʹ��TIM9����ж�
	
	TIM_Cmd(TIM9, ENABLE);   //ʹ��TIM9
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM9_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//



//================== TIM10_Init =======================//
//   0 < period_us < 65535 / 10 ms
void TIM10_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
	TIM_DeInit(TIM10);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 10;                //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (16800 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM10, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM10, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);    //ʹ��TIM8����ж�
	
	TIM_Cmd(TIM10, ENABLE);   //ʹ��TIM10
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM10_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//


//================== TIM11_Init =======================//
//   0 < period_us < 65535 / 10 ms
void TIM11_Init(unsigned short period_ms)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
	TIM_DeInit(TIM11);
	TIM_TimeBaseStructure.TIM_Period = period_ms * 10;         
	TIM_TimeBaseStructure.TIM_Prescaler = (16800 - 1);	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM11, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM11, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM11, TIM_IT_Update, ENABLE);    //ʹ��TIM8����ж�
	
	TIM_Cmd(TIM11, ENABLE);   //ʹ��TIM10
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM111_Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//


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
	TIM_TimeBaseStructure.TIM_Period = period_ms*10;                //1�����
	TIM_TimeBaseStructure.TIM_Prescaler = (2 * 4200 - 1);	    //1ms��1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	//TIM_PrescalerConfig(TIM11, 1000, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM12, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);    //ʹ��TIM12����ж�
	
	TIM_Cmd(TIM12, ENABLE);   //ʹ��TIM12
}
//<<<<<<<<<<<<<<<<<<<<<<<< TIM12Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//




