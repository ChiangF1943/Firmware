void Usart1_Init(unsigned int BaudRate);
void Usart1_Interp_Config(void);
void Usart2_Init(unsigned int BaudRate);
void Usart3_Init(unsigned int BaudRate);
void Usart3_Interp_Config(void);

void Usart5_Init(unsigned int BaudRate);
void Usart5_Interp_Config(void);

void Usart6_Init(unsigned int BaudRate);
void Usart6_Interp_Config(void);

void Usart1_Tran(char ch);
void Usart2_Tran(char ch);
void Usart3_Tran(char ch);
void Usart6_Tran(char ch);

unsigned char Usart1_Rece(void);
unsigned char Usart2_Rece(void);
unsigned char Usart3_Rece(void);
unsigned char Usart6_Rece(void);

void Usart3_Tran_Str(unsigned char * str, unsigned short count);

int printf_ur6(const char *fmt, ...);



