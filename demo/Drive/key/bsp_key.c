/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  solo
  * @version V0.0.2
  * @date    2016-03-07
  * @brief   按键驱动
  *
  * @verbatim 对外API
  *           - KEY_Init    //按键初始化
  *           - KEY_Scan    //按键处理
  *   
  * @note    
  *         1、软件滤波，抗干扰
  * @endverbatim  
  *                                  
  */

/* 头文件 ------------------------------------------------------------------*/
#include "sys.h"

#include "bsp_key.h"

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下，0表示未按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown1(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}
static uint8_t IsKeyDown2(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}
static uint8_t IsKeyDown3(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}

static uint8_t IsKeyDown4(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) != 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* 初始化按键变量 */
	bsp_InitKeyHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;
    
    #if FREERTOS_ENABLED 	//如FREERTOS_ENABLED定义了,说明使用FreeRTOS了.
    vTaskDelay(5/portTICK_RATE_MS);//GUI靠按键阻塞的
    #endif
    
	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参:  _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参：_ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime    = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;		/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;					/* 连发计数器 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyHard
*	功能说明: 配置按键对应的GPIO
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入输出设置，输入/输出/复用/模拟
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入输出设置，输入/输出/复用/模拟
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_tKey.Read  = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = 0;			            /* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count    = KEY_FILTER_TIME / 2;	/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State    = 0;						/* 按键缺省状态，0为未按下 */
        s_tBtn[i].KeyCodeDown = 3 * i + 1;			/* 按键按下的键值代码 */
        s_tBtn[i].KeyCodeUp   = 3 * i + 2;			/* 按键弹起的键值代码 */
        s_tBtn[i].KeyCodeLong = 3 * i + 3;			/* 按键被持续按下的键值代码 */
		s_tBtn[i].RepeatSpeed = 0;					/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;					/* 连发计数器 */
        s_tBtn[i].attribute   = 0;                  //按键属性，通用按键
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
    //长按连发配置
	s_tBtn[KID_K1].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K1].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	s_tBtn[KID_K2].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K2].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	s_tBtn[KID_K3].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K3].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	s_tBtn[KID_K4].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K4].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

    //按键属性
//    s_tBtn[KID_K6].attribute  = 2;//shift键
//    s_tBtn[KID_K7].attribute  = 1;//组合键
//    s_tBtn[KID_K8].attribute  = 1;//组合键
//    s_tBtn[KID_K9].attribute  = 1;//组合键
//    s_tBtn[KID_K10].attribute = 1;//组合键
    
	/* 判断按键按下的函数 */
	s_tBtn[KID_K1].IsKeyDownFunc  = IsKeyDown1;
	s_tBtn[KID_K2].IsKeyDownFunc  = IsKeyDown2;
	s_tBtn[KID_K3].IsKeyDownFunc  = IsKeyDown3;
	s_tBtn[KID_K4].IsKeyDownFunc  = IsKeyDown4;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参:  按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
    #if KEY_SHIF //组合键模式
    static u8 flag = 0;//组合按键触发标识
    #endif
    
	KEY_T *pBtn = &s_tBtn[i];

	if (pBtn->IsKeyDownFunc()) //读按键状态，按下
	{
		if (pBtn->Count < KEY_FILTER_TIME) 
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME) //滤波
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0) //保证只触发一次
			{
				pBtn->State = 1;
                
                #if KEY_SHIF //组合键模式
                if(pBtn->attribute == 1) //组合按键，需要消除独立按键的干扰
                {
                    flag = 1;
                    bsp_ClearKey();//发送组合按键前，要清一次键值
                }
                #endif
                
				/* 发送按钮按下的消息 */
				bsp_PutKey(s_tBtn[i].KeyCodeDown);
			}

			if (pBtn->LongTime > 0) //长按处理代码
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime) 
					{
						/* 键值放入按键FIFO */
						bsp_PutKey(s_tBtn[i].KeyCodeLong);//第一个长按键发送
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0) //连续按键周期
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* 常按键后，每隔10ms发送1个按键 */
							bsp_PutKey(s_tBtn[i].KeyCodeDown);//长安键值周期发送
						}
					}
				}
			}
		}
	}
	else //弹起
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
                
				/* 发送按钮弹起的消息 */
				bsp_PutKey(s_tBtn[i].KeyCodeUp);
                
                #if KEY_SHIF //组合键模式
                if((flag == 1) && //触发了组合键
                   ((pBtn->attribute == 2) || ((bsp_GetKeyState(KID_SHIFT) == 0) && (pBtn->attribute == 1)))) //shift键释放或者组合键同时释放清键值
                {
                    flag = 0;
                    bsp_ClearKey();//要清一次键值
                }
                #endif
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
    #if KEY_CLEAR //定时请键
    static uint32_t cont = 0;//计数
    #endif
	uint8_t i;
    
    #if KEY_CLEAR //定时请键
    if(cont++ >= 50) 
    {
        cont = 0;
        bsp_ClearKey();//要清一次键值
    }
    #endif
    
	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}
