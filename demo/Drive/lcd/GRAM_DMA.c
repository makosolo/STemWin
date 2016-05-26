#include <stdio.h>

#include "stm32f10x_dma.h"

static DMA_InitTypeDef DMA_InitStructure;

extern u16 *GDRAM;
	  
void DMA_init(void)
{												    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//使能DMA传输
	
    DMA_DeInit(DMA2_Channel5);//将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&GDRAM[0];//(u32)0X68000000;//SRAM
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0X6C000800;//LCD
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//SRAM->LCD
	DMA_InitStructure.DMA_BufferSize = 38400;//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器	  	
}

//开启一次LCD DMA传输
//x:其实传输地址编号(0~240)
void LCD_DMA_Enable(u16 x)
{	
	u16 len;
	len = 38400;
	
 	DMA_Cmd(DMA2_Channel5,DISABLE);				   //关闭DMA传输
	DMA_ClearFlag(DMA2_FLAG_TC5);				   //清除上次的传输完成标记 
	
	DMA2_Channel5->CNDTR=len;					   //传输的数据长度
	DMA2_Channel5->CPAR=(u32)&GDRAM[0];			   //DMA2,改变存储器地址	
	
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //开启DMA传输
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//等着	
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //清除上次的传输完成标记 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[38400];		   //DMA2,改变存储器地址
	DMA_Cmd(DMA2_Channel5,DISABLE);				   //关闭DMA传输
	DMA2_Channel5->CNDTR=len;					   //传输的数据长度
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //开启DMA传输
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//等着
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //清除上次的传输完成标记 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[76800];		   //DMA2,改变存储器地址
	DMA_Cmd(DMA2_Channel5,DISABLE);			       //关闭DMA传输
	DMA2_Channel5->CNDTR=len;					   //传输的数据长度
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //开启DMA传输
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//等着
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //清除上次的传输完成标记 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[115200];	   //DMA2,改变存储器地址
	DMA_Cmd(DMA2_Channel5,DISABLE);				   //关闭DMA传输
	DMA2_Channel5->CNDTR=len;
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //开启DMA传输
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//等着
}
 
