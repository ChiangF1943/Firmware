#串口
1. USART1通过usart-USB连接到上位机；本身引出到座上，但没有使用，波特率115200
2. USART2连接到motor_module，波特率1000000
3. USART3连接到voltage_module，波特率256000
4. USART4连接到front_module，波特率256000
5. USART5引出到座上，接电子罗盘
6. USART6连接到接插件P3，波特率256000
#SPI
1. SPI1连接到ADIS陀螺仪
2. SPI2引出到座上，但没有使用
#IIC
ADC连接到AT24C01的IIC接口上
#定时器中断
1. TIM3设置SPI2读陀螺仪值的周期，5ms
1. TIM4设置串口2收发舵机值的周期，2ms
2. TIM5设置串口4收发前胸板值的周期，500ms
3. TIM6设置串口1与上位机通信的周期，10ms
4. TIM7设置开机时间
5. TIM12
#中断
1. USART1串口中断
2. USART2串口中断
3. USART3串口中断
4. USART4串口中断
5. USART6串口中断


