

extern uint32_t voltage_uint32;
extern uint8_t tx_buff_u4[28];
extern uint8_t buff_u4_len;

extern uint8_t tx_buff_u2[58];
extern uint8_t buff_u2_len;

extern uint8_t tx_buff_u1[58];
extern uint8_t buff_u1_len;

extern uint16_t position_buff[30];
extern uint16_t speed_limit_buff[30];
extern uint16_t torque_limit_buff[30];
extern uint16_t led_buff[30];
extern uint16_t torque_enable_buff[30];
extern uint16_t p_gain_buff[30];
extern uint16_t i_gain_buff[30];
extern uint16_t d_gain_buff[30];

extern uint8_t tim7_flag;
extern uint8_t action_flag;

#define packet_type_position 0x01
#define packet_type_speed_limit 0x02
#define packet_type_torque_limit 0x03
#define packet_type_led 0x04
#define packet_type_torque_enable 0x05
#define packet_type_p_gain 0x06
#define packet_type_i_gain 0x07
#define packet_type_d_gain 0x08

extern uint16_t uart1_tx_rate;
extern uint16_t uart1_tx_count;
extern uint16_t uart1_rx_rate;
extern uint16_t uart1_rx_count;

extern uint8_t uart2_tx_buff[400];
extern uint8_t uart3_tx_buff[400];
extern uint8_t uart4_tx_buff[400];
//extern uint8_t uart6_tx_buff[400];
extern uint8_t buff_u2_len;
extern uint8_t buff_u3_len;
extern uint8_t buff_u4_len;
//extern uint8_t buff_u6_len;
extern uint16_t buff_u2_load_pointer;
extern uint16_t buff_u3_load_pointer;
extern uint16_t buff_u4_load_pointer;
//extern uint16_t buff_u6_load_pointer;


