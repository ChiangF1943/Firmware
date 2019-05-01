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


void max_min(float * data, uint16_t num);


void Load_tx_buff_u2(void);
void Start_u2_tran(void);


void check_sum_reverse(uint8_t * data, uint8_t num_start, uint8_t num_end, uint8_t * res_buff);
unsigned char Check_Sum(unsigned char * coms, unsigned short n1, unsigned short n2);

extern signed short accxyz[3], gyroxyz[3], magnxyz[3];
extern float accf[3], gyrof[3],  magnf[3];
extern unsigned char accraw_adis[6];
extern unsigned char gyroraw_adis[6];
extern unsigned char magnraw_adis[6];

void AdisReadData(void);

extern uint8_t adis_pointer;
