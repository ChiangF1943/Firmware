#include <stm32f4xx.h>

#include <function_conf.h>

	#define BIAS   0x52            //0b1000 0101 0010  1/3duty 4com
	#define SYSDIS 0X00           //0b1000 0000 0000  ����ϵͳ������LCDƫѹ������
	#define SYSEN  0X02           //0b1000 0000 0010 ��ϵͳ ����
	#define LCDOFF 0X04           //0b1000 0000 0100  ��LCDƫѹ
	#define LCDON  0X06           //0b1000 0000 0110  ��LCDƫ ѹ
	#define XTAL   0x28           //0b1000 0010 1000 �ⲿ��ʱ��
	#define RC256  0X30           //0b1000 0011 0000  �ڲ�ʱ��RC256K
	#define TONEON  0X12           //0b1000 0001 0010  ������ ���
	#define TONEOFF 0X10           //0b1000 0001 0000 �ر��������
	#define WDTDIS  0X0A        //0b1000 0000 1010  ��ֹ���Ź�
	#define TONE_4K 0x80        //4k�������Ƶ��
	#define TONE_2K 0xC0    
	//HT1621����λ��Һ��ģ��ӿڶ��壬�������ѵ���Ҫ���ģ�
	
	//_Bool PB_ODR_ODR7 @PB_ODR:7;//CS
	//_Bool PB_ODR_ODR5 @PB_ODR:5;//WR
	//_Bool PB_ODR_ODR6 @PB_ODR:6;//DATA
	
	#define CLS_HT_WR     GPIO_ResetBits(GPIOB, GPIO_Pin_13)   //����
	#define SET_HT_WR     GPIO_SetBits(GPIOB, GPIO_Pin_13)   //����
	#define CLS_HT_CS     GPIO_ResetBits(GPIOB, GPIO_Pin_12)   //����
	#define SET_HT_CS     GPIO_SetBits(GPIOB, GPIO_Pin_12)   //���� 
	#define SET_HT_DAT    GPIO_SetBits(GPIOB, GPIO_Pin_15)      //����λ
	#define CLS_HT_DAT    GPIO_ResetBits(GPIOB, GPIO_Pin_15)      //����λ
    //-----------------------------------------------------------------------------------------
    //�������ƣ�Delay()
    //�� �ܣ���ʱ�ӳ���
    //-----------------------------------------------------------------------------------------
    void Delay(uint8_t us) //5,7,9
    {
		Delayus(us);
    }
    //-----------------------------------------------------------------------------------------
    //�������ƣ�DelayMS()
    //�� �ܣ���ʱ�ӳ���
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
    //�������ƣ�Ht1621Wr_Data()
    //�� �ܣ�д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ
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
    //�������ƣ�void Ht1621WrOneData(uchar Addr,uchar Data)
    //��������: HT1621��ָ����ַд�����ݺ���
    //����˵����AddrΪд���ʼ��ַ��DataΪд������
    //˵ ������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ
    //-----------------------------------------------------------------------------------------
    void Ht1621WrOneData(uint8_t Addr,uint8_t Data)
    {
     CLS_HT_CS;  
     DelayMS(1); 
     Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
     Ht1621Wr_Data(Addr<<2,6); //д���ַ����
     Ht1621Wr_Data(Data<<4,4); //д������
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //�������ƣ�void Ht1621WrOneNum(uchar Addr,uchar Num)
    //��������: HT1621��ָ����ַд��һ�����֣�0-9������
    //����˵����AddrΪд���ʼ��ַ��NumΪд������
    //˵ ������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ
    //-----------------------------------------------------------------------------------------
    void Ht1621WrOneNum(uint8_t Addr,uint8_t num)
    {
     CLS_HT_CS;//HT1621_CS=0;
     Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
     Ht1621Wr_Data(Addr<<2,6); //д���ַ����
     Ht1621Wr_Data(num,8);//дһ��������������������д�������� 
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //�������ƣ�void Ht1621WrAllData()
    //��������: HT1621����д�뷽ʽ����
    //˵ ����HT1621������λ4λ���˴�ÿ������Ϊ8λ��д������������8λ����
    //-----------------------------------------------------------------------------------------
    void Ht1621WrAllData(void)
    {
     uint8_t i;
     CLS_HT_CS;//
     Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
     Ht1621Wr_Data(0x00<<2,6); //д���ַ����
     for (i=0;i<16;i++)
     {
      Ht1621Wr_Data(0x00,8);//0x00,8); //д������
     }
     SET_HT_CS;//
    }
    //-----------------------------------------------------------------------------------------
    //�������ƣ�void Ht1621WrCmd(uchar Cmd)
    //��������: HT1621����д�뺯��
    //����˵����CmdΪд����������
    //˵ ����д�������ʶλ100
    //-----------------------------------------------------------------------------------------
    void Ht1621WrCmd(uint8_t Cmd)
    {
     CLS_HT_CS;//HT1621_CS=0;
     DelayMS(1); 
     Ht1621Wr_Data(0x80,4); //д�������־100
     Ht1621Wr_Data(Cmd,8); //д����������
     SET_HT_CS;//HT1621_CS=1;
    }
    //-----------------------------------------------------------------------------------------
    //�������ƣ�void Ht1621_Init(void)
    //��������: HT1621��ʼ��
    //˵ ������ʼ����Һ���������ֶξ���ʾ
    //-----------------------------------------------------------------------------------------
    void Ht1621_Init(void)
    {
     SET_HT_CS;//HT1621_CS=1;
     SET_HT_WR;//HT1621_WR=1;
     SET_HT_DAT;//HT1621_DAT=1;
     DelayMS(20); //��ʱʹLCD������ѹ�ȶ�
     Ht1621WrCmd(BIAS);
     Ht1621WrCmd(RC256); //ʹ���ڲ�����
     Ht1621WrCmd(SYSDIS);
     Ht1621WrCmd(WDTDIS);
     Ht1621WrCmd(SYSEN);
     Ht1621WrCmd(LCDON);
     DelayMS(20); //��ʱһ��ʱ��
     Ht1621WrAllData();
    }
	
    void main_test(void)
    {
      uint8_t i=0x1d;
		uint8_t j = 0, res = 0;
      //CLK_ICKR |= 0x01;       //���ڲ�����RC
      //CLK_CKDIVR = 0x1B;  //fcpu=2M fmaster=2/8=0.25M
      //CLK_CKDIVR = 0x1a;  //fcpu=2M fmaster=2/4=0.5M//���Թ���
      //CLK_CKDIVR = 0x18;  //fcpu=2M fmaster=2/1=2M //���Թ���
      port_init();
      Ht1621_Init(); //�ϵ��ʼ��LCD     
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

