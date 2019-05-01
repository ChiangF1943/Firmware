#ifndef __USART_H
#define	__USART_H


#include "stm32f4xx.h"
#include "stdio.h"
#include "sys_config.h"
#define DEBUG_USART USART1
void Send_Package(void);
void SendData(uint8_t *data,uint8_t len,USART_TypeDef *USARTx);
void Sync_Send_Read(USART_TypeDef *USARTx, uint8_t StartID, uint8_t Num, uint8_t Type);
void Sync_Send_Write(USART_TypeDef *USARTx, uint8_t *Data, uint8_t Num);
void SendOrder(uint32_t *databuffer,uint8_t len);
void Sync_Send_Init(USART_TypeDef *USARTx, uint8_t StartID,uint8_t Num);
#endif /* __USART_H */
