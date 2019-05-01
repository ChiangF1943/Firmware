# **Firmware 2019**

**The code of lower-board for ZJUDancer in 2019, designed by Jiang Zhang Yuchen and Jiang Chaofeng.**

**Lower-board is removed last year and TX1 controls motors directly through USB-UART-RS485. 
However, the test results show that the delay time in IMU data and motor data reading directly by upper-computer is massive, and it`s unacceptable. As a consequence, the concept of lower-board is used again. 
Core module is replaced by Motor module to reduce workload.**

---

+ **ZJUDancer_code_motor**: Reads IMU data through SPI1 from IMU, sends data to upper-board through UART1, sends to and reads from Dynamixel motors through USART2/3/4 concurrently.

+ **ZJUDancer_code_foot**: Foot pressure sensor board, reads 4-way analog signals of pressure gauge through ADC1/2/3/4(DMA used). Pressure data is sent through UART1, and transferred into RS-485 signals to upper-computer.

