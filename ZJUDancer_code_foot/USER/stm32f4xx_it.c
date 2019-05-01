/**
 ******************************************************************************
 * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.0.1
 * @date    13-April-2012
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "sys_config.h"
#include <stm32f4xx.h>

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void){}

#define OrderID 4
#define Instruction 7
#define SendParamStart 8
#define RevParamStart 9
uint32_t *DataArray;
uint32_t DataSendBuffer[128];
void USART_GetData(uint8_t Data, USART_DataTypeDef *USART_DataTypeStructure)
{
    if (USART_DataTypeStructure->Pointer < 4)
    {
        USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
        return;
    }
    if (USART_DataTypeStructure->Pointer == 4)
    {
        if ((USART_DataTypeStructure->Rx_Buff[0] == 0xFF) && (USART_DataTypeStructure->Rx_Buff[1] == 0xFF) && \
					  (USART_DataTypeStructure->Rx_Buff[2] == 0xFD) && (USART_DataTypeStructure->Rx_Buff[3] == 0x00))
        {
            USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
            USART_DataTypeStructure->Len                                         = Data;
        }
        else
        {
            USART_DataTypeStructure->Rx_Buff[0] = USART_DataTypeStructure->Rx_Buff[1];
            USART_DataTypeStructure->Rx_Buff[1] = USART_DataTypeStructure->Rx_Buff[2];
            USART_DataTypeStructure->Rx_Buff[2] = USART_DataTypeStructure->Rx_Buff[3];
            USART_DataTypeStructure->Rx_Buff[3] = Data;
        }
        return;
    }
    if (USART_DataTypeStructure->Pointer <= 6)
    {
        USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
        if (USART_DataTypeStructure->Pointer == 7)
        {
            USART_DataTypeStructure->Len = (USART_DataTypeStructure->Rx_Buff[6] << 8 | USART_DataTypeStructure->Rx_Buff[5]) + 7;
        }
        return;
    }
    if (USART_DataTypeStructure->Pointer > 6)
    {
        if (USART_DataTypeStructure->Pointer < USART_DataTypeStructure->Len)
        {
            USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
            if (USART_DataTypeStructure->Pointer == USART_DataTypeStructure->Len)
            {
                USART_DataTypeStructure->CRC_Value = CRC_Acc(0, USART_DataTypeStructure->Rx_Buff, USART_DataTypeStructure->Len - 2);

                if (USART_DataTypeStructure->CRC_Value == (USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Len - 1] << 8 | USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Len - 2]))
                {
                    if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x82)
                    {
                        ID=USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Len-3];//当前Read指令中的最后一位的ID
												USART_DataTypeStructure->RevCount=0x01;
										}
                    if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x55)
                    {
                        GETID=USART_DataTypeStructure->Rx_Buff[OrderID];//GET 当前包的ID 如果是和之前收到的Read的ID一样的话就开始发送足底数据
                    }
                }
                USART_DataTypeStructure->Pointer = 0;
                memset(USART_DataTypeStructure->Rx_Buff, 0, 256 * sizeof(uint8_t));
            }
            else
            {
                return;
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    uint8_t temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        USART_GetData(temp, &U1_DataTypeStructure);
    }
}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
