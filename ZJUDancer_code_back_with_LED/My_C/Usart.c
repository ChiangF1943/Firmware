
#include <function_conf.h>
#include "stdarg.h"
#include "stdio.h"

void Usart1_Init(unsigned int BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
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

void Usart1_Interp_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void Usart2_Init(unsigned int BaudRate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); 
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
  	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART2, &USART_InitStructure);
  	USART_Cmd(USART2, ENABLE);
  	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
}

void Usart3_Init(unsigned int BaudRate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
  	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART3, &USART_InitStructure);
  	USART_Cmd(USART3, ENABLE);
  	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);
}

void Usart3_Interp_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


void Usart4_Init(unsigned int BaudRate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4); 
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
  	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(UART4, &USART_InitStructure);
  	USART_Cmd(UART4, ENABLE);
  	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ClearFlag(UART4, USART_FLAG_RXNE);
}

void Usart4_Interp_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(UART4, USART_IT_TC, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
}



void Usart5_Init(unsigned int BaudRate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(UART5, &USART_InitStructure);
  	USART_Cmd(UART5, ENABLE);
  	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_ClearFlag(UART5, USART_FLAG_RXNE);
}

void Usart5_Interp_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(UART5, USART_IT_TC, DISABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}



void Usart6_Init(unsigned int BaudRate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); 
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART6, &USART_InitStructure);
  	USART_Cmd(USART6, ENABLE);
  	USART_ClearFlag(USART6, USART_FLAG_TC);
	USART_ClearFlag(USART6, USART_FLAG_RXNE);
}

void Usart6_Interp_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;           //嵌套通道为ADC_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    //响应优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
}


void Usart1_Tran(char ch)
{
	USART1->DR = ch;
	while(!(USART_GetFlagStatus(USART1, USART_FLAG_TC)));
}

void Usart2_Tran(char ch)
{
	USART2->DR = ch;
	while(!(USART_GetFlagStatus(USART2, USART_FLAG_TC)));
}

void Usart3_Tran(char ch)
{
	USART3->DR = ch;
	while(!(USART_GetFlagStatus(USART3, USART_FLAG_TC)));
}

void Usart4_Tran(char ch)
{
	UART4->DR = ch;
	while(!(USART_GetFlagStatus(UART4, USART_FLAG_TC)));
}

void Usart5_Tran(char ch)
{
	UART5->DR = ch;
	while(!(USART_GetFlagStatus(UART5, USART_FLAG_TC)));
}


void Usart6_Tran(char ch)
{
	USART6->DR = ch;
	while(!(USART_GetFlagStatus(USART6, USART_FLAG_TC)));
}


unsigned char Usart1_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)));
	a = USART1->DR;
	return a;
}


unsigned char Usart2_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)));
	a = USART2->DR;
	return a;
}

unsigned char Usart3_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)));
	a = USART3->DR;
	return a;
}

unsigned char Usart4_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(UART4, USART_FLAG_RXNE)));
	a = UART4->DR;
	return a;
}

unsigned char Usart5_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(UART5, USART_FLAG_RXNE)));
	a = UART5->DR;
	return a;
}


unsigned char Usart6_Rece(void)
{
	unsigned char a;
	while(!(USART_GetFlagStatus(USART6, USART_FLAG_RXNE)));
	a = USART6->DR;
	return a;
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
	while(!(USART_GetFlagStatus(USART6, USART_FLAG_TC)));//循环发送,直到发送完毕   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif 

void Usart1_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart1_Tran(str[i]);
	}
}

void Usart2_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart2_Tran(str[i]);
	}
}

void Usart3_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart3_Tran(str[i]);
	}
}

void Usart4_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart4_Tran(str[i]);
	}
}

void Usart5_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart5_Tran(str[i]);
	}
}

void Usart6_Tran_Str(unsigned char * str, unsigned short count)
{
	unsigned short i = 0;
	for(i = 0; i < count; i++)
	{
		Usart6_Tran(str[i]);
	}
}



int printf_ur6(const char *fmt, ...)	
{
	char s[100];
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);
	Usart6_Tran_Str((unsigned char *)s, cnt);
	return(cnt);
}


