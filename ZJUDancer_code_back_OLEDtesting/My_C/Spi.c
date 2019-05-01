
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
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����ģʽ
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ģʽ��SCKΪ1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//���ݲ����ӵ�2��ʱ����ؿ�ʼ
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�������
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//������
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ģʽ
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC����ʽ
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
	
}

void SPI1_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;           //Ƕ��ͨ��ΪADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //��Ӧ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
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
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����ģʽ
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ģʽ��SCKΪ1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//���ݲ����ӵ�2��ʱ����ؿ�ʼ
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�������
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//������
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ģʽ
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC����ʽ
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
  	SPI_Init(SPI2, &SPI_InitStructure);
  	SPI_Cmd(SPI2, ENABLE);
}

void SPI2_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;           //Ƕ��ͨ��ΪADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //��Ӧ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
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
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����ģʽ
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//����ģʽ��SCKΪ1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//���ݲ����ӵ�1��ʱ����ؿ�ʼ
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�������
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//������
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ģʽ
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC����ʽ
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
  	SPI_Init(SPI3, &SPI_InitStructure);
  	SPI_Cmd(SPI3, ENABLE);
}

void SPI3_Interp_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;           //Ƕ��ͨ��ΪADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //��Ӧ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}


unsigned char SPI3_SeRe(unsigned char data)
{
	while(((SPI3->SR) & 0x02) == RESET);
	SPI3->DR = data;
	while(((SPI3->SR) & 0X01) == RESET);
	return SPI3->DR; 
}

