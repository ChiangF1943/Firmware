#define LED1_ON GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LED1_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define LED2_ON GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define LED2_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_11)

#define LED3_ON GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define LED3_OFF GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define LED4_ON GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define LED4_OFF GPIO_ResetBits(GPIOC,GPIO_Pin_9)

#define FAN1_ON GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define FAN1_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define FAN2_ON GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define FAN2_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define LCD_LED_ON GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LCD_LED_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_2)

extern unsigned int cpu_count;
extern unsigned char  cpu_count_flag;
extern float cpu_usage;


void RCC_Config(void);
void Led_Init(void);

void Led1_Toggle(void);
void Led2_Toggle(void);


void Fan_Init(void);
void LCD_LED_Init(void);

void check_sum_reverse(uint8_t * data, uint8_t num_start, uint8_t num_end, uint8_t * res_buff);
uint32_t check_sum_reverse_to_uint32(uint8_t * data, uint8_t num_start, uint8_t num_end);
uint32_t byte4_to_uint32(uint8_t * byte, uint8_t start_num);

