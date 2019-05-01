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
	
	tx_buff[4] = (uint8_t)(voltage_uint32 >> 24);
	tx_buff[5] = (uint8_t)(voltage_uint32 >> 16);
	tx_buff[6] = (uint8_t)(voltage_uint32 >> 8);
	tx_buff[7] = (uint8_t)(voltage_uint32 >> 0);
	
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


void Bus_485_Con_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	
}

unsigned char Check_Sum(unsigned char * coms, unsigned short n1, unsigned short n2)
{
	unsigned char i;
	unsigned char sum = 0;
	for(i = n1; i <= n2; i++)
	{
		sum += coms[i];
	}
	sum = ~sum;
	return sum;
}

//============================
uint16_t team_1_postion[20] = {0};
uint16_t team_2_postion[20] = {0};
uint16_t team_3_postion[20] = {0};

uint8_t team_1_id[20] = {0};
uint8_t team_2_id[20] = {0};
uint8_t team_3_id[20] = {0};

uint8_t team_1_id_count;
uint8_t team_2_id_count;
uint8_t team_3_id_count;

void Motor_Buff_Init(void)
{

	team_1_id_count = 0;
	team_2_id_count = 0;
	team_3_id_count = 0;
	
	
	team_1_id[team_1_id_count++] = 1;
	team_1_id[team_1_id_count++] = 2;
	team_1_id[team_1_id_count++] = 3;
	team_1_id[team_1_id_count++] = 4;
	team_1_id[team_1_id_count++] = 5;
	team_1_id[team_1_id_count++] = 6;
	
	team_2_id[team_2_id_count++] = 7;
	team_2_id[team_2_id_count++] = 8;
	team_2_id[team_2_id_count++] = 9;
	team_2_id[team_2_id_count++] = 10;
	team_2_id[team_2_id_count++] = 11;
	team_2_id[team_2_id_count++] = 12;
	team_2_id[team_2_id_count++] = 13;
	
	team_3_id[team_3_id_count++] = 14;
	team_3_id[team_3_id_count++] = 15;
	team_3_id[team_3_id_count++] = 16;
	team_3_id[team_3_id_count++] = 17;
	team_3_id[team_3_id_count++] = 18;
	team_3_id[team_3_id_count++] = 19;
	team_3_id[team_3_id_count++] = 20;
	
}



uint8_t Motor_Command_String_Get(uint8_t id, uint8_t address, uint8_t * data, uint8_t data_length, uint8_t * res_buff)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t check_sum_temp = 0;
	
	res_buff[i++] = 0xff;
	res_buff[i++] = 0xff;
	res_buff[i++] = id;
	res_buff[i++] = 0x03 + data_length;   //length
	res_buff[i++] = INS_REG_WRITE;
	res_buff[i++] = address;
	for(j = 0; j < data_length; j++)
		res_buff[i++] = data[j];
	
	check_sum_temp = Check_Sum(res_buff, 2, i-1);
	res_buff[i++] = check_sum_temp;
	
	return i;
}

uint8_t Motor_Command_String_Get_Uint16(uint8_t id, uint8_t address, uint16_t data, uint8_t * res_buff)
{
	uint8_t i = 0;
	
	uint8_t check_sum_temp = 0;
	
	res_buff[i++] = 0xff;
	res_buff[i++] = 0xff;
	res_buff[i++] = id;
	res_buff[i++] = 0x05;   //length
	res_buff[i++] = INS_REG_WRITE;
	res_buff[i++] = address;
	
	res_buff[i++] = (uint8_t)(data & 0xff);
	res_buff[i++] = (uint8_t)(data >> 8);
	
	check_sum_temp = Check_Sum(res_buff, 2, i-1);
	res_buff[i++] = check_sum_temp;
	
	return i;
}

uint8_t Motor_Command_String_Get_Uint8(uint8_t id, uint8_t address, uint8_t data, uint8_t * res_buff)
{
	uint8_t i = 0;
	
	uint8_t check_sum_temp = 0;
	
	res_buff[i++] = 0xff;
	res_buff[i++] = 0xff;
	res_buff[i++] = id;
	res_buff[i++] = 0x04;   //length
	res_buff[i++] = INS_REG_WRITE;
	res_buff[i++] = address;
	
	res_buff[i++] = data;
	
	check_sum_temp = Check_Sum(res_buff, 2, i-1);
	res_buff[i++] = check_sum_temp;
	
	return i;
}

uint8_t Motor_Command_String_Get_Action(uint8_t id, uint8_t * res_buff)
{
	uint8_t i = 0;
	
	uint8_t check_sum_temp = 0;
	
	res_buff[i++] = 0xff;
	res_buff[i++] = 0xff;
	res_buff[i++] = id;
	res_buff[i++] = 0x02;   //length
	res_buff[i++] = INS_ACTION;

	check_sum_temp = Check_Sum(res_buff, 2, i-1);
	res_buff[i++] = check_sum_temp;
	
	return i;
}

void Usart_234_Tran(uint8_t * buff1, uint8_t * buff2, uint8_t * buff3, uint8_t len1)
{
	uint8_t i = 0;
	for(i = 0; i < len1; i++)
	{
		USART2->DR = buff1[i];
		USART3->DR = buff2[i];
		UART4->DR = buff3[i];
		while(!(USART_GetFlagStatus(USART2, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(USART3, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(UART4, USART_FLAG_TC)));
	}
}


void Usart_234_Tran_Send(uint8_t * buff, uint8_t len)
{
	uint8_t i = 0;
	
	B485_1_S;
	B485_2_S;
	B485_3_S;
	Delayus(1);
	
	
	for(i = 0; i < 2; i++)
	{
		USART2->DR = 0x00;
		USART3->DR = 0x00;
		UART4->DR = 0x00;
		while(!(USART_GetFlagStatus(USART2, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(USART3, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(UART4, USART_FLAG_TC)));
	}
	
	for(i = 0; i < len; i++)
	{
		USART2->DR = buff[i];
		USART3->DR = buff[i];
		UART4->DR = buff[i];
		while(!(USART_GetFlagStatus(USART2, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(USART3, USART_FLAG_TC)));
		while(!(USART_GetFlagStatus(UART4, USART_FLAG_TC)));
	}
	Delayus(1);
	B485_1_R;
	B485_2_R;
	B485_3_R;
}


void Motor_V0_Torque(uint8_t id, uint8_t state)
{
    //uint8_t k;
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x04; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]=Torque_Enable;  //address
    com[6]=state;
    com[7]=Check_Sum(com,2,6);

    Usart_234_Tran_Send(com, 8);
	
}

void Motor_V0_LED(uint8_t id, uint8_t state)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x04; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]=LED;  //address
    com[6]=state;
    com[7]=Check_Sum(com,2,6);

    Usart_234_Tran_Send(com, 8);
	
}

void Motor_V0_P_Gain(uint8_t id, uint8_t state)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x04; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]=P_Gain;  //address
    com[6]=state;
    com[7]=Check_Sum(com,2,6);

    Usart_234_Tran_Send(com, 8);
	
}


void Motor_V0_I_Gain(uint8_t id, uint8_t state)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x04; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]=I_Gain;  //address
    com[6]=state;
    com[7]=Check_Sum(com,2,6);

    Usart_234_Tran_Send(com, 8);
	
}


void Motor_V0_D_Gain(uint8_t id, uint8_t state)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x04; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]=D_Gain;  //address
    com[6]=state;
    com[7]=Check_Sum(com,2,6);

    Usart_234_Tran_Send(com, 8);
	
}


void Motor_V0_Position(uint8_t id, uint16_t position)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x05; //length
    com[4]=INS_REG_WRITE; //instruction
    com[5]= Goal_Position_L;  //address
    com[6] = (uint8_t)(position & 0xff);
	com[7] = (uint8_t)(position >> 8);
    com[8]=Check_Sum(com,2,7);

    Usart_234_Tran_Send(com, 9);
	
}


void Motor_V0_Action(uint8_t id)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x02; //length
    com[4]=INS_ACTION; //instruction
    com[5]=Check_Sum(com,2,4);

    Usart_234_Tran_Send(com, 6);
	
}


void Motor_V0_Speed_Limit(uint8_t id, uint16_t speed_limit)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x05; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]= Moving_Speed_L;  //address
    com[6] = (uint8_t)(speed_limit & 0xff);
	com[7] = (uint8_t)(speed_limit >> 8);
    com[8]=Check_Sum(com,2,7);
	
    Usart_234_Tran_Send(com, 9);
	
}


void Motor_V0_Torque_Limit(uint8_t id, uint16_t torque_limit)
{
	uint8_t com[12] = {0};
	
	com[0] = 0xff;
	com[1] = 0xff;
    com[2]=id;
    com[3]=0x05; //length
    com[4]=INS_WRITE_DATA; //instruction
    com[5]= Torque_Limit_L;  //address
    com[6] = (uint8_t)(torque_limit & 0xff);
	com[7] = (uint8_t)(torque_limit >> 8);
    com[8]=Check_Sum(com,2,7);
	
    Usart_234_Tran_Send(com, 9);
	
}

uint8_t Motor_V0_LED_Com_Get(uint8_t id, uint8_t state, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x04; //length
    com_buff[4]=INS_WRITE_DATA; //instruction
    com_buff[5]=LED;  //address
    com_buff[6]=state;
    com_buff[7]=Check_Sum(com_buff,2,6);
	com_buff[8]=0xaa;
	return 9;
}

uint8_t Motor_V0_Position_Com_Get(uint8_t id, uint16_t position, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x05; //length
    com_buff[4]=INS_REG_WRITE; //instruction
    com_buff[5]= Goal_Position_L;  //address
    com_buff[6] = (uint8_t)(position & 0xff);
	com_buff[7] = (uint8_t)(position >> 8);
    com_buff[8]=Check_Sum(com_buff,2,7);
	com_buff[9]=0xaa;
	return 10;
}

uint8_t Motor_V0_Action_Com_Get(uint8_t id,  uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x02; //length
    com_buff[4]=INS_ACTION; //instruction
    com_buff[5]=Check_Sum(com_buff,2,4);
	com_buff[6]=0xaa;
	return 7;
}
uint8_t Motor_V0_Torque_Enable_Com_Get(uint8_t id, uint8_t state, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x04; //length
    com_buff[4]=INS_WRITE_DATA; //instruction
    com_buff[5]=Torque_Enable;  //address
    com_buff[6]=state;
    com_buff[7]=Check_Sum(com_buff,2,6);
	com_buff[8]=0xaa;
	return 9;
}
uint8_t Motor_V0_Speed_Limit_Com_Get(uint8_t id, uint16_t Speed_Limit, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x05; //length
    com_buff[4]=INS_WRITE_DATA; //instruction
    com_buff[5]= Moving_Speed_L;  //address
    com_buff[6] = (uint8_t)(Speed_Limit & 0xff);
	com_buff[7] = (uint8_t)(Speed_Limit >> 8);
    com_buff[8]=Check_Sum(com_buff,2,7);
	com_buff[9]=0xaa;
	return 10;
}

uint8_t Motor_V0_Torque_Limit_Com_Get(uint8_t id, uint16_t Torque_Limit, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x05; //length
    com_buff[4]=INS_WRITE_DATA; //instruction
    com_buff[5]= Torque_Limit_L;  //address
    com_buff[6] = (uint8_t)(Torque_Limit & 0xff);
	com_buff[7] = (uint8_t)(Torque_Limit >> 8);
    com_buff[8]=Check_Sum(com_buff,2,7);
	com_buff[9]=0xaa;
	return 10;
}

void Reset_uart2_tx_buff(void)
{
	buff_u2_load_pointer = 0;
	buff_u2_len = 0;
}
void Reset_uart3_tx_buff(void)
{
	buff_u3_load_pointer = 0;
	buff_u3_len = 0;
}
void Reset_uart4_tx_buff(void)
{
	buff_u4_load_pointer = 0;
	buff_u4_len = 0;
}



void Load_uart2_tx_buff(uint8_t * data, uint16_t num)
{
	uint16_t i = 0;
	for(i = 0; i < num; i++)
		uart2_tx_buff[buff_u2_load_pointer++] = data[i];
}
void Load_uart3_tx_buff(uint8_t * data, uint16_t num)
{
	uint16_t i = 0;
	for(i = 0; i < num; i++)
		uart3_tx_buff[buff_u3_load_pointer++] = data[i];
}
void Load_uart4_tx_buff(uint8_t * data, uint16_t num)
{
	uint16_t i = 0;
	for(i = 0; i < num; i++)
		uart4_tx_buff[buff_u4_load_pointer++] = data[i];
}



void Start_u2_tran(void)
{
	USART2->DR = uart2_tx_buff[0];
	//USART6->DR = tx_buff_u1[0];
}
void Start_u3_tran(void)
{
	USART3->DR = uart3_tx_buff[0];
	//USART6->DR = tx_buff_u1[0];
}
void Start_u4_tran(void)
{
	UART4->DR = uart4_tx_buff[0];
	//USART6->DR = tx_buff_u1[0];
}


void Motor_Led_Load_Uart2()
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_LED_Com_Get(id_buff[i], led_buff[i], com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Led_Load_Uart3()
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_LED_Com_Get(id_buff[i], led_buff[i], com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Led_Load_Uart4()
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_LED_Com_Get(id_buff[i], led_buff[i], com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}


void Motor_Position_Load_Uart2(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Position_Com_Get(id_buff[i], position_buff[i], com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Position_Load_Uart3(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Position_Com_Get(id_buff[i], position_buff[i], com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Position_Load_Uart4(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Position_Com_Get(id_buff[i], position_buff[i], com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}

void Motor_Action_Load_Uart2(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 1; i++)
	{
		com_len = Motor_V0_Action_Com_Get(ID_ALL, com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Action_Load_Uart3(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 1; i++)
	{
		com_len = Motor_V0_Action_Com_Get(ID_ALL, com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Action_Load_Uart4(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 1; i++)
	{
		com_len = Motor_V0_Action_Com_Get(ID_ALL, com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}


void Motor_Torque_Enable_Load_Uart2(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Enable_Com_Get(id_buff[i], torque_enable_buff[i], com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Torque_Enable_Load_Uart3(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Enable_Com_Get(id_buff[i], torque_enable_buff[i], com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Torque_Enable_Load_Uart4(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Enable_Com_Get(id_buff[i], torque_enable_buff[i], com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}


//Motor_V0_Speed_Limit();
void Motor_Speed_Limit_Load_Uart2(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Speed_Limit_Com_Get(id_buff[i], speed_limit_buff[i], com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Speed_Limit_Load_Uart3(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Speed_Limit_Com_Get(id_buff[i], speed_limit_buff[i], com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Speed_Limit_Load_Uart4(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Speed_Limit_Com_Get(id_buff[i], speed_limit_buff[i], com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}


void Motor_Torque_Limit_Load_Uart2(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Limit_Com_Get(id_buff[i], torque_limit_buff[i], com_buff_temp);
		Load_uart2_tx_buff(com_buff_temp, com_len);
		buff_u2_len += com_len;
	}
}
void Motor_Torque_Limit_Load_Uart3(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Limit_Com_Get(id_buff[i], torque_limit_buff[i], com_buff_temp);
		Load_uart3_tx_buff(com_buff_temp, com_len);
		buff_u3_len += com_len;
	}
}
void Motor_Torque_Limit_Load_Uart4(void)
{
	uint8_t i;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	for(i = 0; i < 20; i++)
	{
		com_len = Motor_V0_Torque_Limit_Com_Get(id_buff[i], torque_limit_buff[i], com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
	}
}




void Motor_Go_Position_All_One_Bus(void)
{
	uint8_t i = 0;
	uint16_t temp_buff[20] = {0};
	
	for(i = 0; i < 20; i++)
		temp_buff[i] =  position_buff[i];
	
	for(i = 0; i < 20; i++)
		Motor_V0_Position(id_buff[i], temp_buff[i]);
}


void Motor_Torque_Limit_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Torque_Limit(id_buff[i], torque_limit_buff[i]);
}


void Motor_Speed_Limit_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Speed_Limit(id_buff[i], speed_limit_buff[i]);
}


void Motor_Led_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_LED(id_buff[i], led_buff[i]);
}


void Motor_Torque_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Torque(id_buff[i], torque_enable_buff[i]);
}


void Motor_P_Gain_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Torque(id_buff[i], p_gain_buff[i]);
}


void Motor_I_Gain_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Torque(id_buff[i], i_gain_buff[i]);
}


void Motor_D_Gain_All_One_Bus(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		Motor_V0_Torque(id_buff[i], d_gain_buff[i]);
}


void DMA_Uart2_Tx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure ;
	/*首先开DMA2时钟，由407参考手册-RM0090-Reference manual
	165页可知，UASRT6与DMA2映射，而且DMA2挂载在AHB1时钟总线上*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	/*由RM0090-Reference manual第165页映射表可知，USART6映射在
	Channel_5的Stream6和Stream7上，在这里可以选择Stream6 */ 
	DMA_DeInit(DMA1_Stream6);
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; //选择Channel_5
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART2_DR_Addr; //数据传输的外设首地址，详解见上
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart2_tx_buff; //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize = 200; //传输数据大小为8，单位由以下确定，大小要配合定义的数组类型和外设数据类型
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //这里也选Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High
	/*双缓冲模式，在DMA_Init之前调用在Circular模式有效，会强制Circular，
	*不支持Memory toMemory，(uint32_t)Buffer2为DMA_Memory_1，DMA先将Buffer
	*中的数据发送完毕后在发送Buffer2的数据，当然顺序可以改变
	DMA_DoubleBufferModeConfig(DMA2_Stream6, (uint32_t)Buffer2, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream6, ENABLE);
	*/
	//指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式    
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	//指定了FIFO阈值水平  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	//指定的Burst转移配置内存传输   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	//指定的Burst转移配置外围转移 

	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	//DMA_Cmd(DMA1_Stream6, ENABLE); //使能DMA2_Stream6通道
	/*DMA中断开*/
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, DISABLE);
	//USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	/*DMA中断开*/
	/*
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;  
	NVIC_Init(&NVIC_InitStructure); 
	*/
	//DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream6, DISABLE); //使能DMA2_Stream6通道
	USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);

}


void DMA1_Stream6_IRQHandler(void)  
{
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET)   
	{  
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);			
		DMA_Cmd(DMA1_Stream6,DISABLE);
	}
}



void DMA_Uart3_Tx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream3);
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; //选择Channel_5
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART3_DR_Addr; //数据传输的外设首地址，详解见上
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart3_tx_buff; //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize = 8; //传输数据大小为8，单位由以下确定，大小要配合定义的数组类型和外设数据类型
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //这里也选Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	//指定了FIFO阈值水平  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	//指定的Burst转移配置内存传输   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	//指定的Burst转移配置外围转移 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, DISABLE);
	DMA_Cmd(DMA1_Stream3, DISABLE); //使能DMA2_Stream6通道
	USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
	DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);

}


void DMA1_Stream3_IRQHandler(void)  
{
	if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET)   
	{  
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);			
		DMA_Cmd(DMA1_Stream3,DISABLE);
	}
}



void DMA_Uart4_Tx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Stream4);
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; //选择Channel_5
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART4_DR_Addr; //数据传输的外设首地址，详解见上
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart4_tx_buff; //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize = 8; //传输数据大小为8，单位由以下确定，大小要配合定义的数组类型和外设数据类型
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //这里也选Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	//指定了FIFO阈值水平  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	//指定的Burst转移配置内存传输   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	//指定的Burst转移配置外围转移 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, DISABLE);
	DMA_Cmd(DMA1_Stream4, DISABLE); //使能DMA2_Stream6通道
	USART_DMACmd(UART4, USART_DMAReq_Tx, DISABLE);
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);

}


void DMA1_Stream4_IRQHandler(void)  
{
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4) != RESET)   
	{  
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);			
		DMA_Cmd(DMA1_Stream4,DISABLE);
	}
}


void DMA_Uart6_Tx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream6);
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5; //选择Channel_5
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART6_DR_Addr; //数据传输的外设首地址，详解见上
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)printf_buff; //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize = 8; //传输数据大小为8，单位由以下确定，大小要配合定义的数组类型和外设数据类型
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //这里也选Byte
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	//指定了FIFO阈值水平  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	//指定的Burst转移配置内存传输   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	//指定的Burst转移配置外围转移 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream6, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream6, DMA_IT_TC, DISABLE);
	DMA_Cmd(DMA2_Stream6, DISABLE); //使能DMA2_Stream6通道
	USART_DMACmd(USART6, USART_DMAReq_Tx, DISABLE);
	DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF4);

}


void DMA2_Stream6_IRQHandler(void)  
{
	if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)   
	{  
		DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);			
		DMA_Cmd(DMA2_Stream6,DISABLE);
	}
}

void DMA_Uart2_Tx_Start(void)
{
	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); 
	DMA_SetCurrDataCounter(DMA1_Stream6, buff_u2_len);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Stream6, ENABLE); 
}
void DMA_Uart3_Tx_Start(void)
{
	DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3); 
	DMA_SetCurrDataCounter(DMA1_Stream3, buff_u3_len);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Stream3, ENABLE); 
}
void DMA_Uart4_Tx_Start(void)
{
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4); 
	DMA_SetCurrDataCounter(DMA1_Stream4, buff_u4_len);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Stream4, ENABLE); 
}



void Set_Motor_Torque_Enable_All(uint8_t state)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		torque_enable_buff[i] = state;
}


void Set_Motor_Led_All(uint8_t state)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		led_buff[i] = state;
}


void Set_Motor_Speed_Limit(uint16_t speed_limit)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		speed_limit_buff[i] = speed_limit;
}


void Set_Motor_Torque_Limit(uint16_t torque_limit)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		torque_limit_buff[i] = torque_limit;
}

void Set_Motor_Position(uint16_t position)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
		position_buff[i] = position;
}


void Motor_Present_Position_Load_Uart4(void)
{
	uint8_t id = 22;
	uint8_t com_buff_temp[20] = {0};
	uint8_t com_len = 0;
	
		com_len = Motor_Current_Position_Com_Get(id, com_buff_temp);
		Load_uart4_tx_buff(com_buff_temp, com_len);
		buff_u4_len += com_len;
}

uint8_t Motor_Current_Position_Com_Get(uint8_t id, uint8_t * com_buff)
{
	com_buff[0] = 0xff;
	com_buff[1] = 0xff;
    com_buff[2]=id;
    com_buff[3]=0x04; //length
    com_buff[4]=0x02; //instruction
    com_buff[5]=0x24;  //address
    com_buff[6]=0x02;
    com_buff[7]=Check_Sum(com_buff,2,6);
	com_buff[8]=0xaa;
	return 9;
}