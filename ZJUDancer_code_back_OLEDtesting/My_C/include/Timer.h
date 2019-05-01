#include "stdint.h"

extern uint16_t tim_int_counter[13];

void Delayus(unsigned int us);
void Delayms(unsigned int ms);
unsigned short Nop(unsigned short steps);

void TIM2_Init(unsigned short period_us);
void TIM3_Init(unsigned short period_ms);
void TIM4_Init(unsigned short period_ms);
void TIM5_Init(unsigned short period_ms);
void TIM6_Init(unsigned short period_ms);
void TIM7_Init(unsigned short period_ms);

void TIM8_Init(unsigned short period_ms);
void TIM12_Init(unsigned short period_ms);

void TIM4_PWM_Init(u32 arr,u32 psc);
void TIM4_IT_Init(void);
void TIM5_PWM_Init(u32 arr,u32 psc);
void TIM5_IT_Init(void);

