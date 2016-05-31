/**
  ******************************************************************************
  * @file    bsp_init.c
  * @author  solo
  * @version V0.0.1
  * @date    2016-02-16
  * @brief   硬件驱动
  * @verbatim 
  *   
  * @note    
  *
  * @endverbatim  
  *                                  
  */

/* 头文件 ------------------------------------------------------------------*/
#include "sys.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "bsp_24cxx.h" 
#include "bsp_SRAM.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"

#include "bsp_init.h"

/*******************************************************************************
* 函数名	: NVIC_Configuration
* 描述  	: 中断配置
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //只有0-15的先占优先级
}

/*******************************************************************************
* 函数名	: HW_Init
* 描述  	: 硬件初始化
* 参数  	: 无
* 返回值	: 无
*******************************************************************************/
void bsp_Init(void)
{
	NVIC_Configuration(); //中断分组
    
    bsp_InitUsart1(9600); //初始化串口
    bsp_InitSRAM();	      //初始化SRAM	
    bsp_InitLED();        //初始化LED
    bsp_InitKey();	      //初始化按键
	bsp_InitLCD();	      //初始化LCD
    TP_Init();            //初始化触摸
//	DMA_init();		      //初始化DMA   

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//使用STemWin必须先使能CRC
}
