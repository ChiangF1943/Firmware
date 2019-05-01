#define LED1_ON GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED1_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LED2_ON GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LED2_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define B485_1_S GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define B485_1_R GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define B485_2_S GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define B485_2_R GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define B485_3_S GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define B485_3_R GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define B485_4_S GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define B485_4_R GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define UART2_DR_Addr (0x40004400+0x04)
#define UART3_DR_Addr (0x40004800+0x04)
#define UART4_DR_Addr (0x40004C00+0x04)
#define UART6_DR_Addr (0x40011400+0x04)

extern unsigned int cpu_count;
extern unsigned char  cpu_count_flag;
extern float cpu_usage;


void RCC_Config(void);
void Led_Init(void);

void Led1_Toggle(void);
void Led2_Toggle(void);


void max_min(float * data, uint16_t num);
void AD7171_Reset_Init(void);

void Load_tx_buff(void);
void check_sum_reverse(uint8_t * data, uint8_t num_start, uint8_t num_end, uint8_t * res_buff);
uint32_t check_sum_reverse_to_uint32(uint8_t * data, uint8_t num_start, uint8_t num_end);
uint32_t byte4_to_uint32(uint8_t * byte, uint8_t start_num);


extern signed short accxyz[3], gyroxyz[3];
extern float accf[3], gyrof[3];
extern unsigned char accraw_adis[6];
extern unsigned char gyroraw_adis[6];

void AdisReadData(void);
void Bus_485_Con_Init(void);


#define Return_Delay_Time     (0x05)
#define Status_Return_Level   (0X10)

#define Torque_Enable         (0X18)
#define LED                   (0X19)

#define D_Gain                (0X1A)
#define I_Gain                (0X1B)
#define P_Gain                (0X1C)

#define CW_Compliance_Margin  (0X1A)
#define CCW_Compliance_Margin (0X1B)
#define CW_Compliance_Slope   (0X1C)
#define CCW_Compliance_Slope  (0X1D)
#define Goal_Position_L       (0X1E)
#define Goal_Position_H       (0X1F)
#define Moving_Speed_L        (0X20)
#define Moving_Speed_H        (0X21)
#define Torque_Limit_L        (0X22)
#define Torque_Limit_H        (0X23)

#define Present_Position_L    (0X24)
#define Present_Position_H    (0X25)
#define Present_Speed_L       (0X26)
#define Present_Speed_H       (0X27)
#define Present_Load_L        (0X28)
#define Present_Load_H        (0X29)
#define Present_Voltage       (0X2A)
#define Present_Temperature   (0X2B)
#define Registered_Instruction  (0X2C)

#define Low_Limit_Voltage     (0x0c)
#define High_Limit_Voltage     (0x0d)

#define Alarm_Shutdown  0x12

#define Moving                (0X2E)
#define Lock                  (0X2F)
#define Punch_L               (0X30)
#define Punch_H               (0X31)

#define INS_PING        0X01
#define INS_READ_DATA   0X02
#define INS_WRITE_DATA  0X03
#define INS_REG_WRITE   0X04
#define INS_ACTION      0X05
#define INS_RESET       0X06
#define INS_SYNC_WRITE  0X83

#define ID_ALL 0XFE

unsigned char Check_Sum(unsigned char * coms, unsigned short n1, unsigned short n2);

void Motor_Led(unsigned char id, unsigned char led_new_state);
void Motor_Control(uint16_t pos);
void Motor_Buff_Init(void);

extern uint16_t team_1_postion[20];
extern uint16_t team_2_postion[20];
extern uint16_t team_3_postion[20];

extern uint8_t team_1_id[20];
extern uint8_t team_2_id[20];
extern uint8_t team_3_id[20];

extern uint8_t team_1_id_count;
extern uint8_t team_2_id_count;
extern uint8_t team_3_id_count;

void Motor_Led_Control(uint8_t state);
void Motor_Torque_Control(uint8_t state);
void Motor_Action_Control(void);

void Motor_V0_Torque(uint8_t id, uint8_t state);
void Motor_V0_LED(uint8_t id, uint8_t state);
void Motor_V0_Position(uint8_t id, uint16_t position);
void Motor_V0_Action(uint8_t id);
void Motor_V0_Speed_Limit(uint8_t id, uint16_t speed_limit);

void Motor_V0_Torque_Limit(uint8_t id, uint16_t torque_limit);

void Motor_V0_P_Gain(uint8_t id, uint8_t state);
void Motor_V0_I_Gain(uint8_t id, uint8_t state);
void Motor_V0_D_Gain(uint8_t id, uint8_t state);


void Motor_Go_Position_All_One_Bus(void);
void Motor_Torque_Limit_All_One_Bus(void);
void Motor_Speed_Limit_All_One_Bus(void);
void Motor_Led_All_One_Bus(void);
void Motor_Torque_All_One_Bus(void);
void Motor_P_Gain_All_One_Bus(void);
void Motor_I_Gain_All_One_Bus(void);
void Motor_D_Gain_All_One_Bus(void);

uint8_t Motor_V0_LED_Com_Get(uint8_t id, uint8_t state, uint8_t * com_buff);
uint8_t Motor_V0_Position_Com_Get(uint8_t id, uint16_t position, uint8_t * com_buff);
uint8_t Motor_V0_Action_Com_Get(uint8_t id,  uint8_t * com_buff);

void Reset_uart2_tx_buff(void);
void Reset_uart3_tx_buff(void);
void Reset_uart4_tx_buff(void);

void Load_uart2_tx_buff(uint8_t * data, uint16_t num);
void Load_uart3_tx_buff(uint8_t * data, uint16_t num);
void Load_uart4_tx_buff(uint8_t * data, uint16_t num);

void Start_u2_tran(void);
void Start_u3_tran(void);
void Start_u4_tran(void);


void Motor_Led_Load_Uart2(void);
void Motor_Led_Load_Uart3(void);
void Motor_Led_Load_Uart4(void);

void Motor_Position_Load_Uart2(void);
void Motor_Position_Load_Uart3(void);
void Motor_Position_Load_Uart4(void);

void Motor_Action_Load_Uart2(void);
void Motor_Action_Load_Uart3(void);
void Motor_Action_Load_Uart4(void);

void Motor_Torque_Enable_Load_Uart2(void);
void Motor_Torque_Enable_Load_Uart3(void);
void Motor_Torque_Enable_Load_Uart4(void);

void Motor_Speed_Limit_Load_Uart2(void);
void Motor_Speed_Limit_Load_Uart3(void);
void Motor_Speed_Limit_Load_Uart4(void);

void Motor_Torque_Limit_Load_Uart2(void);
void Motor_Torque_Limit_Load_Uart3(void);
void Motor_Torque_Limit_Load_Uart4(void);



void DMA_Uart2_Tx_Config(void);
void DMA_Uart3_Tx_Config(void);
void DMA_Uart4_Tx_Config(void);
void DMA_Uart6_Tx_Config(void);

void DMA_Uart2_Tx_Start(void);
void DMA_Uart3_Tx_Start(void);
void DMA_Uart4_Tx_Start(void);


void Set_Motor_Torque_Enable_All(uint8_t state);
void Set_Motor_Led_All(uint8_t state);
void Set_Motor_Speed_Limit(uint16_t speed_limit);
void Set_Motor_Torque_Limit(uint16_t torque_limit);
void Set_Motor_Position(uint16_t position);



uint8_t Motor_Current_Position_Com_Get(uint8_t id, uint8_t * com_buff);
void Motor_Present_Position_Load_Uart4(void);