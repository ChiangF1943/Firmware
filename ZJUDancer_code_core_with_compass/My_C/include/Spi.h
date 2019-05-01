
void SPI1_Init(void);
void SPI2_Init(void);
void SPI3_Init(void);

unsigned char SPI1_SeRe(unsigned char data);
unsigned char SPI2_SeRe(unsigned char data);
unsigned char SPI3_SeRe(unsigned char data);

void SPI1_Interp_config(void);
void SPI2_Interp_config(void);
void SPI3_Interp_config(void);

#define SPI1_CS0 GPIO_ResetBits(GPIOA,GPIO_Pin_4);
#define SPI1_CS1 GPIO_SetBits(GPIOA,GPIO_Pin_4);

#define SPI2_CS0 GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define SPI2_CS1 GPIO_SetBits(GPIOB,GPIO_Pin_12);

#define SPI3_CS0 GPIO_ResetBits(GPIOC,GPIO_Pin_13);
#define SPI3_CS1 GPIO_SetBits(GPIOC,GPIO_Pin_13);


