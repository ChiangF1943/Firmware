
#include <stm32f4xx.h>
#include <function_conf.h>


void SPI1_Init()
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI1_CS1;
	
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
	
}

void SPI1_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
}

unsigned char SPI1_SeRe(unsigned char data)
{
	while(((SPI1->SR) & 0x02) == RESET);
	SPI1->DR = data;
	while(((SPI1->SR) & 0X01) == RESET);
	return SPI1->DR;   
}

void SPI2_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI2_CS1;
	
  	SPI_I2S_DeInit(SPI2);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  	SPI_Init(SPI2, &SPI_InitStructure);
  	SPI_Cmd(SPI2, ENABLE);
}

void SPI2_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
}

unsigned char SPI2_SeRe(unsigned char data)
{
	while(((SPI2->SR) & 0x02) == 0);
	SPI2->DR = data;
	while(((SPI2->SR) & 0X01) == 0);
	return SPI2->DR;   
}

void SPI3_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SPI3_CS1;
	
  	SPI_I2S_DeInit(SPI3);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//数据采样从第1个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  	SPI_Init(SPI3, &SPI_InitStructure);
  	SPI_Cmd(SPI3, ENABLE);
}

void SPI3_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
}


unsigned char SPI3_SeRe(unsigned char data)
{
	while(((SPI3->SR) & 0x02) == RESET);
	SPI3->DR = data;
	while(((SPI3->SR) & 0X01) == RESET);
	return SPI3->DR; 
}

