#include "sys.h"

#include "bsp_usart.h"	 
#include "delay.h"

#include "bsp_lcd.h"

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR = 0x0000; //画笔颜色
u16 BACK_COLOR  = 0xFFFF; //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

/*******************************************************************************
* 函数名	: LCD_WR_REG
* 描述  	: 写寄存器
* 参数  	: regval:寄存器值
* 返回值	: 无
*******************************************************************************/
void LCD_WR_REG(const u16 regval)
{ 
	LCD->LCD_REG = regval;//写入要写的寄存器序号	 
}
/*******************************************************************************
* 函数名	: LCD_WR_DATA
* 描述  	: 写LCD数据
* 参数  	: data:数据
* 返回值	: 无
*******************************************************************************/
void LCD_WR_DATA(const u16 data)
{										    	   
	LCD->LCD_RAM = data;		 
}

/*******************************************************************************
* 函数名	: LCD_WR_DATA
* 描述  	: 读LCD数据
* 参数  	: 无
* 返回值	: 读到的值
*******************************************************************************/
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   

/*******************************************************************************
* 函数名	: LCD_WriteReg
* 描述  	: 写寄存器
* 参数  	: LCD_Reg:寄存器地址
*             LCD_RegValue:要写入的数据
* 返回值	: 读到的值
*******************************************************************************/
void LCD_WriteReg(const u16 LCD_Reg, const u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   

/*******************************************************************************
* 函数名	: LCD_ReadReg
* 描述  	: 读寄存器
* 参数  	: LCD_Reg:寄存器地址
* 返回值	: 读到的值
*******************************************************************************/
u16 LCD_ReadReg(const u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}

/*******************************************************************************
* 函数名	: LCD_DisplayOn
* 描述  	: LCD开启显示
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29); //开启显示
}

/*******************************************************************************
* 函数名	: LCD_DisplayOff
* 描述  	: LCD关闭显示
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28); //关闭显示
}

/*******************************************************************************
* 函数名	: LCD_SetCursor
* 描述  	: 设置光标位置
* 参数  	: Xpos:横坐标
*             Ypos:纵坐标
* 返回值	: 无
*******************************************************************************/
void LCD_SetCursor(const u16 Xpos, const u16 Ypos)
{	 		    
    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(Xpos>>8); 
    LCD_WR_DATA(Xpos&0XFF);	 
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(Ypos>>8); 
    LCD_WR_DATA(Ypos&0XFF); 
}

/*******************************************************************************
* 函数名	: LCD_Scan_Dir
* 描述  	: 设置LCD的自动扫描方向，一般设置为L2R_U2D（从左到右，从上到下）
* 参数  	: dir:0~7,代表8个方向(具体定义见lcd.h)
* 返回值	: 无
*******************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval = 0;
	u16 dirreg = 0;
	u16 temp;  

    switch(dir)//方向转换
    {
        case 0:dir=6;break;
        case 1:dir=7;break;
        case 2:dir=4;break;
        case 3:dir=5;break;
        case 4:dir=1;break;
        case 5:dir=0;break;
        case 6:dir=3;break;
        case 7:dir=2;break;	     
    }
    
    switch(dir)
    {
        case L2R_U2D://从左到右,从上到下
            regval|=(0<<7)|(0<<6)|(0<<5); 
            break;
        case L2R_D2U://从左到右,从下到上
            regval|=(1<<7)|(0<<6)|(0<<5); 
            break;
        case R2L_U2D://从右到左,从上到下
            regval|=(0<<7)|(1<<6)|(0<<5); 
            break;
        case R2L_D2U://从右到左,从下到上
            regval|=(1<<7)|(1<<6)|(0<<5); 
            break;	 
        case U2D_L2R://从上到下,从左到右
            regval|=(0<<7)|(0<<6)|(1<<5); 
            break;
        case U2D_R2L://从上到下,从右到左
            regval|=(0<<7)|(1<<6)|(1<<5); 
            break;
        case D2U_L2R://从下到上,从左到右
            regval|=(1<<7)|(0<<6)|(1<<5); 
            break;
        case D2U_R2L://从下到上,从右到左
            regval|=(1<<7)|(1<<6)|(1<<5); 
            break;	 
    }
    
    dirreg = 0X36;
    
    regval |= 0X08;//5310/5510不需要BGR
    
    LCD_WriteReg(dirreg, regval);
    
    if((regval & 0X20) || (lcddev.dir == 1))
    {
        if(lcddev.width < lcddev.height)//交换X,Y
        {
            temp = lcddev.width;
            lcddev.width  = lcddev.height;
            lcddev.height = temp;
        }
    }else  
    {
        if(lcddev.width > lcddev.height)//交换X,Y
        {
            temp = lcddev.width;
            lcddev.width  = lcddev.height;
            lcddev.height = temp;
        }
    } 

    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width-1)>>8);
    LCD_WR_DATA((lcddev.width-1)&0XFF);
    
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height-1)>>8);
    LCD_WR_DATA((lcddev.height-1)&0XFF);
}   

/*******************************************************************************
* 函数名	: LCD_Display_Dir
* 描述  	: 设置LCD显示方向
* 参数  	: dir:0,竖屏；1,横屏
* 返回值	: 无
*******************************************************************************/
void LCD_Display_Dir(const u8 dir)
{
	if(dir == 0) //竖屏
	{
		lcddev.dir    = 0;
        lcddev.width  = 320;
        lcddev.height = 480;
	}
    else //横屏
	{	  				
		lcddev.dir    = 1;
        lcddev.width  = 480;
        lcddev.height = 320; 
	}
    
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 

/*******************************************************************************
* 函数名	: LCD_Set_Window
* 描述  	: 设置窗口
* 参数  	: sx,sy:窗口起始坐标(左上角)
*             width,height:窗口宽度和高度,必须大于0!!
* 返回值	: 无
*******************************************************************************/
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	width  = sx + width - 1;
	height = sy + height - 1;

    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(sx>>8); 
    LCD_WR_DATA(sx&0XFF);	 
    LCD_WR_DATA(width>>8); 
    LCD_WR_DATA(width&0XFF);
    
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(sy>>8); 
    LCD_WR_DATA(sy&0XFF); 
    LCD_WR_DATA(height>>8); 
    LCD_WR_DATA(height&0XFF); 
} 

/*******************************************************************************
* 函数名	: LCD_ReadID
* 描述  	: 读取LCD的ID
* 参数  	: 无
* 返回值	: ID
*******************************************************************************/
u16 LCD_ReadID(void)
{
    u16 lcd_ID = 0;
    
    LCD_WR_REG(0XD4);				   
    LCD_RD_DATA();//dummy read  
    LCD_RD_DATA();//读回0X01	
    lcd_ID = LCD_RD_DATA();//读回0X53	
    lcd_ID<<=8;	 
    lcd_ID |= LCD_RD_DATA();//这里读回0X10

    return lcd_ID;
}

/*******************************************************************************
* 函数名	: opt_delay
* 描述  	: 当mdk -O1时间优化时需要设置
* 参数  	: i:延时
* 返回值	: 读到的值
*******************************************************************************/
void opt_delay(volatile u8 i)
{
	while(i--);
}

static void delay_100ms(void)
{
	volatile u32 i=800000;
	
	while(i--);	
}

/*******************************************************************************
* 函数名	: LCD_GPIO_Init
* 描述  	: 液晶驱动初始化
*			  IO的初始化因为和FSMC驱动外部SRAM的引脚有部分重复，为了不出错
*			  （初始化循序混乱就会出错）固部分IO重复初始化
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
static void LCD_GPIO_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

/*******************************************************************************
* 函数名	: LCD_FSMC_Init
* 描述  	: FSMC总线初始化
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
static void LCD_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    
	writeTiming.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x02;		 ////数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置
   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 
}

/*******************************************************************************
* 函数名	: LCD_Init
* 描述  	: 初始化lcd
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
void bsp_InitLCD(void)
{    
    LCD_GPIO_Init();//LCD IO初始化
    LCD_FSMC_Init();//FSMC初始化
    
	delay_100ms(); // delay 50 ms 
  	lcddev.id = LCD_ReadID();
 	printf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID  
    
    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0xFE);

    LCD_WR_REG(0xEE);
    LCD_WR_DATA(0xDE);
    LCD_WR_DATA(0x21);

    LCD_WR_REG(0xF1);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xDF);
    LCD_WR_DATA(0x10);

    //VCOMvoltage//
    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x8F);	  //5f

    LCD_WR_REG(0xC6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xE2);
    LCD_WR_DATA(0xE2);
    LCD_WR_DATA(0xE2);
    LCD_WR_REG(0xBF);
    LCD_WR_DATA(0xAA);

    LCD_WR_REG(0xB0);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x21);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x5D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x5D);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x80);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x8B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x96);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x8B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x96);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA1);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB5);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x5E);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x8C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xAC);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDC);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x70);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x90);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xEB);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDC);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xB8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xBA);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xFF);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x26);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x26);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x62);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x84);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA4);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x50);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x95);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xE6);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x65);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x76);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC6);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x21);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x46);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x52);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x7A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x8B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC4);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xCA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xE0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x45);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x52);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x7A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x8B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC4);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xCA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xE0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE2);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x4F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x61);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x79);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x97);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDD);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xE3);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x50);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x62);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x78);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x97);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC7);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDD);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE4);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x2A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x5D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x74);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x84);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x93);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xBE);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC4);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xCD);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDD);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xE5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x5D);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x74);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x84);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x93);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xB3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xBE);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC4);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xCD);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD3);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xDC);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xF3);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE6);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x56);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x76);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x77);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x66);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xBB);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x66);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x45);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE7);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x76);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x66);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x67);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x67);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x87);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xBB);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x77);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x56);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x23); 
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x45);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x87);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x77);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x66);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x88);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xAA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xBB);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x99);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x66);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE9);
    LCD_WR_DATA(0xAA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x00);
    LCD_WR_DATA(0xAA);

    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x50);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xF3);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xF9);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);	//66

    LCD_WR_REG(0x11);
    delay_100ms();
    LCD_WR_REG(0x29);
    LCD_WR_REG(0x35);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x51);
    LCD_WR_DATA(0xFF);
    LCD_WR_REG(0x53);
    LCD_WR_DATA(0x2C);
    LCD_WR_REG(0x55);
    LCD_WR_DATA(0x82);
    LCD_WR_REG(0x2c);
		 
	LCD_Display_Dir(1); //默认为横屏

	LCD_LED = 1;        //点亮背光
	LCD_Clear(WHITE);
}  

/*******************************************************************************
* 函数名	: LCD_Clear
* 描述  	: 清屏函数
* 参数  	: color:要清屏的填充色
* 返回值	: 颜色
*******************************************************************************/
void LCD_Clear(u16 color)
{
	u32 index = 0;      
	u32 totalpoint = lcddev.width;
    
	totalpoint *= lcddev.height; //得到总点数
    
    LCD_SetCursor(0x00, 0x0000); //设置光标位置 
    LCD_WR_REG(0X2C);            //开始写入GRAM
    
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA(color);
	}
}  

/*******************************************************************************
* 函数名	: LCD_Fill
* 描述  	: 在指定区域内填充单个颜色
* 参数  	: (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1) 
*             color:要填充的颜色
* 返回值	: 颜色
*******************************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen = 0;
    
    xlen = ex-sx+1;
    
    for(i=sy;i<=ey;i++)
    {
        LCD_SetCursor(sx, i); //设置光标位置
        LCD_WR_REG(0X2C);    //开始写入GRAM
        
        for(j=0;j<xlen;j++) 
        {
            LCD_WR_DATA(color);
        }
    }	 
}  

/*******************************************************************************
* 函数名	: LCD_Fill_Color
* 描述  	: 在指定区域内填充指定颜色块
* 参数  	: (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1) 
*             color:要填充的颜色
* 返回值	: 无
*******************************************************************************/
void LCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
    
	width  = ex-sx+1; //得到填充的宽度
	height = ey-sy+1; //高度
    
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i); //设置光标位置 
        LCD_WR_REG(0X2C);       //开始写入GRAM
		for(j=0;j<width;j++) LCD_WR_DATA(color[i*height+j]);//写入数据
	}	  
}

/*******************************************************************************
* 函数名	: LCD_ReadPoint
* 描述  	: 读取个某点的颜色值
* 参数  	: x,y:坐标
* 返回值	: 颜色
*******************************************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
    
//	if((x >= lcddev.width) || (y >= lcddev.height)) return 0;//超过了范围,直接返回
    
	LCD_SetCursor(x, y);//设置光标位置
	LCD_WR_REG(0X2E);//9341/6804/3510 发送读GRAM指令

	if(LCD->LCD_RAM) r = 0; //dummy Read
	opt_delay(2);
 	r = LCD->LCD_RAM; //实际坐标颜色
    
    opt_delay(2);
    b = LCD->LCD_RAM; 
    g = r&0XFF;//第一次读取的是RG的值,R在前,G在后,各占8位
    g <<= 8;

	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//需要公式转换一下
}

/*******************************************************************************
* 函数名	: LCD_DrawPoint
* 描述  	: 画点
* 参数  	: x,y:坐标
* 返回值	: 无
*******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
//	LCD_SetCursor(x, y); //设置光标位置
//    LCD_WR_REG(0X2C);    //开始写入GRAM
//	LCD->LCD_RAM = POINT_COLOR; 
    if((x >= lcddev.width) || (y >= lcddev.height)) return;//超过了范围,直接返回
    
    //设置光标位置
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = x>>8;
    LCD->LCD_RAM = x&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = y>>8;
    LCD->LCD_RAM = y&0XFF;
    
    LCD->LCD_REG = 0X2C;
    LCD->LCD_RAM = POINT_COLOR;
}

/*******************************************************************************
* 函数名	: LCD_DrawPoint_Color
* 描述  	: 画点
* 参数  	: x,y:坐标
* 返回值	: 无
*******************************************************************************/
void LCD_DrawPoint_Color(u16 x,u16 y,u16 color)
{
//    LCD_SetCursor(x, y); //设置光标位置
//	LCD->LCD_REG = 0X2C; 
//	LCD->LCD_RAM = color;
//    if((x >= lcddev.width) || (y >= lcddev.height)) return;//超过了范围,直接返回
    
    //设置光标位置
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = x>>8;
    LCD->LCD_RAM = x&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = y>>8;
    LCD->LCD_RAM = y&0XFF;
    
    LCD->LCD_REG = 0X2C;
    LCD->LCD_RAM = color;
}  
