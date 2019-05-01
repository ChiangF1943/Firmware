#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "sys_config.h"

void Peripherals_Init(void);
void Led1_Toggle(void);
void Led2_Toggle(void);

#define LED1_ON GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED1_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LED2_ON GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LED2_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_1)


#endif
