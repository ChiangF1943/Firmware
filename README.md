# **Firmware 2018**

**The code of lower-board for ZJUDancer in 2017, redesigned by Jiang Chaofeng and Li Xinghan.**

**Due to the use of NVIDIA JETSON TX1/TX2, Motor module and BatMeasure module are removed, and their function are respectively replaced by upper-computer and Bottom Board.**

---

+ **ZJUDancer_code_core**: code for Core module is rewritten to be a communication converter for IMU. UART1, SPI2, TIM3 are used.

+ **ZJUDancer_code_foot**: Based on code_foot in 2016,we added DMA to speed up pressure analog signal reading. Finally the Foot module came into use in RoboCup World Competition.

