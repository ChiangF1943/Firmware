#ifndef _COMMU_H
#define _COMMU_H
#include "stm32f4xx.h" 

#define COMMU_IDLE 1
#define COMMU_STARTTRANS 2
#define COMMU_GETDATA 3
#define COMMU_GETCOMMAND 4


extern uint8_t com_flag;
extern uint8_t com_buf[20];
extern union sendfloat sf;
union sendfloat
{
char c[16];
float f[4];
};

void USART3_Init(u32 bound);
void USART3_IRQHandler(void);
uint8_t commu_decode(void);
void commu_get(uint8_t buf);
void commu_start(void);
void commu_encode(float data0,float data1,float data2,float data3);
void commu_send(char num);
void direction_485_ctrl(uint8_t key);
unsigned char Check_Sum(uint8_t com[20], unsigned short n1, unsigned short n2);

#endif
