#include "stdint.h"

extern uint16_t tim_int_counter[13];

void Delayus(unsigned int us);
void Delayms(unsigned int ms);
unsigned short Nop(unsigned short steps);

void TIM3_Init(unsigned short period_ms);
void TIM6_Init(unsigned short period_ms);
void TIM7_Init(unsigned short period_ms);
void TIM12_Init(unsigned short period_ms);

void SysTick_Interp_config(void);
