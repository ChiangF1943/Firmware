#define LED1_ON GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define LED1_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define SPI1_CS1_0 GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define SPI1_CS1_1 GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define SPI1_CS2_0 GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define SPI1_CS2_1 GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define SPI1_CS3_0 GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define SPI1_CS3_1 GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define SPI1_CS4_0 GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SPI1_CS4_1 GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define SPI1_CS5_0 GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define SPI1_CS5_1 GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define SPI1_CS6_0 GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define SPI1_CS6_1 GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define SPI1_CS7_0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define SPI1_CS7_1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define SPI1_CS8_0 GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SPI1_CS8_1 GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define SPI1_CS9_0 GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define SPI1_CS9_1 GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define SPI1_CS10_0 GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define SPI1_CS10_1 GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define SPI1_CS11_0 GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define SPI1_CS11_1 GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define SPI1_CS12_0 GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define SPI1_CS12_1 GPIO_SetBits(GPIOC,GPIO_Pin_13)

void LED_Init(void);
void TIM2_Init(uint16_t prescaler, uint16_t period);
void TIM3_Init(uint16_t prescaler, uint16_t period);

void Usart1_Init(unsigned int BaudRate);
void Usart1_Tran(char ch);
void Delayus(unsigned int us);
void Delayms(unsigned int ms);

extern signed short magtempxyz[12][3];
extern signed short magxyz_averge[3];
extern int16_t magn_move_average[3];
extern uint8_t systick_over_flag;
extern uint8_t who[12];
void SpiPortInit(void);

void Spi1Cs(unsigned char num, unsigned char state);
void LsmWriteRegister(unsigned char num, unsigned char address, unsigned char data);
unsigned char LsmReadRegister(unsigned char num, unsigned char address);
void LsmReadRegisterMultiple(unsigned char num, unsigned char address, unsigned char* buff, unsigned char size);
void LsmInit(void);
void LsmMagneticReadSingle(unsigned char num, signed short * magxyz);
void LsmMagneticReadAverage(signed short * magxyz);
unsigned short Nop(unsigned short steps);
void LsmWhoRead(void);
void Load_tx_buff(void);
void Usart1_Tran_Str(unsigned char * str, unsigned short count);
extern uint8_t tx_buff[28];



