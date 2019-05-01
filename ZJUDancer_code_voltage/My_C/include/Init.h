#define LED1_ON GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED1_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LED2_ON GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LED2_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_1)



extern unsigned int cpu_count;
extern unsigned char  cpu_count_flag;
extern float cpu_usage;


void RCC_Config(void);
void Led_Init(void);

void Led1_Toggle(void);
void Led2_Toggle(void);
void Led3_Toggle(void);

void SYSCLK_Init(uint16_t PLL_N_set, uint16_t PLL_P_set);
uint16_t AD7171_Read(void);
void Buzzer_Frequence_Set(uint8_t frequence);

void Buzzer2_Disable(void);
void Buzzer2_Enable(void);

void max_min(float * data, uint16_t num);
void AD7171_Reset_Init(void);

void Load_tx_buff(void);
void check_sum_reverse(uint8_t * data, uint8_t num_start, uint8_t num_end, uint8_t * res_buff);

