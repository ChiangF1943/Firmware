#include "commu.h"
#define com_length 20
#define ID 0x18
uint8_t com_flag=COMMU_IDLE;
uint8_t com_buf[com_length];
uint8_t com_buf_num=0;
uint8_t com_buf_s[23];
uint8_t com_buf_s_num=0;
uint8_t length = 0;
uint8_t start_read = 0;
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);	
	
}

void USART1_IRQHandler(void)
{
	uint8_t buf=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//
		buf=USART_ReceiveData(USART1);  
		//USART_SendData(USART1,buf);
		commu_get(buf);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)    
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
		//delay_us(1);
}

//uint8_t commu_decode(void)
//{
//	int i;
//	if(com_buf[0]==0xff&&com_buf[1]==0xff&&com_buf[2]==0x30&&com_buf[3]==0x12&&com_buf[4]==0x00)
//	{
//		if(com_flag==COMMU_GETDATA)
//		{
//			for(i=5;i<21;i++)
//				sf.c[i]=com_buf[i];
//			return 1;
//		}
//	}
//	return 0;
//}

void commu_get(uint8_t buf)
{
	int i;
	uint8_t cksum;
//	if((buf == 0xff&&com_buf_num==0)||(buf == 0xff&& com_buf_num==1)||(buf == ID&& com_buf_num==2))
//	{
//		com_buf[com_buf_num++]=buf;
//	}
//	else if(com_buf_num==3){
//		length=buf;
//		com_buf[com_buf_num++]=buf;
//		start_read = 1;
//	}
//	else if(com_buf_num>3&&start_read == 1){
//			if(com_buf_num>(3+length)){
//				start_read=0;
//				com_buf_num=0;
//				for (i=0;i<com_length;i++)
//					com_buf[i]=0;
//			}
//			else if(com_buf_num==3+length)
//			{
//				com_buf[com_buf_num]=buf;
//				cksum=Check_Sum(com_buf,2,com_buf_num-1);
//				if(cksum==com_buf[com_buf_num]){
//					com_flag=COMMU_GETDATA;
//				}
//				else 
//				{
//				com_buf_num=0;
//				for (i=0;i<com_length;i++)
//					com_buf[i]=0;
//				start_read = 0;
//				}
//				
//			}
//			else
//				com_buf[com_buf_num++]=buf;
//	}
//	else {
//		com_buf_num=0;
//		for (i=0;i<com_length;i++)
//			com_buf[i]=0;
//		start_read = 0;
//	}
//	
//	

	if(buf==ID&&com_flag==COMMU_IDLE&&com_buf[com_buf_num-1]==0xff&&com_buf[com_buf_num-2]==0xff){
		for(i=3;i<=20;i++)//empty the buf
			com_buf[i]=0;
		
		com_flag=COMMU_STARTTRANS;	 //start commu
		com_buf_num=2;
		com_buf[2]=buf;
		com_buf[1]=0xff;
		com_buf[0]=0xff;
		com_buf_num++;
	}
	
	else if(buf==0xaa)
	{
		com_buf[com_buf_num]=buf;
		com_buf_num++;
		cksum=Check_Sum(com_buf,2,com_buf_num-3);
		if(com_buf[com_buf_num-2]==cksum&&com_buf[3]==(com_buf_num-5)&&com_buf[2]==ID)
		{	com_flag=COMMU_GETDATA;	
			com_buf_num=0;
		}			//get something
		else
			com_flag=COMMU_IDLE;	//get something
		if(com_buf[1]==0xAA && com_buf[2]==0x55)
			com_flag=COMMU_GETCOMMAND;//start command	
	}
	else
	{
		com_buf[com_buf_num]=buf;
		com_buf_num++;
		com_buf_num = com_buf_num%20;
	}

}
//void commu_start(void)
//{
//	while(com_flag!=COMMU_IDLE);
//	com_buf_s[0]=0x5a;
//	com_buf_s[1]=0xAA;
//	com_buf_s[2]=0x55;
//	com_buf_s[3]=0xa5;
//	commu_send(4);
//}

void commu_encode(float data0,float data1,float data2,float data3)
{
	int i;
	while(com_flag!=COMMU_IDLE);
	sf.f[0]=data0;
	sf.f[1]=data1;
	sf.f[2]=data2;
	sf.f[3]=data3;
	com_buf_s[0]=0xff; //head
	com_buf_s[1]=0xff;
	com_buf_s[2]=ID;
	com_buf_s[3]=0x12;
	com_buf_s[4]=0x00;//err
	for(i=5;i<21;i++) //data
		com_buf_s[i]=sf.c[i-5];
	com_buf_s[21]=Check_Sum(com_buf_s,3,18); 
	com_buf_s[22]=0xaa;//end
	commu_send(23);
}

void commu_send(char num)
{
	int i;
	for(i=0;i<num;i++)
	{
		USART_SendData(USART1,com_buf_s[i]);
		while((USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)){};
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
