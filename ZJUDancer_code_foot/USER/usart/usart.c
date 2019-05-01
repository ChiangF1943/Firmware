/**
 ******************************************************************************
 * @file    usart.c
 * @version V1.0
 * @date    2018-02-01
 * @brief   ????
 ******************************************************************************
 */
#include "./usart/usart.h"
uint8_t Feedback[256]; //反馈指令缓冲



void SendData(uint8_t *data, uint8_t len, USART_TypeDef *USARTx)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        USART_SendData(USARTx, data[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
    }
}

void SendOrder(uint32_t *databuffer, uint8_t len)
{
    uint8_t *data;
    data = (uint8_t *)databuffer;
    uint8_t i;
    uint16_t sum     = 0;
    uint8_t Checksum = 0;
    Feedback[0]      = 0x12;
    Feedback[1]      = 0x34;
    Feedback[2]      = len + 6;
    sum              = Feedback[2];
    memcpy(&Feedback[3], data, len);
    for (i = 0; i < len; i++) { sum = sum + data[i]; }
    Checksum          = (uint8_t)(sum & 0xFF);
    Feedback[3 + len] = Checksum;
    Feedback[4 + len] = 0x1F;
    Feedback[5 + len] = 0x2F;
    SendData(Feedback, len + 6, USART1);
}

void Send_Package(void)
{
    int i;
    uint16_t CRC_Data;
    Feedback[0]  = 0xFF;
    Feedback[1]  = 0xFF;
    Feedback[2]  = 0xFD;
    Feedback[3]  = 0x00;
    Feedback[4]  = 0xF1;
    Feedback[5]  = 0x0C;
    Feedback[6]  = 0x00;
    Feedback[7]  = 0x56;
    Feedback[8]  = 0x00;
    Feedback[9]  = (uint8_t)(ADC_Value_Buffer[0] >> 8) & 0xFF; //Higher-order byte
    Feedback[10] = (uint8_t)(ADC_Value_Buffer[0]) & 0xFF; //Lower-order byte
    Feedback[11] = (uint8_t)(ADC_Value_Buffer[1] >> 8) & 0xFF;
    Feedback[12] = (uint8_t)(ADC_Value_Buffer[1]) & 0xFF;
    Feedback[13] = (uint8_t)(ADC_Value_Buffer[2] >> 8) & 0xFF;
    Feedback[14] = (uint8_t)(ADC_Value_Buffer[2]) & 0xFF;
    Feedback[15] = (uint8_t)(ADC_Value_Buffer[3] >> 8) & 0xFF;
    Feedback[16] = (uint8_t)(ADC_Value_Buffer[3]) & 0xFF;
    CRC_Data     = CRC_Acc(0, Feedback, 17);
    Feedback[17] = CRC_Data & 0x00FF;
    Feedback[18] = (CRC_Data >> 8) & 0x00FF;
    B485_1_S;
    delay_us(10);
    for (i = 0; i < 19; i++)
    {
        USART_SendData(USART1, (uint8_t)Feedback[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {};
    }
    delay_us(10);
    B485_1_R;
}



int fputc(int ch, FILE *f)
{
    USART_SendData(DEBUG_USART, (uint8_t)ch);
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET) {};
    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET) {};
    return (int)USART_ReceiveData(DEBUG_USART);
}

/*********************************************END OF FILE**********************/
