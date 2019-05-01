#include <stm32f4xx.h>

#include <function_conf.h>

	#define BIAS   0x52            //0b1000 0101 0010  1/3duty 4com
	#define SYSDIS 0X00           //0b1000 0000 0000  关振系统荡器和LCD偏压发生器
	#define SYSEN  0X02           //0b1000 0000 0010 打开系统 振荡器
	#define LCDOFF 0X04           //0b1000 0000 0100  关LCD偏压
	#define LCDON  0X06           //0b1000 0000 0110  打开LCD偏 压
	#define XTAL   0x28           //0b1000 0010 1000 外部接时钟
	#define RC256  0X30           //0b1000 0011 0000  内部时钟RC256K
	#define TONEON  0X12           //0b1000 0001 0010  打开声音 输出
	#define TONEOFF 0X10           //0b1000 0001 0000 关闭声音输出
	#define WDTDIS  0X0A        //0b1000 0000 1010  禁止看门狗
	#define TONE_4K 0x80        //4k声音输出频率
	#define TONE_2K 0xC0    
	//HT1621控制位（液晶模块接口定义，根据自已的需要更改）
	
	//_Bool PB_ODR_ODR7 @PB_ODR:7;//CS
	//_Bool PB_ODR_ODR5 @PB_ODR:5;//WR
	//_Bool PB_ODR_ODR6 @PB_ODR:6;//DATA
	
	#define CLS_HT_WR     GPIO_ResetBits(GPIOB, GPIO_Pin_13)   //拉低
	#define SET_HT_WR     GPIO_SetBits(GPIOB, GPIO_Pin_13)   //拉高
	#define CLS_HT_CS     GPIO_ResetBits(GPIOB, GPIO_Pin_12)   //拉低
	#define SET_HT_CS     GPIO_SetBits(GPIOB, GPIO_Pin_12)   //拉高 
	#define SET_HT_DAT    GPIO_SetBits(GPIOB, GPIO_Pin_15)      //数据位
	#define CLS_HT_DAT    GPIO_ResetBits(GPIOB, GPIO_Pin_15)      //数据位
    //-----------------------------------------------------------------------------------------
    //函数名称：Delay()
    //功 能：延时子程序
    //-----------------------------------------------------------------------------------------
    void Delay(uint8_t us) //5,7,9
    {
		Delayus(us);
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：DelayMS()
    //功 能：延时子程序
    //-----------------------------------------------------------------------------------------
    void DelayMS(uint32_t iMs)
    {
		Delayms(iMs);
    }

    void port_init(void)
    {
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
		GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：Ht1621Wr_Data()
    //功 能：写数据函数,cnt为传送数据位数,数据传送为低位在前
    //-----------------------------------------------------------------------------------------
    void Ht1621Wr_Data(uint8_t Data,uint8_t cnt)
    {
     uint8_t i;
     for (i=0;i < cnt; i++)
     {
       CLS_HT_WR;
       Delay(10); 
      if((Data & 0x80)==0x80)
      {
         SET_HT_DAT;
      }
      else
      {
         CLS_HT_DAT;
      }
      SET_HT_WR;   
      Delay(10); 
      Data<<=1;
     }
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
    //功能描述: HT1621在指定地址写入数据函数
    //参数说明：Addr为写入初始地址，Data为写入数据
    //说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
    //-----------------------------------------------------------------------------------------
    void Ht1621WrOneData(uint8_t Addr,uint8_t Data)
    {
     CLS_HT_CS;  
     DelayMS(1); 
     Ht1621Wr_Data(0xa0,3); //写入数据标志101
     Ht1621Wr_Data(Addr<<2,6); //写入地址数据
     Ht1621Wr_Data(Data<<4,4); //写入数据
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：void Ht1621WrOneNum(uchar Addr,uchar Num)
    //功能描述: HT1621在指定地址写入一个数字（0-9）函数
    //参数说明：Addr为写入初始地址，Num为写入数据
    //说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
    //-----------------------------------------------------------------------------------------
    void Ht1621WrOneNum(uint8_t Addr,uint8_t num)
    {
     CLS_HT_CS;//HT1621_CS=0;
     Ht1621Wr_Data(0xa0,3); //写入数据标志101
     Ht1621Wr_Data(Addr<<2,6); //写入地址数据
     Ht1621Wr_Data(num,8);//写一个完整的数。就是连续写两个数。 
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：void Ht1621WrAllData()
    //功能描述: HT1621连续写入方式函数
    //说 明：HT1621的数据位4位，此处每次数据为8位，写入数据总数按8位计算
    //-----------------------------------------------------------------------------------------
    void Ht1621WrAllData(void)
    {
     uint8_t i;
     CLS_HT_CS;//
     Ht1621Wr_Data(0xa0,3); //写入数据标志101
     Ht1621Wr_Data(0x00<<2,6); //写入地址数据
     for (i=0;i<16;i++)
     {
      Ht1621Wr_Data(0x00,8);//0x00,8); //写入数据
     }
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：void Ht1621WrCmd(uchar Cmd)
    //功能描述: HT1621命令写入函数
    //参数说明：Cmd为写入命令数据
    //说 明：写入命令标识位100
    //-----------------------------------------------------------------------------------------
    void Ht1621WrCmd(uint8_t Cmd)
    {
     CLS_HT_CS;//HT1621_CS=0;
     DelayMS(1); 
     Ht1621Wr_Data(0x80,4); //写入命令标志100
     Ht1621Wr_Data(Cmd,8); //写入命令数据
     SET_HT_CS;//HT1621_CS=1;
    }
    //-----------------------------------------------------------------------------------------
    //函数名称：void Ht1621_Init(void)
    //功能描述: HT1621初始化
    //说 明：初始化后，液晶屏所有字段均显示
    //-----------------------------------------------------------------------------------------
    void Ht1621_Init(void)
    {
     SET_HT_CS;//HT1621_CS=1;
     SET_HT_WR;//HT1621_WR=1;
     SET_HT_DAT;//HT1621_DAT=1;
     DelayMS(20); //延时使LCD工作电压稳定
     Ht1621WrCmd(BIAS);
     Ht1621WrCmd(RC256); //使用内部振荡器
     Ht1621WrCmd(SYSDIS);
     Ht1621WrCmd(WDTDIS);
     Ht1621WrCmd(SYSEN);
     Ht1621WrCmd(LCDON);
     DelayMS(20); //延时一段时间
     Ht1621WrAllData();
    }
	
    void main_test(void)
    {
      uint8_t i=0x1d;
		uint8_t j = 0, res = 0;
      //CLK_ICKR |= 0x01;       //打开内部高速RC
      //CLK_CKDIVR = 0x1B;  //fcpu=2M fmaster=2/8=0.25M
      //CLK_CKDIVR = 0x1a;  //fcpu=2M fmaster=2/4=0.5M//可以工作
      //CLK_CKDIVR = 0x18;  //fcpu=2M fmaster=2/1=2M //可以工作
      port_init();
      Ht1621_Init(); //上电初始化LCD     
      while(1)
     {
     
      if(i%2)
      {
		 res = Decode_lcd(j);
      Ht1621WrOneNum(3, res); //1
    

      }
      else
      {
        Ht1621WrOneNum(1,0);//6
		j += 1;
		  if(j > 9)
		  {
			  j = 0;
			  Delayms(1000);
		  }
      }
      i=i-1;
      if(i==0x01)
      {
       i = 0x21;
      }
      //PE_ODR=PE_ODR^0x20;
      
      DelayMS(200);
      
     }
    }  
	
uint8_t Decode_lcd(uint8_t num)
{
	uint8_t res = 0;
	switch(num)
	{
		case 0: res = 0xbe; break;
		case 1: res = 0x06; break;
		case 2: res = 0x7c; break;
		case 3: res = 0x5e; break;
		case 4: res = 0xc6; break;
		case 5: res = 0xda; break;
		case 6: res = 0xfa; break;
		case 7: res = 0x0e; break;
		case 8: res = 0xfe; break;
		case 9: res = 0xde; break;
		
	}
	return res;
}

void print_lcd(uint32_t number)
{
	char str[6] = {0};
	uint8_t i = 0, ch = 0, seg = 0;
	number = number % 10000000;
	sprintf(str, "%6d", number);
	for(i = 0; i < 6; i++)
	{
		ch = str[i] - '0';
		seg = Decode_lcd(ch);
		if(i == 1)
			seg |= 0x01;
		if(i == 5)
			seg = 0x00;
		Ht1621WrOneNum(i*2-1 +2, seg);
		
	}
}

