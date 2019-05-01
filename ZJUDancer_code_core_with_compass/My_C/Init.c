#include <stm32f4xx.h>

#include <function_conf.h>


unsigned int cpu_count = 0;
unsigned char  cpu_count_flag = 0;
float cpu_usage = 0;
unsigned char magnraw_LSM[6];



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

void Load_tx_buff_u4(void)
{
	uint8_t i = 0;
	uint8_t checksum[4];
	
	tx_buff_u4[0] = 0x12;
	tx_buff_u4[1] = 0x34;
	tx_buff_u4[2] = 0x56;
	tx_buff_u4[3] = 0x78;
	
	tx_buff_u4[4] = (uint8_t)(voltage_uint32 >> 24);
	tx_buff_u4[5] = (uint8_t)(voltage_uint32 >> 16);
	tx_buff_u4[6] = (uint8_t)(voltage_uint32 >> 8);
	tx_buff_u4[7] = (uint8_t)(voltage_uint32 >> 0);
	
	for(i = 8; i < 28 - 4; i++)
		tx_buff_u4[i] = 0;
	check_sum_reverse(tx_buff_u4, 0, 28-4-1, checksum);
	tx_buff_u4[24] = checksum[0];
	tx_buff_u4[25] = checksum[1];
	tx_buff_u4[26] = checksum[2];
	tx_buff_u4[27] = checksum[3];
	
	buff_u4_len = 28;
}

void Start_u4_tran(void)
{
	UART4->DR = tx_buff_u4[0];
}

void Load_tx_buff_u2(uint32_t type)
{
	uint8_t i = 0, j = 0;
	uint8_t checksum[4];
	uint16_t * buff_pointer;
	
	tx_buff_u2[0] = 0x12;
	tx_buff_u2[1] = 0x34;
	tx_buff_u2[2] = 0x56;
	tx_buff_u2[3] = 0x78;
	
	tx_buff_u2[4] = (uint8_t)((type >> 24) & 0xff);
	tx_buff_u2[5] = (uint8_t)((type >> 16) & 0xff);
	tx_buff_u2[6] = (uint8_t)((type >> 8) & 0xff);
	tx_buff_u2[7] = (uint8_t)((type >> 0) & 0xff);
	
	if(type == packet_type_position)
	{
		tx_buff_u2[8] = 0x00;
		tx_buff_u2[9] = 0x00;
		tx_buff_u2[10] = 0x00;
		tx_buff_u2[11] = action_flag;
		
		buff_pointer = position_buff;
	}
	if(type == packet_type_speed_limit)
		buff_pointer = speed_limit_buff;
	if(type == packet_type_torque_limit)
		buff_pointer = torque_limit_buff;
	if(type == packet_type_led)
		buff_pointer = led_buff;
	if(type == packet_type_torque_enable)
		buff_pointer = torque_enable_buff;
	if(type == packet_type_p_gain)
		buff_pointer = p_gain_buff;
	if(type == packet_type_i_gain)
		buff_pointer = i_gain_buff;
	if(type == packet_type_d_gain)
		buff_pointer = d_gain_buff;
	
	for(i = 0, j = 12; i < 20; i++)
	{
		tx_buff_u2[j] = buff_pointer[i] >> 8;
		tx_buff_u2[j+1] = buff_pointer[i] & 0xff;
		j = j+2;
	}
	
	check_sum_reverse(tx_buff_u2, 0, 58-4-1, checksum);
	
	tx_buff_u2[54] = checksum[0];
	tx_buff_u2[55] = checksum[1];
	tx_buff_u2[56] = checksum[2];
	tx_buff_u2[57] = checksum[3];
	
	buff_u2_len = 58;
}

void Start_u2_tran(void)
{
	USART2->DR = tx_buff_u2[0];
	//USART6->DR = tx_buff_u2[0];
}

void Load_tx_buff_u1(void)
{
	//uint8_t i = 0, j = 0;
	//uint8_t checksum[4];
	
	tx_buff_u1[0] = 0xee;
	tx_buff_u1[1] = 0xee;
	tx_buff_u1[2] = 0x10 | 0x40;
	
	//i = 3;
	tx_buff_u1[3] = gyroraw_adis[0];
	tx_buff_u1[4] = gyroraw_adis[1];
	tx_buff_u1[5] = gyroraw_adis[2];
	tx_buff_u1[6] = gyroraw_adis[3];
	tx_buff_u1[7] = gyroraw_adis[4];
	tx_buff_u1[8] = gyroraw_adis[5];
	
	tx_buff_u1[9] = accraw_adis[0];
	tx_buff_u1[10] = accraw_adis[1];
	tx_buff_u1[11] = accraw_adis[2];
	tx_buff_u1[12] = accraw_adis[3];
	tx_buff_u1[13] = accraw_adis[4];
	tx_buff_u1[14] = accraw_adis[5];
	
	tx_buff_u1[15] = Check_Sum(tx_buff_u1, 3, 14);
	

	//_buff_u1[16] = magnraw_adis[0];
	//_buff_u1[17] = magnraw_adis[1];
	//_buff_u1[18] = magnraw_adis[2];
	//_buff_u1[19] = magnraw_adis[3];
	//_buff_u1[20] = magnraw_adis[4];
	//_buff_u1[21] = magnraw_adis[5];
	
	tx_buff_u1[16] = magnraw_LSM[0];
	tx_buff_u1[17] = magnraw_LSM[1];
	tx_buff_u1[18] = magnraw_LSM[2];
	tx_buff_u1[19] = magnraw_LSM[3];
	tx_buff_u1[20] = magnraw_LSM[4];
	tx_buff_u1[21] = magnraw_LSM[5];
	
	tx_buff_u1[22] = 0;
	tx_buff_u1[23] = 25;
	tx_buff_u1[24] = Check_Sum(tx_buff_u1, 16, 23);
	
	tx_buff_u1[25] = 'A';
	tx_buff_u1[26] = 'O';
	tx_buff_u1[27] = 0x00;
	
	buff_u1_len = 28;
}

void Start_u1_tran(void)
{
	USART1->DR = tx_buff_u1[0];
	//USART6->DR = tx_buff_u1[0];
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

void uint32_to_byte4(uint32_t data, uint8_t * byte)
{
	byte[0] = (uint8_t)((data >> 24) & 0xff);
	byte[1] = (uint8_t)((data >> 16) & 0xff);
	byte[2] = (uint8_t)((data >> 8) & 0xff);
	byte[3] = (uint8_t)((data >> 0) & 0xff);
	
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

signed short accxyz[3], gyroxyz[3], magnxyz[3];
float accf[3], gyrof[3], magnf[3];
unsigned char accraw_adis[6];
unsigned char gyroraw_adis[6];
unsigned char magnraw_adis[6];

signed short comtoform(signed short data)
{
	signed short data1, newdata, flagbit;
	

	flagbit = (data & 0x2000) >> 13;
	
	if(flagbit == 0x0000)
		newdata = data;
	else
	{
		data1 = (signed short)((~(((unsigned short)data)-1)) & 0x3fff);
		newdata = (-1) * data1;
	}
	return newdata;
}





void AdisReadData(void)
{
	const unsigned char SPI_Trans_buf[22]={//·¢ËÍÊý¾Ýbuffer
	0x04,0x00,0x06,0x00,0x08,0x00,0x0A,0x00,0x0C,0x00,0x0E, 0x00, 0x10, 0x00, 0x12, 0x00, 0x14, 0x00,0x3c,0x00,0x00,0x00
		};
	unsigned char SPI_Rec_buf[22];

	int i;

	for(i=0;i<=21;i=i+1)
	{
		SPI2_CS0;
		//Delayus(10);
		Nop(500);
		SPI_Rec_buf[i]=SPI2_SeRe(SPI_Trans_buf[i]);
		i = i+1;
		//delay_us(1);
		SPI_Rec_buf[i]=SPI2_SeRe(SPI_Trans_buf[i]);
		
		//delay_us(1);
		SPI2_CS1;
		//Delayus(10);
		Nop(500);
	}
	
	gyroxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[2]) << 8) | ((unsigned short)SPI_Rec_buf[3]);
	gyroxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[4]) << 8) | ((unsigned short)SPI_Rec_buf[5]);
	gyroxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[6]) << 8) | ((unsigned short)SPI_Rec_buf[7]);
	
	accxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[8]) << 8) | ((unsigned short)SPI_Rec_buf[9]);
	accxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[10]) << 8) | ((unsigned short)SPI_Rec_buf[11]);
	accxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[12]) << 8) | ((unsigned short)SPI_Rec_buf[13]);
	
	magnxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[14]) << 8) | ((unsigned short)SPI_Rec_buf[15]);
	magnxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[16]) << 8) | ((unsigned short)SPI_Rec_buf[17]);
	magnxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[18]) << 8) | ((unsigned short)SPI_Rec_buf[19]);
	
	accxyz[0] = accxyz[0] & 0x3fff;
	accxyz[1] = accxyz[1] & 0x3fff;
	accxyz[2] = accxyz[2] & 0x3fff;
	
	gyroxyz[0] = gyroxyz[0] & 0x3fff;
	gyroxyz[1] = gyroxyz[1] & 0x3fff;
	gyroxyz[2] = gyroxyz[2] & 0x3fff;
	
	magnxyz[0] = magnxyz[0] & 0x3fff;
	magnxyz[1] = magnxyz[1] & 0x3fff;
	magnxyz[2] = magnxyz[2] & 0x3fff;
	
	accxyz[0] = comtoform(accxyz[0]);
	accxyz[1] = comtoform(accxyz[1]);
	accxyz[2] = comtoform(accxyz[2]);
	
	gyroxyz[0] = comtoform(gyroxyz[0]);
	gyroxyz[1] = comtoform(gyroxyz[1]);
	gyroxyz[2] = comtoform(gyroxyz[2]);
	
	magnxyz[0] = comtoform(magnxyz[0]);
	magnxyz[1] = comtoform(magnxyz[1]);
	magnxyz[2] = comtoform(magnxyz[2]);
	
	accraw_adis[0] = (accxyz[0] & 0xFF00)>>8;
	accraw_adis[1] = accxyz[0] & 0x0FF;
	accraw_adis[2] = (accxyz[1] & 0xFF00)>>8;
	accraw_adis[3] = accxyz[1] & 0x0FF;
	accraw_adis[4] = (accxyz[2] & 0xFF00)>>8;
	accraw_adis[5] = accxyz[2] & 0x0FF;
	
	gyroraw_adis[0] = (gyroxyz[0] & 0xFF00)>>8;
	gyroraw_adis[1] = gyroxyz[0] & 0x0FF;
	gyroraw_adis[2] = (gyroxyz[1] & 0xFF00)>>8;
	gyroraw_adis[3] = gyroxyz[1] & 0x0FF;
	gyroraw_adis[4] = (gyroxyz[2] & 0xFF00)>>8;
	gyroraw_adis[5] = gyroxyz[2] & 0x0FF;
	
	magnraw_adis[0] = (magnxyz[0] & 0xFF00)>>8;
	magnraw_adis[1] = (magnxyz[0] & 0xFF00)>>8;
	magnraw_adis[2] = (magnxyz[1] & 0xFF00)>>8;
	magnraw_adis[3] = (magnxyz[1] & 0xFF00)>>8;
	magnraw_adis[4] = (magnxyz[2] & 0xFF00)>>8;
	magnraw_adis[5] = (magnxyz[2] & 0xFF00)>>8;
	
	accf[0] = (float)(accxyz[0]) / (float)1000 * (float)2.522;
	accf[1] = (float)(accxyz[1]) / (float)1000 * (float)2.522;
	accf[2] = (float)(accxyz[2]) / (float)1000 * (float)2.522;
	
	gyrof[0] = (float)(gyroxyz[0]) * (float)0.07326;
	gyrof[1] = (float)(gyroxyz[1]) * (float)0.07326;
	gyrof[2] = (float)(gyroxyz[2]) * (float)0.07326;
	
	//printf("accxyz = %5.3f; accxyz = %5.3f; accxyz = %5.3f; \n", accf[0], accf[1], accf[2]);
	//printf("gyrox = %5.3f; gyroy = %5.3f; gyroz = %5.3f; \n", gyrof[0], gyrof[1], gyrof[2]);
	
}


