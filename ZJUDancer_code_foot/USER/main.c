#include "stm32f4xx.h"
#include "sys_config.h"

USART_DataTypeDef U1_DataTypeStructure;
uint16_t ADC_Value_Buffer[4];
uint8_t ID;
uint8_t GETID;
int main(void)
{
    sys_init();
    delay_ms(100);
    B485_1_R;
    while (1)
    {
        if (U1_DataTypeStructure.RevCount == 0x01)
        {
            while (ID != GETID) {}
            Send_Package();
            ID                            = 0;
            GETID                         = 0;
            U1_DataTypeStructure.RevCount = 0;
        }
    }
}
//流程
// 1. ROS发送正常的Sync_write的21个舵机位置指令下来
// 2. STM32接收到之后放到暂存区,并用标志位控制在主程序中发送3部分的写位置信息指令
// 3. STM32接着发送读位置指令,开启中断等待全部接收完毕,通过RevCount==当前串口控制舵机数量判断,判断OK则在主程序中将接收到
//   的数据转换到传输缓存区 接着发送读速度指令
// 4. 主程序中循环判断RevCount==当前串口控制舵机数量*2,判断OK则传输完毕,将数据再搬到传输缓存区,这时读取IMU数据,同样放到传输缓存区
// 5. 最后将传输缓存区的数据当作uint8_t数组发出 ROS部分按读取后再转换回Int类型和IMU数据

// TODOList:
// 1. 修改串口引脚到正式测试板上
// 2. 移植IMU读取函数
// 3. 将关键参数改成宏形式方便修改

/*********************************************END OF FILE**********************/
