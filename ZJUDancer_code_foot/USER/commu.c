#include "commu.h"

uint8_t com_flag=COMMU_IDLE;
uint8_t com_buf[20];
uint8_t com_buf_num=0;
uint8_t com_buf_s[20];
uint8_t com_buf_s_num=0;
union sendfloat sf;

void USART1_Init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TXE);
	USART_ClearFlag(USART1, USART_FLAG_RXNE);

	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);	
	
}

void USART1_IRQHandler(void)
{
	uint8_t buf=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //Èô½ÓÊÕÊý¾Ý¼Ä´æÆ÷Âú
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//½ÓÊÕ±ê¼Ç
		buf=USART_ReceiveData(USART1);  //½ÓÊÕÊý¾Ý
		//USART_SendData(USART1,buf);
		commu_get(buf);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)    //Èô·¢ËÍÍê³É
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		//com_flag=COMMU_IDLE;
	}
}

void direction_485_ctrl(uint8_t key)
{
   if(key==1)//delivery
     GPIO_SetBits(GPIOC,GPIO_Pin_4);
   else
     GPIO_ResetBits(GPIOC,GPIO_Pin_4);
//     delay_us(1);
}

uint8_t commu_decode(void)
{
	int i;
	if(com_buf[0]==0x5a)
	{
		if(com_flag==COMMU_GETDATA)
		{
			for(i=0;i<16;i++)
				sf.c[i]=com_buf[i+1];
			return 1;
		}
	}
	return 0;
}

void commu_get(uint8_t buf)
{
	int i;
	uint8_t cksum;
	if(buf==0x5a && com_flag==COMMU_IDLE)
	{
	for(i=0;i<20;i++)//empty the buf
		com_buf[i]=0;
		
		com_flag=COMMU_STARTTRANS;	 //start commu
		com_buf_num=0;
		com_buf[com_buf_num]=buf;
	}
	else if(buf==0xa5)
	{
		com_buf_num++;
		com_buf[com_buf_num]=buf;
		com_buf_num=0;
		cksum=Check_Sum(com_buf,1,16);
		if(com_buf[17]==cksum)
			com_flag=COMMU_GETDATA;	//get something
		else
			com_flag=COMMU_IDLE;	//get something
		if(com_buf[1]==0xAA && com_buf[2]==0x55)
			com_flag=COMMU_GETCOMMAND;//start command
	}
	else
	{
		com_buf_num++;
		com_buf[com_buf_num]=buf;	
	}
}

void commu_start(void)
{
	while(com_flag!=COMMU_IDLE);
	com_buf_s[0]=0x5a;
	com_buf_s[1]=0xAA;
	com_buf_s[2]=0x55;
	com_buf_s[3]=0xa5;
	commu_send(4);
}

void commu_encode(float data0,float data1,float data2,float data3)
{
	int i;
	while(com_flag!=COMMU_IDLE);
	sf.f[0]=data0;
	sf.f[1]=data1;
	sf.f[2]=data2;
	sf.f[3]=data3;
	com_buf_s[0]=0x5a; //head
	for(i=1;i<17;i++) //data
	com_buf_s[i]=sf.c[i-1];
	com_buf_s[17]=Check_Sum(com_buf_s,1,16); 
	com_buf_s[18]=0xa5;//end
	commu_send(19);
}

void commu_send(char num)
{
	int i;
	for(i=0;i<num;i++)
	{
		USART_SendData(USART1,com_buf_s[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    	while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));//等待串口发送完毕
	}
}

unsigned char Check_Sum(uint8_t com[20], unsigned short n1, unsigned short n2)
{
	unsigned char i;
	unsigned char sum = 0;
	for(i = n1; i <= n2; i++)
	{
		sum += com[i];
	}
	sum = ~sum;
	return sum;
}
