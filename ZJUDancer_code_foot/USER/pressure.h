#ifndef PRESSURE_H
#define PRESSURE_H

#include <stm32f4xx.h>
#include "stm32f4xx_conf.h"
#include "main.h"
#include <stdio.h>


#define ADC1_DR_ADDRESS          ((uint32_t)0x4001204C)
#define Max_buf 2
//at least 2

int Pressure_Read(void);
static void ADC_Config(void);

#endif