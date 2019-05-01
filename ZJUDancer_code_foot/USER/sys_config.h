#ifndef __SYS_CONFIG_H_
#define __SYS_CONFIG_H_
#include "stdint.h"
#include "string.h"
#include "./motor/motor.h"
#include "./sys/sys.h"
#include "./usart/usart.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define BaudRate 1000000
#define B485_1_S GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define B485_1_R GPIO_ResetBits(GPIOC,GPIO_Pin_4)

typedef struct
{
  uint16_t Pointer;
  uint8_t Rx_Buff[256]; 
  uint16_t Len;
  uint16_t Count;
  uint16_t CRC_Value;
  uint8_t RevCount;
  uint8_t Temp_Rev[256];
  uint32_t DataArray[48];
} USART_DataTypeDef;

extern USART_DataTypeDef U1_DataTypeStructure;
extern USART_DataTypeDef U2_DataTypeStructure;
extern USART_DataTypeDef U3_DataTypeStructure;
extern USART_DataTypeDef U4_DataTypeStructure;
extern uint16_t ADC_Value_Buffer[4];
extern uint8_t ID;
extern uint8_t GETID;
#endif
