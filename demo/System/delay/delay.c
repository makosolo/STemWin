#include "delay.h"
#include "sys.h"

/*******************************************************************************
* 函数名	: Dellayus
* 描述  	: 延时函数（局部使用）,调用一次延时的时间：
*			  72M下是1us，120M下是0.6us
* 参数  	: usec
* 返回值	: 无
*******************************************************************************/
__asm int delay_us(volatile u32 usec)
{
	ALIGN
	PUSH.W {r1} //2时钟周期
	MOV r1,#18  //1时钟周期
	MUL r0,r1   //1时钟周期
	SUB r0,#3   //1时钟周期
loop
	SUBS r0,#1  //1时钟周期
	BNE loop    //如果跳转则为3个周期，不跳则只有1个周期
	POP {r1}    //2时钟周期
	BX lr       //3个时钟周期
	//总共所用周期为(usec*4)-4,此处减4主要用于抵消调用此函数的消耗时钟周期（传参1时钟，BLX跳转3时钟）
}

void delay_ms(volatile u32 msec)
{
	while(msec--)
	{
		delay_us(1000);//测试结果
	}
}

































