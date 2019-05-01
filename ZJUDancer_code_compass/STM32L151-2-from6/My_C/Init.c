#include <stm32l1xx.h>

#include <function_conf.h>

signed short magtempxyz[12][3];
uint8_t who[12] = {0};
signed short magxyz_averge[3];

uint8_t systick_over_flag = 0;
uint8_t tx_buff[28] = {0};

int16_t magn_move_average[3];


unsigned short Nop(unsigned short steps)
{
	unsigned short i;
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
	SysTick_Config(16*us);
	while(systick_over_flag == 0);
	systick_over_flag = 0;
}

void Delayms(unsigned int ms)
{
	uint32_t i = 0;
	for(i = 0; i < ms; i++)
		Delayus(1000);
}







void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
}



void TIM2_Init(uint16_t prescaler, uint16_t period)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   //重新将Timer设置为缺省值

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_DeInit(TIM2);//采用内部时钟给TIM2提供时钟源

	TIM_InternalClockConfig(TIM2);//预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;//设置时钟分割
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置计数溢出大小，每计2000个数就产生一个更新事件

	TIM_TimeBaseStructure.TIM_Period = period - 1;//将配置应用到TIM2中
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//清除溢出中断标志
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//禁止ARR预装载缓冲器
	TIM_ARRPreloadConfig(TIM2, ENABLE);//开启TIM2的中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//选择中断分组1
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   //选择TIM2的中断通道   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //使能中断
	NVIC_Init(&NVIC_InitStructure);

}

void TIM3_Init(uint16_t prescaler, uint16_t period)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   //重新将Timer设置为缺省值

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_DeInit(TIM3);//采用内部时钟给TIM2提供时钟源

	TIM_InternalClockConfig(TIM3);//预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;//设置时钟分割
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置计数溢出大小，每计2000个数就产生一个更新事件

	TIM_TimeBaseStructure.TIM_Period = period - 1;//将配置应用到TIM2中
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//清除溢出中断标志
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//禁止ARR预装载缓冲器
	TIM_ARRPreloadConfig(TIM3, ENABLE);//开启TIM2的中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//选择中断分组1
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;   //选择TIM2的中断通道   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //使能中断
	NVIC_Init(&NVIC_InitStructure);

}


void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	USART_Init(USART1, &USART_InitStructure);
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	USART_Cmd(USART1, ENABLE);


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);

	// A2 ?T2X
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// A3 ?R2X
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPIO_PinAFConfig 
	
	
}

void Usart1_Init(unsigned int BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    


	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	//USART1->CR3 |= (1<<6) | (1<<7);
	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	
	//Usart1_Tran(0x00);
}


void Usart1_Tran(char ch)
{
	USART1->DR = ch;
	while(!(USART_GetFlagStatus(USART1, USART_FLAG_TC)));
}

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(!(USART_GetFlagStatus(USART1, USART_FLAG_TC)));//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch; 
	return ch;
}
#endif 



void SPI1_Init()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA ,ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/
	//SPI1_CS1;
	
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
	
}
/*
void SPI1_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
}
*/
unsigned char SPI1_SeRe(unsigned char data)
{
	while(((SPI1->SR) & 0x02) == RESET);
	SPI1->DR = data;
	while(((SPI1->SR) & 0X01) == RESET);
	return SPI1->DR;
}

void SpiPortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI1_Init();
	//SPI2_Init();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB ,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC ,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9  | GPIO_Pin_12  | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SPI1_CS1_1;
	SPI1_CS2_1;
	SPI1_CS3_1;
	SPI1_CS4_1;
	SPI1_CS5_1;
	SPI1_CS6_1;
	SPI1_CS7_1;
	SPI1_CS8_1;
	SPI1_CS9_1;
	SPI1_CS10_1;
	SPI1_CS11_1;
	SPI1_CS12_1;
	
}


void Spi1Cs(unsigned char num, unsigned char state)
{
	if(num == 1)
	{
		if(state == 0)
			SPI1_CS1_0;
		else
			SPI1_CS1_1;
	}
	if(num == 2)
	{
		if(state == 0)
			SPI1_CS2_0;
		else
			SPI1_CS2_1;
	}
	if(num == 3)
	{
		if(state == 0)
			SPI1_CS3_0;
		else
			SPI1_CS3_1;
	}
	if(num == 4)
	{
		if(state == 0)
			SPI1_CS4_0;
		else
			SPI1_CS4_1;
	}
	if(num == 5)
	{
		if(state == 0)
			SPI1_CS5_0;
		else
			SPI1_CS5_1;
	}
	if(num == 6)
	{
		if(state == 0)
			SPI1_CS6_0;
		else
			SPI1_CS6_1;
	}
	if(num == 7)
	{
		if(state == 0)
			SPI1_CS7_0;
		else
			SPI1_CS7_1;
	}
	if(num == 8)
	{
		if(state == 0)
			SPI1_CS8_0;
		else
			SPI1_CS8_1;
	}
	if(num == 9)
	{
		if(state == 0)
			SPI1_CS9_0;
		else
			SPI1_CS9_1;
	}
	if(num == 10)
	{
		if(state == 0)
			SPI1_CS10_0;
		else
			SPI1_CS10_1;
	}
	if(num == 11)
	{
		if(state == 0)
			SPI1_CS11_0;
		else
			SPI1_CS11_1;
	}
	if(num == 12)
	{
		if(state == 0)
			SPI1_CS12_0;
		else
			SPI1_CS12_1;
	}
}


void LsmWriteRegister(unsigned char num, unsigned char address, unsigned char data)
{
	Spi1Cs(num, 0);
	Nop(10);
	SPI1_SeRe(address);
	Nop(10);
	SPI1_SeRe(data);
	Spi1Cs(num, 1);

}

unsigned char LsmReadRegister(unsigned char num, unsigned char address)
{
	unsigned char send_byte, result;
	send_byte = 0x80 | address;
	
	Spi1Cs(num, 0);
	Nop(10);
	SPI1_SeRe(send_byte);
	Nop(10);
	result = SPI1_SeRe(0x00);
	Spi1Cs(num, 1);
	return result;
}

void LsmReadRegisterMultiple(unsigned char num, unsigned char address, unsigned char* buff, unsigned char size)
{
	unsigned char send_byte, i;
	send_byte = 0x80 | 0x40 | address;
	
	Spi1Cs(num, 0);
	Nop(10);
	SPI1_SeRe(send_byte);
	Nop(10);
	for(i = 0; i < size; i++)
		buff[i] = SPI1_SeRe(0x00);
	Spi1Cs(num, 1);
	
}


void LsmInit(void)
{
	unsigned char num = 0;
	for(num = 1; num <= 12; num++)
	{
		LsmWriteRegister(num, 0x1f, 0x00);
		LsmWriteRegister(num, 0x20, 0xa7);
		LsmWriteRegister(num, 0x21, 0x20);
		LsmWriteRegister(num, 0x22, 0x80);
		LsmWriteRegister(num, 0x23, 0x00);
		LsmWriteRegister(num, 0x24, 0xf4);
		LsmWriteRegister(num, 0x25, 0x40);
		LsmWriteRegister(num, 0x26, 0x00);
	}
}

void LsmMagneticReadSingle(unsigned char num, signed short * magxyz)
{
	unsigned char buffa[6];
	LsmReadRegisterMultiple(num, 0x08, buffa, 6);
	magxyz[0] = (signed short)(((unsigned short)buffa[1]) << 8) | ((unsigned short)buffa[0]);
	magxyz[1] = (signed short)(((unsigned short)buffa[3]) << 8) | ((unsigned short)buffa[2]);
	magxyz[2] = (signed short)(((unsigned short)buffa[5]) << 8) | ((unsigned short)buffa[4]);
}

void LsmMagneticReadAverage(signed short * magxyz)
{
	int32_t sum;
	uint8_t i;
	
	LsmMagneticReadSingle(2, magtempxyz[0]);
	/*
	LsmMagneticReadSingle(2, magtempxyz[1]);
	LsmMagneticReadSingle(3, magtempxyz[2]);
	LsmMagneticReadSingle(4, magtempxyz[3]);
	LsmMagneticReadSingle(5, magtempxyz[4]);
	LsmMagneticReadSingle(6, magtempxyz[5]);
	LsmMagneticReadSingle(7, magtempxyz[6]);
	LsmMagneticReadSingle(8, magtempxyz[7]);
	LsmMagneticReadSingle(9, magtempxyz[8]);
	LsmMagneticReadSingle(10, magtempxyz[9]);
	LsmMagneticReadSingle(11, magtempxyz[10]);
	LsmMagneticReadSingle(12, magtempxyz[11]);
	*/
	
	//magxyz[0] = (magtempxyz[0][0] + magtempxyz[1][0] + magtempxyz[2][0] + magtempxyz[3][0]) / 4;
	//magxyz[1] = (magtempxyz[0][1] + magtempxyz[1][1] + magtempxyz[2][1] + magtempxyz[3][1]) / 4;
	//magxyz[2] = (magtempxyz[0][2] + magtempxyz[1][2] + magtempxyz[2][2] + magtempxyz[3][2]) / 4;
	/*
	for(i = 0, sum = 0; i < 11; i++)
		sum += magtempxyz[i][0];
	magxyz_averge[0] = (int16_t)(sum / 11);
	
	for(i = 0, sum = 0; i < 11; i++)
		sum += magtempxyz[i][1];
	magxyz_averge[1] = (int16_t)(sum / 11);
	
	for(i = 0, sum = 0; i < 11; i++)
		sum += magtempxyz[i][2];
	magxyz_averge[2] = (int16_t)(sum / 11);*/
	magxyz_averge[0] = magtempxyz[0][0];
	magxyz_averge[1] = -magtempxyz[0][1];
	magxyz_averge[2] = magtempxyz[0][2];
}

void LsmWhoRead(void)
{
	uint8_t i = 0;
	for(i = 0; i < 12; i++)
		who[i] = LsmReadRegister(i+1, 0x0f);
	
	
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

void Load_tx_buff(void)
{
	uint8_t i = 0;
	uint8_t checksum[4];
	
	tx_buff[0] = 0x12;
	tx_buff[1] = 0x34;
	tx_buff[2] = 0x56;
	tx_buff[3] = 0x78;
	
	tx_buff[4] = (uint8_t)(magxyz_averge[0] >> 8);
	tx_buff[5] = (uint8_t)(magxyz_averge[0]);
	tx_buff[6] = (uint8_t)(magxyz_averge[1] >> 8);
	tx_buff[7] = (uint8_t)(magxyz_averge[1]);
	tx_buff[8] = (uint8_t)(magxyz_averge[2] >> 8);
	tx_buff[9] = (uint8_t)(magxyz_averge[2]);
	
	
	for(i = 10; i < 28 - 4; i++)
		tx_buff[i] = 0;
	check_sum_reverse(tx_buff, 0, 28-4-1, checksum);
	tx_buff[24] = checksum[0];
	tx_buff[25] = checksum[1];
	tx_buff[26] = checksum[2];
	tx_buff[27] = checksum[3];
}


void Usart1_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart1_Tran(str[i]);
	}
}
